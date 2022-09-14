const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "znetwork",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run znetwork tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/main.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("znetwork", thisDir() ++ "/src/main.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.addIncludeDir(thisDir() ++ "/libs/enet/include");
    lib.linkSystemLibraryName("c");

    if (exe.target.isWindows()) {
        lib.linkSystemLibraryName("ws2_32");
        lib.linkSystemLibraryName("winmm");
    }

    const defines = .{
        "-DHAS_FCNTL=1",
        "-DHAS_POLL=1",
        "-DHAS_GETNAMEINFO=1",
        "-DHAS_GETADDRINFO=1",
        "-DHAS_GETHOSTBYNAME_R=1",
        "-DHAS_GETHOSTBYADDR_R=1",
        "-DHAS_INET_PTON=1",
        "-DHAS_INET_NTOP=1",
        "-DHAS_MSGHDR_FLAGS=1",
        "-DHAS_SOCKLEN_T=1",
        "-fno-sanitize=undefined",
    };
    lib.addCSourceFile(thisDir() ++ "/libs/enet/callbacks.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/compress.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/host.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/list.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/packet.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/peer.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/protocol.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/unix.c", &defines);
    lib.addCSourceFile(thisDir() ++ "/libs/enet/win32.c", &defines);

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
