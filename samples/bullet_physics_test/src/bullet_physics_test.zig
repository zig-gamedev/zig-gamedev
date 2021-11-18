const builtin = @import("builtin");
const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const dml = win32.directml;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const pix = common.pix;
const vm = common.vectormath;
const tracy = common.tracy;
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: bullet physics test";
const window_width = 1920;
const window_height = 1080;
const num_msaa_samples = 4;

const camera_fovy: f32 = math.pi / @as(f32, 3.0);

const default_linear_damping: f32 = 0.1;
const default_angular_damping: f32 = 0.1;
const default_world_friction: f32 = 0.15;

const Scene = enum {
    scene1,
    scene2,
    scene3,
};

const PhysicsObjectsPool = struct {
    const max_num_bodies = 2 * 1024;
    const max_num_constraints = 44;
    const max_num_shapes = 48;
    bodies: []c.CbtBodyHandle,
    constraints: []c.CbtConstraintHandle,
    shapes: []c.CbtShapeHandle,

    fn init() PhysicsObjectsPool {
        const mem = std.heap.page_allocator.alloc(
            c.CbtBodyHandle,
            max_num_bodies + max_num_constraints + max_num_shapes,
        ) catch unreachable;

        const bodies = mem[0..max_num_bodies];
        const constraints = @ptrCast([*]c.CbtConstraintHandle, mem.ptr)[max_num_bodies .. max_num_bodies +
            max_num_constraints];
        const shapes = @ptrCast([*]c.CbtShapeHandle, mem.ptr)[max_num_bodies + max_num_constraints .. max_num_bodies +
            max_num_constraints + max_num_shapes];

        var pool = PhysicsObjectsPool{
            .bodies = bodies,
            .constraints = constraints,
            .shapes = shapes,
        };

        // Bodies
        c.cbtBodyAllocateBatch(max_num_bodies, pool.bodies.ptr);

        // Constraints
        {
            var counter: u32 = 0;
            var i: u32 = 0;
            while (i < 32) : (i += 1) {
                pool.constraints[counter] = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_POINT2POINT);
                counter += 1;
            }
            i = 0;
            while (i < 3) : (i += 1) {
                pool.constraints[counter] = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_GEAR);
                counter += 1;
            }
            i = 0;
            while (i < 3) : (i += 1) {
                pool.constraints[counter] = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_HINGE);
                counter += 1;
            }
            i = 0;
            while (i < 3) : (i += 1) {
                pool.constraints[counter] = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_SLIDER);
                counter += 1;
            }
            i = 0;
            while (i < 3) : (i += 1) {
                pool.constraints[counter] = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_CONETWIST);
                counter += 1;
            }
            assert(counter == max_num_constraints);
        }

        // Shapes
        {
            var counter: u32 = 0;
            var i: u32 = 0;
            while (i < 8) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_SPHERE);
                counter += 1;
            }
            i = 0;
            while (i < 8) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_BOX);
                counter += 1;
            }
            i = 0;
            while (i < 8) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_COMPOUND);
                counter += 1;
            }
            i = 0;
            while (i < 8) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_TRIANGLE_MESH);
                counter += 1;
            }
            i = 0;
            while (i < 8) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_CYLINDER);
                counter += 1;
            }
            i = 0;
            while (i < 4) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_CAPSULE);
                counter += 1;
            }
            i = 0;
            while (i < 4) : (i += 1) {
                pool.shapes[counter] = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_CONE);
                counter += 1;
            }
            assert(counter == max_num_shapes);
        }

        return pool;
    }

    fn deinit(pool: *PhysicsObjectsPool, world: c.CbtWorldHandle) void {
        pool.destroyAllObjects(world);
        c.cbtBodyDeallocateBatch(@intCast(u32, pool.bodies.len), pool.bodies.ptr);
        for (pool.constraints) |con| {
            c.cbtConDeallocate(con);
        }
        for (pool.shapes) |shape| {
            c.cbtShapeDeallocate(shape);
        }
        std.heap.page_allocator.free(pool.bodies);
        pool.* = undefined;
    }

    fn getBody(pool: PhysicsObjectsPool) c.CbtBodyHandle {
        for (pool.bodies) |body| {
            if (!c.cbtBodyIsCreated(body)) {
                return body;
            }
        }
        unreachable;
    }

    fn getConstraint(pool: PhysicsObjectsPool, con_type: i32) c.CbtConstraintHandle {
        for (pool.constraints) |con| {
            if (!c.cbtConIsCreated(con) and c.cbtConGetType(con) == con_type) {
                return con;
            }
        }
        unreachable;
    }

    fn getShape(pool: PhysicsObjectsPool, shape_type: i32) c.CbtShapeHandle {
        for (pool.shapes) |shape| {
            if (!c.cbtShapeIsCreated(shape) and c.cbtShapeGetType(shape) == shape_type) {
                return shape;
            }
        }
        unreachable;
    }

    fn destroyAllObjects(pool: PhysicsObjectsPool, world: c.CbtWorldHandle) void {
        {
            var i = c.cbtWorldGetNumConstraints(world) - 1;
            while (i >= 0) : (i -= 1) {
                const constraint = c.cbtWorldGetConstraint(world, i);
                c.cbtWorldRemoveConstraint(world, constraint);
                c.cbtConDestroy(constraint);
            }
        }
        {
            var i = c.cbtWorldGetNumBodies(world) - 1;
            while (i >= 0) : (i -= 1) {
                const body = c.cbtWorldGetBody(world, i);
                c.cbtWorldRemoveBody(world, body);
                c.cbtBodyDestroy(body);
            }
        }
        for (pool.shapes) |shape| {
            if (c.cbtShapeIsCreated(shape)) {
                if (c.cbtShapeGetType(shape) == c.CBT_SHAPE_TYPE_TRIANGLE_MESH) {
                    c.cbtShapeTriMeshDestroy(shape);
                } else {
                    c.cbtShapeDestroy(shape);
                }
            }
        }
    }
};

const Vertex = struct {
    position: Vec3,
    normal: Vec3,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    num_indices: u32,
    num_vertices: u32,
};

const mesh_cube: u16 = 0;
const mesh_sphere: u16 = 1;
const mesh_capsule: u16 = 2;
const mesh_cylinder: u16 = 3;
const mesh_cone: u16 = 4;
const mesh_world: u16 = 5;
const mesh_compound: u16 = 0xffff;

fn loadAllMeshes(
    all_meshes: *std.ArrayList(Mesh),
    all_positions: *std.ArrayList(Vec3),
    all_normals: *std.ArrayList(Vec3),
    all_indices: *std.ArrayList(u32),
) void {
    const paths = [_][]const u8{
        "content/cube.gltf",
        "content/sphere.gltf",
        "content/capsule.gltf",
        "content/cylinder.gltf",
        "content/cone.gltf",
        "content/world.gltf",
    };
    for (paths) |path| {
        const pre_indices_len = all_indices.items.len;
        const pre_positions_len = all_positions.items.len;

        const data = lib.parseAndLoadGltfFile(path);
        defer c.cgltf_free(data);
        lib.appendMeshPrimitive(data, 0, 0, all_indices, all_positions, all_normals, null, null);

        all_meshes.append(.{
            .index_offset = @intCast(u32, pre_indices_len),
            .vertex_offset = @intCast(u32, pre_positions_len),
            .num_indices = @intCast(u32, all_indices.items.len - pre_indices_len),
            .num_vertices = @intCast(u32, all_positions.items.len - pre_positions_len),
        }) catch unreachable;
    }
}

const Entity = extern struct {
    body: c.CbtBodyHandle,
    base_color_roughness: Vec4,
    size: Vec3,
    flags: u16 = 0,
    mesh_index: u16,
};

const Camera = struct {
    position: Vec3,
    forward: Vec3,
    pitch: f32,
    yaw: f32,
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    info_txtfmt: *dwrite.ITextFormat,

    physics_debug_pso: gr.PipelineHandle,
    simple_entity_pso: gr.PipelineHandle,

    color_texture: gr.ResourceHandle,
    depth_texture: gr.ResourceHandle,
    color_texture_rtv: d3d12.CPU_DESCRIPTOR_HANDLE,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,

    physics_debug: *PhysicsDebug,
    physics_world: c.CbtWorldHandle,
    physics_objects_pool: PhysicsObjectsPool,

    entities: std.ArrayList(Entity),
    meshes: std.ArrayList(Mesh),

    current_scene_index: i32,
    selected_entity_index: u32,
    keyboard_delay: f32,
    simulation_is_paused: bool,
    do_simulation_step: bool,

    camera: Camera,
    mouse: struct {
        cursor_prev_x: i32,
        cursor_prev_y: i32,
    },
    pick: struct {
        body: c.CbtBodyHandle,
        constraint: c.CbtConstraintHandle,
        saved_linear_damping: f32,
        saved_angular_damping: f32,
        distance: f32,
    },
};

const PsoPhysicsDebug_Vertex = extern struct {
    position: [3]f32,
    color: u32,
};

const PsoPhysicsDebug_FrameConst = extern struct {
    world_to_clip: Mat4,
};

const PsoSimpleEntity_DrawConst = extern struct {
    object_to_world: Mat4,
    base_color_roughness: Vec4,
    flags: u32,
    padding: [3]u32 = undefined,
};

const PsoSimpleEntity_FrameConst = extern struct {
    world_to_clip: Mat4,
    camera_position: Vec3,
};

const PhysicsDebug = struct {
    lines: std.ArrayList(PsoPhysicsDebug_Vertex),

    fn init(alloc: *std.mem.Allocator) PhysicsDebug {
        return .{ .lines = std.ArrayList(PsoPhysicsDebug_Vertex).init(alloc) };
    }

    fn deinit(debug: *PhysicsDebug) void {
        debug.lines.deinit();
        debug.* = undefined;
    }

    fn drawLine1(debug: *PhysicsDebug, p0: Vec3, p1: Vec3, color: Vec3) void {
        const r = @floatToInt(u32, color.c[0] * 255.0);
        const g = @floatToInt(u32, color.c[1] * 255.0) << 8;
        const b = @floatToInt(u32, color.c[2] * 255.0) << 16;
        const rgb = r | g | b;
        debug.lines.append(.{ .position = p0.c, .color = rgb }) catch unreachable;
        debug.lines.append(.{ .position = p1.c, .color = rgb }) catch unreachable;
    }

    fn drawLine2(debug: *PhysicsDebug, p0: Vec3, p1: Vec3, color0: Vec3, color1: Vec3) void {
        const r0 = @floatToInt(u32, color0.c[0] * 255.0);
        const g0 = @floatToInt(u32, color0.c[1] * 255.0) << 8;
        const b0 = @floatToInt(u32, color0.c[2] * 255.0) << 16;
        const rgb0 = r0 | g0 | b0;

        const r1 = @floatToInt(u32, color1.c[0] * 255.0);
        const g1 = @floatToInt(u32, color1.c[1] * 255.0) << 8;
        const b1 = @floatToInt(u32, color1.c[2] * 255.0) << 16;
        const rgb1 = r1 | g1 | b1;

        debug.lines.append(.{ .position = p0.c, .color = rgb0 }) catch unreachable;
        debug.lines.append(.{ .position = p1.c, .color = rgb1 }) catch unreachable;
    }

    fn drawContactPoint(debug: *PhysicsDebug, point: Vec3, normal: Vec3, distance: f32, color: Vec3) void {
        debug.drawLine1(point, point.add(normal.scale(distance)), color);
        debug.drawLine1(point, point.add(normal.scale(0.01)), Vec3.init(0, 0, 0));
    }

    fn drawLine1Callback(p0: [*c]const f32, p1: [*c]const f32, color: [*c]const f32, user: ?*c_void) callconv(.C) void {
        const ptr = @ptrCast(*PhysicsDebug, @alignCast(@alignOf(PhysicsDebug), user.?));
        ptr.drawLine1(
            Vec3.init(p0[0], p0[1], p0[2]),
            Vec3.init(p1[0], p1[1], p1[2]),
            Vec3.init(color[0], color[1], color[2]),
        );
    }

    fn drawLine2Callback(
        p0: [*c]const f32,
        p1: [*c]const f32,
        color0: [*c]const f32,
        color1: [*c]const f32,
        user: ?*c_void,
    ) callconv(.C) void {
        const ptr = @ptrCast(*PhysicsDebug, @alignCast(@alignOf(PhysicsDebug), user.?));
        ptr.drawLine2(
            Vec3.init(p0[0], p0[1], p0[2]),
            Vec3.init(p1[0], p1[1], p1[2]),
            Vec3.init(color0[0], color0[1], color0[2]),
            Vec3.init(color1[0], color1[1], color1[2]),
        );
    }

    fn drawContactPointCallback(
        point: [*c]const f32,
        normal: [*c]const f32,
        distance: f32,
        _: c_int,
        color: [*c]const f32,
        user: ?*c_void,
    ) callconv(.C) void {
        const ptr = @ptrCast(*PhysicsDebug, @alignCast(@alignOf(PhysicsDebug), user.?));
        ptr.drawContactPoint(
            Vec3.init(point[0], point[1], point[2]),
            Vec3.init(normal[0], normal[1], normal[2]),
            distance,
            Vec3.init(color[0], color[1], color[2]),
        );
    }

    fn reportErrorWarningCallback(str: [*c]const u8, _: ?*c_void) callconv(.C) void {
        std.log.info("{s}", .{str});
    }
};

var shape_sphere_r1: c.CbtShapeHandle = undefined;
var shape_box_e111: c.CbtShapeHandle = undefined;
var shape_world: c.CbtShapeHandle = undefined;

fn createScene1(
    world: c.CbtWorldHandle,
    physics_objects_pool: PhysicsObjectsPool,
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    const world_body = physics_objects_pool.getBody();
    c.cbtBodyCreate(world_body, 0.0, &Mat4.initTranslation(Vec3.init(0, 0, 0)).toArray4x3(), shape_world);
    createAddEntity(world, world_body, Vec4.init(0.25, 0.25, 0.25, 0.125), entities);
    c.cbtBodySetFriction(world_body, default_world_friction);

    const sphere_shape = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_SPHERE);
    c.cbtShapeSphereCreate(sphere_shape, 1.5);

    const box_shape = physics_objects_pool.getShape(c.CBT_SHAPE_TYPE_BOX);
    c.cbtShapeBoxCreate(box_shape, &Vec3.init(0.5, 1.0, 2.0).c);

    const capsule_shape = physics_objects_pool.getShape(c.CBT_SHAPE_TYPE_CAPSULE);
    c.cbtShapeCapsuleCreate(capsule_shape, 1.0, 2.0, c.CBT_LINEAR_AXIS_Y);

    const cylinder_shape = physics_objects_pool.getShape(c.CBT_SHAPE_TYPE_CYLINDER);
    c.cbtShapeCylinderCreate(cylinder_shape, &Vec3.init(1.5, 2.0, 1.5).c, c.CBT_LINEAR_AXIS_Y);

    const cone_shape = physics_objects_pool.getShape(c.CBT_SHAPE_TYPE_CONE);
    c.cbtShapeConeCreate(cone_shape, 1.0, 2.0, c.CBT_LINEAR_AXIS_Y);

    const body0 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body0, 10.0, &Mat4.initTranslation(Vec3.init(3, 3.5, 5)).toArray4x3(), shape_box_e111);

    const body1 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body1, 50.0, &Mat4.initTranslation(Vec3.init(-3, 3.5, 5)).toArray4x3(), box_shape);

    const body2 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body2, 15.0, &Mat4.initTranslation(Vec3.init(-3, 3.5, 10)).toArray4x3(), sphere_shape);

    const body3 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body3, 10.0, &Mat4.initTranslation(Vec3.init(-5, 3.5, 10)).toArray4x3(), capsule_shape);

    const body4 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body4, 10.0, &Mat4.initTranslation(Vec3.init(5, 3.5, 10)).toArray4x3(), cylinder_shape);

    const body5 = physics_objects_pool.getBody();
    c.cbtBodyCreate(body5, 10.0, &Mat4.initTranslation(Vec3.init(0, 3.5, 7)).toArray4x3(), cone_shape);

    createAddEntity(world, body0, Vec4.init(0.75, 0.0, 0.0, 0.5), entities);
    createAddEntity(world, body1, Vec4.init(1.0, 0.9, 0.0, 0.75), entities);
    createAddEntity(world, body2, Vec4.init(0.0, 0.1, 1.0, 0.25), entities);
    createAddEntity(world, body3, Vec4.init(0.0, 1.0, 0.0, 0.25), entities);
    createAddEntity(world, body4, Vec4.init(1.0, 1.0, 1.0, 0.75), entities);
    createAddEntity(world, body5, Vec4.init(1.0, 0.5, 0.0, 0.8), entities);

    camera.* = .{
        .position = Vec3.init(0.0, 3.0, 0.0),
        .forward = Vec3.initZero(),
        .pitch = math.pi * 0.05,
        .yaw = 0.0,
    };
}

fn createScene2(
    world: c.CbtWorldHandle,
    physics_objects_pool: PhysicsObjectsPool,
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    const world_body = physics_objects_pool.getBody();
    c.cbtBodyCreate(world_body, 0.0, &Mat4.initTranslation(Vec3.init(0, 0, 0)).toArray4x3(), shape_world);
    createAddEntity(world, world_body, Vec4.init(0.25, 0.25, 0.25, 0.125), entities);
    c.cbtBodySetFriction(world_body, default_world_friction);

    var level: u32 = 0;
    var y: f32 = 2.0;
    while (y <= 14.0) : (y += 2.0) {
        const bound: f32 = 16.0 - y;
        var z: f32 = -bound;
        const base_color_roughness = if (level % 2 == 1)
            Vec4.init(0.5, 0.0, 0.0, 0.5)
        else
            Vec4.init(0.7, 0.6, 0.0, 0.75);
        level += 1;
        while (z <= bound) : (z += 2.0) {
            var x: f32 = -bound;
            while (x <= bound) : (x += 2.0) {
                const body = physics_objects_pool.getBody();
                c.cbtBodyCreate(body, 1.0, &Mat4.initTranslation(Vec3.init(x, y, z)).toArray4x3(), shape_box_e111);
                createAddEntity(world, body, base_color_roughness, entities);
            }
        }
    }

    camera.* = .{
        .position = Vec3.init(30.0, 30.0, -30.0),
        .forward = Vec3.initZero(),
        .pitch = math.pi * 0.2,
        .yaw = -math.pi * 0.25,
    };
}

fn createScene3(
    world: c.CbtWorldHandle,
    physics_objects_pool: PhysicsObjectsPool,
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    const world_body = physics_objects_pool.getBody();
    c.cbtBodyCreate(world_body, 0.0, &Mat4.initTranslation(Vec3.init(0, 0, 0)).toArray4x3(), shape_world);
    createAddEntity(world, world_body, Vec4.init(0.25, 0.25, 0.25, 0.125), entities);
    c.cbtBodySetFriction(world_body, default_world_friction);

    // Chain of boxes
    var x: f32 = -14.0;
    var prev_body: c.CbtBodyHandle = null;
    while (x <= 14.0) : (x += 4.0) {
        const body = physics_objects_pool.getBody();
        c.cbtBodyCreate(body, 10.0, &Mat4.initTranslation(Vec3.init(x, 3.5, 5)).toArray4x3(), shape_box_e111);
        createAddEntity(world, body, Vec4.init(0.75, 0.0, 0.0, 0.5), entities);

        if (prev_body != null) {
            const p2p = physics_objects_pool.getConstraint(c.CBT_CONSTRAINT_TYPE_POINT2POINT);
            c.cbtConPoint2PointCreate2(p2p, prev_body, body, &Vec3.init(1.25, 0, 0).c, &Vec3.init(-1.25, 0, 0).c);
            c.cbtConPoint2PointSetTau(p2p, 0.001);
            c.cbtWorldAddConstraint(world, p2p, false);
        }
        prev_body = body;
    }

    // Chain of spheres
    x = -14.0;
    prev_body = null;
    while (x <= 14.0) : (x += 4.0) {
        const body = physics_objects_pool.getBody();
        c.cbtBodyCreate(body, 10.0, &Mat4.initTranslation(Vec3.init(x, 3.5, 10)).toArray4x3(), shape_sphere_r1);
        createAddEntity(world, body, Vec4.init(0.0, 0.75, 0.0, 0.5), entities);

        if (prev_body != null) {
            const p2p = physics_objects_pool.getConstraint(c.CBT_CONSTRAINT_TYPE_POINT2POINT);
            c.cbtConPoint2PointCreate2(p2p, prev_body, body, &Vec3.init(1.1, 0, 0).c, &Vec3.init(-1.1, 0, 0).c);
            c.cbtConPoint2PointSetTau(p2p, 0.001);
            c.cbtWorldAddConstraint(world, p2p, false);
        }
        prev_body = body;
    }

    // Fixed chain of spheres
    var y: f32 = 16.0;
    prev_body = null;

    const static_body = physics_objects_pool.getBody();
    c.cbtBodyCreate(static_body, 0.0, &Mat4.initTranslation(Vec3.init(10, y, 10)).toArray4x3(), shape_box_e111);
    createAddEntity(world, static_body, Vec4.init(0.75, 0.75, 0.0, 0.5), entities);

    while (y >= 1.0) : (y -= 4.0) {
        const body = physics_objects_pool.getBody();
        c.cbtBodyCreate(body, 10.0, &Mat4.initTranslation(Vec3.init(10, y, 10)).toArray4x3(), shape_sphere_r1);
        createAddEntity(world, body, Vec4.init(0.0, 0.25, 1.0, 0.25), entities);

        if (prev_body != null) {
            const p2p = physics_objects_pool.getConstraint(c.CBT_CONSTRAINT_TYPE_POINT2POINT);
            c.cbtConPoint2PointCreate2(p2p, body, prev_body, &Vec3.init(0, 1.25, 0).c, &Vec3.init(0, -1.25, 0).c);
            c.cbtConPoint2PointSetTau(p2p, 0.001);
            c.cbtWorldAddConstraint(world, p2p, false);
        } else {
            const p2p = physics_objects_pool.getConstraint(c.CBT_CONSTRAINT_TYPE_POINT2POINT);
            c.cbtConPoint2PointCreate2(p2p, body, static_body, &Vec3.init(0, 1.25, 0).c, &Vec3.init(0, -1.25, 0).c);
            c.cbtConPoint2PointSetTau(p2p, 0.001);
            c.cbtWorldAddConstraint(world, p2p, false);
        }
        prev_body = body;
    }

    camera.* = .{
        .position = Vec3.init(0.0, 7.0, -5.0),
        .forward = Vec3.initZero(),
        .pitch = math.pi * 0.125,
        .yaw = 0.0,
    };
}

fn init(gpa: *std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;

    var arena_allocator = std.heap.ArenaAllocator.init(gpa);
    defer arena_allocator.deinit();

    _ = pix.loadGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);

    const barycentrics_supported = blk: {
        var options3: d3d12.FEATURE_DATA_D3D12_OPTIONS3 = undefined;
        const res = grfx.device.CheckFeatureSupport(.OPTIONS3, &options3, @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS3));
        break :blk options3.BarycentricsSupported == w.TRUE and res == w.S_OK;
    };

    const brush = blk: {
        var brush: *d2d1.ISolidColorBrush = undefined;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            @ptrCast(*?*d2d1.ISolidColorBrush, &brush),
        ));
        break :blk brush;
    };

    const info_txtfmt = blk: {
        var info_txtfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.BOLD,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            @ptrCast(*?*dwrite.ITextFormat, &info_txtfmt),
        ));
        break :blk info_txtfmt;
    };
    hrPanicOnFail(info_txtfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(info_txtfmt.SetParagraphAlignment(.NEAR));

    const physics_debug_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .LINE;
        pso_desc.RasterizerState.AntialiasedLineEnable = w.TRUE;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.SampleDesc = .{ .Count = num_msaa_samples, .Quality = 0 };

        break :blk grfx.createGraphicsShaderPipeline(
            &arena_allocator.allocator,
            &pso_desc,
            "content/shaders/physics_debug.vs.cso",
            "content/shaders/physics_debug.ps.cso",
        );
    };

    const simple_entity_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.SampleDesc = .{ .Count = num_msaa_samples, .Quality = 0 };

        if (!barycentrics_supported) {
            break :blk grfx.createGraphicsShaderPipelineAllStages(
                &arena_allocator.allocator,
                &pso_desc,
                "content/shaders/simple_entity.vs.cso",
                "content/shaders/simple_entity.gs.cso",
                "content/shaders/simple_entity_with_gs.ps.cso",
            );
        } else {
            break :blk grfx.createGraphicsShaderPipeline(
                &arena_allocator.allocator,
                &pso_desc,
                "content/shaders/simple_entity.vs.cso",
                "content/shaders/simple_entity.ps.cso",
            );
        }
    };

    const color_texture = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, grfx.viewport_width, grfx.viewport_height, 1);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            desc.SampleDesc.Count = num_msaa_samples;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_RENDER_TARGET,
        &d3d12.CLEAR_VALUE.initColor(.R8G8B8A8_UNORM, &.{ 0.0, 0.0, 0.0, 1.0 }),
    ) catch |err| hrPanic(err);

    const color_texture_rtv = grfx.allocateCpuDescriptors(.RTV, 1);
    grfx.device.CreateRenderTargetView(grfx.getResource(color_texture), null, color_texture_rtv);

    const depth_texture = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, grfx.viewport_width, grfx.viewport_height, 1);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | d3d12.RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            desc.SampleDesc.Count = num_msaa_samples;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_DEPTH_WRITE,
        &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
    ) catch |err| hrPanic(err);

    const depth_texture_dsv = grfx.allocateCpuDescriptors(.DSV, 1);
    grfx.device.CreateDepthStencilView(grfx.getResource(depth_texture), null, depth_texture_dsv);

    var all_meshes = std.ArrayList(Mesh).init(gpa);
    var all_positions = std.ArrayList(Vec3).init(&arena_allocator.allocator);
    var all_normals = std.ArrayList(Vec3).init(&arena_allocator.allocator);
    var all_indices = std.ArrayList(u32).init(&arena_allocator.allocator);
    loadAllMeshes(&all_meshes, &all_positions, &all_normals, &all_indices);

    var physics_debug = gpa.create(PhysicsDebug) catch unreachable;
    physics_debug.* = PhysicsDebug.init(gpa);

    const physics_world = c.cbtWorldCreate();
    c.cbtWorldSetGravity(physics_world, &Vec3.init(0.0, -10.0, 0.0).c);

    c.cbtWorldDebugSetCallbacks(physics_world, &.{
        .drawLine1 = PhysicsDebug.drawLine1Callback,
        .drawLine2 = PhysicsDebug.drawLine2Callback,
        .drawContactPoint = PhysicsDebug.drawContactPointCallback,
        .reportErrorWarning = PhysicsDebug.reportErrorWarningCallback,
        .user_data = physics_debug,
    });

    // Create common shapes.
    {
        shape_world = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_TRIANGLE_MESH);
        c.cbtShapeTriMeshCreateBegin(shape_world);
        c.cbtShapeTriMeshAddIndexVertexArray(
            shape_world,
            @intCast(i32, all_meshes.items[mesh_world].num_indices / 3),
            &all_indices.items[all_meshes.items[mesh_world].index_offset],
            3 * @sizeOf(u32),
            @intCast(i32, all_meshes.items[mesh_world].num_vertices),
            &all_positions.items[all_meshes.items[mesh_world].vertex_offset],
            3 * @sizeOf(f32),
        );
        c.cbtShapeTriMeshCreateEnd(shape_world);

        shape_sphere_r1 = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_SPHERE);
        c.cbtShapeSphereCreate(shape_sphere_r1, 1.0);

        shape_box_e111 = c.cbtShapeAllocate(c.CBT_SHAPE_TYPE_BOX);
        c.cbtShapeBoxCreate(shape_box_e111, &Vec3.init(1.0, 1.0, 1.0).c);
    }

    const physics_objects_pool = PhysicsObjectsPool.init();
    var camera: Camera = undefined;
    var entities = std.ArrayList(Entity).init(gpa);
    createScene1(physics_world, physics_objects_pool, &entities, &camera);
    entities.items[0].flags = 1;

    var vertex_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_positions.items.len * @sizeOf(Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    var index_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    //
    // Begin frame to init/upload resources to the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx, num_msaa_samples);

    {
        const upload = grfx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_positions.items.len));
        for (all_positions.items) |_, i| {
            upload.cpu_slice[i].position = all_positions.items[i];
            upload.cpu_slice[i].normal = all_normals.items[i];
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(vertex_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    }

    {
        const upload = grfx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |_, i| {
            upload.cpu_slice[i] = all_indices.items[i];
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(index_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_INDEX_BUFFER);
    }

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .info_txtfmt = info_txtfmt,
        .physics_world = physics_world,
        .physics_debug = physics_debug,
        .physics_objects_pool = physics_objects_pool,
        .entities = entities,
        .physics_debug_pso = physics_debug_pso,
        .simple_entity_pso = simple_entity_pso,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .meshes = all_meshes,
        .color_texture = color_texture,
        .depth_texture = depth_texture,
        .color_texture_rtv = color_texture_rtv,
        .depth_texture_dsv = depth_texture_dsv,
        .camera = camera,
        .mouse = .{
            .cursor_prev_x = 0,
            .cursor_prev_y = 0,
        },
        .pick = .{
            .body = null,
            .saved_linear_damping = 0.0,
            .saved_angular_damping = 0.0,
            .constraint = c.cbtConAllocate(c.CBT_CONSTRAINT_TYPE_POINT2POINT),
            .distance = 0.0,
        },
        .current_scene_index = 0,
        .selected_entity_index = 0,
        .keyboard_delay = 0.0,
        .simulation_is_paused = false,
        .do_simulation_step = false,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    demo.meshes.deinit();
    _ = demo.brush.Release();
    _ = demo.info_txtfmt.Release();
    _ = demo.grfx.releasePipeline(demo.physics_debug_pso);
    _ = demo.grfx.releasePipeline(demo.simple_entity_pso);
    _ = demo.grfx.releaseResource(demo.color_texture);
    _ = demo.grfx.releaseResource(demo.depth_texture);
    _ = demo.grfx.releaseResource(demo.vertex_buffer);
    _ = demo.grfx.releaseResource(demo.index_buffer);
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa);
    if (c.cbtConIsCreated(demo.pick.constraint)) {
        c.cbtWorldRemoveConstraint(demo.physics_world, demo.pick.constraint);
        c.cbtConDestroy(demo.pick.constraint);
    }
    c.cbtConDeallocate(demo.pick.constraint);
    demo.entities.deinit();
    demo.physics_objects_pool.deinit(demo.physics_world);
    demo.physics_debug.deinit();
    gpa.destroy(demo.physics_debug);
    c.cbtWorldDestroy(demo.physics_world);
    demo.* = undefined;
}

fn createAddEntity(
    world: c.CbtWorldHandle,
    body: c.CbtBodyHandle,
    base_color_roughness: Vec4,
    entities: *std.ArrayList(Entity),
) void {
    //var tweaked_size = size;
    const shape = c.cbtBodyGetShape(body);
    const shape_type = c.cbtShapeGetType(shape);

    const mesh_index = switch (shape_type) {
        c.CBT_SHAPE_TYPE_BOX => mesh_cube,
        c.CBT_SHAPE_TYPE_SPHERE => mesh_sphere,
        c.CBT_SHAPE_TYPE_CONE => mesh_cone,
        c.CBT_SHAPE_TYPE_CYLINDER => mesh_cylinder,
        c.CBT_SHAPE_TYPE_CAPSULE => mesh_capsule,
        c.CBT_SHAPE_TYPE_TRIANGLE_MESH => mesh_world,
        c.CBT_SHAPE_TYPE_COMPOUND => mesh_compound,
        else => blk: {
            assert(false);
            break :blk 0;
        },
    };
    const mesh_size = switch (shape_type) {
        c.CBT_SHAPE_TYPE_BOX => blk: {
            var half_extents: Vec3 = undefined;
            c.cbtShapeBoxGetHalfExtentsWithoutMargin(shape, &half_extents.c);
            break :blk half_extents;
        },
        c.CBT_SHAPE_TYPE_SPHERE => blk: {
            break :blk Vec3.initS(c.cbtShapeSphereGetRadius(shape));
        },
        c.CBT_SHAPE_TYPE_CONE => blk: {
            // TODO(mziulek): Cone mesh seems wrong, fix it.
            assert(c.cbtShapeConeGetUpAxis(shape) == c.CBT_LINEAR_AXIS_Y);
            const radius = c.cbtShapeConeGetRadius(shape);
            const height = c.cbtShapeConeGetHeight(shape);
            break :blk Vec3.init(radius, 0.5 * height, radius);
        },
        c.CBT_SHAPE_TYPE_CYLINDER => blk: {
            var half_extents: Vec3 = undefined;
            assert(c.cbtShapeCylinderGetUpAxis(shape) == c.CBT_LINEAR_AXIS_Y);
            assert(half_extents.c[0] == half_extents.c[2]);
            c.cbtShapeCylinderGetHalfExtentsWithoutMargin(shape, &half_extents.c);
            break :blk half_extents;
        },
        c.CBT_SHAPE_TYPE_CAPSULE => blk: {
            // TODO(mziulek): Capsule mesh seems wrong, fix it.
            assert(c.cbtShapeCapsuleGetUpAxis(shape) == c.CBT_LINEAR_AXIS_Y);
            const radius = c.cbtShapeCapsuleGetRadius(shape);
            const height = c.cbtShapeCapsuleGetHalfHeight(shape);
            break :blk Vec3.init(radius, height, radius);
        },
        c.CBT_SHAPE_TYPE_TRIANGLE_MESH => Vec3.initS(1),
        c.CBT_SHAPE_TYPE_COMPOUND => Vec3.initS(1),
        else => blk: {
            assert(false);
            break :blk Vec3.initS(1);
        },
    };

    entities.append(.{
        .body = body,
        .base_color_roughness = base_color_roughness,
        .size = mesh_size,
        .mesh_index = mesh_index,
    }) catch unreachable;
    const entity_index = @intCast(i32, entities.items.len - 1);
    c.cbtBodySetUserIndex(body, 0, entity_index);
    c.cbtBodySetDamping(body, default_linear_damping, default_angular_damping);
    c.cbtBodySetActivationState(body, c.CBT_DISABLE_DEACTIVATION);
    c.cbtWorldAddBody(world, body);
}

fn update(demo: *DemoState) void {
    const dt = demo.frame_stats.delta_time;

    if (!demo.simulation_is_paused) {
        _ = c.cbtWorldStepSimulation(demo.physics_world, dt, 1, 1.0 / 60.0);
    } else if (demo.do_simulation_step) {
        _ = c.cbtWorldStepSimulation(demo.physics_world, 1.0 / 60.0, 1, 1.0 / 60.0);
        demo.do_simulation_step = false;
    }

    demo.frame_stats.update();
    lib.newImGuiFrame(dt);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.grfx.viewport_width) - 600.0 - 20, .y = 20.0 },
        c.ImGuiCond_FirstUseEver,
        c.ImVec2{ .x = 0.0, .y = 0.0 },
    );
    c.igSetNextWindowSize(.{ .x = 600.0, .y = -1 }, c.ImGuiCond_Always);

    _ = c.igBegin(
        "Demo Settings",
        null,
        c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
    );
    {
        _ = c.igCombo_Str(
            "",
            &demo.current_scene_index,
            "Scene: Collision Shapes\x00Scene: Stack of Boxes\x00Scene: Chains\x00\x00",
            -1,
        );
        c.igSameLine(0.0, -1.0);
        if (c.igButton("  Load  ", .{ .x = 0, .y = 0 })) {
            demo.physics_objects_pool.destroyAllObjects(demo.physics_world);
            demo.entities.resize(0) catch unreachable;
            const scene = @intToEnum(Scene, demo.current_scene_index);
            switch (scene) {
                .scene1 => createScene1(demo.physics_world, demo.physics_objects_pool, &demo.entities, &demo.camera),
                .scene2 => createScene2(demo.physics_world, demo.physics_objects_pool, &demo.entities, &demo.camera),
                .scene3 => createScene3(demo.physics_world, demo.physics_objects_pool, &demo.entities, &demo.camera),
            }
            demo.selected_entity_index = 0;
            demo.entities.items[demo.selected_entity_index].flags = 1;
        }
        c.igSpacing();

        if (c.igCollapsingHeader_TreeNodeFlags("Scene Settings", c.ImGuiTreeNodeFlags_None)) {
            var gravity: c.CbtVector3 = undefined;
            c.cbtWorldGetGravity(demo.physics_world, &gravity);
            if (c.igSliderFloat("Gravity", &gravity[1], -15.0, 15.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtWorldSetGravity(demo.physics_world, &gravity);
            }
            if (c.igButton(
                if (demo.simulation_is_paused) "  Resume Simulation  " else "  Pause Simulation  ",
                .{ .x = 0, .y = 0 },
            )) {
                demo.simulation_is_paused = !demo.simulation_is_paused;
            }
            if (demo.simulation_is_paused) {
                c.igSameLine(0.0, -1.0);
                if (c.igButton("  Step  ", .{ .x = 0, .y = 0 })) {
                    demo.do_simulation_step = true;
                }
            }
        }
        c.igSpacing();
    }
    {
        const body = demo.entities.items[demo.selected_entity_index].body;

        if (c.igCollapsingHeader_TreeNodeFlags("Selected Object Settings", c.ImGuiTreeNodeFlags_None)) {
            c.igSeparator();

            var linear_damping = c.cbtBodyGetLinearDamping(body);
            var angular_damping = c.cbtBodyGetAngularDamping(body);
            if (c.igSliderFloat("Linear Damping", &linear_damping, 0.0, 1.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtBodySetDamping(body, linear_damping, angular_damping);
            }
            if (c.igSliderFloat("Angular Damping", &angular_damping, 0.0, 1.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtBodySetDamping(body, linear_damping, angular_damping);
            }
            c.igSeparator();

            var friction = c.cbtBodyGetFriction(body);
            if (c.igSliderFloat("Friction", &friction, 0.0, 1.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtBodySetFriction(body, friction);
            }
            var rolling_friction = c.cbtBodyGetRollingFriction(body);
            if (c.igSliderFloat("Rolling Friction", &rolling_friction, 0.0, 1.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtBodySetRollingFriction(body, rolling_friction);
            }
            c.igSeparator();

            var restitution = c.cbtBodyGetRestitution(body);
            if (c.igSliderFloat("Restitution", &restitution, 0.0, 1.0, null, c.ImGuiSliderFlags_None)) {
                c.cbtBodySetRestitution(body, restitution);
            }
            c.igSeparator();

            const mass_flag = if (c.cbtBodyIsStaticOrKinematic(body))
                c.ImGuiInputTextFlags_ReadOnly
            else
                c.ImGuiInputTextFlags_EnterReturnsTrue;
            var mass = c.cbtBodyGetMass(body);
            if (c.igInputFloat("Mass", &mass, 1.0, 1.0, null, mass_flag)) {
                var inertia = c.CbtVector3{ 0, 0, 0 };
                if (mass > 0.0) {
                    c.cbtShapeCalculateLocalInertia(c.cbtBodyGetShape(body), mass, &inertia);
                }
                _ = c.igInputFloat3("Inertia", &inertia, null, c.ImGuiInputTextFlags_ReadOnly);
                c.cbtBodySetMassProps(body, mass, &inertia);
            }
        }
    }
    c.igEnd();

    if (demo.simulation_is_paused and demo.selected_entity_index > 0) { // index 0 is static world
        const body = demo.entities.items[demo.selected_entity_index].body;

        var linear_velocity: c.CbtVector3 = undefined;
        var angular_velocity: c.CbtVector3 = undefined;
        var position: c.CbtVector3 = undefined;
        c.cbtBodyGetLinearVelocity(body, &linear_velocity);
        c.cbtBodyGetAngularVelocity(body, &angular_velocity);
        c.cbtBodyGetCenterOfMassPosition(body, &position);

        const p1_linear = (Vec3{ .c = position }).add(Vec3{ .c = linear_velocity }).c;
        const p1_angular = (Vec3{ .c = position }).add(Vec3{ .c = angular_velocity }).c;
        const color_linear = c.CbtVector3{ 1.0, 0.0, 1.0 };
        const color_angular = c.CbtVector3{ 0.0, 1.0, 1.0 };

        c.cbtWorldDebugDrawLine1(demo.physics_world, &position, &p1_linear, &color_linear);
        c.cbtWorldDebugDrawLine1(demo.physics_world, &position, &p1_angular, &color_angular);
    }

    // Handle camera rotation with mouse.
    {
        var pos: w.POINT = undefined;
        _ = w.GetCursorPos(&pos);
        const delta_x = @intToFloat(f32, pos.x) - @intToFloat(f32, demo.mouse.cursor_prev_x);
        const delta_y = @intToFloat(f32, pos.y) - @intToFloat(f32, demo.mouse.cursor_prev_y);
        demo.mouse.cursor_prev_x = pos.x;
        demo.mouse.cursor_prev_y = pos.y;

        if (w.GetAsyncKeyState(w.VK_RBUTTON) < 0) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = math.min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = math.max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = vm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed: f32 = 5.0;
        const delta_time = demo.frame_stats.delta_time;
        const transform = Mat4.initRotationX(demo.camera.pitch).mul(Mat4.initRotationY(demo.camera.yaw));
        var forward = Vec3.init(0.0, 0.0, 1.0).transform(transform).normalize();

        demo.camera.forward = forward;
        const right = Vec3.init(0.0, 1.0, 0.0).cross(forward).normalize().scale(speed * delta_time);
        forward = forward.scale(speed * delta_time);

        if (w.GetAsyncKeyState('W') < 0) {
            demo.camera.position = demo.camera.position.add(forward);
        } else if (w.GetAsyncKeyState('S') < 0) {
            demo.camera.position = demo.camera.position.sub(forward);
        }
        if (w.GetAsyncKeyState('D') < 0) {
            demo.camera.position = demo.camera.position.add(right);
        } else if (w.GetAsyncKeyState('A') < 0) {
            demo.camera.position = demo.camera.position.sub(right);
        }
    }

    demo.keyboard_delay += dt;
    if (demo.keyboard_delay >= 0.5) {
        if (w.GetAsyncKeyState(w.VK_SPACE) < 0) {
            demo.keyboard_delay = 0.0;
            const body = demo.physics_objects_pool.getBody();
            c.cbtBodyCreate(body, 2.0, &Mat4.initTranslation(demo.camera.position).toArray4x3(), shape_sphere_r1);
            c.cbtBodyApplyCentralImpulse(body, &demo.camera.forward.scale(100.0).c);
            createAddEntity(
                demo.physics_world,
                body,
                Vec4.init(0, 1.0, 0.0, 0.7),
                &demo.entities,
            );
        }
    }

    const mouse_button_is_down = c.igIsMouseDown(c.ImGuiMouseButton_Left) and !c.igGetIO().?.*.WantCaptureMouse;

    const ray_from = demo.camera.position;
    const ray_to = blk: {
        var ui = c.igGetIO().?;
        const mousex = ui.*.MousePos.x;
        const mousey = ui.*.MousePos.y;

        const far_plane: f32 = 10000.0;
        const tanfov = math.tan(0.5 * camera_fovy);
        const width = @intToFloat(f32, demo.grfx.viewport_width);
        const height = @intToFloat(f32, demo.grfx.viewport_height);
        const aspect = width / height;

        const ray_forward = demo.camera.forward.scale(far_plane);

        var hor = Vec3.init(0, 1, 0).cross(ray_forward).normalize();
        var vertical = hor.cross(ray_forward).normalize();

        hor = hor.scale(2.0 * far_plane * tanfov * aspect);
        vertical = vertical.scale(2.0 * far_plane * tanfov);

        const ray_to_center = ray_from.add(ray_forward);
        const dhor = hor.scale(1.0 / width);
        const dvert = vertical.scale(1.0 / height);

        var ray_to = ray_to_center.sub(hor.scale(0.5)).sub(vertical.scale(0.5));
        ray_to = ray_to.add(dhor.scale(mousex));
        ray_to = ray_to.add(dvert.scale(mousey));

        break :blk ray_to;
    };

    if (!c.cbtConIsCreated(demo.pick.constraint) and mouse_button_is_down) {
        var result: c.CbtRayCastResult = undefined;
        const hit = c.cbtRayTestClosest(
            demo.physics_world,
            &ray_from.c,
            &ray_to.c,
            c.CBT_COLLISION_FILTER_DEFAULT,
            c.CBT_COLLISION_FILTER_ALL,
            c.CBT_RAYCAST_FLAG_USE_USE_GJK_CONVEX_TEST,
            &result,
        );

        if (hit and result.body != null) {
            demo.pick.body = result.body;

            demo.entities.items[demo.selected_entity_index].flags = 0;
            const entity_index = c.cbtBodyGetUserIndex(result.body, 0);
            demo.entities.items[@intCast(u32, entity_index)].flags = 1;
            demo.selected_entity_index = @intCast(u32, entity_index);

            if (!c.cbtBodyIsStaticOrKinematic(result.body)) {
                demo.pick.saved_linear_damping = c.cbtBodyGetLinearDamping(result.body);
                demo.pick.saved_angular_damping = c.cbtBodyGetAngularDamping(result.body);
                c.cbtBodySetDamping(result.body, 0.4, 0.4);

                var inv_trans: [4]c.CbtVector3 = undefined;
                c.cbtBodyGetInvCenterOfMassTransform(result.body, &inv_trans);
                const hit_point_world = Vec3{ .c = result.hit_point_world };
                const pivot_a = hit_point_world.transform(Mat4.initArray4x3(inv_trans));

                c.cbtConPoint2PointCreate1(demo.pick.constraint, result.body, &pivot_a.c);
                c.cbtConPoint2PointSetImpulseClamp(demo.pick.constraint, 30.0);
                c.cbtConPoint2PointSetTau(demo.pick.constraint, 0.001);
                c.cbtConSetDebugDrawSize(demo.pick.constraint, 0.15);

                c.cbtWorldAddConstraint(demo.physics_world, demo.pick.constraint, true);
                demo.pick.distance = hit_point_world.sub(ray_from).length();
            }
        }
    } else if (c.cbtConIsCreated(demo.pick.constraint)) {
        const to = ray_from.add(ray_to.normalize().scale(demo.pick.distance));
        c.cbtConPoint2PointSetPivotB(demo.pick.constraint, &to.c);

        const body_a = c.cbtConGetBodyA(demo.pick.constraint);
        const body_b = c.cbtConGetBodyB(demo.pick.constraint);

        var trans_a: [4]c.CbtVector3 = undefined;
        var trans_b: [4]c.CbtVector3 = undefined;
        c.cbtBodyGetCenterOfMassTransform(body_a, &trans_a);
        c.cbtBodyGetCenterOfMassTransform(body_b, &trans_b);

        var pivot_a: c.CbtVector3 = undefined;
        var pivot_b: c.CbtVector3 = undefined;
        c.cbtConPoint2PointGetPivotA(demo.pick.constraint, &pivot_a);
        c.cbtConPoint2PointGetPivotB(demo.pick.constraint, &pivot_b);

        const position_a = (Vec3{ .c = pivot_a }).transform(Mat4.initArray4x3(trans_a));
        const position_b = (Vec3{ .c = pivot_b }).transform(Mat4.initArray4x3(trans_b));

        const color0 = c.CbtVector3{ 1.0, 1.0, 0.0 };
        const color1 = c.CbtVector3{ 1.0, 0.0, 0.0 };
        c.cbtWorldDebugDrawLine2(demo.physics_world, &position_a.c, &position_b.c, &color0, &color1);

        const color2 = c.CbtVector3{ 0.0, 1.0, 0.0 };
        c.cbtWorldDebugDrawSphere(demo.physics_world, &position_a.c, 0.07, &color2);
    }

    if (!mouse_button_is_down and c.cbtConIsCreated(demo.pick.constraint)) {
        c.cbtWorldRemoveConstraint(demo.physics_world, demo.pick.constraint);
        c.cbtConDestroy(demo.pick.constraint);
        c.cbtBodySetDamping(demo.pick.body, demo.pick.saved_linear_damping, demo.pick.saved_angular_damping);
        demo.pick.body = null;
    }

    // Draw all Point2Point constraints as lines
    {
        const num_constraints: i32 = c.cbtWorldGetNumConstraints(demo.physics_world);
        var i: i32 = 0;
        while (i < num_constraints) : (i += 1) {
            const constraint = c.cbtWorldGetConstraint(demo.physics_world, i);
            if (c.cbtConGetType(constraint) != c.CBT_CONSTRAINT_TYPE_POINT2POINT) continue;
            if (constraint == demo.pick.constraint) continue;

            const body_a = c.cbtConGetBodyA(constraint);
            const body_b = c.cbtConGetBodyB(constraint);

            var trans_a: [4]c.CbtVector3 = undefined;
            var trans_b: [4]c.CbtVector3 = undefined;
            c.cbtBodyGetCenterOfMassTransform(body_a, &trans_a);
            c.cbtBodyGetCenterOfMassTransform(body_b, &trans_b);

            var pivot_a: c.CbtVector3 = undefined;
            var pivot_b: c.CbtVector3 = undefined;
            c.cbtConPoint2PointGetPivotA(constraint, &pivot_a);
            c.cbtConPoint2PointGetPivotB(constraint, &pivot_b);

            var body_position_a: c.CbtVector3 = undefined;
            var body_position_b: c.CbtVector3 = undefined;
            c.cbtBodyGetCenterOfMassPosition(body_a, &body_position_a);
            c.cbtBodyGetCenterOfMassPosition(body_b, &body_position_b);

            const position_a = (Vec3{ .c = pivot_a }).transform(Mat4.initArray4x3(trans_a));
            const position_b = (Vec3{ .c = pivot_b }).transform(Mat4.initArray4x3(trans_b));

            const color = c.CbtVector3{ 1.0, 1.0, 0.0 };
            c.cbtWorldDebugDrawLine1(demo.physics_world, &position_a.c, &position_b.c, &color);
            c.cbtWorldDebugDrawLine1(demo.physics_world, &body_position_a, &position_a.c, &color);
            c.cbtWorldDebugDrawLine1(demo.physics_world, &body_position_b, &position_b.c, &color);
        }
    }
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const cam_world_to_view = Mat4.initLookToLh(
        demo.camera.position,
        demo.camera.forward,
        Vec3.init(0.0, 1.0, 0.0),
    );
    const cam_view_to_clip = Mat4.initPerspectiveFovLh(
        camera_fovy,
        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
        0.01,
        200.0,
    );
    const cam_world_to_clip = cam_world_to_view.mul(cam_view_to_clip);

    grfx.addTransitionBarrier(demo.color_texture, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{demo.color_texture_rtv},
        w.TRUE,
        &demo.depth_texture_dsv,
    );
    grfx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);
    grfx.cmdlist.ClearRenderTargetView(
        demo.color_texture_rtv,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    {
        grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
            .BufferLocation = grfx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = @intCast(u32, grfx.getResourceSize(demo.vertex_buffer)),
            .StrideInBytes = @sizeOf(Vertex),
        }});
        grfx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = grfx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = @intCast(u32, grfx.getResourceSize(demo.index_buffer)),
            .Format = .R32_UINT,
        });

        grfx.setCurrentPipeline(demo.simple_entity_pso);
        grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        {
            const mem = grfx.allocateUploadMemory(PsoSimpleEntity_FrameConst, 1);
            mem.cpu_slice[0].world_to_clip = cam_world_to_clip.transpose();
            mem.cpu_slice[0].camera_position = demo.camera.position;
            grfx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        }
        for (demo.entities.items) |entity| {
            if (entity.mesh_index == mesh_compound) {
                const world_transform = blk: {
                    var transform: [4]c.CbtVector3 = undefined;
                    c.cbtBodyGetGraphicsWorldTransform(entity.body, &transform);
                    break :blk Mat4.initArray4x3(transform);
                };
                const shape = c.cbtBodyGetShape(entity.body);

                const num_childs = c.cbtShapeCompoundGetNumChilds(shape);
                var child_index: i32 = 0;
                while (child_index < num_childs) : (child_index += 1) {
                    const local_transform = blk: {
                        var transform: [4]c.CbtVector3 = undefined;
                        c.cbtShapeCompoundGetChildTransform(shape, child_index, &transform);
                        break :blk Mat4.initArray4x3(transform);
                    };

                    const child_shape = c.cbtShapeCompoundGetChild(shape, child_index);
                    const mesh_index = switch (c.cbtShapeGetType(child_shape)) {
                        c.CBT_SHAPE_TYPE_BOX => mesh_cube,
                        c.CBT_SHAPE_TYPE_SPHERE => mesh_sphere,
                        c.CBT_SHAPE_TYPE_CONE => mesh_cone,
                        c.CBT_SHAPE_TYPE_CYLINDER => mesh_cylinder,
                        c.CBT_SHAPE_TYPE_CAPSULE => mesh_capsule,
                        else => blk: {
                            assert(false);
                            break :blk 0;
                        },
                    };

                    const mem = grfx.allocateUploadMemory(PsoSimpleEntity_DrawConst, 1);
                    mem.cpu_slice[0].object_to_world = local_transform.mul(world_transform).transpose();
                    mem.cpu_slice[0].base_color_roughness = entity.base_color_roughness;
                    mem.cpu_slice[0].flags = entity.flags;

                    grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
                    grfx.cmdlist.DrawIndexedInstanced(
                        demo.meshes.items[mesh_index].num_indices,
                        1,
                        demo.meshes.items[mesh_index].index_offset,
                        @intCast(i32, demo.meshes.items[mesh_index].vertex_offset),
                        0,
                    );
                }
            } else {
                var transform: [4]c.CbtVector3 = undefined;
                c.cbtBodyGetGraphicsWorldTransform(entity.body, &transform);

                const scaling = Mat4.initScaling(entity.size);

                const mem = grfx.allocateUploadMemory(PsoSimpleEntity_DrawConst, 1);
                mem.cpu_slice[0].object_to_world = scaling.mul(Mat4.initArray4x3(transform)).transpose();
                mem.cpu_slice[0].base_color_roughness = entity.base_color_roughness;
                mem.cpu_slice[0].flags = entity.flags;

                grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
                grfx.cmdlist.DrawIndexedInstanced(
                    demo.meshes.items[entity.mesh_index].num_indices,
                    1,
                    demo.meshes.items[entity.mesh_index].index_offset,
                    @intCast(i32, demo.meshes.items[entity.mesh_index].vertex_offset),
                    0,
                );
            }
        }
    }

    c.cbtWorldDebugDraw(demo.physics_world);
    if (demo.physics_debug.lines.items.len > 0) {
        grfx.setCurrentPipeline(demo.physics_debug_pso);
        grfx.cmdlist.IASetPrimitiveTopology(.LINELIST);
        {
            const mem = grfx.allocateUploadMemory(Mat4, 1);
            mem.cpu_slice[0] = cam_world_to_clip.transpose();
            grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }
        const num_vertices = @intCast(u32, demo.physics_debug.lines.items.len);
        {
            const mem = grfx.allocateUploadMemory(PsoPhysicsDebug_Vertex, num_vertices);
            for (demo.physics_debug.lines.items) |p, i| {
                mem.cpu_slice[i] = p;
            }
            grfx.cmdlist.SetGraphicsRootShaderResourceView(1, mem.gpu_base);
        }
        grfx.cmdlist.DrawInstanced(num_vertices, 1, 0, 0);
        demo.physics_debug.lines.resize(0) catch unreachable;
    }

    demo.gui.draw(grfx);

    const back_buffer = grfx.getBackBuffer();
    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RESOLVE_DEST);
    grfx.addTransitionBarrier(demo.color_texture, d3d12.RESOURCE_STATE_RESOLVE_SOURCE);
    grfx.flushResourceBarriers();

    grfx.cmdlist.ResolveSubresource(
        grfx.getResource(back_buffer.resource_handle),
        0,
        grfx.getResource(demo.color_texture),
        0,
        .R8G8B8A8_UNORM,
    );
    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.beginDraw2d();
    {
        const stats = &demo.frame_stats;
        var buffer = [_]u8{0} ** 64;
        const text = std.fmt.bufPrint(
            buffer[0..],
            "FPS: {d:.1}\nCPU time: {d:.3} ms\nRigid bodies: {d}",
            .{ stats.fps, stats.average_cpu_time, c.cbtWorldGetNumBodies(demo.physics_world) },
        ) catch unreachable;

        demo.brush.SetColor(&.{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        lib.drawText(
            grfx.d2d.context,
            text,
            demo.info_txtfmt,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, grfx.viewport_width),
                .bottom = @intToFloat(f32, grfx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }
    grfx.endDraw2d();

    grfx.endFrame();
}

pub fn main() !void {
    lib.init();
    defer lib.deinit();

    var gpa_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa_allocator.deinit();
        std.debug.assert(leaked == false);
    }
    const gpa = &gpa_allocator.allocator;

    var demo = init(gpa);
    defer deinit(&demo, gpa);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch false;
        if (has_message) {
            _ = w.user32.translateMessage(&message);
            _ = w.user32.dispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT) {
                break;
            }
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
