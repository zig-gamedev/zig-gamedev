const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "triangle_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("triangle", thisDir() ++ "/src/triangle.zig");

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

    const zd3d12_pkg = zd3d12.getPkg(&.{ zwin32.pkg, zd3d12_options.getPkg() });
    const common_pkg = common.getPkg(&.{ zd3d12_pkg, zwin32.pkg });

    exe.addPackage(zd3d12_pkg);
    exe.addPackage(common_pkg);
    exe.addPackage(zwin32.pkg);

    zd3d12.link(exe, zd3d12_options);
    common.link(exe);

    return exe;
}

fn buildShaders(b: *std.build.Builder) *std.build.Step {
    const dxc_step = b.step("triangle-dxc", "Build shaders for 'triangle' demo");

    {
        var dxc_command = makeDxcCmd(
            "../../libs/common/src/hlsl/common.hlsl",
            "vsImGui",
            "imgui.vs.cso",
            "vs",
            "PSO__IMGUI",
        );
        const cmd_step = b.addSystemCommand(&dxc_command);
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
        dxc_step.dependOn(&cmd_step.step);
    }
    {
        var dxc_command = makeDxcCmd(
            "../../libs/common/src/hlsl/common.hlsl",
            "psImGui",
            "imgui.ps.cso",
            "ps",
            "PSO__IMGUI",
        );
        const cmd_step = b.addSystemCommand(&dxc_command);
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
        dxc_step.dependOn(&cmd_step.step);
    }
    {
        var dxc_command = makeDxcCmd(
            "src/triangle.hlsl",
            "vsTriangle",
            "triangle.vs.cso",
            "vs",
            "",
        );
        const cmd_step = b.addSystemCommand(&dxc_command);
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
        dxc_step.dependOn(&cmd_step.step);
    }
    {
        var dxc_command = makeDxcCmd(
            "src/triangle.hlsl",
            "psTriangle",
            "triangle.ps.cso",
            "ps",
            "",
        );
        const cmd_step = b.addSystemCommand(&dxc_command);
        cmd_step.setEnvironmentVariable("LD_LIBRARY_PATH", thisDir() ++ "/../../libs/zwin32/bin/x64");
        dxc_step.dependOn(&cmd_step.step);
    }

    return dxc_step;
}

fn makeDxcCmd(
    comptime input_path: []const u8,
    comptime entry_point: []const u8,
    comptime output_filename: []const u8,
    comptime profile: []const u8,
    comptime define: []const u8,
) [9][]const u8 {
    const shader_ver = "6_6";
    const shader_dir = thisDir() ++ "/" ++ content_dir ++ "shaders/";
    return [9][]const u8{
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
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
