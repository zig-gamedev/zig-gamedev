const std = @import("std");

pub const Monitor = *opaque {
    pub const getPos = glfwGetMonitorPos;
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

pub const Window = *opaque {
    pub fn shouldClose(window: Window) bool {
        return if (glfwWindowShouldClose(window) == 0) false else true;
    }
    extern fn glfwWindowShouldClose(window: Window) i32;
};

pub const Error = error{
    PlatformError,
};

pub fn init() Error!void {
    if (glfwInit() == 0) return Error.PlatformError;
}
extern fn glfwInit() i32;

pub const terminate = glfwTerminate;
extern fn glfwTerminate() void;

const expect = std.testing.expect;

test "zglfw.basic" {
    try init();
    defer terminate();

    const primary_monitor = getPrimaryMonitor();
    if (primary_monitor) |pm| {
        const monitors = getMonitors().?;
        try expect(pm == monitors[0]);
    }
}
