const std = @import("std");

pub const Package = struct {
    module: *std.Build.Module,
};

pub fn package(b: *std.Build, _: struct {}) Package {
    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zjobs.zig" },
    });
    return .{ .module = module };
}

pub fn build(b: *std.Build) void {
    const build_mode = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zjobs tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zjobs.zig" },
        .target = target,
        .optimize = build_mode,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
