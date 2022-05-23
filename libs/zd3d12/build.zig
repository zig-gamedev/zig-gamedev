const std = @import("std");
const zwin32 = @import("../zwin32/build.zig");
const ztracy = @import("../ztracy/build.zig");

pub fn build(b: *std.build.Builder) void {
    _ = b;
}

pub fn getPkg(b: *std.build.Builder, options_pkg: std.build.Pkg) std.build.Pkg {
    const pkg = std.build.Pkg{
        .name = "zd3d12",
        .path = .{ .path = thisDir() ++ "/src/zd3d12.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32.pkg,
            ztracy.getPkg(b, options_pkg),
            options_pkg,
        },
    };
    return b.dupePkg(pkg);
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12Core.dll" },
            "bin/d3d12/D3D12Core.dll",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12Core.pdb" },
            "bin/d3d12/D3D12Core.pdb",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12SDKLayers.dll" },
            "bin/d3d12/D3D12SDKLayers.dll",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../zwin32/bin/x64/D3D12SDKLayers.pdb" },
            "bin/d3d12/D3D12SDKLayers.pdb",
        ).step,
    );
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
