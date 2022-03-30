const std = @import("std");
const Builder = std.build.Builder;
const LibExeObjStep = std.build.LibExeObjStep;

/// Build required sources, use tracy by importing "tracy.zig"
pub fn link(step: *LibExeObjStep, opt_path: ?[]const u8) void {
    if (opt_path) |path| {
        step.addIncludePath(path);
        const tracy_client_source_path = std.fs.path.join(step.builder.allocator, &.{ path, "TracyClient.cpp" }) catch unreachable;
        step.addCSourceFile(tracy_client_source_path, &[_][]const u8{
            "-DTRACY_ENABLE",
            // MinGW doesn't have all the newfangled windows features,
            // so we need to pretend to have an older windows version.
            "-D_WIN32_WINNT=0x601",
            "-fno-sanitize=undefined",
        });

        step.linkLibC();
        step.linkSystemLibrary("c++");

        if (step.target.isWindows()) {
            step.linkSystemLibrary("Advapi32");
            step.linkSystemLibrary("User32");
            step.linkSystemLibrary("Ws2_32");
            step.linkSystemLibrary("DbgHelp");
        }
    }
}
