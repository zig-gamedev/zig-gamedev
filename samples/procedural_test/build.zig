const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "procedural_test_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const exe = b.addExecutable("procedural_test", thisDir() ++ "/src/procedural_test.zig");
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    exe.addOptions("build_options", exe_options);

    exe.want_lto = false;

    const options_pkg = std.build.Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const common_pkg = std.build.Pkg{
        .name = "common",
        .path = .{ .path = thisDir() ++ "/../../libs/common/src/common.zig" },
        .dependencies = &[_]std.build.Pkg{
            options_pkg,
        },
    };
    exe.addPackage(common_pkg);
    @import("../../libs/common/build.zig").link(b, exe);

    exe.addPackagePath("glfw", thisDir() ++ "/../../libs/mach-glfw/src/main.zig");
    @import("../../libs/mach-glfw/build.zig").link(b, exe, .{ .opengl = true });

    const zminigl_pkg = std.build.Pkg{
        .name = "zminigl",
        .path = .{ .path = thisDir() ++ "/../../libs/zminigl/src/zminigl.zig" },
    };
    exe.addPackage(zminigl_pkg);

    return exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
