const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const ztracy = @import("../../libs/ztracy/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "rasterization_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("rasterization", thisDir() ++ "/src/rasterization.zig");

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption(bool, "enable_dx_debug", options.enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", options.enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_d2d", false);
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
    exe.want_lto = false;

    const zmesh_options = zmesh.BuildOptionsStep.init(b, .{});
    const ztracy_options = ztracy.BuildOptionsStep.init(b, .{ .enable_ztracy = options.ztracy_enable });

    const options_pkg = exe_options.getPackage("build_options");
    const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});
    const zmesh_pkg = zmesh.getPkg(&.{zmesh_options.getPkg()});
    const zd3d12_pkg = zd3d12.getPkg(&.{ ztracy_pkg, zwin32.pkg, options_pkg });
    const common_pkg = common.getPkg(&.{ zd3d12_pkg, ztracy_pkg, zwin32.pkg, options_pkg });

    exe.addPackage(zmesh_pkg);
    exe.addPackage(ztracy_pkg);
    exe.addPackage(zd3d12_pkg);
    exe.addPackage(common_pkg);
    exe.addPackage(zwin32.pkg);
    exe.addPackage(zmath.pkg);

    ztracy.link(exe, ztracy_options);
    zmesh.link(exe, zmesh_options);
    zd3d12.link(exe);
    common.link(exe);

    return exe;
}

fn buildShaders(b: *std.build.Builder) *std.build.Step {
    const dxc_step = b.step("rasterization-dxc", "Build shaders for 'rasterization' demo");

    var dxc_command = makeDxcCmd(
        "../../libs/common/src/hlsl/common.hlsl",
        "vsImGui",
        "imgui.vs.cso",
        "vs",
        "PSO__IMGUI",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd(
        "../../libs/common/src/hlsl/common.hlsl",
        "psImGui",
        "imgui.ps.cso",
        "ps",
        "PSO__IMGUI",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "../../libs/common/src/hlsl/common.hlsl",
        "csGenerateMipmaps",
        "generate_mipmaps.cs.cso",
        "cs",
        "PSO__GENERATE_MIPMAPS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "vsRecordPixels",
        "record_pixels.vs.cso",
        "vs",
        "PSO__RECORD_PIXELS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "psRecordPixels",
        "record_pixels.ps.cso",
        "ps",
        "PSO__RECORD_PIXELS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "vsDrawMesh",
        "draw_mesh.vs.cso",
        "vs",
        "PSO__DRAW_MESH",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "psDrawMesh",
        "draw_mesh.ps.cso",
        "ps",
        "PSO__DRAW_MESH",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "csDrawPixels",
        "draw_pixels.cs.cso",
        "cs",
        "PSO__DRAW_PIXELS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "src/rasterization.hlsl",
        "csClearPixels",
        "clear_pixels.cs.cso",
        "cs",
        "PSO__CLEAR_PIXELS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

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
        thisDir() ++ "/../../libs/zwin32/bin/x64/dxc.exe",
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

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
