const std = @import("std");
const assert = std.debug.assert;
const glfw = @import("glfw");
const c = @cImport({
    @cInclude("dawn/dawn_proc.h");
    @cInclude("dawn_native_mach.h");
});
const objc = @cImport({
    @cInclude("objc/message.h");
});
pub const cimgui = @cImport({
    @cDefine("CIMGUI_DEFINE_ENUMS_AND_STRUCTS", "");
    @cDefine("CIMGUI_NO_EXPORT", "");
    @cInclude("imgui/cimgui.h");
});
pub const gpu = @import("mach-gpu/main.zig");

pub const GraphicsContext = struct {
    native_instance: gpu.NativeInstance,
    adapter_type: gpu.Adapter.Type,
    backend_type: gpu.Adapter.BackendType,
    device: gpu.Device,
    queue: gpu.Queue,
    window: glfw.Window,
    window_surface: gpu.Surface,
    swapchain: gpu.SwapChain,
    swapchain_descriptor: gpu.SwapChain.Descriptor,
    buffer_pool: BufferPool,
    texture_pool: TexturePool,
    texture_view_pool: TextureViewPool,
    render_pipeline_pool: RenderPipelinePool,
    compute_pipeline_pool: ComputePipelinePool,

    pub const swapchain_format = gpu.Texture.Format.bgra8_unorm;
    // TODO: Adjust pool sizes.
    const buffer_pool_size = 256;
    const texture_pool_size = 256;
    const texture_view_pool_size = 256;
    const render_pipeline_pool_size = 128;
    const compute_pipeline_pool_size = 128;

    pub fn init(allocator: std.mem.Allocator, window: glfw.Window) GraphicsContext {
        c.dawnProcSetProcs(c.machDawnNativeGetProcs());
        const instance = c.machDawnNativeInstance_init();
        c.machDawnNativeInstance_discoverDefaultAdapters(instance);

        var native_instance = gpu.NativeInstance.wrap(c.machDawnNativeInstance_get(instance).?);

        const gpu_interface = native_instance.interface();
        const backend_adapter = switch (gpu_interface.waitForAdapter(&.{
            .power_preference = .high_performance,
        })) {
            .adapter => |v| v,
            .err => |err| {
                std.debug.print("[zgpu] Failed to get adapter: error={} {s}.\n", .{ err.code, err.message });
                std.process.exit(1);
            },
        };

        const props = backend_adapter.properties;
        std.debug.print("[zgpu] High-performance device has been selected:\n", .{});
        std.debug.print("[zgpu]   Name: {s}\n", .{props.name});
        std.debug.print("[zgpu]   Driver: {s}\n", .{props.driver_description});
        std.debug.print("[zgpu]   Adapter type: {s}\n", .{gpu.Adapter.typeName(props.adapter_type)});
        std.debug.print("[zgpu]   Backend type: {s}\n", .{gpu.Adapter.backendTypeName(props.backend_type)});

        const device = switch (backend_adapter.waitForDevice(&.{})) {
            .device => |v| v,
            .err => |err| {
                std.debug.print("[zgpu] Failed to get device: error={} {s}\n", .{ err.code, err.message });
                std.process.exit(1);
            },
        };
        device.setUncapturedErrorCallback(&printUnhandledErrorCallback);

        const window_surface = createSurfaceForWindow(
            &native_instance,
            window,
            comptime detectGLFWOptions(),
        );
        const framebuffer_size = window.getFramebufferSize() catch unreachable;

        const swapchain_descriptor = gpu.SwapChain.Descriptor{
            .label = "main window swap chain",
            .usage = .{ .render_attachment = true },
            .format = swapchain_format,
            .width = framebuffer_size.width,
            .height = framebuffer_size.height,
            .present_mode = .fifo,
            .implementation = 0,
        };
        const swapchain = device.nativeCreateSwapChain(
            window_surface,
            &swapchain_descriptor,
        );

        return GraphicsContext{
            .native_instance = native_instance,
            .adapter_type = props.adapter_type,
            .backend_type = props.backend_type,
            .device = device,
            .queue = device.getQueue(),
            .window = window,
            .window_surface = window_surface,
            .swapchain = swapchain,
            .swapchain_descriptor = swapchain_descriptor,
            .buffer_pool = BufferPool.init(allocator, buffer_pool_size),
            .texture_pool = TexturePool.init(allocator, texture_pool_size),
            .texture_view_pool = TextureViewPool.init(allocator, texture_view_pool_size),
            .render_pipeline_pool = RenderPipelinePool.init(allocator, render_pipeline_pool_size),
            .compute_pipeline_pool = ComputePipelinePool.init(allocator, compute_pipeline_pool_size),
        };
    }

    pub fn deinit(gctx: *GraphicsContext, allocator: std.mem.Allocator) void {
        // TODO: Make sure all GPU commands are completed.
        // TODO: How to release `native_instance`?
        gctx.buffer_pool.deinit(allocator);
        gctx.texture_view_pool.deinit(allocator);
        gctx.texture_pool.deinit(allocator);
        gctx.render_pipeline_pool.deinit(allocator);
        gctx.compute_pipeline_pool.deinit(allocator);
        gctx.window_surface.release();
        gctx.swapchain.release();
        gctx.queue.release();
        gctx.device.release();
        gctx.* = undefined;
    }

    pub fn update(gctx: *GraphicsContext) bool {
        const fb_size = gctx.window.getFramebufferSize() catch unreachable;
        if (gctx.swapchain_descriptor.width != fb_size.width or
            gctx.swapchain_descriptor.height != fb_size.height)
        {
            gctx.swapchain_descriptor.width = fb_size.width;
            gctx.swapchain_descriptor.height = fb_size.height;
            gctx.swapchain.release();
            gctx.swapchain = gctx.device.nativeCreateSwapChain(
                gctx.window_surface,
                &gctx.swapchain_descriptor,
            );

            std.debug.print(
                "[zgpu] Swap chain has been resized to: {d}x{d}\n",
                .{ gctx.swapchain_descriptor.width, gctx.swapchain_descriptor.height },
            );
            return false; // Swap chain has been resized.
        }
        return true;
    }

    pub fn createBuffer(gctx: *GraphicsContext, descriptor: gpu.Buffer.Descriptor) BufferHandle {
        const gpuobj = gctx.device.createBuffer(&descriptor);
        return gctx.buffer_pool.addResource(.{
            .gpuobj = gpuobj,
            .size = descriptor.size,
            .usage = descriptor.usage,
        });
    }

    pub fn destroyBuffer(gctx: GraphicsContext, handle: BufferHandle) void {
        gctx.buffer_pool.destroyResource(handle);
    }

    pub fn lookupBuffer(gctx: GraphicsContext, handle: BufferHandle) ?gpu.Buffer {
        if (gctx.isResourceValid(handle)) {
            return gctx.buffer_pool.resources[handle.index].gpuobj.?;
        }
        return null;
    }

    pub fn lookupBufferInfo(gctx: GraphicsContext, handle: BufferHandle) ?BufferInfo {
        if (gctx.isResourceValid(handle)) {
            return gctx.buffer_pool.resources[handle.index];
        }
        return null;
    }

    pub fn createTexture(gctx: *GraphicsContext, descriptor: gpu.Texture.Descriptor) TextureHandle {
        const gpuobj = gctx.device.createTexture(&descriptor);
        return gctx.texture_pool.addResource(.{
            .gpuobj = gpuobj,
            .usage = descriptor.usage,
            .dimension = descriptor.dimension,
            .size = descriptor.size,
            .format = descriptor.format,
            .mip_level_count = descriptor.mip_level_count,
            .sample_count = descriptor.sample_count,
        });
    }

    pub fn destroyTexture(gctx: GraphicsContext, handle: TextureHandle) void {
        gctx.texture_pool.destroyResource(handle);
    }

    pub fn lookupTexture(gctx: GraphicsContext, handle: TextureHandle) ?gpu.Texture {
        if (gctx.isResourceValid(handle)) {
            return gctx.texture_pool.resources[handle.index].gpuobj.?;
        }
        return null;
    }

    pub fn lookupTextureInfo(gctx: GraphicsContext, handle: TextureHandle) ?TextureInfo {
        if (gctx.isResourceValid(handle)) {
            return gctx.texture_pool.resources[handle.index];
        }
        return null;
    }

    pub fn createTextureView(
        gctx: *GraphicsContext,
        texture_handle: TextureHandle,
        descriptor: gpu.TextureView.Descriptor,
    ) TextureViewHandle {
        const texture = gctx.lookupTexture(texture_handle).?;
        const gpuobj = texture.createView(&descriptor);
        return gctx.texture_view_pool.addResource(.{
            .gpuobj = gpuobj,
            .format = descriptor.format,
            .dimension = descriptor.dimension,
            .base_mip_level = descriptor.base_mip_level,
            .base_array_layer = descriptor.base_array_layer,
            .array_layer_count = descriptor.array_layer_count,
            .aspect = descriptor.aspect,
            .parent_texture_handle = texture_handle,
        });
    }

    pub fn destroyTextureView(gctx: GraphicsContext, handle: TextureViewHandle) void {
        gctx.texture_view_pool.destroyResource(handle);
    }

    pub fn lookupTextureView(gctx: GraphicsContext, handle: TextureViewHandle) ?gpu.TextureView {
        if (gctx.isResourceValid(handle)) {
            return gctx.texture_view_pool.resources[handle.index].gpuobj.?;
        }
        return null;
    }

    pub fn lookupTextureViewInfo(gctx: GraphicsContext, handle: TextureViewHandle) ?TextureViewInfo {
        if (gctx.isResourceValid(handle)) {
            return gctx.texture_view_pool.resources[handle.index];
        }
        return null;
    }

    pub fn createRenderPipeline(
        gctx: *GraphicsContext,
        descriptor: gpu.RenderPipeline.Descriptor,
    ) RenderPipelineHandle {
        const gpuobj = gctx.device.createRenderPipeline(&descriptor);
        return gctx.render_pipeline_pool.addResource(.{
            .gpuobj = gpuobj,
        });
    }

    pub fn destroyRenderPipeline(gctx: GraphicsContext, handle: RenderPipelineHandle) void {
        gctx.render_pipeline_pool.destroyResource(handle);
    }

    pub fn lookupRenderPipeline(gctx: GraphicsContext, handle: RenderPipelineHandle) ?gpu.RenderPipeline {
        if (gctx.isResourceValid(handle)) {
            return gctx.render_pipeline_pool.resources[handle.index].gpuobj.?;
        }
        return null;
    }

    pub fn createComputePipeline(
        gctx: *GraphicsContext,
        descriptor: gpu.ComputePipeline.Descriptor,
    ) RenderPipelineHandle {
        const gpuobj = gctx.device.createComputePipeline(&descriptor);
        return gctx.compute_pipeline_pool.addResource(.{
            .gpuobj = gpuobj,
        });
    }

    pub fn destroyComputePipeline(gctx: GraphicsContext, handle: ComputePipelineHandle) void {
        gctx.compute_pipeline_pool.destroyResource(handle);
    }

    pub fn lookupComputePipeline(gctx: GraphicsContext, handle: ComputePipelineHandle) ?gpu.ComputePipeline {
        if (gctx.isResourceValid(handle)) {
            return gctx.compute_pipeline_pool.resources[handle.index].gpuobj.?;
        }
        return null;
    }

    pub fn isResourceValid(gctx: GraphicsContext, handle: anytype) bool {
        const T = @TypeOf(handle);
        switch (T) {
            BufferHandle => return gctx.buffer_pool.isHandleValid(handle),
            TextureHandle => return gctx.texture_pool.isHandleValid(handle),
            TextureViewHandle => {
                if (gctx.texture_view_pool.isHandleValid(handle)) {
                    const texture = gctx.texture_view_pool.resources[handle.index].parent_texture_handle;
                    return gctx.texture_pool.isHandleValid(texture);
                }
                return false;
            },
            RenderPipelineHandle => return gctx.render_pipeline_pool.isHandleValid(handle),
            ComputePipelineHandle => return gctx.compute_pipeline_pool.isHandleValid(handle),
            else => @compileError("[zgpu] GraphicsContext.isResourceValid() not implemented for " ++ @typeName(T)),
        }
    }
};

pub const BufferHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

pub const TextureHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

pub const TextureViewHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

pub const RenderPipelineHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

pub const ComputePipelineHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

pub const BufferInfo = struct {
    gpuobj: ?gpu.Buffer = null,
    size: usize = 0,
    usage: gpu.BufferUsage = .{},
};

pub const TextureInfo = struct {
    gpuobj: ?gpu.Texture = null,
    usage: gpu.Texture.Usage = .{},
    dimension: gpu.Texture.Dimension = .dimension_1d,
    size: gpu.Extent3D = .{ .width = 0 },
    format: gpu.Texture.Format = .none,
    mip_level_count: u32 = 0,
    sample_count: u32 = 0,
};

pub const TextureViewInfo = struct {
    gpuobj: ?gpu.TextureView = null,
    format: gpu.Texture.Format = .none,
    dimension: gpu.TextureView.Dimension = .dimension_none,
    base_mip_level: u32 = 0,
    base_array_layer: u32 = 0,
    array_layer_count: u32 = 0,
    aspect: gpu.Texture.Aspect = .all,
    parent_texture_handle: TextureHandle = .{},
};

const RenderPipelineInfo = struct {
    gpuobj: ?gpu.RenderPipeline = null,
};

const ComputePipelineInfo = struct {
    gpuobj: ?gpu.ComputePipeline = null,
};

// TODO: Complete it, use tagged unions.
pub const BindGroupInfo = struct {
    gpuobj: ?gpu.BindGroup,
    entries: [8]struct {
        buffer: ?BufferHandle = null,
        offset: u64 = 0,
        size: u64 = 0,
        //sampler: ?Sampler = null,
        texture_view: ?TextureViewHandle = null,
    } = .{},
};

const BufferPool = ResourcePool(BufferInfo, BufferHandle);
const TexturePool = ResourcePool(TextureInfo, TextureHandle);
const TextureViewPool = ResourcePool(TextureViewInfo, TextureViewHandle);
const RenderPipelinePool = ResourcePool(RenderPipelineInfo, RenderPipelineHandle);
const ComputePipelinePool = ResourcePool(ComputePipelineInfo, ComputePipelineHandle);

fn ResourcePool(comptime ResourceInfo: type, comptime ResourceHandle: type) type {
    return struct {
        const Self = @This();

        resources: []ResourceInfo,
        generations: []u16,
        start_slot_index: u32 = 0,

        fn init(allocator: std.mem.Allocator, capacity: u32) Self {
            var resources = allocator.alloc(ResourceInfo, capacity + 1) catch unreachable;
            for (resources) |*resource| resource.* = .{};

            var generations = allocator.alloc(u16, capacity + 1) catch unreachable;
            for (generations) |*gen| gen.* = 0;

            return .{
                .resources = resources,
                .generations = generations,
            };
        }

        fn deinit(pool: *Self, allocator: std.mem.Allocator) void {
            for (pool.resources) |resource| {
                if (resource.gpuobj) |gpuobj| {
                    if (@hasDecl(@TypeOf(gpuobj), "destroy")) {
                        gpuobj.destroy();
                    }
                    gpuobj.release();
                }
            }
            allocator.free(pool.resources);
            allocator.free(pool.generations);
            pool.* = undefined;
        }

        fn addResource(pool: *Self, resource: ResourceInfo) ResourceHandle {
            assert(resource.gpuobj != null);

            var index: u32 = 0;
            var found_slot_index: u32 = 0;
            while (index < pool.resources.len) : (index += 1) {
                const slot_index = (pool.start_slot_index + index) % @intCast(u32, pool.resources.len);
                if (slot_index == 0)
                    continue;
                if (pool.resources[slot_index].gpuobj == null) {
                    found_slot_index = slot_index;
                    break;
                }
            }
            assert(found_slot_index > 0 and found_slot_index < pool.resources.len);

            pool.start_slot_index = found_slot_index + 1;
            pool.resources[found_slot_index] = resource;
            return .{
                .index = @intCast(u16, found_slot_index),
                .generation = blk: {
                    pool.generations[found_slot_index] += 1;
                    break :blk pool.generations[found_slot_index];
                },
            };
        }

        fn destroyResource(pool: Self, handle: ResourceHandle) void {
            if (!pool.isHandleValid(handle))
                return;
            var resource_info = &pool.resources[handle.index];

            const gpuobj = resource_info.gpuobj.?;
            if (@hasDecl(@TypeOf(gpuobj), "destroy")) {
                gpuobj.destroy();
            }
            gpuobj.release();
            resource_info.* = .{};
        }

        fn isHandleValid(pool: Self, handle: ResourceHandle) bool {
            return handle.index > 0 and
                handle.index < pool.resources.len and
                handle.generation > 0 and
                handle.generation == pool.generations[handle.index] and
                pool.resources[handle.index].gpuobj != null;
        }
    };
}

pub fn checkContent(comptime content_dir: []const u8) !void {
    const local = struct {
        fn impl() !void {
            // Change directory to where an executable is located.
            {
                var exe_path_buffer: [1024]u8 = undefined;
                const exe_path = std.fs.selfExeDirPath(exe_path_buffer[0..]) catch "./";
                std.os.chdir(exe_path) catch {};
            }
            // Make sure font file is a valid data file and not just a Git LFS pointer.
            {
                const file = try std.fs.cwd().openFile(content_dir ++ "Roboto-Medium.ttf", .{});
                defer file.close();

                const size = @intCast(usize, try file.getEndPos());
                if (size <= 1024) {
                    return error.InvalidDataFiles;
                }
            }
        }
    };
    local.impl() catch |err| {
        std.debug.print(
            \\
            \\ERROR
            \\Invalid data files or missing content folder.
            \\Please install Git LFS (Large File Support) and run:
            \\git lfs install
            \\git pull
            \\
            \\For more info please see: https://git-lfs.github.com/
            \\
            \\
        ,
            .{},
        );
        return err;
    };
}

pub const FrameStats = struct {
    time: f64 = 0.0,
    delta_time: f32 = 0.0,
    fps: f32 = 0.0,
    average_cpu_time: f32 = 0.0,
    previous_time: f64 = 0.0,
    fps_refresh_time: f64 = 0.0,
    frame_counter: u64 = 0,
    frame_number: u64 = 0,

    pub fn update(self: *FrameStats, window: glfw.Window, window_name: []const u8) void {
        self.time = glfw.getTime();
        self.delta_time = @floatCast(f32, self.time - self.previous_time);
        self.previous_time = self.time;

        if ((self.time - self.fps_refresh_time) >= 1.0) {
            const t = self.time - self.fps_refresh_time;
            const fps = @intToFloat(f64, self.frame_counter) / t;
            const ms = (1.0 / fps) * 1000.0;

            self.fps = @floatCast(f32, fps);
            self.average_cpu_time = @floatCast(f32, ms);
            self.fps_refresh_time = self.time;
            self.frame_counter = 0;

            var buffer = [_]u8{0} ** 128;
            const text = std.fmt.bufPrint(
                buffer[0..],
                "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                .{ self.fps, self.average_cpu_time, window_name },
            ) catch unreachable;
            window.setTitle(@ptrCast([*:0]const u8, text.ptr)) catch unreachable;
        }
        self.frame_counter += 1;
        self.frame_number += 1;
    }
};

pub const gui = struct {
    pub fn init(window: glfw.Window, device: gpu.Device, font: [*:0]const u8, font_size: f32) void {
        assert(cimgui.igGetCurrentContext() == null);
        _ = cimgui.igCreateContext(null);

        if (!ImGui_ImplGlfw_InitForOther(window.handle, true)) {
            unreachable;
        }

        const io = cimgui.igGetIO().?;
        if (cimgui.ImFontAtlas_AddFontFromFileTTF(io.*.Fonts, font, font_size, null, null) == null) {
            unreachable;
        }

        if (!ImGui_ImplWGPU_Init(device.ptr, 2, @enumToInt(GraphicsContext.swapchain_format))) {
            unreachable;
        }
    }

    pub fn deinit() void {
        assert(cimgui.igGetCurrentContext() != null);
        ImGui_ImplWGPU_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        cimgui.igDestroyContext(null);
    }

    pub fn newFrame(fb_width: u32, fb_height: u32) void {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplWGPU_NewFrame();
        {
            const io = cimgui.igGetIO().?;
            io.*.DisplaySize = .{
                .x = @intToFloat(f32, fb_width),
                .y = @intToFloat(f32, fb_height),
            };
            io.*.DisplayFramebufferScale = .{ .x = 1.0, .y = 1.0 };
        }
        cimgui.igNewFrame();
    }

    pub fn draw(pass: gpu.RenderPassEncoder) void {
        cimgui.igRender();
        ImGui_ImplWGPU_RenderDrawData(cimgui.igGetDrawData(), pass.ptr);
    }

    extern fn ImGui_ImplGlfw_InitForOther(window: *anyopaque, install_callbacks: bool) bool;
    extern fn ImGui_ImplGlfw_NewFrame() void;
    extern fn ImGui_ImplGlfw_Shutdown() void;
    extern fn ImGui_ImplWGPU_Init(device: *anyopaque, num_frames_in_flight: u32, rt_format: u32) bool;
    extern fn ImGui_ImplWGPU_NewFrame() void;
    extern fn ImGui_ImplWGPU_RenderDrawData(draw_data: *anyopaque, pass_encoder: *anyopaque) void;
    extern fn ImGui_ImplWGPU_Shutdown() void;
};

fn detectGLFWOptions() glfw.BackendOptions {
    const target = @import("builtin").target;
    if (target.isDarwin()) return .{ .cocoa = true };
    return switch (target.os.tag) {
        .windows => .{ .win32 = true },
        .linux => .{ .x11 = true },
        else => .{},
    };
}

fn createSurfaceForWindow(
    native_instance: *const gpu.NativeInstance,
    window: glfw.Window,
    comptime glfw_options: glfw.BackendOptions,
) gpu.Surface {
    const glfw_native = glfw.Native(glfw_options);
    const descriptor = if (glfw_options.win32) gpu.Surface.Descriptor{
        .windows_hwnd = .{
            .label = "basic surface",
            .hinstance = std.os.windows.kernel32.GetModuleHandleW(null).?,
            .hwnd = glfw_native.getWin32Window(window),
        },
    } else if (glfw_options.x11) gpu.Surface.Descriptor{
        .xlib = .{
            .label = "basic surface",
            .display = glfw_native.getX11Display(),
            .window = glfw_native.getX11Window(window),
        },
    } else if (glfw_options.cocoa) blk: {
        const ns_window = glfw_native.getCocoaWindow(window);
        const ns_view = msgSend(ns_window, "contentView", .{}, *anyopaque); // [nsWindow contentView]

        // Create a CAMetalLayer that covers the whole window that will be passed to CreateSurface.
        msgSend(ns_view, "setWantsLayer:", .{true}, void); // [view setWantsLayer:YES]
        const layer = msgSend(objc.objc_getClass("CAMetalLayer"), "layer", .{}, ?*anyopaque); // [CAMetalLayer layer]
        if (layer == null) @panic("failed to create Metal layer");
        msgSend(ns_view, "setLayer:", .{layer.?}, void); // [view setLayer:layer]

        // Use retina if the window was created with retina support.
        const scale_factor = msgSend(ns_window, "backingScaleFactor", .{}, f64); // [ns_window backingScaleFactor]
        msgSend(layer.?, "setContentsScale:", .{scale_factor}, void); // [layer setContentsScale:scale_factor]

        break :blk gpu.Surface.Descriptor{
            .metal_layer = .{
                .label = "basic surface",
                .layer = layer.?,
            },
        };
    } else if (glfw_options.wayland) {
        // bugs.chromium.org/p/dawn/issues/detail?id=1246&q=surface&can=2
        @panic("Dawn does not yet have Wayland support");
    } else unreachable;

    return native_instance.createSurface(&descriptor);
}

// Borrowed from https://github.com/hazeycode/zig-objcrt
fn msgSend(obj: anytype, sel_name: [:0]const u8, args: anytype, comptime ReturnType: type) ReturnType {
    const args_meta = @typeInfo(@TypeOf(args)).Struct.fields;

    const FnType = switch (args_meta.len) {
        0 => fn (@TypeOf(obj), objc.SEL) callconv(.C) ReturnType,
        1 => fn (@TypeOf(obj), objc.SEL, args_meta[0].field_type) callconv(.C) ReturnType,
        2 => fn (@TypeOf(obj), objc.SEL, args_meta[0].field_type, args_meta[1].field_type) callconv(.C) ReturnType,
        3 => fn (
            @TypeOf(obj),
            objc.SEL,
            args_meta[0].field_type,
            args_meta[1].field_type,
            args_meta[2].field_type,
        ) callconv(.C) ReturnType,
        4 => fn (
            @TypeOf(obj),
            objc.SEL,
            args_meta[0].field_type,
            args_meta[1].field_type,
            args_meta[2].field_type,
            args_meta[3].field_type,
        ) callconv(.C) ReturnType,
        else => @compileError("Unsupported number of args"),
    };

    // NOTE: `func` is a var because making it const causes a compile error which I believe is a compiler bug.
    var func = @ptrCast(FnType, objc.objc_msgSend);
    const sel = objc.sel_getUid(sel_name);

    return @call(.{}, func, .{ obj, sel } ++ args);
}

fn printUnhandledError(_: void, typ: gpu.ErrorType, message: [*:0]const u8) void {
    switch (typ) {
        .validation => std.debug.print("[zgpu] Validation error: {s}\n", .{message}),
        .out_of_memory => std.debug.print("[zgpu] Out of memory: {s}\n", .{message}),
        .device_lost => std.debug.print("[zgpu] Device lost: {s}\n", .{message}),
        .unknown => std.debug.print("[zgpu] Unknown error: {s}\n", .{message}),
        else => unreachable,
    }
    std.process.exit(1);
}
var printUnhandledErrorCallback = gpu.ErrorCallback.init(void, {}, printUnhandledError);
