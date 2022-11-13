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
\\  @group(0) @binding(0) var<uniform> object_to_clip: mat4x4<f32>;
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
\\          vec2<f32>(-1.0,  1.0),
\\          vec2<f32>(-1.0, -1.0),
\\          vec2<f32>( 1.0, -1.0),
\\          vec2<f32>(-1.0,  1.0),
\\          vec2<f32>( 1.0, -1.0),
\\          vec2<f32>( 1.0,  1.0),
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
\\      let object = vec4<f32>(position[vertex.index], 0.0, 1.0);
\\      fragment.position = object * object_to_clip;
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
    text_texture_cache_key: struct {
        text: [32]u8,
        scaled_font: usize,
    } = .{
        .text = undefined,
        .scaled_font = 0,
    },

    sample_text: [32]u8 = [_]u8{0} ** 32,
    render_scale: i32 = 1,
    offset: [2]f32 = .{ 0, 0 },
    scale: f32 = 1,
    angle: f32 = 0,

    pipeline: zgpu.RenderPipelineHandle = .{},
    bind_group: zgpu.BindGroupHandle = .{},
    sampler: zgpu.SamplerHandle,

    scaled_fonts: [6]zgui.Font,

    fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
        const gctx = try zgpu.GraphicsContext.create(allocator, window);

        zgui.init(allocator);

        const screen_scale_factor = screen_scale_factor: {
            const scale = window.getContentScale();
            break :screen_scale_factor @max(scale[0], scale[1]);
        };
        {
            zgui.useGlfw();
            _ = zgui.io.addFontFromFile(
                content_dir ++ "Roboto-Medium.ttf",
                @floor(20.0 * screen_scale_factor),
            );
            zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));
        }

        zgui.initOffscreen();
        var scaled_fonts: [6]zgui.Font = undefined;
        {
            zgui.useOffscreen();
            var i: usize = 0;
            while (i < scaled_fonts.len) : (i += 1) {
                const render_scale = @intToFloat(f32, i + 1);
                scaled_fonts[i] = zgui.io.addFontFromFile(
                    content_dir ++ "Roboto-Medium.ttf",
                    @floor(render_scale * 20.0 * screen_scale_factor),
                );
            }
            zgui.offscreen.init(gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));
        }

        const sampler = gctx.createSampler(.{ .min_filter = .linear });

        var demo = DemoState{
            .gctx = gctx,
            .sampler = sampler,
            .scaled_fonts = scaled_fonts,
        };

        const default_text = "Sample";
        std.mem.copy(u8, &demo.sample_text, default_text);

        return demo;
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        const gctx = demo.gctx;
        zgui.offscreen.deinit();
        zgui.deinitOffscreen();

        zgui.backend.deinit();
        zgui.deinit();
        gctx.destroy(allocator);
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        const gctx = demo.gctx;

        {
            zgui.useGlfw();
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

            _ = zgui.inputText("Text", .{ .buf = &demo.sample_text });
            _ = zgui.sliderInt("Render scale", .{
                .v = &demo.render_scale,
                .min = 1,
                .max = 6,
                .cfmt = "%dx",
            });
            _ = zgui.sliderFloat2("Translate", .{
                .v = &demo.offset,
                .min = -500,
                .max = 500,
                .cfmt = "%.0f",
            });
            _ = zgui.sliderFloat("Scale", .{
                .v = &demo.scale,
                .min = 0.01,
                .max = 20,
                .cfmt = "%.2f",
            });
            _ = zgui.sliderAngle("Rotate", .{
                .vrad = &demo.angle,
                .deg_min = -180,
                .deg_max = 180,
            });
        }

        const scaled_font: usize = @intCast(usize, demo.render_scale - 1);
        if (!std.mem.eql(u8, &demo.text_texture_cache_key.text, &demo.sample_text) or demo.text_texture_cache_key.scaled_font != scaled_font) {
            demo.rerenderTextTexture(demo.scaled_fonts[scaled_font]);
            std.mem.copy(u8, &demo.text_texture_cache_key.text, &demo.sample_text);
            demo.text_texture_cache_key.scaled_font = scaled_font;
        }
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
                {
                    const screen_aspect_ratio = @intToFloat(f32, gctx.swapchain_descriptor.width) / @intToFloat(f32, gctx.swapchain_descriptor.height);
                    const texture_aspect_ratio = @intToFloat(f32, text_texture_info.size.width) / @intToFloat(f32, text_texture_info.size.height);
                    const object_to_clip = zm.mul(
                        zm.mul(
                            zm.scaling(
                                texture_aspect_ratio,
                                1.0,
                                1.0,
                            ),
                            zm.mul(
                                zm.scaling(
                                    @intToFloat(f32, text_texture_info.size.height) / @intToFloat(f32, gctx.swapchain_descriptor.height),
                                    @intToFloat(f32, text_texture_info.size.height) / @intToFloat(f32, gctx.swapchain_descriptor.height),
                                    1.0,
                                ),
                                zm.mul(
                                    zm.rotationZ(demo.angle),
                                    zm.mul(
                                        zm.scaling(
                                            demo.scale / @intToFloat(f32, demo.render_scale),
                                            demo.scale / @intToFloat(f32, demo.render_scale),
                                            1.0,
                                        ),
                                        zm.translation(
                                            2 * demo.offset[0] / @intToFloat(f32, gctx.swapchain_descriptor.width),
                                            2 * demo.offset[1] / @intToFloat(f32, gctx.swapchain_descriptor.height),
                                            0.0,
                                        ),
                                    ),
                                ),
                            ),
                        ),
                        zm.scaling(
                            1.0 / screen_aspect_ratio,
                            1.0,
                            1.0,
                        ),
                    );

                    const mem = gctx.uniformsAllocate(zm.Mat, 1);
                    mem.slice[0] = zm.transpose(object_to_clip);

                    pass.setBindGroup(0, bind_group, &.{mem.offset});
                    pass.draw(6, 1, 0, 0);
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

                zgui.useGlfw();
                zgui.backend.draw(pass);
            }

            break :commands encoder.finish(null);
        };
        defer commands.release();

        gctx.submit(&.{commands});
        _ = gctx.present();
    }

    fn rerenderTextTexture(demo: *DemoState, scaled_font: zgui.Font) void {
        const gctx = demo.gctx;

        zgui.useOffscreen();
        zgui.offscreen.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        const length = std.mem.indexOf(u8, &demo.sample_text, &[_]u8{0});
        const text = demo.sample_text[0 .. length orelse 32];

        zgui.pushFont(scaled_font);
        const text_size = zgui.calcTextSize(text, .{});
        zgui.popFont();

        const texture_size = .{
            .width = @floatToInt(u32, @ceil(text_size[0])),
            .height = @floatToInt(u32, @ceil(text_size[1])),
        };
        zgui.endFrame();

        zgui.offscreen.newFrame(
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

            zgui.pushFont(scaled_font);
            zgui.textUnformatted(text);
            zgui.popFont();
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

                zgui.offscreen.draw(pass);
            }

            break :commands encoder.finish(null);
        };
        defer commands.release();

        gctx.submit(&.{commands});

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
        zgui.useGlfw();
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.draw();
    }
}
