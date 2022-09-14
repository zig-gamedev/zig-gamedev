const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zstbi = @import("zstbi");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: wrinkles (wgpu)";

const wgsl_common = @embedFile("wrinkles_common.wgsl");
const wgsl_vs = wgsl_common ++ @embedFile("wrinkles_vs.wgsl");
const wgsl_fs = wgsl_common ++ @embedFile("wrinkles_fs.wgsl");

// must match wrinkles_common
const Vertex = extern struct {
    position: [2]f32,
    uv: [2]f32,
};

const Uniforms = extern struct {
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

fn init(allocator: std.mem.Allocator, window: zglfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const bind_group_layout = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
        zgpu.bglTexture(1, .{ .fragment = true }, .float, .tvdim_2d, false),
        zgpu.bglSampler(2, .{ .fragment = true }, .filtering),
    });
    defer gctx.releaseResource(bind_group_layout);

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
    var image = try zstbi.Image(u8).init(content_dir ++ "genart_0025_5.png", 4);
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
        .{ .texture = gctx.lookupResource(texture).? },
        .{
            .bytes_per_row = image.width * image.channels_in_memory,
            .rows_per_image = image.height,
        },
        .{ .width = image.width, .height = image.height },
        u8,
        image.data,
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
    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        gctx.generateMipmaps(arena, encoder, demo.texture);

        break :commands encoder.finish(null);
    };
    defer commands.release();
    gctx.submit(&.{commands});

    // (Async) Create a render pipeline.
    {
        const pipeline_layout = gctx.createPipelineLayout(&.{
            bind_group_layout,
        });
        defer gctx.releaseResource(pipeline_layout);

        const vs_module = zgpu.util.createWgslShaderModule(gctx.device, wgsl_vs, "vs");
        defer vs_module.release();

        const fs_module = zgpu.util.createWgslShaderModule(gctx.device, wgsl_fs, "fs");
        defer fs_module.release();

        const color_targets = [_]wgpu.ColorTargetState{.{
            .format = zgpu.GraphicsContext.swapchain_format,
        }};

        const vertex_attributes = [_]wgpu.VertexAttribute{
            .{ .format = .float32x2, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x2, .offset = @offsetOf(Vertex, "uv"), .shader_location = 1 },
        };
        const vertex_buffers = [_]wgpu.VertexBufferLayout{.{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        }};

        // Create a render pipeline.
        const pipeline_descriptor = wgpu.RenderPipelineDescriptor{
            .vertex = wgpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffer_count = vertex_buffers.len,
                .buffers = &vertex_buffers,
            },
            .primitive = wgpu.PrimitiveState{
                .front_face = .cw,
                .cull_mode = .back,
                .topology = .triangle_list,
            },
            .fragment = &wgpu.FragmentState{
                .module = fs_module,
                .entry_point = "main",
                .target_count = color_targets.len,
                .targets = &color_targets,
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
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    zgui.setNextWindowPos(.{ .x = 20.0, .y = 120.0, .cond = .first_use_ever });
    zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0, .cond = .first_use_ever });

    if (!zgui.begin("Demo Settings", .{})) {
        zgui.end();
        return;
    } 

    zgui.bulletText(
        "Average : {d:.3} ms/frame ({d:.1} fps)",
        .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
    );
    zgui.spacing();
    _ = zgui.sliderInt("Mipmap Level", .{
        .v = &demo.mip_level,
        .min = 0,
        .max = @intCast(i32, demo.gctx.lookupResourceInfo(demo.texture).?.mip_level_count - 1),
    });

    const viewport = zgui.getMainViewport();
    const vsz = viewport.getWorkSize();
    std.debug.print("vp: {d}, {d}\n", .{vsz[0], vsz[1]});

    const draw_list = zgui.getWindowDrawList();
    draw_list.pushClipRect(.{ .pmin = .{ 0, 0 }, .pmax = .{ 400, 400 } });
    draw_list.addLine(.{ .p1 = .{ 0, 0 }, .p2 = .{ 400, 400 }, .col = 0xff_ff_00_ff, .thickness = 5.0 });
    draw_list.popClipRect();

    draw_list.pushClipRectFullScreen();
    draw_list.addRectFilled(.{
        .pmin = .{ 100, 100 },
        .pmax = .{ 300, 200 },
        .col = 0xff_ff_ff_ff,
        .rounding = 25.0,
    });
    draw_list.addRectFilledMultiColor(.{
        .pmin = .{ 100, 300 },
        .pmax = .{ 200, 400 },
        .col_upr_left = 0xff_00_00_ff,
        .col_upr_right = 0xff_00_ff_00,
        .col_bot_right = 0xff_ff_00_00,
        .col_bot_left = 0xff_00_ff_ff,
    });
    draw_list.addQuadFilled(.{
        .p1 = .{ 150, 400 },
        .p2 = .{ 250, 400 },
        .p3 = .{ 200, 500 },
        .p4 = .{ 100, 500 },
        .col = 0xff_ff_ff_ff,
    });
    draw_list.addQuad(.{
        .p1 = .{ 170, 420 },
        .p2 = .{ 270, 420 },
        .p3 = .{ 220, 520 },
        .p4 = .{ 120, 520 },
        .col = 0xff_00_00_ff,
        .thickness = 3.0,
    });
    draw_list.addText(.{ 130, 130 }, 0xff_00_00_ff, "The number is: {}", .{7});
    draw_list.addCircleFilled(.{ .p = .{ 200, 600 }, .r = 50, .col = 0xff_ff_ff_ff });
    draw_list.addCircle(.{ .p = .{ 200, 600 }, .r = 30, .col = 0xff_00_00_ff, .thickness = 11 });
    draw_list.addPolyline(
        &.{ .{ 100, 700 }, .{ 200, 600 }, .{ 300, 700 }, .{ 400, 600 } },
        .{ .col = 0xff_00_aa_11, .thickness = 7 },
    );
    _ = draw_list.getClipRectMin();
    _ = draw_list.getClipRectMax();
    draw_list.popClipRect();

    if (zgui.plot.beginPlot("test_plot", .{})) {
        zgui.plot.plotLineValuesInt("Some data", &.{ 0, 1, 0, 1, 0, 1 }, .{});
        zgui.plot.endPlot();
    }

    zgui.end();
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

            const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                .view = back_buffer_view,
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
    _ = gctx.present();
}

pub fn main() !void {
    try zglfw.init();
    defer zglfw.terminate();

    zgpu.checkSystem(content_dir) catch {
        // In case of error zgpu.checkSystem() will print error message.
        return;
    };

    zglfw.defaultWindowHints();
    zglfw.windowHint(.cocoa_retina_framebuffer, 1);
    zglfw.windowHint(.client_api, 0);
    const window = try zglfw.createWindow(1600, 1000, window_title, null, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try init(allocator, window);
    defer deinit(allocator, demo);

    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale.x, scale.y);
    };

    zgui.init();
    zgui.plot.init();
    defer zgui.plot.deinit();
    //defer zgui.backend.deinit(); will be invoked by glfw
    defer zgui.deinit();

    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", 16.0 * scale_factor);

    zgui.backend.init(
        window,
        demo.gctx.device,
        @enumToInt(zgpu.GraphicsContext.swapchain_format),
    );
    defer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    while (!window.shouldClose()) {
        zglfw.pollEvents();
        update(demo);
        draw(demo);
    }
}
