const builtin = @import("builtin");
const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "minimal_glfw_d3d12";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_name ++ ".zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    @import("system_sdk").addLibraryPathsTo(exe);

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zwin32 = b.dependency("zwin32", .{
        .target = options.target,
    });
    const zwin32_module = zwin32.module("root");
    exe.root_module.addImport("zwin32", zwin32_module);

    const zd3d12 = b.dependency("zd3d12", .{
        .target = options.target,
        .debug_layer = options.zd3d12_enable_debug_layer,
        .gbv = options.zd3d12_enable_gbv,
    });
    const zd3d12_module = zd3d12.module("root");
    exe.root_module.addImport("zd3d12", zd3d12_module);

    @import("../common/build.zig").link(exe, .{
        .zwin32 = zwin32_module,
        .zd3d12 = zd3d12_module,
    });

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });

    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const dxc_step = buildShaders(b);
        install_content_step.step.dependOn(dxc_step);
    }
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwin32").install_d3d12(&exe.step, .bin, "libs/zwin32") catch unreachable;

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    return exe;
}

fn buildShaders(b: *std.Build) *std.Build.Step {
    const dxc_step = b.step(demo_name ++ "-dxc", "Build shaders for '" ++ demo_name ++ "' demo");

    makeDxcCmd(b, dxc_step, "src/" ++ demo_name ++ ".hlsl", "vsMain", demo_name ++ ".vs.cso", "vs", "");
    makeDxcCmd(b, dxc_step, "src/" ++ demo_name ++ ".hlsl", "psMain", demo_name ++ ".ps.cso", "ps", "");

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
    const shader_dir = thisDir() ++ "/" ++ content_dir;

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
