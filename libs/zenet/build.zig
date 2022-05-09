const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zenet",
    .path = .{ .path = thisDir() ++ "/src/zenet.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zenet tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(comptime thisDir() ++ "/src/zenet.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zenet", comptime thisDir() ++ "/src/zenet.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.want_lto = false;
    lib.addIncludeDir(comptime thisDir() ++ "/libs/enet/include");
    lib.linkSystemLibrary("c");

    if (exe.target.isWindows()) {
        lib.linkSystemLibrary("ws2_32");
        lib.linkSystemLibrary("winmm");
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
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/callbacks.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/compress.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/host.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/list.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/packet.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/peer.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/protocol.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/unix.c", &defines);
    lib.addCSourceFile(comptime thisDir() ++ "/libs/enet/win32.c", &defines);

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
