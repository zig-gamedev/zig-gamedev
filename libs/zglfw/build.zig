const std = @import("std");

pub const Package = struct {
    zglfw: *std.Build.Module,
    zglfw_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zglfw", pkg.zglfw);

        const b = exe.step.owner;
        const target = exe.rootModuleTarget();

        const system_sdk = b.dependency("system_sdk", .{});

        switch (target.os.tag) {
            .windows => {},
            .macos => {
                exe.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    exe.addLibraryPath(.{ .path = system_sdk.path("linux/lib/x86_64-linux-gnu").getPath(b) });
                } else {
                    exe.addLibraryPath(.{ .path = system_sdk.path("linux/lib/aarch64-linux-gnu").getPath(b) });
                }
            },
            else => {},
        }

        switch (target.os.tag) {
            .windows => {
                if (pkg.zglfw_c_cpp.linkage) |linkage| {
                    if (linkage == .dynamic) {
                        exe.defineCMacro("GLFW_DLL", null);
                    }
                }
            },
            else => {},
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

    zglfw_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/glfw/include" });
    zglfw_c_cpp.linkLibC();

    const system_sdk = b.dependency("system_sdk", .{});

    const src_dir = thisDir() ++ "/libs/glfw/src/";
    switch (target.result.os.tag) {
        .windows => {
            zglfw_c_cpp.linkSystemLibrary("gdi32");
            zglfw_c_cpp.linkSystemLibrary("user32");
            zglfw_c_cpp.linkSystemLibrary("shell32");
            zglfw_c_cpp.addCSourceFiles(.{
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
            zglfw_c_cpp.addFrameworkPath(
                .{ .path = system_sdk.path("macos12/System/Library/Frameworks").getPath(b) },
            );
            zglfw_c_cpp.addSystemIncludePath(.{
                .path = system_sdk.path("macos12/usr/include").getPath(b),
            });
            zglfw_c_cpp.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });
            zglfw_c_cpp.linkSystemLibrary("objc");
            zglfw_c_cpp.linkFramework("IOKit");
            zglfw_c_cpp.linkFramework("CoreFoundation");
            zglfw_c_cpp.linkFramework("Metal");
            zglfw_c_cpp.linkFramework("AppKit");
            zglfw_c_cpp.linkFramework("CoreServices");
            zglfw_c_cpp.linkFramework("CoreGraphics");
            zglfw_c_cpp.linkFramework("Foundation");
            zglfw_c_cpp.addCSourceFiles(.{
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
            zglfw_c_cpp.addSystemIncludePath(.{
                .path = system_sdk.path("linux/include").getPath(b),
            });
            if (target.result.cpu.arch.isX86()) {
                zglfw_c_cpp.addLibraryPath(.{
                    .path = system_sdk.path("linux/lib/x86_64-linux-gnu").getPath(b),
                });
            } else {
                zglfw_c_cpp.addLibraryPath(.{
                    .path = system_sdk.path("linux/lib/aarch64-linux-gnu").getPath(b),
                });
            }
            zglfw_c_cpp.linkSystemLibrary("X11");
            zglfw_c_cpp.addCSourceFiles(.{
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
                    src_dir ++ "posix_poll.c",
                    src_dir ++ "egl_context.c",
                    src_dir ++ "glx_context.c",
                    src_dir ++ "linux_joystick.c",
                    src_dir ++ "xkb_unicode.c",
                    src_dir ++ "x11_init.c",
                    src_dir ++ "x11_window.c",
                    src_dir ++ "x11_monitor.c",
                },
                .flags = &.{"-D_GLFW_X11"},
            });
        },
        else => {},
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

    tests.addIncludePath(.{ .path = thisDir() ++ "/libs/glfw/include" });
    switch (target.result.os.tag) {
        .linux => {
            tests.addSystemIncludePath(.{
                .path = b.dependency("system_sdk", .{}).path("linux/include").getPath(b),
            });
        },
        else => {},
    }

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
