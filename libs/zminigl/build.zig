const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const tests = b.addTest("src/znoise.zig");
    tests.setBuildMode(b.standardReleaseOptions());
    tests.setTarget(b.standardTargetOptions(.{}));
    link(b, tests);

    const test_step = b.step("test", "Run library tests");
    test_step.dependOn(&tests.step);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}

fn buildLibrary(b: *std.build.Builder, step: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = b.addStaticLibrary("znoise", thisDir() ++ "/src/znoise.zig");

    lib.setBuildMode(step.build_mode);
    lib.setTarget(step.target);
    lib.want_lto = false;
    lib.addIncludeDir(thisDir() ++ "/libs/FastNoiseLite");
    lib.linkSystemLibrary("c");

    lib.addCSourceFile(
        thisDir() ++ "/libs/FastNoiseLite/FastNoiseLite.c",
        &.{ "-std=c99", "-fno-sanitize=undefined" },
    );

    lib.install();
    return lib;
}

pub fn link(b: *std.build.Builder, step: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(b, step);
    step.linkLibrary(lib);
}
