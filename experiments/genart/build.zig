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
    const zsdl = b.dependency("zsdl", .{
        .target = target,
    });
    const zsdl2_module = zsdl.module("zsdl2");

    const zopengl = b.dependency("zopengl", .{
        .target = target,
    });
    const zopengl_module = zopengl.module("root");

    const zmath = b.dependency("zmath", .{
        .target = target,
    });
    const zmath_module = zmath.module("root");

    const znoise = b.dependency("znoise", .{
        .target = target,
    });
    const znoise_module = znoise.module("root");

    const zstbi = b.dependency("zstbi", .{
        .target = target,
    });
    const zstbi_module = zstbi.module("root");

    comptime var desc_name: [256]u8 = [_]u8{0} ** 256;
    comptime _ = std.mem.replace(u8, name, "_", " ", desc_name[0..]);
    const desc_size = comptime std.mem.indexOf(u8, &desc_name, "\x00").?;

    const xcommon = b.createModule(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/xcommon.zig" },
        .imports = &.{
            .{ .name = "zsdl2", .module = zsdl2_module },
            .{ .name = "zopengl", .module = zopengl_module },
            .{ .name = "zstbi", .module = zstbi_module },
        },
    });
    const ximpl = b.createModule(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ name ++ ".zig" },
        .imports = &.{
            .{ .name = "zsdl2", .module = zsdl2_module },
            .{ .name = "zopengl", .module = zopengl_module },
            .{ .name = "zmath", .module = zmath_module },
            .{ .name = "znoise", .module = znoise_module },
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

    exe.root_module.addImport("zstbi", zstbi_module);
    exe.linkLibrary(zstbi.artifact("zstbi"));

    exe.root_module.addImport("zsdl2", zsdl2_module);
    switch (target.result.os.tag) {
        .windows => {
            if (target.result.cpu.arch.isX86()) {
                exe.addLibraryPath(.{
                    .path = zsdl.path("libs/x86_64-windows-gnu/lib").getPath(b),
                });
            }
        },
        .linux => {
            if (target.result.cpu.arch.isX86()) {
                exe.addLibraryPath(.{
                    .path = zsdl.path("libs/x86_64-linux-gnu/lib").getPath(b),
                });
            }
        },
        .macos => {
            exe.addFrameworkPath(.{
                .path = zsdl.path("libs/macos/Frameworks").getPath(b),
            });
        },
        else => {},
    }

    @import("zsdl").link_SDL2(exe);

    exe.root_module.addImport("zopengl", zopengl_module);

    const install_step = b.step(
        name,
        "Build '" ++ desc_name[0..desc_size] ++ "' genart experiment",
    );
    install_step.dependOn(&b.addInstallArtifact(exe, .{}).step);

    const run_step = b.step(
        name ++ "-run",
        "Run '" ++ desc_name[0..desc_size] ++ "' genart experiment",
    );
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install_step);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
