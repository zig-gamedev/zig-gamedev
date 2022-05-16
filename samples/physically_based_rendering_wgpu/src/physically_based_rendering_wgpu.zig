const std = @import("std");
const math = std.math;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const gpu = zgpu.gpu;
const c = zgpu.cimgui;
const zm = @import("zmath");
const zmesh = @import("zmesh");
const wgsl = @import("physically_based_rendering_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: physically based rendering (wgpu)";

const Vertex = extern struct {
    position: [3]f32,
    normal: [3]f32,
    texcoord: [2]f32,
    tangent: [4]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,
};

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    mesh_pipeline: zgpu.RenderPipelineHandle = .{},

    vertex_buffer: zgpu.BufferHandle,
    index_buffer: zgpu.BufferHandle,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    meshes: std.ArrayList(Mesh),

    camera: struct {
        position: [3]f32 = .{ 0.0, 4.0, -4.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.15 * math.pi,
        yaw: f32 = 0.0,
    } = .{},
    mouse: struct {
        cursor: glfw.Window.CursorPos = .{ .xpos = 0.0, .ypos = 0.0 },
    } = .{},
};

fn loadAllMeshes(
    arena: std.mem.Allocator,
    out_meshes: *std.ArrayList(Mesh),
    out_vertices: *std.ArrayList(Vertex),
    out_indices: *std.ArrayList(u32),
) !void {
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList([3]f32).init(arena);
    var normals = std.ArrayList([3]f32).init(arena);
    var texcoords = std.ArrayList([2]f32).init(arena);
    var tangents = std.ArrayList([4]f32).init(arena);

    {
        const pre_indices_len = indices.items.len;
        const pre_positions_len = positions.items.len;

        const data = try zmesh.io.parseAndLoadFile(content_dir ++ "cube.gltf");
        defer zmesh.io.cgltf.free(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, &texcoords, &tangents);

        try out_meshes.append(.{
            .index_offset = @intCast(u32, pre_indices_len),
            .vertex_offset = @intCast(i32, pre_positions_len),
            .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
            .num_vertices = @intCast(u32, positions.items.len - pre_positions_len),
        });
    }
    {
        const pre_indices_len = indices.items.len;
        const pre_positions_len = positions.items.len;

        const data = try zmesh.io.parseAndLoadFile(content_dir ++ "SciFiHelmet/SciFiHelmet.gltf");
        defer zmesh.io.cgltf.free(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, &texcoords, &tangents);

        try out_meshes.append(.{
            .index_offset = @intCast(u32, pre_indices_len),
            .vertex_offset = @intCast(i32, pre_positions_len),
            .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
            .num_vertices = @intCast(u32, positions.items.len - pre_positions_len),
        });
    }

    try out_indices.ensureTotalCapacity(indices.items.len);
    for (indices.items) |mesh_index| {
        out_indices.appendAssumeCapacity(mesh_index);
    }

    try out_vertices.ensureTotalCapacity(positions.items.len);
    for (positions.items) |_, index| {
        out_vertices.appendAssumeCapacity(.{
            .position = positions.items[index],
            .normal = normals.items[index],
            .texcoord = texcoords.items[index],
            .tangent = tangents.items[index],
        });
    }
}

fn init(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const frame_uniforms_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.destroyResource(frame_uniforms_bgl);

    const mesh_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.destroyResource(mesh_bgl);

    zmesh.init(arena);
    defer zmesh.deinit();

    var meshes = std.ArrayList(Mesh).init(allocator);
    var vertices = std.ArrayList(Vertex).init(arena);
    var indices = std.ArrayList(u32).init(arena);
    try loadAllMeshes(arena, &meshes, &vertices, &indices);

    const total_num_vertices = @intCast(u32, vertices.items.len);
    const total_num_indices = @intCast(u32, indices.items.len);

    // Create a vertex buffer.
    const vertex_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, vertices.items);

    // Create an index buffer.
    const index_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(u32),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u32, indices.items);

    // Create a depth texture and its 'view'.
    const depth = createDepthTexture(gctx);

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
        .meshes = meshes,
    };

    createMeshPipeline(allocator, gctx, &.{ frame_uniforms_bgl, mesh_bgl }, &demo.mesh_pipeline);

    return demo;
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.meshes.deinit();
    demo.gctx.deinit(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    if (c.igBegin("Demo Settings", null, c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize)) {
        c.igBulletText(
            "Average :  %.3f ms/frame (%.1f fps)",
            demo.gctx.stats.average_cpu_time,
            demo.gctx.stats.fps,
        );
        c.igBulletText("Right Mouse Button + drag :  rotate camera");
        c.igBulletText("W, A, S, D :  move camera");
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
        const speed = zm.f32x4s(2.0);
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.store(demo.camera.forward[0..], forward, 3);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var cpos = zm.load(demo.camera.position[0..], zm.Vec, 3);

        if (window.getKey(.w) == .press) {
            cpos += forward;
        } else if (window.getKey(.s) == .press) {
            cpos -= forward;
        }
        if (window.getKey(.d) == .press) {
            cpos += right;
        } else if (window.getKey(.a) == .press) {
            cpos -= right;
        }

        zm.store(demo.camera.position[0..], cpos, 3);
    }
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    //const fb_width = gctx.swapchain_descriptor.width;
    //const fb_height = gctx.swapchain_descriptor.height;

    const back_buffer_view = gctx.swapchain.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Gui pass.
        {
            const color_attachment = gpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .load_op = .load,
                .store_op = .store,
            };
            const render_pass_info = gpu.RenderPassEncoder.Descriptor{
                .color_attachments = &.{color_attachment},
            };
            const pass = encoder.beginRenderPass(&render_pass_info);
            defer {
                pass.end();
                pass.release();
            }
            zgpu.gui.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});

    if (gctx.present() == .swap_chain_resized) {
        // Release old depth texture.
        gctx.destroyResource(demo.depth_texture_view);
        gctx.destroyResource(demo.depth_texture);

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(gctx);
        demo.depth_texture = depth.texture;
        demo.depth_texture_view = depth.view;
    }
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    texture: zgpu.TextureHandle,
    view: zgpu.TextureViewHandle,
} {
    const texture = gctx.createTexture(.{
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
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
}

fn createMeshPipeline(
    allocator: std.mem.Allocator,
    gctx: *zgpu.GraphicsContext,
    bind_group_layouts: []const zgpu.BindGroupLayoutHandle,
    out_pipeline: *zgpu.RenderPipelineHandle,
) void {
    const pipeline_layout = gctx.createPipelineLayout(bind_group_layouts);
    defer gctx.destroyResource(pipeline_layout);

    const module_vs = gctx.device.createShaderModule(&.{
        .label = "mesh_vs",
        .code = .{ .wgsl = wgsl.mesh_vs },
    });
    defer module_vs.release();

    const module_fs = gctx.device.createShaderModule(&.{
        .label = "mesh_fs",
        .code = .{ .wgsl = wgsl.mesh_fs },
    });
    defer module_fs.release();

    const color_target = gpu.ColorTargetState{
        .format = zgpu.GraphicsContext.swapchain_format,
        .blend = &.{ .color = .{}, .alpha = .{} },
    };

    const vertex_attributes = [_]gpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
        .{ .format = .float32x2, .offset = @offsetOf(Vertex, "texcoord"), .shader_location = 2 },
        .{ .format = .float32x4, .offset = @offsetOf(Vertex, "tangent"), .shader_location = 3 },
    };
    const vertex_buffer_layout = gpu.VertexBufferLayout{
        .array_stride = @sizeOf(Vertex),
        .attribute_count = vertex_attributes.len,
        .attributes = &vertex_attributes,
    };

    // Create a render pipeline.
    const pipeline_descriptor = gpu.RenderPipeline.Descriptor{
        .vertex = gpu.VertexState{
            .module = module_vs,
            .entry_point = "main",
            .buffers = &.{vertex_buffer_layout},
        },
        .primitive = gpu.PrimitiveState{
            .front_face = .cw,
            .cull_mode = .back,
            .topology = .triangle_list,
        },
        .fragment = &gpu.FragmentState{
            .module = module_fs,
            .entry_point = "main",
            .targets = &.{color_target},
        },
    };
    gctx.createRenderPipelineAsync(allocator, pipeline_layout, pipeline_descriptor, out_pipeline);
}

pub fn main() !void {
    zgpu.checkContent(content_dir) catch {
        // In case of error zgpu.checkContent() will print error message.
        return;
    };

    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(1280, 960, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 400, .height = 400 }, .{ .width = null, .height = null });

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

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
