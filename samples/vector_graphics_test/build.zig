const std = @import("std");

const demo_name = "vector_graphics_test";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwindows = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = options.zxaudio2_debug_layer,
        .zd3d12_debug_layer = options.zd3d12_debug_layer,
        .zd3d12_gbv = options.zd3d12_gbv,
    });
    const zwindows_module = zwindows.module("zwindows");
    const zd3d12_module = zwindows.module("zd3d12");

    exe.root_module.addImport("zwindows", zwindows_module);
    exe.root_module.addImport("zd3d12", zd3d12_module);

    @import("../common/build.zig").link(exe, .{
        .zwindows = zwindows_module,
        .zd3d12 = zd3d12_module,
    });

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwindows").install_d3d12(&exe.step, .bin);

    return exe;
}
