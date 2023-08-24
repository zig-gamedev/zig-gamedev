const std = @import("std");

pub const Options = struct {
    enable_ztracy: bool = false,
    enable_fibers: bool = false,
};

pub const Package = struct {
    options: Options,
    ztracy: *std.Build.Module,
    ztracy_options: *std.Build.Module,
    ztracy_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("ztracy", pkg.ztracy);
        exe.addModule("ztracy_options", pkg.ztracy_options);
        if (pkg.options.enable_ztracy) {
            exe.addIncludePath(.{ .path = thisDir() ++ "/libs/tracy/tracy" });
            exe.linkLibrary(pkg.ztracy_c_cpp);
        }
    }
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
    step.addOption(bool, "enable_ztracy", args.options.enable_ztracy);

    const ztracy_options = step.createModule();

    const ztracy = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .dependencies = &.{
            .{ .name = "ztracy_options", .module = ztracy_options },
        },
    });

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
        ztracy_c_cpp.linkLibCpp();

        switch (target.getOs().tag) {
            .windows => {
                ztracy_c_cpp.linkSystemLibraryName("ws2_32");
                ztracy_c_cpp.linkSystemLibraryName("dbghelp");
            },
            .macos => {
                ztracy_c_cpp.addFrameworkPath(
                    .{ .path = thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks" },
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

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
