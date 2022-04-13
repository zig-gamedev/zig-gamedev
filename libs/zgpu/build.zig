const std = @import("std");
const glfw = @import("../mach-glfw/build.zig");
const gpu_dawn = @import("../mach-gpu-dawn/build.zig");

pub const Options = struct {
    glfw_options: glfw.Options = .{},
    gpu_dawn_options: gpu_dawn.Options = .{},
};

fn buildLibrary(
    exe: *std.build.LibExeObjStep,
    options: Options,
) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zgpu", thisDir() ++ "/src/zgpu.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    glfw.link(exe.builder, lib, options.glfw_options);
    gpu_dawn.link(exe.builder, lib, options.gpu_dawn_options);

    // We add imgui to zgpu for simplicity - this is not ideal but works for now.
    lib.addIncludeDir(thisDir() ++ "/libs");
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/cimgui.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});

    lib.install();
    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep, options: Options) void {
    glfw.link(exe.builder, exe, options.glfw_options);
    gpu_dawn.link(exe.builder, exe, options.gpu_dawn_options);

    const lib = buildLibrary(exe, options);
    exe.linkLibrary(lib);

    // imgui
    exe.addIncludeDir(thisDir() ++ "/libs");
}

pub const pkg = std.build.Pkg{
    .name = "zgpu",
    .path = .{ .path = thisDir() ++ "/src/zgpu.zig" },
    .dependencies = &.{glfw.pkg},
};

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
