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
    options: Options,
    deps: struct { zwin32_module: *std.Build.Module },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable", options.enable);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zpix.zig" },
        .dependencies = &.{
            .{ .name = "zpix_options", .module = options_module },
            .{ .name = "zwin32", .module = deps.zwin32_module },
        },
    });

    return .{
        .module = module,
        .options = options,
        .options_module = options_module,
    };
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
