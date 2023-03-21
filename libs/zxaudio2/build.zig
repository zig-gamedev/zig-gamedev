const std = @import("std");

pub const Options = struct {
    enable_debug_layer: bool = false,
};

pub const Package = struct {
    options: Options,
    zxaudio2: *std.Build.Module,
    zxaudio2_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zxaudio2", pkg.zxaudio2);
        exe.addModule("zxaudio2_options", pkg.zxaudio2_options);
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

    const zxaudio2_options = step.createModule();

    const zxaudio2 = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .dependencies = &.{
            .{ .name = "zxaudio2_options", .module = zxaudio2_options },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    return .{
        .options = args.options,
        .zxaudio2 = zxaudio2,
        .zxaudio2_options = zxaudio2_options,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
