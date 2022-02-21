const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    _ = b;
    //const tests = b.addTest("src/zbullet.zig");
    //const zmath = std.build.Pkg{
    //    .name = "zmath",
    //    .path = .{ .path = thisDir() ++ "/../zmath/zmath.zig" },
    //};
    //tests.addPackage(zmath);
    //tests.setBuildMode(b.standardReleaseOptions());
    //tests.setTarget(b.standardTargetOptions(.{}));
    //link(b, tests);

    //const test_step = b.step("test", "Run library tests");
    //test_step.dependOn(&tests.step);
}

pub fn link(b: *std.build.Builder, step: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(b, step);
    step.linkLibrary(lib);
    step.addIncludeDir(thisDir() ++ "/src/c");
}

fn buildLibrary(b: *std.build.Builder, step: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = b.addStaticLibrary("common", thisDir() ++ "/src/common.zig");

    lib.setBuildMode(step.build_mode);
    lib.setTarget(step.target);
    lib.want_lto = false;
    lib.addIncludeDir(thisDir() ++ "/src/c");

    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");
    lib.linkSystemLibrary("imm32");

    lib.addCSourceFile(thisDir() ++ "/src/c/imgui/imgui.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/imgui/imgui_widgets.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/imgui/imgui_tables.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/imgui/imgui_draw.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/imgui/imgui_demo.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/cimgui.cpp", &.{""});

    lib.addCSourceFile(thisDir() ++ "/src/c/cgltf.c", &.{"-std=c99"});

    lib.addCSourceFile(thisDir() ++ "/src/c/stb_image.c", &.{"-std=c99"});

    lib.addCSourceFile(thisDir() ++ "/src/c/meshoptimizer/clusterizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/meshoptimizer/indexgenerator.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/meshoptimizer/vcacheoptimizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/src/c/meshoptimizer/vfetchoptimizer.cpp", &.{""});

    lib.install();
    return lib;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
