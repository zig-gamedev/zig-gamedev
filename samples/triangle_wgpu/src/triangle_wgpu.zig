const std = @import("std");
const math = std.math;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const zm = @import("zmath");

// zig fmt: off
const wgsl_vs =
\\  @group(0) @binding(0) var<uniform> object_to_clip : mat4x4<f32>;
\\  struct VertexOut {
\\      @builtin(position) position_clip : vec4<f32>;
\\      @location(0) color : vec3<f32>;
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position : vec3<f32>,
\\      @location(1) color : vec3<f32>
\\  ) -> VertexOut {
\\     var output : VertexOut;
\\     output.position_clip = vec4(position, 1.0) * object_to_clip;
\\     output.color = color;
\\     return output;
\\ }
;
const wgsl_fs =
\\  @stage(fragment) fn main(
\\      @location(0) color : vec3<f32>
\\  ) -> @location(0) vec4<f32> {
\\      return vec4(color, 1.0);
\\  }
// zig fmt: on
;

const Vertex = struct {
    position: [3]f32,
    color: [3]f32,
};

const DemoState = struct {
    gctx: zgpu.GraphicsContext,
    pipeline: zgpu.RenderPipeline,
    bind_group: zgpu.BindGroup,
    vertex_buffer: zgpu.Buffer,
    index_buffer: zgpu.Buffer,
    uniform_buffer: zgpu.Buffer,
    depth_texture: zgpu.Texture,
    depth_texture_view: zgpu.TextureView,
};

fn init(window: glfw.Window) DemoState {
    var gctx = zgpu.GraphicsContext.init(window);

    const vs_module = gctx.device.createShaderModule(&.{ .label = "vs", .code = .{ .wgsl = wgsl_vs } });
    defer vs_module.release();

    const fs_module = gctx.device.createShaderModule(&.{ .label = "fs", .code = .{ .wgsl = wgsl_fs } });
    defer fs_module.release();

    // Setup a 'fragment state' for our render pipeline.
    const blend = zgpu.BlendState{
        .color = .{ .operation = .add, .src_factor = .one, .dst_factor = .zero },
        .alpha = .{ .operation = .add, .src_factor = .one, .dst_factor = .zero },
    };
    const color_target = zgpu.ColorTargetState{
        .format = gctx.swapchain_format,
        .blend = &blend,
        .write_mask = zgpu.ColorWriteMask.all,
    };
    const fragment_state = zgpu.FragmentState{
        .module = fs_module,
        .entry_point = "main",
        .targets = &.{color_target},
    };

    // Setup a 'vertex state' for our render pipeline.
    const vertex_attributes = [_]zgpu.VertexAttribute{
        zgpu.VertexAttribute{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        zgpu.VertexAttribute{ .format = .float32x3, .offset = @sizeOf([3]f32), .shader_location = 1 },
    };
    const vertex_buffer_layout = zgpu.VertexBufferLayout{
        .array_stride = @sizeOf(Vertex),
        .step_mode = .vertex,
        .attribute_count = vertex_attributes.len,
        .attributes = &vertex_attributes,
    };
    const vertex_state = zgpu.VertexState{
        .module = vs_module,
        .entry_point = "main",
        .buffers = &.{vertex_buffer_layout},
    };

    // Create a bind group layout needed for our render pipeline.
    const bgl = gctx.device.createBindGroupLayout(
        &zgpu.BindGroupLayout.Descriptor{
            .entries = &.{
                zgpu.BindGroupLayout.Entry.buffer(0, .{ .vertex = true }, .uniform, true, 0),
            },
        },
    );
    defer bgl.release();

    const pl = gctx.device.createPipelineLayout(&zgpu.PipelineLayout.Descriptor{
        .bind_group_layouts = &.{bgl},
    });
    defer pl.release();

    // Create a render pipeline.
    const pipeline_descriptor = zgpu.RenderPipeline.Descriptor{
        .fragment = &fragment_state,
        .layout = pl,
        .depth_stencil = &.{
            .format = .depth32_float,
            .depth_write_enabled = true,
            .depth_compare = .less,
            .stencil_front = .{
                .compare = .always,
                .fail_op = .keep,
                .depth_fail_op = .keep,
                .pass_op = .keep,
            },
            .stencil_back = .{
                .compare = .always,
                .fail_op = .keep,
                .depth_fail_op = .keep,
                .pass_op = .keep,
            },
            .stencil_read_mask = 0,
            .stencil_write_mask = 0,
            .depth_bias = 0,
            .depth_bias_slope_scale = 0.0,
            .depth_bias_clamp = 0.0,
        },
        .vertex = vertex_state,
        .multisample = .{
            .count = 1,
            .mask = 0xffff_ffff,
            .alpha_to_coverage_enabled = false,
        },
        .primitive = .{
            .front_face = .ccw,
            .cull_mode = .none,
            .topology = .triangle_list,
            .strip_index_format = .none,
        },
    };
    const pipeline = gctx.device.createRenderPipeline(&pipeline_descriptor);

    // Create an uniform buffer and a bind group for it.
    const uniform_buffer = gctx.device.createBuffer(&.{
        .usage = .{ .copy_dst = true, .uniform = true },
        .size = 512,
        .mapped_at_creation = false,
    });
    const bind_group = gctx.device.createBindGroup(
        &zgpu.BindGroup.Descriptor{
            .layout = bgl,
            .entries = &.{zgpu.BindGroup.Entry.buffer(0, uniform_buffer, 0, @sizeOf(zm.Mat))},
        },
    );

    // Create a vertex buffer.
    const vertex_buffer = gctx.device.createBuffer(&.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = 3 * @sizeOf(Vertex),
        .mapped_at_creation = false,
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
        .mapped_at_creation = false,
    });
    const index_data = [_]u32{ 0, 1, 2 };
    gctx.queue.writeBuffer(index_buffer, 0, u32, index_data[0..]);

    // Create a depth texture and it's 'view'.
    const fb_size = window.getFramebufferSize() catch unreachable;
    const depth = createDepthTexture(gctx.device, fb_size.width, fb_size.height);

    return .{
        .gctx = gctx,
        .pipeline = pipeline,
        .bind_group = bind_group,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .uniform_buffer = uniform_buffer,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
    };
}

fn deinit(demo: *DemoState) void {
    demo.pipeline.release();
    demo.bind_group.release();
    demo.vertex_buffer.release();
    demo.index_buffer.release();
    demo.uniform_buffer.release();
    demo.depth_texture_view.release();
    demo.depth_texture.release();
    demo.gctx.deinit();
    demo.* = undefined;
}

fn draw(demo: *DemoState, time: f64) void {
    var gctx = &demo.gctx;
    if (!gctx.update()) {
        // Release old depth texture.
        demo.depth_texture_view.release();
        demo.depth_texture.release();

        // Create new depth texture to match new window size.
        const depth = createDepthTexture(
            demo.gctx.device,
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        demo.depth_texture = depth.texture;
        demo.depth_texture_view = depth.view;
    }
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.width;
    const t = @floatCast(f32, time);

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

        // Update xform matrix for triangle 1.
        {
            const object_to_world = zm.mul(zm.rotationY(t), zm.translation(-1.0, 0.0, 0.0));
            const object_to_clip = zm.mul(object_to_world, cam_world_to_clip);

            var xform: [16]f32 = undefined;
            zm.storeMat(xform[0..], zm.transpose(object_to_clip));

            // Write data at offset 0.
            encoder.writeBuffer(demo.uniform_buffer, 0, f32, xform[0..]);
        }

        // Update xform matrix for triangle 2.
        {
            const object_to_world = zm.mul(zm.rotationY(0.75 * t), zm.translation(1.0, 0.0, 0.0));
            const object_to_clip = zm.mul(object_to_world, cam_world_to_clip);

            var xform: [16]f32 = undefined;
            zm.storeMat(xform[0..], zm.transpose(object_to_clip));

            // Write data at offset 256 (dynamic offsets need to be aligned to 256 bytes).
            encoder.writeBuffer(demo.uniform_buffer, 256, f32, xform[0..]);
        }

        {
            const color_attachment = zgpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .resolve_target = null,
                .clear_value = std.mem.zeroes(zgpu.Color),
                .load_op = .clear,
                .store_op = .store,
            };
            const depth_attachment = zgpu.RenderPassDepthStencilAttachment{
                .view = demo.depth_texture_view,
                .depth_load_op = .clear,
                .depth_store_op = .store,
                .clear_depth = math.nan_f32,
                .depth_clear_value = 1.0,
                .depth_read_only = false,
                .stencil_load_op = .clear,
                .stencil_store_op = .store,
                .clear_stencil = 0,
                .stencil_clear_value = 0,
                .stencil_read_only = false,
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

            pass.setBindGroup(0, demo.bind_group, &.{0});
            pass.drawIndexed(3, 1, 0, 0, 0);

            pass.setBindGroup(0, demo.bind_group, &.{256});
            pass.drawIndexed(3, 1, 0, 0, 0);

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

    const window_title = "zig-gamedev: triangle wgpu";
    const window = try glfw.Window.create(1280, 960, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();

    var demo = init(window);
    defer deinit(&demo);

    var stats = zgpu.FrameStats.init();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        stats.update(window, window_title);
        draw(&demo, stats.time);
    }
}
