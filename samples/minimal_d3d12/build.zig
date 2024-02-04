const builtin = @import("builtin");
const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "minimal_d3d12_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_d3d12",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_d3d12.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwin32_pkg = @import("../../build.zig").zwin32_pkg;

    zwin32_pkg.link(exe, .{ .d3d12 = true });

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const dxc_step = buildShaders(
            b,
        );
        exe.step.dependOn(dxc_step);
        install_content_step.step.dependOn(dxc_step);
    }
    exe.step.dependOn(&install_content_step.step);

    exe.rdynamic = true;

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("minimal_d3d12-dxc", "Build shaders for 'minimal d3d12' demo");

    makeDxcCmd(b, dxc_step, "src/minimal_d3d12.hlsl", "vsMain", "../" ++ content_dir ++ "minimal_d3d12.vs.cso", "vs", "");
    makeDxcCmd(b, dxc_step, "src/minimal_d3d12.hlsl", "psMain", "../" ++ content_dir ++ "minimal_d3d12.ps.cso", "ps", "");

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
    const shader_ver = "6_0";
    const shader_dir = thisDir() ++ "/src/";

    const dxc_path = switch (builtin.target.os.tag) {
        .windows => thisDir() ++ "/../../libs/zwin32/bin/x64/dxc.exe",
        .linux => thisDir() ++ "/../../libs/zwin32/bin/x64/dxc",
        else => @panic("Unsupported target"),
    };

    const dxc_command = [9][]const u8{
        dxc_path,
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
    if (builtin.target.os.tag == .linux) {
        cmd_step.setEnvironmentVariable(
            "LD_LIBRARY_PATH",
            thisDir() ++ "/../../libs/zwin32/bin/x64",
        );
    }
    dxc_step.dependOn(&cmd_step.step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
