#include <SFML/Graphics/Image.hpp>
#include "RenderManager.hpp"
#include "../globals.hpp"

RenderManager::~RenderManager(){
	for(int i=0;i<this->drawList.size();i++){
		delete drawList[i];
	}
}

glm::mat4 bias(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
	);

void RenderManager::renderShadow(ShaderProgram *prg, float dt){
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint tempTex;

	/*glGenTextures(1, &tempTex);
	glBindTexture(GL_TEXTURE_2D, tempTex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,width,height,0,GL_RED,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	//TODO: Enable this for loop when implementing multiple lights, set lastpass on last run through loop
	//for(int i=0;i<lights.size();i++){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tempTex);
	glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D,depthTextures[i]);
	glBindTexture(GL_TEXTURE_2D,depthTexture);
	glUniform1i(prg->getUniform("lastpass"),0);
	glUniform1i(prg->getUniform("shadowTexture"),0);
	glUniform1i(prg->getUniform("depthTexture"),1);
	glUniformMatrix4fv(prg->getUniform("lightMat"),1,GL_FALSE,glm::value_ptr(bias*light.mvp()));
	glUniformMatrix4fv(prg->getUniform("cameraMat"),1,GL_FALSE,glm::value_ptr(currentCam->view()));
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
	//}
	GLfloat *pixels = new GLfloat[width*height];
	glReadPixels(0,0,width,height,GL_RED,GL_FLOAT,pixels);

	sf::Image image;
	image.create(width,height,sf::Color::Black);
	for(unsigned int i=0,x=0,y=0;i<width*height;i++){
		x = i%width;
		if(x == 0 && i>0)
			y++;
		image.setPixel(x,y,sf::Color(	ceil((double)(pixels[i]*255.0f)),
		0,0,255));
	}
	image.saveToFile("asdf.png");

}

void RenderManager::renderDepth(ShaderProgram *prg, float dt){
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
	GLfloat *pixels = new GLfloat[1024*1024];
	glReadPixels(0,0,1024,1024,GL_DEPTH_COMPONENT,GL_FLOAT,pixels);

/*	sf::Image image;
	image.create(1024,1024,sf::Color::Black);
	for(unsigned int i=0,x=0,y=0;i<1024*1024;i++){
		x = i%1024;
		if(x == 0 && i>0)
			y++;
		image.setPixel(x,y,sf::Color(	ceil((double)(pixels[i]*255.0f)),
		0,0,255));
	}
	image.saveToFile("asdf.png");*/

	glViewport(0,0,width,height);
}

void RenderManager::render(ShaderProgram *prg, float dt){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
	glUniform1i(prg->getUniform("shadowMap"), 1);

	glm::mat4 mv = glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f)*currentCam->view();
	glm::mat4 depthMVP = bias*mv;
	glUniformMatrix4fv(prg->getUniform("shadowMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
	
	for(int i=0;i<this->drawList.size();i++){
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
		glm::mat4 modelMat = rot * scale * trans;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform("modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
		drawList[i]->model->draw(prg,drawList[i]->outframe, true);
	}
}
