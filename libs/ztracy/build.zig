const std = @import("std");

pub const Options = struct {
    enable_ztracy: bool = false,
    enable_fibers: bool = false,
};

pub const Package = struct {
    module: *std.Build.Module,
    options: Options,
    options_module: *std.Build.Module,
};

pub fn package(
    b: *std.Build,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "enable_ztracy", args.options.enable_ztracy);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .dependencies = &.{
            .{ .name = "ztracy_options", .module = options_module },
        },
    });

    return .{
        .module = module,
        .options = args.options,
        .options_module = options_module,
    };
}

pub fn build(_: *std.Build) void {}

pub fn link(exe: *std.Build.CompileStep, options: Options) void {
    if (options.enable_ztracy) {
        const enable_fibers = if (options.enable_fibers) "-DTRACY_FIBERS" else "";

        exe.addIncludePath(thisDir() ++ "/libs/tracy/tracy");
        exe.addCSourceFile(thisDir() ++ "/libs/tracy/TracyClient.cpp", &.{
            "-DTRACY_ENABLE",
            enable_fibers,
            // MinGW doesn't have all the newfangled windows features,
            // so we need to pretend to have an older windows version.
            "-D_WIN32_WINNT=0x601",
            "-fno-sanitize=undefined",
        });

        exe.linkSystemLibraryName("c");
        exe.linkSystemLibraryName("c++");

        if (exe.target.isWindows()) {
            exe.linkSystemLibraryName("advapi32");
            exe.linkSystemLibraryName("user32");
            exe.linkSystemLibraryName("ws2_32");
            exe.linkSystemLibraryName("dbghelp");
        }
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
