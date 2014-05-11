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
	castShadow = true;
	animate = false;
	hasAnimation = false;
	aTime = 0;
	mass = 1;

	lookat = glm::vec3(1, 0, 0);
	right = glm::vec3(0, 0 ,1);

	rot = btQuaternion(0,0,0,1);

	body = NULL;
	trimesh = NULL;
	collisionshape = NULL;
	motion = NULL;
}

GameObject::~GameObject(){
	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	if(trimesh != NULL)
		delete trimesh;
	if(collisionshape != NULL)
		delete collisionshape;
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
	textures = model->textures;
	float oox = (model->e.maxx+model->e.minx)/2.0;
	float ooy = (model->e.maxy+model->e.miny)/2.0;
	float ooz = (model->e.maxz+model->e.minz)/2.0;

	originOffset.setX(oox);
	originOffset.setY(ooy);
	originOffset.setZ(ooz);
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
void GameObject::createTriangleRigidBody(){
	if(trimesh != NULL)
		delete trimesh;
	trimesh = new btTriangleMesh();
	//we need to rotate the triangles -90 on the x axis to fix the orientation
	glm::mat4 rot = glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1,0,0));
	for(int i=0;i<model->triangles.size();i++){
		int p1 = model->triangles[i].vertex[0];
		int p2 = model->triangles[i].vertex[1];
		int p3 = model->triangles[i].vertex[2];
		glm::vec4 v1(model->verts[p1].position[0],
				model->verts[p1].position[1],
				model->verts[p1].position[2],1);
		glm::vec4 v2(model->verts[p2].position[0],
				model->verts[p2].position[1],
				model->verts[p2].position[2],1);
		glm::vec4 v3(model->verts[p3].position[0],
				model->verts[p3].position[1],
				model->verts[p3].position[2],1);

		v1 = rot*v1;
		v2 = rot*v2;
		v3 = rot*v3;

		btVector3 bV1(v1.x,v1.y,v1.z);
		btVector3 bV2(v2.x,v2.y,v2.z);
		btVector3 bV3(v3.x,v3.y,v3.z);

		trimesh->addTriangle(bV1,bV2,bV3);

	}

	if(collisionshape != NULL)
		delete collisionshape;
	btGImpactMeshShape *gimpact = new btGImpactMeshShape(trimesh);
	gimpact->updateBound();
	collisionshape = gimpact;
	if(motion != NULL)
		delete motion;
	motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
			btVector3(0,0,0)));

	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	btVector3 inertia;
	collisionshape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,collisionshape,inertia);
	body = new btRigidBody(ci);

	physworld.addBody(body);
}
void GameObject::createConvexRigidBody(){
	btConvexHullShape  *o = new btConvexHullShape();
	glm::mat4 rot = glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1,0,0));
	for(int i=0;i<model->triangles.size();i++){
		int p1 = model->triangles[i].vertex[0];
		int p2 = model->triangles[i].vertex[1];
		int p3 = model->triangles[i].vertex[2];
		glm::vec4 v1(model->verts[p1].position[0],
				model->verts[p1].position[1],
				model->verts[p1].position[2],1);
		glm::vec4 v2(model->verts[p2].position[0],
				model->verts[p2].position[1],
				model->verts[p2].position[2],1);
		glm::vec4 v3(model->verts[p3].position[0],
				model->verts[p3].position[1],
				model->verts[p3].position[2],1);

		v1 = rot*v1;
		v2 = rot*v2;
		v3 = rot*v3;

		btVector3 bV1(v1.x,v1.y,v1.z);
		btVector3 bV2(v2.x,v2.y,v2.z);
		btVector3 bV3(v3.x,v3.y,v3.z);

		o->addPoint(btVector3(v1.x,v1.y,v1.z),false);
		o->addPoint(btVector3(v2.x,v2.y,v2.z),false);
		o->addPoint(btVector3(v3.x,v3.y,v3.z),true);

	}
	o->recalcLocalAabb();

	btShapeHull *hull = new btShapeHull(o);
	btScalar margin = o->getMargin();
	hull->buildHull(margin);
	o->setUserPointer(hull);

	if(collisionshape != NULL)
		delete collisionshape;
	collisionshape = new btConvexHullShape();
	btConvexHullShape *tmp = (btConvexHullShape *)collisionshape;
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
	btVector3 inertia;
	collisionshape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,collisionshape,inertia);
	body = new btRigidBody(ci);

	physworld.addBody(body);
	delete hull;
	delete o;
}
void GameObject::createCubeRigidBody(){
	float xsize,ysize,zsize;
	xsize =	model->e.maxx-model->e.minx;
	ysize = model->e.maxy-model->e.miny;
	zsize = model->e.maxz-model->e.minz;
	btVector3 boxVector(xsize/2.0,ysize/2.0,zsize/2.0);

	if(collisionshape != NULL)
		delete collisionshape;
	collisionshape = new btBoxShape(boxVector);

	if(motion != NULL)
		delete motion;
	motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));

	btVector3 inertia;
	collisionshape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,collisionshape,inertia);
	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	body = new btRigidBody(ci);
	if(this->tag.substr(0,4) == "node"){
		physworld.addBody(body,1<<2,1);
	}
	else
		physworld.addBody(body,1<<2,1<<2);
}
void GameObject::createCubeRigidBody(extents e){
	float xsize,ysize,zsize;
	xsize = e.maxx-e.minx;
	ysize = e.maxy-e.miny;
	zsize = e.maxz-e.minz;
	btVector3 boxVector(xsize/2.0,ysize/2.0,zsize/2.0);

	if(collisionshape != NULL)
		delete collisionshape;
	collisionshape = new btBoxShape(boxVector);

	if(motion != NULL)
		delete motion;
	motion = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));

	btVector3 inertia;
	collisionshape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass,motion,collisionshape,inertia);
	if(body != NULL){
		physworld.removeBody(body);
		delete body;
	}
	body = new btRigidBody(ci);
	if(this->tag.substr(0,4) == "node"){
		physworld.addBody(body,1<<2,1);
	}
	else
		physworld.addBody(body,1<<2,1<<2);

	float oox = (e.maxx+e.minx)/2.0;
	float ooy = (e.maxy+e.miny)/2.0;
	float ooz = (e.maxz+e.minz)/2.0;

	originOffset.setX(oox);
	originOffset.setY(ooy);
	originOffset.setZ(ooz);
}

void GameObject::updateMass(float mass){
	this->mass = btScalar(mass);
	btVector3 inertia;
	if(body != NULL){
		physworld.removeBody(body);
		body->getCollisionShape()->calculateLocalInertia(this->mass, inertia);
		body->setMassProps(this->mass, inertia);
		if(this->tag.substr(0,4) == "node"){
			physworld.addBody(body,1,1);
		}
		else
			physworld.addBody(body,1<<2,1<<2);
	}
	else
		cout << "body is null" << endl;
}
