const std = @import("std");
const assert = std.debug.assert;
const options = @import("zphysics_options");
const c = @cImport({
    if (options.use_double_precision) @cDefine("JPH_DOUBLE_PRECISION", "");
    if (options.enable_asserts) @cDefine("JPH_ENABLE_ASSERTS", "");
    @cInclude("JoltPhysicsC.h");
});

pub const Real = c.JPC_Real;
comptime {
    assert(if (options.use_double_precision) Real == f64 else Real == f32);
}

pub const rvec_align = if (Real == f64) 32 else 16;

pub const Material = opaque {};
pub const GroupFilter = opaque {};
pub const BodyLockInterface = opaque {};
pub const SharedMutex = opaque {};

pub const BroadPhaseLayer = c.JPC_BroadPhaseLayer;
pub const ObjectLayer = c.JPC_ObjectLayer;
pub const BodyId = c.JPC_BodyID;
pub const SubShapeId = c.JPC_SubShapeID;

pub const ObjectVsBroadPhaseLayerFilter = *const fn (ObjectLayer, BroadPhaseLayer) callconv(.C) bool;
pub const ObjectLayerPairFilter = *const fn (ObjectLayer, ObjectLayer) callconv(.C) bool;

pub const max_physics_jobs: u32 = c.JPC_MAX_PHYSICS_JOBS;
pub const max_physics_barriers: u32 = c.JPC_MAX_PHYSICS_BARRIERS;

pub const body_id_invalid: BodyId = c.JPC_BODY_ID_INVALID;
pub const body_id_index_bits: BodyId = c.JPC_BODY_ID_INDEX_BITS;
pub const body_id_sequence_bits: BodyId = c.JPC_BODY_ID_SEQUENCE_BITS;
pub const body_id_sequence_shift: BodyId = c.JPC_BODY_ID_SEQUENCE_SHIFT;

const TempAllocator = opaque {};
const JobSystem = opaque {};

/// Check if this is a valid body pointer.
/// When a body is freed the memory that the pointer occupies is reused to store a freelist.
/// NOTE: This function is *not* protected by a lock, use with care!
pub inline fn isValidBodyPointer(body: *const Body) bool {
    return (@ptrToInt(body) & c._JPC_IS_FREED_BODY_BIT) == 0;
}

/// Access a body, will return a `null` if the `body_id` is no longer valid.
/// Use `PhysicsSystem.getBodies()` to get all the bodies.
/// NOTE: This function is *not* protected by a lock, use with care!
pub inline fn tryGetBody(all_bodies: []const *const Body, body_id: BodyId) ?*const Body {
    const body = all_bodies[body_id & body_id_index_bits];
    return if (isValidBodyPointer(body) and body.id == body_id) body else null;
}
/// Access a body, will return a `null` if the `body_id` is no longer valid.
/// Use `PhysicsSystem.getBodiesMut()` to get all the bodies.
/// NOTE: This function is *not* protected by a lock, use with care!
pub inline fn tryGetBodyMut(all_bodies: []const *Body, body_id: BodyId) ?*Body {
    const body = all_bodies[body_id & body_id_index_bits];
    return if (isValidBodyPointer(body) and body.id == body_id) body else null;
}

pub const BroadPhaseLayerInterfaceVTable = extern struct {
    getNumBroadPhaseLayers: *const fn (self: *const anyopaque) callconv(.C) u32,
    getBroadPhaseLayer: *const fn (self: *const anyopaque, layer: ObjectLayer) callconv(.C) BroadPhaseLayer,

    comptime {
        assert(@sizeOf(BroadPhaseLayerInterfaceVTable) == @sizeOf(c.JPC_BroadPhaseLayerInterfaceVTable));
        assert(@offsetOf(BroadPhaseLayerInterfaceVTable, "getBroadPhaseLayer") == @offsetOf(
            c.JPC_BroadPhaseLayerInterfaceVTable,
            "GetBroadPhaseLayer",
        ));
    }
};

pub const BodyActivationListenerVTable = extern struct {
    onBodyActivated: *const fn (self: *anyopaque, body_id: BodyId, user_data: u64) callconv(.C) void,
    onBodyDeactivated: *const fn (self: *anyopaque, body_id: BodyId, user_data: u64) callconv(.C) void,

    comptime {
        assert(@sizeOf(BodyActivationListenerVTable) == @sizeOf(c.JPC_BodyActivationListenerVTable));
        assert(@offsetOf(BodyActivationListenerVTable, "onBodyDeactivated") == @offsetOf(
            c.JPC_BodyActivationListenerVTable,
            "OnBodyDeactivated",
        ));
    }
};

pub const ContactListenerVTable = extern struct {
    onContactValidate: ?*const fn (
        self: *anyopaque,
        body1: *const Body,
        body2: *const Body,
        in_base_offset: *const [3]Real,
        collision_result: *const CollideShapeResult,
    ) callconv(.C) ValidateResult = null,

    onContactAdded: ?*const fn (
        self: *anyopaque,
        body1: *const Body,
        body2: *const Body,
        manifold: *const ContactManifold,
        settings: *ContactSettings,
    ) callconv(.C) void = null,

    onContactPersisted: ?*const fn (
        self: *anyopaque,
        body1: *const Body,
        body2: *const Body,
        manifold: *const ContactManifold,
        settings: *ContactSettings,
    ) callconv(.C) void = null,

    onContactRemoved: ?*const fn (
        self: *anyopaque,
        sub_shape_pair: *const SubShapeIdPair,
    ) callconv(.C) void = null,

    comptime {
        assert(@sizeOf(ContactListenerVTable) == @sizeOf(c.JPC_ContactListenerVTable));
        assert(@offsetOf(ContactListenerVTable, "onContactAdded") == @offsetOf(
            c.JPC_ContactListenerVTable,
            "OnContactAdded",
        ));
        assert(@offsetOf(ContactListenerVTable, "onContactRemoved") == @offsetOf(
            c.JPC_ContactListenerVTable,
            "OnContactRemoved",
        ));
    }
};

pub const ContactSettings = extern struct {
    combined_friction: f32,
    combined_restitution: f32,
    is_sensor: bool,

    comptime {
        assert(@sizeOf(ContactSettings) == @sizeOf(c.JPC_ContactSettings));
        assert(@offsetOf(ContactSettings, "combined_restitution") == @offsetOf(
            c.JPC_ContactSettings,
            "combined_restitution",
        ));
    }
};

pub const MassProperties = extern struct {
    mass: f32 = 0.0,
    inertia: [16]f32 align(16) = [_]f32{0} ** 16,

    comptime {
        assert(@sizeOf(MassProperties) == @sizeOf(c.JPC_MassProperties));
        assert(@offsetOf(MassProperties, "inertia") == @offsetOf(c.JPC_MassProperties, "inertia"));
    }
};

pub const SubShapeIdPair = extern struct {
    first: extern struct {
        body_id: BodyId,
        sub_shape_id: SubShapeId,
    },
    second: extern struct {
        body_id: BodyId,
        sub_shape_id: SubShapeId,
    },

    comptime {
        assert(@sizeOf(SubShapeIdPair) == @sizeOf(c.JPC_SubShapeIDPair));
        assert(@offsetOf(SubShapeIdPair, "second") == @offsetOf(c.JPC_SubShapeIDPair, "second"));
    }
};

pub const CollideShapeResult = extern struct {
    shape1_contact_point: [4]f32 align(16), // 4th element is ignored; world space
    shape2_contact_point: [4]f32 align(16), // 4th element is ignored; world space
    penetration_axis: [4]f32 align(16), // 4th element is ignored; world space
    penetration_depth: f32,
    shape1_sub_shape_id: SubShapeId,
    shape2_sub_shape_id: SubShapeId,
    body2_id: BodyId,
    shape1_face: extern struct {
        num_points: u32 align(16),
        points: [32][4]f32 align(16), // 4th element is ignored; world space
    },
    shape2_face: extern struct {
        num_points: u32 align(16),
        points: [32][4]f32 align(16), // 4th element is ignored; world space
    },

    comptime {
        assert(@sizeOf(CollideShapeResult) == @sizeOf(c.JPC_CollideShapeResult));
        assert(@offsetOf(CollideShapeResult, "shape2_face") == @offsetOf(c.JPC_CollideShapeResult, "shape2_face"));
    }
};

pub const ContactManifold = extern struct {
    base_offset: [4]Real align(rvec_align), // 4th element is ignored; world space
    normal: [4]f32 align(16), // 4th element is ignored; world space
    penetration_depth: f32,
    shape1_sub_shape_id: SubShapeId,
    shape2_sub_shape_id: SubShapeId,
    shape1_relative_contact: extern struct {
        num_points: u32 align(16),
        points: [64][4]f32 align(16), // 4th element is ignored; world space
    },
    shape2_relative_contact: extern struct {
        num_points: u32 align(16),
        points: [64][4]f32 align(16), // 4th element is ignored; world space
    },

    comptime {
        assert(@sizeOf(ContactManifold) == @sizeOf(c.JPC_ContactManifold));
        assert(@offsetOf(ContactManifold, "shape2_relative_contact") ==
            @offsetOf(c.JPC_ContactManifold, "shape2_relative_contact"));
    }
};

pub const CollisionGroup = extern struct {
    filter: ?*GroupFilter = null,
    group_id: GroupId = invalid_group,
    sub_group_id: SubGroupId = invalid_sub_group,

    pub const GroupId = c.JPC_CollisionGroupID;
    pub const SubGroupId = c.JPC_CollisionSubGroupID;

    const invalid_group = @as(GroupId, c.JPC_COLLISION_GROUP_INVALID_GROUP);
    const invalid_sub_group = @as(SubGroupId, c.JPC_COLLISION_GROUP_INVALID_SUB_GROUP);

    comptime {
        assert(@sizeOf(CollisionGroup) == @sizeOf(c.JPC_CollisionGroup));
    }
};

pub const Activation = enum(c.JPC_Activation) {
    activate = c.JPC_ACTIVATION_ACTIVATE,
    dont_activate = c.JPC_ACTIVATION_DONT_ACTIVATE,
};

pub const ValidateResult = enum(c.JPC_ValidateResult) {
    accept_all_contacts = c.JPC_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS,
    accept_contact = c.JPC_VALIDATE_RESULT_ACCEPT_CONTACT,
    reject_contact = c.JPC_VALIDATE_RESULT_REJECT_CONTACT,
    reject_all_contacts = c.JPC_VALIDATE_RESULT_REJECT_ALL_CONTACTS,
};

pub const MotionType = enum(c.JPC_MotionType) {
    static = c.JPC_MOTION_TYPE_STATIC,
    kinematic = c.JPC_MOTION_TYPE_KINEMATIC,
    dynamic = c.JPC_MOTION_TYPE_DYNAMIC,
};

pub const MotionQuality = enum(c.JPC_MotionQuality) {
    discrete = c.JPC_MOTION_QUALITY_DISCRETE,
    linear_cast = c.JPC_MOTION_QUALITY_LINEAR_CAST,
};

pub const OverrideMassProperties = enum(c.JPC_OverrideMassProperties) {
    calc_mass_inertia = c.JPC_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA,
    calc_inertia = c.JPC_OVERRIDE_MASS_PROPS_CALC_INERTIA,
    mass_inertia_provided = c.JPC_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED,
};

pub const BodyCreationSettings = extern struct {
    position: [4]Real align(rvec_align) = .{ 0, 0, 0, 0 }, // 4th element is ignored
    rotation: [4]f32 align(16) = .{ 0, 0, 0, 1 },
    linear_velocity: [4]f32 align(16) = .{ 0, 0, 0, 0 }, // 4th element is ignored
    angular_velocity: [4]f32 align(16) = .{ 0, 0, 0, 0 }, // 4th element is ignored
    user_data: u64 = 0,
    object_layer: ObjectLayer = 0,
    collision_group: CollisionGroup = .{},
    motion_type: MotionType = .dynamic,
    allow_dynamic_or_kinematic: bool = false,
    is_sensor: bool = false,
    motion_quality: MotionQuality = .discrete,
    allow_sleeping: bool = true,
    friction: f32 = 0.2,
    restitution: f32 = 0.0,
    linear_damping: f32 = 0.05,
    angular_damping: f32 = 0.05,
    max_linear_velocity: f32 = 500.0,
    max_angular_velocity: f32 = 0.25 * c.JPC_PI * 60.0,
    gravity_factor: f32 = 1.0,
    override_mass_properties: OverrideMassProperties = .calc_mass_inertia,
    inertia_multiplier: f32 = 1.0,
    mass_properties_override: MassProperties = .{},
    reserved: ?*const anyopaque = null,
    shape: ?*const Shape = null,

    comptime {
        assert(@sizeOf(BodyCreationSettings) == @sizeOf(c.JPC_BodyCreationSettings));
        assert(@offsetOf(BodyCreationSettings, "is_sensor") == @offsetOf(c.JPC_BodyCreationSettings, "is_sensor"));
        assert(@offsetOf(BodyCreationSettings, "shape") == @offsetOf(c.JPC_BodyCreationSettings, "shape"));
        assert(@offsetOf(BodyCreationSettings, "user_data") == @offsetOf(c.JPC_BodyCreationSettings, "user_data"));
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

var temp_allocator: ?*TempAllocator = null;
var job_system: ?*JobSystem = null;

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

    c.JPC_RegisterCustomAllocator(zphysicsAlloc, zphysicsFree, zphysicsAlignedAlloc, zphysicsFree);

    c.JPC_CreateFactory();
    c.JPC_RegisterTypes();

    assert(temp_allocator == null and job_system == null);
    temp_allocator = @ptrCast(*TempAllocator, c.JPC_TempAllocator_Create(args.temp_allocator_size));
    job_system = @ptrCast(*JobSystem, c.JPC_JobSystem_Create(args.max_jobs, args.max_barriers, args.num_threads));
}

pub fn deinit() void {
    c.JPC_JobSystem_Destroy(@ptrCast(*c.JPC_JobSystem, job_system));
    job_system = null;
    c.JPC_TempAllocator_Destroy(@ptrCast(*c.JPC_TempAllocator, temp_allocator));
    temp_allocator = null;
    c.JPC_DestroyFactory();

    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}
//--------------------------------------------------------------------------------------------------
//
// PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
pub const PhysicsSystem = opaque {
    pub fn create(
        broad_phase_layer_interface: *const anyopaque,
        object_vs_broad_phase_layer_filter: ObjectVsBroadPhaseLayerFilter,
        object_layer_pair_filter: ObjectLayerPairFilter,
        args: struct {
            max_bodies: u32 = 1024,
            num_body_mutexes: u32 = 0,
            max_body_pairs: u32 = 1024,
            max_contact_constraints: u32 = 1024,
        },
    ) !*PhysicsSystem {
        return @ptrCast(*PhysicsSystem, c.JPC_PhysicsSystem_Create(
            args.max_bodies,
            args.num_body_mutexes,
            args.max_body_pairs,
            args.max_contact_constraints,
            broad_phase_layer_interface,
            object_vs_broad_phase_layer_filter,
            object_layer_pair_filter,
        ));
    }

    pub fn destroy(physics_system: *PhysicsSystem) void {
        c.JPC_PhysicsSystem_Destroy(@ptrCast(*c.JPC_PhysicsSystem, physics_system));
    }

    pub fn getNumBodies(physics_system: *const PhysicsSystem) u32 {
        return c.JPC_PhysicsSystem_GetNumBodies(@ptrCast(*const c.JPC_PhysicsSystem, physics_system));
    }
    pub fn getNumActiveBodies(physics_system: *const PhysicsSystem) u32 {
        return c.JPC_PhysicsSystem_GetNumActiveBodies(@ptrCast(*const c.JPC_PhysicsSystem, physics_system));
    }
    pub fn getMaxBodies(physics_system: *const PhysicsSystem) u32 {
        return c.JPC_PhysicsSystem_GetMaxBodies(@ptrCast(*const c.JPC_PhysicsSystem, physics_system));
    }

    pub fn getBodyInterface(physics_system: *const PhysicsSystem) *const BodyInterface {
        return @ptrCast(
            *const BodyInterface,
            c.JPC_PhysicsSystem_GetBodyInterface(@intToPtr(*c.JPC_PhysicsSystem, @ptrToInt(physics_system))),
        );
    }
    pub fn getBodyInterfaceNoLock(physics_system: *const PhysicsSystem) *const BodyInterface {
        return @ptrCast(
            *const BodyInterface,
            c.JPC_PhysicsSystem_GetBodyInterfaceNoLock(@intToPtr(*c.JPC_PhysicsSystem, @ptrToInt(physics_system))),
        );
    }
    pub fn getBodyInterfaceMut(physics_system: *PhysicsSystem) *BodyInterface {
        return @ptrCast(
            *BodyInterface,
            c.JPC_PhysicsSystem_GetBodyInterface(@ptrCast(*c.JPC_PhysicsSystem, physics_system)),
        );
    }
    pub fn getBodyInterfaceMutNoLock(physics_system: *PhysicsSystem) *BodyInterface {
        return @ptrCast(
            *BodyInterface,
            c.JPC_PhysicsSystem_GetBodyInterfaceNoLock(@ptrCast(*c.JPC_PhysicsSystem, physics_system)),
        );
    }

    pub fn getBodyLockInterface(physics_system: *const PhysicsSystem) *const BodyLockInterface {
        return @ptrCast(
            *const BodyLockInterface,
            c.JPC_PhysicsSystem_GetBodyLockInterface(@ptrCast(*const c.JPC_PhysicsSystem, physics_system)),
        );
    }
    pub fn getBodyLockInterfaceNoLock(physics_system: *const PhysicsSystem) *const BodyLockInterface {
        return @ptrCast(
            *const BodyLockInterface,
            c.JPC_PhysicsSystem_GetBodyLockInterfaceNoLock(@ptrCast(*const c.JPC_PhysicsSystem, physics_system)),
        );
    }

    pub fn setBodyActivationListener(physics_system: *PhysicsSystem, listener: ?*anyopaque) void {
        c.JPC_PhysicsSystem_SetBodyActivationListener(@ptrCast(*c.JPC_PhysicsSystem, physics_system), listener);
    }
    pub fn getBodyActivationListener(physics_system: *const PhysicsSystem) ?*anyopaque {
        return c.JPC_PhysicsSystem_GetBodyActivationListener(@ptrCast(*const c.JPC_PhysicsSystem, physics_system));
    }

    pub fn setContactListener(physics_system: *PhysicsSystem, listener: ?*anyopaque) void {
        c.JPC_PhysicsSystem_SetContactListener(@ptrCast(*c.JPC_PhysicsSystem, physics_system), listener);
    }
    pub fn getContactListener(physics_system: *const PhysicsSystem) ?*anyopaque {
        return c.JPC_PhysicsSystem_GetContactListener(@ptrCast(*const c.JPC_PhysicsSystem, physics_system));
    }

    pub fn optimizeBroadPhase(physics_system: *PhysicsSystem) void {
        c.JPC_PhysicsSystem_OptimizeBroadPhase(@ptrCast(*c.JPC_PhysicsSystem, physics_system));
    }

    pub fn update(
        physics_system: *PhysicsSystem,
        delta_time: f32,
        args: struct {
            collision_steps: i32 = 1,
            integration_sub_steps: i32 = 1,
        },
    ) void {
        c.JPC_PhysicsSystem_Update(
            @ptrCast(*c.JPC_PhysicsSystem, physics_system),
            delta_time,
            args.collision_steps,
            args.integration_sub_steps,
            @ptrCast(*c.JPC_TempAllocator, temp_allocator),
            @ptrCast(*c.JPC_JobSystem, job_system),
        );
    }

    pub fn getBodyIds(physics_system: *const PhysicsSystem, body_ids: *std.ArrayList(BodyId)) !void {
        try body_ids.ensureTotalCapacityPrecise(physics_system.getMaxBodies());
        var num_body_ids: u32 = 0;
        c.JPC_PhysicsSystem_GetBodyIDs(
            @ptrCast(*const c.JPC_PhysicsSystem, physics_system),
            @intCast(u32, body_ids.capacity),
            &num_body_ids,
            body_ids.items.ptr,
        );
        body_ids.items.len = num_body_ids;
    }

    pub fn getActiveBodyIds(physics_system: *const PhysicsSystem, body_ids: *std.ArrayList(BodyId)) !void {
        try body_ids.ensureTotalCapacityPrecise(physics_system.getMaxBodies());
        var num_body_ids: u32 = 0;
        c.JPC_PhysicsSystem_GetActiveBodyIDs(
            @ptrCast(*const c.JPC_PhysicsSystem, physics_system),
            @intCast(u32, body_ids.capacity),
            &num_body_ids,
            body_ids.items.ptr,
        );
        body_ids.items.len = num_body_ids;
    }

    /// NOTE: Advanced. This function is *not* protected by a lock, use with care!
    pub fn getBodiesUnsafe(physics_system: *const PhysicsSystem) []const *const Body {
        const ptr = c.JPC_PhysicsSystem_GetBodiesUnsafe(
            @intToPtr(*c.JPC_PhysicsSystem, @ptrToInt(physics_system)),
        );
        return @ptrCast([*]const *const Body, ptr)[0..physics_system.getNumBodies()];
    }
    /// NOTE: Advanced. This function is *not* protected by a lock, use with care!
    pub fn getBodiesMutUnsafe(physics_system: *PhysicsSystem) []const *Body {
        const ptr = c.JPC_PhysicsSystem_GetBodiesUnsafe(@ptrCast(*c.JPC_PhysicsSystem, physics_system));
        return @ptrCast([*]const *Body, ptr)[0..physics_system.getNumBodies()];
    }
};
//--------------------------------------------------------------------------------------------------
//
// BodyLock*
//
//--------------------------------------------------------------------------------------------------
pub const BodyLockRead = extern struct {
    lock_interface: *const BodyLockInterface = undefined,
    mutex: ?*SharedMutex = null,
    body: ?*const Body = null,

    pub fn lock(
        read_lock: *BodyLockRead,
        lock_interface: *const BodyLockInterface,
        body_id: BodyId,
    ) void {
        c.JPC_BodyLockRead_Lock(
            @ptrCast(*c.JPC_BodyLockRead, read_lock),
            @ptrCast(*const c.JPC_BodyLockInterface, lock_interface),
            body_id,
        );
    }

    pub fn unlock(read_lock: *BodyLockRead) void {
        c.JPC_BodyLockRead_Unlock(@ptrCast(*c.JPC_BodyLockRead, read_lock));
    }

    comptime {
        assert(@sizeOf(BodyLockRead) == @sizeOf(c.JPC_BodyLockRead));
        assert(@offsetOf(BodyLockRead, "mutex") == @offsetOf(c.JPC_BodyLockRead, "mutex"));
        assert(@offsetOf(BodyLockRead, "body") == @offsetOf(c.JPC_BodyLockRead, "body"));
    }
};

pub const BodyLockWrite = extern struct {
    lock_interface: *const BodyLockInterface = undefined,
    mutex: ?*SharedMutex = null,
    body: ?*Body = null,

    pub fn lock(
        write_lock: *BodyLockWrite,
        lock_interface: *const BodyLockInterface,
        body_id: BodyId,
    ) void {
        c.JPC_BodyLockWrite_Lock(
            @ptrCast(*c.JPC_BodyLockWrite, write_lock),
            @ptrCast(*const c.JPC_BodyLockInterface, lock_interface),
            body_id,
        );
    }

    pub fn unlock(write_lock: *BodyLockWrite) void {
        c.JPC_BodyLockWrite_Unlock(@ptrCast(*c.JPC_BodyLockWrite, write_lock));
    }

    comptime {
        assert(@sizeOf(BodyLockWrite) == @sizeOf(c.JPC_BodyLockWrite));
        assert(@offsetOf(BodyLockWrite, "mutex") == @offsetOf(c.JPC_BodyLockWrite, "mutex"));
        assert(@offsetOf(BodyLockWrite, "body") == @offsetOf(c.JPC_BodyLockWrite, "body"));
    }
};
//--------------------------------------------------------------------------------------------------
//
// BodyInterface
//
//--------------------------------------------------------------------------------------------------
pub const BodyInterface = opaque {
    pub fn createBody(body_iface: *BodyInterface, settings: BodyCreationSettings) !*Body {
        const body = c.JPC_BodyInterface_CreateBody(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            @ptrCast(*const c.JPC_BodyCreationSettings, &settings),
        );
        if (body == null)
            return error.FailedToCreateBody;
        return @ptrCast(*Body, body);
    }

    pub fn destroyBody(body_iface: *BodyInterface, body_id: BodyId) void {
        c.JPC_BodyInterface_DestroyBody(@ptrCast(*c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn addBody(body_iface: *BodyInterface, body_id: BodyId, mode: Activation) void {
        c.JPC_BodyInterface_AddBody(@ptrCast(*c.JPC_BodyInterface, body_iface), body_id, @enumToInt(mode));
    }

    pub fn removeBody(body_iface: *BodyInterface, body_id: BodyId) void {
        c.JPC_BodyInterface_RemoveBody(@ptrCast(*c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn createAndAddBody(body_iface: *BodyInterface, settings: BodyCreationSettings, mode: Activation) !BodyId {
        const body_id = c.JPC_BodyInterface_CreateAndAddBody(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            @ptrCast(*const c.JPC_BodyCreationSettings, &settings),
            @enumToInt(mode),
        );
        if (body_id == body_id_invalid)
            return error.FailedToCreateBody;
        return body_id;
    }

    pub fn isAdded(body_iface: *const BodyInterface, body_id: BodyId) bool {
        return c.JPC_BodyInterface_IsAdded(@ptrCast(*const c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn isActive(body_iface: *const BodyInterface, body_id: BodyId) bool {
        return c.JPC_BodyInterface_IsActive(@ptrCast(*const c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn setLinearVelocity(body_iface: *BodyInterface, body_id: BodyId, velocity: [3]f32) void {
        return c.JPC_BodyInterface_SetLinearVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &velocity,
        );
    }

    pub fn getLinearVelocity(body_iface: *const BodyInterface, body_id: BodyId) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_BodyInterface_GetLinearVelocity(
            @ptrCast(*const c.JPC_BodyInterface, body_iface),
            body_id,
            &velocity,
        );
        return velocity;
    }

    pub fn getCenterOfMassPosition(body_iface: *const BodyInterface, body_id: BodyId) [3]Real {
        var position: [3]Real = undefined;
        c.JPC_BodyInterface_GetCenterOfMassPosition(
            @ptrCast(*const c.JPC_BodyInterface, body_iface),
            body_id,
            &position,
        );
        return position;
    }
};
//--------------------------------------------------------------------------------------------------
//
// Body
//
//--------------------------------------------------------------------------------------------------
pub const Body = extern struct {
    position: [4]Real align(rvec_align), // 4th element is ignored
    rotation: [4]f32 align(16),
    bounds_min: [4]f32 align(16), // 4th element is ignored
    bounds_max: [4]f32 align(16), // 4th element is ignored

    shape: *const Shape,
    motion_properties: ?*MotionProperties, // Will be null for static objects
    user_data: u64,
    collision_group: CollisionGroup,

    friction: f32,
    restitution: f32,
    id: BodyId,

    object_layer: ObjectLayer,

    broad_phase_layer: BroadPhaseLayer,
    motion_type: MotionType,
    flags: u8,

    pub fn getId(body: *const Body) BodyId {
        return c.JPC_Body_GetID(@ptrCast(*const c.JPC_Body, body));
    }

    pub fn isActive(body: *const Body) bool {
        return c.JPC_Body_IsActive(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn isStatic(body: *const Body) bool {
        return c.JPC_Body_IsStatic(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn isKinematic(body: *const Body) bool {
        return c.JPC_Body_IsKinematic(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn isDynamic(body: *const Body) bool {
        return c.JPC_Body_IsDynamic(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn canBeKinematicOrDynamic(body: *const Body) bool {
        return c.JPC_Body_CanBeKinematicOrDynamic(@ptrCast(*const c.JPC_Body, body));
    }

    pub fn isSensor(body: *const Body) bool {
        return c.JPC_Body_IsSensor(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn setIsSensor(body: *Body, is_sensor: bool) void {
        c.JPC_Body_SetIsSensor(@ptrCast(*c.JPC_Body, body), is_sensor);
    }

    comptime {
        assert(@sizeOf(Body) == @sizeOf(c.JPC_Body));
        assert(@offsetOf(Body, "flags") == @offsetOf(c.JPC_Body, "flags"));
        assert(@offsetOf(Body, "motion_properties") == @offsetOf(c.JPC_Body, "motion_properties"));
        assert(@offsetOf(Body, "object_layer") == @offsetOf(c.JPC_Body, "object_layer"));
        assert(@offsetOf(Body, "rotation") == @offsetOf(c.JPC_Body, "rotation"));
    }
};
//--------------------------------------------------------------------------------------------------
//
// MotionProperties
//
//--------------------------------------------------------------------------------------------------
pub const MotionProperties = extern struct {
    linear_velocity: [4]f32 align(16), // 4th element is ignored
    angular_velocity: [4]f32 align(16), // 4th element is ignored
    inv_inertia_diagnonal: [4]f32 align(16),
    inertia_rotation: [4]f32 align(16),

    force: [3]f32,
    torque: [3]f32,
    inv_mass: f32,
    linear_damping: f32,
    angular_damping: f32,
    max_linear_velocity: f32,
    max_angular_velocity: f32,
    gravity_factor: f32,
    index_in_active_bodies: u32,
    island_index: u32,

    motion_quality: MotionQuality,
    allow_sleeping: bool,

    reserved: [52 + c.JPC_ENABLE_ASSERTS * 3 + c.JPC_DOUBLE_PRECISION * 24]u8 align(4 + 4 * c.JPC_DOUBLE_PRECISION),

    comptime {
        assert(@sizeOf(MotionProperties) == @sizeOf(c.JPC_MotionProperties));
        assert(@offsetOf(MotionProperties, "force") == @offsetOf(c.JPC_MotionProperties, "force"));
        assert(@offsetOf(MotionProperties, "motion_quality") == @offsetOf(c.JPC_MotionProperties, "motion_quality"));
        assert(@offsetOf(MotionProperties, "gravity_factor") == @offsetOf(c.JPC_MotionProperties, "gravity_factor"));
    }
};
//--------------------------------------------------------------------------------------------------
//
// ShapeSettings
//
//--------------------------------------------------------------------------------------------------
pub const ShapeSettings = opaque {
    pub usingnamespace Methods(@This());

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asShapeSettings(shape_settings: *const T) *const ShapeSettings {
                return @ptrCast(*const ShapeSettings, shape_settings);
            }
            pub fn asShapeSettingsMut(shape_settings: *T) *ShapeSettings {
                return @ptrCast(*ShapeSettings, shape_settings);
            }

            pub fn addRef(shape_settings: *T) void {
                c.JPC_ShapeSettings_AddRef(@ptrCast(*c.JPC_ShapeSettings, shape_settings));
            }
            pub fn release(shape_settings: *T) void {
                c.JPC_ShapeSettings_Release(@ptrCast(*c.JPC_ShapeSettings, shape_settings));
            }
            pub fn getRefCount(shape_settings: *const T) u32 {
                return c.JPC_ShapeSettings_GetRefCount(@ptrCast(*const c.JPC_ShapeSettings, shape_settings));
            }

            pub fn createShape(shape_settings: *const T) !*Shape {
                const shape = c.JPC_ShapeSettings_CreateShape(@ptrCast(*const c.JPC_ShapeSettings, shape_settings));
                if (shape == null)
                    return error.FailedToCreateShape;
                return @ptrCast(*Shape, shape);
            }

            pub fn getUserData(shape_settings: *const T) u64 {
                return c.JPC_ShapeSettings_GetUserData(@ptrCast(*const c.JPC_ShapeSettings, shape_settings));
            }
            pub fn setUserData(shape_settings: *T, user_data: u64) void {
                return c.JPC_ShapeSettings_SetUserData(@ptrCast(*c.JPC_ShapeSettings, shape_settings), user_data);
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// ConvexShapeSettings (-> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const ConvexShapeSettings = opaque {
    pub usingnamespace Methods(@This());

    fn Methods(comptime T: type) type {
        return struct {
            pub usingnamespace ShapeSettings.Methods(T);

            pub fn asConvexShapeSettings(convex_shape_settings: *T) *ConvexShapeSettings {
                return @ptrCast(*ConvexShapeSettings, convex_shape_settings);
            }

            pub fn getMaterial(convex_shape_settings: *const T) ?*const Material {
                return @ptrCast(?*const Material, c.JPC_ConvexShapeSettings_GetMaterial(
                    @ptrCast(*const c.JPC_ConvexShapeSettings, convex_shape_settings),
                ));
            }
            pub fn setMaterial(convex_shape_settings: *T, material: ?*Material) void {
                c.JPC_ConvexShapeSettings_SetMaterial(
                    @ptrCast(*c.JPC_ConvexShapeSettings, convex_shape_settings),
                    @ptrCast(?*c.JPC_PhysicsMaterial, material),
                );
            }

            pub fn getDensity(convex_shape_settings: *const T) f32 {
                return c.JPC_ConvexShapeSettings_GetDensity(
                    @ptrCast(*const c.JPC_ConvexShapeSettings, convex_shape_settings),
                );
            }
            pub fn setDensity(shape_settings: *T, density: f32) void {
                c.JPC_ConvexShapeSettings_SetDensity(
                    @ptrCast(*c.JPC_ConvexShapeSettings, shape_settings),
                    density,
                );
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// BoxShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const BoxShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(half_extent: [3]f32) !*BoxShapeSettings {
        const box_shape_settings = c.JPC_BoxShapeSettings_Create(&half_extent);
        if (box_shape_settings == null)
            return error.FailedToCreateBoxShapeSettings;
        return @ptrCast(*BoxShapeSettings, box_shape_settings);
    }

    pub fn getHalfExtent(box_shape_settings: *const BoxShapeSettings) [3]f32 {
        var half_extent: [3]f32 = undefined;
        c.JPC_BoxShapeSettings_GetHalfExtent(
            @ptrCast(*const c.JPC_BoxShapeSettings, box_shape_settings),
            &half_extent,
        );
        return half_extent;
    }
    pub fn setHalfExtent(box_shape_settings: *BoxShapeSettings, half_extent: [3]f32) void {
        c.JPC_BoxShapeSettings_SetHalfExtent(@ptrCast(*c.JPC_BoxShapeSettings, box_shape_settings), &half_extent);
    }

    pub fn getConvexRadius(box_shape_settings: *const BoxShapeSettings) f32 {
        return c.JPC_BoxShapeSettings_GetConvexRadius(@ptrCast(*const c.JPC_BoxShapeSettings, box_shape_settings));
    }
    pub fn setConvexRadius(box_shape_settings: *BoxShapeSettings, convex_radius: f32) void {
        c.JPC_BoxShapeSettings_SetConvexRadius(
            @ptrCast(*c.JPC_BoxShapeSettings, box_shape_settings),
            convex_radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// Shape
//
//--------------------------------------------------------------------------------------------------
pub const Shape = opaque {
    pub usingnamespace Methods(@This());

    pub const Type = enum(c.JPC_ShapeType) {
        convex = c.JPC_SHAPE_TYPE_CONVEX,
        compound = c.JPC_SHAPE_TYPE_COMPOUND,
        decorated = c.JPC_SHAPE_TYPE_DECORATED,
        mesh = c.JPC_SHAPE_TYPE_MESH,
        height_field = c.JPC_SHAPE_TYPE_HEIGHT_FIELD,
        user1 = c.JPC_SHAPE_TYPE_USER1,
        user2 = c.JPC_SHAPE_TYPE_USER2,
        user3 = c.JPC_SHAPE_TYPE_USER3,
        user4 = c.JPC_SHAPE_TYPE_USER4,
    };

    pub const SubType = enum(c.JPC_ShapeSubType) {
        sphere = c.JPC_SHAPE_SUB_TYPE_SPHERE,
        box = c.JPC_SHAPE_SUB_TYPE_BOX,
        triangle = c.JPC_SHAPE_SUB_TYPE_TRIANGLE,
        capsule = c.JPC_SHAPE_SUB_TYPE_CAPSULE,
        tapered_capsule = c.JPC_SHAPE_SUB_TYPE_TAPERED_CAPSULE,
        cylinder = c.JPC_SHAPE_SUB_TYPE_CYLINDER,
        convex_hull = c.JPC_SHAPE_SUB_TYPE_CONVEX_HULL,
        static_compound = c.JPC_SHAPE_SUB_TYPE_STATIC_COMPOUND,
        mutable_compound = c.JPC_SHAPE_SUB_TYPE_MUTABLE_COMPOUND,
        rotated_translated = c.JPC_SHAPE_SUB_TYPE_ROTATED_TRANSLATED,
        scaled = c.JPC_SHAPE_SUB_TYPE_SCALED,
        offset_center_of_mass = c.JPC_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS,
        mesh = c.JPC_SHAPE_SUB_TYPE_MESH,
        height_field = c.JPC_SHAPE_SUB_TYPE_HEIGHT_FIELD,
        user1 = c.JPC_SHAPE_SUB_TYPE_USER1,
        user2 = c.JPC_SHAPE_SUB_TYPE_USER2,
        user3 = c.JPC_SHAPE_SUB_TYPE_USER3,
        user4 = c.JPC_SHAPE_SUB_TYPE_USER4,
        user5 = c.JPC_SHAPE_SUB_TYPE_USER5,
        user6 = c.JPC_SHAPE_SUB_TYPE_USER6,
        user7 = c.JPC_SHAPE_SUB_TYPE_USER7,
        user8 = c.JPC_SHAPE_SUB_TYPE_USER8,
    };

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asShape(shape: *const T) *const Shape {
                return @ptrCast(*const Shape, shape);
            }
            pub fn asShapeMut(shape: *T) *Shape {
                return @ptrCast(*Shape, shape);
            }

            pub fn addRef(shape: *T) void {
                c.JPC_Shape_AddRef(@ptrCast(*c.JPC_Shape, shape));
            }
            pub fn release(shape: *T) void {
                c.JPC_Shape_Release(@ptrCast(*c.JPC_Shape, shape));
            }
            pub fn getRefCount(shape: *const T) u32 {
                return c.JPC_Shape_GetRefCount(@ptrCast(*const c.JPC_Shape, shape));
            }

            pub fn getType(shape: *const T) Type {
                return @intToEnum(
                    Type,
                    c.JPC_Shape_GetType(@ptrCast(*const c.JPC_Shape, shape)),
                );
            }
            pub fn getSubType(shape: *const T) SubType {
                return @intToEnum(
                    SubType,
                    c.JPC_Shape_GetSubType(@ptrCast(*const c.JPC_Shape, shape)),
                );
            }

            pub fn getUserData(shape: *const T) u64 {
                return c.JPC_Shape_GetUserData(@ptrCast(*const c.JPC_Shape, shape));
            }
            pub fn setUserData(shape: *T, user_data: u64) void {
                return c.JPC_Shape_SetUserData(@ptrCast(*c.JPC_Shape, shape), user_data);
            }
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Memory allocation
//
//--------------------------------------------------------------------------------------------------
fn zphysicsAlloc(size: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const mem = mem_allocator.?.alignedAlloc(
        u8,
        mem_alignment,
        size,
    ) catch @panic("zphysics: out of memory");

    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch @panic("zphysics: out of memory");

    return mem.ptr;
}

fn zphysicsAlignedAlloc(size: usize, alignment: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const ptr = mem_allocator.?.rawAlloc(
        size,
        std.math.log2_int(u29, @intCast(u29, alignment)),
        @returnAddress(),
    );
    if (ptr == null) @panic("zphysics: out of memory");

    mem_allocations.?.put(@ptrToInt(ptr.?), size) catch @panic("zphysics: out of memory");

    return ptr;
}

fn zphysicsFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const size = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;
        const mem = @ptrCast([*]align(mem_alignment) u8, @alignCast(mem_alignment, ptr))[0..size];
        mem_allocator.?.free(mem);
    }
}
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zphysics.BodyCreationSettings" {
    const eql = std.mem.eql;
    const asBytes = std.mem.asBytes;
    const approxEql = std.math.approxEqAbs;

    const bcs0 = BodyCreationSettings{};
    const bcs1 = blk: {
        var settings: c.JPC_BodyCreationSettings = undefined;
        c.JPC_BodyCreationSettings_SetDefault(&settings);
        break :blk @ptrCast(*const BodyCreationSettings, &settings).*;
    };

    try expect(approxEql(Real, bcs0.position[0], bcs1.position[0], 0.0001));
    try expect(approxEql(Real, bcs0.position[1], bcs1.position[1], 0.0001));
    try expect(approxEql(Real, bcs0.position[2], bcs1.position[2], 0.0001));

    try expect(eql(u8, asBytes(&bcs0.rotation), asBytes(&bcs1.rotation)));

    try expect(approxEql(Real, bcs0.linear_velocity[0], bcs1.linear_velocity[0], 0.0001));
    try expect(approxEql(Real, bcs0.linear_velocity[1], bcs1.linear_velocity[1], 0.0001));
    try expect(approxEql(Real, bcs0.linear_velocity[2], bcs1.linear_velocity[2], 0.0001));

    try expect(approxEql(Real, bcs0.angular_velocity[0], bcs1.angular_velocity[0], 0.0001));
    try expect(approxEql(Real, bcs0.angular_velocity[1], bcs1.angular_velocity[1], 0.0001));
    try expect(approxEql(Real, bcs0.angular_velocity[2], bcs1.angular_velocity[2], 0.0001));

    try expect(bcs0.user_data == bcs1.user_data);
    try expect(bcs0.object_layer == bcs1.object_layer);
    try expect(eql(u8, asBytes(&bcs0.collision_group), asBytes(&bcs1.collision_group)));
    try expect(bcs0.motion_type == bcs1.motion_type);
    try expect(bcs0.allow_dynamic_or_kinematic == bcs1.allow_dynamic_or_kinematic);
    try expect(bcs0.is_sensor == bcs1.is_sensor);
    try expect(bcs0.motion_quality == bcs1.motion_quality);
    try expect(bcs0.allow_sleeping == bcs1.allow_sleeping);
    try expect(approxEql(f32, bcs0.friction, bcs1.friction, 0.0001));
    try expect(approxEql(f32, bcs0.restitution, bcs1.restitution, 0.0001));
    try expect(approxEql(f32, bcs0.linear_damping, bcs1.linear_damping, 0.0001));
    try expect(approxEql(f32, bcs0.angular_damping, bcs1.angular_damping, 0.0001));
    try expect(approxEql(f32, bcs0.max_linear_velocity, bcs1.max_linear_velocity, 0.0001));
    try expect(approxEql(f32, bcs0.max_angular_velocity, bcs1.max_angular_velocity, 0.0001));
    try expect(approxEql(f32, bcs0.gravity_factor, bcs1.gravity_factor, 0.0001));
    try expect(bcs0.override_mass_properties == bcs1.override_mass_properties);
    try expect(approxEql(f32, bcs0.inertia_multiplier, bcs1.inertia_multiplier, 0.0001));
    try expect(approxEql(f32, bcs0.mass_properties_override.mass, bcs1.mass_properties_override.mass, 0.0001));
    try expect(eql(
        u8,
        asBytes(&bcs0.mass_properties_override.inertia),
        asBytes(&bcs1.mass_properties_override.inertia),
    ));
    try expect(bcs0.reserved == bcs1.reserved);
    try expect(bcs0.shape == bcs1.shape);
}

test "zphysics.basic" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const broad_phase_layer_interface = test_cb1.BPLayerInterfaceImpl.init();

    const physics_system = try PhysicsSystem.create(
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
    _ = physics_system.getBodyInterfaceNoLock();
    _ = physics_system.getBodyInterfaceMut();
    _ = physics_system.getBodyInterfaceMutNoLock();
    _ = physics_system.getBodyLockInterface();
    _ = physics_system.getBodyLockInterfaceNoLock();

    physics_system.optimizeBroadPhase();
    physics_system.update(1.0 / 60.0, .{ .collision_steps = 1, .integration_sub_steps = 1 });
    physics_system.update(1.0 / 60.0, .{});

    var box_shape_settings: ?*BoxShapeSettings = null;
    box_shape_settings = try BoxShapeSettings.create(.{ 1.0, 2.0, 3.0 });
    defer {
        if (box_shape_settings) |bss| bss.release();
    }

    box_shape_settings.?.setDensity(2.0);
    try expect(box_shape_settings.?.getDensity() == 2.0);

    box_shape_settings.?.setUserData(123);
    try expect(box_shape_settings.?.getUserData() == 123);

    box_shape_settings.?.setConvexRadius(0.5);
    try expect(box_shape_settings.?.getConvexRadius() == 0.5);

    try expect(box_shape_settings.?.getRefCount() == 1);
    box_shape_settings.?.addRef();
    try expect(box_shape_settings.?.getRefCount() == 2);
    box_shape_settings.?.release();
    try expect(box_shape_settings.?.getRefCount() == 1);

    {
        var he = box_shape_settings.?.getHalfExtent();
        try expect(he[0] == 1.0 and he[1] == 2.0 and he[2] == 3.0);
        box_shape_settings.?.setHalfExtent(.{ 4.0, 5.0, 6.0 });
        he = box_shape_settings.?.getHalfExtent();
        try expect(he[0] == 4.0 and he[1] == 5.0 and he[2] == 6.0);
    }

    try expect(box_shape_settings.?.asConvexShapeSettings().getDensity() == 2.0);
    try expect(box_shape_settings.?.asShapeSettings().getRefCount() == 1);

    const box_shape = try box_shape_settings.?.createShape();
    defer box_shape.release();

    {
        const bs = try box_shape_settings.?.createShape();
        defer bs.release();
        try expect(bs == box_shape);
        try expect(bs.getRefCount() == 3);
    }

    try expect(box_shape.getRefCount() == 2);
    box_shape_settings.?.release();
    box_shape_settings = null;
    try expect(box_shape.getRefCount() == 1);

    try expect(box_shape.getType() == .convex);
    try expect(box_shape.getSubType() == .box);

    box_shape.setUserData(456);
    try expect(box_shape.getUserData() == 456);
}

test "zphysics.body.basic" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const broad_phase_layer_interface = test_cb1.BPLayerInterfaceImpl.init();

    const physics_system = try PhysicsSystem.create(
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

    const body_interface_mut = physics_system.getBodyInterfaceMut();
    const body_interface = physics_system.getBodyInterface();

    const floor_shape_settings = try BoxShapeSettings.create(.{ 100.0, 1.0, 100.0 });
    defer floor_shape_settings.release();

    const floor_shape = try floor_shape_settings.createShape();
    defer floor_shape.release();

    const floor_settings = BodyCreationSettings{
        .position = .{ 0.0, -1.0, 0.0, 1.0 },
        .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
        .shape = floor_shape,
        .motion_type = .static,
        .object_layer = test_cb1.layers.non_moving,
    };
    const body_id = try body_interface_mut.createAndAddBody(floor_settings, .dont_activate);
    defer {
        body_interface_mut.removeBody(body_id);
        body_interface_mut.destroyBody(body_id);
    }

    {
        var body_ids = std.ArrayList(BodyId).init(std.testing.allocator);
        defer body_ids.deinit();
        try physics_system.getBodyIds(&body_ids);
        try expect(body_ids.items.len == 1);
        try expect(body_ids.capacity >= physics_system.getMaxBodies());
        try expect(body_ids.items[0] == body_id);
    }

    {
        var body_ids = std.ArrayList(BodyId).init(std.testing.allocator);
        defer body_ids.deinit();
        try physics_system.getActiveBodyIds(&body_ids);
        try expect(body_ids.items.len == 0);
        try expect(body_ids.capacity >= physics_system.getMaxBodies());
    }

    {
        const lock_interface = physics_system.getBodyLockInterfaceNoLock();

        var read_lock: BodyLockRead = .{};
        read_lock.lock(lock_interface, body_id);
        defer read_lock.unlock();

        if (read_lock.body) |locked_body| {
            const all_bodies: []const *const Body = physics_system.getBodiesUnsafe();

            try expect(isValidBodyPointer(all_bodies[body_id & body_id_index_bits]));
            try expect(locked_body == all_bodies[body_id & body_id_index_bits]);
            try expect(locked_body.id == body_id);
            try expect(locked_body.id == all_bodies[body_id & body_id_index_bits].id);
        }
    }
    {
        const lock_interface = physics_system.getBodyLockInterface();
        var write_lock: BodyLockWrite = .{};
        write_lock.lock(lock_interface, body_id);

        if (write_lock.body) |locked_body| {
            defer write_lock.unlock();

            const all_bodies_mut: []const *Body = physics_system.getBodiesMutUnsafe();

            try expect(isValidBodyPointer(all_bodies_mut[body_id & body_id_index_bits]));
            try expect(locked_body == all_bodies_mut[body_id & body_id_index_bits]);
            try expect(locked_body.id == body_id);
            try expect(locked_body.id == all_bodies_mut[body_id & body_id_index_bits].id);

            all_bodies_mut[body_id & body_id_index_bits].user_data = 12345;
            try expect(all_bodies_mut[body_id & body_id_index_bits].user_data == 12345);
        }
    }

    try expect(physics_system.getNumBodies() == 1);
    try expect(physics_system.getNumActiveBodies() == 0);

    {
        const body1 = try body_interface_mut.createBody(floor_settings);
        defer body_interface_mut.destroyBody(body1.id);
        try expect(body_interface.isAdded(body1.getId()) == false);

        body_interface_mut.addBody(body1.getId(), .activate);
        try expect(body_interface_mut.isAdded(body1.getId()) == true);
        try expect(body_interface.isActive(body1.id) == false);

        body_interface_mut.removeBody(body1.getId());
        try expect(body_interface.isAdded(body1.id) == false);

        try expect(physics_system.getNumBodies() == 2);
        try expect(physics_system.getNumActiveBodies() == 0);
    }

    try expect(physics_system.getNumBodies() == 1);
    try expect(physics_system.getNumActiveBodies() == 0);
}

test {
    std.testing.refAllDecls(@This());
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
