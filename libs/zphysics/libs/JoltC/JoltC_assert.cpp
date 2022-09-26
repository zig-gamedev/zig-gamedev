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
    offsetof(JPH::MotionProperties, mSleepTestTimer) ==
    offsetof(JPH_MotionProperties, sleep_test_timer));

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

static_assert(
    offsetof(JPH::Sphere, mRadius) ==
    offsetof(JPH_Sphere, radius));

static_assert(
    offsetof(JPH::AABox, mMax) ==
    offsetof(JPH_AABox, max));
