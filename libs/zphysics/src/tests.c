#include <JoltC.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

//#define PRINT_OUTPUT
#define NUM_LAYERS 2
#define LAYER_NON_MOVING 0
#define LAYER_MOVING 1
#define BP_LAYER_NON_MOVING 0
#define BP_LAYER_MOVING 1

typedef struct BPLayerInterfaceImpl BPLayerInterfaceImpl;
typedef struct MyContactListener MyContactListener;
typedef struct MyActivationListener MyActivationListener;
//--------------------------------------------------------------------------------------------------
// BPLayerInterface
//--------------------------------------------------------------------------------------------------
struct BPLayerInterfaceImpl
{
    const JPH_BroadPhaseLayerInterfaceVTable *vtable;
    JPH_BroadPhaseLayer                       object_to_broad_phase[NUM_LAYERS];
};

static uint32_t
BPLayerInterface_GetNumBroadPhaseLayers(const void *in_self)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "BPLayerInterface_GetNumBroadPhaseLayers()\n");
#endif
    return NUM_LAYERS;
}

static JPH_BroadPhaseLayer
BPLayerInterface_GetBroadPhaseLayer(const void *in_self, JPH_ObjectLayer in_layer)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "BPLayerInterface_GetBroadPhaseLayer()\n");
#endif
    assert(in_layer < NUM_LAYERS);
    const BPLayerInterfaceImpl *self = (BPLayerInterfaceImpl *)in_self;
    return self->object_to_broad_phase[in_layer];
}

static const JPH_BroadPhaseLayerInterfaceVTable g_bp_layer_interface_vtable =
{
    .GetNumBroadPhaseLayers = BPLayerInterface_GetNumBroadPhaseLayers,
    .GetBroadPhaseLayer     = BPLayerInterface_GetBroadPhaseLayer,
};

static BPLayerInterfaceImpl
BPLayerInterface_Init(void)
{
    BPLayerInterfaceImpl impl =
    {
        .vtable = &g_bp_layer_interface_vtable,
    };
    impl.object_to_broad_phase[LAYER_NON_MOVING] = BP_LAYER_NON_MOVING;
    impl.object_to_broad_phase[LAYER_MOVING]     = BP_LAYER_MOVING;

    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyContactListener
//--------------------------------------------------------------------------------------------------
struct MyContactListener
{
    const JPH_ContactListenerVTable *vtable;
};

static JPH_ValidateResult
MyContactListener_OnContactValidate(void *in_self,
                                    const JPH_Body *in_body1,
                                    const JPH_Body *in_body2,
                                    const JPH_CollideShapeResult *in_collision_result)
{
    const JPH_BodyID body1_id = JPH_Body_GetID(in_body1);
    const JPH_BodyID body2_id = JPH_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactValidate(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(body1_id), JPH_BodyID_GetIndex(body1_id),
            JPH_BodyID_GetSequenceNumber(body2_id), JPH_BodyID_GetIndex(body2_id));
#endif

    return JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS;
}

static void
MyContactListener_OnContactAdded(void *in_self,
                                 const JPH_Body *in_body1,
                                 const JPH_Body *in_body2,
                                 const JPH_ContactManifold *in_manifold,
                                 JPH_ContactSettings *io_settings)
{
    const JPH_BodyID body1_id = JPH_Body_GetID(in_body1);
    const JPH_BodyID body2_id = JPH_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactAdded(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(body1_id), JPH_BodyID_GetIndex(body1_id),
            JPH_BodyID_GetSequenceNumber(body2_id), JPH_BodyID_GetIndex(body2_id));
#endif
}

static void
MyContactListener_OnContactPersisted(void *in_self,
                                     const JPH_Body *in_body1,
                                     const JPH_Body *in_body2,
                                     const JPH_ContactManifold *in_manifold,
                                     JPH_ContactSettings *io_settings)
{
    const JPH_BodyID body1_id = JPH_Body_GetID(in_body1);
    const JPH_BodyID body2_id = JPH_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactPersisted(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(body1_id), JPH_BodyID_GetIndex(body1_id),
            JPH_BodyID_GetSequenceNumber(body2_id), JPH_BodyID_GetIndex(body2_id));
#endif
}

static void
MyContactListener_OnContactRemoved(void *in_self, const JPH_SubShapeIDPair *in_sub_shape_pair)
{
    const JPH_BodyID body1_id = in_sub_shape_pair->body1_id;
    const JPH_BodyID body2_id = in_sub_shape_pair->body2_id;
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactRemoved(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(body1_id), JPH_BodyID_GetIndex(body1_id),
            JPH_BodyID_GetSequenceNumber(body2_id), JPH_BodyID_GetIndex(body2_id));
#endif
}

static const JPH_ContactListenerVTable g_contact_listener_vtable =
{
    .OnContactValidate  = MyContactListener_OnContactValidate,
    .OnContactAdded     = MyContactListener_OnContactAdded,
    .OnContactPersisted = MyContactListener_OnContactPersisted,
    .OnContactRemoved   = MyContactListener_OnContactRemoved,
};

static MyContactListener
MyContactListener_Init(void)
{
    MyContactListener impl =
    {
        .vtable = &g_contact_listener_vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyActivationListener
//--------------------------------------------------------------------------------------------------
struct MyActivationListener
{
    const JPH_BodyActivationListenerVTable *vtable;
};

static void
MyActivationListener_OnBodyActivated(void *in_self, const JPH_BodyID *in_body_id, uint64_t in_user_data)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnBodyActivated(): BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(*in_body_id),
            JPH_BodyID_GetIndex(*in_body_id));
#endif
}

static void
MyActivationListener_OnBodyDeactivated(void *in_self, const JPH_BodyID *in_body_id, uint64_t in_user_data)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnBodyDeactivated(): BodyID is (%d, %d)\n",
            JPH_BodyID_GetSequenceNumber(*in_body_id),
            JPH_BodyID_GetIndex(*in_body_id));
#endif
}

static const JPH_BodyActivationListenerVTable g_activation_listener_vtable =
{
    .OnBodyActivated   = MyActivationListener_OnBodyActivated,
    .OnBodyDeactivated = MyActivationListener_OnBodyDeactivated,
};

static MyActivationListener
MyActivationListener_Init(void)
{
    MyActivationListener impl =
    {
        .vtable = &g_activation_listener_vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
static bool
MyObjectCanCollide(JPH_ObjectLayer in_object1, JPH_ObjectLayer in_object2)
{
    switch (in_object1)
    {
        case LAYER_NON_MOVING:
            return in_object2 == LAYER_MOVING;
        case LAYER_MOVING:
            return true;
        default:
            assert(false);
            return false;
    }
}
//--------------------------------------------------------------------------------------------------
static bool
MyBroadPhaseCanCollide(JPH_ObjectLayer in_layer1, JPH_BroadPhaseLayer in_layer2)
{
    switch (in_layer1)
    {
        case LAYER_NON_MOVING:
            return in_layer2 == BP_LAYER_MOVING;
        case LAYER_MOVING:
            return true;
        default:
            assert(false);
            return false;
    }
}
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_Basic1(void)
{
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
        MyBroadPhaseCanCollide,
        MyObjectCanCollide);

    const float half_extent[3] = { 10.0, 20.0, 30.0 };
    JPH_BoxShapeSettings *box_settings = JPH_BoxShapeSettings_Create(half_extent);

    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)box_settings) != 1) return 0;
    JPH_ShapeSettings_AddRef((JPH_ShapeSettings *)box_settings);
    JPH_ShapeSettings_Release((JPH_ShapeSettings *)box_settings);
    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)box_settings) != 1) return 0;

    JPH_BoxShapeSettings_SetConvexRadius(box_settings, 1.0);
    if (JPH_BoxShapeSettings_GetConvexRadius(box_settings) != 1.0) return 0;

    JPH_ConvexShapeSettings_SetDensity((JPH_ConvexShapeSettings *)box_settings, 100.0);
    if (JPH_ConvexShapeSettings_GetDensity((JPH_ConvexShapeSettings *)box_settings) != 100.0) return 0;

    JPH_Shape *box_shape = JPH_ShapeSettings_CreateShape((JPH_ShapeSettings *)box_settings);
    if (box_shape == NULL) return 0;
    if (JPH_Shape_GetType(box_shape) != JPH_SHAPE_TYPE_CONVEX) return 0;
    if (JPH_Shape_GetSubType(box_shape) != JPH_SHAPE_SUB_TYPE_BOX) return 0;

    if (JPH_Shape_GetRefCount(box_shape) != 2) return 0;

    JPH_ShapeSettings_CreateShape((JPH_ShapeSettings *)box_settings);
    if (JPH_Shape_GetRefCount(box_shape) != 3) return 0;
    JPH_Shape_Release(box_shape);
    if (JPH_Shape_GetRefCount(box_shape) != 2) return 0;

    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)box_settings) != 1) return 0;
    JPH_ShapeSettings_Release((JPH_ShapeSettings *)box_settings);
    box_settings = NULL;

    if (JPH_Shape_GetRefCount(box_shape) != 1) return 0;
    JPH_Shape_Release(box_shape);
    box_shape = NULL;

    JPH_PhysicsSystem_Destroy(physics_system);
    physics_system = NULL;

    JPH_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_Basic2(void)
{
    JPH_RegisterDefaultAllocator();
    JPH_CreateFactory();
    JPH_RegisterTypes();
    JPH_PhysicsSystem *physics_system = JPH_PhysicsSystem_Create();

    JPH_TempAllocator *temp_allocator = JPH_TempAllocator_Create(10 * 1024 * 1024);
    JPH_JobSystem *job_system = JPH_JobSystem_Create(JPH_MAX_PHYSICS_JOBS, JPH_MAX_PHYSICS_BARRIERS, -1);

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
        MyBroadPhaseCanCollide,
        MyObjectCanCollide);

    MyActivationListener body_activation_listener = MyActivationListener_Init();
    JPH_PhysicsSystem_SetBodyActivationListener(physics_system, &body_activation_listener);

    MyContactListener contact_listener = MyContactListener_Init();
    JPH_PhysicsSystem_SetContactListener(physics_system, &contact_listener);

    const float floor_half_extent[3] = { 100.0, 1.0, 100.0 };
    JPH_BoxShapeSettings *floor_shape_settings = JPH_BoxShapeSettings_Create(floor_half_extent);
    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)floor_shape_settings) != 1) return 0;

    JPH_Shape *floor_shape = JPH_ShapeSettings_CreateShape((JPH_ShapeSettings *)floor_shape_settings);
    if (floor_shape == NULL) return 0;
    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)floor_shape_settings) != 1) return 0;
    if (JPH_Shape_GetRefCount(floor_shape) != 2) return 0;

    const float floor_position[3] = { 0.0f, -1.0f, 0.0f };
    const float floor_rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const JPH_BodyCreationSettings floor_settings = JPH_BodyCreationSettings_Init(
        floor_shape,
        floor_position,
        floor_rotation,
        JPH_MOTION_TYPE_STATIC,
        LAYER_NON_MOVING);

    JPH_BodyInterface *body_interface = JPH_PhysicsSystem_GetBodyInterface(physics_system);

    JPH_Body *floor = JPH_BodyInterface_CreateBody(body_interface, &floor_settings);
    if (floor == NULL) return 0;
    const JPH_BodyID floor_id = JPH_Body_GetID(floor);
    if (((floor_id & 0xff000000) >> 24) != 1) return 0;
    if ((floor_id & 0x00ffffff) != 0) return 0;
    if (JPH_Body_IsStatic(floor) == false) return 0;
    if (JPH_Body_IsDynamic(floor) == true) return 0;

    if (JPH_Shape_GetRefCount(floor_shape) != 3) return 0;

    JPH_Body *floor1 = JPH_BodyInterface_CreateBody(body_interface, &floor_settings);
    if (floor1 == NULL) return 0;
    const JPH_BodyID floor1_id = JPH_Body_GetID(floor1);
    if (((floor1_id & 0xff000000) >> 24) != 1) return 0;
    if ((floor1_id & 0x00ffffff) != 1) return 0;

    if (JPH_BodyInterface_IsAdded(body_interface, floor_id) != false) return 0;
    if (JPH_BodyInterface_IsAdded(body_interface, floor1_id) != false) return 0;

    JPH_BodyInterface_AddBody(body_interface, floor_id, JPH_ACTIVATION_ACTIVATE);
    if (JPH_BodyInterface_IsAdded(body_interface, floor_id) != true) return 0;

    JPH_BodyInterface_RemoveBody(body_interface, floor_id);
    if (JPH_BodyInterface_IsAdded(body_interface, floor_id) != false) return 0;

    if (JPH_Shape_GetRefCount(floor_shape) != 4) return 0;

    if (JPH_ShapeSettings_GetRefCount((JPH_ShapeSettings *)floor_shape_settings) != 1) return 0;
    JPH_ShapeSettings_Release((JPH_ShapeSettings *)floor_shape_settings);
    if (JPH_Shape_GetRefCount(floor_shape) != 3) return 0;

    JPH_BodyInterface_DestroyBody(body_interface, floor_id);
    if (JPH_Shape_GetRefCount(floor_shape) != 2) return 0;

    JPH_BodyInterface_DestroyBody(body_interface, floor1_id);
    if (JPH_Shape_GetRefCount(floor_shape) != 1) return 0;

    JPH_Shape_Release(floor_shape);

    JPH_PhysicsSystem_Destroy(physics_system);
    JPH_JobSystem_Destroy(job_system);
    JPH_TempAllocator_Destroy(temp_allocator);

    JPH_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_HelloWorld(void)
{
    JPH_RegisterDefaultAllocator();
    JPH_CreateFactory();
    JPH_RegisterTypes();
    JPH_PhysicsSystem *physics_system = JPH_PhysicsSystem_Create();

    JPH_TempAllocator *temp_allocator = JPH_TempAllocator_Create(10 * 1024 * 1024);
    JPH_JobSystem *job_system = JPH_JobSystem_Create(JPH_MAX_PHYSICS_JOBS, JPH_MAX_PHYSICS_BARRIERS, -1);

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
        MyBroadPhaseCanCollide,
        MyObjectCanCollide);

    MyActivationListener body_activation_listener = MyActivationListener_Init();
    JPH_PhysicsSystem_SetBodyActivationListener(physics_system, &body_activation_listener);

    MyContactListener contact_listener = MyContactListener_Init();
    JPH_PhysicsSystem_SetContactListener(physics_system, &contact_listener);

    JPH_BodyInterface *body_interface = JPH_PhysicsSystem_GetBodyInterface(physics_system);

    //
    // Static floor
    //
    const float floor_half_extent[3] = { 100.0, 1.0, 100.0 };
    JPH_BoxShapeSettings *floor_shape_settings = JPH_BoxShapeSettings_Create(floor_half_extent);
    JPH_Shape *floor_shape = JPH_ShapeSettings_CreateShape((JPH_ShapeSettings *)floor_shape_settings);

    const float identity_rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const float floor_position[3] = { 0.0f, -1.0f, 0.0f };
    const JPH_BodyCreationSettings floor_settings = JPH_BodyCreationSettings_Init(
        floor_shape,
        floor_position,
        identity_rotation,
        JPH_MOTION_TYPE_STATIC,
        LAYER_NON_MOVING);

    JPH_Body *floor = JPH_BodyInterface_CreateBody(body_interface, &floor_settings);
    const JPH_BodyID floor_id = JPH_Body_GetID(floor);
    JPH_BodyInterface_AddBody(body_interface, floor_id, JPH_ACTIVATION_DONT_ACTIVATE);

    //
    // Falling sphere
    //
    JPH_SphereShapeSettings *sphere_shape_settings = JPH_SphereShapeSettings_Create(0.5f);
    JPH_Shape *sphere_shape = JPH_ShapeSettings_CreateShape((JPH_ShapeSettings *)sphere_shape_settings);

    const float sphere_position[3] = { 0.0f, 2.0f, 0.0f };
    const JPH_BodyCreationSettings sphere_settings = JPH_BodyCreationSettings_Init(
        sphere_shape,
        sphere_position,
        identity_rotation,
        JPH_MOTION_TYPE_DYNAMIC,
        LAYER_MOVING);

    const JPH_BodyID sphere_id = JPH_BodyInterface_CreateAndAddBody(
        body_interface,
        &sphere_settings,
        JPH_ACTIVATION_ACTIVATE);


    //if (JPH_Body_IsStatic(floor) == false) return 0;
    //if (JPH_Body_IsDynamic(floor) == true) return 0;

    const float sphere_velocity[3] = { 0.0f, -5.0f, 0.0f };
    JPH_BodyInterface_SetLinearVelocity(body_interface, sphere_id, sphere_velocity);


    JPH_PhysicsSystem_OptimizeBroadPhase(physics_system);

    uint32_t step = 0;
    while (JPH_BodyInterface_IsActive(body_interface, sphere_id))
    {
        step += 1;

        float position[3];
        JPH_BodyInterface_GetCenterOfMassPosition(body_interface, sphere_id, &position[0]);

        float velocity[3];
        JPH_BodyInterface_GetLinearVelocity(body_interface, sphere_id, &velocity[0]);

#ifdef PRINT_OUTPUT
        fprintf(stderr, "Step %d\n\tPosition = (%f, %f, %f), Velocity(%f, %f, %f)\n",
                step,
                position[0], position[1], position[2],
                velocity[0], velocity[1], velocity[2]);
#endif

        const float delta_time = 1.0f / 60.0f;
        const int collision_steps = 1;
        const int integration_sub_steps = 1;

        JPH_PhysicsSystem_Update(
            physics_system,
            delta_time,
            collision_steps,
            integration_sub_steps,
            temp_allocator,
            job_system);
    }

    JPH_BodyInterface_RemoveBody(body_interface, sphere_id);
    JPH_BodyInterface_DestroyBody(body_interface, sphere_id);

    JPH_BodyInterface_RemoveBody(body_interface, floor_id);
    JPH_BodyInterface_DestroyBody(body_interface, floor_id);

    JPH_ShapeSettings_Release((JPH_ShapeSettings *)floor_shape_settings);
    if (JPH_Shape_GetRefCount(floor_shape) != 1) return 0;
    JPH_Shape_Release(floor_shape);

    JPH_ShapeSettings_Release((JPH_ShapeSettings *)sphere_shape_settings);
    if (JPH_Shape_GetRefCount(sphere_shape) != 1) return 0;
    JPH_Shape_Release(sphere_shape);

    JPH_PhysicsSystem_Destroy(physics_system);
    JPH_JobSystem_Destroy(job_system);
    JPH_TempAllocator_Destroy(temp_allocator);

    JPH_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
