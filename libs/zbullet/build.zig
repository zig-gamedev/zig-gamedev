const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zbullet",
    .source = .{ .path = thisDir() ++ "/src/zbullet.zig" },
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
    const tests = b.addTest(thisDir() ++ "/src/zbullet.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zbullet", thisDir() ++ "/src/zbullet.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.addIncludeDir(thisDir() ++ "/libs/cbullet");
    lib.addIncludeDir(thisDir() ++ "/libs/bullet");
    lib.linkSystemLibraryName("c");
    lib.linkSystemLibraryName("c++");

    // TODO: Use the old damping method for now otherwise there is a hang in powf().
    const flags = &.{
        "-DBT_USE_OLD_DAMPING_METHOD",
        "-DBT_THREADSAFE=1",
        "-std=c++11",
        "-fno-sanitize=undefined",
    };
    lib.addCSourceFile(thisDir() ++ "/libs/cbullet/cbullet.cpp", flags);
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btLinearMathAll.cpp", flags);
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp", flags);
    lib.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp", flags);

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
