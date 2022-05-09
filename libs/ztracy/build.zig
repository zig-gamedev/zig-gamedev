const std = @import("std");

pub fn getPkg(b: *std.build.Builder, options_pkg: std.build.Pkg) std.build.Pkg {
    const pkg = std.build.Pkg{
        .name = "ztracy",
        .path = .{ .path = comptime thisDir() ++ "/src/ztracy.zig" },
        .dependencies = &[_]std.build.Pkg{options_pkg},
    };
    return b.dupePkg(pkg);
}

pub const TracyBuildOptions = struct {
    fibers: bool = false,
};

pub fn link(
    exe: *std.build.LibExeObjStep,
    enable_tracy: bool,
    build_opts: TracyBuildOptions,
) void {
    if (enable_tracy) {
        const fibers_flag = if (build_opts.fibers) "-DTRACY_FIBERS" else "";

        exe.addIncludeDir(comptime thisDir() ++ "/libs/tracy");
        exe.addCSourceFile(comptime thisDir() ++ "/libs/tracy/TracyClient.cpp", &.{
            "-DTRACY_ENABLE",
            fibers_flag,
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
    return std.fs.path.dirname(@src().file) orelse ".";
}
