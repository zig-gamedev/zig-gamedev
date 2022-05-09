const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zbullet",
    .path = .{ .path = thisDir() ++ "/src/zbullet.zig" },
};

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
    const tests = b.addTest(comptime thisDir() ++ "/src/zbullet.zig");
    const zmath = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = comptime thisDir() ++ "/../zmath/src/zmath.zig" },
    };
    tests.addPackage(zmath);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zbullet", comptime thisDir() ++ "/src/zbullet.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.want_lto = false;
    lib.addIncludeDir(comptime thisDir() ++ "/libs/cbullet");
    lib.addIncludeDir(comptime thisDir() ++ "/libs/bullet");
    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");

    lib.addCSourceFile(comptime thisDir() ++ "/libs/cbullet/cbullet.cpp", &.{""});
    lib.addCSourceFile(comptime thisDir() ++ "/libs/bullet/btLinearMathAll.cpp", &.{""});
    lib.addCSourceFile(comptime thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp", &.{""});
    lib.addCSourceFile(comptime thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp", &.{""});

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
