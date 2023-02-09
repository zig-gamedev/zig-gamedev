const std = @import("std");

pub const Options = struct {
    module_support: bool = true,
    pipeline_support: bool = true,
    meta_support: bool = true,
    http_support: bool = false,
};

pub const Package = struct {
    module: *std.Build.Module,
    options: Options,
    options_module: *std.Build.Module,
};

pub fn package(
    b: *std.Build,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "pipeline_support", args.options.pipeline_support);
    step.addOption(bool, "http_support", args.options.http_support);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zflecs.zig" },
        .dependencies = &.{
            .{ .name = "zflecs_options", .module = options_module },
        },
    });

    return .{
        .module = module,
        .options = args.options,
        .options_module = options_module,
    };
}

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zflecs.zig" },
        .target = target,
        .optimize = build_mode,
    });
    link(tests, .{});
    const zflecs_pkg = package(b, .{});
    tests.addModule("zflecs_options", zflecs_pkg.options_module);
    return tests;
}

pub fn link(exe: *std.Build.CompileStep, options: Options) void {
    exe.linkLibC();

    const pipeline_def = if (!options.pipeline_support) "-DFLECS_NO_PIPELINE" else "";

    exe.addIncludePath(thisDir() ++ "/libs/flecs");
    exe.addCSourceFile(
        thisDir() ++ "/libs/flecs/flecs.c",
        &.{ "-std=c99", "-fno-sanitize=undefined", pipeline_def },
    );
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
