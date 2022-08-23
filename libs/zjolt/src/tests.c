#include <JoltC.h>
#include <assert.h>
#include <stddef.h>

#define NUM_LAYERS 2

static const JPH_ObjectLayer gLayerNonMoving = 0;
static const JPH_ObjectLayer gLayerMoving = 1;

static const JPH_BroadPhaseLayer gBpLayerNonMoving = 0;
static const JPH_BroadPhaseLayer gBpLayerMoving = 1;

typedef struct BPLayerInterfaceImpl
{
    const JPH_BroadPhaseLayerInterfaceVTable *  vtable;
    JPH_BroadPhaseLayer                         objectToBroadPhase[NUM_LAYERS];
} BPLayerInterfaceImpl;

static uint32_t BPLayerInterface_GetNumBroadPhaseLayers(const void *inSelf)
{
    return NUM_LAYERS;
}

static JPH_BroadPhaseLayer BPLayerInterface_GetBroadPhaseLayer(const void *inSelf, JPH_ObjectLayer inLayer)
{
    assert(inLayer < NUM_LAYERS);
    const BPLayerInterfaceImpl *self = (BPLayerInterfaceImpl *)inSelf;
    return self->objectToBroadPhase[inLayer];
}

static const JPH_BroadPhaseLayerInterfaceVTable gBpLayerInterfaceVTable =
{
    .GetNumBroadPhaseLayers = BPLayerInterface_GetNumBroadPhaseLayers,
    .GetBroadPhaseLayer     = BPLayerInterface_GetBroadPhaseLayer,
};

static BPLayerInterfaceImpl BPLayerInterface_Init(void)
{
    BPLayerInterfaceImpl impl =
    {
        .vtable = &gBpLayerInterfaceVTable,
    };
    impl.objectToBroadPhase[gLayerNonMoving] = gBpLayerNonMoving;
    impl.objectToBroadPhase[gLayerMoving]    = gBpLayerMoving;

    return impl;
}

static bool MyObjectCanCollide(JPH_ObjectLayer inObject1, JPH_ObjectLayer inObject2)
{
    switch (inObject1)
    {
        case gLayerNonMoving:
            return inObject2 == gLayerMoving;
        case gLayerMoving:
            return true;
        default:
            assert(false);
            return false;
    }
}

static bool MyBroadPhaseCanCollide(JPH_ObjectLayer inLayer1, JPH_BroadPhaseLayer inLayer2)
{
    switch (inLayer1)
    {
        case gLayerNonMoving:
            return inLayer2 == gBpLayerMoving;
        case gLayerMoving:
            return true;
        default:
            assert(false);
            return false;
    }
}

static bool TestBasic(void)
{
    JPH_RegisterDefaultAllocator();
    JPH_CreateFactory();
    JPH_RegisterTypes();
    JPH_PhysicsSystem *physicsSystem = JPH_PhysicsSystem_Create();

    const uint32_t maxBodies = 1024;
    const uint32_t numBodyMutexes = 0;
    const uint32_t maxBodyPairs = 1024;
    const uint32_t maxContactConstraints = 1024;

    BPLayerInterfaceImpl broadPhaseLayerInterface = BPLayerInterface_Init();

    JPH_PhysicsSystem_Init(
        physicsSystem,
        maxBodies,
        numBodyMutexes,
        maxBodyPairs,
        maxContactConstraints,
        &broadPhaseLayerInterface,
        MyBroadPhaseCanCollide,
        MyObjectCanCollide
    );

    const float halfExtent[3] = { 10.0, 20.0, 30.0 };
    JPH_BoxShapeSettings *boxSettings = JPH_BoxShapeSettings_Create(halfExtent);
    JPH_BoxShapeSettings_SetConvexRadius(boxSettings, 1.0);
    if (JPH_BoxShapeSettings_GetConvexRadius(boxSettings) != 1.0) return false;

    JPH_ConvexShapeSettings_SetDensity((JPH_ConvexShapeSettings *)boxSettings, 100.0);
    if (JPH_ConvexShapeSettings_GetDensity((JPH_ConvexShapeSettings *)boxSettings) != 100.0) return false;

    JPH_Shape *boxShape = JPH_ShapeSettings_Cook((JPH_ShapeSettings *)boxSettings);
    if (boxShape == NULL) return false;
    if (JPH_Shape_GetType(boxShape) != JPH_SHAPE_TYPE_CONVEX) return false;
    if (JPH_Shape_GetSubType(boxShape) != JPH_SHAPE_SUB_TYPE_BOX) return false;

    JPH_ShapeSettings_Destroy((JPH_ShapeSettings *)boxSettings);
    boxSettings = NULL;

    JPH_PhysicsSystem_Destroy(physicsSystem);
    physicsSystem = NULL;

    JPH_DestroyFactory();

    return true;
}

bool joltcRunAllCTests(void)
{
    if (!TestBasic()) return false;
    return true;
}
