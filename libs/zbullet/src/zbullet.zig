// zbullet v0.2
// Zig bindings for Bullet Physics SDK

const builtin = @import("builtin");
const std = @import("std");
const Mutex = std.Thread.Mutex;
const expect = std.testing.expect;

pub const World = *align(@sizeOf(usize)) WorldImpl;
pub const Shape = *align(@sizeOf(usize)) ShapeImpl;
pub const BoxShape = *align(@sizeOf(usize)) BoxShapeImpl;
pub const SphereShape = *align(@sizeOf(usize)) SphereShapeImpl;
pub const CapsuleShape = *align(@sizeOf(usize)) CapsuleShapeImpl;
pub const CylinderShape = *align(@sizeOf(usize)) CylinderShapeImpl;
pub const CompoundShape = *align(@sizeOf(usize)) CompoundShapeImpl;
pub const TriangleMeshShape = *align(@sizeOf(usize)) TriangleMeshShapeImpl;
pub const Body = *align(@sizeOf(usize)) BodyImpl;
pub const Constraint = *align(@sizeOf(usize)) ConstraintImpl;
pub const Point2PointConstraint = *align(@sizeOf(usize)) Point2PointConstraintImpl;

pub const AllocFn = *const fn (size: usize, alignment: i32) callconv(.C) ?*anyopaque;
pub const FreeFn = *const fn (ptr: ?*anyopaque) callconv(.C) void;

extern fn cbtAlignedAllocSetCustomAligned(
    alloc: ?AllocFn,
    free: ?FreeFn,
) void;

const SizeAndAlignment = packed struct(u64) {
    size: u48,
    alignment: u16,
};
var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, SizeAndAlignment) = null;
var mem_mutex: std.Thread.Mutex = .{};

export fn zbulletAlloc(size: usize, alignment: i32) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const ptr = mem_allocator.?.rawAlloc(
        size,
        std.math.log2_int(u29, @as(u29, @intCast(alignment))),
        @returnAddress(),
    );
    if (ptr == null) @panic("zbullet: out of memory");

    mem_allocations.?.put(
        @intFromPtr(ptr),
        .{ .size = @as(u32, @intCast(size)), .alignment = @as(u16, @intCast(alignment)) },
    ) catch @panic("zbullet: out of memory");

    return ptr;
}

export fn zbulletFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const info = mem_allocations.?.fetchRemove(@intFromPtr(ptr)).?.value;

        const mem = @as([*]u8, @ptrCast(ptr))[0..info.size];

        mem_allocator.?.rawFree(
            mem,
            std.math.log2_int(u29, @as(u29, @intCast(info.alignment))),
            @returnAddress(),
        );
    }
}

extern fn cbtTaskSchedInit() void;
extern fn cbtTaskSchedDeinit() void;

pub fn init(alloc: std.mem.Allocator) void {
    std.debug.assert(mem_allocator == null and mem_allocations == null);
    mem_allocator = alloc;
    mem_allocations = std.AutoHashMap(usize, SizeAndAlignment).init(mem_allocator.?);
    mem_allocations.?.ensureTotalCapacity(256) catch @panic("zbullet: out of memory");
    cbtAlignedAllocSetCustomAligned(zbulletAlloc, zbulletFree);
    cbtTaskSchedInit();
    _ = ConstraintImpl.getFixedBody(); // This will allocate 'fixed body' singleton on the heap.
}

pub fn deinit() void {
    ConstraintImpl.destroyFixedBody();
    cbtTaskSchedDeinit();
    cbtAlignedAllocSetCustomAligned(null, null);
    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}

pub const CollisionFilter = packed struct {
    default: bool = false,
    static: bool = false,
    kinematic: bool = false,
    debris: bool = false,
    sensor_trigger: bool = false,
    character: bool = false,

    _pad0: u10 = 0,
    _pad1: u16 = 0,

    pub const all = @as(CollisionFilter, @bitCast(~@as(u32, 0)));

    comptime {
        std.debug.assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const RayCastFlags = packed struct {
    trimesh_skip_backfaces: bool = false,
    trimesh_keep_unflipped_normals: bool = false,
    use_subsimplex_convex_test: bool = false, // used by default, faster but less accurate
    use_gjk_convex_test: bool = false,

    _pad0: u12 = 0,
    _pad1: u16 = 0,

    comptime {
        std.debug.assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const RayCastResult = extern struct {
    hit_normal_world: [3]f32,
    hit_point_world: [3]f32,
    hit_fraction: f32,
    body: ?Body,
};

pub fn initWorld() World {
    return WorldImpl.init();
}

const WorldImpl = opaque {
    fn init() World {
        std.debug.assert(mem_allocator != null and mem_allocations != null);
        return cbtWorldCreate();
    }
    extern fn cbtWorldCreate() World;

    pub fn deinit(world: World) void {
        std.debug.assert(world.getNumBodies() == 0);
        std.debug.assert(world.getNumConstraints() == 0);
        cbtWorldDestroy(world);
    }
    extern fn cbtWorldDestroy(world: World) void;

    pub const setGravity = cbtWorldSetGravity;
    extern fn cbtWorldSetGravity(world: World, gravity: *const [3]f32) void;

    pub const getGravity = cbtWorldGetGravity;
    extern fn cbtWorldGetGravity(world: World, gravity: *[3]f32) void;

    pub fn stepSimulation(world: World, time_step: f32, args: struct {
        max_sub_steps: u32 = 1,
        fixed_time_step: f32 = 1.0 / 60.0,
    }) u32 {
        return cbtWorldStepSimulation(
            world,
            time_step,
            args.max_sub_steps,
            args.fixed_time_step,
        );
    }
    extern fn cbtWorldStepSimulation(
        world: World,
        time_step: f32,
        max_sub_steps: u32,
        fixed_time_step: f32,
    ) u32;

    pub const addBody = cbtWorldAddBody;
    extern fn cbtWorldAddBody(world: World, body: Body) void;

    pub const removeBody = cbtWorldRemoveBody;
    extern fn cbtWorldRemoveBody(world: World, body: Body) void;

    pub const getBody = cbtWorldGetBody;
    extern fn cbtWorldGetBody(world: World, index: i32) Body;

    pub const getNumBodies = cbtWorldGetNumBodies;
    extern fn cbtWorldGetNumBodies(world: World) i32;

    pub const addConstraint = cbtWorldAddConstraint;
    extern fn cbtWorldAddConstraint(
        world: World,
        con: Constraint,
        disable_collision_between_linked_bodies: bool,
    ) void;

    pub const removeConstraint = cbtWorldRemoveConstraint;
    extern fn cbtWorldRemoveConstraint(world: World, con: Constraint) void;

    pub const getConstraint = cbtWorldGetConstraint;
    extern fn cbtWorldGetConstraint(world: World, index: i32) Constraint;

    pub const getNumConstraints = cbtWorldGetNumConstraints;
    extern fn cbtWorldGetNumConstraints(world: World) i32;

    pub const debugSetDrawer = cbtWorldDebugSetDrawer;
    extern fn cbtWorldDebugSetDrawer(world: World, debug: *const DebugDraw) void;

    pub fn debugSetMode(world: World, mode: DebugMode) void {
        cbtWorldDebugSetMode(world, @as(c_int, @bitCast(mode)));
    }
    extern fn cbtWorldDebugSetMode(world: World, mode: c_int) void;

    pub fn debugGetMode(world: World) DebugMode {
        return @as(DebugMode, @bitCast(cbtWorldDebugGetMode(world)));
    }
    extern fn cbtWorldDebugGetMode(world: World) c_int;

    pub const debugDrawAll = cbtWorldDebugDrawAll;
    extern fn cbtWorldDebugDrawAll(world: World) void;

    pub const debugDrawLine1 = cbtWorldDebugDrawLine1;
    extern fn cbtWorldDebugDrawLine1(
        world: World,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color: *const [3]f32,
    ) void;

    pub const debugDrawLine2 = cbtWorldDebugDrawLine2;
    extern fn cbtWorldDebugDrawLine2(
        world: World,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color0: *const [3]f32,
        color1: *const [3]f32,
    ) void;

    pub const debugDrawSphere = cbtWorldDebugDrawSphere;
    extern fn cbtWorldDebugDrawSphere(
        world: World,
        position: *const [3]f32,
        radius: f32,
        color: *const [3]f32,
    ) void;

    pub fn rayTestClosest(
        world: World,
        ray_from_world: *const [3]f32,
        ray_to_world: *const [3]f32,
        group: CollisionFilter,
        mask: CollisionFilter,
        flags: RayCastFlags,
        raycast_result: ?*RayCastResult,
    ) bool {
        return cbtWorldRayTestClosest(
            world,
            ray_from_world,
            ray_to_world,
            @as(c_int, @bitCast(group)),
            @as(c_int, @bitCast(mask)),
            @as(c_int, @bitCast(flags)),
            raycast_result,
        );
    }
    extern fn cbtWorldRayTestClosest(
        world: World,
        ray_from_world: *const [3]f32,
        ray_to_world: *const [3]f32,
        group: c_int,
        mask: c_int,
        flags: c_int,
        raycast_result: ?*RayCastResult,
    ) bool;
};

pub const Axis = enum(c_int) {
    x = 0,
    y = 1,
    z = 2,
};

pub const ShapeType = enum(c_int) {
    box = 0,
    sphere = 8,
    capsule = 10,
    cylinder = 13,
    compound = 31,
    trimesh = 21,
};

const ShapeImpl = opaque {
    pub const alloc = cbtShapeAllocate;
    extern fn cbtShapeAllocate(stype: ShapeType) Shape;

    pub const dealloc = cbtShapeDeallocate;
    extern fn cbtShapeDeallocate(shape: Shape) void;

    pub fn deinit(shape: Shape) void {
        shape.destroy();
        shape.dealloc();
    }

    pub fn destroy(shape: Shape) void {
        switch (shape.getType()) {
            .box,
            .sphere,
            .capsule,
            .cylinder,
            .compound,
            => cbtShapeDestroy(shape),
            .trimesh => cbtShapeTriMeshDestroy(shape),
        }
    }
    extern fn cbtShapeDestroy(shape: Shape) void;
    extern fn cbtShapeTriMeshDestroy(shape: Shape) void;

    pub const isCreated = cbtShapeIsCreated;
    extern fn cbtShapeIsCreated(shape: Shape) bool;

    pub const getType = cbtShapeGetType;
    extern fn cbtShapeGetType(shape: Shape) ShapeType;

    pub const setMargin = cbtShapeSetMargin;
    extern fn cbtShapeSetMargin(shape: Shape, margin: f32) void;

    pub const getMargin = cbtShapeGetMargin;
    extern fn cbtShapeGetMargin(shape: Shape) f32;

    pub const isPolyhedral = cbtShapeIsPolyhedral;
    extern fn cbtShapeIsPolyhedral(shape: Shape) bool;

    pub const isConvex2d = cbtShapeIsConvex2d;
    extern fn cbtShapeIsConvex2d(shape: Shape) bool;

    pub const isConvex = cbtShapeIsConvex;
    extern fn cbtShapeIsConvex(shape: Shape) bool;

    pub const isNonMoving = cbtShapeIsNonMoving;
    extern fn cbtShapeIsNonMoving(shape: Shape) bool;

    pub const isConcave = cbtShapeIsConcave;
    extern fn cbtShapeIsConcave(shape: Shape) bool;

    pub const isCompound = cbtShapeIsCompound;
    extern fn cbtShapeIsCompound(shape: Shape) bool;

    pub const calculateLocalInertia = cbtShapeCalculateLocalInertia;
    extern fn cbtShapeCalculateLocalInertia(
        shape: Shape,
        mass: f32,
        inertia: *[3]f32,
    ) void;

    pub const setUserPointer = cbtShapeSetUserPointer;
    extern fn cbtShapeSetUserPointer(shape: Shape, ptr: ?*anyopaque) void;

    pub const getUserPointer = cbtShapeGetUserPointer;
    extern fn cbtShapeGetUserPointer(shape: Shape) ?*anyopaque;

    pub const setUserIndex = cbtShapeSetUserIndex;
    extern fn cbtShapeSetUserIndex(shape: Shape, slot: u32, index: i32) void;

    pub const getUserIndex = cbtShapeGetUserIndex;
    extern fn cbtShapeGetUserIndex(shape: Shape, slot: u32) i32;

    pub fn as(shape: Shape, comptime stype: ShapeType) switch (stype) {
        .box => BoxShape,
        .sphere => SphereShape,
        .cylinder => CylinderShape,
        .capsule => CapsuleShape,
        .compound => CompoundShape,
        .trimesh => TriangleMeshShape,
    } {
        std.debug.assert(shape.getType() == stype);
        return switch (stype) {
            .box => @as(BoxShape, @ptrCast(shape)),
            .sphere => @as(SphereShape, @ptrCast(shape)),
            .cylinder => @as(CylinderShape, @ptrCast(shape)),
            .capsule => @as(CapsuleShape, @ptrCast(shape)),
            .compound => @as(CompoundShape, @ptrCast(shape)),
            .trimesh => @as(TriangleMeshShape, @ptrCast(shape)),
        };
    }
};

fn ShapeFunctions(comptime T: type) type {
    return struct {
        pub fn asShape(shape: T) Shape {
            return @as(Shape, @ptrCast(shape));
        }

        pub fn dealloc(shape: T) void {
            shape.asShape().dealloc();
        }
        pub fn destroy(shape: T) void {
            shape.asShape().destroy();
        }
        pub fn deinit(shape: T) void {
            shape.asShape().deinit();
        }
        pub fn isCreated(shape: T) bool {
            return shape.asShape().isCreated();
        }
        pub fn getType(shape: T) ShapeType {
            return shape.asShape().getType();
        }
        pub fn setMargin(shape: T, margin: f32) void {
            shape.asShape().setMargin(margin);
        }
        pub fn getMargin(shape: T) f32 {
            return shape.asShape().getMargin();
        }
        pub fn isPolyhedral(shape: T) bool {
            return shape.asShape().isPolyhedral();
        }
        pub fn isConvex2d(shape: T) bool {
            return shape.asShape().isConvex2d();
        }
        pub fn isConvex(shape: T) bool {
            return shape.asShape().isConvex();
        }
        pub fn isNonMoving(shape: T) bool {
            return shape.asShape().isNonMoving();
        }
        pub fn isConcave(shape: T) bool {
            return shape.asShape().isConcave();
        }
        pub fn isCompound(shape: T) bool {
            return shape.asShape().isCompound();
        }
        pub fn calculateLocalInertia(shape: Shape, mass: f32, inertia: *[3]f32) void {
            shape.asShape().calculateLocalInertia(shape, mass, inertia);
        }
        pub fn setUserPointer(shape: T, ptr: ?*anyopaque) void {
            shape.asShape().setUserPointer(ptr);
        }
        pub fn getUserPointer(shape: T) ?*anyopaque {
            return shape.asShape().getUserPointer();
        }
        pub fn setUserIndex(shape: T, slot: u32, index: i32) void {
            shape.asShape().setUserIndex(slot, index);
        }
        pub fn getUserIndex(shape: T, slot: u32) i32 {
            return shape.asShape().getUserIndex(slot);
        }
    };
}

pub fn initBoxShape(half_extents: *const [3]f32) BoxShape {
    const box = BoxShapeImpl.alloc();
    box.create(half_extents);
    return box;
}

const BoxShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(BoxShape);

    fn alloc() BoxShape {
        return @as(BoxShape, @ptrCast(ShapeImpl.alloc(.box)));
    }

    pub const create = cbtShapeBoxCreate;
    extern fn cbtShapeBoxCreate(box: BoxShape, half_extents: *const [3]f32) void;

    pub const getHalfExtentsWithoutMargin = cbtShapeBoxGetHalfExtentsWithoutMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithoutMargin(box: BoxShape, half_extents: *[3]f32) void;

    pub const getHalfExtentsWithMargin = cbtShapeBoxGetHalfExtentsWithMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithMargin(box: BoxShape, half_extents: *[3]f32) void;
};

pub fn initSphereShape(radius: f32) SphereShape {
    const sphere = SphereShapeImpl.alloc();
    sphere.create(radius);
    return sphere;
}

const SphereShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(SphereShape);

    fn alloc() SphereShape {
        return @as(SphereShape, @ptrCast(ShapeImpl.alloc(.sphere)));
    }

    pub const create = cbtShapeSphereCreate;
    extern fn cbtShapeSphereCreate(sphere: SphereShape, radius: f32) void;

    pub const getRadius = cbtShapeSphereGetRadius;
    extern fn cbtShapeSphereGetRadius(sphere: SphereShape) f32;

    pub const setUnscaledRadius = cbtShapeSphereSetUnscaledRadius;
    extern fn cbtShapeSphereSetUnscaledRadius(sphere: SphereShape, radius: f32) void;
};

pub fn initCapsuleShape(radius: f32, height: f32, upaxis: Axis) CapsuleShape {
    const capsule = CapsuleShapeImpl.alloc();
    capsule.create(radius, height, upaxis);
    return capsule;
}

const CapsuleShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(CapsuleShape);

    fn alloc() CapsuleShape {
        return @as(CapsuleShape, @ptrCast(ShapeImpl.alloc(.capsule)));
    }

    pub const create = cbtShapeCapsuleCreate;
    extern fn cbtShapeCapsuleCreate(
        capsule: CapsuleShape,
        radius: f32,
        height: f32,
        upaxis: Axis,
    ) void;

    pub const getUpAxis = cbtShapeCapsuleGetUpAxis;
    extern fn cbtShapeCapsuleGetUpAxis(capsule: CapsuleShape) Axis;

    pub const getHalfHeight = cbtShapeCapsuleGetHalfHeight;
    extern fn cbtShapeCapsuleGetHalfHeight(capsule: CapsuleShape) f32;

    pub const getRadius = cbtShapeCapsuleGetRadius;
    extern fn cbtShapeCapsuleGetRadius(capsule: CapsuleShape) f32;
};

pub fn initCylinderShape(
    half_extents: *const [3]f32,
    upaxis: Axis,
) CylinderShape {
    const cylinder = CylinderShapeImpl.alloc();
    cylinder.create(half_extents, upaxis);
    return cylinder;
}

const CylinderShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(CylinderShape);

    fn alloc() CylinderShape {
        return @as(CylinderShape, @ptrCast(ShapeImpl.alloc(.cylinder)));
    }

    pub const create = cbtShapeCylinderCreate;
    extern fn cbtShapeCylinderCreate(
        cylinder: CylinderShape,
        half_extents: *const [3]f32,
        upaxis: Axis,
    ) void;

    pub const getHalfExtentsWithoutMargin = cbtShapeCylinderGetHalfExtentsWithoutMargin;
    extern fn cbtShapeCylinderGetHalfExtentsWithoutMargin(
        cylinder: CylinderShape,
        half_extents: *[3]f32,
    ) void;

    pub const getHalfExtentsWithMargin = cbtShapeCylinderGetHalfExtentsWithMargin;
    extern fn cbtShapeCylinderGetHalfExtentsWithMargin(
        cylinder: CylinderShape,
        half_extents: *[3]f32,
    ) void;

    pub const getUpAxis = cbtShapeCylinderGetUpAxis;
    extern fn cbtShapeCylinderGetUpAxis(capsule: CylinderShape) Axis;
};

pub fn initCompoundShape(
    args: struct {
        enable_dynamic_aabb_tree: bool = true,
        initial_child_capacity: u32 = 0,
    },
) CompoundShape {
    const cshape = CompoundShapeImpl.alloc();
    cshape.create(args.enable_dynamic_aabb_tree, args.initial_child_capacity);
    return cshape;
}

const CompoundShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(CompoundShape);

    fn alloc() CompoundShape {
        return @as(CompoundShape, @ptrCast(ShapeImpl.alloc(.compound)));
    }

    pub const create = cbtShapeCompoundCreate;
    extern fn cbtShapeCompoundCreate(
        cshape: CompoundShape,
        enable_dynamic_aabb_tree: bool,
        initial_child_capacity: u32,
    ) void;

    pub const addChild = cbtShapeCompoundAddChild;
    extern fn cbtShapeCompoundAddChild(
        cshape: CompoundShape,
        local_transform: *const [12]f32,
        child_shape: Shape,
    ) void;

    pub const removeChild = cbtShapeCompoundRemoveChild;
    extern fn cbtShapeCompoundRemoveChild(cshape: CompoundShape, child_shape: Shape) void;

    pub const removeChildByIndex = cbtShapeCompoundRemoveChildByIndex;
    extern fn cbtShapeCompoundRemoveChildByIndex(cshape: CompoundShape, index: i32) void;

    pub const getNumChilds = cbtShapeCompoundGetNumChilds;
    extern fn cbtShapeCompoundGetNumChilds(cshape: CompoundShape) i32;

    pub const getChild = cbtShapeCompoundGetChild;
    extern fn cbtShapeCompoundGetChild(cshape: CompoundShape, index: i32) Shape;

    pub const getChildTransform = cbtShapeCompoundGetChildTransform;
    extern fn cbtShapeCompoundGetChildTransform(
        cshape: CompoundShape,
        index: i32,
        local_transform: *[12]f32,
    ) void;
};

pub fn initTriangleMeshShape() TriangleMeshShape {
    const trimesh = TriangleMeshShapeImpl.alloc();
    trimesh.createBegin();
    return trimesh;
}

const TriangleMeshShapeImpl = opaque {
    pub usingnamespace ShapeFunctions(TriangleMeshShape);

    pub fn finish(trimesh: TriangleMeshShape) void {
        trimesh.createEnd();
    }

    fn alloc() TriangleMeshShape {
        return @as(TriangleMeshShape, @ptrCast(ShapeImpl.alloc(.trimesh)));
    }

    pub const addIndexVertexArray = cbtShapeTriMeshAddIndexVertexArray;
    extern fn cbtShapeTriMeshAddIndexVertexArray(
        trimesh: TriangleMeshShape,
        num_triangles: u32,
        triangles_base: *const anyopaque,
        triangle_stride: u32,
        num_vertices: u32,
        vertices_base: *const anyopaque,
        vertex_stride: u32,
    ) void;

    pub const createBegin = cbtShapeTriMeshCreateBegin;
    extern fn cbtShapeTriMeshCreateBegin(trimesh: TriangleMeshShape) void;

    pub const createEnd = cbtShapeTriMeshCreateEnd;
    extern fn cbtShapeTriMeshCreateEnd(trimesh: TriangleMeshShape) void;
};

pub const BodyActivationState = enum(c_int) {
    active = 1,
    sleeping = 2,
    wants_deactivation = 3,
    deactivation_disabled = 4,
    simulation_disabled = 5,
};

pub const CollisionFlags = packed struct(c_int) {
    static_object: bool = false,
    kinematic_object: bool = false,
    no_contact_response: bool = false,
    custom_material_callback: bool = false,
    character_object: bool = false,
    disable_visualize_object: bool = false,
    disable_spu_collision_processing: bool = false,
    has_contact_stiffness_damping: bool = false,
    has_custom_debug_rendering_color: bool = false,
    has_friction_anchor: bool = false,
    has_collision_sound_trigger: bool = false,
    _padding: u21 = 0,
};

pub fn initBody(
    mass: f32,
    transform: *const [12]f32,
    shape: Shape,
) Body {
    const body = BodyImpl.alloc();
    body.create(mass, transform, shape);
    return body;
}

const BodyImpl = opaque {
    pub fn deinit(body: Body) void {
        body.destroy();
        body.dealloc();
    }

    pub const alloc = cbtBodyAllocate;
    extern fn cbtBodyAllocate() Body;

    pub const dealloc = cbtBodyDeallocate;
    extern fn cbtBodyDeallocate(body: Body) void;

    pub const create = cbtBodyCreate;
    extern fn cbtBodyCreate(
        body: Body,
        mass: f32,
        transform: *const [12]f32,
        shape: Shape,
    ) void;

    pub const destroy = cbtBodyDestroy;
    extern fn cbtBodyDestroy(body: Body) void;

    pub const isCreated = cbtBodyIsCreated;
    extern fn cbtBodyIsCreated(body: Body) bool;

    pub const setShape = cbtBodySetShape;
    extern fn cbtBodySetShape(body: Body, shape: Shape) void;

    pub const getShape = cbtBodyGetShape;
    extern fn cbtBodyGetShape(body: Body) Shape;

    pub const getMass = cbtBodyGetMass;
    extern fn cbtBodyGetMass(body: Body) f32;

    pub const setRestitution = cbtBodySetRestitution;
    extern fn cbtBodySetRestitution(body: Body, restitution: f32) void;

    pub const getRestitution = cbtBodyGetRestitution;
    extern fn cbtBodyGetRestitution(body: Body) f32;

    pub const setFriction = cbtBodySetFriction;
    extern fn cbtBodySetFriction(body: Body, friction: f32) void;

    pub const setRollingFriction = cbtBodySetRollingFriction;
    extern fn cbtBodySetRollingFriction(body: Body, friction: f32) void;

    pub const setSpinningFriction = cbtBodySetSpinningFriction;
    extern fn cbtBodySetSpinningFriction(body: Body, friction: f32) void;

    pub const setAnisotropicFriction = cbtBodySetAnisotropicFriction;
    extern fn cbtBodySetAnisotropicFriction(body: Body, friction: f32) void;

    pub const getGraphicsWorldTransform = cbtBodyGetGraphicsWorldTransform;
    extern fn cbtBodyGetGraphicsWorldTransform(
        body: Body,
        transform: *[12]f32,
    ) void;

    pub const getCenterOfMassTransform = cbtBodyGetCenterOfMassTransform;
    extern fn cbtBodyGetCenterOfMassTransform(
        body: Body,
        transform: *[12]f32,
    ) void;

    pub const getInvCenterOfMassTransform = cbtBodyGetInvCenterOfMassTransform;
    extern fn cbtBodyGetInvCenterOfMassTransform(
        body: Body,
        transform: *[12]f32,
    ) void;

    pub const applyCentralImpulse = cbtBodyApplyCentralImpulse;
    extern fn cbtBodyApplyCentralImpulse(body: Body, impulse: *const [3]f32) void;

    pub const applyBodyTorque = cbtBodyApplyTorque;
    extern fn cbtBodyApplyTorque(body: Body, impulse: *const [3]f32) void;

    pub const setUserIndex = cbtBodySetUserIndex;
    extern fn cbtBodySetUserIndex(body: Body, slot: u32, index: i32) void;

    pub const getUserIndex = cbtBodyGetUserIndex;
    extern fn cbtBodyGetUserIndex(body: Body, slot: u32) i32;

    pub const getCcdSweptSphereRadius = cbtBodyGetCcdSweptSphereRadius;
    extern fn cbtBodyGetCcdSweptSphereRadius(body: Body) f32;

    pub const setCcdSweptSphereRadius = cbtBodySetCcdSweptSphereRadius;
    extern fn cbtBodySetCcdSweptSphereRadius(body: Body, radius: f32) void;

    pub const getCcdMotionThreshold = cbtBodyGetCcdMotionThreshold;
    extern fn cbtBodyGetCcdMotionThreshold(body: Body) f32;

    pub const setCcdMotionThreshold = cbtBodySetCcdMotionThreshold;
    extern fn cbtBodySetCcdMotionThreshold(body: Body, threshold: f32) void;

    pub fn setCollisionFlags(body: Body, flags: CollisionFlags) void {
        cbtBodySetCollisionFlags(body, @as(c_int, @bitCast(flags)));
    }
    extern fn cbtBodySetCollisionFlags(body: Body, flags: c_int) void;

    pub const setMassProps = cbtBodySetMassProps;
    extern fn cbtBodySetMassProps(body: Body, mass: f32, inertia: *const [3]f32) void;

    pub const setDamping = cbtBodySetDamping;
    extern fn cbtBodySetDamping(body: Body, linear: f32, angular: f32) void;

    pub const getLinearDamping = cbtBodyGetLinearDamping;
    extern fn cbtBodyGetLinearDamping(body: Body) f32;

    pub const getAngularDamping = cbtBodyGetAngularDamping;
    extern fn cbtBodyGetAngularDamping(body: Body) f32;

    pub const getActivationState = cbtBodyGetActivationState;
    extern fn cbtBodyGetActivationState(body: Body) BodyActivationState;

    pub const setActivationState = cbtBodySetActivationState;
    extern fn cbtBodySetActivationState(body: Body, state: BodyActivationState) void;

    pub const forceActivationState = cbtBodyForceActivationState;
    extern fn cbtBodyForceActivationState(body: Body, state: BodyActivationState) void;

    pub const getDeactivationTime = cbtBodyGetDeactivationTime;
    extern fn cbtBodyGetDeactivationTime(body: Body) f32;

    pub const setDeactivationTime = cbtBodySetDeactivationTime;
    extern fn cbtBodySetDeactivationTime(body: Body, time: f32) void;

    pub const isActive = cbtBodyIsActive;
    extern fn cbtBodyIsActive(body: Body) bool;

    pub const isInWorld = cbtBodyIsInWorld;
    extern fn cbtBodyIsInWorld(body: Body) bool;

    pub const isStatic = cbtBodyIsStatic;
    extern fn cbtBodyIsStatic(body: Body) bool;

    pub const isKinematic = cbtBodyIsKinematic;
    extern fn cbtBodyIsKinematic(body: Body) bool;

    pub const isStaticOrKinematic = cbtBodyIsStaticOrKinematic;
    extern fn cbtBodyIsStaticOrKinematic(body: Body) bool;
};

pub const ConstraintType = enum(c_int) {
    _dummy = 0, // TODO: Self-hosted bug.
    point2point = 3,
};

const ConstraintImpl = opaque {
    pub const getFixedBody = cbtConGetFixedBody;
    extern fn cbtConGetFixedBody() Body;

    pub const destroyFixedBody = cbtConDestroyFixedBody;
    extern fn cbtConDestroyFixedBody() void;

    pub const alloc = cbtConAllocate;
    extern fn cbtConAllocate(ctype: ConstraintType) Constraint;

    pub const dealloc = cbtConDeallocate;
    extern fn cbtConDeallocate(con: Constraint) void;

    pub const destroy = cbtConDestroy;
    extern fn cbtConDestroy(con: Constraint) void;

    pub const isCreated = cbtConIsCreated;
    extern fn cbtConIsCreated(con: Constraint) bool;

    pub const getType = cbtConGetType;
    extern fn cbtConGetType(con: Constraint) ConstraintType;

    pub const setEnabled = cbtConSetEnabled;
    extern fn cbtConSetEnabled(con: Constraint, enabled: bool) void;

    pub const isEnabled = cbtConIsEnabled;
    extern fn cbtConIsEnabled(con: Constraint) bool;

    pub const getBodyA = cbtConGetBodyA;
    extern fn cbtConGetBodyA(con: Constraint) Body;

    pub const getBodyB = cbtConGetBodyB;
    extern fn cbtConGetBodyB(con: Constraint) Body;

    pub const setDebugDrawSize = cbtConSetDebugDrawSize;
    extern fn cbtConSetDebugDrawSize(con: Constraint, size: f32) void;
};

fn ConstraintFunctions(comptime T: type) type {
    return struct {
        pub fn asConstraint(con: T) Constraint {
            return @as(Constraint, @ptrCast(con));
        }
        pub fn dealloc(con: T) void {
            con.asConstraint().dealloc();
        }
        pub fn destroy(con: T) void {
            con.asConstraint().destroy();
        }
        pub fn getType(con: T) ConstraintType {
            return con.asConstraint().getType();
        }
        pub fn isCreated(con: T) bool {
            return con.asConstraint().isCreated();
        }
        pub fn setEnabled(con: T, enabled: bool) void {
            con.asConstraint().setEnabled(enabled);
        }
        pub fn isEnabled(con: T) bool {
            return con.asConstraint().isEnabled();
        }
        pub fn getBodyA(con: T) Body {
            return con.asConstraint().getBodyA();
        }
        pub fn getBodyB(con: T) Body {
            return con.asConstraint().getBodyB();
        }
        pub fn setDebugDrawSize(con: T, size: f32) void {
            con.asConstraint().setDebugDrawSize(size);
        }
    };
}

pub fn allocPoint2PointConstraint() Point2PointConstraint {
    return Point2PointConstraintImpl.alloc();
}

const Point2PointConstraintImpl = opaque {
    pub usingnamespace ConstraintFunctions(Point2PointConstraint);

    fn alloc() Point2PointConstraint {
        return @as(Point2PointConstraint, @ptrCast(ConstraintImpl.alloc(.point2point)));
    }

    pub const create1 = cbtConPoint2PointCreate1;
    extern fn cbtConPoint2PointCreate1(
        con: Point2PointConstraint,
        body: Body,
        pivot: *const [3]f32,
    ) void;

    pub const create2 = cbtConPoint2PointCreate2;
    extern fn cbtConPoint2PointCreate2(
        con: Point2PointConstraint,
        body_a: Body,
        body_b: Body,
        pivot_a: *const [3]f32,
        pivot_b: *const [3]f32,
    ) void;

    pub const setPivotA = cbtConPoint2PointSetPivotA;
    extern fn cbtConPoint2PointSetPivotA(con: Point2PointConstraint, pivot: *const [3]f32) void;

    pub const setPivotB = cbtConPoint2PointSetPivotB;
    extern fn cbtConPoint2PointSetPivotB(con: Point2PointConstraint, pivot: *const [3]f32) void;

    pub const getPivotA = cbtConPoint2PointGetPivotA;
    extern fn cbtConPoint2PointGetPivotA(con: Point2PointConstraint, pivot: *[3]f32) void;

    pub const getPivotB = cbtConPoint2PointGetPivotB;
    extern fn cbtConPoint2PointGetPivotB(con: Point2PointConstraint, pivot: *[3]f32) void;

    pub const setTau = cbtConPoint2PointSetTau;
    extern fn cbtConPoint2PointSetTau(con: Point2PointConstraint, tau: f32) void;

    pub const setDamping = cbtConPoint2PointSetDamping;
    extern fn cbtConPoint2PointSetDamping(con: Point2PointConstraint, damping: f32) void;

    pub const setImpulseClamp = cbtConPoint2PointSetImpulseClamp;
    extern fn cbtConPoint2PointSetImpulseClamp(con: Point2PointConstraint, impulse_clamp: f32) void;
};

pub const DebugMode = packed struct {
    draw_wireframe: bool = false,
    draw_aabb: bool = false,

    _pad0: u14 = 0,
    _pad1: u16 = 0,

    pub const disabled = @as(DebugMode, @bitCast(~@as(u32, 0)));
    pub const user_only = @as(DebugMode, @bitCast(@as(u32, 0)));

    comptime {
        std.debug.assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const DebugDraw = extern struct {
    const DrawLine1Fn = *const fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
    ) callconv(.C) void;

    const DrawLine2Fn = *const fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
    ) callconv(.C) void;

    const DrawContactPointFn = *const fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        f32,
        *const [3]f32,
    ) callconv(.C) void;

    drawLine1: DrawLine1Fn,
    drawLine2: ?DrawLine2Fn,
    drawContactPoint: ?DrawContactPointFn,
    context: ?*anyopaque,
};

pub const DebugDrawer = struct {
    lines: std.ArrayList(Vertex),

    pub const Vertex = struct {
        position: [3]f32,
        color: u32,
    };

    pub fn init(alloc: std.mem.Allocator) DebugDrawer {
        return .{ .lines = std.ArrayList(Vertex).init(alloc) };
    }

    pub fn deinit(debug: *DebugDrawer) void {
        debug.lines.deinit();
        debug.* = undefined;
    }

    pub fn getDebugDraw(debug: *DebugDrawer) DebugDraw {
        return .{
            .drawLine1 = drawLine1,
            .drawLine2 = drawLine2,
            .drawContactPoint = null,
            .context = debug,
        };
    }

    fn drawLine1(
        context: ?*anyopaque,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color: *const [3]f32,
    ) callconv(.C) void {
        const debug = @as(
            *DebugDrawer,
            @ptrCast(@alignCast(context.?)),
        );

        const r = @as(u32, @intFromFloat(color[0] * 255.0));
        const g = @as(u32, @intFromFloat(color[1] * 255.0)) << 8;
        const b = @as(u32, @intFromFloat(color[2] * 255.0)) << 16;
        const rgb = r | g | b;

        debug.lines.append(
            .{ .position = .{ p0[0], p0[1], p0[2] }, .color = rgb },
        ) catch unreachable;
        debug.lines.append(
            .{ .position = .{ p1[0], p1[1], p1[2] }, .color = rgb },
        ) catch unreachable;
    }

    fn drawLine2(
        context: ?*anyopaque,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color0: *const [3]f32,
        color1: *const [3]f32,
    ) callconv(.C) void {
        const debug = @as(
            *DebugDrawer,
            @ptrCast(@alignCast(context.?)),
        );

        const r0 = @as(u32, @intFromFloat(color0[0] * 255.0));
        const g0 = @as(u32, @intFromFloat(color0[1] * 255.0)) << 8;
        const b0 = @as(u32, @intFromFloat(color0[2] * 255.0)) << 16;
        const rgb0 = r0 | g0 | b0;

        const r1 = @as(u32, @intFromFloat(color1[0] * 255.0));
        const g1 = @as(u32, @intFromFloat(color1[1] * 255.0)) << 8;
        const b1 = @as(u32, @intFromFloat(color1[2] * 255.0)) << 16;
        const rgb1 = r1 | g1 | b1;

        debug.lines.append(
            .{ .position = .{ p0[0], p0[1], p0[2] }, .color = rgb0 },
        ) catch unreachable;
        debug.lines.append(
            .{ .position = .{ p1[0], p1[1], p1[2] }, .color = rgb1 },
        ) catch unreachable;
    }
};

test {
    std.testing.refAllDeclsRecursive(@This());
}

test "zbullet.world.gravity" {
    const zm = @import("zmath");
    init(std.testing.allocator);
    defer deinit();

    const world = initWorld();
    defer world.deinit();

    world.setGravity(&.{ 0.0, -10.0, 0.0 });

    const num_substeps = world.stepSimulation(1.0 / 60.0, .{});
    try expect(num_substeps == 1);

    var gravity: [3]f32 = undefined;
    world.getGravity(&gravity);
    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);

    world.setGravity(zm.arr3Ptr(&zm.f32x4(1.0, 2.0, 3.0, 0.0)));
    world.getGravity(&gravity);
    try expect(gravity[0] == 1.0 and gravity[1] == 2.0 and gravity[2] == 3.0);
}

test "zbullet.shape.box" {
    init(std.testing.allocator);
    defer deinit();
    {
        const box = initBoxShape(&.{ 4.0, 4.0, 4.0 });
        defer box.deinit();
        try expect(box.isCreated());
        try expect(box.getType() == .box);
        box.setMargin(0.1);
        try expect(box.getMargin() == 0.1);

        var half_extents: [3]f32 = undefined;
        box.getHalfExtentsWithoutMargin(&half_extents);
        try expect(half_extents[0] == 3.9 and half_extents[1] == 3.9 and
            half_extents[2] == 3.9);

        box.getHalfExtentsWithMargin(&half_extents);
        try expect(half_extents[0] == 4.0 and half_extents[1] == 4.0 and
            half_extents[2] == 4.0);

        try expect(box.isPolyhedral() == true);
        try expect(box.isConvex() == true);

        box.setUserIndex(0, 123);
        try expect(box.getUserIndex(0) == 123);

        box.setUserPointer(null);
        try expect(box.getUserPointer() == null);

        box.setUserPointer(&half_extents);
        try expect(box.getUserPointer() == @as(*anyopaque, @ptrCast(&half_extents)));

        const shape = box.asShape();
        try expect(shape.getType() == .box);
        try expect(shape.isCreated());
    }
    {
        const box = BoxShapeImpl.alloc();
        defer box.dealloc();
        try expect(box.isCreated() == false);

        box.create(&.{ 1.0, 2.0, 3.0 });
        defer box.destroy();
        try expect(box.getType() == .box);
        try expect(box.isCreated() == true);
    }
}

test "zbullet.shape.sphere" {
    init(std.testing.allocator);
    defer deinit();
    {
        const sphere = initSphereShape(3.0);
        defer sphere.deinit();
        try expect(sphere.isCreated());
        try expect(sphere.getType() == .sphere);
        sphere.setMargin(0.1);
        try expect(sphere.getMargin() == 3.0); // For spheres margin == radius.

        try expect(sphere.getRadius() == 3.0);

        sphere.setUnscaledRadius(1.0);
        try expect(sphere.getRadius() == 1.0);

        const shape = sphere.asShape();
        try expect(shape.getType() == .sphere);
        try expect(shape.isCreated());
    }
    {
        const sphere = SphereShapeImpl.alloc();
        errdefer sphere.dealloc();
        try expect(sphere.isCreated() == false);

        sphere.create(1.0);
        try expect(sphere.getType() == .sphere);
        try expect(sphere.isCreated() == true);
        try expect(sphere.getRadius() == 1.0);

        sphere.destroy();
        try expect(sphere.isCreated() == false);

        sphere.create(2.0);
        try expect(sphere.getType() == .sphere);
        try expect(sphere.isCreated() == true);
        try expect(sphere.getRadius() == 2.0);

        sphere.destroy();
        try expect(sphere.isCreated() == false);
        sphere.dealloc();
    }
}

test "zbullet.shape.capsule" {
    init(std.testing.allocator);
    defer deinit();
    const capsule = initCapsuleShape(2.0, 1.0, .y);
    defer capsule.deinit();
    try expect(capsule.isCreated());
    try expect(capsule.getType() == .capsule);
    capsule.setMargin(0.1);
    try expect(capsule.getMargin() == 2.0); // For capsules margin == radius.
    try expect(capsule.getRadius() == 2.0);
    try expect(capsule.getHalfHeight() == 0.5);
    try expect(capsule.getUpAxis() == .y);
}

test "zbullet.shape.cylinder" {
    init(std.testing.allocator);
    defer deinit();
    const cylinder = initCylinderShape(&.{ 1.0, 2.0, 3.0 }, .y);
    defer cylinder.deinit();
    try expect(cylinder.isCreated());
    try expect(cylinder.getType() == .cylinder);
    cylinder.setMargin(0.1);
    try expect(cylinder.getMargin() == 0.1);
    try expect(cylinder.getUpAxis() == .y);

    try expect(cylinder.isPolyhedral() == false);
    try expect(cylinder.isConvex() == true);

    var half_extents: [3]f32 = undefined;
    cylinder.getHalfExtentsWithoutMargin(&half_extents);
    try expect(half_extents[0] == 0.9 and half_extents[1] == 1.9 and
        half_extents[2] == 2.9);

    cylinder.getHalfExtentsWithMargin(&half_extents);
    try expect(half_extents[0] == 1.0 and half_extents[1] == 2.0 and
        half_extents[2] == 3.0);
}

test "zbullet.shape.compound" {
    const zm = @import("zmath");
    init(std.testing.allocator);
    defer deinit();

    const cshape = initCompoundShape(.{});
    defer cshape.deinit();
    try expect(cshape.isCreated());
    try expect(cshape.getType() == .compound);

    try expect(cshape.isPolyhedral() == false);
    try expect(cshape.isConvex() == false);
    try expect(cshape.isCompound() == true);

    const sphere = initSphereShape(3.0);
    defer sphere.deinit();

    const box = initBoxShape(&.{ 1.0, 2.0, 3.0 });
    defer box.deinit();

    cshape.addChild(
        &zm.matToArr43(zm.translation(1.0, 2.0, 3.0)),
        sphere.asShape(),
    );
    cshape.addChild(
        &zm.matToArr43(zm.translation(-1.0, -2.0, -3.0)),
        box.asShape(),
    );
    try expect(cshape.getNumChilds() == 2);

    try expect(cshape.getChild(0) == sphere.asShape());
    try expect(cshape.getChild(1) == box.asShape());

    var transform: [12]f32 = undefined;
    cshape.getChildTransform(1, &transform);

    const m = zm.loadMat43(transform[0..]);
    try zm.expectVecApproxEqAbs(m[0], zm.f32x4(1.0, 0.0, 0.0, 0.0), 0.0001);
    try zm.expectVecApproxEqAbs(m[1], zm.f32x4(0.0, 1.0, 0.0, 0.0), 0.0001);
    try zm.expectVecApproxEqAbs(m[2], zm.f32x4(0.0, 0.0, 1.0, 0.0), 0.0001);
    try zm.expectVecApproxEqAbs(m[3], zm.f32x4(-1.0, -2.0, -3.0, 1.0), 0.0001);

    cshape.removeChild(sphere.asShape());
    try expect(cshape.getNumChilds() == 1);
    try expect(cshape.getChild(0) == box.asShape());

    cshape.removeChildByIndex(0);
    try expect(cshape.getNumChilds() == 0);
}

test "zbullet.shape.trimesh" {
    init(std.testing.allocator);
    defer deinit();
    const trimesh = initTriangleMeshShape();
    const triangles = [3]u32{ 0, 1, 2 };
    const vertices = [_]f32{0.0} ** 9;
    trimesh.addIndexVertexArray(
        1, // num_triangles
        &triangles, // triangles_base
        12, // triangle_stride
        3, // num_vertices
        &vertices, // vertices_base
        12, // vertex_stride
    );
    trimesh.finish();
    defer trimesh.deinit();
    try expect(trimesh.isCreated());
    try expect(trimesh.isNonMoving());
    try expect(trimesh.getType() == .trimesh);
}

test "zbullet.body.basic" {
    init(std.testing.allocator);
    defer deinit();
    {
        const world = initWorld();
        defer world.deinit();

        const sphere = initSphereShape(3.0);
        defer sphere.deinit();

        const transform = [12]f32{
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            2.0, 2.0, 2.0,
        };
        const body = initBody(1.0, &transform, sphere.asShape());
        defer body.deinit();
        try expect(body.isCreated() == true);
        try expect(body.getShape() == sphere.asShape());

        world.addBody(body);
        try expect(world.getNumBodies() == 1);
        try expect(world.getBody(0) == body);

        world.removeBody(body);
        try expect(world.getNumBodies() == 0);
    }
    {
        const zm = @import("zmath");

        const sphere = initSphereShape(3.0);
        defer sphere.deinit();

        var transform: [12]f32 = undefined;
        zm.storeMat43(transform[0..], zm.translation(2.0, 3.0, 4.0));

        const body = initBody(1.0, &transform, sphere.asShape());
        errdefer body.deinit();

        try expect(body.isCreated() == true);
        try expect(body.getShape() == sphere.asShape());

        body.destroy();
        try expect(body.isCreated() == false);

        body.dealloc();
    }
    {
        const zm = @import("zmath");

        const sphere = initSphereShape(3.0);
        defer sphere.deinit();

        const body = initBody(
            0.0, // static body
            &zm.matToArr43(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body.deinit();

        var transform: [12]f32 = undefined;
        body.getGraphicsWorldTransform(&transform);

        const m = zm.loadMat43(transform[0..]);
        try zm.expectVecApproxEqAbs(m[0], zm.f32x4(1.0, 0.0, 0.0, 0.0), 0.0001);
        try zm.expectVecApproxEqAbs(m[1], zm.f32x4(0.0, 1.0, 0.0, 0.0), 0.0001);
        try zm.expectVecApproxEqAbs(m[2], zm.f32x4(0.0, 0.0, 1.0, 0.0), 0.0001);
        try zm.expectVecApproxEqAbs(m[3], zm.f32x4(2.0, 3.0, 4.0, 1.0), 0.0001);
    }
}

test "zbullet.constraint.point2point" {
    const zm = @import("zmath");
    init(std.testing.allocator);
    defer deinit();
    {
        const world = initWorld();
        defer world.deinit();

        const sphere = initSphereShape(3.0);
        defer sphere.deinit();

        const body = initBody(
            1.0,
            &zm.matToArr43(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body.deinit();

        const p2p = allocPoint2PointConstraint();
        defer p2p.dealloc();

        try expect(p2p.getType() == .point2point);
        try expect(p2p.isCreated() == false);

        p2p.create1(body, &.{ 1.0, 2.0, 3.0 });
        defer p2p.destroy();

        try expect(p2p.getType() == .point2point);
        try expect(p2p.isCreated() == true);
        try expect(p2p.isEnabled() == true);
        try expect(p2p.getBodyA() == body);
        try expect(p2p.getBodyB() == ConstraintImpl.getFixedBody());

        var pivot: [3]f32 = undefined;
        p2p.getPivotA(&pivot);
        try expect(pivot[0] == 1.0 and pivot[1] == 2.0 and pivot[2] == 3.0);

        p2p.setPivotA(&.{ -1.0, -2.0, -3.0 });
        p2p.getPivotA(&pivot);
        try expect(pivot[0] == -1.0 and pivot[1] == -2.0 and pivot[2] == -3.0);
    }
    {
        const world = initWorld();
        defer world.deinit();

        const sphere = initSphereShape(3.0);
        defer sphere.deinit();

        const body0 = initBody(
            1.0,
            &zm.matToArr43(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body0.deinit();

        const body1 = initBody(
            1.0,
            &zm.matToArr43(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body1.deinit();

        const p2p = allocPoint2PointConstraint();
        defer p2p.dealloc();

        p2p.create2(body0, body1, &.{ 1.0, 2.0, 3.0 }, &.{ -1.0, -2.0, -3.0 });
        defer p2p.destroy();

        try expect(p2p.isEnabled() == true);
        try expect(p2p.getBodyA() == body0);
        try expect(p2p.getBodyB() == body1);
        try expect(p2p.getType() == .point2point);
        try expect(p2p.isCreated() == true);

        var pivot: [3]f32 = undefined;

        p2p.getPivotA(&pivot);
        try expect(pivot[0] == 1.0 and pivot[1] == 2.0 and pivot[2] == 3.0);

        p2p.getPivotB(&pivot);
        try expect(pivot[0] == -1.0 and pivot[1] == -2.0 and pivot[2] == -3.0);

        world.addConstraint(p2p.asConstraint(), false);
        try expect(world.getNumConstraints() == 1);
        try expect(world.getConstraint(0) == p2p.asConstraint());

        world.removeConstraint(p2p.asConstraint());
        try expect(world.getNumConstraints() == 0);
    }
}
