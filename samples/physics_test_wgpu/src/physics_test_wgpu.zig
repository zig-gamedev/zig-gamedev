const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zphy = @import("zphysics");
const zmesh = @import("zmesh");
const wgsl = @import("physics_test_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: physics test (wgpu)";

const FrameUniforms = struct {
    world_to_clip: zm.Mat,
    camera_position: [3]f32,
};

const DrawUniforms = struct {
    object_to_world: zm.Mat,
    basecolor_roughness: [4]f32,
};

const Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
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
        const self = @as(*const BroadPhaseLayerInterface, @ptrCast(iself));
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
    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,

    render_pipe: zgpu.RenderPipelineHandle = .{},
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

    camera: struct {
        position: [3]f32 = .{ 0.0, 8.0, -8.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.125 * math.pi,
        yaw: f32 = 0.0,
    } = .{},
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
    } = .{},
};

fn appendMesh(
    mesh: zmesh.Shape,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(Mesh.IndexType),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    meshes.append(.{
        .index_offset = @as(u32, @intCast(meshes_indices.items.len)),
        .vertex_offset = @as(i32, @intCast(meshes_positions.items.len)),
        .num_indices = @as(u32, @intCast(mesh.indices.len)),
        .num_vertices = @as(u32, @intCast(mesh.positions.len)),
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
        ground.scale(128.0, 1.0, 128.0);
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

    const total_num_vertices = @as(u32, @intCast(meshes_positions.items.len));
    const total_num_indices = @as(u32, @intCast(meshes_indices.items.len));

    //
    // Graphics
    //
    const gctx = try zgpu.GraphicsContext.create(
        allocator,
        .{
            .window = window,
            .fn_getTime = @ptrCast(&zglfw.getTime),
            .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
            .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
            .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
            .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
            .fn_getWaylandDisplay = @ptrCast(&zglfw.getWaylandDisplay),
            .fn_getWaylandSurface = @ptrCast(&zglfw.getWaylandWindow),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

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
            vertex_data.items[i].normal = meshes_normals.items[i];
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
        @as(*const zphy.BroadPhaseLayerInterface, @ptrCast(broad_phase_layer_interface)),
        @as(*const zphy.ObjectVsBroadPhaseLayerFilter, @ptrCast(object_vs_broad_phase_layer_filter)),
        @as(*const zphy.ObjectLayerPairFilter, @ptrCast(object_layer_pair_filter)),
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );

    {
        const body_interface = physics_system.getBodyInterfaceMut();

        const floor_shape_settings = try zphy.BoxShapeSettings.create(.{ 100.0, 1.0, 100.0 });
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
        while (i < 16) : (i += 1) {
            const fi = @as(f32, @floatFromInt(i));
            _ = try body_interface.createAndAddBody(.{
                .position = .{ 0.0, 8.0 + fi * 1.2, 8.0, 1.0 },
                .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
                .shape = box_shape,
                .motion_type = .dynamic,
                .object_layer = object_layers.moving,
                .angular_velocity = .{ 0.0, 0.0, 0.0, 0 },
                //.allow_sleeping = false,
            }, .activate);
        }

        physics_system.optimizeBroadPhase();
    }

    //
    // Demo
    //
    const demo = try allocator.create(DemoState);
    demo.* = .{
        .window = window,
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
    };

    //
    // GPU pipelines (async compiled; need to be created *after* the `demo` instance is constructed)
    //
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.vs,
        wgsl.fs,
        @sizeOf(Vertex),
        &.{
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
        },
        .{ .topology = .triangle_list },
        zgpu.GraphicsContext.swapchain_format,
        .{
            .format = .depth32_float,
            .depth_write_enabled = true,
            .depth_compare = .less,
        },
        &demo.render_pipe,
    );

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
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

const frame_rate_target = 60.0;

fn update(demo: *DemoState) void {
    zgui.backend.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);
    demo.physics_system.update(1.0 / frame_rate_target, .{}) catch unreachable;

    const window = demo.window;

    // Handle camera rotation with mouse.
    {
        const cursor_pos = window.getCursorPos();
        const delta_x = @as(f32, @floatCast(cursor_pos[0] - demo.mouse.cursor_pos[0]));
        const delta_y = @as(f32, @floatCast(cursor_pos[1] - demo.mouse.cursor_pos[1]));
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
        const speed = zm.f32x4s(2.0);
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.storeArr3(&demo.camera.forward, forward);

        const right = speed * delta_time *
            zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var cam_pos = zm.loadArr3(demo.camera.position);

        if (window.getKey(.w) == .press) {
            cam_pos += forward;
        } else if (window.getKey(.s) == .press) {
            cam_pos -= forward;
        }
        if (window.getKey(.d) == .press) {
            cam_pos += right;
        } else if (window.getKey(.a) == .press) {
            cam_pos -= right;
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
        @as(f32, @floatFromInt(fb_width)) / @as(f32, @floatFromInt(fb_height)),
        0.01,
        200.0,
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
            const render_pipe = gctx.lookupResource(demo.render_pipe) orelse break :pass;

            const pass = zgpu.beginRenderPassSimple(
                encoder,
                .clear,
                swapchain_texv,
                .{ .r = 0.2, .g = 0.4, .b = 0.8, .a = 1.0 },
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
            pass.setPipeline(render_pipe);

            // Update "world to clip" (camera) xform.
            {
                const mem = gctx.uniformsAllocate(FrameUniforms, 1);
                mem.slice[0] = .{
                    .world_to_clip = zm.transpose(cam_world_to_clip),
                    .camera_position = demo.camera.position,
                };
                pass.setBindGroup(0, uniform_bg, &.{mem.offset});
            }

            // Draw "floor".
            {
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.identity(),
                    .basecolor_roughness = .{ 0.2, 0.2, 0.2, 1.0 },
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

            // Draw all dynamic bodies.
            const bodies = demo.physics_system.getBodiesUnsafe();
            for (bodies) |body| {
                if (!zphy.isValidBodyPointer(body) or body.motion_properties == null) continue;

                const object_to_world = object_to_world: {
                    const position = if (zphy.Real == f32)
                        zm.loadArr4(body.position)
                    else
                        zm.loadArr4(.{
                            @as(f32, @floatCast(body.position[0])),
                            @as(f32, @floatCast(body.position[1])),
                            @as(f32, @floatCast(body.position[2])),
                            @as(f32, @floatCast(body.position[3])),
                        });
                    const rotation = zm.loadArr4(body.rotation);
                    var xform = zm.matFromQuat(rotation);
                    xform[3] = position;
                    xform[3][3] = 1.0;
                    break :object_to_world xform;
                };

                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.transpose(object_to_world),
                    .basecolor_roughness = .{ 0.1, 0.5, 0.05, 0.5 },
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
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(1600, 1000, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try create(allocator, window);
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
        @intFromEnum(wgpu.TextureFormat.undef),
    );
    defer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    var frame_timer = try std.time.Timer.start();

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        {
            // spin loop for frame limiter
            const target_ns = @divTrunc(std.time.ns_per_s, frame_rate_target);
            while (frame_timer.read() < target_ns) {
                std.atomic.spinLoopHint();
            }
            frame_timer.reset();
        }

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
