const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        enable: bool = false,
    };

    options: Options,
    zpix: *std.Build.Module,
    zpix_options: *std.Build.Module,

    pub fn build(
        b: *std.Build,
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
};

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
