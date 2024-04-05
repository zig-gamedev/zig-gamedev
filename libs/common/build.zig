const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const options = .{
        .zd3d12_debug_layer = b.option(
            bool,
            "zd3d12_debug_layer",
            "Enable debug layer",
        ) orelse false,
        .zd3d12_gbv = b.option(
            bool,
            "zd3d12_gbv",
            "Enable GPU-based validation",
        ) orelse false,
    };

    const lib = b.addStaticLibrary(.{
        .name = "common",
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(lib);

    lib.linkLibC();
    if (target.result.abi != .msvc)
        lib.linkLibCpp();
    lib.linkSystemLibrary("imm32");

    lib.addIncludePath(.{ .path = "libs" });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/imgui.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/imgui_widgets.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/imgui_tables.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/imgui_draw.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/imgui_demo.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "samples/common/libs/imgui/cimgui.cpp" }, .flags = &.{""} });

    lib.addIncludePath(.{ .path = "libs/zmesh/libs/cgltf" });
    lib.addCSourceFile(.{
        .file = .{ .path = "libs/cgltf/cgltf.c" },
        .flags = &.{"-std=c99"},
    });

    module.addIncludePath(.{ .path = "samples/common/libs/imgui" });
    module.addIncludePath(.{ .path = "libs/zmesh/libs/cgltf" });
}
