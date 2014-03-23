#include "RenderManager.hpp"
#include <SFML/Graphics/Image.hpp>

RenderManager::~RenderManager(){
	for(int i=0;i<this->drawList.size();i++){
		delete drawList[i];
	}
}

void RenderManager::render(ShaderProgram *prg, float dt){
	glViewport(0,0,1024,1024);
	//glScissor(0,0,2048,2048);

	GLuint madetex;
	glGenTextures(1,&madetex);
	glBindTexture(GL_TEXTURE_2D,madetex);

	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,800,600,0,GL_RGB,GL_FLOAT,0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,1024,1024,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D,0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,madetex,0);
	//glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,madetex,0);


	/*GLuint dummybuffer;
	glGenRenderbuffers(1,&dummybuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, dummybuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,1024,1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,dummybuffer);*/

	//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_BACK);

	GLenum asdgfasd = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::cout << asdgfasd << std::endl;
	for(int i=0;i<this->drawList.size();i++){
		if(drawList[i]->animate){
			drawList[i]->aTime += dt; 
			drawList[i]->model->animate(drawList[i]->currentAnimation,
					drawList[i]->aTime,&drawList[i]->outframe);
		}

		currentCam->pos = glm::vec3(-4.0,6.0,-4.0);
		currentCam->angle = glm::vec3(-137.6f,115.7f,0.0f);
		//std::cout << currentCam->angle.x << "," << currentCam->angle.y << "," << currentCam->angle.z << std::endl;
		glm::mat4 view = currentCam->view();
		glUniformMatrix4fv(prg->getUniform(5), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 scale = glm::scale(glm::mat4(1),drawList[i]->scale);
		glm::mat4 rot = \
			glm::rotate(glm::mat4(1),drawList[i]->rotation.x,glm::vec3(1.0,0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.y,glm::vec3(0,1.0,0)) *
			glm::rotate(glm::mat4(1),drawList[i]->rotation.z,glm::vec3(0,0,1.0));
		glm::mat4 trans = glm::translate(glm::mat4(1), drawList[i]->position);
		glm::mat4 modelMat = rot * scale * trans;
		modelMat *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
		glUniformMatrix4fv(prg->getUniform(3),1,GL_FALSE,glm::value_ptr(modelMat));
		drawList[i]->model->draw(prg,drawList[i]->outframe);
	}
	GLfloat *pixels = new GLfloat[1024*1024];
	glReadPixels(0,0,1024,1024,GL_DEPTH_COMPONENT,GL_FLOAT,pixels);

	sf::Image image;
	image.create(1024,1024,sf::Color::Black);
	for(unsigned int i=0,x=0,y=0;i<1024*1024;i++){
		x = i%1024;
		if(x == 0 && i>0)
			y++;
		image.setPixel(x,y,sf::Color(	ceil((double)(pixels[i]*255.0f)),
										ceil((double)(pixels[i]*255.0f)),
										ceil((double)(pixels[i]*255.0f)),255));
	}
	image.saveToFile("asdf.tga");

	glDeleteTextures(1,&madetex);
	delete[] pixels;
}
