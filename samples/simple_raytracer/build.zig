const builtin = @import("builtin");
const std = @import("std");

const content_dir = "simple_raytracer_content/";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "simple_raytracer",
        .root_source_file = .{ .path = thisDir() ++ "/src/simple_raytracer.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zpix = b.dependency("zpix", .{
        .target = options.target,
    });
    exe.root_module.addImport("zpix", zpix.module("root"));

    const zwindows = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = options.zxaudio2_debug_layer,
        .zd3d12_debug_layer = options.zd3d12_debug_layer,
        .zd3d12_gbv = options.zd3d12_gbv,
    });
    const zwindows_module = zwindows.module("zwindows");
    const zd3d12_module = zwindows.module("zd3d12");

    exe.root_module.addImport("zwindows", zwindows_module);
    exe.root_module.addImport("zd3d12", zd3d12_module);

    @import("../common/build.zig").link(exe, .{
        .zwindows = zwindows_module,
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

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwindows").install_d3d12(&exe.step, .bin);

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("simple_raytracer-dxc", "Build shaders for 'simple raytracer' demo");

    makeDxcCmd(
        b,
        dxc_step,
        "../common/src/hlsl/common.hlsl",
        "vsImGui",
        "imgui.vs.cso",
        "vs",
        "PSO__IMGUI",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "../common/src/hlsl/common.hlsl",
        "psImGui",
        "imgui.ps.cso",
        "ps",
        "PSO__IMGUI",
    );
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
        "src/simple_raytracer.hlsl",
        "vsRastStaticMesh",
        "rast_static_mesh.vs.cso",
        "vs",
        "PSO__RAST_STATIC_MESH",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "psRastStaticMesh",
        "rast_static_mesh.ps.cso",
        "ps",
        "PSO__RAST_STATIC_MESH",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "vsZPrePass",
        "z_pre_pass.vs.cso",
        "vs",
        "PSO__Z_PRE_PASS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "psZPrePass",
        "z_pre_pass.ps.cso",
        "ps",
        "PSO__Z_PRE_PASS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "vsGenShadowRays",
        "gen_shadow_rays.vs.cso",
        "vs",
        "PSO__GEN_SHADOW_RAYS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "psGenShadowRays",
        "gen_shadow_rays.ps.cso",
        "ps",
        "PSO__GEN_SHADOW_RAYS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/simple_raytracer.hlsl",
        "",
        "trace_shadow_rays.lib.cso",
        "lib",
        "PSO__TRACE_SHADOW_RAYS",
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
        if (@import("builtin").target.os.tag == .windows)
            thisDir() ++ "/../../libs/zwindows/bin/x64/dxc.exe"
        else if (@import("builtin").target.os.tag == .linux)
            thisDir() ++ "/../../libs/zwindows/bin/x64/dxc",
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
    if (@import("builtin").target.os.tag == .linux)
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwindows/bin/x64");
    dxc_step.dependOn(&cmd_step.step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
