const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "vector_graphics_test",
        .root_source_file = .{ .path = thisDir() ++ "/src/vector_graphics_test.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zwin32_pkg = @import("../../build.zig").zwin32_pkg;
    const zd3d12_d2d_pkg = @import("../../build.zig").zd3d12_d2d_pkg;
    const common_d2d_pkg = @import("../../build.zig").common_d2d_pkg;

    zwin32_pkg.link(exe, .{ .d3d12 = true });
    common_d2d_pkg.link(exe);
    zd3d12_d2d_pkg.link(exe);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
