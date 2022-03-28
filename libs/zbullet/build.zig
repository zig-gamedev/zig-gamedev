const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zbullet tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zbullet.zig");
    const zmath = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = thisDir() ++ "/../zmath/src/zmath.zig" },
    };
    tests.addPackage(zmath);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(b, tests);
    return tests;
}

fn buildLibrary(b: *std.build.Builder, step: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = b.addStaticLibrary("zbullet", thisDir() ++ "/src/zbullet.zig");

    lib.setBuildMode(step.build_mode);
    lib.setTarget(step.target);
    lib.want_lto = false;
    lib.addIncludeDir(thisDir() ++ "/libs/cbullet");
    lib.addIncludeDir(thisDir() ++ "/libs/bullet");
    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");

    lib.addCSourceFile(thisDir() ++ "/libs/cbullet/cbullet.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btLinearMathAll.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp", &.{""});

    lib.install();
    return lib;
}

pub fn link(b: *std.build.Builder, step: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(b, step);
    step.linkLibrary(lib);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
