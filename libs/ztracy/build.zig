const std = @import("std");

pub const BuildOptions = struct {
    enable_ztracy: bool = false,
    enable_fibers: bool = false,
};

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.build.OptionsStep,

    pub fn init(b: *std.build.Builder, options: BuildOptions) BuildOptionsStep {
        const bos = .{
            .options = options,
            .step = b.addOptions(),
        };
        bos.step.addOption(bool, "enable_ztracy", bos.options.enable_ztracy);
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.build.Pkg {
        return bos.step.getPackage("ztracy_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.build.LibExeObjStep) void {
        target_step.addOptions("ztracy_options", bos.step);
    }
};

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "ztracy",
        .source = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .dependencies = dependencies,
    };
}

pub fn link(exe: *std.build.LibExeObjStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);
    if (bos.options.enable_ztracy) {
        const enable_fibers = if (bos.options.enable_fibers) "-DTRACY_FIBERS" else "";

        exe.addIncludeDir(thisDir() ++ "/libs/tracy");
        exe.addCSourceFile(thisDir() ++ "/libs/tracy/TracyClient.cpp", &.{
            "-DTRACY_ENABLE",
            enable_fibers,
            // MinGW doesn't have all the newfangled windows features,
            // so we need to pretend to have an older windows version.
            "-D_WIN32_WINNT=0x601",
            "-fno-sanitize=undefined",
        });

        exe.linkSystemLibrary("c");
        exe.linkSystemLibrary("c++");

        if (exe.target.isWindows()) {
            exe.linkSystemLibrary("Advapi32");
            exe.linkSystemLibrary("User32");
            exe.linkSystemLibrary("Ws2_32");
            exe.linkSystemLibrary("DbgHelp");
        }
    }
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
