#include "JoltC.h"

#include <assert.h>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>

JPH_SUPPRESS_WARNINGS

#ifdef JPH_ENABLE_ASSERTS

static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine) {
	return true;
}

#endif

static_assert(sizeof(uint8) == 1, "sizeof(uint8) != 1");
static_assert(sizeof(uint16) == 2, "sizeof(uint16) != 2");
static_assert(sizeof(uint) == 4, "sizeof(uint) != 4");
static_assert(sizeof(uint64) == 8, "sizeof(uint64) != 8");

void JPH_RegisterDefaultAllocator(void) {
    JPH::RegisterDefaultAllocator();
}

void JPH_CreateFactory(void) {
    assert(JPH::Factory::sInstance == nullptr);
    JPH::Factory::sInstance = new JPH::Factory();
}

void JPH_DestroyFactory(void) {
    assert(JPH::Factory::sInstance != nullptr);
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void JPH_RegisterTypes(void) {
    JPH::RegisterTypes();
}

//
// PhysicsSystem
//
JPH_PhysicsSystem* JPH_PhysicsSystem_Create(void) {
    return reinterpret_cast<JPH_PhysicsSystem*>(new JPH::PhysicsSystem());
}

void JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem *inPhysicsSystem) {
    assert(inPhysicsSystem != nullptr);
    delete reinterpret_cast<JPH::PhysicsSystem *>(inPhysicsSystem);
}

void JPH_PhysicsSystem_Init(
    JPH_PhysicsSystem *inPhysicsSystem,
    uint inMaxBodies,
    uint inNumBodyMutexes,
    uint inMaxBodyPairs,
    uint inMaxContactConstraints,
    const void *inBroadPhaseLayerInterface,
    JPH_ObjectVsBroadPhaseLayerFilter inObjectVsBroadPhaseLayerFilter,
    JPH_ObjectLayerPairFilter inObjectLayerPairFilter
) {
    assert(inPhysicsSystem != nullptr);
    assert(inBroadPhaseLayerInterface != nullptr);
    assert(inObjectVsBroadPhaseLayerFilter != nullptr);
    assert(inObjectLayerPairFilter != nullptr);
    reinterpret_cast<JPH::PhysicsSystem *>(inPhysicsSystem)->Init(
        inMaxBodies,
        inNumBodyMutexes,
        inMaxBodyPairs,
        inMaxContactConstraints,
        *static_cast<const JPH::BroadPhaseLayerInterface *>(inBroadPhaseLayerInterface),
        reinterpret_cast<JPH::ObjectVsBroadPhaseLayerFilter>(inObjectVsBroadPhaseLayerFilter),
        reinterpret_cast<JPH::ObjectLayerPairFilter>(inObjectLayerPairFilter)
    );
}

uint JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *inPhysicsSystem) {
    assert(inPhysicsSystem != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(inPhysicsSystem)->GetNumBodies();
}

uint JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *inPhysicsSystem) {
    assert(inPhysicsSystem != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(inPhysicsSystem)->GetNumActiveBodies();
}

uint JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *inPhysicsSystem) {
    assert(inPhysicsSystem != nullptr);
    return reinterpret_cast<const JPH::PhysicsSystem *>(inPhysicsSystem)->GetMaxBodies();
}

//
// Shape
//
JPH_ShapeType JPH_Shape_GetType(const JPH_Shape *inShape) {
    assert(inShape != nullptr);
    return static_cast<JPH_ShapeType>(reinterpret_cast<const JPH::Shape *>(inShape)->GetType());
}

JPH_ShapeSubType JPH_Shape_GetSubType(const JPH_Shape *inShape) {
    assert(inShape != nullptr);
    return static_cast<JPH_ShapeSubType>(reinterpret_cast<const JPH::Shape *>(inShape)->GetSubType());
}

uint64 JPH_Shape_GetUserData(const JPH_Shape *inShape) {
    assert(inShape != nullptr);
    return reinterpret_cast<const JPH::Shape *>(inShape)->GetUserData();
}

void JPH_Shape_SetUserData(JPH_Shape *inShape, uint64 inUserData) {
    assert(inShape != nullptr);
    return reinterpret_cast<JPH::Shape *>(inShape)->SetUserData(inUserData);
}
