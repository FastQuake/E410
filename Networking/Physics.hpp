#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class PhysWorld {
private:
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *conf;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
public:
	btDiscreteDynamicsWorld *dynWorld;
	PhysWorld();
	~PhysWorld();

	void step(float dt);
	void addBody(btRigidBody* body);
	void removeBody(btRigidBody* body);
};

#endif
