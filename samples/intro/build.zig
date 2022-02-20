const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "intro_content/";

pub fn build(b: *std.build.Builder, options: Options, comptime intro_index: u32) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_pix", options.enable_pix);
    exe_options.addOption(bool, "enable_dx_debug", options.enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", options.enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_tracy", options.tracy != null);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const intro_index_str = comptime std.fmt.comptimePrint("{}", .{intro_index});
    const exe = b.addExecutable(
        "intro" ++ intro_index_str,
        thisDir() ++ "/src/intro" ++ intro_index_str ++ ".zig",
    );
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    exe.addOptions("build_options", exe_options);

    exe.step.dependOn(
        &b.addInstallDirectory(.{
            .source_dir = thisDir() ++ "/" ++ content_dir,
            .install_dir = .{ .custom = "" },
            .install_subdir = "bin/" ++ content_dir,
        }).step,
    );

    buildShaders(b, exe);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;
    exe.want_lto = false;

    const options_pkg = std.build.Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const zwin32_pkg = std.build.Pkg{
        .name = "zwin32",
        .path = .{ .path = thisDir() ++ "/../../libs/zwin32/zwin32.zig" },
    };
    exe.addPackage(zwin32_pkg);

    const zmath_pkg = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = thisDir() ++ "/../../libs/zmath/zmath.zig" },
    };
    exe.addPackage(zmath_pkg);

    const ztracy_pkg = std.build.Pkg{
        .name = "ztracy",
        .path = .{ .path = thisDir() ++ "/../../libs/ztracy/src/ztracy.zig" },
        .dependencies = &[_]std.build.Pkg{options_pkg},
    };
    exe.addPackage(ztracy_pkg);
    @import("../../libs/ztracy/build.zig").link(b, exe, .{ .tracy_path = options.tracy });

    const zd3d12_pkg = std.build.Pkg{
        .name = "zd3d12",
        .path = .{ .path = thisDir() ++ "/../../libs/zd3d12/src/zd3d12.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32_pkg,
            ztracy_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zd3d12_pkg);
    @import("../../libs/zd3d12/build.zig").link(b, exe);

    const common_pkg = std.build.Pkg{
        .name = "common",
        .path = .{ .path = thisDir() ++ "/../../libs/common/common.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32_pkg,
            zd3d12_pkg,
            ztracy_pkg,
            options_pkg,
        },
    };
    exe.addPackage(common_pkg);

    const external = thisDir() ++ "/../../external/src";
    exe.addIncludeDir(external);

    exe.linkSystemLibrary("c");
    exe.linkSystemLibrary("c++");
    exe.linkSystemLibrary("imm32");

    exe.addCSourceFile(external ++ "/imgui/imgui.cpp", &.{""});
    exe.addCSourceFile(external ++ "/imgui/imgui_widgets.cpp", &.{""});
    exe.addCSourceFile(external ++ "/imgui/imgui_tables.cpp", &.{""});
    exe.addCSourceFile(external ++ "/imgui/imgui_draw.cpp", &.{""});
    exe.addCSourceFile(external ++ "/imgui/imgui_demo.cpp", &.{""});
    exe.addCSourceFile(external ++ "/cimgui.cpp", &.{""});

    exe.addCSourceFile(external ++ "/cgltf.c", &.{"-std=c99"});

    const zbullet_pkg = std.build.Pkg{
        .name = "zbullet",
        .path = .{ .path = thisDir() ++ "/../../libs/zbullet/src/zbullet.zig" },
    };
    exe.addPackage(zbullet_pkg);
    @import("../../libs/zbullet/build.zig").link(b, exe);

    return exe;
}

fn buildShaders(b: *std.build.Builder, exe: *std.build.LibExeObjStep) void {
    var dxc_command = makeDxcCmd("../../libs/common/common.hlsl", "vsImGui", "imgui.vs.cso", "vs", "PSO__IMGUI");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("../../libs/common/common.hlsl", "psImGui", "imgui.ps.cso", "ps", "PSO__IMGUI");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro1.hlsl", "vsMain", "intro1.vs.cso", "vs", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro1.hlsl", "psMain", "intro1.ps.cso", "ps", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro2.hlsl", "vsMain", "intro2.vs.cso", "vs", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro2.hlsl", "psMain", "intro2.ps.cso", "ps", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro3.hlsl", "vsMain", "intro3.vs.cso", "vs", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro3.hlsl", "psMain", "intro3.ps.cso", "ps", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4.vs.cso", "vs", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4.ps.cso", "ps", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4_bindless.vs.cso", "vs", "PSO__BINDLESS");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4_bindless.ps.cso", "ps", "PSO__BINDLESS");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro5.hlsl", "vsMain", "intro5.vs.cso", "vs", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro5.hlsl", "psMain", "intro5.ps.cso", "ps", "");
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "../../libs/common/common.hlsl",
        "csGenerateMipmaps",
        "generate_mipmaps.cs.cso",
        "cs",
        "PSO__GENERATE_MIPMAPS",
    );
    exe.step.dependOn(&b.addSystemCommand(&dxc_command).step);
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
    return std.fs.path.dirname(@src().file) orelse ".";
}
