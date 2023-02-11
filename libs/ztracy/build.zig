const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        enable_ztracy: bool = false,
        enable_fibers: bool = false,
    };

    options: Options,
    ztracy: *std.Build.Module,
    ztracy_options: *std.Build.Module,

    pub fn build(
        b: *std.Build,
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

        return .{
            .options = args.options,
            .ztracy = ztracy,
            .ztracy_options = ztracy_options,
        };
    }

    pub fn link(ztracy_pkg: Package, exe: *std.Build.CompileStep) void {
        if (ztracy_pkg.options.enable_ztracy) {
            const enable_fibers = if (ztracy_pkg.options.enable_fibers) "-DTRACY_FIBERS" else "";

            exe.addIncludePath(thisDir() ++ "/libs/tracy/tracy");
            exe.addCSourceFile(thisDir() ++ "/libs/tracy/TracyClient.cpp", &.{
                "-DTRACY_ENABLE",
                enable_fibers,
                // MinGW doesn't have all the newfangled windows features,
                // so we need to pretend to have an older windows version.
                "-D_WIN32_WINNT=0x601",
                "-fno-sanitize=undefined",
            });

            exe.linkLibC();
            exe.linkLibCpp();

            if (exe.target.isWindows()) {
                exe.linkSystemLibraryName("ws2_32");
                exe.linkSystemLibraryName("dbghelp");
            }
        }
    }
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
