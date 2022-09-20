const builtin = @import("builtin");
const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    // TODO: Make 'windows' branch work with stage2/stage3 compiler.
    if (@import("builtin").target.os.tag != .windows or @import("builtin").zig_backend != .stage1) {
        std.debug.print(
            "This branch compiles only on Windows. It requires stage1 compiler for now (zig build -fstage1)\n",
            .{},
        );
        std.process.exit(1);
    }

    var options = Options{
        .build_mode = b.standardReleaseOptions(),
        .target = b.standardTargetOptions(.{}),
    };

    options.ztracy_enable = b.option(bool, "ztracy-enable", "Enable Tracy profiler") orelse false;

    options.zpix_enable = b.option(
        bool,
        "zpix-enable",
        "Enable PIX GPU events and markers",
    ) orelse false;
    options.enable_dx_debug = b.option(
        bool,
        "enable-dx-debug",
        "Enable debug layer for D3D12, D2D1, DirectML and DXGI",
    ) orelse false;
    options.enable_dx_gpu_debug = b.option(
        bool,
        "enable-dx-gpu-debug",
        "Enable GPU-based validation for D3D12",
    ) orelse false;

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
    installDemo(b, minimal.build(b, options), "minimal");
    installDemo(b, procedural_mesh.build(b, options), "procedural_mesh");

    comptime var intro_index: u32 = 0;
    inline while (intro_index < 7) : (intro_index += 1) {
        const name = "intro" ++ comptime std.fmt.comptimePrint("{}", .{intro_index});
        installDemo(b, intro.build(b, options, intro_index), name);
    }
}

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
const minimal = @import("samples/minimal/build.zig");
const procedural_mesh = @import("samples/procedural_mesh/build.zig");

pub const Options = struct {
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,

    ztracy_enable: bool = false,
    zpix_enable: bool = false,

    enable_dx_debug: bool = false,
    enable_dx_gpu_debug: bool = false,
};

fn installDemo(
    b: *std.build.Builder,
    exe: *std.build.LibExeObjStep,
    comptime name: []const u8,
) void {
    // TODO: Problems with LTO on Windows.
    exe.want_lto = false;
    if (exe.build_mode == .ReleaseFast)
        exe.strip = true;

    comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    comptime _ = std.mem.replace(u8, name, "_", " ", desc_name[0..]);
    comptime var desc_size = std.mem.indexOf(u8, &desc_name, "\x00").?;

    const install = b.step(name, "Build '" ++ desc_name[0..desc_size] ++ "' demo");
    install.dependOn(&b.addInstallArtifact(exe).step);

    const run_step = b.step(name ++ "-run", "Run '" ++ desc_name[0..desc_size] ++ "' demo");
    const run_cmd = exe.run();
    run_cmd.step.dependOn(install);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install);
}
