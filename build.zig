const builtin = @import("builtin");
const std = @import("std");

pub const min_zig_version = std.SemanticVersion{ .major = 0, .minor = 12, .patch = 0, .pre = "dev.126" };

pub fn build(b: *std.Build) void {
    //
    // Environment checks
    //
    ensureZigVersion() catch return;
    ensureGit(b.allocator) catch return;
    ensureGitLfs(b.allocator, "install") catch return;
    ensureGitLfs(b.allocator, "pull") catch return;
    ensureGitLfsContent("/samples/triangle_wgpu/triangle_wgpu_content/Roboto-Medium.ttf") catch return;

    //
    // Build options
    //
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    ensureTarget(target) catch return;

    const zd3d12_enable_debug_layer = b.option(
        bool,
        "zd3d12-enable-debug-layer",
        "Enable DirectX 12 debug layer",
    ) orelse false;

    const zd3d12_enable_gbv = b.option(
        bool,
        "zd3d12-enable-gbv",
        "Enable DirectX 12 GPU-Based Validation (GBV)",
    ) orelse false;

    const zpix_enable = b.option(
        bool,
        "zpix-enable",
        "Enable PIX for Windows profiler",
    ) orelse false;

    //
    // Sample applications
    //
    @import("samples/build.zig").buildWithOptions(b, .{
        .optimize = optimize,
        .target = target,
        .zd3d12_enable_debug_layer = zd3d12_enable_debug_layer,
        .zd3d12_enable_gbv = zd3d12_enable_gbv,
        .zpix_enable = zpix_enable,
    });

    //
    // Tests
    //
    {
        const test_step = b.step("test", "Run all tests");
        test_step.dependOn(zmath.runTests(b, optimize, target));
        test_step.dependOn(zmesh.runTests(b, optimize, target));
        test_step.dependOn(zstbi.runTests(b, optimize, target));
        test_step.dependOn(znoise.runTests(b, optimize, target));
        test_step.dependOn(zglfw.runTests(b, optimize, target));
        test_step.dependOn(zpool.runTests(b, optimize, target));
        test_step.dependOn(zjobs.runTests(b, optimize, target));
        test_step.dependOn(zaudio.runTests(b, optimize, target));
        test_step.dependOn(zflecs.runTests(b, optimize, target));
        test_step.dependOn(zphysics.runTests(b, optimize, target));
        // TODO: zsdl test not included in top-level tests until https://github.com/michal-z/zig-gamedev/issues/312 is resolved
        //test_step.dependOn(zsdl.runTests(b, optimize, target));
    }

    //
    // Benchmarks
    //
    {
        const benchmark_step = b.step("benchmark", "Run all benchmarks");
        benchmark_step.dependOn(zmath.runBenchmarks(b, target));
    }

    //
    // Experiments
    //
    if (b.option(
        bool,
        "experiments",
        "Build our prototypes and experimental programs",
    ) orelse false) {
        @import("experiments/build.zig").buildWithOptions(b, .{
            .target = target,
            .optimize = optimize,
        });
    }
}

const zsdl = @import("libs/zsdl/build.zig");
const zopengl = @import("libs/zopengl/build.zig");
const zmath = @import("libs/zmath/build.zig");
const zglfw = @import("libs/zglfw/build.zig");
const zpool = @import("libs/zpool/build.zig");
const zjobs = @import("libs/zjobs/build.zig");
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

fn ensureTarget(cross: std.zig.CrossTarget) !void {
    const target = (std.zig.system.NativeTargetInfo.detect(cross) catch unreachable).target;

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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
