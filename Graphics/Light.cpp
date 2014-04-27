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
	glm::mat4 out = glm::lookAt(pos,glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
//	out = glm::mat4(1.0f);
	switch(face){
	case 4: //Right
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y,pos.z+0.1),glm::vec3(0.0f,1.0f,0.0f));
	//	out = glm::rotate(out,90.0f,glm::vec3(0,1,0));
		break;
	case 5: //Left
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y,pos.z-0.1),glm::vec3(0.0f,1.0f,0.0f));
	//	out = glm::rotate(out,-90.0f,glm::vec3(0,1,0));
		break;
	case 2: //Up
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y+0.1,pos.z),glm::vec3(0.0f,0.0f,1.0f));
	//	out = glm::rotate(out,-90.0f,glm::vec3(0,1,0));
	//	out = glm::rotate(out,-90.0f,glm::vec3(1,0,0));
		break;
	case 3: //Down
		out = glm::lookAt(pos,glm::vec3(pos.x,pos.y-0.1,pos.z),glm::vec3(0.0f,0.0f,-1.0f));
	//	out = glm::rotate(out,-90.0f,glm::vec3(0,1,0));
	//	out = glm::rotate(out,90.0f,glm::vec3(1,0,0));
		break;
	case 1: //Forward
		out = glm::lookAt(pos,glm::vec3(pos.x-0.1,pos.y,pos.z),glm::vec3(0.0f,1.0f,0.0f));
	//	out = glm::rotate(out,-90.0f,glm::vec3(0,1,0));
		break;
	case 0: //Backward
		out = glm::lookAt(pos,glm::vec3(pos.x+0.1,pos.y,pos.z),glm::vec3(0.0f,1.0f,0.0f));
	//	out = glm::rotate(out,180.0f,glm::vec3(0,1,0));
		break;
	default:
		break;
	}
	glm::mat4 projection = glm::perspective(90.0f, 1.0f, 0.6f, 50.0f);
	//glm::mat4 projection = glm::ortho<float>(-10.0f,10.0f,-10.0f,10.0f, -10.0 ,10.0f);
	return projection*out;
}

glm::mat4 PLight::mvp(){
	return glm::perspective(90.0f, 1.0f, 0.6f, 50.0f)*glm::translate(glm::mat4(1.0f),-pos);
}

DLight::DLight(){
	this->type = DIRECTIONAL_LIGHT;
}

PLight::PLight(){
	this->type = POINT_LIGHT;
}
