const std = @import("std");

const triangle_wgpu = @import("triangle_wgpu.zig");

pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try triangle_wgpu.init(allocator);
    defer demo.deinit(allocator);

    while (demo.shouldQuit() == false) {
        demo.update();
        demo.draw();
    }
}
