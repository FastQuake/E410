#include <iostream>
#include "RenderManager.hpp"
#include "../globals.hpp"
#include "../Networking/Physics.hpp"
#include "../Networking/server.hpp"
using namespace std;

GameObject *RenderManager::getId(uint32_t id){
	for(int i=0;i<drawList.size();i++){
		if(drawList[i]->id == id){
			return drawList[i];
		}
	}
	return NULL;
}

void RenderManager::drawScene(ShaderProgram *prg, bool texture, bool normal){
	for(int i=0;i<this->drawList.size();i++){
		if(this->drawList[i]->visible == false)
			continue;

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		btQuaternion quat(toRad(drawList[i]->rotation.y), toRad(drawList[i]->rotation.x), toRad(drawList[i]->rotation.z));
		quat = drawList[i]->rot;
		glm::quat outquat;
		outquat.x = quat.getX();
		outquat.y = quat.getY();
		outquat.z = quat.getZ();
		outquat.w = quat.getW();
		glm::mat4 rot = glm::mat4_cast(glm::normalize(outquat));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::vec3 originOffset = glm::vec3(	drawList[i]->originOffset.getX(),
											drawList[i]->originOffset.getY(),
											drawList[i]->originOffset.getZ());
		glm::mat4 transOrigin = glm::translate(glm::mat4(1), originOffset);
		glm::mat4 modelMat = trans * transOrigin * rot * glm::inverse(transOrigin) * scale;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		if(texture){
			glm::mat4 modelMatIT = glm::inverse(glm::transpose(modelMat));
			glUniformMatrix4fv(prg->getUniform("modelMatIT"),1,GL_FALSE,glm::value_ptr(modelMatIT));
		}
		drawList[i]->model->draw(prg, drawList[i]->textures, drawList[i]->outframe, texture, normal);
	}
}

void RenderManager::renderDepth(ShaderProgram *prg, int lightIndex){
	#ifdef WINDOWS
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	#endif
	glViewport(0,0,512,512);
	Light *light = lights[lightIndex];

	if(light->type == DIRECTIONAL_LIGHT){
		glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,rendman.depthTextures,0,lightIndex);
		glClear(GL_DEPTH_BUFFER_BIT);
		DLight myLight = *static_cast<DLight*>(light);
		glm::mat4 depthMVP = myLight.mvp();
		glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));

		drawScene(prg,false,false);
	}else if(light->type == POINT_LIGHT){
		PLight myLight = *static_cast<PLight*>(light);
		for(int i=0;i<6;i++){
			glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,rendman.depthCubemaps,0,lightIndex*6+i);
			glm::mat4 depthMVP = myLight.mvp(i);
			glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));
			glClear(GL_DEPTH_BUFFER_BIT);
			drawScene(prg,false,false);
		}
	}
	glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,0,0,0);
	glViewport(0,0,width,height);
}

void RenderManager::renderDeferred(ShaderProgram *prg){
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,renderbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,normalTex,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef WINDOWS
	GLenum draw_bufs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1,draw_bufs);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
#endif
	glm::mat4 view = currentCam->view();
	glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	drawScene(prg,false,true);
}

void RenderManager::render(ShaderProgram *prg, ShaderProgram *skyprg, float dt){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*width/height, 0.01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0,0,0), currentCam->getLookat(),glm::vec3(0,1,0));

	//Draw the skybox without writing any depth information
	glUseProgram(skyprg->getID());
	glDepthMask(0);
	view *= glm::scale(glm::mat4(1),glm::vec3(50,50,50));
	glUniformMatrix4fv(skyprg->getUniform("view"),1,GL_FALSE,glm::value_ptr(view));
	skybox.model->draw(skyprg,skybox.textures,skybox.outframe,true,false);
	glDepthMask(1);

	view = currentCam->view();
	//cout << currentCam->pos.x << "," << currentCam->pos.y << "," << currentCam->pos.z << " | " << currentCam->angle.x << "," << currentCam->angle.y << "," << currentCam->angle.z << "," << endl;
	glUseProgram(prg->getID());
	glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	drawScene(prg,true,true);
	/*glUseProgram(debugprg->getID());
	glUniformMatrix4fv(debugprg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	physworld.dynWorld->debugDrawWorld();*/
}

void RenderManager::updateUBO(){
	struct toUBO{
			glm::mat4 depthMVPs[MAX_LIGHTS];
			glm::vec4 lightPositions[MAX_LIGHTS];
			glm::vec4 lightTypes[MAX_LIGHTS];
			glm::vec4 numLights;
		};
	toUBO data;
	for(int i=0;i<lights.size();i++){
			glm::mat4 mv = lights[i]->mvp();
			data.depthMVPs[i] = mv;
			data.lightPositions[i] = glm::vec4(lights[i]->pos,1.0);
			data.lightTypes[i].x = lights[i]->type;
	}
	data.numLights.x = lights.size();
	glBindBufferARB(GL_UNIFORM_BUFFER,ubo);
	glBufferSubDataARB(GL_UNIFORM_BUFFER,0,sizeof(data),&data);
	glBindBufferARB(GL_UNIFORM_BUFFER,0);
}

void RenderManager::remove(GameObject *obj){
	for(int i=0;i<drawList.size();i++){
		if(obj == drawList[i]){
			drawList.erase(drawList.begin() + i);
			return;
		}
	}
}

void RenderManager::removeLight(Light *light){
	for(int i=0;i<lights.size();i++){
		if(light == lights[i]){
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

GameObject *RenderManager::getBody(const btCollisionObject *body){
	for(int i=0;i<drawList.size();i++){
		if(drawList[i]->body->upcast(body) == drawList[i]->body){
			return drawList[i];
		}
	}
	return NULL;
}

GLDebugDrawer::GLDebugDrawer(){
}

GLDebugDrawer::~GLDebugDrawer(){}
void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3&  fromColor, const btVector3& toColor){
	glm::vec3 line[2];
	line[0] = glm::vec3(from.getX(),from.getY(),from.getZ());
	line[1] = glm::vec3(to.getX(),to.getY(),to.getZ());
	glBindBuffer(GL_ARRAY_BUFFER,debugvbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*2,&line[0],GL_STATIC_DRAW);
	glEnableVertexAttribArray(debugprg->getAttribute("vertpos"));
	glVertexAttribPointer(debugprg->getAttribute("vertpos"),3,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays( GL_POINTS, 0, 2 );
	glDrawArrays(GL_LINES,0,2);
	glDisableVertexAttribArray(debugprg->getAttribute("vertpos"));
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GLDebugDrawer::setDebugMode(int debugMode){
	m_debugMode = debugMode;
}

void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color){drawLine(from,to,color,color);}
void GLDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color){}
void GLDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha){
	drawLine(a,b,color,color);
	drawLine(b,c,color,color);
	drawLine(c,a,color,color);
}
void GLDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){}
void GLDebugDrawer::reportErrorWarning(const char* warningString){std::cout << warningString << std::endl;}
void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString){}
int GLDebugDrawer::getDebugMode() const {return m_debugMode;}
