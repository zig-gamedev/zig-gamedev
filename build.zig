const builtin = @import("builtin");
const std = @import("std");

pub fn build(b: *std.Build) void {
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
        .zxaudio2_debug_layer = b.option(
            bool,
            "zxaudio2_debug_layer",
            "Enable XAudio2 debug layer",
        ) orelse false,
        .zd3d12_debug_layer = b.option(
            bool,
            "zd3d12_debug_layer",
            "Enable DirectX 12 debug layer",
        ) orelse false,
        .zd3d12_gbv = b.option(
            bool,
            "zd3d12_gbv",
            "Enable DirectX 12 GPU-Based Validation (GBV)",
        ) orelse false,
        .zpix_enable = zpix_enable,
        .zpix_path = b.option(
            []const u8,
            "zpix-path",
            "Installed PIX path",
        ) orelse if (zpix_enable) @panic("PIX path is required when enabled") else "",
    };

    if (target.result.os.tag == .windows) {
        if (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux) {
            const activate_zwindows = @import("zwindows").activateSdk(b, b.dependency("zwindows", .{}));
            b.default_step.dependOn(activate_zwindows);

            samples.build(b, options, activate_zwindows, samples.windows_linux_cross);

            if (builtin.target.os.tag == .windows) {
                // TODO: Try to upgrade these to windows_linux_cross
                samples.build(b, options, activate_zwindows, samples.windows_only);
            }
        } else @panic("Unsupported host OS for Windows target");
    }

    if (target.result.os.tag == .emscripten) {
        const activate_emsdk = @import("zemscripten").activateEmsdkStep(b);
        b.default_step.dependOn(activate_emsdk);

        const web_options = .{
            .optimize = optimize,
            .target = target,
        };
        samples.buildWeb(b, web_options, activate_emsdk);
    } else {
        samples.build(b, options, null, samples.crossplatform);
    }

    // Install prebuilt SDL2 libs in bin output dir
    if (@import("zsdl").prebuilt_sdl2.install(b, options.target.result, .bin, .{
        .ttf = true,
        .image = true,
    })) |install_sdl2_step| {
        b.getInstallStep().dependOn(install_sdl2_step);
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

pub const samples = struct {
    /// Windows sample apps that can only be build using Windows. Try to move these to `windows_linux_cross`...
    pub const windows_only = struct {
        pub const audio_experiments = @import("samples/audio_experiments/build.zig");
        pub const audio_playback_test = @import("samples/audio_playback_test/build.zig");
        pub const directml_convolution_test = @import("samples/directml_convolution_test/build.zig");
        pub const vector_graphics_test = @import("samples/vector_graphics_test/build.zig");
    };

    /// Windows sample apps that can be cross-compiled from Linux.
    pub const windows_linux_cross = struct {
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
        pub const zphysics_instanced_cubes_d3d12 = @import("samples/zphysics_instanced_cubes_d3d12/build.zig");
    };

    /// Sample apps that can build and run on Windows, macOS and Linux. Cross-compilation should also work.
    pub const crossplatform = struct {
        pub const sdl2_demo = @import("samples/sdl2_demo/build.zig");

        // OpenGL samples
        pub const minimal_glfw_gl = @import("samples/minimal_glfw_gl/build.zig");
        pub const minimal_sdl_gl = @import("samples/minimal_sdl_gl/build.zig");
        pub const minimal_zgui_glfw_gl = @import("samples/minimal_zgui_glfw_gl/build.zig");

        // WebGPU samples
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

    /// Sample apps that can be built as web applications using zemscripten.
    pub const web = struct {
        pub const sdl2_demo = samples.crossplatform.sdl2_demo;

        // TODO: WebGL samples
        // pub const minimal_glfw_gl = samples.crossplatform.minimal_glfw_gl;
        // pub const minimal_sdl_gl = samples.crossplatform.minimal_sdl_gl;
        // pub const minimal_zgui_glfw_gl = samples.crossplatform.minimal_zgui_glfw_gl;

        // TODO: WebGPU samples
        // pub const audio_experiments_wgpu = samples.crossplatform.audio_experiments_wgpu;
        // pub const bullet_physics_test_wgpu = samples.crossplatform.bullet_physics_test_wgpu;
        // pub const gamepad_wgpu = samples.crossplatform.gamepad_wgpu;
        // pub const gui_test_wgpu = samples.crossplatform.gui_test_wgpu;
        // pub const instanced_pills_wgpu = samples.crossplatform.instanced_pills_wgpu;
        // pub const layers_wgpu = samples.crossplatform.layers_wgpu;
        // pub const minimal_zgpu_zgui = samples.crossplatform.minimal_zgpu_zgui;
        // pub const monolith = samples.crossplatform.monolith;
        // pub const physically_based_rendering_wgpu = samples.crossplatform.physically_based_rendering_wgpu;
        // pub const physics_test_wgpu = samples.crossplatform.physics_test_wgpu;
        // pub const procedural_mesh_wgpu = samples.crossplatform.procedural_mesh_wgpu;
        // pub const textured_quad_wgpu = samples.crossplatform.textured_quad_wgpu;
        // pub const triangle_wgpu = samples.crossplatform.triangle_wgpu;
    };

    fn build(
        b: *std.Build,
        options: anytype,
        maybe_depend_step: ?*std.Build.Step,
        comptime apps: anytype,
    ) void {
        inline for (comptime std.meta.declarations(apps)) |d| {
            const exe = buildExe(b, options, @field(apps, d.name));
            if (maybe_depend_step) |step| {
                exe.step.dependOn(step);
            }
        }
    }

    fn buildWeb(
        b: *std.Build,
        options: anytype,
        maybe_depend_step: ?*std.Build.Step,
    ) void {
        inline for (comptime std.meta.declarations(samples.web)) |d| {
            const build_web_app_step = @field(samples.web, d.name).buildWeb(b, options);

            if (maybe_depend_step) |step| {
                build_web_app_step.dependOn(step);
            }

            b.getInstallStep().dependOn(build_web_app_step);

            const html_filename = std.fmt.allocPrint(
                b.allocator,
                "{s}.html",
                .{d.name},
            ) catch unreachable;

            const emrun_step = @import("zemscripten").emrunStep(
                b,
                b.getInstallPath(.{ .custom = "web" }, html_filename),
                &.{},
            );
            emrun_step.dependOn(build_web_app_step);

            b.step(
                d.name,
                "Build '" ++ d.name ++ "' sample as a web app",
            ).dependOn(build_web_app_step);

            b.step(
                d.name ++ "-emrun",
                "Build '" ++ d.name ++ "' sample as a web app and serve locally using `emrun`",
            ).dependOn(emrun_step);
        }
    }
};

fn buildExe(b: *std.Build, options: anytype, sample: anytype) *std.Build.Step.Compile {
    const exe = sample.build(b, options);

    if (exe.rootModuleTarget().os.tag == .windows) {
        // TODO: Problems with LTO on Windows.
        exe.want_lto = false;
    }

    if (exe.root_module.optimize != .Debug) {
        exe.root_module.strip = true;
    }

    const install_exe = b.addInstallArtifact(exe, .{});
    b.getInstallStep().dependOn(&install_exe.step);
    b.step(sample.demo_name, "Build '" ++ sample.demo_name ++ "' demo").dependOn(&install_exe.step);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(&install_exe.step);
    b.step(sample.demo_name ++ "-run", "Run '" ++ sample.demo_name ++ "' demo").dependOn(&run_cmd.step);

    return exe;
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
