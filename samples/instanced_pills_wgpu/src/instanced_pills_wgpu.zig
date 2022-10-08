const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const vertex_generator = @import("vertex_generator.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: instanced pills (wgpu)";

// zig fmt: off
const wgsl_vs =
\\  @group(0) @binding(0) var<uniform> object_to_clip: mat4x4<f32>;
\\
\\  struct Vertex {
\\      @location(0) position: vec2<f32>,
\\      @location(1) side: f32,
\\  }
\\
\\  struct Instance {
\\      @location(10) width: f32,
\\      @location(11) length: f32,
\\      @location(12) angle: f32,
\\      @location(13) x: f32,
\\      @location(14) y: f32,
\\  }
\\
\\  struct Fragment {
\\      @builtin(position) position: vec4<f32>,
\\      @location(0) color: vec4<f32>,
\\  }
\\
\\  @vertex fn main(vertex: Vertex, instance: Instance) -> Fragment {
\\      // WebGPU mat4x4 are column vectors
\\      var width_mat: mat4x4<f32> = mat4x4(
\\          instance.width, 0.0, 0.0, 0.0,
\\          0.0, instance.width, 0.0, 0.0,
\\          0.0, 0.0, instance.width, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var length_mat: mat4x4<f32> = mat4x4(
\\          1.0, 0.0, 0.0, vertex.side * instance.length,
\\          0.0, 1.0, 0.0, 0.0,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var angle_mat: mat4x4<f32> = mat4x4(
\\          cos(instance.angle), -sin(instance.angle), 0.0, 0.0,
\\          sin(instance.angle), cos(instance.angle), 0.0, 0.0,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var position_mat: mat4x4<f32> = mat4x4(
\\          1.0, 0.0, 0.0, instance.x,
\\          0.0, 1.0, 0.0, instance.y,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var fragment: Fragment;
\\      fragment.position = vec4(vertex.position, 0.0, 1.0) * width_mat * length_mat * angle_mat * position_mat * object_to_clip;
\\      fragment.color = vec4(1.0, 0.0, 0.0, 1.0);
\\      return fragment;
\\  }
;
const wgsl_fs =
\\  struct Fragment {
\\      @location(0) color: vec4<f32>,
\\  }
\\  struct Screen {
\\      @location(0) color: vec4<f32>,
\\  }
\\
\\  @fragment fn main(fragment: Fragment) -> Screen {
\\      var screen: Screen;
\\      screen.color = fragment.color;
\\      return screen;
\\  }
// zig fmt: on
;

const Vertex = vertex_generator.Vertex;
const Pill = struct {
    width: f32,
    length: f32,
    angle: f32,
    x: f32,
    y: f32,
};

const Dimension = struct {
    width: f32,
    height: f32,
};

const DemoState = @This();

gctx: *zgpu.GraphicsContext,

pills: std.ArrayList(Pill),
vertex_count: u32,

dimension: Dimension,

pipeline: zgpu.RenderPipelineHandle,
bind_group: zgpu.BindGroupHandle,

vertex_buffer: ?zgpu.BufferHandle,
index_buffer: ?zgpu.BufferHandle,
instance_buffer: ?zgpu.BufferHandle,

depth_texture: zgpu.TextureHandle,
depth_texture_view: zgpu.TextureViewHandle,

fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
    const gctx = try zgpu.GraphicsContext.create(allocator, window);

    zgui.init(allocator);
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale[0], scale[1]);
    };
    const font_size = 20.0 * scale_factor;
    const font_normal = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", font_size);
    assert(zgui.io.getFont(0) == font_normal);

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

    const style = zgui.getStyle();

    style.window_min_size = .{ 320.0, 240.0 };
    style.window_border_size = 8.0;
    style.scrollbar_size = 6.0;
    {
        var color = style.getColor(.scrollbar_grab);
        color[1] = 0.8;
        style.setColor(.scrollbar_grab, color);
    }
    style.scaleAllSizes(scale_factor);

    // Create a bind group layout needed for our render pipeline.
    const bind_group_layout = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(bind_group_layout);

    const pipeline_layout = gctx.createPipelineLayout(&.{bind_group_layout});
    defer gctx.releaseResource(pipeline_layout);

    const pipeline = pipline: {
        const vs_module = zgpu.createWgslShaderModule(gctx.device, wgsl_vs, "vs");
        defer vs_module.release();

        const fs_module = zgpu.createWgslShaderModule(gctx.device, wgsl_fs, "fs");
        defer fs_module.release();

        const color_targets = [_]wgpu.ColorTargetState{.{
            .format = zgpu.GraphicsContext.swapchain_format,
        }};

        const vertex_attributes = [_]wgpu.VertexAttribute{
            .{
                .format = .float32x2,
                .offset = @offsetOf(Vertex, "position"),
                .shader_location = 0,
            },
            .{
                .format = .float32,
                .offset = @offsetOf(Vertex, "side"),
                .shader_location = 1,
            },
        };
        const instance_attributes = [_]wgpu.VertexAttribute{
            .{
                .format = .float32,
                .offset = @offsetOf(Pill, "width"),
                .shader_location = 10,
            },
            .{
                .format = .float32,
                .offset = @offsetOf(Pill, "length"),
                .shader_location = 11,
            },
            .{
                .format = .float32,
                .offset = @offsetOf(Pill, "angle"),
                .shader_location = 12,
            },
            .{
                .format = .float32,
                .offset = @offsetOf(Pill, "x"),
                .shader_location = 13,
            },
            .{
                .format = .float32,
                .offset = @offsetOf(Pill, "y"),
                .shader_location = 14,
            },
        };
        const vertex_buffers = [_]wgpu.VertexBufferLayout{
            .{
                .array_stride = @sizeOf(Vertex),
                .attribute_count = vertex_attributes.len,
                .attributes = &vertex_attributes,
            },
            .{
                .array_stride = @sizeOf(Pill),
                .step_mode = .instance,
                .attribute_count = instance_attributes.len,
                .attributes = &instance_attributes,
            },
        };

        const pipeline_descriptor = wgpu.RenderPipelineDescriptor{
            .vertex = wgpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffer_count = vertex_buffers.len,
                .buffers = &vertex_buffers,
            },
            .primitive = wgpu.PrimitiveState{
                .front_face = .ccw,
                .cull_mode = .back,
                .topology = .triangle_strip,
                .strip_index_format = .uint32,
            },
            .depth_stencil = &wgpu.DepthStencilState{
                .format = .depth32_float,
                .depth_write_enabled = true,
                .depth_compare = .less,
            },
            .fragment = &wgpu.FragmentState{
                .module = fs_module,
                .entry_point = "main",
                .target_count = color_targets.len,
                .targets = &color_targets,
            },
        };
        break :pipline gctx.createRenderPipeline(pipeline_layout, pipeline_descriptor);
    };

    const bind_group = gctx.createBindGroup(bind_group_layout, &[_]zgpu.BindGroupEntryInfo{
        .{
            .binding = 0,
            .buffer_handle = gctx.uniforms.buffer,
            .offset = 0,
            .size = @sizeOf(zm.Mat),
        },
    });

    // Create a depth texture and its 'view'.
    const depth = createDepthTexture(gctx);

    return .{
        .gctx = gctx,
        .pills = std.ArrayList(Pill).init(allocator),
        .vertex_count = 0,
        .dimension = calculate_dimensions(gctx),
        .pipeline = pipeline,
        .vertex_buffer = null,
        .index_buffer = null,
        .instance_buffer = null,
        .bind_group = bind_group,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    const gctx = demo.gctx;
    zgui.backend.deinit();
    zgui.deinit();
    if (demo.vertex_buffer) |vb| {
        gctx.destroyResource(vb);
    }
    if (demo.index_buffer) |idb| {
        gctx.destroyResource(idb);
    }
    if (demo.instance_buffer) |itb| {
        gctx.destroyResource(itb);
    }
    demo.pills.deinit();
    gctx.destroy(allocator);
}

fn ensure_4b_multiple(size: usize) usize {
    return (size + 3) & ~@as(usize, 3);
}

fn update(demo: *DemoState, allocator: std.mem.Allocator) !void {
    const gctx = demo.gctx;

    zgui.backend.newFrame(
        gctx.swapchain_descriptor.width,
        gctx.swapchain_descriptor.height,
    );
    if (zgui.begin("Pill control", .{})) {
        zgui.text(
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ gctx.stats.average_cpu_time, gctx.stats.fps },
        );

        const pill_control = struct {
            var segments: i32 = 7;
            var length: f32 = 0.5;
            var width: f32 = 0.1;
            var x: f32 = 0.5;
            var y: f32 = -0.25;
            var angle: f32 = math.pi / 3.0;
        };
        const init_buffers = demo.vertex_buffer == null;
        const needs_vertex_update = zgui.sliderInt("Segments", .{ .v = &pill_control.segments, .min = 2, .max = 20 });
        if (init_buffers or needs_vertex_update) {
            const segments = @intCast(u32, pill_control.segments);
            const vertex_count = 2 * (segments + 1);
            var vertex_data = try allocator.alloc(Vertex, @intCast(usize, vertex_count));
            defer allocator.free(vertex_data);

            var index_data = try allocator.alloc(u32, @intCast(usize, vertex_count));
            defer allocator.free(index_data);

            vertex_generator.pill(segments, vertex_data, index_data);

            if (demo.vertex_buffer) |vb| {
                gctx.destroyResource(vb);
            }
            const vertex_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .vertex = true },
                .size = ensure_4b_multiple(vertex_count * @sizeOf(Vertex)),
            });
            gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, vertex_data);
            demo.vertex_buffer = vertex_buffer;

            if (demo.index_buffer) |idb| {
                gctx.destroyResource(idb);
            }
            const index_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .index = true },
                .size = ensure_4b_multiple(vertex_count * @sizeOf(u32)),
            });
            gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u32, index_data);
            demo.index_buffer = index_buffer;

            if (demo.instance_buffer) |itb| {
                gctx.destroyResource(itb);
            }
            const instance_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .vertex = true },
                .size = ensure_4b_multiple(1 + @sizeOf(Pill)),
            });
            demo.instance_buffer = instance_buffer;

            demo.vertex_count = vertex_count;
        }

        var need_instance_update = std.bit_set.ArrayBitSet(u8, 5).initEmpty();
        need_instance_update.setValue(0, zgui.sliderFloat("Width", .{ .v = &pill_control.width, .min = 0.0, .max = 0.5 }));
        need_instance_update.setValue(1, zgui.sliderFloat("Length", .{ .v = &pill_control.length, .min = 0.0, .max = 1.0 }));
        need_instance_update.setValue(2, zgui.sliderAngle("Angle", .{ .vrad = &pill_control.angle, .deg_min = 0.0, .deg_max = 360.0 }));
        need_instance_update.setValue(3, zgui.sliderFloat("X", .{ .v = &pill_control.x, .min = -1.0, .max = 1.0 }));
        need_instance_update.setValue(4, zgui.sliderFloat("Y", .{ .v = &pill_control.y, .min = -1.0, .max = 1.0 }));
        if (init_buffers or need_instance_update.findFirstSet() != null) {
            demo.pills.clearRetainingCapacity();
            try demo.pills.append(.{
                .width = pill_control.width,
                .length = pill_control.length,
                .angle = pill_control.angle,
                .x = pill_control.x,
                .y = pill_control.y,
            });
        }
    }
    zgui.end();
}

fn calculate_dimensions(gctx: *zgpu.GraphicsContext) Dimension {
    const width = @intToFloat(f32, gctx.swapchain_descriptor.width);
    const height = @intToFloat(f32, gctx.swapchain_descriptor.height);
    const delta = math.sign(@bitCast(i32, gctx.swapchain_descriptor.width) - @bitCast(i32, gctx.swapchain_descriptor.height));
    return switch (delta) {
        -1 => .{ .width = 2.0, .height = 2 * width / height },
        0 => .{ .width = 2.0, .height = 2.0 },
        1 => .{ .width = 2 * height / width, .height = 2.0 },
        else => unreachable,
    };
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;

    const back_buffer_view = gctx.swapchain.getCurrentTextureView();
    defer back_buffer_view.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buffer.?) orelse break :pass;
            const itb_info = gctx.lookupResourceInfo(demo.instance_buffer.?) orelse break :pass;
            const idb_info = gctx.lookupResourceInfo(demo.index_buffer.?) orelse break :pass;
            const pipeline = gctx.lookupResource(demo.pipeline) orelse break :pass;
            const bind_group = gctx.lookupResource(demo.bind_group) orelse break :pass;
            const depth_view = gctx.lookupResource(demo.depth_texture_view) orelse break :pass;

            gctx.queue.writeBuffer(gctx.lookupResource(demo.instance_buffer.?).?, 0, Pill, demo.pills.items);

            const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                .view = back_buffer_view,
                .load_op = .clear,
                .store_op = .store,
            }};
            const depth_attachment = wgpu.RenderPassDepthStencilAttachment{
                .view = depth_view,
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
            pass.setVertexBuffer(1, itb_info.gpuobj.?, 0, itb_info.size);

            pass.setIndexBuffer(idb_info.gpuobj.?, .uint32, 0, idb_info.size);

            pass.setPipeline(pipeline);

            {
                const object_to_clip = zm.scaling(demo.dimension.width / 2, demo.dimension.height / 2, 1.0);

                const mem = gctx.uniformsAllocate(zm.Mat, 1);
                mem.slice[0] = zm.transpose(object_to_clip);

                pass.setBindGroup(0, bind_group, &.{mem.offset});
                pass.drawIndexed(demo.vertex_count, @intCast(u32, demo.pills.items.len), 0, 0, 0);
            }
        }
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
        demo.dimension = calculate_dimensions(gctx);

        // Release old depth texture.
        gctx.releaseResource(demo.depth_texture_view);
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
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
}

pub fn main() !void {
    zglfw.init() catch {
        std.log.err("Failed to initialize GLFW library.", .{});
        return;
    };
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    zglfw.defaultWindowHints();
    zglfw.windowHint(.cocoa_retina_framebuffer, 1);
    zglfw.windowHint(.client_api, 0);
    const window = zglfw.createWindow(1600, 1000, window_title, null, null) catch {
        std.log.err("Failed to create demo window.", .{});
        return;
    };
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = init(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer demo.deinit(allocator);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.draw();
    }
}
