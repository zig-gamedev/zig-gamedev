#pragma once

#define CBT_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

// cbtRayTestClosest
#define CBT_COLLISION_FILTER_DEFAULT 1
#define CBT_COLLISION_FILTER_STATIC 2
#define CBT_COLLISION_FILTER_KINEMATIC 4
#define CBT_COLLISION_FILTER_DEBRIS 8
#define CBT_COLLISION_FILTER_SENSOR_TRIGGER 16
#define CBT_COLLISION_FILTER_CHARACTER 32
#define CBT_COLLISION_FILTER_ALL -1

// CbtRayCastResult::flags
#define CBT_RAYCAST_FLAG_NONE 0
#define CBT_RAYCAST_FLAG_TRIMESH_SKIP_BACKFACES 1
#define CBT_RAYCAST_FLAG_TRIMESH_KEEP_UNFLIPPED_NORMALS 2
#define CBT_RAYCAST_FLAG_USE_SUB_SIMPLEX_CONVEX_TEST 4 // default, faster but less accurate
#define CBT_RAYCAST_FLAG_USE_USE_GJK_CONVEX_TEST 8

// cbtBodySetAnisotropicFriction
#define CBT_ANISOTROPIC_FRICTION_DISABLED 0
#define CBT_ANISOTROPIC_FRICTION 1
#define CBT_ANISOTROPIC_ROLLING_FRICTION 2

// cbtShapeGetType
#define CBT_SHAPE_TYPE_BOX 0
#define CBT_SHAPE_TYPE_BOX_2D 17
#define CBT_SHAPE_TYPE_SPHERE 8
#define CBT_SHAPE_TYPE_CAPSULE 10
#define CBT_SHAPE_TYPE_CONE 11
#define CBT_SHAPE_TYPE_CYLINDER 13
#define CBT_SHAPE_TYPE_STATIC_PLANE 28

#define CBT_CONSTRAINT_TYPE_POINT2POINT 3
#define CBT_CONSTRAINT_TYPE_HINGE 4
#define CBT_CONSTRAINT_TYPE_CONETWIST 5
#define CBT_CONSTRAINT_TYPE_D6 6
#define CBT_CONSTRAINT_TYPE_SLIDER 7
#define CBT_CONSTRAINT_TYPE_CONTACT 8
#define CBT_CONSTRAINT_TYPE_D6_SPRING 9
#define CBT_CONSTRAINT_TYPE_GEAR 10
#define CBT_CONSTRAINT_TYPE_FIXED 11
#define CBT_CONSTRAINT_TYPE_D6_SPRING_2 12

// cbtBodyGetActivationState, cbtBodySetActivationState
#define CBT_ACTIVE_TAG 1
#define CBT_ISLAND_SLEEPING 2
#define CBT_DISABLE_DEACTIVATION 4
#define CBT_DISABLE_SIMULATION 5

// cbtShapeCreateCapsule, cbtShapeCreateCylinder, cbtShapeCreateCone
#define CBT_AXIS_X 0
#define CBT_AXIS_Y 1
#define CBT_AXIS_Z 2

#define CBT_FALSE 0
#define CBT_TRUE 1

typedef float CbtVector3[3];
typedef int CbtBool;

#ifdef __cplusplus
extern "C" {
#endif

CBT_DECLARE_HANDLE(CbtWorldHandle);
CBT_DECLARE_HANDLE(CbtShapeHandle);
CBT_DECLARE_HANDLE(CbtBodyHandle);
CBT_DECLARE_HANDLE(CbtConstraintHandle);

typedef void (*CbtDrawLineCallback)(const CbtVector3 p0, const CbtVector3 p1, const CbtVector3 color, void* user_data);
typedef void (*CbtDrawContactPointCallback)(
    const CbtVector3 point,
    const CbtVector3 normal,
    float distance,
    int life_time,
    const CbtVector3 color,
    void* user_data
);
typedef void (*CbtReportErrorWarningCallback)(const char* str, void* user_data);

typedef struct CbtDebugDrawCallbacks {
    CbtDrawLineCallback drawLine;
    CbtDrawContactPointCallback drawContactPoint;
    CbtReportErrorWarningCallback reportErrorWarning;
    void* user_data;
} CbtDebugDrawCallbacks;

typedef struct CbtRayCastResult {
    CbtVector3 hit_normal_world;
    CbtVector3 hit_point_world;
    float hit_fraction;
    CbtBodyHandle body;
} CbtRayCastResult;

//
// World
//
CbtWorldHandle cbtWorldCreate(void);
void cbtWorldDestroy(CbtWorldHandle handle);
void cbtWorldSetGravity(CbtWorldHandle handle, const CbtVector3 gravity);
int cbtWorldStepSimulation(CbtWorldHandle handle, float time_step, int max_sub_steps, float fixed_time_step);

void cbtWorldAddBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle);
void cbtWorldAddConstraint(
    CbtWorldHandle world_handle,
    CbtConstraintHandle constraint_handle,
    CbtBool disable_collision_between_linked_bodies
);

void cbtWorldRemoveBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle);
void cbtWorldRemoveConstraint(CbtWorldHandle world_handle, CbtConstraintHandle constraint_handle);

// Returns CBT_TRUE when hits something, CBT_FALSE otherwise
CbtBool cbtRayTestClosest(
    CbtWorldHandle handle,
    const CbtVector3 ray_from_world,
    const CbtVector3 ray_to_world,
    int collision_filter_group,
    int collision_filter_mask,
    unsigned int flags,
    CbtRayCastResult* result
);

void cbtWorldDebugSetCallbacks(CbtWorldHandle handle, const CbtDebugDrawCallbacks* callbacks);
void cbtWorldDebugDraw(CbtWorldHandle handle);
void cbtWorldDebugDrawLine(CbtWorldHandle handle, const CbtVector3 p0, const CbtVector3 p1, const CbtVector3 color);
void cbtWorldDebugDrawSphere(CbtWorldHandle handle, const CbtVector3 position, float radius, const CbtVector3 color);

//
// Shape
//
CbtShapeHandle cbtShapeAllocate(int shape_type);
void cbtShapeDeallocate(CbtShapeHandle shape_handle);

void cbtShapeDestroy(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsCreated(CbtShapeHandle shape_handle);
int cbtShapeGetType(CbtShapeHandle shape_handle);

void cbtShapeBoxCreate(CbtShapeHandle shape_handle, const CbtVector3 half_extents);

void cbtShapeBox2dCreate(CbtShapeHandle shape_handle, float x_half_extent, float y_half_extent);

void cbtShapeSphereCreate(CbtShapeHandle shape_handle, float radius);
//void cbtShapeSphereSetUnscaledRadius(CbtShapeHandle shape_handle, float radius);
//float cbtShapeSphereGetRadius(CbtShapeHandle shape_handle);

void cbtShapeStaticPlaneCreate(CbtShapeHandle shape_handle, const CbtVector3 normal, float distance);

void cbtShapeCapsuleCreate(CbtShapeHandle shape_handle, float radius, float height, int axis);
//int cbtShapeCapsuleGetUpAxis(CbtShapeHandle shape_handle);
//float cbtShapeCapsuleGetHalfHeight(CbtShapeHandle shape_handle);
//float cbtShapeCapsuleGetRadius(CbtShapeHandle shape_handle);

void cbtShapeCylinderCreate(CbtShapeHandle shape_handle, const CbtVector3 half_extents, int axis);

void cbtShapeConeCreate(CbtShapeHandle shape_handle, float radius, float height, int axis);

CbtBool cbtShapeIsPolyhedral(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConvex2d(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConvex(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsNonMoving(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConcave(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsCompound(CbtShapeHandle shape_handle);

void cbtShapeCalculateLocalInertia(CbtShapeHandle shape_handle, float mass, CbtVector3 inertia);

void cbtShapeSetUserPointer(CbtShapeHandle shape_handle, void* user_pointer);
void* cbtShapeGetUserPointer(CbtShapeHandle shape_handle);
void cbtShapeSetUserIndex(CbtShapeHandle shape_handle, int slot, int user_index); // slot can be 0 or 1
int cbtShapeGetUserIndex(CbtShapeHandle shape_handle, int slot); // slot can be 0 or 1

//
// Body
//
void cbtBodyAllocate(unsigned int num, CbtBodyHandle* body_handles);
void cbtBodyDeallocate(unsigned int num, CbtBodyHandle* body_handles);

void cbtBodyCreate(
    CbtBodyHandle body_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
);
void cbtBodyDestroy(CbtBodyHandle body_handle);
CbtBool cbtBodyIsCreated(CbtBodyHandle body_handle);

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle);
CbtShapeHandle cbtBodyGetShape(CbtBodyHandle body_handle);

void cbtBodySetRestitution(CbtBodyHandle body_handle, float restitution);

void cbtBodySetFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetRollingFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetSpinningFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetAnisotropicFriction(CbtBodyHandle body_handle, const CbtVector3 friction, int mode);

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle body_handle, float stiffness, float damping);

void cbtBodySetMassProps(CbtBodyHandle body_handle, float mass, const CbtVector3 inertia);

void cbtBodySetDamping(CbtBodyHandle body_handle, float linear, float angular);

void cbtBodySetLinearVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity);
void cbtBodySetAngularVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity);

void cbtBodySetLinearFactor(CbtBodyHandle body_handle, const CbtVector3 factor);
void cbtBodySetAngularFactor(CbtBodyHandle body_handle, const CbtVector3 factor);

void cbtBodyApplyCentralForce(CbtBodyHandle body_handle, const CbtVector3 force);
void cbtBodyApplyCentralImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse);
void cbtBodyApplyForce(CbtBodyHandle body_handle, const CbtVector3 force, const CbtVector3 rel_pos);
void cbtBodyApplyImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse, const CbtVector3 rel_pos);
void cbtBodyApplyTorque(CbtBodyHandle body_handle, const CbtVector3 torque);
void cbtBodyApplyTorqueImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse);

void cbtBodyClearForces(CbtBodyHandle body_handle);

float cbtBodyGetRestitution(CbtBodyHandle body_handle);

float cbtBodyGetFriction(CbtBodyHandle body_handle);
float cbtBodyGetRollingFriction(CbtBodyHandle body_handle);
float cbtBodyGetSpinningFriction(CbtBodyHandle body_handle);
void cbtBodyGetAnisotropicFriction(CbtBodyHandle body_handle, CbtVector3 friction);

float cbtBodyGetContactStiffness(CbtBodyHandle body_handle);
float cbtBodyGetContactDamping(CbtBodyHandle body_handle);

float cbtBodyGetMass(CbtBodyHandle body_handle);

float cbtBodyGetLinearDamping(CbtBodyHandle body_handle);
float cbtBodyGetAngularDamping(CbtBodyHandle body_handle);

void cbtBodyGetLinearVelocity(CbtBodyHandle body_handle, CbtVector3 velocity);
void cbtBodyGetAngularVelocity(CbtBodyHandle body_handle, CbtVector3 velocity);

CbtBool cbtBodyIsStatic(CbtBodyHandle body_handle);
CbtBool cbtBodyIsKinematic(CbtBodyHandle body_handle);
CbtBool cbtBodyIsStaticOrKinematic(CbtBodyHandle body_handle);

float cbtBodyGetDeactivationTime(CbtBodyHandle body_handle);
void cbtBodySetDeactivationTime(CbtBodyHandle body_handle, float time);
int cbtBodyGetActivationState(CbtBodyHandle body_handle);
void cbtBodySetActivationState(CbtBodyHandle body_handle, int state);
void cbtBodyForceActivationState(CbtBodyHandle body_handle, int state);
CbtBool cbtBodyIsActive(CbtBodyHandle body_handle);
CbtBool cbtBodyIsInWorld(CbtBodyHandle body_handle);

void cbtBodySetUserPointer(CbtBodyHandle body_handle, void* user_pointer);
void* cbtBodyGetUserPointer(CbtBodyHandle body_handle, int slot);
void cbtBodySetUserIndex(CbtBodyHandle body_handle, int slot, int user_index); // slot can be 0, 1 or 2
int cbtBodyGetUserIndex(CbtBodyHandle body_handle, int slot); // slot can be 0, 1 or 2

void cbtBodySetCenterOfMassTransform(CbtBodyHandle body_handle, const CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassPosition(CbtBodyHandle body_handle, CbtVector3 position);
void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);
void cbtBodyGetGraphicsWorldTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);

//
// Constraints
//
CbtBodyHandle cbtConGetFixedBody(void);

CbtConstraintHandle cbtConAllocate(int con_type);
void cbtConDeallocate(CbtConstraintHandle con_handle);

void cbtConDestroy(CbtConstraintHandle con_handle);
CbtBool cbtConIsCreated(CbtConstraintHandle con_handle);
int cbtConGetType(CbtConstraintHandle con_handle);

void cbtConCreatePoint2Point(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b
);
void cbtConPoint2PointSetPivotA(CbtConstraintHandle con_handle, const CbtVector3 pivot);
void cbtConPoint2PointSetPivotB(CbtConstraintHandle con_handle, const CbtVector3 pivot);
void cbtConPoint2PointSetTau(CbtConstraintHandle con_handle, float tau);
void cbtConPoint2PointSetDamping(CbtConstraintHandle con_handle, float damping);
void cbtConPoint2PointSetImpulseClamp(CbtConstraintHandle con_handle, float impulse_clamp);

#ifdef __cplusplus
}
#endif
