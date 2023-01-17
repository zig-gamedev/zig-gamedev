const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "minimal/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("minimal", thisDir() ++ "/src/minimal.zig");

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../../libs/zwin32/bin/x64/D3D12Core.dll" },
            "bin/d3d12/D3D12Core.dll",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../../libs/zwin32/bin/x64/D3D12Core.pdb" },
            "bin/d3d12/D3D12Core.pdb",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../../libs/zwin32/bin/x64/D3D12SDKLayers.dll" },
            "bin/d3d12/D3D12SDKLayers.dll",
        ).step,
    );
    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../../libs/zwin32/bin/x64/D3D12SDKLayers.pdb" },
            "bin/d3d12/D3D12SDKLayers.pdb",
        ).step,
    );

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    exe.rdynamic = true;

    exe.addPackage(zwin32.pkg);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
