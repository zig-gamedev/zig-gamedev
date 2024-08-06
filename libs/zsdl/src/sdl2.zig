const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

comptime {
    _ = std.testing.refAllDecls(@This());
}

const sdl2 = @This();

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

/// Initialize the SDL library.
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

/// Set a hint with normal priority.
pub fn setHint(name: [:0]const u8, value: [:0]const u8) bool {
    return SDL_SetHint(name, value) == True;
}
extern fn SDL_SetHint(name: [*:0]const u8, value: [*:0]const u8) Bool;

//--------------------------------------------------------------------------------------------------
//
// Error Handling
//
//--------------------------------------------------------------------------------------------------
pub const Error = error{SdlError};

/// Get SDL error string
pub fn getError() ?[:0]const u8 {
    if (SDL_GetError()) |ptr| {
        return std.mem.sliceTo(ptr, 0);
    }
    return null;
}
extern fn SDL_GetError() ?[*:0]const u8;

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
/// Information about the version of SDL in use.
pub const Version = extern struct {
    major: u8,
    minor: u8,
    patch: u8,
};

/// Compiled SDL version
pub const VERSION = Version{
    .major = 2,
    .minor = 24,
    .patch = 1,
};

/// Returns the linked SDL version
pub fn getVersion() Version {
    var version: Version = undefined;
    SDL_GetVersion(&version);

    return version;
}
extern fn SDL_GetVersion(version: *Version) void;

//--------------------------------------------------------------------------------------------------
//
// Display and Window management
//
//--------------------------------------------------------------------------------------------------
pub const DisplayId = u32;

pub const WindowId = u32;

pub const DisplayMode = extern struct {
    format: PixelFormatEnum,
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
        __unused21: u7 = 0,
        vulkan: bool = false, // 0x10000000
        metal: bool = false,
        __unused30: u2 = 0,

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

    pub const create = createWindow;
    pub const destroy = destroyWindow;
    pub const getDisplayMode = windowGetDisplayMode;
    pub const getPosition = windowGetPosition;
    pub const getSize = windowGetSize;
    pub const setTitle = windowSetTitle;
    pub const getSurface = getWindowSurface;
};

/// Create a window with the specified position, dimensions, and flags.
pub fn createWindow(title: ?[*:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Window.Flags) Error!*Window {
    return SDL_CreateWindow(title, x, y, w, h, flags) orelse return makeError();
}
extern fn SDL_CreateWindow(title: ?[*:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Window.Flags) ?*Window;

/// Destroy a window.
pub const destroyWindow = SDL_DestroyWindow;
extern fn SDL_DestroyWindow(window: *Window) void;

/// Get the SDL surface associated with the window.
pub fn getWindowSurface(window: *const Window) *Surface {
    return SDL_GetWindowSurface(window);
}
extern fn SDL_GetWindowSurface(*const Window) *Surface;

/// Query the display mode to use when a window is visible at fullscreen.
pub fn windowGetDisplayMode(window: *Window) Error!DisplayMode {
    var mode: DisplayMode = undefined;
    if (SDL_GetWindowDisplayMode(window, &mode) < 0) return makeError();
    return mode;
}
extern fn SDL_GetWindowDisplayMode(window: *Window, mode: *DisplayMode) i32;

/// Get the position of a window.
pub fn windowGetPosition(window: *Window, w: ?*i32, h: ?*i32) Error!void {
    if (SDL_GetWindowPosition(window, w, h) < 0) return makeError();
}
extern fn SDL_GetWindowPosition(window: *Window, x: ?*i32, y: ?*i32) i32;

/// Get the size of a window's client area.
pub fn windowGetSize(window: *Window, w: ?*i32, h: ?*i32) Error!void {
    if (SDL_GetWindowSize(window, w, h) < 0) return makeError();
}
extern fn SDL_GetWindowSize(window: *Window, w: ?*i32, h: ?*i32) i32;

/// Set the title of a window.
pub fn windowSetTitle(window: *Window, title: [:0]const u8) void {
    SDL_SetWindowTitle(window, title);
}
extern fn SDL_SetWindowTitle(window: *Window, title: ?[*:0]const u8) void;

/// Get the number of video drivers compiled into SDL.
pub fn getNumVideoDrivers() Error!u16 {
    const res = SDL_GetNumVideoDrivers();
    if (res < 1) return makeError();
    return @intCast(res);
}
extern fn SDL_GetNumVideoDrivers() c_int;

/// Get the name of a built in video driver.
pub fn getVideoDriver(index: u16) ?[:0]const u8 {
    if (SDL_GetVideoDriver(@intCast(index))) |ptr| {
        return std.mem.sliceTo(ptr, 0);
    }
    return null;
}
extern fn SDL_GetVideoDriver(index: c_int) ?[*:0]const u8;

pub const gl = struct {
    pub const Context = *anyopaque;

    pub const FunctionPointer = ?*const anyopaque;

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

    pub const Profile = enum(c_int) {
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

    pub const ContextResetNotification = enum(c_int) {
        no_notification = 0x0000,
        lose_context = 0x0001,
    };

    pub fn setAttribute(attr: Attr, value: i32) Error!void {
        if (SDL_GL_SetAttribute(attr, value) < 0) return makeError();
    }
    extern fn SDL_GL_SetAttribute(attr: Attr, value: c_int) c_int;

    pub fn getAttribute(attr: Attr) Error!i32 {
        var value: c_int = undefined;
        if (SDL_GL_GetAttribute(attr, &value) < 0) return makeError();
        return value;
    }
    extern fn SDL_GL_GetAttribute(attr: Attr, value: *c_int) c_int;

    pub fn setSwapInterval(interval: i32) Error!void {
        if (SDL_GL_SetSwapInterval(interval) < 0) return makeError();
    }
    extern fn SDL_GL_SetSwapInterval(interval: c_int) c_int;

    pub const getSwapInterval = SDL_GL_GetSwapInterval;
    extern fn SDL_GL_GetSwapInterval() c_int;

    pub const swapWindow = SDL_GL_SwapWindow;
    extern fn SDL_GL_SwapWindow(window: *Window) void;

    pub fn getProcAddress(proc: [*:0]const u8) FunctionPointer {
        return SDL_GL_GetProcAddress(proc);
    }
    extern fn SDL_GL_GetProcAddress(proc: ?[*:0]const u8) FunctionPointer;

    pub fn isExtensionSupported(extension: [:0]const u8) bool {
        return SDL_GL_ExtensionSupported(extension) == True;
    }
    extern fn SDL_GL_ExtensionSupported(extension: ?[*:0]const u8) Bool;

    pub fn createContext(window: *Window) Error!Context {
        return SDL_GL_CreateContext(window) orelse return makeError();
    }
    extern fn SDL_GL_CreateContext(window: *Window) ?Context;

    pub fn makeCurrent(window: *Window, context: Context) Error!void {
        if (SDL_GL_MakeCurrent(window, context) < 0) return makeError();
    }
    extern fn SDL_GL_MakeCurrent(window: *Window, context: Context) c_int;

    pub const deleteContext = SDL_GL_DeleteContext;
    extern fn SDL_GL_DeleteContext(context: Context) void;

    pub const getDrawableSize = SDL_GL_GetDrawableSize;
    extern fn SDL_GL_GetDrawableSize(window: *Window, w: ?*c_int, h: ?*c_int) void;
};

//--------------------------------------------------------------------------------------------------
//
// 2D Accelerated Rendering
//
//--------------------------------------------------------------------------------------------------
pub const TextureAccess = enum(c_int) {
    static,
    streaming,
    target,
};

/// An efficient driver-specific representation of pixel data
pub const Texture = opaque {
    pub const query = queryTexture;
    pub const lock = lockTexture;
    pub const unlock = unlockTexture;
    pub const destroy = destroyTexture;
};

/// Query the attributes of a texture.
pub fn queryTexture(
    texture: *Texture,
    format: ?*PixelFormatEnum,
    access: ?*TextureAccess,
    w: ?*i32,
    h: ?*i32,
) !void {
    if (SDL_QueryTexture(texture, format, access, w, h) != 0) {
        return makeError();
    }
}
extern fn SDL_QueryTexture(
    texture: *Texture,
    format: ?*PixelFormatEnum,
    access: ?*TextureAccess,
    w: ?*c_int,
    h: ?*c_int,
) c_int;

/// Lock a portion of the texture for write-only pixel access.
pub fn lockTexture(texture: *Texture, rect: ?*Rect) !struct {
    pixels: [*]u8,
    pitch: i32,
} {
    var pixels: *anyopaque = undefined;
    var pitch: i32 = undefined;
    if (SDL_LockTexture(texture, rect, &pixels, &pitch) != 0) {
        return makeError();
    }
    return .{
        .pixels = @ptrCast(pixels),
        .pitch = pitch,
    };
}
extern fn SDL_LockTexture(
    texture: *Texture,
    rect: ?*Rect,
    pixels: **anyopaque,
    pitch: *c_int,
) c_int;

/// Unlock a texture, uploading the changes to video memory, if needed.
pub fn unlockTexture(texture: *Texture) void {
    SDL_UnlockTexture(texture);
}
extern fn SDL_UnlockTexture(texture: *Texture) void;

/// Destroy the specified texture.
pub fn destroyTexture(tex: *Texture) void {
    SDL_DestroyTexture(tex);
}
extern fn SDL_DestroyTexture(texture: ?*Texture) void;

pub const Vertex = extern struct {
    position: FPoint,
    color: Color,
    tex_coord: FPoint,
};

pub const BlendMode = enum(c_int) {
    none = 0x00000000,
    blend = 0x00000001,
    add = 0x00000002,
    mod = 0x00000004,
    multiply = 0x00000008,
    invalid = 0x7fffffff,
};

pub const ScaleMode = enum(c_int) {
    nearest = 0x0000,
    linear = 0x0001,
    best = 0x0002,
};

pub const RendererFlip = enum(c_int) {
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
        __unused5: u28 = 0,
    };
    pub const create = createRenderer;
    pub const destroy = destroyRenderer;
    pub const clear = renderClear;
    pub const present = renderPresent;
    pub const copy = renderCopy;
    pub const copyF = renderCopyF;
    pub const copyEx = renderCopyEx;
    pub const copyExF = renderCopyExF;
    pub const setScale = renderSetScale;
    pub const drawLine = renderDrawLine;
    pub const drawLineF = renderDrawLineF;
    pub const drawPoint = renderDrawPoint;
    pub const drawPointF = renderDrawPointF;
    pub const fillRect = renderFillRect;
    pub const fillRectF = renderFillRectF;
    pub const drawRect = renderDrawRect;
    pub const drawRectF = renderDrawRectF;
    pub const renderGeometry = sdl2.renderGeometry;
    pub const getDrawColor = getRenderDrawColor;
    pub const setDrawColor = setRenderDrawColor;
    pub const setDrawColorRGB = setRenderDrawColorRGB;
    pub const setDrawColorRGBA = setRenderDrawColorRGBA;
    pub const getDrawBlendMode = getRenderDrawBlendMode;
    pub const setDrawBlendMode = setRenderDrawBlendMode;
    pub const getOutputSize = getRendererOutputSize;
    pub const createTexture = sdl2.createTexture;
    pub const createTextureFromSurface = sdl2.createTextureFromSurface;
    pub const getInfo = getRendererInfo;
    pub const isClipEnabled = renderIsClipEnabled;
    pub const getClipRect = renderGetClipRect;
    pub const setClipRect = renderSetClipRect;
    pub const getLogicalSize = renderGetLogicalSize;
    pub const setLogicalSize = renderSetLogicalSize;
    pub const getViewport = renderGetViewport;
    pub const setViewport = renderSetViewport;
    pub const setTarget = setRenderTarget;
    pub const readPixels = renderReadPixels;
};

/// Create a 2D rendering context for a window.
pub fn createRenderer(window: *Window, index: ?i32, flags: Renderer.Flags) Error!*Renderer {
    return SDL_CreateRenderer(window, index orelse -1, flags) orelse makeError();
}
extern fn SDL_CreateRenderer(window: *Window, index: i32, flags: Renderer.Flags) ?*Renderer;

/// Destroy the rendering context for a window and free associated textures.
pub const destroyRenderer = SDL_DestroyRenderer;
extern fn SDL_DestroyRenderer(r: *Renderer) void;

/// Clear the current rendering target with the drawing color.
pub fn renderClear(r: *Renderer) !void {
    if (SDL_RenderClear(r) < 0) return makeError();
}
extern fn SDL_RenderClear(r: *Renderer) i32;

/// Update the screen with any rendering performed since the previous call.
pub const renderPresent = SDL_RenderPresent;
extern fn SDL_RenderPresent(r: *Renderer) void;

/// Copy a portion of the texture to the current rendering target.
pub fn renderCopy(
    r: *Renderer,
    tex: *Texture,
    src: ?*const Rect,
    dst: ?*const Rect,
) Error!void {
    if (SDL_RenderCopy(r, tex, src, dst) < 0) return makeError();
}
extern fn SDL_RenderCopy(
    r: *Renderer,
    t: *Texture,
    srcrect: ?*const Rect,
    dstrect: ?*const Rect,
) c_int;

/// Copy a portion of the texture to the current rendering target at subpixel precision.
pub fn renderCopyF(
    r: *Renderer,
    tex: *Texture,
    src: ?*const Rect,
    dst: ?*const FRect,
) Error!void {
    if (SDL_RenderCopyF(r, tex, src, dst) < 0) return makeError();
}
extern fn SDL_RenderCopyF(
    r: *Renderer,
    t: *Texture,
    srcrect: ?*const Rect,
    dstrect: ?*const FRect,
) c_int;

/// Copy a portion of the texture to the current rendering, with optional rotation and flipping.
pub fn renderCopyEx(
    r: *Renderer,
    tex: *Texture,
    src: ?*const Rect,
    dst: ?*const Rect,
    angle: f64,
    center: ?*const Point,
    flip: RendererFlip,
) Error!void {
    if (SDL_RenderCopyEx(r, tex, src, dst, angle, center, flip) < 0) return makeError();
}
extern fn SDL_RenderCopyEx(
    r: *Renderer,
    t: *Texture,
    srcrect: ?*const Rect,
    dstrect: ?*const Rect,
    angle: f64,
    center: ?*const Point,
    flip: RendererFlip,
) c_int;

/// Copy a portion of the source texture to the current rendering target, with rotation and flipping, at subpixel precision.
pub fn renderCopyExF(
    r: *Renderer,
    tex: *Texture,
    src: ?*const Rect,
    dst: ?*const FRect,
    angle: f64,
    center: ?*const FPoint,
    flip: RendererFlip,
) Error!void {
    if (SDL_RenderCopyExF(r, tex, src, dst, angle, center, flip) < 0) {
        return makeError();
    }
}
extern fn SDL_RenderCopyExF(
    r: *Renderer,
    t: *Texture,
    srcrect: ?*const Rect,
    dstrect: ?*const FRect,
    angle: f64,
    center: ?*const FPoint,
    flip: RendererFlip,
) c_int;

/// Set the drawing scale for rendering on the current target.
pub fn renderSetScale(r: *Renderer, x: f32, y: f32) Error!void {
    if (SDL_RenderSetScale(r, x, y) > 0) return makeError();
}
extern fn SDL_RenderSetScale(renderer: *Renderer, scaleX: f32, scaleY: f32) c_int;

/// Draw a line on the current rendering target.
pub fn renderDrawLine(r: *Renderer, x0: i32, y0: i32, x1: i32, y1: i32) Error!void {
    if (SDL_RenderDrawLine(r, x0, y0, x1, y1) < 0) return makeError();
}
extern fn SDL_RenderDrawLine(renderer: *Renderer, x1: i32, y1: i32, x2: i32, y2: i32) c_int;

/// Draw a line on the current rendering target.
pub fn renderDrawLineF(r: *Renderer, x0: f32, y0: f32, x1: f32, y1: f32) Error!void {
    if (SDL_RenderDrawLineF(r, x0, y0, x1, y1) < 0) return makeError();
}
extern fn SDL_RenderDrawLineF(renderer: *Renderer, x1: f32, y1: f32, x2: f32, y2: f32) c_int;

/// Draw a point on the current rendering target.
pub fn renderDrawPoint(r: *Renderer, x: i32, y: i32) Error!void {
    if (SDL_RenderDrawPoint(r, x, y) < 0) return makeError();
}
extern fn SDL_RenderDrawPoint(renderer: *Renderer, x: c_int, y: c_int) c_int;

/// Draw a point on the current rendering target at subpixel precision.
pub fn renderDrawPointF(r: *Renderer, x: f32, y: f32) Error!void {
    if (SDL_RenderDrawPointF(r, x, y) < 0) return makeError();
}
extern fn SDL_RenderDrawPointF(renderer: *Renderer, x: f32, y: f32) c_int;

/// Fill a rectangle on the current rendering target with the drawing color.
pub fn renderFillRect(r: *Renderer, rect: Rect) Error!void {
    if (SDL_RenderFillRect(r, &rect) < 0) return makeError();
}
extern fn SDL_RenderFillRect(renderer: ?*Renderer, rect: *const Rect) c_int;

/// Fill a rectangle on the current rendering target with the drawing color at subpixel precision.
pub fn renderFillRectF(r: *Renderer, rect: FRect) Error!void {
    if (SDL_RenderFillRectF(r, &rect) < 0) return makeError();
}
extern fn SDL_RenderFillRectF(renderer: *Renderer, rect: *const FRect) c_int;

/// Draw a rectangle on the current rendering target.
pub fn renderDrawRect(r: *Renderer, rect: Rect) Error!void {
    if (SDL_RenderDrawRect(r, &rect) < 0) return makeError();
}
extern fn SDL_RenderDrawRect(renderer: *Renderer, rect: *const Rect) c_int;

/// Draw a rectangle on the current rendering target at subpixel precision.
pub fn renderDrawRectF(r: *Renderer, rect: FRect) Error!void {
    if (SDL_RenderDrawRectF(r, &rect) < 0) return makeError();
}
extern fn SDL_RenderDrawRectF(renderer: *Renderer, rect: *const FRect) c_int;

/// Render a list of triangles, optionally using a texture and indices into the vertex array Color and alpha modulation is done per vertex (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored).
pub fn renderGeometry(
    r: *Renderer,
    tex: ?*const Texture,
    vertices: []const Vertex,
    indices: ?[]const u32,
) Error!void {
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
) c_int;

/// Get the color used for drawing operations (Rect, Line and Clear).
pub fn getRenderDrawColor(renderer: *const Renderer) Error!Color {
    var color: Color = undefined;
    if (SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a) < 0) {
        return makeError();
    }
    return color;
}
extern fn SDL_GetRenderDrawColor(renderer: *const Renderer, r: *u8, g: *u8, b: *u8, a: *u8) c_int;

/// Set the color used for drawing operations (Rect, Line and Clear).
pub fn setRenderDrawColor(renderer: *Renderer, color: Color) Error!void {
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0) {
        return makeError();
    }
}

/// Set the color used for drawing operations (Rect, Line and Clear).
pub fn setRenderDrawColorRGB(renderer: *Renderer, r: u8, g: u8, b: u8) Error!void {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, 255) < 0) return makeError();
}

/// Set the color used for drawing operations (Rect, Line and Clear).
pub fn setRenderDrawColorRGBA(renderer: *Renderer, r: u8, g: u8, b: u8, a: u8) Error!void {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) return makeError();
}
extern fn SDL_SetRenderDrawColor(renderer: *Renderer, r: u8, g: u8, b: u8, a: u8) c_int;

/// Get the blend mode used for drawing operations.
pub fn getRenderDrawBlendMode(r: *const Renderer) Error!BlendMode {
    var blend_mode: BlendMode = undefined;
    if (SDL_GetRenderDrawBlendMode(r, &blend_mode) < 0) return makeError();
    return blend_mode;
}
extern fn SDL_GetRenderDrawBlendMode(renderer: *const Renderer, blendMode: *BlendMode) c_int;

/// Set the blend mode used for drawing operations (Fill and Line).
pub fn setRenderDrawBlendMode(r: *Renderer, blend_mode: BlendMode) Error!void {
    if (SDL_SetRenderDrawBlendMode(r, blend_mode) < 0) return makeError();
}
extern fn SDL_SetRenderDrawBlendMode(renderer: *Renderer, blendMode: BlendMode) c_int;

/// Get the output size in pixels of a rendering context.
pub fn getRendererOutputSize(renderer: *const Renderer) Error!struct { w: i32, h: i32 } {
    var w: i32 = undefined;
    var h: i32 = undefined;
    if (SDL_GetRendererOutputSize(renderer, &w, &h) < 0) return makeError();
    return .{ .w = w, .h = h };
}
extern fn SDL_GetRendererOutputSize(renderer: *const Renderer, w: *i32, h: *i32) c_int;

/// Create a texture for a rendering context.
pub fn createTexture(
    renderer: *Renderer,
    format: PixelFormatEnum,
    access: TextureAccess,
    width: i32,
    height: i32,
) Error!*Texture {
    return SDL_CreateTexture(renderer, format, access, width, height) orelse makeError();
}
extern fn SDL_CreateTexture(
    renderer: *Renderer,
    format: PixelFormatEnum,
    access: TextureAccess,
    w: c_int,
    h: c_int,
) ?*Texture;

/// Create a texture from an existing surface.
pub fn createTextureFromSurface(renderer: *Renderer, surface: *Surface) Error!*Texture {
    return SDL_CreateTextureFromSurface(renderer, surface) orelse makeError();
}
extern fn SDL_CreateTextureFromSurface(renderer: *Renderer, surface: *Surface) ?*Texture;

/// Get information about a rendering context.
pub fn getRendererInfo(r: *const Renderer) Error!RendererInfo {
    var result: RendererInfo = undefined;
    if (SDL_GetRendererInfo(r, &result) < 0) return makeError();
    return result;
}
extern fn SDL_GetRendererInfo(renderer: *const Renderer, info: *RendererInfo) c_int;

/// Get whether clipping is enabled on the given renderer.
pub fn renderIsClipEnabled(renderer: *const Renderer) bool {
    return SDL_RenderIsClipEnabled(renderer) == True;
}
pub extern fn SDL_RenderIsClipEnabled(renderer: *const Renderer) Bool;

/// Get the clip rectangle for the current target.
pub fn renderGetClipRect(r: *const Renderer) Rect {
    var clip_rect: Rect = undefined;
    SDL_RenderGetClipRect(r, &clip_rect);
    return clip_rect;
}
extern fn SDL_RenderGetClipRect(renderer: *const Renderer, rect: ?*Rect) void;

/// Set the clip rectangle for rendering on the specified target.
pub fn renderSetClipRect(r: *Renderer, clip_rect: ?*const Rect) Error!void {
    if (SDL_RenderSetClipRect(r, clip_rect) < 0) return makeError();
}
extern fn SDL_RenderSetClipRect(renderer: *Renderer, rect: ?*const Rect) c_int;

/// Set a device independent resolution for rendering.
pub fn renderGetLogicalSize(r: *const Renderer) struct { width: i32, height: i32 } {
    var width: i32 = undefined;
    var height: i32 = undefined;
    SDL_RenderGetLogicalSize(r, &width, &height);
    return .{
        .width = width,
        .height = height,
    };
}
extern fn SDL_RenderGetLogicalSize(renderer: *const Renderer, w: *i32, h: *i32) void;

/// Set a device independent resolution for rendering.
pub fn renderSetLogicalSize(r: *Renderer, width: i32, height: i32) Error!void {
    if (SDL_RenderSetLogicalSize(r, width, height) < 0) return makeError();
}
extern fn SDL_RenderSetLogicalSize(renderer: *Renderer, w: i32, h: i32) c_int;

/// Get the drawing area for the current target.
pub fn renderGetViewport(r: *const Renderer) Rect {
    var result: Rect = undefined;
    SDL_RenderGetViewport(r, &result);
    return result;
}
extern fn SDL_RenderGetViewport(renderer: *const Renderer, rect: *Rect) void;

/// Set the drawing area for rendering on the current target.
pub fn renderSetViewport(renderer: *Renderer, maybe_rect: ?*const Rect) Error!void {
    if (SDL_RenderSetViewport(renderer, maybe_rect) != 0) {
        return makeError();
    }
}
extern fn SDL_RenderSetViewport(renderer: *Renderer, rect: ?*const Rect) c_int;

/// Set a texture as the current rendering target.
pub fn setRenderTarget(r: *Renderer, tex: ?*const Texture) Error!void {
    if (SDL_SetRenderTarget(r, tex) < 0) return makeError();
}
extern fn SDL_SetRenderTarget(renderer: *Renderer, texture: ?*const Texture) c_int;

/// Read pixels from the current rendering target to an array of pixels.
pub fn renderReadPixels(
    renderer: *const Renderer,
    rect: ?*const Rect,
    format: PixelFormatEnum,
    pixels: [*]u8,
    pitch: i32,
) Error!void {
    if (SDL_RenderReadPixels(renderer, rect, format, pixels, pitch) < 0) {
        return makeError();
    }
}
extern fn SDL_RenderReadPixels(
    renderer: *const Renderer,
    rect: ?*const Rect,
    format: PixelFormatEnum,
    pixels: ?*anyopaque,
    pitch: c_int,
) c_int;

/// Create a window and default renderer.
pub fn createWindowAndRenderer(
    width: u32,
    height: u32,
    window_flags: Window.Flags,
    window: **Window,
    renderer: **Renderer,
) Error!void {
    if (SDL_CreateWindowAndRenderer(
        @bitCast(width),
        @bitCast(height),
        window_flags,
        @ptrCast(window),
        @ptrCast(renderer),
    ) != 0) return makeError();
}
extern fn SDL_CreateWindowAndRenderer(
    width: c_int,
    height: c_int,
    window_flags: Window.Flags,
    window: ?*?*Window,
    renderer: ?*?*Renderer,
) c_int;

//--------------------------------------------------------------------------------------------------
//
// Pixel Formats and Conversion Routines
//
//--------------------------------------------------------------------------------------------------
pub const PixelType = enum(c_int) {
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
pub const BitmapOrder = enum(c_int) {
    none = 0,
    @"4321",
    @"1234",
};
pub const PackedOrder = enum(c_int) {
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
pub const ArrayOrder = enum(c_int) {
    none = 0,
    rgb,
    rgba,
    argb,
    bgr,
    bgra,
    abgr,
};
pub const PackedLayout = enum(c_int) {
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
fn definePixelFormatEnum(
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
pub const PixelFormatEnum = enum(u32) {
    index1lsb = definePixelFormatEnum(.index1, BitmapOrder.@"4321", 0, 1, 0),
    index1msb = definePixelFormatEnum(.index1, BitmapOrder.@"1234", 0, 1, 0),
    index4lsb = definePixelFormatEnum(.index4, BitmapOrder.@"4321", 0, 4, 0),
    index4msb = definePixelFormatEnum(.index4, BitmapOrder.@"1234", 0, 4, 0),
    index8 = definePixelFormatEnum(.index8, BitmapOrder.none, 0, 8, 1),
    rgb332 = definePixelFormatEnum(.packed8, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"332"), 8, 1),
    xrgb4444 = definePixelFormatEnum(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"4444"), 12, 2),
    xbgr4444 = definePixelFormatEnum(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"4444"), 12, 2),
    xrgb1555 = definePixelFormatEnum(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"1555"), 15, 2),
    xbgr1555 = definePixelFormatEnum(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"1555"), 15, 2),
    argb4444 = definePixelFormatEnum(.packed16, PackedOrder.argb, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    rgba4444 = definePixelFormatEnum(.packed16, PackedOrder.rgba, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    abgr4444 = definePixelFormatEnum(.packed16, PackedOrder.abgr, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    bgra4444 = definePixelFormatEnum(.packed16, PackedOrder.bgra, @intFromEnum(PackedLayout.@"4444"), 16, 2),
    argb1555 = definePixelFormatEnum(.packed16, PackedOrder.argb, @intFromEnum(PackedLayout.@"1555"), 16, 2),
    rgba5551 = definePixelFormatEnum(.packed16, PackedOrder.rgba, @intFromEnum(PackedLayout.@"5551"), 16, 2),
    abgr1555 = definePixelFormatEnum(.packed16, PackedOrder.abgr, @intFromEnum(PackedLayout.@"1555"), 16, 2),
    bgra5551 = definePixelFormatEnum(.packed16, PackedOrder.bgra, @intFromEnum(PackedLayout.@"5551"), 16, 2),
    rgb565 = definePixelFormatEnum(.packed16, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"565"), 16, 2),
    bgr565 = definePixelFormatEnum(.packed16, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"565"), 16, 2),
    rgb24 = definePixelFormatEnum(.arrayu8, ArrayOrder.rgb, 0, 24, 3),
    bgr24 = definePixelFormatEnum(.arrayu8, ArrayOrder.bgr, 0, 24, 3),
    xrgb8888 = definePixelFormatEnum(.packed32, PackedOrder.xrgb, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    rgbx8888 = definePixelFormatEnum(.packed32, PackedOrder.rgbx, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    xbgr8888 = definePixelFormatEnum(.packed32, PackedOrder.xbgr, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    bgrx8888 = definePixelFormatEnum(.packed32, PackedOrder.bgrx, @intFromEnum(PackedLayout.@"8888"), 24, 4),
    argb8888 = definePixelFormatEnum(.packed32, PackedOrder.argb, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    rgba8888 = definePixelFormatEnum(.packed32, PackedOrder.rgba, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    abgr8888 = definePixelFormatEnum(.packed32, PackedOrder.abgr, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    bgra8888 = definePixelFormatEnum(.packed32, PackedOrder.bgra, @intFromEnum(PackedLayout.@"8888"), 32, 4),
    argb2101010 = definePixelFormatEnum(.packed32, PackedOrder.argb, @intFromEnum(PackedLayout.@"2101010"), 32, 4),
};

pub const Color = extern struct {
    r: u8,
    g: u8,
    b: u8,
    a: u8,
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
    pub const hasIntersection = sdl2.hasIntersection;
    pub const intersectRect = sdl2.intersectRect;
    pub const intersectRectAndLine = sdl2.intersectRectAndLine;
};

/// Determine whether two rectangles intersect.
pub fn hasIntersection(a: *const Rect, b: *const Rect) bool {
    return SDL_HasIntersection(a, b) == True;
}
extern fn SDL_HasIntersection(a: *const Rect, b: *const Rect) Bool;

/// Calculate the intersection of a rectangle and line segment.
pub fn intersectRect(a: *const Rect, b: *const Rect, result: *Rect) bool {
    return SDL_IntersectRect(a, b, result) == True;
}
extern fn SDL_IntersectRect(a: *const Rect, b: *const Rect, result: *Rect) Bool;

/// Calculate the intersection of a rectangle and line segment.
pub fn intersectRectAndLine(rect: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) bool {
    return SDL_IntersectRectAndLine(rect, x1, y1, x2, y2) == True;
}
extern fn SDL_IntersectRectAndLine(r: *const Rect, x1: *i32, y1: *i32, x2: *i32, y2: *i32) Bool;

pub const FRect = extern struct {
    x: f32,
    y: f32,
    w: f32,
    h: f32,
    pub const hasIntersection = hasIntersectionF;
    pub const intersectRect = intersectFRect;
    pub const intersectRectAndLine = intersectFRectAndLine;
};

/// Determine whether two rectangles intersect with float precision.
pub fn hasIntersectionF(a: *const FRect, b: *const FRect) bool {
    return SDL_HasIntersectionF(a, b) == True;
}
extern fn SDL_HasIntersectionF(a: *const FRect, b: *const FRect) Bool;

/// Calculate the intersection of two rectangles.
pub fn intersectFRect(a: *const FRect, b: *const FRect, result: *FRect) bool {
    return SDL_IntersectFRect(a, b, result) == True;
}
extern fn SDL_IntersectFRect(a: *const FRect, b: *const FRect, result: *FRect) Bool;

/// Calculate the intersection of a rectangle and line segment with float precision.
pub fn intersectFRectAndLine(rect: *const FRect, x1: *f32, y1: *f32, x2: *f32, y2: *f32) bool {
    return SDL_IntersectFRectAndLine(rect, x1, y1, x2, y2) == True;
}
extern fn SDL_IntersectFRectAndLine(r: *const FRect, x1: *f32, y1: *f32, x2: *f32, y2: *f32) Bool;

pub const Point = extern struct {
    x: i32,
    y: i32,
};

pub const FPoint = extern struct {
    x: f32,
    y: f32,
};

//--------------------------------------------------------------------------------------------------
//
// Surface Creation and Simple Drawing
//
//--------------------------------------------------------------------------------------------------
pub const Surface = extern struct {
    /// Read-only
    flags: u32,

    /// Read-only
    format: ?*PixelFormatEnum,

    /// Read-only
    w: c_int,

    /// Read-only
    h: c_int,

    /// Read-only
    pitch: c_int,

    /// Read-write
    pixels: ?*anyopaque,

    /// Application data associated with the surface (Read-write)
    userdate: ?*anyopaque,

    /// information needed for surfaces requiring locks (Read-only)
    locked: c_int,

    /// list of BlitMap that hold a reference to this surface (Private)
    list_blitmap: ?*anyopaque,

    /// clipping information (Read-only)
    clip_rect: Rect,

    /// info for fast blit mapping to other surfaces (Private)
    map: ?*anyopaque,

    /// Reference count -- used when freeing surface (Read-mostly)
    refcount: c_int,

    pub const free = freeSurface;
    pub const blit = blitSurface;
};

/// Free an RGB surface.
pub fn freeSurface(surface: *Surface) void {
    SDL_FreeSurface(surface);
}
extern fn SDL_FreeSurface(*Surface) void;

/// Performs a fast blit from the source surface to the destination surface.
pub fn blitSurface(
    src_surface: *Surface,
    src_rect: ?*const Rect,
    dest_surface: *Surface,
    dest_rect: ?*const Rect,
) !void {
    if (SDL_BlitSurface(
        src_surface,
        src_rect,
        dest_surface,
        dest_rect,
    ) != 0) return makeError();
}
const SDL_BlitSurface = SDL_UpperBlit;
extern fn SDL_UpperBlit(
    src_surface: *Surface,
    src_rect: ?*const Rect,
    dest_surface: *Surface,
    dest_rect: ?*const Rect,
) c_int;

//--------------------------------------------------------------------------------------------------
//
// Platform-specific Window Management
//
//--------------------------------------------------------------------------------------------------
pub const SysWMType = enum(i32) {
    unknown,
    windows,
    x11,
    directfb,
    cocoa,
    uikit,
    wayland,
    mir, // no longer available, left for api/abi compatibility. remove in 2.1!
    winrt,
    android,
    vivante,
    os2,
    haiku,
    kmsdrm,
    riscos,
};

pub const SysWMInfo = extern struct {
    version: Version,
    subsystem: SysWMType,
    info: extern union {
        win: extern struct {
            hwnd: *opaque {},
            hdc: *opaque {},
            hinstance: *opaque {},
        },
        x11: extern struct {
            display: *opaque {},
            window: *opaque {},
        },
        winrt: extern struct {
            window: *opaque {},
        },
        dfb: extern struct {
            dfb: *opaque {},
            window: *opaque {},
            surface: *opaque {},
        },
        cocoa: extern struct {
            window: *opaque {},
        },
        uikit: extern struct {
            window: *opaque {},
            framebuffer: c_uint,
            colorbuffer: c_uint,
            resolveFramebuffer: c_uint,
        },
        wl: extern struct {
            display: *opaque {},
            surface: *opaque {},
            shell_surface: *opaque {},
        },
        android: extern struct {
            window: *opaque {},
            surface: *opaque {},
        },
        vivante: extern struct {
            display: *opaque {},
            window: *opaque {},
        },
        dummy: [64]u8,
        // MIR -- SDL unsupported and recommended to drop after 2.1

        comptime {
            assert(@sizeOf(@This()) == 64);
        }
    },
};

/// Get driver-specific information about a window.
///
/// The caller must initialize the `info` structure's version by using
/// `VERSION`, and then this function will fill in the rest
/// of the structure with information about the given window.
///
/// returns true if the function is implemented and the `version` member
/// of the `info` struct is valid, or false if the information
/// could not be retrieved
pub fn getWindowWMInfo(window: *Window, info: *SysWMInfo) bool {
    return SDL_GetWindowWMInfo(window, info) == True;
}
extern fn SDL_GetWindowWMInfo(window: *Window, info: *SysWMInfo) Bool;

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

/// Returns true if event was added
///         false if event was filtered out
pub fn pushEvent(event: *Event) Error!bool {
    const status = SDL_PushEvent(event);
    if (status < 0) return makeError();
    return status == 1;
}
extern fn SDL_PushEvent(event: *Event) i32;

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

pub fn showCursor(toggle: enum(i32) { enable = 1, disable = 0 }) Error!void {
    if (SDL_ShowCursor(@intFromEnum(toggle)) < 0) return makeError();
}
extern fn SDL_ShowCursor(toggle: c_int) c_int;

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
    pub const open = gameControllerOpen;
    pub const close = gameControllerClose;
    pub const getAxis = gameControllerGetAxis;
    pub const getButton = gameControllerGetButton;
};

/// Open a game controller for use.
pub fn gameControllerOpen(joystick_index: i32) ?*GameController {
    return SDL_GameControllerOpen(joystick_index);
}
extern fn SDL_GameControllerOpen(joystick_index: i32) ?*GameController;

/// Close a game controller previously opened with SDL_GameControllerOpen().
pub fn gameControllerClose(controller: *GameController) void {
    SDL_GameControllerClose(controller);
}
extern fn SDL_GameControllerClose(joystick: *GameController) void;

/// Get the current state of an axis control on a game controller.
pub fn gameControllerGetAxis(controller: *GameController, axis: GameController.Axis) i16 {
    return SDL_GameControllerGetAxis(controller, @intFromEnum(axis));
}
extern fn SDL_GameControllerGetAxis(*GameController, axis: c_int) i16;

/// Get the current state of a button on a game controller.
pub fn gameControllerGetButton(controller: *GameController, button: GameController.Button) bool {
    return (SDL_GameControllerGetButton(controller, @intFromEnum(button)) != 0);
}
extern fn SDL_GameControllerGetButton(controller: *GameController, button: c_int) u8;

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
    .little => AUDIO_U16LSB,
    .big => AUDIO_U16MSB,
};
pub const AUDIO_S16SYS = switch (builtin.target.cpu.arch.endian()) {
    .little => AUDIO_S16LSB,
    .big => AUDIO_S16MSB,
};
pub const AUDIO_S32SYS = switch (builtin.target.cpu.arch.endian()) {
    .little => AUDIO_S32LSB,
    .big => AUDIO_S32MSB,
};
pub const AUDIO_F32SYS = switch (builtin.target.cpu.arch.endian()) {
    .little => AUDIO_F32LSB,
    .big => AUDIO_F32MSB,
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

pub fn queueAudio(
    comptime SampleType: type,
    device: AudioDeviceId,
    data: []const SampleType,
) Error!void {
    if (SDL_QueueAudio(device, data.ptr, @sizeOf(SampleType) * @as(u32, @intCast(data.len))) != 0) {
        return makeError();
    }
}
extern fn SDL_QueueAudio(AudioDeviceId, data: *const anyopaque, len: u32) c_int;

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
// Thread Synchronization Primitives
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
pub const Bool = c_int;
pub const False = @as(Bool, 0);
pub const True = @as(Bool, 1);
