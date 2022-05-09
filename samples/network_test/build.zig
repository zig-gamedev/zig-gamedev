const std = @import("std");
const zenet = @import("../../libs/zenet/build.zig");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const client_exe = b.addExecutable("zenet_test_client", comptime thisDir() ++ "/src/client.zig");
    client_exe.setBuildMode(options.build_mode);
    client_exe.setTarget(options.target);
    client_exe.want_lto = false;
    client_exe.addPackage(zenet.pkg);
    zenet.link(client_exe);

    const server_exe = b.addExecutable("zenet_test_server", comptime thisDir() ++ "/src/server.zig");
    server_exe.setBuildMode(options.build_mode);
    server_exe.setTarget(options.target);
    server_exe.want_lto = false;
    server_exe.addPackage(zenet.pkg);
    server_exe.step.dependOn(&b.addInstallArtifact(client_exe).step);
    zenet.link(server_exe);

    return server_exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
