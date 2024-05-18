const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const options = .{
        .enable = b.option(bool, "enable", "enable zpix") orelse false,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    const zwin32 = b.dependency("zwin32", .{
        .target = target,
    });
    const zwin32_module = zwin32.module("root");

    _ = b.createModule(.{
        .root_source_file = b.path("src/zpix.zig"),
        .imports = &.{
            .{ .name = "zpix_options", .module = options_module },
            .{ .name = "zwin32", .module = zwin32_module },
        },
    });

    const test_step = b.step("test", "Run zpix tests");

    const tests = b.addTest(.{
        .name = "zpix-tests",
        .root_source_file = b.path("src/zpix.zig"),
        .target = target,
        .optimize = optimize,
    });
    tests.root_module.addImport("zwin32", zwin32_module);
    tests.root_module.addImport("zpix_options", options_module);
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
