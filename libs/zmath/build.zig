const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        enable_cross_platform_determinism: bool = true,
    };

    options: Options,
    zmath: *std.Build.Module,
    zmath_options: *std.Build.Module,

    pub fn build(
        b: *std.Build,
        args: struct {
            options: Options = .{},
        },
    ) Package {
        const step = b.addOptions();
        step.addOption(bool, "enable_cross_platform_determinism", args.options.enable_cross_platform_determinism);

        const zmath_options = step.createModule();

        const zmath = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
            .dependencies = &.{
                .{ .name = "zmath_options", .module = zmath_options },
            },
        });

        return .{
            .options = args.options,
            .zmath = zmath,
            .zmath_options = zmath_options,
        };
    }
};

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zmath_pkg = Package.build(b, .{});

    const tests = buildTests(b, optimize, target);
    tests.addModule("zmath_options", zmath_pkg.zmath_options);

    const test_step = b.step("test", "Run zmath tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

pub fn buildBenchmarks(
    b: *std.Build,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "benchmark",
        .root_source_file = .{ .path = thisDir() ++ "/src/benchmark.zig" },
        .target = target,
        .optimize = .ReleaseFast,
    });
    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
