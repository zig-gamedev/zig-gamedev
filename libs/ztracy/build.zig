const std = @import("std");

pub const Options = struct {
    enable_ztracy: bool = false,
    enable_fibers: bool = false,
};

pub const Package = struct {
    options: Options,
    ztracy: *std.Build.Module,
    ztracy_options: *std.Build.Module,
    ztracy_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("ztracy", pkg.ztracy);
        exe.root_module.addImport("ztracy_options", pkg.ztracy_options);
        if (pkg.options.enable_ztracy) {
            exe.addIncludePath(.{ .path = thisDir() ++ "/libs/tracy/tracy" });
            exe.linkLibrary(pkg.ztracy_c_cpp);
        }
    }
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
    step.addOption(bool, "enable_ztracy", args.options.enable_ztracy);

    const ztracy_options = step.createModule();

    const ztracy = b.addModule("ztracy", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .imports = &.{
            .{ .name = "ztracy_options", .module = ztracy_options },
        },
    });

    ztracy.addIncludePath(.{ .path = thisDir() ++ "/libs/tracy/tracy" });

    const ztracy_c_cpp = if (args.options.enable_ztracy) ztracy_c_cpp: {
        const enable_fibers = if (args.options.enable_fibers) "-DTRACY_FIBERS" else "";

        const ztracy_c_cpp = b.addStaticLibrary(.{
            .name = "ztracy",
            .target = target,
            .optimize = optimize,
        });

        ztracy_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/tracy/tracy" });
        ztracy_c_cpp.addCSourceFile(.{
            .file = .{ .path = thisDir() ++ "/libs/tracy/TracyClient.cpp" },
            .flags = &.{
                "-DTRACY_ENABLE",
                enable_fibers,
                // MinGW doesn't have all the newfangled windows features,
                // so we need to pretend to have an older windows version.
                "-D_WIN32_WINNT=0x601",
                "-fno-sanitize=undefined",
            },
        });

        ztracy_c_cpp.linkLibC();
        if (target.result.abi != .msvc)
            ztracy_c_cpp.linkLibCpp();

        switch (target.result.os.tag) {
            .windows => {
                ztracy_c_cpp.linkSystemLibrary("ws2_32");
                ztracy_c_cpp.linkSystemLibrary("dbghelp");
            },
            .macos => {
                const system_sdk = b.dependency("system_sdk", .{});
                ztracy_c_cpp.addFrameworkPath(
                    .{ .path = system_sdk.path("System/Library/Frameworks").getPath(b) },
                );
            },
            else => {},
        }

        break :ztracy_c_cpp ztracy_c_cpp;
    } else undefined;

    return .{
        .options = args.options,
        .ztracy = ztracy,
        .ztracy_options = ztracy_options,
        .ztracy_c_cpp = ztracy_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = package(b, target, optimize, .{
        .options = .{
            .enable_ztracy = b.option(
                bool,
                "enable_ztracy",
                "Enable Tracy profile markers",
            ) orelse false,
            .enable_fibers = b.option(
                bool,
                "enable_fibers",
                "Enable Tracy fiber support",
            ) orelse false,
        },
    });

    const test_step = b.step("test", "Run ztracy tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "ztracy-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .target = target,
        .optimize = optimize,
    });
    const pkg = package(b, target, optimize, .{});
    pkg.link(tests);
    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
