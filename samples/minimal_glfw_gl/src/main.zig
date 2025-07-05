const std = @import("std");

const minimal_glfw_gl = @import("minimal_glfw_gl.zig");

pub fn main() !void {
    { // Change current working directory to where the executable is located.
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    try minimal_glfw_gl.init(.{
        .api = .opengl_api,
        .version_major = 4,
        .version_minor = 0,
    });
    defer minimal_glfw_gl.deinit();

    while (minimal_glfw_gl.shouldQuit() == false) {
        minimal_glfw_gl.updateAndRender();
    }
}
