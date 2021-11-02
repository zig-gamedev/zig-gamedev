#include "cbullet.h"
#include <assert.h>
#include <stdint.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletDynamics/ConstraintSolver/btContactConstraint.h"

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

void cbtWorldDestroy(CbtWorldHandle world_handle) {
    assert(world_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;

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

void cbtWorldSetGravity(CbtWorldHandle world_handle, const CbtVector3 gravity) {
    assert(world_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    world->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));
}

int cbtWorldStepSimulation(CbtWorldHandle world_handle, float time_step, int max_sub_steps, float fixed_time_step) {
    assert(world_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    return world->stepSimulation(time_step, max_sub_steps, fixed_time_step);
}

void cbtWorldAddBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle) {
    assert(world_handle);
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = (btRigidBody*)body_handle;
    world->addRigidBody(body);
}

void cbtWorldAddConstraint(
    CbtWorldHandle world_handle,
    CbtConstraintHandle con_handle,
    CbtBool disable_collision_between_linked_bodies
) {
    assert(world_handle);
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(
        disable_collision_between_linked_bodies == CBT_FALSE ||
        disable_collision_between_linked_bodies == CBT_TRUE
    );
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btTypedConstraint* con = (btTypedConstraint*)con_handle;
    world->addConstraint(con, disable_collision_between_linked_bodies == CBT_FALSE ? false : true);
}

void cbtWorldRemoveBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle) {
    assert(world_handle);
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btRigidBody* body = (btRigidBody*)body_handle;
    world->removeRigidBody(body);
}

void cbtWorldRemoveConstraint(CbtWorldHandle world_handle, CbtConstraintHandle con_handle) {
    assert(world_handle && con_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    btTypedConstraint* con = (btTypedConstraint*)con_handle;
    world->removeConstraint(con);
}

int cbtWorldGetNumBodies(CbtWorldHandle world_handle) {
    assert(world_handle);
    auto world = (btDiscreteDynamicsWorld*)world_handle;
    return (int)world->getCollisionObjectArray().size();
}

int cbtWorldGetNumConstraints(CbtWorldHandle world_handle) {
    assert(world_handle);
    auto world = (btDiscreteDynamicsWorld*)world_handle;
    return world->getNumConstraints();
}

CbtBodyHandle cbtWorldGetBody(CbtWorldHandle world_handle, int body_index) {
    assert(world_handle);
    auto world = (btDiscreteDynamicsWorld*)world_handle;
    return (CbtBodyHandle)world->getCollisionObjectArray()[body_index];
}

CbtConstraintHandle cbtWorldGetConstraint(CbtWorldHandle world_handle, int con_index) {
    assert(world_handle);
    auto world = (btDiscreteDynamicsWorld*)world_handle;
    return (CbtConstraintHandle)world->getConstraint(con_index);
}

CbtBool cbtRayTestClosest(
    CbtWorldHandle world_handle,
    const CbtVector3 ray_from_world,
    const CbtVector3 ray_to_world,
    int collision_filter_group,
    int collision_filter_mask,
    unsigned int flags,
    CbtRayCastResult* result
) {
    assert(world_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;

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
    return closest.m_collisionObject == 0 ? CBT_FALSE : CBT_TRUE;
}

void cbtWorldDebugSetCallbacks(CbtWorldHandle world_handle, const CbtDebugDrawCallbacks* callbacks) {
    assert(world_handle && callbacks);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;

    CbtDebugDraw* debug = (CbtDebugDraw*)world->getDebugDrawer();
    if (debug == nullptr) {
        debug = new CbtDebugDraw();
        debug->setDebugMode(
            btIDebugDraw::DBG_DrawWireframe |
            btIDebugDraw::DBG_DrawFrames |
            btIDebugDraw::DBG_DrawContactPoints |
            //btIDebugDraw::DBG_DrawNormals |
            btIDebugDraw::DBG_DrawConstraints
        );
        world->setDebugDrawer(debug);
    }

    debug->callbacks = *callbacks;
}

void cbtWorldDebugDraw(CbtWorldHandle world_handle) {
    assert(world_handle);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    world->debugDrawWorld();
}

void cbtWorldDebugDrawLine(
    CbtWorldHandle world_handle,
    const CbtVector3 p0,
    const CbtVector3 p1,
    const CbtVector3 color
) {
    assert(world_handle);
    assert(p0 && p1 && color);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    assert(world->getDebugDrawer());

    world->getDebugDrawer()->drawLine(
        btVector3(p0[0], p0[1], p0[2]),
        btVector3(p1[0], p1[1], p1[2]),
        btVector3(color[0], color[1], color[2])
    );
}

void cbtWorldDebugDrawSphere(
    CbtWorldHandle world_handle,
    const CbtVector3 position,
    float radius,
    const CbtVector3 color
) {
    assert(world_handle);
    assert(position && radius > 0.0 && color);
    btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)world_handle;
    assert(world->getDebugDrawer());

    world->getDebugDrawer()->drawSphere(
        btVector3(position[0], position[1], position[2]),
        radius,
        btVector3(color[0], color[1], color[2])
    );
}

static_assert(sizeof(btCapsuleShape) == sizeof(btCapsuleShapeX), "wrong size");
static_assert(sizeof(btCapsuleShape) == sizeof(btCapsuleShapeZ), "wrong size");
static_assert(sizeof(btConeShape) == sizeof(btConeShapeX), "wrong size");
static_assert(sizeof(btConeShape) == sizeof(btConeShapeZ), "wrong size");
static_assert(sizeof(btCylinderShape) == sizeof(btCylinderShapeX), "wrong size");
static_assert(sizeof(btCylinderShape) == sizeof(btCylinderShapeZ), "wrong size");

CbtShapeHandle cbtShapeAllocate(int shape_type) {
    size_t size = 0;
    switch (shape_type) {
        case CBT_SHAPE_TYPE_BOX: size = sizeof(btBoxShape); break;
        case CBT_SHAPE_TYPE_BOX_2D: size = sizeof(btBox2dShape); break;
        case CBT_SHAPE_TYPE_SPHERE: size = sizeof(btSphereShape); break;
        case CBT_SHAPE_TYPE_CAPSULE: size = sizeof(btCapsuleShape); break;
        case CBT_SHAPE_TYPE_CONE: size = sizeof(btConeShape); break;
        case CBT_SHAPE_TYPE_CYLINDER: size = sizeof(btCylinderShape); break;
        case CBT_SHAPE_TYPE_STATIC_PLANE: size = sizeof(btStaticPlaneShape); break;
        case CBT_SHAPE_TYPE_COMPOUND: size = sizeof(btCompoundShape); break;
        case CBT_SHAPE_TYPE_TRIANGLE_MESH:
            size = sizeof(btBvhTriangleMeshShape) + sizeof(btTriangleIndexVertexArray);
            break;
        default:
            assert(0);
    }
    auto shape = (int*)btAlignedAlloc(size, 16);

    // Set vtable to 0, this means that object is not created.
    shape[0] = 0;
    shape[1] = 0;
    // btCollisionShape::m_shapeType field is just after vtable (offset: 8).
    shape[2] = shape_type;

    if (shape_type == CBT_SHAPE_TYPE_TRIANGLE_MESH) {
        ((uint64_t*)((uint8_t*)shape + sizeof(btBvhTriangleMeshShape)))[0] = 0;
    }

    return (CbtShapeHandle)shape;
}

void cbtShapeDeallocate(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    btAlignedFree(shape_handle);
}

void cbtShapeDestroy(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) != CBT_SHAPE_TYPE_TRIANGLE_MESH);
    auto shape = (btCollisionShape*)shape_handle;
    shape->~btCollisionShape();
    // Set vtable to 0, this means that object is not created.
    ((uint64_t*)shape_handle)[0] = 0;
}

CbtBool cbtShapeIsCreated(CbtShapeHandle shape_handle) {
    assert(shape_handle);
    // vtable == 0 means that object is not created.
    return ((uint64_t*)shape_handle)[0] == 0 ? CBT_FALSE : CBT_TRUE;
}

int cbtShapeGetType(CbtShapeHandle shape_handle) {
    assert(shape_handle);
    auto shape = (int*)shape_handle;
    // btCollisionShape::m_shapeType field is just after vtable (offset: 8).
    // This function works even for not yet created shapes (cbtShapeAllocate sets the type).
    return shape[2];
}

void cbtShapeBoxCreate(CbtShapeHandle shape_handle, const CbtVector3 half_extents) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_BOX);
    assert(half_extents[0] > 0.0 && half_extents[1] > 0.0 && half_extents[2] > 0.0);
    new (shape_handle) btBoxShape(btVector3(half_extents[0], half_extents[1], half_extents[2]));
}

void cbtShapeBox2dCreate(CbtShapeHandle shape_handle, float x_half_extent, float y_half_extent) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_BOX_2D);
    assert(x_half_extent > 0.0 && y_half_extent > 0.0);
    new (shape_handle) btBox2dShape(btVector3(x_half_extent, y_half_extent, 0.0));
}

void cbtShapeSphereCreate(CbtShapeHandle shape_handle, float radius) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_SPHERE);
    assert(radius > 0.0);
    new (shape_handle) btSphereShape(radius);
}

void cbtShapeSphereSetUnscaledRadius(CbtShapeHandle shape_handle, float radius) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_SPHERE);
    assert(radius > 0.0);
    auto shape = (btSphereShape*)shape_handle;
    shape->setUnscaledRadius(radius);
}

float cbtShapeSphereGetRadius(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_SPHERE);
    auto shape = (btSphereShape*)shape_handle;
    return shape->getRadius();
}

void cbtShapePlaneCreate(CbtShapeHandle shape_handle, const CbtVector3 normal, float distance) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_STATIC_PLANE);
    new (shape_handle) btStaticPlaneShape(btVector3(normal[0], normal[1], normal[2]), distance);
}

void cbtShapeCapsuleCreate(CbtShapeHandle shape_handle, float radius, float height, int axis) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CAPSULE);
    assert(radius > 0.0 && height > 0 && axis >= CBT_AXIS_X && axis <= CBT_AXIS_Z);
    if (axis == CBT_AXIS_X) {
        new (shape_handle) btCapsuleShapeX(radius, height);
    } else if (axis == CBT_AXIS_Y) {
        new (shape_handle) btCapsuleShape(radius, height);
    } else {
        new (shape_handle) btCapsuleShapeZ(radius, height);
    }
}

int cbtShapeCapsuleGetUpAxis(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CAPSULE);
    auto shape = (btCapsuleShape*)shape_handle;
    return shape->getUpAxis();
}

float cbtShapeCapsuleGetHalfHeight(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CAPSULE);
    auto shape = (btCapsuleShape*)shape_handle;
    return shape->getHalfHeight();
}

float cbtShapeCapsuleGetRadius(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CAPSULE);
    auto shape = (btCapsuleShape*)shape_handle;
    return shape->getRadius();
}

void cbtShapeCylinderCreate(CbtShapeHandle shape_handle, const CbtVector3 half_extents, int axis) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CYLINDER);
    assert(half_extents[0] > 0.0 && half_extents[1] > 0.0 && half_extents[2] > 0.0);
    if (axis == CBT_AXIS_X) {
        new (shape_handle) btCylinderShapeX(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    } else if (axis == CBT_AXIS_Y) {
        new (shape_handle) btCylinderShape(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    } else {
        new (shape_handle) btCylinderShapeZ(btVector3(half_extents[0], half_extents[1], half_extents[2]));
    }
}

void cbtShapeConeCreate(CbtShapeHandle shape_handle, float radius, float height, int axis) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_CONE);
    assert(radius > 0.0 && height > 0 && axis >= CBT_AXIS_X && axis <= CBT_AXIS_Z);
    if (axis == CBT_AXIS_X) {
        new (shape_handle) btConeShapeX(radius, height);
    } else if (axis == CBT_AXIS_Y) {
        new (shape_handle) btConeShape(radius, height);
    } else {
        new (shape_handle) btConeShapeZ(radius, height);
    }
}

void cbtShapeCompoundCreate(
    CbtShapeHandle shape_handle,
    CbtBool enable_dynamic_aabb_tree,
    int initial_child_capacity
) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(enable_dynamic_aabb_tree == CBT_FALSE || enable_dynamic_aabb_tree == CBT_TRUE);
    assert(initial_child_capacity >= 0);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);
    new (shape_handle) btCompoundShape(enable_dynamic_aabb_tree == CBT_FALSE ? false : true, initial_child_capacity);
}

void cbtShapeCompoundAddChild(
    CbtShapeHandle shape_handle,
    const CbtVector3 local_transform[4],
    CbtShapeHandle child_shape_handle
) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(child_shape_handle && cbtShapeIsCreated(child_shape_handle) == CBT_TRUE);
    assert(local_transform);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);

    auto parent = (btCompoundShape*)shape_handle;
    auto child = (btCollisionShape*)child_shape_handle;

    // NOTE(mziulek): Bullet uses M * v order/convention so we need to transpose matrix.
    parent->addChildShape(
        btTransform(
            btMatrix3x3(
                btVector3(local_transform[0][0], local_transform[1][0], local_transform[2][0]),
                btVector3(local_transform[0][1], local_transform[1][1], local_transform[2][1]),
                btVector3(local_transform[0][2], local_transform[1][2], local_transform[2][2])
            ),
            btVector3(local_transform[3][0], local_transform[3][1], local_transform[3][2])
        ),
        child
    );
}

void cbtShapeCompoundRemoveChild(CbtShapeHandle shape_handle, CbtShapeHandle child_shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(child_shape_handle && cbtShapeIsCreated(child_shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);
    auto parent = (btCompoundShape*)shape_handle;
    auto child = (btCollisionShape*)child_shape_handle;
    parent->removeChildShape(child);
}

void cbtShapeCompoundRemoveChildByIndex(CbtShapeHandle shape_handle, int child_shape_index) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);
    auto shape = (btCompoundShape*)shape_handle;
    shape->removeChildShapeByIndex(child_shape_index);
}

int cbtShapeCompoundGetNumChilds(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);
    auto shape = (btCompoundShape*)shape_handle;
    return shape->getNumChildShapes();
}

CbtShapeHandle cbtShapeCompoundGetChild(CbtShapeHandle shape_handle, int child_shape_index) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_COMPOUND);
    auto shape = (btCompoundShape*)shape_handle;
    return (CbtShapeHandle)shape->getChildShape(child_shape_index);
}

static_assert((sizeof(btBvhTriangleMeshShape) % 16) == 0, "sizeof(btBvhTriangleMeshShape) is not multiple of 16");
static_assert(
    (sizeof(btTriangleIndexVertexArray) % 16) == 0,
    "sizeof(btTriangleIndexVertexArray) is not multiple of 16"
);
static_assert(
    ((sizeof(btBvhTriangleMeshShape) + sizeof(btTriangleIndexVertexArray)) % 16) == 0,
    "sizeof(btBvhTriangleMeshShape) + sizeof(btTriangleIndexVertexArray) is not multiple of 16"
);

void cbtShapeTriMeshCreateBegin(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_TRIANGLE_MESH);
    assert(((uint64_t*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape)))[0] == 0);

    void* mesh_interface_mem = (void*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape));
    new (mesh_interface_mem) btTriangleIndexVertexArray();
}

void cbtShapeTriMeshCreateEnd(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_TRIANGLE_MESH);
    assert(((uint64_t*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape)))[0] != 0);

    auto mesh_interface = (btTriangleIndexVertexArray*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape));
    assert(mesh_interface->getNumSubParts() > 0);

    new (shape_handle) btBvhTriangleMeshShape(mesh_interface, false, true);
}

void cbtShapeTriMeshDestroy(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_TRIANGLE_MESH);

    auto shape = (btBvhTriangleMeshShape*)shape_handle;
    auto mesh_interface = (btTriangleIndexVertexArray*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape));

    const IndexedMeshArray& arr = mesh_interface->getIndexedMeshArray();
    for (size_t i = 0; i < arr.size(); ++i) {
        // We keep triangles and vertices in one memory buffer, so only one deallocation is needed.
        btAlignedFree((void*)arr[i].m_triangleIndexBase);
    }

    mesh_interface->~btTriangleIndexVertexArray();
    shape->~btBvhTriangleMeshShape();

    // Set vtable to 0, this means that object is not created.
    ((uint64_t*)shape_handle)[0] = 0;
}

void cbtShapeTriMeshAddIndexVertexArray(
    CbtShapeHandle shape_handle,
    int num_triangles,
    const void* triangles_base,
    int triangle_stride,
    int num_vertices,
    const void* vertices_base,
    int vertex_stride
) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_TRIANGLE_MESH);
    assert(((uint64_t*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape)))[0] != 0);
    assert(num_triangles > 0 && num_vertices > 0);
    assert(triangles_base != nullptr && vertices_base != nullptr);
    assert(vertex_stride >= 12);
    // NOTE(mziulek): We currently require triangles to be tightly packed.
    assert(triangle_stride == 3 || triangle_stride == 6 || triangle_stride == 12);

    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_FALSE);
    assert(cbtShapeGetType(shape_handle) == CBT_SHAPE_TYPE_TRIANGLE_MESH);

    const size_t vertices_size = num_vertices * 3 * sizeof(float);
    const size_t indices_size = num_triangles * triangle_stride;
    const size_t vertices_size_aligned = (vertices_size + 15) & ~15;
    const size_t indices_size_aligned = (indices_size + 15) & ~15;

    auto mem = (uint8_t*)btAlignedAlloc(vertices_size_aligned + indices_size_aligned, 16);

    btIndexedMesh indexed_mesh;
    indexed_mesh.m_numTriangles = num_triangles;
    indexed_mesh.m_triangleIndexBase = mem;
    indexed_mesh.m_triangleIndexStride = triangle_stride;
    indexed_mesh.m_numVertices = num_vertices;
    indexed_mesh.m_vertexBase = mem + indices_size_aligned;
    indexed_mesh.m_vertexStride = 3 * sizeof(float);

    for (int i = 0; i < num_vertices; ++i) {
        const float* src_vertex = (const float*)((uint8_t*)vertices_base + i * vertex_stride);
        float* dst_vertex = (float*)((uint8_t*)indexed_mesh.m_vertexBase + i * 3 * sizeof(float));
        dst_vertex[0] = src_vertex[0];
        dst_vertex[1] = src_vertex[1];
        dst_vertex[2] = src_vertex[2];
    }
    memcpy((void*)indexed_mesh.m_triangleIndexBase, triangles_base, indices_size);

    auto mesh_interface = (btTriangleIndexVertexArray*)((uint8_t*)shape_handle + sizeof(btBvhTriangleMeshShape));
    mesh_interface->addIndexedMesh(
        indexed_mesh,
        triangle_stride == 12 ? PHY_INTEGER : (triangle_stride == 6 ? PHY_SHORT : PHY_UCHAR)
    );
}

CbtBool cbtShapeIsPolyhedral(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isPolyhedral() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConvex2d(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isConvex2d() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConvex(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isConvex() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsNonMoving(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isNonMoving() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsConcave(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isConcave() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtShapeIsCompound(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->isCompound() ? CBT_TRUE : CBT_FALSE;
}

void cbtShapeCalculateLocalInertia(CbtShapeHandle shape_handle, float mass, CbtVector3 inertia) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(mass > 0.0);
    auto shape = (btCollisionShape*)shape_handle;
    btVector3 ine;
    shape->calculateLocalInertia(mass, ine);
    inertia[0] = ine.x();
    inertia[1] = ine.y();
    inertia[2] = ine.z();
}

void cbtShapeSetUserPointer(CbtShapeHandle shape_handle, void* user_pointer) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    shape->setUserPointer(user_pointer);
}

void* cbtShapeGetUserPointer(CbtShapeHandle shape_handle) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    auto shape = (btCollisionShape*)shape_handle;
    return shape->getUserPointer();
}

void cbtShapeSetUserIndex(CbtShapeHandle shape_handle, int slot, int user_index) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(slot == 0 || slot == 1);
    auto shape = (btCollisionShape*)shape_handle;
    if (slot == 0) {
        shape->setUserIndex(user_index);
    } else {
        shape->setUserIndex2(user_index);
    }
}

int cbtShapeGetUserIndex(CbtShapeHandle shape_handle, int slot) {
    assert(shape_handle && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    assert(slot == 0 || slot == 1);
    auto shape = (btCollisionShape*)shape_handle;
    if (slot == 0) {
        return shape->getUserIndex();
    }
    return shape->getUserIndex2();
}

static_assert((sizeof(btRigidBody) % 16) == 0, "sizeof(btRigidBody) is not multiple of 16");
static_assert((sizeof(btDefaultMotionState) % 16) == 0, "sizeof(btDefaultMotionState) is not multiple of 16");
static_assert(
    ((sizeof(btRigidBody) + sizeof(btDefaultMotionState)) % 16) == 0,
    "sizeof(btRigidBody) + sizeof(btDefaultMotionState) is not multiple of 16"
);

CbtBodyHandle cbtBodyAllocate(void) {
    auto base = (uint64_t*)btAlignedAlloc(sizeof(btRigidBody) + sizeof(btDefaultMotionState), 16);
    // Set vtable to 0. This means that body is not created.
    base[0] = 0;
    return (CbtBodyHandle)base;
}

void cbtBodyDeallocate(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_FALSE);
    btAlignedFree(body_handle);
}

void cbtBodyAllocateBatch(unsigned int num, CbtBodyHandle* body_handles) {
    assert(num > 0 && body_handles);
    const size_t element_size = sizeof(btRigidBody) + sizeof(btDefaultMotionState);
    uint8_t* base = (uint8_t*)btAlignedAlloc(num * element_size, 16);
    for (unsigned int i = 0; i < num; ++i) {
        body_handles[i] = (CbtBodyHandle)(base + i * element_size);
        // Set vtable to 0. This means that body is not created.
        ((uint64_t*)body_handles[i])[0] = 0;
    }
}

void cbtBodyDeallocateBatch(unsigned int num, CbtBodyHandle* body_handles) {
    assert(num > 0 && body_handles);
#ifdef _DEBUG
    for (unsigned int i = 0; i < num; ++i) {
        assert(cbtBodyIsCreated(body_handles[i]) == CBT_FALSE);
    }
#endif
    // NOTE(mziulek): All handles must come from a single 'batch'.
    btAlignedFree(body_handles[0]);
}

void cbtBodyCreate(
    CbtBodyHandle body_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
) {
    assert(body_handle && shape_handle);
    assert(transform && mass >= 0.0);
    assert(cbtBodyIsCreated(body_handle) == CBT_FALSE);
    assert(cbtShapeIsCreated(shape_handle) == CBT_TRUE);

    auto shape = (btCollisionShape*)shape_handle;

    void* body_mem = (void*)body_handle;
    void* motion_state_mem = (void*)((uint8_t*)body_handle + sizeof(btRigidBody));

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
    new (body_mem) btRigidBody(info);
}

void cbtBodyDestroy(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);

    auto body = (btRigidBody*)body_handle;
    auto motion_state = (btDefaultMotionState*)((uint8_t*)body_handle + sizeof(btRigidBody));

    motion_state->~btDefaultMotionState();
    body->~btRigidBody();

    // Set vtable to 0, this means that object is not created.
    ((uint64_t*)body)[0] = 0;
}

CbtBool cbtBodyIsCreated(CbtBodyHandle body_handle) {
    assert(body_handle);
    // vtable == 0 means that object is not created.
    return ((uint64_t*)body_handle)[0] == 0 ? CBT_FALSE : CBT_TRUE;
}

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle) {
    assert(body_handle && shape_handle);
    assert(cbtBodyIsCreated(body_handle) == CBT_TRUE && cbtShapeIsCreated(shape_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    auto shape = (btCollisionShape*)shape_handle;
    body->setCollisionShape(shape);
}

CbtShapeHandle cbtBodyGetShape(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return (CbtShapeHandle)body->getCollisionShape();
}

void cbtBodySetRestitution(CbtBodyHandle body_handle, float restitution) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setRestitution(restitution);
}

void cbtBodySetFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setFriction(friction);
}

void cbtBodySetRollingFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setRollingFriction(friction);
}

void cbtBodySetSpinningFriction(CbtBodyHandle body_handle, float friction) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setSpinningFriction(friction);
}

void cbtBodySetAnisotropicFriction(CbtBodyHandle body_handle, const CbtVector3 friction, int mode) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(friction);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setAnisotropicFriction(btVector3(friction[0], friction[1], friction[2]), mode);
}

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle body_handle, float stiffness, float damping) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setContactStiffnessAndDamping(stiffness, damping);
}

void cbtBodySetMassProps(CbtBodyHandle body_handle, float mass, const CbtVector3 inertia) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(inertia);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setMassProps(mass, btVector3(inertia[0], inertia[1], inertia[2]));
}

void cbtBodySetDamping(CbtBodyHandle body_handle, float linear, float angular) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setDamping(linear, angular);
}

void cbtBodySetLinearVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(velocity);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setLinearVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetAngularVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(velocity);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setAngularVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
}

void cbtBodySetLinearFactor(CbtBodyHandle body_handle, const CbtVector3 factor) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(factor);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setLinearFactor(btVector3(factor[0], factor[1], factor[2]));
}

void cbtBodySetAngularFactor(CbtBodyHandle body_handle, const CbtVector3 factor) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(factor);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setAngularFactor(btVector3(factor[0], factor[1], factor[2]));
}

void cbtBodyApplyCentralForce(CbtBodyHandle body_handle, const CbtVector3 force) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(force);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyCentralForce(btVector3(force[0], force[1], force[2]));
}

void cbtBodyApplyCentralImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(impulse);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyCentralImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

void cbtBodyApplyForce(CbtBodyHandle body_handle, const CbtVector3 force, const CbtVector3 rel_pos) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(force && rel_pos);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyForce(btVector3(force[0], force[1], force[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyClearForces(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->clearForces();
}

void cbtBodyApplyImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse, const CbtVector3 rel_pos) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(impulse && rel_pos);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyImpulse(btVector3(impulse[0], impulse[1], impulse[2]), btVector3(rel_pos[0], rel_pos[1], rel_pos[2]));
}

void cbtBodyApplyTorque(CbtBodyHandle body_handle, const CbtVector3 torque) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(torque);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyTorque(btVector3(torque[0], torque[1], torque[2]));
}

void cbtBodyApplyTorqueImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(impulse);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->applyTorqueImpulse(btVector3(impulse[0], impulse[1], impulse[2]));
}

float cbtBodyGetRestitution(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getRestitution();
}

float cbtBodyGetFriction(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getFriction();
}

float cbtBodyGetRollingFriction(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getRollingFriction();
}

float cbtBodyGetSpinningFriction(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getSpinningFriction();
}

void cbtBodyGetAnisotropicFriction(CbtBodyHandle body_handle, CbtVector3 friction) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(friction);
    btRigidBody* body = (btRigidBody*)body_handle;
    const btVector3& f = body->getAnisotropicFriction();
    friction[0] = f.x();
    friction[1] = f.y();
    friction[2] = f.z();
}

float cbtBodyGetContactStiffness(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getContactStiffness();
}

float cbtBodyGetContactDamping(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getContactDamping();
}

float cbtBodyGetMass(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getMass();
}

float cbtBodyGetLinearDamping(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getLinearDamping();
}

float cbtBodyGetAngularDamping(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getAngularDamping();
}

void cbtBodyGetLinearVelocity(CbtBodyHandle body_handle, CbtVector3 velocity) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(velocity);
    btRigidBody* body = (btRigidBody*)body_handle;
    const btVector3& vel = body->getLinearVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

void cbtBodyGetAngularVelocity(CbtBodyHandle body_handle, CbtVector3 velocity) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(body_handle && velocity);
    btRigidBody* body = (btRigidBody*)body_handle;
    const btVector3& vel = body->getAngularVelocity();
    velocity[0] = vel.x();
    velocity[1] = vel.y();
    velocity[2] = vel.z();
}

CbtBool cbtBodyIsStatic(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->isStaticObject() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsKinematic(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->isKinematicObject() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsStaticOrKinematic(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->isStaticOrKinematicObject() ? CBT_TRUE : CBT_FALSE;
}

float cbtBodyGetDeactivationTime(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(body_handle);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getDeactivationTime();
}

void cbtBodySetDeactivationTime(CbtBodyHandle body_handle, float time) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->setDeactivationTime(time);
}

int cbtBodyGetActivationState(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getActivationState();
}

void cbtBodySetActivationState(CbtBodyHandle body_handle, int state) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->setActivationState(state);
}

void cbtBodyForceActivationState(CbtBodyHandle body_handle, int state) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->forceActivationState(state);
}

CbtBool cbtBodyIsActive(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->isActive() ? CBT_TRUE : CBT_FALSE;
}

CbtBool cbtBodyIsInWorld(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->isInWorld() ? CBT_TRUE : CBT_FALSE;
}

void cbtBodySetUserPointer(CbtBodyHandle body_handle, void* user_pointer) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    body->setUserPointer(user_pointer);
}

void* cbtBodyGetUserPointer(CbtBodyHandle body_handle) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
    return body->getUserPointer();
}

void cbtBodySetUserIndex(CbtBodyHandle body_handle, int slot, int user_index) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(slot >= 0 && slot <= 2);
    btRigidBody* body = (btRigidBody*)body_handle;
    if (slot == 0) {
        body->setUserIndex(user_index);
    } else if (slot == 1) {
        body->setUserIndex2(user_index);
    } else {
        body->setUserIndex3(user_index);
    }
}

int cbtBodyGetUserIndex(CbtBodyHandle body_handle, int slot) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(slot >= 0 && slot <= 2);
    btRigidBody* body = (btRigidBody*)body_handle;
    if (slot == 0) {
        return body->getUserIndex();
    }
    if (slot == 1) {
        return body->getUserIndex2();
    }
    return body->getUserIndex3();
}

void cbtBodySetCenterOfMassTransform(CbtBodyHandle body_handle, const CbtVector3 transform[4]) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    btRigidBody* body = (btRigidBody*)body_handle;
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
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(transform);
    btRigidBody* body = (btRigidBody*)body_handle;

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
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(position);
    btRigidBody* body = (btRigidBody*)body_handle;

    const btTransform& trans = body->getCenterOfMassTransform();
    const btVector3& origin = trans.getOrigin();

    position[0] = origin.x();
    position[1] = origin.y();
    position[2] = origin.z();
}

void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]) {
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(transform);
    btRigidBody* body = (btRigidBody*)body_handle;

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
    assert(body_handle && cbtBodyIsCreated(body_handle) == CBT_TRUE);
    assert(transform);
    btRigidBody* body = (btRigidBody*)body_handle;

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

CbtConstraintHandle cbtConAllocate(int con_type) {
    size_t size = 0;
    switch (con_type) {
        case CBT_CONSTRAINT_TYPE_POINT2POINT: size = sizeof(btPoint2PointConstraint); break;
        case CBT_CONSTRAINT_TYPE_HINGE: size = sizeof(btHingeConstraint); break;
        case CBT_CONSTRAINT_TYPE_CONETWIST: size = sizeof(btConeTwistConstraint); break;
        case CBT_CONSTRAINT_TYPE_D6: size = sizeof(btGeneric6DofConstraint); break;
        case CBT_CONSTRAINT_TYPE_SLIDER: size = sizeof(btSliderConstraint); break;
        case CBT_CONSTRAINT_TYPE_CONTACT: size = sizeof(btContactConstraint); break;
        case CBT_CONSTRAINT_TYPE_D6_SPRING: size = sizeof(btGeneric6DofSpringConstraint); break;
        case CBT_CONSTRAINT_TYPE_GEAR: size = sizeof(btGearConstraint); break;
        case CBT_CONSTRAINT_TYPE_FIXED: size = sizeof(btFixedConstraint); break;
        case CBT_CONSTRAINT_TYPE_D6_SPRING_2: size = sizeof(btGeneric6DofSpring2Constraint); break;
        default: assert(0);
    }
    auto constraint = (int*)btAlignedAlloc(size, 16);
    // Set vtable to 0, this means that object is not created.
    constraint[0] = 0; 
    constraint[1] = 0;
    // btTypedObject::m_objectType field is just after vtable (offset: 8).
    constraint[2] = con_type; 
    return (CbtConstraintHandle)constraint;
}

void cbtConDeallocate(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_FALSE);
    btAlignedFree(con_handle);
}

void cbtConDestroy(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto constraint = (btTypedConstraint*)con_handle;
    constraint->~btTypedConstraint();
    // Set vtable to 0, this means that object is not created.
    ((uint64_t*)con_handle)[0] = 0;
}

CbtBool cbtConIsCreated(CbtConstraintHandle con_handle) {
    assert(con_handle);
    // vtable == 0 means that object is not created.
    return ((uint64_t*)con_handle)[0] == 0 ? CBT_FALSE : CBT_TRUE;
}

int cbtConGetType(CbtConstraintHandle con_handle) {
    assert(con_handle);
    auto constraint = (int*)con_handle;
    // btTypedObject::m_objectType field is just after vtable (offset: 8).
    // This function works even for not yet created shapes (cbtConAllocate sets the type).
    return constraint[2];
}

void cbtConSetEnabled(CbtConstraintHandle con_handle, CbtBool enabled) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(enabled == CBT_FALSE || enabled == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    con->setEnabled(enabled == CBT_FALSE ? false : true);
}

CbtBool cbtConIsEnabled(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    return con->isEnabled() ? CBT_TRUE : CBT_FALSE;
}

CbtBodyHandle cbtConGetBodyA(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    return (CbtBodyHandle)&con->getRigidBodyA();
}

CbtBodyHandle cbtConGetBodyB(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    return (CbtBodyHandle)&con->getRigidBodyB();
}

void cbtConSetBreakingImpulseThreshold(CbtConstraintHandle con_handle, float threshold) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    con->setBreakingImpulseThreshold(threshold);
}

float cbtConGetBreakingImpulseThreshold(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    return con->getBreakingImpulseThreshold();
}

void cbtConSetDebugDrawSize(CbtConstraintHandle con_handle, float size) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    con->setDbgDrawSize(size);
}

float cbtConGetDebugDrawSize(CbtConstraintHandle con_handle) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    auto con = (btTypedConstraint*)con_handle;
    return con->getDbgDrawSize();
}

void cbtConPoint2PointCreate(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b
) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_FALSE);
    assert(body_handle_a && cbtBodyIsCreated(body_handle_a) == CBT_TRUE);
    assert(body_handle_b && cbtBodyIsCreated(body_handle_b) == CBT_TRUE);

    btRigidBody* body_a = (btRigidBody*)body_handle_a;
    btRigidBody* body_b = (btRigidBody*)body_handle_b;
    new (con_handle) btPoint2PointConstraint(
        *body_a,
        *body_b,
        btVector3(pivot_a[0], pivot_a[1], pivot_a[2]),
        btVector3(pivot_b[0], pivot_b[1], pivot_b[2])
    );
}

void cbtConPoint2PointSetPivotA(CbtConstraintHandle con_handle, const CbtVector3 pivot) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(cbtConGetType(con_handle) == CBT_CONSTRAINT_TYPE_POINT2POINT);
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)con_handle;
    con->setPivotA(btVector3(pivot[0], pivot[1], pivot[2]));
}

void cbtConPoint2PointSetPivotB(CbtConstraintHandle con_handle, const CbtVector3 pivot) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(cbtConGetType(con_handle) == CBT_CONSTRAINT_TYPE_POINT2POINT);
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)con_handle;
    con->setPivotB(btVector3(pivot[0], pivot[1], pivot[2]));
}

void cbtConPoint2PointSetTau(CbtConstraintHandle con_handle, float tau) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(cbtConGetType(con_handle) == CBT_CONSTRAINT_TYPE_POINT2POINT);
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)con_handle;
    con->m_setting.m_tau = tau;
}

void cbtConPoint2PointSetDamping(CbtConstraintHandle con_handle, float damping) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(cbtConGetType(con_handle) == CBT_CONSTRAINT_TYPE_POINT2POINT);
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)con_handle;
    con->m_setting.m_damping = damping;
}

void cbtConPoint2PointSetImpulseClamp(CbtConstraintHandle con_handle, float impulse_clamp) {
    assert(con_handle && cbtConIsCreated(con_handle) == CBT_TRUE);
    assert(cbtConGetType(con_handle) == CBT_CONSTRAINT_TYPE_POINT2POINT);
    btPoint2PointConstraint* con = (btPoint2PointConstraint*)con_handle;
    con->m_setting.m_impulseClamp = impulse_clamp;
}
