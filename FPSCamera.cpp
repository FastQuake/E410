#include "FPSCamera.hpp"
#include "globals.hpp"

float toDegrees(float rad){
	return rad*(180.0/PI);
}

float toRad(float degrees){
	return degrees*(PI/180.0);
}

FPSCamera::FPSCamera(float x, float y, float z,float xangle, float yangle){
	pos = glm::vec3(-x,y,z);
	angle = glm::vec3(xangle,yangle,0);
}

void FPSCamera::move(float amount){
	pos.x -= sin(toRad(angle.x)) * amount;
	pos.z += cos(toRad(angle.x)) * amount;
	pos.y += sin(toRad(angle.y)) * amount;
}

void FPSCamera::strafe(float amount){
	pos.x -= cos(toRad(angle.x)) * amount;
	pos.z -= sin(toRad(angle.x)) * amount;	
}

void FPSCamera::turnX(float amount){
	angle.x += amount;
}

void FPSCamera::turnY(float amount){
	angle.y += amount;
	if(angle.y > 90){
		angle.y = 90;
	}else if(angle.y < -90){
		angle.y = -90;
	}
}

glm::mat4 FPSCamera::view(){
	glm::mat4 rot = \
			glm::rotate(glm::mat4(1.0f),angle.y,glm::vec3(1,0,0)) *
			glm::rotate(glm::mat4(1.0f),angle.x,glm::vec3(0,1,0)) *
			glm::rotate(glm::mat4(1.0f),angle.z,glm::vec3(0,0,1));

	glm::mat4 trans = glm::translate(glm::mat4(1.0f),pos);

	glm::mat4 out = rot * trans;
	return out;
}
