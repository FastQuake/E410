#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include <iostream>
#include "model.hpp"

class GameObject{
public:
	Model model;
	std::string modelName;
	float x,y,z;
};

#endif
