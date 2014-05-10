#include <iostream>
#include "Sprite.hpp"
#include "../globals.hpp"
using namespace std;

ShaderProgram *spriteprg;

Sprite::Sprite(std::string name){
	texture = resman.loadTexture(name,&imgwidth,&imgheight);
	sprite.setModel(resman.loadModel("cube.iqm"));
	//sprite.textures[0] = texture;
}

void Sprite::draw(float dt, glm::vec3 camrot){
	//construct the matrix
	glm::mat4 trans = glm::translate(glm::mat4(1),position);
	glm::mat4 rot = \
	glm::rotate(glm::mat4(1),camrot.x,glm::vec3(1.0,0,0)) *
	glm::rotate(glm::mat4(1),camrot.y,glm::vec3(0,1.0,0)) *
	glm::rotate(glm::mat4(1),camrot.z,glm::vec3(0,0,1.0));
	glm::mat4 scalem = glm::scale(glm::mat4(1),glm::vec3(scale,scale,scale));
	glm::mat4 model = trans/*rot*/*scale;
	glUniformMatrix4fv(spriteprg->getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	sprite.model->draw(spriteprg,sprite.textures, sprite.outframe, true, false);
}
