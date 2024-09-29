const std = @import("std");

const sdl2_demo = @import("sdl2_demo.zig");

pub fn main() !void {
    { // Change current working directory to where the executable is located.
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    try sdl2_demo.init();
    defer sdl2_demo.deinit();

    while (sdl2_demo.shouldQuit() == false) {
        try sdl2_demo.updateAndRender();
    }
}
