const std = @import("std");

pub const Options = struct {
    enable: bool = false,
};

pub const Package = struct {
    module: *std.Build.Module,
    options: Options,
    options_module: *std.Build.Module,
};

pub fn package(
    b: *std.Build,
    args: struct {
        options: Options = .{},
        deps: struct { zwin32: *std.Build.Module },
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable", args.options.enable);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zpix.zig" },
        .dependencies = &.{
            .{ .name = "zpix_options", .module = options_module },
            .{ .name = "zwin32", .module = args.deps.zwin32 },
        },
    });

    return .{
        .module = module,
        .options = args.options,
        .options_module = options_module,
    };
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
