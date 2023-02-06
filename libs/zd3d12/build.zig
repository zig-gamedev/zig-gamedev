const std = @import("std");

pub const Options = struct {
    enable_debug_layer: bool = false,
    enable_gbv: bool = false,
    enable_d2d: bool = false,
    upload_heap_capacity: u32 = 24 * 1024 * 1024,
};

pub const Package = struct {
    module: *std.Build.Module,
    options: Options,
    options_module: *std.Build.Module,
};

pub fn package(
    b: *std.Build,
    options: Options,
    deps: struct { zwin32_module: *std.Build.Module },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable_debug_layer", options.enable_debug_layer);
    step.addOption(bool, "enable_gbv", options.enable_gbv);
    step.addOption(bool, "enable_d2d", options.enable_d2d);
    step.addOption(u32, "upload_heap_capacity", options.upload_heap_capacity);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zd3d12.zig" },
        .dependencies = &.{
            .{ .name = "zd3d12_options", .module = options_module },
            .{ .name = "zwin32", .module = deps.zwin32_module },
        },
    });

    return .{
        .module = module,
        .options = options,
        .options_module = options_module,
    };
}

pub fn build(_: *std.Build) void {}

pub fn link(exe: *std.Build.CompileStep, _: Options) void {
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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
