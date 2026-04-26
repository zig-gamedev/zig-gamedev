const std = @import("std");

const sdl2_demo = @import("sdl2_demo.zig");

pub fn main(init: std.process.Init) !void {
    const io = init.io;
    const allocator = init.gpa;
    {
        const path = try std.process.executablePathAlloc(io, allocator);
        defer allocator.free(path);
        try std.process.setCurrentPath(io, path);
    }

    try sdl2_demo.init();
    defer sdl2_demo.deinit();

    while (sdl2_demo.shouldQuit() == false) {
        try sdl2_demo.updateAndRender();
    }
}
