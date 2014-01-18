#include <iostream>
#include "FPSCamera.hpp"
#include "globals.hpp"
using namespace std;

float toDegrees(float rad){
	return rad*(180.0/PI);
}

float toRad(float degrees){
	return degrees*(PI/180.0);
}

FPSCamera::FPSCamera(float x, float y, float z){
	pos = glm::vec3(x,y,z);
	right = glm::vec3(1,0,0);
	lookat = glm::vec3(0,0,-1);
	up = glm::vec3(0,1,0);
}

void FPSCamera::move(float amount){
	pos += amount*lookat;
}

void FPSCamera::strafe(float amount){
	pos += amount*right;
}

void FPSCamera::turn(glm::vec2 amount){
	right = glm::cross(lookat,up);
	glm::mat4 turn = glm::rotate(glm::mat4(1),-amount.x,up) *
					 glm::rotate(glm::mat4(1),-amount.y,right);
	lookat = glm::mat3(turn)*lookat;
}


glm::mat4 FPSCamera::view(){
	return glm::lookAt(pos,lookat+pos,up);
}
