const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;
//--------------------------------------------------------------------------------------------------
//
// Init
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
// Error
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
// Video driver
//
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
//
// Display
//
//--------------------------------------------------------------------------------------------------
pub const DisplayId = u32;

pub const DisplayMode = DisplayMode_SDL2;

const DisplayMode_SDL2 = extern struct {
    format: u32,
    w: i32,
    h: i32,
    refresh_rate: i32,
    driverdata: ?*anyopaque,
};

const DisplayMode_SDL3 = extern struct {
    display_id: DisplayId,
    format: u32,
    pixel_w: i32,
    pixel_h: i32,
    screen_w: i32,
    screen_h: i32,
    display_scale: f32,
    refresh_rate: f32,
    driverdata: ?*anyopaque,
};
//--------------------------------------------------------------------------------------------------
//
// Window
//
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
//
// Events
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

pub const MouseWheelDirection = enum(u32) {
    normal,
    flipped,
};

pub const Scancode = @import("keyboard.zig").Scancode;

pub const Keycode = @import("keyboard.zig").Keycode;

pub const Keysym = extern struct {
    scancode: Scancode,
    sym: Keycode,
    mod: u16,
    unused: u32,
};

pub const CommonEvent = extern struct {
    type: EventType,
    timestamp: u32,
};

pub const DisplayEvent = extern struct {
    type: EventType,
    timestamp: u32,
    display: u32,
    event: DisplayEventId,
    padding1: u8,
    padding2: u8,
    padding3: u8,
    data1: i32,
};

pub const WindowEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
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
    window_id: u32,
    state: ReleasedOrPressed,
    repeat: u8,
    padding2: u8,
    padding3: u8,
    keysym: Keysym,
};

pub const TextEditingEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [text_size]u8,
    start: i32,
    length: i32,

    const text_size = 32;
};

pub const TextEditingExtEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [*:0]u8,
    start: i32,
    length: i32,
};

pub const TextInputEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [text_size]u8,

    const text_size = 32;
};

pub const MouseMotionEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    which: u32,
    state: u32,
    x: i32,
    y: i32,
    xrel: i32,
    yrel: i32,
};

pub const MouseButtonEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    which: u32,
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
    window_id: u32,
    which: u32,
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
    window_id: u32,
};

pub const ControllerDeviceEvent = extern struct {
    type: EventType,
    timestamp: u32,
    which: i32,
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

pub const JOYSTICK_AXIS_MAX = 32767;
pub const JOYSTICK_AXIS_MIN = -32768;

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
        return SDL_GameControllerGetAxis(controller, @enumToInt(axis));
    }
    extern fn SDL_GameControllerGetAxis(*GameController, axis: c_int) i16;

    pub fn getButton(controller: *GameController, button: Button) bool {
        return (SDL_GameControllerGetButton(controller, @enumToInt(button)) != 0);
    }
    extern fn SDL_GameControllerGetButton(controller: *GameController, button: c_int) u8;
};

pub fn pollEvent(event: ?*Event) bool {
    return SDL_PollEvent(event) != 0;
}
extern fn SDL_PollEvent(event: ?*Event) i32;

/// `pub fn SDL_GetKeyboardState(numkeys: ?*i32) ?[*]const u8`
pub fn getKeyboardState() []const u8 {
    var numkeys: i32 = 0;
    const ptr = SDL_GetKeyboardState(&numkeys).?;
    return ptr[0..@intCast(usize, numkeys)];
}
extern fn SDL_GetKeyboardState(numkeys: ?*i32) ?[*]const u8;

/// `pub fn getMouseFocus() ?*Window`
pub const getMouseFocus = SDL_GetMouseFocus;
extern fn SDL_GetMouseFocus() ?*Window;

/// `pub fn getMouseState(x: ?*i32, y: ?*i32) u32`
pub const getMouseState = SDL_GetMouseState;
extern fn SDL_GetMouseState(x: ?*i32, y: ?*i32) u32;
//--------------------------------------------------------------------------------------------------
//
// Hints
//
//--------------------------------------------------------------------------------------------------
pub const hint_windows_dpi_awareness = "SDL_WINDOWS_DPI_AWARENESS";

pub fn setHint(name: [:0]const u8, value: [:0]const u8) bool {
    return SDL_SetHint(name, value) != 0;
}
extern fn SDL_SetHint(name: [*:0]const u8, value: [*:0]const u8) i32;
//--------------------------------------------------------------------------------------------------
//
// Message box
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
// Audio
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
    return SDL_QueueAudio(device, data.ptr, @sizeOf(SampleType) * @intCast(u32, data.len)) == 0;
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
// Timer
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
// File Abstraction
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
// OpenGL
//
//--------------------------------------------------------------------------------------------------
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
