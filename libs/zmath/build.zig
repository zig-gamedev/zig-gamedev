const std = @import("std");

pub const Options = struct {
    enable_cross_platform_determinism: bool = true,
};

pub const Package = struct {
    options: Options,
    zmath: *std.Build.Module,
    zmath_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zmath", pkg.zmath);
        exe.root_module.addImport("zmath_options", pkg.zmath_options);
    }
};

pub fn package(
    b: *std.Build,
    _: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(
        bool,
        "enable_cross_platform_determinism",
        args.options.enable_cross_platform_determinism,
    );

    const zmath_options = step.createModule();

    const zmath = b.addModule("zmath", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .imports = &.{
            .{ .name = "zmath_options", .module = zmath_options },
        },
    });

    return .{
        .options = args.options,
        .zmath = zmath,
        .zmath_options = zmath_options,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = package(b, target, optimize, .{ .options = .{
        .enable_cross_platform_determinism = b.option(
            bool,
            "enable_cross_platform_determinism",
            "Whether to enable cross-platform determinism.",
        ) orelse true,
    } });

    const test_step = b.step("test", "Run zmath tests");
    test_step.dependOn(runTests(b, optimize, target));

    const benchmark_step = b.step("benchmark", "Run zmath benchmarks");
    benchmark_step.dependOn(runBenchmarks(b, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zmath-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zmath_pkg = package(b, target, optimize, .{});
    tests.root_module.addImport("zmath_options", zmath_pkg.zmath_options);

    return &b.addRunArtifact(tests).step;
}

pub fn runBenchmarks(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const exe = b.addExecutable(.{
        .name = "zmath-benchmarks",
        .root_source_file = .{ .path = thisDir() ++ "/src/benchmark.zig" },
        .target = target,
        .optimize = .ReleaseFast,
    });

    const zmath_pkg = package(b, target, .ReleaseFast, .{});
    exe.root_module.addImport("zmath", zmath_pkg.zmath);

    return &b.addRunArtifact(exe).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
