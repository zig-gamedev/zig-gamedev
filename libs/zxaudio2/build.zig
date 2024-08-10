const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const options = .{
        .debug_layer = b.option(bool, "debug_layer", "Enables debug layer") orelse false,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    const zwin32 = b.dependency("zwin32", .{
        .target = target,
    });

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zxaudio2.zig"),
        .imports = &.{
            .{ .name = "zxaudio2_options", .module = options_module },
            .{ .name = "zwin32", .module = zwin32.module("root") },
        },
    });
}
