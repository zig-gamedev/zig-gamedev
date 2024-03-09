//! Builds Dawn from sources into a single static lib. Inspired by mach-gpu-dawn: https://github.com/hexops/mach-gpu-dawn
//!
//! TODO:
//! - Use Zig package manager to fetch Dawn and DirectXShaderCompiler sources, eliminate git requirement!
//!

const std = @import("std");

const dawn_git_url: []const u8 = "https://github.com/zig-gamedev/dawn";
const dawn_git_revision: []const u8 = "generated-2023-08-10.1691685418";
const dxcompiler_git_url: []const u8 = "https://github.com/zig-gamedev/DirectXShaderCompiler";
const dxcompiler_git_revision: []const u8 = "bb5211aa247978e2ab75bea9f5c985ba3fabd269";

pub const DefaultOptions = struct {
    pub const optimize: std.builtin.Mode = .ReleaseFast;
    pub const disable_logging: bool = true;
};

pub const Options = struct {
    optimize: std.builtin.Mode = DefaultOptions.optimize,
    disable_logging: bool = DefaultOptions.disable_logging,
};

pub fn buildStaticLibrary(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    options: Options,
) !*std.Build.Step.Compile {
    try ensureGitRepoCloned(
        b.allocator,
        dawn_git_url,
        dawn_git_revision,
        thisDir() ++ "/tmp/dawn",
        true,
    );

    try ensureGitRepoCloned(
        b.allocator,
        dxcompiler_git_url,
        dxcompiler_git_revision,
        thisDir() ++ "/tmp/DirectXShaderCompiler",
        false,
    );

    const lib = b.addStaticLibrary(.{
        .name = "dawn",
        .target = target,
        .optimize = options.optimize,
    });

    if (options.optimize != .Debug) {
        lib.root_module.strip = true;
    }

    if (options.disable_logging) {
        lib.defineCMacro("DAWN_DISABLE_LOGGING", "1");
    }

    lib.defineCMacro("DAWN_ENABLE_BACKEND_NULL", "1");

    switch (target.result.os.tag) {
        .windows => {
            lib.defineCMacro("DAWN_ENABLE_BACKEND_D3D12", "1");

            lib.defineCMacro("_HRESULT_DEFINED", "");
            lib.defineCMacro("HRESULT", "long");
            lib.defineCMacro("DAWN_NO_WINDOWS_UI", "1");
            lib.defineCMacro("__EMULATE_UUID", "");
            lib.defineCMacro("_CRT_SECURE_NO_WARNINGS", "");
            lib.defineCMacro("WIN32_LEAN_AND_MEAN", "");
            lib.defineCMacro("D3D10_ARBITRARY_HEADER_ORDERING", "");
            lib.defineCMacro("NOMINMAX", "");

            // for abseil-cpp
            lib.defineCMacro("ABSL_FORCE_THREAD_IDENTITY_MODE", "2");

            // for dxcompiler
            lib.defineCMacro("UNREFERENCED_PARAMETER(x)", "");
            lib.defineCMacro("MSFT_SUPPORTS_CHILD_PROCESSES", "1");
            lib.defineCMacro("HAVE_LIBPSAPI", "1");
            lib.defineCMacro("HAVE_LIBSHELL32", "1");
            lib.defineCMacro("LLVM_ON_WIN32", "1");
        },
        .macos, .ios => {
            lib.defineCMacro("DAWN_ENABLE_BACKEND_METAL", "1");

            // MacOS: this must be defined for macOS 13.3 and older.
            // Critically, this MUST NOT be included as a -D__kernel_ptr_semantics flag. If it is,
            // then this macro will not be defined even if `defineCMacro` was also called!
            lib.defineCMacro("__kernel_ptr_semantics", "");
        },
        else => {
            lib.defineCMacro("DAWN_ENABLE_BACKEND_VULKAN", "1");

            if (target.result.isAndroid()) {
                lib.defineCMacro("DAWN_USE_SYNC_FDS", "1");
            }
        },
    }

    if (target.result.abi == .musl) {
        // musl needs this defined in order for off64_t to be a type, which abseil-cpp uses
        lib.defineCMacro("_FILE_OFFSET_BITS", "64");
        lib.defineCMacro("_LARGEFILE64_SOURCE", "");
    }

    lib.defineCMacro("TINT_BUILD_SPV_READER", "1");
    lib.defineCMacro("TINT_BUILD_SPV_WRITER", "1");
    lib.defineCMacro("TINT_BUILD_WGSL_READER", "1");
    lib.defineCMacro("TINT_BUILD_WGSL_WRITER", "1");
    lib.defineCMacro("TINT_BUILD_MSL_WRITER", "1");
    lib.defineCMacro("TINT_BUILD_HLSL_WRITER", "1");
    lib.defineCMacro("TINT_BUILD_GLSL_WRITER", "0");
    lib.defineCMacro("TINT_BUILD_SYNTAX_TREE_WRITER", "1");

    try addSourcesDawnCommon(b, lib, options);
    try addSourcesDawnPlatform(b, lib, options);
    try addSourcesAbseil(b, lib, options);
    try addSourcesDawnNative(b, lib, options);
    try addSourcesDawnWire(b, lib, options);
    try addSourcesSPIRVTools(b, lib, options);
    try addSourcesTint(b, lib, options);
    if (target.result.os.tag == .windows) {
        try addSourcesDxcompiler(b, lib, options);
    }

    const system_sdk = b.dependency("system_sdk", .{});

    switch (target.result.os.tag) {
        .windows => {
            lib.addSystemIncludePath(.{ .path = thisDir() ++ "/libs/d3d/include" });
            lib.linkSystemLibrary("oleaut32");
            lib.linkSystemLibrary("ole32");
            lib.linkSystemLibrary("dbghelp");
            lib.linkSystemLibrary("bcrypt");
        },
        .macos => {
            lib.addFrameworkPath(.{ .path = system_sdk.path("macos12/System/Library/Frameworks").getPath(b) });
            lib.addSystemIncludePath(.{ .path = system_sdk.path("macos12/usr/include").getPath(b) });
            lib.addLibraryPath(.{ .path = system_sdk.path("macos12/usr/lib").getPath(b) });
            lib.linkSystemLibrary("objc");
            lib.linkFramework("Foundation");
            lib.linkFramework("CoreFoundation");
            lib.linkFramework("Metal");
            lib.linkFramework("CoreGraphics");
            lib.linkFramework("Foundation");
            lib.linkFramework("IOKit");
            lib.linkFramework("IOSurface");
            lib.linkFramework("QuartzCore");
        },
        else => if (isLinuxDesktopLike(target.result.os.tag)) {
            lib.addSystemIncludePath(.{ .path = system_sdk.path("linux/include").getPath(b) });
            lib.addLibraryPath(.{ .path = system_sdk.path("linux/lib").getPath(b) });
            lib.addSystemIncludePath(.{ .path = thisDir() ++ "/libs/vulkan-headers/include" });
        },
    }

    lib.linkLibCpp();

    return lib;
}

/// Adds common sources; derived from src/common/BUILD.gn
fn addSourcesDawnCommon(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    const target = step.rootModuleTarget();

    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/src",
    });
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/src/dawn/common/",
            "/tmp/dawn/out/Debug/gen/src/dawn/common/",
        },
        .flags = flags.items,
        .excluding_contains = &.{
            "test",
            "benchmark",
            "mock",
            "WindowsUtils.cpp",
        },
    });

    var cpp_sources = std.ArrayList([]const u8).init(b.allocator);
    switch (target.os.tag) {
        .macos => {
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/src/dawn/common/SystemUtils_mac.mm");
        },
        .windows => {
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/src/dawn/common/WindowsUtils.cpp");
        },
        else => {},
    }

    var cpp_flags = std.ArrayList([]const u8).init(b.allocator);
    try cpp_flags.appendSlice(flags.items);
    try appendFlags(step, &cpp_flags, options.optimize, true);
    step.addCSourceFiles(.{ .files = cpp_sources.items, .flags = cpp_flags.items });
}

/// Adds dawn platform sources; derived from src/dawn/platform/BUILD.gn
fn addSourcesDawnPlatform(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    var cpp_flags = std.ArrayList([]const u8).init(b.allocator);
    try appendFlags(step, &cpp_flags, options.optimize, true);
    try cpp_flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/include",
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/dawn/platform/",
            "tmp/dawn/src/dawn/platform/metrics/",
            "tmp/dawn/src/dawn/platform/tracing/",
        },
        .flags = cpp_flags.items,
        .excluding_contains = &.{},
    });
}

/// Adds third_party/abseil sources; derived from:
/// ```
/// $ find third_party/abseil-cpp/absl | grep '\.cc' | grep -v 'test' | grep -v 'benchmark' | grep -v gaussian_distribution_gentables | grep -v print_hash_of | grep -v chi_square
/// ```
fn addSourcesAbseil(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    const target = step.rootModuleTarget();

    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/abseil-cpp",
        "-Wno-deprecated-declarations",
        "-Wno-deprecated-builtins",
    });
    if (target.os.tag == .windows) {
        try flags.append(thisDir() ++ "/src/dawn/zig_mingw_pthread");
    }

    // absl
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/third_party/abseil-cpp/absl/strings/",
            "tmp/dawn/third_party/abseil-cpp/absl/strings/internal/",
            "tmp/dawn/third_party/abseil-cpp/absl/strings/internal/str_format/",
            "tmp/dawn/third_party/abseil-cpp/absl/numeric/",
            "tmp/dawn/third_party/abseil-cpp/absl/base/internal/",
            "tmp/dawn/third_party/abseil-cpp/absl/base/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "_test", "_testing", "benchmark", "print_hash_of.cc", "gaussian_distribution_gentables.cc" },
    });
}

/// Adds dawn native sources; derived from src/dawn/native/BUILD.gn
fn addSourcesDawnNative(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    const target = step.rootModuleTarget();

    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn",
        "-I" ++ thisDir() ++ "/tmp/dawn/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/khronos",

        "-Wno-deprecated-declarations",
        "-Wno-deprecated-builtins",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/abseil-cpp",

        "-I" ++ thisDir() ++ "/tmp/dawn/",
        "-I" ++ thisDir() ++ "/tmp/dawn/include/tint",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/vulkan-tools/src/",

        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/src",
    });
    if (target.os.tag == .windows) {
        try flags.appendSlice(&.{
            "-Wno-nonportable-include-path",
            "-Wno-extern-c-compat",
            "-Wno-invalid-noreturn",
            "-Wno-pragma-pack",
            "-Wno-microsoft-template-shadow",
            "-Wno-unused-command-line-argument",
            "-Wno-microsoft-exception-spec",
            "-Wno-implicit-exception-spec-mismatch",
            "-Wno-unknown-attributes",
            "-Wno-c++20-extensions",
        });
    }

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/out/Debug/gen/src/dawn/",
            "/tmp/dawn/src/dawn/native/",
            "/tmp/dawn/src/dawn/native/utils/",
            "/tmp/dawn/src/dawn/native/stream/",
        },
        .flags = flags.items,
        .excluding_contains = &.{
            "test",
            "benchmark",
            "mock",
            "SpirvValidation.cpp",
            "X11Functions.cpp",
            "XlibXcbFunctions.cpp",
            "dawn_proc.c",
        },
    });

    // dawn_native_gen
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/out/Debug/gen/src/dawn/native/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark", "mock", "webgpu_dawn_native_proc.cpp" },
    });

    var cpp_sources = std.ArrayList([]const u8).init(b.allocator);
    switch (target.os.tag) {
        .windows => {
            inline for ([_][]const u8{
                "src/dawn/mingw_helpers.cpp",
            }) |path| {
                const abs_path = thisDir() ++ "/" ++ path;
                try cpp_sources.append(abs_path);
            }

            try appendLangScannedSources(b, step, options, .{
                .rel_dirs = &.{
                    "tmp/dawn/src/dawn/native/d3d/",
                    "tmp/dawn/src/dawn/native/d3d12/",
                },
                .flags = flags.items,
                .excluding_contains = &.{ "test", "benchmark", "mock" },
            });

            inline for ([_][]const u8{
                "src/dawn/native/d3d12/D3D12Backend.cpp",
            }) |path| {
                const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                try cpp_sources.append(abs_path);
            }
        },
        .macos, .ios => {
            try appendLangScannedSources(b, step, options, .{
                .objc = true,
                .rel_dirs = &.{
                    "tmp/dawn/src/dawn/native/metal/",
                    "tmp/dawn/src/dawn/native/",
                },
                .flags = flags.items,
                .excluding_contains = &.{ "test", "benchmark", "mock" },
            });
        },
        else => {
            try appendLangScannedSources(b, step, options, .{
                .rel_dirs = &.{
                    "tmp/dawn/src/dawn/native/vulkan/",
                },
                .flags = flags.items,
                .excluding_contains = &.{ "test", "benchmark", "mock" },
            });
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/" ++
                "src/dawn/native/vulkan/external_memory/MemoryService.cpp");
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/" ++
                "src/dawn/native/vulkan/external_memory/MemoryServiceImplementation.cpp");
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/" ++
                "src/dawn/native/vulkan/external_memory/MemoryServiceImplementationDmaBuf.cpp");
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/" ++
                "src/dawn/native/vulkan/external_semaphore/SemaphoreService.cpp");
            try cpp_sources.append(thisDir() ++ "/tmp/dawn/" ++
                "src/dawn/native/vulkan/external_semaphore/SemaphoreServiceImplementation.cpp");

            inline for ([_][]const u8{
                "src/dawn/native/SpirvValidation.cpp",
            }) |path| {
                const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                try cpp_sources.append(abs_path);
            }

            if (isLinuxDesktopLike(target.os.tag)) {
                inline for ([_][]const u8{
                    "src/dawn/native/X11Functions.cpp",
                }) |path| {
                    const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                    try cpp_sources.append(abs_path);
                }
                inline for ([_][]const u8{
                    "src/dawn/native/vulkan/external_memory/MemoryServiceImplementationOpaqueFD.cpp",
                    "src/dawn/native/vulkan/external_semaphore/SemaphoreServiceImplementationFD.cpp",
                }) |path| {
                    const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                    try cpp_sources.append(abs_path);
                }
            }

            if (target.os.tag == .fuchsia) {
                inline for ([_][]const u8{
                    "src/dawn/native/vulkan/external_memory/MemoryServiceImplementationZirconHandle.cpp",
                    "src/dawn/native/vulkan/external_semaphore/SemaphoreServiceImplementationZirconHandle.cpp",
                }) |path| {
                    const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                    try cpp_sources.append(abs_path);
                }
            }

            if (target.isAndroid()) {
                inline for ([_][]const u8{
                    "src/dawn/native/vulkan/external_memory/MemoryServiceImplementationAHardwareBuffer.cpp",
                    "src/dawn/native/vulkan/external_semaphore/SemaphoreServiceImplementationFD.cpp",
                }) |path| {
                    const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
                    try cpp_sources.append(abs_path);
                }
            }
        },
    }

    inline for ([_][]const u8{
        "src/dawn/native/null/DeviceNull.cpp",
    }) |path| {
        const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
        try cpp_sources.append(abs_path);
    }

    inline for ([_][]const u8{
        "src/dawn/native/null/NullBackend.cpp",
    }) |path| {
        const abs_path = thisDir() ++ "/tmp/dawn/" ++ path;
        try cpp_sources.append(abs_path);
    }

    var cpp_flags = std.ArrayList([]const u8).init(b.allocator);
    try cpp_flags.appendSlice(flags.items);
    try appendFlags(step, &cpp_flags, options.optimize, true);
    step.addCSourceFiles(.{ .files = cpp_sources.items, .flags = cpp_flags.items });
}

/// Adds dawn wire sources; derived from src/dawn/wire/BUILD.gn
fn addSourcesDawnWire(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn",
        "-I" ++ thisDir() ++ "/tmp/dawn/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/src",
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/out/Debug/gen/src/dawn/wire/",
            "/tmp/dawn/out/Debug/gen/src/dawn/wire/client/",
            "/tmp/dawn/out/Debug/gen/src/dawn/wire/server/",
            "/tmp/dawn/src/dawn/wire/",
            "/tmp/dawn/src/dawn/wire/client/",
            "/tmp/dawn/src/dawn/wire/server/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark", "mock" },
    });
}

/// Adds third_party/vulkan-deps/spirv-tools sources; derived from third_party/vulkan-deps/spirv-tools/src/BUILD.gn
fn addSourcesSPIRVTools(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-headers/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/third_party/vulkan-deps/spirv-tools/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/third_party/vulkan-deps/spirv-tools/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-headers/src/include/spirv/unified1",
    });

    // spvtools
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/source/",
            "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/source/util/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark" },
    });

    // spvtools_val
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/source/val/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark" },
    });

    // spvtools_opt
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/source/opt/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark" },
    });

    // spvtools_link
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/source/link/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "benchmark" },
    });
}

/// Adds tint sources; derived from src/tint/BUILD.gn
fn addSourcesTint(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    const target = step.rootModuleTarget();

    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/dawn/",
        "-I" ++ thisDir() ++ "/tmp/dawn/include/tint",

        // Required for TINT_BUILD_SPV_READER=1 and TINT_BUILD_SPV_WRITER=1, if specified
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-tools/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/vulkan-deps/spirv-headers/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/third_party/vulkan-deps/spirv-tools/src",
        "-I" ++ thisDir() ++ "/tmp/dawn/out/Debug/gen/third_party/vulkan-deps/spirv-tools/src/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/include",
        "-I" ++ thisDir() ++ "/tmp/dawn/third_party/abseil-cpp",
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/tint/",
            "tmp/dawn/src/tint/lang/core/",
            "tmp/dawn/src/tint/lang/core/constant/",
            "tmp/dawn/src/tint/lang/core/intrinsic/",
            "tmp/dawn/src/tint/lang/core/intrinsic/data/",
            "tmp/dawn/src/tint/lang/core/ir/",
            "tmp/dawn/src/tint/lang/core/ir/transform/",
            "tmp/dawn/src/tint/lang/core/type/",
            "tmp/dawn/src/tint/utils/debug/",
            "tmp/dawn/src/tint/utils/diagnostic/",
            "tmp/dawn/src/tint/utils/generator/",
            "tmp/dawn/src/tint/utils/ice/",
            "tmp/dawn/src/tint/utils/id/",
            "tmp/dawn/src/tint/utils/rtti/",
            "tmp/dawn/src/tint/utils/strconv/",
            "tmp/dawn/src/tint/utils/symbol/",
            "tmp/dawn/src/tint/utils/text/",
        },
        .flags = flags.items,
        .excluding_contains = &.{
            "test",
            "bench",
            "printer_other.cc",
            "printer_posix.cc",
            "printer_windows.cc",
        },
    });

    var cpp_sources = std.ArrayList([]const u8).init(b.allocator);

    if (target.os.tag == .windows) {
        try cpp_sources.append(thisDir() ++ "/tmp/dawn/src/tint/utils/diagnostic/printer_windows.cc");
    } else if (target.os.tag.isDarwin() or isLinuxDesktopLike(target.os.tag)) {
        try cpp_sources.append(thisDir() ++ "/tmp/dawn/src/tint/utils/diagnostic/printer_posix.cc");
    } else {
        try cpp_sources.append(thisDir() ++ "/tmp/dawn/src/tint/utils/diagnostic/printer_other.cc");
    }

    // libtint_spv_lang_src
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/tint/lang/spirv/reader",
            "tmp/dawn/src/tint/lang/spirv/reader/ast_parser/",
            "tmp/dawn/src/tint/lang/spirv/writer/",
            "tmp/dawn/src/tint/lang/spirv/writer/ast_printer/",
            "tmp/dawn/src/tint/lang/spirv/writer/common/",
            "tmp/dawn/src/tint/lang/spirv/writer/printer/",
            "tmp/dawn/src/tint/lang/spirv/writer/raise/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "bench" },
    });

    // libtint_wgsl_lang_src
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/tint/lang/wgsl/ast/",
            "tmp/dawn/src/tint/lang/wgsl/ast/transform/",
            "tmp/dawn/src/tint/lang/wgsl/helpers/",
            "tmp/dawn/src/tint/lang/wgsl/inspector/",
            "tmp/dawn/src/tint/lang/wgsl/program/",
            "tmp/dawn/src/tint/lang/wgsl/reader/",
            "tmp/dawn/src/tint/lang/wgsl/reader/parser/",
            "tmp/dawn/src/tint/lang/wgsl/reader/program_to_ir",
            "tmp/dawn/src/tint/lang/wgsl/resolver/",
            "tmp/dawn/src/tint/lang/wgsl/sem/",
            "tmp/dawn/src/tint/lang/wgsl/writer/",
            "tmp/dawn/src/tint/lang/wgsl/writer/ast_printer",
            "tmp/dawn/src/tint/lang/wgsl/writer/ir_to_program",
            "tmp/dawn/src/tint/lang/wgsl/writer/syntax_tree_printer",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "bench" },
    });

    // libtint_msl_lang_src
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/tint/lang/msl/validate/",
            "tmp/dawn/src/tint/lang/msl/writer/",
            "tmp/dawn/src/tint/lang/msl/writer/ast_printer/",
            "tmp/dawn/src/tint/lang/msl/writer/common/",
            "tmp/dawn/src/tint/lang/msl/writer/printer/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "bench" },
    });

    // libtint_hlsl_lang_src
    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "tmp/dawn/src/tint/lang/hlsl/validate/",
            "tmp/dawn/src/tint/lang/hlsl/writer/",
            "tmp/dawn/src/tint/lang/hlsl/writer/ast_printer/",
            "tmp/dawn/src/tint/lang/hlsl/writer/common/",
        },
        .flags = flags.items,
        .excluding_contains = &.{ "test", "bench" },
    });

    var cpp_flags = std.ArrayList([]const u8).init(b.allocator);
    try cpp_flags.appendSlice(flags.items);
    try appendFlags(step, &cpp_flags, options.optimize, true);
    step.addCSourceFiles(.{ .files = cpp_sources.items, .flags = cpp_flags.items });
}

/// Adds dxcompiler sources; derived from libs/DirectXShaderCompiler/CMakeLists.txt
fn addSourcesDxcompiler(b: *std.Build, step: *std.Build.Step.Compile, options: Options) !void {
    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(&.{
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/include/llvm/llvm_assert",
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/include",
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/build/include",
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/build/lib/HLSL",
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/build/lib/DxilPIXPasses",
        "-I" ++ thisDir() ++ "/tmp/DirectXShaderCompiler/build/include",
        "-Wno-inconsistent-missing-override",
        "-Wno-missing-exception-spec",
        "-Wno-switch",
        "-Wno-deprecated-declarations",
        "-Wno-macro-redefined", // regex2.h and regcomp.c requires this for OUT redefinition
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/DirectXShaderCompiler/lib/DxcSupport",

            // NOTE(mziulek): We don't need it for now.
            // "/tmp/DirectXShaderCompiler/lib/Analysis/IPA",
            // "/tmp/DirectXShaderCompiler/lib/Analysis",
            // "/tmp/DirectXShaderCompiler/lib/AsmParser",
            // "/tmp/DirectXShaderCompiler/lib/Bitcode/Writer",
            // "/tmp/DirectXShaderCompiler/lib/DxcBindingTable",
            // "/tmp/DirectXShaderCompiler/lib/DxilContainer",
            // "/tmp/DirectXShaderCompiler/lib/DxilPIXPasses",
            // "/tmp/DirectXShaderCompiler/lib/DxilRootSignature",
            // "/tmp/DirectXShaderCompiler/lib/DXIL",
            // "/tmp/DirectXShaderCompiler/lib/DxrFallback",
            // "/tmp/DirectXShaderCompiler/lib/HLSL",
            // "/tmp/DirectXShaderCompiler/lib/IRReader",
            // "/tmp/DirectXShaderCompiler/lib/IR",
            // "/tmp/DirectXShaderCompiler/lib/Linker",
            // "/tmp/DirectXShaderCompiler/lib/Miniz",
            // "/tmp/DirectXShaderCompiler/lib/Option",
            // "/tmp/DirectXShaderCompiler/lib/PassPrinters",
            // "/tmp/DirectXShaderCompiler/lib/Passes",
            // "/tmp/DirectXShaderCompiler/lib/ProfileData",
            // "/tmp/DirectXShaderCompiler/lib/Target",
            // "/tmp/DirectXShaderCompiler/lib/Transforms/InstCombine",
            // "/tmp/DirectXShaderCompiler/lib/Transforms/IPO",
            // "/tmp/DirectXShaderCompiler/lib/Transforms/Scalar",
            // "/tmp/DirectXShaderCompiler/lib/Transforms/Utils",
            // "/tmp/DirectXShaderCompiler/lib/Transforms/Vectorize",
        },
        .flags = flags.items,
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/DirectXShaderCompiler/lib/Support",
        },
        .flags = flags.items,
        .excluding_contains = &.{
            "DynamicLibrary.cpp", // ignore, HLSL_IGNORE_SOURCES
            "PluginLoader.cpp", // ignore, HLSL_IGNORE_SOURCES
            "Path.cpp", // ignore, LLVM_INCLUDE_TESTS
            "DynamicLibrary.cpp", // ignore
        },
    });

    try appendLangScannedSources(b, step, options, .{
        .rel_dirs = &.{
            "/tmp/DirectXShaderCompiler/lib/Bitcode/Reader",
        },
        .flags = flags.items,
        .excluding_contains = &.{
            "BitReader.cpp", // ignore
        },
    });
}

fn isLinuxDesktopLike(tag: std.Target.Os.Tag) bool {
    return switch (tag) {
        .linux,
        .freebsd,
        .kfreebsd,
        .openbsd,
        .dragonfly,
        => true,
        else => false,
    };
}

pub fn appendFlags(
    step: *std.Build.Step.Compile,
    flags: *std.ArrayList([]const u8),
    optimize: std.builtin.OptimizeMode,
    is_cpp: bool,
) !void {
    if (optimize == .Debug) try flags.append("-g1") else try flags.append("-g0");
    if (is_cpp) try flags.append("-std=c++17");
    if (isLinuxDesktopLike(step.rootModuleTarget().os.tag)) {
        step.defineCMacro("DAWN_USE_X11", "1");
        step.defineCMacro("DAWN_USE_WAYLAND", "1");
    }
}

fn appendLangScannedSources(
    b: *std.Build,
    step: *std.Build.Step.Compile,
    options: Options,
    args: struct {
        flags: []const []const u8,
        rel_dirs: []const []const u8 = &.{},
        objc: bool = false,
        excluding: []const []const u8 = &.{},
        excluding_contains: []const []const u8 = &.{},
    },
) !void {
    var cpp_flags = std.ArrayList([]const u8).init(b.allocator);
    try cpp_flags.appendSlice(args.flags);
    try appendFlags(step, &cpp_flags, options.optimize, true);
    const cpp_extensions: []const []const u8 = if (args.objc) &.{".mm"} else &.{ ".cpp", ".cc" };
    try appendScannedSources(b, step, .{
        .flags = cpp_flags.items,
        .rel_dirs = args.rel_dirs,
        .extensions = cpp_extensions,
        .excluding = args.excluding,
        .excluding_contains = args.excluding_contains,
    });

    var flags = std.ArrayList([]const u8).init(b.allocator);
    try flags.appendSlice(args.flags);
    try appendFlags(step, &flags, options.optimize, false);
    const c_extensions: []const []const u8 = if (args.objc) &.{".m"} else &.{".c"};
    try appendScannedSources(b, step, .{
        .flags = flags.items,
        .rel_dirs = args.rel_dirs,
        .extensions = c_extensions,
        .excluding = args.excluding,
        .excluding_contains = args.excluding_contains,
    });
}

fn appendScannedSources(b: *std.Build, step: *std.Build.Step.Compile, args: struct {
    flags: []const []const u8,
    rel_dirs: []const []const u8 = &.{},
    extensions: []const []const u8,
    excluding: []const []const u8 = &.{},
    excluding_contains: []const []const u8 = &.{},
}) !void {
    var sources = std.ArrayList([]const u8).init(b.allocator);
    for (args.rel_dirs) |rel_dir| {
        try scanSources(b, &sources, rel_dir, args.extensions, args.excluding, args.excluding_contains);
    }
    step.addCSourceFiles(.{ .files = sources.items, .flags = args.flags });
}

/// Scans rel_dir for sources ending with one of the provided extensions, excluding relative paths
/// listed in the excluded list.
/// Results are appended to the dst ArrayList.
fn scanSources(
    b: *std.Build,
    dst: *std.ArrayList([]const u8),
    rel_dir: []const u8,
    extensions: []const []const u8,
    excluding: []const []const u8,
    excluding_contains: []const []const u8,
) !void {
    const abs_dir = try std.fs.path.join(
        b.allocator,
        &.{ thisDir(), rel_dir },
    );
    var dir = std.fs.openDirAbsolute(abs_dir, .{ .iterate = true }) catch |err| {
        std.log.err("Failed to open {s}", .{abs_dir});
        return err;
    };
    defer dir.close();
    var dir_it = dir.iterate();
    while (try dir_it.next()) |entry| {
        if (entry.kind != .file) continue;
        var abs_path = try std.fs.path.join(b.allocator, &.{ abs_dir, entry.name });
        abs_path = try std.fs.realpathAlloc(b.allocator, abs_path);

        const allowed_extension = blk: {
            const ours = std.fs.path.extension(entry.name);
            for (extensions) |ext| {
                if (std.mem.eql(u8, ours, ext)) break :blk true;
            }
            break :blk false;
        };
        if (!allowed_extension) continue;

        const excluded = blk: {
            for (excluding) |excluded| {
                if (std.mem.eql(u8, entry.name, excluded)) break :blk true;
            }
            break :blk false;
        };
        if (excluded) continue;

        const excluded_contains = blk: {
            for (excluding_contains) |contains| {
                if (std.mem.containsAtLeast(u8, entry.name, 1, contains)) break :blk true;
            }
            break :blk false;
        };
        if (excluded_contains) continue;

        try dst.append(abs_path);
    }
}

fn ensureGitRepoCloned(
    allocator: std.mem.Allocator,
    clone_url: []const u8,
    revision: []const u8,
    dir: []const u8,
    get_submodules: bool,
) !void {
    ensureGit(allocator);

    if (std.fs.openDirAbsolute(dir, .{})) |_| {
        const current_revision = try getCurrentGitRevision(allocator, dir);
        if (!std.mem.eql(u8, current_revision, revision)) {
            // Reset to the desired revision
            exec(
                allocator,
                &[_][]const u8{ "git", "fetch" },
                dir,
            ) catch |err| std.debug.print(
                "warning: failed to 'git fetch' in {s}: {s}\n",
                .{ dir, @errorName(err) },
            );
            try exec(
                allocator,
                &[_][]const u8{ "git", "checkout", "--quiet", "--force", revision },
                dir,
            );
            if (get_submodules) {
                try exec(
                    allocator,
                    &[_][]const u8{ "git", "submodule", "update", "--init", "--recursive" },
                    dir,
                );
            }
        }
        return;
    } else |err| return switch (err) {
        error.FileNotFound => {
            std.log.info(
                "cloning required dependency..\ngit clone {s} {s}..\n",
                .{ clone_url, dir },
            );

            try exec(
                allocator,
                &[_][]const u8{ "git", "clone", "-c", "core.longpaths=true", clone_url, dir },
                thisDir(),
            );
            try exec(
                allocator,
                &[_][]const u8{ "git", "checkout", "--quiet", "--force", revision },
                dir,
            );
            if (get_submodules) {
                try exec(
                    allocator,
                    &[_][]const u8{ "git", "submodule", "update", "--init", "--recursive" },
                    dir,
                );
            }
            return;
        },
        else => err,
    };
}

fn getCurrentGitRevision(allocator: std.mem.Allocator, cwd: []const u8) ![]const u8 {
    const result = try std.ChildProcess.run(.{
        .allocator = allocator,
        .argv = &.{ "git", "rev-parse", "HEAD" },
        .cwd = cwd,
    });
    allocator.free(result.stderr);
    if (result.stdout.len > 0) return result.stdout[0 .. result.stdout.len - 1]; // trim newline
    return result.stdout;
}

fn ensureGit(allocator: std.mem.Allocator) void {
    const argv = &[_][]const u8{ "git", "--version" };
    const result = std.ChildProcess.run(.{
        .allocator = allocator,
        .argv = argv,
        .cwd = ".",
    }) catch { // e.g. FileNotFound
        std.log.err("'git --version' failed. Is git not installed?", .{});
        std.process.exit(1);
    };
    defer {
        allocator.free(result.stderr);
        allocator.free(result.stdout);
    }
    if (result.term.Exited != 0) {
        std.log.err("'git --version' failed. Is git not installed?", .{});
        std.process.exit(1);
    }
}

fn exec(allocator: std.mem.Allocator, argv: []const []const u8, cwd: []const u8) !void {
    var child = std.ChildProcess.init(argv, allocator);
    child.cwd = cwd;
    _ = try child.spawnAndWait();
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
