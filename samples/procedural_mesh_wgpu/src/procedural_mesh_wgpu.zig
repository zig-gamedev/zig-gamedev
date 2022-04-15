const std = @import("std");
const math = std.math;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const c = zgpu.cimgui;
const zm = @import("zmath");
const zmesh = @import("zmesh");
const znoise = @import("znoise");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: procedural mesh wgpu";

// zig fmt: off
const wgsl_vs =
\\  struct DrawUniforms {
\\      object_to_world: mat4x4<f32>;
\\  }
\\  @group(0) @binding(0) var<uniform> draw_uniforms: DrawUniforms;
\\
\\  struct FrameUniforms {
\\      world_to_clip: mat4x4<f32>;
\\  }
\\  @group(1) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
\\
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>;
\\      @location(0) color: vec3<f32>;
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) color: vec3<f32>,
\\  ) -> VertexOut {
\\     var output: VertexOut;
\\     output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
\\     output.color = color;
\\     return output;
\\ }
;
const wgsl_fs =
\\  @stage(fragment) fn main(
\\      @location(0) color: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      return vec4(color, 1.0);
\\  }
// zig fmt: on
;

const Vertex = struct {
    position: [3]f32,
    color: [3]f32,
};

const FrameUniforms = struct {
    world_to_clip: [16]f32,
};

const DrawUniforms = struct {
    object_to_world: [16]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,
};

const DemoState = struct {
    gctx: zgpu.GraphicsContext,
    stats: zgpu.FrameStats,

    pipeline: zgpu.RenderPipeline,
    draw_bind_group: zgpu.BindGroup,
    frame_bind_group: zgpu.BindGroup,

    vertex_buffer: zgpu.Buffer,
    index_buffer: zgpu.Buffer,
    uniform_buffer: zgpu.Buffer,

    depth_texture: zgpu.Texture,
    depth_texture_view: zgpu.TextureView,
};

fn appendMesh(
    mesh: zmesh.Mesh,
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

fn init(window: glfw.Window) DemoState {
    var gctx = zgpu.GraphicsContext.init(window);

    const draw_bgl = gctx.device.createBindGroupLayout(
        &zgpu.BindGroupLayout.Descriptor{
            .entries = &.{
                zgpu.BindGroupLayout.Entry.buffer(0, .{ .vertex = true }, .uniform, true, 0),
            },
        },
    );
    defer draw_bgl.release();

    const frame_bgl = gctx.device.createBindGroupLayout(
        &zgpu.BindGroupLayout.Descriptor{
            .entries = &.{
                zgpu.BindGroupLayout.Entry.buffer(0, .{ .vertex = true }, .uniform, false, 0),
            },
        },
    );
    defer frame_bgl.release();

    const pl = gctx.device.createPipelineLayout(&zgpu.PipelineLayout.Descriptor{
        .bind_group_layouts = &.{ draw_bgl, frame_bgl },
    });
    defer pl.release();

    const pipeline = blk: {
        const vs_module = gctx.device.createShaderModule(&.{ .label = "vs", .code = .{ .wgsl = wgsl_vs } });
        defer vs_module.release();

        const fs_module = gctx.device.createShaderModule(&.{ .label = "fs", .code = .{ .wgsl = wgsl_fs } });
        defer fs_module.release();

        const color_target = zgpu.ColorTargetState{
            .format = zgpu.GraphicsContext.swapchain_format,
            .blend = &.{ .color = .{}, .alpha = .{} },
        };

        const vertex_attributes = [_]zgpu.VertexAttribute{
            zgpu.VertexAttribute{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            zgpu.VertexAttribute{ .format = .float32x3, .offset = @sizeOf([3]f32), .shader_location = 1 },
        };
        const vertex_buffer_layout = zgpu.VertexBufferLayout{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        };

        // Create a render pipeline.
        const pipeline_descriptor = zgpu.RenderPipeline.Descriptor{
            .layout = pl,
            .vertex = zgpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffers = &.{vertex_buffer_layout},
            },
            .primitive = zgpu.PrimitiveState{
                .front_face = .ccw,
                .cull_mode = .none,
                .topology = .triangle_list,
            },
            .depth_stencil = &zgpu.DepthStencilState{
                .format = .depth32_float,
                .depth_write_enabled = true,
                .depth_compare = .less,
            },
            .fragment = &zgpu.FragmentState{
                .module = fs_module,
                .entry_point = "main",
                .targets = &.{color_target},
            },
        };
        break :blk gctx.device.createRenderPipeline(&pipeline_descriptor);
    };

    // Create an uniform buffer and a bind group for it.
    const uniform_buffer = gctx.device.createBuffer(&.{
        .usage = .{ .copy_dst = true, .uniform = true },
        .size = 64 * 1024,
    });

    const draw_bind_group = gctx.device.createBindGroup(
        &zgpu.BindGroup.Descriptor{
            .layout = draw_bgl,
            .entries = &.{zgpu.BindGroup.Entry.buffer(0, uniform_buffer, 512, @sizeOf(zm.Mat))},
        },
    );
    const frame_bind_group = gctx.device.createBindGroup(
        &zgpu.BindGroup.Descriptor{
            .layout = frame_bgl,
            .entries = &.{zgpu.BindGroup.Entry.buffer(0, uniform_buffer, 0, @sizeOf(zm.Mat))},
        },
    );

    // Create a vertex buffer.
    const vertex_buffer = gctx.device.createBuffer(&.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = 3 * @sizeOf(Vertex),
    });
    const vertex_data = [_]Vertex{
        .{ .position = [3]f32{ 0.0, 0.5, 0.0 }, .color = [3]f32{ 1.0, 0.0, 0.0 } },
        .{ .position = [3]f32{ -0.5, -0.5, 0.0 }, .color = [3]f32{ 0.0, 1.0, 0.0 } },
        .{ .position = [3]f32{ 0.5, -0.5, 0.0 }, .color = [3]f32{ 0.0, 0.0, 1.0 } },
    };
    gctx.queue.writeBuffer(vertex_buffer, 0, Vertex, vertex_data[0..]);

    // Create an index buffer.
    const index_buffer = gctx.device.createBuffer(&.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = 3 * @sizeOf(u32),
    });
    const index_data = [_]u32{ 0, 1, 2 };
    gctx.queue.writeBuffer(index_buffer, 0, u32, index_data[0..]);

    // Create a depth texture and it's 'view'.
    const fb_size = window.getFramebufferSize() catch unreachable;
    const depth = createDepthTexture(gctx.device, fb_size.width, fb_size.height);

    return .{
        .gctx = gctx,
        .stats = zgpu.FrameStats.init(),
        .pipeline = pipeline,
        .draw_bind_group = draw_bind_group,
        .frame_bind_group = frame_bind_group,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .uniform_buffer = uniform_buffer,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
    };
}

fn deinit(demo: *DemoState) void {
    demo.pipeline.release();
    demo.draw_bind_group.release();
    demo.frame_bind_group.release();
    demo.vertex_buffer.release();
    demo.index_buffer.release();
    demo.uniform_buffer.release();
    demo.depth_texture_view.release();
    demo.depth_texture.release();
    demo.gctx.deinit();
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.stats.update(demo.gctx.window, window_title);
    zgpu.gui.newFrame();
    c.igShowDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    if (!gctx.update()) {
        // Release old depth texture.
        demo.depth_texture_view.release();
        demo.depth_texture.release();

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(
            demo.gctx.device,
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        demo.depth_texture = depth.texture;
        demo.depth_texture_view = depth.view;
    }
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;
    const t = @floatCast(f32, demo.stats.time);

    const cam_world_to_view = zm.lookAtLh(
        zm.f32x4(3.0, 3.0, -3.0, 1.0),
        zm.f32x4(0.0, 0.0, 0.0, 1.0),
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

    const commands = blk: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Update camera xform.
        {
            var frame_uniforms: FrameUniforms = undefined;
            zm.storeMat(frame_uniforms.world_to_clip[0..], zm.transpose(cam_world_to_clip));
            encoder.writeBuffer(demo.uniform_buffer, 0, @TypeOf(frame_uniforms), &.{frame_uniforms});
        }

        // Update xform matrix for triangle 1.
        {
            const object_to_world = zm.mul(zm.rotationY(t), zm.translation(-1.0, 0.0, 0.0));

            var draw_uniforms: DrawUniforms = undefined;
            zm.storeMat(draw_uniforms.object_to_world[0..], zm.transpose(object_to_world));

            encoder.writeBuffer(demo.uniform_buffer, 512, @TypeOf(draw_uniforms), &.{draw_uniforms});
        }

        // Update xform matrix for triangle 2.
        {
            const object_to_world = zm.mul(zm.rotationY(0.75 * t), zm.translation(1.0, 0.0, 0.0));

            var draw_uniforms: DrawUniforms = undefined;
            zm.storeMat(draw_uniforms.object_to_world[0..], zm.transpose(object_to_world));

            encoder.writeBuffer(demo.uniform_buffer, 512 + 256, @TypeOf(draw_uniforms), &.{draw_uniforms});
        }

        // Main pass.
        {
            const color_attachment = zgpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .load_op = .clear,
                .store_op = .store,
            };
            const depth_attachment = zgpu.RenderPassDepthStencilAttachment{
                .view = demo.depth_texture_view,
                .depth_load_op = .clear,
                .depth_store_op = .store,
                .depth_clear_value = 1.0,
                .stencil_load_op = .clear,
                .stencil_store_op = .store,
            };
            const render_pass_info = zgpu.RenderPassEncoder.Descriptor{
                .color_attachments = &.{color_attachment},
                .depth_stencil_attachment = &depth_attachment,
            };
            const pass = encoder.beginRenderPass(&render_pass_info);
            defer pass.release();

            pass.setVertexBuffer(0, demo.vertex_buffer, 0, 3 * @sizeOf(Vertex));
            pass.setIndexBuffer(demo.index_buffer, .uint32, 0, 3 * @sizeOf(u32));

            pass.setPipeline(demo.pipeline);
            pass.setBindGroup(1, demo.frame_bind_group, &.{});

            pass.setBindGroup(0, demo.draw_bind_group, &.{0});
            pass.drawIndexed(3, 1, 0, 0, 0);

            pass.setBindGroup(0, demo.draw_bind_group, &.{256});
            pass.drawIndexed(3, 1, 0, 0, 0);

            pass.end();
        }

        // Gui pass.
        {
            const color_attachment = zgpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .load_op = .load,
                .store_op = .store,
            };
            const render_pass_info = zgpu.RenderPassEncoder.Descriptor{
                .color_attachments = &.{color_attachment},
            };
            const pass = encoder.beginRenderPass(&render_pass_info);
            defer pass.release();

            zgpu.gui.draw(pass);

            pass.end();
        }

        break :blk encoder.finish(null);
    };
    defer commands.release();

    gctx.queue.submit(&.{commands});
    gctx.swapchain.present();
}

fn createDepthTexture(device: zgpu.Device, width: u32, height: u32) struct {
    texture: zgpu.Texture,
    view: zgpu.TextureView,
} {
    const texture = device.createTexture(&zgpu.Texture.Descriptor{
        .usage = .{ .render_attachment = true },
        .dimension = .dimension_2d,
        .size = .{ .width = width, .height = height, .depth_or_array_layers = 1 },
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const view = texture.createView(&zgpu.TextureView.Descriptor{
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
    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(1280, 960, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 200, .height = 200 }, .{ .width = null, .height = null });

    var demo = init(window);
    defer deinit(&demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir ++ "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        update(&demo);
        draw(&demo);
    }
}
