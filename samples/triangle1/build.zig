const std = @import("std");

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
        "/WX",
        "/Ges",
        "/O3",
    };
    const shader_dir = "content/shaders/";
    const shader_ver = "6_6";

    hlsl_command[1] = "src/triangle1.hlsl";
    hlsl_command[2] = "/E vsTriangle";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "triangle1.vs.cso";
    hlsl_command[4] = "/T vs_" ++ shader_ver;
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "src/triangle1.hlsl";
    hlsl_command[2] = "/E psTriangle";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "triangle1.ps.cso";
    hlsl_command[4] = "/T ps_" ++ shader_ver;
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

    const exe = b.addExecutable("triangle1", "src/triangle1.zig");
    exe.setTarget(target);
    exe.setBuildMode(mode);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    exe.addPackagePath("win32", "../../libs/win32/win32.zig");
    exe.addPackagePath("graphics", "../../libs/common/graphics.zig");
    exe.addPackagePath("vectormath", "../../libs/common/vectormath.zig");
    exe.addPackagePath("library", "../../libs/common/library.zig");
    exe.addPackagePath("c", "../../libs/common/c.zig");

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
