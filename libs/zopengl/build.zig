const std = @import("std");

pub const Options = struct {
    api: enum {
        raw,
        wrapper,
    },
};

pub const Package = struct {
    options: Options,
    zopengl: *std.Build.Module,
    zopengl_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zopengl", pkg.zopengl);
    }
};

pub fn package(
    b: *std.Build,
    _: std.zig.CrossTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{
            .api = .raw,
        },
    },
) Package {
    const options_step = b.addOptions();
    inline for (std.meta.fields(Options)) |option_field| {
        const option_val = @field(args.options, option_field.name);
        options_step.addOption(@TypeOf(option_val), option_field.name, option_val);
    }

    const options = options_step.createModule();

    const zopengl = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
        .dependencies = &.{
            .{ .name = "zopengl_options", .module = options },
        },
    });

    return .{
        .options = args.options,
        .zopengl = zopengl,
        .zopengl_options = options,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
