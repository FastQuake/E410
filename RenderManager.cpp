#include "RenderManager.hpp"

RenderManager::~RenderManager(){
	for(int i=0;i<this->drawList.size();i++){
		delete drawList[i];
	}
}

void RenderManager::render(ShaderProgram *prg, float curTime){

	for(int i=0;i<this->drawList.size();i++){
		drawList[i]->model->animate("Taunt",curTime);

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x-90,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = scale * rot * trans;
		glUniformMatrix4fv(prg->getUniform(3),1,GL_FALSE,glm::value_ptr(modelMat));
		drawList[i]->model->draw(prg);
	}
}
