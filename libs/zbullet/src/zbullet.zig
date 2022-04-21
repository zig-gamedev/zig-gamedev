// zbullet - version 0.1
// Zig bindings for Bullet Physics SDK

const std = @import("std");
const Mutex = std.Thread.Mutex;
const expect = std.testing.expect;

extern fn cbtAlignedAllocSetCustomAligned(
    alloc: fn (size: usize, alignment: i32) callconv(.C) ?*anyopaque,
    free: fn (ptr: ?*anyopaque) callconv(.C) void,
) void;

var allocator: ?std.mem.Allocator = null;
var allocations: ?std.AutoHashMap(usize, usize) = null;
var mutex: Mutex = .{};

export fn zbulletAlloc(size: usize, alignment: i32) callconv(.C) ?*anyopaque {
    mutex.lock();
    defer mutex.unlock();

    var slice = allocator.?.allocBytes(
        @intCast(u29, alignment),
        size,
        0,
        @returnAddress(),
    ) catch @panic("zbullet: out of memory");

    allocations.?.put(@ptrToInt(slice.ptr), size) catch
        @panic("zbullet: out of memory");

    return slice.ptr;
}

export fn zbulletFree(ptr: ?*anyopaque) callconv(.C) void {
    if (ptr != null) {
        mutex.lock();
        defer mutex.unlock();

        const size = allocations.?.fetchRemove(@ptrToInt(ptr.?)).?.value;
        const slice = @ptrCast([*]u8, ptr.?)[0..size];
        allocator.?.free(slice);
    }
}

pub fn init(alloc: std.mem.Allocator) void {
    std.debug.assert(allocator == null and allocations == null);
    allocator = alloc;
    allocations = std.AutoHashMap(usize, usize).init(allocator.?);
    allocations.?.ensureTotalCapacity(256) catch @panic("zbullet: out of memory");
    cbtAlignedAllocSetCustomAligned(zbulletAlloc, zbulletFree);
}

pub fn deinit() void {
    allocations.?.deinit();
    allocations = null;
    allocator = null;
}

pub const World = opaque {
    pub fn init(params: struct {}) *const World {
        _ = params;
        std.debug.assert(allocator != null and allocations != null);
        return cbtWorldCreate();
    }
    extern fn cbtWorldCreate() *const World;

    pub fn deinit(world: *const World) void {
        std.debug.assert(world.getNumBodies() == 0);
        cbtWorldDestroy(world);
    }
    extern fn cbtWorldDestroy(world: *const World) void;

    pub const setGravity = cbtWorldSetGravity;
    extern fn cbtWorldSetGravity(world: *const World, gravity: *const [3]f32) void;

    pub const getGravity = cbtWorldGetGravity;
    extern fn cbtWorldGetGravity(world: *const World, gravity: *[3]f32) void;

    pub fn stepSimulation(world: *const World, time_step: f32, params: struct {
        max_sub_steps: u32 = 1,
        fixed_time_step: f32 = 1.0 / 60.0,
    }) u32 {
        return cbtWorldStepSimulation(
            world,
            time_step,
            params.max_sub_steps,
            params.fixed_time_step,
        );
    }
    extern fn cbtWorldStepSimulation(
        world: *const World,
        time_step: f32,
        max_sub_steps: u32,
        fixed_time_step: f32,
    ) u32;

    pub const addBody = cbtWorldAddBody;
    extern fn cbtWorldAddBody(world: *const World, body: *const Body) void;

    pub const removeBody = cbtWorldRemoveBody;
    extern fn cbtWorldRemoveBody(world: *const World, body: *const Body) void;

    pub const getBody = cbtWorldGetBody;
    extern fn cbtWorldGetBody(world: *const World, index: i32) *const Body;

    pub const getNumBodies = cbtWorldGetNumBodies;
    extern fn cbtWorldGetNumBodies(world: *const World) i32;

    pub const addConstraint = cbtWorldAddConstraint;
    extern fn cbtWorldAddConstraint(
        world: *const World,
        con: *const Constraint,
        disable_collision_between_linked_bodies: bool,
    ) void;

    pub const removeConstraint = cbtWorldRemoveConstraint;
    extern fn cbtWorldRemoveConstraint(
        world: *const World,
        con: *const Constraint,
    ) void;

    pub const getConstraint = cbtWorldGetConstraint;
    extern fn cbtWorldGetConstraint(
        world: *const World,
        index: i32,
    ) *const Constraint;

    pub const getNumConstraints = cbtWorldGetNumConstraints;
    extern fn cbtWorldGetNumConstraints(world: *const World) i32;

    pub const debugSetDrawer = cbtWorldDebugSetDrawer;
    extern fn cbtWorldDebugSetDrawer(
        world: *const World,
        debug: *const DebugDraw,
    ) void;

    pub const debugSetMode = cbtWorldDebugSetMode;
    extern fn cbtWorldDebugSetMode(world: *const World, mode: DebugMode) void;

    pub const debugDrawAll = cbtWorldDebugDrawAll;
    extern fn cbtWorldDebugDrawAll(world: *const World) void;

    pub const debugDrawLine1 = cbtWorldDebugDrawLine1;
    extern fn cbtWorldDebugDrawLine1(
        world: *const World,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color: *const [3]f32,
    ) void;

    pub const debugDrawLine2 = cbtWorldDebugDrawLine2;
    extern fn cbtWorldDebugDrawLine2(
        world: *const World,
        p0: *const [3]f32,
        p1: *const [3]f32,
        color0: *const [3]f32,
        color1: *const [3]f32,
    ) void;

    pub const debugDrawSphere = cbtWorldDebugDrawSphere;
    extern fn cbtWorldDebugDrawSphere(
        world: *const World,
        position: *const [3]f32,
        radius: f32,
        color: *const [3]f32,
    ) void;
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

pub const Shape = opaque {
    pub const allocate = cbtShapeAllocate;
    extern fn cbtShapeAllocate(stype: ShapeType) *const Shape;

    pub const deallocate = cbtShapeDeallocate;
    extern fn cbtShapeDeallocate(shape: *const Shape) void;

    pub fn deinit(shape: *const Shape) void {
        shape.destroy();
        shape.deallocate();
    }

    pub fn destroy(shape: *const Shape) void {
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
    extern fn cbtShapeDestroy(shape: *const Shape) void;
    extern fn cbtShapeTriMeshDestroy(shape: *const Shape) void;

    pub const isCreated = cbtShapeIsCreated;
    extern fn cbtShapeIsCreated(shape: *const Shape) bool;

    pub const getType = cbtShapeGetType;
    extern fn cbtShapeGetType(shape: *const Shape) ShapeType;

    pub const setMargin = cbtShapeSetMargin;
    extern fn cbtShapeSetMargin(shape: *const Shape, margin: f32) void;

    pub const getMargin = cbtShapeGetMargin;
    extern fn cbtShapeGetMargin(shape: *const Shape) f32;

    pub const isPolyhedral = cbtShapeIsPolyhedral;
    extern fn cbtShapeIsPolyhedral(shape: *const Shape) bool;

    pub const isConvex2d = cbtShapeIsConvex2d;
    extern fn cbtShapeIsConvex2d(shape: *const Shape) bool;

    pub const isConvex = cbtShapeIsConvex;
    extern fn cbtShapeIsConvex(shape: *const Shape) bool;

    pub const isNonMoving = cbtShapeIsNonMoving;
    extern fn cbtShapeIsNonMoving(shape: *const Shape) bool;

    pub const isConcave = cbtShapeIsConcave;
    extern fn cbtShapeIsConcave(shape: *const Shape) bool;

    pub const isCompound = cbtShapeIsCompound;
    extern fn cbtShapeIsCompound(shape: *const Shape) bool;

    pub const calculateLocalInertia = cbtShapeCalculateLocalInertia;
    extern fn cbtShapeCalculateLocalInertia(
        shape: *const Shape,
        mass: f32,
        inertia: *[3]f32,
    ) void;

    pub const setUserPointer = cbtShapeSetUserPointer;
    extern fn cbtShapeSetUserPointer(shape: *const Shape, ptr: ?*anyopaque) void;

    pub const getUserPointer = cbtShapeGetUserPointer;
    extern fn cbtShapeGetUserPointer(shape: *const Shape) ?*anyopaque;

    pub const setUserIndex = cbtShapeSetUserIndex;
    extern fn cbtShapeSetUserIndex(shape: *const Shape, slot: u32, index: i32) void;

    pub const getUserIndex = cbtShapeGetUserIndex;
    extern fn cbtShapeGetUserIndex(shape: *const Shape, slot: u32) i32;
};

fn ShapeFunctions(comptime T: type) type {
    return struct {
        pub fn asShape(shape: *const T) *const Shape {
            return @ptrCast(*const Shape, shape);
        }
        pub fn deallocate(shape: *const T) void {
            shape.asShape().deallocate();
        }
        pub fn destroy(shape: *const T) void {
            shape.asShape().destroy();
        }
        pub fn deinit(shape: *const T) void {
            shape.asShape().deinit();
        }
        pub fn isCreated(shape: *const T) bool {
            return shape.asShape().isCreated();
        }
        pub fn getType(shape: *const T) ShapeType {
            return shape.asShape().getType();
        }
        pub fn setMargin(shape: *const T, margin: f32) void {
            shape.asShape().setMargin(margin);
        }
        pub fn getMargin(shape: *const T) f32 {
            return shape.asShape().getMargin();
        }
        pub fn isPolyhedral(shape: *const T) bool {
            return shape.asShape().isPolyhedral();
        }
        pub fn isConvex2d(shape: *const T) bool {
            return shape.asShape().isConvex2d();
        }
        pub fn isConvex(shape: *const T) bool {
            return shape.asShape().isConvex();
        }
        pub fn isNonMoving(shape: *const T) bool {
            return shape.asShape().isNonMoving();
        }
        pub fn isConcave(shape: *const T) bool {
            return shape.asShape().isConcave();
        }
        pub fn isCompound(shape: *const T) bool {
            return shape.asShape().isCompound();
        }
        pub fn calculateLocalInertia(
            shape: *const Shape,
            mass: f32,
            inertia: *[3]f32,
        ) void {
            shape.asShape().calculateLocalInertia(shape, mass, inertia);
        }
        pub fn setUserPointer(shape: *const T, ptr: ?*anyopaque) void {
            shape.asShape().setUserPointer(ptr);
        }
        pub fn getUserPointer(shape: *const T) ?*anyopaque {
            return shape.asShape().getUserPointer();
        }
        pub fn setUserIndex(shape: *const T, slot: u32, index: i32) void {
            shape.asShape().setUserIndex(slot, index);
        }
        pub fn getUserIndex(shape: *const T, slot: u32) i32 {
            return shape.asShape().getUserIndex(slot);
        }
    };
}

pub const BoxShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init(half_extents: *const [3]f32) *const BoxShape {
        const box = allocate();
        box.create(half_extents);
        return box;
    }

    pub fn allocate() *const BoxShape {
        return @ptrCast(*const BoxShape, Shape.allocate(.box));
    }

    pub const create = cbtShapeBoxCreate;
    extern fn cbtShapeBoxCreate(
        box: *const BoxShape,
        half_extents: *const [3]f32,
    ) void;

    pub const getHalfExtentsWithoutMargin = cbtShapeBoxGetHalfExtentsWithoutMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithoutMargin(
        box: *const BoxShape,
        half_extents: *[3]f32,
    ) void;

    pub const getHalfExtentsWithMargin = cbtShapeBoxGetHalfExtentsWithMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithMargin(
        box: *const BoxShape,
        half_extents: *[3]f32,
    ) void;
};

pub const SphereShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init(radius: f32) *const SphereShape {
        const sphere = allocate();
        sphere.create(radius);
        return sphere;
    }

    pub fn allocate() *const SphereShape {
        return @ptrCast(*const SphereShape, Shape.allocate(.sphere));
    }

    pub const create = cbtShapeSphereCreate;
    extern fn cbtShapeSphereCreate(sphere: *const SphereShape, radius: f32) void;

    pub const getRadius = cbtShapeSphereGetRadius;
    extern fn cbtShapeSphereGetRadius(sphere: *const SphereShape) f32;

    pub const setUnscaledRadius = cbtShapeSphereSetUnscaledRadius;
    extern fn cbtShapeSphereSetUnscaledRadius(
        sphere: *const SphereShape,
        radius: f32,
    ) void;
};

pub const CapsuleShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init(radius: f32, height: f32, upaxis: Axis) *const CapsuleShape {
        const capsule = allocate();
        capsule.create(radius, height, upaxis);
        return capsule;
    }

    pub fn allocate() *const CapsuleShape {
        return @ptrCast(*const CapsuleShape, Shape.allocate(.capsule));
    }

    pub const create = cbtShapeCapsuleCreate;
    extern fn cbtShapeCapsuleCreate(
        capsule: *const CapsuleShape,
        radius: f32,
        height: f32,
        upaxis: Axis,
    ) void;

    pub const getUpAxis = cbtShapeCapsuleGetUpAxis;
    extern fn cbtShapeCapsuleGetUpAxis(capsule: *const CapsuleShape) Axis;

    pub const getHalfHeight = cbtShapeCapsuleGetHalfHeight;
    extern fn cbtShapeCapsuleGetHalfHeight(capsule: *const CapsuleShape) f32;

    pub const getRadius = cbtShapeCapsuleGetRadius;
    extern fn cbtShapeCapsuleGetRadius(capsule: *const CapsuleShape) f32;
};

pub const CylinderShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init(
        half_extents: *const [3]f32,
        upaxis: Axis,
    ) *const CylinderShape {
        const cylinder = allocate();
        cylinder.create(half_extents, upaxis);
        return cylinder;
    }

    pub fn allocate() *const CylinderShape {
        return @ptrCast(*const CylinderShape, Shape.allocate(.cylinder));
    }

    pub const create = cbtShapeCylinderCreate;
    extern fn cbtShapeCylinderCreate(
        cylinder: *const CylinderShape,
        half_extents: *const [3]f32,
        upaxis: Axis,
    ) void;

    pub const getHalfExtentsWithoutMargin =
        cbtShapeCylinderGetHalfExtentsWithoutMargin;
    extern fn cbtShapeCylinderGetHalfExtentsWithoutMargin(
        cylinder: *const CylinderShape,
        half_extents: *[3]f32,
    ) void;

    pub const getHalfExtentsWithMargin = cbtShapeCylinderGetHalfExtentsWithMargin;
    extern fn cbtShapeCylinderGetHalfExtentsWithMargin(
        cylinder: *const CylinderShape,
        half_extents: *[3]f32,
    ) void;

    pub const getUpAxis = cbtShapeCylinderGetUpAxis;
    extern fn cbtShapeCylinderGetUpAxis(capsule: *const CylinderShape) Axis;
};

pub const CompoundShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init(
        params: struct {
            enable_dynamic_aabb_tree: bool = true,
            initial_child_capacity: u32 = 0,
        },
    ) *const CompoundShape {
        const cshape = allocate();
        cshape.create(
            params.enable_dynamic_aabb_tree,
            params.initial_child_capacity,
        );
        return cshape;
    }

    pub fn allocate() *const CompoundShape {
        return @ptrCast(*const CompoundShape, Shape.allocate(.compound));
    }

    pub const create = cbtShapeCompoundCreate;
    extern fn cbtShapeCompoundCreate(
        cshape: *const CompoundShape,
        enable_dynamic_aabb_tree: bool,
        initial_child_capacity: u32,
    ) void;

    pub const addChild = cbtShapeCompoundAddChild;
    extern fn cbtShapeCompoundAddChild(
        cshape: *const CompoundShape,
        local_transform: *[12]f32,
        child_shape: *const Shape,
    ) void;

    pub const removeChild = cbtShapeCompoundRemoveChild;
    extern fn cbtShapeCompoundRemoveChild(
        cshape: *const CompoundShape,
        child_shape: *const Shape,
    ) void;

    pub const removeChildByIndex = cbtShapeCompoundRemoveChildByIndex;
    extern fn cbtShapeCompoundRemoveChildByIndex(
        cshape: *const CompoundShape,
        index: i32,
    ) void;

    pub const getNumChilds = cbtShapeCompoundGetNumChilds;
    extern fn cbtShapeCompoundGetNumChilds(cshape: *const CompoundShape) i32;

    pub const getChild = cbtShapeCompoundGetChild;
    extern fn cbtShapeCompoundGetChild(
        cshape: *const CompoundShape,
        index: i32,
    ) *const Shape;

    pub const getChildTransform = cbtShapeCompoundGetChildTransform;
    extern fn cbtShapeCompoundGetChildTransform(
        cshape: *const CompoundShape,
        index: i32,
        local_transform: *[12]f32,
    ) void;
};

pub const TriangleMeshShape = opaque {
    usingnamespace ShapeFunctions(@This());

    pub fn init() *const TriangleMeshShape {
        const trimesh = allocate();
        trimesh.createBegin();
        return trimesh;
    }

    pub fn finalize(trimesh: *const TriangleMeshShape) void {
        trimesh.createEnd();
    }

    pub fn allocate() *const TriangleMeshShape {
        return @ptrCast(*const TriangleMeshShape, Shape.allocate(.trimesh));
    }

    pub const addIndexVertexArray = cbtShapeTriMeshAddIndexVertexArray;
    extern fn cbtShapeTriMeshAddIndexVertexArray(
        trimesh: *const TriangleMeshShape,
        num_triangles: u32,
        triangles_base: *const anyopaque,
        triangle_stride: u32,
        num_vertices: u32,
        vertices_base: *const anyopaque,
        vertex_stride: u32,
    ) void;

    pub const createBegin = cbtShapeTriMeshCreateBegin;
    extern fn cbtShapeTriMeshCreateBegin(trimesh: *const TriangleMeshShape) void;

    pub const createEnd = cbtShapeTriMeshCreateEnd;
    extern fn cbtShapeTriMeshCreateEnd(trimesh: *const TriangleMeshShape) void;
};

pub const Body = opaque {
    pub fn init(
        mass: f32,
        transform: *const [12]f32,
        shape: *const Shape,
    ) *const Body {
        const body = allocate();
        body.create(mass, transform, shape);
        return body;
    }

    pub fn deinit(body: *const Body) void {
        body.destroy();
        body.deallocate();
    }

    pub const allocate = cbtBodyAllocate;
    extern fn cbtBodyAllocate() *const Body;

    pub const deallocate = cbtBodyDeallocate;
    extern fn cbtBodyDeallocate(body: *const Body) void;

    pub const create = cbtBodyCreate;
    extern fn cbtBodyCreate(
        body: *const Body,
        mass: f32,
        transform: *const [12]f32,
        shape: *const Shape,
    ) void;

    pub const destroy = cbtBodyDestroy;
    extern fn cbtBodyDestroy(body: *const Body) void;

    pub const isCreated = cbtBodyIsCreated;
    extern fn cbtBodyIsCreated(body: *const Body) bool;

    pub const setShape = cbtBodySetShape;
    extern fn cbtBodySetShape(body: *const Body, shape: *const Shape) void;

    pub const getShape = cbtBodyGetShape;
    extern fn cbtBodyGetShape(body: *const Body) *const Shape;

    pub const getMass = cbtBodyGetMass;
    extern fn cbtBodyGetMass(body: *const Body) f32;

    pub const setRestitution = cbtBodySetRestitution;
    extern fn cbtBodySetRestitution(body: *const Body, restitution: f32) void;

    pub const getRestitution = cbtBodyGetRestitution;
    extern fn cbtBodyGetRestitution(body: *const Body) f32;

    pub const setFriction = cbtBodySetFriction;
    extern fn cbtBodySetFriction(body: *const Body, friction: f32) void;

    pub const getGraphicsWorldTransform = cbtBodyGetGraphicsWorldTransform;
    extern fn cbtBodyGetGraphicsWorldTransform(
        body: *const Body,
        transform: *[12]f32,
    ) void;

    pub const applyCentralImpulse = cbtBodyApplyCentralImpulse;
    extern fn cbtBodyApplyCentralImpulse(body: *const Body, impulse: *[3]f32) void;
};

pub const ConstraintType = enum(c_int) {
    point2point = 3,
};

pub const Constraint = opaque {
    pub const getFixedBody = cbtConGetFixedBody;
    extern fn cbtConGetFixedBody() *const Body;

    pub const allocate = cbtConAllocate;
    extern fn cbtConAllocate(ctype: ConstraintType) *const Constraint;

    pub const deallocate = cbtConDeallocate;
    extern fn cbtConDeallocate(con: *const Constraint) void;

    pub const destroy = cbtConDestroy;
    extern fn cbtConDestroy(con: *const Constraint) void;

    pub const isCreated = cbtConIsCreated;
    extern fn cbtConIsCreated(con: *const Constraint) bool;

    pub const getType = cbtConGetType;
    extern fn cbtConGetType(con: *const Constraint) ConstraintType;

    pub const setEnabled = cbtConSetEnabled;
    extern fn cbtConSetEnabled(con: *const Constraint, enabled: bool) void;

    pub const isEnabled = cbtConIsEnabled;
    extern fn cbtConIsEnabled(con: *const Constraint) bool;

    pub const getBodyA = cbtConGetBodyA;
    extern fn cbtConGetBodyA(con: *const Constraint) *const Body;

    pub const getBodyB = cbtConGetBodyB;
    extern fn cbtConGetBodyB(con: *const Constraint) *const Body;
};

fn ConstraintFunctions(comptime T: type) type {
    return struct {
        pub fn asConstraint(con: *const T) *const Constraint {
            return @ptrCast(*const Constraint, con);
        }
        pub fn deallocate(con: *const T) void {
            con.asConstraint().deallocate();
        }
        pub fn destroy(con: *const T) void {
            con.asConstraint().destroy();
        }
        pub fn getType(con: *const T) ConstraintType {
            return con.asConstraint().getType();
        }
        pub fn isCreated(con: *const T) bool {
            return con.asConstraint().isCreated();
        }
        pub fn setEnabled(con: *const T, enabled: bool) void {
            con.asConstraint().setEnabled(enabled);
        }
        pub fn isEnabled(con: *const T) bool {
            return con.asConstraint().isEnabled();
        }
        pub fn getBodyA(con: *const T) *const Body {
            return con.asConstraint().getBodyA();
        }
        pub fn getBodyB(con: *const T) *const Body {
            return con.asConstraint().getBodyB();
        }
    };
}

pub const Point2PointConstraint = opaque {
    usingnamespace ConstraintFunctions(@This());

    pub fn allocate() *const Point2PointConstraint {
        return @ptrCast(
            *const Point2PointConstraint,
            Constraint.allocate(.point2point),
        );
    }

    pub const create1 = cbtConPoint2PointCreate1;
    extern fn cbtConPoint2PointCreate1(
        con: *const Point2PointConstraint,
        body: *const Body,
        pivot: *const [3]f32,
    ) void;

    pub const create2 = cbtConPoint2PointCreate2;
    extern fn cbtConPoint2PointCreate2(
        con: *const Point2PointConstraint,
        body_a: *const Body,
        body_b: *const Body,
        pivot_a: *const [3]f32,
        pivot_b: *const [3]f32,
    ) void;

    pub const setPivotA = cbtConPoint2PointSetPivotA;
    extern fn cbtConPoint2PointSetPivotA(
        con: *const Point2PointConstraint,
        pivot: *const [3]f32,
    ) void;

    pub const setPivotB = cbtConPoint2PointSetPivotB;
    extern fn cbtConPoint2PointSetPivotB(
        con: *const Point2PointConstraint,
        pivot: *const [3]f32,
    ) void;

    pub const getPivotA = cbtConPoint2PointGetPivotA;
    extern fn cbtConPoint2PointGetPivotA(
        con: *const Point2PointConstraint,
        pivot: *[3]f32,
    ) void;

    pub const getPivotB = cbtConPoint2PointGetPivotB;
    extern fn cbtConPoint2PointGetPivotB(
        con: *const Point2PointConstraint,
        pivot: *[3]f32,
    ) void;

    pub const setTau = cbtConPoint2PointSetPivotB;
    extern fn cbtConPoint2PointSetTau(
        con: *const Point2PointConstraint,
        tau: f32,
    ) void;

    pub const setDamping = cbtConPoint2PointSetDamping;
    extern fn cbtConPoint2PointSetDamping(
        con: *const Point2PointConstraint,
        damping: f32,
    ) void;

    pub const setImpulseClamp = cbtConPoint2PointSetImpulseClamp;
    extern fn cbtConPoint2PointSetImpulseClamp(
        con: *const Point2PointConstraint,
        damping: f32,
    ) void;
};

pub const DebugMode = i32;
pub const dbgmode_disabled: DebugMode = -1;
pub const dbgmode_no_debug: DebugMode = 0;
pub const dbgmode_draw_wireframe: DebugMode = 1;
pub const dbgmode_draw_aabb: DebugMode = 2;

pub const DebugDraw = extern struct {
    drawLine1: fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
    ) callconv(.C) void,
    drawLine2: ?fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
        *const [3]f32,
    ) callconv(.C) void,
    drawContactPoint: ?fn (
        ?*anyopaque,
        *const [3]f32,
        *const [3]f32,
        f32,
        *const [3]f32,
    ) callconv(.C) void,
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
        const debug = @ptrCast(
            *DebugDrawer,
            @alignCast(@alignOf(DebugDrawer), context.?),
        );

        const r = @floatToInt(u32, color[0] * 255.0);
        const g = @floatToInt(u32, color[1] * 255.0) << 8;
        const b = @floatToInt(u32, color[2] * 255.0) << 16;
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
        const debug = @ptrCast(
            *DebugDrawer,
            @alignCast(@alignOf(DebugDrawer), context.?),
        );

        const r0 = @floatToInt(u32, color0[0] * 255.0);
        const g0 = @floatToInt(u32, color0[1] * 255.0) << 8;
        const b0 = @floatToInt(u32, color0[2] * 255.0) << 16;
        const rgb0 = r0 | g0 | b0;

        const r1 = @floatToInt(u32, color1[0] * 255.0);
        const g1 = @floatToInt(u32, color1[1] * 255.0) << 8;
        const b1 = @floatToInt(u32, color1[2] * 255.0) << 16;
        const rgb1 = r1 | g1 | b1;

        debug.lines.append(
            .{ .position = .{ p0[0], p0[1], p0[2] }, .color = rgb0 },
        ) catch unreachable;
        debug.lines.append(
            .{ .position = .{ p1[0], p1[1], p1[2] }, .color = rgb1 },
        ) catch unreachable;
    }
};

test "zbullet.world.gravity" {
    const zm = @import("zmath");
    init(std.testing.allocator);
    defer deinit();

    const world = World.init(.{});
    defer world.deinit();

    world.setGravity(&.{ 0.0, -10.0, 0.0 });

    const num_substeps = world.stepSimulation(1.0 / 60.0, .{});
    try expect(num_substeps == 1);

    var gravity: [3]f32 = undefined;
    world.getGravity(&gravity);
    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);

    world.setGravity(&zm.vec3ToArray(zm.f32x4(1.0, 2.0, 3.0, 0.0)));
    world.getGravity(&gravity);
    try expect(gravity[0] == 1.0 and gravity[1] == 2.0 and gravity[2] == 3.0);
}

test "zbullet.shape.box" {
    init(std.testing.allocator);
    defer deinit();
    {
        const box = BoxShape.init(&.{ 4.0, 4.0, 4.0 });
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
        try expect(box.getUserPointer() == @ptrCast(*anyopaque, &half_extents));

        const shape = box.asShape();
        try expect(shape.getType() == .box);
        try expect(shape.isCreated());
    }
    {
        const box = BoxShape.allocate();
        defer box.deallocate();
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
        const sphere = SphereShape.init(3.0);
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
        const sphere = SphereShape.allocate();
        errdefer sphere.deallocate();
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
        sphere.deallocate();
    }
}

test "zbullet.shape.capsule" {
    init(std.testing.allocator);
    defer deinit();
    const capsule = CapsuleShape.init(2.0, 1.0, .y);
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
    const cylinder = CylinderShape.init(&.{ 1.0, 2.0, 3.0 }, .y);
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

    const cshape = CompoundShape.init(.{});
    defer cshape.deinit();
    try expect(cshape.isCreated());
    try expect(cshape.getType() == .compound);

    try expect(cshape.isPolyhedral() == false);
    try expect(cshape.isConvex() == false);
    try expect(cshape.isCompound() == true);

    const sphere = SphereShape.init(3.0);
    defer sphere.deinit();

    const box = BoxShape.init(&.{ 1.0, 2.0, 3.0 });
    defer box.deinit();

    cshape.addChild(
        &zm.mat43ToArray(zm.translation(1.0, 2.0, 3.0)),
        sphere.asShape(),
    );
    cshape.addChild(
        &zm.mat43ToArray(zm.translation(-1.0, -2.0, -3.0)),
        box.asShape(),
    );
    try expect(cshape.getNumChilds() == 2);

    try expect(cshape.getChild(0) == sphere.asShape());
    try expect(cshape.getChild(1) == box.asShape());

    var transform: [12]f32 = undefined;
    cshape.getChildTransform(1, &transform);

    const m = zm.loadMat43(transform[0..]);
    try expect(zm.approxEqAbs(m[0], zm.f32x4(1.0, 0.0, 0.0, 0.0), 0.0001));
    try expect(zm.approxEqAbs(m[1], zm.f32x4(0.0, 1.0, 0.0, 0.0), 0.0001));
    try expect(zm.approxEqAbs(m[2], zm.f32x4(0.0, 0.0, 1.0, 0.0), 0.0001));
    try expect(zm.approxEqAbs(m[3], zm.f32x4(-1.0, -2.0, -3.0, 1.0), 0.0001));

    cshape.removeChild(sphere.asShape());
    try expect(cshape.getNumChilds() == 1);
    try expect(cshape.getChild(0) == box.asShape());

    cshape.removeChildByIndex(0);
    try expect(cshape.getNumChilds() == 0);
}

test "zbullet.shape.trimesh" {
    init(std.testing.allocator);
    defer deinit();
    const trimesh = TriangleMeshShape.init();
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
    trimesh.finalize();
    defer trimesh.deinit();
    try expect(trimesh.isCreated());
    try expect(trimesh.isNonMoving());
    try expect(trimesh.getType() == .trimesh);
}

test "zbullet.body.basic" {
    init(std.testing.allocator);
    defer deinit();
    {
        const world = World.init(.{});
        defer world.deinit();

        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();

        const transform = [12]f32{
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            2.0, 2.0, 2.0,
        };
        const body = Body.init(1.0, &transform, sphere.asShape());
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

        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();

        var transform: [12]f32 = undefined;
        zm.storeMat43(transform[0..], zm.translation(2.0, 3.0, 4.0));

        const body = Body.init(1.0, &transform, sphere.asShape());
        errdefer body.deinit();

        try expect(body.isCreated() == true);
        try expect(body.getShape() == sphere.asShape());

        body.destroy();
        try expect(body.isCreated() == false);

        body.deallocate();
    }
    {
        const zm = @import("zmath");

        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();

        const body = Body.init(
            0.0, // static body
            &zm.mat43ToArray(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body.deinit();

        var transform: [12]f32 = undefined;
        body.getGraphicsWorldTransform(&transform);

        const m = zm.loadMat43(transform[0..]);
        try expect(zm.approxEqAbs(m[0], zm.f32x4(1.0, 0.0, 0.0, 0.0), 0.0001));
        try expect(zm.approxEqAbs(m[1], zm.f32x4(0.0, 1.0, 0.0, 0.0), 0.0001));
        try expect(zm.approxEqAbs(m[2], zm.f32x4(0.0, 0.0, 1.0, 0.0), 0.0001));
        try expect(zm.approxEqAbs(m[3], zm.f32x4(2.0, 3.0, 4.0, 1.0), 0.0001));
    }
}

test "zbullet.constraint.point2point" {
    const zm = @import("zmath");
    init(std.testing.allocator);
    defer deinit();
    {
        const world = World.init(.{});
        defer world.deinit();

        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();

        const body = Body.init(
            1.0,
            &zm.mat43ToArray(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body.deinit();

        const p2p = Point2PointConstraint.allocate();
        defer p2p.deallocate();

        try expect(p2p.getType() == .point2point);
        try expect(p2p.isCreated() == false);

        p2p.create1(body, &.{ 1.0, 2.0, 3.0 });
        defer p2p.destroy();

        try expect(p2p.getType() == .point2point);
        try expect(p2p.isCreated() == true);
        try expect(p2p.isEnabled() == true);
        try expect(p2p.getBodyA() == body);
        try expect(p2p.getBodyB() == Constraint.getFixedBody());

        var pivot: [3]f32 = undefined;
        p2p.getPivotA(&pivot);
        try expect(pivot[0] == 1.0 and pivot[1] == 2.0 and pivot[2] == 3.0);

        p2p.setPivotA(&.{ -1.0, -2.0, -3.0 });
        p2p.getPivotA(&pivot);
        try expect(pivot[0] == -1.0 and pivot[1] == -2.0 and pivot[2] == -3.0);
    }
    {
        const world = World.init(.{});
        defer world.deinit();

        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();

        const body0 = Body.init(
            1.0,
            &zm.mat43ToArray(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body0.deinit();

        const body1 = Body.init(
            1.0,
            &zm.mat43ToArray(zm.translation(2.0, 3.0, 4.0)),
            sphere.asShape(),
        );
        defer body1.deinit();

        const p2p = Point2PointConstraint.allocate();
        defer p2p.deallocate();

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
