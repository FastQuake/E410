#include "Light.hpp"
#include "../globals.hpp"

glm::mat4 PLight::pointProjection = glm::perspective(90.0f, 1.0f, 1.0f, 75.0f);

glm::mat4 DLight::mvp(){
	glm::vec3 p;
	p.x = sin(toRad(-rot.x+90.0f)) * cos(toRad(rot.y));
	p.y = cos(toRad(-rot.x+90.0f));
	p.z = sin(toRad(-rot.x+90.0f)) * sin(toRad(rot.y));
	//glm::mat4 lookat = glm::lookAt(pos,pos+p,glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 lookat = glm::lookAt(pos,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

	/*glm::mat4 lookat;
	glm::mat4 rotmat = \
		glm::rotate(glm::mat4(1),rot.x, glm::vec3(1,0,0)) *
		glm::rotate(glm::mat4(1),rot.y+90.0f, glm::vec3(0,1,0)) *
		glm::rotate(glm::mat4(1),rot.z, glm::vec3(0,0,1));
	
	glm::mat4 trans = glm::translate(glm::mat4(1), pos);
	lookat = trans*rotmat;
	*/
	glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f,-10.0f,50.0f);
	return projection*lookat;
}

glm::mat4 PLight::mvp(int face){
	glm::mat4 out;
	switch(face){
	case 0:
		out = glm::lookAt(pos,glm::vec3(pos.x+0.1,pos.y,pos.z),glm::vec3(0.0f,-1.0f,0.0f));
		break;
	case 1:
		out = glm::lookAt(pos,glm::vec3(pos.x-0.1,pos.y,pos.z),glm::vec3(0.0f,-1.0f,0.0f));
		break;
	case 2:
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y+0.1,pos.z),glm::vec3(0.0f,0.0f,1.0f));
		break;
	case 3:
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y-0.1,pos.z),glm::vec3(0.0f,0.0f,-1.0f));
		break;
	case 4:
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y,pos.z+0.1),glm::vec3(0.0f,-1.0f,0.0f));
		break;
	case 5:
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y,pos.z-0.1),glm::vec3(0.0f,-1.0f,0.0f));
		break;
	default:
		break;
	}
	//glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f, -10.0 ,10.0f);
	return pointProjection*out;
}

glm::mat4 PLight::mvp(){
	glm::mat4 biasInverse = glm::mat4{
			2.0, 0.0, 0.0, 0.0,
			0.0, 2.0, 0.0, 0.0,
			0.0, 0.0, 2.0, 0.0,
			-1.0, -1.0, -1.0, 1.0
	};
	return biasInverse*glm::translate(glm::mat4(1.0f),-pos);
}

DLight::DLight(){
	this->type = DIRECTIONAL_LIGHT;
}

PLight::PLight(){
	this->type = POINT_LIGHT;
}
