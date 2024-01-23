const std = @import("std");

pub const API = enum {
    raw_bindings,
    wrapper,
};

pub const Options = struct {
    api: API,
};

pub const Package = struct {
    options: Options,
    zopengl: *std.Build.Module,
    zopengl_options: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zopengl", pkg.zopengl);
    }
};

pub fn package(
    b: *std.Build,
    _: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{
            .api = .raw_bindings,
        },
    },
) Package {
    const options_step = b.addOptions();
    inline for (std.meta.fields(Options)) |option_field| {
        const option_val = @field(args.options, option_field.name);
        options_step.addOption(@TypeOf(option_val), option_field.name, option_val);
    }

    const options = options_step.createModule();

    const zopengl = b.addModule("zopengl", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
        .imports = &.{
            .{ .name = "zopengl_options", .module = options },
        },
    });

    return .{
        .options = args.options,
        .zopengl = zopengl,
        .zopengl_options = options,
    };
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zopengl-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
        .target = target,
        .optimize = optimize,
    });
    const zopengl_pkg = package(b, target, optimize, .{
        .options = .{
            .api = .wrapper,
        },
    });
    tests.root_module.addImport("zopengl_options", zopengl_pkg.zopengl_options);
    zopengl_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zopengl tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{
        .options = .{
            .api = b.option(API, "api", "Select API") orelse .raw_bindings,
        },
    });
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
