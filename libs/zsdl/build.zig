const std = @import("std");
const assert = std.debug.assert;

pub const Options = struct {
    enable_ttf: bool = false,
};

pub const Package = struct {
    options: Options,
    zsdl: *std.Build.Module,
    zsdl_options: *std.Build.Module,
    install: *std.Build.Step,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibC();

        exe.addModule("zsdl", pkg.zsdl);

        exe.step.dependOn(pkg.install);

        const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        switch (target.os.tag) {
            .windows => {
                assert(target.cpu.arch.isX86());

                exe.addLibraryPath(.{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/lib" });
                exe.linkSystemLibraryName("SDL2");
                exe.linkSystemLibraryName("SDL2main");

                if (pkg.options.enable_ttf) {
                    exe.linkSystemLibraryName("SDL2_ttf");
                }
            },
            .linux => {
                assert(target.cpu.arch.isX86());

                exe.addLibraryPath(.{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib" });
                exe.linkSystemLibraryName("SDL2-2.0");
                exe.addRPath(.{ .path = "$ORIGIN" });

                if (pkg.options.enable_ttf) {
                    exe.linkSystemLibraryName("SDL2_ttf-2.0");
                }
            },
            .macos => {
                exe.addFrameworkPath(.{ .path = thisDir() ++ "/libs/macos/Frameworks" });
                exe.linkFramework("SDL2");
                exe.addRPath(.{ .path = "@executable_path/Frameworks" });

                if (pkg.options.enable_ttf) {
                    exe.linkFramework("SDL2_ttf");
                }
            },
            else => unreachable,
        }
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const options_step = b.addOptions();
    inline for (std.meta.fields(Options)) |option_field| {
        const option_val = @field(args.options, option_field.name);
        options_step.addOption(@TypeOf(option_val), option_field.name, option_val);
    }

    const options = options_step.createModule();

    const zsdl = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        .dependencies = &.{
            .{ .name = "zsdl_options", .module = options },
        },
    });

    const install_step = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_step.* = std.Build.Step.init(.{ .id = .custom, .name = "zsdl-install", .owner = b });

    if (target.isWindows()) {
        install_step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                "bin/SDL2.dll",
            ).step,
        );
        if (args.options.enable_ttf) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2_ttf.dll" },
                    "bin/SDL2_ttf.dll",
                ).step,
            );
        }
    } else if (target.isLinux()) {
        install_step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                "bin/libSDL2-2.0.so.0",
            ).step,
        );
        if (args.options.enable_ttf) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2_ttf-2.0.so" },
                    "bin/libSDL2_ttf-2.0.so.0",
                ).step,
            );
        }
    } else if (target.isDarwin()) {
        install_step.dependOn(
            &b.addInstallDirectory(.{
                .source_dir = .{ .path = thisDir() ++ "/libs/macos/Frameworks/SDL2.framework" },
                .install_dir = .{ .custom = "" },
                .install_subdir = "bin/Frameworks/SDL2.framework",
            }).step,
        );
        if (args.options.enable_ttf) {
            install_step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{ .path = thisDir() ++ "/libs/macos/Frameworks/SDL2_ttf.framework" },
                    .install_dir = .{ .custom = "" },
                    .install_subdir = "bin/Frameworks/SDL2_ttf.framework",
                }).step,
            );
        }
    } else unreachable;

    return .{
        .options = args.options,
        .zsdl = zsdl,
        .zsdl_options = options,
        .install = install_step,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zsdl tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zsdl-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zsdl_pkg = package(b, target, optimize, .{
        .options = .{
            .enable_ttf = true,
        },
    });
    tests.addModule("zsdl_options", zsdl_pkg.zsdl_options);
    zsdl_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
