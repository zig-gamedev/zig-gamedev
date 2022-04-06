const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zmesh",
    .path = .{ .path = thisDir() ++ "/src/zmesh.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zmesh tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zmesh.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zmesh", thisDir() ++ "/src/zmesh.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.want_lto = false;
    lib.addIncludeDir(thisDir() ++ "/libs/par_shapes");
    lib.linkSystemLibrary("c");

    lib.addCSourceFile(
        thisDir() ++ "/libs/par_shapes/par_shapes.c",
        &.{ "-std=c99", "-fno-sanitize=undefined" },
    );

    lib.install();
    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
