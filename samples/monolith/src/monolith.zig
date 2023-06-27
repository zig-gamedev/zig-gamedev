const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zphy = @import("zphysics");
const zmesh = @import("zmesh");
const wgsl = @import("monolith_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: physics test (wgpu)";

const FrameUniforms = extern struct {
    world_to_clip: zm.Mat   align(16),
    floor_material: [4]f32  align(16),
    box_rotation: [3][4]f32 align(16),  // GPU will expect each element to be 16-aligned too, so extra f32 here to pad.
    box_center: [4]f32      align(16),  // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    box_radius: [4]f32      align(16),  // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    box_inv_radius: [4]f32  align(16),  // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    camera_position: [3]f32 align(16),  // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
};

const DrawUniforms = extern struct {
    object_to_world: zm.Mat     align(16),
    basecolor_roughness: [4]f32 align(16),
};

const Vertex = extern struct {
    position: [3]f32,
    second: extern union {
        normal: [3]f32,
        color: [3]f32,
    },
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,

    const IndexType = zmesh.Shape.IndexType;
};

const object_layers = struct {
    const non_moving: zphy.ObjectLayer = 0;
    const moving: zphy.ObjectLayer = 1;
    const len: u32 = 2;
};

const broad_phase_layers = struct {
    const non_moving: zphy.BroadPhaseLayer = 0;
    const moving: zphy.BroadPhaseLayer = 1;
    const len: u32 = 2;
};

const BroadPhaseLayerInterface = extern struct {
    usingnamespace zphy.BroadPhaseLayerInterface.Methods(@This());
    __v: *const zphy.BroadPhaseLayerInterface.VTable = &vtable,

    object_to_broad_phase: [object_layers.len]zphy.BroadPhaseLayer = undefined,

    const vtable = zphy.BroadPhaseLayerInterface.VTable{
        .getNumBroadPhaseLayers = _getNumBroadPhaseLayers,
        .getBroadPhaseLayer = _getBroadPhaseLayer,
    };

    fn init() BroadPhaseLayerInterface {
        var layer_interface: BroadPhaseLayerInterface = .{};
        layer_interface.object_to_broad_phase[object_layers.non_moving] = broad_phase_layers.non_moving;
        layer_interface.object_to_broad_phase[object_layers.moving] = broad_phase_layers.moving;
        return layer_interface;
    }

    fn _getNumBroadPhaseLayers(_: *const zphy.BroadPhaseLayerInterface) callconv(.C) u32 {
        return broad_phase_layers.len;
    }

    fn _getBroadPhaseLayer(
        iself: *const zphy.BroadPhaseLayerInterface,
        layer: zphy.ObjectLayer,
    ) callconv(.C) zphy.BroadPhaseLayer {
        const self = @ptrCast(*const BroadPhaseLayerInterface, iself);
        return self.object_to_broad_phase[layer];
    }
};

const ObjectVsBroadPhaseLayerFilter = extern struct {
    usingnamespace zphy.ObjectVsBroadPhaseLayerFilter.Methods(@This());
    __v: *const zphy.ObjectVsBroadPhaseLayerFilter.VTable = &vtable,

    const vtable = zphy.ObjectVsBroadPhaseLayerFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphy.ObjectVsBroadPhaseLayerFilter,
        layer1: zphy.ObjectLayer,
        layer2: zphy.BroadPhaseLayer,
    ) callconv(.C) bool {
        return switch (layer1) {
            object_layers.non_moving => layer2 == broad_phase_layers.moving,
            object_layers.moving => true,
            else => unreachable,
        };
    }
};

const ObjectLayerPairFilter = extern struct {
    usingnamespace zphy.ObjectLayerPairFilter.Methods(@This());
    __v: *const zphy.ObjectLayerPairFilter.VTable = &vtable,

    const vtable = zphy.ObjectLayerPairFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphy.ObjectLayerPairFilter,
        object1: zphy.ObjectLayer,
        object2: zphy.ObjectLayer,
    ) callconv(.C) bool {
        return switch (object1) {
            object_layers.non_moving => object2 == object_layers.moving,
            object_layers.moving => true,
            else => unreachable,
        };
    }
};

const ContactListener = extern struct {
    usingnamespace zphy.ContactListener.Methods(@This());
    __v: *const zphy.ContactListener.VTable = &vtable,

    const vtable = zphy.ContactListener.VTable{ .onContactValidate = _onContactValidate };

    fn _onContactValidate(
        self: *zphy.ContactListener,
        body1: *const zphy.Body,
        body2: *const zphy.Body,
        base_offset: *const [3]zphy.Real,
        collision_result: *const zphy.CollideShapeResult,
    ) callconv(.C) zphy.ValidateResult {
        _ = self;
        _ = body1;
        _ = body2;
        _ = base_offset;
        _ = collision_result;
        return .accept_all_contacts;
    }
};

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    mesh_render_pipe: zgpu.RenderPipelineHandle = .{},
    line_render_pipe: zgpu.RenderPipelineHandle = .{},
    uniform_bg: zgpu.BindGroupHandle,

    vertex_buf: zgpu.BufferHandle,
    index_buf: zgpu.BufferHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    meshes: std.ArrayList(Mesh),

    broad_phase_layer_interface: *BroadPhaseLayerInterface,
    object_vs_broad_phase_layer_filter: *ObjectVsBroadPhaseLayerFilter,
    object_layer_pair_filter: *ObjectLayerPairFilter,
    contact_listener: *ContactListener,
    physics_system: *zphy.PhysicsSystem,

    physics_objects: [9]zphy.BodyId = .{0} ** 9,
    physics_debug_renderer: MyDebugRenderer,

    camera: struct {
        position: [3]f32 = .{ 0.0, 32.0, -32.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.16 * math.pi,
        yaw: f32 = 0.0,
    } = .{},
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
    } = .{},
};

const MyDebugRenderer = extern struct {

    const MyRenderPrimitive = extern struct {
        // Actual render data goes here
        foobar: i32 = 0,
    };

    usingnamespace zphy.DebugRenderer.Methods(@This());
    __v: *const zphy.DebugRenderer.VTable(@This()) = &vtable,

    demo: *DemoState,
    body_draw_settings: zphy.DebugRenderer.BodyDrawSettings = .{
        .shape_wireframe = true,
    },
    body_draw_filter: *zphy.DebugRenderer.BodyDrawFilter,

    primitives: [32]MyRenderPrimitive = [_]MyRenderPrimitive{ .{} } ** 32,
    prim_head: i32 = -1,

    const vtable = zphy.DebugRenderer.VTable(@This()){
        .drawLine = drawLine,
        .drawTriangle = drawTriangle,
        .createTriangleBatch = createTriangleBatch,
        .createTriangleBatchIndexed = createTriangleBatchIndexed,
        .drawGeometry = drawGeometry,
        .drawText3D = drawText3D,
    };

    pub fn shouldBodyDraw(
        _: *const zphy.Body
    ) align(zphy.DebugRenderer.BodyDrawFilterFuncAlignment) callconv(.C) bool {
        return true;
    }

    fn drawLine (
        self: *MyDebugRenderer,
        from: *const [3]zphy.Real,
        to: *const [3]zphy.Real,
        color: *const zphy.DebugRenderer.Color,
    ) callconv(.C) void {
        _ = self;
        _ = from;
        _ = to;
        _ = color;
    }
    fn drawTriangle (
        self: *MyDebugRenderer,
        v1: *const [3]zphy.Real,
        v2: *const [3]zphy.Real,
        v3: *const [3]zphy.Real,
        color: *const zphy.DebugRenderer.Color,
    ) callconv(.C) void {
        _ = self;
        _ = v1;
        _ = v2;
        _ = v3;
        _ = color;
    }
    fn createTriangleBatch (
        self: *MyDebugRenderer,
        triangles: [*]zphy.DebugRenderer.Triangle,
        triangle_count: u32,
    ) callconv(.C) *anyopaque {
        _ = triangles;
        _ = triangle_count;
        self.prim_head += 1;
        const prim = &self.primitives[@intCast(usize, self.prim_head)];
        return zphy.DebugRenderer.createTriangleBatch(prim);
    }
    fn createTriangleBatchIndexed (
        self: *MyDebugRenderer,
        vertices: [*]zphy.DebugRenderer.Vertex,
        vertex_count: u32,
        indices: [*]u32,
        index_count: u32,
    ) callconv(.C) *anyopaque {
        _ = vertices;
        _ = vertex_count;
        _ = indices;
        _ = index_count;
        self.prim_head += 1;
        const prim = &self.primitives[@intCast(usize, self.prim_head)];
        return zphy.DebugRenderer.createTriangleBatch(prim);
    }
    fn drawGeometry (
        self: *MyDebugRenderer,
        model_matrix: *const [16]zphy.Real,
        world_space_bound: *const zphy.DebugRenderer.AABox,
        lod_scale_sq: f32,
        color: zphy.DebugRenderer.Color,
        geometry: *anyopaque,
        cull_mode: zphy.DebugRenderer.CullMode,
        cast_shadow: zphy.DebugRenderer.CastShadow,
        draw_mode: zphy.DebugRenderer.DrawMode,
    ) callconv(.C) void {
        _ = self;
        _ = model_matrix;
        _ = world_space_bound;
        _ = lod_scale_sq;
        _ = color;
        _ = geometry;
        _ = cull_mode;
        _ = cast_shadow;
        _ = draw_mode;
    }
    fn drawText3D (
        self: *MyDebugRenderer,
        positions: *const [3]zphy.Real,
        string: [*:0]const u8,
        color: zphy.DebugRenderer.Color,
        height: f32,
    ) callconv(.C) void {
        _ = self;
        _ = positions;
        _ = string;
        _ = color;
        _ = height;
    }
};

fn appendMesh(
    mesh: zmesh.Shape,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(Mesh.IndexType),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    meshes.append(.{
        .index_offset = @intCast(u32, meshes_indices.items.len),
        .vertex_offset = @intCast(i32, meshes_positions.items.len),
        .num_indices = @intCast(u32, mesh.indices.len),
        .num_vertices = @intCast(u32, mesh.positions.len),
    }) catch unreachable;

    meshes_indices.appendSlice(mesh.indices) catch unreachable;
    meshes_positions.appendSlice(mesh.positions) catch unreachable;
    meshes_normals.appendSlice(mesh.normals.?) catch unreachable;
}

const mesh_floor = 0;
const mesh_cube = 1;

fn generateMeshes(
    allocator: std.mem.Allocator,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(Mesh.IndexType),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    zmesh.init(arena);
    defer zmesh.deinit();

    {
        const terrain = (struct {
            fn impl(uv: *const [2]f32, position: *[3]f32, userdata: ?*anyopaque) callconv(.C) void {
                _ = userdata;
                position[0] = uv[0];
                position[1] = 0.0;
                position[2] = uv[1];
            }
        }).impl;
        var ground = zmesh.Shape.initParametric(terrain, 32, 32, null);
        defer ground.deinit();
        ground.translate(-0.5, 0.0, -0.5);
        ground.invert(0, 0);
        ground.scale(2000.0, 1.0, 2000.0);
        ground.unweld();
        ground.computeNormals();

        appendMesh(ground, meshes, meshes_indices, meshes_positions, meshes_normals);
    }

    {
        var cube = zmesh.Shape.initCube();
        defer cube.deinit();
        cube.translate(-0.5, -0.5, -0.5);
        cube.unweld();
        cube.computeNormals();

        appendMesh(cube, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
}

fn create(allocator: std.mem.Allocator, window: *zglfw.Window) !*DemoState {
    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    //
    // Procedural meshes
    //
    var meshes = std.ArrayList(Mesh).init(allocator);
    var meshes_indices = std.ArrayList(Mesh.IndexType).init(arena);
    var meshes_positions = std.ArrayList([3]f32).init(arena);
    var meshes_normals = std.ArrayList([3]f32).init(arena);
    generateMeshes(allocator, &meshes, &meshes_indices, &meshes_positions, &meshes_normals);

    const total_num_vertices = @intCast(u32, meshes_positions.items.len);
    const total_num_indices = @intCast(u32, meshes_indices.items.len);

    //
    // Graphics
    //
    const gctx = try zgpu.GraphicsContext.create(allocator, window);

    // Uniform buffer and layout
    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    const uniform_bg = gctx.createBindGroup(uniform_bgl, &.{
        .{
            .binding = 0,
            .buffer_handle = gctx.uniforms.buffer,
            .offset = 0,
            .size = @max(@sizeOf(FrameUniforms), @sizeOf(DrawUniforms)),
        },
    });

    // Vertex buffer
    const vertex_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    {
        var vertex_data = std.ArrayList(Vertex).init(arena);
        defer vertex_data.deinit();
        vertex_data.resize(total_num_vertices) catch unreachable;

        for (meshes_positions.items, 0..) |_, i| {
            vertex_data.items[i].position = meshes_positions.items[i];
            vertex_data.items[i].second.normal = meshes_normals.items[i];
        }
        gctx.queue.writeBuffer(gctx.lookupResource(vertex_buf).?, 0, Vertex, vertex_data.items);
    }

    // Index buffer
    const index_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(Mesh.IndexType),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buf).?, 0, Mesh.IndexType, meshes_indices.items);

    // Depth texture
    const depth = createDepthTexture(gctx);

    //
    // Physics
    //
    try zphy.init(allocator, .{});

    const broad_phase_layer_interface = try allocator.create(BroadPhaseLayerInterface);
    broad_phase_layer_interface.* = BroadPhaseLayerInterface.init();

    const object_vs_broad_phase_layer_filter = try allocator.create(ObjectVsBroadPhaseLayerFilter);
    object_vs_broad_phase_layer_filter.* = .{};

    const object_layer_pair_filter = try allocator.create(ObjectLayerPairFilter);
    object_layer_pair_filter.* = .{};

    const contact_listener = try allocator.create(ContactListener);
    contact_listener.* = .{};

    const physics_system = try zphy.PhysicsSystem.create(
        @ptrCast(*const zphy.BroadPhaseLayerInterface, broad_phase_layer_interface),
        @ptrCast(*const zphy.ObjectVsBroadPhaseLayerFilter, object_vs_broad_phase_layer_filter),
        @ptrCast(*const zphy.ObjectLayerPairFilter, object_layer_pair_filter),
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );

    //
    // Demo
    //
    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .uniform_bg = uniform_bg,
        .vertex_buf = vertex_buf,
        .index_buf = index_buf,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .meshes = meshes,
        .broad_phase_layer_interface = broad_phase_layer_interface,
        .object_vs_broad_phase_layer_filter = object_vs_broad_phase_layer_filter,
        .object_layer_pair_filter = object_layer_pair_filter,
        .contact_listener = contact_listener,
        .physics_system = physics_system,
        .physics_debug_renderer = .{
            .demo = demo,
            .body_draw_filter = zphy.DebugRenderer.createBodyDrawFilter(MyDebugRenderer.shouldBodyDraw),
        },
    };

    //
    // Physics Objects
    //
    {
        const body_interface = physics_system.getBodyInterfaceMut();

        const floor_shape_settings = try zphy.BoxShapeSettings.create(.{ 500.0, 1.0, 500.0 });
        defer floor_shape_settings.release();

        const floor_shape = try floor_shape_settings.createShape();
        defer floor_shape.release();

        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0.0, -1.0, 0.0, 1.0 },
            .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);

        const box_shape_settings = try zphy.BoxShapeSettings.create(.{ 0.5, 0.5, 0.5 });
        defer box_shape_settings.release();

        const box_shape = try box_shape_settings.createShape();
        defer box_shape.release();

        var i: u32 = 0;
        while (i < 9) : (i += 1) {
            const fi = @floatFromInt(f32, i);
            const angle: f32 = std.math.degreesToRadians(f32, fi * 40.0);
            demo.physics_objects[i] = try body_interface.createAndAddBody(.{
                .position = .{ 6.0 * std.math.cos(angle), 4.0, 6.0 * std.math.sin(angle), 1.0 },
                .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
                .shape = box_shape,
                .motion_type = .dynamic,
                .object_layer = object_layers.moving,
                .angular_velocity = .{ 0.0, 0.0, 0.0, 0 },
            }, .activate);
        }

        physics_system.optimizeBroadPhase();
    }

    // Physics Debug Renderer
    try zphy.DebugRenderer.createSingleton(&demo.physics_debug_renderer);

    //
    // GPU pipelines (async compiled; need to be created *after* the `demo` instance is constructed)
    //
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.mesh.vs,
        wgsl.mesh.fs,
        @sizeOf(Vertex),
        &.{
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @offsetOf(Vertex, "second"), .shader_location = 1 },
        },
        .{
            .topology = .triangle_list,
            .front_face = .cw,
            .cull_mode= .back,
        },
        zgpu.GraphicsContext.swapchain_format,
        .{
            .format = .depth32_float,
            .depth_write_enabled = true,
            .depth_compare = .less,
        },
        &demo.mesh_render_pipe,
    );
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.line.vs,
        wgsl.line.fs,
        @sizeOf(Vertex),
        &.{
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @offsetOf(Vertex, "second"), .shader_location = 1 },
        },
        .{ .topology = .line_list },
        zgpu.GraphicsContext.swapchain_format,
        .{
            .format = .depth32_float,
            .depth_write_enabled = true,
            .depth_compare = .less,
        },
        &demo.line_render_pipe,
    );

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    zphy.DebugRenderer.destroyBodyDrawFilter(demo.physics_debug_renderer.body_draw_filter);
    zphy.DebugRenderer.destroySingleton();
    demo.meshes.deinit();
    demo.physics_system.destroy();
    allocator.destroy(demo.contact_listener);
    allocator.destroy(demo.object_vs_broad_phase_layer_filter);
    allocator.destroy(demo.object_layer_pair_filter);
    allocator.destroy(demo.broad_phase_layer_interface);
    zphy.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
    zgui.backend.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);
    demo.physics_system.update(1.0 / 60.0, .{}) catch unreachable;

    const window = demo.gctx.window;

    // Handle camera rotation with mouse.
    {
        const cursor_pos = window.getCursorPos();
        const delta_x = @floatCast(f32, cursor_pos[0] - demo.mouse.cursor_pos[0]);
        const delta_y = @floatCast(f32, cursor_pos[1] - demo.mouse.cursor_pos[1]);
        demo.mouse.cursor_pos = cursor_pos;

        if (window.getMouseButton(.right) == .press) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = @min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = @max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = zm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));

        const up = zm.f32x4(0.0, 1.0, 0.0, 0.0);
        const forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));
        const right = zm.normalize3(zm.cross3(up, forward));

        zm.storeArr3(&demo.camera.forward, forward);
        var cam_pos = zm.loadArr3(demo.camera.position);

        var speed = zm.f32x4s(25.0);
        if (window.getKey(.left_shift) == .press) {
            speed *= zm.f32x4s(4.0);
        }
        if (window.getKey(.w) == .press) {
            cam_pos += speed * delta_time * forward;
        } else if (window.getKey(.s) == .press) {
            cam_pos -= speed * delta_time * forward;
        }
        if (window.getKey(.d) == .press) {
            cam_pos += speed * delta_time * right;
        } else if (window.getKey(.a) == .press) {
            cam_pos -= speed * delta_time * right;
        }
        if (window.getKey(.space) == .press) {
            cam_pos += speed * delta_time * up;
        } else if (window.getKey(.left_control) == .press) {
            cam_pos -= speed * delta_time * up;
        }

        zm.storeArr3(&demo.camera.position, cam_pos);
    }
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const cam_world_to_view = zm.lookToLh(
        zm.loadArr3(demo.camera.position),
        zm.loadArr3(demo.camera.forward),
        zm.f32x4(0.0, 1.0, 0.0, 0.0),
    );
    const cam_view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @floatFromInt(f32, fb_width) / @floatFromInt(f32, fb_height),
        0.1,
        10000.0,
    );
    const cam_world_to_clip = zm.mul(cam_world_to_view, cam_view_to_clip);

    // Lookup common resources which may be needed for all the passes.
    const depth_texv = gctx.lookupResource(demo.depth_texv) orelse return;
    const uniform_bg = gctx.lookupResource(demo.uniform_bg) orelse return;
    const vertex_buf_info = gctx.lookupResourceInfo(demo.vertex_buf) orelse return;
    const index_buf_info = gctx.lookupResourceInfo(demo.index_buf) orelse return;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        pass: {
            const mesh_render_pipe = gctx.lookupResource(demo.mesh_render_pipe) orelse break :pass;
            const line_render_pipe = gctx.lookupResource(demo.line_render_pipe) orelse break :pass;

            const pass = zgpu.beginRenderPassSimple(
                encoder,
                .clear,
                swapchain_texv,
                .{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 },
                depth_texv,
                1.0,
            );
            defer zgpu.endReleasePass(pass);

            pass.setVertexBuffer(0, vertex_buf_info.gpuobj.?, 0, vertex_buf_info.size);
            pass.setIndexBuffer(
                index_buf_info.gpuobj.?,
                if (Mesh.IndexType == u16) .uint16 else .uint32,
                0,
                index_buf_info.size,
            );
            pass.setPipeline(mesh_render_pipe);

            const box_scale = zm.scaling(10, 50, 5);
            const box_rotate = zm.mul(zm.rotationZ(0.24), zm.rotationY(0.3));
            const box_center = zm.f32x4(-15, 0, 30, 1);
            const box_radius = zm.mul(box_scale, zm.f32x4(0.4999, 0.4999, 0.4999, 0)); // .4999 is anti-artifact bias
            const box_translate = zm.transpose(zm.translationV(box_center));
            const box_transform = zm.mul(box_translate, zm.mul(box_rotate, box_scale));
            const box_rotate_t = zm.transpose(box_rotate);
            const floor_material = zm.f32x4(-0.9, -0.9, -0.9, 0.8);

            { // Update frame uniforms
                const mem = gctx.uniformsAllocate(FrameUniforms, 1);
                mem.slice[0] = .{
                    .world_to_clip = zm.transpose(cam_world_to_clip),
                    .floor_material = floor_material,
                    .box_rotation = .{ box_rotate_t[0], box_rotate_t[1], box_rotate_t[2] },
                    .box_center = box_center,
                    .box_radius = box_radius,
                    .box_inv_radius = .{ 1.0 / box_radius[0], 1.0 / box_radius[1], 1.0 / box_radius[2], 0 },
                    .camera_position = demo.camera.position,
                };
                pass.setBindGroup(0, uniform_bg, &.{mem.offset});
            }

            { // Draw floor
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.identity(),
                    .basecolor_roughness = floor_material,
                    // .basecolor_roughness = .{ 0.9, 0.9, 0.9, -0.3 },
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_floor].num_indices,
                    1,
                    demo.meshes.items[mesh_floor].index_offset,
                    demo.meshes.items[mesh_floor].vertex_offset,
                    0,
                );
            }

            { // Draw skybox-analogue (lazy way to ensure whole frame runs fragment shader)
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.scaling(-2000, -2000, -2000),
                    .basecolor_roughness = .{ 0.0, 0.0, 0.0, 1.0 },
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_cube].num_indices,
                    1,
                    demo.meshes.items[mesh_cube].index_offset,
                    demo.meshes.items[mesh_cube].vertex_offset,
                    0,
                );
            }

            { // Draw monolith
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = box_transform,
                    // .basecolor_roughness = .{ 0.03, 0.02, 0.04, -0.16 },
                    // .basecolor_roughness = .{ 0.3, 0.3, 0.3, -0.02 },
                    // .basecolor_roughness = .{ 0.92, 0.9, 0.94, -0.24 },
                    .basecolor_roughness = .{ 0.010, 0.010, 0.014, -0.05 },
                    // .basecolor_roughness = .{ 0.010, 0.010, 0.014, -0.3 },
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_cube].num_indices,
                    1,
                    demo.meshes.items[mesh_cube].index_offset,
                    demo.meshes.items[mesh_cube].vertex_offset,
                    0,
                );
            }

            pass.setPipeline(line_render_pipe);

            demo.physics_system.drawBodies(
                &demo.physics_debug_renderer.body_draw_settings,
                demo.physics_debug_renderer.body_draw_filter,
            );
        }

        {
            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.endReleasePass(pass);
            zgui.backend.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    tex: zgpu.TextureHandle,
    texv: zgpu.TextureViewHandle,
} {
    const tex = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .tdim_2d,
        .size = .{
            .width = gctx.swapchain_descriptor.width,
            .height = gctx.swapchain_descriptor.height,
            .depth_or_array_layers = 1,
        },
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const texv = gctx.createTextureView(tex, .{});
    return .{ .tex = tex, .texv = texv };
}

pub fn main() !void {
    zglfw.init() catch {
        std.log.err("Failed to initialize GLFW library.", .{});
        return;
    };
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    const window = zglfw.Window.create(1600, 1000, window_title, null) catch {
        std.log.err("Failed to create demo window.", .{});
        return;
    };
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = create(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer destroy(allocator, demo);

    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };

    zgui.init(allocator);
    defer zgui.deinit();

    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(16.0 * scale_factor));

    zgui.backend.init(
        window,
        demo.gctx.device,
        @intFromEnum(zgpu.GraphicsContext.swapchain_format),
    );
    defer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();

        update(demo);
        draw(demo);

        if (demo.gctx.present() == .swap_chain_resized) {
            // Release old depth texture.
            demo.gctx.releaseResource(demo.depth_texv);
            demo.gctx.destroyResource(demo.depth_tex);

            // Create a new depth texture to match the new window size.
            const depth = createDepthTexture(demo.gctx);
            demo.depth_tex = depth.tex;
            demo.depth_texv = depth.texv;
        }
    }
}
