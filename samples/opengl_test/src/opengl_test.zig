const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const glfw = @import("glfw");

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: opengl test";
const window_width = 1920;
const window_height = 1080;

const gl = struct {
    const COLOR = 0x1800;

    var clearBufferfv: fn (u32, i32, *const [4]f32) callconv(.C) void = undefined;
};

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(window_width, window_height, window_name, null, null, .{});
    defer window.destroy();

    try glfw.makeContextCurrent(window);

    gl.clearBufferfv = @ptrCast(@TypeOf(gl.clearBufferfv), glfw.getProcAddress("glClearBufferfv").?);

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        gl.clearBufferfv(gl.COLOR, 0, &.{ 0.7, 0.0, 0.0, 1.0 });
        try window.swapBuffers();
    }
}
