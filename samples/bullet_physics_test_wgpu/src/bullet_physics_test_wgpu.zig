const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zmesh = @import("zmesh");
const zbt = @import("zbullet");
const wgsl = @import("bullet_physics_test_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: bullet physics test (wgpu)";

const Vertex = extern struct {
    position: [3]f32,
    normal: [3]f32,
};

const FrameUniforms = struct {
    world_to_clip: zm.Mat,
    camera_position: [3]f32,
};

const DrawUniforms = struct {
    object_to_world: zm.Mat,
    basecolor_roughness: [4]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    num_indices: u32,
    num_vertices: u32,
};

const Entity = struct {
    body: zbt.Body,
    basecolor_roughness: [4]f32,
    size: [3]f32,
    mesh_index: u32,
};

const Camera = struct {
    position: [3]f32,
    forward: [3]f32 = .{ 0, 0, 0 },
    pitch: f32,
    yaw: f32,
};

const scenes = [_]Scene{
    .{ .name = "Collision shapes", .setup = setupScene0 },
    .{ .name = "Stacks of boxes", .setup = setupScene1 },
    .{ .name = "Pyramid", .setup = setupScene2 },
    .{ .name = "Tower", .setup = setupScene3, .has_gravity_ui = false },
};
const initial_scene = 0;

const mesh_index_cube: u32 = 0;
const mesh_index_sphere: u32 = 1;
const mesh_index_cylinder: u32 = 2;
const mesh_index_capsule: u32 = 3;
const mesh_index_compound0: u32 = 4;
const mesh_index_compound1: u32 = 5;
const mesh_index_world: u32 = 6;
const mesh_count: u32 = 7;

const default_linear_damping: f32 = 0.05;
const default_angular_damping: f32 = 0.05;
const safe_uniform_size = 256;
const camera_fovy: f32 = math.pi / @as(f32, 3.0);
const ccd_motion_threshold: f32 = 1e-7;
const ccd_swept_sphere_radius: f32 = 0.5;
const default_gravity: f32 = 10.0;

const DemoState = struct {
    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,

    mesh_pipe: zgpu.RenderPipelineHandle = .{},
    physics_debug_pipe: zgpu.RenderPipelineHandle = .{},

    vertex_buf: zgpu.BufferHandle,
    index_buf: zgpu.BufferHandle,

    physics_debug_buf: zgpu.BufferHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    uniform_bg: zgpu.BindGroupHandle,

    meshes: std.ArrayList(Mesh),
    entities: std.ArrayList(Entity),

    keyboard_delay: f32 = 1.0,
    current_scene_index: i32 = initial_scene,

    physics: struct {
        world: zbt.World,
        common_shapes: std.ArrayList(zbt.Shape),
        scene_shapes: std.ArrayList(zbt.Shape),
        debug: *zbt.DebugDrawer,
    },
    camera: Camera,
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
    } = .{},
    pick: struct {
        body: ?zbt.Body = null,
        p2p: zbt.Point2PointConstraint,
        saved_linear_damping: f32 = 0.0,
        saved_angular_damping: f32 = 0.0,
        saved_activation_state: zbt.BodyActivationState = .active,
        distance: f32 = 0.0,
    },
};

fn create(allocator: std.mem.Allocator, window: *zglfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.create(
        allocator,
        .{
            .window = window,
            .fn_getTime = @ptrCast(&zglfw.getTime),
            .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
            .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
            .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
            .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    //
    // Create meshes.
    //
    zmesh.init(arena);
    defer zmesh.deinit();

    var common_shapes = std.ArrayList(zbt.Shape).init(allocator);
    var meshes = std.ArrayList(Mesh).init(allocator);
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList([3]f32).init(arena);
    var normals = std.ArrayList([3]f32).init(arena);
    try initMeshes(arena, &common_shapes, &meshes, &indices, &positions, &normals);

    const total_num_vertices = @as(u32, @intCast(positions.items.len));
    const total_num_indices = @as(u32, @intCast(indices.items.len));

    // Create a vertex buffer.
    const vertex_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    {
        var vertex_data = std.ArrayList(Vertex).init(arena);
        defer vertex_data.deinit();
        try vertex_data.resize(total_num_vertices);

        for (positions.items, 0..) |_, i| {
            vertex_data.items[i].position = positions.items[i];
            vertex_data.items[i].normal = normals.items[i];
        }
        gctx.queue.writeBuffer(gctx.lookupResource(vertex_buf).?, 0, Vertex, vertex_data.items);
    }

    // Create an index buffer.
    const index_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(u32),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buf).?, 0, u32, indices.items);

    const physics_debug_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = 1024 * @sizeOf(zbt.DebugDrawer.Vertex),
    });

    //
    // Create textures.
    //
    const depth = createDepthTexture(gctx);

    //
    // Create bind groups.
    //
    const uniform_bg = gctx.createBindGroup(uniform_bgl, &[_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = safe_uniform_size,
    }});

    //
    // Init physics.
    //
    const physics_world = zbt.initWorld();
    physics_world.setGravity(&.{ 0.0, -default_gravity, 0.0 });

    var physics_debug = try allocator.create(zbt.DebugDrawer);
    physics_debug.* = zbt.DebugDrawer.init(allocator);

    physics_world.debugSetDrawer(&physics_debug.getDebugDraw());
    physics_world.debugSetMode(zbt.DebugMode.user_only);

    var scene_shapes = std.ArrayList(zbt.Shape).init(allocator);
    var entities = std.ArrayList(Entity).init(allocator);
    var camera: Camera = undefined;
    scenes[initial_scene].setup(physics_world, common_shapes, &scene_shapes, &entities, &camera);

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .window = window,
        .gctx = gctx,
        .vertex_buf = vertex_buf,
        .index_buf = index_buf,
        .physics_debug_buf = physics_debug_buf,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .uniform_bg = uniform_bg,
        .meshes = meshes,
        .entities = entities,
        .camera = camera,
        .physics = .{
            .world = physics_world,
            .common_shapes = common_shapes,
            .scene_shapes = scene_shapes,
            .debug = physics_debug,
        },
        .pick = .{
            .p2p = zbt.allocPoint2PointConstraint(),
        },
    };

    //
    // Create pipelines.
    //
    const common_depth_state = wgpu.DepthStencilState{
        .format = .depth32_float,
        .depth_write_enabled = true,
        .depth_compare = .less,
    };

    const pos_norm_attribs = [_]wgpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
    };
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.mesh_vs,
        wgsl.mesh_fs,
        @sizeOf(Vertex),
        pos_norm_attribs[0..],
        .{ .front_face = .cw, .cull_mode = .none },
        zgpu.GraphicsContext.swapchain_format,
        common_depth_state,
        &demo.mesh_pipe,
    );

    const pos_color_attribs = [_]wgpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .uint32, .offset = @offsetOf(zbt.DebugDrawer.Vertex, "color"), .shader_location = 1 },
    };
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{uniform_bgl},
        wgsl.physics_debug_vs,
        wgsl.physics_debug_fs,
        @sizeOf(zbt.DebugDrawer.Vertex),
        pos_color_attribs[0..],
        .{ .topology = .line_list },
        zgpu.GraphicsContext.swapchain_format,
        common_depth_state,
        &demo.physics_debug_pipe,
    );

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    if (demo.pick.p2p.isCreated()) {
        demo.physics.world.removeConstraint(demo.pick.p2p.asConstraint());
        demo.pick.p2p.destroy();
    }
    demo.pick.p2p.dealloc();
    cleanupScene(demo.physics.world, &demo.physics.scene_shapes, &demo.entities);
    demo.physics.scene_shapes.deinit();
    for (demo.physics.common_shapes.items) |shape| shape.deinit();
    demo.physics.common_shapes.deinit();
    demo.physics.debug.deinit();
    allocator.destroy(demo.physics.debug);
    demo.physics.world.deinit();
    demo.entities.deinit();
    demo.meshes.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
    const dt = demo.gctx.stats.delta_time;
    _ = demo.physics.world.stepSimulation(dt, .{});

    const want_capture_mouse = zgui.io.getWantCaptureMouse();
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .always });
    zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0, .cond = .always });

    if (zgui.begin("Demo Settings", .{ .flags = .{ .no_move = true, .no_resize = true } })) {
        zgui.bulletText(
            "Average : {d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );
        zgui.bulletText("LMB + drag : pick up and move object", .{});
        zgui.bulletText("RMB + drag : rotate camera", .{});
        zgui.bulletText("W, A, S, D : move camera", .{});
        zgui.bulletText("Space : shoot", .{});
        zgui.bulletText("Number of objects : {}", .{demo.physics.world.getNumBodies()});
        // Scene selection.
        {
            zgui.spacing();
            zgui.spacing();
            comptime var str: [:0]const u8 = "";
            comptime var i: u32 = 0;
            inline while (i < scenes.len) : (i += 1) {
                str = str ++ "Scene: " ++ scenes[i].name ++ "\x00";
            }
            str = str ++ "\x00";
            _ = zgui.combo(
                "##",
                .{ .current_item = &demo.current_scene_index, .items_separated_by_zeros = str },
            );
            zgui.sameLine(.{});
            if (zgui.button("  Setup Scene  ", .{})) {
                cleanupScene(demo.physics.world, &demo.physics.scene_shapes, &demo.entities);
                // Call scene-setup function.
                scenes[@as(usize, @intCast(demo.current_scene_index))].setup(
                    demo.physics.world,
                    demo.physics.common_shapes,
                    &demo.physics.scene_shapes,
                    &demo.entities,
                    &demo.camera,
                );
            }
        }
        // Gravity.
        {
            const is_enabled = scenes[@as(usize, @intCast(demo.current_scene_index))].has_gravity_ui;
            if (!is_enabled) {
                zgui.beginDisabled(.{});
            }
            var gravity: [3]f32 = undefined;
            demo.physics.world.getGravity(&gravity);
            if (zgui.sliderFloat(
                "Gravity",
                .{ .v = &gravity[1], .min = -default_gravity, .max = default_gravity },
            )) {
                demo.physics.world.setGravity(&gravity);
            }
            if (zgui.button("  Disable gravity  ", .{})) {
                demo.physics.world.setGravity(&.{ 0, 0, 0 });
            }
            if (!is_enabled) {
                zgui.endDisabled();
            }
        }
        // Debug draw mode.
        {
            var is_enabled = demo.physics.world.debugGetMode().draw_wireframe;
            _ = zgui.checkbox("Debug draw enabled", .{ .v = &is_enabled });
            if (is_enabled) {
                demo.physics.world.debugSetMode(.{ .draw_wireframe = true, .draw_aabb = true });
            } else {
                demo.physics.world.debugSetMode(zbt.DebugMode.user_only);
            }
        }
    }
    zgui.end();

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
        const speed = zm.f32x4s(5.0);
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.storeArr3(&demo.camera.forward, forward);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
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

    objectPicking(demo, want_capture_mouse);

    // Shooting.
    {
        demo.keyboard_delay += dt;
        if (window.getKey(.space) == .press and demo.keyboard_delay >= 0.5) {
            demo.keyboard_delay = 0.0;

            const transform = zm.translationV(zm.loadArr3(demo.camera.position));
            const impulse = zm.f32x4s(80.0) * zm.loadArr3(demo.camera.forward);

            const body = zbt.initBody(
                1.0,
                &zm.matToArr43(transform),
                demo.physics.common_shapes.items[mesh_index_sphere],
            );
            body.applyCentralImpulse(zm.arr3Ptr(&impulse));

            createEntity(demo.physics.world, body, .{ 0.0, 0.8, 0.0, 0.2 }, &demo.entities);
        }
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
        camera_fovy,
        @as(f32, @floatFromInt(fb_width)) / @as(f32, @floatFromInt(fb_height)),
        0.01,
        200.0,
    );
    const cam_world_to_clip = zm.mul(cam_world_to_view, cam_view_to_clip);

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Main pass.
        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buf) orelse break :pass;
            const ib_info = gctx.lookupResourceInfo(demo.index_buf) orelse break :pass;
            const mesh_pipe = gctx.lookupResource(demo.mesh_pipe) orelse break :pass;
            const uniform_bg = gctx.lookupResource(demo.uniform_bg) orelse break :pass;
            const depth_texv = gctx.lookupResource(demo.depth_texv) orelse break :pass;

            const pass = zgpu.beginRenderPassSimple(encoder, .clear, swapchain_texv, null, depth_texv, 1.0);
            defer zgpu.endReleasePass(pass);

            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setIndexBuffer(ib_info.gpuobj.?, .uint32, 0, ib_info.size);
            pass.setPipeline(mesh_pipe);
            {
                const mem = gctx.uniformsAllocate(FrameUniforms, 1);
                mem.slice[0] = .{
                    .world_to_clip = zm.transpose(cam_world_to_clip),
                    .camera_position = demo.camera.position,
                };
                pass.setBindGroup(0, uniform_bg, &.{mem.offset});
            }

            const num_bodies = demo.physics.world.getNumBodies();
            var body_index: i32 = 0;
            while (body_index < num_bodies) : (body_index += 1) {
                const body = demo.physics.world.getBody(body_index);
                const entity = &demo.entities.items[@as(usize, @intCast(body.getUserIndex(0)))];

                // Get transform matrix from the physics simulator.
                const transform = object_to_world: {
                    var transform: [12]f32 = undefined;
                    body.getGraphicsWorldTransform(&transform);
                    break :object_to_world zm.loadMat43(transform[0..]);
                };
                const object_to_world = zm.mul(zm.scalingV(zm.loadArr3(entity.size)), transform);

                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.transpose(object_to_world),
                    .basecolor_roughness = entity.basecolor_roughness,
                };

                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[entity.mesh_index].num_indices,
                    1,
                    demo.meshes.items[entity.mesh_index].index_offset,
                    @as(i32, @intCast(demo.meshes.items[entity.mesh_index].vertex_offset)),
                    0,
                );
            }
        }

        // Physics debug pass.
        pass: {
            demo.physics.world.debugDrawAll();
            const num_vertices = @as(u32, @intCast(demo.physics.debug.lines.items.len));
            if (num_vertices == 0) break :pass;

            var vb_info = gctx.lookupResourceInfo(demo.physics_debug_buf) orelse break :pass;
            const physics_debug_pipe = gctx.lookupResource(demo.physics_debug_pipe) orelse break :pass;
            const uniform_bg = gctx.lookupResource(demo.uniform_bg) orelse break :pass;
            const depth_texv = gctx.lookupResource(demo.depth_texv) orelse break :pass;

            // Resize `physics_debug_buf` if it is too small.
            if (num_vertices * @sizeOf(zbt.DebugDrawer.Vertex) > vb_info.size) {
                gctx.destroyResource(demo.physics_debug_buf);
                demo.physics_debug_buf = gctx.createBuffer(.{
                    .usage = .{ .copy_dst = true, .vertex = true },
                    .size = (2 * num_vertices) * @sizeOf(zbt.DebugDrawer.Vertex),
                });
                vb_info = gctx.lookupResourceInfo(demo.physics_debug_buf) orelse break :pass;
            }

            gctx.queue.writeBuffer(vb_info.gpuobj.?, 0, zbt.DebugDrawer.Vertex, demo.physics.debug.lines.items);
            demo.physics.debug.lines.clearRetainingCapacity();

            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, depth_texv, null);
            defer zgpu.endReleasePass(pass);

            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, num_vertices * @sizeOf(zbt.DebugDrawer.Vertex));
            pass.setPipeline(physics_debug_pipe);
            {
                const mem = gctx.uniformsAllocate(zm.Mat, 1);
                mem.slice[0] = zm.transpose(cam_world_to_clip);
                pass.setBindGroup(0, uniform_bg, &.{mem.offset});
            }
            pass.draw(num_vertices, 1, 0, 0);
        }

        // Gui pass.
        {
            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.endReleasePass(pass);
            zgui.backend.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});

    if (gctx.present() == .swap_chain_resized) {
        // Release old depth texture.
        gctx.releaseResource(demo.depth_texv);
        gctx.destroyResource(demo.depth_tex);

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(gctx);
        demo.depth_tex = depth.tex;
        demo.depth_texv = depth.texv;
    }
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

const SceneSetupFunc = *const fn (
    world: zbt.World,
    common_shapes: std.ArrayList(zbt.Shape),
    scene_shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void;

const Scene = struct {
    name: []const u8,
    setup: SceneSetupFunc,
    has_gravity_ui: bool = true,
};

fn setupScene0(
    world: zbt.World,
    common_shapes: std.ArrayList(zbt.Shape),
    scene_shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    assert(entities.items.len == 0);

    const world_body = zbt.initBody(
        0.0,
        &zm.matToArr43(zm.identity()),
        common_shapes.items[mesh_index_world],
    );
    createEntity(world, world_body, .{ 0.25, 0.25, 0.25, 0.125 }, entities);
    {
        const body = zbt.initBody(
            25.0,
            &zm.matToArr43(zm.translation(0.0, 5.0, 5.0)),
            common_shapes.items[mesh_index_cube],
        );
        createEntity(world, body, .{ 0.8, 0.0, 0.0, 0.25 }, entities);
    }
    {
        const body = zbt.initBody(
            50.0,
            &zm.matToArr43(zm.translation(0.0, 5.0, 10.0)),
            common_shapes.items[mesh_index_compound0],
        );
        createEntity(world, body, .{ 0.8, 0.0, 0.9, 0.25 }, entities);
    }
    {
        const body = zbt.initBody(
            10.0,
            &zm.matToArr43(zm.translation(-5.0, 5.0, 10.0)),
            common_shapes.items[mesh_index_cylinder],
        );
        createEntity(world, body, .{ 1.0, 0.0, 0.0, 0.15 }, entities);
    }
    {
        const body = zbt.initBody(
            10.0,
            &zm.matToArr43(zm.translation(-5.0, 8.0, 10.0)),
            common_shapes.items[mesh_index_capsule],
        );
        createEntity(world, body, .{ 1.0, 0.5, 0.0, 0.5 }, entities);
    }
    {
        const body = zbt.initBody(
            40.0,
            &zm.matToArr43(zm.translation(5.0, 5.0, 10.0)),
            common_shapes.items[mesh_index_compound1],
        );
        createEntity(world, body, .{ 0.05, 0.1, 0.8, 0.5 }, entities);
    }
    {
        const box = zbt.initBoxShape(&.{ 0.5, 1.0, 2.0 });
        box.setUserIndex(0, @as(i32, @intCast(mesh_index_cube)));
        scene_shapes.append(box.asShape()) catch unreachable;

        const box_body = zbt.initBody(15.0, &zm.matToArr43(zm.translation(-5.0, 5.0, 5.0)), box.asShape());
        createEntity(world, box_body, .{ 1.0, 0.9, 0.0, 0.75 }, entities);
    }
    {
        const sphere = zbt.initSphereShape(1.5);
        sphere.setUserIndex(0, @as(i32, @intCast(mesh_index_sphere)));
        scene_shapes.append(sphere.asShape()) catch unreachable;

        const sphere_body = zbt.initBody(
            10.0,
            &zm.matToArr43(zm.translation(-5.0, 10.0, 5.0)),
            sphere.asShape(),
        );
        createEntity(world, sphere_body, .{ 0.0, 0.0, 1.0, 0.5 }, entities);
    }
    camera.* = .{
        .position = .{ 0.0, 7.0, -7.0 },
        .pitch = math.pi * 0.1,
        .yaw = 0.0,
    };
}

fn setupScene1(
    world: zbt.World,
    common_shapes: std.ArrayList(zbt.Shape),
    scene_shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    _ = scene_shapes;
    assert(entities.items.len == 0);

    const world_body = zbt.initBody(
        0.0,
        &zm.matToArr43(zm.identity()),
        common_shapes.items[mesh_index_world],
    );
    createEntity(world, world_body, .{ 0.25, 0.25, 0.25, 0.125 }, entities);

    const num_stacks = 32;
    const num_cubes_per_stack = 12;
    const radius: f32 = 15.0;

    var j: u32 = 0;
    while (j < num_stacks) : (j += 1) {
        const theta = @as(f32, @floatFromInt(j)) * math.tau / @as(f32, @floatFromInt(num_stacks));
        const x = radius * @cos(theta);
        const z = radius * @sin(theta);
        var i: u32 = 0;
        while (i < num_cubes_per_stack) : (i += 1) {
            const box_body = zbt.initBody(
                2.5,
                &zm.matToArr43(zm.translation(x, 2.2 + @as(f32, @floatFromInt(i)) * 2.0 + 0.05, z)),
                common_shapes.items[mesh_index_cube],
            );
            createEntity(
                world,
                box_body,
                if (j % 2 == 1) .{ 0.8, 0.0, 0.0, 0.25 } else .{ 1.0, 0.9, 0.0, 0.75 },
                entities,
            );
        }
    }
    camera.* = .{
        .position = .{ 30.0, 30.0, -30.0 },
        .pitch = math.pi * 0.15,
        .yaw = -math.pi * 0.25,
    };
}

fn setupScene2(
    world: zbt.World,
    common_shapes: std.ArrayList(zbt.Shape),
    scene_shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    _ = scene_shapes;
    assert(entities.items.len == 0);

    const world_body = zbt.initBody(
        0.0,
        &zm.matToArr43(zm.identity()),
        common_shapes.items[mesh_index_world],
    );
    createEntity(world, world_body, .{ 0.25, 0.25, 0.25, 0.125 }, entities);

    var level: u32 = 0;
    var y: f32 = 2.0;
    while (y <= 12.0) : (y += 2.0) {
        const bound: f32 = 12.0 - y;
        var z: f32 = -bound;
        level += 1;
        while (z <= bound) : (z += 2.0) {
            var x: f32 = -bound;
            while (x <= bound) : (x += 2.0) {
                const box_body = zbt.initBody(
                    0.5,
                    &zm.matToArr43(zm.translation(x, y, z)),
                    common_shapes.items[mesh_index_cube],
                );
                createEntity(
                    world,
                    box_body,
                    if (level % 2 == 1) .{ 0.5, 0.0, 0.0, 0.5 } else .{ 0.7, 0.6, 0.0, 0.75 },
                    entities,
                );
            }
        }
    }
    camera.* = .{
        .position = .{ 30.0, 30.0, -30.0 },
        .pitch = math.pi * 0.2,
        .yaw = -math.pi * 0.25,
    };
}

fn setupScene3(
    world: zbt.World,
    common_shapes: std.ArrayList(zbt.Shape),
    scene_shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
    camera: *Camera,
) void {
    assert(entities.items.len == 0);

    const world_body = zbt.initBody(
        0.0,
        &zm.matToArr43(zm.identity()),
        common_shapes.items[mesh_index_world],
    );
    createEntity(world, world_body, .{ 0.25, 0.25, 0.25, 0.125 }, entities);

    const box = zbt.initBoxShape(&.{ 0.5, 3.0, 1.5 });
    box.setUserIndex(0, @as(i32, @intCast(mesh_index_cube)));
    scene_shapes.append(box.asShape()) catch unreachable;

    const mass: f32 = 10.0;

    const heights = [_]u32{ 18, 8, 8, 6, 6 };
    const xoffsets = [_]f32{ 0.0, -7.0, 7.0, 0.0, 0.0 };
    const zoffsets = [_]f32{ 0.0, 0.0, 0.0, -4.0, 4.0 };

    var j: u32 = 0;
    while (j < heights.len) : (j += 1) {
        var i: u32 = 0;
        while (i < heights[j]) : (i += 1) {
            const y = 4.0 + @as(f32, @floatFromInt(i)) * 7.0;

            const left_body = zbt.initBody(
                mass,
                &zm.matToArr43(zm.translation(xoffsets[j] + -2.5, y, zoffsets[j] + 0.0)),
                box.asShape(),
            );
            const right_body = zbt.initBody(
                mass,
                &zm.matToArr43(zm.translation(xoffsets[j] + 2.5, y, zoffsets[j] + 0.0)),
                box.asShape(),
            );
            const top_body = zbt.initBody(
                mass,
                &zm.matToArr43(
                    zm.mul(zm.rotationZ(0.5 * math.pi), zm.translation(xoffsets[j], y + 3.5, zoffsets[j])),
                ),
                box.asShape(),
            );

            createEntity(world, left_body, .{ 1.0, 1.0, 0.0, 1.0 }, entities);
            createEntity(world, right_body, .{ 1.0, 1.0, 0.0, 1.0 }, entities);
            createEntity(world, top_body, .{ 0.0, 0.5, 0.0, 1.0 }, entities);

            left_body.forceActivationState(.wants_deactivation);
            right_body.forceActivationState(.wants_deactivation);
            top_body.forceActivationState(.wants_deactivation);
        }
    }

    const num_boxes: u32 = 35;
    const radius: f32 = 25.0;
    var i: u32 = 0;
    while (i < num_boxes) : (i += 1) {
        const theta = @as(f32, @floatFromInt(i)) * math.tau / @as(f32, @floatFromInt(num_boxes));
        const x = radius * @cos(theta);
        const z = radius * @sin(theta);

        const box_body = zbt.initBody(
            mass,
            &zm.matToArr43(
                zm.mul(zm.rotationY(-theta), zm.translation(x, 4.0, z)),
            ),
            box.asShape(),
        );
        createEntity(world, box_body, .{ 0.0, 0.1, 1.0, 1.0 }, entities);
    }

    camera.* = .{
        .position = .{ 30.0, 30.0, -30.0 },
        .pitch = math.pi * 0.2,
        .yaw = -math.pi * 0.25,
    };
}

fn cleanupScene(
    world: zbt.World,
    shapes: *std.ArrayList(zbt.Shape),
    entities: *std.ArrayList(Entity),
) void {
    var i = world.getNumBodies() - 1;
    while (i >= 0) : (i -= 1) {
        const body = world.getBody(i);
        world.removeBody(body);
        body.deinit();
    }
    for (shapes.items) |shape| shape.deinit();

    shapes.clearRetainingCapacity();
    entities.clearRetainingCapacity();

    world.setGravity(&.{ 0.0, -default_gravity, 0.0 });
}

fn createEntity(
    world: zbt.World,
    body: zbt.Body,
    basecolor_roughness: [4]f32,
    entities: *std.ArrayList(Entity),
) void {
    const shape = body.getShape();
    const mesh_index = @as(u32, @intCast(shape.getUserIndex(0)));
    const mesh_size = switch (shape.getType()) {
        .box => mesh_size: {
            var half_extents: [3]f32 = undefined;
            shape.as(.box).getHalfExtentsWithMargin(&half_extents);
            break :mesh_size half_extents;
        },
        .sphere => mesh_size: {
            const r = shape.as(.sphere).getRadius();
            break :mesh_size [3]f32{ r, r, r };
        },
        .cylinder => mesh_size: {
            var half_extents: [3]f32 = undefined;
            shape.as(.cylinder).getHalfExtentsWithMargin(&half_extents);
            break :mesh_size half_extents;
        },
        else => mesh_size: {
            break :mesh_size [3]f32{ 1.0, 1.0, 1.0 }; // No scaling support for this mesh.
        },
    };
    if (shape.getType() != .trimesh) {
        body.setCcdSweptSphereRadius(ccd_swept_sphere_radius);
        body.setCcdMotionThreshold(ccd_motion_threshold);
    }
    const entity_index = @as(i32, @intCast(entities.items.len));
    entities.append(.{
        .body = body,
        .basecolor_roughness = basecolor_roughness,
        .size = mesh_size,
        .mesh_index = mesh_index,
    }) catch unreachable;
    body.setUserIndex(0, entity_index);
    body.setDamping(default_linear_damping, default_angular_damping);
    body.setActivationState(.deactivation_disabled);
    world.addBody(body);
}

fn appendMesh(
    mesh: zmesh.Shape,
    all_meshes: *std.ArrayList(Mesh),
    all_indices: *std.ArrayList(u32),
    all_positions: *std.ArrayList([3]f32),
    all_normals: *std.ArrayList([3]f32),
) !u32 {
    const mesh_index = @as(u32, @intCast(all_meshes.items.len));
    try all_meshes.append(.{
        .index_offset = @as(u32, @intCast(all_indices.items.len)),
        .vertex_offset = @as(u32, @intCast(all_positions.items.len)),
        .num_indices = @as(u32, @intCast(mesh.indices.len)),
        .num_vertices = @as(u32, @intCast(mesh.positions.len)),
    });
    try all_indices.appendSlice(mesh.indices);
    try all_positions.appendSlice(mesh.positions);
    try all_normals.appendSlice(mesh.normals.?);
    return mesh_index;
}

fn initMeshes(
    arena: std.mem.Allocator,
    shapes: *std.ArrayList(zbt.Shape),
    all_meshes: *std.ArrayList(Mesh),
    all_indices: *std.ArrayList(u32),
    all_positions: *std.ArrayList([3]f32),
    all_normals: *std.ArrayList([3]f32),
) !void {
    assert(shapes.items.len == 0);
    try shapes.resize(mesh_count);

    // Cube mesh.
    {
        var mesh = zmesh.Shape.initCube();
        defer mesh.deinit();
        mesh.translate(-0.5, -0.5, -0.5);
        mesh.scale(2.0, 2.0, 2.0);
        mesh.unweld();
        mesh.computeNormals();

        const mesh_index = try appendMesh(mesh, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_cube);

        shapes.items[mesh_index] = zbt.initBoxShape(&.{ 1.0, 1.0, 1.0 }).asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index)));
    }

    // Parametric sphere mesh.
    {
        var mesh = zmesh.Shape.initParametricSphere(8, 8);
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        const mesh_index = try appendMesh(mesh, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_sphere);

        shapes.items[mesh_index] = zbt.initSphereShape(1.0).asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index)));
    }

    // Cylinder mesh.
    {
        var cylinder = zmesh.Shape.initCylinder(10, 6);
        defer cylinder.deinit();
        cylinder.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        cylinder.scale(1.0, 2.0, 1.0);
        cylinder.translate(0.0, 1.0, 0.0);

        // Top cap.
        var top = zmesh.Shape.initParametricDisk(10, 2);
        defer top.deinit();
        top.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        top.translate(0.0, 1.0, 0.0);

        // Bottom cap.
        var bottom = top.clone();
        defer bottom.deinit();
        bottom.translate(0.0, -2.0, 0.0);

        cylinder.merge(top);
        cylinder.merge(bottom);
        cylinder.unweld();
        cylinder.computeNormals();

        const mesh_index = try appendMesh(cylinder, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_cylinder);

        shapes.items[mesh_index] = zbt.initCylinderShape(&.{ 1.0, 1.0, 1.0 }, .y).asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index)));
    }

    // Capsule mesh.
    {
        var cylinder = zmesh.Shape.initCylinder(12, 6);
        defer cylinder.deinit();
        cylinder.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        cylinder.translate(0.0, 0.5, 0.0);

        // Top hemisphere.
        var top = zmesh.Shape.initHemisphere(12, 6);
        defer top.deinit();
        top.translate(0.0, 0.5, 0.0);

        // Bottom hemisphere.
        var bottom = top.clone();
        defer bottom.deinit();
        bottom.rotate(math.pi, 1.0, 0.0, 0.0);

        cylinder.merge(top);
        cylinder.merge(bottom);
        cylinder.unweld();
        cylinder.computeNormals();

        const mesh_index = try appendMesh(cylinder, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_capsule);

        shapes.items[mesh_index] = zbt.initCapsuleShape(1.0, 1.0, .y).asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index)));
    }

    // Compound0 mesh.
    {
        var cube0 = zmesh.Shape.initCube();
        defer cube0.deinit();
        cube0.translate(-0.5, -0.5, -0.5);
        cube0.scale(2.0, 2.0, 2.0);
        cube0.unweld();
        cube0.computeNormals();

        var cube1 = cube0.clone();
        defer cube1.deinit();
        var cube2 = cube0.clone();
        defer cube2.deinit();
        var cube3 = cube0.clone();
        defer cube3.deinit();

        cube0.translate(2.0, 0.0, 0.0);
        cube1.translate(-2.0, 0.0, 0.0);
        cube2.translate(0.0, 2.0, 0.0);
        cube3.translate(0.0, -2.0, 0.0);

        cube0.merge(cube1);
        cube2.merge(cube3);
        cube0.merge(cube2);

        const mesh_index = try appendMesh(cube0, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_compound0);

        const compound = zbt.initCompoundShape(.{});
        compound.addChild(&zm.matToArr43(zm.translation(2.0, 0.0, 0.0)), shapes.items[mesh_index_cube]);
        compound.addChild(&zm.matToArr43(zm.translation(-2.0, 0.0, 0.0)), shapes.items[mesh_index_cube]);
        compound.addChild(&zm.matToArr43(zm.translation(0.0, 2.0, 0.0)), shapes.items[mesh_index_cube]);
        compound.addChild(&zm.matToArr43(zm.translation(0.0, -2.0, 0.0)), shapes.items[mesh_index_cube]);
        shapes.items[mesh_index] = compound.asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index_compound0)));
    }

    // Compound1 mesh.
    {
        var cube = zmesh.Shape.initCube();
        defer cube.deinit();
        cube.translate(-0.5, -0.5, -0.5);
        cube.scale(2.0, 2.0, 2.0);
        cube.unweld();
        cube.computeNormals();

        var sphere = zmesh.Shape.initParametricSphere(10, 10);
        defer sphere.deinit();
        sphere.unweld();
        sphere.computeNormals();
        sphere.translate(0.0, 4.0, 0.0);

        var cylinder = zmesh.Shape.initCylinder(10, 6);
        defer cylinder.deinit();
        cylinder.scale(0.25, 0.25, 4.0);
        cylinder.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        cylinder.translate(0.0, 3.5, 0.0);
        cylinder.unweld();
        cylinder.computeNormals();

        cube.merge(cylinder);
        cube.merge(sphere);

        const mesh_index = try appendMesh(cube, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_index_compound1);

        const cylinder_shape = zbt.initCylinderShape(&.{ 0.25, 2.0, 0.25 }, .y).asShape();
        try shapes.append(cylinder_shape);

        const compound = zbt.initCompoundShape(.{});
        compound.addChild(&zm.matToArr43(zm.translation(0.0, 0.0, 0.0)), shapes.items[mesh_index_cube]);
        compound.addChild(&zm.matToArr43(zm.translation(0.0, 4.0, 0.0)), shapes.items[mesh_index_sphere]);
        compound.addChild(&zm.matToArr43(zm.translation(0.0, 2.5, 0.0)), cylinder_shape);
        shapes.items[mesh_index] = compound.asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index_compound1)));
    }

    // World mesh.
    {
        const mesh_index = @as(u32, @intCast(all_meshes.items.len));
        const index_offset = @as(u32, @intCast(all_indices.items.len));
        const vertex_offset = @as(u32, @intCast(all_positions.items.len));

        var indices = std.ArrayList(u32).init(arena);
        defer indices.deinit();
        var positions = std.ArrayList([3]f32).init(arena);
        defer positions.deinit();
        var normals = std.ArrayList([3]f32).init(arena);
        defer normals.deinit();

        const data = try zmesh.io.parseAndLoadFile(content_dir ++ "world.gltf");
        defer zmesh.io.freeData(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, null, null);

        // "Unweld" mesh, this creates un-optimized mesh with duplicated vertices.
        // We need it for wireframes and facet look.
        for (indices.items, 0..) |ind, i| {
            try all_positions.append(positions.items[ind]);
            try all_normals.append(normals.items[ind]);
            try all_indices.append(@as(u32, @intCast(i)));
        }

        try all_meshes.append(.{
            .index_offset = index_offset,
            .vertex_offset = vertex_offset,
            .num_indices = @as(u32, @intCast(all_indices.items.len)) - index_offset,
            .num_vertices = @as(u32, @intCast(all_positions.items.len)) - vertex_offset,
        });

        const trimesh = zbt.initTriangleMeshShape();
        trimesh.addIndexVertexArray(
            @as(u32, @intCast(indices.items.len / 3)),
            indices.items.ptr,
            @sizeOf([3]u32),
            @as(u32, @intCast(positions.items.len)),
            positions.items.ptr,
            @sizeOf([3]f32),
        );
        trimesh.finish();
        shapes.items[mesh_index] = trimesh.asShape();
        shapes.items[mesh_index].setUserIndex(0, @as(i32, @intCast(mesh_index)));
    }
}

fn objectPicking(demo: *DemoState, want_capture_mouse: bool) void {
    const window = demo.window;

    const mouse_button_is_down = window.getMouseButton(.left) == .press and !want_capture_mouse;

    const ray_from = zm.loadArr3(demo.camera.position);
    const ray_to = ray_to: {
        const cursor_pos = window.getCursorPos();
        const mousex = @as(f32, @floatCast(cursor_pos[0]));
        const mousey = @as(f32, @floatCast(cursor_pos[1]));

        const far_plane = zm.f32x4s(10_000.0);
        const tanfov = zm.f32x4s(@tan(0.5 * camera_fovy));
        const winsize = window.getSize();
        const width = @as(f32, @floatFromInt(winsize[0]));
        const height = @as(f32, @floatFromInt(winsize[1]));
        const aspect = zm.f32x4s(width / height);

        const ray_forward = zm.loadArr3(demo.camera.forward) * far_plane;

        const hor = zm.normalize3(zm.cross3(zm.f32x4(0, 1, 0, 0), ray_forward)) *
            zm.f32x4s(2.0) * far_plane * tanfov * aspect;
        const vert = zm.normalize3(zm.cross3(hor, ray_forward)) *
            zm.f32x4s(2.0) * far_plane * tanfov;

        const ray_to_center = ray_from + ray_forward;

        const dhor = zm.f32x4s(1.0 / width) * hor;
        const dvert = zm.f32x4s(1.0 / height) * vert;

        var ray_to = ray_to_center + zm.f32x4s(-0.5) * hor + zm.f32x4s(-0.5) * vert;
        ray_to += dhor * zm.f32x4s(mousex);
        ray_to += dvert * zm.f32x4s(mousey);
        break :ray_to ray_to;
    };

    if (!demo.pick.p2p.isCreated() and mouse_button_is_down) {
        var result: zbt.RayCastResult = undefined;
        const is_hit = demo.physics.world.rayTestClosest(
            zm.arr3Ptr(&ray_from),
            zm.arr3Ptr(&ray_to),
            .{ .default = true },
            zbt.CollisionFilter.all,
            .{ .use_gjk_convex_test = true },
            &result,
        );

        if (is_hit) if (result.body) |body| if (!body.isStaticOrKinematic()) {
            demo.pick.body = body;

            demo.pick.saved_linear_damping = body.getLinearDamping();
            demo.pick.saved_angular_damping = body.getAngularDamping();
            demo.pick.saved_activation_state = body.getActivationState();

            body.setDamping(0.4, 0.4);
            body.forceActivationState(.deactivation_disabled);

            const pivot_a = zm.mul(
                zm.loadArr3w(result.hit_point_world, 1.0),
                loadInvCenterOfMassTransform(body),
            );
            demo.pick.p2p.create1(body, zm.arr3Ptr(&pivot_a));
            demo.pick.p2p.setImpulseClamp(30.0);
            demo.pick.p2p.setDebugDrawSize(0.15);

            demo.physics.world.addConstraint(demo.pick.p2p.asConstraint(), true);

            demo.pick.distance = zm.length3(zm.loadArr3(result.hit_point_world) - ray_from)[0];
        };
    } else if (demo.pick.p2p.isCreated() and mouse_button_is_down) {
        const to = ray_from + zm.normalize3(ray_to) * zm.f32x4s(demo.pick.distance);
        demo.pick.p2p.setPivotB(zm.arr3Ptr(&to));

        const trans_a = loadCenterOfMassTransform(demo.pick.p2p.getBodyA());
        const trans_b = loadCenterOfMassTransform(demo.pick.p2p.getBodyB());

        const pivot_a = loadPivotA(demo.pick.p2p);
        const pivot_b = loadPivotB(demo.pick.p2p);

        const position_a = zm.mul(pivot_a, trans_a);
        const position_b = zm.mul(pivot_b, trans_b);

        demo.physics.world.debugDrawLine2(
            zm.arr3Ptr(&position_a),
            zm.arr3Ptr(&position_b),
            &.{ 1.0, 1.0, 0.0 },
            &.{ 1.0, 0.0, 0.0 },
        );
        demo.physics.world.debugDrawSphere(zm.arr3Ptr(&position_a), 0.05, &.{ 0.0, 1.0, 0.0 });
    }

    if (!mouse_button_is_down and demo.pick.p2p.isCreated()) {
        demo.physics.world.removeConstraint(demo.pick.p2p.asConstraint());
        demo.pick.p2p.destroy();
        demo.pick.body.?.setDamping(demo.pick.saved_linear_damping, demo.pick.saved_angular_damping);
        demo.pick.body.?.setActivationState(demo.pick.saved_activation_state);
        demo.pick.body = null;
    }
}

fn loadCenterOfMassTransform(body: zbt.Body) zm.Mat {
    var transform: [12]f32 = undefined;
    body.getCenterOfMassTransform(&transform);
    return zm.loadMat43(transform[0..]);
}

fn loadInvCenterOfMassTransform(body: zbt.Body) zm.Mat {
    var transform: [12]f32 = undefined;
    body.getInvCenterOfMassTransform(&transform);
    return zm.loadMat43(transform[0..]);
}

fn loadPivotA(p2p: zbt.Point2PointConstraint) zm.Vec {
    var pivot: [3]f32 = undefined;
    p2p.getPivotA(&pivot);
    return zm.loadArr3w(pivot, 1.0);
}

fn loadPivotB(p2p: zbt.Point2PointConstraint) zm.Vec {
    var pivot: [3]f32 = undefined;
    p2p.getPivotB(&pivot);
    return zm.loadArr3w(pivot, 1.0);
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

    // Init zbullet library.
    zbt.init(allocator);
    defer zbt.deinit();

    const demo = try create(allocator, window);
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

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        update(demo);
        draw(demo);
    }
}
