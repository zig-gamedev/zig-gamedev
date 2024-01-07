const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "rasterization_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "rasterization",
        .root_source_file = .{ .path = thisDir() ++ "/src/rasterization.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwin32_pkg = @import("../../build.zig").zwin32_pkg;
    const zd3d12_pkg = @import("../../build.zig").zd3d12_pkg;
    const common_pkg = @import("../../build.zig").common_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const zmesh_pkg = @import("../../build.zig").zmesh_pkg;

    zwin32_pkg.link(exe, .{ .d3d12 = true });
    zmesh_pkg.link(exe);
    common_pkg.link(exe);
    zmath_pkg.link(exe);
    zd3d12_pkg.link(exe);

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const dxc_step = buildShaders(b);
    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    install_content_step.step.dependOn(dxc_step);
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("rasterization-dxc", "Build shaders for 'rasterization' demo");

    makeDxcCmd(
        b,
        dxc_step,
        "../../libs/common/src/hlsl/common.hlsl",
        "vsImGui",
        "imgui.vs.cso",
        "vs",
        "PSO__IMGUI",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "../../libs/common/src/hlsl/common.hlsl",
        "psImGui",
        "imgui.ps.cso",
        "ps",
        "PSO__IMGUI",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "../../libs/common/src/hlsl/common.hlsl",
        "csGenerateMipmaps",
        "generate_mipmaps.cs.cso",
        "cs",
        "PSO__GENERATE_MIPMAPS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "vsRecordPixels",
        "record_pixels.vs.cso",
        "vs",
        "PSO__RECORD_PIXELS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "psRecordPixels",
        "record_pixels.ps.cso",
        "ps",
        "PSO__RECORD_PIXELS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "vsDrawMesh",
        "draw_mesh.vs.cso",
        "vs",
        "PSO__DRAW_MESH",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "psDrawMesh",
        "draw_mesh.ps.cso",
        "ps",
        "PSO__DRAW_MESH",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "csDrawPixels",
        "draw_pixels.cs.cso",
        "cs",
        "PSO__DRAW_PIXELS",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/rasterization.hlsl",
        "csClearPixels",
        "clear_pixels.cs.cso",
        "cs",
        "PSO__CLEAR_PIXELS",
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
            thisDir() ++ "/../../libs/zwin32/bin/x64/dxc.exe"
        else if (@import("builtin").target.os.tag == .linux)
            thisDir() ++ "/../../libs/zwin32/bin/x64/dxc",
        thisDir() ++ "/" ++ input_path,
        "/E " ++ entry_point,
        "/Fo " ++ shader_dir ++ output_filename,
        "/T " ++ profile ++ "_" ++ shader_ver,
        if (define.len == 0) "" else "/D " ++ define,
        "/WX",
        "/Ges",
        "/O3",
    };

    const cmd_step = b.addSystemCommand(&dxc_command);
    if (@import("builtin").target.os.tag == .linux)
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
    dxc_step.dependOn(&cmd_step.step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
