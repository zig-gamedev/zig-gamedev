#include "cbullet.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

struct DebugDraw : public btIDebugDraw {
    plDrawLineCallback draw_line_callback = nullptr;
    plErrorWarningCallback error_warning_callback = nullptr;

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        if (draw_line_callback) {
            plVector3 p0 = { from.x(), from.y(), from.z() };
            plVector3 p1 = { to.x(), to.y(), to.z() };
            plVector3 c = { color.x(), color.y(), color.z() };
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

void plWorldDestroy(plWorldHandle world_handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
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

void plWorldDebugSetDrawLineCallback(plWorldHandle world_handle, plDrawLineCallback callback) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->draw_line_callback = callback;
}

void plWorldDebugSetErrorWarningCallback(plWorldHandle world_handle, plErrorWarningCallback callback) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    assert(world);

    DebugDraw* debug = getDebug(world);
    debug->error_warning_callback = callback;
}
