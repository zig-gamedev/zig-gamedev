#include "cbullet.h"
#include <assert.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"

struct CbtDebugDraw : public btIDebugDraw {
    CbtDebugDrawCallbacks callbacks = {};
    int debug_mode = 0;

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        if (callbacks.drawLine) {
            const CbtVector3 p0 = { from.x(), from.y(), from.z() };
            const CbtVector3 p1 = { to.x(), to.y(), to.z() };
            const CbtVector3 c = { color.x(), color.y(), color.z() };
            callbacks.drawLine(p0, p1, c, callbacks.user_data);
        }
    }

    virtual void drawContactPoint(
        const btVector3& point,
        const btVector3& normal,
        btScalar distance,
        int life_time,
        const btVector3& color
    ) override {
        if (callbacks.drawContactPoint) {
            const CbtVector3 p = { point.x(), point.y(), point.z() };
            const CbtVector3 n = { normal.x(), normal.y(), normal.z() };
            const CbtVector3 c = { color.x(), color.y(), color.z() };
            callbacks.drawContactPoint(p, n, distance, life_time, c, callbacks.user_data);
        }
    }

    virtual void reportErrorWarning(const char* warning_string) override {
        if (callbacks.reportErrorWarning && warning_string) {
            callbacks.reportErrorWarning(warning_string, callbacks.user_data);
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

CbtWorldHandle cbtWorldCreate(void) {
    btDefaultCollisionConfiguration* collision_config = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_config);
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
    return (CbtWorldHandle)world;
}

void cbtWorldDestroy(CbtWorldHandle handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);

    if (world->getDebugDrawer()) {
        delete world->getDebugDrawer();
    }

    btCollisionDispatcher* dispatcher = (btCollisionDispatcher*)world->getDispatcher();
    delete dispatcher->getCollisionConfiguration();
    delete dispatcher;

    delete world->getBroadphase();
    delete world->getConstraintSolver();
    delete world;
}

void cbtWorldSetGravity(CbtWorldHandle handle, const CbtVector3 gravity) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    world->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));
}

int cbtWorldStepSimulation(CbtWorldHandle handle, float time_step, int max_sub_steps, float fixed_time_step) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    return world->stepSimulation(time_step, max_sub_steps, fixed_time_step);
}

int cbtRayTestClosest(
    CbtWorldHandle handle,
    const CbtVector3 ray_from_world,
    const CbtVector3 ray_to_world,
    int collision_filter_group,
    int collision_filter_mask,
    unsigned int flags,
    CbtRayCastResult* result
) {
    assert(handle && result);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;

    const btVector3 from(ray_from_world[0], ray_from_world[1], ray_from_world[2]);
    const btVector3 to(ray_to_world[0], ray_to_world[1], ray_to_world[2]);

    btCollisionWorld::ClosestRayResultCallback closest(from, to);
    closest.m_collisionFilterGroup = collision_filter_group;
    closest.m_collisionFilterMask = collision_filter_mask;
    closest.m_flags = flags;

    world->rayTest(from, to, closest);

    result->hit_normal_world[0] = closest.m_hitNormalWorld.x();
    result->hit_normal_world[1] = closest.m_hitNormalWorld.y();
    result->hit_normal_world[2] = closest.m_hitNormalWorld.z();
    result->hit_point_world[0] = closest.m_hitPointWorld.x();
    result->hit_point_world[1] = closest.m_hitPointWorld.y();
    result->hit_point_world[2] = closest.m_hitPointWorld.z();
    result->hit_fraction = closest.m_closestHitFraction;
    result->body = (CbtBodyHandle)closest.m_collisionObject;
    return closest.m_collisionObject != 0;
}

void cbtWorldDebugSetCallbacks(CbtWorldHandle handle, const CbtDebugDrawCallbacks* callbacks) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world && callbacks);

    CbtDebugDraw* debug = (CbtDebugDraw*)world->getDebugDrawer();
    if (debug == nullptr) {
        debug = new CbtDebugDraw();
        debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawFrames);
        world->setDebugDrawer(debug);
    }

    debug->callbacks = *callbacks;
}

void cbtWorldDebugDraw(CbtWorldHandle handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world);
    world->debugDrawWorld();
}

void cbtWorldDebugDrawLine(CbtWorldHandle handle, const CbtVector3 p0, const CbtVector3 p1, const CbtVector3 color) {
    assert(p0 && p1 && color);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world && world->getDebugDrawer());

    world->getDebugDrawer()->drawLine(
        btVector3(p0[0], p0[1], p0[2]),
        btVector3(p1[0], p1[1], p1[2]),
        btVector3(color[0], color[1], color[2])
    );
}

void cbtWorldDebugDrawSphere(CbtWorldHandle handle, const CbtVector3 position, float radius, const CbtVector3 color) {
    assert(position && radius > 0.0 && color);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world && world->getDebugDrawer());

    world->getDebugDrawer()->drawSphere(
        btVector3(position[0], position[1], position[2]),
        radius,
        btVector3(color[0], color[1], color[2])
    );
}

int cbtShapeGetType(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getShapeType();
}

CbtShapeHandle cbtShapeCreateBox(const CbtVector3 half_extents) {
    assert(half_extents[0] > 0.0 && half_extents[1] > 0.0 && half_extents[2] > 0.0);
    btBoxShape* box = new btBoxShape(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    return (CbtShapeHandle)box;
}

CbtShapeHandle cbtShapeCreateBox2d(float x_half_extent, float y_half_extent) {
    assert(x_half_extent > 0.0 && y_half_extent > 0.0);
    btBox2dShape* box = new btBox2dShape(btVector3(x_half_extent, y_half_extent, 0.0));
    return (CbtShapeHandle)box;
}

CbtShapeHandle cbtShapeCreateSphere(float radius) {
    assert(radius > 0.0f);
    btSphereShape* sphere = new btSphereShape(radius);
    return (CbtShapeHandle)sphere;
}

CbtShapeHandle cbtShapeCreatePlane(const CbtVector3 normal, float distance) {
    btStaticPlaneShape* cbtane = new btStaticPlaneShape(btVector3(normal[0], normal[1], normal[2]), distance);
    return (CbtShapeHandle)cbtane;
}

CbtShapeHandle cbtShapeCreateCapsule(float radius, float height, int axis) {
    assert(radius > 0.0 && height > 0 && axis >= CBT_AXIS_X && axis <= CBT_AXIS_Z);
    btCapsuleShape* shape = nullptr;
    if (axis == CBT_AXIS_X) {
        shape = new btCapsuleShapeX(radius, height);
    } else if (axis == CBT_AXIS_Y) {
        shape = new btCapsuleShape(radius, height);
    } else {
        shape = new btCapsuleShapeZ(radius, height);
    }
    return (CbtShapeHandle)shape;
}

CbtShapeHandle cbtShapeCreateCylinder(const CbtVector3 half_extents, int axis) {
    assert(half_extents[0] > 0.0 && half_extents[1] > 0.0 && half_extents[2] > 0.0);
    btCylinderShape* shape = nullptr;
    if (axis == CBT_AXIS_X) {
        shape = new btCylinderShapeX(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    } else if (axis == CBT_AXIS_Y) {
        shape = new btCylinderShape(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    } else {
        shape = new btCylinderShapeZ(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    }
    return (CbtShapeHandle)shape;
}

CbtShapeHandle cbtShapeCreateCone(float radius, float height, int axis) {
    assert(radius > 0.0 && height > 0 && axis >= CBT_AXIS_X && axis <= CBT_AXIS_Z);
    btConeShape* shape = nullptr;
    if (axis == CBT_AXIS_X) {
        shape = new btConeShapeX(radius, height);
    } else if (axis == CBT_AXIS_Y) {
        shape = new btConeShape(radius, height);
    } else {
        shape = new btConeShapeZ(radius, height);
    }
    return (CbtShapeHandle)shape;
}

int cbtShapeIsPolyhedral(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isPolyhedral();
}

int cbtShapeIsConvex2d(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isConvex2d();
}

int cbtShapeIsConvex(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isConvex();
}

int cbtShapeIsNonMoving(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isNonMoving();
}

int cbtShapeIsConcave(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isConcave();
}

int cbtShapeIsCompound(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return (int)shape->isCompound();
}

void cbtShapeCalculateLocalInertia(CbtShapeHandle handle, float mass, CbtVector3 inertia) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape && mass > 0.0);
    btVector3 ine;
    shape->calculateLocalInertia(mass, ine);
    inertia[0] = ine.x();
    inertia[1] = ine.y();
    inertia[2] = ine.z();
}

void cbtShapeSetUserPointer(CbtShapeHandle handle, void* user_pointer) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    shape->setUserPointer(user_pointer);
}

void cbtShapeSetUserIndex(CbtShapeHandle handle, int user_index) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    shape->setUserIndex(user_index);
}

void* cbtShapeGetUserPointer(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getUserPointer();
}

int cbtShapeGetUserIndex(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getUserIndex();
}

void cbtShapeDestroy(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    delete shape;
}

CbtBodyHandle cbtBodyCreate(
    CbtWorldHandle world_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btCollisionShape* shape = (btCollisionShape*)shape_handle;
    assert(world && shape && transform && mass >= 0.0);

    const bool is_dynamic = (mass != 0.0);

    btVector3 local_inertia(0.0, 0.0, 0.0);
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
    world->addRigidBody(body);

    return (CbtBodyHandle)body;
}

void cbtBodyDestroy(CbtWorldHandle world_handle, CbtBodyHandle body_handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = (btRigidBody*)body_handle;
    assert(world && body);

    if (body->getMotionState()) {
        delete body->getMotionState();
    }
    world->removeRigidBody(body);
    delete body;
}

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle) {
    btRigidBody* body = (btRigidBody*)body_handle;
    btCollisionShape* shape = (btCollisionShape*)shape_handle;
    assert(body && shape);
    body->setCollisionShape(shape);
}

CbtShapeHandle cbtBodyGetShape(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return (CbtShapeHandle)body->getCollisionShape();
}

void cbtBodySetRestitution(CbtBodyHandle handle, float restitution) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setRestitution(restitution);
}

void cbtBodySetFriction(CbtBodyHandle handle, float friction) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setFriction(friction);
}

void cbtBodySetRollingFriction(CbtBodyHandle handle, float friction) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setRollingFriction(friction);
}

void cbtBodySetSpinningFriction(CbtBodyHandle handle, float friction) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setSpinningFriction(friction);
}

void cbtBodySetAnisotropicFriction(CbtBodyHandle handle, const CbtVector3 friction, int mode) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && friction);
    body->setAnisotropicFriction(btVector3(friction[0], friction[1], friction[2]), mode);
}

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle handle, float stiffness, float damping) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setContactStiffnessAndDamping(stiffness, damping);
}

void cbtBodySetMassProps(CbtBodyHandle handle, float mass, const CbtVector3 inertia) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && inertia);
    body->setMassProps(mass, btVector3(inertia[0], inertia[1], inertia[2]));
}

void cbtBodySetDamping(CbtBodyHandle handle, float linear, float angular) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->setDamping(linear, angular);
}

void cbtBodySetLinearVelocity(CbtBodyHandle handle, const CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    body->setLinearVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetAngularVelocity(CbtBodyHandle handle, const CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    body->setAngularVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetPushVelocity(CbtBodyHandle handle, const CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    body->setPushVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetTurnVelocity(CbtBodyHandle handle, const CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    body->setTurnVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodyApplyCentralForce(CbtBodyHandle handle, const CbtVector3 force) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && force);
    body->applyCentralForce(btVector3(force[0], force[1], force[2]));
}

void cbtBodyApplyCentralImpulse(CbtBodyHandle handle, const CbtVector3 impulse) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse);
    body->applyCentralImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

void cbtBodyApplyCentralPushImpulse(CbtBodyHandle handle, const CbtVector3 impulse) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse);
    body->applyCentralPushImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

void cbtBodyApplyForce(CbtBodyHandle handle, const CbtVector3 force, const CbtVector3 rel_pos) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && force && rel_pos);
    body->applyForce(btVector3(force[0], force[1], force[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyClearForces(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    body->clearForces();
}

void cbtBodyApplyImpulse(CbtBodyHandle handle, const CbtVector3 impulse, const CbtVector3 rel_pos) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse && rel_pos);
    body->applyImpulse(btVector3(impulse[0], impulse[1], impulse[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyApplyPushImpulse(CbtBodyHandle handle, const CbtVector3 impulse, const CbtVector3 rel_pos) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse && rel_pos);
    body->applyPushImpulse(btVector3(impulse[0], impulse[1], impulse[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyApplyTorque(CbtBodyHandle handle, const CbtVector3 torque) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && torque);
    body->applyTorque(btVector3(torque[0], torque[1], torque[2]));
}

void cbtBodyApplyTorqueImpulse(CbtBodyHandle handle, const CbtVector3 impulse) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse);
    body->applyTorqueImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

void cbtBodyApplyTorqueTurnImpulse(CbtBodyHandle handle, const CbtVector3 impulse) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && impulse);
    body->applyTorqueTurnImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

float cbtBodyGetRestitution(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getRestitution();
}

float cbtBodyGetFriction(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getFriction();
}

float cbtBodyGetRollingFriction(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getRollingFriction();
}

float cbtBodyGetSpinningFriction(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getSpinningFriction();
}

void cbtBodyGetAnisotropicFriction(CbtBodyHandle handle, CbtVector3 friction) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && friction);
    const btVector3& f = body->getAnisotropicFriction();
    friction[0] = f.x();
    friction[1] = f.y();
    friction[2] = f.z();
}

float cbtBodyGetContactStiffness(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getContactStiffness();
}

float cbtBodyGetContactDamping(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getContactDamping();
}

float cbtBodyGetMass(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getMass();
}

float cbtBodyGetLinearDamping(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getLinearDamping();
}

float cbtBodyGetAngularDamping(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getAngularDamping();
}

void cbtBodyGetLinearVelocity(CbtBodyHandle handle, CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    const btVector3& vel = body->getLinearVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetAngularVelocity(CbtBodyHandle handle, CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    const btVector3& vel = body->getAngularVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetPushVelocity(CbtBodyHandle handle, CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    const btVector3& vel = body->getPushVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetTurnVelocity(CbtBodyHandle handle, CbtVector3 velocity) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && velocity);
    const btVector3& vel = body->getTurnVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetTotalForce(CbtBodyHandle handle, CbtVector3 force) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && force);
    const btVector3& f = body->getTotalForce();
    force[0] = f.x();
    force[1] = f.y();
    force[2] = f.z();
}

void cbtBodyGetTotalTorque(CbtBodyHandle handle, CbtVector3 torque) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && torque);
    const btVector3& t = body->getTotalTorque();
    torque[0] = t.x();
    torque[1] = t.y();
    torque[2] = t.z();
}

int cbtBodyIsStatic(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return (int)body->isStaticObject();
}

int cbtBodyIsKinematic(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return (int)body->isKinematicObject();
}

int cbtBodyIsStaticOrKinematic(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return (int)body->isStaticOrKinematicObject();
}

float cbtBodyGetDeactivationTime(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getDeactivationTime();
}

void cbtBodySetDeactivationTime(CbtBodyHandle handle, float time) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->setDeactivationTime(time);
}

int cbtBodyGetActivationState(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->getActivationState();
}

void cbtBodySetActivationState(CbtBodyHandle handle, int state) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return body->setActivationState(state);
}

int cbtBodyIsActive(CbtBodyHandle handle) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body);
    return (int)body->isActive();
}

void cbtBodyGetCenterOfMassTransform(CbtBodyHandle handle, CbtVector3 transform[4]) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && transform);

    const btTransform& trans = body->getCenterOfMassTransform();
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

void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle handle, CbtVector3 transform[4]) {
    btRigidBody* body = (btRigidBody*)handle;
    assert(body && transform);

    const btTransform trans = body->getCenterOfMassTransform().inverse();
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

void cbtBodyGetGraphicsWorldTransform(CbtBodyHandle handle, CbtVector3 transform[4]) {
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
