#include <iostream>
#include <cfloat>
#include "FPSCamera.hpp"
#include "../globals.hpp"
using namespace std;

float toDegrees(float rad){
	return rad*(180.0/PI);
}

float toRad(float degrees){
	return degrees*(PI/180.0);
}

FPSCamera::FPSCamera(float x, float y, float z){
	magic = CAMMAGIC;
	pos = glm::vec3(x,y,z);
	right = glm::vec3(1.0, 0.0, 0.0);
	lookat = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	angle = glm::vec3(0.0, 0.0, 0.0);
}

void FPSCamera::move(float amount){
	pos += amount*lookat;
}

void FPSCamera::strafe(float amount){
	pos += amount*glm::normalize(right);
}

void FPSCamera::turn(glm::vec2 amount){
	angle += glm::vec3(-amount,0.0);
	angle.x += 90;
	if(angle.x >= 179){
		angle.x = 179;
	} else if(angle.x <= 1){
		angle.x = 1;
	}
	if(angle.y > 360){
		angle.y = abs(angle.y)-360.0;
	} else if(angle.y < -360){
		angle.y = -(abs(angle.y)-360.0);
	}
	angle.x -= 90.0f;
	updateLookat();
}
void FPSCamera::updateLookat(){
	angle.x += 90.0f;
	lookat.x = sin(toRad(angle.x)) * cos(toRad(-angle.y));
	lookat.y = cos(toRad(angle.x));
	lookat.z = sin(toRad(angle.x)) * sin(toRad(-angle.y));
	right = glm::cross(lookat,up);
	angle.x -= 90;
}

glm::mat4 FPSCamera::view(){
	return glm::lookAt(pos,lookat+pos,up);
}

glm::vec3 FPSCamera::getLookat(){
	return lookat;
}
