const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "vector_graphics_test",
        .root_source_file = .{ .path = thisDir() ++ "/src/vector_graphics_test.zig" },
        .target = options.target,
        .optimize = options.build_mode,
    });

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    const zwin32_pkg = zwin32.package(b, .{}, .{});
    const zd3d12_pkg = zd3d12.package(
        b,
        .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
        },
        .{ .zwin32_module = zwin32_pkg.module },
    );
    const common_pkg = common.package(
        b,
        .{},
        .{ .zwin32_module = zwin32_pkg.module, .zd3d12_module = zd3d12_pkg.module },
    );

    exe.addModule("zd3d12", zd3d12_pkg.module);
    exe.addModule("common", common_pkg.module);
    exe.addModule("zwin32", zwin32_pkg.module);

    zd3d12.link(exe, zd3d12_pkg.options);
    common.link(exe, .{});

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
