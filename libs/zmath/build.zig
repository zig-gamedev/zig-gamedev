const std = @import("std");

pub const pkg = std.Build.Pkg{
    .name = "zmath",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

pub fn build(b: *std.Build) void {
    const build_mode = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zmath tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = build_mode,
    });
    return tests;
}

pub fn buildBenchmarks(
    b: *std.Build,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "benchmark",
        .root_source_file = .{ .path = thisDir() ++ "/src/benchmark.zig" },
        .target = target,
        .optimize = .ReleaseFast,
    });
    exe.addPackage(pkg);
    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
