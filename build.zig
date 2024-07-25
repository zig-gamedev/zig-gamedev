const builtin = @import("builtin");
const std = @import("std");

pub const min_zig_version = std.SemanticVersion{
    .major = 0,
    .minor = 13,
    .patch = 0,
    .pre = "dev.351",
};

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

    const zpix_enable = b.option(
        bool,
        "zpix-enable",
        "Enable PIX for Windows profiler",
    ) orelse false;
    const options = .{
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
        .zpix_enable = zpix_enable,
        .zpix_path = b.option(
            []const u8,
            "zpix-path",
            "Installed PIX path",
        ) orelse if (zpix_enable) @panic("PIX path is required when enabled") else "",
    };

    if (target.result.os.tag == .emscripten) {
        // If user did not set --sysroot then default to zemscripten's emsdk path
        if (b.sysroot == null) {
            b.sysroot = b.dependency("emsdk", .{}).path("upstream/emscripten/cache/sysroot").getPath(b);
            std.log.info("sysroot set to \"{s}\"", .{b.sysroot.?});
        }
        buildAndInstallSamplesWeb(b, .{
            .optimize = optimize,
            .target = target,
        });
    } else {
        buildAndInstallSamples(b, options, samples_cross_platform);
        if (target.result.os.tag == .windows) {
            if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
                buildAndInstallSamples(b, options, samples_windows_linux);
                if (builtin.os.tag == .windows) {
                    buildAndInstallSamples(b, options, samples_windows);
                }
            }
        }

        { // Tests
            const test_step = b.step("test", "Run all tests");
            tests(b, target, optimize, test_step);
            if (builtin.os.tag == .windows) {
                testsWindows(b, target, optimize, test_step);
            }
        }

        { // Benchmarks
            const benchmark_step = b.step("benchmark", "Run all benchmarks");
            const zmath = b.dependency("zmath", .{
                .optimize = .ReleaseFast,
            });
            benchmark_step.dependOn(&b.addRunArtifact(zmath.artifact("zmath-benchmarks")).step);
        }

        // Experiments
        if (b.option(bool, "experiments", "Build our prototypes and experimental programs") orelse false) {
            @import("experiments/build.zig").build(b, options);
        }
    }
}

const samples_windows = struct {
    pub const audio_experiments = @import("samples/audio_experiments/build.zig");
    pub const audio_playback_test = @import("samples/audio_playback_test/build.zig");
    pub const directml_convolution_test = @import("samples/directml_convolution_test/build.zig");
    pub const vector_graphics_test = @import("samples/vector_graphics_test/build.zig");
};

const samples_windows_linux = struct {
    pub const bindless = @import("samples/bindless/build.zig");
    pub const mesh_shader_test = @import("samples/mesh_shader_test/build.zig");
    pub const minimal_d3d12 = @import("samples/minimal_d3d12/build.zig");
    pub const minimal_glfw_d3d12 = @import("samples/minimal_glfw_d3d12/build.zig");
    pub const minimal_zgui_glfw_d3d12 = @import("samples/minimal_zgui_glfw_d3d12/build.zig");
    pub const minimal_zgui_win32_d3d12 = @import("samples/minimal_zgui_win32_d3d12/build.zig");
    pub const openvr_test = @import("samples/openvr_test/build.zig");
    pub const simple_openvr = @import("samples/simple_openvr/build.zig");
    pub const rasterization = @import("samples/rasterization/build.zig");
    // TODO: get simple raytracer working again
    //pub const simple_raytracer = @import("samples/simple_raytracer/build.zig");
    pub const textured_quad = @import("samples/textured_quad/build.zig");
    pub const triangle = @import("samples/triangle/build.zig");
};

const samples_cross_platform = struct {
    pub usingnamespace struct { // OpenGL samples
        pub const minimal_glfw_gl = @import("samples/minimal_glfw_gl/build.zig");
        pub const minimal_sdl_gl = @import("samples/minimal_sdl_gl/build.zig");
        pub const minimal_zgui_glfw_gl = @import("samples/minimal_zgui_glfw_gl/build.zig");
    };
    pub usingnamespace struct { // WebGPU samples
        pub const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
        pub const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
        pub const frame_pacing_wgpu = @import("samples/frame_pacing_wgpu/build.zig");
        pub const gamepad_wgpu = @import("samples/gamepad_wgpu/build.zig");
        pub const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");
        pub const instanced_pills_wgpu = @import("samples/instanced_pills_wgpu/build.zig");
        pub const layers_wgpu = @import("samples/layers_wgpu/build.zig");
        pub const minimal_zgpu_zgui = @import("samples/minimal_zgpu_zgui/build.zig");
        pub const monolith = @import("samples/monolith/build.zig");
        pub const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
        pub const physics_test_wgpu = @import("samples/physics_test_wgpu/build.zig");
        pub const procedural_mesh_wgpu = @import("samples/procedural_mesh_wgpu/build.zig");
        pub const textured_quad_wgpu = @import("samples/textured_quad_wgpu/build.zig");
        pub const triangle_wgpu = @import("samples/triangle_wgpu/build.zig");
    };
};

const samples_web = struct {
    pub usingnamespace struct { // WebGL samples
        // TODO
        // pub const minimal_glfw_gl = samples_cross_platform.minimal_glfw_gl;
        // pub const minimal_sdl_gl = samples_cross_platform.minimal_sdl_gl;
        // pub const minimal_zgui_glfw_gl = samples_cross_platform.minimal_zgui_glfw_gl;
    };

    pub usingnamespace struct { // WebGPU samples
        // TODO
        // pub const audio_experiments_wgpu = samples_cross_platform.audio_experiments_wgpu;
        // pub const bullet_physics_test_wgpu = samples_cross_platform.bullet_physics_test_wgpu;
        // pub const gamepad_wgpu = samples_cross_platform.gamepad_wgpu;
        // pub const gui_test_wgpu = samples_cross_platform.gui_test_wgpu;
        // pub const instanced_pills_wgpu = samples_cross_platform.instanced_pills_wgpu;
        // pub const layers_wgpu = samples_cross_platform.layers_wgpu;
        // pub const minimal_zgpu_zgui = samples_cross_platform.minimal_zgpu_zgui;
        // pub const monolith = samples_cross_platform.monolith;
        // pub const physically_based_rendering_wgpu = samples_cross_platform.physically_based_rendering_wgpu;
        // pub const physics_test_wgpu = samples_cross_platform.physics_test_wgpu;
        // pub const procedural_mesh_wgpu = samples_cross_platform.procedural_mesh_wgpu;
        // pub const textured_quad_wgpu = samples_cross_platform.textured_quad_wgpu;
        // pub const triangle_wgpu = samples_cross_platform.triangle_wgpu;
    };
};

fn buildAndInstallSamples(b: *std.Build, options: anytype, comptime samples: anytype) void {
    inline for (comptime std.meta.declarations(samples)) |d| {
        const exe = @field(samples, d.name).build(b, options);

        // TODO: Problems with LTO on Windows.
        if (exe.rootModuleTarget().os.tag == .windows) {
            exe.want_lto = false;
        }

        if (exe.root_module.optimize != .Debug) {
            exe.root_module.strip = true;
        }

        const install_exe = b.addInstallArtifact(exe, .{});
        b.getInstallStep().dependOn(&install_exe.step);
        b.step(d.name, "Build '" ++ d.name ++ "' demo").dependOn(&install_exe.step);

        const run_cmd = b.addRunArtifact(exe);
        run_cmd.step.dependOn(&install_exe.step);
        b.step(d.name ++ "-run", "Run '" ++ d.name ++ "' demo").dependOn(&run_cmd.step);
    }
}

fn buildAndInstallSamplesWeb(b: *std.Build, options: anytype) void {
    inline for (comptime std.meta.declarations(samples_web)) |d| {
        const web_install_step = @field(samples_web, d.name).buildWeb(b, options);

        b.step(d.name, "Build '" ++ d.name ++ "' demo").dependOn(web_install_step);

        const html_filename = std.fmt.allocPrint(b.allocator, "{s}.html", .{d.name}) catch unreachable;
        const emrun_step = @import("zemscripten").emrunStep(b, b.getInstallPath(.{ .custom = "web" }, html_filename));
        emrun_step.dependOn(web_install_step);

        const run_step = b.step("run", "Serve and run the web app locally");
        run_step.dependOn(emrun_step);
    }
}

fn tests(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    test_step: *std.Build.Step,
) void {
    // TODO: Renable randomly failing zaudio tests on windows
    if (target.result.os.tag != .windows) {
        const zaudio = b.dependency("zaudio", .{
            .target = target,
            .optimize = optimize,
        });
        test_step.dependOn(&b.addRunArtifact(zaudio.artifact("zaudio-tests")).step);
    }

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

    const zgpu = b.dependency("zgpu", .{
        .target = target,
        .optimize = optimize,
    });
    test_step.dependOn(&b.addRunArtifact(zgpu.artifact("zgpu-tests")).step);

    const zgui = b.dependency("zgui", .{
        .target = target,
        .optimize = optimize,
        .with_te = true,
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

    const zopenvr = b.dependency("zopenvr", .{
        .target = target,
        .optimize = optimize,
    });
    const openvr_tests = b.addRunArtifact(zopenvr.artifact("openvr-tests"));
    openvr_tests.setCwd(.{ .cwd_relative = b.getInstallPath(.bin, "") });

    test_step.dependOn(&openvr_tests.step);
    @import("zopenvr").installOpenVR(&openvr_tests.step, target.result, .bin);
}

// TODO: Delete this once Zig checks minimum_zig_version in build.zig.zon
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
    const result = std.process.Child.run(.{
        .allocator = allocator,
        .argv = argv,
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
    const result = std.process.Child.run(.{
        .allocator = allocator,
        .argv = argv,
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
    const expected_contents =
        \\DO NOT EDIT OR DELETE
        \\This file is used to check if Git LFS content has been downloaded
    ;
    var buf: [expected_contents.len]u8 = undefined;
    _ = std.fs.cwd().readFile(".lfs-content-token", &buf) catch {
        return false;
    };
    return std.mem.eql(u8, expected_contents, &buf);
}
