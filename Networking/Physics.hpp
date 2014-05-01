#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class PhysWorld {
private:
	btDiscreteDynamicsWorld *dynWorld;
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *conf;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseContraintSolver *solver;
public:
	PhysWorld();
	~PhysWorld();
};

#endif
