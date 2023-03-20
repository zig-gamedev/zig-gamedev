const std = @import("std");
const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) void {
    install(b, options.optimize, options.target, "x0001");
}

fn install(
    b: *std.build.Builder,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
    comptime name: []const u8,
) void {
    const zsdl_pkg = @import("../../build.zig").zsdl_pkg;
    const zopengl_pkg = @import("../../build.zig").zopengl_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const znoise_pkg = @import("../../build.zig").znoise_pkg;

    comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    comptime _ = std.mem.replace(u8, name, "_", " ", desc_name[0..]);
    comptime var desc_size = std.mem.indexOf(u8, &desc_name, "\x00").?;

    const xcommon = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/xcommon.zig" },
        .dependencies = &.{
            .{ .name = "zsdl", .module = zsdl_pkg.zsdl },
            .{ .name = "zopengl", .module = zopengl_pkg.zopengl },
        },
    });
    const ximpl = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/" ++ name ++ ".zig" },
        .dependencies = &.{
            .{ .name = "zsdl", .module = zsdl_pkg.zsdl },
            .{ .name = "zopengl", .module = zopengl_pkg.zopengl },
            .{ .name = "zmath", .module = zmath_pkg.zmath },
            .{ .name = "znoise", .module = znoise_pkg.znoise },
            .{ .name = "xcommon", .module = xcommon },
        },
    });
    const exe = b.addExecutable(.{
        .name = name,
        .root_source_file = .{ .path = thisDir() ++ "/src/genart.zig" },
        .target = target,
        .optimize = optimize,
    });
    exe.rdynamic = true;
    exe.addModule("xcommon", xcommon);
    exe.addModule("ximpl", ximpl);
    exe.addModule("zsdl", zsdl_pkg.zsdl);
    exe.addModule("zopengl", zopengl_pkg.zopengl);
    zsdl_pkg.link(b, exe);

    const install_step = b.step(name, "Build '" ++ desc_name[0..desc_size] ++ "' genart experiment");
    install_step.dependOn(&b.addInstallArtifact(exe).step);

    const run_step = b.step(name ++ "-run", "Run '" ++ desc_name[0..desc_size] ++ "' genart experiment");
    const run_cmd = exe.run();
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install_step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
