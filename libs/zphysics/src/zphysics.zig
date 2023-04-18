pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 0, .patch = 5 };

const std = @import("std");
const assert = std.debug.assert;
const options = @import("zphysics_options");
const c = @cImport({
    if (options.use_double_precision) @cDefine("JPH_DOUBLE_PRECISION", "");
    if (options.enable_asserts) @cDefine("JPH_ENABLE_ASSERTS", "");
    if (options.enable_cross_platform_determinism) @cDefine("JPH_CROSS_PLATFORM_DETERMINISTIC", "");
    @cInclude("JoltPhysicsC.h");
});

pub const Real = c.JPC_Real;
comptime {
    assert(if (options.use_double_precision) Real == f64 else Real == f32);
}

pub const rvec_align = if (Real == f64) 32 else 16;

pub const flt_epsilon = c.JPC_FLT_EPSILON;

pub const Material = opaque {};
pub const GroupFilter = opaque {};
pub const BodyLockInterface = opaque {};
pub const SharedMutex = opaque {};

pub const BroadPhaseLayer = c.JPC_BroadPhaseLayer;
pub const ObjectLayer = c.JPC_ObjectLayer;
pub const BodyId = c.JPC_BodyID;
pub const SubShapeId = c.JPC_SubShapeID;

pub const max_physics_jobs = c.JPC_MAX_PHYSICS_JOBS;
pub const max_physics_barriers = c.JPC_MAX_PHYSICS_BARRIERS;

pub const body_id_invalid: BodyId = c.JPC_BODY_ID_INVALID;
pub const body_id_index_bits: BodyId = c.JPC_BODY_ID_INDEX_BITS;
pub const body_id_sequence_bits: BodyId = c.JPC_BODY_ID_SEQUENCE_BITS;
pub const body_id_sequence_shift: BodyId = c.JPC_BODY_ID_SEQUENCE_SHIFT;

pub const sub_shape_id_empty: SubShapeId = c.JPC_SUB_SHAPE_ID_EMPTY;

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

pub const BroadPhaseLayerInterface = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn getNumBroadPhaseLayers(self: *const T) u32 {
                return @ptrCast(*const BroadPhaseLayerInterface.VTable, self.__v)
                    .getNumBroadPhaseLayers(@ptrCast(*const BroadPhaseLayerInterface, self));
            }
            pub inline fn getBroadPhaseLayer(self: *const T, layer: ObjectLayer) u32 {
                return @ptrCast(*const BroadPhaseLayerInterface.VTable, self.__v)
                    .getBroadPhaseLayer(@ptrCast(*const BroadPhaseLayerInterface, self), layer);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        getNumBroadPhaseLayers: *const fn (self: *const BroadPhaseLayerInterface) callconv(.C) u32,
        getBroadPhaseLayer: *const fn (
            self: *const BroadPhaseLayerInterface,
            layer: ObjectLayer,
        ) callconv(.C) BroadPhaseLayer,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_BroadPhaseLayerInterfaceVTable));
        assert(@offsetOf(VTable, "getBroadPhaseLayer") == @offsetOf(
            c.JPC_BroadPhaseLayerInterfaceVTable,
            "GetBroadPhaseLayer",
        ));
    }
};

pub const ObjectVsBroadPhaseLayerFilter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn shouldCollide(self: *const T, layer1: ObjectLayer, layer2: BroadPhaseLayer) bool {
                return @ptrCast(*const ObjectVsBroadPhaseLayerFilter.VTable, self.__v)
                    .shouldCollide(@ptrCast(*const ObjectVsBroadPhaseLayerFilter, self), layer1, layer2);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        shouldCollide: *const fn (
            self: *const ObjectVsBroadPhaseLayerFilter,
            layer1: ObjectLayer,
            layer2: BroadPhaseLayer,
        ) callconv(.C) bool,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_ObjectVsBroadPhaseLayerFilterVTable));
        assert(@offsetOf(VTable, "shouldCollide") == @offsetOf(
            c.JPC_ObjectVsBroadPhaseLayerFilterVTable,
            "ShouldCollide",
        ));
    }
};

pub const BroadPhaseLayerFilter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn shouldCollide(self: *const T, layer: BroadPhaseLayer) bool {
                return @ptrCast(*const BroadPhaseLayerFilter.VTable, self.__v)
                    .shouldCollide(@ptrCast(*const BroadPhaseLayerFilter, self), layer);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        shouldCollide: *const fn (
            self: *const BroadPhaseLayerFilter,
            layer: BroadPhaseLayer,
        ) callconv(.C) bool,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_BroadPhaseLayerFilterVTable));
        assert(
            @offsetOf(VTable, "shouldCollide") == @offsetOf(c.JPC_BroadPhaseLayerFilterVTable, "ShouldCollide"),
        );
    }
};

pub const ObjectLayerPairFilter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn shouldCollide(self: *const T, layer1: ObjectLayer, layer2: ObjectLayer) bool {
                return @ptrCast(*const ObjectLayerPairFilter.VTable, self.__v)
                    .shouldCollide(@ptrCast(*const ObjectLayerPairFilter, self), layer1, layer2);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        shouldCollide: *const fn (self: *const ObjectLayerPairFilter, ObjectLayer, ObjectLayer) callconv(.C) bool,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_ObjectLayerPairFilterVTable));
        assert(
            @offsetOf(VTable, "shouldCollide") == @offsetOf(c.JPC_ObjectLayerPairFilterVTable, "ShouldCollide"),
        );
    }
};

pub const ObjectLayerFilter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn shouldCollide(self: *const T, layer: ObjectLayer) bool {
                return @ptrCast(*const ObjectLayerFilter.VTable, self.__v)
                    .shouldCollide(@ptrCast(*const ObjectLayerFilter, self), layer);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        shouldCollide: *const fn (self: *const ObjectLayerFilter, ObjectLayer) callconv(.C) bool,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_ObjectLayerFilterVTable));
        assert(@offsetOf(VTable, "shouldCollide") == @offsetOf(c.JPC_ObjectLayerFilterVTable, "ShouldCollide"));
    }
};

pub const BodyActivationListener = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn onBodyActivated(
                self: *T,
                body_id: *const BodyId,
                user_data: u64,
            ) void {
                @ptrCast(*const BodyActivationListener.VTable, self.__v)
                    .onBodyActivated(@ptrCast(*const BodyActivationListener, self), body_id, user_data);
            }
            pub inline fn onBodyDeactivated(
                self: *T,
                body_id: *const BodyId,
                user_data: u64,
            ) void {
                @ptrCast(*const BodyActivationListener.VTable, self.__v)
                    .onBodyDeactivated(@ptrCast(*const BodyActivationListener, self), body_id, user_data);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        onBodyActivated: *const fn (
            self: *BodyActivationListener,
            body_id: *const BodyId,
            user_data: u64,
        ) callconv(.C) void,
        onBodyDeactivated: *const fn (
            self: *BodyActivationListener,
            body_id: *const BodyId,
            user_data: u64,
        ) callconv(.C) void,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_BodyActivationListenerVTable));
        assert(@offsetOf(VTable, "onBodyDeactivated") == @offsetOf(
            c.JPC_BodyActivationListenerVTable,
            "OnBodyDeactivated",
        ));
    }
};

pub const ContactListener = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn onContactValidate(
                self: *T,
                body1: *const Body,
                body2: *const Body,
                base_offset: *const [3]Real,
                collision_result: *const CollideShapeResult,
            ) ValidateResult {
                return @ptrCast(*const ContactListener.VTable, self.__v)
                    .onContactValidate(
                    @ptrCast(*const ContactListener, self),
                    body1,
                    body2,
                    base_offset,
                    collision_result,
                );
            }
            pub inline fn onContactAdded(
                self: *T,
                body1: *const Body,
                body2: *const Body,
                manifold: *const ContactManifold,
                settings: *ContactSettings,
            ) void {
                @ptrCast(*const ContactListener.VTable, self.__v)
                    .onContactAdded(@ptrCast(*const ContactListener, self), body1, body2, manifold, settings);
            }
            pub inline fn onContactPersisted(
                self: *T,
                body1: *const Body,
                body2: *const Body,
                manifold: *const ContactManifold,
                settings: *ContactSettings,
            ) void {
                @ptrCast(*const ContactListener.VTable, self.__v)
                    .onContactPersisted(@ptrCast(*const ContactListener, self), body1, body2, manifold, settings);
            }
            pub inline fn onContactRemoved(
                self: *T,
                sub_shape_pair: *const SubShapeIdPair,
            ) void {
                @ptrCast(*const ContactListener.VTable, self.__v)
                    .onContactRemoved(@ptrCast(*const ContactListener, self), sub_shape_pair);
            }
        };
    }

    pub const VTable = extern struct {
        onContactValidate: ?*const fn (
            self: *ContactListener,
            body1: *const Body,
            body2: *const Body,
            base_offset: *const [3]Real,
            collision_result: *const CollideShapeResult,
        ) callconv(.C) ValidateResult = null,

        onContactAdded: ?*const fn (
            self: *ContactListener,
            body1: *const Body,
            body2: *const Body,
            manifold: *const ContactManifold,
            settings: *ContactSettings,
        ) callconv(.C) void = null,

        onContactPersisted: ?*const fn (
            self: *ContactListener,
            body1: *const Body,
            body2: *const Body,
            manifold: *const ContactManifold,
            settings: *ContactSettings,
        ) callconv(.C) void = null,

        onContactRemoved: ?*const fn (
            self: *ContactListener,
            sub_shape_pair: *const SubShapeIdPair,
        ) callconv(.C) void = null,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_ContactListenerVTable));
        assert(@offsetOf(VTable, "onContactAdded") == @offsetOf(
            c.JPC_ContactListenerVTable,
            "OnContactAdded",
        ));
        assert(
            @offsetOf(VTable, "onContactRemoved") == @offsetOf(c.JPC_ContactListenerVTable, "OnContactRemoved"),
        );
    }
};

pub const BodyFilter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn shouldCollide(self: *const T, body_id: *const BodyId) bool {
                return @ptrCast(*const BodyFilter.VTable, self.__v)
                    .shouldCollide(@ptrCast(*const BodyFilter, self), body_id);
            }
            pub inline fn shouldCollideLocked(self: *const T, body: *const Body) bool {
                return @ptrCast(*const BodyFilter.VTable, self.__v)
                    .shouldCollideLocked(@ptrCast(*const BodyFilter, self), body);
            }
        };
    }

    pub const VTable = extern struct {
        __unused0: ?*const anyopaque = null,
        __unused1: ?*const anyopaque = null,
        shouldCollide: *const fn (self: *const BodyFilter, body_id: *const BodyId) callconv(.C) bool,
        shouldCollideLocked: *const fn (self: *const BodyFilter, body: *const Body) callconv(.C) bool,
    };

    comptime {
        assert(@sizeOf(VTable) == @sizeOf(c.JPC_BodyFilterVTable));
        assert(@offsetOf(VTable, "shouldCollide") == @offsetOf(c.JPC_BodyFilterVTable, "ShouldCollide"));
        assert(
            @offsetOf(VTable, "shouldCollideLocked") == @offsetOf(c.JPC_BodyFilterVTable, "ShouldCollideLocked"),
        );
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
    use_manifold_reduction: bool = true,
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
        assert(@offsetOf(BodyCreationSettings, "motion_quality") ==
            @offsetOf(c.JPC_BodyCreationSettings, "motion_quality"));
    }
};

pub const RRayCast = extern struct {
    origin: [4]Real align(rvec_align), // 4th element is ignored
    direction: [4]f32 align(16), // 4th element is ignored

    comptime {
        assert(@sizeOf(RRayCast) == @sizeOf(c.JPC_RRayCast));
        assert(@offsetOf(RRayCast, "origin") == @offsetOf(c.JPC_RRayCast, "origin"));
        assert(@offsetOf(RRayCast, "direction") == @offsetOf(c.JPC_RRayCast, "direction"));
    }
};

pub const RayCastResult = extern struct {
    body_id: BodyId = body_id_invalid,
    fraction: f32 = 1.0 + flt_epsilon,
    sub_shape_id: SubShapeId = undefined,

    comptime {
        assert(@sizeOf(RayCastResult) == @sizeOf(c.JPC_RayCastResult));
        assert(@offsetOf(RayCastResult, "body_id") == @offsetOf(c.JPC_RayCastResult, "body_id"));
        assert(@offsetOf(RayCastResult, "fraction") == @offsetOf(c.JPC_RayCastResult, "fraction"));
        assert(@offsetOf(RayCastResult, "sub_shape_id") == @offsetOf(c.JPC_RayCastResult, "sub_shape_id"));
    }
};

pub const BackFaceMode = enum(c.JPC_BackFaceMode) {
    ignore_back_faces = c.JPC_BACK_FACE_IGNORE,
    collide_with_back_faces = c.JPC_BACK_FACE_COLLIDE,
};

pub const RayCastSettings = extern struct {
    back_face_mode: BackFaceMode,
    treat_convex_as_solid: bool,

    comptime {
        assert(@sizeOf(RayCastSettings) == @sizeOf(c.JPC_RayCastSettings));
        assert(
            @offsetOf(RayCastSettings, "back_face_mode") == @offsetOf(c.JPC_RayCastSettings, "back_face_mode"),
        );
        assert(@offsetOf(RayCastSettings, "treat_convex_as_solid") ==
            @offsetOf(c.JPC_RayCastSettings, "treat_convex_as_solid"));
    }
};
//--------------------------------------------------------------------------------------------------
//
// Init/deinit and global state
//
//--------------------------------------------------------------------------------------------------
const SizeAndAlignment = packed struct(u64) {
    size: u48,
    alignment: u16,
};
var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, SizeAndAlignment) = null;
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
    mem_allocations = std.AutoHashMap(usize, SizeAndAlignment).init(allocator);
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
        broad_phase_layer_interface: *const BroadPhaseLayerInterface,
        object_vs_broad_phase_layer_filter: *const ObjectVsBroadPhaseLayerFilter,
        object_layer_pair_filter: *const ObjectLayerPairFilter,
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

    pub fn getGravity(physics_system: *const PhysicsSystem) [3]f32 {
        var gravity: [3]f32 = undefined;
        c.JPC_PhysicsSystem_GetGravity(@ptrCast(*const c.JPC_PhysicsSystem, physics_system), &gravity);
        return gravity;
    }
    pub fn setGravity(physics_system: *PhysicsSystem, gravity: [3]f32) void {
        c.JPC_PhysicsSystem_SetGravity(@ptrCast(*c.JPC_PhysicsSystem, physics_system), &gravity);
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

    pub fn getNarrowPhaseQuery(physics_system: *const PhysicsSystem) *const NarrowPhaseQuery {
        return @ptrCast(
            *const NarrowPhaseQuery,
            c.JPC_PhysicsSystem_GetNarrowPhaseQuery(@ptrCast(*const c.JPC_PhysicsSystem, physics_system)),
        );
    }
    pub fn getNarrowPhaseQueryNoLock(physics_system: *const PhysicsSystem) *const NarrowPhaseQuery {
        return @ptrCast(
            *const NarrowPhaseQuery,
            c.JPC_PhysicsSystem_GetNarrowPhaseQueryNoLock(@ptrCast(*const c.JPC_PhysicsSystem, physics_system)),
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
        c.JPC_BodyLockInterface_LockRead(
            @ptrCast(*const c.JPC_BodyLockInterface, lock_interface),
            body_id,
            @ptrCast(*c.JPC_BodyLockRead, read_lock),
        );
    }

    pub fn unlock(read_lock: *BodyLockRead) void {
        c.JPC_BodyLockInterface_UnlockRead(
            @ptrCast(*const c.JPC_BodyLockInterface, read_lock.lock_interface),
            @ptrCast(*c.JPC_BodyLockRead, read_lock),
        );
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
        c.JPC_BodyLockInterface_LockWrite(
            @ptrCast(*const c.JPC_BodyLockInterface, lock_interface),
            body_id,
            @ptrCast(*c.JPC_BodyLockWrite, write_lock),
        );
    }

    pub fn unlock(write_lock: *BodyLockWrite) void {
        c.JPC_BodyLockInterface_UnlockWrite(
            @ptrCast(*const c.JPC_BodyLockInterface, write_lock.lock_interface),
            @ptrCast(*c.JPC_BodyLockWrite, write_lock),
        );
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

    pub fn removeAndDestroyBody(body_iface: *BodyInterface, body_id: BodyId) void {
        body_iface.removeBody(body_id);
        body_iface.destroyBody(body_id);
    }

    pub fn isAdded(body_iface: *const BodyInterface, body_id: BodyId) bool {
        return c.JPC_BodyInterface_IsAdded(@ptrCast(*const c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn isActive(body_iface: *const BodyInterface, body_id: BodyId) bool {
        return c.JPC_BodyInterface_IsActive(@ptrCast(*const c.JPC_BodyInterface, body_iface), body_id);
    }

    pub fn setLinearAndAngularVelocity(
        body_iface: *BodyInterface,
        body_id: BodyId,
        linear_velocity: [3]f32,
        angular_velocity: [3]f32,
    ) void {
        return c.JPC_BodyInterface_SetLinearAndAngularVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &linear_velocity,
            &angular_velocity,
        );
    }
    pub fn getLinearAndAngularVelocity(
        body_iface: *const BodyInterface,
        body_id: BodyId,
    ) struct { linear: [3]f32, angular: [3]f32 } {
        var linear: [3]f32 = undefined;
        var angular: [3]f32 = undefined;
        c.JPC_BodyInterface_GetLinearAndAngularVelocity(
            @ptrCast(*const c.JPC_BodyInterface, body_iface),
            body_id,
            &linear,
            &angular,
        );
        return .{ .linear = linear, .angular = angular };
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

    pub fn addLinearVelocity(body_iface: *BodyInterface, body_id: BodyId, velocity: [3]f32) void {
        return c.JPC_BodyInterface_AddLinearVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &velocity,
        );
    }

    pub fn addLinearAndAngularVelocity(
        body_iface: *BodyInterface,
        body_id: BodyId,
        linear_velocity: [3]f32,
        angular_velocity: [3]f32,
    ) void {
        return c.JPC_BodyInterface_AddLinearAndAngularVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &linear_velocity,
            &angular_velocity,
        );
    }

    pub fn setAngularVelocity(body_iface: *BodyInterface, body_id: BodyId, velocity: [3]f32) void {
        return c.JPC_BodyInterface_SetAngularVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &velocity,
        );
    }
    pub fn getAngularVelocity(body_iface: *const BodyInterface, body_id: BodyId) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_BodyInterface_GetAngularVelocity(
            @ptrCast(*const c.JPC_BodyInterface, body_iface),
            body_id,
            &velocity,
        );
        return velocity;
    }

    pub fn getPointVelocity(body_iface: *const BodyInterface, body_id: BodyId, point: [3]Real) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_BodyInterface_GetPointVelocity(
            @ptrCast(*const c.JPC_BodyInterface, body_iface),
            body_id,
            &point,
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

    pub fn setPositionRotationAndVelocity(
        body_iface: *BodyInterface,
        body_id: BodyId,
        position: [3]Real,
        rotation: [4]f32,
        linear_velocity: [3]f32,
        angular_velocity: [3]f32,
    ) void {
        return c.JPC_BodyInterface_SetPositionRotationAndVelocity(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &position,
            &rotation,
            &linear_velocity,
            &angular_velocity,
        );
    }

    pub fn addForce(body_iface: *BodyInterface, body_id: BodyId, force: [3]f32) void {
        return c.JPC_BodyInterface_AddForce(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &force,
        );
    }
    pub fn addForceAtPosition(body_iface: *BodyInterface, body_id: BodyId, force: [3]f32, position: [3]Real) void {
        return c.JPC_BodyInterface_AddForceAtPosition(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &force,
            &position,
        );
    }

    pub fn addTorque(body_iface: *BodyInterface, body_id: BodyId, torque: [3]f32) void {
        return c.JPC_BodyInterface_AddTorque(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &torque,
        );
    }
    pub fn addForceAndTorque(body_iface: *BodyInterface, body_id: BodyId, force: [3]f32, torque: [3]f32) void {
        return c.JPC_BodyInterface_AddForceAndTorque(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &force,
            &torque,
        );
    }

    pub fn addImpulse(body_iface: *BodyInterface, body_id: BodyId, impulse: [3]f32) void {
        return c.JPC_BodyInterface_AddImpulse(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &impulse,
        );
    }
    pub fn addImpulseAtPosition(
        body_iface: *BodyInterface,
        body_id: BodyId,
        impulse: [3]f32,
        position: [3]Real,
    ) void {
        return c.JPC_BodyInterface_AddImpulseAtPosition(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &impulse,
            &position,
        );
    }

    pub fn addAngularImpulse(body_iface: *BodyInterface, body_id: BodyId, impulse: [3]f32) void {
        return c.JPC_BodyInterface_AddAngularImpulse(
            @ptrCast(*c.JPC_BodyInterface, body_iface),
            body_id,
            &impulse,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// NarrowPhaseQuery
//
//--------------------------------------------------------------------------------------------------
pub const NarrowPhaseQuery = opaque {
    pub fn castRay(
        query: *const NarrowPhaseQuery,
        ray: RRayCast,
        args: struct {
            broad_phase_layer_filter: ?*const BroadPhaseLayerFilter = null,
            object_layer_filter: ?*const ObjectLayerFilter = null,
            body_filter: ?*const BodyFilter = null,
        },
    ) struct { has_hit: bool, hit: RayCastResult } {
        var hit: RayCastResult = .{};
        const has_hit = c.JPC_NarrowPhaseQuery_CastRay(
            @ptrCast(*const c.JPC_NarrowPhaseQuery, query),
            @ptrCast(*const c.JPC_RRayCast, &ray),
            @ptrCast(*c.JPC_RayCastResult, &hit),
            args.broad_phase_layer_filter,
            args.object_layer_filter,
            args.body_filter,
        );
        return .{ .has_hit = has_hit, .hit = hit };
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

    pub fn getMotionType(body: *const Body) MotionType {
        return @intToEnum(MotionType, c.JPC_Body_GetMotionType(@ptrCast(*const c.JPC_Body, body)));
    }
    pub fn setMotionType(body: *Body, motion_type: MotionType) void {
        return c.JPC_Body_SetMotionType(@ptrCast(*c.JPC_Body, body), @enumToInt(motion_type));
    }

    pub fn getBroadPhaseLayer(body: *const Body) BroadPhaseLayer {
        return c.JPC_Body_GetBroadPhaseLayer(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn getObjectLayer(body: *const Body) ObjectLayer {
        return c.JPC_Body_GetObjectLayer(@ptrCast(*const c.JPC_Body, body));
    }

    pub fn getCollisionGroup(body: *const Body) *const CollisionGroup {
        return @ptrCast(
            *const CollisionGroup,
            c.JPC_Body_GetCollisionGroup(@intToPtr(*c.JPC_Body, @ptrToInt(body))),
        );
    }
    pub fn getCollisionGroupMut(body: *Body) *CollisionGroup {
        return @ptrCast(
            *CollisionGroup,
            c.JPC_Body_GetCollisionGroup(@ptrCast(*c.JPC_Body, body)),
        );
    }
    pub fn setCollisionGroup(body: *Body, group: CollisionGroup) void {
        c.JPC_Body_SetCollisionGroup(
            @ptrCast(*c.JPC_Body, body),
            @ptrCast(*const c.JPC_CollisionGroup, &group),
        );
    }

    pub fn getAllowSleeping(body: *const Body) bool {
        return c.JPC_Body_GetAllowSleeping(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn setAllowSleeping(body: *Body, allow: bool) void {
        c.JPC_Body_SetAllowSleeping(@ptrCast(*c.JPC_Body, body), allow);
    }

    pub fn getFriction(body: *const Body) f32 {
        return c.JPC_Body_GetFriction(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn setFriction(body: *Body, friction: f32) void {
        c.JPC_Body_SetFriction(@ptrCast(*c.JPC_Body, body), friction);
    }

    pub fn getRestitution(body: *const Body) f32 {
        return c.JPC_Body_GetRestitution(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn setRestitution(body: *Body, restitution: f32) void {
        c.JPC_Body_SetRestitution(@ptrCast(*c.JPC_Body, body), restitution);
    }

    pub fn getLinearVelocity(body: *const Body) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_Body_GetLinearVelocity(@ptrCast(*const c.JPC_Body, body), &velocity);
        return velocity;
    }
    pub fn setLinearVelocity(body: *Body, velocity: [3]f32) void {
        c.JPC_Body_SetLinearVelocity(@ptrCast(*c.JPC_Body, body), &velocity);
    }
    pub fn setLinearVelocityClamped(body: *Body, velocity: [3]f32) void {
        c.JPC_Body_SetLinearVelocityClamped(@ptrCast(*c.JPC_Body, body), &velocity);
    }

    pub fn getAngularVelocity(body: *const Body) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_Body_GetAngularVelocity(@ptrCast(*const c.JPC_Body, body), &velocity);
        return velocity;
    }
    pub fn setAngularVelocity(body: *Body, velocity: [3]f32) void {
        c.JPC_Body_SetAnglularVelocity(@ptrCast(*c.JPC_Body, body), &velocity);
    }
    pub fn setAngularVelocityClamped(body: *Body, velocity: [3]f32) void {
        c.JPC_Body_SetAnglularVelocityClamped(@ptrCast(*c.JPC_Body, body), &velocity);
    }

    /// `point` is relative to the center of mass (com)
    pub fn getPointVelocityCom(body: *const Body, point: [3]f32) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_Body_GetPointVelocityCOM(@ptrCast(*const c.JPC_Body, body), &point, &velocity);
        return velocity;
    }
    /// `point` is in the world space
    pub fn getPointVelocity(body: *const Body, point: [3]Real) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_Body_GetPointVelocity(@ptrCast(*const c.JPC_Body, body), &point, &velocity);
        return velocity;
    }

    pub fn addForce(body: *Body, force: [3]f32) void {
        c.JPC_Body_AddForce(@ptrCast(*c.JPC_Body, body), &force);
    }
    pub fn addForceAtPosition(body: *Body, force: [3]f32, position: [3]Real) void {
        c.JPC_Body_AddForceAtPosition(@ptrCast(*c.JPC_Body, body), &force, &position);
    }

    pub fn addTorque(body: *Body, torque: [3]f32) void {
        c.JPC_Body_AddTorque(@ptrCast(*c.JPC_Body, body), &torque);
    }

    pub fn getInverseInertia(body: *const Body) [16]f32 {
        var inverse_inertia: [16]f32 = undefined;
        c.JPC_Body_GetInverseInertia(@ptrCast(*const c.JPC_Body, body), &inverse_inertia);
        return inverse_inertia;
    }

    pub fn addImpulse(body: *Body, impulse: [3]f32) void {
        c.JPC_Body_AddImpulse(@ptrCast(*c.JPC_Body, body), &impulse);
    }
    pub fn addImpulseAtPosition(body: *Body, impulse: [3]f32, position: [3]Real) void {
        c.JPC_Body_AddImpulseAtPosition(@ptrCast(*c.JPC_Body, body), &impulse, &position);
    }

    pub fn addAngularImpulse(body: *Body, impulse: [3]f32) void {
        c.JPC_Body_AddAngularImpulse(@ptrCast(*c.JPC_Body, body), &impulse);
    }

    pub fn moveKinematic(
        body: *Body,
        target_position: [3]Real,
        target_rotation: [4]f32,
        delta_time: f32,
    ) void {
        c.JPC_Body_MoveKinematic(
            @ptrCast(*c.JPC_Body, body),
            &target_position,
            &target_rotation,
            delta_time,
        );
    }

    pub fn applyBuoyancyImpulse(
        body: *Body,
        surface_position: [3]Real,
        surface_normal: [3]f32,
        buoyancy: f32,
        linear_drag: f32,
        angular_drag: f32,
        fluid_velocity: [3]f32,
        gravity: [3]f32,
        delta_time: f32,
    ) void {
        c.JPC_Body_ApplyBuoyancyImpulse(
            @ptrCast(*c.JPC_Body, body),
            &surface_position,
            &surface_normal,
            buoyancy,
            linear_drag,
            angular_drag,
            &fluid_velocity,
            &gravity,
            delta_time,
        );
    }

    pub fn isInBroadPhase(body: *const Body) bool {
        return c.JPC_Body_IsInBroadPhase(@ptrCast(*const c.JPC_Body, body));
    }

    pub fn isCollisionCacheInvalid(body: *const Body) bool {
        return c.JPC_Body_IsCollisionCacheInvalid(@ptrCast(*const c.JPC_Body, body));
    }

    pub fn getShape(body: *const Body) *const Shape {
        return @ptrCast(*const Shape, c.JPC_Body_GetShape(@ptrCast(*const c.JPC_Body, body)));
    }

    pub fn getPosition(body: *const Body) [3]Real {
        var position: [3]Real = undefined;
        c.JPC_Body_GetPosition(@ptrCast(*const c.JPC_Body, body), &position);
        return position;
    }

    pub fn getRotation(body: *const Body) [4]f32 {
        var rotation: [4]f32 = undefined;
        c.JPC_Body_GetRotation(@ptrCast(*const c.JPC_Body, body), &rotation);
        return rotation;
    }

    pub fn getWorldTransform(body: *const Body) struct {
        rotation: [9]f32,
        position: [3]Real,
    } {
        var rotation: [9]f32 = undefined;
        var position: [3]Real = undefined;
        c.JPC_Body_GetWorldTransform(@ptrCast(*const c.JPC_Body, body), &rotation, &position);
        return .{ .rotation = rotation, .position = position };
    }

    pub fn getCenterOfMassPosition(body: *const Body) [3]Real {
        var position: [3]Real = undefined;
        c.JPC_Body_GetCenterOfMassPosition(@ptrCast(*const c.JPC_Body, body), &position);
        return position;
    }

    pub fn getCenterOfMassTransform(body: *const Body) struct {
        rotation: [9]f32,
        position: [3]Real,
    } {
        var rotation: [9]f32 = undefined;
        var position: [3]Real = undefined;
        c.JPC_Body_GetCenterOfMassTransform(@ptrCast(*const c.JPC_Body, body), &rotation, &position);
        return .{ .rotation = rotation, .position = position };
    }

    pub fn getInverseCenterOfMassTransform(body: *const Body) struct {
        rotation: [9]f32,
        position: [3]Real,
    } {
        var rotation: [9]f32 = undefined;
        var position: [3]Real = undefined;
        c.JPC_Body_GetInverseCenterOfMassTransform(@ptrCast(*const c.JPC_Body, body), &rotation, &position);
        return .{ .rotation = rotation, .position = position };
    }

    pub fn getWorldSpaceBounds(body: *const Body) struct {
        min: [3]f32,
        max: [3]f32,
    } {
        var min: [3]f32 = undefined;
        var max: [3]f32 = undefined;
        c.JPC_Body_GetWorldSpaceBounds(@ptrCast(*const c.JPC_Body, body), &min, &max);
        return .{ .min = min, .max = max };
    }

    pub fn getMotionProperties(body: *const Body) *const MotionProperties {
        return @ptrCast(
            *const MotionProperties,
            c.JPC_Body_GetMotionProperties(@intToPtr(*c.JPC_Body, @ptrToInt(body))),
        );
    }
    pub fn getMotionPropertiesMut(body: *Body) *MotionProperties {
        return @ptrCast(
            *MotionProperties,
            c.JPC_Body_GetMotionProperties(@ptrCast(*c.JPC_Body, body)),
        );
    }

    pub fn getUserData(body: *const Body) u64 {
        return c.JPC_Body_GetUserData(@ptrCast(*const c.JPC_Body, body));
    }
    pub fn setUserData(body: *Body, user_data: u64) void {
        return c.JPC_Body_SetUserData(@ptrCast(*c.JPC_Body, body), user_data);
    }

    pub fn getWorldSpaceSurfaceNormal(
        body: *const Body,
        sub_shape_id: SubShapeId,
        position: [3]Real, // world space
    ) [3]f32 {
        var normal: [3]f32 = undefined;
        c.JPC_Body_GetWorldSpaceSurfaceNormal(
            @ptrCast(*const c.JPC_Body, body),
            sub_shape_id,
            &position,
            &normal,
        );
        return normal;
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

    pub fn getMotionQuality(motion: *const MotionProperties) MotionQuality {
        return @intToEnum(MotionQuality, c.JPC_MotionProperties_GetMotionQuality(
            @ptrCast(*const c.JPC_MotionProperties, motion),
        ));
    }

    pub fn getLinearVelocity(motion: *const MotionProperties) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_MotionProperties_GetLinearVelocity(@ptrCast(*const c.JPC_MotionProperties, motion), &velocity);
        return velocity;
    }
    pub fn setLinearVelocity(motion: *MotionProperties, velocity: [3]f32) void {
        c.JPC_MotionProperties_SetLinearVelocity(@ptrCast(*c.JPC_MotionProperties, motion), &velocity);
    }
    pub fn setLinearVelocityClamped(motion: *MotionProperties, velocity: [3]f32) void {
        c.JPC_MotionProperties_SetLinearVelocityClamped(@ptrCast(*c.JPC_MotionProperties, motion), &velocity);
    }

    pub fn getAngularVelocity(motion: *const MotionProperties) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_MotionProperties_GetAngularVelocity(@ptrCast(*const c.JPC_MotionProperties, motion), &velocity);
        return velocity;
    }
    pub fn setAngularVelocity(motion: *MotionProperties, velocity: [3]f32) void {
        c.JPC_MotionProperties_SetAnglularVelocity(@ptrCast(*c.JPC_MotionProperties, motion), &velocity);
    }
    pub fn setAngularVelocityClamped(motion: *MotionProperties, velocity: [3]f32) void {
        c.JPC_MotionProperties_SetAnglularVelocityClamped(@ptrCast(*c.JPC_MotionProperties, motion), &velocity);
    }

    /// `point` is relative to the center of mass (com)
    pub fn getPointVelocityCom(motion: *const MotionProperties, point: [3]f32) [3]f32 {
        var velocity: [3]f32 = undefined;
        c.JPC_MotionProperties_GetPointVelocityCOM(
            @ptrCast(*const c.JPC_MotionProperties, motion),
            &point,
            &velocity,
        );
        return velocity;
    }

    pub fn getMaxLinearVelocity(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetMaxLinearVelocity(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setMaxLinearVelocity(motion: *MotionProperties, velocity: f32) void {
        c.JPC_MotionProperties_SetMaxLinearVelocity(@ptrCast(*c.JPC_MotionProperties, motion), velocity);
    }

    pub fn getMaxAngularVelocity(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetMaxAngularVelocity(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setMaxAngularVelocity(motion: *MotionProperties, velocity: f32) void {
        c.JPC_MotionProperties_SetMaxAngularVelocity(@ptrCast(*c.JPC_MotionProperties, motion), velocity);
    }

    pub fn moveKinematic(
        motion: *MotionProperties,
        delta_position: [3]f32,
        delta_rotation: [4]f32,
        delta_time: f32,
    ) void {
        c.JPC_MotionProperties_MoveKinematic(
            @ptrCast(*c.JPC_MotionProperties, motion),
            &delta_position,
            &delta_rotation,
            delta_time,
        );
    }

    pub fn clampLinearVelocity(motion: *MotionProperties) void {
        c.JPC_MotionProperties_ClampLinearVelocity(@ptrCast(*c.JPC_MotionProperties, motion));
    }
    pub fn clampAngularVelocity(motion: *MotionProperties) void {
        c.JPC_MotionProperties_ClampAngularVelocity(@ptrCast(*c.JPC_MotionProperties, motion));
    }

    pub fn getLinearDamping(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetLinearDamping(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setLinearDamping(motion: *MotionProperties, damping: f32) void {
        c.JPC_MotionProperties_SetLinearDamping(@ptrCast(*c.JPC_MotionProperties, motion), damping);
    }

    pub fn getAngularDamping(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetAngularDamping(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setAngularDamping(motion: *MotionProperties, damping: f32) void {
        c.JPC_MotionProperties_SetAngularDamping(@ptrCast(*c.JPC_MotionProperties, motion), damping);
    }

    pub fn getGravityFactor(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetGravityFactor(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setGravityFactor(motion: *MotionProperties, factor: f32) void {
        c.JPC_MotionProperties_SetGravityFactor(@ptrCast(*c.JPC_MotionProperties, motion), factor);
    }

    pub fn setMassProperties(motion: *MotionProperties, mass_properties: MassProperties) void {
        c.JPC_MotionProperties_SetMassProperties(
            @ptrCast(*c.JPC_MotionProperties, motion),
            @ptrCast(*const c.JPC_MassProperties, &mass_properties),
        );
    }

    pub fn getInverseMass(motion: *const MotionProperties) f32 {
        return c.JPC_MotionProperties_GetInverseMass(@ptrCast(*const c.JPC_MotionProperties, motion));
    }
    pub fn setInverseMass(motion: *MotionProperties, inverse_mass: f32) void {
        c.JPC_MotionProperties_SetInverseMass(@ptrCast(*c.JPC_MotionProperties, motion), inverse_mass);
    }

    pub fn getInverseInertiaDiagonal(motion: *const MotionProperties) [3]f32 {
        var diagonal: [3]f32 = undefined;
        c.JPC_MotionProperties_GetInverseInertiaDiagonal(
            @ptrCast(*const c.JPC_MotionProperties, motion),
            &diagonal,
        );
        return diagonal;
    }

    pub fn getInertiaRotation(motion: *const MotionProperties) [4]f32 {
        var rotation: [4]f32 = undefined;
        c.JPC_MotionProperties_GetInertiaRotation(@ptrCast(*const c.JPC_MotionProperties, motion), &rotation);
        return rotation;
    }

    pub fn setInverseInertia(motion: *MotionProperties, diagonal: [3]f32, rotation: [4]f32) void {
        c.JPC_MotionProperties_SetInverseInertia(@ptrCast(*c.JPC_MotionProperties, motion), &diagonal, &rotation);
    }

    pub fn getLocalSpaceInverseInertia(motion: *const MotionProperties) [16]f32 {
        var inertia: [16]f32 = undefined;
        c.JPC_MotionProperties_GetLocalSpaceInverseInertia(
            @ptrCast(*const c.JPC_MotionProperties, motion),
            &inertia,
        );
        return inertia;
    }

    pub fn getInverseInertiaForRotation(motion: *const MotionProperties, rotation_matrix: [16]f32) [16]f32 {
        var inertia: [16]f32 = undefined;
        c.JPC_MotionProperties_GetInverseInertiaForRotation(
            @ptrCast(*const c.JPC_MotionProperties, motion),
            &rotation_matrix,
            &inertia,
        );
        return inertia;
    }

    pub fn multiplyWorldSpaceInverseInertiaByVector(
        motion: *const MotionProperties,
        rotation: [4]f32,
        vector: [3]f32,
    ) [3]f32 {
        var out: [3]f32 = undefined;
        c.JPC_MotionProperties_MultiplyWorldSpaceInverseInertiaByVector(
            @ptrCast(*const c.JPC_MotionProperties, motion),
            &rotation,
            &vector,
            &out,
        );
        return out;
    }

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
// SphereShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const SphereShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(radius: f32) !*SphereShapeSettings {
        const sphere_shape_settings = c.JPC_SphereShapeSettings_Create(radius);
        if (sphere_shape_settings == null)
            return error.FailedToCreateSphereShapeSettings;
        return @ptrCast(*SphereShapeSettings, sphere_shape_settings);
    }

    pub fn getRadius(sphere_shape_settings: *const SphereShapeSettings) f32 {
        return c.JPC_SphereShapeSettings_GetRadius(
            @ptrCast(*const c.JPC_SphereShapeSettings, sphere_shape_settings),
        );
    }
    pub fn setRadius(sphere_shape_settings: *SphereShapeSettings, radius: f32) void {
        c.JPC_SphereShapeSettings_SetRadius(
            @ptrCast(*c.JPC_SphereShapeSettings, sphere_shape_settings),
            radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// TriangleShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const TriangleShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(v1: [3]f32, v2: [3]f32, v3: [3]f32) !*TriangleShapeSettings {
        const triangle_shape_settings = c.JPC_TriangleShapeSettings_Create(&v1, &v2, &v3);
        if (triangle_shape_settings == null)
            return error.FailedToCreateTriangleShapeSettings;
        return @ptrCast(*TriangleShapeSettings, triangle_shape_settings);
    }

    pub fn getConvexRadius(triangle_shape_settings: *const TriangleShapeSettings) f32 {
        return c.JPC_TriangleShapeSettings_GetConvexRadius(
            @ptrCast(*const c.JPC_TriangleShapeSettings, triangle_shape_settings),
        );
    }
    pub fn setConvexRadius(triangle_shape_settings: *TriangleShapeSettings, convex_radius: f32) void {
        c.JPC_TriangleShapeSettings_SetConvexRadius(
            @ptrCast(*c.JPC_TriangleShapeSettings, triangle_shape_settings),
            convex_radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// CapsuleShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const CapsuleShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(half_height: f32, radius: f32) !*CapsuleShapeSettings {
        const capsule_shape_settings = c.JPC_CapsuleShapeSettings_Create(half_height, radius);
        if (capsule_shape_settings == null)
            return error.FailedToCreateCapsuleShapeSettings;
        return @ptrCast(*CapsuleShapeSettings, capsule_shape_settings);
    }

    pub fn getHalfHeight(capsule_shape_settings: *const CapsuleShapeSettings) f32 {
        return c.JPC_CapsuleShapeSettings_GetHalfHeight(
            @ptrCast(*const c.JPC_CapsuleShapeSettings, capsule_shape_settings),
        );
    }
    pub fn setHalfHeight(capsule_shape_settings: *CapsuleShapeSettings, half_height: f32) void {
        c.JPC_CapsuleShapeSettings_SetHalfHeight(
            @ptrCast(*c.JPC_CapsuleShapeSettings, capsule_shape_settings),
            half_height,
        );
    }

    pub fn getRadius(capsule_shape_settings: *const CapsuleShapeSettings) f32 {
        return c.JPC_CapsuleShapeSettings_GetRadius(
            @ptrCast(*const c.JPC_CapsuleShapeSettings, capsule_shape_settings),
        );
    }
    pub fn setRadius(capsule_shape_settings: *CapsuleShapeSettings, radius: f32) void {
        c.JPC_CapsuleShapeSettings_SetRadius(
            @ptrCast(*c.JPC_CapsuleShapeSettings, capsule_shape_settings),
            radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// TaperedCapsuleShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const TaperedCapsuleShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(half_height: f32, top_radius: f32, bottom_radius: f32) !*TaperedCapsuleShapeSettings {
        const capsule_shape_settings = c.JPC_TaperedCapsuleShapeSettings_Create(
            half_height,
            top_radius,
            bottom_radius,
        );
        if (capsule_shape_settings == null)
            return error.FailedToCreateTaperedCapsuleShapeSettings;
        return @ptrCast(*TaperedCapsuleShapeSettings, capsule_shape_settings);
    }

    pub fn getHalfHeight(capsule_shape_settings: *const TaperedCapsuleShapeSettings) f32 {
        return c.JPC_TaperedCapsuleShapeSettings_GetHalfHeight(
            @ptrCast(*const c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
        );
    }
    pub fn setHalfHeight(capsule_shape_settings: *TaperedCapsuleShapeSettings, half_height: f32) void {
        c.JPC_CapsuleShapeSettings_SetHalfHeight(
            @ptrCast(*c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
            half_height,
        );
    }

    pub fn getTopRadius(capsule_shape_settings: *const TaperedCapsuleShapeSettings) f32 {
        return c.JPC_TaperedCapsuleShapeSettings_GetTopRadius(
            @ptrCast(*const c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
        );
    }
    pub fn setTopRadius(capsule_shape_settings: *TaperedCapsuleShapeSettings, radius: f32) void {
        c.JPC_TaperedCapsuleShapeSettings_SetTopRadius(
            @ptrCast(*c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
            radius,
        );
    }

    pub fn getBottomRadius(capsule_shape_settings: *const TaperedCapsuleShapeSettings) f32 {
        return c.JPC_TaperedCapsuleShapeSettings_GetBottomRadius(
            @ptrCast(*const c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
        );
    }
    pub fn setBottomRadius(capsule_shape_settings: *TaperedCapsuleShapeSettings, radius: f32) void {
        c.JPC_TaperedCapsuleShapeSettings_SetBottomRadius(
            @ptrCast(*c.JPC_TaperedCapsuleShapeSettings, capsule_shape_settings),
            radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// CylinderShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const CylinderShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(half_height: f32, radius: f32) !*CylinderShapeSettings {
        const cylinder_shape_settings = c.JPC_CylinderShapeSettings_Create(half_height, radius);
        if (cylinder_shape_settings == null)
            return error.FailedToCreateCylinderShapeSettings;
        return @ptrCast(*CylinderShapeSettings, cylinder_shape_settings);
    }

    pub fn getConvexRadius(cylinder_shape_settings: *const CylinderShapeSettings) f32 {
        return c.JPC_CylinderShapeSettings_GetConvexRadius(
            @ptrCast(*const c.JPC_CylinderShapeSettings, cylinder_shape_settings),
        );
    }
    pub fn setConvexRadius(cylinder_shape_settings: *CylinderShapeSettings, convex_radius: f32) void {
        c.JPC_CylinderShapeSettings_SetConvexRadius(
            @ptrCast(*c.JPC_CylinderShapeSettings, cylinder_shape_settings),
            convex_radius,
        );
    }

    pub fn getHalfHeight(cylinder_shape_settings: *const CylinderShapeSettings) f32 {
        return c.JPC_CylinderShapeSettings_GetHalfHeight(
            @ptrCast(*const c.JPC_CylinderShapeSettings, cylinder_shape_settings),
        );
    }
    pub fn setHalfHeight(cylinder_shape_settings: *CylinderShapeSettings, half_height: f32) void {
        c.JPC_CylinderShapeSettings_SetHalfHeight(
            @ptrCast(*c.JPC_CylinderShapeSettings, cylinder_shape_settings),
            half_height,
        );
    }

    pub fn getRadius(cylinder_shape_settings: *const CylinderShapeSettings) f32 {
        return c.JPC_CylinderShapeSettings_GetRadius(
            @ptrCast(*const c.JPC_CylinderShapeSettings, cylinder_shape_settings),
        );
    }
    pub fn setRadius(cylinder_shape_settings: *CylinderShapeSettings, radius: f32) void {
        c.JPC_CylinderShapeSettings_SetRadius(
            @ptrCast(*c.JPC_CylinderShapeSettings, cylinder_shape_settings),
            radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// ConvexHullShapeSettings (-> ConvexShapeSettings -> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const ConvexHullShapeSettings = opaque {
    pub usingnamespace ConvexShapeSettings.Methods(@This());

    pub fn create(vertices: *const anyopaque, num_vertices: u32, vertex_size: u32) !*ConvexHullShapeSettings {
        const settings = c.JPC_ConvexHullShapeSettings_Create(vertices, num_vertices, vertex_size);
        if (settings == null)
            return error.FailedToCreateConvexHullShapeSettings;
        return @ptrCast(*ConvexHullShapeSettings, settings);
    }

    pub fn getMaxConvexRadius(settings: *const ConvexHullShapeSettings) f32 {
        return c.JPC_ConvexHullShapeSettings_GetMaxConvexRadius(
            @ptrCast(*const c.JPC_ConvexHullShapeSettings, settings),
        );
    }
    pub fn setMaxConvexRadius(settings: *ConvexHullShapeSettings, radius: f32) void {
        c.JPC_ConvexHullShapeSettings_SetMaxConvexRadius(
            @ptrCast(*c.JPC_ConvexHullShapeSettings, settings),
            radius,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// HeightFieldShapeSettings (-> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const HeightFieldShapeSettings = opaque {
    pub usingnamespace ShapeSettings.Methods(@This());

    pub fn create(
        samples: [*]const f32, // height_field_size^2 samples
        height_field_size: u32, //  height_field_size / block_size must be a power of 2 and minimally 2
    ) !*HeightFieldShapeSettings {
        const settings = c.JPC_HeightFieldShapeSettings_Create(samples, height_field_size);
        if (settings == null)
            return error.FailedToCreateHeightFieldShapeSettings;
        return @ptrCast(*HeightFieldShapeSettings, settings);
    }

    pub fn getBlockSize(settings: *const HeightFieldShapeSettings) u32 {
        return c.JPC_HeightFieldShapeSettings_GetBlockSize(
            @ptrCast(*const c.JPC_HeightFieldShapeSettings, settings),
        );
    }
    pub fn setBlockSize(settings: *HeightFieldShapeSettings, block_size: u32) void {
        c.JPC_HeightFieldShapeSettings_SetBlockSize(
            @ptrCast(*c.JPC_HeightFieldShapeSettings, settings),
            block_size,
        );
    }

    pub fn getBitsPerSample(settings: *const HeightFieldShapeSettings) u32 {
        return c.JPC_HeightFieldShapeSettings_GetBitsPerSample(
            @ptrCast(*const c.JPC_HeightFieldShapeSettings, settings),
        );
    }
    pub fn setBitsPerSample(settings: *HeightFieldShapeSettings, num_bits: u32) void {
        c.JPC_HeightFieldShapeSettings_SetBitsPerSample(
            @ptrCast(*c.JPC_HeightFieldShapeSettings, settings),
            num_bits,
        );
    }

    pub fn getOffset(settings: *const HeightFieldShapeSettings) [3]f32 {
        var offset: [3]f32 = undefined;
        c.JPC_HeightFieldShapeSettings_GetOffset(
            @ptrCast(*const c.JPC_HeightFieldShapeSettings, settings),
            &offset,
        );
        return offset;
    }
    pub fn setOffset(settings: *HeightFieldShapeSettings, offset: [3]f32) void {
        c.JPC_HeightFieldShapeSettings_SetOffset(
            @ptrCast(*c.JPC_HeightFieldShapeSettings, settings),
            &offset,
        );
    }

    pub fn getScale(settings: *const HeightFieldShapeSettings) [3]f32 {
        var scale: [3]f32 = undefined;
        c.JPC_HeightFieldShapeSettings_GetScale(
            @ptrCast(*const c.JPC_HeightFieldShapeSettings, settings),
            &scale,
        );
        return scale;
    }
    pub fn setScale(settings: *HeightFieldShapeSettings, scale: [3]f32) void {
        c.JPC_HeightFieldShapeSettings_SetScale(
            @ptrCast(*c.JPC_HeightFieldShapeSettings, settings),
            &scale,
        );
    }
};
//--------------------------------------------------------------------------------------------------
//
// MeshShapeSettings (-> ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
pub const MeshShapeSettings = opaque {
    pub usingnamespace ShapeSettings.Methods(@This());

    pub fn create(
        vertices: *const anyopaque,
        num_vertices: u32,
        vertex_size: u32,
        indices: []const u32,
    ) !*MeshShapeSettings {
        const settings = c.JPC_MeshShapeSettings_Create(
            vertices,
            num_vertices,
            vertex_size,
            indices.ptr,
            @intCast(u32, indices.len),
        );
        if (settings == null)
            return error.FailedToCreateMeshShapeSettings;
        return @ptrCast(*MeshShapeSettings, settings);
    }

    pub fn getMaxTrianglesPerLeaf(settings: *const MeshShapeSettings) u32 {
        return c.JPC_MeshShapeSettings_GetMaxTrianglesPerLeaf(
            @ptrCast(*const c.JPC_MeshShapeSettings, settings),
        );
    }
    pub fn setMaxTrianglesPerLeaf(settings: *MeshShapeSettings, max_triangles: u32) void {
        c.JPC_MeshShapeSettings_SetMaxTrianglesPerLeaf(
            @ptrCast(*c.JPC_MeshShapeSettings, settings),
            max_triangles,
        );
    }

    pub fn sanitize(settings: *MeshShapeSettings) void {
        c.JPC_MeshShapeSettings_Sanitize(@ptrCast(*c.JPC_MeshShapeSettings, settings));
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
        user_convex1 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX1,
        user_convex2 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX2,
        user_convex3 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX3,
        user_convex4 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX4,
        user_convex5 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX5,
        user_convex6 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX6,
        user_convex7 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX7,
        user_convex8 = c.JPC_SHAPE_SUB_TYPE_USER_CONVEX8,
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

    const ptr = mem_allocator.?.rawAlloc(
        size,
        std.math.log2_int(u29, @intCast(u29, mem_alignment)),
        @returnAddress(),
    );
    if (ptr == null) @panic("zphysics: out of memory");

    mem_allocations.?.put(
        @ptrToInt(ptr),
        .{ .size = @intCast(u48, size), .alignment = mem_alignment },
    ) catch @panic("zphysics: out of memory");

    return ptr;
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

    mem_allocations.?.put(
        @ptrToInt(ptr),
        .{ .size = @intCast(u32, size), .alignment = @intCast(u16, alignment) },
    ) catch @panic("zphysics: out of memory");

    return ptr;
}

fn zphysicsFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const info = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;

        const mem = @ptrCast([*]u8, ptr)[0..info.size];

        mem_allocator.?.rawFree(
            mem,
            std.math.log2_int(u29, @intCast(u29, info.alignment)),
            @returnAddress(),
        );
    }
}
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zphysics.BodyCreationSettings" {
    try init(std.testing.allocator, .{});
    defer deinit();

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

    try expect(approxEql(f32, bcs0.rotation[0], bcs1.rotation[0], 0.0001));
    try expect(approxEql(f32, bcs0.rotation[1], bcs1.rotation[1], 0.0001));
    try expect(approxEql(f32, bcs0.rotation[2], bcs1.rotation[2], 0.0001));
    try expect(approxEql(f32, bcs0.rotation[3], bcs1.rotation[3], 0.0001));

    try expect(approxEql(f32, bcs0.linear_velocity[0], bcs1.linear_velocity[0], 0.0001));
    try expect(approxEql(f32, bcs0.linear_velocity[1], bcs1.linear_velocity[1], 0.0001));
    try expect(approxEql(f32, bcs0.linear_velocity[2], bcs1.linear_velocity[2], 0.0001));

    try expect(approxEql(f32, bcs0.angular_velocity[0], bcs1.angular_velocity[0], 0.0001));
    try expect(approxEql(f32, bcs0.angular_velocity[1], bcs1.angular_velocity[1], 0.0001));
    try expect(approxEql(f32, bcs0.angular_velocity[2], bcs1.angular_velocity[2], 0.0001));

    try expect(bcs0.user_data == bcs1.user_data);
    try expect(bcs0.object_layer == bcs1.object_layer);
    //try expect(eql(u8, asBytes(&bcs0.collision_group), asBytes(&bcs1.collision_group)));
    try expect(bcs0.motion_type == bcs1.motion_type);
    try expect(bcs0.allow_dynamic_or_kinematic == bcs1.allow_dynamic_or_kinematic);
    try expect(bcs0.is_sensor == bcs1.is_sensor);
    try expect(bcs0.use_manifold_reduction == bcs1.use_manifold_reduction);
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
    //try expect(eql(
    //    u8,
    //    asBytes(&bcs0.mass_properties_override.inertia),
    //    asBytes(&bcs1.mass_properties_override.inertia),
    //));
    try expect(bcs0.reserved == bcs1.reserved);
    try expect(bcs0.shape == bcs1.shape);
}

test "zphysics.basic" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
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

    {
        physics_system.setGravity(.{ 0, -10.0, 0 });
        const gravity = physics_system.getGravity();
        try expect(gravity[0] == 0 and gravity[1] == -10.0 and gravity[2] == 0);
    }

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
    _ = physics_system.getNarrowPhaseQuery();
    _ = physics_system.getNarrowPhaseQueryNoLock();

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

test "zphysics.shape.sphere" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const sphere_shape_settings = try SphereShapeSettings.create(10.0);
    defer sphere_shape_settings.release();

    try expect(sphere_shape_settings.getRadius() == 10.0);

    sphere_shape_settings.setRadius(2.0);
    try expect(sphere_shape_settings.getRadius() == 2.0);

    sphere_shape_settings.setDensity(2.0);
    try expect(sphere_shape_settings.getDensity() == 2.0);

    sphere_shape_settings.setMaterial(null);
    try expect(sphere_shape_settings.getMaterial() == null);

    const sphere_shape = try sphere_shape_settings.createShape();
    defer sphere_shape.release();

    try expect(sphere_shape.getRefCount() == 2);
    try expect(sphere_shape.getType() == .convex);
    try expect(sphere_shape.getSubType() == .sphere);

    sphere_shape.setUserData(1456);
    try expect(sphere_shape.getUserData() == 1456);
}

test "zphysics.shape.capsule" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const capsule_shape_settings = try CapsuleShapeSettings.create(10.0, 2.0);
    defer capsule_shape_settings.release();

    try expect(capsule_shape_settings.getRadius() == 2.0);
    try expect(capsule_shape_settings.getHalfHeight() == 10.0);

    capsule_shape_settings.setRadius(4.0);
    try expect(capsule_shape_settings.getRadius() == 4.0);

    capsule_shape_settings.setHalfHeight(1.0);
    try expect(capsule_shape_settings.getHalfHeight() == 1.0);

    const capsule_shape = try capsule_shape_settings.createShape();
    defer capsule_shape.release();

    try expect(capsule_shape.getRefCount() == 2);
    try expect(capsule_shape.getType() == .convex);
    try expect(capsule_shape.getSubType() == .capsule);

    capsule_shape.setUserData(146);
    try expect(capsule_shape.getUserData() == 146);
}

test "zphysics.shape.taperedcapsule" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const capsule_shape_settings = try TaperedCapsuleShapeSettings.create(10.0, 2.0, 3.0);
    defer capsule_shape_settings.release();

    try expect(capsule_shape_settings.getTopRadius() == 2.0);
    try expect(capsule_shape_settings.getBottomRadius() == 3.0);
    try expect(capsule_shape_settings.getHalfHeight() == 10.0);

    capsule_shape_settings.setTopRadius(4.0);
    try expect(capsule_shape_settings.getTopRadius() == 4.0);

    capsule_shape_settings.setBottomRadius(1.0);
    try expect(capsule_shape_settings.getBottomRadius() == 1.0);

    const capsule_shape = try capsule_shape_settings.createShape();
    defer capsule_shape.release();

    try expect(capsule_shape.getRefCount() == 2);
    try expect(capsule_shape.getType() == .convex);
    try expect(capsule_shape.getSubType() == .tapered_capsule);

    capsule_shape.setUserData(1146);
    try expect(capsule_shape.getUserData() == 1146);
}

test "zphysics.shape.cylinder" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const cylinder_shape_settings = try CylinderShapeSettings.create(10.0, 2.0);
    defer cylinder_shape_settings.release();

    try expect(cylinder_shape_settings.getRadius() == 2.0);
    try expect(cylinder_shape_settings.getHalfHeight() == 10.0);

    cylinder_shape_settings.setRadius(4.0);
    try expect(cylinder_shape_settings.getRadius() == 4.0);

    cylinder_shape_settings.setHalfHeight(1.0);
    try expect(cylinder_shape_settings.getHalfHeight() == 1.0);

    cylinder_shape_settings.setConvexRadius(0.5);
    try expect(cylinder_shape_settings.getConvexRadius() == 0.5);

    const cylinder_shape = try cylinder_shape_settings.createShape();
    defer cylinder_shape.release();

    try expect(cylinder_shape.getRefCount() == 2);
    try expect(cylinder_shape.getType() == .convex);
    try expect(cylinder_shape.getSubType() == .cylinder);

    cylinder_shape.setUserData(146);
    try expect(cylinder_shape.getUserData() == 146);
}

test "zphysics.shape.convexhull" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const points = [_]f32{ 0, 0, 0, 1, 1, 1, 1, 1, 0 };

    const settings = try ConvexHullShapeSettings.create(&points, 3, 12);
    defer settings.release();

    settings.setMaxConvexRadius(0.1);
    try expect(settings.getMaxConvexRadius() == 0.1);

    const shape = try settings.createShape();
    defer shape.release();

    try expect(shape.getRefCount() == 2);
    try expect(shape.getType() == .convex);
    try expect(shape.getSubType() == .convex_hull);

    shape.setUserData(111);
    try expect(shape.getUserData() == 111);
}

test "zphysics.shape.heightfield" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const points = [16]f32{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2 };

    // Height field size is 4x4
    const settings = try HeightFieldShapeSettings.create(&points, 4);
    defer settings.release();

    settings.setBlockSize(2);
    settings.setBitsPerSample(6);
    settings.setOffset(.{ 1, 2, 3 });
    settings.setScale(.{ 4, 5, 6 });

    try expect(settings.getBlockSize() == 2);
    try expect(settings.getBitsPerSample() == 6);
    try expect(settings.getOffset()[0] == 1);
    try expect(settings.getOffset()[1] == 2);
    try expect(settings.getOffset()[2] == 3);
    try expect(settings.getScale()[0] == 4);
    try expect(settings.getScale()[1] == 5);
    try expect(settings.getScale()[2] == 6);

    const shape = try settings.createShape();
    defer shape.release();

    try expect(shape.getRefCount() == 2);
    try expect(shape.getType() == .height_field);
    try expect(shape.getSubType() == .height_field);

    shape.setUserData(1112);
    try expect(shape.getUserData() == 1112);
}

test "zphysics.shape.meshshape" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const vertices = [9]f32{ 0, 0, 0, 1, 1, 1, 1, -1, 1 };
    const indices = [3]u32{ 0, 1, 2 };

    const settings = try MeshShapeSettings.create(&vertices, 3, @sizeOf([3]f32), &indices);
    defer settings.release();

    settings.setMaxTrianglesPerLeaf(4);
    settings.sanitize();

    try expect(settings.getMaxTrianglesPerLeaf() == 4);

    const shape = try settings.createShape();
    defer shape.release();

    try expect(shape.getRefCount() == 2);
    try expect(shape.getType() == .mesh);
    try expect(shape.getSubType() == .mesh);

    shape.setUserData(1112);
    try expect(shape.getUserData() == 1112);
}

test "zphysics.body.basic" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
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
        .object_layer = test_cb1.object_layers.non_moving,
    };
    const body_id = try body_interface_mut.createAndAddBody(floor_settings, .activate);
    defer {
        body_interface_mut.removeBody(body_id);
        body_interface_mut.destroyBody(body_id);
    }

    physics_system.optimizeBroadPhase();

    {
        const query = physics_system.getNarrowPhaseQuery();

        var result = query.castRay(.{ .origin = .{ 0, 10, 0, 1 }, .direction = .{ 0, -20, 0, 0 } }, .{});
        try expect(result.has_hit == true);
        try expect(result.hit.body_id == body_id);
        try expect(result.hit.sub_shape_id == sub_shape_id_empty);
        try expect(std.math.approxEqAbs(f32, result.hit.fraction, 0.5, 0.001) == true);

        result = query.castRay(.{ .origin = .{ 0, 10, 0, 1 }, .direction = .{ 0, 20, 0, 0 } }, .{});
        try expect(result.has_hit == false);
        try expect(result.hit.body_id == body_id_invalid);

        result = query.castRay(.{ .origin = .{ 0, 10, 0, 1 }, .direction = .{ 0, -5, 0, 0 } }, .{});
        try expect(result.has_hit == false);
        try expect(result.hit.body_id == body_id_invalid);

        const ray = c.JPC_RRayCast{
            .origin = .{ 0, 10, 0, 0 },
            .direction = .{ 0, -20, 0, 0 },
        };
        var hit: c.JPC_RayCastResult = .{
            .body_id = body_id_invalid,
            .fraction = 1.0 + flt_epsilon,
            .sub_shape_id = undefined,
        };
        const has_hit = c.JPC_NarrowPhaseQuery_CastRay(
            @ptrCast(*const c.JPC_NarrowPhaseQuery, query),
            &ray,
            &hit,
            null, // broad_phase_layer_filter
            null, // object_layer_filter
            null, // body_filter
        );
        try expect(has_hit == true);
        try expect(std.math.approxEqAbs(f32, hit.fraction, 0.5, 0.001) == true);
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
        defer write_lock.unlock();

        if (write_lock.body) |locked_body| {
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

        const xform = body1.getWorldTransform();
        try expect(xform.rotation[0] == 1.0);
        try expect(xform.position[1] == -1.0);

        body1.setUserData(12345);
        try expect(body1.getUserData() == 12345);

        body1.setMotionType(.static);
        try expect(body1.getMotionType() == .static);

        body1.setCollisionGroup(.{ .group_id = 123 });
        try expect(body1.getCollisionGroup().group_id == 123);
        body1.getCollisionGroupMut().group_id += 1;
        try expect(body1.getCollisionGroup().group_id == 124);

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

test "zphysics.body.motion" {
    try init(std.testing.allocator, .{});
    defer deinit();

    const my_broad_phase_layer_interface = test_cb1.MyBroadphaseLayerInterface.init();
    const my_broad_phase_should_collide = test_cb1.MyObjectVsBroadPhaseLayerFilter{};
    const my_object_should_collide = test_cb1.MyObjectLayerPairFilter{};

    const physics_system = try PhysicsSystem.create(
        @ptrCast(*const BroadPhaseLayerInterface, &my_broad_phase_layer_interface),
        @ptrCast(*const ObjectVsBroadPhaseLayerFilter, &my_broad_phase_should_collide),
        @ptrCast(*const ObjectLayerPairFilter, &my_object_should_collide),
        .{},
    );
    defer physics_system.destroy();

    const body_interface = physics_system.getBodyInterfaceMut();
    const lock_interface = physics_system.getBodyLockInterface();

    const shape_settings = try BoxShapeSettings.create(.{ 1.0, 2.0, 3.0 });
    defer shape_settings.release();

    const shape = try shape_settings.createShape();
    defer shape.release();

    const body_settings = BodyCreationSettings{
        .position = .{ 0.0, 10.0, 0.0, 1.0 },
        .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
        .shape = shape,
        .motion_type = .dynamic,
        .object_layer = test_cb1.object_layers.moving,
    };
    const body_id = try body_interface.createAndAddBody(body_settings, .activate);
    defer body_interface.removeAndDestroyBody(body_id);

    physics_system.optimizeBroadPhase();

    var write_lock: BodyLockWrite = .{};
    write_lock.lock(lock_interface, body_id);
    defer write_lock.unlock();
    const body = write_lock.body.?;

    body.setRestitution(0.5);
    body.setFriction(0.25);
    body.setUserData(0xC0DE_C0DE_C0DE_C0DE);
    body.setAllowSleeping(false);

    try expect(body.getFriction() == 0.25);
    try expect(body.friction == 0.25);
    try expect(body.getRestitution() == 0.5);
    try expect(body.restitution == 0.5);
    try expect(body.isInBroadPhase() == true);
    try expect(body.isDynamic() == true);
    try expect(body.isStatic() == false);
    try expect(body.isSensor() == false);
    try expect(body.getShape() == shape);
    try expect(body.shape == shape);
    try expect(body.getUserData() == 0xC0DE_C0DE_C0DE_C0DE);
    try expect(body.user_data == 0xC0DE_C0DE_C0DE_C0DE);
    try expect(body.getAllowSleeping() == false);

    const normal0 = body.getWorldSpaceSurfaceNormal(sub_shape_id_empty, .{ 0, 12, 0 });
    const normal1 = body.getWorldSpaceSurfaceNormal(sub_shape_id_empty, .{ -1, 10, 0 });

    try expect(std.math.approxEqAbs(f32, normal0[0], 0.0, 0.001) == true);
    try expect(std.math.approxEqAbs(f32, normal0[1], 1.0, 0.001) == true);
    try expect(std.math.approxEqAbs(f32, normal0[2], 0.0, 0.001) == true);
    try expect(std.math.approxEqAbs(f32, normal1[0], -1.0, 0.001) == true);
    try expect(std.math.approxEqAbs(f32, normal1[1], 0.0, 0.001) == true);
    try expect(std.math.approxEqAbs(f32, normal1[2], 0.0, 0.001) == true);

    const motion = body.getMotionPropertiesMut();

    try expect(body.motion_properties.? == motion);

    motion.setLinearDamping(0.5);
    motion.setAngularDamping(0.25);
    motion.setGravityFactor(0.5);

    try expect(motion.allow_sleeping == false);
    try expect(motion.getLinearDamping() == 0.5);
    try expect(motion.linear_damping == 0.5);
    try expect(motion.getAngularDamping() == 0.25);
    try expect(motion.angular_damping == 0.25);
    try expect(motion.getGravityFactor() == 0.5);
    try expect(motion.gravity_factor == 0.5);
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
    const object_layers = struct {
        const non_moving: ObjectLayer = 0;
        const moving: ObjectLayer = 1;
        const len: u32 = 2;
    };

    const broad_phase_layers = struct {
        const non_moving: BroadPhaseLayer = 0;
        const moving: BroadPhaseLayer = 1;
        const len: u32 = 2;
    };

    const MyBroadphaseLayerInterface = extern struct {
        usingnamespace BroadPhaseLayerInterface.Methods(@This());
        __v: *const BroadPhaseLayerInterface.VTable = &vtable,

        object_to_broad_phase: [object_layers.len]BroadPhaseLayer = undefined,

        const vtable = BroadPhaseLayerInterface.VTable{
            .getNumBroadPhaseLayers = _getNumBroadPhaseLayers,
            .getBroadPhaseLayer = _getBroadPhaseLayer,
        };

        fn init() MyBroadphaseLayerInterface {
            var layer_interface: MyBroadphaseLayerInterface = .{};
            layer_interface.object_to_broad_phase[object_layers.non_moving] = broad_phase_layers.non_moving;
            layer_interface.object_to_broad_phase[object_layers.moving] = broad_phase_layers.moving;
            return layer_interface;
        }

        fn _getNumBroadPhaseLayers(iself: *const BroadPhaseLayerInterface) callconv(.C) u32 {
            const self = @ptrCast(*const MyBroadphaseLayerInterface, iself);
            return @intCast(u32, self.object_to_broad_phase.len);
        }

        fn _getBroadPhaseLayer(
            iself: *const BroadPhaseLayerInterface,
            layer: ObjectLayer,
        ) callconv(.C) BroadPhaseLayer {
            const self = @ptrCast(*const MyBroadphaseLayerInterface, iself);
            return self.object_to_broad_phase[@intCast(usize, layer)];
        }
    };

    const MyObjectVsBroadPhaseLayerFilter = extern struct {
        usingnamespace ObjectVsBroadPhaseLayerFilter.Methods(@This());
        __v: *const ObjectVsBroadPhaseLayerFilter.VTable = &vtable,

        const vtable = ObjectVsBroadPhaseLayerFilter.VTable{ .shouldCollide = _shouldCollide };

        fn _shouldCollide(
            _: *const ObjectVsBroadPhaseLayerFilter,
            layer1: ObjectLayer,
            layer2: BroadPhaseLayer,
        ) callconv(.C) bool {
            return switch (layer1) {
                object_layers.non_moving => layer2 == broad_phase_layers.moving,
                object_layers.moving => true,
                else => unreachable,
            };
        }
    };

    const MyObjectLayerPairFilter = extern struct {
        usingnamespace ObjectLayerPairFilter.Methods(@This());
        __v: *const ObjectLayerPairFilter.VTable = &vtable,

        const vtable = ObjectLayerPairFilter.VTable{ .shouldCollide = _shouldCollide };

        fn _shouldCollide(
            _: *const ObjectLayerPairFilter,
            object1: ObjectLayer,
            object2: ObjectLayer,
        ) callconv(.C) bool {
            return switch (object1) {
                object_layers.non_moving => object2 == object_layers.moving,
                object_layers.moving => true,
                else => unreachable,
            };
        }
    };
};
//--------------------------------------------------------------------------------------------------
