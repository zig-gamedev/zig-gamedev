const std = @import("std");
const znet = @import("../../libs/znetwork/build.zig");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const client_exe = b.addExecutable("zenet_test_client", thisDir() ++ "/src/client.zig");
    client_exe.setBuildMode(options.build_mode);
    client_exe.setTarget(options.target);
    client_exe.want_lto = false;
    client_exe.addPackage(znet.pkg);
    znet.link(client_exe);

    const server_exe = b.addExecutable("zenet_test_server", thisDir() ++ "/src/server.zig");
    server_exe.setBuildMode(options.build_mode);
    server_exe.setTarget(options.target);
    server_exe.want_lto = false;
    server_exe.addPackage(znet.pkg);
    server_exe.step.dependOn(&b.addInstallArtifact(client_exe).step);
    znet.link(server_exe);

    return server_exe;
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
