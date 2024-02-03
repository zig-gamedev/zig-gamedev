const std = @import("std");

const default_options = struct {
    const enable_debug_layer = false;
    const enable_gbv = false;
    const enable_d2d = false;
    const upload_heap_capacity = 24 * 1024 * 1024;
};

pub const Options = struct {
    enable_debug_layer: bool = default_options.enable_debug_layer,
    enable_gbv: bool = default_options.enable_gbv,
    enable_d2d: bool = default_options.enable_d2d,
    upload_heap_capacity: u32 = default_options.upload_heap_capacity,
};

pub const Package = struct {
    options: Options,
    zd3d12: *std.Build.Module,
    zd3d12_options: *std.Build.Module,
    tests_step: *std.Build.Step,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zd3d12", pkg.zd3d12);
        exe.root_module.addImport("zd3d12_options", pkg.zd3d12_options);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options,
        deps: struct { zwin32: *std.Build.Module },
    },
) Package {
    const options = b.addOptions();
    options.addOption(bool, "enable_debug_layer", args.options.enable_debug_layer);
    options.addOption(bool, "enable_gbv", args.options.enable_gbv);
    options.addOption(bool, "enable_d2d", args.options.enable_d2d);
    options.addOption(u32, "upload_heap_capacity", args.options.upload_heap_capacity);

    const zd3d12_options = options.createModule();

    const zd3d12 = b.addModule("zd3d12", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zd3d12.zig" },
        .imports = &.{
            .{ .name = "zd3d12_options", .module = zd3d12_options },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    const tests = b.addTest(.{
        .name = "zd3d12-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zd3d12.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.root_module.addImport("zd3d12_options", zd3d12_options);
    tests.root_module.addImport("zwin32", args.deps.zwin32);

    return .{
        .options = args.options,
        .zd3d12 = zd3d12,
        .zd3d12_options = zd3d12_options,
        .tests_step = &b.addRunArtifact(tests).step,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32 = b.dependency("zwin32", .{});

    const pkg = package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = b.option(
                bool,
                "enable_debug_layer",
                "Enable debug layer",
            ) orelse default_options.enable_debug_layer,
            .enable_gbv = b.option(
                bool,
                "enable_gbv",
                "Enable GPU-based validation",
            ) orelse default_options.enable_gbv,
            .enable_d2d = b.option(
                bool,
                "enable_d2d",
                "Enable Direct2D",
            ) orelse default_options.enable_d2d,
            .upload_heap_capacity = b.option(
                u32,
                "upload_heap_capacity",
                "Set upload heap capacity",
            ) orelse default_options.upload_heap_capacity,
        },
        .deps = .{
            .zwin32 = zwin32.module("zwin32"),
        },
    });

    b.step("test", "Run zd3d12 tests").dependOn(pkg.tests_step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
