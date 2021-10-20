#pragma once

#define CBT_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

#define CBT_COLLISION_FILTER_DEFAULT 1
#define CBT_COLLISION_FILTER_STATIC 2
#define CBT_COLLISION_FILTER_KINEMATIC 4
#define CBT_COLLISION_FILTER_DEBRIS 8
#define CBT_COLLISION_FILTER_SENSOR_TRIGGER 16
#define CBT_COLLISION_FILTER_CHARACTER 32
#define CBT_COLLISION_FILTER_ALL -1

// 'mode' for cbtBodySetAnisotropicFriction
#define CBT_ANISOTROPIC_FRICTION_DISABLED 0
#define CBT_ANISOTROPIC_FRICTION 1
#define CBT_ANISOTROPIC_ROLLING_FRICTION 2

typedef float CbtVector3[3];

#ifdef __cplusplus
extern "C" {
#endif

CBT_DECLARE_HANDLE(CbtWorldHandle);
CBT_DECLARE_HANDLE(CbtShapeHandle);
CBT_DECLARE_HANDLE(CbtBodyHandle);

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
    float closest_hit_fraction;
    CbtBodyHandle body;
    int collision_filter_group;
    int collision_filter_mask;
    unsigned int flags;
} cbtRayCastResult;

CbtWorldHandle cbtWorldCreate(void);
void cbtWorldDestroy(CbtWorldHandle handle);
void cbtWorldSetGravity(CbtWorldHandle handle, const CbtVector3 gravity);
int cbtWorldStepSimulation(CbtWorldHandle handle, float time_step, int max_sub_steps, float fixed_time_step);

void cbtWorldDebugSetCallbacks(CbtWorldHandle handle, const CbtDebugDrawCallbacks* callbacks);
void cbtWorldDebugDraw(CbtWorldHandle handle);
void cbtWorldDebugDrawLine(CbtWorldHandle handle, const CbtVector3 p0, const CbtVector3 p1, const CbtVector3 color);
void cbtWorldDebugDrawSphere(CbtWorldHandle handle, const CbtVector3 position, float radius, const CbtVector3 color);

CbtShapeHandle cbtShapeCreateBox(const CbtVector3 half_extents);
CbtShapeHandle cbtShapeCreateSphere(float radius);
CbtShapeHandle cbtShapeCreatePlane(const CbtVector3 normal, float distance);
CbtShapeHandle cbtShapeCreateCapsuleX(float radius, float height);
CbtShapeHandle cbtShapeCreateCapsuleY(float radius, float height);
CbtShapeHandle cbtShapeCreateCapsuleZ(float radius, float height);
void cbtShapeDestroy(CbtShapeHandle handle);
int cbtShapeGetType(CbtShapeHandle handle);

CbtBodyHandle cbtBodyCreate(
    CbtWorldHandle world_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
);
void cbtBodyDestroy(CbtWorldHandle world_handle, CbtBodyHandle body_handle);

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

void cbtBodyGetGraphicsTransform(CbtBodyHandle handle, CbtVector3 transform[4]);

#ifdef __cplusplus
}
#endif
