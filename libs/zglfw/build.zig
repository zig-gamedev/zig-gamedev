const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const system_sdk = b.dependency("system_sdk", .{});

    const options = .{
        .shared = b.option(
            bool,
            "shared",
            "Build GLFW as shared lib",
        ) orelse false,
        .enable_x11 = b.option(
            bool,
            "x11",
            "Whether to build with X11 support (default: true)",
        ) orelse true,
        .enable_wayland = b.option(
            bool,
            "wayland",
            "Whether to build with Wayland support (default: true)",
        ) orelse true,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    _ = b.addModule("root", .{
        .root_source_file = .{ .path = "src/zglfw.zig" },
        .imports = &.{
            .{ .name = "zglfw_options", .module = options_module },
        },
    });

    const glfw = if (options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "glfw",
            .target = target,
            .optimize = optimize,
        });
        if (target.result.os.tag == .windows) {
            lib.defineCMacro("_GLFW_BUILD_DLL", null);
        }
        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "glfw",
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(glfw);

    glfw.addIncludePath(.{ .path = "libs/glfw/include" });
    glfw.linkLibC();

    const src_dir = "libs/glfw/src/";
    switch (target.result.os.tag) {
        .windows => {
            glfw.linkSystemLibrary("gdi32");
            glfw.linkSystemLibrary("user32");
            glfw.linkSystemLibrary("shell32");
            glfw.addCSourceFiles(.{
                .files = &.{
                    src_dir ++ "platform.c",
                    src_dir ++ "monitor.c",
                    src_dir ++ "init.c",
                    src_dir ++ "vulkan.c",
                    src_dir ++ "input.c",
                    src_dir ++ "context.c",
                    src_dir ++ "window.c",
                    src_dir ++ "osmesa_context.c",
                    src_dir ++ "egl_context.c",
                    src_dir ++ "null_init.c",
                    src_dir ++ "null_monitor.c",
                    src_dir ++ "null_window.c",
                    src_dir ++ "null_joystick.c",
                    src_dir ++ "wgl_context.c",
                    src_dir ++ "win32_thread.c",
                    src_dir ++ "win32_init.c",
                    src_dir ++ "win32_monitor.c",
                    src_dir ++ "win32_time.c",
                    src_dir ++ "win32_joystick.c",
                    src_dir ++ "win32_window.c",
                    src_dir ++ "win32_module.c",
                },
                .flags = &.{"-D_GLFW_WIN32"},
            });
        },
        .macos => {
            glfw.addFrameworkPath(
                .{ .path = system_sdk.path("macos12/System/Library/Frameworks").getPath(b) },
            );
            glfw.addSystemIncludePath(.{
                .path = system_sdk.path("macos12/usr/include").getPath(b),
            });
            glfw.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });
            glfw.linkSystemLibrary("objc");
            glfw.linkFramework("IOKit");
            glfw.linkFramework("CoreFoundation");
            glfw.linkFramework("Metal");
            glfw.linkFramework("AppKit");
            glfw.linkFramework("CoreServices");
            glfw.linkFramework("CoreGraphics");
            glfw.linkFramework("Foundation");
            glfw.addCSourceFiles(.{
                .files = &.{
                    src_dir ++ "platform.c",
                    src_dir ++ "monitor.c",
                    src_dir ++ "init.c",
                    src_dir ++ "vulkan.c",
                    src_dir ++ "input.c",
                    src_dir ++ "context.c",
                    src_dir ++ "window.c",
                    src_dir ++ "osmesa_context.c",
                    src_dir ++ "egl_context.c",
                    src_dir ++ "null_init.c",
                    src_dir ++ "null_monitor.c",
                    src_dir ++ "null_window.c",
                    src_dir ++ "null_joystick.c",
                    src_dir ++ "posix_thread.c",
                    src_dir ++ "posix_module.c",
                    src_dir ++ "posix_poll.c",
                    src_dir ++ "nsgl_context.m",
                    src_dir ++ "cocoa_time.c",
                    src_dir ++ "cocoa_joystick.m",
                    src_dir ++ "cocoa_init.m",
                    src_dir ++ "cocoa_window.m",
                    src_dir ++ "cocoa_monitor.m",
                },
                .flags = &.{"-D_GLFW_COCOA"},
            });
        },
        .linux => {
            glfw.addSystemIncludePath(.{
                .path = system_sdk.path("linux/include").getPath(b),
            });
            glfw.addSystemIncludePath(.{
                .path = system_sdk.path("linux/include/wayland").getPath(b),
            });
            glfw.addIncludePath(.{ .path = src_dir ++ "wayland" });

            if (target.result.cpu.arch.isX86()) {
                glfw.addLibraryPath(.{
                    .path = system_sdk.path("linux/lib/x86_64-linux-gnu").getPath(b),
                });
            } else {
                glfw.addLibraryPath(.{
                    .path = system_sdk.path("linux/lib/aarch64-linux-gnu").getPath(b),
                });
            }
            glfw.addCSourceFiles(.{
                .files = &.{
                    src_dir ++ "platform.c",
                    src_dir ++ "monitor.c",
                    src_dir ++ "init.c",
                    src_dir ++ "vulkan.c",
                    src_dir ++ "input.c",
                    src_dir ++ "context.c",
                    src_dir ++ "window.c",
                    src_dir ++ "osmesa_context.c",
                    src_dir ++ "egl_context.c",
                    src_dir ++ "null_init.c",
                    src_dir ++ "null_monitor.c",
                    src_dir ++ "null_window.c",
                    src_dir ++ "null_joystick.c",
                    src_dir ++ "posix_time.c",
                    src_dir ++ "posix_thread.c",
                    src_dir ++ "posix_module.c",
                    src_dir ++ "egl_context.c",
                },
                .flags = &.{},
            });
            if (options.enable_x11 or options.enable_wayland) {
                glfw.addCSourceFiles(.{
                    .files = &.{
                        src_dir ++ "xkb_unicode.c",
                        src_dir ++ "linux_joystick.c",
                        src_dir ++ "posix_poll.c",
                    },
                    .flags = &.{},
                });
            }
            if (options.enable_x11) {
                glfw.addCSourceFiles(.{
                    .files = &.{
                        src_dir ++ "x11_init.c",
                        src_dir ++ "x11_monitor.c",
                        src_dir ++ "x11_window.c",
                        src_dir ++ "glx_context.c",
                    },
                    .flags = &.{},
                });
                glfw.defineCMacro("_GLFW_X11", "1");
                glfw.linkSystemLibrary("X11");
            }
            if (options.enable_wayland) {
                glfw.addCSourceFiles(.{
                    .files = &.{
                        src_dir ++ "wl_init.c",
                        src_dir ++ "wl_monitor.c",
                        src_dir ++ "wl_window.c",
                    },
                    .flags = &.{},
                });
                glfw.defineCMacro("_GLFW_WAYLAND", "1");
            }
        },
        else => {},
    }

    const test_step = b.step("test", "Run zglfw tests");

    const tests = b.addTest(.{
        .name = "zglfw-tests",
        .root_source_file = .{ .path = "src/zglfw.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.root_module.addImport("zglfw_options", options_module);
    b.installArtifact(tests);

    tests.addIncludePath(.{ .path = "libs/glfw/include" });
    switch (target.result.os.tag) {
        .linux => {
            tests.addSystemIncludePath(.{
                .path = system_sdk.path("linux/include").getPath(b),
            });
            if (options.enable_wayland) {
                glfw.addSystemIncludePath(.{
                    .path = system_sdk.path("linux/include/wayland").getPath(b),
                });
            }
        },
        else => {},
    }

    tests.linkLibrary(glfw);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
