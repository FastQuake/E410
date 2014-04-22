#include <iostream>
#include "RenderManager.hpp"
#include "../globals.hpp"
using namespace std;

GameObject *RenderManager::getId(uint32_t id){
	for(int i=0;i<drawList.size();i++){
		if(drawList[i]->id == id){
			return drawList[i];
		}
	}
	return NULL;
}

void RenderManager::render(ShaderProgram *prg, ShaderProgram *skyprg, float dt){
	
	if(currentCam == NULL){
		return;
	}
	
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*width/height, 0.01f, 1000.0f);

	//Draw the skybox without writing any depth information
	glDepthMask(0);
	glUseProgram(skyprg->getID());
	glUniformMatrix4fv(skyprg->getUniform("projection"),1,GL_FALSE,glm::value_ptr(projection));
	glm::mat4 view = glm::lookAt(glm::vec3(0,0,0),currentCam->getLookat(),glm::vec3(0,1,0));
	view *= glm::scale(glm::mat4(1),glm::vec3(50,50,50));
	glUniformMatrix4fv(skyprg->getUniform("view"),1,GL_FALSE,glm::value_ptr(view));
	skybox.model->draw(skyprg,skybox.textures,skybox.outframe);
	glDepthMask(1);

	glUseProgram(prg->getID());
	for(int i=0;i<this->drawList.size();i++){
		if(this->drawList[i]->visible == false)
			continue;
		glUniformMatrix4fv(prg->getUniform("projection"),1,GL_FALSE,glm::value_ptr(projection));
		if(drawList[i]->animate){
			drawList[i]->aTime += dt; 
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
		}

		glm::mat4 view = currentCam->view();
		glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = trans * rot * scale;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		drawList[i]->model->draw(prg,drawList[i]->textures,drawList[i]->outframe);
	}
}

void RenderManager::remove(GameObject *obj){
	for(int i=0;i<drawList.size();i++){
		if(obj == drawList[i]){
			drawList.erase(drawList.begin() + i);
		}
	}
}

