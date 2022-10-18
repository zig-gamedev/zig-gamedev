//--------------------------------------------------------------------------------------------------
#include "JoltC.h"
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
JPH_SUPPRESS_WARNINGS
//--------------------------------------------------------------------------------------------------
static_assert(sizeof(JPH::BodyID)                  == sizeof(JPH_BodyID));
static_assert(sizeof(JPH::SubShapeID)              == sizeof(JPH_SubShapeID));
static_assert(sizeof(JPH::SubShapeIDCreator)       == sizeof(JPH_SubShapeIDCreator));
static_assert(sizeof(JPH::EShapeType)              == sizeof(JPH_ShapeType));
static_assert(sizeof(JPH::EShapeSubType)           == sizeof(JPH_ShapeSubType));
static_assert(sizeof(JPH::EMotionType)             == sizeof(JPH_MotionType));
static_assert(sizeof(JPH::EMotionQuality)          == sizeof(JPH_MotionQuality));
static_assert(sizeof(JPH::EOverrideMassProperties) == sizeof(JPH_OverrideMassProperties));
static_assert(sizeof(JPH::EActivation)             == sizeof(JPH_Activation));
static_assert(sizeof(JPH::ValidateResult)          == sizeof(JPH_ValidateResult));
static_assert(sizeof(JPH::BroadPhaseLayer)         == sizeof(JPH_BroadPhaseLayer));
static_assert(sizeof(JPH::ObjectLayer)             == sizeof(JPH_ObjectLayer));

static_assert(sizeof(JPH::MassProperties)       == sizeof(JPH_MassProperties));
static_assert(sizeof(JPH::MotionProperties)     == sizeof(JPH_MotionProperties));
static_assert(sizeof(JPH::CollisionGroup)       == sizeof(JPH_CollisionGroup));
static_assert(sizeof(JPH::BodyCreationSettings) == sizeof(JPH_BodyCreationSettings));
static_assert(sizeof(JPH::ContactManifold)      == sizeof(JPH_ContactManifold));
static_assert(sizeof(JPH::ContactSettings)      == sizeof(JPH_ContactSettings));
static_assert(sizeof(JPH::SubShapeIDPair)       == sizeof(JPH_SubShapeIDPair));
static_assert(sizeof(JPH::CollideShapeResult)   == sizeof(JPH_CollideShapeResult));
static_assert(sizeof(JPH::TransformedShape)     == sizeof(JPH_TransformedShape));
//--------------------------------------------------------------------------------------------------
static_assert(alignof(JPH::MassProperties)       == alignof(JPH_MassProperties));
static_assert(alignof(JPH::MotionProperties)     == alignof(JPH_MotionProperties));
static_assert(alignof(JPH::CollisionGroup)       == alignof(JPH_CollisionGroup));
static_assert(alignof(JPH::BodyCreationSettings) == alignof(JPH_BodyCreationSettings));
static_assert(alignof(JPH::ContactManifold)      == alignof(JPH_ContactManifold));
static_assert(alignof(JPH::ContactSettings)      == alignof(JPH_ContactSettings));
static_assert(alignof(JPH::SubShapeIDPair)       == alignof(JPH_SubShapeIDPair));
static_assert(alignof(JPH::CollideShapeResult)   == alignof(JPH_CollideShapeResult));
static_assert(alignof(JPH::TransformedShape)     == alignof(JPH_TransformedShape));
//--------------------------------------------------------------------------------------------------
#define ENSURE_ENUM_EQ(c_const, cpp_enum) static_assert(c_const == static_cast<int>(cpp_enum))

ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_CONVEX,       JPH::EShapeType::Convex);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_COMPOUND,     JPH::EShapeType::Compound);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_DECORATED,    JPH::EShapeType::Decorated);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_MESH,         JPH::EShapeType::Mesh);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_HEIGHT_FIELD, JPH::EShapeType::HeightField);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_USER1,        JPH::EShapeType::User1);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_USER2,        JPH::EShapeType::User2);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_USER3,        JPH::EShapeType::User3);
ENSURE_ENUM_EQ(JPH_SHAPE_TYPE_USER4,        JPH::EShapeType::User4);

ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_SPHERE,                JPH::EShapeSubType::Sphere);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_BOX,                   JPH::EShapeSubType::Box);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_TRIANGLE,              JPH::EShapeSubType::Triangle);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_CAPSULE,               JPH::EShapeSubType::Capsule);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE,       JPH::EShapeSubType::TaperedCapsule);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_CYLINDER,              JPH::EShapeSubType::Cylinder);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_CONVEX_HULL,           JPH::EShapeSubType::ConvexHull);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_STATIC_COMPOUND,       JPH::EShapeSubType::StaticCompound);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_MUTABLE_COMPOUND,      JPH::EShapeSubType::MutableCompound);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_ROTATED_TRANSLATED,    JPH::EShapeSubType::RotatedTranslated);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_SCALED,                JPH::EShapeSubType::Scaled);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS, JPH::EShapeSubType::OffsetCenterOfMass);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_MESH,                  JPH::EShapeSubType::Mesh);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_HEIGHT_FIELD,          JPH::EShapeSubType::HeightField);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER1,                 JPH::EShapeSubType::User1);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER2,                 JPH::EShapeSubType::User2);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER3,                 JPH::EShapeSubType::User3);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER4,                 JPH::EShapeSubType::User4);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER5,                 JPH::EShapeSubType::User5);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER6,                 JPH::EShapeSubType::User6);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER7,                 JPH::EShapeSubType::User7);
ENSURE_ENUM_EQ(JPH_SHAPE_SUB_TYPE_USER8,                 JPH::EShapeSubType::User8);

ENSURE_ENUM_EQ(JPH_MOTION_TYPE_STATIC,    JPH::EMotionType::Static);
ENSURE_ENUM_EQ(JPH_MOTION_TYPE_KINEMATIC, JPH::EMotionType::Kinematic);
ENSURE_ENUM_EQ(JPH_MOTION_TYPE_DYNAMIC,   JPH::EMotionType::Dynamic);

ENSURE_ENUM_EQ(JPH_MOTION_QUALITY_DISCRETE,    JPH::EMotionQuality::Discrete);
ENSURE_ENUM_EQ(JPH_MOTION_QUALITY_LINEAR_CAST, JPH::EMotionQuality::LinearCast);

ENSURE_ENUM_EQ(JPH_ACTIVATION_ACTIVATE,      JPH::EActivation::Activate);
ENSURE_ENUM_EQ(JPH_ACTIVATION_DONT_ACTIVATE, JPH::EActivation::DontActivate);

ENSURE_ENUM_EQ(JPH_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA,
               JPH::EOverrideMassProperties::CalculateMassAndInertia);
ENSURE_ENUM_EQ(JPH_OVERRIDE_MASS_PROPS_CALC_INERTIA,
               JPH::EOverrideMassProperties::CalculateInertia);
ENSURE_ENUM_EQ(JPH_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED,
               JPH::EOverrideMassProperties::MassAndInertiaProvided);

ENSURE_ENUM_EQ(JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS,
               JPH::ValidateResult::AcceptAllContactsForThisBodyPair);
ENSURE_ENUM_EQ(JPH_VALIDATE_RESULT_ACCEPT_CONTACT,
               JPH::ValidateResult::AcceptContact);
ENSURE_ENUM_EQ(JPH_VALIDATE_RESULT_REJECT_CONTACT,
               JPH::ValidateResult::RejectContact);
ENSURE_ENUM_EQ(JPH_VALIDATE_RESULT_REJECT_ALL_CONTACTS,
               JPH::ValidateResult::RejectAllContactsForThisBodyPair);

ENSURE_ENUM_EQ(JPH_MAX_PHYSICS_JOBS,     JPH::cMaxPhysicsJobs);
ENSURE_ENUM_EQ(JPH_MAX_PHYSICS_BARRIERS, JPH::cMaxPhysicsBarriers);
//--------------------------------------------------------------------------------------------------
static_assert(
    offsetof(JPH::BodyCreationSettings, mInertiaMultiplier) ==
    offsetof(JPH_BodyCreationSettings, inertia_multiplier));
static_assert(
    offsetof(JPH::BodyCreationSettings, mIsSensor) ==
    offsetof(JPH_BodyCreationSettings, is_sensor));
static_assert(
    offsetof(JPH::BodyCreationSettings, mAngularDamping) ==
    offsetof(JPH_BodyCreationSettings, angular_damping));

static_assert(
    offsetof(JPH::ContactManifold, mPenetrationDepth) ==
    offsetof(JPH_ContactManifold, penetration_depth));
static_assert(
    offsetof(JPH::ContactManifold, mWorldSpaceContactPointsOn1) ==
    offsetof(JPH_ContactManifold, num_points1));

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) ==
    offsetof(JPH_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) ==
    offsetof(JPH_CollideShapeResult, num_face_points1));

static_assert(
    offsetof(JPH::MotionProperties, mForce) ==
    offsetof(JPH_MotionProperties, force));
static_assert(
    offsetof(JPH::MotionProperties, mTorque) ==
    offsetof(JPH_MotionProperties, torque));
static_assert(
    offsetof(JPH::MotionProperties, mMotionQuality) ==
    offsetof(JPH_MotionProperties, motion_quality));

static_assert(
    offsetof(JPH::MassProperties, mInertia) ==
    offsetof(JPH_MassProperties, inertia));

static_assert(
    offsetof(JPH::CollideShapeResult, mPenetrationDepth) ==
    offsetof(JPH_CollideShapeResult, penetration_depth));
static_assert(
    offsetof(JPH::CollideShapeResult, mBodyID2) ==
    offsetof(JPH_CollideShapeResult, body2_id));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape1Face) ==
    offsetof(JPH_CollideShapeResult, num_face_points1));
static_assert(
    offsetof(JPH::CollideShapeResult, mShape2Face) ==
    offsetof(JPH_CollideShapeResult, num_face_points2));

static_assert(
    offsetof(JPH::SubShapeIDPair, mBody2ID) ==
    offsetof(JPH_SubShapeIDPair, body2_id));

static_assert(
    offsetof(JPH::SubShapeIDCreator, mCurrentBit) ==
    offsetof(JPH_SubShapeIDCreator, current_bit));

static_assert(
    offsetof(JPH::CollisionGroup, mGroupID) ==
    offsetof(JPH_CollisionGroup, group_id));
//--------------------------------------------------------------------------------------------------
