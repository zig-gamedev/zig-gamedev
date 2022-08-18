#pragma once

#include <stdbool.h>

#define JPH_CAPI

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

typedef unsigned short JPH_ObjectLayer;
typedef unsigned char JPH_BroadPhaseLayer;

typedef struct JPH_PhysicsSystem JPH_PhysicsSystem;

JPH_CAPI void JPH_RegisterDefaultAllocator(void);

JPH_CAPI void JPH_CreateFactory(void);
JPH_CAPI void JPH_DestroyFactory(void);

JPH_CAPI void JPH_RegisterTypes(void);

typedef bool (*JPH_ObjectLayerPairFilter)(JPH_ObjectLayer inLayer1, JPH_ObjectLayer inLayer2);

typedef bool (*JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer inLayer1, JPH_BroadPhaseLayer inLayer2);

typedef struct JPH_BroadPhaseLayerInterfaceVTable {
    void *reserved0;
    void *reserved1;
    uint (*GetNumBroadPhaseLayers)(const void *inThis);
    JPH_BroadPhaseLayer (*GetBroadPhaseLayer)(const void *inThis, JPH_ObjectLayer inLayer);
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char * (*GetBroadPhaseLayerName)(const void *inThis, JPH_BroadPhaseLayer inLayer);
#endif
} JPH_BroadPhaseLayerInterfaceVTable;

//
// PhysicsSystem
//
JPH_CAPI JPH_PhysicsSystem * JPH_PhysicsSystem_Create(void);
JPH_CAPI void JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem *inPhysicsSystem);
JPH_CAPI void JPH_PhysicsSystem_Init(
    JPH_PhysicsSystem *inPhysicsSystem,
    uint inMaxBodies,
    uint inNumBodyMutexes,
    uint inMaxBodyPairs,
    uint inMaxContactConstraints,
    const void *inBroadPhaseLayerInterface,
    JPH_ObjectVsBroadPhaseLayerFilter inObjectVsBroadPhaseLayerFilter,
    JPH_ObjectLayerPairFilter inObjectLayerPairFilter
);
JPH_CAPI uint JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *inPhysicsSystem);
JPH_CAPI uint JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *inPhysicsSystem);
JPH_CAPI uint JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *inPhysicsSystem);

#ifdef __cplusplus
}
#endif
