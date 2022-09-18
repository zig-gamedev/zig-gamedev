const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zbullet",
    .source = .{ .path = thisDir() ++ "/src/zbullet.zig" },
};

pub fn build(_: *std.build.Builder) void {}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(pkg.source.path);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs/cbullet");
    exe.addIncludePath(thisDir() ++ "/libs/bullet");
    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    // TODO: Use the old damping method for now otherwise there is a hang in powf().
    const flags = &.{
        "-DBT_USE_OLD_DAMPING_METHOD",
        "-DBT_THREADSAFE=1",
        "-std=c++11",
        "-fno-sanitize=undefined",
    };
    exe.addCSourceFile(thisDir() ++ "/libs/cbullet/cbullet.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/bullet/btLinearMathAll.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp", flags);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
