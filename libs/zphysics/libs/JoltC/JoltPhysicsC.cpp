#include "JoltPhysicsC.h"

#include <assert.h>
#include <stddef.h>

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
#include <Jolt/Physics/Collision/PhysicsMaterial.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Body/BodyLockMulti.h>

JPH_SUPPRESS_WARNINGS

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
#error Currently JoltPhysicsC does not support profiling. Please undef JPH_EXTERNAL_PROFILE and JPH_PROFILE_ENABLED.
#endif

auto toJph(const JPC_Body *in) { assert(in); return reinterpret_cast<const JPH::Body *>(in); }
auto toJph(JPC_Body *in) { assert(in); return reinterpret_cast<JPH::Body *>(in); }

auto toJpc(JPH::CollisionGroup *in) { assert(in); return reinterpret_cast<JPC_CollisionGroup *>(in); }
auto toJph(const JPC_CollisionGroup *in) { assert(in); return reinterpret_cast<const JPH::CollisionGroup *>(in); }

auto toJpc(JPH::EMotionType in) { return static_cast<JPC_MotionType>(in); }
auto toJph(JPC_MotionType in) { return static_cast<JPH::EMotionType>(in); }

auto toJpc(JPH::BroadPhaseLayer in) { return static_cast<JPC_BroadPhaseLayer>(in); }
auto toJpc(JPH::ObjectLayer in) { return static_cast<JPC_ObjectLayer>(in); }

#define ENSURE_TYPE(o, t) \
    assert(reinterpret_cast<const JPH::SerializableObject *>(o)->CastTo(JPH_RTTI(t)) != nullptr)

static inline JPH::Vec3
loadVec3(const float in_xyz[3])
{
    return JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_xyz));
}

static inline JPH::RVec3
loadRVec3(const Real in_xyz[3])
{
#if JPC_DOUBLE_PRECISION == 0
    return JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_xyz));
#else
    return JPH::DVec3(in_xyz[0], in_xyz[1], in_xyz[2]);
#endif
}

static inline void
storeVec3(float out_xyz[3], JPH::Vec3Arg in_vec3)
{
    in_vec3.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_xyz));
}

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
JPC_API void
JPC_RegisterDefaultAllocator(void)
{
    JPH::RegisterDefaultAllocator();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_RegisterCustomAllocator(JPC_AllocateFunction in_alloc,
                            JPC_FreeFunction in_free,
                            JPC_AlignedAllocateFunction in_aligned_alloc,
                            JPC_AlignedFreeFunction in_aligned_free)
{
#ifndef JPH_DISABLE_CUSTOM_ALLOCATOR
    JPH::Allocate = in_alloc;
    JPH::Free = in_free;
    JPH::AlignedAllocate = in_aligned_alloc;
    JPH::AlignedFree = in_aligned_free;
#endif
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CreateFactory(void)
{
    assert(JPH::Factory::sInstance == nullptr);
    JPH::Factory::sInstance = new JPH::Factory();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_DestroyFactory(void)
{
    assert(JPH::Factory::sInstance != nullptr);
    JPH::PhysicsMaterial::sDefault = nullptr;
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_RegisterTypes(void)
{
    JPH::RegisterTypes();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CollisionGroup_SetDefault(JPC_CollisionGroup *out_group)
{
    assert(out_group != nullptr);
    const JPH::CollisionGroup group;
    *out_group = *reinterpret_cast<const JPC_CollisionGroup *>(&group);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyCreationSettings_SetDefault(JPC_BodyCreationSettings *out_settings)
{
    assert(out_settings != nullptr);
    const JPH::BodyCreationSettings settings;
    *out_settings = *reinterpret_cast<const JPC_BodyCreationSettings *>(&settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyCreationSettings_Set(JPC_BodyCreationSettings *out_settings,
                             const JPC_Shape *in_shape,
                             const float in_position[3],
                             const float in_rotation[4],
                             JPC_MotionType in_motion_type,
                             JPC_ObjectLayer in_layer)
{
    assert(out_settings != nullptr && in_shape != nullptr && in_position != nullptr && in_rotation != nullptr);

    JPC_BodyCreationSettings settings;
    JPC_BodyCreationSettings_SetDefault(&settings);

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

    *out_settings = settings;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_TempAllocator
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TempAllocator *
JPC_TempAllocator_Create(uint32_t in_size)
{
    auto impl = new JPH::TempAllocatorImpl(in_size);
    return reinterpret_cast<JPC_TempAllocator *>(impl);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TempAllocator_Destroy(JPC_TempAllocator *in_allocator)
{
    assert(in_allocator != nullptr);
    delete reinterpret_cast<JPH::TempAllocator *>(in_allocator);
}
//--------------------------------------------------------------------------------------------------
//
// JPC_JobSystem
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_JobSystem *
JPC_JobSystem_Create(uint32_t in_max_jobs, uint32_t in_max_barriers, int in_num_threads)
{
    auto job_system = new JPH::JobSystemThreadPool(in_max_jobs, in_max_barriers, in_num_threads);
    return reinterpret_cast<JPC_JobSystem *>(job_system);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_JobSystem_Destroy(JPC_JobSystem *in_job_system)
{
    assert(in_job_system != nullptr);
    delete reinterpret_cast<JPH::JobSystemThreadPool *>(in_job_system);
}
//--------------------------------------------------------------------------------------------------
//
// JPC_PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_PhysicsSystem *
JPC_PhysicsSystem_Create(uint32_t in_max_bodies,
                         uint32_t in_num_body_mutexes,
                         uint32_t in_max_body_pairs,
                         uint32_t in_max_contact_constraints,
                         const void *in_broad_phase_layer_interface,
                         JPC_ObjectVsBroadPhaseLayerFilter in_object_vs_broad_phase_layer_filter,
                         JPC_ObjectLayerPairFilter in_object_layer_pair_filter)
{
    assert(in_broad_phase_layer_interface != nullptr);
    assert(in_object_vs_broad_phase_layer_filter != nullptr);
    assert(in_object_layer_pair_filter != nullptr);

    auto physics_system = new JPH::PhysicsSystem();

    physics_system->Init(
        in_max_bodies,
        in_num_body_mutexes,
        in_max_body_pairs,
        in_max_contact_constraints,
        *static_cast<const JPH::BroadPhaseLayerInterface *>(in_broad_phase_layer_interface),
        reinterpret_cast<JPH::ObjectVsBroadPhaseLayerFilter>(in_object_vs_broad_phase_layer_filter),
        reinterpret_cast<JPH::ObjectLayerPairFilter>(in_object_layer_pair_filter));

    return reinterpret_cast<JPC_PhysicsSystem *>(physics_system);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_Destroy(JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    delete reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_SetBodyActivationListener(JPC_PhysicsSystem *in_physics_system, void *in_listener)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    physics_system->SetBodyActivationListener(
        reinterpret_cast<JPH::BodyActivationListener *>(in_listener));
}
//--------------------------------------------------------------------------------------------------
JPC_API void *
JPC_PhysicsSystem_GetBodyActivationListener(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return physics_system->GetBodyActivationListener();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_SetContactListener(JPC_PhysicsSystem *in_physics_system, void *in_listener)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    physics_system->SetContactListener(reinterpret_cast<JPH::ContactListener *>(in_listener));
}
//--------------------------------------------------------------------------------------------------
JPC_API void *
JPC_PhysicsSystem_GetContactListener(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return physics_system->GetContactListener();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_PhysicsSystem_GetNumBodies(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetNumBodies();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_PhysicsSystem_GetNumActiveBodies(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetNumActiveBodies();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_PhysicsSystem_GetMaxBodies(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system)->GetMaxBodies();
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BodyInterface *
JPC_PhysicsSystem_GetBodyInterface(JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<JPC_BodyInterface *>(&physics_system->GetBodyInterface());
}
JPC_API JPC_BodyInterface *
JPC_PhysicsSystem_GetBodyInterfaceNoLock(JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<JPC_BodyInterface *>(&physics_system->GetBodyInterfaceNoLock());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_OptimizeBroadPhase(JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system)->OptimizeBroadPhase();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_Update(JPC_PhysicsSystem *in_physics_system,
                         float in_delta_time,
                         int in_collision_steps,
                         int in_integration_sub_steps,
                         JPC_TempAllocator *in_temp_allocator,
                         JPC_JobSystem *in_job_system)
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
JPC_API const JPC_BodyLockInterface *
JPC_PhysicsSystem_GetBodyLockInterface(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<const JPC_BodyLockInterface *>(&physics_system->GetBodyLockInterface());
}
JPC_API const JPC_BodyLockInterface *
JPC_PhysicsSystem_GetBodyLockInterfaceNoLock(const JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<const JPC_BodyLockInterface *>(&physics_system->GetBodyLockInterfaceNoLock());
}
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyLock*
//
//--------------------------------------------------------------------------------------------------
void JPC_API
JPC_BodyLockRead_Lock(JPC_BodyLockRead *out_lock,
                      const JPC_BodyLockInterface *in_lock_interface,
                      JPC_BodyID in_body_id)
{
    assert(out_lock != nullptr && in_lock_interface != nullptr);
    new (out_lock) JPH::BodyLockRead(
        *reinterpret_cast<const JPH::BodyLockInterface *>(in_lock_interface), JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
void JPC_API
JPC_BodyLockRead_Unlock(JPC_BodyLockRead *io_lock)
{
    assert(io_lock != nullptr);
    reinterpret_cast<JPH::BodyLockRead *>(io_lock)->~BodyLockRead();
}
//--------------------------------------------------------------------------------------------------
void JPC_API
JPC_BodyLockWrite_Lock(JPC_BodyLockWrite *out_lock,
                       const JPC_BodyLockInterface *in_lock_interface,
                       JPC_BodyID in_body_id)
{
    assert(out_lock != nullptr && in_lock_interface != nullptr);
    new (out_lock) JPH::BodyLockWrite(
        *reinterpret_cast<const JPH::BodyLockInterface *>(in_lock_interface), JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
void JPC_API
JPC_BodyLockWrite_Unlock(JPC_BodyLockWrite *io_lock)
{
    assert(io_lock != nullptr);
    reinterpret_cast<JPH::BodyLockWrite *>(io_lock)->~BodyLockWrite();
}
//--------------------------------------------------------------------------------------------------
//
// JPC_ShapeSettings
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ShapeSettings_AddRef(JPC_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->AddRef();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ShapeSettings_Release(JPC_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->Release();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_ShapeSettings_GetRefCount(const JPC_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    return reinterpret_cast<const JPH::ShapeSettings *>(in_settings)->GetRefCount();
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Shape *
JPC_ShapeSettings_CreateShape(const JPC_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    auto settings = reinterpret_cast<const JPH::ShapeSettings *>(in_settings);
    const JPH::Result result = settings->Create();
    if (result.HasError())
        return nullptr;
    JPH::Shape *shape = const_cast<JPH::Shape *>(result.Get().GetPtr());
    shape->AddRef();
    return reinterpret_cast<JPC_Shape *>(shape);
}
//--------------------------------------------------------------------------------------------------
JPC_API uint64_t
JPC_ShapeSettings_GetUserData(const JPC_ShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    return reinterpret_cast<const JPH::ShapeSettings *>(in_settings)->mUserData;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ShapeSettings_SetUserData(JPC_ShapeSettings *in_settings, uint64_t in_user_data)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ShapeSettings);
    reinterpret_cast<JPH::ShapeSettings *>(in_settings)->mUserData = in_user_data;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_ConvexShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API const JPC_PhysicsMaterial *
JPC_ConvexShapeSettings_GetMaterial(const JPC_ConvexShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    auto settings = reinterpret_cast<const JPH::ConvexShapeSettings *>(in_settings);
    return reinterpret_cast<const JPC_PhysicsMaterial *>(settings->mMaterial.GetPtr());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConvexShapeSettings_SetMaterial(JPC_ConvexShapeSettings *in_settings,
                                    const JPC_PhysicsMaterial *in_material)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    auto settings = reinterpret_cast<JPH::ConvexShapeSettings *>(in_settings);
    settings->mMaterial = reinterpret_cast<const JPH::PhysicsMaterial *>(in_material);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_ConvexShapeSettings_GetDensity(const JPC_ConvexShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    return reinterpret_cast<const JPH::ConvexShapeSettings *>(in_settings)->mDensity;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConvexShapeSettings_SetDensity(JPC_ConvexShapeSettings *in_settings, float in_density)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexShapeSettings);
    reinterpret_cast<JPH::ConvexShapeSettings *>(in_settings)->SetDensity(in_density);
}
//--------------------------------------------------------------------------------------------------
//
// JPC_BoxShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BoxShapeSettings *
JPC_BoxShapeSettings_Create(const float in_half_extent[3])
{
    assert(in_half_extent != nullptr);
    auto settings = new JPH::BoxShapeSettings(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_half_extent)));
    settings->AddRef();
    return reinterpret_cast<JPC_BoxShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BoxShapeSettings_GetHalfExtent(const JPC_BoxShapeSettings *in_settings, float out_half_extent[3])
{
    assert(in_settings != nullptr && out_half_extent != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    auto settings = reinterpret_cast<const JPH::BoxShapeSettings *>(in_settings);
    settings->mHalfExtent.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_half_extent));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BoxShapeSettings_SetHalfExtent(JPC_BoxShapeSettings *in_settings, const float in_half_extent[3])
{
    assert(in_settings != nullptr && in_half_extent != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    auto settings = reinterpret_cast<JPH::BoxShapeSettings *>(in_settings);
    settings->mHalfExtent = JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_half_extent));
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_BoxShapeSettings_GetConvexRadius(const JPC_BoxShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    return reinterpret_cast<const JPH::BoxShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BoxShapeSettings_SetConvexRadius(JPC_BoxShapeSettings *in_settings, float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::BoxShapeSettings);
    reinterpret_cast<JPH::BoxShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_SphereShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_SphereShapeSettings *
JPC_SphereShapeSettings_Create(float in_radius)
{
    auto settings = new JPH::SphereShapeSettings(in_radius);
    settings->AddRef();
    return reinterpret_cast<JPC_SphereShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_SphereShapeSettings_GetRadius(const JPC_SphereShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    return reinterpret_cast<const JPH::SphereShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_SphereShapeSettings_SetRadius(JPC_SphereShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::SphereShapeSettings);
    reinterpret_cast<JPH::SphereShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_TriangleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TriangleShapeSettings *
JPC_TriangleShapeSettings_Create(const float in_v1[3], const float in_v2[3], const float in_v3[3])
{
    assert(in_v1 != nullptr && in_v2 != nullptr && in_v3 != nullptr);
    auto settings = new JPH::TriangleShapeSettings(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v1)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v2)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_v3)));
    settings->AddRef();
    return reinterpret_cast<JPC_TriangleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TriangleShapeSettings_SetVertices(JPC_TriangleShapeSettings *in_settings,
                                      const float in_v1[3],
                                      const float in_v2[3],
                                      const float in_v3[3])
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    assert(in_v1 != nullptr && in_v2 != nullptr && in_v3 != nullptr);
    auto settings = reinterpret_cast<JPH::TriangleShapeSettings *>(in_settings);
    settings->mV1 = loadVec3(in_v1);
    settings->mV2 = loadVec3(in_v2);
    settings->mV3 = loadVec3(in_v3);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TriangleShapeSettings_GetVertices(const JPC_TriangleShapeSettings *in_settings,
                                      float out_v1[3],
                                      float out_v2[3],
                                      float out_v3[3])
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    assert(out_v1 != nullptr && out_v2 != nullptr && out_v3 != nullptr);
    auto settings = reinterpret_cast<const JPH::TriangleShapeSettings *>(in_settings);
    storeVec3(out_v1, settings->mV1);
    storeVec3(out_v2, settings->mV2);
    storeVec3(out_v3, settings->mV3);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_TriangleShapeSettings_GetConvexRadius(const JPC_TriangleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    return reinterpret_cast<const JPH::TriangleShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TriangleShapeSettings_SetConvexRadius(JPC_TriangleShapeSettings *in_settings,
                                          float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TriangleShapeSettings);
    reinterpret_cast<JPH::TriangleShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_CapsuleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CapsuleShapeSettings *
JPC_CapsuleShapeSettings_Create(float in_half_height_of_cylinder, float in_radius)
{
    auto settings = new JPH::CapsuleShapeSettings(in_half_height_of_cylinder, in_radius);
    settings->AddRef();
    return reinterpret_cast<JPC_CapsuleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_CapsuleShapeSettings_GetHalfHeightOfCylinder(const JPC_CapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    return reinterpret_cast<const JPH::CapsuleShapeSettings *>(in_settings)->mHalfHeightOfCylinder;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CapsuleShapeSettings_SetHalfHeightOfCylinder(JPC_CapsuleShapeSettings *in_settings,
                                                 float in_half_height_of_cylinder)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    reinterpret_cast<JPH::CapsuleShapeSettings *>(in_settings)->mHalfHeightOfCylinder =
        in_half_height_of_cylinder;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_CapsuleShapeSettings_GetRadius(const JPC_CapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    return reinterpret_cast<const JPH::CapsuleShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CapsuleShapeSettings_SetRadius(JPC_CapsuleShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CapsuleShapeSettings);
    reinterpret_cast<JPH::CapsuleShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_TaperedCapsuleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TaperedCapsuleShapeSettings *
JPC_TaperedCapsuleShapeSettings_Create(float in_half_height, float in_top_radius, float in_bottom_radius)
{
    auto settings = new JPH::TaperedCapsuleShapeSettings(in_half_height, in_top_radius, in_bottom_radius);
    settings->AddRef();
    return reinterpret_cast<JPC_TaperedCapsuleShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_TaperedCapsuleShapeSettings_GetHalfHeightOfTaperedCylinder(const JPC_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mHalfHeightOfTaperedCylinder;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TaperedCapsuleShapeSettings_SetHalfHeightOfTaperedCylinder(JPC_TaperedCapsuleShapeSettings *in_settings,
                                                               float in_half_height)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mHalfHeightOfTaperedCylinder = 
        in_half_height;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_TaperedCapsuleShapeSettings_GetTopRadius(const JPC_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mTopRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TaperedCapsuleShapeSettings_SetTopRadius(JPC_TaperedCapsuleShapeSettings *in_settings, float in_top_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mTopRadius = in_top_radius;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_TaperedCapsuleShapeSettings_GetBottomRadius(const JPC_TaperedCapsuleShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    return reinterpret_cast<const JPH::TaperedCapsuleShapeSettings *>(in_settings)->mBottomRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_TaperedCapsuleShapeSettings_SetBottomRadius(JPC_TaperedCapsuleShapeSettings *in_settings,
                                                float in_bottom_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::TaperedCapsuleShapeSettings);
    reinterpret_cast<JPH::TaperedCapsuleShapeSettings *>(in_settings)->mBottomRadius =
        in_bottom_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_CylinderShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CylinderShapeSettings *
JPC_CylinderShapeSettings_Create(float in_half_height, float in_radius)
{
    auto settings = new JPH::CylinderShapeSettings(in_half_height, in_radius);
    settings->AddRef();
    return reinterpret_cast<JPC_CylinderShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_CylinderShapeSettings_GetConvexRadius(const JPC_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CylinderShapeSettings_SetConvexRadius(JPC_CylinderShapeSettings *in_settings, float in_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mConvexRadius = in_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_CylinderShapeSettings_GetHalfHeight(const JPC_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mHalfHeight;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CylinderShapeSettings_SetHalfHeight(JPC_CylinderShapeSettings *in_settings, float in_half_height)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mHalfHeight = in_half_height;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_CylinderShapeSettings_GetRadius(const JPC_CylinderShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    return reinterpret_cast<const JPH::CylinderShapeSettings *>(in_settings)->mRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_CylinderShapeSettings_SetRadius(JPC_CylinderShapeSettings *in_settings, float in_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::CylinderShapeSettings);
    reinterpret_cast<JPH::CylinderShapeSettings *>(in_settings)->mRadius = in_radius;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_ConvexHullShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_ConvexHullShapeSettings *
JPC_ConvexHullShapeSettings_Create(const float in_points[][4], int in_num_points)
{
    assert(in_points != nullptr && in_num_points > 0);
    assert((reinterpret_cast<uintptr_t>(&in_points[0][0]) & 0xf) == 0);
    auto settings = new JPH::ConvexHullShapeSettings(
        reinterpret_cast<const JPH::Vec3 *>(&in_points[0][0]), in_num_points);
    settings->AddRef();
    return reinterpret_cast<JPC_ConvexHullShapeSettings *>(settings);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_ConvexHullShapeSettings_GetMaxConvexRadius(const JPC_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mMaxConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConvexHullShapeSettings_SetMaxConvexRadius(JPC_ConvexHullShapeSettings *in_settings,
                                               float in_max_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mMaxConvexRadius = in_max_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_ConvexHullShapeSettings_GetMaxErrorConvexRadius(const JPC_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mMaxErrorConvexRadius;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConvexHullShapeSettings_SetMaxErrorConvexRadius(JPC_ConvexHullShapeSettings *in_settings,
                                                    float in_max_err_convex_radius)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mMaxErrorConvexRadius =
        in_max_err_convex_radius;
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_ConvexHullShapeSettings_GetHullTolerance(const JPC_ConvexHullShapeSettings *in_settings)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    return reinterpret_cast<const JPH::ConvexHullShapeSettings *>(in_settings)->mHullTolerance;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConvexHullShapeSettings_SetHullTolerance(JPC_ConvexHullShapeSettings *in_settings,
                                             float in_hull_tolerance)
{
    assert(in_settings != nullptr);
    ENSURE_TYPE(in_settings, JPH::ConvexHullShapeSettings);
    reinterpret_cast<JPH::ConvexHullShapeSettings *>(in_settings)->mHullTolerance = in_hull_tolerance;
}
//--------------------------------------------------------------------------------------------------
//
// JPC_Shape
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Shape_AddRef(JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    reinterpret_cast<JPH::Shape *>(in_shape)->AddRef();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Shape_Release(JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    reinterpret_cast<JPH::Shape *>(in_shape)->Release();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_Shape_GetRefCount(const JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<const JPH::Shape *>(in_shape)->GetRefCount();
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_ShapeType
JPC_Shape_GetType(const JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return static_cast<JPC_ShapeType>(reinterpret_cast<const JPH::Shape *>(in_shape)->GetType());
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_ShapeSubType
JPC_Shape_GetSubType(const JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return static_cast<JPC_ShapeSubType>(reinterpret_cast<const JPH::Shape *>(in_shape)->GetSubType());
}
//--------------------------------------------------------------------------------------------------
JPC_API uint64_t
JPC_Shape_GetUserData(const JPC_Shape *in_shape)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<const JPH::Shape *>(in_shape)->GetUserData();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Shape_SetUserData(JPC_Shape *in_shape, uint64_t in_user_data)
{
    assert(in_shape != nullptr);
    return reinterpret_cast<JPH::Shape *>(in_shape)->SetUserData(in_user_data);
}
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyInterface
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Body *
JPC_BodyInterface_CreateBody(JPC_BodyInterface *in_iface, const JPC_BodyCreationSettings *in_setting)
{
    assert(in_iface != nullptr && in_setting != nullptr);
    auto iface = reinterpret_cast<JPH::BodyInterface *>(in_iface);
    auto settings = reinterpret_cast<const JPH::BodyCreationSettings *>(in_setting);
    return reinterpret_cast<JPC_Body *>(iface->CreateBody(*settings));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_DestroyBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->DestroyBody(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_AddBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, JPC_Activation in_mode)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->AddBody(
        JPH::BodyID(in_body_id),
        static_cast<JPH::EActivation>(in_mode));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_RemoveBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->RemoveBody(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BodyID
JPC_BodyInterface_CreateAndAddBody(JPC_BodyInterface *in_iface,
                                   const JPC_BodyCreationSettings *in_settings,
                                   JPC_Activation in_mode)
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
JPC_API bool
JPC_BodyInterface_IsAdded(const JPC_BodyInterface *in_iface, JPC_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    return reinterpret_cast<const JPH::BodyInterface *>(in_iface)->IsAdded(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_SetLinearVelocity(JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    const float in_velocity[3])
{
    assert(in_iface != nullptr && in_velocity != nullptr);
    reinterpret_cast<JPH::BodyInterface *>(in_iface)->SetLinearVelocity(
        JPH::BodyID(in_body_id),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_GetLinearVelocity(const JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    float out_velocity[3])
{
    assert(in_iface != nullptr && out_velocity != nullptr);
    auto v = reinterpret_cast<const JPH::BodyInterface *>(in_iface)->GetLinearVelocity(
        JPH::BodyID(in_body_id));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyInterface_GetCenterOfMassPosition(const JPC_BodyInterface *in_iface,
                                          JPC_BodyID in_body_id,
                                          float out_position[3])
{
    assert(in_iface != nullptr && out_position != nullptr);
    auto v = reinterpret_cast<const JPH::BodyInterface *>(in_iface)->GetCenterOfMassPosition(
        JPH::BodyID(in_body_id));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_BodyInterface_IsActive(const JPC_BodyInterface *in_iface, JPC_BodyID in_body_id)
{
    assert(in_iface != nullptr);
    return reinterpret_cast<const JPH::BodyInterface *>(in_iface)->IsActive(JPH::BodyID(in_body_id));
}
//--------------------------------------------------------------------------------------------------
//
// JPC_Body
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BodyID
JPC_Body_GetID(const JPC_Body *in_body)
{
    return toJph(in_body)->GetID().GetIndexAndSequenceNumber();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsActive(const JPC_Body *in_body)
{
    return toJph(in_body)->IsActive();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsStatic(const JPC_Body *in_body)
{
    return toJph(in_body)->IsStatic();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsKinematic(const JPC_Body *in_body)
{
    return toJph(in_body)->IsKinematic();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsDynamic(const JPC_Body *in_body)
{
    return toJph(in_body)->IsDynamic();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_CanBeKinematicOrDynamic(const JPC_Body *in_body)
{
    return toJph(in_body)->CanBeKinematicOrDynamic();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetIsSensor(JPC_Body *in_body, bool in_is_sensor)
{
    toJph(in_body)->SetIsSensor(in_is_sensor);
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsSensor(const JPC_Body *in_body)
{
    return toJph(in_body)->IsSensor();
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MotionType
JPC_Body_GetMotionType(const JPC_Body *in_body)
{
    return toJpc(toJph(in_body)->GetMotionType());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetMotionType(JPC_Body *in_body, JPC_MotionType in_motion_type)
{
    toJph(in_body)->SetMotionType(toJph(in_motion_type));
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BroadPhaseLayer
JPC_Body_GetBroadPhaseLayer(const JPC_Body *in_body)
{
    return toJpc(toJph(in_body)->GetBroadPhaseLayer());
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_ObjectLayer
JPC_Body_GetObjectLayer(const JPC_Body *in_body)
{
    return toJpc(toJph(in_body)->GetObjectLayer());
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CollisionGroup *
JPC_Body_GetCollisionGroup(JPC_Body *in_body)
{
    return toJpc(&toJph(in_body)->GetCollisionGroup());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetCollisionGroup(JPC_Body *in_body, const JPC_CollisionGroup *in_group)
{
    toJph(in_body)->SetCollisionGroup(*toJph(in_group));
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_GetAllowSleeping(const JPC_Body *in_body)
{
    return toJph(in_body)->GetAllowSleeping();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetAllowSleeping(JPC_Body *in_body, bool in_allow)
{
    toJph(in_body)->SetAllowSleeping(in_allow);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_Body_GetFriction(const JPC_Body *in_body)
{
    return toJph(in_body)->GetFriction();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetFriction(JPC_Body *in_body, float in_friction)
{
    toJph(in_body)->SetFriction(in_friction);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_Body_GetRestitution(const JPC_Body *in_body)
{
    return toJph(in_body)->GetRestitution();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetRestitution(JPC_Body *in_body, float in_restitution)
{
    toJph(in_body)->SetRestitution(in_restitution);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetLinearVelocity(JPC_Body *in_body, float out_linear_velocity[3])
{
    storeVec3(out_linear_velocity, toJph(in_body)->GetLinearVelocity());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetLinearVelocity(JPC_Body *in_body, const float in_linear_velocity[3])
{
    toJph(in_body)->SetLinearVelocity(loadVec3(in_linear_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetLinearVelocityClamped(JPC_Body *in_body, const float in_linear_velocity[3])
{
    toJph(in_body)->SetLinearVelocityClamped(loadVec3(in_linear_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetAngularVelocity(JPC_Body *in_body, float out_angular_velocity[3])
{
    storeVec3(out_angular_velocity, toJph(in_body)->GetAngularVelocity());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetAnglularVelocity(JPC_Body *in_body, const float in_angular_velocity[3])
{
    toJph(in_body)->SetAngularVelocity(loadVec3(in_angular_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetAnglularVelocityClamped(JPC_Body *in_body, const float in_angular_velocity[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetAngularVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetPointVelocityCOM(
    JPC_Body *in_body,
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
JPC_API void
JPC_Body_GetPointVelocity(JPC_Body *in_body, const float in_point[3], float out_velocity[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetPointVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_point)));
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_AddForce(JPC_Body *in_body, const float in_force[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddForce(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_force)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_AddForceAtPosition(JPC_Body *in_body, const float in_force[3], const float in_position[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddForce(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_force)),
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_position))
    );
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_AddTorque(JPC_Body *in_body, const float in_torque[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddTorque(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_torque)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetInverseInertia(const JPC_Body *in_body, float out_inverse_inertia[16])
{
    assert(in_body != nullptr);
    const JPH::Mat44 m = reinterpret_cast<const JPH::Body *>(in_body)->GetInverseInertia();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_inverse_inertia));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_AddImpulse(JPC_Body *in_body, const float in_impulse[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddImpulse(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_impulse)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_AddAngularImpulse(JPC_Body *in_body, const float in_angular_impulse[3])
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->AddAngularImpulse(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_impulse)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_MoveKinematic(JPC_Body *in_body,
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
JPC_API void
JPC_Body_ApplyBuoyancyImpulse(JPC_Body *in_body,
                              const Real in_surface_position[3],
                              const float in_surface_normal[3],
                              float in_buoyancy,
                              float in_linear_drag,
                              float in_angular_drag,
                              const float in_fluid_velocity[3],
                              const float in_gravity[3],
                              float in_delta_time)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->ApplyBuoyancyImpulse(
        loadRVec3(in_surface_position),
        loadVec3(in_surface_normal),
        in_buoyancy,
        in_linear_drag,
        in_angular_drag,
        loadVec3(in_fluid_velocity),
        loadVec3(in_gravity),
        in_delta_time);
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsInBroadPhase(const JPC_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsInBroadPhase();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_Body_IsCollisionCacheInvalid(const JPC_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->IsCollisionCacheInvalid();
}
//--------------------------------------------------------------------------------------------------
JPC_API const JPC_Shape *
JPC_Body_GetShape(const JPC_Body *in_body)
{
    assert(in_body != nullptr);
    const JPC_Shape* shape = reinterpret_cast<const JPC_Shape *>(
        reinterpret_cast<const JPH::Body *>(in_body)->GetShape()
    );
    assert(shape != nullptr);
    JPC_Shape_AddRef(const_cast<JPC_Shape *>(shape));
    return shape;
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetPosition(const JPC_Body *in_body, float out_position[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetPosition();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetRotation(const JPC_Body *in_body, float out_rotation[4])
{
    assert(in_body != nullptr);
    const JPH::Quat q = reinterpret_cast<const JPH::Body *>(in_body)->GetRotation();
    q.GetXYZW().StoreFloat4(reinterpret_cast<JPH::Float4 *>(out_rotation));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetWorldTransform(const JPC_Body *in_body, float out_transform[16])
{
    assert(in_body != nullptr);
    const JPH::Mat44 m = reinterpret_cast<const JPH::Body *>(in_body)->GetWorldTransform();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_transform));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetCenterOfMassPosition(const JPC_Body *in_body, float out_position[3])
{
    assert(in_body != nullptr);
    const JPH::Vec3 v = reinterpret_cast<const JPH::Body *>(in_body)->GetCenterOfMassPosition();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_position));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetInverseCenterOfMassTransform(const JPC_Body *in_body, float out_transform[16])
{
    assert(in_body != nullptr);
    const JPH::Body *body = body;
    const JPH::Mat44 m = body->GetInverseCenterOfMassTransform();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_transform));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetWorldSpaceBounds(const JPC_Body *in_body, float out_min[3], float out_max[3])
{
    assert(in_body != nullptr);
    const JPH::AABox& aabb = reinterpret_cast<const JPH::Body *>(in_body)->GetWorldSpaceBounds();
    aabb.mMin.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_min));
    aabb.mMax.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_max));
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MotionProperties *
JPC_Body_GetMotionProperties(JPC_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<JPH::Body *>(in_body);
    return reinterpret_cast<JPC_MotionProperties *>(body->GetMotionProperties());
}
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MotionProperties *
JPC_Body_GetMotionPropertiesUnchecked(JPC_Body *in_body)
{
    assert(in_body != nullptr);
    const auto body = reinterpret_cast<JPH::Body *>(in_body);
    return reinterpret_cast<JPC_MotionProperties *>(body->GetMotionPropertiesUnchecked());
}
//--------------------------------------------------------------------------------------------------
JPC_API uint64_t
JPC_Body_GetUserData(const JPC_Body *in_body)
{
    assert(in_body != nullptr);
    return reinterpret_cast<const JPH::Body *>(in_body)->GetUserData();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_SetUserData(JPC_Body *in_body, uint64_t in_user_data)
{
    assert(in_body != nullptr);
    reinterpret_cast<JPH::Body *>(in_body)->SetUserData(in_user_data);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetWorldSpaceSurfaceNormal(const JPC_Body *in_body,
                                    const JPC_SubShapeID *in_sub_shape_id,
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
JPC_API void
JPC_Body_GetTransformedShape(const JPC_Body *in_body, JPC_TransformedShape *out_shape)
{
    assert(in_body != nullptr && out_shape != nullptr);
    const auto body = reinterpret_cast<const JPH::Body *>(in_body);
    const JPH::TransformedShape transformed_shape = body->GetTransformedShape();
    *out_shape = *reinterpret_cast<const JPC_TransformedShape *>(&transformed_shape);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Body_GetBodyCreationSettings(const JPC_Body *in_body, JPC_BodyCreationSettings *out_settings)
{
    assert(in_body != nullptr && out_settings != nullptr);
    const auto body = reinterpret_cast<const JPH::Body *>(in_body);
    const JPH::BodyCreationSettings settings = body->GetBodyCreationSettings();
    *out_settings = *reinterpret_cast<const JPC_BodyCreationSettings *>(&settings);
}
//--------------------------------------------------------------------------------------------------
//
// JPC_MotionProperties
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MotionQuality
JPC_MotionProperties_GetMotionQuality(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return static_cast<JPC_MotionQuality>(
        reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMotionQuality()
    );
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetLinearVelocity(const JPC_MotionProperties *in_properties,
                                       float out_linear_velocity[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->GetLinearVelocity();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_linear_velocity));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetLinearVelocity(JPC_MotionProperties *in_properties,
                                       const float in_linear_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetLinearVelocityClamped(JPC_MotionProperties *in_properties,
                                              const float in_linear_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetAngularVelocity(JPC_MotionProperties *in_properties,
                                        const float in_angular_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularVelocity(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetAngularVelocityClamped(JPC_MotionProperties *in_properties,
                                               const float in_angular_velocity[3])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularVelocityClamped(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_MoveKinematic(JPC_MotionProperties *in_properties,
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
JPC_API void
JPC_MotionProperties_ClampLinearVelocity(JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->ClampLinearVelocity();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_ClampAngularVelocity(JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->ClampAngularVelocity();
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetLinearDamping(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetLinearDamping();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetLinearDamping(JPC_MotionProperties *in_properties,
                                      float in_linear_damping)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetLinearDamping(in_linear_damping);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetAngularDamping(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetAngularDamping();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetAngularDamping(JPC_MotionProperties *in_properties,
                                       float in_angular_damping)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetAngularDamping(in_angular_damping);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetGravityFactor(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetGravityFactor();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetGravityFactor(JPC_MotionProperties *in_properties,
                                      float in_gravity_factor)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetGravityFactor(in_gravity_factor);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetMassProperties(JPC_MotionProperties *in_properties,
                                       const JPC_MassProperties *in_mass_properties)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetMassProperties(
        *reinterpret_cast<const JPH::MassProperties *>(in_mass_properties));
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetInverseMass(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetInverseMass();
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetInverseMassUnchecked(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetInverseMassUnchecked();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetInverseMass(JPC_MotionProperties *in_properties, float in_inv_mass)
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetInverseMass(in_inv_mass);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetInverseInertiaDiagonal(const JPC_MotionProperties *in_properties,
                                               float out_inverse_inertia_diagonal[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Vec3 v = properties->GetInverseInertiaDiagonal();
    v.StoreFloat3(reinterpret_cast<JPH::Float3 *>(out_inverse_inertia_diagonal));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetInertiaRotation(const JPC_MotionProperties *in_properties,
                                        float out_inertia_rotation[4])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Quat v = properties->GetInertiaRotation();
    v.GetXYZW().StoreFloat4(reinterpret_cast<JPH::Float4 *>(out_inertia_rotation));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetInverseInertia(JPC_MotionProperties *in_properties,
                                       const float in_diagonal[3],
                                       const float in_rotation[4])
{
    assert(in_properties != nullptr);
    reinterpret_cast<JPH::MotionProperties *>(in_properties)->SetInverseInertia(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_diagonal)),
        JPH::Quat(JPH::Vec4::sLoadFloat4(reinterpret_cast<const JPH::Float4 *>(in_rotation))));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetLocalSpaceInverseInertia(const JPC_MotionProperties *in_properties,
                                                 float out_matrix[16])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Mat44 m = properties->GetLocalSpaceInverseInertia();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_matrix));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetLocalSpaceInverseInertiaUnchecked(const JPC_MotionProperties *in_properties,
                                                          float out_matrix[16])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<const JPH::MotionProperties *>(in_properties);
    const JPH::Mat44 m = properties->GetLocalSpaceInverseInertiaUnchecked();
    m.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(out_matrix));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_GetInverseInertiaForRotation(const JPC_MotionProperties *in_properties,
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
JPC_API void
JPC_MotionProperties_MultiplyWorldSpaceInverseInertiaByVector(const JPC_MotionProperties *in_properties,
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
JPC_API void
JPC_MotionProperties_GetPointVelocityCOM(const JPC_MotionProperties *in_properties,
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
JPC_API float
JPC_MotionProperties_GetMaxLinearVelocity(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMaxLinearVelocity();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetMaxLinearVelocity(JPC_MotionProperties *in_properties,
                                          float in_max_linear_velocity)
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SetMaxLinearVelocity(in_max_linear_velocity);
}
//--------------------------------------------------------------------------------------------------
JPC_API float
JPC_MotionProperties_GetMaxAngularVelocity(const JPC_MotionProperties *in_properties)
{
    assert(in_properties != nullptr);
    return reinterpret_cast<const JPH::MotionProperties *>(in_properties)->GetMaxAngularVelocity();
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SetMaxAngularVelocity(JPC_MotionProperties *in_properties,
                                           float in_max_angular_velocity)
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SetMaxAngularVelocity(in_max_angular_velocity);
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_AddLinearVelocityStep(JPC_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->AddLinearVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SubLinearVelocityStep(JPC_MotionProperties *in_properties,
                                           const float in_linear_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SubLinearVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_linear_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_AddAngularVelocityStep(JPC_MotionProperties *in_properties,
                                            const float in_angular_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->AddAngularVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_MotionProperties_SubAngularVelocityStep(JPC_MotionProperties *in_properties,
                                            const float in_angular_velocity_change[3])
{
    assert(in_properties != nullptr);
    const auto properties = reinterpret_cast<JPH::MotionProperties *>(in_properties);
    properties->SubAngularVelocityStep(
        JPH::Vec3(*reinterpret_cast<const JPH::Float3 *>(in_angular_velocity_change)));
}
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyID
//
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_BodyID_GetIndex(JPC_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).GetIndex();
}
//--------------------------------------------------------------------------------------------------
JPC_API uint8_t
JPC_BodyID_GetSequenceNumber(JPC_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).GetSequenceNumber();
}
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_BodyID_IsInvalid(JPC_BodyID in_body_id)
{
    return JPH::BodyID(in_body_id).IsInvalid();
}
//--------------------------------------------------------------------------------------------------
