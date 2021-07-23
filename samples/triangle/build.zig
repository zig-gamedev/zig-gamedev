const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const files = [_][]const u8{
        "D3D12Core.dll",
        "D3D12Core.pdb",
        "D3D12SDKLayers.dll",
        "D3D12SDKLayers.pdb",
    };
    inline for (files) |file| {
        std.fs.Dir.copyFile(
            std.fs.cwd(),
            "../../external/bin/D3D12/" ++ file,
            std.fs.cwd(),
            "zig-out/bin/D3D12/" ++ file,
            .{},
        ) catch unreachable;
    }

    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    const exe = b.addExecutable("triangle", "src/triangle.zig");
    exe.setTarget(target);
    exe.setBuildMode(mode);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    exe.addPackagePath("winsdk", "../../libs/winsdk/winsdk.zig");

    exe.linkSystemLibrary("c");
    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const hlsl_step = b.step("hlsl", "Build shaders");
    var hlsl_command = [_][]const u8{
        "../../external/bin/dxc.exe",
        "path to input file",
        "entry point name",
        "path to output file",
        "target profile",
        "/WX",
        "/Ges",
        "/O3",
    };
    const shader_dir = "zig-out/bin/data/shaders/";
    const shader_ver = "6_6";

    hlsl_command[1] = "src/triangle.hlsl";
    hlsl_command[2] = "/E vsTriangle";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "triangle.vs.cso";
    hlsl_command[4] = "/T vs_" ++ shader_ver;
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    hlsl_command[1] = "src/triangle.hlsl";
    hlsl_command[2] = "/E psTriangle";
    hlsl_command[3] = "/Fo " ++ shader_dir ++ "triangle.ps.cso";
    hlsl_command[4] = "/T ps_" ++ shader_ver;
    hlsl_step.dependOn(&b.addSystemCommand(&hlsl_command).step);

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(hlsl_step);
    run_step.dependOn(&run_cmd.step);
}
