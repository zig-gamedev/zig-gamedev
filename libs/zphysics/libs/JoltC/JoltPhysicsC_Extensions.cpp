//--------------------------------------------------------------------------------------------------
#include "JoltPhysicsC.h"
#include <assert.h>

#ifdef _MSC_VER
#define _ALLOW_KEYWORD_MACROS
#endif

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/RayCast.h>
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
#include <Jolt/Physics/Body/BodyLock.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <Jolt/Physics/PhysicsLock.cpp>
#endif

JPH_SUPPRESS_WARNINGS
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Body **
JPC_PhysicsSystem_GetBodiesUnsafe(JPC_PhysicsSystem *in_physics_system)
{
    assert(in_physics_system != nullptr);
    auto physics_system = reinterpret_cast<JPH::PhysicsSystem *>(in_physics_system);
    return reinterpret_cast<JPC_Body **>(physics_system->mBodyManager.mBodies.data());
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_GetBodyIDs(const JPC_PhysicsSystem *in_physics_system,
                             uint32_t in_max_body_ids,
                             uint32_t *out_num_body_ids,
                             JPC_BodyID *out_body_ids)
{
    assert(in_physics_system != nullptr && out_body_ids != nullptr);
    assert(in_max_body_ids > 0);

    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);

    JPH::UniqueLock lock(
        physics_system->mBodyManager.mBodiesMutex
        JPH_IF_ENABLE_ASSERTS(, &physics_system->mBodyManager, JPH::EPhysicsLockTypes::BodiesList));

    if (out_num_body_ids) *out_num_body_ids = 0;

    for (const JPH::Body *b : physics_system->mBodyManager.mBodies)
        if (JPH::BodyManager::sIsValidBodyPointer(b))
        {
            *out_body_ids = b->GetID().GetIndexAndSequenceNumber();
            out_body_ids += 1;
            if (out_num_body_ids) *out_num_body_ids += 1;
            in_max_body_ids -= 1;
            if (in_max_body_ids == 0)
                break;
        }
}
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_PhysicsSystem_GetActiveBodyIDs(const JPC_PhysicsSystem *in_physics_system,
                                   uint32_t in_max_body_ids,
                                   uint32_t *out_num_body_ids,
                                   JPC_BodyID *out_body_ids)
{
    assert(in_physics_system != nullptr && out_body_ids != nullptr);
    assert(in_max_body_ids > 0);

    auto physics_system = reinterpret_cast<const JPH::PhysicsSystem *>(in_physics_system);

    JPH::UniqueLock lock(
        physics_system->mBodyManager.mBodiesMutex
        JPH_IF_ENABLE_ASSERTS(, &physics_system->mBodyManager, JPH::EPhysicsLockTypes::BodiesList));

    if (out_num_body_ids) *out_num_body_ids = 0;

    for (uint32_t i = 0; i < physics_system->mBodyManager.mNumActiveBodies; ++i)
    {
        const JPH::BodyID body_id = physics_system->mBodyManager.mActiveBodies[i];
        *out_body_ids = body_id.GetIndexAndSequenceNumber();
        out_body_ids += 1;
        if (out_num_body_ids) *out_num_body_ids += 1;
        in_max_body_ids -= 1;
        if (in_max_body_ids == 0)
            break;
    }
}
//--------------------------------------------------------------------------------------------------
static_assert(JPC_COLLISION_GROUP_INVALID_GROUP     == JPH::CollisionGroup::cInvalidGroup);
static_assert(JPC_COLLISION_GROUP_INVALID_SUB_GROUP == JPH::CollisionGroup::cInvalidSubGroup);
static_assert(JPC_BODY_ID_INVALID                   == JPH::BodyID::cInvalidBodyID);
static_assert(JPC_BODY_ID_INDEX_BITS                == JPH::BodyID::cMaxBodyIndex);
static_assert(_JPC_IS_FREED_BODY_BIT                == JPH::BodyManager::cIsFreedBody);
static_assert(JPC_SUB_SHAPE_ID_EMPTY                == JPH::SubShapeID::cEmpty);

static_assert((JPC_BODY_ID_SEQUENCE_BITS >> JPC_BODY_ID_SEQUENCE_SHIFT) == JPH::BodyID::cMaxSequenceNumber);
//--------------------------------------------------------------------------------------------------
#define ENSURE_SIZE_ALIGN(type0, type1) \
    static_assert(sizeof(type0) == sizeof(type1)); \
    static_assert(alignof(type0) == alignof(type1));

ENSURE_SIZE_ALIGN(JPH::BodyID,                  JPC_BodyID)
ENSURE_SIZE_ALIGN(JPH::SubShapeID,              JPC_SubShapeID)
ENSURE_SIZE_ALIGN(JPH::SubShapeIDCreator,       JPC_SubShapeIDCreator)
ENSURE_SIZE_ALIGN(JPH::EShapeType,              JPC_ShapeType)
ENSURE_SIZE_ALIGN(JPH::EShapeSubType,           JPC_ShapeSubType)
ENSURE_SIZE_ALIGN(JPH::EMotionType,             JPC_MotionType)
ENSURE_SIZE_ALIGN(JPH::EMotionQuality,          JPC_MotionQuality)
ENSURE_SIZE_ALIGN(JPH::EBackFaceMode,           JPC_BackFaceMode)
ENSURE_SIZE_ALIGN(JPH::EOverrideMassProperties, JPC_OverrideMassProperties)
ENSURE_SIZE_ALIGN(JPH::EActivation,             JPC_Activation)
ENSURE_SIZE_ALIGN(JPH::ValidateResult,          JPC_ValidateResult)
ENSURE_SIZE_ALIGN(JPH::BroadPhaseLayer,         JPC_BroadPhaseLayer)
ENSURE_SIZE_ALIGN(JPH::ObjectLayer,             JPC_ObjectLayer)

ENSURE_SIZE_ALIGN(JPH::CollisionGroup::GroupID,    JPC_CollisionGroupID)
ENSURE_SIZE_ALIGN(JPH::CollisionGroup::SubGroupID, JPC_CollisionSubGroupID)

ENSURE_SIZE_ALIGN(JPH::MassProperties,       JPC_MassProperties)
ENSURE_SIZE_ALIGN(JPH::MotionProperties,     JPC_MotionProperties)
ENSURE_SIZE_ALIGN(JPH::CollisionGroup,       JPC_CollisionGroup)
ENSURE_SIZE_ALIGN(JPH::BodyCreationSettings, JPC_BodyCreationSettings)
ENSURE_SIZE_ALIGN(JPH::ContactManifold,      JPC_ContactManifold)
ENSURE_SIZE_ALIGN(JPH::ContactSettings,      JPC_ContactSettings)
ENSURE_SIZE_ALIGN(JPH::SubShapeIDPair,       JPC_SubShapeIDPair)
ENSURE_SIZE_ALIGN(JPH::CollideShapeResult,   JPC_CollideShapeResult)
ENSURE_SIZE_ALIGN(JPH::TransformedShape,     JPC_TransformedShape)
ENSURE_SIZE_ALIGN(JPH::Body,                 JPC_Body)

ENSURE_SIZE_ALIGN(JPH::BodyLockRead,  JPC_BodyLockRead)
ENSURE_SIZE_ALIGN(JPH::BodyLockWrite, JPC_BodyLockWrite)

ENSURE_SIZE_ALIGN(JPH::RayCast, JPC_RayCast)
ENSURE_SIZE_ALIGN(JPH::RRayCast, JPC_RRayCast)
ENSURE_SIZE_ALIGN(JPH::RayCastResult, JPC_RayCastResult)
ENSURE_SIZE_ALIGN(JPH::RayCastSettings, JPC_RayCastSettings)

ENSURE_SIZE_ALIGN(JPH::AABox, JPC_AABox)
ENSURE_SIZE_ALIGN(JPH::RMat44, JPC_RMatrix)
//--------------------------------------------------------------------------------------------------
#define ENSURE_ENUM_EQ(c_const, cpp_enum) static_assert(c_const == static_cast<int>(cpp_enum))

ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_CONVEX,       JPH::EShapeType::Convex);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_COMPOUND,     JPH::EShapeType::Compound);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_DECORATED,    JPH::EShapeType::Decorated);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_MESH,         JPH::EShapeType::Mesh);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_HEIGHT_FIELD, JPH::EShapeType::HeightField);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_USER1,        JPH::EShapeType::User1);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_USER2,        JPH::EShapeType::User2);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_USER3,        JPH::EShapeType::User3);
ENSURE_ENUM_EQ(JPC_SHAPE_TYPE_USER4,        JPH::EShapeType::User4);

ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_SPHERE,                JPH::EShapeSubType::Sphere);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_BOX,                   JPH::EShapeSubType::Box);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_TRIANGLE,              JPH::EShapeSubType::Triangle);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_CAPSULE,               JPH::EShapeSubType::Capsule);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_TAPERED_CAPSULE,       JPH::EShapeSubType::TaperedCapsule);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_CYLINDER,              JPH::EShapeSubType::Cylinder);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_CONVEX_HULL,           JPH::EShapeSubType::ConvexHull);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_STATIC_COMPOUND,       JPH::EShapeSubType::StaticCompound);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_MUTABLE_COMPOUND,      JPH::EShapeSubType::MutableCompound);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_ROTATED_TRANSLATED,    JPH::EShapeSubType::RotatedTranslated);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_SCALED,                JPH::EShapeSubType::Scaled);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS, JPH::EShapeSubType::OffsetCenterOfMass);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_MESH,                  JPH::EShapeSubType::Mesh);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_HEIGHT_FIELD,          JPH::EShapeSubType::HeightField);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER1,                 JPH::EShapeSubType::User1);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER2,                 JPH::EShapeSubType::User2);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER3,                 JPH::EShapeSubType::User3);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER4,                 JPH::EShapeSubType::User4);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER5,                 JPH::EShapeSubType::User5);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER6,                 JPH::EShapeSubType::User6);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER7,                 JPH::EShapeSubType::User7);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER8,                 JPH::EShapeSubType::User8);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX1,          JPH::EShapeSubType::UserConvex1);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX2,          JPH::EShapeSubType::UserConvex2);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX3,          JPH::EShapeSubType::UserConvex3);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX4,          JPH::EShapeSubType::UserConvex4);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX5,          JPH::EShapeSubType::UserConvex5);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX6,          JPH::EShapeSubType::UserConvex6);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX7,          JPH::EShapeSubType::UserConvex7);
ENSURE_ENUM_EQ(JPC_SHAPE_SUB_TYPE_USER_CONVEX8,          JPH::EShapeSubType::UserConvex8);

ENSURE_ENUM_EQ(JPC_MOTION_TYPE_STATIC,    JPH::EMotionType::Static);
ENSURE_ENUM_EQ(JPC_MOTION_TYPE_KINEMATIC, JPH::EMotionType::Kinematic);
ENSURE_ENUM_EQ(JPC_MOTION_TYPE_DYNAMIC,   JPH::EMotionType::Dynamic);

ENSURE_ENUM_EQ(JPC_MOTION_QUALITY_DISCRETE,    JPH::EMotionQuality::Discrete);
ENSURE_ENUM_EQ(JPC_MOTION_QUALITY_LINEAR_CAST, JPH::EMotionQuality::LinearCast);

ENSURE_ENUM_EQ(JPC_ACTIVATION_ACTIVATE,      JPH::EActivation::Activate);
ENSURE_ENUM_EQ(JPC_ACTIVATION_DONT_ACTIVATE, JPH::EActivation::DontActivate);

ENSURE_ENUM_EQ(JPC_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA,
               JPH::EOverrideMassProperties::CalculateMassAndInertia);
ENSURE_ENUM_EQ(JPC_OVERRIDE_MASS_PROPS_CALC_INERTIA,
               JPH::EOverrideMassProperties::CalculateInertia);
ENSURE_ENUM_EQ(JPC_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED,
               JPH::EOverrideMassProperties::MassAndInertiaProvided);

ENSURE_ENUM_EQ(JPC_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS,
               JPH::ValidateResult::AcceptAllContactsForThisBodyPair);
ENSURE_ENUM_EQ(JPC_VALIDATE_RESULT_ACCEPT_CONTACT,
               JPH::ValidateResult::AcceptContact);
ENSURE_ENUM_EQ(JPC_VALIDATE_RESULT_REJECT_CONTACT,
               JPH::ValidateResult::RejectContact);
ENSURE_ENUM_EQ(JPC_VALIDATE_RESULT_REJECT_ALL_CONTACTS,
               JPH::ValidateResult::RejectAllContactsForThisBodyPair);

ENSURE_ENUM_EQ(JPC_MAX_PHYSICS_JOBS,     JPH::cMaxPhysicsJobs);
ENSURE_ENUM_EQ(JPC_MAX_PHYSICS_BARRIERS, JPH::cMaxPhysicsBarriers);

ENSURE_ENUM_EQ(JPC_BACK_FACE_IGNORE,  JPH::EBackFaceMode::IgnoreBackFaces);
ENSURE_ENUM_EQ(JPC_BACK_FACE_COLLIDE, JPH::EBackFaceMode::CollideWithBackFaces);
//--------------------------------------------------------------------------------------------------
static_assert(
    offsetof(JPH::BodyCreationSettings, mInertiaMultiplier) ==
    offsetof(JPC_BodyCreationSettings, inertia_multiplier));
static_assert(
    offsetof(JPH::BodyCreationSettings, mIsSensor) == offsetof(JPC_BodyCreationSettings, is_sensor));
static_assert(
    offsetof(JPH::BodyCreationSettings, mAngularDamping) == offsetof(JPC_BodyCreationSettings, angular_damping));

static_assert(
    offsetof(JPH::ContactManifold, mWorldSpaceNormal) == offsetof(JPC_ContactManifold, normal));
static_assert(
    offsetof(JPH::ContactManifold, mPenetrationDepth) == offsetof(JPC_ContactManifold, penetration_depth));
static_assert(
        offsetof(JPH::ContactManifold, mRelativeContactPointsOn1) ==
        offsetof(JPC_ContactManifold, shape1_relative_contact));
static_assert(
    offsetof(JPH::ContactManifold, mRelativeContactPointsOn2) ==
    offsetof(JPC_ContactManifold, shape2_relative_contact));

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) == offsetof(JPC_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) == offsetof(JPC_CollideShapeResult, shape1_face));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape2Face) == offsetof(JPC_CollideShapeResult, shape2_face));
static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) == offsetof(JPC_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mBodyID2) == offsetof(JPC_CollideShapeResult, body2_id));

static_assert(offsetof(JPH::MotionProperties, mForce) == offsetof(JPC_MotionProperties, force));
static_assert(offsetof(JPH::MotionProperties, mTorque) == offsetof(JPC_MotionProperties, torque));
static_assert(offsetof(JPH::MotionProperties, mMotionQuality) == offsetof(JPC_MotionProperties, motion_quality));
static_assert(offsetof(JPH::MotionProperties, mGravityFactor) == offsetof(JPC_MotionProperties, gravity_factor));
#if JPC_ENABLE_ASSERTS == 1
static_assert(
    offsetof(JPH::MotionProperties, mCachedMotionType) == offsetof(JPC_MotionProperties, cached_motion_type));
#endif

static_assert(offsetof(JPH::MassProperties, mInertia) == offsetof(JPC_MassProperties, inertia));

static_assert(offsetof(JPH::SubShapeIDPair, mBody2ID) == offsetof(JPC_SubShapeIDPair, second));

static_assert(offsetof(JPH::SubShapeIDCreator, mCurrentBit) == offsetof(JPC_SubShapeIDCreator, current_bit));

static_assert(offsetof(JPH::CollisionGroup, mGroupID) == offsetof(JPC_CollisionGroup, group_id));

static_assert(offsetof(JPH::Body, mFlags) == offsetof(JPC_Body, flags));
static_assert(offsetof(JPH::Body, mMotionProperties) == offsetof(JPC_Body, motion_properties));
static_assert(offsetof(JPH::Body, mObjectLayer) == offsetof(JPC_Body, object_layer));
static_assert(offsetof(JPH::Body, mRotation) == offsetof(JPC_Body, rotation));
static_assert(offsetof(JPH::Body, mID) == offsetof(JPC_Body, id));

static_assert(offsetof(JPH::BodyLockRead, mBodyLockInterface) == offsetof(JPC_BodyLockRead, lock_interface));
static_assert(offsetof(JPH::BodyLockRead, mBodyLockMutex) == offsetof(JPC_BodyLockRead, mutex));
static_assert(offsetof(JPH::BodyLockRead, mBody) == offsetof(JPC_BodyLockRead, body));

static_assert(offsetof(JPH::RayCastResult, mBodyID) == offsetof(JPC_RayCastResult, body_id));
static_assert(offsetof(JPH::RayCastResult, mFraction) == offsetof(JPC_RayCastResult, fraction));
static_assert(offsetof(JPH::RayCastResult, mSubShapeID2) == offsetof(JPC_RayCastResult, sub_shape_id));

static_assert(offsetof(JPH::RayCastSettings, mBackFaceMode) == offsetof(JPC_RayCastSettings, back_face_mode));
static_assert(offsetof(JPH::RayCastSettings, mTreatConvexAsSolid) ==
    offsetof(JPC_RayCastSettings, treat_convex_as_solid));

static_assert(offsetof(JPH::RRayCast, mOrigin) == offsetof(JPC_RRayCast, origin));
static_assert(offsetof(JPH::RRayCast, mDirection) == offsetof(JPC_RRayCast, direction));

static_assert(sizeof(JPH::BodyID) == 4);
static_assert(sizeof(JPH::SubShapeID) == 4);
static_assert(sizeof(JPH::CollisionGroup::GroupID) == 4);
static_assert(sizeof(JPH::CollisionGroup::SubGroupID) == 4);
//--------------------------------------------------------------------------------------------------
