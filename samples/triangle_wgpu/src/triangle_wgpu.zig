const std = @import("std");
const glfw = @import("glfw");
const gpu = @import("gpu");
const zgpu = @import("zgpu");

pub fn main() !void {
    //var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    //var allocator = gpa.allocator();

    try glfw.init(.{});

    // Create the test window and discover adapters using it (esp. for OpenGL)
    const hints = glfw.Window.Hints{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    };
    const window = try glfw.Window.create(1280, 960, "zig-gamedev: triangle (WebGPU)", null, null, hints);

    var gctx = zgpu.GraphicsContext.init(window);

    //window.setUserPointer(&gctx);

    const vs =
        \\ @stage(vertex) fn main(
        \\     @builtin(vertex_index) VertexIndex : u32
        \\ ) -> @builtin(position) vec4<f32> {
        \\     var pos = array<vec2<f32>, 3>(
        \\         vec2<f32>( 0.0,  0.5),
        \\         vec2<f32>(-0.5, -0.5),
        \\         vec2<f32>( 0.5, -0.5)
        \\     );
        \\     return vec4<f32>(pos[VertexIndex], 0.0, 1.0);
        \\ }
    ;
    const vs_module = gctx.device.createShaderModule(&.{
        .label = "my vertex shader",
        .code = .{ .wgsl = vs },
    });

    const fs =
        \\ @stage(fragment) fn main() -> @location(0) vec4<f32> {
        \\     return vec4<f32>(0.0, 0.8, 0.0, 1.0);
        \\ }
    ;
    const fs_module = gctx.device.createShaderModule(&.{
        .label = "my fragment shader",
        .code = .{ .wgsl = fs },
    });

    // Fragment state
    const blend = gpu.BlendState{
        .color = .{
            .operation = .add,
            .src_factor = .one,
            .dst_factor = .one,
        },
        .alpha = .{
            .operation = .add,
            .src_factor = .one,
            .dst_factor = .one,
        },
    };
    const color_target = gpu.ColorTargetState{
        .format = gctx.swap_chain_format,
        .blend = &blend,
        .write_mask = .all,
    };
    const fragment = gpu.FragmentState{
        .module = fs_module,
        .entry_point = "main",
        .targets = &.{color_target},
        .constants = null,
    };
    const pipeline_descriptor = gpu.RenderPipeline.Descriptor{
        .fragment = &fragment,
        .layout = null,
        .depth_stencil = null,
        .vertex = .{
            .module = vs_module,
            .entry_point = "main",
            .buffers = null,
        },
        .multisample = .{
            .count = 1,
            .mask = 0xFFFFFFFF,
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

    vs_module.release();
    fs_module.release();

    // Reconfigure the swap chain with the new framebuffer width/height, otherwise e.g. the Vulkan
    // device would be lost after a resize.
    //window.setFramebufferSizeCallback((struct {
    //    fn callback(win: glfw.Window, width: u32, height: u32) void {
    //        const gctx = win.getUserPointer(zgpu.GraphicsContext);
    //        gctx.?.swap_chain_descriptor_target.width = width;
    //        gctx.?.swap_chain_descriptor_target.height = height;
    //    }
    //}).callback);

    const queue = gctx.device.getQueue();
    while (!window.shouldClose()) {
        try frame(.{
            .gctx = &gctx,
            .pipeline = pipeline,
            .queue = queue,
        });
        std.time.sleep(16 * std.time.ns_per_ms);
    }
}

const FrameParams = struct {
    gctx: *zgpu.GraphicsContext,
    pipeline: gpu.RenderPipeline,
    queue: gpu.Queue,
};

fn frame(params: FrameParams) !void {
    try glfw.pollEvents();
    params.gctx.update();

    const back_buffer_view = params.gctx.swap_chain.?.getCurrentTextureView();
    const color_attachment = gpu.RenderPassColorAttachment{
        .view = back_buffer_view,
        .resolve_target = null,
        .clear_value = std.mem.zeroes(gpu.Color),
        .load_op = .clear,
        .store_op = .store,
    };

    const encoder = params.gctx.device.createCommandEncoder(null);
    const render_pass_info = gpu.RenderPassEncoder.Descriptor{
        .color_attachments = &.{color_attachment},
        .depth_stencil_attachment = null,
    };
    const pass = encoder.beginRenderPass(&render_pass_info);
    pass.setPipeline(params.pipeline);
    pass.draw(3, 1, 0, 0);
    pass.end();
    pass.release();

    var command = encoder.finish(null);
    encoder.release();

    params.queue.submit(&.{command});
    command.release();
    params.gctx.swap_chain.?.present();
    back_buffer_view.release();
}
