const std = @import("std");

pub const Package = struct {
    znoise: *std.Build.Module,
    znoise_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("znoise", pkg.znoise);
        exe.linkLibrary(pkg.znoise_c_cpp);
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    _: struct {},
) Package {
    const znoise = b.addModule("znoise", .{
        .source_file = .{ .path = thisDir() ++ "/src/znoise.zig" },
    });

    const znoise_c_cpp = b.addStaticLibrary(.{
        .name = "znoise",
        .target = target,
        .optimize = optimize,
    });
    znoise_c_cpp.linkLibC();
    znoise_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/FastNoiseLite" });
    znoise_c_cpp.addCSourceFile(.{
        .file = .{ .path = thisDir() ++ "/libs/FastNoiseLite/FastNoiseLite.c" },
        .flags = &.{ "-std=c99", "-fno-sanitize=undefined" },
    });

    return .{
        .znoise = znoise,
        .znoise_c_cpp = znoise_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run znoise tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{});
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "znoise-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/znoise.zig" },
        .target = target,
        .optimize = optimize,
    });

    const znoise_pkg = package(b, target, optimize, .{});
    znoise_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
