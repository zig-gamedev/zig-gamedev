const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    _ = b;
    //const tests = b.addTest("src/zbullet.zig");
    //const zmath = std.build.Pkg{
    //    .name = "zmath",
    //    .path = .{ .path = thisDir() ++ "/../zmath/zmath.zig" },
    //};
    //tests.addPackage(zmath);
    //tests.setBuildMode(b.standardReleaseOptions());
    //tests.setTarget(b.standardTargetOptions(.{}));
    //link(b, tests);

    //const test_step = b.step("test", "Run library tests");
    //test_step.dependOn(&tests.step);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}

pub fn link(b: *std.build.Builder, exe: *std.build.LibExeObjStep) void {
    exe.step.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12Core.dll" },
            "bin/d3d12/D3D12Core.dll",
        ).step,
    );
    exe.step.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12Core.pdb" },
            "bin/d3d12/D3D12Core.pdb",
        ).step,
    );
    exe.step.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12SDKLayers.dll" },
            "bin/d3d12/D3D12SDKLayers.dll",
        ).step,
    );
    exe.step.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12SDKLayers.pdb" },
            "bin/d3d12/D3D12SDKLayers.pdb",
        ).step,
    );
}
