const std = @import("std");

pub const BuildOptions = struct {
    enable_ztracy: bool = false,
    enable_fibers: bool = false,
};

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.Build.OptionsStep,

    pub fn init(b: *std.Build, options: BuildOptions) BuildOptionsStep {
        const bos = .{
            .options = options,
            .step = b.addOptions(),
        };
        bos.step.addOption(bool, "enable_ztracy", bos.options.enable_ztracy);
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.Build.Pkg {
        return bos.step.getPackage("ztracy_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.Build.CompileStep) void {
        target_step.addOptions("ztracy_options", bos.step);
    }
};

pub fn getPkg(dependencies: []const std.Build.Pkg) std.Build.Pkg {
    return .{
        .name = "ztracy",
        .source = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .dependencies = dependencies,
    };
}

pub fn link(exe: *std.Build.CompileStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);
    if (bos.options.enable_ztracy) {
        const enable_fibers = if (bos.options.enable_fibers) "-DTRACY_FIBERS" else "";

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
