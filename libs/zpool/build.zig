const std = @import("std");

const main = .{ .zig = thisDir() ++ "/src/main.zig" };

pub const pkg = std.build.Pkg{
    .name = "zpool",
    .source = .{ .path = main.zig },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zpool tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(main.zig);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
