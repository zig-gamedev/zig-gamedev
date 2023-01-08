const std = @import("std");

pub fn pkg(b: *std.build.Builder) std.build.Pkg {
    const cached_result = struct {
        var pkg: ?std.build.Pkg = null;
    };
    if (cached_result.pkg == null) {
        cached_result.pkg = .{
            .name = "zmath",
            .source = .{ .path = projectPath(b, "src/main.zig") },
        };
    }
    return cached_result.pkg.?;
}

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zmath tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(projectPath(b, "src/main.zig"));
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    return tests;
}

pub fn buildBenchmarks(
    b: *std.build.Builder,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const exe = b.addExecutable("benchmark", projectPath(b, "src/benchmark.zig"));
    exe.setBuildMode(std.builtin.Mode.ReleaseFast);
    exe.setTarget(target);
    exe.addPackage(pkg(b));
    return exe;
}

const projectPath = (struct {
    inline fn projectRoot() []const u8 {
        return comptime std.fs.path.dirname(@src().file) orelse ".";
    }

    inline fn projectPath(allocator: anytype, comptime suffix: []const u8) []const u8 {
        if (@TypeOf(allocator) == std.mem.Allocator) {
            return resolvePath(allocator, suffix.len, suffix[0..suffix.len].*);
        } else {
            return resolvePath(allocator.allocator, suffix.len, suffix[0..suffix.len].*);
        }
    }

    fn cwd(allocator: std.mem.Allocator) []const u8 {
        const cached_result = struct {
            var resolved_path: ?[]const u8 = null;
        };
        if (cached_result.resolved_path == null) {
            cached_result.resolved_path = std.process.getCwdAlloc(allocator) catch unreachable;
        }
        return cached_result.resolved_path.?;
    }

    fn resolvePath(allocator: std.mem.Allocator, comptime len: usize, comptime suffix: [len]u8) []const u8 {
        const project_path = projectRoot() ++ "/" ++ suffix[0..];
        if (comptime std.fs.path.isAbsolute(project_path)) {
            return project_path;
        }
        const cached_result = struct {
            var resolved_path: ?[]const u8 = null;
        };
        if (cached_result.resolved_path == null) {
            cached_result.resolved_path = std.fs.path.resolve(
                allocator,
                &.{ cwd(allocator), project_path },
            ) catch unreachable;
        }
        return cached_result.resolved_path.?;
    }
}).projectPath;
