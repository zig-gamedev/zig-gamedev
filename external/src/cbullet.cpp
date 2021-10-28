#include "cbullet.h"
#include <assert.h>
#include <stdint.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"

#define HANDLE_TO_POINTER(type, handle) ((type*)((uint64_t)handle & ~0x1))

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

void cbtWorldAddBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle) {
    assert(world_handle && body_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    world->addRigidBody(body);
}

void cbtWorldAddConstraint(
    CbtWorldHandle world_handle,
    CbtConstraintHandle constraint_handle,
    CbtBool disable_collision_between_linked_bodies
) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btTypedConstraint* constraint = (btTypedConstraint*)constraint_handle;
    assert(world && constraint);
    world->addConstraint(constraint, disable_collision_between_linked_bodies ? true : false);
}

void cbtWorldRemoveBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle) {
    assert(world_handle && body_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    world->removeRigidBody(body);
}

void cbtWorldRemoveConstraint(CbtWorldHandle world_handle, CbtConstraintHandle constraint_handle) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btTypedConstraint* constraint = (btTypedConstraint*)constraint_handle;
    assert(world && constraint);
    world->removeConstraint(constraint);
}

CbtBool cbtRayTestClosest(
    CbtWorldHandle handle,
    const CbtVector3 ray_from_world,
    const CbtVector3 ray_to_world,
    int collision_filter_group,
    int collision_filter_mask,
    unsigned int flags,
    CbtRayCastResult* result
) {
    assert(handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;

    const btVector3 from(ray_from_world[0], ray_from_world[1], ray_from_world[2]);
    const btVector3 to(ray_to_world[0], ray_to_world[1], ray_to_world[2]);

    btCollisionWorld::ClosestRayResultCallback closest(from, to);
    closest.m_collisionFilterGroup = collision_filter_group;
    closest.m_collisionFilterMask = collision_filter_mask;
    closest.m_flags = flags;

    world->rayTest(from, to, closest);

    if (result) {
        result->hit_normal_world[0] = closest.m_hitNormalWorld.x();
        result->hit_normal_world[1] = closest.m_hitNormalWorld.y();
        result->hit_normal_world[2] = closest.m_hitNormalWorld.z();
        result->hit_point_world[0] = closest.m_hitPointWorld.x();
        result->hit_point_world[1] = closest.m_hitPointWorld.y();
        result->hit_point_world[2] = closest.m_hitPointWorld.z();
        result->hit_fraction = closest.m_closestHitFraction;
        result->body = (CbtBodyHandle)closest.m_collisionObject;
    }
    return closest.m_collisionObject != 0;
}

void cbtWorldDebugSetCallbacks(CbtWorldHandle handle, const CbtDebugDrawCallbacks* callbacks) {
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)handle;
    assert(world && callbacks);

    CbtDebugDraw* debug = (CbtDebugDraw*)world->getDebugDrawer();
    if (debug == nullptr) {
        debug = new CbtDebugDraw();
        debug->setDebugMode(
            btIDebugDraw::DBG_DrawWireframe |
            btIDebugDraw::DBG_DrawFrames |
            btIDebugDraw::DBG_DrawContactPoints |
            btIDebugDraw::DBG_DrawConstraints
        );
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

CbtBool cbtShapeIsPolyhedral(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isPolyhedral() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConvex2d(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isConvex2d() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConvex(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isConvex() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsNonMoving(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isNonMoving() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConcave(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isConcave() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsCompound(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->isCompound() ? CBT_TRUE : CBT_FALSE;
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

void* cbtShapeGetUserPointer(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    return shape->getUserPointer();
}

void cbtShapeSetUserIndex(CbtShapeHandle handle, int slot, int user_index) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    assert(slot == 0 || slot == 1);
    if (slot == 0) {
        shape->setUserIndex(user_index);
    } else {
        shape->setUserIndex2(user_index);
    }
}

int cbtShapeGetUserIndex(CbtShapeHandle handle, int slot) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    assert(slot == 0 || slot == 1);
    if (slot == 0) {
        return shape->getUserIndex();
    }
    return shape->getUserIndex2();
}

void cbtShapeDestroy(CbtShapeHandle handle) {
    btCollisionShape* shape = (btCollisionShape*)handle;
    assert(shape);
    delete shape;
}

static_assert((sizeof(btRigidBody) % 16) == 0, "sizeof(btRigidBody) is not multiple of 16");
static_assert((sizeof(btDefaultMotionState) % 16) == 0, "sizeof(btDefaultMotionState) is not multiple of 16");
static_assert(
    ((sizeof(btRigidBody) + sizeof(btDefaultMotionState)) % 16) == 0,
    "sizeof(btRigidBody) + sizeof(btDefaultMotionState) is not multiple of 16"
);

void cbtBodyAllocate(unsigned int num, CbtBodyHandle* body_handles) {
    assert(num > 0 && body_handles);
    const size_t element_size = sizeof(btRigidBody) + sizeof(btDefaultMotionState);
    uint8_t* base = (uint8_t*)_aligned_malloc(num * element_size, 16);
    for (unsigned int i = 0; i < num; ++i) {
        body_handles[i] = (CbtBodyHandle)(base + i * element_size);
    }
    body_handles[0] = (CbtBodyHandle)((uint64_t)body_handles[0] | 0x1);
}

void cbtBodyDeallocate(unsigned int num, CbtBodyHandle* body_handles) {
    assert(num > 0 && body_handles);
    for (unsigned int i = 0; i < num; ++i) {
        if ((uint64_t)body_handles[i] & 0x1) {
            _aligned_free(HANDLE_TO_POINTER(btRigidBody, body_handles[i]));
        }
    }
}

void cbtBodyCreate(
    CbtBodyHandle body_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
) {
    assert(body_handle && shape_handle);
    assert(transform && mass >= 0.0);

    btCollisionShape* shape = (btCollisionShape*)shape_handle;

    void* body_mem = (void*)HANDLE_TO_POINTER(btRigidBody, body_handle);
    void* motion_state_mem = (void*)((uint8_t*)HANDLE_TO_POINTER(btRigidBody, body_handle) + sizeof(btRigidBody));

    const bool is_dynamic = (mass != 0.0);

    btVector3 local_inertia(0.0, 0.0, 0.0);
    if (is_dynamic) {
        shape->calculateLocalInertia(mass, local_inertia);
    }

    // NOTE(mziulek): Bullet uses M * v order/convention so we need to transpose matrix.
    btDefaultMotionState* motion_state = new (motion_state_mem) btDefaultMotionState(
        btTransform(
            btMatrix3x3(
                btVector3(transform[0][0], transform[1][0], transform[2][0]),
                btVector3(transform[0][1], transform[1][1], transform[2][1]),
                btVector3(transform[0][2], transform[1][2], transform[2][2])
            ),
            btVector3(transform[3][0], transform[3][1], transform[3][2])
    ));

    btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, local_inertia);
    btRigidBody* body = new (body_mem) btRigidBody(info);
}

void cbtBodyDestroy(CbtBodyHandle body_handle) {
    assert(body_handle);

    auto* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    auto* motion_state =
        (btDefaultMotionState*)((uint8_t*)HANDLE_TO_POINTER(btRigidBody, body_handle) + sizeof(btRigidBody));

    motion_state->~btDefaultMotionState();
    body->~btRigidBody();
}

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle) {
    assert(body_handle && shape_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    btCollisionShape* shape = (btCollisionShape*)shape_handle;
    body->setCollisionShape(shape);
}

CbtShapeHandle cbtBodyGetShape(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return (CbtShapeHandle)body->getCollisionShape();
}

void cbtBodySetRestitution(CbtBodyHandle body_handle, float restitution) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setRestitution(restitution);
}

void cbtBodySetFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setFriction(friction);
}

void cbtBodySetRollingFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setRollingFriction(friction);
}

void cbtBodySetSpinningFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setSpinningFriction(friction);
}

void cbtBodySetAnisotropicFriction(CbtBodyHandle body_handle, const CbtVector3 friction, int mode) {
    assert(body_handle && friction);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setAnisotropicFriction(btVector3(friction[0], friction[1], friction[2]), mode);
}

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle body_handle, float stiffness, float damping) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setContactStiffnessAndDamping(stiffness, damping);
}

void cbtBodySetMassProps(CbtBodyHandle body_handle, float mass, const CbtVector3 inertia) {
    assert(body_handle && inertia);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setMassProps(mass, btVector3(inertia[0], inertia[1], inertia[2]));
}

void cbtBodySetDamping(CbtBodyHandle body_handle, float linear, float angular) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setDamping(linear, angular);
}

void cbtBodySetLinearVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity) {
    assert(body_handle && velocity);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setLinearVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetAngularVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity) {
    assert(body_handle && velocity);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setAngularVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetLinearFactor(CbtBodyHandle body_handle, const CbtVector3 factor) {
    assert(body_handle && factor);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setLinearFactor(btVector3(factor[0], factor[1], factor[2]));
}

void cbtBodySetAngularFactor(CbtBodyHandle body_handle, const CbtVector3 factor) {
    assert(body_handle && factor);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setAngularFactor(btVector3(factor[0], factor[1], factor[2]));
}

void cbtBodyApplyCentralForce(CbtBodyHandle body_handle, const CbtVector3 force) {
    assert(body_handle && force);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyCentralForce(btVector3(force[0], force[1], force[2]));
}

void cbtBodyApplyCentralImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse) {
    assert(body_handle && impulse);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyCentralImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

void cbtBodyApplyForce(CbtBodyHandle body_handle, const CbtVector3 force, const CbtVector3 rel_pos) {
    assert(body_handle && force && rel_pos);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyForce(btVector3(force[0], force[1], force[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyClearForces(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->clearForces();
}

void cbtBodyApplyImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse, const CbtVector3 rel_pos) {
    assert(body_handle && impulse && rel_pos);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyImpulse(btVector3(impulse[0], impulse[1], impulse[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyApplyTorque(CbtBodyHandle body_handle, const CbtVector3 torque) {
    assert(body_handle && torque);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyTorque(btVector3(torque[0], torque[1], torque[2]));
}

void cbtBodyApplyTorqueImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse) {
    assert(body_handle && impulse);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->applyTorqueImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

float cbtBodyGetRestitution(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getRestitution();
}

float cbtBodyGetFriction(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getFriction();
}

float cbtBodyGetRollingFriction(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getRollingFriction();
}

float cbtBodyGetSpinningFriction(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getSpinningFriction();
}

void cbtBodyGetAnisotropicFriction(CbtBodyHandle body_handle, CbtVector3 friction) {
    assert(body_handle && friction);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    const btVector3& f = body->getAnisotropicFriction();
    friction[0] = f.x();
    friction[1] = f.y();
    friction[2] = f.z();
}

float cbtBodyGetContactStiffness(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getContactStiffness();
}

float cbtBodyGetContactDamping(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getContactDamping();
}

float cbtBodyGetMass(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getMass();
}

float cbtBodyGetLinearDamping(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getLinearDamping();
}

float cbtBodyGetAngularDamping(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getAngularDamping();
}

void cbtBodyGetLinearVelocity(CbtBodyHandle body_handle, CbtVector3 velocity) {
    assert(body_handle && velocity);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    const btVector3& vel = body->getLinearVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetAngularVelocity(CbtBodyHandle body_handle, CbtVector3 velocity) {
    assert(body_handle && velocity);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    const btVector3& vel = body->getAngularVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

CbtBool cbtBodyIsStatic(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->isStaticObject() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsKinematic(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->isKinematicObject() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsStaticOrKinematic(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->isStaticOrKinematicObject() ? CBT_TRUE : CBT_FALSE;
}

float cbtBodyGetDeactivationTime(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getDeactivationTime();
}

void cbtBodySetDeactivationTime(CbtBodyHandle body_handle, float time) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->setDeactivationTime(time);
}

int cbtBodyGetActivationState(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getActivationState();
}

void cbtBodySetActivationState(CbtBodyHandle body_handle, int state) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->setActivationState(state);
}

void cbtBodyForceActivationState(CbtBodyHandle body_handle, int state) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->forceActivationState(state);
}

CbtBool cbtBodyIsActive(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->isActive() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsInWorld(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->isInWorld() ? CBT_TRUE : CBT_FALSE;
}

void cbtBodySetUserPointer(CbtBodyHandle body_handle, void* user_pointer) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    body->setUserPointer(user_pointer);
}

void* cbtBodyGetUserPointer(CbtBodyHandle body_handle) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    return body->getUserPointer();
}

void cbtBodySetUserIndex(CbtBodyHandle body_handle, int slot, int user_index) {
    assert(body_handle);
    assert(slot >= 0 && slot <= 2);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    if (slot == 0) {
        body->setUserIndex(user_index);
    } else if (slot == 1) {
        body->setUserIndex2(user_index);
    } else {
        body->setUserIndex3(user_index);
    }
}

int cbtBodyGetUserIndex(CbtBodyHandle body_handle, int slot) {
    assert(body_handle);
    assert(slot >= 0 && slot <= 2);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    if (slot == 0) {
        return body->getUserIndex();
    }
    if (slot == 1) {
        return body->getUserIndex2();
    }
    return body->getUserIndex3();
}

void cbtBodySetCenterOfMassTransform(CbtBodyHandle body_handle, const CbtVector3 transform[4]) {
    assert(body_handle);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);
    // NOTE(mziulek): Bullet uses M * v order/convention so we need to transpose matrix.
    body->setCenterOfMassTransform(btTransform(
        btMatrix3x3(
            btVector3(transform[0][0], transform[1][0], transform[2][0]),
            btVector3(transform[0][1], transform[1][1], transform[2][1]),
            btVector3(transform[0][2], transform[1][2], transform[2][2])
        ),
        btVector3(transform[3][0], transform[3][1], transform[3][2])
    ));
}

void cbtBodyGetCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]) {
    assert(body_handle && transform);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);

    const btTransform& trans = body->getCenterOfMassTransform();
    const btMatrix3x3& basis = trans.getBasis();
    const btVector3& origin = trans.getOrigin();

    // NOTE(mziulek): We transpose Bullet matrix here to make it compatible with: v * M convention.
    transform[0][0] = basis.getRow(0).x();
    transform[1][0] = basis.getRow(0).y();
    transform[2][0] = basis.getRow(0).z();
    transform[0][1] = basis.getRow(1).x();
    transform[1][1] = basis.getRow(1).y();
    transform[2][1] = basis.getRow(1).z();
    transform[0][2] = basis.getRow(2).x();
    transform[1][2] = basis.getRow(2).y();
    transform[2][2] = basis.getRow(2).z();

    transform[3][0] = origin.x();
    transform[3][1] = origin.y();
    transform[3][2] = origin.z();
}

void cbtBodyGetCenterOfMassPosition(CbtBodyHandle body_handle, CbtVector3 position) {
    assert(body_handle && position);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);

    const btTransform& trans = body->getCenterOfMassTransform();
    const btVector3& origin = trans.getOrigin();

    position[0] = origin.x();
    position[1] = origin.y();
    position[2] = origin.z();
}

void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]) {
    assert(body_handle && transform);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);

    const btTransform trans = body->getCenterOfMassTransform().inverse();
    const btMatrix3x3& basis = trans.getBasis();
    const btVector3& origin = trans.getOrigin();

    // NOTE(mziulek): We transpose Bullet matrix here to make it compatible with: v * M convention.
    transform[0][0] = basis.getRow(0).x();
    transform[1][0] = basis.getRow(0).y();
    transform[2][0] = basis.getRow(0).z();
    transform[0][1] = basis.getRow(1).x();
    transform[1][1] = basis.getRow(1).y();
    transform[2][1] = basis.getRow(1).z();
    transform[0][2] = basis.getRow(2).x();
    transform[1][2] = basis.getRow(2).y();
    transform[2][2] = basis.getRow(2).z();

    transform[3][0] = origin.x();
    transform[3][1] = origin.y();
    transform[3][2] = origin.z();
}

void cbtBodyGetGraphicsWorldTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]) {
    assert(body_handle && transform);
    btRigidBody* body = HANDLE_TO_POINTER(btRigidBody, body_handle);

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

CbtBodyHandle cbtConGetFixedBody(void) {
    return (CbtBodyHandle)&btTypedConstraint::getFixedBody();
}

void cbtConDestroy(CbtConstraintHandle handle) {
    btTypedConstraint* con = (btTypedConstraint*)handle;
    assert(con);
    delete con;
}

CbtConstraintHandle cbtConCreatePoint2Point(
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b
) {
    assert(body_handle_a && body_handle_b);
    btRigidBody* body_a = HANDLE_TO_POINTER(btRigidBody, body_handle_a);
    btRigidBody* body_b = HANDLE_TO_POINTER(btRigidBody, body_handle_b);
    btPoint2PointConstraint* constraint = new btPoint2PointConstraint(
        *body_a,
        *body_b,
        btVector3(pivot_a[0], pivot_a[1], pivot_a[2]),
        btVector3(pivot_b[0], pivot_b[1], pivot_b[2])
    );
    return (CbtConstraintHandle)constraint;
}

void cbtConPoint2PointSetPivotA(CbtConstraintHandle handle, const CbtVector3 pivot) {
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)handle;
    assert(con && con->getConstraintType() == POINT2POINT_CONSTRAINT_TYPE);
    con->setPivotA(btVector3(pivot[0], pivot[1], pivot[2]));
}

void cbtConPoint2PointSetPivotB(CbtConstraintHandle handle, const CbtVector3 pivot) {
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)handle;
    assert(con && con->getConstraintType() == POINT2POINT_CONSTRAINT_TYPE);
    con->setPivotB(btVector3(pivot[0], pivot[1], pivot[2]));
}

void cbtConPoint2PointSetTau(CbtConstraintHandle handle, float tau) {
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)handle;
    assert(con && con->getConstraintType() == POINT2POINT_CONSTRAINT_TYPE);
    con->m_setting.m_tau = tau;
}

void cbtConPoint2PointSetDamping(CbtConstraintHandle handle, float damping) {
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)handle;
    assert(con && con->getConstraintType() == POINT2POINT_CONSTRAINT_TYPE);
    con->m_setting.m_damping = damping;
}

void cbtConPoint2PointSetImpulseClamp(CbtConstraintHandle handle, float impulse_clamp) {
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)handle;
    assert(con && con->getConstraintType() == POINT2POINT_CONSTRAINT_TYPE);
    con->m_setting.m_impulseClamp = impulse_clamp;
}
