#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SFML/Graphics.hpp>
#include "model.hpp"
#include "../globals.hpp"
using namespace std;


/**
 * Multiplies things backwards. Why do we need this? My guess is as good as yours.
 */
static glm::mat4 mul(const glm::mat4 & m1, const glm::mat4 & m2){
	return m2*m1; //lol why does this work
}

/**
 * Makes a bone transformation matrix
 */
glm::mat4 makeBoneMat(glm::quat rot, glm::vec3 trans, glm::vec3 scale){
	glm::mat3 thingy = glm::inverse(glm::mat3_cast(glm::normalize(rot)));
	thingy[0] *= scale;
	thingy[1] *= scale;
	thingy[2] *= scale;

	glm::mat4 out = glm::mat4(thingy);
	out[0].w = trans.x;
	out[1].w = trans.y;
	out[2].w = trans.z;
	out[3].w = 1.0;

	return out;
}

bool loadIQMMesh(string filename, iqmheader header, Model &target, unsigned char *buf){

	iqmvertexarray *iva = (iqmvertexarray *)&buf[header.ofs_vertexarrays];
	float *inposition = NULL, *intexcoord = NULL, *innormal=NULL, *intangent=NULL;
	unsigned char *inblendindex=NULL, *inblendweight=NULL;
	const char *str = header.ofs_text ? (char *)&buf[header.ofs_text] : "";
	for(int i=0;i < (int)header.num_vertexarrays; i++){
		iqmvertexarray &va = iva[i];
		switch(va.type){
			case IQM_POSITION:
				if(va.format != IQM_FLOAT || va.size != 3){
					return false; //Bad vertex position
				}
				inposition = (float *)&buf[va.offset];
				break;
			case IQM_TEXCOORD:
				if(va.format != IQM_FLOAT || va.size !=2){
					return false; //Bad texture coords
				}
				intexcoord = (float *)&buf[va.offset];
				break;
			case IQM_NORMAL:
				if(va.format != IQM_FLOAT || va.size != 3){
					return false; //Bad normal
				}
				innormal = (float*)&buf[va.offset];
				break;
			case IQM_TANGENT:
				if(va.format != IQM_FLOAT || va.size != 4){
					return false; //Bad tangent
				}
				intangent = (float*)&buf[va.offset];
				break;
			case IQM_BLENDWEIGHTS:
				if(va.format != IQM_UBYTE || va.size != 4){
					return false; //Bad blendweight
				}
				inblendweight = (unsigned char*)&buf[va.offset];
				break;
			case IQM_BLENDINDEXES:
				if(va.format != IQM_UBYTE || va.size != 4){
					return false; //Bad indexes
				}
				inblendindex = (unsigned char*)&buf[va.offset];
				break;
		}
	}

	iqmmesh *meshes = (iqmmesh*)&buf[header.ofs_meshes];

	GLuint texid;
	for(int i=0;i<(int)header.num_meshes;i++){
		iqmmesh &m = meshes[i];
		cout << "LOADING MESH: " << &str[m.name] << endl;
		cout << "WITH TEXTURE: " << &str[m.material] << endl;
		//string texture = "./data/textures/";
		string texture = &str[m.material];
		texid =	resman.loadTexture(texture);

		target.setTEXID(texid);
		target.textureIDS.push_back(texid);
		target.meshes.push_back(m);

		//cout << "IMAGE X:"<<img.getSize().x << " IMAGE Y:" << img.getSize().y << endl;
	}

	//load joints
	iqmjoint *joints = (iqmjoint*)&buf[header.ofs_joints];
	target.baseframe.resize(header.num_joints);
	target.inversebaseframe.resize(header.num_joints);
	for(int i=0;i<(int)header.num_joints;i++){
		iqmjoint &j = joints[i];
		target.joints.push_back(j);

		glm::quat q;
		q.x = j.rotate[0];
		q.y = j.rotate[1];
		q.z = j.rotate[2];
		q.w = j.rotate[3];

        glm::vec3 scalevec = glm::vec3(j.scale[0],j.scale[1],j.scale[2]);
        glm::vec3 trans(j.translate[0],j.translate[1],j.translate[2]);

		target.baseframe[i] = makeBoneMat(q,trans,scalevec);
		target.inversebaseframe[i] = glm::inverse(target.baseframe[i]);

		//If the parent attribute is less than zero, it is a root bone; otherwise it is not
		if(j.parent >= 0){
			target.baseframe[i] = mul(target.baseframe[j.parent], target.baseframe[i]);
			target.inversebaseframe[i] = mul(target.inversebaseframe[i],target.inversebaseframe[j.parent]);
		}
	}


	//Load triangles;
	GLuint ebo;
	iqmtriangle *tris = (iqmtriangle *)&buf[header.ofs_triangles];
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			header.num_triangles*sizeof(iqmtriangle), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	target.setEBO(ebo);

	for(int i=0;i<(int)header.num_vertexes;i++){
		vertex v;
		if(inposition)
			memcpy(v.position, &inposition[i*3], sizeof(v.position));
		if(intexcoord)
			memcpy(v.texcoord, &intexcoord[i*2], sizeof(v.texcoord));
		if(intangent)
			memcpy(v.tangent, &intangent[i*4],sizeof(v.tangent));
		if(innormal)
			memcpy(v.normal, &innormal[i*3], sizeof(v.normal));
		if(inblendindex)
			memcpy(v.blendindex, &inblendindex[i*4],sizeof(v.blendindex));
		if(inblendweight)
			memcpy(v.blendweight, &inblendweight[i*4],sizeof(v.blendweight));

		target.verts.push_back(v);
	}

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	target.setVBO(vbo);
	glBufferData(GL_ARRAY_BUFFER,header.num_vertexes*sizeof(vertex),
			&target.verts[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	cout << "VBO: " << vbo << " EBO: " << ebo << " ID " << texid << endl;

	return true;
}

bool loadIQMAnim(string filename, iqmheader header,Model &target, unsigned char *buf){
	if(header.num_poses != header.num_joints)
		return false;

	const char *str = header.ofs_text ? (char*)&buf[header.ofs_text] : "";
	iqmanim *anims = (iqmanim*)&buf[header.ofs_anims];
	iqmpose *poses = (iqmpose*)&buf[header.ofs_poses];

	vector<glm::mat4> frames(header.num_frames * header.num_poses);
	unsigned short *framedata = (uint16_t*)&buf[header.ofs_frames];

	for(int i=0; i < (int)header.num_frames;i++){
		for(int j=0; j < (int)header.num_poses;j++){
			iqmpose &p = poses[j];
			glm::quat rotation; //Rotation quaternion
            glm::vec3 trans, scale; //Translation and scaling vectors, respectively

            //Load data from buffer into variables
            trans.x = p.channeloffset[0]; if(p.mask&0x01) trans.x += *framedata++ * p.channelscale[0];
            trans.y = p.channeloffset[1]; if(p.mask&0x02) trans.y += *framedata++ * p.channelscale[1];
            trans.z = p.channeloffset[2]; if(p.mask&0x04) trans.z += *framedata++ * p.channelscale[2];
            rotation.x = p.channeloffset[3]; if(p.mask&0x08) rotation.x += *framedata++ * p.channelscale[3];
            rotation.y = p.channeloffset[4]; if(p.mask&0x10) rotation.y += *framedata++ * p.channelscale[4];
            rotation.z = p.channeloffset[5]; if(p.mask&0x20) rotation.z += *framedata++ * p.channelscale[5];
            rotation.w = p.channeloffset[6]; if(p.mask&0x40) rotation.w += *framedata++ * p.channelscale[6];
            scale.x = p.channeloffset[7]; if(p.mask&0x80) scale.x += *framedata++ * p.channelscale[7];
            scale.y = p.channeloffset[8]; if(p.mask&0x100) scale.y += *framedata++ * p.channelscale[8];
            scale.z = p.channeloffset[9]; if(p.mask&0x200) scale.z += *framedata++ * p.channelscale[9];

			glm::mat4 m = makeBoneMat(rotation,trans,scale);
			if(p.parent >= 0) 
				frames[i*header.num_poses + j] = mul(mul(target.baseframe[p.parent],m), target.inversebaseframe[j]);
			else
				frames[i*header.num_poses+j] = target.inversebaseframe[j] * m;
		}

	}

	target.numFrames = header.num_frames-1;

	target.frames = frames;
	for(int i=0;i<(int)header.num_anims;i++){
		iqmanim &a = anims[i];
		cout << "Loaded animation: " << &str[a.name] << endl;
		target.anims.push_back(a);
		string animName = &str[a.name];
		target.animNames.push_back(animName);
	}

	return true;
}

bool loadIQM(string filename, Model &target){
	ifstream file;
	iqmheader header;
	file.open(filename.c_str(), ios::binary);
	if(file.is_open()){
		file.read((char*)&header,sizeof(iqmheader));
		if(strcmp(header.magic,IQM_MAGIC) == true || header.version != IQM_VERSION){
			cout << "Invalid IQM model: " << filename << endl;
			return false;
		}
		if(header.filesize > (16<<20)){
			cout << "IQM model: " << filename << " is over 16mb not loading" << endl;
			return false;
		}

		cout << "Loading valid IQM model: " << filename << endl;


		unsigned char *buf = new unsigned char[header.filesize];
		file.read((char*)buf+sizeof(header),header.filesize - sizeof(header));

		file.close();

		loadIQMMesh(filename, header, target, buf);
		loadIQMAnim(filename, header, target, buf);
		delete[] buf;

		return true;
	}else{
		cout << "Could not open file: " << filename << endl;
		return false;
	}

	return false;
}

void Model::setEBO(GLuint ebo){
	this->tris_ebo = ebo;
}

void Model::setVBO(GLuint vbo){
	this->verts_vbo = vbo;
}

void Model::setTEXID(GLuint id){
	this->textureID = id;
}

/**
 * Sets animation matrixes for model
 * @param animName Name of the animation to run
 * @param curTime Current elapsed frame time
 */
void Model::animate(string animName, float curTime, std::vector<glm::mat4> *outframe){
	bool foundAnim = false;
	iqmanim anim;
	for(int i=0;i<anims.size();i++)
		if(animNames[i] == animName){
			anim = anims[i];
			foundAnim = true;
		}
	if(!foundAnim){
		cout << "No such animation: " << animName << endl;
		return;
	}

	float currentframe = (curTime * anim.framerate);
	currentframe = fmod(currentframe,anim.num_frames); //Wrap currentframe to length of anim

	if(frames.size() <= 0) return;
	int frame1 = (int)floor(currentframe);
	int frame2 = frame1 + 1;

	float frameoffset = currentframe - frame1;
	frame1 += anim.first_frame; //Offset frame1 to current anim

	frame2 %= anim.num_frames; //Wrap frame2 to length of anim (in case it was incremented over it)
	frame2 += anim.first_frame; //Offset frame2 to current anim

	glm::mat4 *mat1 = &frames[frame1 * joints.size()];
	glm::mat4 *mat2 = &frames[frame2 * joints.size()];

	for(int i=0;i<joints.size();i++){
		glm::mat4 mat = (1-frameoffset)*mat1[i] + frameoffset*mat2[i];
		if(joints[i].parent >= 0)
			(*outframe)[i] = mat * (*outframe)[joints[i].parent];
		else
			(*outframe)[i] = mat;
	}
}

void Model::draw(ShaderProgram *prg, vector<glm::mat4> outframe){
	bool skin = true;
	glUseProgram(prg->getID());
	glm::mat3x4 outframe3x4[outframe.size()];
	for(int i=0;i<outframe.size();i++)
		outframe3x4[i] = glm::mat3x4(outframe[i]);

	GLsizei arrsize = outframe.size(); //OpenGL will complain if I feed it a size_t
	if(frames.size() > 0)
		glUniformMatrix3x4fv(prg->getUniform(1), arrsize, GL_FALSE, glm::value_ptr(outframe3x4[0]));
	else
		skin = false;

	glUniform1i(prg->getUniform(4),skin);

	glBindBuffer(GL_ARRAY_BUFFER,verts_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris_ebo);
	vertex *vert = NULL;
	glVertexAttribPointer(
			prg->getAttribute(0),
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(vertex),
			&vert->position);
	glVertexAttribPointer(
			prg->getAttribute(1),
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(vertex),
			&vert->texcoord);
	if(frames.size() > 0){
		glVertexAttribPointer(
				prg->getAttribute(2),
				4,
				GL_UNSIGNED_BYTE,
				GL_TRUE,
				sizeof(vertex),
				&vert->blendweight);
		glVertexAttribPointer(
				prg->getAttribute(3),
				4,
				GL_UNSIGNED_BYTE,
				GL_FALSE,
				sizeof(vertex),
				&vert->blendindex);
	}
	glEnableVertexAttribArray(prg->getAttribute(0));
	glEnableVertexAttribArray(prg->getAttribute(1));
	glEnableVertexAttribArray(prg->getAttribute(2));
	glEnableVertexAttribArray(prg->getAttribute(3));

	iqmtriangle *tris = NULL;
	for(int i=0;i<meshes.size();i++){
		iqmmesh &m = meshes[i];
		glBindTexture(GL_TEXTURE_2D, textureIDS[i]);
		glUniform1i(prg->getUniform(2),0);
		glDrawElements(GL_TRIANGLES, 3*m.num_triangles, GL_UNSIGNED_INT, &tris[m.first_triangle]);
	}

	glDisableVertexAttribArray(prg->getAttribute(0));
	glDisableVertexAttribArray(prg->getAttribute(1));
	glDisableVertexAttribArray(prg->getAttribute(2));
	glDisableVertexAttribArray(prg->getAttribute(3));
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
