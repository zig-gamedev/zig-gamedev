const std = @import("std");

const zjobs = .{ .zig = thisDir() ++ "/src/zjobs.zig" };

pub const pkg = std.build.Pkg{
    .name = "zjobs",
    .source = .{ .path = zjobs.zig },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zjobs tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(zjobs.zig);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
