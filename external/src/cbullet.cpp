#include "cbullet.h"
#include <assert.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

struct DebugDraw : public btIDebugDraw {
    plDrawLineCallback draw_line_callback = nullptr;
    void* draw_line_user = nullptr;

    plErrorWarningCallback error_warning_callback = nullptr;

    int debug_mode = 0;

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        if (draw_line_callback) {
            const plVector3 p0 = { from.x(), from.y(), from.z() };
            const plVector3 p1 = { to.x(), to.y(), to.z() };
            const plVector3 c = { color.x(), color.y(), color.z() };
            draw_line_callback(p0, p1, c, draw_line_user);
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
        debug_mode = in_debug_mode;
    }

    virtual int getDebugMode() const override {
        return debug_mode;
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

void plWorldSetGravity(plWorldHandle handle, float gx, float gy, float gz) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    world->setGravity(btVector3(gx, gy, gz));
}

int plWorldStepSimulation(plWorldHandle handle, float time_step, int max_sub_steps, float fixed_time_step) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    return world->stepSimulation(time_step, max_sub_steps, fixed_time_step);
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

void plWorldDebugSetDrawLineCallback(plWorldHandle handle, plDrawLineCallback callback, void* user) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->draw_line_callback = callback;
    debug->draw_line_user = user;
}

void plWorldDebugSetErrorWarningCallback(plWorldHandle handle, plErrorWarningCallback callback) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->error_warning_callback = callback;
}

void plWorldDebugDraw(plWorldHandle handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    world->debugDrawWorld();
}

int plShapeGetType(plShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getShapeType();
}

plShapeHandle plShapeCreateBox(float half_x, float half_y, float half_z) {
    assert(half_x > 0.0 && half_y > 0.0 && half_z > 0.0);
    btBoxShape* box = new btBoxShape(btVector3(half_x, half_y, half_z));
    return (plShapeHandle)box;
}

plShapeHandle plShapeCreateSphere(float radius) {
    assert(radius > 0.0f);
    btSphereShape* sphere = new btSphereShape(radius);
    return (plShapeHandle)sphere;
}

plShapeHandle plShapeCreatePlane(float nx, float ny, float nz, float d) {
    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(nx, ny, nz), d);
    return (plShapeHandle)plane;
}

plShapeHandle plShapeCreateCapsule(float radius, float height, int up_axis) {
    assert(up_axis >= 0 && up_axis <= 2);
    assert(radius > 0.0 && height > 0);

    btCapsuleShape* capsule = nullptr;
    if (up_axis == 0) {
        capsule = new btCapsuleShapeX(radius, height);
    } else if (up_axis == 2) {
        capsule = new btCapsuleShapeZ(radius, height);
    } else {
        capsule = new btCapsuleShape(radius, height);
    }
    return (plShapeHandle)capsule;
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

    if (body->getMotionState()) {
        delete body->getMotionState();
    }
    world->removeRigidBody(body);
    delete body;
}

void plBodyGetGraphicsTransform(plBodyHandle handle, plVector3 transform[4]) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && body->getMotionState() && transform);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);

    const btMatrix3x3& basis = trans.getBasis();
    const btVector3& origin = trans.getOrigin();

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
