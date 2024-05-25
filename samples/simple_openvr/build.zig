const std = @import("std");
const builtin = @import("builtin");

const Options = @import("../../build.zig").Options;

const demo_name = "simple_openvr";
const content_dir = demo_name ++ "_content/";

// in future zig version e342433
pub fn pathResolve(b: *std.Build, paths: []const []const u8) []u8 {
    return std.fs.path.resolve(b.allocator, paths) catch @panic("OOM");
}

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
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

    const zopenvr = b.dependency("zopenvr", .{
        .target = options.target,
    });
    exe.root_module.addImport("zopenvr", zopenvr.module("root"));

    @import("zopenvr").addLibraryPathsTo(exe);
    @import("zopenvr").linkOpenVR(exe);
    @import("zopenvr").installOpenVR(&exe.step, options.target.result, .bin);

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
        const compile_shaders = @import("zwin32").addCompileShaders(b, demo_name, .{ .shader_ver = "6_6" });
        const root_path = pathResolve(b, &.{ @src().file, "..", "..", ".." });
        const shaders_path = b.pathJoin(&.{ root_path, content_path, "shaders" });

        const common_hlsl_path = b.pathJoin(&.{ root_path, "samples", "common/src/hlsl/common.hlsl" });
        compile_shaders.addVsShader(common_hlsl_path, "vsImGui", b.pathJoin(&.{ shaders_path, "imgui.vs.cso" }), "PSO__IMGUI");
        compile_shaders.addPsShader(common_hlsl_path, "psImGui", b.pathJoin(&.{ shaders_path, "imgui.ps.cso" }), "PSO__IMGUI");
        compile_shaders.addCsShader(common_hlsl_path, "csGenerateMipmaps", b.pathJoin(&.{ shaders_path, "generate_mipmaps.cs.cso" }), "PSO__GENERATE_MIPMAPS");

        const axes_hlsl_path = b.pathJoin(&.{ root_path, src_path, "axes.hlsl" });
        compile_shaders.addVsShader(axes_hlsl_path, "VSMain", b.pathJoin(&.{ shaders_path, "axes.vs.cso" }), "PSO__AXES");
        compile_shaders.addPsShader(axes_hlsl_path, "PSMain", b.pathJoin(&.{ shaders_path, "axes.ps.cso" }), "PSO__AXES");

        const companion_hlsl_path = b.pathJoin(&.{ root_path, src_path, "companion.hlsl" });
        compile_shaders.addVsShader(companion_hlsl_path, "VSMain", b.pathJoin(&.{ shaders_path, "companion.vs.cso" }), "PSO__COMPANION");
        compile_shaders.addPsShader(companion_hlsl_path, "PSMain", b.pathJoin(&.{ shaders_path, "companion.ps.cso" }), "PSO__COMPANION");

        const render_model_hlsl_path = b.pathJoin(&.{ root_path, src_path, "render_model.hlsl" });
        compile_shaders.addVsShader(render_model_hlsl_path, "VSMain", b.pathJoin(&.{ shaders_path, "render_model.vs.cso" }), "PSO__RENDER_MODEL");
        compile_shaders.addPsShader(render_model_hlsl_path, "PSMain", b.pathJoin(&.{ shaders_path, "render_model.ps.cso" }), "PSO__RENDER_MODEL");

        const scene_hlsl_path = b.pathJoin(&.{ root_path, src_path, "scene.hlsl" });
        compile_shaders.addVsShader(scene_hlsl_path, "VSMain", b.pathJoin(&.{ shaders_path, "scene.vs.cso" }), "PSO__SCENE");
        compile_shaders.addPsShader(scene_hlsl_path, "PSMain", b.pathJoin(&.{ shaders_path, "scene.ps.cso" }), "PSO__SCENE");

        install_content_step.step.dependOn(compile_shaders.step);
    }
    exe.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwin32").install_d3d12(&exe.step, .bin);

    return exe;
}
