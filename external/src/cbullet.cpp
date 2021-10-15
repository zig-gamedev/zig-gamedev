#include "cbullet.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

struct DebugDraw : public btIDebugDraw {
    plDrawLineCallback draw_line_callback = nullptr;
    plErrorWarningCallback error_warning_callback = nullptr;

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        if (draw_line_callback) {
            const plVector3 p0 = { from.x(), from.y(), from.z() };
            const plVector3 p1 = { to.x(), to.y(), to.z() };
            const plVector3 c = { color.x(), color.y(), color.z() };
            draw_line_callback(p0, p1, c);
        }
    }

    virtual void drawContactPoint(
        const btVector3&,
        const btVector3&,
        btScalar,
        int,
        const btVector3&
    ) override {
    }

    virtual void reportErrorWarning(const char* warning_string) override {
        if (error_warning_callback && warning_string) {
            error_warning_callback(warning_string);
        }
    }

    virtual void draw3dText(const btVector3&, const char*) override {
    }

    virtual void setDebugMode(int in_debug_mode) override {
    }

    virtual int getDebugMode() const override {
        return 0;
    }
};

plWorldHandle plWorldCreate(void) {
    btDefaultCollisionConfiguration* collision_config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_config);
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
    return (plWorldHandle)world;
}

void plWorldDestroy(plWorldHandle handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    btCollisionDispatcher* dispatcher = (btCollisionDispatcher*)world->getDispatcher();
    delete dispatcher->getCollisionConfiguration();
    delete dispatcher;

    delete world->getBroadphase();
    delete world->getConstraintSolver();
    delete world;
}

static DebugDraw* getDebug(btDiscreteDynamicsWorld* world) {
    assert(world);
    DebugDraw* debug = (DebugDraw*)world->getDebugDrawer();
    if (debug == nullptr) {
        debug = new DebugDraw();
        debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawFrames);
        world->setDebugDrawer(debug);
    }
    assert(debug);
    return debug;
}

void plWorldDebugSetDrawLineCallback(plWorldHandle handle, plDrawLineCallback callback) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->draw_line_callback = callback;
}

void plWorldDebugSetErrorWarningCallback(plWorldHandle handle, plErrorWarningCallback callback) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->error_warning_callback = callback;
}

int plShapeGetType(plShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getShapeType();
}

plShapeHandle plShapeCreateBox(const plVector3 half_extents) {
    assert(half_extents);
    btBoxShape* box = new btBoxShape(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    return (plShapeHandle)box;
}

plShapeHandle plShapeCreateSphere(float radius) {
    assert(radius > 0.0f);
    btSphereShape* sphere = new btSphereShape(radius);
    return (plShapeHandle)sphere;
}

void plShapeDestroy(plShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    delete shape;
}

plBodyHandle plBodyCreate(
    plWorldHandle world_handle,
    float mass,
    const plVector3 transform[4],
    plShapeHandle shape_handle
) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btCollisionShape* shape = (btCollisionShape*)shape_handle;
    assert(world && shape && transform && mass >= 0.0f);

    const bool is_dynamic = (mass != 0.0f);

    btVector3 local_inertia(0.0f, 0.0f, 0.0f);
    if (is_dynamic)
        shape->calculateLocalInertia(mass, local_inertia);

    btDefaultMotionState* motion_state = new btDefaultMotionState(
        btTransform(
            btMatrix3x3(
                btVector3(transform[0][0], transform[0][1], transform[0][2]),
                btVector3(transform[1][0], transform[1][1], transform[1][2]),
                btVector3(transform[2][0], transform[2][1], transform[2][2])
            ),
            btVector3(transform[3][0], transform[3][1], transform[3][2])
        )
    );

    btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, local_inertia);
    btRigidBody* body = new btRigidBody(info);
    body->setUserIndex(-1);
    world->addRigidBody(body);

    return (plBodyHandle)body;
}

void plBodyDestroy(plWorldHandle world_handle, plBodyHandle body_handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = (btRigidBody*)body_handle;
    assert(world && body);

    world->removeRigidBody(body);
    delete body;
}

void plBodyGetGraphicsTransform(plBodyHandle handle, plVector3 transform[4]) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && transform);

    btDefaultMotionState* ms = (btDefaultMotionState*)body->getMotionState();
    const btMatrix3x3& basis = ms->m_graphicsWorldTrans.getBasis();
    const btVector3& origin = ms->m_graphicsWorldTrans.getOrigin();

    transform[0][0] = basis.getRow(0).x();
    transform[0][1] = basis.getRow(0).y();
    transform[0][2] = basis.getRow(0).z();
    transform[1][0] = basis.getRow(1).x();
    transform[1][1] = basis.getRow(1).y();
    transform[1][2] = basis.getRow(1).z();
    transform[2][0] = basis.getRow(2).x();
    transform[2][1] = basis.getRow(2).y();
    transform[2][2] = basis.getRow(2).z();

    transform[3][0] = origin.x();
    transform[3][1] = origin.y();
    transform[3][2] = origin.z();
}
