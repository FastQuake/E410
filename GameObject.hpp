#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include <iostream>
#include <glm/glm.hpp>
#include "model.hpp"

class GameObject{
public:
	Model *model;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::string currentAnimation;
	std::vector<glm::mat4> outframe;
	float aTime;

	bool visible, animate, hasAnimation;

	GameObject();

	void setModel(Model *model);
};

#endif
