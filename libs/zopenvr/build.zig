const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const mod = b.addModule("root", .{
        .root_source_file = b.path("src/openvr.zig"),
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    var backends = struct {
        d3d11: bool = false,
        d3d12: bool = false,
        // valken: bool = false,
        opengl: bool = false,
    }{};

    var need_zwin32 = false;
    if (b.option(bool, "d3d12", "Enable Direct3D 12 backend") orelse false) {
        backends.d3d12 = true;
        need_zwin32 = true;
    }
    if (b.option(bool, "d3d11", "Enable Direct3D 11 backend") orelse false) {
        backends.d3d11 = true;
        need_zwin32 = true;
    }
    if (need_zwin32) {
        if (b.lazyDependency("zwin32", .{ .target = target })) |zwin32| {
            mod.addImport("zwin32", zwin32.module("root"));
        }
    }
    if (b.option(bool, "opengl", "Enable OpenGL backend") orelse false) {
        backends.opengl = true;
        if (b.lazyDependency("zopengl", .{})) |zopengl| {
            mod.addImport("zopengl", zopengl.module("root"));
        }
    }

    const options = b.addOptions();
    inline for (@typeInfo(@TypeOf(backends)).Struct.fields) |field| {
        options.addOption(field.type, field.name, @field(backends, field.name));
    }
    mod.addOptions("rendermodesConfig", options);

    {
        const unit_tests = b.step("test", "Run zopenvr tests");
        {
            const tests = b.addTest(.{
                .name = "openvr-tests",
                .root_source_file = b.path("src/tests.zig"),
                .target = target,
                .optimize = optimize,
            });
            tests.root_module.addImport("openvr", mod);
            addLibraryPathsTo(tests);
            addRPathsTo(tests);
            linkOpenVR(tests);
            b.installArtifact(tests);

            const tests_exe = b.addRunArtifact(tests);
            if (target.result.os.tag == .windows) {
                tests_exe.setCwd(.{
                    .cwd_relative = b.getInstallPath(.bin, ""),
                });
            }
            unit_tests.dependOn(&tests_exe.step);
        }

        installOpenVR(unit_tests, target.result, .bin);
    }
}

fn testSuportedTarget(step: *std.Build.Step, target: std.Target) error{MakeFailed}!void {
    const supportedArch = switch (target.cpu.arch) {
        .x86, .x86_64 => true,
        else => false,
    };
    const supportedOs = switch (target.os.tag) {
        .windows, .linux => true,
        else => false,
    };
    if (supportedOs and supportedArch) return;

    if (step.fail("zopenvr does not support building for {s}{s}{s}{s}{s}", .{
        if (!supportedArch and supportedOs) "the " else "",
        if (!supportedArch) @tagName(target.cpu.arch) else "",
        if (!supportedArch and supportedOs) " platform" else "",
        if (!supportedArch and !supportedOs) " " else "",
        if (!supportedOs) @tagName(target.os.tag) else "",
    }) == error.OutOfMemory) @panic("OOM");
    return error.MakeFailed;
}

pub fn addLibraryPathsTo(compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";

    testSuportedTarget(&compile_step.step, target) catch return;

    const arch = target.cpu.arch;
    const path: []const u8 = switch (target.os.tag) {
        .windows => switch (arch) {
            .x86_64 => "libs/openvr/lib/win64",
            .x86 => "libs/openvr/lib/win32",
            else => unreachable,
        },
        .linux => switch (arch) {
            .x86_64 => "libs/openvr/bin/linux64",
            .x86 => "libs/openvr/bin/linux32",
            else => unreachable,
        },
        else => unreachable,
    };

    compile_step.addLibraryPath(.{
        .cwd_relative = b.pathJoin(&.{ source_path_prefix, path }),
    });
}

pub fn addRPathsTo(compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";

    testSuportedTarget(&compile_step.step, target) catch return;

    const arch = target.cpu.arch;
    const path: []const u8 = switch (target.os.tag) {
        .windows => switch (arch) {
            .x86_64 => "libs/openvr/bin/win64",
            .x86 => "libs/openvr/bin/win32",
            else => unreachable,
        },
        .linux => switch (arch) {
            .x86_64 => "libs/openvr/bin/linux64",
            .x86 => "libs/openvr/bin/linux32",
            else => unreachable,
        },
        else => unreachable,
    };

    compile_step.addRPath(.{
        .cwd_relative = b.pathJoin(&.{ source_path_prefix, path }),
    });
}

pub fn linkOpenVR(compile_step: *std.Build.Step.Compile) void {
    testSuportedTarget(&compile_step.step, compile_step.rootModuleTarget()) catch return;

    switch (compile_step.rootModuleTarget().os.tag) {
        .windows => compile_step.linkSystemLibrary("openvr_api"),
        .linux => {
            compile_step.root_module.linkSystemLibrary("openvr_api", .{ .needed = true });
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        else => unreachable,
    }
}

pub fn installOpenVR(
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    testSuportedTarget(step, target) catch return;

    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";

    const arch = target.cpu.arch;
    const path: []const u8 = switch (target.os.tag) {
        .windows => switch (arch) {
            .x86_64 => "libs/openvr/bin/win64/openvr_api.dll",
            .x86 => "libs/openvr/bin/win32/openvr_api.dll",
            else => unreachable,
        },
        .linux => switch (arch) {
            .x86_64 => "libs/openvr/bin/linux64/libopenvr_api.so",
            .x86 => "libs/openvr/bin/linux32/libopenvr_api.so",
            else => unreachable,
        },
        else => unreachable,
    };

    step.dependOn(switch (target.os.tag) {
        .windows => &b.addInstallFileWithDir(
            .{ .cwd_relative = b.pathJoin(&.{ source_path_prefix, path }) },
            install_dir,
            "openvr_api.dll",
        ).step,
        .linux => &b.addInstallFileWithDir(
            .{ .cwd_relative = b.pathJoin(&.{ source_path_prefix, path }) },
            install_dir,
            "libopenvr_api.so",
        ).step,
        else => unreachable,
    });
}
