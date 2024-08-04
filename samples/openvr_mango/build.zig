const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "openvr_mango";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zopenvr = b.dependency("zopenvr", .{
        .target = options.target,
    });
    exe.root_module.addImport("zopenvr", zopenvr.module("root"));

    @import("zopenvr").addLibraryPathsTo(exe);
    @import("zopenvr").linkOpenVR(exe);
    @import("zopenvr").installOpenVR(&exe.step, options.target.result, .bin);

    return exe;
}
