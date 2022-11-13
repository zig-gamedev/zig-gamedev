const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: text transform(wgpu)";

// zig fmt: off
const wgsl_vs =
\\  struct Uniforms {
\\      aspect_ratio: f32,
\\  }
\\  @group(0) @binding(0) var<uniform> uniforms: Uniforms;
\\
\\  struct Vertex {
\\      @builtin(vertex_index) index : u32,
\\  }
\\
\\  struct Fragment {
\\      @builtin(position) position: vec4<f32>,
\\      @location(0) uv: vec2<f32>,
\\  }
\\
\\  @vertex
\\  fn main(vertex: Vertex) -> Fragment {
\\      var position = array<vec2<f32>, 6>(
\\          vec2<f32>(-0.25,  0.25),
\\          vec2<f32>(-0.25, -0.25),
\\          vec2<f32>( 0.25, -0.25),
\\          vec2<f32>(-0.25,  0.25),
\\          vec2<f32>( 0.25, -0.25),
\\          vec2<f32>( 0.25,  0.25),
\\      );
\\      var uv = array<vec2<f32>, 6>(
\\          vec2<f32>(0.0, 0.0),
\\          vec2<f32>(0.0, 1.0),
\\          vec2<f32>(1.0, 1.0),
\\          vec2<f32>(0.0, 0.0),
\\          vec2<f32>(1.0, 1.0),
\\          vec2<f32>(1.0, 0.0),
\\      );
\\      var fragment: Fragment;
\\      fragment.position = vec4<f32>(position[vertex.index].x / uniforms.aspect_ratio, position[vertex.index].y, 0.0, 1.0);
\\      fragment.uv = uv[vertex.index];
\\      return fragment;
\\  }
;
const wgsl_fs =
\\  @group(0) @binding(1) var text_sampler: sampler;
\\  @group(0) @binding(2) var text_texture: texture_2d<f32>;
\\
\\  struct Fragment {
\\      @location(0) uv: vec2<f32>,
\\  }
\\
\\  struct Screen {
\\      @location(0) color: vec4<f32>,
\\  }
\\
\\  @fragment
\\  fn main(fragment: Fragment) -> Screen {
\\      var screen: Screen;
\\      screen.color = textureSampleLevel(text_texture, text_sampler, fragment.uv, 1.0);
\\      return screen;
\\  }
// zig fmt: on
;

const Uniforms = extern struct {
    aspect_ratio: f32,
};

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    text_texture: zgpu.TextureHandle = .{},
    text_texture_view: zgpu.TextureViewHandle = .{},
    text_texture_cache_key: [32]u8 = undefined,

    sample_text: [32]u8 = [_]u8{0} ** 32,
    render_scale: f32 = 1,
    msaa: bool = false,
    offset: [2]f32 = .{ 0, 0 },
    scale: [2]f32 = .{ 1, 1 },
    angle: f32 = 0,

    pipeline: zgpu.RenderPipelineHandle = .{},
    bind_group: zgpu.BindGroupHandle = .{},
    sampler: zgpu.SamplerHandle,

    fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
        const gctx = try zgpu.GraphicsContext.create(allocator, window);

        zgui.init(allocator);
        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor math.max(scale[0], scale[1]);
        };
        _ = zgui.io.addFontFromFile(
            content_dir ++ "Roboto-Medium.ttf",
            math.floor(20.0 * scale_factor),
        );
        {
            var config = zgui.FontConfig.init();
            config.merge_mode = true;
            const ranges: []const u16 = &.{ 0x02DA, 0x02DB, 0 };
            _ = zgui.io.addFontFromFileWithConfig(
                content_dir ++ "Roboto-Medium.ttf",
                math.floor(20.0 * scale_factor),
                config,
                ranges.ptr,
            );
        }

        // This needs to be called *after* adding your custom fonts.
        zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

        const sampler = gctx.createSampler(.{});

        var demo = DemoState{
            .gctx = gctx,
            .sampler = sampler,
        };

        const default_text = "Greetings!";
        std.mem.copy(u8, &demo.sample_text, default_text);

        return demo;
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        const gctx = demo.gctx;
        zgui.backend.deinit();
        zgui.deinit();
        gctx.destroy(allocator);
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        const gctx = demo.gctx;

        if (!std.mem.eql(u8, &demo.text_texture_cache_key, &demo.sample_text)) {
            demo.rerenderTextTexture();
            std.mem.copy(u8, &demo.text_texture_cache_key, &demo.sample_text);

            const bind_group_layout = gctx.createBindGroupLayout(&.{
                zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
                zgpu.samplerEntry(1, .{ .fragment = true }, .filtering),
                zgpu.textureEntry(2, .{ .fragment = true }, .float, .tvdim_2d, false),
            });
            defer gctx.releaseResource(bind_group_layout);

            gctx.releaseResource(demo.bind_group);
            demo.bind_group = gctx.createBindGroup(bind_group_layout, &.{
                .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = 256 },
                .{ .binding = 1, .sampler_handle = demo.sampler },
                .{ .binding = 2, .texture_view_handle = demo.text_texture_view },
            });

            {
                gctx.releaseResource(demo.pipeline);
                const pipeline_layout = gctx.createPipelineLayout(&.{bind_group_layout});
                defer gctx.releaseResource(pipeline_layout);

                const vs_module = zgpu.createWgslShaderModule(gctx.device, wgsl_vs, "vs");
                defer vs_module.release();

                const fs_module = zgpu.createWgslShaderModule(gctx.device, wgsl_fs, "fs");
                defer fs_module.release();

                const color_targets = [_]wgpu.ColorTargetState{.{
                    .format = zgpu.GraphicsContext.swapchain_format,
                }};

                // Create a render pipeline.
                const pipeline_descriptor = wgpu.RenderPipelineDescriptor{
                    .vertex = .{
                        .module = vs_module,
                        .entry_point = "main",
                    },
                    .primitive = .{
                        .front_face = .ccw,
                        .cull_mode = .back,
                        .topology = .triangle_list,
                    },
                    .fragment = &.{
                        .module = fs_module,
                        .entry_point = "main",
                        .target_count = color_targets.len,
                        .targets = &color_targets,
                    },
                };
                demo.pipeline = gctx.createRenderPipeline(pipeline_layout, pipeline_descriptor);
            }
        }

        zgui.backend.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );

        _ = zgui.begin("Controls", .{
            .flags = .{
                .no_title_bar = true,
                .no_move = true,
                .no_collapse = true,
                .always_auto_resize = true,
            },
        });
        defer zgui.end();

        if (!zgui.isAnyItemActive()) {
            zgui.setKeyboardFocusHere(0);
        }
        _ = zgui.inputText("Sample text", .{ .buf = demo.sample_text[0..] });
        _ = zgui.sliderFloat("Render scale", .{
            .v = &demo.render_scale,
            .min = 0.01,
            .max = 8,
            .cfmt = "%.2fx",
        });
        _ = zgui.checkbox("4x multisample anti-aliasing", .{ .v = &demo.msaa });
        _ = zgui.sliderFloat2("Translate", .{
            .v = &demo.offset,
            .min = -1000,
            .max = 1000,
            .cfmt = "%.0f",
        });
        _ = zgui.sliderFloat2("Scale", .{
            .v = &demo.scale,
            .min = -2,
            .max = 2,
            .cfmt = "%.2f x",
        });
        _ = zgui.sliderFloat("Rotate", .{
            .v = &demo.angle,
            .min = 0,
            .max = 360,
            .cfmt = "%.0f˚",
        });
    }

    fn draw(demo: *DemoState) void {
        const gctx = demo.gctx;

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            {
                const text_texture_info = gctx.lookupResourceInfo(demo.text_texture).?;
                const pipeline = gctx.lookupResource(demo.pipeline).?;
                const bind_group = gctx.lookupResource(demo.bind_group).?;
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
                pass.setPipeline(pipeline);
                const screen_aspect_ratio = @intToFloat(f32, gctx.swapchain_descriptor.width) / @intToFloat(f32, gctx.swapchain_descriptor.height);
                const texture_aspect_ratio = @intToFloat(f32, text_texture_info.size.width) / @intToFloat(f32, text_texture_info.size.height);
                const mem = gctx.uniformsAllocate(Uniforms, 1);
                mem.slice[0] = .{
                    .aspect_ratio = screen_aspect_ratio / texture_aspect_ratio,
                };
                pass.setBindGroup(0, bind_group, &.{mem.offset});
                pass.draw(6, 1, 0, 0);
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
        _ = gctx.present();
    }

    fn rerenderTextTexture(demo: *DemoState) void {
        const gctx = demo.gctx;

        zgui.backend.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        const length = std.mem.indexOf(u8, &demo.sample_text, &[_]u8{0});
        const text = demo.sample_text[0 .. length orelse 32];
        const text_size = zgui.calcTextSize(text, .{});
        const texture_size = .{
            .width = @floatToInt(u32, @ceil(text_size[0])),
            .height = @floatToInt(u32, @ceil(text_size[1])),
        };
        zgui.endFrame();

        zgui.backend.newFrame(
            texture_size.width,
            texture_size.height,
        );
        zgui.pushStyleVar2f(.{
            .idx = .window_padding,
            .v = .{ 0, 0 },
        });
        zgui.pushStyleVar1f(.{
            .idx = .window_border_size,
            .v = 0,
        });
        zgui.pushStyleVar2f(.{
            .idx = .frame_padding,
            .v = .{ 0, 0 },
        });
        zgui.pushStyleVar1f(.{
            .idx = .frame_border_size,
            .v = 0,
        });
        {
            zgui.setNextWindowPos(.{ .x = 0, .y = 0 });
            zgui.setNextWindowSize(.{
                .w = @intToFloat(f32, texture_size.width),
                .h = @intToFloat(f32, texture_size.height),
            });
            _ = zgui.begin("Text", .{
                .flags = .{
                    .no_title_bar = true,
                    .no_resize = true,
                    .no_move = true,
                    .no_collapse = true,
                },
            });
            defer zgui.end();
            zgui.textUnformatted(text);
        }

        zgui.popStyleVar(.{
            .count = 4,
        });

        gctx.releaseResource(demo.text_texture_view);
        gctx.destroyResource(demo.text_texture);
        demo.text_texture = gctx.createTexture(.{
            .usage = .{
                .texture_binding = true,
                .render_attachment = true,
            },
            .dimension = .tdim_2d,
            .size = texture_size,
            .format = gctx.swapchain_descriptor.format,
            .sample_count = 1,
        });
        demo.text_texture_view = gctx.createTextureView(demo.text_texture, .{});

        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            {
                const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                    .view = gctx.lookupResource(demo.text_texture_view).?,
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
    }
};

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

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = DemoState.init(allocator, window) catch {
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
