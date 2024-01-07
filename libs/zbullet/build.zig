const std = @import("std");

pub const Package = struct {
    zbullet: *std.Build.Module,
    zbullet_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.linkLibrary(pkg.zbullet_c_cpp);
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

    zbullet_c_cpp.root_module.addIncludePath(.{
        .path = thisDir() ++ "/libs/cbullet",
    });
    zbullet_c_cpp.root_module.addIncludePath(.{
        .path = thisDir() ++ "/libs/bullet",
    });
    zbullet_c_cpp.root_module.link_libc = true;
    zbullet_c_cpp.root_module.link_libcpp = true;

    // TODO: Use the old damping method for now otherwise there is a hang in powf().
    const flags = &.{
        "-DBT_USE_OLD_DAMPING_METHOD",
        "-DBT_THREADSAFE=1",
        "-std=c++11",
        "-fno-sanitize=undefined",
    };
    zbullet_c_cpp.root_module.addCSourceFiles(.{
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

    _ = package(b, target, optimize, .{});
}
inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
