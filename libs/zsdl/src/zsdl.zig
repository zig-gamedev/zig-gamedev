const std = @import("std");

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

pub fn getError() ?[:0]const u8 {
    if (SDL_GetError()) |cstr| {
        return std.mem.sliceTo(cstr, 0);
    } else {
        return null;
    }
}
extern fn SDL_GetError() ?[*:0]const u8;

pub const Error = error{SdlError};

pub fn makeError() error{SdlError} {
    if (getError()) |str| {
        std.log.debug("SDL2: {s}", .{str});
    }
    return error.SdlError;
}

pub const Window = opaque {
    pub const Flags = packed struct(u32) {
        fullscreen: bool = false,
        opengl: bool = false,
        shown: bool = false,
        hidden: bool = false,
        borderless: bool = false,
        resizable: bool = false,
        minimized: bool = false,
        maximized: bool = false,
        mouse_grabbed: bool = false,
        input_focus: bool = false,
        mouse_focus: bool = false,
        foreign: bool = false,
        _desktop: bool = false,
        allow_highdpi: bool = false,
        mouse_capture: bool = false,
        always_on_top: bool = false,
        skip_taskbar: bool = false,
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
};

pub const GlContext = *anyopaque;

pub const GlAttr = enum(i32) {
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

pub const GlProfile = enum(i32) {
    core = 0x0001,
    compatibility = 0x0002,
    es = 0x0004,
};

pub const GlContextFlags = packed struct(i32) {
    debug: bool = false,
    forward_compatible: bool = false,
    robust_access: bool = false,
    reset_isolation: bool = false,
    __unused: i28 = 0,
};

pub const GlContextReleaseFlags = packed struct(i32) {
    flush: bool = false,
    __unused: i31 = 0,
};

pub const GlContextResetNotification = enum(i32) {
    no_notification = 0x0000,
    lose_context = 0x0001,
};

pub fn setGlAttr(attr: GlAttr, value: i32) Error!void {
    if (SDL_GL_SetAttribute(attr, value) < 0) return makeError();
}
extern fn SDL_GL_SetAttribute(attr: GlAttr, value: i32) i32;

pub fn getGlAttr(attr: GlAttr) Error!i32 {
    var value: i32 = undefined;
    if (SDL_GL_GetAttribute(attr, &value) < 0) return makeError();
    return value;
}
extern fn SDL_GL_GetAttribute(attr: GlAttr, value: i32) i32;

pub fn setGlSwapInterval(interval: i32) Error!void {
    if (SDL_GL_SetSwapInterval(interval) < 0) return makeError();
}
extern fn SDL_GL_SetSwapInterval(interval: i32) i32;

/// `pub fn getGlSwapInterval() i32`
pub const getGlSwapInterval = SDL_GL_GetSwapInterval;
extern fn SDL_GL_GetSwapInterval() i32;

/// `pub fn swapGlWindow(window: *Window) void`
pub const swapGlWindow = SDL_GL_SwapWindow;
extern fn SDL_GL_SwapWindow(window: *Window) void;

pub fn getGlProcAddress(proc: [:0]const u8) ?*anyopaque {
    return SDL_GL_GetProcAddress(proc);
}
extern fn SDL_GL_GetProcAddress(proc: ?[*:0]const u8) ?*anyopaque;

pub fn isGlExtensionSupported(extension: [:0]const u8) bool {
    return SDL_GL_ExtensionSupported(extension) != 0;
}
extern fn SDL_GL_ExtensionSupported(extension: ?[*:0]const u8) i32;

pub fn createGlContext(window: *Window) Error!GlContext {
    return SDL_GL_CreateContext(window) orelse return makeError();
}
extern fn SDL_GL_CreateContext(window: *Window) ?GlContext;

pub fn makeGlContextCurrent(window: *Window, context: GlContext) Error!void {
    if (SDL_GL_MakeCurrent(window, context) < 0) return makeError();
}
extern fn SDL_GL_MakeCurrent(window: *Window, context: GlContext) i32;

/// `pub fn deleteGlContext(context: GlContext) void`
pub const deleteGlContext = SDL_GL_DeleteContext;
extern fn SDL_GL_DeleteContext(context: GlContext) void;
