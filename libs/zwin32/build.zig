const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zwin32",
    .source = .{ .path = thisDir() ++ "/src/zwin32.zig" },
};

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
