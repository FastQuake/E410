#include "Image.hpp"
#include "../globals.hpp"
using namespace std;

Image::Image(){
	magic = GUIIMAGE_MAGIC;

	tex = resman.loadTexture("default.png");

	scale = glm::vec2(tex.width,tex.height);
	colour = glm::vec4(0,0,0,1);
	
	visible = true;
	updates = false;
	alive = true;
	locks = false;
}

void Image::setImage(std::string name){
	tex = resman.loadTexture(name);
	scale = glm::vec2(tex.width, tex.height);
}

void Image::setImage(Texture t){
	tex = t;
	scale = glm::vec2(tex.width, tex.height);
}

void Image::draw(ShaderProgram *prg){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}

	glm::vec2 tScale;
	tScale.x = scale.x/width;
	tScale.y = scale.y/height;

	tpos.x = tpos.x/width;
	tpos.y = tpos.y/height;
	tpos.x = remap(tpos.x,0,1,-1,1)+(tScale.x);
	tpos.y = -(remap(tpos.y,0,1,-1,1)+(tScale.y));

	glBindTexture(GL_TEXTURE_2D, tex.id);
	glUniform1i(prg->getUniform("inTexture"),0);
	glUniform2fv(prg->getUniform("pos"),1,glm::value_ptr(tpos));
	glUniform2fv(prg->getUniform("scale"),1,glm::value_ptr(tScale));
	glUniform4fv(prg->getUniform("colour"),1,glm::value_ptr(colour));
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glVertexAttribPointer(
			prg->getAttribute("coord2d"),
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GL_FLOAT)*2,
			0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
