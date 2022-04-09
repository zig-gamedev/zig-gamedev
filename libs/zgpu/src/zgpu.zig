const std = @import("std");
const glfw = @import("glfw");
const zgpu = @import("mach-gpu/main.zig");
const c = @cImport({
    @cInclude("dawn/dawn_proc.h");
    @cInclude("dawn_native_mach.h");
});
const objc = @cImport({
    @cInclude("objc/message.h");
});

pub usingnamespace zgpu;

pub const GraphicsContext = struct {
    native_instance: zgpu.NativeInstance,
    adapter_type: zgpu.Adapter.Type,
    backend_type: zgpu.Adapter.BackendType,
    device: zgpu.Device,
    queue: zgpu.Queue,
    window_surface: zgpu.Surface,
    swap_chain: ?zgpu.SwapChain,
    swap_chain_format: zgpu.Texture.Format,
    swap_chain_descriptor_current: zgpu.SwapChain.Descriptor,
    swap_chain_descriptor_target: zgpu.SwapChain.Descriptor,

    pub fn create(allocator: std.mem.Allocator, window: glfw.Window) *GraphicsContext {
        c.dawnProcSetProcs(c.machDawnNativeGetProcs());
        const instance = c.machDawnNativeInstance_init();
        c.machDawnNativeInstance_discoverDefaultAdapters(instance);

        var native_instance = zgpu.NativeInstance.wrap(c.machDawnNativeInstance_get(instance).?);

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
        std.debug.print("[zgpu]   Adapter type: {s}\n", .{zgpu.Adapter.typeName(props.adapter_type)});
        std.debug.print("[zgpu]   Backend type: {s}\n", .{zgpu.Adapter.backendTypeName(props.backend_type)});

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
        const swap_chain_format = .bgra8_unorm;
        const swap_chain_descriptor = zgpu.SwapChain.Descriptor{
            .label = "main window swap chain",
            .usage = .{ .render_attachment = true },
            .format = swap_chain_format,
            .width = framebuffer_size.width,
            .height = framebuffer_size.height,
            .present_mode = .fifo,
            .implementation = 0,
        };

        const gctx = allocator.create(GraphicsContext) catch unreachable;
        gctx.* = .{
            .native_instance = native_instance,
            .adapter_type = props.adapter_type,
            .backend_type = props.backend_type,
            .device = device,
            .queue = device.getQueue(),
            .window_surface = window_surface,
            .swap_chain = null,
            .swap_chain_format = swap_chain_format,
            .swap_chain_descriptor_current = swap_chain_descriptor,
            .swap_chain_descriptor_target = swap_chain_descriptor,
        };
        window.setUserPointer(gctx);
        window.setFramebufferSizeCallback((struct {
            fn callback(win: glfw.Window, win_width: u32, win_height: u32) void {
                const ctx = win.getUserPointer(GraphicsContext);
                ctx.?.swap_chain_descriptor_target.width = win_width;
                ctx.?.swap_chain_descriptor_target.height = win_height;
            }
        }).callback);
        return gctx;
    }

    pub fn destroy(gctx: *GraphicsContext, allocator: std.mem.Allocator) void {
        // TODO: make sure all GPU commands are completed
        // TODO: how to release `native_instance`?
        gctx.window_surface.release();
        gctx.swap_chain.release();
        gctx.queue.release();
        gctx.device.release();
        allocator.destroy(gctx);
    }

    pub fn update(gctx: *GraphicsContext) void {
        if (gctx.swap_chain == null or
            !gctx.swap_chain_descriptor_current.equal(&gctx.swap_chain_descriptor_target))
        {
            gctx.swap_chain = gctx.device.nativeCreateSwapChain(
                gctx.window_surface,
                &gctx.swap_chain_descriptor_target,
            );
            gctx.swap_chain_descriptor_current = gctx.swap_chain_descriptor_target;

            std.debug.print(
                "[zgpu] Swap chain has been resized to: {d}x{d}\n",
                .{ gctx.swap_chain_descriptor_current.width, gctx.swap_chain_descriptor_current.height },
            );
        }
    }
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
    native_instance: *const zgpu.NativeInstance,
    window: glfw.Window,
    comptime glfw_options: glfw.BackendOptions,
) zgpu.Surface {
    const glfw_native = glfw.Native(glfw_options);
    const descriptor = if (glfw_options.win32) zgpu.Surface.Descriptor{
        .windows_hwnd = .{
            .label = "basic surface",
            .hinstance = std.os.windows.kernel32.GetModuleHandleW(null).?,
            .hwnd = glfw_native.getWin32Window(window),
        },
    } else if (glfw_options.x11) zgpu.Surface.Descriptor{
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

        break :blk zgpu.Surface.Descriptor{
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

    // NOTE: func is a var because making it const causes a compile error which I believe is a compiler bug
    var func = @ptrCast(FnType, objc.objc_msgSend);
    const sel = objc.sel_getUid(sel_name);

    return @call(.{}, func, .{ obj, sel } ++ args);
}

fn printUnhandledError(_: void, typ: zgpu.ErrorType, message: [*:0]const u8) void {
    switch (typ) {
        .validation => std.debug.print("[zgpu] Validation error: {s}\n", .{message}),
        .out_of_memory => std.debug.print("[zgpu] Out of memory: {s}\n", .{message}),
        .device_lost => std.debug.print("[zgpu] Device lost: {s}\n", .{message}),
        .unknown => std.debug.print("[zgpu] Unknown error: {s}\n", .{message}),
        else => unreachable,
    }
    std.process.exit(1);
}
var printUnhandledErrorCallback = zgpu.ErrorCallback.init(void, {}, printUnhandledError);
