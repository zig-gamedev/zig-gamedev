const builtin = @import("builtin");
const std = @import("std");

pub const emsdk_ver_major = "3";
pub const emsdk_ver_minor = "1";
pub const emsdk_ver_tiny = "52";
pub const emsdk_version = emsdk_ver_major ++ "." ++ emsdk_ver_minor ++ "." ++ emsdk_ver_tiny;

pub const Package = struct {
    module: *std.Build.Module,
    emsdk_sysroot: []const u8,
    emsdk_path: []const u8,
    emsdk_setup_step: *std.Build.Step,

    pub fn emscStep(
        pkg: Package,
        compile_step: *std.Build.Step.Compile,
        emcc_options: []const u8,
    ) *std.Build.Step {
        compile_step.root_module.addImport("zemscripten", pkg.module);

        const b = compile_step.step.owner;

        const emcc = b.addSystemCommand(&.{
            b.pathJoin(&.{ pkg.emsdk_path, "upstream/emscripten/emcc" }),
            std.fmt.allocPrint(
                b.allocator,
                "-smalloc=emmalloc {s}",
                .{emcc_options},
            ) catch |err| switch (err) {
                error.OutOfMemory => @panic("Out of memory"),
            },
        });
        emcc.step.dependOn(pkg.emsdk_setup_step);

        emcc.addArtifactArg(compile_step);

        return &emcc.step;
    }
};

pub fn package(b: *std.Build) Package {
    const module = b.addModule("root", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/root.zig" },
    });

    const emsdk_path = b.dependency("emsdk", .{}).path("").getPath(b);

    const emsdk_bin_path = switch (builtin.target.os.tag) {
        .windows => b.pathJoin(&.{ emsdk_path, "emsdk.bat" }),
        else => b.pathJoin(&.{ emsdk_path, "emsdk" }),
    };

    var emsdk_install = b.addSystemCommand(&.{ emsdk_bin_path, "install", emsdk_version });

    switch (builtin.target.os.tag) {
        .linux, .macos => {
            emsdk_install.step.dependOn(&b.addSystemCommand(&.{ "chmod", "+x", emsdk_bin_path }).step);
        },
        else => {},
    }

    var emsdk_activate = b.addSystemCommand(&.{ emsdk_bin_path, "activate", emsdk_version });
    emsdk_activate.step.dependOn(&emsdk_install.step);

    const sysroot = b.pathJoin(&.{ emsdk_path, "upstream/emscripten/cache/sysroot" });

    return .{
        .module = module,
        .emsdk_sysroot = sysroot,
        .emsdk_path = emsdk_path,
        .emsdk_setup_step = &emsdk_activate.step,
    };
}

pub fn build(b: *std.Build) void {
    _ = package(b);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
