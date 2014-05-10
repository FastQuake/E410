#include <iostream>
#include "Sprite.hpp"
#include "../globals.hpp"
using namespace std;

#define SPRITE_FRAMERATE 10.0;

ShaderProgram *spriteprg;

Sprite::Sprite(std::string name):sprite(){
	texture = resman.loadTexture(name,&imgwidth,&imgheight);
	sprite.setModel(resman.loadModel("cube.iqm"));
	sprite.textures[0] = texture;
	position = glm::vec3(0,0,0);
	spritewidth = imgwidth;
	spriteheight = imgheight;
	scale = 1;
	visible = true;
	animate = false;
	cframe = 0;
	fcframe = 0;
	xframes = 1;
	yframes = 1;
	magic = SPRITE_MAGIC;
}

void Sprite::setFrames(int hframes, int vframes){
	spritewidth = imgwidth/hframes;
	spriteheight = imgheight/vframes;
}

void Sprite::draw(float dt, FPSCamera *cam){
	if(animate){
		fcframe += dt*SPRITE_FRAMERATE; 
		cframe = int(floor(fcframe));
		if(cframe > (xframes*yframes)+1){
			fcframe = 0;
			cframe = 0;
		}
	}
	//construct the matrix
	glm::mat4 trans = glm::translate(glm::mat4(1),position);
	glm::mat4 model = trans;
	//glUniformMatrix4fv(spriteprg->getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	float xpos = ((cframe)/xframes)+1;
	float ypos = ((cframe)%xframes)+1;
	GLfloat v[] = {scale,scale};
	GLfloat f[] = {(xpos*spritewidth)/imgwidth,(ypos*spriteheight)/imgheight,((xpos-1)*spritewidth)/imgwidth,((ypos-1)*spriteheight)/imgheight};
	GLfloat b[] = {position.x, position.y, position.z};
	glUniform2fv(spriteprg->getUniform("size"),1,v);
	glUniform4fv(spriteprg->getUniform("frame"),1,f);
	glUniform3fv(spriteprg->getUniform("pos_cen"),1,b);
	sprite.model->draw(spriteprg,sprite.textures, sprite.outframe, true, false);
}
