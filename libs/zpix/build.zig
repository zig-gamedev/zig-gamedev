const std = @import("std");

pub const Options = struct {
    enable: bool = false,
};

pub const Package = struct {
    options: Options,
    zpix: *std.Build.Module,
    zpix_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zpix", pkg.zpix);
        exe.addModule("zpix_options", pkg.zpix_options);
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
    step.addOption(bool, "enable", args.options.enable);

    const zpix_options = step.createModule();

    const zpix = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zpix.zig" },
        .dependencies = &.{
            .{ .name = "zpix_options", .module = zpix_options },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    return .{
        .options = args.options,
        .zpix = zpix,
        .zpix_options = zpix_options,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32 = b.dependency("zwin32", .{});

    _ = package(b, target, optimize, .{
        .options = .{
            .enable = b.option(bool, "enable", "enable zpix") orelse false,
        },
        .deps = .{
            .zwin32 = zwin32.module("zwin32"),
        },
    });
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
