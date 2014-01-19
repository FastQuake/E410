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
	right = glm::vec3(1.0,0.0,0.0);
	lookat = glm::vec3(0.0,0.0,-1.0);
	up = glm::vec3(0.0,1.0,0.0);
}

void FPSCamera::move(float amount){
	pos += amount*lookat;
}

void FPSCamera::strafe(float amount){
	pos += amount*glm::normalize(right);
}

void FPSCamera::turn(glm::vec2 amount){
	//FIXME: Camera flips when moved all the way up or down
	right = glm::cross(lookat,up);
	glm::mat4 turn = glm::rotate(glm::mat4(1.0),-amount.x,up) *
					 glm::rotate(glm::mat4(1.0),-amount.y,right);

	//Angle between new lookat and up; if this is less than 2 or greater than 358, then do not apply the change
	float vecAngle = acos(glm::dot(glm::mat3(turn)*lookat,up));
	if(vecAngle != vecAngle || vecAngle <= PI/90.0 || vecAngle >= PI-PI/90.0){
			lookat = glm::mat3(glm::rotate(glm::mat4(1.0),-amount.x,up))*lookat; //Do apply the yaw change, however
		}else{
			lookat = glm::mat3(turn)*lookat;
			cout << lookat.x << "," << lookat.y << "," << lookat.z << endl;
		}
}


glm::mat4 FPSCamera::view(){
	return glm::lookAt(pos,lookat+pos,up);
}
