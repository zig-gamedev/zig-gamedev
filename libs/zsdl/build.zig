const std = @import("std");
const assert = std.debug.assert;

pub const ApiVersion = enum {
    sdl2,
    sdl3,
};

pub const Options = struct {
    api_version: ApiVersion = .sdl2,
    enable_ttf: bool = false,
};

pub const Package = struct {
    target: std.Build.ResolvedTarget,
    options: Options,
    zsdl: *std.Build.Module,
    zsdl_options: *std.Build.Module,
    install: *std.Build.Step,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.linkLibC();

        exe.root_module.addImport("zsdl_options", pkg.zsdl_options);
        exe.root_module.addImport("zsdl", pkg.zsdl);

        exe.step.dependOn(pkg.install);

        switch (pkg.target.result.os.tag) {
            .windows => {
                assert(pkg.target.result.cpu.arch.isX86());

                exe.addLibraryPath(.{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/lib" });

                switch (pkg.options.api_version) {
                    .sdl2 => {
                        exe.linkSystemLibrary("SDL2");
                        exe.linkSystemLibrary("SDL2main");
                        if (pkg.options.enable_ttf) {
                            exe.linkSystemLibrary("SDL2_ttf");
                        }
                    },
                    .sdl3 => {
                        // TODO: link SDL3 (windows)
                    },
                }
            },
            .linux => {
                assert(pkg.target.result.cpu.arch.isX86());

                exe.root_module.addRPathSpecial("$ORIGIN");

                exe.addLibraryPath(.{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib" });

                switch (pkg.options.api_version) {
                    .sdl2 => {
                        exe.linkSystemLibrary("SDL2-2.0");
                        if (pkg.options.enable_ttf) {
                            exe.linkSystemLibrary("SDL2_ttf-2.0");
                        }
                    },
                    .sdl3 => {
                        // TODO: link SDL3 (linux)
                    },
                }
            },
            .macos => {
                exe.root_module.addRPathSpecial("@executable_path/Frameworks");

                exe.addFrameworkPath(.{ .path = thisDir() ++ "/libs/macos/Frameworks" });

                switch (pkg.options.api_version) {
                    .sdl2 => {
                        exe.linkFramework("SDL2");
                        if (pkg.options.enable_ttf) {
                            exe.linkFramework("SDL2_ttf");
                        }
                    },
                    .sdl3 => {
                        // TODO: bundle SDL3.framework instead of this hack
                        // exe.linkFramework("SDL3");
                        exe.addLibraryPath(.{ .path = "/usr/local/lib" });
                        exe.linkSystemLibrary("SDL3");

                        if (pkg.options.enable_ttf) {
                            exe.linkFramework("SDL2_ttf");
                        }
                    },
                }
            },
            else => {},
        }
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const options_step = b.addOptions();
    inline for (std.meta.fields(Options)) |option_field| {
        const option_val = @field(args.options, option_field.name);
        options_step.addOption(@TypeOf(option_val), option_field.name, option_val);
    }

    const options = options_step.createModule();

    const zsdl = b.addModule("zsdl", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        .imports = &.{
            .{ .name = "zsdl_options", .module = options },
        },
    });

    const install_step = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_step.* = std.Build.Step.init(.{ .id = .custom, .name = "zsdl-install", .owner = b });

    switch (target.result.os.tag) {
        .windows => {
            switch (args.options.api_version) {
                .sdl2 => {
                    install_step.dependOn(
                        &b.addInstallFile(
                            .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                            "bin/SDL2.dll",
                        ).step,
                    );
                    if (args.options.enable_ttf) {
                        install_step.dependOn(
                            &b.addInstallFile(
                                .{
                                    .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2_ttf.dll",
                                },
                                "bin/SDL2_ttf.dll",
                            ).step,
                        );
                    }
                },
                .sdl3 => {},
            }
        },
        .linux => {
            switch (args.options.api_version) {
                .sdl2 => {
                    install_step.dependOn(
                        &b.addInstallFile(
                            .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                            "bin/libSDL2-2.0.so.0",
                        ).step,
                    );
                    if (args.options.enable_ttf) {
                        install_step.dependOn(
                            &b.addInstallFile(
                                .{
                                    .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2_ttf-2.0.so",
                                },
                                "bin/libSDL2_ttf-2.0.so.0",
                            ).step,
                        );
                    }
                },
                .sdl3 => {},
            }
        },
        .macos => {
            switch (args.options.api_version) {
                .sdl2 => {
                    install_step.dependOn(
                        &b.addInstallDirectory(.{
                            .source_dir = .{
                                .path = thisDir() ++ "/libs/macos/Frameworks/SDL2.framework",
                            },
                            .install_dir = .{ .custom = "" },
                            .install_subdir = "bin/Frameworks/SDL2.framework",
                        }).step,
                    );
                    if (args.options.enable_ttf) {
                        install_step.dependOn(
                            &b.addInstallDirectory(.{
                                .source_dir = .{
                                    .path = thisDir() ++ "/libs/macos/Frameworks/SDL2_ttf.framework",
                                },
                                .install_dir = .{ .custom = "" },
                                .install_subdir = "bin/Frameworks/SDL2_ttf.framework",
                            }).step,
                        );
                    }
                },
                .sdl3 => {},
            }
        },
        else => {},
    }

    return .{
        .target = target,
        .options = args.options,
        .zsdl = zsdl,
        .zsdl_options = options,
        .install = install_step,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zsdl tests");
    test_step.dependOn(runTests(b, optimize, target));

    const version_check_step = b.step("version-check", "checks runtime library version is the same as the compiled version");
    version_check_step.dependOn(testStep("src/sdl2_version_check.zig", b, "zsdl-sdl2-version-check", target, optimize, .{
        .api_version = .sdl2,
        .enable_ttf = true,
    }));

    _ = package(b, target, optimize, .{
        .options = .{
            .api_version = b.option(ApiVersion, "api_version", "Select an SDL API version") orelse .sdl2,
        },
    });
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const step = b.allocator.create(std.Build.Step) catch @panic("OOM");
    step.* = std.Build.Step.init(.{ .id = .custom, .name = "zsdl-tests", .owner = b });

    step.dependOn(testStep("src/zsdl.zig", b, "zsdl-tests-sdl2", target, optimize, .{
        .api_version = .sdl2,
        .enable_ttf = true,
    }));

    // TODO: link SDL3 libs on all platforms
    // step.dependOn(testStep("src/zsdl.zig", b, "zsdl-tests-sdl3", target, optimize, .{
    //     .api_version = .sdl3,
    //     .enable_ttf = true,
    // }));

    return step;
}

fn testStep(
    comptime test_entry: []const u8,
    b: *std.Build,
    name: []const u8,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    options: Options,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = name,
        .root_source_file = .{ .path = thisDir() ++ "/" ++ test_entry },
        .target = target,
        .optimize = optimize,
    });
    const pkg = package(b, target, optimize, .{ .options = options });
    pkg.link(tests);
    var test_run = b.addRunArtifact(tests);
    switch (target.result.os.tag) {
        .windows => test_run.setCwd(.{
            .path = b.getInstallPath(.bin, ""),
        }),
        else => tests.addRPath(.{
            .path = b.getInstallPath(.bin, ""),
        }),
    }
    return &test_run.step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
