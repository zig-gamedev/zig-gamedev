const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zwin32",
    .source = .{ .path = thisDir() ++ "/src/zwin32.zig" },
};

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
