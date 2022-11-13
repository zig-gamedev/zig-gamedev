const std = @import("std");
const assert = std.debug.assert;

pub const BroadPhaseLayer = u16;
pub const ObjectLayer = u8;
pub const ObjectVsBroadPhaseLayerFilter = *const fn (ObjectLayer, BroadPhaseLayer) callconv(.C) bool;
pub const ObjectLayerPairFilter = *const fn (ObjectLayer, ObjectLayer) callconv(.C) bool;

pub const BroadPhaseLayerInterfaceVTable = extern struct {
    reserved0: ?*const anyopaque = null,
    reserved1: ?*const anyopaque = null,
    GetNumBroadPhaseLayers: *const fn (self: *const anyopaque) callconv(.C) u32,
    GetBroadPhaseLayer: *const fn (self: *const anyopaque, layer: ObjectLayer) callconv(.C) BroadPhaseLayer,
};

pub const MassProperties = extern struct {
    mass: f32,
    inertia: [16]f32 align(16),
};

pub const Shape = *opaque {};
pub const GroupFilter = *opaque {};

pub const CollisionGroup = extern struct {
    filter: ?*const GroupFilter,
    group_id: GroupId,
    sub_group_id: SubGroupId,

    pub const GroupId = u32;
    pub const SubGroupId = u32;

    const invalid_group = ~@as(GroupId, 0);
    const invalid_sub_group = ~@as(SubGroupId, 0);

    pub fn init() CollisionGroup {
        return JPH_CollisionGroup_InitDefault();
    }
    extern fn JPH_CollisionGroup_InitDefault() CollisionGroup;
};

pub const MotionType = enum(u8) {
    static,
    kinematic,
    dynamic,
};

pub const MotionQuality = enum(u8) {
    discrete,
    linear_cast,
};

pub const OverrideMassProperties = enum(u8) {
    calc_mass_inertia,
    calc_inertia,
    mass_inertia_provided,
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
        return JPH_BodyCreationSettings_InitDefault();
    }
    extern fn JPH_BodyCreationSettings_InitDefault() BodyCreationSettings;
};

pub const max_physics_jobs: u32 = 2048;
pub const max_physics_barriers: u32 = 8;

const TempAllocator = *opaque {};
const JobSystem = *opaque {};

var temp_allocator: ?TempAllocator = null;
var job_system: ?JobSystem = null;

pub fn init(allocator: std.mem.Allocator, args: struct {
    temp_allocator_size: u32 = 16 * 1024 * 1024,
    max_jobs: u32 = max_physics_jobs,
    max_barriers: u32 = max_physics_barriers,
    num_threads: i32 = -1,
}) !void {
    // TODO: Add support for Zig allocator (JPH_RegisterCustomAllocator).
    _ = allocator;
    JPH_RegisterDefaultAllocator();

    JPH_CreateFactory();
    JPH_RegisterTypes();

    assert(temp_allocator == null and job_system == null);
    temp_allocator = JPH_TempAllocator_Create(args.temp_allocator_size);
    job_system = JPH_JobSystem_Create(args.max_jobs, args.max_barriers, args.num_threads);
}
extern fn JPH_RegisterDefaultAllocator() void;
extern fn JPH_CreateFactory() void;
extern fn JPH_RegisterTypes() void;
extern fn JPH_TempAllocator_Create(size: u32) TempAllocator;
extern fn JPH_JobSystem_Create(max_jobs: u32, max_barriers: u32, num_threads: i32) JobSystem;

pub fn deinit() void {
    JPH_JobSystem_Destroy(job_system.?);
    job_system = null;
    JPH_TempAllocator_Destroy(temp_allocator.?);
    temp_allocator = null;
    JPH_DestroyFactory();
}
extern fn JPH_DestroyFactory() void;
extern fn JPH_TempAllocator_Destroy(temp_allocator: TempAllocator) void;
extern fn JPH_JobSystem_Destroy(job_system: JobSystem) void;

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
    const physics_system = JPH_PhysicsSystem_Create();
    JPH_PhysicsSystem_Init(
        physics_system,
        args.max_bodies,
        args.num_body_mutexes,
        args.max_body_pairs,
        args.max_contact_constraints,
        broad_phase_layer_interface,
        object_vs_broad_phase_layer_filter,
        object_layer_pair_filter,
    );
    return physics_system;
}
extern fn JPH_PhysicsSystem_Create() PhysicsSystem;
extern fn JPH_PhysicsSystem_Init(
    physics_system: PhysicsSystem,
    max_bodies: u32,
    num_body_mutexes: u32,
    max_body_pairs: u32,
    max_contact_constraints: u32,
    broad_phase_layer_interface: *const anyopaque,
    object_vs_broad_phase_layer_filter: ObjectVsBroadPhaseLayerFilter,
    object_layer_pair_filter: ObjectLayerPairFilter,
) void;

pub const PhysicsSystem = *opaque {
    pub fn destroy(physics_system: PhysicsSystem) void {
        JPH_PhysicsSystem_Destroy(physics_system);
    }
    extern fn JPH_PhysicsSystem_Destroy(physics_system: PhysicsSystem) void;
};
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

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
            .GetNumBroadPhaseLayers = getNumBroadPhaseLayers,
            .GetBroadPhaseLayer = getBroadPhaseLayer,
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
}
