#include "JoltC.h"

#include <assert.h>
#include <stddef.h>
//#include <stdio.h>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

JPH_SUPPRESS_WARNINGS

#define ENSURE_TYPE(o, t) assert(reinterpret_cast<const JPH::SerializableObject *>(o)->CastTo(JPH_RTTI(t)) != nullptr)

#ifdef JPH_ENABLE_ASSERTS

static bool
AssertFailedImpl(const char *in_expression, const char *in_message, const char *in_file, uint32_t in_line)
{
	return true;
}

#endif
//--------------------------------------------------------------------------------------------------
static_assert(sizeof(JPH::BodyID)                  == sizeof(JPH_BodyID),                 "");
static_assert(sizeof(JPH::SubShapeID)              == sizeof(JPH_SubShapeID),             "");
static_assert(sizeof(JPH::EShapeType)              == sizeof(JPH_ShapeType),              "");
static_assert(sizeof(JPH::EShapeSubType)           == sizeof(JPH_ShapeSubType),           "");
static_assert(sizeof(JPH::EMotionType)             == sizeof(JPH_MotionType),             "");
static_assert(sizeof(JPH::EMotionQuality)          == sizeof(JPH_MotionQuality),          "");
static_assert(sizeof(JPH::EOverrideMassProperties) == sizeof(JPH_OverrideMassProperties), "");
static_assert(sizeof(JPH::EActivation)             == sizeof(JPH_Activation),             "");
static_assert(sizeof(JPH::ValidateResult)          == sizeof(JPH_ValidateResult),         "");
static_assert(sizeof(JPH::BroadPhaseLayer)         == sizeof(JPH_BroadPhaseLayer),        "");
static_assert(sizeof(JPH::ObjectLayer)             == sizeof(JPH_ObjectLayer),            "");
static_assert(sizeof(JPH::MassProperties)          == sizeof(JPH_MassProperties),         "");
static_assert(sizeof(JPH::CollisionGroup)          == sizeof(JPH_CollisionGroup),         "");
static_assert(sizeof(JPH::BodyCreationSettings)    == sizeof(JPH_BodyCreationSettings),   "");
static_assert(sizeof(JPH::ContactManifold)         == sizeof(JPH_ContactManifold),        "");
static_assert(sizeof(JPH::ContactSettings)         == sizeof(JPH_ContactSettings),        "");
static_assert(sizeof(JPH::SubShapeIDPair)          == sizeof(JPH_SubShapeIDPair),         "");
static_assert(sizeof(JPH::CollideShapeResult)      == sizeof(JPH_CollideShapeResult),     "");

static_assert(alignof(JPH::MassProperties)       == alignof(JPH_MassProperties),       "");
static_assert(alignof(JPH::CollisionGroup)       == alignof(JPH_CollisionGroup),       "");
static_assert(alignof(JPH::BodyCreationSettings) == alignof(JPH_BodyCreationSettings), "");
static_assert(alignof(JPH::ContactManifold)      == alignof(JPH_ContactManifold),      "");
static_assert(alignof(JPH::ContactSettings)      == alignof(JPH_ContactSettings),      "");
static_assert(alignof(JPH::SubShapeIDPair)       == alignof(JPH_SubShapeIDPair),       "");
static_assert(alignof(JPH::CollideShapeResult)   == alignof(JPH_CollideShapeResult),   "");

static_assert(JPH_SHAPE_TYPE_CONVEX       == static_cast<int>(JPH::EShapeType::Convex));
static_assert(JPH_SHAPE_TYPE_COMPOUND     == static_cast<int>(JPH::EShapeType::Compound));
static_assert(JPH_SHAPE_TYPE_DECORATED    == static_cast<int>(JPH::EShapeType::Decorated));
static_assert(JPH_SHAPE_TYPE_MESH         == static_cast<int>(JPH::EShapeType::Mesh));
static_assert(JPH_SHAPE_TYPE_HEIGHT_FIELD == static_cast<int>(JPH::EShapeType::HeightField));
static_assert(JPH_SHAPE_TYPE_USER1        == static_cast<int>(JPH::EShapeType::User1));
static_assert(JPH_SHAPE_TYPE_USER2        == static_cast<int>(JPH::EShapeType::User2));
static_assert(JPH_SHAPE_TYPE_USER3        == static_cast<int>(JPH::EShapeType::User3));
static_assert(JPH_SHAPE_TYPE_USER4        == static_cast<int>(JPH::EShapeType::User4));

static_assert(JPH_SHAPE_SUB_TYPE_SPHERE                == static_cast<int>(JPH::EShapeSubType::Sphere));
static_assert(JPH_SHAPE_SUB_TYPE_BOX                   == static_cast<int>(JPH::EShapeSubType::Box));
static_assert(JPH_SHAPE_SUB_TYPE_TRIANGLE              == static_cast<int>(JPH::EShapeSubType::Triangle));
static_assert(JPH_SHAPE_SUB_TYPE_CAPSULE               == static_cast<int>(JPH::EShapeSubType::Capsule));
static_assert(JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE       == static_cast<int>(JPH::EShapeSubType::TaperedCapsule));
static_assert(JPH_SHAPE_SUB_TYPE_CYLINDER              == static_cast<int>(JPH::EShapeSubType::Cylinder));
static_assert(JPH_SHAPE_SUB_TYPE_CONVEX_HULL           == static_cast<int>(JPH::EShapeSubType::ConvexHull));
static_assert(JPH_SHAPE_SUB_TYPE_STATIC_COMPOUND       == static_cast<int>(JPH::EShapeSubType::StaticCompound));
static_assert(JPH_SHAPE_SUB_TYPE_MUTABLE_COMPOUND      == static_cast<int>(JPH::EShapeSubType::MutableCompound));
static_assert(JPH_SHAPE_SUB_TYPE_ROTATED_TRANSLATED    == static_cast<int>(JPH::EShapeSubType::RotatedTranslated));
static_assert(JPH_SHAPE_SUB_TYPE_SCALED                == static_cast<int>(JPH::EShapeSubType::Scaled));
static_assert(JPH_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS == static_cast<int>(JPH::EShapeSubType::OffsetCenterOfMass));
static_assert(JPH_SHAPE_SUB_TYPE_MESH                  == static_cast<int>(JPH::EShapeSubType::Mesh));
static_assert(JPH_SHAPE_SUB_TYPE_HEIGHT_FIELD          == static_cast<int>(JPH::EShapeSubType::HeightField));
static_assert(JPH_SHAPE_SUB_TYPE_USER1                 == static_cast<int>(JPH::EShapeSubType::User1));
static_assert(JPH_SHAPE_SUB_TYPE_USER2                 == static_cast<int>(JPH::EShapeSubType::User2));
static_assert(JPH_SHAPE_SUB_TYPE_USER3                 == static_cast<int>(JPH::EShapeSubType::User3));
static_assert(JPH_SHAPE_SUB_TYPE_USER4                 == static_cast<int>(JPH::EShapeSubType::User4));
static_assert(JPH_SHAPE_SUB_TYPE_USER5                 == static_cast<int>(JPH::EShapeSubType::User5));
static_assert(JPH_SHAPE_SUB_TYPE_USER6                 == static_cast<int>(JPH::EShapeSubType::User6));
static_assert(JPH_SHAPE_SUB_TYPE_USER7                 == static_cast<int>(JPH::EShapeSubType::User7));
static_assert(JPH_SHAPE_SUB_TYPE_USER8                 == static_cast<int>(JPH::EShapeSubType::User8));

static_assert(
    offsetof(JPH::BodyCreationSettings, mInertiaMultiplier) ==
    offsetof(JPH_BodyCreationSettings, inertia_multiplier),
    "");
static_assert(
    offsetof(JPH::BodyCreationSettings, mIsSensor) ==
    offsetof(JPH_BodyCreationSettings, is_sensor),
    "");
static_assert(
    offsetof(JPH::BodyCreationSettings, mAngularDamping) ==
    offsetof(JPH_BodyCreationSettings, angular_damping),
    "");

static_assert(
    offsetof(JPH::ContactManifold, mPenetrationDepth) ==
    offsetof(JPH_ContactManifold, penetration_depth),
    "");
static_assert(
    offsetof(JPH::ContactManifold, mWorldSpaceContactPointsOn1) ==
    offsetof(JPH_ContactManifold, num_points1),
    "");

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) ==
    offsetof(JPH_CollideShapeResult, penetration_depth),
    "");
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) ==
    offsetof(JPH_CollideShapeResult, num_face_points1),
    "");
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterDefaultAllocator(void)
{
    JPH::RegisterDefaultAllocator();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CreateFactory(void)
{
    assert(JPH::Factory::sInstance == nullptr);
    JPH::Factory::sInstance = new JPH::Factory();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_DestroyFactory(void)
{
    assert(JPH::Factory::sInstance != nullptr);
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterTypes(void)
{
    JPH::RegisterTypes();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CollisionGroup
JPH_CollisionGroup_InitDefault(void)
{
    const JPH::CollisionGroup group;
    return *reinterpret_cast<const JPH_CollisionGroup *>(&group);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyCreationSettings
JPH_BodyCreationSettings_InitDefault(void)
{
    const JPH::BodyCreationSettings settings;
    return *reinterpret_cast<const JPH_BodyCreationSettings *>(&settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyCreationSettings
JPH_BodyCreationSettings_Init(const JPH_Shape *in_shape,
                              const float in_position[3],
                              const float in_rotation[4],
                              JPH_MotionType in_motion_type,
                              JPH_ObjectLayer in_layer)
{
    assert(in_shape != nullptr && in_position != nullptr && in_rotation != nullptr);
    JPH_BodyCreationSettings settings = JPH_BodyCreationSettings_InitDefault();
    settings.position[0] = in_position[0];
    settings.position[1] = in_position[1];
    settings.position[2] = in_position[2];
    settings.rotation[0] = in_rotation[0];
    settings.rotation[1] = in_rotation[1];
    settings.rotation[2] = in_rotation[2];
    settings.rotation[3] = in_rotation[3];
    settings.object_layer = in_layer;
    settings.motion_type = in_motion_type;
    settings.shape = in_shape;
    return settings;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_TempAllocator
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TempAllocator *
JPH_TempAllocator_Create(uint32_t in_size)
{
    auto impl = new JPH::TempAllocatorImpl(in_size);
    return reinterpret_cast<JPH_TempAllocator *>(impl);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TempAllocator_Destroy(JPH_TempAllocator *in_allocator)
{
    assert(in_allocator != nullptr);
    delete reinterpret_cast<JPH::TempAllocator *>(in_allocator);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_JobSystem
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_JobSystem *
JPH_JobSystem_Create(uint32_t in_max_jobs, uint32_t in_max_barriers, int in_num_threads)
{
    auto job_system = new JPH::JobSystemThreadPool(in_max_jobs, in_max_barriers, in_num_threads);
    return reinterpret_cast<JPH_JobSystem *>(job_system);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_JobSystem_Destroy(JPH_JobSystem *in_job_system)
{
    assert(in_job_system != nullptr);
    delete reinterpret_cast<JPH::JobSystemThreadPool *>(in_job_system);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_PhysicsSystem *
JPH_PhysicsSystem_Create(void)
{
    return reinterpret_cast<JPH_PhysicsSystem*>(new JPH::PhysicsSystem());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    delete reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_Init(JPH_PhysicsSystem *in_physics_system,
                       uint32_t in_max_bodies,
                       uint32_t in_num_bodyMutexes,
                       uint32_t in_max_bodyPairs,
                       uint32_t in_max_contactConstraints,
                       const void *in_broad_phase_layer_interface,
                       JPH_ObjectVsBroadPhaseLayerFilter in_object_vs_broad_phase_layer_filter,
                       JPH_ObjectLayerPairFilter in_object_layer_pair_filter)
{
    assert(in_physics_system != nullptr);
    assert(in_broad_phase_layer_interface != nullptr);
    assert(in_object_vs_broad_phase_layer_filter != nullptr);
    assert(in_object_layer_pair_filter != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    physics_system->Init(
        in_max_bodies,
        in_num_bodyMutexes,
        in_max_bodyPairs,
        in_max_contactConstraints,
        *static_cast<const JPH::BroadPhaseLayerInterface *>(in_broad_phase_layer_interface),
        reinterpret_cast<JPH::ObjectVsBroadPhaseLayerFilter>(in_object_vs_broad_phase_layer_filter),
        reinterpret_cast<JPH::ObjectLayerPairFilter>(in_object_layer_pair_filter));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_SetBodyActivationListener(JPH_PhysicsSystem *in_physics_system, void *in_listener)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    physics_system->SetBodyActivationListener(reinterpret_cast<JPH::BodyActivationListener *>(in_listener));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void *
JPH_PhysicsSystem_GetBodyActivationListener(const JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return physics_system->GetBodyActivationListener();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_SetContactListener(JPH_PhysicsSystem *in_physics_system, void *in_listener)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    physics_system->SetContactListener(reinterpret_cast<JPH::ContactListener *>(in_listener));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void *
JPH_PhysicsSystem_GetContactListener(const JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return physics_system->GetContactListener();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetNumBodies();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetNumActiveBodies();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetMaxBodies();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyInterface *
JPH_PhysicsSystem_GetBodyInterface(JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<JPH_BodyInterface *>(&physics_system->GetBodyInterface());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_OptimizeBroadPhase(JPH_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system)->OptimizeBroadPhase();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_PhysicsSystem_Update(JPH_PhysicsSystem *in_physics_system,
                         float in_delta_time,
                         int in_collision_steps,
                         int in_integration_sub_steps,
                         JPH_TempAllocator *in_temp_allocator,
                         JPH_JobSystem *in_job_system)
{
    assert(in_physics_system != nullptr && in_temp_allocator != nullptr && in_job_system != nullptr);
    reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system)->Update(
        in_delta_time,
        in_collision_steps,
        in_integration_sub_steps,
        reinterpret_cast<JPH::TempAllocator *>(in_temp_allocator),
        reinterpret_cast<JPH::JobSystem *>(in_job_system));
}
//--------------------------------------------------------------------------------------------------
//
// JPH_ShapeSettings
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ShapeSettings_AddRef(JPH_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->AddRef();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ShapeSettings_Release(JPH_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->Release();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_ShapeSettings_GetRefCount(const JPH_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    return reinterpret_cast<const JPH::ShapeSettings *>(in_settings)->GetRefCount();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_Shape *
JPH_ShapeSettings_CreateShape(const JPH_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    auto settings = reinterpret_cast<const JPH::ShapeSettings *>(in_settings);
    const JPH::Result result = settings->Create();
    if (result.HasError())
        return nullptr;
    JPH::Shape *shape = const_cast<JPH::Shape *>(result.Get().GetPtr());
    shape->AddRef();
    return reinterpret_cast<JPH_Shape *>(shape);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint64_t
JPH_ShapeSettings_GetUserData(const JPH_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    return reinterpret_cast<const JPH::ShapeSettings *>(in_settings)->mUserData;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ShapeSettings_SetUserData(JPH_ShapeSettings *in_settings, uint64_t in_user_data)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->mUserData = in_user_data;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_ConvexShapeSettings (-> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI const JPH_PhysicsMaterial *
JPH_ConvexShapeSettings_GetMaterial(const JPH_ConvexShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    auto settings = reinterpret_cast<const JPH::ConvexShapeSettings *>(in_settings);
    return reinterpret_cast<const JPH_PhysicsMaterial *>(settings->mMaterial.GetPtr());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ConvexShapeSettings_SetMaterial(JPH_ConvexShapeSettings *in_settings,
                                    const JPH_PhysicsMaterial *in_material)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    auto settings = reinterpret_cast<JPH::ConvexShapeSettings *>(in_settings);
    settings->mMaterial = reinterpret_cast<const JPH::PhysicsMaterial *>(in_material);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_ConvexShapeSettings_GetDensity(const JPH_ConvexShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    return reinterpret_cast<const JPH::ConvexShapeSettings *>(in_settings)->mDensity;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ConvexShapeSettings_SetDensity(JPH_ConvexShapeSettings *in_settings, float in_density)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    reinterpret_cast<JPH::ConvexShapeSettings *>(in_settings)->SetDensity(in_density);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_BoxShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BoxShapeSettings *
JPH_BoxShapeSettings_Create(const float in_half_extent[3])
{
    assert(in_half_extent != nullptr);
    auto settings = new JPH::BoxShapeSettings(JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_half_extent)));
    settings->AddRef();
    return reinterpret_cast<JPH_BoxShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BoxShapeSettings_GetHalfExtent(const JPH_BoxShapeSettings *in_settings, float out_half_extent[3])
{
    assert(in_settings != nullptr && out_half_extent != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    auto settings = reinterpret_cast<const JPH::BoxShapeSettings *>(in_settings);
    settings->mHalfExtent.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_half_extent));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BoxShapeSettings_SetHalfExtent(JPH_BoxShapeSettings *in_settings, const float in_half_extent[3])
{
    assert(in_settings != nullptr && in_half_extent != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    auto settings = reinterpret_cast<JPH::BoxShapeSettings *>(in_settings);
    settings->mHalfExtent = JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_half_extent));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_BoxShapeSettings_GetConvexRadius(const JPH_BoxShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    return reinterpret_cast<const JPH::BoxShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BoxShapeSettings_SetConvexRadius(JPH_BoxShapeSettings *in_settings, float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    reinterpret_cast<JPH::BoxShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_SphereShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_SphereShapeSettings *
JPH_SphereShapeSettings_Create(float in_radius)
{
    auto settings = new JPH::SphereShapeSettings(in_radius);
    settings->AddRef();
    return reinterpret_cast<JPH_SphereShapeSettings *>(settings);
}

JPH_CAPI float
JPH_SphereShapeSettings_GetRadius(const JPH_SphereShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    return reinterpret_cast<const JPH::SphereShapeSettings *>(in_settings)->mRadius;
}

JPH_CAPI void
JPH_SphereShapeSettings_SetRadius(JPH_SphereShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    reinterpret_cast<JPH::SphereShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_Shape
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Shape_AddRef(JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    reinterpret_cast<JPH::Shape *>(in_shape)->AddRef();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Shape_Release(JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    reinterpret_cast<JPH::Shape *>(in_shape)->Release();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_Shape_GetRefCount(const JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<const JPH::Shape *>(in_shape)->GetRefCount();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_ShapeType
JPH_Shape_GetType(const JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return static_cast<JPH_ShapeType>(reinterpret_cast<const JPH::Shape *>(in_shape)->GetType());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_ShapeSubType
JPH_Shape_GetSubType(const JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return static_cast<JPH_ShapeSubType>(reinterpret_cast<const JPH::Shape *>(in_shape)->GetSubType());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint64_t
JPH_Shape_GetUserData(const JPH_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<const JPH::Shape *>(in_shape)->GetUserData();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Shape_SetUserData(JPH_Shape *in_shape, uint64_t in_user_data)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<JPH::Shape *>(in_shape)->SetUserData(in_user_data);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_BodyInterface
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_Body *
JPH_BodyInterface_CreateBody(JPH_BodyInterface *in_iface, const JPH_BodyCreationSettings *in_setting)
{
    assert(in_iface != nullptr && in_setting != nullptr);
    auto iface = reinterpret_cast<JPH::BodyInterface *>(in_iface);
    auto settings = reinterpret_cast<const JPH::BodyCreationSettings *>(in_setting);
    return reinterpret_cast<JPH_Body *>(iface->CreateBody(*settings));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_DestroyBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->DestroyBody(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_AddBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id, JPH_Activation in_mode)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->AddBody(
        JPH::BodyID(in_body_id),
        static_cast<JPH::EActivation>(in_mode));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_RemoveBody(JPH_BodyInterface *in_iface, JPH_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->RemoveBody(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyID
JPH_BodyInterface_CreateAndAddBody(JPH_BodyInterface *in_iface,
                                   const JPH_BodyCreationSettings *in_setting,
                                   JPH_Activation in_mode)
{
    assert(in_iface != nullptr && in_setting != nullptr);
    auto iface = reinterpret_cast<JPH::BodyInterface *>(in_iface);
    auto settings = reinterpret_cast<const JPH::BodyCreationSettings *>(in_setting);
    const JPH::BodyID body_id = iface->CreateAndAddBody(*settings, static_cast<JPH::EActivation>(in_mode));
    return *reinterpret_cast<const JPH_BodyID *>(&body_id);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_BodyInterface_IsAdded(const JPH_BodyInterface *in_iface, JPH_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    return reinterpret_cast<const JPH::BodyInterface *>(in_iface)->IsAdded(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_SetLinearVelocity(JPH_BodyInterface *in_iface,
                                    JPH_BodyID in_body_id,
                                    const float in_velocity[3])
{
    assert(in_iface != nullptr && in_velocity != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->SetLinearVelocity(
        JPH::BodyID(in_body_id),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_GetLinearVelocity(const JPH_BodyInterface *in_iface,
                                    JPH_BodyID in_body_id,
                                    float out_velocity[3])
{
    assert(in_iface != nullptr && out_velocity != nullptr);
    auto v = reinterpret_cast<const JPH::BodyInterface *>(in_iface)->GetLinearVelocity(JPH::BodyID(in_body_id));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_BodyInterface_GetCenterOfMassPosition(const JPH_BodyInterface *in_iface,
                                          JPH_BodyID in_body_id,
                                          float out_position[3])
{
    assert(in_iface != nullptr && out_position != nullptr);
    auto v = reinterpret_cast<const JPH::BodyInterface *>(in_iface)->GetCenterOfMassPosition(
        JPH::BodyID(in_body_id));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_BodyInterface_IsActive(const JPH_BodyInterface *in_iface, JPH_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    return reinterpret_cast<const JPH::BodyInterface *>(in_iface)->IsActive(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
//
// JPH_Body
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyID
JPH_Body_GetID(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    const JPH::BodyID body_id = reinterpret_cast<const JPH::Body *>(in_body)->GetID();
    return *reinterpret_cast<const JPH_BodyID *>(&body_id);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_BodyID
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint32_t
JPH_BodyID_GetIndex(JPH_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).GetIndex();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint8_t
JPH_BodyID_GetSequenceNumber(JPH_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).GetSequenceNumber();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_BodyID_IsInvalid(JPH_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).IsInvalid();
}
//--------------------------------------------------------------------------------------------------
