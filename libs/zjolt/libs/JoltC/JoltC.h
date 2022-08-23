#pragma once

#include <stdbool.h>
#include <stdint.h>

#define JPH_CAPI

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t JPH_ObjectLayer;
typedef uint8_t JPH_BroadPhaseLayer;
typedef uint32_t JPH_BodyID;
typedef uint8_t JPH_ShapeType;
typedef uint8_t JPH_ShapeSubType;

typedef struct JPH_PhysicsSystem JPH_PhysicsSystem;
typedef struct JPH_Shape JPH_Shape;
typedef struct JPH_PhysicsMaterial JPH_PhysicsMaterial;

typedef struct JPH_ShapeSettings JPH_ShapeSettings;
typedef struct JPH_ConvexShapeSettings JPH_ConvexShapeSettings;
typedef struct JPH_BoxShapeSettings JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings JPH_SphereShapeSettings;

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

//
// JPH_ShapeSettings
//
JPH_CAPI JPH_Shape *
JPH_ShapeSettings_Cook(const JPH_ShapeSettings *inSettings);

JPH_CAPI uint64_t
JPH_ShapeSettings_GetUserData(const JPH_ShapeSettings *inSettings);

JPH_CAPI void
JPH_ShapeSettings_SetUserData(JPH_ShapeSettings *inSettings, uint64_t inUserData);

//
// JPH_ConvexShapeSettings (-> JPH_ShapeSettings)
//
JPH_CAPI const JPH_PhysicsMaterial *
JPH_ConvexShapeSettings_GetMaterial(const JPH_ConvexShapeSettings *inSettings);

JPH_CAPI void
JPH_ConvexShapeSettings_SetMaterial(
    JPH_ConvexShapeSettings *inSettings,
    const JPH_PhysicsMaterial *inMaterial
);

JPH_CAPI float
JPH_ConvexShapeSettings_GetDensity(const JPH_ConvexShapeSettings *inSettings);

JPH_CAPI void
JPH_ConvexShapeSettings_SetDensity( JPH_ConvexShapeSettings *inSettings, float inDensity);

//
// JPH_BoxShapeSettings (-> JPH_ConvexShapeSettings -> JPH_ShapeSettings)
//
JPH_CAPI JPH_BoxShapeSettings *
JPH_BoxShapeSettings_Create(const float inHalfExtent[3]);

JPH_CAPI void
JPH_BoxShapeSettings_GetHalfExtent(const JPH_BoxShapeSettings *inSettings, float outHalfExtent[3]);

JPH_CAPI void
JPH_BoxShapeSettings_SetHalfExtent(JPH_BoxShapeSettings *inSettings, const float inHalfExtent[3]);

JPH_CAPI float
JPH_BoxShapeSettings_GetConvexRadius(const JPH_BoxShapeSettings *inSettings);

JPH_CAPI void
JPH_BoxShapeSettings_SetConvexRadius(JPH_BoxShapeSettings *inSettings, float inConvexRadius);

//
// Misc
//
JPH_CAPI void
JPH_RegisterDefaultAllocator(void);

JPH_CAPI void
JPH_CreateFactory(void);

JPH_CAPI void
JPH_DestroyFactory(void);

JPH_CAPI void
JPH_RegisterTypes(void);

typedef bool (*JPH_ObjectLayerPairFilter)(JPH_ObjectLayer inLayer1, JPH_ObjectLayer inLayer2);
typedef bool (*JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer inLayer1, JPH_BroadPhaseLayer inLayer2);

typedef struct JPH_BroadPhaseLayerInterfaceVTable {
    void *reserved0;
    void *reserved1;
    uint32_t (*GetNumBroadPhaseLayers)(const void *inThis);
    JPH_BroadPhaseLayer (*GetBroadPhaseLayer)(const void *inThis, JPH_ObjectLayer inLayer);
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char *(*GetBroadPhaseLayerName)(const void *inThis, JPH_BroadPhaseLayer inLayer);
#endif
} JPH_BroadPhaseLayerInterfaceVTable;

//
// PhysicsSystem
//
JPH_CAPI JPH_PhysicsSystem *
JPH_PhysicsSystem_Create(void);

JPH_CAPI void
JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem *inPhysicsSystem);

JPH_CAPI void
JPH_PhysicsSystem_Init(
    JPH_PhysicsSystem *inPhysicsSystem,
    uint32_t inMaxBodies,
    uint32_t inNumBodyMutexes,
    uint32_t inMaxBodyPairs,
    uint32_t inMaxContactConstraints,
    const void *inBroadPhaseLayerInterface,
    JPH_ObjectVsBroadPhaseLayerFilter inObjectVsBroadPhaseLayerFilter,
    JPH_ObjectLayerPairFilter inObjectLayerPairFilter
);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem *inPhysicsSystem);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem *inPhysicsSystem);

JPH_CAPI uint32_t
JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem *inPhysicsSystem);

//
// Shape
//
JPH_CAPI JPH_ShapeType
JPH_Shape_GetType(const JPH_Shape *inShape);

JPH_CAPI JPH_ShapeSubType
JPH_Shape_GetSubType(const JPH_Shape *inShape);

JPH_CAPI uint64_t
JPH_Shape_GetUserData(const JPH_Shape *inShape);

JPH_CAPI void
JPH_Shape_SetUserData(JPH_Shape *inShape, uint64_t inUserData);

#ifdef __cplusplus
}
#endif
