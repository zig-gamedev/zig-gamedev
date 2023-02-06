const std = @import("std");

pub const Options = struct {
    enable_debug_layer: bool = false,
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

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .dependencies = &.{
            .{ .name = "zxaudio2_options", .module = options_module },
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

pub fn link(exe: *std.Build.CompileStep, options: Options) void {
    if (options.enable_debug_layer) {
        exe.step.dependOn(
            &exe.builder.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist_debug.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    } else {
        exe.step.dependOn(
            &exe.builder.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
