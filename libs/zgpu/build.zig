const std = @import("std");
const glfw = @import("../mach-glfw/build.zig");
const gpu = @import("../mach-gpu/build.zig");
const gpu_dawn = @import("../mach-gpu-dawn/build.zig");

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zgpu tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zgpu.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(b, tests);
    return tests;
}

pub const Options = struct {
    glfw_options: glfw.Options = .{},
    gpu_dawn_options: gpu_dawn.Options = .{},
};

fn buildLibrary(
    b: *std.build.Builder,
    step: *std.build.LibExeObjStep,
    options: Options,
) *std.build.LibExeObjStep {
    const lib = b.addStaticLibrary("zgpu", thisDir() ++ "/src/zgpu.zig");

    lib.setBuildMode(step.build_mode);
    lib.setTarget(step.target);
    //lib.want_lto = false;
    glfw.link(b, lib, options.glfw_options);
    gpu_dawn.link(b, lib, options.gpu_dawn_options);

    lib.install();
    return lib;
}

pub fn link(b: *std.build.Builder, exe: *std.build.LibExeObjStep, options: Options) void {
    glfw.link(b, exe, options.glfw_options);
    gpu_dawn.link(b, exe, options.gpu_dawn_options);

    //const lib = buildLibrary(b, exe, options);
    //exe.linkLibrary(lib);
}

pub const pkg = .{
    .name = "zgpu",
    .path = .{ .path = thisDir() ++ "/src/zgpu.zig" },
    .dependencies = &.{ glfw.pkg, gpu.pkg },
};

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
