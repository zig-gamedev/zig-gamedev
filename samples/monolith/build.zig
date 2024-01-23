const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "monolith_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "monolith",
        .root_source_file = .{ .path = thisDir() ++ "/src/monolith.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zgui_pkg = @import("../../build.zig").zgui_glfw_wgpu_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const zgpu_pkg = @import("../../build.zig").zgpu_pkg;
    const zglfw_pkg = @import("../../build.zig").zglfw_pkg;
    const zmesh_pkg = @import("../../build.zig").zmesh_pkg;

    const zphysics_pkg = @import("zphysics").package(b, options.target, options.optimize, .{
        .options = .{
            .use_double_precision = false,
            .enable_debug_renderer = true,
            .enable_asserts = true,
        },
    });

    zmath_pkg.link(exe);
    zgui_pkg.link(exe);
    zgpu_pkg.link(exe);
    zglfw_pkg.link(exe);
    zmesh_pkg.link(exe);
    zphysics_pkg.link(exe);

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
