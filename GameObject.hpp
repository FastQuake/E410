#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "Graphics/model.hpp"
#include "stdint.h"

#define GOMAGIC "Game"

class GameObject{
public:
	std::string magic;
	std::string modelName;
	std::string tag;
	uint32_t id;	
	bool moved;
	bool scaled;
	bool rotated;
	Model *model;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 lookat;
	glm::vec3 right;
	std::string currentAnimation;
	std::vector<glm::mat4> outframe;
	std::vector<GLuint> textures;
	float aTime;

	//Don't forget to delete pointers
	btScalar mass;
	btRigidBody *body;
	btMotionState *motion;

	bool visible, animate, hasAnimation;

	GameObject();

	void setModel(Model *model);
	void move(float amount);
	void strafe(float amount);
	void turn(glm::vec2 amount);
	void updateLookat();

	void createRidgidBody();
};

#endif
