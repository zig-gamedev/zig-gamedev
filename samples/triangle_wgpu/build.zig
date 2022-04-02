const std = @import("std");
const system_sdk = @import("../../libs/mach-glfw/system_sdk.zig");
const glfw = @import("../../libs/mach-glfw/build.zig");
const gpu_dawn = @import("../../libs/mach-gpu-dawn/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "triangle_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const gpu_dawn_options = gpu_dawn.Options{
        .from_source = b.option(bool, "dawn-from-source", "Build Dawn from source") orelse false,
    };

    const exe = b.addExecutable("triangle_wgpu", thisDir() ++ "/src/triangle_wgpu.zig");
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    glfw.link(b, exe, .{ .system_sdk = .{ .set_sysroot = false } });
    gpu_dawn.link(b, exe, gpu_dawn_options);
    exe.addPackagePath("glfw", thisDir() ++ "/../../libs/mach-glfw/src/main.zig");

    return exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
