const std = @import("std");

pub const Options = struct {
    uniforms_buffer_size: u64 = 4 * 1024 * 1024,
    dawn_skip_validation: bool = false,
    buffer_pool_size: u32 = 256,
    texture_pool_size: u32 = 256,
    texture_view_pool_size: u32 = 256,
    sampler_pool_size: u32 = 16,
    render_pipeline_pool_size: u32 = 128,
    compute_pipeline_pool_size: u32 = 128,
    bind_group_pool_size: u32 = 32,
    bind_group_layout_pool_size: u32 = 32,
    pipeline_layout_pool_size: u32 = 32,
};

pub const Package = struct {
    options: Options,
    zgpu: *std.Build.Module,
    zgpu_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        exe.addModule("zgpu", pkg.zgpu);
        exe.addModule("zgpu_options", pkg.zgpu_options);

        const b = exe.step.owner;

        switch (target.os.tag) {
            .windows => {
                const dawn_dep = b.dependency("dawn_x86_64_windows_gnu", .{});
                exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                exe.addLibraryPath(.{ .path = thisDir() ++ "/../system-sdk/windows/lib/x86_64-windows-gnu" });

                exe.linkSystemLibraryName("ole32");
                exe.linkSystemLibraryName("dxguid");
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    const dawn_dep = b.dependency("dawn_x86_64_linux_gnu", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                } else {
                    const dawn_dep = b.dependency("dawn_aarch64_linux_gnu", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                }
            },
            .macos => {
                exe.addFrameworkPath(.{ .path = thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks" });
                exe.addSystemIncludePath(.{ .path = thisDir() ++ "/../system-sdk/macos12/usr/include" });
                exe.addLibraryPath(.{ .path = thisDir() ++ "/../system-sdk/macos12/usr/lib" });

                if (target.cpu.arch.isX86()) {
                    const dawn_dep = b.dependency("dawn_x86_64_macos", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                } else {
                    const dawn_dep = b.dependency("dawn_aarch64_macos", .{});
                    exe.addLibraryPath(.{ .path = dawn_dep.builder.build_root.path.? });
                }

                exe.linkSystemLibraryName("objc");
                exe.linkFramework("Metal");
                exe.linkFramework("CoreGraphics");
                exe.linkFramework("Foundation");
                exe.linkFramework("IOKit");
                exe.linkFramework("IOSurface");
                exe.linkFramework("QuartzCore");
            },
            else => unreachable,
        }

        exe.linkSystemLibraryName("dawn");
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
    _: std.zig.CrossTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
        deps: struct {
            zglfw: *std.Build.Module,
            zpool: *std.Build.Module,
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

    const zgpu = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = &.{
            .{ .name = "zgpu_options", .module = zgpu_options },
            .{ .name = "zglfw", .module = args.deps.zglfw },
            .{ .name = "zpool", .module = args.deps.zpool },
        },
    });

    return .{
        .options = args.options,
        .zgpu = zgpu,
        .zgpu_options = zgpu_options,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
