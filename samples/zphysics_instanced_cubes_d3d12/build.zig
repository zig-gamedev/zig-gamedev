const builtin = @import("builtin");
const std = @import("std");

const demo_name = "zphysics_instanced_cubes_d3d12";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zmath = b.dependency("zmath", .{
        .target = options.target,
    });
    exe.root_module.addImport("zmath", zmath.module("root"));

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zgui = b.dependency("zgui", .{
        .target = options.target,
        .backend = .glfw_dx12,
    });
    exe.root_module.addImport("zgui", zgui.module("root"));
    exe.linkLibrary(zgui.artifact("imgui"));

    const zwindows = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = options.zxaudio2_debug_layer,
        .zd3d12_debug_layer = options.zd3d12_debug_layer,
        .zd3d12_gbv = options.zd3d12_gbv,
    });
    const zwindows_module = zwindows.module("zwindows");
    const zd3d12_module = zwindows.module("zd3d12");

    exe.root_module.addImport("zwindows", zwindows_module);
    exe.root_module.addImport("zd3d12", zd3d12_module);

    const zphysics = b.dependency("zphysics", .{
        .target = options.target,
        .use_double_precision = false,
        .enable_debug_renderer = true,
        .enable_asserts = true,
    });
    exe.root_module.addImport("zphysics", zphysics.module("root"));
    exe.linkLibrary(zphysics.artifact("joltc"));

    const zpix = b.dependency("zpix", .{
        .enable = options.zpix_enable,
        .path = options.zpix_path,
    });
    exe.root_module.addImport("zpix", zpix.module("root"));

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);
    exe_options.addOption([]const u8, "demo_name", demo_name);

    const content_path = b.pathJoin(&.{ cwd_path, content_dir });
    const install_content_step = b.addInstallDirectory(.{
        .source_dir = b.path(content_path),
        .install_dir = .{ .custom = "" },
        .install_subdir = b.pathJoin(&.{ "bin", content_dir }),
    });
    exe.step.dependOn(&install_content_step.step);

    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const compile_shaders = @import("zwindows").addCompileShaders(b, demo_name, .{ .shader_ver = "6_6" });
        const root_path = b.pathResolve(&.{ @src().file, "..", "..", ".." });

        const hlsl_path = b.pathJoin(&.{ root_path, src_path, demo_name ++ ".hlsl" });
        compile_shaders.addVsShader(hlsl_path, "cubeVs", b.pathJoin(&.{ root_path, content_path, demo_name ++ "-cube.vs.cso" }), "");
        compile_shaders.addPsShader(hlsl_path, "ps", b.pathJoin(&.{ root_path, content_path, demo_name ++ ".ps.cso" }), "");

        install_content_step.step.dependOn(compile_shaders.step);
    }
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwindows").install_d3d12(&exe.step, .bin);

    return exe;
}
