const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    _ = b;
    //const tests = b.addTest("src/zbullet.zig");
    //const zmath = std.build.Pkg{
    //    .name = "zmath",
    //    .path = .{ .path = thisDir() ++ "/../zmath/zmath.zig" },
    //};
    //tests.addPackage(zmath);
    //tests.setBuildMode(b.standardReleaseOptions());
    //tests.setTarget(b.standardTargetOptions(.{}));
    //link(b, tests);

    //const test_step = b.step("test", "Run library tests");
    //test_step.dependOn(&tests.step);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}

pub const Options = struct {
    tracy_path: ?[]const u8 = null,
};

pub fn link(b: *std.build.Builder, exe: *std.build.LibExeObjStep, options: Options) void {
    if (options.tracy_path) |tracy_path| {
        const client_cpp = std.fs.path.join(
            b.allocator,
            &[_][]const u8{ tracy_path, "TracyClient.cpp" },
        ) catch unreachable;
        exe.addIncludeDir(tracy_path);
        exe.addCSourceFile(client_cpp, &[_][]const u8{
            "-DTRACY_ENABLE=1",
            "-fno-sanitize=undefined",
            "-D_WIN32_WINNT=0x601",
        });
        exe.linkSystemLibrary("ws2_32");
        exe.linkSystemLibrary("dbghelp");
    }
}
