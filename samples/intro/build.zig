const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const ztracy = @import("../../libs/ztracy/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const znoise = @import("../../libs/znoise/build.zig");
const zbullet = @import("../../libs/zbullet/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "intro_content/";

pub fn build(b: *std.build.Builder, options: Options, comptime intro_index: u32) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_pix", options.enable_pix);
    exe_options.addOption(bool, "enable_dx_debug", options.enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", options.enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_tracy", options.enable_tracy);
    if (intro_index == 0) {
        exe_options.addOption(bool, "enable_d2d", true);
    } else {
        exe_options.addOption(bool, "enable_d2d", false);
    }
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const intro_index_str = comptime std.fmt.comptimePrint("{}", .{intro_index});
    const exe = b.addExecutable(
        "intro" ++ intro_index_str,
        thisDir() ++ "/src/intro" ++ intro_index_str ++ ".zig",
    );
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    exe.addOptions("build_options", exe_options);

    const dxc_step = buildShaders(b, intro_index_str);
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

    const options_pkg = exe_options.getPackage("build_options");
    exe.addPackage(ztracy.getPkg(b, options_pkg));
    exe.addPackage(zd3d12.getPkg(b, options_pkg));
    exe.addPackage(common.getPkg(b, options_pkg));
    exe.addPackage(zwin32.pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(zmesh.pkg);
    exe.addPackage(znoise.pkg);
    exe.addPackage(zbullet.pkg);

    ztracy.link(exe, options.enable_tracy, .{});
    zd3d12.link(exe);
    zmesh.link(exe);
    znoise.link(exe);
    zbullet.link(exe);
    common.link(exe);

    return exe;
}

fn buildShaders(b: *std.build.Builder, comptime intro_index_str: []const u8) *std.build.Step {
    const dxc_step = b.step(
        "intro" ++ intro_index_str ++ "-dxc",
        "Build shaders for 'intro" ++ intro_index_str ++ "' demo",
    );

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

    dxc_command = makeDxcCmd("src/intro1.hlsl", "vsMain", "intro1.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro1.hlsl", "psMain", "intro1.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro2.hlsl", "vsMain", "intro2.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro2.hlsl", "psMain", "intro2.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro3.hlsl", "vsMain", "intro3.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro3.hlsl", "psMain", "intro3.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4_bindless.vs.cso", "vs", "PSO__BINDLESS");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4_bindless.ps.cso", "ps", "PSO__BINDLESS");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro5.hlsl", "vsMain", "intro5.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro5.hlsl", "psMain", "intro5.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro6.hlsl", "vsMain", "simple.vs.cso", "vs", "PSO__SIMPLE");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro6.hlsl", "psMain", "simple.ps.cso", "ps", "PSO__SIMPLE");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro6.hlsl", "vsPhysicsDebug", "physics_debug.vs.cso", "vs", "PSO__PHYSICS_DEBUG");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro6.hlsl", "psPhysicsDebug", "physics_debug.ps.cso", "ps", "PSO__PHYSICS_DEBUG");
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
