const std = @import("std");

pub const Package = struct {
    zglfw: *std.Build.Module,
    zglfw_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zglfw", pkg.zglfw);

        const host = exe.rootModuleTarget();

        switch (host.os.tag) {
            .windows => {},
            .macos => {
                exe.root_module.addLibraryPath(.{
                    .path = thisDir() ++ "/../system-sdk/macos12/usr/lib",
                });
            },
            else => {
                // We assume Linux (X11)
                if (host.cpu.arch.isX86()) {
                    exe.root_module.addLibraryPath(.{
                        .path = thisDir() ++ "/../system-sdk/linux/lib/x86_64-linux-gnu",
                    });
                } else {
                    exe.root_module.addLibraryPath(.{
                        .path = thisDir() ++ "/../system-sdk/linux/lib/aarch64-linux-gnu",
                    });
                }
            },
        }

        if (pkg.zglfw_c_cpp.linkage) |linkage| {
            if (host.os.tag == .windows and linkage == .dynamic) {
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
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "shared", args.options.shared);

    const zglfw = b.addModule("zglfw", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zglfw.zig" },
    });

    const zglfw_c_cpp = if (args.options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "libglfw",
            .target = target,
            .optimize = optimize,
        });

        if (target.result.os.tag == .windows) {
            lib.defineCMacro("_GLFW_BUILD_DLL", null);
        }

        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "libglfw",
        .target = target,
        .optimize = optimize,
    });

    zglfw_c_cpp.root_module.addIncludePath(.{
        .path = thisDir() ++ "/libs/glfw/include",
    });
    zglfw_c_cpp.root_module.link_libc = true;

    const host = target.result;

    const src_dir = thisDir() ++ "/libs/glfw/src/";

    switch (host.os.tag) {
        .windows => {
            zglfw_c_cpp.root_module.linkSystemLibrary("gdi32", .{});
            zglfw_c_cpp.root_module.linkSystemLibrary("user32", .{});
            zglfw_c_cpp.root_module.linkSystemLibrary("shell32", .{});
            zglfw_c_cpp.root_module.addCSourceFiles(.{
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
            zglfw_c_cpp.root_module.addFrameworkPath(
                .{ .path = thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks" },
            );
            zglfw_c_cpp.root_module.addSystemIncludePath(.{
                .path = thisDir() ++ "/../system-sdk/macos12/usr/include",
            });
            zglfw_c_cpp.root_module.addLibraryPath(.{
                .path = thisDir() ++ "/../system-sdk/macos12/usr/lib",
            });
            zglfw_c_cpp.root_module.linkSystemLibrary("objc", .{});
            zglfw_c_cpp.root_module.linkFramework("IOKit", .{});
            zglfw_c_cpp.root_module.linkFramework("CoreFoundation", .{});
            zglfw_c_cpp.root_module.linkFramework("Metal", .{});
            zglfw_c_cpp.root_module.linkFramework("AppKit", .{});
            zglfw_c_cpp.root_module.linkFramework("CoreServices", .{});
            zglfw_c_cpp.root_module.linkFramework("CoreGraphics", .{});
            zglfw_c_cpp.root_module.linkFramework("Foundation", .{});
            zglfw_c_cpp.root_module.addCSourceFiles(.{
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
            zglfw_c_cpp.root_module.addSystemIncludePath(.{
                .path = thisDir() ++ "/../system-sdk/linux/include",
            });
            if (host.cpu.arch.isX86()) {
                zglfw_c_cpp.root_module.addLibraryPath(.{
                    .path = thisDir() ++ "/../system-sdk/linux/lib/x86_64-linux-gnu",
                });
            } else {
                zglfw_c_cpp.root_module.addLibraryPath(.{
                    .path = thisDir() ++ "/../system-sdk/linux/lib/aarch64-linux-gnu",
                });
            }
            zglfw_c_cpp.root_module.linkSystemLibrary("X11", .{});
            zglfw_c_cpp.root_module.addCSourceFiles(.{
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
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zglfw-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zglfw.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zglfw_pkg = package(b, target, optimize, .{});
    zglfw_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
