const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

const options = @import("zglfw_options");

comptime {
    _ = std.testing.refAllDeclsRecursive(@This());
}

//--------------------------------------------------------------------------------------------------
//
// Misc
//
//--------------------------------------------------------------------------------------------------
pub const Hint = enum(i32) {
    joystick_hat_buttons = 0x00050001,
    angle_platform_type = 0x00050002,
    platform = 0x00050003,
    cocoa_chdir_resources = 0x00051001,
    cocoa_menubar = 0x00051002,
    x11_xcb_vulkan_surface = 0x00052001,
    wayland_libdecor = 0x00053001,

    pub fn set(hint: Hint, value: bool) void {
        glfwInitHint(hint, @intFromBool(value));
    }
    extern fn glfwInitHint(hint: Hint, value: i32) void;
};

pub fn init() Error!void {
    if (glfwInit() != 0) return;
    try maybeError();
    unreachable;
}
extern fn glfwInit() i32;

/// `pub fn terminate() void`
pub const terminate = glfwTerminate;
extern fn glfwTerminate() void;

/// `pub fn pollEvents() void`
pub const pollEvents = glfwPollEvents;
extern fn glfwPollEvents() void;

/// `pub fn waitEvents() void`
pub const waitEvents = glfwWaitEvents;
extern fn glfwWaitEvents() void;

/// `pub fn waitEventsTimeout(timeout: f64) void`
pub const waitEventsTimeout = glfwWaitEventsTimeout;
extern fn glfwWaitEventsTimeout(timeout: f64) void;

pub fn isVulkanSupported() bool {
    return if (glfwVulkanSupported() == 0) false else true;
}
extern fn glfwVulkanSupported() i32;

pub fn getRequiredInstanceExtensions() Error![][*:0]const u8 {
    var count: u32 = 0;
    if (glfwGetRequiredInstanceExtensions(&count)) |extensions| {
        return @as([*][*:0]const u8, @ptrCast(extensions))[0..count];
    }
    try maybeError();
    return error.APIUnavailable;
}
extern fn glfwGetRequiredInstanceExtensions(count: *u32) ?*?[*:0]const u8;

/// `pub fn getTime() f64`
pub const getTime = glfwGetTime;
extern fn glfwGetTime() f64;

/// `pub fn setTime(time: f64) void`
pub const setTime = glfwSetTime;
extern fn glfwSetTime(time: f64) void;

pub const Error = error{
    NotInitialized,
    NoCurrentContext,
    InvalidEnum,
    InvalidValue,
    OutOfMemory,
    APIUnavailable,
    VersionUnavailable,
    PlatformError,
    FormatUnavailable,
    NoWindowContext,
    CursorUnavailable,
    FeatureUnavailable,
    FeatureUnimplemented,
    PlatformUnavailable,
    Unknown,
};

fn convertError(e: i32) Error!void {
    return switch (e) {
        0 => {},
        0x00010001 => Error.NotInitialized,
        0x00010002 => Error.NoCurrentContext,
        0x00010003 => Error.InvalidEnum,
        0x00010004 => Error.InvalidValue,
        0x00010005 => Error.OutOfMemory,
        0x00010006 => Error.APIUnavailable,
        0x00010007 => Error.VersionUnavailable,
        0x00010008 => Error.PlatformError,
        0x00010009 => Error.FormatUnavailable,
        0x0001000A => Error.NoWindowContext,
        0x0001000B => Error.CursorUnavailable,
        0x0001000C => Error.FeatureUnavailable,
        0x0001000D => Error.FeatureUnimplemented,
        0x0001000E => Error.PlatformUnavailable,
        else => Error.Unknown,
    };
}

pub fn maybeError() Error!void {
    return convertError(glfwGetError(null));
}
pub fn maybeErrorString(str: *?[:0]const u8) Error!void {
    var c_str: ?[*:0]const u8 = undefined;
    convertError(glfwGetError(&c_str)) catch |err| {
        str.* = if (c_str) |s| std.mem.span(s) else null;
        return err;
    };
}
extern fn glfwGetError(description: ?*?[*:0]const u8) i32;

/// `pub fn setErrorCallback(callback: ?ErrorFn) ?ErrorFn`
pub const setErrorCallback = glfwSetErrorCallback;
extern fn glfwSetErrorCallback(callback: ?ErrorFn) ?ErrorFn;
pub const ErrorFn = *const fn (
    error_code: i32,
    description: *?[:0]const u8,
) callconv(.C) void;

pub const InputMode = enum(i32) {
    cursor = 0x00033001,
    sticky_keys = 0x00033002,
    sticky_mouse_buttons = 0x00033003,
    lock_key_mods = 0x00033004,
    raw_mouse_motion = 0x00033005,
};

pub fn rawMouseMotionSupported() bool {
    return glfwRawMouseMotionSupported() == 1;
}
extern fn glfwRawMouseMotionSupported() i32;

pub const makeContextCurrent = glfwMakeContextCurrent;
extern fn glfwMakeContextCurrent(window: *Window) void;

pub const getCurrentContext = glfwGetCurrentContext;
extern fn glfwGetCurrentContext() *Window;

pub const swapInterval = glfwSwapInterval;
extern fn glfwSwapInterval(interval: i32) void;

pub const GlProc = *const anyopaque;

pub fn getProcAddress(procname: [*:0]const u8) ?GlProc {
    return glfwGetProcAddress(procname);
}
extern fn glfwGetProcAddress(procname: [*:0]const u8) ?GlProc;

//--------------------------------------------------------------------------------------------------
//
// Keyboard/Mouse
//
//--------------------------------------------------------------------------------------------------
pub const Action = enum(i32) {
    release,
    press,
    repeat,
};

pub const MouseButton = enum(i32) {
    left,
    right,
    middle,
    four,
    five,
    six,
    seven,
    eight,
};

pub const Key = enum(i32) {
    unknown = -1,

    space = 32,
    apostrophe = 39,
    comma = 44,
    minus = 45,
    period = 46,
    slash = 47,
    zero = 48,
    one = 49,
    two = 50,
    three = 51,
    four = 52,
    five = 53,
    six = 54,
    seven = 55,
    eight = 56,
    nine = 57,
    semicolon = 59,
    equal = 61,
    a = 65,
    b = 66,
    c = 67,
    d = 68,
    e = 69,
    f = 70,
    g = 71,
    h = 72,
    i = 73,
    j = 74,
    k = 75,
    l = 76,
    m = 77,
    n = 78,
    o = 79,
    p = 80,
    q = 81,
    r = 82,
    s = 83,
    t = 84,
    u = 85,
    v = 86,
    w = 87,
    x = 88,
    y = 89,
    z = 90,
    left_bracket = 91,
    backslash = 92,
    right_bracket = 93,
    grave_accent = 96,
    world_1 = 161,
    world_2 = 162,

    escape = 256,
    enter = 257,
    tab = 258,
    backspace = 259,
    insert = 260,
    delete = 261,
    right = 262,
    left = 263,
    down = 264,
    up = 265,
    page_up = 266,
    page_down = 267,
    home = 268,
    end = 269,
    caps_lock = 280,
    scroll_lock = 281,
    num_lock = 282,
    print_screen = 283,
    pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    kp_0 = 320,
    kp_1 = 321,
    kp_2 = 322,
    kp_3 = 323,
    kp_4 = 324,
    kp_5 = 325,
    kp_6 = 326,
    kp_7 = 327,
    kp_8 = 328,
    kp_9 = 329,
    kp_decimal = 330,
    kp_divide = 331,
    kp_multiply = 332,
    kp_subtract = 333,
    kp_add = 334,
    kp_enter = 335,
    kp_equal = 336,
    left_shift = 340,
    left_control = 341,
    left_alt = 342,
    left_super = 343,
    right_shift = 344,
    right_control = 345,
    right_alt = 346,
    right_super = 347,
    menu = 348,
};

pub const Mods = packed struct(i32) {
    shift: bool = false,
    control: bool = false,
    alt: bool = false,
    super: bool = false,
    caps_lock: bool = false,
    num_lock: bool = false,
    _padding: i26 = 0,
};
//--------------------------------------------------------------------------------------------------
//
// Cursor
//
//--------------------------------------------------------------------------------------------------
pub const Cursor = opaque {
    pub const Shape = enum(i32) {
        arrow = 0x00036001,
        ibeam = 0x00036002,
        crosshair = 0x00036003,
        hand = 0x00036004,
        /// Previously named hresize
        resize_ew = 0x00036005,
        /// Previously named vresize
        resize_ns = 0x00036006,
        resize_nwse = 0x00036007,
        resize_nesw = 0x00036008,
        resize_all = 0x00036009,
        not_allowed = 0x0003600A,
    };

    pub const Mode = enum(i32) {
        normal = 0x00034001,
        hidden = 0x00034002,
        disabled = 0x00034003,
        captured = 0x00034004,
    };

    /// `pub fn destroy(cursor: *Cursor) void`
    pub const destroy = glfwDestroyCursor;
    extern fn glfwDestroyCursor(cursor: *Cursor) void;

    pub fn create(width: i32, height: i32, pixels: []const u8, xhot: i32, yhot: i32) Error!*Cursor {
        assert(pixels.len == 4 * width * height);
        if (glfwCreateCursor(&.{
            .width = width,
            .height = height,
            .pixels = @constCast(pixels.ptr),
        }, xhot, yhot)) |ptr| return ptr;
        try maybeError();
        unreachable;
    }
    extern fn glfwCreateCursor(image: *const Image, xhot: c_int, yhot: c_int) ?*Cursor;

    pub fn createStandard(shape: Shape) Error!*Cursor {
        if (glfwCreateStandardCursor(shape)) |ptr| return ptr;
        try maybeError();
        unreachable;
    }
    extern fn glfwCreateStandardCursor(shape: Shape) ?*Cursor;
};
//--------------------------------------------------------------------------------------------------
//
// Joystick
//
//--------------------------------------------------------------------------------------------------
pub const Joystick = struct {
    jid: Id,

    pub const Id = u4;

    pub const maximum_supported = std.math.maxInt(Id) + 1;

    pub const ButtonAction = enum(u8) {
        release = 0,
        press = 1,
    };

    pub fn getGuid(self: Joystick) [:0]const u8 {
        return std.mem.span(glfwGetJoystickGUID(@as(i32, @intCast(self.jid))));
    }
    extern fn glfwGetJoystickGUID(jid: i32) [*:0]const u8;

    pub fn getAxes(self: Joystick) []const f32 {
        var count: i32 = undefined;
        const state = glfwGetJoystickAxes(@as(i32, @intCast(self.jid)), &count);
        if (count == 0) {
            return @as([*]const f32, undefined)[0..0];
        }
        return state[0..@as(usize, @intCast(count))];
    }
    extern fn glfwGetJoystickAxes(jid: i32, count: *i32) [*]const f32;

    pub fn getButtons(self: Joystick) []const ButtonAction {
        var count: i32 = undefined;
        const state = glfwGetJoystickButtons(@as(i32, @intCast(self.jid)), &count);
        if (count == 0) {
            return @as([*]const ButtonAction, undefined)[0..0];
        }
        return @as([]const ButtonAction, @ptrCast(state[0..@as(usize, @intCast(count))]));
    }
    extern fn glfwGetJoystickButtons(jid: i32, count: *i32) [*]const u8;

    fn isGamepad(self: Joystick) bool {
        return glfwJoystickIsGamepad(@as(i32, @intCast(self.jid))) == @intFromBool(true);
    }

    pub fn asGamepad(self: Joystick) ?Gamepad {
        return if (self.isGamepad()) .{ .jid = self.jid } else null;
    }
    extern fn glfwJoystickIsGamepad(jid: i32) i32;

    pub fn isPresent(jid: Id) bool {
        return glfwJoystickPresent(@as(i32, @intCast(jid))) == @intFromBool(true);
    }
    extern fn glfwJoystickPresent(jid: i32) i32;

    pub fn get(jid: Id) ?Joystick {
        if (!isPresent(jid)) {
            return null;
        }
        return .{ .jid = jid };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Gamepad
//
//--------------------------------------------------------------------------------------------------
pub const Gamepad = struct {
    jid: Joystick.Id,

    pub const Axis = enum(u8) {
        left_x = 0,
        left_y = 1,
        right_x = 2,
        right_y = 3,
        left_trigger = 4,
        right_trigger = 5,

        const last = Axis.right_trigger;
    };

    pub const Button = enum(u8) {
        a = 0,
        b = 1,
        x = 2,
        y = 3,
        left_bumper = 4,
        right_bumper = 5,
        back = 6,
        start = 7,
        guide = 8,
        left_thumb = 9,
        right_thumb = 10,
        dpad_up = 11,
        dpad_right = 12,
        dpad_down = 13,
        dpad_left = 14,

        const last = Button.dpad_left;

        const cross = Button.a;
        const circle = Button.b;
        const square = Button.x;
        const triangle = Button.y;
    };

    pub const State = extern struct {
        comptime {
            const c = @cImport(@cInclude("GLFW/glfw3.h"));
            assert(@sizeOf(c.GLFWgamepadstate) == @sizeOf(State));
            for (std.meta.fieldNames(State)) |field_name| {
                assert(@offsetOf(c.GLFWgamepadstate, field_name) == @offsetOf(State, field_name));
            }
        }
        buttons: [15]Joystick.ButtonAction,
        axes: [6]f32,
    };

    pub fn getName(self: Gamepad) [:0]const u8 {
        return std.mem.span(glfwGetGamepadName(@as(i32, @intCast(self.jid))));
    }
    extern fn glfwGetGamepadName(jid: i32) [*:0]const u8;

    pub fn getState(self: Gamepad) State {
        var state: State = undefined;
        _ = glfwGetGamepadState(@as(i32, @intCast(self.jid)), &state);
        // return value of glfwGetGamepadState is ignored as
        // it is expected this is guarded by glfwJoystickIsGamepad
        return state;
    }
    extern fn glfwGetGamepadState(jid: i32, state: *Gamepad.State) i32;

    pub fn updateMappings(mappings: [:0]const u8) bool {
        return glfwUpdateGamepadMappings(mappings) == @intFromBool(true);
    }
    extern fn glfwUpdateGamepadMappings(mappings: [*:0]const u8) i32;
};
//--------------------------------------------------------------------------------------------------
//
// Monitor
//
//--------------------------------------------------------------------------------------------------
pub const Monitor = opaque {
    pub fn getPos(monitor: *Monitor) [2]i32 {
        var xpos: i32 = 0;
        var ypos: i32 = 0;
        glfwGetMonitorPos(monitor, &xpos, &ypos);
        return .{ xpos, ypos };
    }
    extern fn glfwGetMonitorPos(monitor: *Monitor, xpos: *i32, ypos: *i32) void;

    /// `pub fn getPrimary() ?*Monitor`
    pub const getPrimary = glfwGetPrimaryMonitor;
    extern fn glfwGetPrimaryMonitor() ?*Monitor;

    pub fn getAll() ?[]*Monitor {
        var count: i32 = 0;
        if (glfwGetMonitors(&count)) |monitors| {
            return monitors[0..@as(usize, @intCast(count))];
        }
        return null;
    }
    extern fn glfwGetMonitors(count: *i32) ?[*]*Monitor;

    pub fn getName(monitor: *Monitor) Error![*:0]const u8 {
        if (glfwGetMonitorName(monitor)) |name| {
            return name;
        }
        try maybeError();
        unreachable;
    }
    extern fn glfwGetMonitorName(monitor: *Monitor) ?[*:0]const u8;

    pub fn getVideoMode(monitor: *Monitor) Error!*VideoMode {
        if (glfwGetVideoMode(monitor)) |video_mode| return video_mode;
        try maybeError();
        unreachable;
    }
    extern fn glfwGetVideoMode(monitor: *Monitor) ?*VideoMode;

    pub fn getVideoModes(monitor: *Monitor) Error![]VideoMode {
        var count: i32 = 0;
        if (glfwGetVideoModes(monitor, &count)) |video_modes| {
            return video_modes[0..@as(usize, @intCast(count))];
        }
        try maybeError();
        unreachable;
    }
    extern fn glfwGetVideoModes(monitor: *Monitor, count: *i32) ?[*]VideoMode;
};

pub const VideoMode = extern struct {
    comptime {
        const c = @cImport(@cInclude("GLFW/glfw3.h"));
        assert(@sizeOf(c.GLFWvidmode) == @sizeOf(VideoMode));
        for (std.meta.fieldNames(VideoMode), 0..) |field_name, i| {
            assert(@offsetOf(c.GLFWvidmode, std.meta.fieldNames(c.GLFWvidmode)[i]) ==
                @offsetOf(VideoMode, field_name));
        }
    }
    width: c_int,
    height: c_int,
    red_bits: c_int,
    green_bits: c_int,
    blue_bits: c_int,
    refresh_rate: c_int,
};
//--------------------------------------------------------------------------------------------------
//
// Image
//
//--------------------------------------------------------------------------------------------------
pub const Image = extern struct {
    comptime {
        const c = @cImport(@cInclude("GLFW/glfw3.h"));
        assert(@sizeOf(c.GLFWimage) == @sizeOf(Image));
        for (std.meta.fieldNames(Image)) |field_name| {
            assert(@offsetOf(c.GLFWimage, field_name) == @offsetOf(Image, field_name));
        }
    }
    width: c_int,
    height: c_int,
    pixels: [*]u8,
};
//--------------------------------------------------------------------------------------------------
//
// Window
//
//--------------------------------------------------------------------------------------------------
pub const Window = opaque {
    pub const Attribute = enum(i32) {
        focused = 0x00020001,
        iconified = 0x00020002,
        resizable = 0x00020003,
        visible = 0x00020004,
        decorated = 0x00020005,
        auto_iconify = 0x00020006,
        floating = 0x00020007,
        maximized = 0x00020008,
        center_cursor = 0x00020009,
        transparent_framebuffer = 0x0002000A,
        hovered = 0x0002000B,
        focus_on_show = 0x0002000C,
    };
    pub fn getAttribute(window: *Window, attrib: Attribute) bool {
        return glfwGetWindowAttrib(window, attrib) != 0;
    }
    extern fn glfwGetWindowAttrib(window: *Window, attrib: Attribute) i32;

    pub fn setAttribute(window: *Window, attrib: Attribute, value: bool) void {
        glfwSetWindowAttrib(window, attrib, @intFromBool(value));
    }
    extern fn glfwSetWindowAttrib(window: *Window, attrib: Attribute, value: i32) void;

    pub fn getUserPointer(window: *Window, comptime T: type) ?*T {
        return @ptrCast(@alignCast(glfwGetWindowUserPointer(window)));
    }
    extern fn glfwGetWindowUserPointer(window: *Window) ?*anyopaque;

    pub fn setUserPointer(window: *Window, pointer: ?*anyopaque) void {
        glfwSetWindowUserPointer(window, pointer);
    }
    extern fn glfwSetWindowUserPointer(window: *Window, pointer: ?*anyopaque) void;

    pub fn shouldClose(window: *Window) bool {
        return if (glfwWindowShouldClose(window) == 0) false else true;
    }
    extern fn glfwWindowShouldClose(window: *Window) i32;

    pub fn setShouldClose(window: *Window, should_close: bool) void {
        return glfwSetWindowShouldClose(window, if (should_close) 1 else 0);
    }
    extern fn glfwSetWindowShouldClose(window: *Window, should_close: i32) void;

    /// `pub fn destroy(window: *Window) void`
    pub const destroy = glfwDestroyWindow;
    extern fn glfwDestroyWindow(window: *Window) void;

    /// `pub fn setSizeLimits(window: *Window, min_w: i32, min_h: i32, max_w: i32, max_h: i32) void`
    pub const setSizeLimits = glfwSetWindowSizeLimits;
    extern fn glfwSetWindowSizeLimits(window: *Window, min_w: i32, min_h: i32, max_w: i32, max_h: i32) void;

    pub fn getContentScale(window: *Window) [2]f32 {
        var xscale: f32 = 0.0;
        var yscale: f32 = 0.0;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        return .{ xscale, yscale };
    }
    extern fn glfwGetWindowContentScale(window: *Window, xscale: *f32, yscale: *f32) void;

    /// `pub getKey(window: *Window, key: Key) Action`
    pub const getKey = glfwGetKey;
    extern fn glfwGetKey(window: *Window, key: Key) Action;

    /// `pub fn getMouseButton(window: *Window, button: MouseButton) Action`
    pub const getMouseButton = glfwGetMouseButton;
    extern fn glfwGetMouseButton(window: *Window, button: MouseButton) Action;

    pub fn getCursorPos(window: *Window) [2]f64 {
        var xpos: f64 = 0.0;
        var ypos: f64 = 0.0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return .{ xpos, ypos };
    }
    extern fn glfwGetCursorPos(window: *Window, xpos: *f64, ypos: *f64) void;

    pub fn getFramebufferSize(window: *Window) [2]i32 {
        var width: i32 = 0.0;
        var height: i32 = 0.0;
        glfwGetFramebufferSize(window, &width, &height);
        return .{ width, height };
    }
    extern fn glfwGetFramebufferSize(window: *Window, width: *i32, height: *i32) void;

    pub fn getSize(window: *Window) [2]i32 {
        var width: i32 = 0.0;
        var height: i32 = 0.0;
        glfwGetWindowSize(window, &width, &height);
        return .{ width, height };
    }
    extern fn glfwGetWindowSize(window: *Window, width: *i32, height: *i32) void;

    /// `pub fn setSize(window: *Window, width: i32, height: i32) void`
    pub const setSize = glfwSetWindowSize;
    extern fn glfwSetWindowSize(window: *Window, width: i32, height: i32) void;

    pub fn getPos(window: *Window) [2]i32 {
        var xpos: i32 = 0.0;
        var ypos: i32 = 0.0;
        glfwGetWindowPos(window, &xpos, &ypos);
        return .{ xpos, ypos };
    }
    extern fn glfwGetWindowPos(window: *Window, xpos: *i32, ypos: *i32) void;

    /// `pub fn setPos(window: *Window, width: i32, height: i32) void`
    pub const setPos = glfwSetWindowPos;
    extern fn glfwSetWindowPos(window: *Window, xpos: i32, ypos: i32) void;

    pub inline fn setTitle(window: *Window, title: [:0]const u8) void {
        glfwSetWindowTitle(window, title);
    }
    extern fn glfwSetWindowTitle(window: *Window, title: [*:0]const u8) void;

    pub fn getClipboardString(window: *Window) ?[:0]const u8 {
        return std.mem.span(glfwGetClipboardString(window));
    }
    extern fn glfwGetClipboardString(window: *Window) ?[*:0]const u8;

    pub inline fn setClipboardString(window: *Window, string: [:0]const u8) void {
        return glfwSetClipboardString(window, string);
    }
    extern fn glfwSetClipboardString(
        window: *Window,
        string: [*:0]const u8,
    ) void;

    /// `pub fn setFramebufferSizeCallback(window: *Window, callback: ?FramebufferSizeFn) ?FramebufferSizeFn`
    pub const setFramebufferSizeCallback = glfwSetFramebufferSizeCallback;
    extern fn glfwSetFramebufferSizeCallback(window: *Window, callback: ?FramebufferSizeFn) ?FramebufferSizeFn;
    pub const FramebufferSizeFn = *const fn (
        window: *Window,
        width: i32,
        height: i32,
    ) callconv(.C) void;

    /// `pub fn setSizeCallback(window: *Window, callback: ?WindowSizeFn) ?WindowSizeFn`
    pub const setSizeCallback = glfwSetWindowSizeCallback;
    extern fn glfwSetWindowSizeCallback(window: *Window, callback: ?WindowSizeFn) ?WindowSizeFn;
    pub const WindowSizeFn = *const fn (
        window: *Window,
        width: i32,
        height: i32,
    ) callconv(.C) void;

    /// `pub fn setPosCallback(window: *Window, callback: ?WindowPosFn) ?WindowPosFn`
    pub const setPosCallback = glfwSetWindowPosCallback;
    extern fn glfwSetWindowPosCallback(window: *Window, callback: ?WindowPosFn) ?WindowPosFn;
    pub const WindowPosFn = *const fn (
        window: *Window,
        xpos: i32,
        ypos: i32,
    ) callconv(.C) void;

    /// `pub const setFocusCallback(window: *Window, callback: ?WindowFocusFn) ?WindowFocusFn`
    pub const setFocusCallback = glfwSetWindowFocusCallback;
    extern fn glfwSetWindowFocusCallback(window: *Window, callback: ?WindowFocusFn) ?WindowFocusFn;
    pub const WindowFocusFn = *const fn (
        window: *Window,
        focused: i32,
    ) callconv(.C) void;

    /// `pub const setContentScaleCallback(window: *Window, callback: ?WindowContentScaleFn) ?WindowContentScaleFn`
    pub const setContentScaleCallback = glfwSetWindowContentScaleCallback;
    extern fn glfwSetWindowContentScaleCallback(window: *Window, callback: ?WindowContentScaleFn) ?WindowContentScaleFn;
    pub const WindowContentScaleFn = *const fn (
        window: *Window,
        xscale: f32,
        yscale: f32,
    ) callconv(.C) void;

    /// `pub fn setKeyCallback(window: *Window, callback: ?KeyFn) ?KeyFn`
    pub const setKeyCallback = glfwSetKeyCallback;
    extern fn glfwSetKeyCallback(window: *Window, callback: ?KeyFn) ?KeyFn;
    pub const KeyFn = *const fn (
        window: *Window,
        key: Key,
        scancode: i32,
        action: Action,
        mods: Mods,
    ) callconv(.C) void;

    /// `pub fn setCharCallback(window: *Window, callback: ?CharFn) ?CharFn`
    pub const setCharCallback = glfwSetCharCallback;
    extern fn glfwSetCharCallback(window: *Window, callback: ?CharFn) ?CharFn;
    pub const CharFn = *const fn (
        window: *Window,
        codepoint: u32,
    ) callconv(.C) void;

    /// `pub fn setDropCallback(window: *Window, callback: ?DropFn) ?DropFn`
    pub const setDropCallback = glfwSetDropCallback;
    extern fn glfwSetDropCallback(window: *Window, callback: ?DropFn) ?DropFn;
    pub const DropFn = *const fn (
        window: *Window,
        path_count: i32,
        paths: [*][*:0]const u8,
    ) callconv(.C) void;

    /// `pub fn setMouseButtonCallback(window: *Window, callback: ?MouseButtonFn) ?MouseButtonFn`
    pub const setMouseButtonCallback = glfwSetMouseButtonCallback;
    extern fn glfwSetMouseButtonCallback(window: *Window, callback: ?MouseButtonFn) ?MouseButtonFn;
    pub const MouseButtonFn = *const fn (
        window: *Window,
        button: MouseButton,
        action: Action,
        mods: Mods,
    ) callconv(.C) void;

    /// `pub fn setCursorPosCallback(window: *Window, callback: ?CursorPosFn) ?CursorPosFn`
    pub const setCursorPosCallback = glfwSetCursorPosCallback;
    extern fn glfwSetCursorPosCallback(window: *Window, callback: ?CursorPosFn) ?CursorPosFn;
    pub const CursorPosFn = *const fn (
        window: *Window,
        xpos: f64,
        ypos: f64,
    ) callconv(.C) void;

    /// `pub fn setScrollCallback(window: *Window, callback: ?ScrollFn) ?ScrollFn`
    pub const setScrollCallback = glfwSetScrollCallback;
    extern fn glfwSetScrollCallback(window: *Window, callback: ?ScrollFn) ?ScrollFn;
    pub const ScrollFn = *const fn (
        window: *Window,
        xoffset: f64,
        yoffset: f64,
    ) callconv(.C) void;

    /// `pub fn setCursorEnterCallback(window: *Window, callback: ?CursorEnterFn) ?CursorEnterFn`
    pub const setCursorEnterCallback = glfwSetCursorEnterCallback;
    extern fn glfwSetCursorEnterCallback(window: *Window, callback: ?CursorEnterFn) ?CursorEnterFn;
    pub const CursorEnterFn = *const fn (
        window: *Window,
        entered: i32,
    ) callconv(.C) void;

    /// `pub fn setCursor(window: *Window, cursor: ?*Cursor) void`
    pub const setCursor = glfwSetCursor;
    extern fn glfwSetCursor(window: *Window, cursor: ?*Cursor) void;

    pub fn setInputMode(window: *Window, mode: InputMode, value: anytype) void {
        const T = @TypeOf(value);
        const i32_value = switch (@typeInfo(T)) {
            .Enum, .EnumLiteral => @intFromEnum(@as(Cursor.Mode, value)),
            .Bool => @intFromBool(value),
            else => unreachable,
        };
        glfwSetInputMode(window, mode, i32_value);
    }
    extern fn glfwSetInputMode(window: *Window, mode: InputMode, value: i32) void;

    pub fn focus(window: *Window) void {
        glfwFocusWindow(window);
    }
    extern fn glfwFocusWindow(window: *Window) void;

    pub const swapBuffers = glfwSwapBuffers;
    extern fn glfwSwapBuffers(window: *Window) void;

    pub fn setMonitor(window: *Window, monitor: ?*Monitor, xpos: i32, ypos: i32, width: i32, height: i32, refreshRate: i32) void {
        glfwSetWindowMonitor(window, monitor, xpos, ypos, width, height, refreshRate);
    }
    extern fn glfwSetWindowMonitor(window: *Window, monitor: ?*Monitor, xpos: i32, ypos: i32, width: i32, height: i32, refreshRate: i32) void;

    pub fn create(
        width: i32,
        height: i32,
        title: [:0]const u8,
        monitor: ?*Monitor,
    ) Error!*Window {
        if (glfwCreateWindow(width, height, title, monitor, null)) |window| return window;
        try maybeError();
        unreachable;
    }
    extern fn glfwCreateWindow(
        width: i32,
        height: i32,
        title: [*:0]const u8,
        monitor: ?*Monitor,
        share: ?*Window,
    ) ?*Window;

    pub fn show(window: *Window) void {
        glfwShowWindow(window);
    }
    extern fn glfwShowWindow(window: *Window) void;
};

pub const WindowHint = enum(i32) {
    focused = 0x00020001,
    iconified = 0x00020002,
    resizable = 0x00020003,
    visible = 0x00020004,
    decorated = 0x00020005,
    auto_iconify = 0x00020006,
    floating = 0x00020007,
    maximized = 0x00020008,
    center_cursor = 0x00020009,
    transparent_framebuffer = 0x0002000A,
    hovered = 0x0002000B,
    focus_on_show = 0x0002000C,
    mouse_passthrough = 0x0002000D,
    position_x = 0x0002000E,
    position_y = 0x0002000F,
    red_bits = 0x00021001,
    green_bits = 0x00021002,
    blue_bits = 0x00021003,
    alpha_bits = 0x00021004,
    depth_bits = 0x00021005,
    stencil_bits = 0x00021006,
    // ACCUM_*_BITS/AUX_BUFFERS are deprecated
    stereo = 0x0002100C,
    samples = 0x0002100D,
    srgb_capable = 0x0002100E,
    refresh_rate = 0x0002100F,
    doublebuffer = 0x00021010,
    client_api = 0x00022001,
    context_version_major = 0x00022002,
    context_version_minor = 0x00022003,
    context_revision = 0x00022004,
    context_robustness = 0x00022005,
    opengl_forward_compat = 0x00022006,
    opengl_debug_context = 0x00022007,
    opengl_profile = 0x00022008,
    context_release_behaviour = 0x00022009,
    context_no_error = 0x0002200A,
    context_creation_api = 0x0002200B,
    scale_to_monitor = 0x0002200C,
    scale_framebuffer = 0x0002200D,
    cocoa_retina_framebuffer = 0x00023001,
    cocoa_frame_name = 0x00023002,
    cocoa_graphics_switching = 0x00023003,
    x11_class_name = 0x00024001,
    x11_instance_name = 0x00024002,
    win32_keyboard_menu = 0x00025001,
    win32_showdefault = 0x00025002,
    wayland_app_id = 0x00026001,

    fn ValueType(comptime window_hint: WindowHint) type {
        return switch (window_hint) {
            .focused,
            .iconified,
            .resizable,
            .visible,
            .decorated,
            .auto_iconify,
            .floating,
            .maximized,
            .center_cursor,
            .transparent_framebuffer,
            .hovered,
            .focus_on_show,
            .mouse_passthrough,
            => bool,
            .position_x, .position_y => i32,
            .red_bits, .green_bits, .blue_bits, .alpha_bits, .depth_bits, .stencil_bits => i32,
            .stereo => bool,
            .samples => i32,
            .srgb_capable => bool,
            .refresh_rate => i32,
            .doublebuffer => bool,
            .client_api => ClientApi,
            .context_version_major, .context_version_minor, .context_revision => i32,
            .context_robustness => ContextRobustness,
            .opengl_forward_compat, .opengl_debug_context => bool,
            .opengl_profile => OpenGLProfile,
            .context_release_behaviour => ReleaseBehaviour,
            .context_no_error => bool,
            .context_creation_api => ContextCreationApi,
            .scale_to_monitor, .scale_framebuffer, .cocoa_retina_framebuffer => bool,
            .cocoa_frame_name => [:0]const u8,
            .cocoa_graphics_switching => bool,
            .x11_class_name, .x11_instance_name => [:0]const u8,
            .win32_keyboard_menu, .win32_showdefault => bool,
            .wayland_app_id => [:0]const u8,
        };
    }

    /// DEPRECATED: Does not allow setting string type hints.
    /// Use `windowHint`, `windowHintString` or `windowHintTyped` instead.
    pub const set = glfwWindowHint;
};

pub fn windowHintTyped(
    comptime window_hint: WindowHint,
    value: WindowHint.ValueType(window_hint),
) void {
    const ValueType = WindowHint.ValueType(window_hint);
    switch (ValueType) {
        else => windowHint(window_hint, switch (@typeInfo(ValueType)) {
            .Int => @intCast(value),
            .Enum => @intFromEnum(value),
            .Bool => @intFromBool(value),
            else => unreachable,
        }),
        [:0]const u8 => windowHintString(window_hint, value),
    }
}

pub const windowHint = glfwWindowHint;
extern fn glfwWindowHint(WindowHint, value: i32) void;

pub fn windowHintString(window_hint: WindowHint, string: [:0]const u8) void {
    glfwWindowHintString(window_hint, string);
}
extern fn glfwWindowHintString(WindowHint, string: [*:0]const u8) void;

pub const ClientApi = enum(i32) {
    no_api = 0,
    opengl_api = 0x00030001,
    opengl_es_api = 0x00030002,
};

pub const OpenGLProfile = enum(i32) {
    opengl_any_profile = 0,
    opengl_core_profile = 0x00032001,
    opengl_compat_profile = 0x00032002,
};

pub const ContextRobustness = enum(i32) {
    no_robustness = 0,
    no_reset_notification = 0x00031001,
    lose_context_on_reset = 0x00031002,
};

pub const ReleaseBehaviour = enum(i32) {
    any = 0,
    flush = 0x00035001,
    none = 0x00035002,
};

pub const ContextCreationApi = enum(i32) {
    native = 0x00036001,
    egl = 0x00036002,
    osmesa = 0x00036003,
};

//--------------------------------------------------------------------------------------------------
//
// Native
//
//--------------------------------------------------------------------------------------------------
pub const getWin32Adapter = if (builtin.target.os.tag == .windows) glfwGetWin32Adapter else _getWin32Adapter;
extern fn glfwGetWin32Adapter(*Monitor) ?[*:0]const u8;
fn _getWin32Adapter(_: *Monitor) ?[*:0]const u8 {
    return null;
}

pub const getWin32Window = if (builtin.target.os.tag == .windows) glfwGetWin32Window else _getWin32Window;
extern fn glfwGetWin32Window(*Window) ?std.os.windows.HWND;
fn _getWin32Window(_: *Window) ?std.os.windows.HWND {
    return null;
}

pub const getX11Adapter = if (_isLinuxDesktopLike() and options.enable_x11) glfwGetX11Adapter else _getX11Adapter;
extern fn glfwGetX11Adapter(*Monitor) u32;
fn _getX11Adapter(_: *Monitor) u32 {
    return 0;
}

pub const getX11Display = if (_isLinuxDesktopLike() and options.enable_x11) glfwGetX11Display else _getX11Display;
extern fn glfwGetX11Display() ?*anyopaque;
fn _getX11Display() ?*anyopaque {
    return null;
}

pub const getX11Window = if (_isLinuxDesktopLike() and options.enable_x11) glfwGetX11Window else _getX11Window;
extern fn glfwGetX11Window(window: *Window) u32;
fn _getX11Window(_: *Window) u32 {
    return 0;
}

pub const getWaylandDisplay = if (_isLinuxDesktopLike() and options.enable_wayland) glfwGetWaylandDisplay else _getWaylandDisplay;
extern fn glfwGetWaylandDisplay() ?*anyopaque;
fn _getWaylandDisplay() ?*anyopaque {
    return null;
}

pub const getWaylandWindow = if (_isLinuxDesktopLike() and options.enable_wayland) glfwGetWaylandWindow else _getWaylandWindow;
extern fn glfwGetWaylandWindow(window: *Window) ?*anyopaque;
fn _getWaylandWindow(_: *Window) ?*anyopaque {
    return null;
}

pub const getCocoaWindow = if (builtin.target.os.tag == .macos) glfwGetCocoaWindow else _getCocoaWindow;
extern fn glfwGetCocoaWindow(window: *Window) ?*anyopaque;
fn _getCocoaWindow(_: *Window) ?*anyopaque {
    return null;
}

fn _isLinuxDesktopLike() bool {
    return switch (builtin.target.os.tag) {
        .linux,
        .freebsd,
        .openbsd,
        .dragonfly,
        => true,
        else => false,
    };
}
