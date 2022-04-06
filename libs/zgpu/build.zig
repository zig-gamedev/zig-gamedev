const std = @import("std");
const glfw = @import("../mach-glfw/build.zig");
const gpu = @import("../mach-gpu/build.zig");
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
    //lib.want_lto = false;
    glfw.link(exe.builder, lib, options.glfw_options);
    gpu_dawn.link(exe.builder, lib, options.gpu_dawn_options);

    lib.install();
    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep, options: Options) void {
    glfw.link(exe.builder, exe, options.glfw_options);
    gpu_dawn.link(exe.builder, exe, options.gpu_dawn_options);

    //const lib = buildLibrary(exe, options);
    //exe.linkLibrary(lib);
}

pub const pkg = std.build.Pkg{
    .name = "zgpu",
    .path = .{ .path = thisDir() ++ "/src/zgpu.zig" },
    .dependencies = &.{ glfw.pkg, gpu.pkg },
};

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
