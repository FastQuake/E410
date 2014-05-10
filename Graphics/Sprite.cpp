#include <iostream>
#include "Sprite.hpp"
#include "../globals.hpp"
using namespace std;

ShaderProgram *spriteprg;

Sprite::Sprite(std::string name):sprite(){
	texture = resman.loadTexture(name,&imgwidth,&imgheight);
	sprite.setModel(resman.loadModel("cube.iqm"));
	sprite.textures[0] = texture;
	position = glm::vec3(0,0,0);
	scale = 1;
}

void Sprite::draw(float dt, FPSCamera *cam){
	//construct the matrix
	glm::mat4 trans = glm::translate(glm::mat4(1),position);
	glm::mat4 model = trans;
	//glUniformMatrix4fv(spriteprg->getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	GLfloat v[] = {scale,scale};
	GLfloat b[] = {position.x, position.y, position.z};
	glUniform2fv(spriteprg->getUniform("size"),1,v);
	glUniform3fv(spriteprg->getUniform("pos_cen"),1,b);
	sprite.model->draw(spriteprg,sprite.textures, sprite.outframe, true, false);
}
