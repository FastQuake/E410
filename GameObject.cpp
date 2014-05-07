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
	textures = model->textureIDS;
}

void GameObject::move(float amount){
	position += amount*lookat;
}
void GameObject::strafe(float amount){
	position += amount*glm::normalize(right);
}
void GameObject::turn(glm::vec2 amount){
	rotation += glm::vec3(amount.x, amount.y, 0);
	updateLookat();
}
void GameObject::updateLookat(){
	rotation.x += 90.0;
	lookat.x = sin(toRad(rotation.x)) * cos(toRad(-rotation.y));
	lookat.y = cos(toRad(rotation.x));
	lookat.z = sin(toRad(rotation.x)) * sin(toRad(-rotation.y));
	right = glm::cross(lookat,glm::vec3(0,1,0));
	rotation.x -= 90.0;
}
