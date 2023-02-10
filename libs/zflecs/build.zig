const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        no_pipeline_support: bool = false,
        no_http_support: bool = false,
    };

    options: Options,
    zflecs: *std.Build.Module,
    zflecs_options: *std.Build.Module,
    zflecs_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        args: struct {
            options: Options = .{},
        },
    ) Package {
        const step = b.addOptions();
        step.addOption(bool, "no_pipeline_support", args.options.no_pipeline_support);
        step.addOption(bool, "no_http_support", args.options.no_http_support);

        const zflecs_options = step.createModule();

        const zflecs = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zflecs.zig" },
            .dependencies = &.{
                .{ .name = "zflecs_options", .module = zflecs_options },
            },
        });

        const zflecs_c_cpp = b.addStaticLibrary(.{
            .name = "zflecs",
            .target = target,
            .optimize = optimize,
        });

        zflecs_c_cpp.linkLibC();

        const pipeline_def = if (args.options.no_pipeline_support) "-DFLECS_NO_PIPELINE" else "";
        const http_def = if (args.options.no_http_support) "-DFLECS_NO_HTTP" else "";

        zflecs_c_cpp.addIncludePath(thisDir() ++ "/libs/flecs");
        zflecs_c_cpp.addCSourceFile(thisDir() ++ "/libs/flecs/flecs.c", &.{
            "-std=c99",
            "-fno-sanitize=undefined",
            pipeline_def,
            http_def,
        });

        if (zflecs_c_cpp.target.isWindows()) {
            zflecs_c_cpp.linkSystemLibrary("ws2_32");
        }

        return .{
            .options = args.options,
            .zflecs = zflecs,
            .zflecs_options = zflecs_options,
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
