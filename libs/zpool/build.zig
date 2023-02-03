const std = @import("std");

const main = .{ .zig = thisDir() ++ "/src/main.zig" };

pub const pkg = std.Build.Pkg{
    .name = "zpool",
    .source = .{ .path = main.zig },
};

pub fn build(b: *std.Build) void {
    const build_mode = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zpool tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = main.zig },
        .target = target,
        .optimize = build_mode,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
