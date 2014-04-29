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
void RenderManager::renderDepth(ShaderProgram *prg, float dt, int lightIndex){
	glViewport(0,0,1024,1024);
	Light *light = lights[lightIndex];

	if(light->type == DIRECTIONAL_LIGHT){
		glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,rendman.depthTextures,0,lightIndex);
		glClear(GL_DEPTH_BUFFER_BIT);
		DLight myLight = *static_cast<DLight*>(light);
		glm::mat4 depthMVP = myLight.mvp();
		glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));

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
			drawList[i]->model->draw(prg,drawList[i]->outframe, false);
		}
	}else if(light->type == POINT_LIGHT){
		PLight myLight = *static_cast<PLight*>(light);
		for(int i=0;i<6;i++){
			glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,rendman.depthCubemaps,0,lightIndex*6+i);
			glm::mat4 depthMVP = myLight.mvp(i);
			glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));
			glClear(GL_DEPTH_BUFFER_BIT);
			for(int j=0;j<this->drawList.size();j++){
				if(drawList[j]->animate){
					drawList[j]->aTime += dt;
					drawList[j]->model->animate(drawList[j]->currentAnimation,
							drawList[j]->aTime,&drawList[j]->outframe);
					drawList[j]->aTime -=dt;
				}

				glm::mat4 scale = glm::scale(glm::mat4(1),drawList[j]->scale);
				glm::mat4 rot = \
					glm::rotate(glm::mat4(1),drawList[j]->rotation.x,glm::vec3(1.0,0,0)) *
					glm::rotate(glm::mat4(1),drawList[j]->rotation.y,glm::vec3(0,1.0,0)) *
					glm::rotate(glm::mat4(1),drawList[j]->rotation.z,glm::vec3(0,0,1.0));
				glm::mat4 trans = glm::translate(glm::mat4(1), drawList[j]->position);
				glm::mat4 modelMat = rot * scale * trans;
				modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
				glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
				drawList[j]->model->draw(prg,drawList[j]->outframe, false);
			}
		}
	}
	glViewport(0,0,width,height);
}

void RenderManager::render(ShaderProgram *prg, float dt){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextures);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY_ARB, depthCubemaps);
	glUniform1i(prg->getUniform("shadowMaps"), 1);
	glUniform1i(prg->getUniform("shadowCubes"), 2);

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
	for(int i=0;i<this->drawList.size();i++){
		if(drawList[i]->animate){
			drawList[i]->aTime += dt;
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
		}

		glm::mat4 view = currentCam->view();
	//	std::cout << currentCam->pos.x << "," << currentCam->pos.y << "," << currentCam->pos.z << "|" << currentCam->angle.x << "," << currentCam->angle.y << "," << currentCam->angle.z << std::endl;
		glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = rot * scale * trans;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glm::mat4 modelMatIT = glm::inverse(glm::transpose(modelMat));
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		glUniformMatrix4fv(prg->getUniform("modelMatIT"),1,GL_FALSE,glm::value_ptr(modelMatIT));
		drawList[i]->model->draw(prg,drawList[i]->outframe, true);
	}
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

