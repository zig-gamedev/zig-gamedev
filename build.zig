const std = @import("std");

const audio_experiments = @import("samples/audio_experiments/build.zig");
const audio_playback_test = @import("samples/audio_playback_test/build.zig");
const bindless = @import("samples/bindless/build.zig");
const bullet_physics_test = @import("samples/bullet_physics_test/build.zig");
const directml_convolution_test = @import("samples/directml_convolution_test/build.zig");
const mesh_shader_test = @import("samples/mesh_shader_test/build.zig");
const physically_based_rendering = @import("samples/physically_based_rendering/build.zig");
const rasterization = @import("samples/rasterization/build.zig");
const simple3d = @import("samples/simple3d/build.zig");
const simple_raytracer = @import("samples/simple_raytracer/build.zig");
const textured_quad = @import("samples/textured_quad/build.zig");
const triangle = @import("samples/triangle/build.zig");
const vector_graphics_test = @import("samples/vector_graphics_test/build.zig");
const intro = @import("samples/intro/build.zig");

pub const Options = struct {
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
    enable_pix: bool,
    enable_dx_debug: bool,
    enable_dx_gpu_debug: bool,
    tracy: ?[]const u8,
};

pub fn build(b: *std.build.Builder) void {
    const enable_pix = b.option(bool, "enable-pix", "Enable PIX GPU events and markers") orelse false;
    const enable_dx_debug = b.option(
        bool,
        "enable-dx-debug",
        "Enable debug layer for D3D12, D2D1, DirectML and DXGI",
    ) orelse false;
    const enable_dx_gpu_debug = b.option(
        bool,
        "enable-dx-gpu-debug",
        "Enable GPU-based validation for D3D12",
    ) orelse false;
    const tracy = b.option([]const u8, "tracy", "Enable Tracy profiler integration (supply path to Tracy source)");

    const options = Options{
        .build_mode = b.standardReleaseOptions(),
        .target = b.standardTargetOptions(.{}),
        .enable_pix = enable_pix,
        .enable_dx_debug = enable_dx_debug,
        .enable_dx_gpu_debug = enable_dx_gpu_debug,
        .tracy = tracy,
    };

    installDemo(b, audio_experiments.build(b, options), "audio_experiments");
    installDemo(b, audio_playback_test.build(b, options), "audio_playback_test");
    installDemo(b, bindless.build(b, options), "bindless");
    installDemo(b, bullet_physics_test.build(b, options), "bullet_physics_test");
    installDemo(b, directml_convolution_test.build(b, options), "directml_convolution_test");
    installDemo(b, mesh_shader_test.build(b, options), "mesh_shader_test");
    installDemo(b, physically_based_rendering.build(b, options), "physically_based_rendering");
    installDemo(b, rasterization.build(b, options), "rasterization");
    installDemo(b, simple3d.build(b, options), "simple3d");
    installDemo(b, simple_raytracer.build(b, options), "simple_raytracer");
    installDemo(b, textured_quad.build(b, options), "textured_quad");
    installDemo(b, vector_graphics_test.build(b, options), "vector_graphics_test");
    installDemo(b, triangle.build(b, options), "triangle");
    installDemo(b, intro.build(b, options, 0), "intro0");
    installDemo(b, intro.build(b, options, 1), "intro1");
    installDemo(b, intro.build(b, options, 2), "intro2");
    installDemo(b, intro.build(b, options, 3), "intro3");
    installDemo(b, intro.build(b, options, 4), "intro4");
    installDemo(b, intro.build(b, options, 5), "intro5");
    installDemo(b, intro.build(b, options, 6), "intro6");
}

fn installDemo(b: *std.build.Builder, exe: *std.build.LibExeObjStep, comptime name: []const u8) void {
    const install = b.step(name, "Build '" ++ name ++ "' demo");
    install.dependOn(&b.addInstallArtifact(exe).step);

    const run_step = b.step(name ++ "_run", "Run '" ++ name ++ "' demo");
    const run_cmd = exe.run();
    run_cmd.step.dependOn(install);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install);
}
