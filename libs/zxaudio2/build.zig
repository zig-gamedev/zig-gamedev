const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        enable_debug_layer: bool = false,
    };

    options: Options,
    zxaudio2: *std.Build.Module,
    zxaudio2_options: *std.Build.Module,

    pub fn build(
        b: *std.Build,
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

    pub fn link(zxaudio2_pkg: Package, exe: *std.Build.CompileStep) void {
        if (zxaudio2_pkg.options.enable_debug_layer) {
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
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
