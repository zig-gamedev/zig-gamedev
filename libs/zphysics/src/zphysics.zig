const std = @import("std");
const assert = std.debug.assert;
const c = @cImport(@cInclude("JoltC.h"));

pub const Body = *opaque {};
pub const Material = *opaque {};
pub const GroupFilter = *opaque {};
pub const BroadPhaseLayer = c.JPH_BroadPhaseLayer;
pub const ObjectLayer = c.JPH_ObjectLayer;
pub const BodyId = c.JPH_BodyID;
pub const SubShapeId = c.JPH_SubShapeID;

pub const ObjectVsBroadPhaseLayerFilter = *const fn (ObjectLayer, BroadPhaseLayer) callconv(.C) bool;
pub const ObjectLayerPairFilter = *const fn (ObjectLayer, ObjectLayer) callconv(.C) bool;

pub const max_physics_jobs: u32 = c.JPH_MAX_PHYSICS_JOBS;
pub const max_physics_barriers: u32 = c.JPH_MAX_PHYSICS_BARRIERS;

const TempAllocator = *opaque {};
const JobSystem = *opaque {};

pub const BroadPhaseLayerInterfaceVTable = extern struct {
    reserved0: ?*const anyopaque = null,
    reserved1: ?*const anyopaque = null,

    // Pure virtual
    getNumBroadPhaseLayers: *const fn (self: *const anyopaque) callconv(.C) u32,

    // Pure virtual
    getBroadPhaseLayer: *const fn (self: *const anyopaque, layer: ObjectLayer) callconv(.C) BroadPhaseLayer,

    // TODO: GetBroadPhaseLayerName() if JPH_EXTERNAL_PROFILE or JPH_PROFILE_ENABLED

    comptime {
        assert(@sizeOf(BroadPhaseLayerInterfaceVTable) == @sizeOf(c.JPH_BroadPhaseLayerInterfaceVTable));
    }
};

pub const BodyActivationListenerVTable = extern struct {
    reserved0: ?*const anyopaque = null,
    reserved1: ?*const anyopaque = null,

    // Pure virtual
    onBodyActivated: *const fn (self: *anyopaque, body_id: *const BodyId, user_data: u64) callconv(.C) void,

    // Pure virtual
    onBodyDeactivated: *const fn (self: *anyopaque, body_id: *const BodyId, user_data: u64) callconv(.C) void,

    comptime {
        assert(@sizeOf(BodyActivationListenerVTable) == @sizeOf(c.JPH_BodyActivationListenerVTable));
    }
};

pub const ContactListenerVTable = extern struct {
    reserved0: ?*const anyopaque = null,
    reserved1: ?*const anyopaque = null,

    onContactValidate: *const fn (
        self: *anyopaque,
        body1: Body,
        body2: Body,
        collision_result: *const CollideShapeResult,
    ) callconv(.C) ValidateResult = onContactValidate,

    onContactAdded: *const fn (
        self: *anyopaque,
        body1: Body,
        body2: Body,
        manifold: *const ContactManifold,
        settings: *ContactSettings,
    ) callconv(.C) void = (struct {
        fn defaultImpl(
            _: *anyopaque,
            _: Body,
            _: Body,
            _: *const ContactManifold,
            _: *ContactSettings,
        ) callconv(.C) void {
            // Do nothing
        }
    }).defaultImpl,

    onContactPersisted: *const fn (
        self: *anyopaque,
        body1: Body,
        body2: Body,
        manifold: *const ContactManifold,
        settings: *ContactSettings,
    ) callconv(.C) void = (struct {
        fn defaultImpl(
            _: *anyopaque,
            _: Body,
            _: Body,
            _: *const ContactManifold,
            _: *ContactSettings,
        ) callconv(.C) void {
            // Do nothing
        }
    }).defaultImpl,

    onContactRemoved: *const fn (
        self: *anyopaque,
        sub_shape_pair: *const SubShapeIdPair,
    ) callconv(.C) void = (struct {
        fn defaultImpl(_: *anyopaque, _: *const SubShapeIdPair) callconv(.C) void {
            // Do nothing
        }
    }).defaultImpl,

    pub fn onContactValidate(
        _: *anyopaque,
        _: Body,
        _: Body,
        _: *const CollideShapeResult,
    ) callconv(.C) ValidateResult {
        return .accept_all_contacts;
    }

    comptime {
        assert(@sizeOf(ContactListenerVTable) == @sizeOf(c.JPH_ContactListenerVTable));
    }
};

pub const ContactSettings = extern struct {
    combined_friction: f32,
    combined_restitution: f32,
    is_sensor: bool,

    comptime {
        assert(@sizeOf(ContactSettings) == @sizeOf(c.JPH_ContactSettings));
    }
};

pub const MassProperties = extern struct {
    mass: f32,
    inertia: [16]f32 align(16),

    comptime {
        assert(@sizeOf(MassProperties) == @sizeOf(c.JPH_MassProperties));
    }
};

pub const SubShapeIdPair = extern struct {
    body1_id: BodyId,
    sub_shape1_id: SubShapeId,
    body2_id: BodyId,
    sub_shape2_id: SubShapeId,

    comptime {
        assert(@sizeOf(SubShapeIdPair) == @sizeOf(c.JPH_SubShapeIDPair));
    }
};

pub const CollideShapeResult = extern struct {
    contact_point1: [4]f32 align(16),
    contact_point2: [4]f32 align(16),
    penetration_axis: [4]f32 align(16),
    penetration_depth: f32,
    sub_shape1_id: SubShapeId,
    sub_shape2_id: SubShapeId,
    body2_id: BodyId,
    num_face_points1: u32 align(16),
    shape1_face: [32][4]f32 align(16),
    num_face_points2: u32 align(16),
    shape2_face: [32][4]f32 align(16),

    comptime {
        assert(@sizeOf(CollideShapeResult) == @sizeOf(c.JPH_CollideShapeResult));
    }
};

pub const ContactManifold = extern struct {
    world_space_normal: [4]f32 align(16),
    penetration_depth: f32 align(16),
    sub_shape1_id: SubShapeId,
    sub_shape2_id: SubShapeId,
    num_points1: u32 align(16),
    world_space_contact_points1: [64][4]f32 align(16),
    num_points2: u32 align(16),
    world_space_contact_points2: [64][4]f32 align(16),

    comptime {
        assert(@sizeOf(ContactManifold) == @sizeOf(c.JPH_ContactManifold));
    }
};

pub const CollisionGroup = extern struct {
    filter: ?GroupFilter,
    group_id: GroupId,
    sub_group_id: SubGroupId,

    pub const GroupId = c.JPH_CollisionGroupID;
    pub const SubGroupId = c.JPH_CollisionSubGroupID;

    const invalid_group = ~@as(GroupId, 0);
    const invalid_sub_group = ~@as(SubGroupId, 0);

    pub fn init() CollisionGroup {
        return @ptrCast(*const CollisionGroup, &c.JPH_CollisionGroup_InitDefault()).*;
    }

    comptime {
        assert(@sizeOf(CollisionGroup) == @sizeOf(c.JPH_CollisionGroup));
    }
};

pub const ValidateResult = enum(c.JPH_ValidateResult) {
    accept_all_contacts = c.JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS,
    accept_contact = c.JPH_VALIDATE_RESULT_ACCEPT_CONTACT,
    reject_contact = c.JPH_VALIDATE_RESULT_REJECT_CONTACT,
    reject_all_contacts = c.JPH_VALIDATE_RESULT_REJECT_ALL_CONTACTS,
};

pub const MotionType = enum(c.JPH_MotionType) {
    static = c.JPH_MOTION_TYPE_STATIC,
    kinematic = c.JPH_MOTION_TYPE_KINEMATIC,
    dynamic = c.JPH_MOTION_TYPE_DYNAMIC,
};

pub const MotionQuality = enum(c.JPH_MotionQuality) {
    discrete = c.JPH_MOTION_QUALITY_DISCRETE,
    linear_cast = c.JPH_MOTION_QUALITY_LINEAR_CAST,
};

pub const OverrideMassProperties = enum(c.JPH_OverrideMassProperties) {
    calc_mass_inertia = c.JPH_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA,
    calc_inertia = c.JPH_OVERRIDE_MASS_PROPS_CALC_INERTIA,
    mass_inertia_provided = c.JPH_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED,
};

pub const BodyCreationSettings = extern struct {
    position: [4]f32 align(16),
    rotation: [4]f32 align(16),
    linear_velocity: [4]f32 align(16),
    angular_velocity: [4]f32 align(16),
    user_data: u64,
    object_layer: ObjectLayer,
    collision_group: CollisionGroup,
    motion_type: MotionType,
    allow_dynamic_or_kinematic: bool,
    is_sensor: bool,
    motion_quality: MotionQuality,
    allow_sleeping: bool,
    friction: f32,
    restitution: f32,
    linear_damping: f32,
    angular_damping: f32,
    max_linear_velocity: f32,
    max_angular_velocity: f32,
    gravity_factor: f32,
    override_mass_properties: OverrideMassProperties,
    inertia_multiplier: f32,
    mass_properties_override: MassProperties,
    reserved: ?*const anyopaque,
    shape: ?Shape,

    pub fn init() BodyCreationSettings {
        return @ptrCast(*const BodyCreationSettings, &c.JPH_BodyCreationSettings_InitDefault()).*;
    }

    comptime {
        assert(@sizeOf(BodyCreationSettings) == @sizeOf(c.JPH_BodyCreationSettings));
    }
};
//--------------------------------------------------------------------------------------------------
//
// Init/deinit and global state
//
//--------------------------------------------------------------------------------------------------
var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, usize) = null;
var mem_mutex: std.Thread.Mutex = .{};
const mem_alignment = 16;

var temp_allocator: ?TempAllocator = null;
var job_system: ?JobSystem = null;

pub fn init(allocator: std.mem.Allocator, args: struct {
    temp_allocator_size: u32 = 16 * 1024 * 1024,
    max_jobs: u32 = max_physics_jobs,
    max_barriers: u32 = max_physics_barriers,
    num_threads: i32 = -1,
}) !void {
    std.debug.assert(mem_allocator == null and mem_allocations == null);

    mem_allocator = allocator;
    mem_allocations = std.AutoHashMap(usize, usize).init(allocator);
    mem_allocations.?.ensureTotalCapacity(32) catch unreachable;

    c.JPH_RegisterCustomAllocator(zphysicsAlloc, zphysicsFree, zphysicsAlignedAlloc, zphysicsFree);

    c.JPH_CreateFactory();
    c.JPH_RegisterTypes();

    assert(temp_allocator == null and job_system == null);
    temp_allocator = @ptrCast(TempAllocator, c.JPH_TempAllocator_Create(args.temp_allocator_size).?);
    job_system = @ptrCast(JobSystem, c.JPH_JobSystem_Create(args.max_jobs, args.max_barriers, args.num_threads).?);
}

pub fn deinit() void {
    c.JPH_JobSystem_Destroy(@ptrCast(*c.JPH_JobSystem, job_system.?));
    job_system = null;
    c.JPH_TempAllocator_Destroy(@ptrCast(*c.JPH_TempAllocator, temp_allocator.?));
    temp_allocator = null;
    c.JPH_DestroyFactory();

    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}
//--------------------------------------------------------------------------------------------------
//
// PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
pub fn createPhysicsSystem(
    broad_phase_layer_interface: *const anyopaque,
    object_vs_broad_phase_layer_filter: ObjectVsBroadPhaseLayerFilter,
    object_layer_pair_filter: ObjectLayerPairFilter,
    args: struct {
        max_bodies: u32 = 1024,
        num_body_mutexes: u32 = 0,
        max_body_pairs: u32 = 1024,
        max_contact_constraints: u32 = 1024,
    },
) !PhysicsSystem {
    const physics_system = c.JPH_PhysicsSystem_Create();
    c.JPH_PhysicsSystem_Init(
        physics_system,
        args.max_bodies,
        args.num_body_mutexes,
        args.max_body_pairs,
        args.max_contact_constraints,
        broad_phase_layer_interface,
        object_vs_broad_phase_layer_filter,
        object_layer_pair_filter,
    );
    return @ptrCast(PhysicsSystem, physics_system);
}

pub const PhysicsSystem = *opaque {
    pub fn destroy(physics_system: PhysicsSystem) void {
        c.JPH_PhysicsSystem_Destroy(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }

    pub fn getNumBodies(physics_system: PhysicsSystem) u32 {
        return c.JPH_PhysicsSystem_GetNumBodies(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }
    pub fn getNumActiveBodies(physics_system: PhysicsSystem) u32 {
        return c.JPH_PhysicsSystem_GetNumActiveBodies(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }
    pub fn getMaxBodies(physics_system: PhysicsSystem) u32 {
        return c.JPH_PhysicsSystem_GetMaxBodies(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }

    pub fn getBodyInterface(physics_system: PhysicsSystem) BodyInterface {
        return @ptrCast(
            BodyInterface,
            c.JPH_PhysicsSystem_GetBodyInterface(@ptrCast(*c.JPH_PhysicsSystem, physics_system)).?,
        );
    }

    pub fn setBodyActivationListener(physics_system: PhysicsSystem, listener: ?*anyopaque) void {
        c.JPH_PhysicsSystem_SetBodyActivationListener(@ptrCast(*c.JPH_PhysicsSystem, physics_system), listener);
    }
    pub fn getBodyActivationListener(physics_system: PhysicsSystem) ?*anyopaque {
        return c.JPH_PhysicsSystem_GetBodyActivationListener(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }

    pub fn setContactListener(physics_system: PhysicsSystem, listener: ?*anyopaque) void {
        c.JPH_PhysicsSystem_SetContactListener(@ptrCast(*c.JPH_PhysicsSystem, physics_system), listener);
    }
    pub fn getContactListener(physics_system: PhysicsSystem) ?*anyopaque {
        return c.JPH_PhysicsSystem_GetContactListener(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }

    pub fn optimizeBroadPhase(physics_system: PhysicsSystem) void {
        c.JPH_PhysicsSystem_OptimizeBroadPhase(@ptrCast(*c.JPH_PhysicsSystem, physics_system));
    }

    pub fn update(
        physics_system: PhysicsSystem,
        delta_time: f32,
        args: struct {
            collision_steps: i32 = 1,
            integration_sub_steps: i32 = 1,
        },
    ) void {
        c.JPH_PhysicsSystem_Update(
            @ptrCast(*c.JPH_PhysicsSystem, physics_system),
            delta_time,
            args.collision_steps,
            args.integration_sub_steps,
            @ptrCast(*c.JPH_TempAllocator, temp_allocator.?),
            @ptrCast(*c.JPH_JobSystem, job_system.?),
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// BodyInterface
//
//--------------------------------------------------------------------------------------------------
pub const BodyInterface = *opaque {
    pub fn createBody(body_iface: BodyInterface, settings: BodyCreationSettings) ?Body {
        return @ptrCast(?Body, c.JPH_BodyInterface_CreateBody(
            @ptrCast(*c.JPH_BodyInterface, body_iface),
            @ptrCast(*const c.JPH_BodyCreationSettings, &settings),
        ));
    }
};
//--------------------------------------------------------------------------------------------------
//
// ShapeSettings
//
//--------------------------------------------------------------------------------------------------
pub const ShapeSettings = *align(@sizeOf(usize)) ShapeSettingsImpl;

pub const ShapeSettingsImpl = opaque {
    pub usingnamespace Methods(ShapeSettings);

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asShapeSettings(shape_settings: T) ShapeSettings {
                return @ptrCast(ShapeSettings, shape_settings);
            }

            pub fn addRef(shape_settings: T) void {
                c.JPH_ShapeSettings_AddRef(@ptrCast(*c.JPH_ShapeSettings, shape_settings));
            }
            pub fn release(shape_settings: T) void {
                c.JPH_ShapeSettings_Release(@ptrCast(*c.JPH_ShapeSettings, shape_settings));
            }
            pub fn getRefCount(shape_settings: T) u32 {
                return c.JPH_ShapeSettings_GetRefCount(@ptrCast(*c.JPH_ShapeSettings, shape_settings));
            }

            pub fn createShape(shape_settings: T) !Shape {
                const shape = c.JPH_ShapeSettings_CreateShape(@ptrCast(*c.JPH_ShapeSettings, shape_settings));
                if (shape == null)
                    return error.FailedToCreateShape;
                return @ptrCast(Shape, @alignCast(@sizeOf(usize), shape.?));
            }

            pub fn getUserData(shape_settings: T) u64 {
                return c.JPH_ShapeSettings_GetUserData(@ptrCast(*c.JPH_ShapeSettings, shape_settings));
            }
            pub fn setUserData(shape_settings: T, user_data: u64) void {
                return c.JPH_ShapeSettings_SetUserData(@ptrCast(*c.JPH_ShapeSettings, shape_settings), user_data);
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// ConvexShapeSettings (-> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const ConvexShapeSettings = *align(@sizeOf(usize)) ConvexShapeSettingsImpl;

pub const ConvexShapeSettingsImpl = opaque {
    pub usingnamespace Methods(ConvexShapeSettings);

    fn Methods(comptime T: type) type {
        return struct {
            pub usingnamespace ShapeSettingsImpl.Methods(T);

            pub fn asConvexShapeSettings(convex_shape_settings: T) ConvexShapeSettings {
                return @ptrCast(ConvexShapeSettings, convex_shape_settings);
            }

            pub fn getMaterial(convex_shape_settings: T) ?Material {
                return @ptrCast(?Material, c.JPH_ConvexShapeSettings_GetMaterial(
                    @ptrCast(*c.JPH_ConvexShapeSettings, convex_shape_settings),
                ));
            }
            pub fn setMaterial(convex_shape_settings: T, material: ?Material) void {
                c.JPH_ConvexShapeSettings_SetMaterial(
                    @ptrCast(*c.JPH_ConvexShapeSettings, convex_shape_settings),
                    @ptrCast(?*c.JPH_PhysicsMaterial, material),
                );
            }

            pub fn getDensity(convex_shape_settings: T) f32 {
                return c.JPH_ConvexShapeSettings_GetDensity(
                    @ptrCast(*c.JPH_ConvexShapeSettings, convex_shape_settings),
                );
            }
            pub fn setDensity(shape_settings: T, density: f32) void {
                c.JPH_ConvexShapeSettings_SetDensity(
                    @ptrCast(*c.JPH_ConvexShapeSettings, shape_settings),
                    density,
                );
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// BoxShapeSettings
//
//--------------------------------------------------------------------------------------------------
pub const BoxShapeSettings = *align(@sizeOf(usize)) BoxShapeSettingsImpl;

pub fn createBoxShapeSettings(half_extent: [3]f32) !BoxShapeSettings {
    const box_shape_settings = c.JPH_BoxShapeSettings_Create(&half_extent);
    if (box_shape_settings == null)
        return error.FailedToCreateBoxShapeSettings;
    return @ptrCast(BoxShapeSettings, @alignCast(@sizeOf(usize), box_shape_settings.?));
}

pub const BoxShapeSettingsImpl = opaque {
    usingnamespace ConvexShapeSettingsImpl.Methods(BoxShapeSettings);

    pub fn getHalfExtent(box_shape_settings: BoxShapeSettings) [3]f32 {
        var half_extent: [3]f32 = undefined;
        c.JPH_BoxShapeSettings_GetHalfExtent(@ptrCast(*c.JPH_BoxShapeSettings, box_shape_settings), &half_extent);
        return half_extent;
    }
    pub fn setHalfExtent(box_shape_settings: BoxShapeSettings, half_extent: [3]f32) void {
        c.JPH_BoxShapeSettings_SetHalfExtent(@ptrCast(*c.JPH_BoxShapeSettings, box_shape_settings), &half_extent);
    }

    pub fn getConvexRadius(box_shape_settings: BoxShapeSettings) f32 {
        return c.JPH_BoxShapeSettings_GetConvexRadius(@ptrCast(*c.JPH_BoxShapeSettings, box_shape_settings));
    }
    pub fn setConvexRadius(box_shape_settings: BoxShapeSettings, convex_radius: f32) void {
        c.JPH_BoxShapeSettings_SetConvexRadius(
            @ptrCast(*c.JPH_BoxShapeSettings, box_shape_settings),
            convex_radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// Shape
//
//--------------------------------------------------------------------------------------------------
pub const ShapeType = enum(c.JPH_ShapeType) {
    convex = c.JPH_SHAPE_TYPE_CONVEX,
    compound = c.JPH_SHAPE_TYPE_COMPOUND,
    decorated = c.JPH_SHAPE_TYPE_DECORATED,
    mesh = c.JPH_SHAPE_TYPE_MESH,
    height_field = c.JPH_SHAPE_TYPE_HEIGHT_FIELD,
    user1 = c.JPH_SHAPE_TYPE_USER1,
    user2 = c.JPH_SHAPE_TYPE_USER2,
    user3 = c.JPH_SHAPE_TYPE_USER3,
    user4 = c.JPH_SHAPE_TYPE_USER4,
};

pub const ShapeSubType = enum(c.JPH_ShapeSubType) {
    sphere = c.JPH_SHAPE_SUB_TYPE_SPHERE,
    box = c.JPH_SHAPE_SUB_TYPE_BOX,
    triangle = c.JPH_SHAPE_SUB_TYPE_TRIANGLE,
    capsule = c.JPH_SHAPE_SUB_TYPE_CAPSULE,
    tapered_capsule = c.JPH_SHAPE_SUB_TYPE_TAPERED_CAPSULE,
    cylinder = c.JPH_SHAPE_SUB_TYPE_CYLINDER,
    convex_hull = c.JPH_SHAPE_SUB_TYPE_CONVEX_HULL,
    static_compound = c.JPH_SHAPE_SUB_TYPE_STATIC_COMPOUND,
    mutable_compound = c.JPH_SHAPE_SUB_TYPE_MUTABLE_COMPOUND,
    rotated_translated = c.JPH_SHAPE_SUB_TYPE_ROTATED_TRANSLATED,
    scaled = c.JPH_SHAPE_SUB_TYPE_SCALED,
    offset_center_of_mass = c.JPH_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS,
    mesh = c.JPH_SHAPE_SUB_TYPE_MESH,
    height_field = c.JPH_SHAPE_SUB_TYPE_HEIGHT_FIELD,
    user1 = c.JPH_SHAPE_SUB_TYPE_USER1,
    user2 = c.JPH_SHAPE_SUB_TYPE_USER2,
    user3 = c.JPH_SHAPE_SUB_TYPE_USER3,
    user4 = c.JPH_SHAPE_SUB_TYPE_USER4,
    user5 = c.JPH_SHAPE_SUB_TYPE_USER5,
    user6 = c.JPH_SHAPE_SUB_TYPE_USER6,
    user7 = c.JPH_SHAPE_SUB_TYPE_USER7,
    user8 = c.JPH_SHAPE_SUB_TYPE_USER8,
};

pub const Shape = *align(@sizeOf(usize)) ShapeImpl;

pub const ShapeImpl = opaque {
    pub usingnamespace Methods(Shape);

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asShape(shape: T) Shape {
                return @ptrCast(Shape, shape);
            }

            pub fn addRef(shape: T) void {
                c.JPH_Shape_AddRef(@ptrCast(*c.JPH_Shape, shape));
            }
            pub fn release(shape: T) void {
                c.JPH_Shape_Release(@ptrCast(*c.JPH_Shape, shape));
            }
            pub fn getRefCount(shape: T) u32 {
                return c.JPH_Shape_GetRefCount(@ptrCast(*c.JPH_Shape, shape));
            }

            pub fn getType(shape: T) ShapeType {
                return @intToEnum(
                    ShapeType,
                    c.JPH_Shape_GetType(@ptrCast(*c.JPH_Shape, shape)),
                );
            }
            pub fn getSubType(shape: T) ShapeSubType {
                return @intToEnum(
                    ShapeSubType,
                    c.JPH_Shape_GetSubType(@ptrCast(*c.JPH_Shape, shape)),
                );
            }

            pub fn getUserData(shape: T) u64 {
                return c.JPH_Shape_GetUserData(@ptrCast(*c.JPH_Shape, shape));
            }
            pub fn setUserData(shape: T, user_data: u64) void {
                return c.JPH_Shape_SetUserData(@ptrCast(*c.JPH_Shape, shape), user_data);
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Memory allocation
//
//--------------------------------------------------------------------------------------------------
export fn zphysicsAlloc(size: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const mem = mem_allocator.?.allocBytes(
        mem_alignment,
        size,
        0,
        @returnAddress(),
    ) catch @panic("zphysics: out of memory");

    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch @panic("zphysics: out of memory");

    return mem.ptr;
}

export fn zphysicsAlignedAlloc(size: usize, alignment: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const mem = mem_allocator.?.allocBytes(
        @intCast(u29, alignment),
        size,
        0,
        @returnAddress(),
    ) catch @panic("zphysics: out of memory");

    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch @panic("zphysics: out of memory");

    return mem.ptr;
}

export fn zphysicsFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const size = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;
        const mem = @ptrCast(
            [*]align(mem_alignment) u8,
            @alignCast(mem_alignment, ptr),
        )[0..size];
        mem_allocator.?.free(mem);
    }
}
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zphysics.basic" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const broad_phase_layer_interface = test_cb1.BPLayerInterfaceImpl.init();

    const physics_system = try createPhysicsSystem(
        &broad_phase_layer_interface,
        test_cb1.myBroadPhaseCanCollide,
        test_cb1.myObjectCanCollide,
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );
    defer physics_system.destroy();

    try expect(physics_system.getNumBodies() == 0);
    try expect(physics_system.getNumActiveBodies() == 0);
    try expect(physics_system.getMaxBodies() == 1024);

    try expect(physics_system.getBodyActivationListener() == null);
    physics_system.setBodyActivationListener(null);
    try expect(physics_system.getBodyActivationListener() == null);

    try expect(physics_system.getContactListener() == null);
    physics_system.setContactListener(null);
    try expect(physics_system.getContactListener() == null);

    _ = physics_system.getBodyInterface();

    physics_system.optimizeBroadPhase();
    physics_system.update(1.0 / 60.0, .{ .collision_steps = 1, .integration_sub_steps = 1 });
    physics_system.update(1.0 / 60.0, .{});

    _ = CollisionGroup.init();
    _ = BodyCreationSettings.init();

    const box_shape_settings = try createBoxShapeSettings(.{ 1.0, 2.0, 3.0 });
    defer box_shape_settings.release();

    box_shape_settings.setDensity(2.0);
    try expect(box_shape_settings.getDensity() == 2.0);

    box_shape_settings.setUserData(123);
    try expect(box_shape_settings.getUserData() == 123);

    box_shape_settings.setConvexRadius(0.5);
    try expect(box_shape_settings.getConvexRadius() == 0.5);

    try expect(box_shape_settings.getRefCount() == 1);
    box_shape_settings.addRef();
    try expect(box_shape_settings.getRefCount() == 2);
    box_shape_settings.release();
    try expect(box_shape_settings.getRefCount() == 1);

    {
        var he = box_shape_settings.getHalfExtent();
        try expect(he[0] == 1.0 and he[1] == 2.0 and he[2] == 3.0);
        box_shape_settings.setHalfExtent(.{ 4.0, 5.0, 6.0 });
        he = box_shape_settings.getHalfExtent();
        try expect(he[0] == 4.0 and he[1] == 5.0 and he[2] == 6.0);
    }

    try expect(box_shape_settings.asConvexShapeSettings().getDensity() == 2.0);
    try expect(box_shape_settings.asShapeSettings().getRefCount() == 1);

    const box_shape = try box_shape_settings.createShape();
    defer box_shape.release();
    try expect(box_shape.getRefCount() == 2);
    try expect(box_shape_settings.getRefCount() == 1);

    try expect(box_shape.getType() == .convex);
    try expect(box_shape.getSubType() == .box);

    box_shape.setUserData(456);
    try expect(box_shape.getUserData() == 456);
}

extern fn JoltCTest_Basic1() u32;
test "jolt_c.basic1" {
    const ret = JoltCTest_Basic1();
    try expect(ret != 0);
}

extern fn JoltCTest_Basic2() u32;
test "jolt_c.basic2" {
    const ret = JoltCTest_Basic2();
    try expect(ret != 0);
}

extern fn JoltCTest_HelloWorld() u32;
test "jolt_c.helloworld" {
    const ret = JoltCTest_HelloWorld();
    try expect(ret != 0);
}

const test_cb1 = struct {
    const layers = struct {
        const non_moving: ObjectLayer = 0;
        const moving: ObjectLayer = 1;
        const len: u32 = 2;
    };

    const broad_phase_layers = struct {
        const non_moving: BroadPhaseLayer = 0;
        const moving: BroadPhaseLayer = 1;
        const len: u32 = 2;
    };

    const BPLayerInterfaceImpl = extern struct {
        vtable_ptr: *const BroadPhaseLayerInterfaceVTable = &vtable,
        object_to_broad_phase: [layers.len]BroadPhaseLayer = undefined,

        const vtable = BroadPhaseLayerInterfaceVTable{
            .getNumBroadPhaseLayers = getNumBroadPhaseLayers,
            .getBroadPhaseLayer = getBroadPhaseLayer,
        };

        fn init() BPLayerInterfaceImpl {
            var layer_interface: BPLayerInterfaceImpl = .{};
            layer_interface.object_to_broad_phase[layers.non_moving] = broad_phase_layers.non_moving;
            layer_interface.object_to_broad_phase[layers.moving] = broad_phase_layers.moving;
            return layer_interface;
        }

        fn getNumBroadPhaseLayers(self: *const anyopaque) callconv(.C) u32 {
            const layer_interface = @ptrCast(*const BPLayerInterfaceImpl, @alignCast(@sizeOf(usize), self));
            return @intCast(u32, layer_interface.object_to_broad_phase.len);
        }

        fn getBroadPhaseLayer(self: *const anyopaque, layer: ObjectLayer) callconv(.C) BroadPhaseLayer {
            const layer_interface = @ptrCast(*const BPLayerInterfaceImpl, @alignCast(@sizeOf(usize), self));
            return layer_interface.object_to_broad_phase[@intCast(usize, layer)];
        }
    };

    fn myBroadPhaseCanCollide(layer1: ObjectLayer, layer2: BroadPhaseLayer) callconv(.C) bool {
        return switch (layer1) {
            layers.non_moving => layer2 == broad_phase_layers.moving,
            layers.moving => true,
            else => unreachable,
        };
    }

    fn myObjectCanCollide(object1: ObjectLayer, object2: ObjectLayer) callconv(.C) bool {
        return switch (object1) {
            layers.non_moving => object2 == layers.moving,
            layers.moving => true,
            else => unreachable,
        };
    }
};
//--------------------------------------------------------------------------------------------------
