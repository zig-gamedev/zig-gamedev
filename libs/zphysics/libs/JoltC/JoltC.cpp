#include "JoltC.h"

#include <assert.h>
#include <stddef.h>
//#include <stdio.h>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/TriangleShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

JPH_SUPPRESS_WARNINGS

#define ENSURE_TYPE(o, t) \
    assert(reinterpret_cast<const JPH::SerializableObject *>(o)->CastTo(JPH_RTTI(t)) != nullptr)

#ifdef JPH_ENABLE_ASSERTS

static bool
AssertFailedImpl(const char *in_expression,
                 const char *in_message,
                 const char *in_file,
                 uint32_t in_line)
{
	return true;
}

#endif
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterDefaultAllocator(void)
{
    JPH::RegisterDefaultAllocator();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_RegisterCustomAllocator(JPH_AllocateFunction in_alloc,
                            JPH_FreeFunction in_free,
                            JPH_AlignedAllocateFunction in_aligned_alloc,
                            JPH_AlignedFreeFunction in_aligned_free)
{
#ifndef JPH_DISABLE_CUSTOM_ALLOCATOR
    JPH::Allocate = in_alloc;
    JPH::Free = in_free;
    JPH::AlignedAllocate = in_aligned_alloc;
    JPH::AlignedFree = in_aligned_free;
#endif
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
    return reinterpret_cast<JPH_PhysicsSystem *>(new JPH::PhysicsSystem());
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
                       uint32_t in_num_body_mutexes,
                       uint32_t in_max_body_pairs,
                       uint32_t in_max_contact_constraints,
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
        in_num_body_mutexes,
        in_max_body_pairs,
        in_max_contact_constraints,
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
    physics_system->SetBodyActivationListener(
        reinterpret_cast<JPH::BodyActivationListener *>(in_listener));
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
    auto settings = new JPH::BoxShapeSettings(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_half_extent)));
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
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_SphereShapeSettings_GetRadius(const JPH_SphereShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    return reinterpret_cast<const JPH::SphereShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_SphereShapeSettings_SetRadius(JPH_SphereShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    reinterpret_cast<JPH::SphereShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_TriangleShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TriangleShapeSettings *
JPH_TriangleShapeSettings_Create(const float in_v1[3], const float in_v2[3], const float in_v3[3])
{
    assert(in_v1 != nullptr && in_v2 != nullptr && in_v3 != nullptr);
    auto settings = new JPH::TriangleShapeSettings(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v1)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v2)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v3)));
    settings->AddRef();
    return reinterpret_cast<JPH_TriangleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TriangleShapeSettings_SetVertices(JPH_TriangleShapeSettings *in_settings,
                                      const float in_v1[3],
                                      const float in_v2[3],
                                      const float in_v3[3])
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    assert(in_v1 != nullptr && in_v2 != nullptr && in_v3 != nullptr);
    auto settings = reinterpret_cast<JPH::TriangleShapeSettings *>(in_settings);
    settings->mV1 = JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v1));
    settings->mV2 = JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v2));
    settings->mV3 = JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v3));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TriangleShapeSettings_GetVertices(const JPH_TriangleShapeSettings *in_settings,
                                      float out_v1[3],
                                      float out_v2[3],
                                      float out_v3[3])
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    assert(out_v1 != nullptr && out_v2 != nullptr && out_v3 != nullptr);
    auto settings = reinterpret_cast<const JPH::TriangleShapeSettings *>(in_settings);
    settings->mV1.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_v1));
    settings->mV2.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_v2));
    settings->mV3.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_v3));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_TriangleShapeSettings_GetConvexRadius(const JPH_TriangleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    return reinterpret_cast<const JPH::TriangleShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TriangleShapeSettings_SetConvexRadius(JPH_TriangleShapeSettings *in_settings,
                                          float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    reinterpret_cast<JPH::TriangleShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_CapsuleShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CapsuleShapeSettings *
JPH_CapsuleShapeSettings_Create(float in_half_height_of_cylinder, float in_radius)
{
    auto settings = new JPH::CapsuleShapeSettings(in_half_height_of_cylinder, in_radius);
    settings->AddRef();
    return reinterpret_cast<JPH_CapsuleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_CapsuleShapeSettings_GetHalfHeightOfCylinder(const JPH_CapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    return reinterpret_cast<const JPH::CapsuleShapeSettings *>(in_settings)->mHalfHeightOfCylinder;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CapsuleShapeSettings_SetHalfHeightOfCylinder(JPH_CapsuleShapeSettings *in_settings,
                                                 float in_half_height_of_cylinder)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    reinterpret_cast<JPH::CapsuleShapeSettings *>(in_settings)->mHalfHeightOfCylinder =
        in_half_height_of_cylinder;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_CapsuleShapeSettings_GetRadius(const JPH_CapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    return reinterpret_cast<const JPH::CapsuleShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CapsuleShapeSettings_SetRadius(JPH_CapsuleShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    reinterpret_cast<JPH::CapsuleShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_TaperedCapsuleShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TaperedCapsuleShapeSettings *
JPH_TaperedCapsuleShapeSettings_Create(float in_half_height, float in_top_radius, float in_bottom_radius)
{
    auto settings = new JPH::TaperedCapsuleShapeSettings(in_half_height, in_top_radius, in_bottom_radius);
    settings->AddRef();
    return reinterpret_cast<JPH_TaperedCapsuleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetHalfHeightOfTaperedCylinder(const JPH_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mHalfHeightOfTaperedCylinder;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetHalfHeightOfTaperedCylinder(JPH_TaperedCapsuleShapeSettings *in_settings,
                                                               float in_half_height)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mHalfHeightOfTaperedCylinder = 
        in_half_height;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetTopRadius(const JPH_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mTopRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetTopRadius(JPH_TaperedCapsuleShapeSettings *in_settings, float in_top_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mTopRadius = in_top_radius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_TaperedCapsuleShapeSettings_GetBottomRadius(const JPH_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mBottomRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_TaperedCapsuleShapeSettings_SetBottomRadius(JPH_TaperedCapsuleShapeSettings *in_settings,
                                                float in_bottom_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mBottomRadius =
        in_bottom_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_CylinderShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CylinderShapeSettings *
JPH_CylinderShapeSettings_Create(float in_half_height, float in_radius)
{
    auto settings = new JPH::CylinderShapeSettings(in_half_height, in_radius);
    settings->AddRef();
    return reinterpret_cast<JPH_CylinderShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_CylinderShapeSettings_GetConvexRadius(const JPH_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CylinderShapeSettings_SetConvexRadius(JPH_CylinderShapeSettings *in_settings, float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_CylinderShapeSettings_GetHalfHeight(const JPH_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mHalfHeight;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CylinderShapeSettings_SetHalfHeight(JPH_CylinderShapeSettings *in_settings, float in_half_height)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mHalfHeight = in_half_height;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_CylinderShapeSettings_GetRadius(const JPH_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_CylinderShapeSettings_SetRadius(JPH_CylinderShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPH_ConvexHullShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_ConvexHullShapeSettings *
JPH_ConvexHullShapeSettings_Create(const float in_points[][4], int in_num_points)
{
    assert(in_points != nullptr && in_num_points > 0);
    assert((reinterpret_cast<intptr_t>(&in_points[0][0]) & 0xf) == 0);
    auto settings = new JPH::ConvexHullShapeSettings(
        reinterpret_cast<const JPH::Vec3 *>(&in_points[0][0]), in_num_points);
    settings->AddRef();
    return reinterpret_cast<JPH_ConvexHullShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_ConvexHullShapeSettings_GetMaxConvexRadius(const JPH_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mMaxConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ConvexHullShapeSettings_SetMaxConvexRadius(JPH_ConvexHullShapeSettings *in_settings,
                                               float in_max_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mMaxConvexRadius =
        in_max_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_ConvexHullShapeSettings_GetMaxErrorConvexRadius(const JPH_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mMaxErrorConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ConvexHullShapeSettings_SetMaxErrorConvexRadius(JPH_ConvexHullShapeSettings *in_settings,
                                                    float in_max_err_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mMaxErrorConvexRadius =
        in_max_err_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_ConvexHullShapeSettings_GetHullTolerance(const JPH_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mHullTolerance;
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_ConvexHullShapeSettings_SetHullTolerance(JPH_ConvexHullShapeSettings *in_settings,
                                             float in_hull_tolerance)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mHullTolerance = in_hull_tolerance;
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
                                   const JPH_BodyCreationSettings *in_settings,
                                   JPH_Activation in_mode)
{
    assert(in_iface != nullptr && in_settings != nullptr);
    auto iface = reinterpret_cast<JPH::BodyInterface *>(in_iface);
    auto settings = reinterpret_cast<const JPH::BodyCreationSettings *>(in_settings);
    const JPH::BodyID body_id = iface->CreateAndAddBody(
        *settings,
        static_cast<JPH::EActivation>(in_mode));
    return body_id.GetIndexAndSequenceNumber();
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
    auto v = reinterpret_cast<const JPH::BodyInterface *>(in_iface)->GetLinearVelocity(
        JPH::BodyID(in_body_id));
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
    return body_id.GetIndexAndSequenceNumber();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsActive(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsActive();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsStatic(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsStatic();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsKinematic(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsKinematic();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsDynamic(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsDynamic();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_CanBeKinematicOrDynamic(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->CanBeKinematicOrDynamic();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetIsSensor(JPH_Body *in_body, bool in_is_sensor)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetIsSensor(in_is_sensor);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsSensor(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsSensor();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_MotionType
JPH_Body_GetMotionType(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return static_cast<JPH_MotionType>(reinterpret_cast<const JPH::Body *>(in_body)->GetMotionType());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetMotionType(JPH_Body *in_body, JPH_MotionType in_motion_type)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetMotionType(static_cast<JPH::EMotionType>(in_motion_type));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BroadPhaseLayer
JPH_Body_GetBroadPhaseLayer(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return static_cast<JPH_BroadPhaseLayer>(reinterpret_cast<const JPH::Body *>(in_body)->GetBroadPhaseLayer());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_ObjectLayer
JPH_Body_GetObjectLayer(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return static_cast<JPH_ObjectLayer>(reinterpret_cast<const JPH::Body *>(in_body)->GetObjectLayer());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_CollisionGroup *
JPH_Body_GetCollisionGroup(JPH_Body *in_body)
{
    assert(in_body != nullptr);
    JPH::CollisionGroup *ptr = &reinterpret_cast<JPH::Body *>(in_body)->GetCollisionGroup();
    return reinterpret_cast<JPH_CollisionGroup *>(ptr);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetCollisionGroup(JPH_Body *in_body, const JPH_CollisionGroup *in_group)
{
    assert(in_body != nullptr && in_group != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetCollisionGroup(
        *reinterpret_cast<const JPH::CollisionGroup *>(in_group));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_GetAllowSleeping(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->GetAllowSleeping();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetAllowSleeping(JPH_Body *in_body, bool in_allow)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetAllowSleeping(in_allow);
}//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_Body_GetFriction(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->GetFriction();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetFriction(JPH_Body *in_body, float in_friction)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetFriction(in_friction);
}//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_Body_GetRestitution(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->GetRestitution();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetRestitution(JPH_Body *in_body, float in_restitution)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetRestitution(in_restitution);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetLinearVelocity(JPH_Body *in_body, float out_linear_velocity[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetLinearVelocity();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_linear_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetLinearVelocity(JPH_Body *in_body, const float in_linear_velocity[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetLinearVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetLinearVelocityClamped(JPH_Body *in_body, const float in_linear_velocity[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetLinearVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetAngularVelocity(JPH_Body *in_body, float out_angular_velocity[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = static_cast<JPH::Vec3>(
        reinterpret_cast<const JPH::Body *>(in_body)->GetAngularVelocity());
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_angular_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetAnglularVelocity(JPH_Body *in_body, const float in_angular_velocity[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetAngularVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetAnglularVelocityClamped(JPH_Body *in_body, const float in_angular_velocity[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetAngularVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetPointVelocityCOM(
    JPH_Body *in_body,
    const float in_point_relative_to_com[3],
    float out_velocity[3]
)
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetPointVelocityCOM(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_point_relative_to_com)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetPointVelocity(JPH_Body *in_body, const float in_point[3], float out_velocity[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetPointVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_point)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_AddForce(JPH_Body *in_body, const float in_force[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddForce(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_force)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_AddForceAtPosition(JPH_Body *in_body, const float in_force[3], const float in_position[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddForce(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_force)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_position))
    );
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_AddTorque(JPH_Body *in_body, const float in_torque[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddTorque(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_torque)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetInverseInertia(const JPH_Body *in_body, float out_inverse_inertia[16])
{
    assert(in_body != nullptr);
    const JPH::Mat44 m = reinterpret_cast<const JPH::Body *>(in_body)->GetInverseInertia();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_inverse_inertia));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_AddImpulse(JPH_Body *in_body, const float in_impulse[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddImpulse(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_impulse)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_AddAngularImpulse(JPH_Body *in_body, const float in_angular_impulse[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddAngularImpulse(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_impulse)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_MoveKinematic(JPH_Body *in_body,
                       const float in_target_position[3],
                       const float in_target_rotation[4],
                       float in_delta_time)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->MoveKinematic(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_target_position)),
        JPH::Quat(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_target_rotation))),
        in_delta_time);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_ApplyBuoyancyImpulse(JPH_Body *in_body,
                              const float in_plane[4],
                              float in_buoyancy,
                              float in_linear_drag,
                              float in_angular_drag,
                              const float in_fluid_velocity[3],
                              const float in_gravity[3],
                              float in_delta_time)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->ApplyBuoyancyImpulse(
        JPH::Plane(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_plane))),
        in_buoyancy,
        in_linear_drag,
        in_angular_drag,
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_fluid_velocity)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_gravity)),
        in_delta_time);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsInBroadPhase(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsInBroadPhase();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI bool
JPH_Body_IsCollisionCacheInvalid(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsCollisionCacheInvalid();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI const JPH_Shape *
JPH_Body_GetShape(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH_Shape *>(
        reinterpret_cast<const JPH::Body *>(in_body)->GetShape()
    );
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetPosition(const JPH_Body *in_body, float out_position[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetPosition();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetRotation(const JPH_Body *in_body, float out_rotation[4])
{
    assert(in_body != nullptr);
    const JPH::Quat q = reinterpret_cast<const JPH::Body *>(in_body)->GetRotation();
    q.GetXYZW().StoreFloat4(reinterpret_cast<JPH::Float4 *>(out_rotation));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetWorldTransform(const JPH_Body *in_body, float out_transform[16])
{
    assert(in_body != nullptr);
    const JPH::Mat44 m = reinterpret_cast<const JPH::Body *>(in_body)->GetWorldTransform();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_transform));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetCenterOfMassPosition(const JPH_Body *in_body, float out_position[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetCenterOfMassPosition();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetInverseCenterOfMassTransform(const JPH_Body *in_body, float out_transform[16])
{
    assert(in_body != nullptr);
    const JPH::Body *body = body;
    const JPH::Mat44 m = body->GetInverseCenterOfMassTransform();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_transform));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetWorldSpaceBounds(const JPH_Body *in_body, float out_min[3], float out_max[3])
{
    assert(in_body != nullptr);
    const JPH::AABox& aabb = reinterpret_cast<const JPH::Body *>(in_body)->GetWorldSpaceBounds();
    aabb.mMin.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_min));
    aabb.mMax.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_max));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_MotionProperties *
JPH_Body_GetMotionProperties(JPH_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<JPH::Body *>(in_body);
    return reinterpret_cast<JPH_MotionProperties *>(body->GetMotionProperties());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_MotionProperties *
JPH_Body_GetMotionPropertiesUnchecked(JPH_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<JPH::Body *>(in_body);
    return reinterpret_cast<JPH_MotionProperties *>(body->GetMotionPropertiesUnchecked());
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI uint64_t
JPH_Body_GetUserData(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->GetUserData();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_SetUserData(JPH_Body *in_body, uint64_t in_user_data)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetUserData(in_user_data);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_Body_GetWorldSpaceSurfaceNormal(const JPH_Body *in_body,
                                    const JPH_SubShapeID *in_sub_shape_id,
                                    const float in_position[3],
                                    float out_normal_vector[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetWorldSpaceSurfaceNormal(
        *reinterpret_cast<const JPH::SubShapeID *>(in_sub_shape_id),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_position)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_normal_vector));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_TransformedShape
JPH_Body_GetTransformedShape(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<const JPH::Body *>(in_body);
    const JPH::TransformedShape transformed_shape = body->GetTransformedShape();
    return *reinterpret_cast<const JPH_TransformedShape *>(&transformed_shape);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_BodyCreationSettings
JPH_Body_GetBodyCreationSettings(const JPH_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<const JPH::Body *>(in_body);
    const JPH::BodyCreationSettings settings = body->GetBodyCreationSettings();
    return *reinterpret_cast<const JPH_BodyCreationSettings *>(&settings);
}
//--------------------------------------------------------------------------------------------------
//
// JPH_MotionProperties
//
//--------------------------------------------------------------------------------------------------
JPH_CAPI JPH_MotionQuality
JPH_MotionProperties_GetMotionQuality(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return static_cast<JPH_MotionQuality>(
        reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMotionQuality()
    );
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetMotionQuality(JPH_MotionProperties *in_properties,
                                      JPH_MotionQuality in_motion_quality)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetMotionQuality(
        static_cast<JPH::EMotionQuality>(in_motion_quality));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetLinearVelocity(const JPH_MotionProperties *in_properties,
                                       float out_linear_velocity[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->GetLinearVelocity();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_linear_velocity));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetLinearVelocity(JPH_MotionProperties *in_properties,
                                       const float in_linear_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetLinearVelocityClamped(JPH_MotionProperties *in_properties,
                                              const float in_linear_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetAngularVelocity(JPH_MotionProperties *in_properties,
                                        const float in_angular_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetAngularVelocityClamped(JPH_MotionProperties *in_properties,
                                               const float in_angular_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_MoveKinematic(JPH_MotionProperties *in_properties,
                                   const float in_delta_position[3],
                                   const float in_delta_rotation[4],
                                   float in_delta_time)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->MoveKinematic(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_delta_position)),
        JPH::Quat(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_delta_rotation))),
        in_delta_time);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_ClampLinearVelocity(JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->ClampLinearVelocity();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_ClampAngularVelocity(JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->ClampAngularVelocity();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetLinearDamping(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetLinearDamping();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetLinearDamping(JPH_MotionProperties *in_properties,
                                      float in_linear_damping)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearDamping(in_linear_damping);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetAngularDamping(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetAngularDamping();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetAngularDamping(JPH_MotionProperties *in_properties,
                                       float in_angular_damping)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularDamping(in_angular_damping);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetGravityFactor(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetGravityFactor();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetGravityFactor(JPH_MotionProperties *in_properties,
                                      float in_gravity_factor)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetGravityFactor(in_gravity_factor);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetMassProperties(JPH_MotionProperties *in_properties,
                                       const JPH_MassProperties *in_mass_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetMassProperties(
        *reinterpret_cast<const JPH::MassProperties *>(in_mass_properties));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetInverseMass(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetInverseMass();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetInverseMassUnchecked(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetInverseMassUnchecked();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetInverseMass(JPH_MotionProperties *in_properties, float in_inv_mass)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetInverseMass(in_inv_mass);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetInverseInertiaDiagonal(const JPH_MotionProperties *in_properties,
                                               float out_inverse_inertia_diagonal[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->GetInverseInertiaDiagonal();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_inverse_inertia_diagonal));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetInertiaRotation(const JPH_MotionProperties *in_properties,
                                        float out_inertia_rotation[4])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Quat v = properties->GetInertiaRotation();
    v.GetXYZW().StoreFloat4(reinterpret_cast<JPH::Float4 *>(out_inertia_rotation));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetInverseInertia(JPH_MotionProperties *in_properties,
                                       const float in_diagonal[3],
                                       const float in_rotation[4])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetInverseInertia(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_diagonal)),
        JPH::Quat(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_rotation))));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetLocalSpaceInverseInertia(const JPH_MotionProperties *in_properties,
                                                 float out_matrix[16])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Mat44 m = properties->GetLocalSpaceInverseInertia();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_matrix));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetLocalSpaceInverseInertiaUnchecked(const JPH_MotionProperties *in_properties,
                                                          float out_matrix[16])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Mat44 m = properties->GetLocalSpaceInverseInertiaUnchecked();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_matrix));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetInverseInertiaForRotation(const JPH_MotionProperties *in_properties,
                                                  const float in_rotation_matrix[16],
                                                  float out_matrix[16])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Mat44 m = properties->GetInverseInertiaForRotation(
        *reinterpret_cast<const JPH::Mat44 *>(in_rotation_matrix));
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_matrix));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_MultiplyWorldSpaceInverseInertiaByVector(const JPH_MotionProperties *in_properties,
                                                              const float in_body_rotation[4],
                                                              const float in_vector[3],
                                                              float out_vector[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->MultiplyWorldSpaceInverseInertiaByVector(
        JPH::Quat(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_body_rotation))),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_vector)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_vector));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_GetPointVelocityCOM(const JPH_MotionProperties *in_properties,
                                         const float in_point_relative_to_com[3],
                                         float out_point[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->GetPointVelocityCOM(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_point_relative_to_com)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_point));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetMaxLinearVelocity(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMaxLinearVelocity();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetMaxLinearVelocity(JPH_MotionProperties *in_properties,
                                          float in_max_linear_velocity)
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SetMaxLinearVelocity(in_max_linear_velocity);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI float
JPH_MotionProperties_GetMaxAngularVelocity(const JPH_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMaxAngularVelocity();
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SetMaxAngularVelocity(JPH_MotionProperties *in_properties,
                                           float in_max_angular_velocity)
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SetMaxAngularVelocity(in_max_angular_velocity);
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_AddLinearVelocityStep(JPH_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->AddLinearVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SubLinearVelocityStep(JPH_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SubLinearVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_AddAngularVelocityStep(JPH_MotionProperties *in_properties,
                                            const float in_angular_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->AddAngularVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPH_CAPI void
JPH_MotionProperties_SubAngularVelocityStep(JPH_MotionProperties *in_properties,
                                            const float in_angular_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SubAngularVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity_change)));
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
