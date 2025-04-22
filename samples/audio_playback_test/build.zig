const builtin = @import("builtin");
const std = @import("std");

pub const demo_name = "audio_playback_test";
pub const content_dir = demo_name ++ "_content/";

// in future zig version e342433
pub fn pathResolve(b: *std.Build, paths: []const []const u8) []u8 {
    return std.fs.path.resolve(b.allocator, paths) catch @panic("OOM");
}

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwindows = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = options.zxaudio2_debug_layer,
        .zd3d12_debug_layer = options.zd3d12_debug_layer,
        .zd3d12_gbv = options.zd3d12_gbv,
    });
    const zwindows_module = zwindows.module("zwindows");
    const zd3d12_module = zwindows.module("zd3d12");

    exe.root_module.addImport("zwindows", zwindows_module);
    exe.root_module.addImport("zd3d12", zd3d12_module);

    @import("../common/build.zig").link(exe, .{
        .zwindows = zwindows_module,
        .zd3d12 = zd3d12_module,
    });

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const content_path = b.pathJoin(&.{ cwd_path, content_dir });
    const install_content_step = b.addInstallDirectory(.{
        .source_dir = b.path(content_path),
        .install_dir = .{ .custom = "" },
        .install_subdir = b.pathJoin(&.{ "bin", content_dir }),
    });
    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const compile_shaders = @import("zwindows").addCompileShaders(b, demo_name, zwindows, .{ .shader_ver = "6_6" });
        const root_path = pathResolve(b, &.{ @src().file, "..", "..", ".." });
        const shaders_path = b.pathJoin(&.{ root_path, content_path, "shaders" });

        const common_hlsl_path = b.pathJoin(&.{ root_path, "samples", "common/src/hlsl/common.hlsl" });
        compile_shaders.addVsShader(common_hlsl_path, "vsImGui", b.pathJoin(&.{ shaders_path, "imgui.vs.cso" }), "PSO__IMGUI");
        compile_shaders.addPsShader(common_hlsl_path, "psImGui", b.pathJoin(&.{ shaders_path, "imgui.ps.cso" }), "PSO__IMGUI");

        const hlsl_path = b.pathJoin(&.{ root_path, src_path, demo_name ++ ".hlsl" });
        compile_shaders.addVsShader(hlsl_path, "vsLines", b.pathJoin(&.{ shaders_path, "lines.vs.cso" }), "PSO__LINES");
        compile_shaders.addPsShader(hlsl_path, "psLines", b.pathJoin(&.{ shaders_path, "lines.ps.cso" }), "PSO__LINES");
        compile_shaders.addVsShader(hlsl_path, "vsImage", b.pathJoin(&.{ shaders_path, "image.vs.cso" }), "PSO__IMAGE");
        compile_shaders.addPsShader(hlsl_path, "psImage", b.pathJoin(&.{ shaders_path, "image.ps.cso" }), "PSO__IMAGE");

        install_content_step.step.dependOn(compile_shaders.step);
    }
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwindows").install_xaudio2(&exe.step, zwindows, .bin);
    @import("zwindows").install_d3d12(&exe.step, zwindows, .bin);

    return exe;
}
