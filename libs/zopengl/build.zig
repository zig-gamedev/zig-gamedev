const std = @import("std");

pub const Package = struct {
    zopengl: *std.Build.Module,

    pub fn build(b: *std.Build, _: struct {}) Package {
        const zopengl = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
        });
        return .{
            .zopengl = zopengl,
        };
    }
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
