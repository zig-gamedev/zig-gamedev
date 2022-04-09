const std = @import("std");
const glfw = @import("glfw");
const zgpu = @import("zgpu");

// zig fmt: off
const wgsl_vs =
\\  struct VertexOut {
\\      @builtin(position) position_clip : vec4<f32>;
\\      @location(0) color : vec3<f32>;
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position : vec3<f32>,
\\      @location(1) color : vec3<f32>
\\  ) -> VertexOut {
\\     var output : VertexOut;
\\     output.position_clip = vec4(position, 1.0);
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
    gctx: *zgpu.GraphicsContext,
    pipeline: zgpu.RenderPipeline,
    vertex_buffer: zgpu.Buffer,
};

fn init(allocator: std.mem.Allocator, window: glfw.Window) DemoState {
    var gctx = zgpu.GraphicsContext.create(allocator, window);

    const vs_module = gctx.device.createShaderModule(&.{ .label = "vs", .code = .{ .wgsl = wgsl_vs } });
    defer vs_module.release();

    const fs_module = gctx.device.createShaderModule(&.{ .label = "fs", .code = .{ .wgsl = wgsl_fs } });
    defer fs_module.release();

    const blend = zgpu.BlendState{
        .color = .{ .operation = .add, .src_factor = .one, .dst_factor = .one },
        .alpha = .{ .operation = .add, .src_factor = .one, .dst_factor = .one },
    };
    const color_target = zgpu.ColorTargetState{
        .format = gctx.swap_chain_format,
        .blend = &blend,
        .write_mask = zgpu.ColorWriteMask.all,
    };
    const fragment = zgpu.FragmentState{
        .module = fs_module,
        .entry_point = "main",
        .targets = &.{color_target},
    };

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
    const vertex = zgpu.VertexState{
        .module = vs_module,
        .entry_point = "main",
        .buffers = &.{vertex_buffer_layout},
    };

    const pipeline_descriptor = zgpu.RenderPipeline.Descriptor{
        .fragment = &fragment,
        .layout = null,
        .depth_stencil = null,
        .vertex = vertex,
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

    return .{
        .gctx = gctx,
        .pipeline = pipeline,
        .vertex_buffer = vertex_buffer,
    };
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.pipeline.release();
    demo.vertex_buffer.release();
    allocator.destroy(demo.gctx);
    demo.* = undefined;
}

fn draw(demo: *DemoState) void {
    var gctx = demo.gctx;
    gctx.update();

    const back_buffer_view = gctx.swap_chain.?.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = blk: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();
        {
            const color_attachment = zgpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .resolve_target = null,
                .clear_value = std.mem.zeroes(zgpu.Color),
                .load_op = .clear,
                .store_op = .store,
            };
            const render_pass_info = zgpu.RenderPassEncoder.Descriptor{
                .color_attachments = &.{color_attachment},
                .depth_stencil_attachment = null,
            };
            const pass = encoder.beginRenderPass(&render_pass_info);
            defer pass.release();

            pass.setPipeline(demo.pipeline);
            pass.setVertexBuffer(0, demo.vertex_buffer, 0, 3 * @sizeOf([6]f32));
            pass.draw(3, 1, 0, 0);
            pass.end();
        }
        break :blk encoder.finish(null);
    };
    defer commands.release();

    gctx.queue.submit(&.{commands});
    gctx.swap_chain.?.present();
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(1280, 960, "zig-gamedev: triangle wgpu", null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();

    var demo = init(allocator, window);
    defer deinit(allocator, &demo);

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        draw(&demo);
    }
}
