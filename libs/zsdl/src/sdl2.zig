const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

//--------------------------------------------------------------------------------------------------
//
// Initialzation and Shutdown
//
//--------------------------------------------------------------------------------------------------
pub const InitFlags = packed struct(u32) {
    timer: bool = false,
    __unused1: bool = false,
    __unused2: bool = false,
    __unused3: bool = false,
    audio: bool = false,
    video: bool = false,
    __unused6: bool = false,
    __unused7: bool = false,
    __unused8: bool = false,
    joystick: bool = false,
    __unused10: bool = false,
    __unused11: bool = false,
    haptic: bool = false,
    gamecontroller: bool = false,
    events: bool = false,
    sensor: bool = false,
    __unused16: bool = false,
    __unused17: bool = false,
    __unused18: bool = false,
    __unused19: bool = false,
    noparachute: bool = false,
    __unused: u11 = 0,

    pub const everything: InitFlags = .{
        .timer = true,
        .audio = true,
        .video = true,
        .events = true,
        .joystick = true,
        .haptic = true,
        .gamecontroller = true,
        .sensor = true,
    };
};

pub fn init(flags: InitFlags) Error!void {
    if (SDL_Init(flags) < 0) return makeError();
}
extern fn SDL_Init(flags: InitFlags) i32;

/// `pub fn quit() void`
pub const quit = SDL_Quit;
extern fn SDL_Quit() void;

//--------------------------------------------------------------------------------------------------
//
// Configuration Variables
//
//--------------------------------------------------------------------------------------------------
pub const hint_windows_dpi_awareness = "SDL_WINDOWS_DPI_AWARENESS";

pub fn setHint(name: [:0]const u8, value: [:0]const u8) bool {
    return SDL_SetHint(name, value) != 0;
}
extern fn SDL_SetHint(name: [*:0]const u8, value: [*:0]const u8) i32;

//--------------------------------------------------------------------------------------------------
//
// Error Handling
//
//--------------------------------------------------------------------------------------------------
pub fn getError() ?[:0]const u8 {
    if (SDL_GetError()) |ptr| {
        return std.mem.sliceTo(ptr, 0);
    }
    return null;
}
extern fn SDL_GetError() ?[*:0]const u8;

pub const Error = error{SdlError};

pub fn makeError() error{SdlError} {
    if (getError()) |str| {
        std.log.debug("SDL2: {s}", .{str});
    }
    return error.SdlError;
}

//--------------------------------------------------------------------------------------------------
//
// Log Handling
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Assertions
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Querying SDL Version
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Display and Window management
//
//--------------------------------------------------------------------------------------------------
pub const DisplayId = u32;

pub const WindowId = u32;

pub const DisplayMode = extern struct {
    format: u32,
    w: i32,
    h: i32,
    refresh_rate: i32,
    driverdata: ?*anyopaque,
};

pub const Window = opaque {
    pub const Flags = packed struct(u32) {
        fullscreen: bool = false,
        opengl: bool = false,
        shown: bool = false,
        hidden: bool = false,
        borderless: bool = false, // 0x10
        resizable: bool = false,
        minimized: bool = false,
        maximized: bool = false,
        mouse_grabbed: bool = false, // 0x100
        input_focus: bool = false,
        mouse_focus: bool = false,
        foreign: bool = false,
        _desktop: bool = false, // 0x1000
        allow_highdpi: bool = false,
        mouse_capture: bool = false,
        always_on_top: bool = false,
        skip_taskbar: bool = false, // 0x10000
        utility: bool = false,
        tooltip: bool = false,
        popup_menu: bool = false,
        keyboard_grabbed: bool = false,

        __unused21: bool = false,
        __unused22: bool = false,
        __unused23: bool = false,
        __unused24: bool = false,

        vulkan: bool = false,
        metal: bool = false,

        __unused: u5 = 0,

        pub const fullscreen_desktop: Flags = .{ .fullscreen = true, ._desktop = true };
        pub const input_grabbed: Flags = .{ .mouse_grabbed = true };
    };

    pub const pos_undefined = posUndefinedDisplay(0);
    pub const pos_centered = posCenteredDisplay(0);

    pub fn posUndefinedDisplay(x: i32) i32 {
        return pos_undefined_mask | x;
    }
    pub fn posCenteredDisplay(x: i32) i32 {
        return pos_centered_mask | x;
    }

    const pos_undefined_mask: i32 = 0x1fff_0000;
    const pos_centered_mask: i32 = 0x2fff_0000;

    pub fn create(title: [:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Flags) Error!*Window {
        return SDL_CreateWindow(title, x, y, w, h, flags) orelse return makeError();
    }
    extern fn SDL_CreateWindow(title: ?[*:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Flags) ?*Window;

    /// `pub fn destroy(window: *Window) void`
    pub const destroy = SDL_DestroyWindow;
    extern fn SDL_DestroyWindow(window: *Window) void;

    pub fn getDisplayMode(window: *Window) Error!DisplayMode {
        var mode: DisplayMode = undefined;
        if (SDL_GetWindowDisplayMode(window, &mode) < 0) return makeError();
        return mode;
    }
    extern fn SDL_GetWindowDisplayMode(window: *Window, mode: *DisplayMode) i32;

    pub fn getPosition(window: *Window, w: ?*i32, h: ?*i32) Error!void {
        if (SDL_GetWindowPosition(window, w, h) < 0) return makeError();
    }
    extern fn SDL_GetWindowPosition(window: *Window, x: ?*i32, y: ?*i32) i32;

    pub fn getSize(window: *Window, w: ?*i32, h: ?*i32) Error!void {
        if (SDL_GetWindowSize(window, w, h) < 0) return makeError();
    }
    extern fn SDL_GetWindowSize(window: *Window, w: ?*i32, h: ?*i32) i32;

    pub fn setTitle(window: *Window, title: [:0]const u8) void {
        SDL_SetWindowTitle(window, title);
    }
    extern fn SDL_SetWindowTitle(window: *Window, title: ?[*:0]const u8) void;
};

/// `pub fn getNumVideoDrivers() i32`
pub const getNumVideoDrivers = SDL_GetNumVideoDrivers;
extern fn SDL_GetNumVideoDrivers() i32;

pub fn getVideoDriver(index: i32) ?[:0]const u8 {
    if (SDL_GetVideoDriver(index)) |ptr| {
        return std.mem.sliceTo(ptr, 0);
    }
    return null;
}
extern fn SDL_GetVideoDriver(index: i32) ?[*:0]const u8;

pub const gl = struct {
    pub const Context = *anyopaque;

    pub const Attr = enum(i32) {
        red_size,
        green_size,
        blue_size,
        alpha_size,
        buffer_size,
        doublebuffer,
        depth_size,
        stencil_size,
        accum_red_size,
        accum_green_size,
        accum_blue_size,
        accum_alpha_size,
        stereo,
        multisamplebuffers,
        multisamplesamples,
        accelerated_visual,
        retained_backing,
        context_major_version,
        context_minor_version,
        context_egl,
        context_flags,
        context_profile_mask,
        share_with_current_context,
        framebuffer_srgb_capable,
        context_release_behavior,
        context_reset_notification,
        context_no_error,
        floatbuffers,
    };

    pub const Profile = enum(i32) {
        core = 0x0001,
        compatibility = 0x0002,
        es = 0x0004,
    };

    pub const ContextFlags = packed struct(i32) {
        debug: bool = false,
        forward_compatible: bool = false,
        robust_access: bool = false,
        reset_isolation: bool = false,
        __unused: i28 = 0,
    };

    pub const ContextReleaseFlags = packed struct(i32) {
        flush: bool = false,
        __unused: i31 = 0,
    };

    pub const ContextResetNotification = enum(i32) {
        no_notification = 0x0000,
        lose_context = 0x0001,
    };

    pub fn setAttribute(attr: Attr, value: i32) Error!void {
        if (SDL_GL_SetAttribute(attr, value) < 0) return makeError();
    }
    extern fn SDL_GL_SetAttribute(attr: Attr, value: i32) i32;

    pub fn getAttribute(attr: Attr) Error!i32 {
        var value: i32 = undefined;
        if (SDL_GL_GetAttribute(attr, &value) < 0) return makeError();
        return value;
    }
    extern fn SDL_GL_GetAttribute(attr: Attr, value: i32) i32;

    pub fn setSwapInterval(interval: i32) Error!void {
        if (SDL_GL_SetSwapInterval(interval) < 0) return makeError();
    }
    extern fn SDL_GL_SetSwapInterval(interval: i32) i32;

    /// `pub fn getSwapInterval() i32`
    pub const getSwapInterval = SDL_GL_GetSwapInterval;
    extern fn SDL_GL_GetSwapInterval() i32;

    /// `pub fn swapWindow(window: *Window) void`
    pub const swapWindow = SDL_GL_SwapWindow;
    extern fn SDL_GL_SwapWindow(window: *Window) void;

    pub fn getProcAddress(proc: [:0]const u8) ?*anyopaque {
        return SDL_GL_GetProcAddress(proc);
    }
    extern fn SDL_GL_GetProcAddress(proc: ?[*:0]const u8) ?*anyopaque;

    pub fn isExtensionSupported(extension: [:0]const u8) bool {
        return SDL_GL_ExtensionSupported(extension) != 0;
    }
    extern fn SDL_GL_ExtensionSupported(extension: ?[*:0]const u8) i32;

    pub fn createContext(window: *Window) Error!Context {
        return SDL_GL_CreateContext(window) orelse return makeError();
    }
    extern fn SDL_GL_CreateContext(window: *Window) ?Context;

    pub fn makeCurrent(window: *Window, context: Context) Error!void {
        if (SDL_GL_MakeCurrent(window, context) < 0) return makeError();
    }
    extern fn SDL_GL_MakeCurrent(window: *Window, context: Context) i32;

    /// `pub fn deleteContext(context: Context) void`
    pub const deleteContext = SDL_GL_DeleteContext;
    extern fn SDL_GL_DeleteContext(context: Context) void;

    /// `pub fn getDrawableSize(window: *Window, w: ?*i32, h: ?*i32) void`
    pub const getDrawableSize = SDL_GL_GetDrawableSize;
    extern fn SDL_GL_GetDrawableSize(window: *Window, w: ?*i32, h: ?*i32) void;
};

//--------------------------------------------------------------------------------------------------
//
// 2D Accelerated Rendering
//
//--------------------------------------------------------------------------------------------------
pub const TextureAccess = enum(i32) {
    static,
    streaming,
    target,
};

pub const Texture = opaque {
    pub fn destroy(tex: *Texture) void {
        SDL_DestroyTexture(tex);
    }
    extern fn SDL_DestroyTexture(texture: ?*Texture) void;

    pub fn query(texture: *Texture, format: ?*PixelFormat, access: ?*TextureAccess, w: ?*i32, h: ?*i32) !void {
        if (SDL_QueryTexture(texture, @as(?*u32, @ptrCast(format)), @as(?*c_int, @ptrCast(access)), w, h) != 0) {
            return makeError();
        }
    }
    extern fn SDL_QueryTexture(texture: *Texture, format: ?*u32, access: ?*c_int, w: ?*c_int, h: ?*c_int) c_int;

    pub fn lock(texture: *Texture, rect: ?*Rect) !struct {
        pixels: [*]u8,
        pitch: u32,
    } {
        var pixels: *anyopaque = undefined;
        var pitch: i32 = undefined;
        if (SDL_LockTexture(texture, rect, &pixels, &pitch) != 0) {
            return makeError();
        }
        return .{
            .pixels = @ptrCast(pixels),
            .pitch = @bitCast(pitch),
        };
    }
    extern fn SDL_LockTexture(texture: *Texture, rect: ?*Rect, pixels: **anyopaque, pitch: *c_int) c_int;

    pub fn unlock(texture: *Texture) void {
        SDL_UnlockTexture(texture);
    }
    extern fn SDL_UnlockTexture(texture: *Texture) void;
};

pub const Vertex = extern struct {
    position: PointF,
    color: Color,
    tex_coord: PointF = undefined,
};

pub const BlendMode = enum(i32) {
    none = 0x00000000,
    blend = 0x00000001,
    add = 0x00000002,
    mod = 0x00000004,
    multiply = 0x00000008,
    invalid = 0x7fffffff,
};

pub const ScaleMode = enum(i32) {
    nearest = 0x0000,
    linear = 0x0001,
    best = 0x0001,
};

pub const RendererFlip = enum(i32) {
    none = 0x0000,
    horizontal = 0x0001,
    vertical = 0x0002,
    both = 0x0003,
};

pub const RendererInfo = extern struct {
    name: [*c]const u8,
    flags: u32,
    num_texture_formats: u32,
    texture_formats: [16]u32,
    max_texture_width: i32,
    max_texture_height: i32,
};

pub const Renderer = opaque {
    pub const Flags = packed struct(u32) {
        software: bool = false,
        accelerated: bool = false,
        present_vsync: bool = false,
        target_texture: bool = false,
        __unused1: bool = false,
        __unused2: bool = false,
        __unused3: bool = false,
        __unused4: bool = false,
        __unused5: bool = false,
        __unused6: bool = false,
        __unused7: bool = false,
        __unused8: bool = false,
        __unused9: bool = false,
        __unused10: bool = false,
        __unused11: bool = false,
        __unused12: bool = false,
        __unused13: bool = false,
        __unused14: bool = false,
        __unused15: bool = false,
        __unused16: bool = false,
        __unused17: bool = false,
        __unused18: bool = false,
        __unused19: bool = false,
        __unused20: bool = false,
        __unused21: bool = false,
        __unused22: bool = false,
        __unused23: bool = false,
        __unused24: bool = false,
        __unused25: bool = false,
        __unused26: bool = false,
        __unused27: bool = false,
        __unused28: bool = false,
    };

    pub fn create(window: *Window, index: ?i32, flags: Flags) !*Renderer {
        return SDL_CreateRenderer(window, index orelse -1, flags) orelse makeError();
    }
    extern fn SDL_CreateRenderer(
        window: *Window,
        index: i32,
        flags: Flags,
    ) ?*Renderer;

    pub fn destroy(r: *Renderer) void {
        SDL_DestroyRenderer(r);
    }
    extern fn SDL_DestroyRenderer(r: *Renderer) void;

    pub fn clear(r: *Renderer) !void {
        if (SDL_RenderClear(r) < 0) return makeError();
    }
    extern fn SDL_RenderClear(r: *Renderer) i32;

    pub fn present(r: *Renderer) void {
        SDL_RenderPresent(r);
    }
    extern fn SDL_RenderPresent(r: *Renderer) void;

    pub fn copy(
        r: *Renderer,
        tex: *Texture,
        src: ?*const Rect,
        dst: ?*const Rect,
    ) !void {
        if (SDL_RenderCopy(r, tex, src, dst) < 0) return makeError();
    }
    extern fn SDL_RenderCopy(
        r: *Renderer,
        t: *Texture,
        srcrect: ?*const Rect,
        dstrect: ?*const Rect,
    ) i32;

    pub fn copyF(
        r: *Renderer,
        tex: *Texture,
        src: ?*const Rect,
        dst: ?*const RectF,
    ) !void {
        if (SDL_RenderCopyF(r, tex, src, dst) < 0) return makeError();
    }
    extern fn SDL_RenderCopyF(
        r: *Renderer,
        t: *Texture,
        srcrect: ?*const Rect,
        dstrect: ?*const RectF,
    ) i32;

    pub fn copyEx(
        r: *Renderer,
        tex: *Texture,
        src: ?*const Rect,
        dst: ?*const Rect,
        angle: f64,
        center: ?*const Point,
        flip: RendererFlip,
    ) !void {
        if (SDL_RenderCopyEx(r, tex, src, dst, angle, center, flip) < 0) return makeError();
    }
    extern fn SDL_RenderCopyEx(
        r: *Renderer,
        t: *Texture,
        srcrect: ?*const Rect,
        dstrect: ?*const Rect,
        angle: f64,
        center: *const Point,
        flip: RendererFlip,
    ) i32;

    pub fn copyExF(
        r: *Renderer,
        tex: *Texture,
        src: ?*const Rect,
        dst: ?*const RectF,
        angle: f64,
        center: ?*const PointF,
        flip: RendererFlip,
    ) !void {
        if (SDL_RenderCopyExF(r, tex, src, dst, angle, center, @intFromEnum(flip)) < 0) return makeError();
    }
    extern fn SDL_RenderCopyExF(
        r: *Renderer,
        t: *Texture,
        srcrect: ?*const Rect,
        dstrect: ?*const RectF,
        angle: f64,
        center: *const PointF,
        flip: RendererFlip,
    ) i32;

    pub fn setScale(r: *Renderer, x: f32, y: f32) !void {
        if (SDL_RenderSetScale(r, x, y) > 0) return makeError();
    }
    extern fn SDL_RenderSetScale(renderer: *Renderer, scaleX: f32, scaleY: f32) i32;

    pub fn drawLine(r: *Renderer, x0: i32, y0: i32, x1: i32, y1: i32) !void {
        if (SDL_RenderDrawLine(r, x0, y0, x1, y1) < 0) return makeError();
    }
    extern fn SDL_RenderDrawLine(renderer: *Renderer, x1: i32, y1: i32, x2: i32, y2: i32) i32;

    pub fn drawLineF(r: *Renderer, x0: f32, y0: f32, x1: f32, y1: f32) !void {
        if (SDL_RenderDrawLineF(r, x0, y0, x1, y1) < 0) return makeError();
    }
    extern fn SDL_RenderDrawLineF(renderer: *Renderer, x1: f32, y1: f32, x2: f32, y2: f32) i32;

    pub fn drawPoint(r: *Renderer, x: i32, y: i32) !void {
        if (SDL_RenderDrawPoint(r, x, y) < 0) return makeError();
    }
    extern fn SDL_RenderDrawPoint(renderer: *Renderer, x: c_int, y: c_int) c_int;

    pub fn drawPointF(r: *Renderer, x: f32, y: f32) !void {
        if (SDL_RenderDrawPointF(r, x, y) < 0) return makeError();
    }
    extern fn SDL_RenderDrawPointF(renderer: *Renderer, x: f32, y: f32) i32;

    pub fn fillRect(r: *Renderer, rect: Rect) !void {
        if (SDL_RenderFillRect(r, &rect) < 0) return makeError();
    }
    extern fn SDL_RenderFillRect(renderer: ?*Renderer, rect: *const Rect) i32;

    pub fn fillRectF(r: *Renderer, rect: RectF) !void {
        if (SDL_RenderFillRectF(r, &rect) < 0) return makeError();
    }
    extern fn SDL_RenderFillRectF(renderer: *Renderer, rect: *const RectF) i32;

    pub fn drawRect(r: *Renderer, rect: Rect) !void {
        if (SDL_RenderDrawRect(r, &rect) < 0) return makeError();
    }
    extern fn SDL_RenderDrawRect(renderer: *Renderer, rect: *const Rect) i32;

    pub fn drawRectF(r: *Renderer, rect: RectF) !void {
        if (SDL_RenderDrawRectF(r, &rect) < 0) return makeError();
    }
    extern fn SDL_RenderDrawRectF(renderer: *Renderer, rect: *const RectF) c_int;

    pub fn drawGeometry(
        r: *Renderer,
        tex: ?*const Texture,
        vertices: []const Vertex,
        indices: ?[]const u32,
    ) !void {
        if (SDL_RenderGeometry(
            r,
            tex,
            vertices.ptr,
            @as(i32, @intCast(vertices.len)),
            if (indices) |idx| @as([*]const i32, @ptrCast(idx.ptr)) else null,
            if (indices) |idx| @as(i32, @intCast(idx.len)) else 0,
        ) < 0)
            return makeError();
    }
    extern fn SDL_RenderGeometry(
        renderer: *Renderer,
        texture: ?*const Texture,
        vertices: [*c]const Vertex,
        num_vertices: i32,
        indices: [*c]const i32,
        num_indices: i32,
    ) i32;

    pub fn setDrawColor(r: *Renderer, color: Color) !void {
        if (SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a) < 0) return makeError();
    }

    pub fn setDrawColorRGB(r: *Renderer, _r: u8, g: u8, b: u8) !void {
        if (SDL_SetRenderDrawColor(r, _r, g, b, 255) < 0) return makeError();
    }

    pub fn setDrawColorRGBA(r: *Renderer, _r: u8, g: u8, b: u8, a: u8) !void {
        if (SDL_SetRenderDrawColor(r, _r, g, b, a) < 0) return makeError();
    }
    extern fn SDL_SetRenderDrawColor(r: *Renderer, _r: u8, g: u8, b: u8, a: u8) i32;

    pub fn getDrawColor(r: *Renderer) !Color {
        var color: Color = undefined;
        if (SDL_GetRenderDrawColor(r, &color.r, &color.g, &color.b, &color.a) < 0) return makeError();
        return color;
    }
    extern fn SDL_GetRenderDrawColor(renderer: *Renderer, r: *u8, g: *u8, b: *u8, a: *u8) i32;

    pub fn getDrawBlendMode(r: *Renderer) !BlendMode {
        var blend_mode: BlendMode = undefined;
        if (SDL_GetRenderDrawBlendMode(r, &blend_mode) < 0) return makeError();
        return blend_mode;
    }
    extern fn SDL_GetRenderDrawBlendMode(renderer: *Renderer, blendMode: *BlendMode) i32;

    pub fn setDrawBlendMode(r: *Renderer, blend_mode: BlendMode) !void {
        if (SDL_SetRenderDrawBlendMode(r, blend_mode) < 0) return makeError();
    }
    extern fn SDL_SetRenderDrawBlendMode(renderer: *Renderer, blendMode: BlendMode) i32;

    pub const OutputSize = struct { width_pixels: i32, height_pixels: i32 };
    pub fn getOutputSize(r: *Renderer) !OutputSize {
        var width_pixels: i32 = undefined;
        var height_pixels: i32 = undefined;
        if (SDL_GetRendererOutputSize(r, &width_pixels, &height_pixels) < 0) return makeError();
        return OutputSize{ .width_pixels = width_pixels, .height_pixels = height_pixels };
    }
    extern fn SDL_GetRendererOutputSize(renderer: *Renderer, w: *i32, h: *i32) i32;

    pub fn createTexture(
        renderer: *Renderer,
        format: PixelFormat,
        access: TextureAccess,
        width: i32,
        height: i32,
    ) !*Texture {
        return SDL_CreateTexture(renderer, @intFromEnum(format), @intFromEnum(access), width, height) orelse makeError();
    }
    extern fn SDL_CreateTexture(renderer: *Renderer, format: u32, access: i32, w: i32, h: i32) ?*Texture;

    pub fn createTextureFromSurface(renderer: *Renderer, surface: *Surface) !*Texture {
        return SDL_CreateTextureFromSurface(renderer, surface) orelse makeError();
    }
    extern fn SDL_CreateTextureFromSurface(renderer: *Renderer, surface: *Surface) ?*Texture;

    pub fn getInfo(r: *Renderer) !RendererInfo {
        var result: RendererInfo = undefined;
        if (SDL_GetRendererInfo(r, &result) < 0) return makeError();
        return result;
    }
    extern fn SDL_GetRendererInfo(renderer: *Renderer, info: *RendererInfo) i32;

    pub fn setClipRect(r: *Renderer, clip_rectangle: ?*const Rect) !void {
        if (SDL_RenderSetClipRect(r, clip_rectangle) < 0) return makeError();
    }
    extern fn SDL_RenderSetClipRect(renderer: *Renderer, rect: *const Rect) c_int;

    pub fn getClipRect(r: *Renderer) !?*const Rect {
        if (SDL_RenderIsClipEnabled(r) == 1) return null;
        var clip_rectangle: Rect = undefined;
        SDL_RenderGetClipRect(r, &clip_rectangle);
        return clip_rectangle;
    }
    extern fn SDL_RenderIsClipEnabled(renderer: *Renderer) i32;
    extern fn SDL_RenderGetClipRect(renderer: *Renderer, rect: *Rect) void;

    pub fn getLogicalSize(r: *Renderer) !struct { width: i32, height: i32 } {
        var width_pixels: i32 = undefined;
        var height_pixels: i32 = undefined;
        if (SDL_RenderGetLogicalSize(r, &width_pixels, &height_pixels) < 0) return makeError();
        return .{
            .width = width_pixels,
            .height = height_pixels,
        };
    }
    extern fn SDL_RenderGetLogicalSize(renderer: *Renderer, w: *i32, h: *i32) void;

    pub fn setLogicalSize(r: *Renderer, width_pixels: i32, height_pixels: i32) !void {
        if (SDL_RenderSetLogicalSize(r, width_pixels, height_pixels) < 0) return makeError();
    }
    extern fn SDL_RenderSetLogicalSize(renderer: *Renderer, w: i32, h: i32) i32;

    pub fn getViewport(r: *Renderer) Rect {
        var result: Rect = undefined;
        SDL_RenderGetViewport(r, &result);
        return result;
    }
    extern fn SDL_RenderGetViewport(renderer: *Renderer, rect: *Rect) void;

    pub fn setViewport(r: *Renderer, rect: Rect) !void {
        if (SDL_RenderSetViewport(r, &rect) < 0) return makeError();
    }
    extern fn SDL_RenderSetViewport(renderer: *Renderer, rect: *const Rect) i32;

    pub fn setTarget(r: *Renderer, tex: ?*const Texture) !void {
        if (SDL_SetRenderTarget(r, tex) < 0) return makeError();
    }
    extern fn SDL_SetRenderTarget(renderer: *Renderer, texture: ?*const Texture) i32;

    pub fn readPixels(
        r: *Renderer,
        rect: ?*const Rect,
        format: ?PixelFormat,
        pixels: [*]u8,
        pitch: u32,
    ) !void {
        if (SDL_RenderReadPixels(
            r,
            rect,
            if (format) |f| @intFromEnum(f) else 0,
            pixels,
            @as(i32, @intCast(pitch)),
        ) < 0) return makeError();
    }
    extern fn SDL_RenderReadPixels(
        renderer: *Renderer,
        rect: ?*const Rect,
        format: u32,
        pixels: ?*anyopaque,
        pitch: i32,
    ) i32;
};

//--------------------------------------------------------------------------------------------------
//
// Pixel Formats and Conversion Routines
//
//--------------------------------------------------------------------------------------------------
const PixelType = enum(u32) {
    none = 0,
    index1,
    index4,
    index8,
    packed8,
    packed16,
    packed32,
    arrayu8,
    arrayu16,
    arrayu32,
    arrayf16,
    arrayf32,
};
const BitmapOrder = enum(u32) {
    none = 0,
    @"4321",
    @"1234",
};
const PackedOrder = enum(u32) {
    none = 0,
    xrgb,
    rgbx,
    argb,
    rgba,
    xbgr,
    bgrx,
    abgr,
    bgra,
};
const ArrayOrder = enum(u32) {
    none = 0,
    rgb,
    rgba,
    argb,
    bgr,
    bgra,
    abgr,
};
const PackedLayout = enum(u32) {
    none = 0,
    @"332",
    @"4444",
    @"1555",
    @"5551",
    @"565",
    @"8888",
    @"2101010",
    @"1010102",
};
fn definePixelFormat(
    _type: PixelType,
    order: anytype,
    layout: u32,
    bits: u32,
    bytes: u32,
) u32 {
    switch (_type) {
        .index1, .index4, .index8 => {
            assert(@TypeOf(order) == BitmapOrder);
        },
        .packed8, .packed16, .packed32 => {
            assert(@TypeOf(order) == PackedOrder);
        },
        .arrayu8, .arrayu16, .arrayu32, .arrayf16, .arrayf32 => {
            assert(@TypeOf(order) == ArrayOrder);
        },
        .none => unreachable,
    }
    return ((1 << 28) | ((@intFromEnum(_type)) << 24) | ((@intFromEnum(order)) << 20) |
        ((layout) << 16) | ((bits) << 8) | ((bytes) << 0));
}
pub const PixelFormat = enum(u32) {
    index1lsb = definePixelFormat(.index1, BitmapOrder.@"4321", 0, 1, 0),
    index1msb = definePixelFormat(.index1, BitmapOrder.@"1234", 0, 1, 0),
    index4lsb = definePixelFormat(.index4, BitmapOrder.@"4321", 0, 4, 0),
    index4msb = definePixelFormat(.index4, BitmapOrder.@"1234", 0, 4, 0),
    index8 = definePixelFormat(.index8, BitmapOrder.none, 0, 8, 1),
    rgb332 = definePixelFormat(.packed8, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"332"), 8, 1),
    xrgb4444 = definePixelFormat(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"4444"), 12, 2),
    xbgr4444 = definePixelFormat(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"4444"), 12, 2),
    xrgb1555 = definePixelFormat(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"1555"), 15, 2),
    xbgr1555 = definePixelFormat(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"1555"), 15, 2),
    argb4444 = definePixelFormat(.packed16, PackedOrder.argb, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    rgba4444 = definePixelFormat(.packed16, PackedOrder.rgba, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    abgr4444 = definePixelFormat(.packed16, PackedOrder.abgr, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    bgra4444 = definePixelFormat(.packed16, PackedOrder.bgra, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    argb1555 = definePixelFormat(.packed16, PackedOrder.argb, @intFromEnum(PackedLayout.@"1555"), 16, 2),
    rgba5551 = definePixelFormat(.packed16, PackedOrder.rgba, @intFromEnum(PackedLayout.@"5551"), 16, 2),
    abgr1555 = definePixelFormat(.packed16, PackedOrder.abgr, @intFromEnum(PackedLayout.@"1555"), 16, 2),
    bgra5551 = definePixelFormat(.packed16, PackedOrder.bgra, @intFromEnum(PackedLayout.@"5551"), 16, 2),
    rgb565 = definePixelFormat(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"565"), 16, 2),
    bgr565 = definePixelFormat(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"565"), 16, 2),
    rgb24 = definePixelFormat(.arrayu8, ArrayOrder.rgb, 0, 24, 3),
    bgr24 = definePixelFormat(.arrayu8, ArrayOrder.bgr, 0, 24, 3),
    xrgb8888 = definePixelFormat(.packed32, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    rgbx8888 = definePixelFormat(.packed32, PackedOrder.rgbx, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    xbgr8888 = definePixelFormat(.packed32, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    bgrx8888 = definePixelFormat(.packed32, PackedOrder.bgrx, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    argb8888 = definePixelFormat(.packed32, PackedOrder.argb, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    rgba8888 = definePixelFormat(.packed32, PackedOrder.rgba, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    abgr8888 = definePixelFormat(.packed32, PackedOrder.abgr, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    bgra8888 = definePixelFormat(.packed32, PackedOrder.bgra, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    argb2101010 = definePixelFormat(.packed32, PackedOrder.argb, @intFromEnum(PackedLayout.@"2101010"), 32, 4),
};

pub const Color = extern struct {
    pub const black = rgb(0x00, 0x00, 0x00);
    pub const white = rgb(0xFF, 0xFF, 0xFF);
    pub const red = rgb(0xFF, 0x00, 0x00);
    pub const green = rgb(0x00, 0xFF, 0x00);
    pub const blue = rgb(0x00, 0x00, 0xFF);
    pub const magenta = rgb(0xFF, 0x00, 0xFF);
    pub const cyan = rgb(0x00, 0xFF, 0xFF);
    pub const yellow = rgb(0xFF, 0xFF, 0x00);

    r: u8,
    g: u8,
    b: u8,
    a: u8,

    /// Returns a initialized color struct with alpha = 255
    pub fn rgb(r: u8, g: u8, b: u8) Color {
        return Color{ .r = r, .g = g, .b = b, .a = 255 };
    }

    /// Returns a initialized color struct
    pub fn rgba(r: u8, g: u8, b: u8, a: u8) Color {
        return Color{ .r = r, .g = g, .b = b, .a = a };
    }

    pub const ParseError = error{
        UnknownFormat,
        InvalidCharacter,
        Overflow,
    };

    /// Parses a hex string color literal.
    /// allowed formats are:
    /// - `RGB`
    /// - `RGBA`
    /// - `#RGB`
    /// - `#RGBA`
    /// - `RRGGBB`
    /// - `#RRGGBB`
    /// - `RRGGBBAA`
    /// - `#RRGGBBAA`
    pub fn parse(str: []const u8) ParseError!Color {
        switch (str.len) {
            // RGB
            3 => {
                const r = try std.fmt.parseInt(u8, str[0..1], 16);
                const g = try std.fmt.parseInt(u8, str[1..2], 16);
                const b = try std.fmt.parseInt(u8, str[2..3], 16);

                return rgb(
                    r | (r << 4),
                    g | (g << 4),
                    b | (b << 4),
                );
            },

            // #RGB, RGBA
            4 => {
                if (str[0] == '#')
                    return parse(str[1..]);

                const r = try std.fmt.parseInt(u8, str[0..1], 16);
                const g = try std.fmt.parseInt(u8, str[1..2], 16);
                const b = try std.fmt.parseInt(u8, str[2..3], 16);
                const a = try std.fmt.parseInt(u8, str[3..4], 16);

                // bit-expand the patters to a uniform range
                return rgba(
                    r | (r << 4),
                    g | (g << 4),
                    b | (b << 4),
                    a | (a << 4),
                );
            },

            // #RGBA
            5 => return parse(str[1..]),

            // RRGGBB
            6 => {
                const r = try std.fmt.parseInt(u8, str[0..2], 16);
                const g = try std.fmt.parseInt(u8, str[2..4], 16);
                const b = try std.fmt.parseInt(u8, str[4..6], 16);

                return rgb(r, g, b);
            },

            // #RRGGBB
            7 => return parse(str[1..]),

            // RRGGBBAA
            8 => {
                const r = try std.fmt.parseInt(u8, str[0..2], 16);
                const g = try std.fmt.parseInt(u8, str[2..4], 16);
                const b = try std.fmt.parseInt(u8, str[4..6], 16);
                const a = try std.fmt.parseInt(u8, str[6..8], 16);

                return rgba(r, g, b, a);
            },

            // #RRGGBBAA
            9 => return parse(str[1..]),

            else => return error.UnknownFormat,
        }
    }
};

//--------------------------------------------------------------------------------------------------
//
// Rectangle Functions
//
//--------------------------------------------------------------------------------------------------
pub const Rect = extern struct {
    x: i32,
    y: i32,
    w: i32,
    h: i32,

    pub fn hasIntersection(a: *const Rect, b: *const Rect) bool {
        return SDL_HasIntersection(a, b) == 1;
    }
    extern fn SDL_HasIntersection(a: *const Rect, b: *const Rect) i32;

    pub fn intersectRect(a: *const Rect, b: *const Rect, result: *Rect) bool {
        return SDL_IntersectRect(a, b, result) == 1;
    }
    extern fn SDL_IntersectRect(a: *const Rect, b: *const Rect, result: *Rect) i32;

    pub fn intersectRectAndLine(rect: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) bool {
        return SDL_IntersectRectAndLine(rect, x1, y1, x2, y2) == 1;
    }
    extern fn SDL_IntersectRectAndLine(r: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) i32;
};

pub const RectF = extern struct {
    x: f32,
    y: f32,
    w: f32,
    h: f32,

    pub fn hasIntersection(a: *const Rect, b: *const Rect) bool {
        return SDL_HasIntersectionF(a, b);
    }
    extern fn SDL_HasIntersectionF(a: *const Rect, b: *const Rect) bool;

    pub fn intersectRect(a: *const Rect, b: *const Rect, result: *Rect) bool {
        return SDL_IntersectFRect(a, b, result);
    }
    extern fn SDL_IntersectFRect(a: *const Rect, b: *const Rect, result: *Rect) bool;

    pub fn intersectRectAndLine(rect: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) bool {
        return SDL_IntersectFRectAndLine(rect, x1, y1, x2, y2);
    }
    extern fn SDL_IntersectFRectAndLine(r: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) bool;
};

pub const Point = extern struct {
    x: i32,
    y: i32,
};

pub const PointF = extern struct {
    x: f32,
    y: f32,
};

//--------------------------------------------------------------------------------------------------
//
// Surface Creation and Simple Drawing
//
//--------------------------------------------------------------------------------------------------
pub const Surface = opaque {
    pub fn free(surface: *Surface) void {
        SDL_FreeSurface(surface);
    }
    extern fn SDL_FreeSurface(surface: *Surface) void;
};

//--------------------------------------------------------------------------------------------------
//
// Platform-specific Window Management
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Clipboard Handling
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Vulkan Support
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Metal Support
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Event Handling
//
//--------------------------------------------------------------------------------------------------
pub const EventType = enum(u32) {
    firstevent = 0,

    quit = 0x100,
    app_terminating,
    app_lowmemory,
    app_willenterbackground,
    app_didenterbackground,
    app_willenterforeground,
    app_didenterforeground,
    localechanged,

    displayevent = 0x150,

    windowevent = 0x200,
    syswmevent,

    keydown = 0x300,
    keyup,
    textediting,
    textinput,
    keymapchanged,
    textediting_ext,
    mousemotion = 0x400,
    mousebuttondown,
    mousebuttonup,
    mousewheel,

    joyaxismotion = 0x600,
    joyballmotion,
    joyhatmotion,
    joybuttondown,
    joybuttonup,
    joydeviceadded,
    joydeviceremoved,
    joybatteryupdated,

    controlleraxismotion = 0x650,
    controllerbuttondown,
    controllerbuttonup,
    controllerdeviceadded,
    controllerdeviceremoved,
    controllerdeviceremapped,
    controllertouchpaddown,
    controllertouchpadmotion,
    controllertouchpadup,
    controllersensorupdate,

    fingerdown = 0x700,
    fingerup,
    fingermotion,

    dollargesture = 0x800,
    dollarrecord,
    multigesture,

    clipboardupdate = 0x900,

    dropfile = 0x1000,
    droptext,
    dropbegin,
    dropcomplete,

    audiodeviceadded = 0x1100,
    audiodeviceremoved,

    sensorupdate = 0x1200,

    render_targets_reset = 0x2000,
    render_device_reset,

    pollsentinel = 0x7f00,

    userevent = 0x8000,

    lastevent = 0xffff,

    _,
};

pub const DisplayEventId = enum(u8) {
    none,
    orientation,
    connected,
    disconnected,
};

pub const WindowEventId = enum(u8) {
    none,
    shown,
    hidden,
    exposed,

    moved,

    resized,
    size_changed,

    minimized,
    maximized,
    restored,

    enter,
    leave,
    focus_gained,
    focus_lost,
    close,
    take_focus,
    hit_test,
    iccprof_changed,
    display_changed,
};

pub const ReleasedOrPressed = enum(u8) {
    released,
    pressed,
};

pub const CommonEvent = extern struct {
    type: EventType,
    timestamp: u32,
};

pub const DisplayEvent = extern struct {
    type: EventType,
    timestamp: u32,
    display: DisplayId,
    event: DisplayEventId,
    padding1: u8,
    padding2: u8,
    padding3: u8,
    data1: i32,
};

pub const WindowEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    event: WindowEventId,
    padding1: u8,
    padding2: u8,
    padding3: u8,
    data1: i32,
    data2: i32,
};

pub const KeyboardEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    state: ReleasedOrPressed,
    repeat: u8,
    padding2: u8,
    padding3: u8,
    keysym: Keysym,
};

pub const TextEditingEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    text: [text_size]u8,
    start: i32,
    length: i32,

    const text_size = 32;
};

pub const TextEditingExtEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    text: [*:0]u8,
    start: i32,
    length: i32,
};

pub const TextInputEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    text: [text_size]u8,

    const text_size = 32;
};

pub const MouseMotionEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    which: MouseId,
    state: u32,
    x: i32,
    y: i32,
    xrel: i32,
    yrel: i32,
};

pub const MouseButtonEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    which: MouseId,
    button: u8,
    state: ReleasedOrPressed,
    clicks: u8,
    padding1: u8,
    x: i32,
    y: i32,
};

pub const MouseWheelEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: WindowId,
    which: MouseId,
    x: i32,
    y: i32,
    direction: MouseWheelDirection,
    preciseX: f32,
    preciseY: f32,
};

pub const QuitEvent = extern struct {
    type: EventType,
    timestamp: u32,
};

pub const DropEvent = extern struct {
    type: EventType,
    timestamp: u32,
    file: ?[*:0]u8,
    window_id: WindowId,
};

pub const ControllerDeviceEvent = extern struct {
    type: EventType,
    timestamp: u32,
    which: JoystickId,
};

pub const Event = extern union {
    type: EventType,
    common: CommonEvent,
    display: DisplayEvent,
    window: WindowEvent,
    key: KeyboardEvent,
    edit: TextEditingEvent,
    editExt: TextEditingExtEvent,
    text: TextInputEvent,
    motion: MouseMotionEvent,
    button: MouseButtonEvent,
    wheel: MouseWheelEvent,
    controllerdevice: ControllerDeviceEvent,
    quit: QuitEvent,
    drop: DropEvent,

    padding: [size]u8,

    const size = if (@sizeOf(usize) <= 8) 56 else if (@sizeOf(usize) == 16) 64 else 3 * @sizeOf(usize);

    comptime {
        assert(@sizeOf(Event) == size);
    }
};

pub fn pollEvent(event: ?*Event) bool {
    return SDL_PollEvent(event) != 0;
}
extern fn SDL_PollEvent(event: ?*Event) i32;

//--------------------------------------------------------------------------------------------------
//
// Keyboard Support
//
//--------------------------------------------------------------------------------------------------
pub const Scancode = @import("keyboard.zig").Scancode;

pub const Keycode = @import("keyboard.zig").Keycode;

pub const Keysym = extern struct {
    scancode: Scancode,
    sym: Keycode,
    mod: u16,
    unused: u32,
};

/// `pub fn SDL_GetKeyboardState(numkeys: ?*i32) ?[*]const u8`
pub fn getKeyboardState() []const u8 {
    var numkeys: i32 = 0;
    const ptr = SDL_GetKeyboardState(&numkeys).?;
    return ptr[0..@as(usize, @intCast(numkeys))];
}
extern fn SDL_GetKeyboardState(numkeys: ?*i32) ?[*]const u8;

//--------------------------------------------------------------------------------------------------
//
// Mouse Support
//
//--------------------------------------------------------------------------------------------------
pub const MouseId = u32;

pub const MouseWheelDirection = enum(u32) {
    normal,
    flipped,
};

/// `pub fn getMouseFocus() ?*Window`
pub const getMouseFocus = SDL_GetMouseFocus;
extern fn SDL_GetMouseFocus() ?*Window;

/// `pub fn getMouseState(x: ?*i32, y: ?*i32) u32`
pub const getMouseState = SDL_GetMouseState;
extern fn SDL_GetMouseState(x: ?*i32, y: ?*i32) u32;

//--------------------------------------------------------------------------------------------------
//
// Joystick Support
//
//--------------------------------------------------------------------------------------------------
pub const JoystickId = i32;

pub const JOYSTICK_AXIS_MAX = 32767;
pub const JOYSTICK_AXIS_MIN = -32768;

//--------------------------------------------------------------------------------------------------
//
// Game Controller Support
//
//--------------------------------------------------------------------------------------------------
pub const GameController = opaque {
    pub const Axis = enum(c_int) {
        leftx,
        lefty,
        rightx,
        righty,
        triggerleft,
        triggerright,
    };
    pub const Button = enum(c_int) {
        a,
        b,
        x,
        y,
        back,
        guide,
        start,
        leftstick,
        rightstick,
        leftshoulder,
        rightshoulder,
        dpad_up,
        dpad_down,
        dpad_left,
        dpad_right,
        misc1,
        paddle1,
        paddle2,
        paddle3,
        paddle4,
        touchpad,
    };

    pub fn open(joystick_index: i32) ?*GameController {
        return SDL_GameControllerOpen(joystick_index);
    }
    extern fn SDL_GameControllerOpen(joystick_index: i32) ?*GameController;

    pub fn close(controller: *GameController) void {
        SDL_GameControllerClose(controller);
    }
    extern fn SDL_GameControllerClose(joystick: *GameController) void;

    pub fn getAxis(controller: *GameController, axis: Axis) i16 {
        return SDL_GameControllerGetAxis(controller, @intFromEnum(axis));
    }
    extern fn SDL_GameControllerGetAxis(*GameController, axis: c_int) i16;

    pub fn getButton(controller: *GameController, button: Button) bool {
        return (SDL_GameControllerGetButton(controller, @intFromEnum(button)) != 0);
    }
    extern fn SDL_GameControllerGetButton(controller: *GameController, button: c_int) u8;
};

//--------------------------------------------------------------------------------------------------
//
// Sensors
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Force Feedback Support
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Audio Device Management, Playing and Recording
//
//--------------------------------------------------------------------------------------------------
pub const AUDIO_MASK_BITSIZE = @as(c_int, 0xFF);
pub const AUDIO_MASK_DATATYPE = @as(c_int, 1) << @as(c_int, 8);
pub const AUDIO_MASK_ENDIAN = @as(c_int, 1) << @as(c_int, 12);
pub const AUDIO_MASK_SIGNED = @as(c_int, 1) << @as(c_int, 15);
pub inline fn AUDIO_BITSIZE(x: c_int) c_int {
    return x & AUDIO_MASK_BITSIZE;
}
pub inline fn AUDIO_ISFLOAT(x: c_int) bool {
    return (x & AUDIO_MASK_DATATYPE) != 0;
}
pub inline fn AUDIO_ISBIGENDIAN(x: c_int) bool {
    return (x & AUDIO_MASK_ENDIAN) != 0;
}
pub inline fn AUDIO_ISSIGNED(x: c_int) bool {
    return (x & AUDIO_MASK_SIGNED) != 0;
}
pub inline fn AUDIO_ISINT(x: c_int) bool {
    return !AUDIO_ISFLOAT(x);
}
pub inline fn AUDIO_ISLITTLEENDIAN(x: c_int) bool {
    return !AUDIO_ISBIGENDIAN(x);
}
pub inline fn AUDIO_ISUNSIGNED(x: c_int) bool {
    return !AUDIO_ISSIGNED(x);
}
pub const AUDIO_U8 = 0x0008;
pub const AUDIO_S8 = 0x8008;
pub const AUDIO_U16LSB = 0x0010;
pub const AUDIO_S16LSB = 0x8010;
pub const AUDIO_U16MSB = 0x1010;
pub const AUDIO_S16MSB = 0x9010;
pub const AUDIO_U16 = AUDIO_U16LSB;
pub const AUDIO_S16 = AUDIO_S16LSB;
pub const AUDIO_S32LSB = 0x8020;
pub const AUDIO_S32MSB = 0x9020;
pub const AUDIO_S32 = AUDIO_S32LSB;
pub const AUDIO_F32LSB = 0x8120;
pub const AUDIO_F32MSB = 0x9120;
pub const AUDIO_F32 = AUDIO_F32LSB;
pub const AUDIO_U16SYS = switch (builtin.target.cpu.arch.endian()) {
    .Little => AUDIO_U16LSB,
    .Big => AUDIO_U16MSB,
};
pub const AUDIO_S16SYS = switch (builtin.target.cpu.arch.endian()) {
    .Little => AUDIO_S16LSB,
    .Big => AUDIO_S16MSB,
};
pub const AUDIO_S32SYS = switch (builtin.target.cpu.arch.endian()) {
    .Little => AUDIO_S32LSB,
    .Big => AUDIO_S32MSB,
};
pub const AUDIO_F32SYS = switch (builtin.target.cpu.arch.endian()) {
    .Little => AUDIO_F32LSB,
    .Big => AUDIO_F32MSB,
};

pub const AudioCallback = *const fn (
    userdata: ?*anyopaque,
    stream: [*c]u8,
    len: c_int,
) callconv(.C) void;

pub const AudioFormat = u16;

pub const AudioSpec = extern struct {
    freq: c_int,
    format: AudioFormat,
    channels: u8,
    silence: u8 = 0,
    samples: u16,
    size: u32 = undefined,
    callback: ?AudioCallback = null,
    userdata: ?*anyopaque = null,
};

pub const AudioDeviceId = u32;

pub fn openAudioDevice(
    maybe_device: ?[:0]const u8,
    iscapture: bool,
    desired: *const AudioSpec,
    obtained: *AudioSpec,
    allowed_changes: c_int,
) AudioDeviceId {
    return SDL_OpenAudioDevice(
        if (maybe_device) |device| device.ptr else null,
        if (iscapture) 1 else 0,
        desired,
        obtained,
        allowed_changes,
    );
}
extern fn SDL_OpenAudioDevice(
    device: ?[*:0]const u8,
    iscapture: c_int,
    desired: *const AudioSpec,
    obtained: *AudioSpec,
    allowed_changes: c_int,
) AudioDeviceId;

pub fn pauseAudioDevice(device: AudioDeviceId, pause: bool) void {
    SDL_PauseAudioDevice(device, if (pause) 1 else 0);
}
extern fn SDL_PauseAudioDevice(AudioDeviceId, pause: c_int) void;

pub fn queueAudio(comptime SampleType: type, device: AudioDeviceId, data: []const SampleType) bool {
    return SDL_QueueAudio(device, data.ptr, @sizeOf(SampleType) * @as(u32, @intCast(data.len))) == 0;
}
extern fn SDL_QueueAudio(AudioDeviceId, data: *const anyopaque, len: u32) c_int;

/// `pub fn getQueuedAudioSize(device: AudioDeviceId) u32`
pub const getQueuedAudioSize = SDL_GetQueuedAudioSize;
extern fn SDL_GetQueuedAudioSize(AudioDeviceId) u32;

/// `pub fn clearQueueAudio(device: AudioDeviceId) void`
pub const clearQueuedAudio = SDL_ClearQueuedAudio;
extern fn SDL_ClearQueuedAudio(AudioDeviceId) void;

//--------------------------------------------------------------------------------------------------
//
// Thread Management
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Force Synchronization Primitives
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Atomic Operations
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Timer Support
//
//--------------------------------------------------------------------------------------------------
/// `pub fn getPerformanceCounter() u64`
pub const getPerformanceCounter = SDL_GetPerformanceCounter;
extern fn SDL_GetPerformanceCounter() u64;

/// `pub fn getPerformanceFrequency() u64`
pub const getPerformanceFrequency = SDL_GetPerformanceFrequency;
extern fn SDL_GetPerformanceFrequency() u64;

/// `pub fn delay(ms: u32) void`
pub const delay = SDL_Delay;
extern fn SDL_Delay(ms: u32) void;

//--------------------------------------------------------------------------------------------------
//
// Filesystem Paths
//
//--------------------------------------------------------------------------------------------------
pub fn getBasePath() ?[]const u8 {
    return if (SDL_GetBasePath()) |path| std.mem.span(path) else null;
}
extern fn SDL_GetBasePath() [*c]const u8;

pub fn getPrefPath(org: [:0]const u8, app: [:0]const u8) ?[]const u8 {
    return if (SDL_GetPrefPath(org.ptr, app.ptr)) |path| std.mem.span(path) else null;
}
extern fn SDL_GetPrefPath(org: [*c]const u8, app: [*c]const u8) [*c]const u8;

//--------------------------------------------------------------------------------------------------
//
// File I/O Abstraction
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Shared Object Loading and Function Lookup
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Platform Detection
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// CPU Feature Detection
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Byte Order and Byte Swapping
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Bit Manipulation
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Power Management Status
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Message boxes
//
//--------------------------------------------------------------------------------------------------
pub const MessageBoxFlags = packed struct(u32) {
    err: bool = false,
    warning: bool = false,
    information: bool = false,
    buttons_left_to_right: bool = false,
    buttons_right_to_left: bool = false,
    __unused: u27 = 0,
};

pub fn showSimpleMessageBox(
    flags: MessageBoxFlags,
    title: [:0]const u8,
    message: [:0]const u8,
    window: ?*Window,
) Error!void {
    if (SDL_ShowSimpleMessageBox(flags, title, message, window) < 0) return makeError();
}
extern fn SDL_ShowSimpleMessageBox(
    flags: MessageBoxFlags,
    title: ?[*:0]const u8,
    message: ?[*:0]const u8,
    window: ?*Window,
) i32;

//--------------------------------------------------------------------------------------------------
//
// Platform-specific Functionality
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Standard Library Functionality
//
//--------------------------------------------------------------------------------------------------
