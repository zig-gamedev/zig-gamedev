const std = @import("std");

const zsdl = @import("../../libs/zsdl/build.zig");
const zopengl = @import("../../libs/zopengl/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const znoise = @import("../../libs/znoise/build.zig");
const zstbi = @import("../../libs/zstbi/build.zig");

var zsdl_pkg: zsdl.Package = undefined;
var zopengl_pkg: zopengl.Package = undefined;
var zmath_pkg: zmath.Package = undefined;
var znoise_pkg: znoise.Package = undefined;
var zstbi_pkg: zstbi.Package = undefined;

const Options = @import("../build.zig").Options;

pub fn buildWithOptions(b: *std.Build, options: Options) void {
    zsdl_pkg = zsdl.package(b, options.target, options.optimize, .{});
    zopengl_pkg = zopengl.package(b, options.target, options.optimize, .{});
    zmath_pkg = zmath.package(b, options.target, options.optimize, .{});
    znoise_pkg = znoise.package(b, options.target, options.optimize, .{});
    zstbi_pkg = zstbi.package(b, options.target, options.optimize, .{});

    const latest_experiment = 31;
    inline for (1..latest_experiment + 1) |i| {
        if (i == 6 or i == 7 or i == 30) continue;
        const name = comptime std.fmt.comptimePrint("x{d:0>4}", .{i});
        install(b, name, options);
    }
}

fn install(b: *std.build.Builder, comptime name: []const u8, options: Options) void {
    comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    comptime _ = std.mem.replace(u8, name, "_", " ", desc_name[0..]);
    comptime var desc_size = std.mem.indexOf(u8, &desc_name, "\x00").?;

    const xcommon = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/xcommon.zig" },
        .dependencies = &.{
            .{ .name = "zsdl", .module = zsdl_pkg.zsdl },
            .{ .name = "zopengl", .module = zopengl_pkg.zopengl },
            .{ .name = "zstbi", .module = zstbi_pkg.zstbi },
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
        .target = options.target,
        .optimize = options.optimize,
    });
    exe.rdynamic = true;
    exe.addModule("xcommon", xcommon);
    exe.addModule("ximpl", ximpl);
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
