const std = @import("std");

const BuildOptions = struct {
    enable_fibers: bool = false,
};

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return std.build.Pkg{
        .name = "ztracy",
        .path = .{ .path = thisDir() ++ "/src/ztracy.zig" },
        .dependencies = dependencies,
    };
}

pub fn link(
    exe: *std.build.LibExeObjStep,
    ztracy_enable: bool,
    build_options: BuildOptions,
) void {
    if (ztracy_enable) {
        const enable_fibers = if (build_options.enable_fibers) "-DTRACY_FIBERS" else "";

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
