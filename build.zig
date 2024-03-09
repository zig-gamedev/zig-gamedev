const builtin = @import("builtin");
const std = @import("std");

pub const min_zig_version = std.SemanticVersion{ .major = 0, .minor = 12, .patch = 0, .pre = "dev.2063" };

pub fn build(b: *std.Build) void {
    ensureZigVersion() catch return;

    if (checkGitLfsContent() == false) {
        ensureGit(b.allocator) catch return;
        ensureGitLfs(b.allocator, "install") catch return;
        ensureGitLfs(b.allocator, "pull") catch return;
        if (checkGitLfsContent() == false) {
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
            return;
        }
    }

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const options = Options{
        .optimize = optimize,
        .target = target,
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
        .zpix_enable = b.option(
            bool,
            "zpix-enable",
            "Enable PIX for Windows profiler",
        ) orelse false,
    };

    if (target.result.os.tag == .windows) {
        install_xaudio2(b.getInstallStep(), .bin);
        install_d3d12(b.getInstallStep(), .bin);
        install_directml(b.getInstallStep(), .bin);
    }

    install_sdl2(b.getInstallStep(), target.result, .bin);
    install_sdl2_ttf(b.getInstallStep(), target.result, .bin);

    //
    // Sample applications
    //
    samples(b, options);
    if (target.result.os.tag == .windows) {
        if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
            samplesWindowsLinux(b, options);

            if (builtin.os.tag == .windows) {
                samplesWindows(b, options);
            }
        }
    }

    //
    // Tests
    //
    const test_step = b.step("test", "Run all tests");
    tests(b, target, optimize, test_step);
    if (builtin.os.tag == .windows) {
        testsWindows(b, target, optimize, test_step);
    }

    //
    // Benchmarks
    //
    {
        const benchmark_step = b.step("benchmark", "Run all benchmarks");

        const zmath = b.dependency("zmath", .{
            .optimize = .ReleaseFast,
        });
        benchmark_step.dependOn(&b.addRunArtifact(zmath.artifact("zmath-benchmarks")).step);
    }

    //
    // Experiments
    //
    if (b.option(bool, "experiments", "Build our prototypes and experimental programs") orelse false) {
        @import("experiments/build.zig").build(b, options);
    }
}

pub fn install_xaudio2(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const zwin32 = b.dependency("zwin32", .{});
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = zwin32.path("bin/x64/xaudio2_9redist.dll").getPath(b) },
            install_dir,
            "xaudio2_9redist.dll",
        ).step,
    );
}

pub fn install_d3d12(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const zwin32 = b.dependency("zwin32", .{});
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = zwin32.path("bin/x64/D3D12Core.dll").getPath(b) },
            install_dir,
            "d3d12/D3D12Core.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = zwin32.path("bin/x64/D3D12SDKLayers.dll").getPath(b) },
            install_dir,
            "d3d12/D3D12SDKLayers.dll",
        ).step,
    );
}

pub fn install_directml(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const zwin32 = b.dependency("zwin32", .{});
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = zwin32.path("bin/x64/DirectML.dll").getPath(b) },
            install_dir,
            "DirectML.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = zwin32.path("bin/x64/DirectML.Debug.dll").getPath(b) },
            install_dir,
            "DirectML.Debug.dll",
        ).step,
    );
}

pub fn install_sdl2(
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const zsdl = b.dependency("zsdl", .{});
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{ .path = zsdl.path("libs/x86_64-windows-gnu/bin/SDL2.dll").getPath(b) },
                        install_dir,
                        "SDL2.dll",
                    ).step,
                );
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{ .path = zsdl.path("libs/x86_64-linux-gnu/lib/libSDL2.so").getPath(b) },
                        install_dir,
                        "libSDL2.so",
                    ).step,
                );
            }
        },
        .macos => {
            step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{
                        .path = zsdl.path("libs/macos/Frameworks/SDL2.framework").getPath(b),
                    },
                    .install_dir = install_dir,
                    .install_subdir = "SDL2.framework",
                }).step,
            );
        },
        else => {},
    }
}

pub fn install_sdl2_ttf(
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const zsdl = b.dependency("zsdl", .{});
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{ .path = zsdl.path("libs/x86_64-windows-gnu/bin/SDL2_ttf.dll").getPath(b) },
                        install_dir,
                        "SDL2_ttf.dll",
                    ).step,
                );
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{ .path = zsdl.path("libs/x86_64-linux-gnu/lib/libSDL2_ttf.so").getPath(b) },
                        install_dir,
                        "libSDL2_ttf.so",
                    ).step,
                );
            }
        },
        .macos => {
            step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{
                        .path = zsdl.path("libs/macos/Frameworks/SDL2_ttf.framework").getPath(b),
                    },
                    .install_dir = install_dir,
                    .install_subdir = "SDL2_ttf.framework",
                }).step,
            );
        },
        else => {},
    }
}

fn samples(b: *std.Build, options: Options) void {
    const minimal_glfw_gl = @import("samples/minimal_glfw_gl/build.zig");
    const minimal_sdl_gl = @import("samples/minimal_sdl_gl/build.zig");
    const minimal_zgui_glfw_gl = @import("samples/minimal_zgui_glfw_gl/build.zig");

    install(b, minimal_glfw_gl.build(b, options), "minimal_glfw_gl");
    install(b, minimal_sdl_gl.build(b, options), "minimal_sdl_gl");
    install(b, minimal_zgui_glfw_gl.build(b, options), "minimal_zgui_glfw_gl");

    if (@import("zgpu").checkTargetSupported(options.target.result)) {
        const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
        const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
        const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
        const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
        const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
        const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
        const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");
        const minimal_zgpu_zgui = @import("samples/minimal_zgpu_zgui/build.zig");
        const frame_pacing_wgpu = @import("samples/frame_pacing_wgpu/build.zig");
        const instanced_pills_wgpu = @import("samples/instanced_pills_wgpu/build.zig");
        const layers_wgpu = @import("samples/layers_wgpu/build.zig");
        const gamepad_wgpu = @import("samples/gamepad_wgpu/build.zig");
        const physics_test_wgpu = @import("samples/physics_test_wgpu/build.zig");
        const monolith = @import("samples/monolith/build.zig");

        install(b, triangle_wgpu.build(b, options), "triangle_wgpu");
        install(b, textured_quad_wgpu.build(b, options), "textured_quad_wgpu");
        install(b, gui_test_wgpu.build(b, options), "gui_test_wgpu");
        install(b, minimal_zgpu_zgui.build(b, options), "minimal_zgpu_zgui");
        install(b, frame_pacing_wgpu.build(b, options), "frame_pacing_wgpu");
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
}

fn samplesWindowsLinux(b: *std.Build, options: Options) void {
    const minimal_d3d12 = @import("samples/minimal_d3d12/build.zig");
    const minimal_glfw_d3d12 = @import("samples/minimal_glfw_d3d12/build.zig");
    const minimal_zgui_glfw_d3d12 = @import("samples/minimal_zgui_glfw_d3d12/build.zig");
    const textured_quad = @import("samples/textured_quad/build.zig");
    const triangle = @import("samples/triangle/build.zig");
    const mesh_shader_test = @import("samples/mesh_shader_test/build.zig");
    const rasterization = @import("samples/rasterization/build.zig");
    const bindless = @import("samples/bindless/build.zig");
    //const simple_raytracer = @import("samples/simple_raytracer/build.zig");

    install(b, minimal_d3d12.build(b, options), "minimal_d3d12");
    install(b, minimal_glfw_d3d12.build(b, options), "minimal_glfw_d3d12");
    install(b, minimal_zgui_glfw_d3d12.build(b, options), "minimal_zgui_glfw_d3d12");
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

fn tests(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    test_step: *std.Build.Step,
) void {
    const zaudio = b.dependency("zaudio", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zaudio.artifact("zaudio-tests")).step);

    // TODO: Get zbullet tests working on Windows again
    if (target.result.os.tag != .windows) {
        const zbullet = b.dependency("zbullet", .{
            .target = target,
            .optimize = optimize,
        });
        test_step.dependOn(&b.addRunArtifact(zbullet.artifact("zbullet-tests")).step);
    }

    const zflecs = b.dependency("zflecs", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zflecs.artifact("zflecs-tests")).step);

    const zglfw = b.dependency("zglfw", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zglfw.artifact("zglfw-tests")).step);

    const zgpu = b.dependency("zgpu", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zgpu.artifact("zgpu-tests")).step);

    const zgui = b.dependency("zgui", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zgui.artifact("zgui-tests")).step);

    const zmath = b.dependency("zmath", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zmath.artifact("zmath-tests")).step);

    const zmesh = b.dependency("zmesh", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zmesh.artifact("zmesh-tests")).step);

    const zopengl = b.dependency("zopengl", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zopengl.artifact("zopengl-tests")).step);

    const zphysics = b.dependency("zphysics", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zphysics.artifact("zphysics-tests")).step);

    const zpool = b.dependency("zpool", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zpool.artifact("zpool-tests")).step);

    // TODO(hazeycode): Fix tests linking SDL on macOS and Windows
    if (target.result.os.tag != .windows) {
        const zsdl = b.dependency("zsdl", .{
            .target = target,
            .optimize = optimize,
        });
        test_step.dependOn(&b.addRunArtifact(zsdl.artifact("sdl2-tests")).step);
        test_step.dependOn(&b.addRunArtifact(zsdl.artifact("sdl2_ttf-tests")).step);
        // TODO(hazeycode): Enable SDL3 tests
        // test_step.dependOn(&b.addRunArtifact(zsdl.artifact("sdl3-tests")).step);
    }

    const zjobs = b.dependency("zjobs", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zjobs.artifact("zjobs-tests")).step);

    const zstbi = b.dependency("zstbi", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zstbi.artifact("zstbi-tests")).step);

    const ztracy = b.dependency("ztracy", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(ztracy.artifact("ztracy-tests")).step);
}

fn testsWindows(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    test_step: *std.Build.Step,
) void {
    const zd3d12 = b.dependency("zd3d12", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zd3d12.artifact("zd3d12-tests")).step);

    const zpix = b.dependency("zpix", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zpix.artifact("zpix-tests")).step);

    const zwin32 = b.dependency("zwin32", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zwin32.artifact("zwin32-tests")).step);

    const zxaudio2 = b.dependency("zxaudio2", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zxaudio2.artifact("zxaudio2-tests")).step);
}

pub const Options = struct {
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,

    zd3d12_enable_debug_layer: bool,
    zd3d12_enable_gbv: bool,

    zpix_enable: bool,
};

fn install(b: *std.Build, exe: *std.Build.Step.Compile, comptime name: []const u8) void {
    // TODO: Problems with LTO on Windows.
    if (exe.rootModuleTarget().os.tag == .windows) {
        exe.want_lto = false;
    }

    if (exe.root_module.optimize == .ReleaseFast) {
        exe.root_module.strip = true;
    }

    const install_step = b.step(name, "Build '" ++ name ++ "' demo");
    install_step.dependOn(&b.addInstallArtifact(exe, .{}).step);

    const run_step = b.step(name ++ "-run", "Run '" ++ name ++ "' demo");
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    install_step.dependOn(b.getInstallStep());
}

fn ensureZigVersion() !void {
    var installed_ver = builtin.zig_version;
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

fn checkGitLfsContent() bool {
    const file = std.fs.openFileAbsolute(thisDir() ++ "/.lfs-content-token", .{}) catch {
        return false;
    };
    defer file.close();
    const expected_contents =
        \\DO NOT EDIT OR DELETE
        \\This file is used to check if Git LFS content has been downloaded
    ;
    var buf: [expected_contents.len]u8 = undefined;
    _ = file.readAll(&buf) catch {
        return false;
    };
    return std.mem.eql(u8, expected_contents, &buf);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
