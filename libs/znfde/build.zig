const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const system_sdk = b.dependency("system_sdk", .{});

    const options = .{
        .with_portal = b.option(
            bool,
            "with_portal",
            "Use xdg-desktop-portal instead of GTK",
        ) orelse true,
    };

    const znfde = b.addModule("root", .{
        .root_source_file = .{ .path = "src/znfde.zig" },
    });

    const nfde = b.addStaticLibrary(.{
        .name = "nfde",
        .target = target,
        .optimize = optimize,
    });

    const cflags = [_][]const u8{};

    nfde.addIncludePath(.{ .path = "nativefiledialog/src/include" });
    znfde.addIncludePath(.{ .path = "nativefiledialog/src/include" });

    switch (nfde.rootModuleTarget().os.tag) {
        .windows => {
            nfde.addCSourceFile(.{ .file = .{ .path = "nativefiledialog/src/nfd_win.cpp" }, .flags = &cflags });
            nfde.linkSystemLibrary("shell32");
            nfde.linkSystemLibrary("ole32");
            nfde.linkSystemLibrary("uuid");
        },
        .macos => {
            nfde.defineCMacro("NFD_MACOS_ALLOWEDCONTENTTYPES", "1");
            nfde.addCSourceFile(.{ .file = .{ .path = "nativefiledialog/src/nfd_cocoa.m" }, .flags = &cflags });
            nfde.linkFramework("AppKit");
            nfde.linkFramework("UniformTypeIdentifiers");
        },
        else => {
            @import("system_sdk").addLibraryPathsTo(nfde);

            if (options.with_portal) {
                znfde.addSystemIncludePath(.{ .path = "includes" });
                nfde.addSystemIncludePath(.{ .path = "includes" });
                nfde.addSystemIncludePath(.{ .path = system_sdk.path("linux/include").getPath(b) });
                nfde.addCSourceFile(.{ .file = .{ .path = "nativefiledialog/src/nfd_portal.cpp" }, .flags = &cflags });
                nfde.linkSystemLibrary("dbus-1");
            } else {
                nfde.addCSourceFile(.{ .file = .{ .path = "nativefiledialog/src/nfd_gtk.cpp" }, .flags = &cflags });
                nfde.linkSystemLibrary("atk-1.0");
                nfde.linkSystemLibrary("gdk-3");
                nfde.linkSystemLibrary("gtk-3");
                nfde.linkSystemLibrary("glib-2.0");
                nfde.linkSystemLibrary("gobject-2.0");
            }
            nfde.linkLibCpp();
        },
    }

    b.installArtifact(nfde);

    const test_step = b.step("test", "Run znfde tests");
    const tests = b.addTest(.{
        .name = "znfde-tests",
        .root_source_file = .{ .path = "src/znfde.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);
    tests.linkLibrary(nfde);
    switch (nfde.rootModuleTarget().os.tag) {
        .linux => @import("system_sdk").addLibraryPathsTo(tests),
        else => {},
    }
    test_step.dependOn(&b.addRunArtifact(tests).step);
}
