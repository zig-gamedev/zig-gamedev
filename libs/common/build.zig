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
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/imgui.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/imgui_widgets.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/imgui_tables.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/imgui_draw.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/imgui_demo.cpp" }, .flags = &.{""} });
    lib.addCSourceFile(.{ .file = .{ .path = "libs/imgui/cimgui.cpp" }, .flags = &.{""} });

    lib.addIncludePath(.{ .path = "../zmesh/libs/cgltf" });
    lib.addCSourceFile(.{
        .file = .{ .path = "../zmesh/libs/cgltf/cgltf.c" },
        .flags = &.{"-std=c99"},
    });

    var module = b.addModule("root", .{
        .root_source_file = .{ .path = "src/common.zig" },
        .imports = &.{
            .{ .name = "zwin32", .module = b.dependency("zwin32", .{
                .target = target,
            }).module("root") },
            .{ .name = "zd3d12", .module = b.dependency("zd3d12", .{
                .target = target,
                .debug_layer = options.zd3d12_debug_layer,
                .gbv = options.zd3d12_gbv,
            }).module("root") },
        },
    });

    module.addIncludePath(.{ .path = "libs/imgui" });
    module.addIncludePath(.{ .path = "../zmesh/libs/cgltf" });
}
