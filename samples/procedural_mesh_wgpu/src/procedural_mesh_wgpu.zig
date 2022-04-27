const std = @import("std");
const math = std.math;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const gpu = zgpu.gpu;
const c = zgpu.cimgui;
const zm = @import("zmath");
const zmesh = @import("zmesh");
const znoise = @import("znoise");
const wgsl = @import("procedural_mesh_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: procedural mesh (wgpu)";

const Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
};

const FrameUniforms = struct {
    world_to_clip: [16]f32,
    camera_position: [3]f32,
};

const DrawUniforms = struct {
    object_to_world: [16]f32,
    basecolor_roughness: [4]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,
};

const Drawable = struct {
    mesh_index: u32,
    position: [3]f32,
    basecolor_roughness: [4]f32,
};

const DemoState = struct {
    gctx: zgpu.GraphicsContext,
    stats: zgpu.FrameStats,

    pipeline: zgpu.RenderPipelineHandle,
    draw_bind_group: zgpu.BindGroupHandle,
    frame_bind_group: zgpu.BindGroupHandle,

    vertex_buffer: zgpu.BufferHandle,
    index_buffer: zgpu.BufferHandle,
    uniform_buffer: zgpu.BufferHandle,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    meshes: std.ArrayList(Mesh),
    drawables: std.ArrayList(Drawable),

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

fn appendMesh(
    mesh: zmesh.Shape,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(u16),
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

fn initScene(
    allocator: std.mem.Allocator,
    drawables: *std.ArrayList(Drawable),
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(u16),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    zmesh.init(arena);
    defer zmesh.deinit();

    // Trefoil knot.
    {
        var mesh = zmesh.Shape.initTrefoilKnot(10, 128, 0.8);
        defer mesh.deinit();
        mesh.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 1, 0 },
            .basecolor_roughness = .{ 0.0, 0.7, 0.0, 0.6 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Parametric sphere.
    {
        var mesh = zmesh.Shape.initParametricSphere(20, 20);
        defer mesh.deinit();
        mesh.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 3, 1, 0 },
            .basecolor_roughness = .{ 0.7, 0.0, 0.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Icosahedron.
    {
        var mesh = zmesh.Shape.initIcosahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -3, 1, 0 },
            .basecolor_roughness = .{ 0.7, 0.6, 0.0, 0.4 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Dodecahedron.
    {
        var mesh = zmesh.Shape.initDodecahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 1, 3 },
            .basecolor_roughness = .{ 0.0, 0.1, 1.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Cylinder with top and bottom caps.
    {
        var disk = zmesh.Shape.initParametricDisk(10, 2);
        defer disk.deinit();
        disk.invert(0, 0);

        var cylinder = zmesh.Shape.initCylinder(10, 4);
        defer cylinder.deinit();

        cylinder.merge(disk);
        cylinder.translate(0, 0, -1);
        disk.invert(0, 0);
        cylinder.merge(disk);

        cylinder.scale(0.5, 0.5, 2);
        cylinder.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);

        cylinder.unweld();
        cylinder.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -3, 0, 3 },
            .basecolor_roughness = .{ 1.0, 0.0, 0.0, 0.3 },
        }) catch unreachable;

        appendMesh(cylinder, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Torus.
    {
        var mesh = zmesh.Shape.initTorus(10, 20, 0.2);
        defer mesh.deinit();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 3, 1.5, 3 },
            .basecolor_roughness = .{ 1.0, 0.5, 0.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Subdivided sphere.
    {
        var mesh = zmesh.Shape.initSubdividedSphere(3);
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 3, 1, 6 },
            .basecolor_roughness = .{ 0.0, 1.0, 0.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Tetrahedron.
    {
        var mesh = zmesh.Shape.initTetrahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 0.5, 6 },
            .basecolor_roughness = .{ 1.0, 0.0, 1.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Octahedron.
    {
        var mesh = zmesh.Shape.initOctahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -3, 1, 6 },
            .basecolor_roughness = .{ 0.2, 0.0, 1.0, 0.2 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Rock.
    {
        var rock = zmesh.Shape.initRock(123, 4);
        defer rock.deinit();
        rock.unweld();
        rock.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -6, 0, 3 },
            .basecolor_roughness = .{ 1.0, 1.0, 1.0, 1.0 },
        }) catch unreachable;

        appendMesh(rock, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Custom parametric (simple terrain).
    {
        const gen = znoise.FnlGenerator{
            .fractal_type = .fbm,
            .frequency = 2.0,
            .octaves = 5,
            .lacunarity = 2.02,
        };
        const local = struct {
            fn terrain(uv: *const [2]f32, position: *[3]f32, userdata: ?*anyopaque) callconv(.C) void {
                _ = userdata;
                position[0] = uv[0];
                position[1] = 0.025 * gen.noise2(uv[0], uv[1]);
                position[2] = uv[1];
            }
        };
        var ground = zmesh.Shape.initParametric(local.terrain, 40, 40, null);
        defer ground.deinit();
        ground.translate(-0.5, -0.0, -0.5);
        ground.invert(0, 0);
        ground.scale(20, 20, 20);
        ground.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 0, 0 },
            .basecolor_roughness = .{ 0.1, 0.1, 0.1, 1.0 },
        }) catch unreachable;

        appendMesh(ground, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
}

fn init(allocator: std.mem.Allocator, window: glfw.Window) DemoState {
    var gctx = zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const draw_bgl = gctx.device.createBindGroupLayout(
        &gpu.BindGroupLayout.Descriptor{
            .entries = &.{
                gpu.BindGroupLayout.Entry.buffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
            },
        },
    );
    defer draw_bgl.release();

    const frame_bgl = gctx.device.createBindGroupLayout(
        &gpu.BindGroupLayout.Descriptor{
            .entries = &.{
                gpu.BindGroupLayout.Entry.buffer(0, .{ .vertex = true, .fragment = true }, .uniform, false, 0),
            },
        },
    );
    defer frame_bgl.release();

    const pl = gctx.device.createPipelineLayout(&gpu.PipelineLayout.Descriptor{
        .bind_group_layouts = &.{ draw_bgl, frame_bgl },
    });
    defer pl.release();

    const pipeline = pipeline: {
        const vs_module = gctx.device.createShaderModule(&.{ .label = "vs", .code = .{ .wgsl = wgsl.vs } });
        defer vs_module.release();

        const fs_module = gctx.device.createShaderModule(&.{ .label = "fs", .code = .{ .wgsl = wgsl.fs } });
        defer fs_module.release();

        const color_target = gpu.ColorTargetState{
            .format = zgpu.GraphicsContext.swapchain_format,
            .blend = &.{ .color = .{}, .alpha = .{} },
        };

        const vertex_attributes = [_]gpu.VertexAttribute{
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @sizeOf([3]f32), .shader_location = 1 },
        };
        const vertex_buffer_layout = gpu.VertexBufferLayout{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        };

        // Create a render pipeline.
        const pipeline_descriptor = gpu.RenderPipeline.Descriptor{
            .layout = pl,
            .vertex = gpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffers = &.{vertex_buffer_layout},
            },
            .primitive = gpu.PrimitiveState{
                .front_face = .ccw,
                .cull_mode = .none,
                .topology = .triangle_list,
            },
            .depth_stencil = &gpu.DepthStencilState{
                .format = .depth32_float,
                .depth_write_enabled = true,
                .depth_compare = .less,
            },
            .fragment = &gpu.FragmentState{
                .module = fs_module,
                .entry_point = "main",
                .targets = &.{color_target},
            },
        };
        break :pipeline gctx.createRenderPipeline(pipeline_descriptor);
    };

    // Create an uniform buffer and a bind group for it.
    const uniform_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .uniform = true },
        .size = 64 * 1024,
    });

    const draw_bind_group = gctx.createBindGroup(draw_bgl, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = uniform_buffer, .offset = 512, .size = @sizeOf(DrawUniforms) },
    });
    const frame_bind_group = gctx.createBindGroup(frame_bgl, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = uniform_buffer, .offset = 0, .size = @sizeOf(FrameUniforms) },
    });

    var drawables = std.ArrayList(Drawable).init(allocator);
    var meshes = std.ArrayList(Mesh).init(allocator);
    var meshes_indices = std.ArrayList(u16).init(arena);
    var meshes_positions = std.ArrayList([3]f32).init(arena);
    var meshes_normals = std.ArrayList([3]f32).init(arena);
    initScene(allocator, &drawables, &meshes, &meshes_indices, &meshes_positions, &meshes_normals);

    const total_num_vertices = @intCast(u32, meshes_positions.items.len);
    const total_num_indices = @intCast(u32, meshes_indices.items.len);

    // Create a vertex buffer.
    const vertex_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    {
        var vertex_data = std.ArrayList(Vertex).init(arena);
        defer vertex_data.deinit();
        vertex_data.resize(total_num_vertices) catch unreachable;

        for (meshes_positions.items) |_, i| {
            vertex_data.items[i].position = meshes_positions.items[i];
            vertex_data.items[i].normal = meshes_normals.items[i];
        }
        gctx.queue.writeBuffer(gctx.lookupBuffer(vertex_buffer).?, 0, Vertex, vertex_data.items);
    }

    // Create an index buffer.
    const index_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(u16),
    });
    gctx.queue.writeBuffer(gctx.lookupBuffer(index_buffer).?, 0, u16, meshes_indices.items);

    // Create a depth texture and it's 'view'.
    const fb_size = window.getFramebufferSize() catch unreachable;
    const depth = createDepthTexture(&gctx, fb_size.width, fb_size.height);

    return .{
        .gctx = gctx,
        .stats = .{},
        .pipeline = pipeline,
        .draw_bind_group = draw_bind_group,
        .frame_bind_group = frame_bind_group,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .uniform_buffer = uniform_buffer,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
        .meshes = meshes,
        .drawables = drawables,
    };
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.meshes.deinit();
    demo.drawables.deinit();
    demo.gctx.deinit(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.stats.update(demo.gctx.window, window_title);
    if (!demo.gctx.update()) {
        // Release old depth texture.
        demo.gctx.destroyTextureView(demo.depth_texture_view);
        demo.gctx.destroyTexture(demo.depth_texture);

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(
            &demo.gctx,
            demo.gctx.swapchain_descriptor.width,
            demo.gctx.swapchain_descriptor.height,
        );
        demo.depth_texture = depth.texture;
        demo.depth_texture_view = depth.view;
    }
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    const window = demo.gctx.window;

    c.igSetNextWindowPos(.{ .x = 10.0, .y = 10.0 }, c.ImGuiCond_FirstUseEver, .{ .x = 0.0, .y = 0.0 });
    c.igSetNextWindowSize(.{ .x = 600.0, .y = -1 }, c.ImGuiCond_Always);
    _ = c.igBegin(
        "Demo Settings",
        null,
        c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
    );
    c.igBulletText("", "");
    c.igSameLine(0, -1);
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "Right Mouse Button + drag", "");
    c.igSameLine(0, -1);
    c.igText(" :  rotate camera", "");

    c.igBulletText("", "");
    c.igSameLine(0, -1);
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "W, A, S, D", "");
    c.igSameLine(0, -1);
    c.igText(" :  move camera", "");

    c.igEnd();

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
        const delta_time = zm.f32x4s(demo.stats.delta_time);
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
    var gctx = &demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const cam_world_to_view = zm.lookToLh(
        zm.load(demo.camera.position[0..], zm.Vec, 3),
        zm.load(demo.camera.forward[0..], zm.Vec, 3),
        zm.f32x4(0.0, 1.0, 0.0, 0.0),
    );
    const cam_view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @intToFloat(f32, fb_width) / @intToFloat(f32, fb_height),
        0.01,
        200.0,
    );
    const cam_world_to_clip = zm.mul(cam_world_to_view, cam_view_to_clip);

    const back_buffer_view = gctx.swapchain.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Update camera xform.
        {
            var frame_uniforms: FrameUniforms = undefined;
            zm.storeMat(frame_uniforms.world_to_clip[0..], zm.transpose(cam_world_to_clip));
            frame_uniforms.camera_position = demo.camera.position;
            encoder.writeBuffer(
                gctx.lookupBuffer(demo.uniform_buffer).?,
                0,
                @TypeOf(frame_uniforms),
                &.{frame_uniforms},
            );
        }

        if (demo.stats.frame_number == 1) {
            for (demo.drawables.items) |drawable, drawable_index| {
                const object_to_world = zm.translationV(
                    zm.load(drawable.position[0..], zm.Vec, 3),
                );
                var draw_uniforms: DrawUniforms = undefined;
                zm.storeMat(draw_uniforms.object_to_world[0..], zm.transpose(object_to_world));
                draw_uniforms.basecolor_roughness = drawable.basecolor_roughness;

                encoder.writeBuffer(
                    gctx.lookupBuffer(demo.uniform_buffer).?,
                    512 + 256 * drawable_index,
                    @TypeOf(draw_uniforms),
                    &.{draw_uniforms},
                );
            }
        }

        // Main pass.
        {
            const color_attachment = gpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .load_op = .clear,
                .store_op = .store,
            };
            const depth_attachment = gpu.RenderPassDepthStencilAttachment{
                .view = gctx.lookupTextureView(demo.depth_texture_view).?,
                .depth_load_op = .clear,
                .depth_store_op = .store,
                .depth_clear_value = 1.0,
            };
            const render_pass_info = gpu.RenderPassEncoder.Descriptor{
                .color_attachments = &.{color_attachment},
                .depth_stencil_attachment = &depth_attachment,
            };
            const pass = encoder.beginRenderPass(&render_pass_info);
            defer pass.release();

            if (gctx.lookupBufferInfo(demo.vertex_buffer)) |vb| {
                pass.setVertexBuffer(0, vb.gpuobj.?, 0, vb.size);
            }
            if (gctx.lookupBufferInfo(demo.index_buffer)) |ib| {
                pass.setIndexBuffer(ib.gpuobj.?, .uint16, 0, ib.size);
            }

            if (gctx.lookupRenderPipeline(demo.pipeline)) |pipeline| {
                pass.setPipeline(pipeline);
                pass.setBindGroup(1, gctx.lookupBindGroup(demo.frame_bind_group).?, &.{});

                for (demo.drawables.items) |drawable, drawable_index| {
                    pass.setBindGroup(
                        0,
                        gctx.lookupBindGroup(demo.draw_bind_group).?,
                        &.{@intCast(u32, drawable_index * 256)},
                    );
                    pass.drawIndexed(
                        demo.meshes.items[drawable.mesh_index].num_indices,
                        1,
                        demo.meshes.items[drawable.mesh_index].index_offset,
                        demo.meshes.items[drawable.mesh_index].vertex_offset,
                        0,
                    );
                }
            }
            pass.end();
        }

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
            defer pass.release();

            zgpu.gui.draw(pass);

            pass.end();
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.queue.submit(&.{commands});
    gctx.swapchain.present();
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext, width: u32, height: u32) struct {
    texture: zgpu.TextureHandle,
    view: zgpu.TextureViewHandle,
} {
    const texture = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .dimension_2d,
        .size = .{ .width = width, .height = height, .depth_or_array_layers = 1 },
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const view = gctx.createTextureView(texture, .{
        .format = .depth32_float,
        .dimension = .dimension_2d,
        .base_mip_level = 0,
        .mip_level_count = 1,
        .base_array_layer = 0,
        .array_layer_count = 1,
        .aspect = .depth_only,
    });
    return .{ .texture = texture, .view = view };
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

    var demo = init(allocator, window);
    defer deinit(allocator, &demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir ++ "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        update(&demo);
        draw(&demo);
    }
}
