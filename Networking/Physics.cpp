#include "Physics.hpp"

PhysWorld::PhysWorld(){
	//Initialize all the things that bullet needs to work
	broadphase = new btDbvtBroadphase();
	conf = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(conf);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);	
	solver = new btSequentialImpulseConstraintSolver();
	dynWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,conf);
	dynWorld->setGravity(btVector3(0,-9.81,0));
}

PhysWorld::~PhysWorld(){
}
