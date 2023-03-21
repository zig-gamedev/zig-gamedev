const std = @import("std");

pub const Options = struct {
    enable_debug_layer: bool = false,
    enable_gbv: bool = false,
    enable_d2d: bool = false,
    upload_heap_capacity: u32 = 24 * 1024 * 1024,
};

pub const Package = struct {
    options: Options,
    zd3d12: *std.Build.Module,
    zd3d12_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zd3d12", pkg.zd3d12);
        exe.addModule("zd3d12_options", pkg.zd3d12_options);
    }
};

pub fn package(
    b: *std.Build,
    _: std.zig.CrossTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
        deps: struct { zwin32: *std.Build.Module },
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable_debug_layer", args.options.enable_debug_layer);
    step.addOption(bool, "enable_gbv", args.options.enable_gbv);
    step.addOption(bool, "enable_d2d", args.options.enable_d2d);
    step.addOption(u32, "upload_heap_capacity", args.options.upload_heap_capacity);

    const zd3d12_options = step.createModule();

    const zd3d12 = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zd3d12.zig" },
        .dependencies = &.{
            .{ .name = "zd3d12_options", .module = zd3d12_options },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    return .{
        .options = args.options,
        .zd3d12 = zd3d12,
        .zd3d12_options = zd3d12_options,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
