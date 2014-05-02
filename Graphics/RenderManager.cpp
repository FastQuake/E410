#include "RenderManager.hpp"
#include "../globals.hpp"
#include <sstream>

GameObject *RenderManager::getId(uint32_t id){
	for(int i=0;i<drawList.size();i++){
		if(drawList[i]->id == id){
			return drawList[i];
		}
	}
	return NULL;
}

void RenderManager::drawScene(ShaderProgram *prg, float dt, bool texture, bool normal){
	for(int i=0;i<this->drawList.size();i++){
		if(drawList[i]->animate){
			drawList[i]->aTime += dt;
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
			drawList[i]->aTime -=dt;
		}

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = rot * scale * trans;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		if(texture){
			glm::mat4 modelMatIT = glm::inverse(glm::transpose(modelMat));
			glUniformMatrix4fv(prg->getUniform("modelMatIT"),1,GL_FALSE,glm::value_ptr(modelMatIT));
		}
		drawList[i]->model->draw(prg,drawList[i]->outframe, texture, normal);
	}
}

void RenderManager::renderDepth(ShaderProgram *prg, float dt, int lightIndex){
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

		drawScene(prg,dt,false,false);
	}else if(light->type == POINT_LIGHT){
		PLight myLight = *static_cast<PLight*>(light);
		for(int i=0;i<6;i++){
			glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,rendman.depthCubemaps,0,lightIndex*6+i);
			glm::mat4 depthMVP = myLight.mvp(i);
			glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));
			glClear(GL_DEPTH_BUFFER_BIT);
			drawScene(prg,dt,false,false);
		}
	}
	glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,0,0,0);
	glViewport(0,0,width,height);
}

void RenderManager::renderDeferred(ShaderProgram *prg, float dt){
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
	drawScene(prg,dt,false,true);
	if(faku){ //doesn't work, will remove this later
		std::cout << "screenshot" << std::endl;
		sf::Image depthimg;
		GLubyte *pixels = new GLubyte[width*height*4];
		glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
		depthimg.create(width,height,sf::Color::Black);
		for(unsigned int i=0,x=0,y=0;i<width*height;i++){
			x = i%(width);
			if(x == 0 && i>0)
				y++;
			depthimg.setPixel(x,y,sf::Color(ceil((double)(pixels[y*width+x]*255.0f)),
			ceil((double)(pixels[y*width+x+1]*255.0f)),
			ceil((double)(pixels[y*width+x+2]*255.0f)),255));
		}
		depthimg.saveToFile("asdf.png");
		delete[] pixels;
	}
}

void RenderManager::render(ShaderProgram *prg, float dt){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextures);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, depthCubemaps);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,normalTex);
	//glUniform1i(prg->getUniform("shadowMaps"), 1);
	glUniform1i(prg->getUniform("shadowCubes"), 1);
	glUniform1i(prg->getUniform("normalTex"), 2);

	struct toUBO{
		glm::mat4 depthMVPs[MAX_LIGHTS];
		glm::vec4 lightPositions[MAX_LIGHTS];
		glm::vec4 lightTypes[MAX_LIGHTS];
		glm::vec4 numLights;
	};

	toUBO data;
	glBindBufferARB(GL_UNIFORM_BUFFER, ubo);
	glUniformBlockBinding(prg->getID(),glGetUniformBlockIndex(prg->getID(), "Light"),0);

	for(int i=0;i<lights.size();i++){
		glm::mat4 mv = lights[i]->mvp();
		data.depthMVPs[i] = mv;
		data.lightPositions[i] = glm::vec4(lights[i]->pos,1.0);
		data.lightTypes[i].x = lights[i]->type;
	}
	data.numLights.x = lights.size();
	//std::cout << lights.size() << std::endl;

	glBufferSubDataARB(GL_UNIFORM_BUFFER,0,sizeof(data),&data);
	glm::mat4 view = currentCam->view();
	glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	drawScene(prg,dt,true,true);
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

