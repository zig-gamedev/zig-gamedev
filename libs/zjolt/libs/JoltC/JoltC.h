#pragma once

#include <stdbool.h>

#define JPH_CAPI

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;
typedef unsigned long long uint64;

typedef unsigned short JPH_ObjectLayer;
typedef unsigned char JPH_BroadPhaseLayer;
typedef unsigned int JPH_BodyID;
typedef unsigned char JPH_ShapeType;
typedef unsigned char JPH_ShapeSubType;

typedef struct JPH_PhysicsSystem JPH_PhysicsSystem;
typedef struct JPH_Shape JPH_Shape;
typedef struct JPH_PhysicsMaterial JPH_PhysicsMaterial;

typedef struct JPH_ShapeSettings JPH_ShapeSettings;
typedef struct JPH_ConvexShapeSettings JPH_ConvexShapeSettings;
typedef struct JPH_BoxShapeSettings JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings JPH_SphereShapeSettings;

JPH_CAPI uint64 JPH_ShapeSettings_GetUserData(const JPH_ShapeSettings *inShapeSattings);
JPH_CAPI void JPH_ShapeSettings_SetUserData(JPH_ShapeSettings *inShapeSattings, uint64 inUserData);

JPH_CAPI JPH_BoxShapeSettings * JPH_BoxShapeSettings_Create(const float *inHalfExtent);

#define JPH_SHAPE_TYPE_CONVEX 0
#define JPH_SHAPE_TYPE_COMPOUND 1
#define JPH_SHAPE_TYPE_DECORATED 2
#define JPH_SHAPE_TYPE_MESH 3
#define JPH_SHAPE_TYPE_HEIGHT_FIELD 4
#define JPH_SHAPE_TYPE_USER1 5
#define JPH_SHAPE_TYPE_USER2 6
#define JPH_SHAPE_TYPE_USER3 7
#define JPH_SHAPE_TYPE_USER4 8

#define JPH_SHAPE_SUB_TYPE_SPHERE 0
#define JPH_SHAPE_SUB_TYPE_BOX 1
#define JPH_SHAPE_SUB_TYPE_TRIANGLE 2
#define JPH_SHAPE_SUB_TYPE_CAPSULE 3
#define JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE 4
#define JPH_SHAPE_SUB_TYPE_CYLINDER 5
#define JPH_SHAPE_SUB_TYPE_CONVEX_HULL 6

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

//
// Shape
//
JPH_CAPI JPH_ShapeType JPH_Shape_GetType(const JPH_Shape *inShape);
JPH_CAPI JPH_ShapeSubType JPH_Shape_GetSubType(const JPH_Shape *inShape);
JPH_CAPI uint64 JPH_Shape_GetUserData(const JPH_Shape *inShape);
JPH_CAPI void JPH_Shape_SetUserData(JPH_Shape *inShape, uint64 inUserData);

#ifdef __cplusplus
}
#endif
