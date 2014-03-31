#include "GameObject.hpp"
#include "globals.hpp"

GameObject::GameObject():
position(0,0,0), rotation(0,0,0), scale(1,1,1){

	magic = GOMAGIC;
	id = 0;
	tag = "None";
	moved = false;
	scaled = false;
	rotated = false;
	visible = true;
	animate = false;
	hasAnimation = false;
	aTime = 0;

	lookat = glm::vec3(1, 0, 0);
	right = glm::vec3(0, 0 ,1);
}

void GameObject::setModel(Model *model){
	this->model = model;
	outframe.resize(model->joints.size());
	if(model->animNames.size() > 0){
		currentAnimation = model->animNames[0];
		hasAnimation = true;
	}
}

void GameObject::move(float amount){
	position += amount*lookat;
}
void GameObject::strafe(float amount){
	position += amount*glm::normalize(right);
}
void GameObject::turn(glm::vec2 amount){
	rotation += glm::vec3(0, -amount.x, amount.y);
	rotation.z -= 90.0;
	/*if(rotation.y > 360){
		rotation.y = abs(rotation.y)-360.0;
	} else if(rotation.y < -360){
		rotation.y = abs(rotation.y)-360.0;
	}
	if(rotation.x > 360){
		rotation.x = abs(rotation.x)-360.0;
	} else if(rotation.x < -360){
		rotation.x = abs(rotation.x)-360.0;
	}*/
	lookat.x = sin(toRad(-rotation.z)) * cos(toRad(-rotation.y));
	lookat.y = cos(toRad(-rotation.z));
	lookat.z = sin(toRad(-rotation.z)) * sin(toRad(-rotation.y));
	right = glm::cross(lookat,glm::vec3(0,1,0));
	rotation.z += 90.0;
}
