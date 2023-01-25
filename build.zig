const builtin = @import("builtin");
const std = @import("std");

const min_zig_version = std.SemanticVersion{ .major = 0, .minor = 11, .patch = 0, .pre = "dev.900" };

pub fn build(b: *std.build.Builder) void {
    ensureZigVersion() catch return;
    const options = Options{
        .build_mode = b.standardReleaseOptions(),
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

    //
    // Sample applications
    //
    installDemo(b, procedural_mesh_wgpu.build(b, options), "procedural_mesh_wgpu");
    installDemo(b, triangle_wgpu.build(b, options), "triangle_wgpu");
    installDemo(b, textured_quad_wgpu.build(b, options), "textured_quad_wgpu");
    installDemo(b, transparent_quads_wgpu.build(b, options), "transparent_quads_wgpu");
    installDemo(b, gui_test_wgpu.build(b, options), "gui_test_wgpu");
    installDemo(b, audio_experiments_wgpu.build(b, options), "audio_experiments_wgpu");
    installDemo(b, bullet_physics_test_wgpu.build(b, options), "bullet_physics_test_wgpu");
    installDemo(b, physically_based_rendering_wgpu.build(b, options), "physically_based_rendering_wgpu");
    installDemo(b, instanced_pills_wgpu.build(b, options), "instanced_pills_wgpu");
    installDemo(b, layers_wgpu.build(b, options), "layers_wgpu");
    installDemo(b, gamepad_wgpu.build(b, options), "gamepad_wgpu");
    installDemo(b, physics_test_wgpu.build(b, options), "physics_test_wgpu");

    if (options.target.isWindows() and
        (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux))
    {
        installDemo(b, minimal.build(b, options), "minimal");
        installDemo(b, triangle.build(b, options), "triangle");
        installDemo(b, simple_raytracer.build(b, options), "simple_raytracer");
        installDemo(b, bindless.build(b, options), "bindless");
        installDemo(b, textured_quad.build(b, options), "textured_quad");
        installDemo(b, rasterization.build(b, options), "rasterization");
        installDemo(b, mesh_shader_test.build(b, options), "mesh_shader_test");

        comptime var intro_index: u32 = 0;
        inline while (intro_index < 7) : (intro_index += 1) {
            const name = "intro" ++ comptime std.fmt.comptimePrint("{}", .{intro_index});
            installDemo(b, intro.build(b, options, intro_index), name);
        }
    }

    if (builtin.target.os.tag == .windows) {
        installDemo(b, vector_graphics_test.build(b, options), "vector_graphics_test");
        installDemo(b, audio_playback_test.build(b, options), "audio_playback_test");
        installDemo(b, audio_experiments.build(b, options), "audio_experiments");
        installDemo(b, directml_convolution_test.build(b, options), "directml_convolution_test");
    }

    //
    // Tests
    //
    const test_step = b.step("test", "Run all tests");

    const zjobs_tests = @import("libs/zjobs/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zjobs_tests.step);

    const zpool_tests = @import("libs/zpool/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zpool_tests.step);

    const zgpu_tests = @import("libs/zgpu/build.zig").buildTests(b, options.build_mode, options.target);
    zgpu_tests.want_lto = false; // TODO: Problems with LTO on Windows.
    zglfw.link(zgpu_tests);
    test_step.dependOn(&zgpu_tests.step);

    const zmath_tests = zmath.buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zmath_tests.step);

    const zbullet_tests = @import("libs/zbullet/build.zig").buildTests(b, options.build_mode, options.target);
    zbullet_tests.addPackage(zmath.pkg);
    test_step.dependOn(&zbullet_tests.step);

    const znoise_tests = @import("libs/znoise/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&znoise_tests.step);

    const zmesh_tests = @import("libs/zmesh/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zmesh_tests.step);

    const zaudio_tests = @import("libs/zaudio/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zaudio_tests.step);

    const zphysics_tests = @import("libs/zphysics/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
        .{},
    );
    test_step.dependOn(&zphysics_tests.step);

    const zphysics_f64_tests = @import("libs/zphysics/build.zig").buildTests(
        b,
        options.build_mode,
        options.target,
        .{ .use_double_precision = true },
    );
    test_step.dependOn(&zphysics_f64_tests.step);

    const zglfw_tests = @import("libs/zglfw/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zglfw_tests.step);

    const zstbi_tests = @import("libs/zstbi/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zstbi_tests.step);

    //
    // Benchmarks
    //
    const benchmark_step = b.step("benchmark", "Run all benchmarks");
    {
        const run_cmd = zmath.buildBenchmarks(b, options.target).run();
        benchmark_step.dependOn(&run_cmd.step);
    }
}

const zmath = @import("libs/zmath/build.zig");
const zglfw = @import("libs/zglfw/build.zig");

const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
const transparent_quads_wgpu = @import("samples/transparent_quads_wgpu/build.zig");
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
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,

    ztracy_enable: bool,

    zd3d12_enable_debug_layer: bool,
    zd3d12_enable_gbv: bool,

    zpix_enable: bool,
};

fn installDemo(b: *std.build.Builder, exe: *std.build.LibExeObjStep, comptime name: []const u8) void {
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
        .cwd = ".",
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
        .cwd = ".",
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
