#include <JoltC.h>
#include <assert.h>
#include <stddef.h>

static const JPH_ObjectLayer layer_non_moving = 0;
static const JPH_ObjectLayer layer_moving = 1;

static const JPH_BroadPhaseLayer bp_layer_non_moving = 0;
static const JPH_BroadPhaseLayer bp_layer_moving = 1;

#define NUM_LAYERS 2

typedef struct BPLayerInterfaceImpl {
    const JPH_BroadPhaseLayerInterfaceVTable *vtable_ptr;
    JPH_BroadPhaseLayer object_to_broad_phase[NUM_LAYERS];
} BPLayerInterfaceImpl;

static uint32_t BPLayerInterface_GetNumBroadPhaseLayers(const void *in_self) {
    return NUM_LAYERS;
}

static JPH_BroadPhaseLayer BPLayerInterface_GetBroadPhaseLayer(const void *in_self, JPH_ObjectLayer layer) {
    assert(layer < NUM_LAYERS);
    const BPLayerInterfaceImpl *self = (BPLayerInterfaceImpl *)in_self;
    return self->object_to_broad_phase[layer];
}

static const JPH_BroadPhaseLayerInterfaceVTable bp_layer_interface_vtable = {
    .GetNumBroadPhaseLayers = BPLayerInterface_GetNumBroadPhaseLayers,
    .GetBroadPhaseLayer = BPLayerInterface_GetBroadPhaseLayer,
};

static BPLayerInterfaceImpl BPLayerInterface_Init(void) {
    BPLayerInterfaceImpl impl = {
        .vtable_ptr = &bp_layer_interface_vtable,
    };
    impl.object_to_broad_phase[layer_non_moving] = bp_layer_non_moving;
    impl.object_to_broad_phase[layer_moving] = bp_layer_moving;
    return impl;
}

static bool myObjectCanCollide(JPH_ObjectLayer in_object1, JPH_ObjectLayer in_object2) {
    switch (in_object1) {
        case layer_non_moving:
            return in_object2 == layer_moving;
        case layer_moving:
            return true;
        default:
            assert(false);
            return false;
    }
}

static bool myBroadPhaseCanCollide(JPH_ObjectLayer in_layer1, JPH_BroadPhaseLayer in_layer2) {
    switch (in_layer1) {
        case layer_non_moving:
            return in_layer2 == bp_layer_moving;
        case layer_moving:
            return true;	
        default:
            assert(false);
            return false;
    }
}

static bool testBasic(void) {
    JPH_RegisterDefaultAllocator();
    JPH_CreateFactory();
    JPH_RegisterTypes();
    JPH_PhysicsSystem *physics_system = JPH_PhysicsSystem_Create();

    const uint32_t max_bodies = 1024;
    const uint32_t num_body_mutexes = 0;
    const uint32_t max_body_pairs = 1024;
    const uint32_t max_contact_constraints = 1024;

    BPLayerInterfaceImpl broad_phase_layer_interface = BPLayerInterface_Init();

    JPH_PhysicsSystem_Init(
        physics_system,
        max_bodies,
        num_body_mutexes,
        max_body_pairs,
        max_contact_constraints,
        &broad_phase_layer_interface,
        myBroadPhaseCanCollide,
        myObjectCanCollide
    );

    const float half_extent[3] = { 10.0, 20.0, 30.0 };
    JPH_BoxShapeSettings *box_settings = JPH_BoxShapeSettings_Create(half_extent);
    JPH_BoxShapeSettings_SetConvexRadius(box_settings, 1.0);
    if (JPH_BoxShapeSettings_GetConvexRadius(box_settings) != 1.0) return false;

    JPH_ConvexShapeSettings_SetDensity((JPH_ConvexShapeSettings *)box_settings, 100.0);
    if (JPH_ConvexShapeSettings_GetDensity((JPH_ConvexShapeSettings *)box_settings) != 100.0) return false;

    JPH_Shape *box_shape = JPH_ShapeSettings_Cook((JPH_ShapeSettings *)box_settings);
    if (box_shape == NULL) return false;
    if (JPH_Shape_GetType(box_shape) != JPH_SHAPE_TYPE_CONVEX) return false;
    if (JPH_Shape_GetSubType(box_shape) != JPH_SHAPE_SUB_TYPE_BOX) return false;

    JPH_ShapeSettings_Destroy((JPH_ShapeSettings *)box_settings);
    box_settings = NULL;

    JPH_PhysicsSystem_Destroy(physics_system);
    JPH_DestroyFactory();

    return true;
}

bool joltcRunAllCTests(void) {
    if (!testBasic()) return false;
    return true;
}
