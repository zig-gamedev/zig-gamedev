const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "opengl_test_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("opengl_test", thisDir() ++ "/src/opengl_test.zig");
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    exe.want_lto = false;

    const zmath_pkg = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = thisDir() ++ "/../../libs/zmath/zmath.zig" },
    };
    exe.addPackage(zmath_pkg);

    exe.addPackagePath("glfw", thisDir() ++ "/../../libs/mach-glfw/src/main.zig");
    @import("../../libs/mach-glfw/build.zig").link(b, exe, .{ .opengl = true, .vulkan = false });

    return exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
