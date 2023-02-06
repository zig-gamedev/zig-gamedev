const std = @import("std");

pub const Options = struct {};

pub const Package = struct {
    module: *std.Build.Module,
};

pub fn package(b: *std.Build, _: Options, _: struct {}) Package {
    return .{
        .module = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zwin32.zig" },
        }),
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
