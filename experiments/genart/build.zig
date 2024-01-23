const std = @import("std");
const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) void {
    const latest_experiment = 31;
    inline for (1..latest_experiment + 1) |i| {
        if (i == 6 or i == 7 or i == 30) continue;
        const name = comptime std.fmt.comptimePrint("x{d:0>4}", .{i});
        install(b, options.optimize, options.target, name);
    }
}

fn install(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
    comptime name: []const u8,
) void {
    const zsdl_pkg = @import("../../build.zig").zsdl_pkg;
    const zopengl_pkg = @import("../../build.zig").zopengl_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const znoise_pkg = @import("../../build.zig").znoise_pkg;
    const zstbi_pkg = @import("../../build.zig").zstbi_pkg;

    comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    comptime _ = std.mem.replace(u8, name, "_", " ", desc_name[0..]);
    const desc_size = comptime std.mem.indexOf(u8, &desc_name, "\x00").?;

    const xcommon = b.createModule(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/xcommon.zig" },
        .imports = &.{
            .{ .name = "zsdl", .module = zsdl_pkg.zsdl },
            .{ .name = "zopengl", .module = zopengl_pkg.zopengl },
            .{ .name = "zstbi", .module = zstbi_pkg.zstbi },
        },
    });
    const ximpl = b.createModule(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ name ++ ".zig" },
        .imports = &.{
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
    exe.root_module.addImport("xcommon", xcommon);
    exe.root_module.addImport("ximpl", ximpl);
    zsdl_pkg.link(exe);
    zopengl_pkg.link(exe);
    zstbi_pkg.link(exe);

    const install_step = b.step(name, "Build '" ++ desc_name[0..desc_size] ++ "' genart experiment");
    install_step.dependOn(&b.addInstallArtifact(exe, .{}).step);

    const run_step = b.step(name ++ "-run", "Run '" ++ desc_name[0..desc_size] ++ "' genart experiment");
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install_step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
