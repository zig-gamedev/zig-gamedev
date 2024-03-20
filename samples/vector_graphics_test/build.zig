const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "vector_graphics_test",
        .root_source_file = .{ .path = thisDir() ++ "/src/vector_graphics_test.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const common = b.dependency("common", .{
        .target = options.target,
        .zd3d12_debug_layer = options.zd3d12_enable_debug_layer,
        .zd3d12_gbv = options.zd3d12_enable_gbv,
    });
    exe.root_module.addImport("common", common.module("root"));
    exe.linkLibrary(common.artifact("common"));

    const zwin32 = b.dependency("zwin32", .{
        .target = options.target,
    });
    exe.root_module.addImport("zwin32", zwin32.module("root"));

    const zd3d12 = b.dependency("zd3d12", .{
        .target = options.target,
        .debug_layer = options.zd3d12_enable_debug_layer,
        .gbv = options.zd3d12_enable_gbv,
    });
    exe.root_module.addImport("zd3d12", zd3d12.module("root"));

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
