const std = @import("std");
const math = std.math;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const gpu = zgpu.gpu;
const c = zgpu.cimgui;
const zm = @import("zmath");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: textured quad (wgpu)";

// zig fmt: off
const wgsl_common =
\\  struct Uniforms {
\\      aspect_ratio: f32,
\\      mip_level: f32,
\\  }
\\  @group(0) @binding(0) var<uniform> uniforms: Uniforms;
;
const wgsl_vs = wgsl_common ++
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) uv: vec2<f32>,
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position: vec2<f32>,
\\      @location(1) uv: vec2<f32>,
\\  ) -> VertexOut {
\\      let p = vec2(position.x / uniforms.aspect_ratio, position.y);
\\      var output: VertexOut;
\\      output.position_clip = vec4(p, 0.0, 1.0);
\\      output.uv = uv;
\\      return output;
\\  }
;
const wgsl_fs = wgsl_common ++
\\  @group(0) @binding(1) var image: texture_2d<f32>;
\\  @group(0) @binding(2) var image_sampler: sampler;
\\  @stage(fragment) fn main(
\\      @location(0) uv: vec2<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      return textureSampleLevel(image, image_sampler, uv, uniforms.mip_level);
\\  }
// zig fmt: on
;

const Vertex = struct {
    position: [2]f32,
    uv: [2]f32,
};

const Uniforms = struct {
    aspect_ratio: f32,
    mip_level: f32,
};

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    pipeline: zgpu.RenderPipelineHandle = .{},
    bind_group: zgpu.BindGroupHandle,

    vertex_buffer: zgpu.BufferHandle,
    index_buffer: zgpu.BufferHandle,

    texture: zgpu.TextureHandle,
    texture_view: zgpu.TextureViewHandle,
    sampler: zgpu.SamplerHandle,

    mip_level: i32 = 0,
};

fn init(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const bind_group_layout = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
        zgpu.bglTexture(1, .{ .fragment = true }, .float, .dimension_2d, false),
        zgpu.bglSampler(2, .{ .fragment = true }, .filtering),
    });
    defer gctx.destroyResource(bind_group_layout);

    // Create a vertex buffer.
    const vertex_data = [_]Vertex{
        .{ .position = [2]f32{ -0.9, 0.9 }, .uv = [2]f32{ 0.0, 0.0 } },
        .{ .position = [2]f32{ 0.9, 0.9 }, .uv = [2]f32{ 1.0, 0.0 } },
        .{ .position = [2]f32{ 0.9, -0.9 }, .uv = [2]f32{ 1.0, 1.0 } },
        .{ .position = [2]f32{ -0.9, -0.9 }, .uv = [2]f32{ 0.0, 1.0 } },
    };
    const vertex_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = vertex_data.len * @sizeOf(Vertex),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, vertex_data[0..]);

    // Create an index buffer.
    const index_data = [_]u16{ 0, 1, 3, 1, 2, 3 };
    const index_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = index_data.len * @sizeOf(u16),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u16, index_data[0..]);

    // Create a texture.
    var image = try zgpu.stbi.Image(u8).init(content_dir ++ "genart_0025_5.png", 4);
    defer image.deinit();

    const texture = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .copy_dst = true },
        .size = .{
            .width = image.width,
            .height = image.height,
            .depth_or_array_layers = 1,
        },
        .format = .rgba8_unorm,
        .mip_level_count = math.log2_int(u32, math.max(image.width, image.height)) + 1,
    });
    const texture_view = gctx.createTextureView(texture, .{});

    gctx.queue.writeTexture(
        &.{ .texture = gctx.lookupResource(texture).? },
        image.data,
        &.{
            .bytes_per_row = image.width * image.channels_in_memory,
            .rows_per_image = image.height,
        },
        &.{ .width = image.width, .height = image.height },
    );

    // Create a sampler.
    const sampler = gctx.createSampler(.{});

    const bind_group = gctx.createBindGroup(bind_group_layout, &[_]zgpu.BindGroupEntryInfo{
        .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = 256 },
        .{ .binding = 1, .texture_view_handle = texture_view },
        .{ .binding = 2, .sampler_handle = sampler },
    });

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .bind_group = bind_group,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .texture = texture,
        .texture_view = texture_view,
        .sampler = sampler,
    };

    // Generate mipmaps on the GPU.
    {
        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            gctx.generateMipmaps(arena, encoder, demo.texture);

            break :commands encoder.finish(null);
        };
        defer commands.release();
        gctx.submit(&.{commands});
    }

    // (Async) Create a render pipeline.
    {
        const pipeline_layout = gctx.createPipelineLayout(&.{
            bind_group_layout,
        });
        defer gctx.destroyResource(pipeline_layout);

        const vs_module = gctx.device.createShaderModule(&.{ .label = "vs", .code = .{ .wgsl = wgsl_vs } });
        defer vs_module.release();

        const fs_module = gctx.device.createShaderModule(&.{ .label = "fs", .code = .{ .wgsl = wgsl_fs } });
        defer fs_module.release();

        const color_target = gpu.ColorTargetState{
            .format = zgpu.GraphicsContext.swapchain_format,
            .blend = &.{ .color = .{}, .alpha = .{} },
        };

        const vertex_attributes = [_]gpu.VertexAttribute{
            .{ .format = .float32x2, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x2, .offset = @offsetOf(Vertex, "uv"), .shader_location = 1 },
        };
        const vertex_buffer_layout = gpu.VertexBufferLayout{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        };

        // Create a render pipeline.
        const pipeline_descriptor = gpu.RenderPipeline.Descriptor{
            .vertex = gpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffers = &.{vertex_buffer_layout},
            },
            .primitive = gpu.PrimitiveState{
                .front_face = .cw,
                .cull_mode = .back,
                .topology = .triangle_list,
            },
            .fragment = &gpu.FragmentState{
                .module = fs_module,
                .entry_point = "main",
                .targets = &.{color_target},
            },
        };
        gctx.createRenderPipelineAsync(allocator, pipeline_layout, pipeline_descriptor, &demo.pipeline);
    }

    return demo;
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
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
        _ = c.igSliderInt(
            "Mipmap Level",
            &demo.mip_level,
            0,
            @intCast(i32, demo.gctx.lookupResourceInfo(demo.texture).?.mip_level_count - 1),
            null,
            c.ImGuiSliderFlags_None,
        );
    }
    c.igEnd();
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const back_buffer_view = gctx.swapchain.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Main pass.
        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buffer) orelse break :pass;
            const ib_info = gctx.lookupResourceInfo(demo.index_buffer) orelse break :pass;
            const pipeline = gctx.lookupResource(demo.pipeline) orelse break :pass;
            const bind_group = gctx.lookupResource(demo.bind_group) orelse break :pass;

            const color_attachment = gpu.RenderPassColorAttachment{
                .view = back_buffer_view,
                .load_op = .clear,
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

            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setIndexBuffer(ib_info.gpuobj.?, .uint16, 0, ib_info.size);

            pass.setPipeline(pipeline);

            const mem = gctx.uniformsAllocate(Uniforms, 1);
            mem.slice[0] = .{
                .aspect_ratio = @intToFloat(f32, fb_width) / @intToFloat(f32, fb_height),
                .mip_level = @intToFloat(f32, demo.mip_level),
            };
            pass.setBindGroup(0, bind_group, &.{mem.offset});
            pass.drawIndexed(6, 1, 0, 0, 0);
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
    _ = gctx.present();
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
