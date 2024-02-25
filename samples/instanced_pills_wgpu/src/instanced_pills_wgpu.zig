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
\\      @location(13) position: vec2<f32>,
\\      @location(14) start_color: vec4<f32>,
\\      @location(15) end_color: vec4<f32>,
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
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var length_mat: mat4x4<f32> = mat4x4(
\\          1.0, 0.0, 0.0, vertex.side * instance.length / 2.0,
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
\\          1.0, 0.0, 0.0, instance.position.x,
\\          0.0, 1.0, 0.0, instance.position.y,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var fragment: Fragment;
\\      fragment.position = vec4(vertex.position, 0.0, 1.0) * width_mat * length_mat * angle_mat *
\\          position_mat * object_to_clip;
\\      fragment.color = select(instance.end_color, instance.start_color, vertex.side == -1);
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
    position: [2]f32,
    start_color: [4]f32,
    end_color: [4]f32,
};

const Dimension = struct {
    width: f32,
    height: f32,
};

const UpdatedPill = struct {
    length: f32,
    angle: f32,
    position: [2]f32,
};

const DemoState = struct {
    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,

    pills: std.ArrayList(Pill),
    vertex_count: u32,

    dimension: Dimension,

    pipeline: zgpu.RenderPipelineHandle,
    bind_group: zgpu.BindGroupHandle,

    vertex_buffer: zgpu.BufferHandle,
    index_buffer: zgpu.BufferHandle,
    instance_buffer: zgpu.BufferHandle,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    fn init(allocator: std.mem.Allocator, window: *zglfw.Window) !DemoState {
        const gctx = try zgpu.GraphicsContext.create(
            allocator,
            .{
                .window = window,
                .fn_getTime = @ptrCast(&zglfw.getTime),
                .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
                .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
                .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
                .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
                .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
            },
            .{},
        );
        errdefer gctx.destroy(allocator);

        zgui.init(allocator);
        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor @max(scale[0], scale[1]);
        };
        const font_normal = zgui.io.addFontFromFile(
            content_dir ++ "Roboto-Medium.ttf",
            math.floor(20.0 * scale_factor),
        );
        assert(zgui.io.getFont(0) == font_normal);

        // This needs to be called *after* adding your custom fonts.
        zgui.backend.init(
            window,
            gctx.device,
            @intFromEnum(zgpu.GraphicsContext.swapchain_format),
            @intFromEnum(wgpu.TextureFormat.undef),
        );

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

            const vertex_attributes = [_]wgpu.VertexAttribute{ .{
                .format = .float32x2,
                .offset = @offsetOf(Vertex, "position"),
                .shader_location = 0,
            }, .{
                .format = .float32,
                .offset = @offsetOf(Vertex, "side"),
                .shader_location = 1,
            } };

            const instance_attributes = [_]wgpu.VertexAttribute{ .{
                .format = .float32,
                .offset = @offsetOf(Pill, "width"),
                .shader_location = 10,
            }, .{
                .format = .float32,
                .offset = @offsetOf(Pill, "length"),
                .shader_location = 11,
            }, .{
                .format = .float32,
                .offset = @offsetOf(Pill, "angle"),
                .shader_location = 12,
            }, .{
                .format = .float32x2,
                .offset = @offsetOf(Pill, "position"),
                .shader_location = 13,
            }, .{
                .format = .float32x4,
                .offset = @offsetOf(Pill, "start_color"),
                .shader_location = 14,
            }, .{
                .format = .float32x4,
                .offset = @offsetOf(Pill, "end_color"),
                .shader_location = 15,
            } };

            const vertex_buffers = [_]wgpu.VertexBufferLayout{ .{
                .array_stride = @sizeOf(Vertex),
                .attribute_count = vertex_attributes.len,
                .attributes = &vertex_attributes,
            }, .{
                .array_stride = @sizeOf(Pill),
                .step_mode = .instance,
                .attribute_count = instance_attributes.len,
                .attributes = &instance_attributes,
            } };

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
                    .strip_index_format = .uint16,
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

        const bind_group = gctx.createBindGroup(bind_group_layout, &[_]zgpu.BindGroupEntryInfo{.{
            .binding = 0,
            .buffer_handle = gctx.uniforms.buffer,
            .offset = 0,
            .size = @sizeOf(zm.Mat),
        }});

        // Create a depth texture and its 'view'.
        const depth = createDepthTexture(gctx);

        return .{
            .window = window,
            .gctx = gctx,
            .pills = std.ArrayList(Pill).init(allocator),
            .vertex_count = 0,
            .dimension = calculateDimensions(gctx),
            .pipeline = pipeline,
            .vertex_buffer = .{},
            .index_buffer = .{},
            .instance_buffer = .{},
            .bind_group = bind_group,
            .depth_texture = depth.texture,
            .depth_texture_view = depth.view,
        };
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        const gctx = demo.gctx;
        zgui.backend.deinit();
        zgui.deinit();
        demo.pills.deinit();
        gctx.destroy(allocator);
    }

    fn addPill(demo: *DemoState, pill: Pill) !void {
        try demo.pills.append(pill);
    }

    fn addPillByEndpoints(
        demo: *DemoState,
        width: f32,
        start_color: [4]f32,
        end_color: [4]f32,
        v0: zm.F32x4,
        v1: zm.F32x4,
    ) !UpdatedPill {
        const dx = v1[0] - v0[0];
        const dy = v1[1] - v0[1];
        const length = @sqrt(dx * dx + dy * dy);
        const angle = math.atan2(dy, dx);
        const position = .{ (v0[0] + v1[0]) / 2.0, (v0[1] + v1[1]) / 2.0 };
        try demo.addPill(.{
            .width = width,
            .length = length,
            .angle = angle,
            .position = position,
            .start_color = start_color,
            .end_color = end_color,
        });

        return .{
            .length = length,
            .angle = angle,
            .position = position,
        };
    }

    fn recreateVertexBuffers(demo: *DemoState, segments: u16, allocator: std.mem.Allocator) !void {
        const gctx = demo.gctx;

        const vertex_count = 2 * (segments + 1);
        const vertex_data = try allocator.alloc(Vertex, @as(usize, @intCast(vertex_count)));
        defer allocator.free(vertex_data);

        const index_data = try allocator.alloc(u16, @as(usize, @intCast(vertex_count)));
        defer allocator.free(index_data);

        vertex_generator.pill(segments, vertex_data, index_data);

        gctx.destroyResource(demo.vertex_buffer);
        const vertex_buffer = gctx.createBuffer(.{
            .usage = .{ .copy_dst = true, .vertex = true },
            .size = ensureFourByteMultiple(vertex_count * @sizeOf(Vertex)),
        });
        gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, vertex_data);
        demo.vertex_buffer = vertex_buffer;

        gctx.destroyResource(demo.index_buffer);
        const index_buffer = gctx.createBuffer(.{
            .usage = .{ .copy_dst = true, .index = true },
            .size = ensureFourByteMultiple(vertex_count * @sizeOf(u16)),
        });
        gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u16, index_data);
        demo.index_buffer = index_buffer;

        demo.vertex_count = vertex_count;
    }

    fn recreateInstanceBuffer(demo: *DemoState, instances: usize) void {
        const gctx = demo.gctx;

        gctx.destroyResource(demo.instance_buffer);
        const instance_buffer = gctx.createBuffer(.{
            .usage = .{ .copy_dst = true, .vertex = true },
            .size = ensureFourByteMultiple(instances * @sizeOf(Pill)),
        });
        demo.instance_buffer = instance_buffer;
    }

    fn update(demo: *DemoState, allocator: std.mem.Allocator) !void {
        const gctx = demo.gctx;

        zgui.backend.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        _ = zgui.begin("Pill", .{
            .flags = .{
                .no_title_bar = true,
                .no_move = true,
                .no_collapse = true,
                .always_auto_resize = true,
            },
        });
        defer zgui.end();

        zgui.text(
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ gctx.stats.average_cpu_time, gctx.stats.fps },
        );
        _ = zgui.beginTabBar("Demo picker", .{});
        defer zgui.endTabBar();

        const demo_picker = struct {
            var active_tab: u8 = 0;
        };
        if (zgui.beginTabItem("Single pill", .{})) {
            const tab_activated = demo_picker.active_tab != 0;
            demo_picker.active_tab = 0;
            const single_pill = struct {
                var segments: i32 = 7;
                var length: f32 = 0.5;
                var width: f32 = 0.1;
                var angle: f32 = math.pi / 3.0;
                var position: [2]f32 = .{ 0.5, -0.25 };
                var start_color: [4]f32 = .{ 1.0, 0.0, 0.0, 1.0 };
                var end_color: [4]f32 = .{ 0.0, 0.0, 1.0, 1.0 };
            };
            zgui.textUnformatted("Drag sliders or pill directly");
            const init_buffers = !gctx.isResourceValid(demo.vertex_buffer);
            const needs_vertex_update = zgui.sliderInt(
                "Segments",
                .{ .v = &single_pill.segments, .min = 2, .max = 20 },
            );
            if (tab_activated or init_buffers or needs_vertex_update) {
                const segments = @as(u16, @intCast(single_pill.segments));
                try demo.recreateVertexBuffers(segments, allocator);
                demo.recreateInstanceBuffer(1);
            }

            var need_instance_update = std.bit_set.ArrayBitSet(u8, 6).initEmpty();
            need_instance_update.setValue(0, zgui.dragFloat(
                "Width",
                .{ .v = &single_pill.width, .min = 0.0, .max = std.math.inf(f32), .speed = 0.001 },
            ));
            need_instance_update.setValue(1, zgui.dragFloat(
                "Length",
                .{ .v = &single_pill.length, .min = 0.0, .max = std.math.inf(f32), .speed = 0.01 },
            ));
            need_instance_update.setValue(2, zgui.sliderAngle(
                "Angle",
                .{ .vrad = &single_pill.angle, .deg_min = -180.0, .deg_max = 180.0 },
            ));
            need_instance_update.setValue(3, zgui.dragFloat2(
                "Position",
                .{ .v = &single_pill.position, .speed = 0.01 },
            ));
            need_instance_update.setValue(4, zgui.colorEdit3(
                "Start color",
                .{ .col = single_pill.start_color[0..3], .flags = .{ .no_options = true } },
            ));
            need_instance_update.setValue(5, zgui.colorEdit3(
                "End color",
                .{ .col = single_pill.end_color[0..3], .flags = .{ .no_options = true } },
            ));

            if (tab_activated or init_buffers or zgui.io.getWantCaptureMouse() or
                need_instance_update.findFirstSet() != null)
            {
                demo.pills.clearRetainingCapacity();
                try demo.addPill(.{
                    .width = single_pill.width,
                    .length = single_pill.length,
                    .angle = single_pill.angle,
                    .position = single_pill.position,
                    .start_color = single_pill.start_color,
                    .end_color = single_pill.end_color,
                });
            } else {
                const State = enum {
                    idle,
                    v0,
                    v1,
                };
                const dragging = struct {
                    var state: State = .idle;
                    var object_position_start: zm.F32x4 = undefined;
                    var vertex_start: zm.F32x4 = undefined;
                };
                const scale = demo.window.getContentScale();
                const screen_to_clip = zm.mul(
                    zm.scaling(
                        2 * scale[0] / @as(f32, @floatFromInt(gctx.swapchain_descriptor.width)),
                        -2 * scale[1] / @as(f32, @floatFromInt(gctx.swapchain_descriptor.height)),
                        1,
                    ),
                    zm.translation(-1, 1, 0.0),
                );
                const clip_to_object = zm.scaling(2 / demo.dimension.width, 2 / demo.dimension.height, 1.0);

                const cursor_position = demo.window.getCursorPos();
                const screen_position = zm.f32x4(
                    @as(f32, @floatCast(cursor_position[0])),
                    @as(f32, @floatCast(cursor_position[1])),
                    0.0,
                    1.0,
                );
                const clip_position = zm.mul(screen_position, screen_to_clip);
                const object_position = zm.mul(clip_position, clip_to_object);

                const width_mat = zm.scaling(single_pill.width, single_pill.width, 1.0);
                const v0_length_mat = zm.translation(-1 * single_pill.length / 2.0, 0.0, 0.0);
                const v1_length_mat = zm.translation(1 * single_pill.length / 2.0, 0.0, 0.0);
                const angle_mat = zm.rotationZ(single_pill.angle);
                const position_mat = zm.translation(single_pill.position[0], single_pill.position[1], 0.0);

                const v = zm.f32x4(0.0, 0.0, 0.0, 1.0);
                const v0 = zm.mul(v, zm.mul(width_mat, zm.mul(v0_length_mat, zm.mul(angle_mat, position_mat))));
                const v1 = zm.mul(v, zm.mul(width_mat, zm.mul(v1_length_mat, zm.mul(angle_mat, position_mat))));

                if (dragging.state == .idle and demo.window.getMouseButton(.left) == .press) {
                    demo.window.setInputMode(.cursor, .disabled);

                    const v0_dx = object_position[0] - v0[0];
                    const v0_dy = object_position[1] - v0[1];
                    const v0_squared_distance = v0_dx * v0_dx + v0_dy * v0_dy;
                    const v1_dx = object_position[0] - v1[0];
                    const v1_dy = object_position[1] - v1[1];
                    const v1_squared_distance = v1_dx * v1_dx + v1_dy * v1_dy;

                    if (v0_squared_distance < v1_squared_distance) {
                        dragging.state = .v0;
                        dragging.vertex_start = v0;
                    } else {
                        dragging.state = .v1;
                        dragging.vertex_start = v1;
                    }
                    dragging.object_position_start = object_position;
                } else {
                    if (demo.window.getMouseButton(.left) == .release) {
                        dragging.state = .idle;
                        demo.window.setInputMode(.cursor, .normal);
                    } else {
                        const object_position_delta = zm.f32x4(
                            object_position[0] - dragging.object_position_start[0],
                            object_position[1] - dragging.object_position_start[1],
                            0.0,
                            1.0,
                        );

                        demo.pills.clearRetainingCapacity();
                        const updated_pill = if (dragging.state == .v0) move_v0: {
                            const moved_v0 = zm.f32x4(
                                dragging.vertex_start[0] + object_position_delta[0],
                                dragging.vertex_start[1] + object_position_delta[1],
                                0.0,
                                1.0,
                            );
                            break :move_v0 try demo.addPillByEndpoints(
                                single_pill.width,
                                single_pill.start_color,
                                single_pill.end_color,
                                moved_v0,
                                v1,
                            );
                        } else move_v1: {
                            const moved_v1 = zm.f32x4(
                                dragging.vertex_start[0] + object_position_delta[0],
                                dragging.vertex_start[1] + object_position_delta[1],
                                0.0,
                                1.0,
                            );
                            break :move_v1 try demo.addPillByEndpoints(
                                single_pill.width,
                                single_pill.start_color,
                                single_pill.end_color,
                                v0,
                                moved_v1,
                            );
                        };
                        single_pill.length = updated_pill.length;
                        single_pill.angle = updated_pill.angle;
                        single_pill.position = updated_pill.position;
                    }
                }
            }
            zgui.endTabItem();
        }
        if (zgui.beginTabItem("Multiple pills", .{})) {
            const tab_activated = demo_picker.active_tab != 1;
            demo_picker.active_tab = 1;

            const multiple_pills = struct {
                var segments: i32 = 7;
                var instance_index: i32 = 0;
                var rng = std.rand.DefaultPrng.init(42);
            };
            const needs_vertex_update = zgui.sliderInt(
                "Segments",
                .{ .v = &multiple_pills.segments, .min = 2, .max = 20 },
            );
            if (tab_activated or needs_vertex_update) {
                const segments = @as(u16, @intCast(multiple_pills.segments));
                try demo.recreateVertexBuffers(segments, allocator);
            }
            const InstanceValues = [_]usize{ 1000, 10000, 100000, 1000000 };
            const InstanceStrings = [_][:0]const u8{ "1,000", "10,000", "100,000", "1,000,000" };
            const need_instance_update = zgui.sliderInt("Instances", .{
                .v = &multiple_pills.instance_index,
                .min = 0,
                .max = InstanceValues.len - 1,
                .cfmt = InstanceStrings[@as(usize, @intCast(multiple_pills.instance_index))],
            });
            if (tab_activated or need_instance_update) {
                const instances = InstanceValues[@as(usize, @intCast(multiple_pills.instance_index))];
                demo.pills.clearRetainingCapacity();
                var i: usize = 0;
                while (i < instances) : (i += 1) {
                    try demo.addPill(.{
                        .width = multiple_pills.rng.random().float(f32) / 50.0 + 0.01,
                        .length = multiple_pills.rng.random().float(f32) / 5.0 + 0.1,
                        .angle = multiple_pills.rng.random().float(f32) * 2.0 * math.pi,
                        .position = .{
                            multiple_pills.rng.random().float(f32) * 2 - 1,
                            multiple_pills.rng.random().float(f32) * 2 - 1,
                        },
                        .start_color = .{
                            multiple_pills.rng.random().float(f32),
                            multiple_pills.rng.random().float(f32),
                            multiple_pills.rng.random().float(f32),
                            1.0,
                        },
                        .end_color = .{
                            multiple_pills.rng.random().float(f32),
                            multiple_pills.rng.random().float(f32),
                            multiple_pills.rng.random().float(f32),
                            1.0,
                        },
                    });
                }
                demo.recreateInstanceBuffer(instances);
            }
            zgui.endTabItem();
        }
    }

    fn draw(demo: *DemoState) void {
        const gctx = demo.gctx;

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            pass: {
                const vb_info = gctx.lookupResourceInfo(demo.vertex_buffer) orelse break :pass;
                const itb_info = gctx.lookupResourceInfo(demo.instance_buffer) orelse break :pass;
                const idb_info = gctx.lookupResourceInfo(demo.index_buffer) orelse break :pass;
                const pipeline = gctx.lookupResource(demo.pipeline) orelse break :pass;
                const bind_group = gctx.lookupResource(demo.bind_group) orelse break :pass;
                const depth_view = gctx.lookupResource(demo.depth_texture_view) orelse break :pass;

                gctx.queue.writeBuffer(gctx.lookupResource(demo.instance_buffer).?, 0, Pill, demo.pills.items);

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

                pass.setIndexBuffer(idb_info.gpuobj.?, .uint16, 0, idb_info.size);

                pass.setPipeline(pipeline);

                {
                    const object_to_clip = zm.scaling(demo.dimension.width / 2, demo.dimension.height / 2, 1.0);

                    const mem = gctx.uniformsAllocate(zm.Mat, 1);
                    mem.slice[0] = zm.transpose(object_to_clip);

                    pass.setBindGroup(0, bind_group, &.{mem.offset});
                    pass.drawIndexed(demo.vertex_count, @as(u32, @intCast(demo.pills.items.len)), 0, 0, 0);
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
            demo.dimension = calculateDimensions(gctx);

            // Release old depth texture.
            gctx.releaseResource(demo.depth_texture_view);
            gctx.destroyResource(demo.depth_texture);

            // Create a new depth texture to match the new window size.
            const depth = createDepthTexture(gctx);
            demo.depth_texture = depth.texture;
            demo.depth_texture_view = depth.view;
        }
    }
};

fn ensureFourByteMultiple(size: usize) usize {
    return (size + 3) & ~@as(usize, 3);
}

fn calculateDimensions(gctx: *zgpu.GraphicsContext) Dimension {
    const width = @as(f32, @floatFromInt(gctx.swapchain_descriptor.width));
    const height = @as(f32, @floatFromInt(gctx.swapchain_descriptor.height));
    const delta = math.sign(
        @as(i32, @bitCast(gctx.swapchain_descriptor.width)) - @as(i32, @bitCast(gctx.swapchain_descriptor.height)),
    );
    return switch (delta) {
        -1 => .{ .width = 2.0, .height = 2 * width / height },
        0 => .{ .width = 2.0, .height = 2.0 },
        1 => .{ .width = 2 * height / width, .height = 2.0 },
        else => unreachable,
    };
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
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(1600, 1000, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try DemoState.init(allocator, window);
    defer demo.deinit(allocator);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.draw();
    }
}
