const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zwin32",
    .path = .{ .path = thisDir() ++ "/src/zwin32.zig" },
};

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
