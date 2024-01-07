const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_d3d12",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_d3d12.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwin32_pkg = @import("../../build.zig").zwin32_pkg;

    zwin32_pkg.link(exe, .{ .d3d12 = true });

    const dxc_step = buildShaders(b);
    exe.step.dependOn(dxc_step);

    exe.rdynamic = true;

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step("minimal_d3d12-dxc", "Build shaders for 'minimal d3d12' demo");

    makeDxcCmd(b, dxc_step, "src/minimal_d3d12.hlsl", "vsMain", "minimal_d3d12.vs.cso", "vs", "");
    makeDxcCmd(b, dxc_step, "src/minimal_d3d12.hlsl", "psMain", "minimal_d3d12.ps.cso", "ps", "");

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
