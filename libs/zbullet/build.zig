const std = @import("std");

pub const Package = struct {
    zbullet: *std.Build.Module,
    zbullet_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        _: struct {},
    ) Package {
        const zbullet = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zbullet.zig" },
        });

        const zbullet_c_cpp = b.addStaticLibrary(.{
            .name = "zbullet",
            .target = target,
            .optimize = optimize,
        });

        zbullet_c_cpp.addIncludePath(thisDir() ++ "/libs/cbullet");
        zbullet_c_cpp.addIncludePath(thisDir() ++ "/libs/bullet");
        zbullet_c_cpp.linkLibC();
        zbullet_c_cpp.linkLibCpp();

        // TODO: Use the old damping method for now otherwise there is a hang in powf().
        const flags = &.{
            "-DBT_USE_OLD_DAMPING_METHOD",
            "-DBT_THREADSAFE=1",
            "-std=c++11",
            "-fno-sanitize=undefined",
        };
        zbullet_c_cpp.addCSourceFile(thisDir() ++ "/libs/cbullet/cbullet.cpp", flags);
        zbullet_c_cpp.addCSourceFile(thisDir() ++ "/libs/bullet/btLinearMathAll.cpp", flags);
        zbullet_c_cpp.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletCollisionAll.cpp", flags);
        zbullet_c_cpp.addCSourceFile(thisDir() ++ "/libs/bullet/btBulletDynamicsAll.cpp", flags);

        return .{
            .zbullet = zbullet,
            .zbullet_c_cpp = zbullet_c_cpp,
        };
    }

    pub fn link(zbullet_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(zbullet_pkg.zbullet_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zbullet.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zbullet_pkg = Package.build(b, target, optimize, .{});
    zbullet_pkg.link(tests);

    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
