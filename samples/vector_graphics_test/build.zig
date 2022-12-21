const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("vector_graphics_test", thisDir() ++ "/src/vector_graphics_test.zig");

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    const zd3d12_options = zd3d12.BuildOptionsStep.init(b, .{
        .enable_debug_layer = options.zd3d12_enable_debug_layer,
        .enable_gbv = options.zd3d12_enable_gbv,
        .enable_d2d = true,
    });

    const zd3d12_pkg = zd3d12.getPkg(&.{ zwin32.pkg, zd3d12_options.getPkg() });
    const common_pkg = common.getPkg(&.{ zd3d12_pkg, zwin32.pkg });

    exe.addPackage(zd3d12_pkg);
    exe.addPackage(zwin32.pkg);
    exe.addPackage(common_pkg);

    zd3d12.link(exe, zd3d12_options);
    common.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
