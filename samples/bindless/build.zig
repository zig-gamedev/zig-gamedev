const builtin = @import("builtin");
const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "bindless";
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

    @import("system_sdk").addLibraryPathsTo(exe);

    const zmesh = b.dependency("zmesh", .{
        .target = options.target,
    });
    exe.root_module.addImport("zmesh", zmesh.module("root"));
    exe.linkLibrary(zmesh.artifact("zmesh"));

    const zstbi = b.dependency("zstbi", .{
        .target = options.target,
    });
    exe.root_module.addImport("zstbi", zstbi.module("root"));
    exe.linkLibrary(zstbi.artifact("zstbi"));

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

        const hlsl_path = b.pathJoin(&.{ root_path, src_path, demo_name ++ ".hlsl" });
        compile_shaders.addVsShader(hlsl_path, "vsMeshPbr", b.pathJoin(&.{ shaders_path, "mesh_pbr.vs.cso" }), "PSO__MESH_PBR");
        compile_shaders.addPsShader(hlsl_path, "psMeshPbr", b.pathJoin(&.{ shaders_path, "mesh_pbr.ps.cso" }), "PSO__MESH_PBR");
        compile_shaders.addVsShader(hlsl_path, "vsGenerateEnvTexture", b.pathJoin(&.{ shaders_path, "generate_env_texture.vs.cso" }), "PSO__GENERATE_ENV_TEXTURE");
        compile_shaders.addPsShader(hlsl_path, "psGenerateEnvTexture", b.pathJoin(&.{ shaders_path, "generate_env_texture.ps.cso" }), "PSO__GENERATE_ENV_TEXTURE");
        compile_shaders.addVsShader(hlsl_path, "vsSampleEnvTexture", b.pathJoin(&.{ shaders_path, "sample_env_texture.vs.cso" }), "PSO__SAMPLE_ENV_TEXTURE");
        compile_shaders.addPsShader(hlsl_path, "psSampleEnvTexture", b.pathJoin(&.{ shaders_path, "sample_env_texture.ps.cso" }), "PSO__SAMPLE_ENV_TEXTURE");
        compile_shaders.addVsShader(hlsl_path, "vsGenerateIrradianceTexture", b.pathJoin(&.{ shaders_path, "generate_irradiance_texture.vs.cso" }), "PSO__GENERATE_IRRADIANCE_TEXTURE");
        compile_shaders.addPsShader(hlsl_path, "psGenerateIrradianceTexture", b.pathJoin(&.{ shaders_path, "generate_irradiance_texture.ps.cso" }), "PSO__GENERATE_IRRADIANCE_TEXTURE");
        compile_shaders.addVsShader(hlsl_path, "vsGeneratePrefilteredEnvTexture", b.pathJoin(&.{ shaders_path, "generate_prefiltered_env_texture.vs.cso" }), "PSO__GENERATE_PREFILTERED_ENV_TEXTURE");
        compile_shaders.addPsShader(hlsl_path, "psGeneratePrefilteredEnvTexture", b.pathJoin(&.{ shaders_path, "generate_prefiltered_env_texture.ps.cso" }), "PSO__GENERATE_PREFILTERED_ENV_TEXTURE");
        compile_shaders.addCsShader(hlsl_path, "csGenerateBrdfIntegrationTexture", b.pathJoin(&.{ shaders_path, "generate_brdf_integration_texture.cs.cso" }), "PSO__GENERATE_BRDF_INTEGRATION_TEXTURE");

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
