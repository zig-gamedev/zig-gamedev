const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "textured_quad_wgpu_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "textured_quad_wgpu",
        .root_source_file = .{ .path = thisDir() ++ "/src/textured_quad_wgpu.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
