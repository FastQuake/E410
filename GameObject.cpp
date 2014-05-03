#include <iostream>
#include "Networking/server.hpp"
#include "GameObject.hpp"
#include "globals.hpp"
using namespace std;

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
	mass = 0;

	lookat = glm::vec3(1, 0, 0);
	right = glm::vec3(0, 0 ,1);

	body = NULL;
	trimesh = NULL;
	trimeshshape = NULL;
	motion = NULL;
}

GameObject::~GameObject(){
	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	if(trimesh != NULL)
		delete trimesh;
	if(trimeshshape != NULL)
		delete trimeshshape;
	if(motion != NULL)
		delete motion;
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
	rotation += glm::vec3(0, -amount.x, amount.y);
	updateLookat();
}
void GameObject::updateLookat(){
	rotation.z -= 90.0;
	lookat.x = sin(toRad(-rotation.z)) * cos(toRad(-rotation.y));
	lookat.y = cos(toRad(-rotation.z));
	lookat.z = sin(toRad(-rotation.z)) * sin(toRad(-rotation.y));
	right = glm::cross(lookat,glm::vec3(0,1,0));
	rotation.z += 90.0;
}

void GameObject::createRidgidBody(){
	if(trimesh != NULL)
		delete trimesh;
	trimesh = new btTriangleMesh();

	for(int i=0;i<model->verts.size();i+=3){
		btVector3 v1(model->verts[i].position[0],
				model->verts[i].position[1],
				model->verts[i].position[2]);
		btVector3 v2(model->verts[i+1].position[0],
				model->verts[i+1].position[1],
				model->verts[i+1].position[2]);
		btVector3 v3(model->verts[i+2].position[0],
				model->verts[i+2].position[1],
				model->verts[i+2].position[2]);

		trimesh->addTriangle(v1,v2,v3);

	}

	if(trimeshshape != NULL)
		delete trimeshshape;
	trimeshshape = new btBvhTriangleMeshShape(trimesh,true);
	if(motion != NULL)
		delete motion;
	motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
			btVector3(0,0,0)));

	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	body = new btRigidBody(1,motion,trimeshshape,btVector3(0,0,0));

	physworld.addBody(body);
}

void GameObject::updateMass(float mass){
	this->mass = btScalar(mass);
	btVector3 intertia(0,0,0);
	trimeshshape->calculateLocalInertia(mass, intertia);
	if(body != NULL)
		body->setMassProps(this->mass, intertia);
	else
		cout << "body is null" << endl;
}
