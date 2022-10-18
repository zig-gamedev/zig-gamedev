#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
//--------------------------------------------------------------------------------------------------
//
// Const
//
//--------------------------------------------------------------------------------------------------
// TODO: Define this
#define JPH_CAPI

// Copied from IssueReporting.h
// Always turn on asserts in Debug mode
#if defined(_DEBUG) && !defined(JPH_ENABLE_ASSERTS)
    #define JPH_ENABLE_ASSERTS
#endif

#ifdef __cplusplus
extern "C" {
#endif

// JPH_JobSystem_Create()
enum
{
    JPH_MAX_PHYSICS_JOBS     = 2048,
    JPH_MAX_PHYSICS_BARRIERS = 8
};

typedef uint8_t JPH_ShapeType;
enum
{
    JPH_SHAPE_TYPE_CONVEX       = 0,
    JPH_SHAPE_TYPE_COMPOUND     = 1,
    JPH_SHAPE_TYPE_DECORATED    = 2,
    JPH_SHAPE_TYPE_MESH         = 3,
    JPH_SHAPE_TYPE_HEIGHT_FIELD = 4,
    JPH_SHAPE_TYPE_USER1        = 5,
    JPH_SHAPE_TYPE_USER2        = 6,
    JPH_SHAPE_TYPE_USER3        = 7,
    JPH_SHAPE_TYPE_USER4        = 8
};

typedef uint8_t JPH_ShapeSubType;
enum
{
    JPH_SHAPE_SUB_TYPE_SPHERE                = 0,
    JPH_SHAPE_SUB_TYPE_BOX                   = 1,
    JPH_SHAPE_SUB_TYPE_TRIANGLE              = 2,
    JPH_SHAPE_SUB_TYPE_CAPSULE               = 3,
    JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE       = 4,
    JPH_SHAPE_SUB_TYPE_CYLINDER              = 5,
    JPH_SHAPE_SUB_TYPE_CONVEX_HULL           = 6,
    JPH_SHAPE_SUB_TYPE_STATIC_COMPOUND       = 7,
    JPH_SHAPE_SUB_TYPE_MUTABLE_COMPOUND      = 8,
    JPH_SHAPE_SUB_TYPE_ROTATED_TRANSLATED    = 9,
    JPH_SHAPE_SUB_TYPE_SCALED                = 10,
    JPH_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS = 11,
    JPH_SHAPE_SUB_TYPE_MESH                  = 12,
    JPH_SHAPE_SUB_TYPE_HEIGHT_FIELD          = 13,
    JPH_SHAPE_SUB_TYPE_USER1                 = 14,
    JPH_SHAPE_SUB_TYPE_USER2                 = 15,
    JPH_SHAPE_SUB_TYPE_USER3                 = 16,
    JPH_SHAPE_SUB_TYPE_USER4                 = 17,
    JPH_SHAPE_SUB_TYPE_USER5                 = 18,
    JPH_SHAPE_SUB_TYPE_USER6                 = 19,
    JPH_SHAPE_SUB_TYPE_USER7                 = 20,
    JPH_SHAPE_SUB_TYPE_USER8                 = 21
};

typedef uint8_t JPH_MotionType;
enum
{
    JPH_MOTION_TYPE_STATIC    = 0,
    JPH_MOTION_TYPE_KINEMATIC = 1,
    JPH_MOTION_TYPE_DYNAMIC   = 2
};

typedef uint8_t JPH_MotionQuality;
enum
{
    JPH_MOTION_QUALITY_DISCRETE    = 0,
    JPH_MOTION_QUALITY_LINEAR_CAST = 1
};

typedef uint8_t JPH_OverrideMassProperties;
enum
{
    JPH_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA     = 0,
    JPH_OVERRIDE_MASS_PROPS_CALC_INERTIA          = 1,
    JPH_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED = 2
};

typedef enum JPH_Activation
{
    JPH_ACTIVATION_ACTIVATE      = 0,
    JPH_ACTIVATION_DONT_ACTIVATE = 1,
    _JPH_ACTIVATION_FORCEU32     = 0x7fffffff
} JPH_Activation;

typedef enum JPH_ValidateResult
{
    JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS = 0,
    JPH_VALIDATE_RESULT_ACCEPT_CONTACT      = 1,
    JPH_VALIDATE_RESULT_REJECT_CONTACT      = 2,
    JPH_VALIDATE_RESULT_REJECT_ALL_CONTACTS = 3,
    _JPH_VALIDATE_RESULT_FORCEU32           = 0x7fffffff
} JPH_ValidateResult;
//--------------------------------------------------------------------------------------------------
//
// Types
//
//--------------------------------------------------------------------------------------------------
typedef uint16_t JPH_ObjectLayer;
typedef uint8_t  JPH_BroadPhaseLayer;

// TODO: Consider using structures for IDs
typedef uint32_t JPH_BodyID;
typedef uint32_t JPH_SubShapeID;
typedef uint32_t JPH_CollisionGroupID;
typedef uint32_t JPH_CollisionSubGroupID;

typedef struct JPH_TempAllocator JPH_TempAllocator;
typedef struct JPH_JobSystem     JPH_JobSystem;
typedef struct JPH_Body          JPH_Body;
typedef struct JPH_BodyInterface JPH_BodyInterface;

// Must be 16 byte aligned
typedef void *(*JPH_AllocateFunction)(size_t in_size);
typedef void (*JPH_FreeFunction)(void *in_block);

typedef void *(*JPH_AlignedAllocateFunction)(size_t in_size, size_t in_alignment);
typedef void (*JPH_AlignedFreeFunction)(void *in_block);
//--------------------------------------------------------------------------------------------------
//
// Geometry Types
//
//--------------------------------------------------------------------------------------------------
typedef struct JPH_ShapeSettings               JPH_ShapeSettings;
typedef struct JPH_ConvexShapeSettings         JPH_ConvexShapeSettings;
typedef struct JPH_BoxShapeSettings            JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings         JPH_SphereShapeSettings;
typedef struct JPH_TriangleShapeSettings       JPH_TriangleShapeSettings;
typedef struct JPH_CapsuleShapeSettings        JPH_CapsuleShapeSettings;
typedef struct JPH_TaperedCapsuleShapeSettings JPH_TaperedCapsuleShapeSettings;
typedef struct JPH_CylinderShapeSettings       JPH_CylinderShapeSettings;
typedef struct JPH_ConvexHullShapeSettings     JPH_ConvexHullShapeSettings;

typedef bool
(*JPH_ObjectLayerPairFilter)(JPH_ObjectLayer in_layer1, JPH_ObjectLayer in_layer2);

typedef bool
(*JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer in_layer1, JPH_BroadPhaseLayer in_layer2);
//--------------------------------------------------------------------------------------------------
//
// Physics Types
//
//--------------------------------------------------------------------------------------------------
typedef struct JPH_PhysicsSystem JPH_PhysicsSystem;
typedef struct JPH_StateRecorder JPH_StateRecorder;
//--------------------------------------------------------------------------------------------------
//
// Physics/Body Types
//
//--------------------------------------------------------------------------------------------------
typedef struct JPH_MassProperties   JPH_MassProperties;
typedef struct JPH_MotionProperties JPH_MotionProperties;

typedef struct JPH_BodyCreationSettings JPH_BodyCreationSettings;
typedef struct JPH_ContactManifold      JPH_ContactManifold;
typedef struct JPH_ContactSettings      JPH_ContactSettings;
typedef struct JPH_CollideShapeResult   JPH_CollideShapeResult;

typedef struct JPH_BroadPhaseLayerInterfaceVTable JPH_BroadPhaseLayerInterfaceVTable;
typedef struct JPH_BodyActivationListenerVTable   JPH_BodyActivationListenerVTable;
typedef struct JPH_ContactListenerVTable          JPH_ContactListenerVTable;
//--------------------------------------------------------------------------------------------------
//
// Physics/Collision Types
//
//--------------------------------------------------------------------------------------------------
typedef struct JPH_Shape             JPH_Shape;
typedef struct JPH_SubShapeIDCreator JPH_SubShapeIDCreator;
typedef struct JPH_SubShapeIDPair    JPH_SubShapeIDPair;
typedef struct JPH_PhysicsMaterial   JPH_PhysicsMaterial;
typedef struct JPH_GroupFilter       JPH_GroupFilter;
typedef struct JPH_CollisionGroup    JPH_CollisionGroup;
typedef struct JPH_TransformedShape  JPH_TransformedShape;
//--------------------------------------------------------------------------------------------------
//
// Structures
//
//--------------------------------------------------------------------------------------------------
// NOTE: Needs to be kept in sync with JPH::MassProperties
struct JPH_MassProperties
{
    float             mass;
    alignas(16) float inertia[16];
};

// NOTE: Needs to be kept in sync with JPH::MotionProperties
struct JPH_MotionProperties
{
    alignas(16) float linear_velocity[4];
    alignas(16) float angular_velocity[4];
    alignas(16) float inv_inertia_diagnonal[4];
    alignas(16) float inertia_rotation[4];

    float             force[3];
    float             torque[3];
    float             inv_mass;
    float             linear_damping;
    float             angular_daming;
    float             max_linear_velocity;
    float             max_angular_velocity;
    float             gravity_factor;
    uint32_t          index_in_active_bodies;
    uint32_t          island_index;

    JPH_MotionQuality motion_quality;
    bool              allow_sleeping;

    float             reserved[13];

#ifdef JPH_ENABLE_ASSERTS
    JPH_MotionType    cached_motion_type;
#endif
};

// NOTE: Needs to be kept in sync with JPH::CollisionGroup
struct JPH_CollisionGroup
{
    const JPH_GroupFilter *  filter;
    JPH_CollisionGroupID     group_id;
    JPH_CollisionSubGroupID  sub_group_id;
};

// NOTE: Needs to be kept in sync with JPH::BodyCreationSettings
struct JPH_BodyCreationSettings
{
    alignas(16) float          position[4];
    alignas(16) float          rotation[4];
    alignas(16) float          linear_velocity[4];
    alignas(16) float          angular_velocity[4];
    uint64_t                   user_data;
    JPH_ObjectLayer            object_layer;
    JPH_CollisionGroup         collision_group;
    JPH_MotionType             motion_type;
    bool                       allow_dynamic_or_kinematic;
    bool                       is_sensor;
    JPH_MotionQuality          motion_quality;
    bool                       allow_sleeping;
    float                      friction;
    float                      restitution;
    float                      linear_damping;
    float                      angular_damping;
    float                      max_linear_velocity;
    float                      max_angular_velocity;
    float                      gravity_factor;
    JPH_OverrideMassProperties override_mass_properties;
    float                      inertia_multiplier;
    JPH_MassProperties         mass_properties_override;
    const void *               reserved;
    const JPH_Shape *          shape;
};

// NOTE: Needs to be kept in sync with JPH::SubShapeIDCreator
struct JPH_SubShapeIDCreator
{
    JPH_SubShapeID id;
    uint32_t       current_bit;
};

// NOTE: Needs to be kept in sync with JPH::SubShapeIDPair
struct JPH_SubShapeIDPair
{
    JPH_BodyID     body1_id;
    JPH_SubShapeID sub_shape1_id;
    JPH_BodyID     body2_id;
    JPH_SubShapeID sub_shape2_id;
};

// NOTE: Needs to be kept in sync with JPH::ContactManifold
struct JPH_ContactManifold
{
    alignas(16) float    world_space_normal[4];
    alignas(16) float    penetration_depth;
    JPH_SubShapeID       sub_shape1_id;
    JPH_SubShapeID       sub_shape2_id;
    alignas(16) uint32_t num_points1;
    alignas(16) float    world_space_contact_points1[64][4];
    alignas(16) uint32_t num_points2;
    alignas(16) float    world_space_contact_points2[64][4];
};

// NOTE: Needs to be kept in sync with JPH::ContactSettings
struct JPH_ContactSettings
{
    float combined_friction;
    float combined_restitution;
    bool  is_sensor;
};

// NOTE: Needs to be kept in sync with JPH::CollideShapeResult
struct JPH_CollideShapeResult
{
    alignas(16) float    contact_point1[4];
    alignas(16) float    contact_point2[4];
    alignas(16) float    penetration_axis[4];
    float                penetration_depth;
    JPH_SubShapeID       sub_shape1_id;
    JPH_SubShapeID       sub_shape2_id;
    JPH_BodyID           body2_id;
    alignas(16) uint32_t num_face_points1;
    alignas(16) float    shape1_face[32][4];
    alignas(16) uint32_t num_face_points2;
    alignas(16) float    shape2_face[32][4];
};

// NOTE: Needs to be kept in sync with JPH::BroadPhaseLayerInterface
struct JPH_BroadPhaseLayerInterfaceVTable
{
    const void *reserved0;
    const void *reserved1;

    uint32_t
    (*GetNumBroadPhaseLayers)(const void *in_self);

    JPH_BroadPhaseLayer
    (*GetBroadPhaseLayer)(const void *in_self, JPH_ObjectLayer in_layer);

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char *
    (*GetBroadPhaseLayerName)(const void *in_self, JPH_BroadPhaseLayer in_layer);
#endif
};

// NOTE: Needs to be kept in sync with JPH::BodyActivationListener
struct JPH_BodyActivationListenerVTable
{
    const void *reserved0;
    const void *reserved1;

    void
    (*OnBodyActivated)(void *in_self, const JPH_BodyID *in_body_id, uint64_t in_user_data);

    void
    (*OnBodyDeactivated)(void *in_self, const JPH_BodyID *in_body_id, uint64_t in_user_data);
};

// NOTE: Needs to be kept in sync with JPH::ContactListener
struct JPH_ContactListenerVTable
{
    const void *reserved0;
    const void *reserved1;

    JPH_ValidateResult
    (*OnContactValidate)(void *in_self,
                         const JPH_Body *in_body1,
                         const JPH_Body *in_body2,
                         const JPH_CollideShapeResult *in_collision_result);
    void
    (*OnContactAdded)(void *in_self,
                      const JPH_Body *in_body1,
                      const JPH_Body *in_body2,
                      const JPH_ContactManifold *in_manifold,
                      JPH_ContactSettings *io_settings);
    void
    (*OnContactPersisted)(void *in_self,
                          const JPH_Body *in_body1,
                          const JPH_Body *in_body2,
                          const JPH_ContactManifold *in_manifold,
                          JPH_ContactSettings *io_settings);
    void
    (*OnContactRemoved)(void *in_self, const JPH_SubShapeIDPair *in_sub_shape_pair);
};

// NOTE: Needs to be kept in sync with JPH::TransformedShape
struct JPH_TransformedShape
{
    alignas(16) float     shape_position_com[4];
    alignas(16) float     shape_rotation[4];
    const JPH_Shape *     shape;
    float                 shape_scale[3];
    JPH_BodyID            body_id;
    JPH_SubShapeIDCreator sub_shape_id_creator;
};
//--------------------------------------------------------------------------------------------------
//
// Misc functions
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterDefaultAllocator(void);

JPH_CAPI void
JPH_RegisterCustomAllocator(JPH_AllocateFunction in_alloc,
                            JPH_FreeFunction in_free,
                            JPH_AlignedAllocateFunction in_aligned_alloc,
                            JPH_AlignedFreeFunction in_aligned_free);
JPH_CAPI void
JPH_CreateFactory(void);

JPH_CAPI void
JPH_DestroyFactory(void);

JPH_CAPI void
JPH_RegisterTypes(void);

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
// JPH_MotionProperties
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_MotionQuality
JPH_MotionProperties_GetMotionQuality(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetMotionQuality(JPH_MotionProperties *in_properties,
                                      JPH_MotionQuality in_motion_quality);
JPH_CAPI void
JPH_MotionProperties_GetLinearVelocity(const JPH_MotionProperties *in_properties,
                                       float out_linear_velocity[3]);
JPH_CAPI void
JPH_MotionProperties_SetLinearVelocity(JPH_MotionProperties *in_properties,
                                       const float in_linear_velocity[3]);
JPH_CAPI void
JPH_MotionProperties_SetLinearVelocityClamped(JPH_MotionProperties *in_properties,
                                              const float in_linear_velocity[3]);
JPH_CAPI void
JPH_MotionProperties_SetAngularVelocity(JPH_MotionProperties *in_properties,
                                        const float in_angular_velocity[3]);
JPH_CAPI void
JPH_MotionProperties_SetAngularVelocityClamped(JPH_MotionProperties *in_properties,
                                               const float in_angular_velocity[3]);
JPH_CAPI void
JPH_MotionProperties_MoveKinematic(JPH_MotionProperties *in_properties,
                                   const float in_delta_position[3],
                                   const float in_delta_rotation[4],
                                   float in_delta_time);
JPH_CAPI void
JPH_MotionProperties_ClampLinearVelocity(JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_ClampAngularVelocity(JPH_MotionProperties *in_properties);

JPH_CAPI float
JPH_MotionProperties_GetLinearDamping(JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetLinearDamping(JPH_MotionProperties *in_properties,
                                      float in_linear_damping);
JPH_CAPI float
JPH_MotionProperties_GetAngularDamping(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetAngularDamping(JPH_MotionProperties *in_properties,
                                       float in_angular_damping);
JPH_CAPI float
JPH_MotionProperties_GetGravityFactor(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetGravityFactor(JPH_MotionProperties *in_properties,
                                      float in_gravity_factor);
JPH_CAPI void
JPH_MotionProperties_SetMassProperties(JPH_MotionProperties *in_properties,
                                       const JPH_MassProperties *in_mass_properties);
JPH_CAPI float
JPH_MotionProperties_GetInverseMass(const JPH_MotionProperties *in_properties);

JPH_CAPI float
JPH_MotionProperties_GetInverseMassUnchecked(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetInverseMass(JPH_MotionProperties *in_properties, float in_inv_mass);

JPH_CAPI void
JPH_MotionProperties_GetInverseInertiaDiagonal(const JPH_MotionProperties *in_properties,
                                               float out_inverse_inertia_diagonal[3]);
JPH_CAPI void
JPH_MotionProperties_GetInertiaRotation(const JPH_MotionProperties *in_properties,
                                        float out_inertia_rotation[4]);
JPH_CAPI void
JPH_MotionProperties_SetInverseInertia(JPH_MotionProperties *in_properties,
                                       const float in_diagonal[3],
                                       const float in_rotation[4]);
JPH_CAPI void
JPH_MotionProperties_GetLocalSpaceInverseInertia(const JPH_MotionProperties *in_properties,
                                                 float out_matrix[16]);
JPH_CAPI void
JPH_MotionProperties_GetLocalSpaceInverseInertiaUnchecked(const JPH_MotionProperties *in_properties,
                                                          float out_matrix[16]);
JPH_CAPI void
JPH_MotionProperties_GetInverseInertiaForRotation(const JPH_MotionProperties *in_properties,
                                                  const float in_rotation_matrix[4],
                                                  float out_matrix[16]);
JPH_CAPI void
JPH_MotionProperties_MultiplyWorldSpaceInverseInertiaByVector(JPH_MotionProperties *in_properties,
                                                              const float in_body_rotation[4],
                                                              const float in_vector[3],
                                                              float out_vector[3]);
JPH_CAPI void
JPH_MotionProperties_GetPointVelocityCOM(const JPH_MotionProperties *in_properties,
                                         const float in_point_relative_to_com[3],
                                         float out_point[3]);
JPH_CAPI float
JPH_MotionProperties_GetMaxLinearVelocity(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetMaxLinearVelocity(JPH_MotionProperties *in_properties,
                                          float in_max_linear_velocity);
JPH_CAPI float
JPH_MotionProperties_GetMaxAngularVelocity(const JPH_MotionProperties *in_properties);

JPH_CAPI void
JPH_MotionProperties_SetMaxAngularVelocity(JPH_MotionProperties *in_properties,
                                           float in_max_angular_velocity);
JPH_CAPI void
JPH_MotionProperties_AddLinearVelocityStep(JPH_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3]);
JPH_CAPI void
JPH_MotionProperties_SubLinearVelocityStep(JPH_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3]);
JPH_CAPI void
JPH_MotionProperties_AddAngularVelocityStep(JPH_MotionProperties *in_properties,
                                            const float in_linear_angular_change[3]);
JPH_CAPI void
JPH_MotionProperties_SubAngularVelocityStep(JPH_MotionProperties *in_properties,
                                            const float in_linear_angular_change[3]);
//--------------------------------------------------------------------------------------------------
//
// JPH_CollisionGroup
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CollisionGroup
JPH_CollisionGroup_InitDefault(void);
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
JPH_JobSystem_Create(uint32_t in_max_jobs, uint32_t in_max_barriers, int in_num_threads);

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
JPH_CAPI void
JPH_PhysicsSystem_SetBodyActivationListener(JPH_PhysicsSystem *in_physics_system, void *in_listener);

JPH_CAPI void *
JPH_PhysicsSystem_GetBodyActivationListener(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI void
JPH_PhysicsSystem_SetContactListener(JPH_PhysicsSystem *in_physics_system, void *in_listener);

JPH_CAPI void *
JPH_PhysicsSystem_GetContactListener(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *in_physics_system);

JPH_CAPI JPH_BodyInterface *
JPH_PhysicsSystem_GetBodyInterface(JPH_PhysicsSystem *in_physics_system);

JPH_CAPI void
JPH_PhysicsSystem_OptimizeBroadPhase(JPH_PhysicsSystem *in_physics_system);

JPH_CAPI void
JPH_PhysicsSystem_Update(JPH_PhysicsSystem *in_physics_system,
                         float in_delta_time,
                         int in_collision_steps,
                         int in_integration_sub_steps,
                         JPH_TempAllocator *in_temp_allocator,
                         JPH_JobSystem *in_job_system);
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
JPH_ShapeSettings_CreateShape(const JPH_ShapeSettings *in_settings);

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
// JPH_SphereShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_SphereShapeSettings *
JPH_SphereShapeSettings_Create(float in_radius);

JPH_CAPI float
JPH_SphereShapeSettings_GetRadius(const JPH_SphereShapeSettings *in_settings);

JPH_CAPI void
JPH_SphereShapeSettings_SetRadius(JPH_SphereShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPH_TriangleShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TriangleShapeSettings *
JPH_TriangleShapeSettings_Create(const float in_v1[3], const float in_v2[3], const float in_v3[3]);

JPH_CAPI void
JPH_TriangleShapeSettings_SetVertices(JPH_TriangleShapeSettings *in_settings,
                                      const float in_v1[3],
                                      const float in_v2[3],
                                      const float in_v3[3]);
JPH_CAPI void
JPH_TriangleShapeSettings_GetVertices(const JPH_TriangleShapeSettings *in_settings,
                                      float out_v1[3],
                                      float out_v2[3],
                                      float out_v3[3]);
JPH_CAPI float
JPH_TriangleShapeSettings_GetConvexRadius(const JPH_TriangleShapeSettings *in_settings);

JPH_CAPI void
JPH_TriangleShapeSettings_SetConvexRadius(JPH_TriangleShapeSettings *in_settings,
                                          float in_convex_radius);
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CapsuleShapeSettings *
JPH_CapsuleShapeSettings_Create(float in_half_height_of_cylinder, float in_radius);

JPH_CAPI float
JPH_CapsuleShapeSettings_GetHalfHeightOfCylinder(const JPH_CapsuleShapeSettings *in_settings);

JPH_CAPI void
JPH_CapsuleShapeSettings_SetHalfHeightOfCylinder(JPH_CapsuleShapeSettings *in_settings,
                                                 float in_half_height_of_cylinder);
JPH_CAPI float
JPH_CapsuleShapeSettings_GetRadius(const JPH_CapsuleShapeSettings *in_settings);

JPH_CAPI void
JPH_CapsuleShapeSettings_SetRadius(JPH_CapsuleShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPH_TaperedCapsuleShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TaperedCapsuleShapeSettings *
JPH_TaperedCapsuleShapeSettings_Create(float in_half_height, float in_top_radius, float in_bottom_radius);

JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetHalfHeightOfTaperedCylinder(const JPH_TaperedCapsuleShapeSettings *in_settings);

JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetHalfHeightOfTaperedCylinder(JPH_TaperedCapsuleShapeSettings *in_settings,
                                                               float in_half_height);
JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetTopRadius(const JPH_TaperedCapsuleShapeSettings *in_settings);

JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetTopRadius(JPH_TaperedCapsuleShapeSettings *in_settings, float in_top_radius);

JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetBottomRadius(const JPH_TaperedCapsuleShapeSettings *in_settings);

JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetBottomRadius(JPH_TaperedCapsuleShapeSettings *in_settings,
                                                float in_bottom_radius);
//--------------------------------------------------------------------------------------------------
//
// JPH_CylinderShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CylinderShapeSettings *
JPH_CylinderShapeSettings_Create(float in_half_height, float in_radius);

JPH_CAPI float
JPH_CylinderShapeSettings_GetConvexRadius(const JPH_CylinderShapeSettings *in_settings);

JPH_CAPI void
JPH_CylinderShapeSettings_SetConvexRadius(JPH_CylinderShapeSettings *in_settings, float in_convex_radius);

JPH_CAPI float
JPH_CylinderShapeSettings_GetHalfHeight(const JPH_CylinderShapeSettings *in_settings);

JPH_CAPI void
JPH_CylinderShapeSettings_SetHalfHeight(JPH_CylinderShapeSettings *in_settings, float in_half_height);

JPH_CAPI float
JPH_CylinderShapeSettings_GetRadius(const JPH_CylinderShapeSettings *in_settings);

JPH_CAPI void
JPH_CylinderShapeSettings_SetRadius(JPH_CylinderShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPH_ConvexHullShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
/// `in_points` needs to be aligned to 16 bytes
JPH_CAPI JPH_ConvexHullShapeSettings *
JPH_ConvexHullShapeSettings_Create(const float in_points[][4], int in_num_points);

JPH_CAPI float
JPH_ConvexHullShapeSettings_GetMaxConvexRadius(const JPH_ConvexHullShapeSettings *in_settings);

JPH_CAPI void
JPH_ConvexHullShapeSettings_SetMaxConvexRadius(JPH_ConvexHullShapeSettings *in_settings,
                                               float in_max_convex_radius);
JPH_CAPI float
JPH_ConvexHullShapeSettings_GetMaxErrorConvexRadius(const JPH_ConvexHullShapeSettings *in_settings);

JPH_CAPI void
JPH_ConvexHullShapeSettings_SetMaxErrorConvexRadius(JPH_ConvexHullShapeSettings *in_settings,
                                                    float in_max_err_convex_radius);
JPH_CAPI float
JPH_ConvexHullShapeSettings_GetHullTolerance(const JPH_ConvexHullShapeSettings *in_settings);

JPH_CAPI void
JPH_ConvexHullShapeSettings_SetHullTolerance(JPH_ConvexHullShapeSettings *in_settings,
                                             float in_hull_tolerance);
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

JPH_CAPI void
JPH_BodyInterface_DestroyBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id);

JPH_CAPI void
JPH_BodyInterface_AddBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id, JPH_Activation in_mode);

JPH_CAPI void
JPH_BodyInterface_RemoveBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id);

JPH_CAPI JPH_BodyID
JPH_BodyInterface_CreateAndAddBody(JPH_BodyInterface *in_iface,
                                   const JPH_BodyCreationSettings *in_settings,
                                   JPH_Activation in_mode);
JPH_CAPI bool
JPH_BodyInterface_IsAdded(const JPH_BodyInterface *in_iface, JPH_BodyID in_body_id);

JPH_CAPI void
JPH_BodyInterface_SetLinearVelocity(JPH_BodyInterface *in_iface,
                                    JPH_BodyID in_body_id,
                                    const float in_velocity[3]);
JPH_CAPI void
JPH_BodyInterface_GetLinearVelocity(const JPH_BodyInterface *in_iface,
                                    JPH_BodyID in_body_id,
                                    float out_velocity[3]);
JPH_CAPI void
JPH_BodyInterface_GetCenterOfMassPosition(const JPH_BodyInterface *in_iface,
                                          JPH_BodyID in_body_id,
                                          float out_position[3]);
JPH_CAPI bool
JPH_BodyInterface_IsActive(const JPH_BodyInterface *in_iface, JPH_BodyID in_body_id);
//--------------------------------------------------------------------------------------------------
//
// JPH_Body
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyID
JPH_Body_GetID(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_IsActive(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_IsStatic(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_IsKinematic(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_IsDynamic(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_CanBeKinematicOrDynamic(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetIsSensor(JPH_Body *in_body, bool in_is_sensor);

JPH_CAPI bool
JPH_Body_IsSensor(const JPH_Body *in_body);

JPH_CAPI JPH_MotionType
JPH_Body_GetMotionType(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetMotionType(JPH_Body *in_body, JPH_MotionType in_motion_type);

JPH_CAPI JPH_BroadPhaseLayer
JPH_Body_GetBroadPhaseLayer(const JPH_Body *in_body);

JPH_CAPI JPH_ObjectLayer
JPH_Body_GetObjectLayer(const JPH_Body *in_body);

JPH_CAPI JPH_CollisionGroup *
JPH_Body_GetCollisionGroup(JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetCollisionGroup(JPH_Body *in_body, const JPH_CollisionGroup *in_group);

JPH_CAPI bool
JPH_Body_GetAllowSleeping(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetAllowSleeping(JPH_Body *in_body, bool in_allow_sleeping);

JPH_CAPI float
JPH_Body_GetFriction(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetFriction(JPH_Body *in_body, float in_friction);

JPH_CAPI float
JPH_Body_GetRestitution(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetRestitution(JPH_Body *in_body, float in_restitution);

JPH_CAPI void
JPH_Body_GetLinearVelocity(const JPH_Body *in_body, float out_angular_velocity[3]);

JPH_CAPI void
JPH_Body_SetLinearVelocity(JPH_Body *in_body, const float in_linear_velocity[3]);

JPH_CAPI void
JPH_Body_SetLinearVelocityClamped(JPH_Body *in_body, const float in_linear_velocity[3]);

JPH_CAPI void
JPH_Body_GetAngularVelocity(const JPH_Body *in_body, float out_angular_velocity[3]);

JPH_CAPI void
JPH_Body_SetAnglularVelocity(JPH_Body *in_body, const float in_angular_velocity[3]);

JPH_CAPI void
JPH_Body_SetAnglularVelocityClamped(JPH_Body *in_body, const float in_angular_velocity[3]);

JPH_CAPI void
JPH_Body_GetPointVelocityCOM(const JPH_Body *in_body,
                             const float in_point_relative_to_com[3],
                             float out_velocity[3]);
JPH_CAPI void
JPH_Body_GetPointVelocity(const JPH_Body *in_body, const float in_point[3], float out_velocity[3]);

JPH_CAPI void
JPH_Body_AddForce(JPH_Body *in_body, const float in_force[3]);

JPH_CAPI void
JPH_Body_AddForceAtPosition(JPH_Body *in_body, const float in_force[3], const float in_position[3]);

JPH_CAPI void
JPH_Body_AddTorque(JPH_Body *in_body, const float in_torque[3]);

JPH_CAPI void
JPH_Body_GetInverseInertia(const JPH_Body *in_body, float out_inverse_inertia[16]);

JPH_CAPI void
JPH_Body_AddImpulse(JPH_Body *in_body, const float in_impulse[3]);

JPH_CAPI void
JPH_Body_AddAngularImpulse(JPH_Body *in_body, const float in_angular_impulse[3]);

JPH_CAPI void
JPH_Body_MoveKinematic(JPH_Body *in_body,
                       const float in_target_rotation[4],
                       float in_delta_time);
JPH_CAPI void
JPH_Body_ApplyBuoyancyImpulse(JPH_Body *in_body,
                              const float in_plane[4],
                              float in_buoyancy,
                              float in_linear_drag,
                              float in_angular_drag,
                              const float in_fluid_velocity[3],
                              const float in_gravity[3],
                              float in_delta_time);
JPH_CAPI bool
JPH_Body_IsInBroadPhase(const JPH_Body *in_body);

JPH_CAPI bool
JPH_Body_IsCollisionCacheInvalid(const JPH_Body *in_body);

JPH_CAPI const JPH_Shape *
JPH_Body_GetShape(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_GetPosition(const JPH_Body *in_body, float out_position[3]);

JPH_CAPI void
JPH_Body_GetRotation(const JPH_Body *in_body, float out_rotation[4]);

JPH_CAPI void
JPH_Body_GetWorldTransform(const JPH_Body *in_body, float out_transform[16]);

JPH_CAPI void
JPH_Body_GetCenterOfMassPosition(const JPH_Body *in_body, float out_position_com[3]);

JPH_CAPI void
JPH_Body_GetInverseCenterOfMassTransform(const JPH_Body *in_body, float out_transform[16]);

JPH_CAPI void
JPH_Body_GetWorldSpaceBounds(const JPH_Body *in_body, float out_min[3], float out_max[3]);

JPH_CAPI JPH_MotionProperties *
JPH_Body_GetMotionProperties(JPH_Body *in_body);

JPH_CAPI JPH_MotionProperties *
JPH_Body_GetMotionPropertiesUnchecked(JPH_Body *in_body);

JPH_CAPI uint64_t
JPH_Body_GetUserData(const JPH_Body *in_body);

JPH_CAPI void
JPH_Body_SetUserData(JPH_Body *in_body, uint64_t in_user_data);

JPH_CAPI void
JPH_Body_GetWorldSpaceSurfaceNormal(const JPH_Body *in_body,
                                    const JPH_SubShapeID *in_sub_shape_id,
                                    const float in_position[3],
                                    float out_normal_vector[3]);
JPH_CAPI JPH_TransformedShape
JPH_Body_GetTransformedShape(const JPH_Body *in_body);

JPH_CAPI JPH_BodyCreationSettings
JPH_Body_GetBodyCreationSettings(const JPH_Body *in_body);
//--------------------------------------------------------------------------------------------------
//
// JPH_BodyID
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_BodyID_GetIndex(JPH_BodyID in_body_id);

JPH_CAPI uint8_t
JPH_BodyID_GetSequenceNumber(JPH_BodyID in_body_id);

JPH_CAPI bool
JPH_BodyID_IsInvalid(JPH_BodyID in_body_id);
//--------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
