const std = @import("std");
const builtin = @import("builtin");

pub fn init() Error!void {
    if (glfwInit() != 0) return;
    try maybeError();
    unreachable;
}
extern fn glfwInit() i32;

pub const terminate = glfwTerminate;
extern fn glfwTerminate() void;

pub const pollEvents = glfwPollEvents;
extern fn glfwPollEvents() void;

pub fn vulkanSupported() bool {
    return if (glfwVulkanSupported() == 0) false else true;
}
extern fn glfwVulkanSupported() i32;

pub fn getRequiredInstanceExtensions() Error![][*:0]const u8 {
    var count: u32 = 0;
    if (glfwGetRequiredInstanceExtensions(&count)) |extensions| {
        return @ptrCast([*][*:0]const u8, extensions)[0..count];
    }
    try maybeError();
    unreachable;
}
extern fn glfwGetRequiredInstanceExtensions(count: *u32) ?*?[*:0]const u8;

pub const getTime = glfwGetTime;
extern fn glfwGetTime() f64;
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
// Error
//
//--------------------------------------------------------------------------------------------------
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
    Unknown,
};

pub fn maybeError() Error!void {
    return switch (glfwGetError(null)) {
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
        else => Error.Unknown,
    };
}
extern fn glfwGetError(description: ?*?[*:0]const u8) i32;
//--------------------------------------------------------------------------------------------------
//
// Monitor
//
//--------------------------------------------------------------------------------------------------
pub const Monitor = *opaque {
    pub fn getPos(monitor: Monitor) struct { x: i32, y: i32 } {
        var xpos: i32 = 0;
        var ypos: i32 = 0;
        glfwGetMonitorPos(monitor, &xpos, &ypos);
        return .{ .x = xpos, .y = ypos };
    }
    extern fn glfwGetMonitorPos(monitor: Monitor, xpos: *i32, ypos: *i32) void;
};

pub const getPrimaryMonitor = glfwGetPrimaryMonitor;
extern fn glfwGetPrimaryMonitor() ?Monitor;

pub fn getMonitors() ?[]Monitor {
    var count: i32 = 0;
    if (glfwGetMonitors(&count)) |monitors| {
        return monitors[0..@intCast(usize, count)];
    }
    return null;
}
extern fn glfwGetMonitors(count: *i32) ?[*]Monitor;
//--------------------------------------------------------------------------------------------------
//
// Window
//
//--------------------------------------------------------------------------------------------------
pub const WindowHint = enum(i32) {
    client_api = 0x00022001,
    cocoa_retina_framebuffer = 0x00023001,
};

pub const Window = *opaque {
    pub fn shouldClose(window: Window) bool {
        return if (glfwWindowShouldClose(window) == 0) false else true;
    }
    extern fn glfwWindowShouldClose(window: Window) i32;

    pub const destroy = glfwDestroyWindow;
    extern fn glfwDestroyWindow(window: Window) void;

    pub const setSizeLimits = glfwSetWindowSizeLimits;
    extern fn glfwSetWindowSizeLimits(
        window: Window,
        minwidth: i32,
        minheight: i32,
        maxwidth: i32,
        maxheight: i32,
    ) void;

    pub fn getContentScale(window: Window) struct { x: f32, y: f32 } {
        var xscale: f32 = 0.0;
        var yscale: f32 = 0.0;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        return .{ .x = xscale, .y = yscale };
    }
    extern fn glfwGetWindowContentScale(window: Window, xscale: *f32, yscale: *f32) void;

    pub const getKey = glfwGetKey;
    extern fn glfwGetKey(window: Window, key: Key) Action;

    pub const getMouseButton = glfwGetMouseButton;
    extern fn glfwGetMouseButton(window: Window, button: MouseButton) Action;

    pub fn getCursorPos(window: Window) struct { x: f64, y: f64 } {
        var xpos: f64 = 0.0;
        var ypos: f64 = 0.0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return .{ .x = xpos, .y = ypos };
    }
    extern fn glfwGetCursorPos(window: Window, xpos: *f64, ypos: *f64) void;

    pub fn getFramebufferSize(window: Window) struct { w: i32, h: i32 } {
        var width: i32 = 0.0;
        var height: i32 = 0.0;
        glfwGetFramebufferSize(window, &width, &height);
        return .{ .w = width, .h = height };
    }
    extern fn glfwGetFramebufferSize(window: Window, width: *i32, height: *i32) void;

    pub fn getSize(window: Window) struct { w: i32, h: i32 } {
        var width: i32 = 0.0;
        var height: i32 = 0.0;
        glfwGetWindowSize(window, &width, &height);
        return .{ .w = width, .h = height };
    }
    extern fn glfwGetWindowSize(window: Window, width: *i32, height: *i32) void;

    pub const setKeyCallback = glfwSetKeyCallback;
    extern fn glfwSetKeyCallback(
        window: Window,
        callback: ?*const fn (
            window: Window,
            key: Key,
            scancode: i32,
            action: Action,
            mods: Mods,
        ) callconv(.C) void,
    ) void;

    pub const setMouseButtonCallback = glfwSetMouseButtonCallback;
    extern fn glfwSetMouseButtonCallback(
        window: Window,
        callback: ?*const fn (window: Window, button: MouseButton, action: Action, mods: Mods) callconv(.C) void,
    ) void;

    pub const setCursorPosCallback = glfwSetCursorPosCallback;
    extern fn glfwSetCursorPosCallback(
        window: Window,
        callback: ?*const fn (Window, xpos: f64, ypos: f64) callconv(.C) void,
    ) void;

    pub const setScrollCallback = glfwSetScrollCallback;
    extern fn glfwSetScrollCallback(
        window: Window,
        callback: ?*const fn (
            window: Window,
            xoffset: f64,
            yoffset: f64,
        ) callconv(.C) void,
    ) void;
};

pub fn createWindow(
    width: i32,
    height: i32,
    title: [*:0]const u8,
    monitor: ?Monitor,
    share: ?Window,
) Error!Window {
    if (glfwCreateWindow(width, height, title, monitor, share)) |window| return window;
    try maybeError();
    unreachable;
}
extern fn glfwCreateWindow(
    width: i32,
    height: i32,
    title: [*:0]const u8,
    monitor: ?Monitor,
    share: ?Window,
) ?Window;

pub const windowHint = glfwWindowHint;
extern fn glfwWindowHint(hint: WindowHint, value: i32) void;

pub const defaultWindowHints = glfwDefaultWindowHints;
extern fn glfwDefaultWindowHints() void;
//--------------------------------------------------------------------------------------------------
//
// Native
//
//--------------------------------------------------------------------------------------------------
pub fn getWin32Adapter(monitor: Monitor) Error![*:0]const u8 {
    if (glfwGetWin32Adapter(monitor)) |adapter| return adapter;
    try maybeError();
    unreachable;
}
extern fn glfwGetWin32Adapter(monitor: Monitor) ?[*:0]const u8;

pub fn getWin32Window(window: Window) Error!std.os.windows.HWND {
    if (glfwGetWin32Window(window)) |hwnd| return hwnd;
    try maybeError();
    unreachable;
}
extern fn glfwGetWin32Window(window: Window) ?std.os.windows.HWND;

pub fn getX11Adapter(monitor: Monitor) Error!u32 {
    const adapter = glfwGetX11Adapter(monitor);
    if (adapter != 0) return adapter;
    try maybeError();
    unreachable;
}
extern fn glfwGetX11Adapter(monitor: Monitor) u32;

pub fn getX11Display() Error!*anyopaque {
    if (glfwGetX11Display()) |display| return display;
    try maybeError();
    unreachable;
}
extern fn glfwGetX11Display() ?*anyopaque;

pub fn getX11Window(window: Window) Error!u32 {
    const window_native = glfwGetX11Window(window);
    if (window_native != 0) return window_native;
    try maybeError();
    unreachable;
}
extern fn glfwGetX11Window(window: Window) u32;

pub fn getCocoaWindow(window: Window) Error!*anyopaque {
    if (glfwGetCocoaWindow(window)) |window_native| return window_native;
    try maybeError();
    unreachable;
}
extern fn glfwGetCocoaWindow(window: Window) ?*anyopaque;
//--------------------------------------------------------------------------------------------------
//
// Test
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

fn cursorPosCallback(window: Window, xpos: f64, ypos: f64) callconv(.C) void {
    _ = window;
    _ = xpos;
    _ = ypos;
}

fn mouseButtonCallback(window: Window, button: MouseButton, action: Action, mods: Mods) callconv(.C) void {
    _ = window;
    _ = button;
    _ = action;
    _ = mods;
}

fn scrollCallback(window: Window, xoffset: f64, yoffset: f64) callconv(.C) void {
    _ = window;
    _ = xoffset;
    _ = yoffset;
}

fn keyCallback(window: Window, key: Key, scancode: i32, action: Action, mods: Mods) callconv(.C) void {
    _ = window;
    _ = key;
    _ = scancode;
    _ = action;
    _ = mods;
}

test "zglfw.basic" {
    try init();
    defer terminate();

    if (vulkanSupported()) {
        _ = try getRequiredInstanceExtensions();
    }

    _ = getTime();

    const primary_monitor = getPrimaryMonitor();
    if (primary_monitor) |monitor| {
        const monitors = getMonitors().?;
        try expect(monitor == monitors[0]);
        const pos = monitor.getPos();
        _ = pos.x;
        _ = pos.y;

        const adapter = switch (@import("builtin").target.os.tag) {
            .windows => try getWin32Adapter(monitor),
            .linux => try getX11Adapter(monitor),
            else => {},
        };
        _ = adapter;
    }

    defaultWindowHints();
    windowHint(.cocoa_retina_framebuffer, 1);
    windowHint(.client_api, 0);
    const window = try createWindow(200, 200, "test", null, null);
    defer window.destroy();

    window.setCursorPosCallback(cursorPosCallback);
    window.setMouseButtonCallback(mouseButtonCallback);
    window.setKeyCallback(keyCallback);
    window.setScrollCallback(scrollCallback);
    window.setKeyCallback(null);

    if (window.getKey(.a) == .press) {}
    if (window.getMouseButton(.right) == .press) {}
    const cursor_pos = window.getCursorPos();
    _ = cursor_pos.x;
    _ = cursor_pos.y;

    const window_native = try switch (@import("builtin").target.os.tag) {
        .windows => getWin32Window(window),
        .linux => getX11Window(window),
        .macos => getCocoaWindow(window),
        else => unreachable,
    };
    _ = window_native;

    window.setSizeLimits(10, 10, 300, 300);
    const content_scale = window.getContentScale();
    _ = content_scale.x;
    _ = content_scale.y;
    pollEvents();
    try maybeError();
}
//--------------------------------------------------------------------------------------------------
