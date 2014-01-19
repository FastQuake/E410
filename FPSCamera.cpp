#include <iostream>
#include <cfloat>
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
	right = glm::vec3(1.0, 0.0, 0.0);
	lookat = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	angle = glm::vec3(0.0, 90.0, 0.0);
}

void FPSCamera::move(float amount){
	pos += amount*lookat;
}

void FPSCamera::strafe(float amount){
	pos += amount*glm::normalize(right);
}

void FPSCamera::turn(glm::vec2 amount){
	angle += glm::vec3(amount,0.0);
	if(angle.y >= 180){
		angle.y = 180-1;
	} else if(angle.y <= 0){
		angle.y = 0+1;
	}
	if(angle.x > 360){
		angle.x = 0;
	} else if(angle.x < -360){
		angle.x = 0;
	}
	lookat.x = sin(toRad(angle.y)) * cos(toRad(angle.x));
	lookat.y = cos(toRad(angle.y));
	lookat.z = sin(toRad(angle.y)) * sin(toRad(angle.x));
	right = glm::cross(lookat,up);
}


glm::mat4 FPSCamera::view(){
	return glm::lookAt(pos,lookat+pos,up);
}
