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
	textures = model->textureIDS;
	extents extents = getExtents();
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
	rotation.x += 90.0;
	lookat.x = sin(toRad(rotation.x)) * cos(toRad(rotation.y));
	lookat.y = cos(toRad(rotation.x));
	lookat.z = sin(toRad(rotation.x)) * sin(toRad(rotation.y));
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
	collisionshape = new btBvhTriangleMeshShape(trimesh,true);
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
		o->addPoint(btVector3(v3.x,v3.y,v3.z),false);

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
		tmp->addPoint(hull->getVertexPointer()[i],false);
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
	extents extents = getExtents();
	float xsize,ysize,zsize;
	xsize = extents.maxx-extents.minx;
	ysize = extents.maxy-extents.miny;
	zsize = extents.maxz-extents.minz;
	btVector3 boxVector(xsize/2.0,ysize/2.0,zsize/2.0);

	originOffset.setX((extents.maxx+extents.minx)/2.0);
	originOffset.setY((extents.maxy+extents.miny)/2.0);
	originOffset.setZ((extents.maxz+extents.minz)/2.0);

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
	physworld.addBody(body);
}

void GameObject::updateMass(float mass){
	this->mass = btScalar(mass);
	btVector3 inertia;
	if(body != NULL){
		physworld.removeBody(body);
		body->getCollisionShape()->calculateLocalInertia(this->mass, inertia);
		body->setMassProps(this->mass, inertia);
		physworld.addBody(body);
	}
	else
		cout << "body is null" << endl;
}

extents GameObject::getExtents(){
	extents out;
	glm::mat4 rot = glm::rotate(glm::mat4(1),-90.0f,glm::vec3(1,0,0));
	glm::vec4 p1(model->verts[0].position[0],
			model->verts[0].position[1],
			model->verts[0].position[2],1.0);
	p1 = rot*p1;
	out.minx = p1.x;
	out.miny = p1.y;
	out.minz = p1.z;
	out.maxx = out.minx;
	out.maxy = out.miny;
	out.maxz = out.minz;
	for(int i=0;i<model->verts.size();i++){
		glm::vec4 p2(model->verts[i].position[0],
			model->verts[i].position[1],
			model->verts[i].position[2],1.0);
		p2 = rot * p2;
		out.minx = min(out.minx, p2.x);
		out.maxx = max(out.maxx, p2.x);
		out.miny = min(out.miny, p2.y);
		out.maxy = max(out.maxy, p2.y);
		out.minz = min(out.minz, p2.z);
		out.maxz = max(out.maxz, p2.z);
	}
	return out;
}
