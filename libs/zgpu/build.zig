const std = @import("std");

pub const BuildOptions = struct {
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

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.build.OptionsStep,

    pub fn init(b: *std.build.Builder, options: BuildOptions) BuildOptionsStep {
        const bos = BuildOptionsStep{
            .options = options,
            .step = b.addOptions(),
        };
        bos.step.addOption(u64, "uniforms_buffer_size", bos.options.uniforms_buffer_size);
        bos.step.addOption(bool, "dawn_skip_validation", bos.options.dawn_skip_validation);
        bos.step.addOption(u32, "buffer_pool_size", bos.options.buffer_pool_size);
        bos.step.addOption(u32, "texture_pool_size", bos.options.texture_pool_size);
        bos.step.addOption(u32, "texture_view_pool_size", bos.options.texture_view_pool_size);
        bos.step.addOption(u32, "sampler_pool_size", bos.options.sampler_pool_size);
        bos.step.addOption(u32, "render_pipeline_pool_size", bos.options.render_pipeline_pool_size);
        bos.step.addOption(u32, "compute_pipeline_pool_size", bos.options.compute_pipeline_pool_size);
        bos.step.addOption(u32, "bind_group_pool_size", bos.options.bind_group_pool_size);
        bos.step.addOption(u32, "bind_group_layout_pool_size", bos.options.bind_group_layout_pool_size);
        bos.step.addOption(u32, "pipeline_layout_pool_size", bos.options.pipeline_layout_pool_size);
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.build.Pkg {
        return bos.step.getPackage("zgpu_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.build.LibExeObjStep) void {
        target_step.addOptions("zgpu_options", bos.step);
    }
};

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = dependencies,
    };
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zgpu.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests, BuildOptionsStep.init(b, .{}));
    return tests;
}

pub fn link(exe: *std.build.LibExeObjStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);

    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    switch (target.os.tag) {
        .windows => {
            exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-windows-gnu");

            exe.linkSystemLibraryName("ole32");
            exe.linkSystemLibraryName("dxguid");
        },
        .linux => {
            if (target.cpu.arch.isX86())
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-linux-gnu")
            else
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/aarch64-linux-gnu");

            exe.linkSystemLibraryName("X11");
        },
        .macos => {
            if (target.cpu.arch.isX86())
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-macos-none")
            else
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/aarch64-macos-none");

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
    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    exe.addIncludePath(thisDir() ++ "/libs/dawn/include");
    exe.addIncludePath(thisDir() ++ "/src");

    exe.addCSourceFile(thisDir() ++ "/src/dawn.cpp", &.{"-std=c++17"});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
