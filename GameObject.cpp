#include "GameObject.hpp"

GameObject::GameObject():
position(0,0,0), rotation(0,0,0), scale(1,1,1){

	visible = true;
	animate = false;
	hasAnimation = false;
	aTime = 0;
}

void GameObject::setModel(Model *model){
	this->model = model;
	outframe.resize(model->joints.size());
	if(model->animNames.size() > 0){
		currentAnimation = model->animNames[0];
		hasAnimation = true;
	}
}
