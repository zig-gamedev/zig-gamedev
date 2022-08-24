#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>

#define JPH_CAPI

// JPH_JobSystem_Create
#define JPH_MAX_PHYSICS_JOBS     2048
#define JPH_MAX_PHYSICS_BARRIERS 8

// JPH_ShapeType
#define JPH_SHAPE_TYPE_CONVEX       0
#define JPH_SHAPE_TYPE_COMPOUND     1
#define JPH_SHAPE_TYPE_DECORATED    2
#define JPH_SHAPE_TYPE_MESH         3
#define JPH_SHAPE_TYPE_HEIGHT_FIELD 4
#define JPH_SHAPE_TYPE_USER1        5
#define JPH_SHAPE_TYPE_USER2        6
#define JPH_SHAPE_TYPE_USER3        7
#define JPH_SHAPE_TYPE_USER4        8

// JPH_ShapeSubType
#define JPH_SHAPE_SUB_TYPE_SPHERE                0
#define JPH_SHAPE_SUB_TYPE_BOX                   1
#define JPH_SHAPE_SUB_TYPE_TRIANGLE              2
#define JPH_SHAPE_SUB_TYPE_CAPSULE               3
#define JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE       4
#define JPH_SHAPE_SUB_TYPE_CYLINDER              5
#define JPH_SHAPE_SUB_TYPE_CONVEX_HULL           6
#define JPH_SHAPE_SUB_TYPE_STATIC_COMPOUND       7
#define JPH_SHAPE_SUB_TYPE_MUTABLE_COMPOUND      8
#define JPH_SHAPE_SUB_TYPE_ROTATED_TRANSLATED    9
#define JPH_SHAPE_SUB_TYPE_SCALED                10
#define JPH_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS 11
#define JPH_SHAPE_SUB_TYPE_MESH                  12
#define JPH_SHAPE_SUB_TYPE_HEIGHT_FIELD          13
#define JPH_SHAPE_SUB_TYPE_USER1                 14
#define JPH_SHAPE_SUB_TYPE_USER2                 15
#define JPH_SHAPE_SUB_TYPE_USER3                 16
#define JPH_SHAPE_SUB_TYPE_USER4                 17
#define JPH_SHAPE_SUB_TYPE_USER5                 18
#define JPH_SHAPE_SUB_TYPE_USER6                 19
#define JPH_SHAPE_SUB_TYPE_USER7                 20
#define JPH_SHAPE_SUB_TYPE_USER8                 21

// JPH_MotionType
#define JPH_MOTION_TYPE_STATIC    0
#define JPH_MOTION_TYPE_KINEMATIC 1
#define JPH_MOTION_TYPE_DYNAMIC   2

// JPH_MotionQuality
#define JPH_MOTION_QUALITY_DISCRETE    0
#define JPH_MOTION_QUALITY_LINEAR_CAST 1

// JPH_OverrideMassProperties
#define JPH_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA     0
#define JPH_OVERRIDE_MASS_PROPS_CALC_INERTIA          1
#define JPH_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED 2

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t    JPH_ObjectLayer;
typedef uint8_t     JPH_BroadPhaseLayer;
typedef uint32_t    JPH_BodyID;
typedef uint32_t    JPH_CollisionGroupID;
typedef uint32_t    JPH_CollisionSubGroupID;
typedef uint8_t     JPH_ShapeType;
typedef uint8_t     JPH_ShapeSubType;
typedef uint8_t     JPH_MotionType;
typedef uint8_t     JPH_MotionQuality;
typedef uint8_t     JPH_OverrideMassProperties;

typedef struct JPH_PhysicsSystem    JPH_PhysicsSystem;
typedef struct JPH_Shape            JPH_Shape;
typedef struct JPH_PhysicsMaterial  JPH_PhysicsMaterial;
typedef struct JPH_TempAllocator    JPH_TempAllocator;
typedef struct JPH_JobSystem        JPH_JobSystem;
typedef struct JPH_Body             JPH_Body;
typedef struct JPH_BodyInterface    JPH_BodyInterface;

typedef struct JPH_ShapeSettings        JPH_ShapeSettings;
typedef struct JPH_ConvexShapeSettings  JPH_ConvexShapeSettings;
typedef struct JPH_BoxShapeSettings     JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings  JPH_SphereShapeSettings;
typedef struct JPH_GroupFilter          JPH_GroupFilter;

// NOTE: Needs to be kept in sync with JPH::MassProperties
typedef struct JPH_MassProperties
{
    float              mass;
    alignas(16) float  inertia[16];
} JPH_MassProperties;

// NOTE: Needs to be kept in sync with JPH::CollisionGroup
typedef struct JPH_CollisionGroup
{
    const JPH_GroupFilter *  filter;
    JPH_CollisionGroupID     group_id;
    JPH_CollisionSubGroupID  sub_group_id;
} JPH_CollisionGroup;

JPH_CAPI JPH_CollisionGroup
JPH_CollisionGroup_InitDefault(void);

// NOTE: Needs to be kept in sync with JPH::BodyCreationSettings
typedef struct JPH_BodyCreationSettings
{
    alignas(16) float           position[3];
    alignas(16) float           rotation[4];
    alignas(16) float           linear_velocity[3];
    alignas(16) float           angular_velocity[3];
    uint64_t                    user_data;
    JPH_ObjectLayer             object_layer;
    JPH_CollisionGroup          collision_group;
    JPH_MotionType              motion_type;
    bool                        allow_dynamic_or_kinematic;
    bool                        is_sensor;
    JPH_MotionQuality           motion_quality;
    bool                        allow_sleeping;
    float                       friction;
    float                       restitution;
    float                       linear_damping;
    float                       angular_damping;
    float                       max_linear_velocity;
    float                       max_angular_velocity;
    float                       gravity_factor;
    JPH_OverrideMassProperties  override_mass_properties;
    float                       inertia_multiplier;
    JPH_MassProperties          mass_properties_override;
    const void *                reserved;
    const JPH_Shape *           shape;
} JPH_BodyCreationSettings;

JPH_CAPI JPH_BodyCreationSettings
JPH_BodyCreationSettings_InitDefault(void);

JPH_CAPI JPH_BodyCreationSettings
JPH_BodyCreationSettings_Init(const JPH_Shape *in_shape,
                              const float in_position[3],
                              const float in_rotation[4],
                              JPH_MotionType in_motion_type,
                              JPH_ObjectLayer in_layer);
//--------------------------------------------------------------------------------------------------
//
// Misc
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterDefaultAllocator(void);

JPH_CAPI void
JPH_CreateFactory(void);

JPH_CAPI void
JPH_DestroyFactory(void);

JPH_CAPI void
JPH_RegisterTypes(void);

typedef bool (*JPH_ObjectLayerPairFilter)(JPH_ObjectLayer in_layer1, JPH_ObjectLayer in_layer2);
typedef bool (*JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer in_layer1, JPH_BroadPhaseLayer in_layer2);

typedef struct JPH_BroadPhaseLayerInterfaceVTable
{
    const void *        reserved0;
    const void *        reserved1;
    uint32_t            (*GetNumBroadPhaseLayers)(const void *in_self);
    JPH_BroadPhaseLayer (*GetBroadPhaseLayer)(const void *in_self, JPH_ObjectLayer in_layer);
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char *        (*GetBroadPhaseLayerName)(const void *in_self, JPH_BroadPhaseLayer in_layer);
#endif
} JPH_BroadPhaseLayerInterfaceVTable;
//--------------------------------------------------------------------------------------------------
//
// JPH_TempAllocator
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TempAllocator *
JPH_TempAllocator_Create(uint32_t in_size);

JPH_CAPI void
JPH_TempAllocator_Destroy(JPH_TempAllocator *in_allocator);
//--------------------------------------------------------------------------------------------------
//
// JPH_JobSystem
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_JobSystem *
JPH_JobSystem_Create(uint32_t in_max_jobs, uint32_t in_max_barriers, int32_t in_num_threads);

JPH_CAPI void
JPH_JobSystem_Destroy(JPH_JobSystem *in_job_system);
//--------------------------------------------------------------------------------------------------
//
// JPH_PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_PhysicsSystem *
JPH_PhysicsSystem_Create(void);

JPH_CAPI void
JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem *in_physics_system);

JPH_CAPI void
JPH_PhysicsSystem_Init(JPH_PhysicsSystem *in_physics_system,
                       uint32_t in_max_bodies,
                       uint32_t in_num_body_mutexes,
                       uint32_t in_max_body_pairs,
                       uint32_t in_max_contact_constraints,
                       const void *in_broad_phase_layer_interface,
                       JPH_ObjectVsBroadPhaseLayerFilter in_object_vs_broad_phase_layer_filter,
                       JPH_ObjectLayerPairFilter in_object_layer_pair_filter);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI JPH_BodyInterface *
JPH_PhysicsSystem_GetBodyInterface(JPH_PhysicsSystem *in_physics_system);
//--------------------------------------------------------------------------------------------------
//
// JPH_ShapeSettings
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ShapeSettings_AddRef(JPH_ShapeSettings *in_settings);

JPH_CAPI void
JPH_ShapeSettings_Release(JPH_ShapeSettings *in_settings);

JPH_CAPI uint32_t
JPH_ShapeSettings_GetRefCount(const JPH_ShapeSettings *in_settings);

JPH_CAPI JPH_Shape *
JPH_ShapeSettings_Cook(const JPH_ShapeSettings *in_settings);

JPH_CAPI uint64_t
JPH_ShapeSettings_GetUserData(const JPH_ShapeSettings *in_settings);

JPH_CAPI void
JPH_ShapeSettings_SetUserData(JPH_ShapeSettings *in_settings, uint64_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPH_ConvexShapeSettings (-> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI const JPH_PhysicsMaterial *
JPH_ConvexShapeSettings_GetMaterial(const JPH_ConvexShapeSettings *in_settings);

JPH_CAPI void
JPH_ConvexShapeSettings_SetMaterial(JPH_ConvexShapeSettings *in_settings,
                                    const JPH_PhysicsMaterial *in_material);

JPH_CAPI float
JPH_ConvexShapeSettings_GetDensity(const JPH_ConvexShapeSettings *in_settings);

JPH_CAPI void
JPH_ConvexShapeSettings_SetDensity(JPH_ConvexShapeSettings *in_settings, float in_density);
//--------------------------------------------------------------------------------------------------
//
// JPH_BoxShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BoxShapeSettings *
JPH_BoxShapeSettings_Create(const float in_half_extent[3]);

JPH_CAPI void
JPH_BoxShapeSettings_GetHalfExtent(const JPH_BoxShapeSettings *in_settings, float out_half_extent[3]);

JPH_CAPI void
JPH_BoxShapeSettings_SetHalfExtent(JPH_BoxShapeSettings *in_settings, const float in_half_extent[3]);

JPH_CAPI float
JPH_BoxShapeSettings_GetConvexRadius(const JPH_BoxShapeSettings *in_settings);

JPH_CAPI void
JPH_BoxShapeSettings_SetConvexRadius(JPH_BoxShapeSettings *in_settings, float in_convex_radius);
//--------------------------------------------------------------------------------------------------
//
// JPH_Shape
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Shape_AddRef(JPH_Shape *in_shape);

JPH_CAPI void
JPH_Shape_Release(JPH_Shape *in_shape);

JPH_CAPI uint32_t
JPH_Shape_GetRefCount(const JPH_Shape *in_shape);

JPH_CAPI JPH_ShapeType
JPH_Shape_GetType(const JPH_Shape *in_shape);

JPH_CAPI JPH_ShapeSubType
JPH_Shape_GetSubType(const JPH_Shape *in_shape);

JPH_CAPI uint64_t
JPH_Shape_GetUserData(const JPH_Shape *in_shape);

JPH_CAPI void
JPH_Shape_SetUserData(JPH_Shape *in_shape, uint64_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPH_BodyInterface
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_Body *
JPH_BodyInterface_CreateBody(JPH_BodyInterface *in_iface, const JPH_BodyCreationSettings *in_setting);
//--------------------------------------------------------------------------------------------------
//
// JPH_Body
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyID
JPH_Body_GetID(const JPH_Body *in_body);

#ifdef __cplusplus
}
#endif
