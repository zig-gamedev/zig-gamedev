const std = @import("std");

pub const Package = struct {
    zglfw: *std.Build.Module,
    zglfw_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        _: struct {},
    ) Package {
        const zglfw = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zglfw.zig" },
        });

        const zglfw_c_cpp = b.addStaticLibrary(.{
            .name = "zglfw",
            .target = target,
            .optimize = optimize,
        });

        zglfw_c_cpp.addIncludePath(thisDir() ++ "/libs/glfw/include");
        zglfw_c_cpp.linkLibC();

        const host = (std.zig.system.NativeTargetInfo.detect(zglfw_c_cpp.target) catch unreachable).target;

        const src_dir = thisDir() ++ "/libs/glfw/src/";

        switch (host.os.tag) {
            .windows => {
                zglfw_c_cpp.linkSystemLibraryName("gdi32");
                zglfw_c_cpp.linkSystemLibraryName("user32");
                zglfw_c_cpp.linkSystemLibraryName("shell32");
                zglfw_c_cpp.addCSourceFiles(&.{
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
                zglfw_c_cpp.addFrameworkPath(thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks");
                zglfw_c_cpp.addSystemIncludePath(thisDir() ++ "/../system-sdk/macos12/usr/include");
                zglfw_c_cpp.addLibraryPath(thisDir() ++ "/../system-sdk/macos12/usr/lib");
                zglfw_c_cpp.linkSystemLibraryName("objc");
                zglfw_c_cpp.linkFramework("IOKit");
                zglfw_c_cpp.linkFramework("CoreFoundation");
                zglfw_c_cpp.linkFramework("Metal");
                zglfw_c_cpp.linkFramework("AppKit");
                zglfw_c_cpp.linkFramework("CoreServices");
                zglfw_c_cpp.linkFramework("CoreGraphics");
                zglfw_c_cpp.linkFramework("Foundation");
                zglfw_c_cpp.addCSourceFiles(&.{
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
                zglfw_c_cpp.addSystemIncludePath(thisDir() ++ "/../system-sdk/linux/include");
                if (host.cpu.arch.isX86()) {
                    zglfw_c_cpp.addLibraryPath(thisDir() ++ "/../system-sdk/linux/lib/x86_64-linux-gnu");
                } else {
                    zglfw_c_cpp.addLibraryPath(thisDir() ++ "/../system-sdk/linux/lib/aarch64-linux-gnu");
                }
                zglfw_c_cpp.linkSystemLibraryName("X11");
                zglfw_c_cpp.addCSourceFiles(&.{
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

        return .{
            .zglfw = zglfw,
            .zglfw_c_cpp = zglfw_c_cpp,
        };
    }

    pub fn link(zglfw_pkg: Package, exe: *std.Build.CompileStep) void {
        const host = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;
        switch (host.os.tag) {
            .windows => {},
            .macos => {
                exe.addLibraryPath(thisDir() ++ "/../system-sdk/macos12/usr/lib");
            },
            else => {
                // We assume Linux (X11)
                if (host.cpu.arch.isX86()) {
                    exe.addLibraryPath(thisDir() ++ "/../system-sdk/linux/lib/x86_64-linux-gnu");
                } else {
                    exe.addLibraryPath(thisDir() ++ "/../system-sdk/linux/lib/aarch64-linux-gnu");
                }
            },
        }
        exe.linkLibrary(zglfw_pkg.zglfw_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zglfw.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zglfw_pkg = Package.build(b, target, optimize, .{});
    zglfw_pkg.link(tests);

    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
