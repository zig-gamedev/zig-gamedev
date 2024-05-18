const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zbullet.zig"),
    });

    const cbullet = b.addStaticLibrary(.{
        .name = "cbullet",
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(cbullet);

    cbullet.addIncludePath(b.path("libs/cbullet"));
    cbullet.addIncludePath(b.path("libs/bullet"));
    cbullet.linkLibC();
    cbullet.linkLibCpp();

    // TODO: Use the old damping method for now otherwise there is a hang in powf().
    const flags = &.{
        "-DBT_USE_OLD_DAMPING_METHOD",
        "-DBT_THREADSAFE=1",
        "-std=c++11",
        "-fno-sanitize=undefined",
    };
    cbullet.addCSourceFiles(.{
        .files = &.{
            "libs/cbullet/cbullet.cpp",
            "libs/bullet/btLinearMathAll.cpp",
            "libs/bullet/btBulletCollisionAll.cpp",
            "libs/bullet/btBulletDynamicsAll.cpp",
        },
        .flags = flags,
    });

    const test_step = b.step("test", "Run zbullet tests");

    const zmath = b.dependency("zmath", .{});

    var tests = b.addTest(.{
        .name = "zbullet-tests",
        .root_source_file = b.path("src/zbullet.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    tests.root_module.addImport("zmath", zmath.module("root"));

    tests.linkLibrary(cbullet);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
