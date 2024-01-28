const std = @import("std");

pub const Options = struct {
    enable_debug_layer: bool = false,
};

pub const Package = struct {
    options: Options,
    zxaudio2: *std.Build.Module,
    zxaudio2_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zxaudio2", pkg.zxaudio2);
        exe.root_module.addImport("zxaudio2_options", pkg.zxaudio2_options);
    }
};

pub fn package(
    b: *std.Build,
    _: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
        deps: struct { zwin32: *std.Build.Module },
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable_debug_layer", args.options.enable_debug_layer);

    const zxaudio2_options = step.createModule();

    const zxaudio2 = b.addModule("zxaudio2", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .imports = &.{
            .{ .name = "zxaudio2_options", .module = zxaudio2_options },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    return .{
        .options = args.options,
        .zxaudio2 = zxaudio2,
        .zxaudio2_options = zxaudio2_options,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zxaudio2 tests");
    test_step.dependOn(runTests(b, optimize, target));

    const zwin32 = b.dependency("zwin32", .{});

    _ = package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = b.option(bool, "enable_debug_layer", "Enables debug layer") orelse false,
        },
        .deps = .{
            .zwin32 = zwin32.module("zwin32"),
        },
    });
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const zwin32 = b.dependency("zwin32", .{}).module("zwin32");

    const tests = b.addTest(.{
        .name = "zxaudio2-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .target = target,
        .optimize = optimize,
    });

    const pkg = package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32 },
    });
    pkg.link(tests);

    tests.root_module.addImport("zwin32", zwin32);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
