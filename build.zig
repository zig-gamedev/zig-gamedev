const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    ensureGitLfs(b.allocator);
    {
        var child = std.ChildProcess.init(&.{ "git", "submodule", "update", "--init", "--remote" }, b.allocator);
        child.cwd = thisDir();
        child.stderr = std.io.getStdErr();
        child.stdout = std.io.getStdOut();
        _ = child.spawnAndWait() catch unreachable;
    }

    var options = Options{
        .build_mode = b.standardReleaseOptions(),
        .target = b.standardTargetOptions(.{}),
    };

    options.ztracy_enable = b.option(bool, "ztracy-enable", "Enable Tracy profiler") orelse false;

    //
    // Sample application
    //
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

    //
    // Tests
    //
    const test_step = b.step("test", "Run all tests");

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

    const zjolt_tests = @import("libs/zjolt/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zjolt_tests.step);

    const zglfw_tests = @import("libs/zglfw/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&zglfw_tests.step);

    const znetwork_tests = @import("libs/znetwork/build.zig").buildTests(b, options.build_mode, options.target);
    test_step.dependOn(&znetwork_tests.step);

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
const physically_based_rendering_wgpu = @import("samples/physically_based_rendering_wgpu/build.zig");
const bullet_physics_test_wgpu = @import("samples/bullet_physics_test_wgpu/build.zig");
const audio_experiments_wgpu = @import("samples/audio_experiments_wgpu/build.zig");
const gui_test_wgpu = @import("samples/gui_test_wgpu/build.zig");

pub const Options = struct {
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,

    ztracy_enable: bool = false,
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
