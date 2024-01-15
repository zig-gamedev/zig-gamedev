const std = @import("std");
const system_sdk = @import("system_sdk");

pub const path = getPath();

inline fn getPath() []const u8 {
    return std.fs.path.dirname(@src().file) orelse unreachable;
}

pub const Package = struct {
    zglfw: *std.Build.Module,
    zglfw_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zglfw", pkg.zglfw);

        const host = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        switch (host.os.tag) {
            .windows => {},
            .macos => {
                exe.addLibraryPath(.{ .path = system_sdk.path ++ "/macos12/usr/lib" });
            },
            else => {
                // We assume Linux (X11)
                if (host.cpu.arch.isX86()) {
                    exe.addLibraryPath(.{ .path = system_sdk.path ++ "/linux/lib/x86_64-linux-gnu" });
                } else {
                    exe.addLibraryPath(.{ .path = system_sdk.path ++ "/linux/lib/aarch64-linux-gnu" });
                }
            },
        }

        if (pkg.zglfw_c_cpp.linkage) |linkage| {
            if (exe.target.isWindows() and linkage == .dynamic) {
                exe.defineCMacro("GLFW_DLL", null);
            }
        }

        exe.linkLibrary(pkg.zglfw_c_cpp);
    }
};

pub const Options = struct {
    shared: bool = false,
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "shared", args.options.shared);

    const zglfw = b.addModule("zglfw", .{
        .source_file = .{ .path = path ++ "/src/zglfw.zig" },
    });

    const zglfw_c_cpp = if (args.options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "libglfw",
            .target = target,
            .optimize = optimize,
        });

        if (target.isWindows()) {
            lib.defineCMacro("_GLFW_BUILD_DLL", null);
        }

        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "libglfw",
        .target = target,
        .optimize = optimize,
    });

    zglfw_c_cpp.addIncludePath(.{ .path = path ++ "/libs/glfw/include" });
    zglfw_c_cpp.linkLibC();

    const host = (std.zig.system.NativeTargetInfo.detect(zglfw_c_cpp.target) catch unreachable).target;

    const src_dir = path ++ "/libs/glfw/src/";

    switch (host.os.tag) {
        .windows => {
            zglfw_c_cpp.linkSystemLibraryName("gdi32");
            zglfw_c_cpp.linkSystemLibraryName("user32");
            zglfw_c_cpp.linkSystemLibraryName("shell32");
            zglfw_c_cpp.addCSourceFiles(.{
                .files = &.{
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
                },
                .flags = &.{"-D_GLFW_WIN32"},
            });
        },
        .macos => {
            zglfw_c_cpp.addFrameworkPath(
                .{ .path = system_sdk.path ++ "/macos12/System/Library/Frameworks" },
            );
            zglfw_c_cpp.addSystemIncludePath(.{ .path = system_sdk.path ++ "/macos12/usr/include" });
            zglfw_c_cpp.addLibraryPath(.{ .path = system_sdk.path ++ "/macos12/usr/lib" });
            zglfw_c_cpp.linkSystemLibraryName("objc");
            zglfw_c_cpp.linkFramework("IOKit");
            zglfw_c_cpp.linkFramework("CoreFoundation");
            zglfw_c_cpp.linkFramework("Metal");
            zglfw_c_cpp.linkFramework("AppKit");
            zglfw_c_cpp.linkFramework("CoreServices");
            zglfw_c_cpp.linkFramework("CoreGraphics");
            zglfw_c_cpp.linkFramework("Foundation");
            zglfw_c_cpp.addCSourceFiles(.{
                .files = &.{
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
                },
                .flags = &.{"-D_GLFW_COCOA"},
            });
        },
        else => {
            // We assume Linux (X11)
            zglfw_c_cpp.addSystemIncludePath(.{ .path = system_sdk.path ++ "/linux/include" });
            if (host.cpu.arch.isX86()) {
                zglfw_c_cpp.addLibraryPath(.{ .path = system_sdk.path ++ "/linux/lib/x86_64-linux-gnu" });
            } else {
                zglfw_c_cpp.addLibraryPath(.{ .path = system_sdk.path ++ "/linux/lib/aarch64-linux-gnu" });
            }
            zglfw_c_cpp.linkSystemLibraryName("X11");
            zglfw_c_cpp.addCSourceFiles(.{
                .files = &.{
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
                },
                .flags = &.{"-D_GLFW_X11"},
            });
        },
    }

    return .{
        .zglfw = zglfw,
        .zglfw_c_cpp = zglfw_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zglfw tests");
    test_step.dependOn(runTests(b, optimize, target));

    const pkg = package(b, target, optimize, .{});
    b.installArtifact(pkg.zglfw_c_cpp);
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zglfw-tests",
        .root_source_file = .{ .path = path ++ "/src/zglfw.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zglfw_pkg = package(b, target, optimize, .{});
    zglfw_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}
