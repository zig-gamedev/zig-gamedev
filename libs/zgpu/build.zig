const std = @import("std");

pub fn link(exe: *std.build.LibExeObjStep) void {
    linkDawn(exe);

    exe.addIncludeDir(thisDir() ++ "/src");
    exe.addCSourceFile(thisDir() ++ "/src/dawn.cpp", &.{"-std=c++17"});
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zgpu.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = dependencies,
    };
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

//
// All below code has been borrowed from: https://github.com/hexops/mach-gpu-dawn/blob/main/sdk.zig
// We did some simplifications because we support only native, desktop applications.
//
const DawnOptions = struct {
    d3d12: ?bool = null,
    metal: ?bool = null,
    vulkan: ?bool = null,
    binary_version: []const u8 = "release-777728f",

    fn init(target: std.Target) DawnOptions {
        const tag = target.os.tag;
        var options = DawnOptions{};
        options.d3d12 = (tag == .windows);
        options.metal = (tag == .macos);
        options.vulkan = (tag == .linux);
        return options;
    }
};

fn linkDawn(exe: *std.build.LibExeObjStep) void {
    const target = (std.zig.system.NativeTargetInfo.detect(
        exe.target,
    ) catch unreachable).target;
    const options = DawnOptions.init(target);
    linkFromBinary(exe.builder, exe, options);
}

fn linkFromBinary(b: *std.build.Builder, step: *std.build.LibExeObjStep, options: DawnOptions) void {
    const target = (std.zig.system.NativeTargetInfo.detect(
        step.target,
    ) catch unreachable).target;
    const binaries_available = switch (target.os.tag) {
        .windows => target.abi.isGnu(),
        .linux => target.cpu.arch.isX86() and (target.abi.isGnu() or target.abi.isMusl()),
        .macos => blk: {
            if (!target.cpu.arch.isX86() and !target.cpu.arch.isAARCH64()) break :blk false;

            // If min. target macOS version is lesser than the min version we have available, then
            // our binary is incompatible with the target.
            const min_available = std.builtin.Version{ .major = 12, .minor = 0 };
            if (target.os.version_range.semver.min.order(min_available) == .lt) break :blk false;
            break :blk true;
        },
        else => false,
    };
    if (!binaries_available) {
        const zig_triple = target.zigTriple(b.allocator) catch unreachable;
        std.log.err("gpu-dawn binaries for {s} not available.", .{zig_triple});
        if (target.os.tag == .macos) {
            std.log.err("", .{});
            if (target.cpu.arch.isX86()) std.log.err(
                "-> Did you mean to use -Dtarget=x86_64-macos.12 ?",
                .{},
            );
            if (target.cpu.arch.isAARCH64()) std.log.err(
                "-> Did you mean to use -Dtarget=aarch64-macos.12 ?",
                .{},
            );
        }
        std.process.exit(1);
    }

    // Always link with release build for smaller downloads and faster iteration times.
    // If you want to debug Dawn please build it from source.
    const is_release = true;

    // Remove OS version range / glibc version from triple (we do not include that in our download
    // URLs.)
    var binary_target = std.zig.CrossTarget.fromTarget(target);
    binary_target.os_version_min = .{ .none = undefined };
    binary_target.os_version_max = .{ .none = undefined };
    binary_target.glibc_version = null;
    const zig_triple = binary_target.zigTriple(b.allocator) catch unreachable;
    ensureBinaryDownloaded(
        b.allocator,
        zig_triple,
        is_release,
        target.os.tag == .windows,
        options.binary_version,
    );

    const base_cache_dir_rel = std.fs.path.join(
        b.allocator,
        &.{ "zig-cache", "mach", "gpu-dawn" },
    ) catch unreachable;
    std.fs.cwd().makePath(base_cache_dir_rel) catch unreachable;
    const base_cache_dir = std.fs.cwd().realpathAlloc(
        b.allocator,
        base_cache_dir_rel,
    ) catch unreachable;
    const commit_cache_dir = std.fs.path.join(
        b.allocator,
        &.{ base_cache_dir, options.binary_version },
    ) catch unreachable;
    const release_tag = if (is_release) "release-fast" else "debug";
    const target_cache_dir = std.fs.path.join(
        b.allocator,
        &.{ commit_cache_dir, zig_triple, release_tag },
    ) catch unreachable;
    const include_dir = std.fs.path.join(
        b.allocator,
        &.{ commit_cache_dir, "include" },
    ) catch unreachable;

    step.addLibraryPath(target_cache_dir);
    step.linkSystemLibraryName("dawn");
    step.linkLibCpp();

    step.addIncludeDir(include_dir);
    step.addIncludeDir((comptime thisDir()) ++ "/src/dawn");

    if (options.vulkan.?) {
        step.linkSystemLibraryName("X11");
    }
    if (options.metal.?) {
        step.linkFramework("Metal");
        step.linkFramework("CoreGraphics");
        step.linkFramework("Foundation");
        step.linkFramework("IOKit");
        step.linkFramework("IOSurface");
        step.linkFramework("QuartzCore");
    }
    if (options.d3d12.?) {
        step.linkSystemLibraryName("ole32");
        step.linkSystemLibraryName("dxguid");
    }
}

fn ensureBinaryDownloaded(
    allocator: std.mem.Allocator,
    zig_triple: []const u8,
    is_release: bool,
    is_windows: bool,
    version: []const u8,
) void {
    // If zig-cache/mach/gpu-dawn/<git revision> does not exist:
    //   If on a commit in the main branch => rm -r zig-cache/mach/gpu-dawn/
    //   else => noop
    // If zig-cache/mach/gpu-dawn/<git revision>/<target> exists:
    //   noop
    // else:
    //   Download archive to zig-cache/mach/gpu-dawn/download/macos-aarch64
    //   Extract to zig-cache/mach/gpu-dawn/<git revision>/macos-aarch64/libgpu.a
    //   Remove zig-cache/mach/gpu-dawn/download

    const base_cache_dir_rel = std.fs.path.join(
        allocator,
        &.{ "zig-cache", "mach", "gpu-dawn" },
    ) catch unreachable;
    std.fs.cwd().makePath(base_cache_dir_rel) catch unreachable;
    const base_cache_dir = std.fs.cwd().realpathAlloc(
        allocator,
        base_cache_dir_rel,
    ) catch unreachable;
    const commit_cache_dir = std.fs.path.join(
        allocator,
        &.{ base_cache_dir, version },
    ) catch unreachable;

    if (!dirExists(commit_cache_dir)) {
        // Commit cache dir does not exist. If the commit we're on is in the main branch, we're
        // probably moving to a newer commit and so we should cleanup older cached binaries.
        const current_git_commit = getCurrentGitCommit(allocator) catch unreachable;
        if (gitBranchContainsCommit(allocator, "main", current_git_commit) catch false) {
            std.fs.deleteTreeAbsolute(base_cache_dir) catch {};
        }
    }

    const release_tag = if (is_release) "release-fast" else "debug";
    const target_cache_dir = std.fs.path.join(
        allocator,
        &.{ commit_cache_dir, zig_triple, release_tag },
    ) catch unreachable;
    if (dirExists(target_cache_dir)) {
        return; // nothing to do, already have the binary
    }
    downloadBinary(
        allocator,
        commit_cache_dir,
        release_tag,
        target_cache_dir,
        zig_triple,
        is_windows,
        version,
    ) catch |err| {
        // A download failed, or extraction failed, so wipe out the directory to ensure we correctly
        // try again next time.
        std.fs.deleteTreeAbsolute(base_cache_dir) catch {};
        std.log.err("mach/gpu-dawn: prebuilt binary download failed: {s}", .{@errorName(err)});
        std.process.exit(1);
    };
}

fn getCurrentGitCommit(allocator: std.mem.Allocator) ![]const u8 {
    const result = try std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = &.{ "git", "rev-parse", "HEAD" },
        .cwd = (comptime thisDir()),
    });
    if (result.stdout.len > 0) return result.stdout[0 .. result.stdout.len - 1]; // trim newline
    return result.stdout;
}

fn ensureCanDownloadFiles(allocator: std.mem.Allocator) void {
    const argv = &[_][]const u8{ "curl", "--version" };
    const result = std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = argv,
        .cwd = (comptime thisDir()),
    }) catch { // e.g. FileNotFound
        std.log.err("mach: error: 'curl --version' failed. Is curl not installed?", .{});
        std.process.exit(1);
    };
    defer {
        allocator.free(result.stderr);
        allocator.free(result.stdout);
    }
    if (result.term.Exited != 0) {
        std.log.err("mach: error: 'curl --version' failed. Is curl not installed?", .{});
        std.process.exit(1);
    }
}

fn downloadBinary(
    allocator: std.mem.Allocator,
    commit_cache_dir: []const u8,
    release_tag: []const u8,
    target_cache_dir: []const u8,
    zig_triple: []const u8,
    is_windows: bool,
    version: []const u8,
) !void {
    ensureCanDownloadFiles(allocator);

    const download_dir = try std.fs.path.join(allocator, &.{ target_cache_dir, "download" });
    try std.fs.cwd().makePath(download_dir);

    // Replace "..." with "---" because GitHub releases has very weird restrictions on file names.
    // https://twitter.com/slimsag/status/1498025997987315713
    const github_triple = try std.mem.replaceOwned(u8, allocator, zig_triple, "...", "---");

    // Compose the download URL, e.g.:
    const lib_prefix = if (is_windows) "dawn_" else "libdawn_";
    const lib_ext = if (is_windows) ".lib" else ".a";
    const lib_file_name = if (is_windows) "dawn.lib" else "libdawn.a";
    const download_url = try std.mem.concat(allocator, u8, &.{
        "https://github.com/hexops/mach-gpu-dawn/releases/download/",
        version,
        "/",
        lib_prefix,
        github_triple,
        "_",
        release_tag,
        lib_ext,
        ".gz",
    });

    // Download and decompress libdawn
    const gz_target_file = try std.fs.path.join(allocator, &.{ download_dir, "compressed.gz" });
    try downloadFile(allocator, gz_target_file, download_url);
    const target_file = try std.fs.path.join(allocator, &.{ target_cache_dir, lib_file_name });
    try gzipDecompress(allocator, gz_target_file, target_file);

    // If we don't yet have the headers (these are shared across architectures), download them.
    const include_dir = try std.fs.path.join(allocator, &.{ commit_cache_dir, "include" });
    if (!dirExists(include_dir)) {
        // Compose the headers download URL, e.g.:
        // https://github.com/hexops/mach-gpu-dawn/releases/download/release-6b59025/headers.json.gz
        const headers_download_url = try std.mem.concat(allocator, u8, &.{
            "https://github.com/hexops/mach-gpu-dawn/releases/download/",
            version,
            "/headers.json.gz",
        });

        // Download and decompress headers.json.gz
        const headers_gz_target_file = try std.fs.path.join(
            allocator,
            &.{ download_dir, "headers.json.gz" },
        );
        try downloadFile(allocator, headers_gz_target_file, headers_download_url);
        const headers_target_file = try std.fs.path.join(
            allocator,
            &.{ target_cache_dir, "headers.json" },
        );
        try gzipDecompress(allocator, headers_gz_target_file, headers_target_file);

        // Extract headers JSON archive.
        try extractHeaders(allocator, headers_target_file, commit_cache_dir);
    }

    try std.fs.deleteTreeAbsolute(download_dir);
}

fn downloadFile(allocator: std.mem.Allocator, target_file: []const u8, url: []const u8) !void {
    std.debug.print("downloading {s}..\n", .{url});
    var child = std.ChildProcess.init(&.{ "curl", "-L", "-o", target_file, url }, allocator);
    child.cwd = (comptime thisDir());
    child.stderr = std.io.getStdErr();
    child.stdout = std.io.getStdOut();
    _ = try child.spawnAndWait();
}

fn extractHeaders(allocator: std.mem.Allocator, json_file: []const u8, out_dir: []const u8) !void {
    const contents = try std.fs.cwd().readFileAlloc(allocator, json_file, std.math.maxInt(usize));

    var parser = std.json.Parser.init(allocator, false);
    defer parser.deinit();
    var tree = try parser.parse(contents);
    defer tree.deinit();

    var iter = tree.root.Object.iterator();
    while (iter.next()) |f| {
        const out_path = try std.fs.path.join(allocator, &.{ out_dir, f.key_ptr.* });
        try std.fs.cwd().makePath(std.fs.path.dirname(out_path).?);

        var new_file = try std.fs.createFileAbsolute(out_path, .{});
        defer new_file.close();
        try new_file.writeAll(f.value_ptr.*.String);
    }
}

fn dirExists(path: []const u8) bool {
    var dir = std.fs.openDirAbsolute(path, .{}) catch return false;
    dir.close();
    return true;
}

fn gzipDecompress(
    allocator: std.mem.Allocator,
    src_absolute_path: []const u8,
    dst_absolute_path: []const u8,
) !void {
    var file = try std.fs.openFileAbsolute(src_absolute_path, .{ .mode = .read_only });
    defer file.close();

    var buf_stream = std.io.bufferedReader(file.reader());
    var gzip_stream = try std.compress.gzip.gzipStream(allocator, buf_stream.reader());
    defer gzip_stream.deinit();

    // Read and decompress the whole file
    const buf = try gzip_stream.reader().readAllAlloc(allocator, std.math.maxInt(usize));
    defer allocator.free(buf);

    var new_file = try std.fs.createFileAbsolute(dst_absolute_path, .{});
    defer new_file.close();

    try new_file.writeAll(buf);
}

fn gitBranchContainsCommit(
    allocator: std.mem.Allocator,
    branch: []const u8,
    commit: []const u8,
) !bool {
    const result = try std.ChildProcess.exec(.{
        .allocator = allocator,
        .argv = &.{ "git", "branch", branch, "--contains", commit },
        .cwd = (comptime thisDir()),
    });
    return result.term.Exited == 0;
}
