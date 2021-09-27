const std = @import("std");
const Builder = std.build.Builder;
const Pkg = std.build.Pkg;

pub fn build(b: *std.build.Builder) void {
    const files = [_][]const u8{
        "D3D12Core.dll",
        "D3D12Core.pdb",
        "D3D12SDKLayers.dll",
        "D3D12SDKLayers.pdb",
    };
    std.fs.cwd().makePath("zig-out/bin/d3d12") catch unreachable;
    inline for (files) |file| {
        std.fs.Dir.copyFile(
            std.fs.cwd(),
            "../../external/bin/d3d12/" ++ file,
            std.fs.cwd(),
            "zig-out/bin/d3d12/" ++ file,
            .{},
        ) catch unreachable;
    }

    const hlsl_step = b.step("hlsl", "Build shaders");
    var hlsl_command = [_][]const u8{
        "../../external/bin/dxc/dxc.exe",
        "path to input file",
        "entry point name",
        "path to output file",
        "target profile",
        "/D",
        "/WX",
        "/Ges",
        "/O3",
    };
    const shader_dir = "content/shaders/";
    const shader_ver = "6_6";

    hlsl_command[1] = "src/textured_quad.hlsl";
    hlsl_command[2] = "/E vsMain";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "textured_quad.vs.cso";
    hlsl_command[4] = "/T vs_" ++ shader_ver;
    hlsl_command[5] = "";
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "src/textured_quad.hlsl";
    hlsl_command[2] = "/E psMain";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "textured_quad.ps.cso";
    hlsl_command[4] = "/T ps_" ++ shader_ver;
    hlsl_command[5] = "";
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "../../libs/common/common.hlsl";
    hlsl_command[2] = "/E vsImGui";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "imgui.vs.cso";
    hlsl_command[4] = "/T vs_" ++ shader_ver;
    hlsl_command[5] = "/D PSO__IMGUI";
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "../../libs/common/common.hlsl";
    hlsl_command[2] = "/E psImGui";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "imgui.ps.cso";
    hlsl_command[4] = "/T ps_" ++ shader_ver;
    hlsl_command[5] = "/D PSO__IMGUI";
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "../../libs/common/common.hlsl";
    hlsl_command[2] = "/E csGenerateMipmaps";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "generate_mipmaps.cs.cso";
    hlsl_command[4] = "/T cs_" ++ shader_ver;
    hlsl_command[5] = "/D PSO__GENERATE_MIPMAPS";
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    b.getInstallStep().dependOn(hlsl_step);

    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    const exe = b.addExecutable("textured_quad", "src/textured_quad.zig");
    exe.setTarget(target);
    exe.setBuildMode(mode);

    const enable_pix = b.option(bool, "enable-pix", "Enable PIX GPU events and markers") orelse false;
    const enable_dx_debug = b.option(bool, "enable-dx-debug", "Enable debug layer for D3D12, D2D1, DirectML and DXGI") orelse false;
    const enable_dx_gpu_debug = b.option(bool, "enable-dx-gpu-debug", "Enable GPU-based validation for D3D12") orelse false;
    const tracy = b.option([]const u8, "tracy", "Enable Tracy profiler integration (supply path to Tracy source)");

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);

    exe_options.addOption(bool, "enable_pix", enable_pix);
    exe_options.addOption(bool, "enable_dx_debug", enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_tracy", tracy != null);
    if (tracy) |tracy_path| {
        const client_cpp = std.fs.path.join(
            b.allocator,
            &[_][]const u8{ tracy_path, "TracyClient.cpp" },
        ) catch unreachable;
        exe.addIncludeDir(tracy_path);
        exe.addCSourceFile(client_cpp, &[_][]const u8{
            "-DTRACY_ENABLE=1",
            "-fno-sanitize=undefined",
            "-D_WIN32_WINNT=0x601",
        });
        exe.linkSystemLibrary("ws2_32");
        exe.linkSystemLibrary("dbghelp");
    }

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    exe.want_lto = false;

    const pkg_win32 = Pkg{
        .name = "win32",
        .path = .{ .path = "../../libs/win32/win32.zig" },
    };
    exe.addPackage(pkg_win32);

    const pkg_common = Pkg{
        .name = "common",
        .path = .{ .path = "../../libs/common/common.zig" },
        .dependencies = &[_]Pkg{
            Pkg{
                .name = "win32",
                .path = .{ .path = "../../libs/win32/win32.zig" },
                .dependencies = null,
            },
            Pkg{
                .name = "build_options",
                .path = exe_options.getSource(),
                .dependencies = null,
            },
        },
    };
    exe.addPackage(pkg_common);

    const external = "../../external/src";
    exe.addIncludeDir(external);

    exe.linkSystemLibrary("c");
    exe.linkSystemLibrary("c++");
    exe.linkSystemLibrary("imm32");
    exe.addCSourceFile(external ++ "/cimgui/imgui/imgui.cpp", &[_][]const u8{""});
    exe.addCSourceFile(external ++ "/cimgui/imgui/imgui_widgets.cpp", &[_][]const u8{""});
    exe.addCSourceFile(external ++ "/cimgui/imgui/imgui_tables.cpp", &[_][]const u8{""});
    exe.addCSourceFile(external ++ "/cimgui/imgui/imgui_draw.cpp", &[_][]const u8{""});
    exe.addCSourceFile(external ++ "/cimgui/imgui/imgui_demo.cpp", &[_][]const u8{""});
    exe.addCSourceFile(external ++ "/cimgui/cimgui.cpp", &[_][]const u8{""});

    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
