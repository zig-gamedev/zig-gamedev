const std = @import("std");

pub const Package = struct {
    zpool: *std.Build.Module,

    pub fn build(b: *std.Build, _: struct {}) Package {
        const zpool = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        });
        return .{ .zpool = zpool };
    }
};

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, optimize, target);

    const test_step = b.step("test", "Run zpool tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
