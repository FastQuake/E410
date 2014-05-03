#include <iostream>
#include <algorithm>
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>
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
	mass = 1;

	lookat = glm::vec3(1, 0, 0);
	right = glm::vec3(0, 0 ,1);

	rot = btQuaternion(0,0,0,1);

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
	rotation += glm::vec3(amount.y, -amount.x, 0);
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
void GameObject::createTriangleRidgidBody(){
	if(trimesh != NULL)
		delete trimesh;
	trimesh = new btTriangleMesh();
	for(int i=0;i<model->triangles.size();i++){
		int p1 = model->triangles[i].vertex[0];
		int p2 = model->triangles[i].vertex[1];
		int p3 = model->triangles[i].vertex[2];
		btVector3 v1(model->verts[p1].position[0],
				model->verts[p1].position[1],
				model->verts[p1].position[2]);
		btVector3 v2(model->verts[p2].position[0],
				model->verts[p2].position[1],
				model->verts[p2].position[2]);
		btVector3 v3(model->verts[p3].position[0],
				model->verts[p3].position[1],
				model->verts[p3].position[2]);

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
	btVector3 intertia;
	trimeshshape->calculateLocalInertia(mass, intertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,trimeshshape,intertia);
	body = new btRigidBody(ci);

	physworld.addBody(body);
}
void GameObject::createConvexRidgidBody(){
	btConvexHullShape  *o = new btConvexHullShape();

	for(int i=0;i<model->verts.size();i++){
		o->addPoint(btVector3(model->verts[i].position[0],
					model->verts[i].position[1],
					model->verts[i].position[2]),true);
	}
	o->recalcLocalAabb();

	btShapeHull *hull = new btShapeHull(o);
	btScalar margin = o->getMargin();
	hull->buildHull(margin);
	o->setUserPointer(hull);

	if(trimeshshape != NULL)
		delete trimeshshape;
	trimeshshape = new btConvexHullShape();
	btConvexHullShape *tmp = (btConvexHullShape *)trimeshshape; 
	for(int i=0;i<hull->numVertices();i++){
		tmp->addPoint(hull->getVertexPointer()[i],true);
	}
	tmp->recalcLocalAabb();
	if(motion != NULL)
		delete motion;
	motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
			btVector3(0,0,0)));

	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	btVector3 intertia;
	trimeshshape->calculateLocalInertia(mass, intertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,trimeshshape,intertia);
	body = new btRigidBody(ci);

	physworld.addBody(body);
}
void GameObject::createCubeRidgidBody(){
	float minx = model->verts[0].position[0];
	float miny = model->verts[0].position[1];
	float minz = model->verts[0].position[2];
	float maxx = minx;
	float maxy = miny;
	float maxz = minz;
	for(int i=0;i<model->verts.size();i++){
		minx = min(minx, model->verts[0].position[0]);
		maxx = max(maxx, model->verts[0].position[0]);
		miny = min(miny, model->verts[0].position[1]);
		maxy = max(maxy, model->verts[0].position[1]);
		minz = min(minz, model->verts[0].position[2]);
		maxz = max(maxz, model->verts[0].position[2]);
	}
}

void GameObject::updateMass(float mass){
	this->mass = btScalar(mass);
	btVector3 intertia;
	if(body != NULL){
		physworld.removeBody(body);
		body->getCollisionShape()->calculateLocalInertia(this->mass, intertia);
		body->setMassProps(this->mass, intertia);
		physworld.addBody(body);
	}
	else
		cout << "body is null" << endl;
}
