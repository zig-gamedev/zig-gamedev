const builtin = @import("builtin");
const std = @import("std");

const min_zig_version = std.SemanticVersion{ .major = 0, .minor = 11, .patch = 0, .pre = "dev.1580" };

pub fn build(b: *std.Build) void {
    ensureZigVersion() catch return;
    const options = Options{
        .optimize = b.standardOptimizeOption(.{}),
        .target = b.standardTargetOptions(.{}),
        .ztracy_enable = b.option(bool, "ztracy-enable", "Enable Tracy profiler") orelse false,
        .zd3d12_enable_debug_layer = b.option(
            bool,
            "zd3d12-enable-debug-layer",
            "Enable DirectX 12 debug layer",
        ) orelse false,
        .zd3d12_enable_gbv = b.option(
            bool,
            "zd3d12-enable-gbv",
            "Enable DirectX 12 GPU-Based Validation (GBV)",
        ) orelse false,
        .zpix_enable = b.option(bool, "zpix-enable", "Enable PIX for Windows profiler") orelse false,
    };
    ensureTarget(options.target) catch return;
    ensureGit(b.allocator) catch return;
    ensureGitLfs(b.allocator, "install") catch return;
    ensureGitLfs(b.allocator, "pull") catch return;
    ensureGitLfsContent("/samples/triangle_wgpu/triangle_wgpu_content/Roboto-Medium.ttf") catch return;

    // Fetch the latest Dawn/WebGPU binaries.
    const skip_dawn_update = b.option(bool, "skip-dawn-update", "Skip updating Dawn binaries") orelse false;
    if (!skip_dawn_update) {
        var child = std.ChildProcess.init(&.{ "git", "submodule", "update", "--init", "--remote" }, b.allocator);
        child.cwd = thisDir();
        child.stderr = std.io.getStdErr();
        child.stdout = std.io.getStdOut();
        _ = child.spawnAndWait() catch {
            std.log.err("Failed to fetch git submodule. Please try to re-clone.", .{});
            return;
        };
    }
    ensureGitLfsContent("/libs/zgpu/libs/dawn/x86_64-windows-gnu/dawn.lib") catch return;

    const zmath_pkg = zmath.Package.build(b, .{});
    const zpool_pkg = zpool.Package.build(b, .{});
    const zmesh_pkg = zmesh.Package.build(b, options.target, options.optimize, .{});
    const zglfw_pkg = zglfw.Package.build(b, options.target, options.optimize, .{});
    const znoise_pkg = znoise.Package.build(b, options.target, options.optimize, .{});
    const zstbi_pkg = zstbi.Package.build(b, options.target, options.optimize, .{});
    const zbullet_pkg = zbullet.Package.build(b, options.target, options.optimize, .{});
    const zgui_pkg = zgui.Package.build(b, options.target, options.optimize, .{
        .options = .{ .backend = .glfw_wgpu },
    });
    const zgpu_pkg = zgpu.Package.build(b, .{
        .options = .{ .uniforms_buffer_size = 4 * 1024 * 1024 },
        .deps = .{ .zpool = zpool_pkg.zpool, .zglfw = zglfw_pkg.zglfw },
    });
    const ztracy_pkg = ztracy.Package.build(b, options.target, options.optimize, .{
        .options = .{ .enable_ztracy = true, .enable_fibers = true },
    });
    const zphysics_pkg = zphysics.Package.build(b, options.target, options.optimize, .{
        .options = .{ .use_double_precision = false },
    });
    const zphysics_f64_pkg = zphysics.Package.build(b, options.target, options.optimize, .{
        .options = .{ .use_double_precision = true },
    });
    const zaudio_pkg = zaudio.Package.build(b, options.target, options.optimize, .{});
    const zflecs_pkg = zflecs.Package.build(b, options.target, options.optimize, .{});

    //
    // Sample applications
    //
    { // triangle wgpu
        const exe = triangle_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        installDemo(b, exe, "triangle_wgpu");
    }
    { // textured quad wgpu
        const exe = textured_quad_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zstbi", zstbi_pkg.zstbi);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        zstbi_pkg.link(exe);
        installDemo(b, exe, "textured_quad_wgpu");
    }
    { // gui test wgpu
        const exe = gui_test_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zstbi", zstbi_pkg.zstbi);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        zstbi_pkg.link(exe);
        installDemo(b, exe, "gui_test_wgpu");
    }
    { // physically based rendering wgpu
        const exe = physically_based_rendering_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zstbi", zstbi_pkg.zstbi);
        exe.addModule("zmesh", zmesh_pkg.zmesh);
        zmesh_pkg.link(exe);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        zstbi_pkg.link(exe);
        installDemo(b, exe, "physically_based_rendering_wgpu");
    }
    { // instanced pills wgpu
        const exe = instanced_pills_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        installDemo(b, exe, "instanced_pills_wgpu");
    }
    { // gamepad wgpu
        const exe = gamepad_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        installDemo(b, exe, "gamepad_wgpu");
    }
    { // layers wgpu
        const exe = layers_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        installDemo(b, exe, "layers_wgpu");
    }
    { // bullet physics test wgpu
        const exe = bullet_physics_test_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zmesh", zmesh_pkg.zmesh);
        exe.addModule("zbullet", zbullet_pkg.zbullet);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zmesh_pkg.link(exe);
        zglfw_pkg.link(exe);
        zbullet_pkg.link(exe);
        installDemo(b, exe, "bullet_physics_test_wgpu");
    }
    { // procedural mesh wgpu
        const exe = procedural_mesh_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zmesh", zmesh_pkg.zmesh);
        exe.addModule("ztracy", ztracy_pkg.ztracy);
        exe.addModule("znoise", znoise_pkg.znoise);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zmesh_pkg.link(exe);
        znoise_pkg.link(exe);
        zglfw_pkg.link(exe);
        ztracy_pkg.link(exe);
        installDemo(b, exe, "procedural_mesh_wgpu");
    }
    { // physics test wgpu
        const exe = physics_test_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zmesh", zmesh_pkg.zmesh);
        exe.addModule("zphysics", zphysics_pkg.zphysics);
        zmesh_pkg.link(exe);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        zphysics_pkg.link(exe);
        installDemo(b, exe, "physics_test_wgpu");
    }
    { // audio experiments wgpu
        const exe = audio_experiments_wgpu.build(b, options);
        exe.addModule("zgpu", zgpu_pkg.zgpu);
        exe.addModule("zgui", zgui_pkg.zgui);
        exe.addModule("zmath", zmath_pkg.zmath);
        exe.addModule("zglfw", zglfw_pkg.zglfw);
        exe.addModule("zaudio", zaudio_pkg.zaudio);
        zgui_pkg.link(exe);
        zgpu_pkg.link(exe);
        zglfw_pkg.link(exe);
        zaudio_pkg.link(exe);
        installDemo(b, exe, "audio_experiments_wgpu");
    }

    if (options.target.isWindows() and
        (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux))
    {
        const zwin32_pkg = zwin32.Package.build(b, .{});
        const zd3d12_pkg = zd3d12.Package.build(b, .{
            .options = .{
                .enable_debug_layer = options.zd3d12_enable_debug_layer,
                .enable_gbv = options.zd3d12_enable_gbv,
                .upload_heap_capacity = 32 * 1024 * 1024,
            },
            .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
        });
        const zpix_pkg = zpix.Package.build(b, .{
            .options = .{ .enable = options.zpix_enable },
            .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
        });
        const common_pkg = common.Package.build(b, .{
            .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_pkg.zd3d12 },
        });

        { // bindless
            const exe = bindless.build(b, options);
            exe.addModule("zmesh", zmesh_pkg.zmesh);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zstbi", zstbi_pkg.zstbi);
            zmesh_pkg.link(exe);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            zstbi_pkg.link(exe);
            installDemo(b, exe, "bindless");
        }
        { // minimal
            const exe = minimal.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            installDemo(b, exe, "minimal");
        }
        { // triangle
            const exe = triangle.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            installDemo(b, exe, "triangle");
        }
        { // simple raytracer
            const exe = simple_raytracer.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            exe.addModule("zpix", zpix_pkg.zpix);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            installDemo(b, exe, "simple_raytracer");
        }
        { // textured quad
            const exe = textured_quad.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            installDemo(b, exe, "textured_quad");
        }
        { // rasterization
            const exe = rasterization.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            exe.addModule("zmath", zmath_pkg.zmath);
            exe.addModule("zmesh", zmesh_pkg.zmesh);
            zmesh_pkg.link(exe);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            installDemo(b, exe, "rasterization");
        }
        { // mesh shader test
            const exe = mesh_shader_test.build(b, options);
            exe.addModule("zwin32", zwin32_pkg.zwin32);
            exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
            exe.addModule("common", common_pkg.common);
            exe.addModule("zmesh", zmesh_pkg.zmesh);
            zmesh_pkg.link(exe);
            zd3d12_pkg.link(exe);
            common_pkg.link(exe);
            installDemo(b, exe, "mesh_shader_test");
        }
        { // intros
            comptime var intro_index: u32 = 1;
            inline while (intro_index < 7) : (intro_index += 1) {
                const exe = intro.build(b, options, intro_index);
                exe.addModule("zmesh", zmesh_pkg.zmesh);
                exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
                exe.addModule("common", common_pkg.common);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                exe.addModule("zmath", zmath_pkg.zmath);
                exe.addModule("znoise", znoise_pkg.znoise);
                exe.addModule("zbullet", zbullet_pkg.zbullet);
                zmesh_pkg.link(exe);
                znoise_pkg.link(exe);
                zd3d12_pkg.link(exe);
                common_pkg.link(exe);
                zbullet_pkg.link(exe);
                const name = "intro" ++ comptime std.fmt.comptimePrint("{}", .{intro_index});
                installDemo(b, exe, name);
            }
        }

        if (builtin.target.os.tag == .windows) {
            const zd3d12_d2d_pkg = zd3d12.Package.build(b, .{
                .options = .{
                    .enable_debug_layer = options.zd3d12_enable_debug_layer,
                    .enable_gbv = options.zd3d12_enable_gbv,
                    .enable_d2d = true,
                },
                .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
            });
            const zxaudio2_pkg = zxaudio2.Package.build(b, .{
                .options = .{ .enable_debug_layer = options.zd3d12_enable_debug_layer },
                .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
            });

            { // intro 0
                const exe = intro.build(b, options, 0);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                exe.addModule("zd3d12", zd3d12_d2d_pkg.zd3d12);
                exe.addModule("common", common_pkg.common);
                zd3d12_d2d_pkg.link(exe);
                common_pkg.link(exe);
                installDemo(b, exe, "intro0");
            }
            { // vector graphics test
                const exe = vector_graphics_test.build(b, options);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                exe.addModule("zd3d12", zd3d12_d2d_pkg.zd3d12);
                exe.addModule("common", common_pkg.common);
                zd3d12_d2d_pkg.link(exe);
                common_pkg.link(exe);
                installDemo(b, exe, "vector_graphics_test");
            }
            { // directml convolution test
                const exe = directml_convolution_test.build(b, options);
                exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
                exe.addModule("zd3d12_options", zd3d12_pkg.zd3d12_options);
                exe.addModule("common", common_pkg.common);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                zd3d12_pkg.link(exe);
                common_pkg.link(exe);
                installDemo(b, exe, "directml_convolution_test");
            }
            { // audio playback test
                const exe = audio_playback_test.build(b, options);
                exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
                exe.addModule("common", common_pkg.common);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                zd3d12_pkg.link(exe);
                common_pkg.link(exe);
                installDemo(b, exe, "audio_playback_test");
            }
            { // audio experiments
                const exe = audio_experiments.build(b, options);
                exe.addModule("zd3d12", zd3d12_pkg.zd3d12);
                exe.addModule("common", common_pkg.common);
                exe.addModule("zwin32", zwin32_pkg.zwin32);
                exe.addModule("zmath", zmath_pkg.zmath);
                exe.addModule("zxaudio2", zxaudio2_pkg.zxaudio2);
                zd3d12_pkg.link(exe);
                common_pkg.link(exe);
                zxaudio2_pkg.link(exe);
                installDemo(b, exe, "audio_experiments");
            }
        }
    }

    //
    // Tests
    //
    const test_step = b.step("test", "Run all tests");

    const zmath_tests = zmath.buildTests(b, options.optimize, options.target);
    zmath_tests.addModule("zmath_options", zmath_pkg.zmath_options);
    test_step.dependOn(&zmath_tests.step);

    const zmesh_tests = @import("libs/zmesh/build.zig").buildTests(b, options.optimize, options.target);
    zmesh_pkg.link(zmesh_tests);
    test_step.dependOn(&zmesh_tests.step);

    const zstbi_tests = @import("libs/zstbi/build.zig").buildTests(b, options.optimize, options.target);
    zstbi_pkg.link(zstbi_tests);
    test_step.dependOn(&zstbi_tests.step);

    const znoise_tests = @import("libs/znoise/build.zig").buildTests(b, options.optimize, options.target);
    znoise_pkg.link(znoise_tests);
    test_step.dependOn(&znoise_tests.step);

    const zbullet_tests = @import("libs/zbullet/build.zig").buildTests(b, options.optimize, options.target);
    zbullet_tests.addModule("zmath", zmath_pkg.zmath);
    zbullet_pkg.link(zbullet_tests);
    test_step.dependOn(&zbullet_tests.step);

    const zglfw_tests = @import("libs/zglfw/build.zig").buildTests(b, options.optimize, options.target);
    zglfw_pkg.link(zglfw_tests);
    test_step.dependOn(&zglfw_tests.step);

    const zpool_tests = @import("libs/zpool/build.zig").buildTests(b, options.optimize, options.target);
    test_step.dependOn(&zpool_tests.step);

    const zjobs_tests = @import("libs/zjobs/build.zig").buildTests(b, options.optimize, options.target);
    test_step.dependOn(&zjobs_tests.step);

    const zgpu_tests = @import("libs/zgpu/build.zig").buildTests(b, options.optimize, options.target);
    zgpu_tests.want_lto = false; // TODO: Problems with LTO on Windows.
    zgpu_pkg.link(zgpu_tests);
    test_step.dependOn(&zgpu_tests.step);

    const zaudio_tests = @import("libs/zaudio/build.zig").buildTests(b, options.optimize, options.target);
    zaudio_pkg.link(zaudio_tests);
    test_step.dependOn(&zaudio_tests.step);

    const zflecs_tests = @import("libs/zflecs/build.zig").buildTests(b, options.optimize, options.target);
    zflecs_tests.addModule("zflecs_options", zflecs_pkg.zflecs_options);
    zflecs_pkg.link(zflecs_tests);
    test_step.dependOn(&zflecs_tests.step);

    const zphysics_tests = @import("libs/zphysics/build.zig").buildTests(
        b,
        options.optimize,
        options.target,
        false,
    );
    zphysics_tests.addModule("zphysics_options", zphysics_pkg.zphysics_options);
    zphysics_pkg.link(zphysics_tests);
    test_step.dependOn(&zphysics_tests.step);

    const zphysics_f64_tests = @import("libs/zphysics/build.zig").buildTests(
        b,
        options.optimize,
        options.target,
        true,
    );
    zphysics_f64_tests.addModule("zphysics_options", zphysics_f64_pkg.zphysics_options);
    zphysics_f64_pkg.link(zphysics_f64_tests);
    test_step.dependOn(&zphysics_f64_tests.step);

    //
    // Benchmarks
    //
    const benchmark_step = b.step("benchmark", "Run all benchmarks");
    {
        const exe = zmath.buildBenchmarks(b, options.target);
        exe.addModule("zmath", zmath_pkg.zmath);
        benchmark_step.dependOn(&exe.run().step);
    }
}

const zmath = @import("libs/zmath/build.zig");
const zglfw = @import("libs/zglfw/build.zig");
const zpool = @import("libs/zpool/build.zig");
const zmesh = @import("libs/zmesh/build.zig");
const znoise = @import("libs/znoise/build.zig");
const zstbi = @import("libs/zstbi/build.zig");
const zwin32 = @import("libs/zwin32/build.zig");
const zd3d12 = @import("libs/zd3d12/build.zig");
const zxaudio2 = @import("libs/zxaudio2/build.zig");
const zpix = @import("libs/zpix/build.zig");
const common = @import("libs/common/build.zig");
const zbullet = @import("libs/zbullet/build.zig");
const zgui = @import("libs/zgui/build.zig");
const zgpu = @import("libs/zgpu/build.zig");
const ztracy = @import("libs/ztracy/build.zig");
const zphysics = @import("libs/zphysics/build.zig");
const zaudio = @import("libs/zaudio/build.zig");
const zflecs = @import("libs/zflecs/build.zig");

const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");
const instanced_pills_wgpu = @import("samples/instanced_pills_wgpu/build.zig");
const layers_wgpu = @import("samples/layers_wgpu/build.zig");
const gamepad_wgpu = @import("samples/gamepad_wgpu/build.zig");
const physics_test_wgpu = @import("samples/physics_test_wgpu/build.zig");

const minimal = @import("samples/minimal/build.zig");
const triangle = @import("samples/triangle/build.zig");
const textured_quad = @import("samples/textured_quad/build.zig");
const mesh_shader_test = @import("samples/mesh_shader_test/build.zig");
const rasterization = @import("samples/rasterization/build.zig");
const vector_graphics_test = @import("samples/vector_graphics_test/build.zig");
const bindless = @import("samples/bindless/build.zig");
const simple_raytracer = @import("samples/simple_raytracer/build.zig");
const intro = @import("samples/intro/build.zig");
const audio_playback_test = @import("samples/audio_playback_test/build.zig");
const audio_experiments = @import("samples/audio_experiments/build.zig");
const directml_convolution_test = @import("samples/directml_convolution_test/build.zig");

pub const Options = struct {
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,

    ztracy_enable: bool,

    zd3d12_enable_debug_layer: bool,
    zd3d12_enable_gbv: bool,

    zpix_enable: bool,
};

fn installDemo(b: *std.Build, exe: *std.Build.CompileStep, comptime name: []const u8) void {
    // TODO: Problems with LTO on Windows.
    exe.want_lto = false;
    if (exe.optimize == .ReleaseFast)
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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

fn ensureTarget(cross: std.zig.CrossTarget) !void {
    const target = (std.zig.system.NativeTargetInfo.detect(cross) catch unreachable).target;

    const supported = switch (target.os.tag) {
        .windows => target.cpu.arch.isX86() and target.abi.isGnu(),
        .linux => (target.cpu.arch.isX86() or target.cpu.arch.isAARCH64()) and target.abi.isGnu(),
        .macos => blk: {
            if (!target.cpu.arch.isX86() and !target.cpu.arch.isAARCH64()) break :blk false;

            // If min. target macOS version is lesser than the min version we have available, then
            // our Dawn binary is incompatible with the target.
            const min_available = std.builtin.Version{ .major = 12, .minor = 0 };
            if (target.os.version_range.semver.min.order(min_available) == .lt) break :blk false;
            break :blk true;
        },
        else => false,
    };
    if (!supported) {
        std.log.err("\n" ++
            \\---------------------------------------------------------------------------
            \\
            \\Unsupported build target. Dawn/WebGPU binary for this target is not available.
            \\
            \\Following targets are supported:
            \\
            \\x86_64-windows-gnu
            \\x86_64-linux-gnu
            \\x86_64-macos.12-none
            \\aarch64-linux-gnu
            \\aarch64-macos.12-none
            \\
            \\---------------------------------------------------------------------------
            \\
        , .{});
        return error.TargetNotSupported;
    }
}

fn ensureZigVersion() !void {
    var installed_ver = @import("builtin").zig_version;
    installed_ver.build = null;

    if (installed_ver.order(min_zig_version) == .lt) {
        std.log.err("\n" ++
            \\---------------------------------------------------------------------------
            \\
            \\Installed Zig compiler version is too old.
            \\
            \\Min. required version: {any}
            \\Installed version: {any}
            \\
            \\Please install newer version and try again.
            \\Latest version can be found here: https://ziglang.org/download/
            \\
            \\---------------------------------------------------------------------------
            \\
        , .{ min_zig_version, installed_ver });
        return error.ZigIsTooOld;
    }
}

fn ensureGit(allocator: std.mem.Allocator) !void {
    const printErrorMsg = (struct {
        fn impl() void {
            std.log.err("\n" ++
                \\---------------------------------------------------------------------------
                \\
                \\'git version' failed. Is Git not installed?
                \\
                \\---------------------------------------------------------------------------
                \\
            , .{});
        }
    }).impl;
    const argv = &[_][]const u8{ "git", "version" };
    const result = std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = argv,
        .cwd = thisDir(),
    }) catch { // e.g. FileNotFound
        printErrorMsg();
        return error.GitNotFound;
    };
    defer {
        allocator.free(result.stderr);
        allocator.free(result.stdout);
    }
    if (result.term.Exited != 0) {
        printErrorMsg();
        return error.GitNotFound;
    }
}

fn ensureGitLfs(allocator: std.mem.Allocator, cmd: []const u8) !void {
    const printNoGitLfs = (struct {
        fn impl() void {
            std.log.err("\n" ++
                \\---------------------------------------------------------------------------
                \\
                \\Please install Git LFS (Large File Support) extension and run 'zig build' again.
                \\
                \\For more info about Git LFS see: https://git-lfs.github.com/
                \\
                \\---------------------------------------------------------------------------
                \\
            , .{});
        }
    }).impl;
    const argv = &[_][]const u8{ "git", "lfs", cmd };
    const result = std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = argv,
        .cwd = thisDir(),
    }) catch { // e.g. FileNotFound
        printNoGitLfs();
        return error.GitLfsNotFound;
    };
    defer {
        allocator.free(result.stderr);
        allocator.free(result.stdout);
    }
    if (result.term.Exited != 0) {
        printNoGitLfs();
        return error.GitLfsNotFound;
    }
}

fn ensureGitLfsContent(comptime file_path: []const u8) !void {
    const printNoGitLfsContent = (struct {
        fn impl() void {
            std.log.err("\n" ++
                \\---------------------------------------------------------------------------
                \\
                \\Something went wrong, Git LFS content has not been downloaded.
                \\
                \\Please try to re-clone the repo and build again.
                \\
                \\---------------------------------------------------------------------------
                \\
            , .{});
        }
    }).impl;
    const file = std.fs.openFileAbsolute(thisDir() ++ file_path, .{}) catch {
        printNoGitLfsContent();
        return error.GitLfsNoContent;
    };
    defer file.close();

    const size = file.getEndPos() catch {
        printNoGitLfsContent();
        return error.GitLfsNoContent;
    };
    if (size <= 1024) {
        printNoGitLfsContent();
        return error.GitLfsNoContent;
    }
}
