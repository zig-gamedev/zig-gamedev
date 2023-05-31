const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");

const build_options = @import("build_options");
const content_dir = build_options.content_dir;
const emscripten = build_options.emscripten;
const window_title = "zig-gamedev: triangle (wgpu)";

pub const std_options = struct {
    // pub const log_level = .info;
    pub const logFn = if (emscripten) emscriptenLog else std.log.defaultLog;
};

// zig fmt: off
const wgsl_vs =
\\  @group(0) @binding(0) var<uniform> object_to_clip: mat4x4<f32>;
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) color: vec3<f32>,
\\  }
\\  @vertex fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) color: vec3<f32>,
\\  ) -> VertexOut {
\\      var output: VertexOut;
\\      output.position_clip = vec4(position, 1.0) * object_to_clip;
\\      output.color = color;
\\      return output;
\\  }
;
const wgsl_fs =
\\  @fragment fn main(
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

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    pipeline: zgpu.RenderPipelineHandle,
    bind_group: zgpu.BindGroupHandle,

    vertex_buffer: zgpu.BufferHandle,
    index_buffer: zgpu.BufferHandle,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,
};

fn init(allocator: std.mem.Allocator, window: *zglfw.Window) !DemoState {
    const gctx = try zgpu.GraphicsContext.create(allocator, window);

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
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @offsetOf(Vertex, "color"), .shader_location = 1 },
        };
        const vertex_buffers = [_]wgpu.VertexBufferLayout{.{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        }};

        const pipeline_descriptor = wgpu.RenderPipelineDescriptor{
            .vertex = wgpu.VertexState{
                .module = vs_module,
                .entry_point = "main",
                .buffer_count = vertex_buffers.len,
                .buffers = &vertex_buffers,
            },
            .primitive = wgpu.PrimitiveState{
                .front_face = .ccw,
                .cull_mode = .none,
                .topology = .triangle_list,
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
        .{ .binding = 0, .buffer_handle = gctx.uniforms.buffer, .offset = 0, .size = @sizeOf(zm.Mat) },
    });

    // Create a vertex buffer.
    const vertex_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = 3 * @sizeOf(Vertex),
    });
    const vertex_data = [_]Vertex{
        .{ .position = [3]f32{ 0.0, 0.5, 0.0 }, .color = [3]f32{ 1.0, 0.0, 0.0 } },
        .{ .position = [3]f32{ -0.5, -0.5, 0.0 }, .color = [3]f32{ 0.0, 1.0, 0.0 } },
        .{ .position = [3]f32{ 0.5, -0.5, 0.0 }, .color = [3]f32{ 0.0, 0.0, 1.0 } },
    };
    gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, vertex_data[0..]);

    // Create an index buffer.
    const index_buffer = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = 3 * @sizeOf(u32),
    });
    const index_data = [_]u32{ 0, 1, 2 };
    gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u32, index_data[0..]);

    // Create a depth texture and its 'view'.
    const depth = createDepthTexture(gctx);

    return DemoState{
        .gctx = gctx,
        .pipeline = pipeline,
        .bind_group = bind_group,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .depth_texture = depth.texture,
        .depth_texture_view = depth.view,
    };
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.gctx.destroy(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );
    zgui.showDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;
    const t = @floatCast(f32, gctx.stats.time);

    if (!gctx.canRender()) {
        std.log.err("Can't render out of buffers!", .{});
    }

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

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        pass: {
            const vb_info = gctx.lookupResourceInfo(demo.vertex_buffer) orelse break :pass;
            const ib_info = gctx.lookupResourceInfo(demo.index_buffer) orelse break :pass;
            const pipeline = gctx.lookupResource(demo.pipeline) orelse break :pass;
            const bind_group = gctx.lookupResource(demo.bind_group) orelse break :pass;
            const depth_view = gctx.lookupResource(demo.depth_texture_view) orelse break :pass;

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
            pass.setIndexBuffer(ib_info.gpuobj.?, .uint32, 0, ib_info.size);

            pass.setPipeline(pipeline);

            // Draw triangle 1.
            {
                const object_to_world = zm.mul(zm.rotationY(t), zm.translation(-1.0, 0.0, 0.0));
                const object_to_clip = zm.mul(object_to_world, cam_world_to_clip);

                const mem = gctx.uniformsAllocate(zm.Mat, 1);
                mem.slice[0] = zm.transpose(object_to_clip);

                pass.setBindGroup(0, bind_group, &.{mem.offset});
                pass.drawIndexed(3, 1, 0, 0, 0);
            }

            // Draw triangle 2.
            {
                const object_to_world = zm.mul(zm.rotationY(0.75 * t), zm.translation(1.0, 0.0, 0.0));
                const object_to_clip = zm.mul(object_to_world, cam_world_to_clip);

                const mem = gctx.uniformsAllocate(zm.Mat, 1);
                mem.slice[0] = zm.transpose(object_to_clip);

                pass.setBindGroup(0, bind_group, &.{mem.offset});
                pass.drawIndexed(3, 1, 0, 0, 0);
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
        std.log.info("resize framebuffer", .{});
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

pub const GPA = if (emscripten) EmmalocAllocator else std.heap.GeneralPurposeAllocator(.{});
pub const MainState = struct {
    is_init: bool = false, // main fully initialized
    window: *zglfw.Window = undefined,
    gpa: GPA = undefined,
    demo: DemoState = undefined,
};
pub var main_state: MainState = .{};

// still main should cleans up with errdefer, but if all goes well this cleans up successfully init state
pub fn mainDeinit() void {
    if (!main_state.is_init) return;
    const allocator = main_state.gpa.allocator();
    zgui.backend.deinit();
    zgui.deinit();
    deinit(allocator, &main_state.demo);
    main_state.window.destroy();
    zglfw.terminate();
    _ = main_state.gpa.deinit();
}

pub fn main() !void {
    defer if (!emscripten) mainDeinit();

    zglfw.init() catch {
        std.log.err("Failed to initialize GLFW library.", .{});
        return;
    };
    errdefer zglfw.terminate();

    // Change current working directory to where the executable is located.
    if (!emscripten) {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    if (emscripten) {
        // by default emscripten initializes on window creation WebGL context
        // this flag skips context creation. otherwise we later can't create webgpu surface
        zglfw.WindowHint.set(.client_api, @enumToInt(zglfw.ClientApi.no_api));
    }
    const window = zglfw.Window.create(1600, 1000, window_title, null) catch |err| {
        std.log.err("Failed to create demo window. {}", .{err});
        return;
    };
    errdefer window.destroy();
    main_state.window = window;
    window.setSizeLimits(400, 400, -1, -1);

    main_state.gpa = GPA{};
    const gpa = &main_state.gpa;
    errdefer _ = if (!emscripten) gpa.deinit();

    const allocator = gpa.allocator();

    main_state.demo = init(allocator, window) catch |err| {
        std.log.err("Failed to initialize the demo. {}", .{err});
        return;
    };
    errdefer deinit(allocator, &main_state.demo);
    const demo = &main_state.demo;

    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale[0], scale[1]);
    };

    zgui.init(allocator);
    errdefer zgui.deinit();

    if (emscripten) {
        zgui.io.setIniFilename(null);
        // todo: font - embed and load from wasm memory?
    }
    //_ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(16.0 * scale_factor));

    zgui.backend.init(
        window,
        demo.gctx.device,
        @enumToInt(zgpu.GraphicsContext.swapchain_format),
    );
    errdefer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    main_state.is_init = true;
    if (!emscripten) {
        while (!window.shouldClose() and window.getKey(.escape) != .press) {
            tick();
        }
    } else {
        const id = emscripten_request_animation_frame_loop(&tickCB, null);
        _ = id;
    }
}

pub fn tick() void {
    if (!main_state.demo.gctx.canRender()) {
        std.log.err("can't render!", .{});
        return;
    }
    zglfw.pollEvents();
    update(&main_state.demo);
    draw(&main_state.demo);
}

export fn tickCB(time: f64, user_data: ?*anyopaque) EmBool {
    _ = user_data;
    _ = time;
    tick();
    return .true; // return false to stop the loop
}

///
///     Emscripten stuff
///
pub fn emscriptenLog(
    comptime level: std.log.Level,
    comptime scope: @TypeOf(.EnumLiteral),
    comptime format: []const u8,
    args: anytype,
) void {
    const level_txt = comptime level.asText();
    const prefix2 = if (scope == .default) ": " else "(" ++ @tagName(scope) ++ "): ";
    const prefix = level_txt ++ prefix2;

    var buf: [1024 * 8]u8 = undefined;
    var slice = std.fmt.bufPrint(buf[0 .. buf.len - 1], prefix ++ format, args) catch {
        emscripten_console_error("emscriptenLog: formatting message failed - log message skipped!");
        return;
    };
    buf[slice.len] = 0;
    switch (level) {
        .err => emscripten_console_error(@ptrCast([*:0]u8, slice.ptr)),
        .warn => emscripten_console_warn(@ptrCast([*:0]u8, slice.ptr)),
        else => emscripten_console_log(@ptrCast([*:0]u8, slice.ptr)),
    }
}

pub const EmBool = enum(u32) {
    true = 1,
    false = 0,
};
// https://emscripten.org/docs/api_reference/html5.h.html#c.emscripten_request_animation_frame_loop
pub const AnimationFrameCallback = *const fn (time: f64, user_data: ?*anyopaque) callconv(.C) EmBool;
extern fn emscripten_request_animation_frame(cb: AnimationFrameCallback, user_data: ?*anyopaque) c_long;
extern fn emscripten_cancel_animation_frame(requestAnimationFrameId: c_long) void;
extern fn emscripten_request_animation_frame_loop(cb: AnimationFrameCallback, user_data: ?*anyopaque) void;
extern fn emscripten_console_log(utf8_string: [*:0]const u8) void;
extern fn emscripten_console_warn(utf8_string: [*:0]const u8) void;
extern fn emscripten_console_error(utf8_string: [*:0]const u8) void;
extern fn emscripten_sleep(ms: u32) void;

/// EmmalocAllocator allocator
/// use with linker flag -sMALLOC=emmalloc
/// for details see docs: https://github.com/emscripten-core/emscripten/blob/main/system/lib/emmalloc.c
extern fn emmalloc_memalign(alignment: usize, size: usize) ?[*]u8;
extern fn emmalloc_realloc_try(ptr: ?[*]u8, size: usize) ?[*]u8;
extern fn emmalloc_free(ptr: ?[*]u8) void;
pub const EmmalocAllocator = struct {
    const Self = @This();
    dummy: u32 = undefined,

    pub fn allocator(self: *Self) std.mem.Allocator {
        return .{
            .ptr = self,
            .vtable = &.{
                .alloc = &alloc,
                .resize = &resize,
                .free = &free,
            },
        };
    }

    fn alloc(
        ctx: *anyopaque,
        len: usize,
        ptr_align_log2: u8,
        return_address: usize,
    ) ?[*]u8 {
        _ = ctx;
        _ = return_address;
        const ptr_align: u32 = @intCast(u32, 1) << @intCast(u5, ptr_align_log2);
        if (!std.math.isPowerOfTwo(ptr_align)) unreachable;
        const ptr = emmalloc_memalign(ptr_align, len) orelse return null;
        return @ptrCast([*]u8, ptr);
    }

    fn resize(
        ctx: *anyopaque,
        buf: []u8,
        buf_align_log2: u8,
        new_len: usize,
        return_address: usize,
    ) bool {
        _ = ctx;
        _ = return_address;
        _ = buf_align_log2;
        return emmalloc_realloc_try(buf.ptr, new_len) != null;
    }

    fn free(
        ctx: *anyopaque,
        buf: []u8,
        buf_align_log2: u8,
        return_address: usize,
    ) void {
        _ = ctx;
        _ = buf_align_log2;
        _ = return_address;
        return emmalloc_free(buf.ptr);
    }
};

usingnamespace if (@import("builtin").cpu.arch == .wasm32) struct {
    // GLFW - emscripten uses older version that doesn't have these functions - implement dummies
    /// use glfwSetCallback instead
    pub export fn glfwGetError() i32 {
        return 0; // no error
    }

    pub export fn glfwGetGamepadState(_: i32, _: ?*anyopaque) i32 {
        return 0; // false - failure
    }

    pub export fn wgpuDeviceTick() void {
        std.log.warn("use of device.tick() should be avoided! It can break if used with callbacks such as requestAnimationFrame etc.", .{});
        emscripten_sleep(1); // requires -sASYNCIFY
    }
} else struct {};
