const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zmath",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zmath tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/main.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    return tests;
}

pub fn buildBenchmarks(
    b: *std.build.Builder,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const exe = b.addExecutable("benchmark", thisDir() ++ "/src/benchmark.zig");
    exe.setBuildMode(std.builtin.Mode.ReleaseFast);
    exe.setTarget(target);
    exe.addPackage(pkg);
    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
