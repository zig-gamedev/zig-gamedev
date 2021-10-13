#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

extern "C" void* cbtCreateDynamicsWorld(void) {
    btDefaultCollisionConfiguration* collision_config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_config);
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
    return (void*)world;
}

extern "C" void cbtDeleteDynamicsWorld(void* world_handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;

    btCollisionDispatcher* dispatcher = (btCollisionDispatcher*)world->getDispatcher();
    delete dispatcher->getCollisionConfiguration();
    delete dispatcher;

    delete world->getBroadphase();
    delete world->getConstraintSolver();
    delete world;
}
