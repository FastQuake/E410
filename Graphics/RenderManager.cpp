#include <SFML/Graphics/Image.hpp>
#include "RenderManager.hpp"
#include "../globals.hpp"

RenderManager::~RenderManager(){
	for(int i=0;i<this->drawList.size();i++){
		delete drawList[i];
	}
}

void RenderManager::renderDepth(ShaderProgram *prg, float dt, Light light){
	glViewport(0,0,1024,1024);

	glm::mat4 depthMVP = light.mvp();
	glUniformMatrix4fv(prg->getUniform("pv"), 1, GL_FALSE, glm::value_ptr(depthMVP));

	glClear(GL_DEPTH_BUFFER_BIT);
	for(int i=0;i<this->drawList.size();i++){
		if(drawList[i]->animate){
			drawList[i]->aTime += dt; 
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
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

	glViewport(0,0,width,height);
}

void RenderManager::render(ShaderProgram *prg, float dt){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, depthTextures);
	glUniform1i(prg->getUniform("shadowMap"), 1);

	glm::mat4 bias(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthMVPs[lights.size()];
	for(int i=0;i<lights.size();i++){
		glm::mat4 mv = lights[i].mvp();
		depthMVPs[i] = bias*mv;
	}
	glUniformMatrix4fv(prg->getUniform("depthMVPs"), lights.size(), GL_FALSE, glm::value_ptr(depthMVPs[0]));
	glUniform1i(prg->getUniform("numLights"),lights.size());

	for(int i=0;i<this->drawList.size();i++){
		if(drawList[i]->animate){
			drawList[i]->aTime += dt;
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
		}

		glm::mat4 view = currentCam->view();
		//std::cout << currentCam->pos.x << "," << currentCam->pos.y << "," << currentCam->pos.z << std::endl;
		//std::cout << currentCam->angle.x << "," << currentCam->angle.y << "," << currentCam->angle.z << std::endl;
		glUniformMatrix4fv(prg->getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = rot * scale * trans;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		drawList[i]->model->draw(prg,drawList[i]->outframe, true);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}
