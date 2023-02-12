const std = @import("std");

pub const Package = struct {
    zflecs: *std.Build.Module,
    zflecs_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        _: struct {},
    ) Package {
        const zflecs = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zflecs.zig" },
            .dependencies = &.{},
        });

        const zflecs_c_cpp = b.addStaticLibrary(.{
            .name = "zflecs",
            .target = target,
            .optimize = optimize,
        });
        zflecs_c_cpp.linkLibC();
        zflecs_c_cpp.addIncludePath(thisDir() ++ "/libs/flecs");
        zflecs_c_cpp.addCSourceFile(thisDir() ++ "/libs/flecs/flecs.c", &.{
            "-fno-sanitize=undefined",
            "-DFLECS_NO_CPP",
        });

        if (zflecs_c_cpp.target.isWindows()) {
            zflecs_c_cpp.linkSystemLibraryName("ws2_32");
        }

        return .{
            .zflecs = zflecs,
            .zflecs_c_cpp = zflecs_c_cpp,
        };
    }

    pub fn link(zflecs_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addIncludePath(thisDir() ++ "/libs/flecs");
        exe.linkLibrary(zflecs_pkg.zflecs_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zflecs.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
