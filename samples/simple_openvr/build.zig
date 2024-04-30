const std = @import("std");
const builtin = @import("builtin");

const Options = @import("../../build.zig").Options;

const demo_name = "simple_openvr";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_name ++ ".zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zmath = b.dependency("zmath", .{
        .target = options.target,
    });
    exe.root_module.addImport("zmath", zmath.module("root"));

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zwin32 = b.dependency("zwin32", .{
        .target = options.target,
    });
    const zwin32_module = zwin32.module("root");
    exe.root_module.addImport("zwin32", zwin32_module);

    const zd3d12 = b.dependency("zd3d12", .{
        .target = options.target,
        .debug_layer = options.zd3d12_enable_debug_layer,
        .gbv = options.zd3d12_enable_gbv,
    });
    const zd3d12_module = zd3d12.module("root");
    exe.root_module.addImport("zd3d12", zd3d12_module);

    const zopenvr = b.dependency("zopenvr", .{
        .target = options.target,
    });
    exe.root_module.addImport("zopenvr", zopenvr.module("root"));

    const zopenvr_path = zopenvr.path("").getPath(b);

    @import("zopenvr").addLibraryPathsTo(exe, zopenvr_path) catch unreachable;
    @import("zopenvr").linkOpenVR(exe);
    @import("zopenvr").installOpenVR(&exe.step, options.target.result, .bin, zopenvr_path) catch unreachable;

    @import("../common/build.zig").link(exe, .{
        .zwin32 = zwin32_module,
        .zd3d12 = zd3d12_module,
    });

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const dxc_step = buildShaders(b);
        exe.step.dependOn(dxc_step);
        install_content_step.step.dependOn(dxc_step);
    }
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("simple_openvr-dxc", "Build shaders for 'simple openvr' demo");
    makeDxcCmd(
        b,
        dxc_step,
        "../common/src/hlsl/common.hlsl",
        "csGenerateMipmaps",
        "generate_mipmaps.cs.cso",
        "cs",
        "PSO__GENERATE_MIPMAPS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/axes.hlsl",
        "VSMain",
        "axes.vs.cso",
        "vs",
        "PSO__AXES",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/axes.hlsl",
        "PSMain",
        "axes.ps.cso",
        "ps",
        "PSO__AXES",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/companion.hlsl",
        "VSMain",
        "companion.vs.cso",
        "vs",
        "PSO__COMPANION",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/companion.hlsl",
        "PSMain",
        "companion.ps.cso",
        "ps",
        "PSO__COMPANION",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/render_model.hlsl",
        "VSMain",
        "render_model.vs.cso",
        "vs",
        "PSO__RENDER_MODEL",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/render_model.hlsl",
        "PSMain",
        "render_model.ps.cso",
        "ps",
        "PSO__RENDER_MODEL",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/scene.hlsl",
        "VSMain",
        "scene.vs.cso",
        "vs",
        "PSO__SCENE",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "./src/scene.hlsl",
        "PSMain",
        "scene.ps.cso",
        "ps",
        "PSO__SCENE",
    );

    return dxc_step;
}

fn makeDxcCmd(
    b: *std.Build,
    dxc_step: *std.Build.Step,
    comptime input_path: []const u8,
    comptime entry_point: []const u8,
    comptime output_filename: []const u8,
    comptime profile: []const u8,
    comptime define: []const u8,
) void {
    const shader_ver = "6_6";
    const shader_dir = thisDir() ++ "/" ++ content_dir ++ "shaders/";

    const dxc_command = [9][]const u8{
        if (builtin.target.os.tag == .windows)
            thisDir() ++ "/../../libs/zwin32/bin/x64/dxc.exe"
        else if (builtin.target.os.tag == .linux)
            thisDir() ++ "/../../libs/zwin32/bin/x64/dxc",
        thisDir() ++ "/" ++ input_path,
        if (entry_point.len == 0) "" else "/E " ++ entry_point,
        "/Fo " ++ shader_dir ++ output_filename,
        "/T " ++ profile ++ "_" ++ shader_ver,
        if (define.len == 0) "" else "/D " ++ define,
        "/WX",
        "/Ges",
        "/O3",
    };

    const cmd_step = b.addSystemCommand(&dxc_command);
    if (builtin.target.os.tag == .linux)
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
    dxc_step.dependOn(&cmd_step.step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
