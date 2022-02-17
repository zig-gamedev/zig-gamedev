const std = @import("std");
const Builder = std.build.Builder;
const Pkg = std.build.Pkg;

fn makeDxcCmd(
    comptime input_path: []const u8,
    comptime entry_point: []const u8,
    comptime output_filename: []const u8,
    comptime profile: []const u8,
    comptime define: []const u8,
) [9][]const u8 {
    const shader_ver = "6_6";
    const shader_dir = "content/shaders/";
    return [9][]const u8{
        "../../external/bin/dxc/dxc.exe",
        input_path,
        "/E " ++ entry_point,
        "/Fo " ++ shader_dir ++ output_filename,
        "/T " ++ profile ++ "_" ++ shader_ver,
        if (define.len == 0) "" else "/D " ++ define,
        "/WX",
        "/Ges",
        "/O3",
    };
}

pub fn build(b: *std.build.Builder) void {
    const enable_pix = b.option(bool, "enable-pix", "Enable PIX GPU events and markers") orelse false;
    const enable_dx_debug = b.option(
        bool,
        "enable-dx-debug",
        "Enable debug layer for D3D12, D2D1, DirectML and DXGI",
    ) orelse false;
    const enable_dx_gpu_debug = b.option(
        bool,
        "enable-dx-gpu-debug",
        "Enable GPU-based validation for D3D12",
    ) orelse false;
    const tracy = b.option([]const u8, "tracy", "Enable Tracy profiler integration (supply path to Tracy source)");

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_pix", enable_pix);
    exe_options.addOption(bool, "enable_dx_debug", enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_tracy", tracy != null);

    b.installFile("../../external/bin/d3d12/D3D12Core.dll", "bin/d3d12/D3D12Core.dll");
    b.installFile("../../external/bin/d3d12/D3D12Core.pdb", "bin/d3d12/D3D12Core.pdb");
    b.installFile("../../external/bin/d3d12/D3D12SDKLayers.dll", "bin/d3d12/D3D12SDKLayers.dll");
    b.installFile("../../external/bin/d3d12/D3D12SDKLayers.pdb", "bin/d3d12/D3D12SDKLayers.pdb");
    const install_content_step = b.addInstallDirectory(
        .{ .source_dir = "content", .install_dir = .{ .custom = "" }, .install_subdir = "bin/content" },
    );
    b.getInstallStep().dependOn(&install_content_step.step);

    const dxc_step = b.step("dxc", "Build shaders");

    var dxc_command = makeDxcCmd("../../libs/common/common.hlsl", "vsImGui", "imgui.vs.cso", "vs", "PSO__IMGUI");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("../../libs/common/common.hlsl", "psImGui", "imgui.ps.cso", "ps", "PSO__IMGUI");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro1.hlsl", "vsMain", "intro1.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro1.hlsl", "psMain", "intro1.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro2.hlsl", "vsMain", "intro2.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro2.hlsl", "psMain", "intro2.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro3.hlsl", "vsMain", "intro3.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro3.hlsl", "psMain", "intro3.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro4.hlsl", "vsMain", "intro4_bindless.vs.cso", "vs", "PSO__BINDLESS");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro4.hlsl", "psMain", "intro4_bindless.ps.cso", "ps", "PSO__BINDLESS");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd("src/intro5.hlsl", "vsMain", "intro5.vs.cso", "vs", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);
    dxc_command = makeDxcCmd("src/intro5.hlsl", "psMain", "intro5.ps.cso", "ps", "");
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    dxc_command = makeDxcCmd(
        "../../libs/common/common.hlsl",
        "csGenerateMipmaps",
        "generate_mipmaps.cs.cso",
        "cs",
        "PSO__GENERATE_MIPMAPS",
    );
    dxc_step.dependOn(&b.addSystemCommand(&dxc_command).step);

    install_content_step.step.dependOn(dxc_step);

    const Program = struct {
        exe: *std.build.LibExeObjStep,
        deps: struct {
            cgltf: bool = false,
            zbullet: bool = false,
        },
    };

    const progs = [_]Program{
        .{ .exe = b.addExecutable("intro0", "src/intro0.zig"), .deps = .{} },
        .{ .exe = b.addExecutable("intro1", "src/intro1.zig"), .deps = .{} },
        .{ .exe = b.addExecutable("intro2", "src/intro2.zig"), .deps = .{ .cgltf = true } },
        .{ .exe = b.addExecutable("intro3", "src/intro3.zig"), .deps = .{ .cgltf = true } },
        .{ .exe = b.addExecutable("intro4", "src/intro4.zig"), .deps = .{ .cgltf = true } },
        .{ .exe = b.addExecutable("intro5", "src/intro5.zig"), .deps = .{ .cgltf = true } },
        .{ .exe = b.addExecutable("intro6", "src/intro6.zig"), .deps = .{ .cgltf = true, .zbullet = true } },
    };
    const active_prog = progs[6];
    const target_options = b.standardTargetOptions(.{});
    const release_options = b.standardReleaseOptions();

    for (progs) |prog| {
        prog.exe.setBuildMode(release_options);
        prog.exe.setTarget(target_options);
        prog.exe.addOptions("build_options", exe_options);

        if (tracy) |tracy_path| {
            const client_cpp = std.fs.path.join(
                b.allocator,
                &[_][]const u8{ tracy_path, "TracyClient.cpp" },
            ) catch unreachable;
            prog.exe.addIncludeDir(tracy_path);
            prog.exe.addCSourceFile(client_cpp, &[_][]const u8{
                "-DTRACY_ENABLE=1",
                "-fno-sanitize=undefined",
                "-D_WIN32_WINNT=0x601",
            });
            prog.exe.linkSystemLibrary("ws2_32");
            prog.exe.linkSystemLibrary("dbghelp");
        }

        // This is needed to export symbols from an .exe file.
        // We export D3D12SDKVersion and D3D12SDKPath symbols which
        // is required by DirectX 12 Agility SDK.
        prog.exe.rdynamic = true;
        prog.exe.want_lto = false;

        const pkg_win32 = Pkg{
            .name = "win32",
            .path = .{ .path = "../../libs/win32/win32.zig" },
        };
        prog.exe.addPackage(pkg_win32);

        const pkg_zmath = Pkg{
            .name = "zmath",
            .path = .{ .path = "../../libs/zmath/zmath.zig" },
        };
        prog.exe.addPackage(pkg_zmath);

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
        prog.exe.addPackage(pkg_common);

        const external = "../../external/src";
        prog.exe.addIncludeDir(external);

        prog.exe.linkSystemLibrary("c");
        prog.exe.linkSystemLibrary("c++");
        prog.exe.linkSystemLibrary("imm32");

        prog.exe.addCSourceFile(external ++ "/imgui/imgui.cpp", &.{""});
        prog.exe.addCSourceFile(external ++ "/imgui/imgui_widgets.cpp", &.{""});
        prog.exe.addCSourceFile(external ++ "/imgui/imgui_tables.cpp", &.{""});
        prog.exe.addCSourceFile(external ++ "/imgui/imgui_draw.cpp", &.{""});
        prog.exe.addCSourceFile(external ++ "/imgui/imgui_demo.cpp", &.{""});
        prog.exe.addCSourceFile(external ++ "/cimgui.cpp", &.{""});

        if (prog.deps.cgltf) {
            prog.exe.addCSourceFile(external ++ "/cgltf.c", &.{""});
        }

        if (prog.deps.zbullet) {
            const zbullet = Pkg{
                .name = "zbullet",
                .path = .{ .path = "../../libs/zbullet/src/zbullet.zig" },
            };
            prog.exe.addPackage(zbullet);

            prog.exe.addIncludeDir("../../libs/zbullet/libs/cbullet");
            prog.exe.addIncludeDir("../../libs/zbullet/libs/bullet");
            prog.exe.addCSourceFile("../../libs/zbullet/libs/cbullet/cbullet.cpp", &.{""});
            prog.exe.addCSourceFile("../../libs/zbullet/libs/bullet/btLinearMathAll.cpp", &.{""});
            prog.exe.addCSourceFile("../../libs/zbullet/libs/bullet/btBulletCollisionAll.cpp", &.{""});
            prog.exe.addCSourceFile("../../libs/zbullet/libs/bullet/btBulletDynamicsAll.cpp", &.{""});
        }

        prog.exe.install();
    }

    const run_cmd = active_prog.exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
