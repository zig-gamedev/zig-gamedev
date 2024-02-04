const std = @import("std");
const log = std.log.scoped(.zgpu);

const zpool = @import("zpool");

const default_options = struct {
    const uniforms_buffer_size = 4 * 1024 * 1024;
    const dawn_skip_validation = false;
    const buffer_pool_size = 256;
    const texture_pool_size = 256;
    const texture_view_pool_size = 256;
    const sampler_pool_size = 16;
    const render_pipeline_pool_size = 128;
    const compute_pipeline_pool_size = 128;
    const bind_group_pool_size = 32;
    const bind_group_layout_pool_size = 32;
    const pipeline_layout_pool_size = 32;
};

pub const Options = struct {
    uniforms_buffer_size: u64 = default_options.uniforms_buffer_size,
    dawn_skip_validation: bool = default_options.dawn_skip_validation,
    buffer_pool_size: u32 = default_options.buffer_pool_size,
    texture_pool_size: u32 = default_options.texture_pool_size,
    texture_view_pool_size: u32 = default_options.texture_view_pool_size,
    sampler_pool_size: u32 = default_options.sampler_pool_size,
    render_pipeline_pool_size: u32 = default_options.render_pipeline_pool_size,
    compute_pipeline_pool_size: u32 = default_options.compute_pipeline_pool_size,
    bind_group_pool_size: u32 = default_options.bind_group_pool_size,
    bind_group_layout_pool_size: u32 = default_options.bind_group_layout_pool_size,
    pipeline_layout_pool_size: u32 = default_options.pipeline_layout_pool_size,
};

pub const Package = struct {
    target: std.Build.ResolvedTarget,
    options: Options,
    zgpu: *std.Build.Module,
    zgpu_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        if (!checkTargetSupported(pkg.target)) {
            @panic("Unsupported target");
        }

        exe.root_module.addImport("zgpu", pkg.zgpu);
        exe.root_module.addImport("zgpu_options", pkg.zgpu_options);

        const b = exe.step.owner;

        const system_sdk = b.dependency("system_sdk", .{});

        switch (pkg.target.result.os.tag) {
            .windows => {
                const dawn_dep = b.dependency("dawn_x86_64_windows_gnu", .{});
                exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                exe.addLibraryPath(.{ .path = system_sdk.path("windows/lib/x86_64-windows-gnu").getPath(b) });

                exe.linkSystemLibrary("ole32");
                exe.linkSystemLibrary("dxguid");
            },
            .linux => {
                if (pkg.target.result.cpu.arch.isX86()) {
                    const dawn_dep = b.dependency("dawn_x86_64_linux_gnu", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                } else {
                    const dawn_dep = b.dependency("dawn_aarch64_linux_gnu", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                }
            },
            .macos => {
                exe.addFrameworkPath(.{ .path = system_sdk.path("macos12/System/Library/Frameworks").getPath(b) });
                exe.addSystemIncludePath(.{ .path = system_sdk.path("macos12/usr/include").getPath(b) });
                exe.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });

                if (pkg.target.result.cpu.arch.isX86()) {
                    const dawn_dep = b.dependency("dawn_x86_64_macos", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                } else {
                    const dawn_dep = b.dependency("dawn_aarch64_macos", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                }

                exe.linkSystemLibrary("objc");
                exe.linkFramework("Metal");
                exe.linkFramework("CoreGraphics");
                exe.linkFramework("Foundation");
                exe.linkFramework("IOKit");
                exe.linkFramework("IOSurface");
                exe.linkFramework("QuartzCore");
            },
            else => {},
        }

        exe.linkSystemLibrary("dawn");
        exe.linkLibC();
        exe.linkLibCpp();

        exe.addIncludePath(.{ .path = thisDir() ++ "/libs/dawn/include" });
        exe.addIncludePath(.{ .path = thisDir() ++ "/src" });

        exe.addCSourceFile(.{
            .file = .{ .path = thisDir() ++ "/src/dawn.cpp" },
            .flags = &.{ "-std=c++17", "-fno-sanitize=undefined" },
        });
        exe.addCSourceFile(.{
            .file = .{ .path = thisDir() ++ "/src/dawn_proc.c" },
            .flags = &.{"-fno-sanitize=undefined"},
        });
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
        deps: struct {
            zpool: zpool.Package,
        },
    },
) Package {
    const step = b.addOptions();
    step.addOption(u64, "uniforms_buffer_size", args.options.uniforms_buffer_size);
    step.addOption(bool, "dawn_skip_validation", args.options.dawn_skip_validation);
    step.addOption(u32, "buffer_pool_size", args.options.buffer_pool_size);
    step.addOption(u32, "texture_pool_size", args.options.texture_pool_size);
    step.addOption(u32, "texture_view_pool_size", args.options.texture_view_pool_size);
    step.addOption(u32, "sampler_pool_size", args.options.sampler_pool_size);
    step.addOption(u32, "render_pipeline_pool_size", args.options.render_pipeline_pool_size);
    step.addOption(u32, "compute_pipeline_pool_size", args.options.compute_pipeline_pool_size);
    step.addOption(u32, "bind_group_pool_size", args.options.bind_group_pool_size);
    step.addOption(u32, "bind_group_layout_pool_size", args.options.bind_group_layout_pool_size);
    step.addOption(u32, "pipeline_layout_pool_size", args.options.pipeline_layout_pool_size);

    const zgpu_options = step.createModule();

    const zgpu = b.addModule("zgpu", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .imports = &.{
            .{ .name = "zgpu_options", .module = zgpu_options },
            .{ .name = "zpool", .module = args.deps.zpool.zpool },
        },
    });

    return .{
        .target = target,
        .options = args.options,
        .zgpu = zgpu,
        .zgpu_options = zgpu_options,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zpool_pkg = zpool.package(b, target, optimize, .{});

    _ = package(b, target, optimize, .{
        .options = .{
            .uniforms_buffer_size = b.option(
                u64,
                "uniforms_buffer_size",
                "Set uniforms buffer size",
            ) orelse default_options.uniforms_buffer_size,
            .dawn_skip_validation = b.option(
                bool,
                "dawn_skip_validation",
                "Disable Dawn validation",
            ) orelse default_options.dawn_skip_validation,
            .buffer_pool_size = b.option(
                u32,
                "buffer_pool_size",
                "Set buffer pool size",
            ) orelse default_options.buffer_pool_size,
            .texture_pool_size = b.option(
                u32,
                "texture_pool_size",
                "Set texture pool size",
            ) orelse default_options.texture_pool_size,
            .texture_view_pool_size = b.option(
                u32,
                "texture_view_pool_size",
                "Set texture view pool size",
            ) orelse default_options.texture_view_pool_size,
            .sampler_pool_size = b.option(
                u32,
                "sampler_pool_size",
                "Set sample pool size",
            ) orelse default_options.sampler_pool_size,
            .render_pipeline_pool_size = b.option(
                u32,
                "render_pipeline_pool_size",
                "Set render pipeline pool size",
            ) orelse default_options.render_pipeline_pool_size,
            .compute_pipeline_pool_size = b.option(
                u32,
                "compute_pipeline_pool_size",
                "Set compute pipeline pool size",
            ) orelse default_options.compute_pipeline_pool_size,
            .bind_group_pool_size = b.option(
                u32,
                "bind_group_pool_size",
                "Set bind group pool size",
            ) orelse default_options.bind_group_pool_size,
            .bind_group_layout_pool_size = b.option(
                u32,
                "bind_group_layout_pool_size",
                "Set bind group layout pool size",
            ) orelse default_options.bind_group_layout_pool_size,
            .pipeline_layout_pool_size = b.option(
                u32,
                "pipeline_layout_pool_size",
                "Set pipeline layout pool size",
            ) orelse default_options.pipeline_layout_pool_size,
        },
        .deps = .{
            .zpool = zpool_pkg,
        },
    });

    const test_step = b.step("test", "Run zgpu tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zgpu-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.addIncludePath(.{ .path = thisDir() ++ "/libs/dawn/include" });
    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

pub fn checkTargetSupported(target: std.Build.ResolvedTarget) bool {
    const supported = switch (target.result.os.tag) {
        .windows => target.result.cpu.arch.isX86() and target.result.abi.isGnu(),
        .linux => (target.result.cpu.arch.isX86() or target.result.cpu.arch.isAARCH64()) and target.result.abi.isGnu(),
        .macos => blk: {
            if (!target.result.cpu.arch.isX86() and !target.result.cpu.arch.isAARCH64()) break :blk false;

            // If min. target macOS version is lesser than the min version we have available, then
            // our Dawn binary is incompatible with the target.
            if (target.result.os.version_range.semver.min.order(
                .{ .major = 12, .minor = 0, .patch = 0 },
            ) == .lt) break :blk false;
            break :blk true;
        },
        else => false,
    };
    if (supported == false) {
        log.warn("\n" ++
            \\---------------------------------------------------------------------------
            \\
            \\Dawn/WebGPU binary for this target is not available.
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
    }
    return supported;
}
