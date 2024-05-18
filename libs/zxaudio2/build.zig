const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
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

    const test_step = b.step("test", "Run zxaudio2 tests");

    const tests = b.addTest(.{
        .name = "zxaudio2-tests",
        .root_source_file = b.path("src/zxaudio2.zig"),
        .target = target,
        .optimize = optimize,
    });
    tests.root_module.addImport("zwin32", zwin32.module("root"));
    tests.root_module.addImport("zxaudio2_options", options_module);
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);

    @import("zwin32").install_xaudio2(&tests.step, .bin);
}
