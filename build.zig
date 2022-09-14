const builtin = @import("builtin");
const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    ensureGitLfs(b.allocator);

    var options = Options{
        .build_mode = b.standardReleaseOptions(),
        .target = b.standardTargetOptions(.{}),
    };

    options.ztracy_enable = b.option(bool, "ztracy-enable", "Enable Tracy profiler") orelse false;

    //
    // Cross-platform demos
    //
    if (!builtin.is_test) {
        installDemo(b, procedural_mesh_wgpu.build(b, options), "procedural_mesh_wgpu");
        installDemo(b, triangle_wgpu.build(b, options), "triangle_wgpu");
        installDemo(b, textured_quad_wgpu.build(b, options), "textured_quad_wgpu");
        installDemo(b, gui_test_wgpu.build(b, options), "gui_test_wgpu");
        installDemo(b, audio_experiments_wgpu.build(b, options), "audio_experiments_wgpu");
        installDemo(b, bullet_physics_test_wgpu.build(b, options), "bullet_physics_test_wgpu");
        installDemo(
            b,
            physically_based_rendering_wgpu.build(b, options),
            "physically_based_rendering_wgpu",
        );
        // TODO: Re-enable zenet when stage3 compiler is more stable
        //installDemo(b, network_test.build(b, options), "network_test");
        installDemo(b, wrinkles.build(b, options), "wrinkles");
    }

    //
    // Windows-only demos
    //
    if (!builtin.is_test and @import("builtin").target.os.tag == .windows and
        @import("builtin").zig_backend == .stage1)
    {
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

    //
    // Tests
    //
    const test_step = b.step("test", "Run all tests");

    const zpool_tests = @import("libs/zpool/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&zpool_tests.step);

    const zgpu_tests = @import("libs/zgpu/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    zglfw.link(zgpu_tests);
    test_step.dependOn(&zgpu_tests.step);

    const zmath_tests = zmath.buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zmath_tests.step);

    const zbullet_tests = @import("libs/zbullet/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    zbullet_tests.addPackage(zmath.pkg);
    test_step.dependOn(&zbullet_tests.step);

    const znoise_tests = @import("libs/znoise/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&znoise_tests.step);

    const zmesh_tests = @import("libs/zmesh/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&zmesh_tests.step);

    const zaudio_tests = @import("libs/zaudio/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&zaudio_tests.step);

    const zjolt_tests = @import("libs/zjolt/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&zjolt_tests.step);

    const zglfw_tests = @import("libs/zglfw/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&zglfw_tests.step);

    const znetwork_tests = @import("libs/znetwork/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
    );
    test_step.dependOn(&znetwork_tests.step);

    //
    // Benchmarks
    //
    if (!builtin.is_test) {
        const benchmark_step = b.step("benchmark", "Run all benchmarks");
        {
            const run_cmd = zmath.buildBenchmarks(b, options.target).run();
            benchmark_step.dependOn(&run_cmd.step);
        }
    }
}

const zmath = @import("libs/zmath/build.zig");
const zglfw = @import("libs/zglfw/build.zig");

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
const network_test = @import("samples/network_test/build.zig");
const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");
const wrinkles = @import("samples/wrinkles/build.zig");

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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

fn ensureGitLfs(allocator: std.mem.Allocator) void {
    const printErrorMsg = (struct {
        fn impl() void {
            std.debug.print(
                \\---------------------------------------------------------------------------
                \\git-lfs not found.
                \\
                \\Please install Git LFS (Large File Support) and run (in the repo):
                \\
                \\git lfs install
                \\git lfs pull
                \\
                \\For more info please see: https://git-lfs.github.com/
                \\---------------------------------------------------------------------------
                \\
            , .{});
        }
    }).impl;

    const argv = &[_][]const u8{ "git-lfs", "--version" };
    const result = std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = argv,
        .cwd = ".",
    }) catch { // e.g. FileNotFound
        printErrorMsg();
        std.process.exit(1);
    };
    defer {
        allocator.free(result.stderr);
        allocator.free(result.stdout);
    }
    if (result.term.Exited != 0) {
        printErrorMsg();
        std.process.exit(1);
    }
}
