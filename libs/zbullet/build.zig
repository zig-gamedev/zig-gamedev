const std = @import("std");

pub const Package = struct {
    zbullet: *std.Build.Module,
    zbullet_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.linkLibrary(pkg.zbullet_c_cpp);
        exe.root_module.addImport("zbullet", pkg.zbullet);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    _: struct {},
) Package {
    const zbullet = b.addModule("zbullet", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zbullet.zig" },
    });

    const zbullet_c_cpp = b.addStaticLibrary(.{
        .name = "zbullet",
        .target = target,
        .optimize = optimize,
    });

    zbullet_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/cbullet" });
    zbullet_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/bullet" });
    zbullet_c_cpp.linkLibC();
    zbullet_c_cpp.linkLibCpp();

    // TODO: Use the old damping method for now otherwise there is a hang in powf().
    const flags = &.{
        "-DBT_USE_OLD_DAMPING_METHOD",
        "-DBT_THREADSAFE=1",
        "-std=c++11",
        "-fno-sanitize=undefined",
    };
    zbullet_c_cpp.addCSourceFiles(.{
        .files = &.{
            thisDir() ++ "/libs/cbullet/cbullet.cpp",
            thisDir() ++ "/libs/bullet/btLinearMathAll.cpp",
            thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp",
            thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp",
        },
        .flags = flags,
    });

    return .{
        .zbullet = zbullet,
        .zbullet_c_cpp = zbullet_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zbullet tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{});
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const zmath = b.dependency("zmath", .{});

    var tests = b.addTest(.{
        .name = "zbullet-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zbullet.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.root_module.addImport("zmath", zmath.module("zmath"));

    const pkg = package(b, target, optimize, .{});
    pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
