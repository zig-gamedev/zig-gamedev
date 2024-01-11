const builtin = @import("builtin");
const std = @import("std");

pub const min_zig_version = std.SemanticVersion{ .major = 0, .minor = 12, .patch = 0, .pre = "dev.2139" };

pub fn build(b: *std.Build) void {
    //
    // Options and system checks
    //
    ensureZigVersion() catch return;
    const options = Options{
        .optimize = b.standardOptimizeOption(.{}),
        .target = b.standardTargetOptions(.{}),
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

    //
    // Packages
    //
    packagesCrossPlatform(b, options);

    if (options.target.result.os.tag == .windows and
        (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux))
    {
        packagesWindowsLinux(b, options);

        if (builtin.target.os.tag == .windows) {
            packagesWindows(b, options);
        }
    }

    //
    // Sample applications
    //
    samplesCrossPlatform(b, options);

    if (options.target.result.os.tag == .windows and
        (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux))
    {
        samplesWindowsLinux(b, options);

        if (builtin.target.os.tag == .windows) {
            samplesWindows(b, options);
        }
    }

    //
    // Tests
    //
    tests(b, options);

    //
    // Benchmarks
    //
    benchmarks(b, options);

    //
    // Experiments
    //
    if (b.option(bool, "experiments", "Build our prototypes and experimental programs") orelse false) {
        @import("experiments/build.zig").build(b, options);
    }
}

fn packagesCrossPlatform(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zopengl_pkg = zopengl.package(b, target, optimize, .{});
    zmath_pkg = zmath.package(b, target, optimize, .{});
    zpool_pkg = zpool.package(b, target, optimize, .{});
    zglfw_pkg = zglfw.package(b, target, optimize, .{});
    zsdl_pkg = zsdl.package(b, target, optimize, .{});
    zmesh_pkg = zmesh.package(b, target, optimize, .{});
    znoise_pkg = znoise.package(b, target, optimize, .{});
    zstbi_pkg = zstbi.package(b, target, optimize, .{});
    zbullet_pkg = zbullet.package(b, target, optimize, .{});
    zgui_glfw_wgpu_pkg = zgui.package(b, target, optimize, .{
        .options = .{ .backend = .glfw_wgpu },
    });
    zgui_glfw_gl_pkg = zgui.package(b, target, optimize, .{
        .options = .{ .backend = .glfw_opengl3 },
    });
    zgpu_pkg = zgpu.package(b, target, optimize, .{
        .options = .{ .uniforms_buffer_size = 4 * 1024 * 1024 },
        .deps = .{ .zpool = zpool_pkg.zpool, .zglfw = zglfw_pkg.zglfw },
    });
    ztracy_pkg = ztracy.package(b, target, optimize, .{
        .options = .{ .enable_ztracy = true, .enable_fibers = true },
    });
    zphysics_pkg = zphysics.package(b, target, optimize, .{});
    zaudio_pkg = zaudio.package(b, target, optimize, .{});
    zflecs_pkg = zflecs.package(b, target, optimize, .{});
}

fn packagesWindowsLinux(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zwin32_pkg = zwin32.package(b, target, optimize, .{});
    zd3d12_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
            .upload_heap_capacity = 32 * 1024 * 1024,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    zpix_pkg = zpix.package(b, target, optimize, .{
        .options = .{ .enable = options.zpix_enable },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    common_pkg = common.package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_pkg.zd3d12 },
    });
}

fn packagesWindows(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zd3d12_d2d_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
            .enable_d2d = true,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    common_d2d_pkg = common.package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_d2d_pkg.zd3d12 },
    });
    zxaudio2_pkg = zxaudio2.package(b, target, optimize, .{
        .options = .{ .enable_debug_layer = options.zd3d12_enable_debug_layer },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
}

fn samplesCrossPlatform(b: *std.Build, options: Options) void {
    const minimal_glfw_gl = @import("samples/minimal_glfw_gl/build.zig");
    const minimal_sdl_gl = @import("samples/minimal_sdl_gl/build.zig");
    const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
    const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
    const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
    const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
    const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
    const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
    const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");
    const minimal_zgpu_zgui = @import("samples/minimal_zgpu_zgui/build.zig");
    const minimal_zgui_glfw_gl = @import("samples/minimal_zgui_glfw_gl/build.zig");
    const instanced_pills_wgpu = @import("samples/instanced_pills_wgpu/build.zig");
    const layers_wgpu = @import("samples/layers_wgpu/build.zig");
    const gamepad_wgpu = @import("samples/gamepad_wgpu/build.zig");
    const physics_test_wgpu = @import("samples/physics_test_wgpu/build.zig");
    const monolith = @import("samples/monolith/build.zig");

    install(b, minimal_glfw_gl.build(b, options), "minimal_glfw_gl");
    install(b, minimal_sdl_gl.build(b, options), "minimal_sdl_gl");
    install(b, triangle_wgpu.build(b, options), "triangle_wgpu");
    install(b, textured_quad_wgpu.build(b, options), "textured_quad_wgpu");
    install(b, gui_test_wgpu.build(b, options), "gui_test_wgpu");
    install(b, minimal_zgpu_zgui.build(b, options), "minimal_zgpu_zgui");
    install(b, minimal_zgui_glfw_gl.build(b, options), "minimal_zgui_glfw_gl");
    install(b, physically_based_rendering_wgpu.build(b, options), "physically_based_rendering_wgpu");
    install(b, instanced_pills_wgpu.build(b, options), "instanced_pills_wgpu");
    install(b, gamepad_wgpu.build(b, options), "gamepad_wgpu");
    install(b, layers_wgpu.build(b, options), "layers_wgpu");
    install(b, bullet_physics_test_wgpu.build(b, options), "bullet_physics_test_wgpu");
    install(b, procedural_mesh_wgpu.build(b, options), "procedural_mesh_wgpu");
    install(b, physics_test_wgpu.build(b, options), "physics_test_wgpu");
    install(b, monolith.build(b, options), "monolith");
    install(b, audio_experiments_wgpu.build(b, options), "audio_experiments_wgpu");
}

fn samplesWindowsLinux(b: *std.Build, options: Options) void {
    const minimal_d3d12 = @import("samples/minimal_d3d12/build.zig");
    const textured_quad = @import("samples/textured_quad/build.zig");
    const triangle = @import("samples/triangle/build.zig");
    const mesh_shader_test = @import("samples/mesh_shader_test/build.zig");
    const rasterization = @import("samples/rasterization/build.zig");
    const bindless = @import("samples/bindless/build.zig");
    //const simple_raytracer = @import("samples/simple_raytracer/build.zig");

    install(b, minimal_d3d12.build(b, options), "minimal_d3d12");
    install(b, bindless.build(b, options), "bindless");
    install(b, triangle.build(b, options), "triangle");
    //install(b, simple_raytracer.build(b, options), "simple_raytracer");
    install(b, textured_quad.build(b, options), "textured_quad");
    install(b, rasterization.build(b, options), "rasterization");
    install(b, mesh_shader_test.build(b, options), "mesh_shader_test");
}

fn samplesWindows(b: *std.Build, options: Options) void {
    const audio_playback_test = @import("samples/audio_playback_test/build.zig");
    const audio_experiments = @import("samples/audio_experiments/build.zig");
    const vector_graphics_test = @import("samples/vector_graphics_test/build.zig");
    const directml_convolution_test = @import("samples/directml_convolution_test/build.zig");

    install(b, vector_graphics_test.build(b, options), "vector_graphics_test");
    install(b, directml_convolution_test.build(b, options), "directml_convolution_test");
    install(b, audio_playback_test.build(b, options), "audio_playback_test");
    install(b, audio_experiments.build(b, options), "audio_experiments");
}

fn tests(b: *std.Build, options: Options) void {
    const test_step = b.step("test", "Run all tests");

    test_step.dependOn(zmath.runTests(b, options.optimize, options.target));
    test_step.dependOn(zmesh.runTests(b, options.optimize, options.target));
    test_step.dependOn(zstbi.runTests(b, options.optimize, options.target));
    test_step.dependOn(znoise.runTests(b, options.optimize, options.target));
    test_step.dependOn(zglfw.runTests(b, options.optimize, options.target));
    test_step.dependOn(zpool.runTests(b, options.optimize, options.target));
    test_step.dependOn(zjobs.runTests(b, options.optimize, options.target));
    test_step.dependOn(zaudio.runTests(b, options.optimize, options.target));
    test_step.dependOn(zflecs.runTests(b, options.optimize, options.target));
    test_step.dependOn(zphysics.runTests(b, options.optimize, options.target));
    test_step.dependOn(zopengl.runTests(b, options.optimize, options.target));
    test_step.dependOn(zgui.runTests(b, options.optimize, options.target));

    // TODO: zsdl tests not included in top-level tests until https://github.com/michal-z/zig-gamedev/issues/312 is resolved
    // test_step.dependOn(zsdl.runTests(b, options.optimize, options.target, .sdl2));
    // test_step.dependOn(zsdl.runTests(b, options.optimize, options.target, .sdl3));

    test_step.dependOn(ztracy_pkg.makeTestStep(b));
}

fn benchmarks(b: *std.Build, options: Options) void {
    const benchmark_step = b.step("benchmark", "Run all benchmarks");

    benchmark_step.dependOn(zmath.runBenchmarks(b, options.target));
}

pub var zmath_pkg: zmath.Package = undefined;
pub var znoise_pkg: znoise.Package = undefined;
pub var zopengl_pkg: zopengl.Package = undefined;
pub var zsdl_pkg: zsdl.Package = undefined;
pub var zpool_pkg: zpool.Package = undefined;
pub var zmesh_pkg: zmesh.Package = undefined;
pub var zglfw_pkg: zglfw.Package = undefined;
pub var zstbi_pkg: zstbi.Package = undefined;
pub var zbullet_pkg: zbullet.Package = undefined;
pub var zgui_glfw_wgpu_pkg: zgui.Package = undefined;
pub var zgui_glfw_gl_pkg: zgui.Package = undefined;
pub var zgpu_pkg: zgpu.Package = undefined;
pub var ztracy_pkg: ztracy.Package = undefined;
pub var zphysics_pkg: zphysics.Package = undefined;
pub var zaudio_pkg: zaudio.Package = undefined;
pub var zflecs_pkg: zflecs.Package = undefined;

pub var zwin32_pkg: zwin32.Package = undefined;
pub var zd3d12_pkg: zd3d12.Package = undefined;
pub var zpix_pkg: zpix.Package = undefined;
pub var zxaudio2_pkg: zxaudio2.Package = undefined;
pub var common_pkg: common.Package = undefined;
pub var common_d2d_pkg: common.Package = undefined;
pub var zd3d12_d2d_pkg: zd3d12.Package = undefined;

const zsdl = @import("zsdl");
const zopengl = @import("zopengl");
const zmath = @import("zmath");
const zglfw = @import("zglfw");
const zpool = @import("zpool");
const zjobs = @import("zjobs");
const zmesh = @import("zmesh");
const znoise = @import("znoise");
const zstbi = @import("zstbi");
const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");
const zxaudio2 = @import("zxaudio2");
const zpix = @import("zpix");
const common = @import("common");
const zbullet = @import("zbullet");
const zgui = @import("zgui");
const zgpu = @import("zgpu");
const ztracy = @import("ztracy");
const zphysics = @import("zphysics");
const zaudio = @import("zaudio");
const zflecs = @import("zflecs");

pub const Options = struct {
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,

    zd3d12_enable_debug_layer: bool,
    zd3d12_enable_gbv: bool,

    zpix_enable: bool,
};

fn install(b: *std.Build, exe: *std.Build.Step.Compile, comptime name: []const u8) void {
    // TODO: Problems with LTO on Windows.
    exe.want_lto = false;
    if (exe.root_module.optimize) |o| {
        if (o == .ReleaseFast) exe.root_module.strip = true;
    }

    //comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    //comptime _ = std.mem.replace(u8, name, "", "", desc_name[0..]);
    //comptime var desc_size = std.mem.indexOf(u8, &desc_name, "\x00").?;

    const install_step = b.step(name, "Build '" ++ name ++ "' demo");
    install_step.dependOn(&b.addInstallArtifact(exe, .{}).step);

    const run_step = b.step(name ++ "-run", "Run '" ++ name ++ "' demo");
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install_step);
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

fn ensureTarget(cross: std.Build.ResolvedTarget) !void {
    const target = cross.result;

    const supported = switch (target.os.tag) {
        .windows => target.cpu.arch.isX86() and target.abi.isGnu(),
        .linux => (target.cpu.arch.isX86() or target.cpu.arch.isAARCH64()) and target.abi.isGnu(),
        .macos => blk: {
            if (!target.cpu.arch.isX86() and !target.cpu.arch.isAARCH64()) break :blk false;

            // If min. target macOS version is lesser than the min version we have available, then
            // our Dawn binary is incompatible with the target.
            if (target.os.version_range.semver.min.order(
                .{ .major = 12, .minor = 0, .patch = 0 },
            ) == .lt) break :blk false;
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
            \\x86_64-macos.12.0.0-none
            \\aarch64-linux-gnu
            \\aarch64-macos.12.0.0-none
            \\
            \\---------------------------------------------------------------------------
            \\
        , .{});
        return error.TargetNotSupported;
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
    const result = std.ChildProcess.run(.{
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
    const result = std.ChildProcess.run(.{
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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
