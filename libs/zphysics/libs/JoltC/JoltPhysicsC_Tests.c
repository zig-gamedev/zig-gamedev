#include "JoltPhysicsC.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

//#define PRINT_OUTPUT

// Object layers
#define NUM_OBJ_LAYERS 2
#define OBJ_LAYER_NON_MOVING 0
#define OBJ_LAYER_MOVING 1

// Broad phase layers
#define NUM_BP_LAYERS 2
#define BP_LAYER_NON_MOVING 0
#define BP_LAYER_MOVING 1
//--------------------------------------------------------------------------------------------------
// BPLayerInterface
//--------------------------------------------------------------------------------------------------
typedef struct BPLayerInterfaceImpl
{
    const JPC_BroadPhaseLayerInterfaceVTable *vtable; // VTable has to be the first field in the struct.
    JPC_BroadPhaseLayer                       object_to_broad_phase[NUM_OBJ_LAYERS];
} BPLayerInterfaceImpl;

static uint32_t
BPLayerInterface_GetNumBroadPhaseLayers(const void *in_self)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "BPLayerInterface_GetNumBroadPhaseLayers()\n");
#endif
    return NUM_BP_LAYERS;
}

static JPC_BroadPhaseLayer
BPLayerInterface_GetBroadPhaseLayer(const void *in_self, JPC_ObjectLayer in_layer)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "BPLayerInterface_GetBroadPhaseLayer()\n");
#endif
    assert(in_layer < NUM_BP_LAYERS);
    const BPLayerInterfaceImpl *self = (BPLayerInterfaceImpl *)in_self;
    return self->object_to_broad_phase[in_layer];
}

static BPLayerInterfaceImpl
BPLayerInterface_Init(void)
{
    static const JPC_BroadPhaseLayerInterfaceVTable vtable =
    {
        .GetNumBroadPhaseLayers = BPLayerInterface_GetNumBroadPhaseLayers,
        .GetBroadPhaseLayer     = BPLayerInterface_GetBroadPhaseLayer,
    };
    BPLayerInterfaceImpl impl =
    {
        .vtable = &vtable,
    };
    impl.object_to_broad_phase[OBJ_LAYER_NON_MOVING] = BP_LAYER_NON_MOVING;
    impl.object_to_broad_phase[OBJ_LAYER_MOVING]     = BP_LAYER_MOVING;

    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyContactListener
//--------------------------------------------------------------------------------------------------
typedef struct MyContactListener
{
    const JPC_ContactListenerVTable *vtable; // VTable has to be the first field in the struct.
} MyContactListener;

static JPC_ValidateResult
MyContactListener_OnContactValidate(void *in_self,
                                    const JPC_Body *in_body1,
                                    const JPC_Body *in_body2,
                                    const JPC_Real in_base_offset[3],
                                    const JPC_CollideShapeResult *in_collision_result)
{
    const JPC_BodyID body1_id = JPC_Body_GetID(in_body1);
    const JPC_BodyID body2_id = JPC_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactValidate(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(body1_id), JPC_BodyID_GetIndex(body1_id),
            JPC_BodyID_GetSequenceNumber(body2_id), JPC_BodyID_GetIndex(body2_id));
    fprintf(stderr,
            "\tOnContactValidate(): in_base_offset (%f, %f, %f)\n",
            in_base_offset[0], in_base_offset[1], in_base_offset[2]);
    fprintf(stderr, "\tOnContactValidate(): penetration_depth (%f)\n", in_collision_result->penetration_depth);
    fprintf(stderr, "\tOnContactValidate(): shape1_sub_shape_id (%d)\n", in_collision_result->shape1_sub_shape_id);
    fprintf(stderr, "\tOnContactValidate(): shape2_sub_shape_id (%d)\n", in_collision_result->shape2_sub_shape_id);
    fprintf(stderr, "\tOnContactValidate(): body2_id (%d)\n", in_collision_result->body2_id);
#endif
    return JPC_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS;
}

static void
MyContactListener_OnContactAdded(void *in_self,
                                 const JPC_Body *in_body1,
                                 const JPC_Body *in_body2,
                                 const JPC_ContactManifold *in_manifold,
                                 JPC_ContactSettings *io_settings)
{
    const JPC_BodyID body1_id = JPC_Body_GetID(in_body1);
    const JPC_BodyID body2_id = JPC_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactAdded(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(body1_id), JPC_BodyID_GetIndex(body1_id),
            JPC_BodyID_GetSequenceNumber(body2_id), JPC_BodyID_GetIndex(body2_id));
#endif
}

static void
MyContactListener_OnContactPersisted(void *in_self,
                                     const JPC_Body *in_body1,
                                     const JPC_Body *in_body2,
                                     const JPC_ContactManifold *in_manifold,
                                     JPC_ContactSettings *io_settings)
{
    const JPC_BodyID body1_id = JPC_Body_GetID(in_body1);
    const JPC_BodyID body2_id = JPC_Body_GetID(in_body2);
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactPersisted(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(body1_id), JPC_BodyID_GetIndex(body1_id),
            JPC_BodyID_GetSequenceNumber(body2_id), JPC_BodyID_GetIndex(body2_id));
#endif
}

static void
MyContactListener_OnContactRemoved(void *in_self, const JPC_SubShapeIDPair *in_sub_shape_pair)
{
    const JPC_BodyID body1_id = in_sub_shape_pair->first.body_id;
    const JPC_BodyID body2_id = in_sub_shape_pair->second.body_id;
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnContactRemoved(): First BodyID is (%d, %d), second BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(body1_id), JPC_BodyID_GetIndex(body1_id),
            JPC_BodyID_GetSequenceNumber(body2_id), JPC_BodyID_GetIndex(body2_id));
#endif
}

static MyContactListener
MyContactListener_Init(void)
{
    static const JPC_ContactListenerVTable vtable =
    {
        .OnContactValidate  = MyContactListener_OnContactValidate,
        .OnContactAdded     = MyContactListener_OnContactAdded,
        .OnContactPersisted = MyContactListener_OnContactPersisted,
        .OnContactRemoved   = MyContactListener_OnContactRemoved,
    };
    MyContactListener impl =
    {
        .vtable = &vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyActivationListener
//--------------------------------------------------------------------------------------------------
typedef struct MyActivationListener
{
    const JPC_BodyActivationListenerVTable *vtable; // VTable has to be the first field in the struct.
} MyActivationListener;

static void
MyActivationListener_OnBodyActivated(void *in_self, const JPC_BodyID *in_body_id, uint64_t in_user_data)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnBodyActivated(): BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(*in_body_id),
            JPC_BodyID_GetIndex(*in_body_id));
#endif
}

static void
MyActivationListener_OnBodyDeactivated(void *in_self, const JPC_BodyID *in_body_id, uint64_t in_user_data)
{
#ifdef PRINT_OUTPUT
    fprintf(stderr, "\tOnBodyDeactivated(): BodyID is (%d, %d)\n",
            JPC_BodyID_GetSequenceNumber(*in_body_id),
            JPC_BodyID_GetIndex(*in_body_id));
#endif
}

static MyActivationListener
MyActivationListener_Init(void)
{
    static const JPC_BodyActivationListenerVTable vtable =
    {
        .OnBodyActivated   = MyActivationListener_OnBodyActivated,
        .OnBodyDeactivated = MyActivationListener_OnBodyDeactivated,
    };
    MyActivationListener impl =
    {
        .vtable = &vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyObjectFilter
//--------------------------------------------------------------------------------------------------
typedef struct MyObjectFilter
{
    const JPC_ObjectLayerPairFilterVTable *vtable; // VTable has to be the first field in the struct.
} MyObjectFilter;

static bool
MyObjectFilter_ShouldCollide(const void *in_self, JPC_ObjectLayer in_object1, JPC_ObjectLayer in_object2)
{
    switch (in_object1)
    {
        case OBJ_LAYER_NON_MOVING:
            return in_object2 == OBJ_LAYER_MOVING;
        case OBJ_LAYER_MOVING:
            return true;
        default:
            assert(false);
            return false;
    }
}

static MyObjectFilter
MyObjectFilter_Init(void)
{
    static const JPC_ObjectLayerPairFilterVTable vtable =
    {
        .ShouldCollide = MyObjectFilter_ShouldCollide,
    };
    MyObjectFilter impl =
    {
        .vtable = &vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
// MyBroadPhaseFilter
//--------------------------------------------------------------------------------------------------
typedef struct MyBroadPhaseFilter
{
    const JPC_ObjectVsBroadPhaseLayerFilterVTable *vtable; // VTable has to be the first field in the struct.
} MyBroadPhaseFilter;

static bool
MyBroadPhaseFilter_ShouldCollide(const void *in_self, JPC_ObjectLayer in_layer1, JPC_BroadPhaseLayer in_layer2)
{
    switch (in_layer1)
    {
        case OBJ_LAYER_NON_MOVING:
            return in_layer2 == BP_LAYER_MOVING;
        case OBJ_LAYER_MOVING:
            return true;
        default:
            assert(false);
            return false;
    }
}

static MyBroadPhaseFilter
MyBroadPhaseFilter_Init(void)
{
    static const JPC_ObjectVsBroadPhaseLayerFilterVTable vtable =
    {
        .ShouldCollide = MyBroadPhaseFilter_ShouldCollide,
    };
    MyBroadPhaseFilter impl =
    {
        .vtable = &vtable,
    };
    return impl;
}
//--------------------------------------------------------------------------------------------------
// Basic1
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_Basic1(void)
{
    JPC_RegisterDefaultAllocator();
    JPC_CreateFactory();
    JPC_RegisterTypes();

    const uint32_t max_bodies = 1024;
    const uint32_t num_body_mutexes = 0;
    const uint32_t max_body_pairs = 1024;
    const uint32_t max_contact_constraints = 1024;

    BPLayerInterfaceImpl *broad_phase_layer_interface = malloc(sizeof(BPLayerInterfaceImpl));
    *broad_phase_layer_interface = BPLayerInterface_Init();

    MyBroadPhaseFilter *broad_phase_filter = malloc(sizeof(MyBroadPhaseFilter));
    *broad_phase_filter = MyBroadPhaseFilter_Init();

    MyObjectFilter *object_filter = malloc(sizeof(MyObjectFilter));
    *object_filter = MyObjectFilter_Init();

    JPC_PhysicsSystem *physics_system = JPC_PhysicsSystem_Create(
        max_bodies,
        num_body_mutexes,
        max_body_pairs,
        max_contact_constraints,
        broad_phase_layer_interface,
        broad_phase_filter,
        object_filter);

    JPC_BoxShapeSettings *box_settings = JPC_BoxShapeSettings_Create((float[]){ 10.0, 20.0, 30.0 });

    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)box_settings) != 1) return 0;
    JPC_ShapeSettings_AddRef((JPC_ShapeSettings *)box_settings);
    JPC_ShapeSettings_Release((JPC_ShapeSettings *)box_settings);
    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)box_settings) != 1) return 0;

    JPC_BoxShapeSettings_SetConvexRadius(box_settings, 1.0);
    if (JPC_BoxShapeSettings_GetConvexRadius(box_settings) != 1.0) return 0;

    JPC_ConvexShapeSettings_SetDensity((JPC_ConvexShapeSettings *)box_settings, 100.0);
    if (JPC_ConvexShapeSettings_GetDensity((JPC_ConvexShapeSettings *)box_settings) != 100.0) return 0;

    JPC_Shape *box_shape = JPC_ShapeSettings_CreateShape((JPC_ShapeSettings *)box_settings);
    if (box_shape == NULL) return 0;
    if (JPC_Shape_GetType(box_shape) != JPC_SHAPE_TYPE_CONVEX) return 0;
    if (JPC_Shape_GetSubType(box_shape) != JPC_SHAPE_SUB_TYPE_BOX) return 0;

    if (JPC_Shape_GetRefCount(box_shape) != 2) return 0;

    JPC_ShapeSettings_CreateShape((JPC_ShapeSettings *)box_settings);
    if (JPC_Shape_GetRefCount(box_shape) != 3) return 0;
    JPC_Shape_Release(box_shape);
    if (JPC_Shape_GetRefCount(box_shape) != 2) return 0;

    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)box_settings) != 1) return 0;
    JPC_ShapeSettings_Release((JPC_ShapeSettings *)box_settings);
    box_settings = NULL;

    if (JPC_Shape_GetRefCount(box_shape) != 1) return 0;
    JPC_Shape_Release(box_shape);
    box_shape = NULL;

    JPC_PhysicsSystem_Destroy(physics_system);
    physics_system = NULL;

    JPC_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
// Basic2
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_Basic2(void)
{
    JPC_RegisterDefaultAllocator();
    JPC_CreateFactory();
    JPC_RegisterTypes();

    JPC_TempAllocator *temp_allocator = JPC_TempAllocator_Create(10 * 1024 * 1024);
    JPC_JobSystem *job_system = JPC_JobSystem_Create(JPC_MAX_PHYSICS_JOBS, JPC_MAX_PHYSICS_BARRIERS, -1);

    const uint32_t max_bodies = 1024;
    const uint32_t num_body_mutexes = 0;
    const uint32_t max_body_pairs = 1024;
    const uint32_t max_contact_constraints = 1024;

    BPLayerInterfaceImpl broad_phase_layer_interface = BPLayerInterface_Init();
    MyBroadPhaseFilter broad_phase_filter = MyBroadPhaseFilter_Init();
    MyObjectFilter object_filter = MyObjectFilter_Init();

    JPC_PhysicsSystem *physics_system = JPC_PhysicsSystem_Create(
        max_bodies,
        num_body_mutexes,
        max_body_pairs,
        max_contact_constraints,
        &broad_phase_layer_interface,
        &broad_phase_filter,
        &object_filter);

    MyActivationListener body_activation_listener = MyActivationListener_Init();
    JPC_PhysicsSystem_SetBodyActivationListener(physics_system, &body_activation_listener);

    MyContactListener contact_listener = MyContactListener_Init();
    JPC_PhysicsSystem_SetContactListener(physics_system, &contact_listener);
    JPC_PhysicsSystem_SetContactListener(physics_system, NULL);
    JPC_PhysicsSystem_SetContactListener(physics_system, &contact_listener);

    JPC_BoxShapeSettings *floor_shape_settings = JPC_BoxShapeSettings_Create((float[]){ 100.0f, 1.0f, 100.0f });
    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)floor_shape_settings) != 1) return 0;

    JPC_Shape *floor_shape = JPC_ShapeSettings_CreateShape((JPC_ShapeSettings *)floor_shape_settings);
    if (floor_shape == NULL) return 0;
    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)floor_shape_settings) != 1) return 0;
    if (JPC_Shape_GetRefCount(floor_shape) != 2) return 0;

    JPC_BodyCreationSettings floor_settings;
    JPC_BodyCreationSettings_Set(
        &floor_settings,
        floor_shape,
        (JPC_Real[]){ 0.0f, -1.0f, 0.0f },
        (float[]){ 0.0f, 0.0f, 0.0f, 1.0f },
        JPC_MOTION_TYPE_STATIC,
        OBJ_LAYER_NON_MOVING);

    JPC_BodyInterface *body_interface = JPC_PhysicsSystem_GetBodyInterface(physics_system);

    JPC_Body *floor = JPC_BodyInterface_CreateBody(body_interface, &floor_settings);
    if (floor == NULL) return 0;
    const JPC_BodyID floor_id = JPC_Body_GetID(floor);
    if (((floor_id & JPC_BODY_ID_SEQUENCE_BITS) >> JPC_BODY_ID_SEQUENCE_SHIFT) != 1) return 0;
    if ((floor_id & JPC_BODY_ID_INDEX_BITS) != 0) return 0;
    if (JPC_Body_IsStatic(floor) == false) return 0;
    if (JPC_Body_IsDynamic(floor) == true) return 0;

    if (JPC_Shape_GetRefCount(floor_shape) != 3) return 0;

    JPC_Body *floor1 = JPC_BodyInterface_CreateBody(body_interface, &floor_settings);
    if (floor1 == NULL) return 0;
    const JPC_BodyID floor1_id = JPC_Body_GetID(floor1);
    if (((floor1_id & JPC_BODY_ID_SEQUENCE_BITS) >> JPC_BODY_ID_SEQUENCE_SHIFT) != 1) return 0;
    if ((floor1_id & JPC_BODY_ID_INDEX_BITS) != 1) return 0;

    if (JPC_BodyInterface_IsAdded(body_interface, floor_id) != false) return 0;
    if (JPC_BodyInterface_IsAdded(body_interface, floor1_id) != false) return 0;

    JPC_BodyInterface_AddBody(body_interface, floor_id, JPC_ACTIVATION_ACTIVATE);
    if (JPC_BodyInterface_IsAdded(body_interface, floor_id) != true) return 0;

    JPC_PhysicsSystem_OptimizeBroadPhase(physics_system);
    JPC_PhysicsSystem_Update(physics_system, 1.0f / 60.0f, 1, 1, temp_allocator, job_system);

    JPC_BodyInterface_RemoveBody(body_interface, floor_id);
    if (JPC_BodyInterface_IsAdded(body_interface, floor_id) != false) return 0;

    if (JPC_Shape_GetRefCount(floor_shape) != 4) return 0;

    if (JPC_ShapeSettings_GetRefCount((JPC_ShapeSettings *)floor_shape_settings) != 1) return 0;
    JPC_ShapeSettings_Release((JPC_ShapeSettings *)floor_shape_settings);
    if (JPC_Shape_GetRefCount(floor_shape) != 3) return 0;

    JPC_BodyInterface_DestroyBody(body_interface, floor_id);
    if (JPC_Shape_GetRefCount(floor_shape) != 2) return 0;

    JPC_BodyInterface_DestroyBody(body_interface, floor1_id);
    if (JPC_Shape_GetRefCount(floor_shape) != 1) return 0;

    JPC_Shape_Release(floor_shape);

    JPC_PhysicsSystem_Destroy(physics_system);
    JPC_JobSystem_Destroy(job_system);
    JPC_TempAllocator_Destroy(temp_allocator);

    JPC_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
// HelloWorld
//--------------------------------------------------------------------------------------------------
uint32_t
JoltCTest_HelloWorld(void)
{
    JPC_RegisterDefaultAllocator();
    JPC_CreateFactory();
    JPC_RegisterTypes();

    JPC_TempAllocator *temp_allocator = JPC_TempAllocator_Create(10 * 1024 * 1024);
    JPC_JobSystem *job_system = JPC_JobSystem_Create(JPC_MAX_PHYSICS_JOBS, JPC_MAX_PHYSICS_BARRIERS, -1);

    const uint32_t max_bodies = 1024;
    const uint32_t num_body_mutexes = 0;
    const uint32_t max_body_pairs = 1024;
    const uint32_t max_contact_constraints = 1024;

    BPLayerInterfaceImpl broad_phase_layer_interface = BPLayerInterface_Init();
    MyBroadPhaseFilter broad_phase_filter = MyBroadPhaseFilter_Init();
    MyObjectFilter object_filter = MyObjectFilter_Init();

    JPC_PhysicsSystem *physics_system = JPC_PhysicsSystem_Create(
        max_bodies,
        num_body_mutexes,
        max_body_pairs,
        max_contact_constraints,
        &broad_phase_layer_interface,
        &broad_phase_filter,
        &object_filter);

    MyActivationListener body_activation_listener = MyActivationListener_Init();
    JPC_PhysicsSystem_SetBodyActivationListener(physics_system, &body_activation_listener);

    MyContactListener contact_listener = MyContactListener_Init();
    JPC_PhysicsSystem_SetContactListener(physics_system, &contact_listener);

    JPC_BodyInterface *body_interface = JPC_PhysicsSystem_GetBodyInterface(physics_system);

    //
    // Static floor
    //
    JPC_BoxShapeSettings *floor_shape_settings = JPC_BoxShapeSettings_Create((float[]){ 100.0, 1.0, 100.0 });
    JPC_Shape *floor_shape = JPC_ShapeSettings_CreateShape((JPC_ShapeSettings *)floor_shape_settings);

    JPC_BodyCreationSettings floor_settings;
    JPC_BodyCreationSettings_Set(
        &floor_settings,
        floor_shape,
        (JPC_Real[]){ 0.0f, -1.0f, 0.0f },
        (float[]){ 0.0f, 0.0f, 0.0f, 1.0f },
        JPC_MOTION_TYPE_STATIC,
        OBJ_LAYER_NON_MOVING);

    JPC_Body *floor = JPC_BodyInterface_CreateBody(body_interface, &floor_settings);
    const JPC_BodyID floor_id = JPC_Body_GetID(floor);
    JPC_BodyInterface_AddBody(body_interface, floor_id, JPC_ACTIVATION_DONT_ACTIVATE);

    //
    // Falling sphere
    //
    JPC_SphereShapeSettings *sphere_shape_settings = JPC_SphereShapeSettings_Create(0.5f);
    JPC_Shape *sphere_shape = JPC_ShapeSettings_CreateShape((JPC_ShapeSettings *)sphere_shape_settings);

    JPC_BodyCreationSettings sphere_settings;
    JPC_BodyCreationSettings_Set(
        &sphere_settings,
        sphere_shape,
        (JPC_Real[]){ 0.0f, 2.0f, 0.0f },
        (float[]){ 0.0f, 0.0f, 0.0f, 1.0f },
        JPC_MOTION_TYPE_DYNAMIC,
        OBJ_LAYER_MOVING);

    const JPC_BodyID sphere_id = JPC_BodyInterface_CreateAndAddBody(
        body_interface,
        &sphere_settings,
        JPC_ACTIVATION_ACTIVATE);

    if (JPC_Body_IsStatic(floor) == false) return 0;
    if (JPC_Body_IsDynamic(floor) == true) return 0;

    JPC_BodyInterface_SetLinearVelocity(body_interface, sphere_id, (float[]){ 0.0f, -5.0f, 0.0f });

    JPC_PhysicsSystem_OptimizeBroadPhase(physics_system);

    // Test JPC_PhysicsSystem_GetBodyIDs()
    {
        JPC_BodyID body_ids[2];
        uint32_t num_body_ids = 0;
        JPC_PhysicsSystem_GetBodyIDs(physics_system, 2, &num_body_ids, &body_ids[0]);
        if (num_body_ids != 2) return 0;
        if (body_ids[0] != floor_id) return 0;
        if (body_ids[1] != sphere_id) return 0;
    }

    // Test JPC_PhysicsSystem_GetActiveBodyIDs()
    {
        JPC_BodyID body_ids[2];
        uint32_t num_body_ids = 0;
        JPC_PhysicsSystem_GetActiveBodyIDs(physics_system, 2, &num_body_ids, &body_ids[0]);
        if (num_body_ids != 1) return 0;
        if (body_ids[0] != sphere_id) return 0;
    }

#ifdef PRINT_OUTPUT
    if (sizeof(JPC_Real) == 8)
        fprintf(stderr, "Using double precision computation...\n");
#endif

    uint32_t step = 0;
    while (JPC_BodyInterface_IsActive(body_interface, sphere_id))
    {
        step += 1;

        JPC_Real position[3];
        JPC_BodyInterface_GetCenterOfMassPosition(body_interface, sphere_id, &position[0]);

        float velocity[3];
        JPC_BodyInterface_GetLinearVelocity(body_interface, sphere_id, &velocity[0]);

        const float delta_time = 1.0f / 60.0f;
        const int collision_steps = 1;
        const int integration_sub_steps = 1;

        JPC_PhysicsSystem_Update(
            physics_system,
            delta_time,
            collision_steps,
            integration_sub_steps,
            temp_allocator,
            job_system);

#ifdef PRINT_OUTPUT
        fprintf(stderr, "Step %d\n\tPosition = (%f, %f, %f), Velocity(%f, %f, %f)\n",
                step,
                position[0], position[1], position[2],
                velocity[0], velocity[1], velocity[2]);
#endif

        // Safe, lock protected way of accessing all bodies (use when you interact with Jolt from multiple threads).
        {
            JPC_BodyID body_ids[16]; // You can use JPC_PhysicsSystem_GetMaxBodies() to pre-allocate storage
            uint32_t num_body_ids = 0;
            JPC_PhysicsSystem_GetBodyIDs(physics_system, 16, &num_body_ids, &body_ids[0]);

            const JPC_BodyLockInterface *lock_iface = JPC_PhysicsSystem_GetBodyLockInterface(physics_system);
            //const JPC_BodyLockInterface *lock_iface = JPC_PhysicsSystem_GetBodyLockInterfaceNoLock(physics_system);

            for (uint32_t i = 0; i < num_body_ids; ++i)
            {
                JPC_BodyLockRead lock;
                JPC_BodyLockInterface_LockRead(lock_iface, body_ids[i], &lock);
                //JPC_BodyLockWrite lock;
                //JPC_BodyLockInterface_LockWrite(lock_iface, body_ids[i], &lock);
                if (lock.body)
                {
                    // Body has been locked, you can safely use `JPC_Body_*()` functions.
                }
                JPC_BodyLockInterface_UnlockRead(lock_iface, &lock);
                //JPC_BodyLockInterface_UnlockWrite(lock_iface, &lock);
            }
        }

        // Low-level, advanced way of accessing body data. Not protected by a lock, no function calls overhead.
        // Use when you interact with Jolt only from one thread or when you are sure that JPC_PhysicsSystem_Update()
        // has already completed in a given frame.
        {
            JPC_Body **bodies = JPC_PhysicsSystem_GetBodiesUnsafe(physics_system);

            // Access a single body (get the body pointer from a body id).
            {
                JPC_Body *sphere = JPC_TRY_GET_BODY(bodies, sphere_id);
                if (sphere)
                {
                    sphere->friction = 0.2f;
                }
            }

            // Access all body pointers.
            for (uint32_t i = 0; i < JPC_PhysicsSystem_GetNumBodies(physics_system); ++i)
            {
                JPC_Body *body = bodies[i];
                if (JPC_IS_VALID_BODY_POINTER(body))
                {
                    // Body pointer is valid (not freed) you can access the data.
                }
            }
        }

        // Test body access.
        {
            JPC_Body **bodies = JPC_PhysicsSystem_GetBodiesUnsafe(physics_system);

            const JPC_BodyLockInterface *lock_iface = JPC_PhysicsSystem_GetBodyLockInterface(physics_system);

            JPC_BodyLockRead lock;
            JPC_BodyLockInterface_LockRead(lock_iface, sphere_id, &lock);
            if (lock.body)
            {
                JPC_Body *body = bodies[sphere_id & JPC_BODY_ID_INDEX_BITS];
                if (!JPC_IS_VALID_BODY_POINTER(body)) return 0;

                if (JPC_Body_IsDynamic(body) != true) return 0;

                JPC_Body *body_checked = JPC_TRY_GET_BODY(bodies, sphere_id);
                if (body_checked == NULL) return 0;

                if (body_checked != body) return 0;
                if (body_checked->id != body->id) return 0;

                if (body != lock.body) return 0;
                if (body->id != sphere_id) return 0;
            }
            JPC_BodyLockInterface_UnlockRead(lock_iface, &lock);
        }
    }

    JPC_BodyInterface_RemoveBody(body_interface, sphere_id);
    JPC_BodyInterface_DestroyBody(body_interface, sphere_id);

    JPC_BodyInterface_RemoveBody(body_interface, floor_id);
    JPC_BodyInterface_DestroyBody(body_interface, floor_id);

    JPC_ShapeSettings_Release((JPC_ShapeSettings *)floor_shape_settings);
    if (JPC_Shape_GetRefCount(floor_shape) != 1) return 0;
    JPC_Shape_Release(floor_shape);

    JPC_ShapeSettings_Release((JPC_ShapeSettings *)sphere_shape_settings);
    if (JPC_Shape_GetRefCount(sphere_shape) != 1) return 0;
    JPC_Shape_Release(sphere_shape);

    JPC_PhysicsSystem_Destroy(physics_system);
    JPC_JobSystem_Destroy(job_system);
    JPC_TempAllocator_Destroy(temp_allocator);

    JPC_DestroyFactory();

    return 1;
}
//--------------------------------------------------------------------------------------------------
