const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zmesh = @import("zmesh");
const wgsl = @import("physically_based_rendering_wgsl.zig");
const zstbi = @import("zstbi");

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

const num_mesh_textures = 4;

const cube_mesh = 0;
const helmet_mesh = 1;

const enable_async_shader_compilation = true;

const env_cube_tex_resolution = 1024;
const irradiance_cube_tex_resolution = 128;
const filtered_env_tex_resolution = 512;
const filtered_env_tex_mip_levels = 6;
const brdf_integration_tex_resolution = 512;

const MeshUniforms = struct {
    object_to_world: zm.Mat,
    world_to_clip: zm.Mat,
    camera_position: [3]f32,
    draw_mode: i32,
};

const DemoState = struct {
    allocator: std.mem.Allocator,

    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,

    precompute_env_tex_pipe: zgpu.RenderPipelineHandle = .{},
    precompute_irradiance_tex_pipe: zgpu.RenderPipelineHandle = .{},
    precompute_filtered_env_tex_pipe: zgpu.RenderPipelineHandle = .{},
    precompute_brdf_integration_tex_pipe: zgpu.ComputePipelineHandle = .{},
    mesh_pipe: zgpu.RenderPipelineHandle = .{},
    sample_env_tex_pipe: zgpu.RenderPipelineHandle = .{},

    uniform_tex2d_sam_bgl: zgpu.BindGroupLayoutHandle,
    uniform_texcube_sam_bgl: zgpu.BindGroupLayoutHandle,
    texstorage2d_bgl: zgpu.BindGroupLayoutHandle,

    vertex_buf: zgpu.BufferHandle,
    index_buf: zgpu.BufferHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    mesh_tex: [num_mesh_textures]zgpu.TextureHandle,
    mesh_texv: [num_mesh_textures]zgpu.TextureViewHandle,

    env_cube_tex: zgpu.TextureHandle,
    env_cube_texv: zgpu.TextureViewHandle,

    irradiance_cube_tex: zgpu.TextureHandle,
    irradiance_cube_texv: zgpu.TextureViewHandle,

    filtered_env_cube_tex: zgpu.TextureHandle,
    filtered_env_cube_texv: zgpu.TextureViewHandle,

    brdf_integration_tex: zgpu.TextureHandle,
    brdf_integration_texv: zgpu.TextureViewHandle,

    mesh_bg: zgpu.BindGroupHandle,
    env_bg: zgpu.BindGroupHandle,

    meshes: std.ArrayList(Mesh),

    draw_mode: i32 = 0,
    current_hdri_index: i32 = 1,
    is_lighting_precomputed: bool = false,

    mesh_yaw: f32 = 0.0,
    camera: struct {
        position: [3]f32 = .{ 3.0, 0.0, 3.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 0.0 },
        pitch: f32 = 0.0,
        yaw: f32 = math.pi + 0.25 * math.pi,
    } = .{},
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
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
        defer zmesh.io.freeData(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, &texcoords, &tangents);

        try out_meshes.append(.{
            .index_offset = @as(u32, @intCast(pre_indices_len)),
            .vertex_offset = @as(i32, @intCast(pre_positions_len)),
            .num_indices = @as(u32, @intCast(indices.items.len - pre_indices_len)),
            .num_vertices = @as(u32, @intCast(positions.items.len - pre_positions_len)),
        });
    }
    {
        const pre_indices_len = indices.items.len;
        const pre_positions_len = positions.items.len;

        const data = try zmesh.io.parseAndLoadFile(content_dir ++ "SciFiHelmet/SciFiHelmet.gltf");
        defer zmesh.io.freeData(data);
        try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, &texcoords, &tangents);

        try out_meshes.append(.{
            .index_offset = @as(u32, @intCast(pre_indices_len)),
            .vertex_offset = @as(i32, @intCast(pre_positions_len)),
            .num_indices = @as(u32, @intCast(indices.items.len - pre_indices_len)),
            .num_vertices = @as(u32, @intCast(positions.items.len - pre_positions_len)),
        });
    }

    try out_indices.ensureTotalCapacity(indices.items.len);
    for (indices.items) |mesh_index| {
        out_indices.appendAssumeCapacity(mesh_index);
    }

    try out_vertices.ensureTotalCapacity(positions.items.len);
    for (positions.items, 0..) |_, index| {
        out_vertices.appendAssumeCapacity(.{
            .position = positions.items[index],
            .normal = normals.items[index],
            .texcoord = texcoords.items[index],
            .tangent = tangents.items[index],
        });
    }
}

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

    //
    // Create bind group layouts.
    //
    const mesh_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
        zgpu.textureEntry(1, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.textureEntry(2, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.textureEntry(3, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.textureEntry(4, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.textureEntry(5, .{ .fragment = true }, .float, .tvdim_cube, false),
        zgpu.textureEntry(6, .{ .fragment = true }, .float, .tvdim_cube, false),
        zgpu.textureEntry(7, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.samplerEntry(8, .{ .fragment = true }, .filtering),
    });
    defer gctx.releaseResource(mesh_bgl);

    const uniform_tex2d_sam_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
        zgpu.textureEntry(1, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.samplerEntry(2, .{ .fragment = true }, .filtering),
    });
    const uniform_texcube_sam_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
        zgpu.textureEntry(1, .{ .fragment = true }, .float, .tvdim_cube, false),
        zgpu.samplerEntry(2, .{ .fragment = true }, .filtering),
    });
    const texstorage2d_bgl = gctx.createBindGroupLayout(&.{
        zgpu.storageTextureEntry(0, .{ .compute = true }, .write_only, .rgba16_float, .tvdim_2d),
    });

    //
    // Create meshes.
    //
    zmesh.init(arena);
    defer zmesh.deinit();

    var meshes = std.ArrayList(Mesh).init(allocator);
    var vertices = std.ArrayList(Vertex).init(arena);
    var indices = std.ArrayList(u32).init(arena);
    try loadAllMeshes(arena, &meshes, &vertices, &indices);

    const total_num_vertices = @as(u32, @intCast(vertices.items.len));
    const total_num_indices = @as(u32, @intCast(indices.items.len));

    // Create a vertex buffer.
    const vertex_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(vertex_buf).?, 0, Vertex, vertices.items);

    // Create an index buffer.
    const index_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(u32),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buf).?, 0, u32, indices.items);

    //
    // Create textures.
    //
    const depth = createDepthTexture(gctx);

    // Create mesh textures.
    const mesh_texture_paths = &[num_mesh_textures][:0]const u8{
        content_dir ++ "SciFiHelmet/SciFiHelmet_AmbientOcclusion.png",
        content_dir ++ "SciFiHelmet/SciFiHelmet_BaseColor.png",
        content_dir ++ "SciFiHelmet/SciFiHelmet_MetallicRoughness.png",
        content_dir ++ "SciFiHelmet/SciFiHelmet_Normal.png",
    };
    var mesh_tex: [num_mesh_textures]zgpu.TextureHandle = undefined;
    var mesh_texv: [num_mesh_textures]zgpu.TextureViewHandle = undefined;

    for (mesh_texture_paths, 0..) |path, tex_index| {
        var image = try zstbi.Image.loadFromFile(path, 4);
        defer image.deinit();

        mesh_tex[tex_index] = gctx.createTexture(.{
            .usage = .{ .texture_binding = true, .copy_dst = true },
            .size = .{
                .width = image.width,
                .height = image.height,
                .depth_or_array_layers = 1,
            },
            .format = zgpu.imageInfoToTextureFormat(
                image.num_components,
                image.bytes_per_component,
                image.is_hdr,
            ),
            .mip_level_count = math.log2_int(u32, @max(image.width, image.height)) + 1,
        });
        mesh_texv[tex_index] = gctx.createTextureView(mesh_tex[tex_index], .{});

        gctx.queue.writeTexture(
            .{ .texture = gctx.lookupResource(mesh_tex[tex_index]).? },
            .{
                .bytes_per_row = image.bytes_per_row,
                .rows_per_image = image.height,
            },
            .{ .width = image.width, .height = image.height },
            u8,
            image.data,
        );
    }

    // Create an empty env. cube texture (we will render to it).
    const env_cube_tex = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .render_attachment = true, .copy_dst = true },
        .size = .{
            .width = env_cube_tex_resolution,
            .height = env_cube_tex_resolution,
            .depth_or_array_layers = 6,
        },
        .format = .rgba16_float,
        .mip_level_count = math.log2_int(u32, env_cube_tex_resolution) + 1,
    });
    const env_cube_texv = gctx.createTextureView(env_cube_tex, .{
        .dimension = .tvdim_cube,
    });

    // Create an empty irradiance cube texture (we will render to it).
    const irradiance_cube_tex = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .render_attachment = true, .copy_dst = true },
        .size = .{
            .width = irradiance_cube_tex_resolution,
            .height = irradiance_cube_tex_resolution,
            .depth_or_array_layers = 6,
        },
        .format = .rgba16_float,
        .mip_level_count = math.log2_int(u32, irradiance_cube_tex_resolution) + 1,
    });
    const irradiance_cube_texv = gctx.createTextureView(irradiance_cube_tex, .{
        .dimension = .tvdim_cube,
    });

    // Create an empty filtered env. cube texture (we will render to it).
    const filtered_env_cube_tex = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .render_attachment = true },
        .size = .{
            .width = filtered_env_tex_resolution,
            .height = filtered_env_tex_resolution,
            .depth_or_array_layers = 6,
        },
        .format = .rgba16_float,
        .mip_level_count = filtered_env_tex_mip_levels,
    });
    const filtered_env_cube_texv = gctx.createTextureView(filtered_env_cube_tex, .{
        .dimension = .tvdim_cube,
    });

    // Create an empty BRDF integration texture (we will generate its content in a compute shader).
    const brdf_integration_tex = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .storage_binding = true },
        .size = .{
            .width = brdf_integration_tex_resolution,
            .height = brdf_integration_tex_resolution,
        },
        .format = .rgba16_float,
        .mip_level_count = 1,
    });
    const brdf_integration_texv = gctx.createTextureView(brdf_integration_tex, .{});

    //
    // Create samplers.
    //
    const aniso_sam = gctx.createSampler(.{
        .mag_filter = .linear,
        .min_filter = .linear,
        .mipmap_filter = .linear,
        .max_anisotropy = 16,
    });

    const trilinear_sam = gctx.createSampler(.{
        .mag_filter = .linear,
        .min_filter = .linear,
        .mipmap_filter = .linear,
    });

    //
    // Generates mipmaps on the GPU.
    //
    {
        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            for (mesh_tex) |texture| {
                gctx.generateMipmaps(arena, encoder, texture);
            }

            break :commands encoder.finish(null);
        };
        defer commands.release();
        gctx.submit(&.{commands});
    }

    //
    // Create bind groups.
    //
    const mesh_bg = gctx.createBindGroup(mesh_bgl, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = @sizeOf(MeshUniforms) },
        .{ .binding = 1, .texture_view_handle = mesh_texv[0] },
        .{ .binding = 2, .texture_view_handle = mesh_texv[1] },
        .{ .binding = 3, .texture_view_handle = mesh_texv[2] },
        .{ .binding = 4, .texture_view_handle = mesh_texv[3] },
        .{ .binding = 5, .texture_view_handle = irradiance_cube_texv },
        .{ .binding = 6, .texture_view_handle = filtered_env_cube_texv },
        .{ .binding = 7, .texture_view_handle = brdf_integration_texv },
        .{ .binding = 8, .sampler_handle = aniso_sam },
    });

    const env_bg = gctx.createBindGroup(uniform_texcube_sam_bgl, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = @sizeOf(zm.Mat) },
        .{ .binding = 1, .texture_view_handle = env_cube_texv },
        .{ .binding = 2, .sampler_handle = trilinear_sam },
    });

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .window = window,
        .gctx = gctx,
        .allocator = allocator,
        .uniform_tex2d_sam_bgl = uniform_tex2d_sam_bgl,
        .uniform_texcube_sam_bgl = uniform_texcube_sam_bgl,
        .texstorage2d_bgl = texstorage2d_bgl,
        .vertex_buf = vertex_buf,
        .index_buf = index_buf,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .mesh_tex = mesh_tex,
        .mesh_texv = mesh_texv,
        .env_cube_tex = env_cube_tex,
        .env_cube_texv = env_cube_texv,
        .irradiance_cube_tex = irradiance_cube_tex,
        .irradiance_cube_texv = irradiance_cube_texv,
        .filtered_env_cube_tex = filtered_env_cube_tex,
        .filtered_env_cube_texv = filtered_env_cube_texv,
        .brdf_integration_tex = brdf_integration_tex,
        .brdf_integration_texv = brdf_integration_texv,
        .mesh_bg = mesh_bg,
        .env_bg = env_bg,
        .meshes = meshes,
    };

    //
    // Create pipelines.
    //
    createRenderPipe(
        allocator,
        gctx,
        &.{mesh_bgl},
        wgsl.mesh_vs,
        wgsl.mesh_fs,
        zgpu.GraphicsContext.swapchain_format,
        false,
        wgpu.DepthStencilState{
            .format = .depth32_float,
            .depth_write_enabled = true,
            .depth_compare = .less,
        },
        &demo.mesh_pipe,
    );
    createRenderPipe(
        allocator,
        gctx,
        &.{uniform_texcube_sam_bgl},
        wgsl.sample_env_tex_vs,
        wgsl.sample_env_tex_fs,
        zgpu.GraphicsContext.swapchain_format,
        true,
        wgpu.DepthStencilState{
            .format = .depth32_float,
            .depth_write_enabled = false,
            .depth_compare = .less_equal,
        },
        &demo.sample_env_tex_pipe,
    );
    createRenderPipe(
        allocator,
        gctx,
        &.{uniform_tex2d_sam_bgl},
        wgsl.precompute_env_tex_vs,
        wgsl.precompute_env_tex_fs,
        .rgba16_float,
        true,
        null,
        &demo.precompute_env_tex_pipe,
    );
    createRenderPipe(
        allocator,
        gctx,
        &.{uniform_texcube_sam_bgl},
        wgsl.precompute_irradiance_tex_vs,
        wgsl.precompute_irradiance_tex_fs,
        .rgba16_float,
        true,
        null,
        &demo.precompute_irradiance_tex_pipe,
    );
    createRenderPipe(
        allocator,
        gctx,
        &.{uniform_texcube_sam_bgl},
        wgsl.precompute_filtered_env_tex_vs,
        wgsl.precompute_filtered_env_tex_fs,
        .rgba16_float,
        true,
        null,
        &demo.precompute_filtered_env_tex_pipe,
    );
    {
        const pl = gctx.createPipelineLayout(&.{texstorage2d_bgl});
        defer gctx.releaseResource(pl);

        const cs_mod = zgpu.createWgslShaderModule(
            gctx.device,
            wgsl.precompute_brdf_integration_tex_cs,
            null,
        );
        defer cs_mod.release();

        const pipe_desc = wgpu.ComputePipelineDescriptor{
            .compute = .{
                .module = cs_mod,
                .entry_point = "main",
            },
        };
        if (enable_async_shader_compilation) {
            gctx.createComputePipelineAsync(allocator, pl, pipe_desc, &demo.precompute_brdf_integration_tex_pipe);
        } else {
            demo.precompute_brdf_integration_tex_pipe = gctx.createComputePipeline(pl, pipe_desc);
        }
    }

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.meshes.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
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
        zgui.bulletText("LMB + drag : rotate helmet", .{});
        zgui.bulletText("RMB + drag : rotate camera", .{});
        zgui.bulletText("W, A, S, D : move camera", .{});

        zgui.spacing();
        zgui.spacing();
        zgui.sameLine(.{ .spacing = 0.0 });
        if (zgui.combo("HDRI", .{
            .current_item = &demo.current_hdri_index,
            .items_separated_by_zeros = "Newport Loft\x00Drackenstein Quarry\x00Freight Station\x00\x00",
        })) {
            demo.is_lighting_precomputed = false;
        }

        zgui.spacing();
        zgui.spacing();
        _ = zgui.radioButtonStatePtr("Draw PBR effect", .{ .v = &demo.draw_mode, .v_button = 0 });
        _ = zgui.radioButtonStatePtr("Draw Ambient Occlusion texture", .{ .v = &demo.draw_mode, .v_button = 1 });
        _ = zgui.radioButtonStatePtr("Draw Base Color texture", .{ .v = &demo.draw_mode, .v_button = 2 });
        _ = zgui.radioButtonStatePtr("Draw Metallic texture", .{ .v = &demo.draw_mode, .v_button = 3 });
        _ = zgui.radioButtonStatePtr("Draw Roughness texture", .{ .v = &demo.draw_mode, .v_button = 4 });
        _ = zgui.radioButtonStatePtr("Draw Normal texture", .{ .v = &demo.draw_mode, .v_button = 5 });
    }
    zgui.end();

    const window = demo.window;

    // Handle camera rotation with mouse.
    {
        const cursor_pos = window.getCursorPos();
        const delta_x = @as(f32, @floatCast(cursor_pos[0] - demo.mouse.cursor_pos[0]));
        const delta_y = @as(f32, @floatCast(cursor_pos[1] - demo.mouse.cursor_pos[1]));
        demo.mouse.cursor_pos = cursor_pos;

        if (window.getMouseButton(.left) == .press) {
            demo.mesh_yaw += 0.0025 * delta_x;
            demo.mesh_yaw = zm.modAngle(demo.mesh_yaw);
        } else if (window.getMouseButton(.right) == .press) {
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

    // Keyboard shortcut for switching scenes
    if (demo.is_lighting_precomputed == true and window.getKey(.tab) == .press) {
        demo.current_hdri_index += 1;
        if (demo.current_hdri_index >= 3) demo.current_hdri_index = 0;
        demo.is_lighting_precomputed = false;
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

    const back_buffer_view = gctx.swapchain.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        if (!demo.is_lighting_precomputed) {
            precomputeImageLighting(demo, encoder);
        }

        // Draw SciFiHelmet.
        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buf) orelse break :pass;
            const ib_info = gctx.lookupResourceInfo(demo.index_buf) orelse break :pass;
            const mesh_pipe = gctx.lookupResource(demo.mesh_pipe) orelse break :pass;
            const mesh_bg = gctx.lookupResource(demo.mesh_bg) orelse break :pass;
            const depth_texv = gctx.lookupResource(demo.depth_texv) orelse break :pass;

            const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                .view = back_buffer_view,
                .load_op = .clear,
                .store_op = .store,
            }};
            const depth_attachment = wgpu.RenderPassDepthStencilAttachment{
                .view = depth_texv,
                .depth_load_op = .clear,
                .depth_store_op = .store,
                .depth_clear_value = 1.0,
            };
            const render_pass_info = wgpu.RenderPassDescriptor{
                .color_attachment_count = color_attachments.len,
                .color_attachments = &color_attachments,
                .depth_stencil_attachment = &depth_attachment,
            };
            const pass = encoder.beginRenderPass(render_pass_info);
            defer {
                pass.end();
                pass.release();
            }
            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setIndexBuffer(ib_info.gpuobj.?, .uint32, 0, ib_info.size);
            pass.setPipeline(mesh_pipe);

            const object_to_world = zm.rotationY(demo.mesh_yaw);

            const mem = gctx.uniformsAllocate(MeshUniforms, 1);
            mem.slice[0] = .{
                .object_to_world = zm.transpose(object_to_world),
                .world_to_clip = zm.transpose(cam_world_to_clip),
                .camera_position = demo.camera.position,
                .draw_mode = demo.draw_mode,
            };

            pass.setBindGroup(0, mesh_bg, &.{mem.offset});
            pass.drawIndexed(
                demo.meshes.items[helmet_mesh].num_indices,
                1,
                demo.meshes.items[helmet_mesh].index_offset,
                demo.meshes.items[helmet_mesh].vertex_offset,
                0,
            );
        }

        // Draw env. cube texture.
        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buf) orelse break :pass;
            const ib_info = gctx.lookupResourceInfo(demo.index_buf) orelse break :pass;
            const env_pipe = gctx.lookupResource(demo.sample_env_tex_pipe) orelse break :pass;
            const env_bg = gctx.lookupResource(demo.env_bg) orelse break :pass;
            const depth_texv = gctx.lookupResource(demo.depth_texv) orelse break :pass;

            const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                .view = back_buffer_view,
                .load_op = .load,
                .store_op = .store,
            }};
            const depth_attachment = wgpu.RenderPassDepthStencilAttachment{
                .view = depth_texv,
                .depth_load_op = .load,
                .depth_store_op = .store,
                .depth_clear_value = 1.0,
            };
            const render_pass_info = wgpu.RenderPassDescriptor{
                .color_attachment_count = color_attachments.len,
                .color_attachments = &color_attachments,
                .depth_stencil_attachment = &depth_attachment,
            };
            const pass = encoder.beginRenderPass(render_pass_info);
            defer {
                pass.end();
                pass.release();
            }
            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setIndexBuffer(ib_info.gpuobj.?, .uint32, 0, ib_info.size);
            pass.setPipeline(env_pipe);

            var world_to_view_origin = cam_world_to_view;
            world_to_view_origin[3] = zm.f32x4(0.0, 0.0, 0.0, 1.0);

            const mem = gctx.uniformsAllocate(zm.Mat, 1);
            mem.slice[0] = zm.transpose(zm.mul(world_to_view_origin, cam_view_to_clip));

            pass.setBindGroup(0, env_bg, &.{mem.offset});
            pass.drawIndexed(
                demo.meshes.items[cube_mesh].num_indices,
                1,
                demo.meshes.items[cube_mesh].index_offset,
                demo.meshes.items[cube_mesh].vertex_offset,
                0,
            );
        }

        // Gui pass.
        {
            const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                .view = back_buffer_view,
                .load_op = .load,
                .store_op = .store,
            }};
            const render_pass_info = wgpu.RenderPassDescriptor{
                .color_attachment_count = color_attachments.len,
                .color_attachments = &color_attachments,
            };
            const pass = encoder.beginRenderPass(render_pass_info);
            defer {
                pass.end();
                pass.release();
            }
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

fn precomputeImageLighting(
    demo: *DemoState,
    encoder: wgpu.CommandEncoder,
) void {
    const gctx = demo.gctx;

    _ = gctx.lookupResource(demo.precompute_env_tex_pipe) orelse return;
    _ = gctx.lookupResource(demo.precompute_irradiance_tex_pipe) orelse return;
    _ = gctx.lookupResource(demo.precompute_filtered_env_tex_pipe) orelse return;
    _ = gctx.lookupResource(demo.precompute_brdf_integration_tex_pipe) orelse return;

    // Create HDR source texture (this is an equirect texture, we will generate cubemap from it).
    const hdr_source_tex = hdr_source_tex: {
        const hdri_paths = [_][:0]const u8{
            content_dir ++ "Newport_Loft.hdr",
            content_dir ++ "drackenstein_quarry_4k.hdr",
            content_dir ++ "freight_station_4k.hdr",
        };
        zstbi.setFlipVerticallyOnLoad(true);
        var image = zstbi.Image.loadFromFile(
            hdri_paths[@as(usize, @intCast(demo.current_hdri_index))],
            4,
        ) catch unreachable;
        defer {
            image.deinit();
            zstbi.setFlipVerticallyOnLoad(false);
        }

        const hdr_source_tex = gctx.createTexture(.{
            .usage = .{ .texture_binding = true, .copy_dst = true },
            .size = .{
                .width = image.width,
                .height = image.height,
                .depth_or_array_layers = 1,
            },
            .format = zgpu.imageInfoToTextureFormat(
                image.num_components,
                image.bytes_per_component,
                image.is_hdr,
            ),
            .mip_level_count = 1,
        });

        gctx.queue.writeTexture(
            .{ .texture = gctx.lookupResource(hdr_source_tex).? },
            .{
                .bytes_per_row = image.bytes_per_row,
                .rows_per_image = image.height,
            },
            .{ .width = image.width, .height = image.height },
            u8,
            image.data,
        );

        break :hdr_source_tex hdr_source_tex;
    };
    defer gctx.releaseResource(hdr_source_tex);

    const hdr_source_texv = gctx.createTextureView(hdr_source_tex, .{});
    defer gctx.releaseResource(hdr_source_texv);

    var arena_state = std.heap.ArenaAllocator.init(demo.allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    //
    // Step 1.
    //
    drawToCubeTexture(
        gctx,
        encoder,
        demo.uniform_tex2d_sam_bgl,
        demo.precompute_env_tex_pipe,
        hdr_source_texv, // Source texture view.
        demo.env_cube_tex, // Dest. texture.
        0, // Dest. mipmap level to render to.
        demo.vertex_buf,
        demo.index_buf,
    );
    gctx.generateMipmaps(arena, encoder, demo.env_cube_tex);

    //
    // Step 2.
    //
    drawToCubeTexture(
        gctx,
        encoder,
        demo.uniform_texcube_sam_bgl,
        demo.precompute_irradiance_tex_pipe,
        demo.env_cube_texv, // Source texture view.
        demo.irradiance_cube_tex, // Dest. texture.
        0, // Dest. mipmap level to render to.
        demo.vertex_buf,
        demo.index_buf,
    );
    gctx.generateMipmaps(arena, encoder, demo.irradiance_cube_tex);

    //
    // Step 3.
    //
    {
        var mip_level: u32 = 0;
        while (mip_level < filtered_env_tex_mip_levels) : (mip_level += 1) {
            drawToCubeTexture(
                gctx,
                encoder,
                demo.uniform_texcube_sam_bgl,
                demo.precompute_filtered_env_tex_pipe,
                demo.env_cube_texv, // Source texture view.
                demo.filtered_env_cube_tex, // Dest. texture.
                mip_level, // Dest. mipmap level to render to.
                demo.vertex_buf,
                demo.index_buf,
            );
        }
    }

    //
    // Step 4.
    //
    {
        const bg = gctx.createBindGroup(demo.texstorage2d_bgl, &[_]zgpu.BindGroupEntryInfo{
            .{ .binding = 0, .texture_view_handle = demo.brdf_integration_texv },
        });
        defer gctx.releaseResource(bg);

        const pass = encoder.beginComputePass(null);
        defer {
            pass.end();
            pass.release();
        }
        const num_groups = @divExact(brdf_integration_tex_resolution, 8);
        pass.setPipeline(gctx.lookupResource(demo.precompute_brdf_integration_tex_pipe).?);
        pass.setBindGroup(0, gctx.lookupResource(bg).?, null);
        pass.dispatchWorkgroups(num_groups, num_groups, 1);
    }

    demo.is_lighting_precomputed = true;
}

fn drawToCubeTexture(
    gctx: *zgpu.GraphicsContext,
    encoder: wgpu.CommandEncoder,
    pipe_bgl: zgpu.BindGroupLayoutHandle,
    pipe: zgpu.RenderPipelineHandle,
    source_texv: zgpu.TextureViewHandle,
    dest_tex: zgpu.TextureHandle,
    dest_mip_level: u32,
    vertex_buf: zgpu.BufferHandle,
    index_buf: zgpu.BufferHandle,
) void {
    const dest_tex_info = gctx.lookupResourceInfo(dest_tex) orelse return;
    const vb_info = gctx.lookupResourceInfo(vertex_buf) orelse return;
    const ib_info = gctx.lookupResourceInfo(index_buf) orelse return;
    const pipeline = gctx.lookupResource(pipe) orelse return;

    assert(dest_mip_level < dest_tex_info.mip_level_count);
    const dest_tex_width = dest_tex_info.size.width >> @as(u5, @intCast(dest_mip_level));
    const dest_tex_height = dest_tex_info.size.height >> @as(u5, @intCast(dest_mip_level));
    assert(dest_tex_width == dest_tex_height);

    const sam = gctx.createSampler(.{
        .mag_filter = .linear,
        .min_filter = .linear,
        .mipmap_filter = .linear,
    });
    defer gctx.releaseResource(sam);

    const Uniforms = extern struct {
        object_to_clip: zm.Mat,
        roughness: f32,
    };

    const bg = gctx.createBindGroup(pipe_bgl, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = @sizeOf(Uniforms) },
        .{ .binding = 1, .texture_view_handle = source_texv },
        .{ .binding = 2, .sampler_handle = sam },
    });
    defer gctx.releaseResource(bg);

    const zero = zm.f32x4(0.0, 0.0, 0.0, 0.0);
    const object_to_view = [_]zm.Mat{
        zm.lookToLh(zero, zm.f32x4(1.0, 0.0, 0.0, 0.0), zm.f32x4(0.0, 1.0, 0.0, 0.0)),
        zm.lookToLh(zero, zm.f32x4(-1.0, 0.0, 0.0, 0.0), zm.f32x4(0.0, 1.0, 0.0, 0.0)),
        zm.lookToLh(zero, zm.f32x4(0.0, 1.0, 0.0, 0.0), zm.f32x4(0.0, 0.0, -1.0, 0.0)),
        zm.lookToLh(zero, zm.f32x4(0.0, -1.0, 0.0, 0.0), zm.f32x4(0.0, 0.0, 1.0, 0.0)),
        zm.lookToLh(zero, zm.f32x4(0.0, 0.0, 1.0, 0.0), zm.f32x4(0.0, 1.0, 0.0, 0.0)),
        zm.lookToLh(zero, zm.f32x4(0.0, 0.0, -1.0, 0.0), zm.f32x4(0.0, 1.0, 0.0, 0.0)),
    };
    const view_to_clip = zm.perspectiveFovLh(math.pi * 0.5, 1.0, 0.1, 10.0);

    var cube_face_idx: u32 = 0;
    while (cube_face_idx < 6) : (cube_face_idx += 1) {
        const face_texv = gctx.createTextureView(dest_tex, .{
            .dimension = .tvdim_2d,
            .base_mip_level = dest_mip_level,
            .mip_level_count = 1,
            .base_array_layer = cube_face_idx,
            .array_layer_count = 1,
        });
        defer gctx.releaseResource(face_texv);

        const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
            .view = gctx.lookupResource(face_texv).?,
            .load_op = .clear,
            .store_op = .store,
        }};
        const render_pass_info = wgpu.RenderPassDescriptor{
            .color_attachment_count = color_attachments.len,
            .color_attachments = &color_attachments,
        };
        const pass = encoder.beginRenderPass(render_pass_info);
        defer {
            pass.end();
            pass.release();
        }

        pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
        pass.setIndexBuffer(ib_info.gpuobj.?, .uint32, 0, ib_info.size);

        pass.setPipeline(pipeline);

        const mem = gctx.uniformsAllocate(Uniforms, 1);
        mem.slice[0] = .{
            .object_to_clip = zm.transpose(zm.mul(object_to_view[cube_face_idx], view_to_clip)),
            .roughness = @as(f32, @floatFromInt(dest_mip_level + 1)) / @as(f32, @floatFromInt(filtered_env_tex_mip_levels)),
        };
        pass.setBindGroup(0, gctx.lookupResource(bg).?, &.{mem.offset});

        // NOTE: We assume that the first mesh in vertex/index buffer is a 'cube'.
        pass.drawIndexed(36, 1, 0, 0, 0);
    }
}

fn createRenderPipe(
    allocator: std.mem.Allocator,
    gctx: *zgpu.GraphicsContext,
    bgls: []const zgpu.BindGroupLayoutHandle,
    wgsl_vs: [:0]const u8,
    wgsl_fs: [:0]const u8,
    format: wgpu.TextureFormat,
    only_position_attrib: bool,
    depth_state: ?wgpu.DepthStencilState,
    out_pipe: *zgpu.RenderPipelineHandle,
) void {
    const pl = gctx.createPipelineLayout(bgls);
    defer gctx.releaseResource(pl);

    const vs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl_vs, null);
    defer vs_mod.release();

    const fs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl_fs, null);
    defer fs_mod.release();

    const color_targets = [_]wgpu.ColorTargetState{.{
        .format = format,
    }};

    const vertex_attributes = [_]wgpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
        .{ .format = .float32x2, .offset = @offsetOf(Vertex, "texcoord"), .shader_location = 2 },
        .{ .format = .float32x4, .offset = @offsetOf(Vertex, "tangent"), .shader_location = 3 },
    };
    const vertex_buffers = [_]wgpu.VertexBufferLayout{.{
        .array_stride = @sizeOf(Vertex),
        .attribute_count = if (only_position_attrib) 1 else vertex_attributes.len,
        .attributes = &vertex_attributes,
    }};

    // Create a render pipeline.
    const pipe_desc = wgpu.RenderPipelineDescriptor{
        .vertex = wgpu.VertexState{
            .module = vs_mod,
            .entry_point = "main",
            .buffer_count = vertex_buffers.len,
            .buffers = &vertex_buffers,
        },
        .fragment = &wgpu.FragmentState{
            .module = fs_mod,
            .entry_point = "main",
            .target_count = color_targets.len,
            .targets = &color_targets,
        },
        .depth_stencil = if (depth_state) |ds| &ds else null,
    };

    if (enable_async_shader_compilation) {
        gctx.createRenderPipelineAsync(allocator, pl, pipe_desc, out_pipe);
    } else {
        out_pipe.* = gctx.createRenderPipeline(pl, pipe_desc);
    }
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

    zstbi.init(allocator);
    defer zstbi.deinit();

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
