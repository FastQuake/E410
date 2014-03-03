#include "GameObject.hpp"

GameObject::GameObject():
position(0,0,0), rotation(0,0,0), scale(1,1,1){

	visible = true;
	animate = false;
	aTime = 0;
}

void GameObject::setModel(Model *model){
		this->model = model;
		currentAnimation = model->animNames[0];
}
