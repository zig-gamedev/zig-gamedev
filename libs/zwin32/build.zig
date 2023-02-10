const std = @import("std");

pub const Package = struct {
    zwin32: *std.Build.Module,

    pub fn build(b: *std.Build, _: struct {}) Package {
        return .{
            .zwin32 = b.createModule(.{
                .source_file = .{ .path = thisDir() ++ "/src/zwin32.zig" },
            }),
        };
    }
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
