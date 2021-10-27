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
#define CBT_RAYCAST_FLAG_SKIP_BACKFACES 1
#define CBT_RAYCAST_FLAG_KEEP_UNFLIPPED_NORMALS 2
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
CbtShapeHandle cbtShapeCreateBox(const CbtVector3 half_extents);
CbtShapeHandle cbtShapeCreateBox2d(float x_half_extent, float y_half_extent);
CbtShapeHandle cbtShapeCreateSphere(float radius);
CbtShapeHandle cbtShapeCreatePlane(const CbtVector3 normal, float distance);
CbtShapeHandle cbtShapeCreateCapsule(float radius, float height, int axis);
CbtShapeHandle cbtShapeCreateCylinder(const CbtVector3 half_extents, int axis);
CbtShapeHandle cbtShapeCreateCone(float radius, float height, int axis);

CbtBool cbtShapeIsPolyhedral(CbtShapeHandle handle);
CbtBool cbtShapeIsConvex2d(CbtShapeHandle handle);
CbtBool cbtShapeIsConvex(CbtShapeHandle handle);
CbtBool cbtShapeIsNonMoving(CbtShapeHandle handle);
CbtBool cbtShapeIsConcave(CbtShapeHandle handle);
CbtBool cbtShapeIsCompound(CbtShapeHandle handle);

void cbtShapeCalculateLocalInertia(CbtShapeHandle handle, float mass, CbtVector3 inertia);

void cbtShapeSetUserPointer(CbtShapeHandle handle, void* user_pointer);
void* cbtShapeGetUserPointer(CbtShapeHandle handle);
void cbtShapeSetUserIndex(CbtShapeHandle handle, int slot, int user_index); // slot can be 0 or 1
int cbtShapeGetUserIndex(CbtShapeHandle handle, int slot);

void cbtShapeDestroy(CbtShapeHandle handle);
int cbtShapeGetType(CbtShapeHandle handle);

//
// Body
//
CbtBodyHandle cbtBodyCreate(
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
);
void cbtBodyDestroy(CbtBodyHandle body_handle);

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle);
CbtShapeHandle cbtBodyGetShape(CbtBodyHandle handle);

void cbtBodySetRestitution(CbtBodyHandle handle, float restitution);

void cbtBodySetFriction(CbtBodyHandle handle, float friction);
void cbtBodySetRollingFriction(CbtBodyHandle handle, float friction);
void cbtBodySetSpinningFriction(CbtBodyHandle handle, float friction);
void cbtBodySetAnisotropicFriction(CbtBodyHandle handle, const CbtVector3 friction, int mode);

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle handle, float stiffness, float damping);

void cbtBodySetMassProps(CbtBodyHandle handle, float mass, const CbtVector3 inertia);

void cbtBodySetDamping(CbtBodyHandle handle, float linear, float angular);

void cbtBodySetLinearVelocity(CbtBodyHandle handle, const CbtVector3 velocity);
void cbtBodySetAngularVelocity(CbtBodyHandle handle, const CbtVector3 velocity);

void cbtBodySetLinearFactor(CbtBodyHandle handle, const CbtVector3 factor);
void cbtBodySetAngularFactor(CbtBodyHandle handle, const CbtVector3 factor);

void cbtBodyApplyCentralForce(CbtBodyHandle handle, const CbtVector3 force);
void cbtBodyApplyCentralImpulse(CbtBodyHandle handle, const CbtVector3 impulse);
void cbtBodyApplyForce(CbtBodyHandle handle, const CbtVector3 force, const CbtVector3 rel_pos);
void cbtBodyApplyImpulse(CbtBodyHandle handle, const CbtVector3 impulse, const CbtVector3 rel_pos);
void cbtBodyApplyTorque(CbtBodyHandle handle, const CbtVector3 torque);
void cbtBodyApplyTorqueImpulse(CbtBodyHandle handle, const CbtVector3 impulse);

void cbtBodyClearForces(CbtBodyHandle handle);

float cbtBodyGetRestitution(CbtBodyHandle handle);

float cbtBodyGetFriction(CbtBodyHandle handle);
float cbtBodyGetRollingFriction(CbtBodyHandle handle);
float cbtBodyGetSpinningFriction(CbtBodyHandle handle);
void cbtBodyGetAnisotropicFriction(CbtBodyHandle handle, CbtVector3 friction);

float cbtBodyGetContactStiffness(CbtBodyHandle handle);
float cbtBodyGetContactDamping(CbtBodyHandle handle);

float cbtBodyGetMass(CbtBodyHandle handle);

float cbtBodyGetLinearDamping(CbtBodyHandle handle);
float cbtBodyGetAngularDamping(CbtBodyHandle handle);

void cbtBodyGetLinearVelocity(CbtBodyHandle handle, CbtVector3 velocity);
void cbtBodyGetAngularVelocity(CbtBodyHandle handle, CbtVector3 velocity);

CbtBool cbtBodyIsStatic(CbtBodyHandle handle);
CbtBool cbtBodyIsKinematic(CbtBodyHandle handle);
CbtBool cbtBodyIsStaticOrKinematic(CbtBodyHandle handle);

float cbtBodyGetDeactivationTime(CbtBodyHandle handle);
void cbtBodySetDeactivationTime(CbtBodyHandle handle, float time);
int cbtBodyGetActivationState(CbtBodyHandle handle);
void cbtBodySetActivationState(CbtBodyHandle handle, int state);
void cbtBodyForceActivationState(CbtBodyHandle handle, int state);
CbtBool cbtBodyIsActive(CbtBodyHandle handle);
CbtBool cbtBodyIsInWorld(CbtBodyHandle handle);

void cbtBodySetUserPointer(CbtBodyHandle handle, void* user_pointer);
void* cbtBodyGetUserPointer(CbtBodyHandle handle, int slot);
void cbtBodySetUserIndex(CbtBodyHandle handle, int slot, int user_index); // slot can be 0, 1 or 2
int cbtBodyGetUserIndex(CbtBodyHandle handle, int slot);


void cbtBodySetCenterOfMassTransform(CbtBodyHandle handle, const CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassTransform(CbtBodyHandle handle, CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassPosition(CbtBodyHandle handle, CbtVector3 position);
void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle handle, CbtVector3 transform[4]);
void cbtBodyGetGraphicsWorldTransform(CbtBodyHandle handle, CbtVector3 transform[4]);

//
// Constraints
//
CbtBodyHandle cbtConGetFixedBody(void);

void cbtConDestroy(CbtConstraintHandle handle);

CbtConstraintHandle cbtConCreatePoint2Point(
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b
);
void cbtConPoint2PointSetPivotA(CbtConstraintHandle handle, const CbtVector3 pivot);
void cbtConPoint2PointSetPivotB(CbtConstraintHandle handle, const CbtVector3 pivot);
void cbtConPoint2PointSetTau(CbtConstraintHandle handle, float tau);
void cbtConPoint2PointSetDamping(CbtConstraintHandle handle, float damping);
void cbtConPoint2PointSetImpulseClamp(CbtConstraintHandle handle, float impulse_clamp);

#ifdef __cplusplus
}
#endif
