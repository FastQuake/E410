#include <iostream>
#include "Sprite.hpp"
#include "../globals.hpp"
using namespace std;

ShaderProgram *spriteprg;

Sprite::Sprite(std::string name):sprite(){
	texture = resman.loadTexture(name,&imgwidth,&imgheight);
	sprite.setModel(resman.loadModel("cube.iqm"));
	//sprite.textures[0] = texture;
	position = glm::vec3(0,0,0);
	scale = 1;
}

void Sprite::draw(float dt, FPSCamera *cam){
	//construct the matrix
	glm::mat4 trans = glm::translate(glm::mat4(1),position);
	glm::mat4 look = glm::lookAt(cam->pos, cam->pos-position, glm::vec3(0,1,0));
	glm::mat4 scalem = glm::scale(glm::mat4(1),glm::vec3(scale,scale,scale));
	glm::mat4 model = trans*look*scale;
	model *= glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1.0,0,0)); //Rotate everything -90deg on x axis
	glUniformMatrix4fv(spriteprg->getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	sprite.model->draw(spriteprg,sprite.textures, sprite.outframe, true, false);
}
