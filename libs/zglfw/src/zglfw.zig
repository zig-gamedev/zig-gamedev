const std = @import("std");

pub fn init() Error!void {
    if (glfwInit() == 0) {
        try maybeError();
    }
}
extern fn glfwInit() i32;

pub const terminate = glfwTerminate;
extern fn glfwTerminate() void;

pub const pollEvents = glfwPollEvents;
extern fn glfwPollEvents() void;
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
};

pub fn createWindow(
    width: i32,
    height: i32,
    title: [*:0]const u8,
    monitor: ?Monitor,
    share: ?Window,
) Error!Window {
    const window = glfwCreateWindow(width, height, title, monitor, share);
    if (window == null) {
        try maybeError();
    }
    return window.?;
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
// Test
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zglfw.basic" {
    try init();
    defer terminate();

    const primary_monitor = getPrimaryMonitor();
    if (primary_monitor) |pm| {
        const monitors = getMonitors().?;
        try expect(pm == monitors[0]);
        const pos = pm.getPos();
        _ = pos.x;
        _ = pos.y;
    }

    defaultWindowHints();
    windowHint(.cocoa_retina_framebuffer, 1);
    windowHint(.client_api, 0);
    const window = try createWindow(200, 200, "test", null, null);
    defer window.destroy();
    window.setSizeLimits(10, 10, 300, 300);
    const content_scale = window.getContentScale();
    _ = content_scale.x;
    _ = content_scale.y;
    pollEvents();
    try maybeError();
}
//--------------------------------------------------------------------------------------------------
