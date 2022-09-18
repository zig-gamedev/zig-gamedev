const std = @import("std");
const system_sdk = @import("system_sdk.zig");

pub fn build(_: *std.build.Builder) void {}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(pkg.source.path);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

pub const pkg = std.build.Pkg{
    .name = "zglfw",
    .source = .{ .path = thisDir() ++ "/src/zglfw.zig" },
};

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs/glfw/include");
    exe.linkSystemLibraryName("c");
    system_sdk.include(exe.builder, exe, .{});

    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    const src_dir = thisDir() ++ "/libs/glfw/src/";

    switch (target.os.tag) {
        .windows => {
            exe.linkSystemLibraryName("gdi32");
            exe.linkSystemLibraryName("user32");
            exe.linkSystemLibraryName("shell32");
            exe.addCSourceFiles(&.{
                src_dir ++ "monitor.c",
                src_dir ++ "init.c",
                src_dir ++ "vulkan.c",
                src_dir ++ "input.c",
                src_dir ++ "context.c",
                src_dir ++ "window.c",
                src_dir ++ "osmesa_context.c",
                src_dir ++ "egl_context.c",
                src_dir ++ "wgl_context.c",
                src_dir ++ "win32_thread.c",
                src_dir ++ "win32_init.c",
                src_dir ++ "win32_monitor.c",
                src_dir ++ "win32_time.c",
                src_dir ++ "win32_joystick.c",
                src_dir ++ "win32_window.c",
            }, &.{"-D_GLFW_WIN32"});
        },
        .macos => {
            exe.linkFramework("IOKit");
            exe.linkFramework("CoreFoundation");
            exe.linkFramework("Metal");
            exe.linkFramework("AppKit");
            exe.linkFramework("CoreServices");
            exe.linkFramework("CoreGraphics");
            exe.linkFramework("Foundation");
            exe.linkSystemLibraryName("objc");
            exe.addCSourceFiles(&.{
                src_dir ++ "monitor.c",
                src_dir ++ "init.c",
                src_dir ++ "vulkan.c",
                src_dir ++ "input.c",
                src_dir ++ "context.c",
                src_dir ++ "window.c",
                src_dir ++ "osmesa_context.c",
                src_dir ++ "egl_context.c",
                src_dir ++ "nsgl_context.m",
                src_dir ++ "posix_thread.c",
                src_dir ++ "cocoa_time.c",
                src_dir ++ "cocoa_joystick.m",
                src_dir ++ "cocoa_init.m",
                src_dir ++ "cocoa_window.m",
                src_dir ++ "cocoa_monitor.m",
            }, &.{"-D_GLFW_COCOA"});
        },
        else => {
            // We assume Linux (X11)
            exe.linkSystemLibraryName("X11");
            exe.linkSystemLibraryName("xcb");
            exe.linkSystemLibraryName("Xau");
            exe.linkSystemLibraryName("Xdmcp");
            exe.addCSourceFiles(&.{
                src_dir ++ "monitor.c",
                src_dir ++ "init.c",
                src_dir ++ "vulkan.c",
                src_dir ++ "input.c",
                src_dir ++ "context.c",
                src_dir ++ "window.c",
                src_dir ++ "osmesa_context.c",
                src_dir ++ "egl_context.c",
                src_dir ++ "glx_context.c",
                src_dir ++ "posix_time.c",
                src_dir ++ "posix_thread.c",
                src_dir ++ "linux_joystick.c",
                src_dir ++ "xkb_unicode.c",
                src_dir ++ "x11_init.c",
                src_dir ++ "x11_window.c",
                src_dir ++ "x11_monitor.c",
            }, &.{"-D_GLFW_X11"});
        },
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
