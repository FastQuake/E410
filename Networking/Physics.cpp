#include "Physics.hpp"
#include "../globals.hpp"

GLDebugDrawer debugDraw;

glm::vec3 quatToEuler(glm::quat quat){
	glm::vec3 angle(0,0,0);
	angle.x = asin(2*quat.x*quat.y + 2*quat.z*quat.w);
	angle.y = atan2(2*quat.x*quat.w - 2*quat.y*quat.z,
					1 - 2*quat.x*quat.x - 2*quat.z*quat.z);
	angle.z = atan2(2*quat.y*quat.w - 2*quat.x*quat.z,
					1 - 2*quat.y*quat.y - 2*quat.z*quat.z);

	angle.x = toDegrees(angle.x);
	angle.y = toDegrees(angle.y);
	angle.z = toDegrees(angle.z);
	angle.x += 180.0f;
	return angle;
}

PhysWorld::PhysWorld(){
	//Initialize all the things that bullet needs to work
	broadphase = new btDbvtBroadphase();
	conf = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(conf);
	//btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);	
	solver = new btSequentialImpulseConstraintSolver();
	dynWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,conf);
	dynWorld->setGravity(btVector3(0,-9.81,0));
	debugDraw.setDebugMode(1);
	dynWorld->setDebugDrawer(&debugDraw);
}

PhysWorld::~PhysWorld(){
	delete dynWorld;
	delete solver;
	delete dispatcher;
	delete conf;
	delete broadphase;
}

void PhysWorld::step(float dt){
	dynWorld->stepSimulation(dt,0,0);
}

void PhysWorld::addBody(btRigidBody *body){
	dynWorld->addRigidBody(body);
}

void PhysWorld::removeBody(btRigidBody *body){
	dynWorld->removeRigidBody(body);
}
