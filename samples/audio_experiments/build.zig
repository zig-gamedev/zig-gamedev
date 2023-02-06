const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const zxaudio2 = @import("../../libs/zxaudio2/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "audio_experiments_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "audio_experiments",
        .root_source_file = .{ .path = thisDir() ++ "/src/audio_experiments.zig" },
        .target = options.target,
        .optimize = options.build_mode,
    });

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

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

    const zwin32_pkg = zwin32.package(b, .{});
    const zmath_pkg = zmath.package(b, .{});
    const zxaudio2_pkg = zxaudio2.package(b, .{
        .options = .{ .enable_debug_layer = options.zd3d12_enable_debug_layer },
        .deps = .{ .zwin32 = zwin32_pkg.module },
    });
    const zd3d12_pkg = zd3d12.package(b, .{
        .options = .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
        },
        .deps = .{ .zwin32 = zwin32_pkg.module },
    });
    const common_pkg = common.package(b, .{
        .deps = .{ .zwin32 = zwin32_pkg.module, .zd3d12 = zd3d12_pkg.module },
    });

    exe.addModule("zd3d12", zd3d12_pkg.module);
    exe.addModule("common", common_pkg.module);
    exe.addModule("zwin32", zwin32_pkg.module);
    exe.addModule("zmath", zmath_pkg.module);
    exe.addModule("zxaudio2", zxaudio2_pkg.module);

    zd3d12.link(exe);
    zxaudio2.link(exe, zxaudio2_pkg.options);
    common.link(exe);

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("audio_experiments-dxc", "Build shaders for 'audio experiments' demo");

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
    makeDxcCmd(b, dxc_step, "src/audio_experiments.hlsl", "vsMain", "lines.vs.cso", "vs", "");
    makeDxcCmd(b, dxc_step, "src/audio_experiments.hlsl", "psMain", "lines.ps.cso", "ps", "");

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
