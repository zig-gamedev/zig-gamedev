const std = @import("std");

pub fn build(b: *std.Build) void {
    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zopengl.zig"),
    });
}
