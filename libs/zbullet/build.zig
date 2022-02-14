const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const tests = b.addTest("src/zbullet.zig");
    const zmath = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = thisDir() ++ "/../zmath/zmath.zig" },
    };
    tests.addPackage(zmath);
    tests.setBuildMode(b.standardReleaseOptions());
    tests.setTarget(b.standardTargetOptions(.{}));
    link(b, tests);

    const test_step = b.step("test", "Run library tests");
    test_step.dependOn(&tests.step);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
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
