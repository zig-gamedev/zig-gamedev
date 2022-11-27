//--------------------------------------------------------------------------------------------------
#include "JoltC.h"
#include <assert.h>
#define private public
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
#include <Jolt/Physics/Collision/Shape/TriangleShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Body/BodyLockMulti.h>
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
static_assert(sizeof(JPH::BodyID)                  == sizeof(JPC_BodyID));
static_assert(sizeof(JPH::SubShapeID)              == sizeof(JPC_SubShapeID));
static_assert(sizeof(JPH::SubShapeIDCreator)       == sizeof(JPC_SubShapeIDCreator));
static_assert(sizeof(JPH::EShapeType)              == sizeof(JPC_ShapeType));
static_assert(sizeof(JPH::EShapeSubType)           == sizeof(JPC_ShapeSubType));
static_assert(sizeof(JPH::EMotionType)             == sizeof(JPC_MotionType));
static_assert(sizeof(JPH::EMotionQuality)          == sizeof(JPC_MotionQuality));
static_assert(sizeof(JPH::EOverrideMassProperties) == sizeof(JPC_OverrideMassProperties));
static_assert(sizeof(JPH::EActivation)             == sizeof(JPC_Activation));
static_assert(sizeof(JPH::ValidateResult)          == sizeof(JPC_ValidateResult));
static_assert(sizeof(JPH::BroadPhaseLayer)         == sizeof(JPC_BroadPhaseLayer));
static_assert(sizeof(JPH::ObjectLayer)             == sizeof(JPC_ObjectLayer));

static_assert(sizeof(JPH::MassProperties)       == sizeof(JPC_MassProperties));
static_assert(sizeof(JPH::MotionProperties)     == sizeof(JPC_MotionProperties));
static_assert(sizeof(JPH::CollisionGroup)       == sizeof(JPC_CollisionGroup));
static_assert(sizeof(JPH::BodyCreationSettings) == sizeof(JPC_BodyCreationSettings));
static_assert(sizeof(JPH::ContactManifold)      == sizeof(JPC_ContactManifold));
static_assert(sizeof(JPH::ContactSettings)      == sizeof(JPC_ContactSettings));
static_assert(sizeof(JPH::SubShapeIDPair)       == sizeof(JPC_SubShapeIDPair));
static_assert(sizeof(JPH::CollideShapeResult)   == sizeof(JPC_CollideShapeResult));
static_assert(sizeof(JPH::TransformedShape)     == sizeof(JPC_TransformedShape));
static_assert(sizeof(JPH::Body)                 == sizeof(JPC_Body));

static_assert(sizeof(JPH::BodyLockRead)       == sizeof(JPC_BodyLockRead));
static_assert(sizeof(JPH::BodyLockWrite)      == sizeof(JPC_BodyLockWrite));
static_assert(sizeof(JPH::BodyLockMultiRead)  == sizeof(JPC_BodyLockMultiRead));
static_assert(sizeof(JPH::BodyLockMultiWrite) == sizeof(JPC_BodyLockMultiWrite));

static_assert(JPC_COLLISION_GROUP_INVALID_GROUP     == JPH::CollisionGroup::cInvalidGroup);
static_assert(JPC_COLLISION_GROUP_INVALID_SUB_GROUP == JPH::CollisionGroup::cInvalidSubGroup);
static_assert(JPC_BODY_ID_INVALID                   == JPH::BodyID::cInvalidBodyID);
static_assert(JPC_BODY_ID_INDEX_BITS                == JPH::BodyID::cMaxBodyIndex);
static_assert(JPC_BODY_ID_BROAD_PHASE_BIT           == JPH::BodyID::cBroadPhaseBit);
static_assert((JPC_BODY_ID_SEQUENCE_BITS >> JPC_BODY_ID_SEQUENCE_SHIFT) == JPH::BodyID::cMaxSequenceNumber);
//--------------------------------------------------------------------------------------------------
static_assert(alignof(JPH::MassProperties)       == alignof(JPC_MassProperties));
static_assert(alignof(JPH::MotionProperties)     == alignof(JPC_MotionProperties));
static_assert(alignof(JPH::CollisionGroup)       == alignof(JPC_CollisionGroup));
static_assert(alignof(JPH::BodyCreationSettings) == alignof(JPC_BodyCreationSettings));
static_assert(alignof(JPH::ContactManifold)      == alignof(JPC_ContactManifold));
static_assert(alignof(JPH::ContactSettings)      == alignof(JPC_ContactSettings));
static_assert(alignof(JPH::SubShapeIDPair)       == alignof(JPC_SubShapeIDPair));
static_assert(alignof(JPH::CollideShapeResult)   == alignof(JPC_CollideShapeResult));
static_assert(alignof(JPH::TransformedShape)     == alignof(JPC_TransformedShape));
static_assert(alignof(JPH::Body)                 == alignof(JPC_Body));

static_assert(alignof(JPH::BodyLockRead)       == alignof(JPC_BodyLockRead));
static_assert(alignof(JPH::BodyLockWrite)      == alignof(JPC_BodyLockWrite));
static_assert(alignof(JPH::BodyLockMultiRead)  == alignof(JPC_BodyLockMultiRead));
static_assert(alignof(JPH::BodyLockMultiWrite) == alignof(JPC_BodyLockMultiWrite));
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
//--------------------------------------------------------------------------------------------------
static_assert(
    offsetof(JPH::BodyCreationSettings, mInertiaMultiplier) ==
    offsetof(JPC_BodyCreationSettings, inertia_multiplier));
static_assert(
    offsetof(JPH::BodyCreationSettings, mIsSensor) == offsetof(JPC_BodyCreationSettings, is_sensor));
static_assert(
    offsetof(JPH::BodyCreationSettings, mAngularDamping) == offsetof(JPC_BodyCreationSettings, angular_damping));

static_assert(
    offsetof(JPH::ContactManifold, mPenetrationDepth) == offsetof(JPC_ContactManifold, penetration_depth));
static_assert(
    offsetof(JPH::ContactManifold, mWorldSpaceContactPointsOn1) == offsetof(JPC_ContactManifold, num_points1));

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) == offsetof(JPC_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) == offsetof(JPC_CollideShapeResult, num_face_points1));

static_assert(offsetof(JPH::MotionProperties, mForce) == offsetof(JPC_MotionProperties, force));
static_assert(offsetof(JPH::MotionProperties, mTorque) == offsetof(JPC_MotionProperties, torque));
static_assert(offsetof(JPH::MotionProperties, mMotionQuality) == offsetof(JPC_MotionProperties, motion_quality));
static_assert(offsetof(JPH::MotionProperties, mGravityFactor) == offsetof(JPC_MotionProperties, gravity_factor));
#ifdef JPH_ENABLE_ASSERTS
static_assert(
    offsetof(JPH::MotionProperties, mCachedMotionType) == offsetof(JPC_MotionProperties, cached_motion_type));
#endif

static_assert(offsetof(JPH::MassProperties, mInertia) == offsetof(JPC_MassProperties, inertia));

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) == offsetof(JPC_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mBodyID2) == offsetof(JPC_CollideShapeResult, body2_id));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) == offsetof(JPC_CollideShapeResult, num_face_points1));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape2Face) == offsetof(JPC_CollideShapeResult, num_face_points2));

static_assert(offsetof(JPH::SubShapeIDPair, mBody2ID) == offsetof(JPC_SubShapeIDPair, body2_id));

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
//--------------------------------------------------------------------------------------------------
