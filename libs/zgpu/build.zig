const std = @import("std");
const log = std.log.scoped(.zgpu);

const zglfw = @import("zglfw");
const zpool = @import("zpool");

const dawn = @import("build-dawn.zig");

const DefaultOptions = struct {
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
    uniforms_buffer_size: u64 = DefaultOptions.uniforms_buffer_size,
    dawn_skip_validation: bool = DefaultOptions.dawn_skip_validation,
    buffer_pool_size: u32 = DefaultOptions.buffer_pool_size,
    texture_pool_size: u32 = DefaultOptions.texture_pool_size,
    texture_view_pool_size: u32 = DefaultOptions.texture_view_pool_size,
    sampler_pool_size: u32 = DefaultOptions.sampler_pool_size,
    render_pipeline_pool_size: u32 = DefaultOptions.render_pipeline_pool_size,
    compute_pipeline_pool_size: u32 = DefaultOptions.compute_pipeline_pool_size,
    bind_group_pool_size: u32 = DefaultOptions.bind_group_pool_size,
    bind_group_layout_pool_size: u32 = DefaultOptions.bind_group_layout_pool_size,
    pipeline_layout_pool_size: u32 = DefaultOptions.pipeline_layout_pool_size,
};

pub const Package = struct {
    target: std.Build.ResolvedTarget,
    options: Options,
    zgpu: *std.Build.Module,
    zgpu_options: *std.Build.Module,
    dawn_lib: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zgpu", pkg.zgpu);
        exe.root_module.addImport("zgpu_options", pkg.zgpu_options);

        const b = exe.step.owner;

        const system_sdk = b.dependency("system_sdk", .{});

        switch (pkg.target.result.os.tag) {
            .windows => {
                exe.addLibraryPath(.{ .path = system_sdk.path("windows/lib/x86_64-windows-gnu").getPath(b) });

                exe.linkSystemLibrary("ole32");
                exe.linkSystemLibrary("dxguid");
            },
            .linux => {},
            .macos => {
                exe.addFrameworkPath(.{ .path = system_sdk.path("macos12/System/Library/Frameworks").getPath(b) });
                exe.addSystemIncludePath(.{ .path = system_sdk.path("macos12/usr/include").getPath(b) });
                exe.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });

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

        exe.linkLibrary(pkg.dawn_lib);

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
        dawn_lib_options: dawn.Options = .{},
        deps: struct {
            zglfw: zglfw.Package,
            zpool: zpool.Package,
        },
    },
) Package {
    const options_step = b.addOptions();
    inline for (std.meta.fields(Options)) |option_field| {
        const option_val = @field(args.options, option_field.name);
        options_step.addOption(@TypeOf(option_val), option_field.name, option_val);
    }

    const zgpu_options = options_step.createModule();

    const zgpu = b.addModule("zgpu", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .imports = &.{
            .{ .name = "zgpu_options", .module = zgpu_options },
            .{ .name = "zglfw", .module = args.deps.zglfw.zglfw },
            .{ .name = "zpool", .module = args.deps.zpool.zpool },
        },
    });

    const dawn_lib = dawn.buildStaticLibrary(b, target, args.dawn_lib_options) catch
        @panic("Failed to build dawn lib");

    return .{
        .target = target,
        .options = args.options,
        .zgpu = zgpu,
        .zgpu_options = zgpu_options,
        .dawn_lib = dawn_lib,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zglfw_pkg = zglfw.package(b, target, optimize, .{});
    const zpool_pkg = zpool.package(b, target, optimize, .{});

    _ = package(b, target, optimize, .{
        .options = .{
            .uniforms_buffer_size = b.option(
                u64,
                "uniforms_buffer_size",
                "Set uniforms buffer size",
            ) orelse DefaultOptions.uniforms_buffer_size,
            .dawn_skip_validation = b.option(
                bool,
                "dawn_skip_validation",
                "Disable Dawn validation",
            ) orelse DefaultOptions.dawn_skip_validation,
            .buffer_pool_size = b.option(
                u32,
                "buffer_pool_size",
                "Set buffer pool size",
            ) orelse DefaultOptions.buffer_pool_size,
            .texture_pool_size = b.option(
                u32,
                "texture_pool_size",
                "Set texture pool size",
            ) orelse DefaultOptions.texture_pool_size,
            .texture_view_pool_size = b.option(
                u32,
                "texture_view_pool_size",
                "Set texture view pool size",
            ) orelse DefaultOptions.texture_view_pool_size,
            .sampler_pool_size = b.option(
                u32,
                "sampler_pool_size",
                "Set sample pool size",
            ) orelse DefaultOptions.sampler_pool_size,
            .render_pipeline_pool_size = b.option(
                u32,
                "render_pipeline_pool_size",
                "Set render pipeline pool size",
            ) orelse DefaultOptions.render_pipeline_pool_size,
            .compute_pipeline_pool_size = b.option(
                u32,
                "compute_pipeline_pool_size",
                "Set compute pipeline pool size",
            ) orelse DefaultOptions.compute_pipeline_pool_size,
            .bind_group_pool_size = b.option(
                u32,
                "bind_group_pool_size",
                "Set bind group pool size",
            ) orelse DefaultOptions.bind_group_pool_size,
            .bind_group_layout_pool_size = b.option(
                u32,
                "bind_group_layout_pool_size",
                "Set bind group layout pool size",
            ) orelse DefaultOptions.bind_group_layout_pool_size,
            .pipeline_layout_pool_size = b.option(
                u32,
                "pipeline_layout_pool_size",
                "Set pipeline layout pool size",
            ) orelse DefaultOptions.pipeline_layout_pool_size,
        },
        .dawn_lib_options = .{
            .optimize = b.option(
                std.builtin.Mode,
                "dawn_lib_optimize",
                "Specifiy build mode for Dawn lib",
            ) orelse dawn.DefaultOptions.optimize,
            .disable_logging = b.option(
                bool,
                "dawn_lib_disable_options",
                "Whether to build Dawn lib with logging disabled",
            ) orelse dawn.DefaultOptions.disable_logging,
        },
        .deps = .{
            .zglfw = zglfw_pkg,
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
