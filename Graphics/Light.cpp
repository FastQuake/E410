#include "Light.hpp"
#include "../globals.hpp"

glm::mat4 DLight::mvp(){
	//TODO: Compute projection based on the camera frustum and light rotation, instead of using lookAt
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
		out = glm::lookAt(glm::vec3(pos.x+1.0,pos.y,pos.z),pos,glm::vec3(0.0f,1.0f,0.0f));
		break;
	case 1:
		out = glm::lookAt(glm::vec3(pos.x-1.0,pos.y,pos.z),pos,glm::vec3(0.0f,1.0f,0.0f));
		break;
	case 2:
		out = glm::lookAt(glm::vec3(pos.x,pos.y+1.0,pos.z),pos,glm::vec3(0.0f,0.0f,-1.0f));
		break;
	case 3:
		out = glm::lookAt(glm::vec3(pos.x,pos.y-1.0,pos.z),pos,glm::vec3(0.0f,0.0f,1.0f));
		break;
	case 4:
		out = glm::lookAt(glm::vec3(pos.x,pos.y,pos.z+1.0),pos,glm::vec3(0.0f,1.0f,0.0f));
		break;
	case 5:
		out = glm::lookAt(glm::vec3(pos.x,pos.y,pos.z-1.0),pos,glm::vec3(0.0f,1.0f,0.0f));
		break;
	default:
		break;
	}
	glm::mat4 projection = glm::perspective(90.0f, 1.0f, 0.1f, 1000.0f);
	//glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f, -10.0f ,50.0f);
	return projection*out;
}

glm::mat4 PLight::mvp(){
	return this->mvp(1);
}

DLight::DLight(){
	this->type = DIRECTIONAL_LIGHT;
}

PLight::PLight(){
	this->type = POINT_LIGHT;
}
