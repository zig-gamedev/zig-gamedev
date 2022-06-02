const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const glfw = @import("glfw");
const gpu = @import("gpu");
const zgpu = @import("zgpu");
const c = zgpu.cimgui;
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
    body: *const zbt.Body,
    basecolor_roughness: [4]f32,
    //size: [3]f32,
    mesh_index: u32,
};

const mesh_cube: u32 = 0;
const mesh_world: u32 = 1;
const mesh_sphere: u32 = 2;

const safe_uniform_size = 256;

const DemoState = struct {
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

    physics: struct {
        world: *const zbt.World,
        shapes: std.ArrayList(*const zbt.Shape),
        debug: *zbt.DebugDrawer,
    },
    camera: struct {
        position: [3]f32 = .{ 0.0, 3.0, 0.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 0.0 },
        pitch: f32 = math.pi * 0.05,
        yaw: f32 = 0.0,
    } = .{},
    mouse: struct {
        cursor: glfw.Window.CursorPos = .{ .xpos = 0.0, .ypos = 0.0 },
    } = .{},
};

fn init(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    //
    // Create meshes.
    //
    zmesh.init(arena);
    defer zmesh.deinit();

    var meshes = std.ArrayList(Mesh).init(allocator);
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList([3]f32).init(arena);
    var normals = std.ArrayList([3]f32).init(arena);
    var physics_shapes = std.ArrayList(*const zbt.Shape).init(allocator);
    try initMeshes(arena, &meshes, &indices, &positions, &normals, &physics_shapes);

    const total_num_vertices = @intCast(u32, positions.items.len);
    const total_num_indices = @intCast(u32, indices.items.len);

    // Create a vertex buffer.
    const vertex_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    {
        var vertex_data = std.ArrayList(Vertex).init(arena);
        defer vertex_data.deinit();
        vertex_data.resize(total_num_vertices) catch unreachable;

        for (positions.items) |_, i| {
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
    // Create bind groups
    //
    const uniform_bg = gctx.createBindGroup(uniform_bgl, &[_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = safe_uniform_size,
    }});

    //
    // Init physics
    //
    const physics_world = zbt.World.init(.{});

    var physics_debug = allocator.create(zbt.DebugDrawer) catch unreachable;
    physics_debug.* = zbt.DebugDrawer.init(allocator);

    physics_world.debugSetDrawer(&physics_debug.getDebugDraw());
    physics_world.debugSetMode(zbt.dbgmode_disabled);

    var entities = std.ArrayList(Entity).init(allocator);

    {
        const box_body = zbt.Body.init(
            1.0, // mass
            &zm.mat43ToArray(zm.translation(0.0, 5.0, 5.0)),
            physics_shapes.items[mesh_cube],
        );
        createEntity(physics_world, box_body, [4]f32{ 0.8, 0.0, 0.0, 0.25 }, &entities);

        const world_body = zbt.Body.init(
            0.0, // static body
            &zm.mat43ToArray(zm.identity()),
            physics_shapes.items[mesh_world],
        );
        createEntity(physics_world, world_body, [4]f32{ 0.25, 0.25, 0.25, 0.125 }, &entities);
    }

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .vertex_buf = vertex_buf,
        .index_buf = index_buf,
        .physics_debug_buf = physics_debug_buf,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .uniform_bg = uniform_bg,
        .meshes = meshes,
        .entities = entities,
        .physics = .{
            .world = physics_world,
            .shapes = physics_shapes,
            .debug = physics_debug,
        },
    };

    //
    // Create pipelines.
    //
    const common_depth_state = gpu.DepthStencilState{
        .format = .depth32_float,
        .depth_write_enabled = true,
        .depth_compare = .less,
    };

    const pos_norm_attribs = [_]gpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
    };
    zgpu.util.createRenderPipelineSimple(
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

    const pos_color_attribs = [_]gpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .uint32, .offset = @offsetOf(zbt.DebugDrawer.Vertex, "color"), .shader_location = 1 },
    };
    zgpu.util.createRenderPipelineSimple(
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

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    {
        var i = demo.physics.world.getNumBodies() - 1;
        while (i >= 0) : (i -= 1) {
            const body = demo.physics.world.getBody(i);
            demo.physics.world.removeBody(body);
            body.deinit();
        }
    }
    for (demo.physics.shapes.items) |shape|
        shape.deinit();
    demo.physics.shapes.deinit();
    demo.physics.debug.deinit();
    allocator.destroy(demo.physics.debug);
    demo.physics.world.deinit();
    demo.entities.deinit();
    demo.meshes.deinit();
    demo.gctx.deinit(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    const dt = demo.gctx.stats.delta_time;
    _ = demo.physics.world.stepSimulation(dt, .{});

    if (c.igBegin("Demo Settings", null, c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize)) {
        c.igBulletText(
            "Average :  %.3f ms/frame (%.1f fps)",
            demo.gctx.stats.average_cpu_time,
            demo.gctx.stats.fps,
        );
        c.igBulletText("Right Mouse Button + drag :  rotate camera");
        c.igBulletText("W, A, S, D :  move camera");
        c.igBulletText("SPACE :  to shoot");
    }
    c.igEnd();

    const window = demo.gctx.window;

    // Handle camera rotation with mouse.
    {
        const cursor = window.getCursorPos() catch unreachable;
        const delta_x = @floatCast(f32, cursor.xpos - demo.mouse.cursor.xpos);
        const delta_y = @floatCast(f32, cursor.ypos - demo.mouse.cursor.ypos);
        demo.mouse.cursor.xpos = cursor.xpos;
        demo.mouse.cursor.ypos = cursor.ypos;

        if (window.getMouseButton(.right) == .press) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = math.min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = math.max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = zm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed = zm.f32x4s(5.0);
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.store3(&demo.camera.forward, forward);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var cam_pos = zm.load3(demo.camera.position);

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

        zm.store3(&demo.camera.position, cam_pos);
    }

    // Shooting.
    {
        demo.keyboard_delay += dt;
        if (window.getKey(.space) == .press and demo.keyboard_delay >= 0.5) {
            demo.keyboard_delay = 0.0;

            const transform = zm.translationV(zm.load3(demo.camera.position));
            const impulse = zm.f32x4s(60.0) * zm.load3(demo.camera.forward);

            const body = zbt.Body.init(
                1.0,
                &zm.mat43ToArray(transform),
                demo.physics.shapes.items[mesh_sphere],
            );
            body.applyCentralImpulse(&zm.vec3ToArray(impulse));

            createEntity(demo.physics.world, body, [4]f32{ 0.0, 0.8, 0.0, 0.2 }, &demo.entities);
        }
    }
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const cam_world_to_view = zm.lookToLh(
        zm.load3(demo.camera.position),
        zm.load3(demo.camera.forward),
        zm.f32x4(0.0, 1.0, 0.0, 0.0),
    );
    const cam_view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @intToFloat(f32, fb_width) / @intToFloat(f32, fb_height),
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

            const pass = zgpu.util.beginRenderPassSimple(encoder, .clear, swapchain_texv, null, depth_texv, 1.0);
            defer zgpu.util.endRelease(pass);

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
                const entity = &demo.entities.items[@intCast(usize, body.getUserIndex(0))];

                // Get transform matrix from the physics simulator.
                const object_to_world = blk: {
                    var transform: [12]f32 = undefined;
                    body.getGraphicsWorldTransform(&transform);
                    break :blk zm.loadMat43(transform[0..]);
                };

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
                    @intCast(i32, demo.meshes.items[entity.mesh_index].vertex_offset),
                    0,
                );
            }
        }

        // Physics debug pass.
        pass: {
            demo.physics.world.debugDrawAll();
            const num_vertices = @intCast(u32, demo.physics.debug.lines.items.len);
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

            const pass = zgpu.util.beginRenderPassSimple(encoder, .load, swapchain_texv, null, depth_texv, null);
            defer zgpu.util.endRelease(pass);

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
            const pass = zgpu.util.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.util.endRelease(pass);
            zgpu.gui.draw(pass);
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
        .dimension = .dimension_2d,
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

fn createEntity(
    world: *const zbt.World,
    body: *const zbt.Body,
    basecolor_roughness: [4]f32,
    entities: *std.ArrayList(Entity),
) void {
    const shape = body.getShape();
    const shape_type = shape.getType();
    const mesh_index = switch (shape_type) {
        .box => mesh_cube,
        .sphere => mesh_sphere,
        .trimesh => mesh_world,
        else => unreachable,
    };
    const entity_index = @intCast(i32, entities.items.len);
    entities.append(.{
        .body = body,
        .basecolor_roughness = basecolor_roughness,
        .mesh_index = mesh_index,
    }) catch unreachable;
    body.setUserIndex(0, entity_index);
    body.setCcdSweptSphereRadius(0.5);
    body.setCcdMotionThreshold(1e-7);
    world.addBody(body);
}

fn appendMesh(
    mesh: zmesh.Shape,
    all_meshes: *std.ArrayList(Mesh),
    all_indices: *std.ArrayList(u32),
    all_positions: *std.ArrayList([3]f32),
    all_normals: *std.ArrayList([3]f32),
) u32 {
    const mesh_index = @intCast(u32, all_meshes.items.len);
    all_meshes.append(.{
        .index_offset = @intCast(u32, all_indices.items.len),
        .vertex_offset = @intCast(u32, all_positions.items.len),
        .num_indices = @intCast(u32, mesh.indices.len),
        .num_vertices = @intCast(u32, mesh.positions.len),
    }) catch unreachable;

    all_indices.appendSlice(mesh.indices) catch unreachable;
    all_positions.appendSlice(mesh.positions) catch unreachable;
    all_normals.appendSlice(mesh.normals.?) catch unreachable;
    return mesh_index;
}

fn initMeshes(
    arena: std.mem.Allocator,
    all_meshes: *std.ArrayList(Mesh),
    all_indices: *std.ArrayList(u32),
    all_positions: *std.ArrayList([3]f32),
    all_normals: *std.ArrayList([3]f32),
    physics_shapes: *std.ArrayList(*const zbt.Shape),
) !void {
    // Cube mesh.
    {
        var mesh = zmesh.Shape.initCube();
        defer mesh.deinit();
        mesh.translate(-0.5, -0.5, -0.5);
        mesh.unweld();
        mesh.computeNormals();

        const mesh_index = appendMesh(mesh, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_cube);

        const shape = zbt.BoxShape.init(&.{ 0.5, 0.5, 0.5 });
        try physics_shapes.append(shape.asShape());
    }

    // World mesh.
    {
        const mesh_index = @intCast(u32, all_meshes.items.len);
        const index_offset = @intCast(u32, all_indices.items.len);
        const vertex_offset = @intCast(u32, all_positions.items.len);

        var indices = std.ArrayList(u32).init(arena);
        defer indices.deinit();
        var positions = std.ArrayList([3]f32).init(arena);
        defer positions.deinit();
        var normals = std.ArrayList([3]f32).init(arena);
        defer normals.deinit();

        const data = try zmesh.io.parseAndLoadFile(content_dir ++ "world.gltf");
        defer zmesh.io.cgltf.free(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, null, null);

        for (indices.items) |ind, i| {
            all_positions.append(positions.items[ind]) catch unreachable;
            all_normals.append(normals.items[ind]) catch unreachable;
            all_indices.append(@intCast(u32, i)) catch unreachable;
        }

        try all_meshes.append(.{
            .index_offset = index_offset,
            .vertex_offset = vertex_offset,
            .num_indices = @intCast(u32, all_indices.items.len) - index_offset,
            .num_vertices = @intCast(u32, all_positions.items.len) - vertex_offset,
        });
        assert(mesh_index == mesh_world);

        const world_shape = zbt.TriangleMeshShape.init();
        try physics_shapes.append(world_shape.asShape());

        world_shape.addIndexVertexArray(
            @intCast(u32, indices.items.len / 3),
            indices.items.ptr,
            @sizeOf([3]u32),
            @intCast(u32, positions.items.len),
            positions.items.ptr,
            @sizeOf([3]f32),
        );
        world_shape.finish();
    }

    // Parametric sphere.
    {
        var mesh = zmesh.Shape.initParametricSphere(8, 8);
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        const mesh_index = appendMesh(mesh, all_meshes, all_indices, all_positions, all_normals);
        assert(mesh_index == mesh_sphere);

        const shape = zbt.SphereShape.init(1.0);
        try physics_shapes.append(shape.asShape());
    }
}

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    zgpu.checkSystem(content_dir) catch {
        // In case of error zgpu.checkSystem() will print error message.
        return;
    };

    const window = try glfw.Window.create(1400, 1000, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 400, .height = 400 }, .{ .width = null, .height = null });

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    // Init zbullet library.
    zbt.init(allocator);
    defer zbt.deinit();

    const demo = try init(allocator, window);
    defer deinit(allocator, demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir, "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        update(demo);
        draw(demo);
    }
}
