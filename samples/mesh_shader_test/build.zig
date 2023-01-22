const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "mesh_shader_test_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("mesh_shader_test", thisDir() ++ "/src/mesh_shader_test.zig");

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    const dxc_step = buildShaders(b);
    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    install_content_step.step.dependOn(dxc_step);
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    const zd3d12_options = zd3d12.BuildOptionsStep.init(b, .{
        .enable_debug_layer = options.zd3d12_enable_debug_layer,
        .enable_gbv = options.zd3d12_enable_gbv,
    });
    const zmesh_options = zmesh.BuildOptionsStep.init(b, .{});

    const zmesh_pkg = zmesh.getPkg(&.{zmesh_options.getPkg()});
    const zd3d12_pkg = zd3d12.getPkg(&.{ zwin32.pkg, zd3d12_options.getPkg() });
    const common_pkg = common.getPkg(&.{ zd3d12_pkg, zwin32.pkg });

    exe.addPackage(zmesh_pkg);
    exe.addPackage(zd3d12_pkg);
    exe.addPackage(zwin32.pkg);
    exe.addPackage(common_pkg);

    zd3d12.link(exe, zd3d12_options);
    zmesh.link(exe, zmesh_options);
    common.link(exe);

    return exe;
}

fn buildShaders(b: *std.build.Builder) *std.build.Step {
    const dxc_step = b.step("mesh_shader_test-dxc", "Build shaders for 'mesh shader test' demo");

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
        "src/mesh_shader_test.hlsl",
        "msMain",
        "mesh_shader.ms.cso",
        "ms",
        "PSO__MESH_SHADER",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/mesh_shader_test.hlsl",
        "psMain",
        "mesh_shader.ps.cso",
        "ps",
        "PSO__MESH_SHADER",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/mesh_shader_test.hlsl",
        "vsMain",
        "vertex_shader.vs.cso",
        "vs",
        "PSO__VERTEX_SHADER",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/mesh_shader_test.hlsl",
        "psMain",
        "vertex_shader.ps.cso",
        "ps",
        "PSO__VERTEX_SHADER",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/mesh_shader_test.hlsl",
        "vsMain",
        "vertex_shader_fixed.vs.cso",
        "vs",
        "PSO__VERTEX_SHADER_FIXED",
    );
    makeDxcCmd(
        b,
        dxc_step,
        "src/mesh_shader_test.hlsl",
        "psMain",
        "vertex_shader_fixed.ps.cso",
        "ps",
        "PSO__VERTEX_SHADER_FIXED",
    );

    return dxc_step;
}

fn makeDxcCmd(
    b: *std.build.Builder,
    dxc_step: *std.build.Step,
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
