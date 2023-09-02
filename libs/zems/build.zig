const std = @import("std");

pub const Package = struct {
    module: *std.Build.Module,
    emscripten: bool,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zems", pkg.module);
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    args: struct {},
) Package {
    _ = optimize;
    _ = args;
    const emscripten = target.getOsTag() == .emscripten;
    const src_root = if (emscripten) "zems.zig" else "dummy.zig";
    var module = b.createModule(.{
        .source_file = .{ .path = b.pathJoin(&.{ thisDir(), src_root }) },
    });
    return .{
        .module = module,
        .emscripten = emscripten,
    };
}

//
// Build utils
//

pub const EmscriptenStep = struct {
    b: *std.Build.Builder,
    args: EmscriptenArgs,
    out_path: ?[]const u8 = null, // zig-out/web/<exe-name> if unset
    lib_exe: ?*std.Build.CompileStep = null,
    link_step: ?*std.Build.Step.Run = null,
    emsdk_path: []const u8,
    emsdk_include_path: []const u8,

    pub fn init(b: *std.Build.Builder) *@This() {
        const emsdk_path = b.env_map.get("EMSDK") orelse @panic("Failed to get emscripten SDK path, have you installed & sourced the SDK?");
        var r = b.allocator.create(EmscriptenStep) catch unreachable;
        r.* = .{
            .b = b,
            .args = EmscriptenArgs.init(b.allocator),
            .emsdk_path = emsdk_path,
            .emsdk_include_path = b.pathJoin(&.{ emsdk_path, "upstream", "emscripten", "cache", "sysroot", "include" }),
        };
        return r;
    }

    pub fn link(self: *@This(), exe: *std.Build.CompileStep) void {
        std.debug.assert(self.lib_exe == null);
        std.debug.assert(self.link_step == null);
        const b = self.b;

        exe.addSystemIncludePath(.{ .path = self.emsdk_include_path });
        exe.stack_protector = false;
        exe.disable_stack_probing = true;
        exe.linkLibC();

        const emlink = b.addSystemCommand(&.{"emcc"});
        emlink.addArtifactArg(exe);
        for (exe.link_objects.items) |link_dependency| {
            switch (link_dependency) {
                .other_step => |o| emlink.addArtifactArg(o),
                // .c_source_file => |f| emlink.addFileSourceArg(f.source), // f.args?
                // .c_source_files => |fs| for (fs.files) |f| emlink.addArg(f), // fs.flags?
                else => {},
            }
        }
        const out_path: []const u8 = self.out_path orelse b.pathJoin(&.{ b.pathFromRoot("."), "zig-out", "web", exe.name });
        std.fs.cwd().makePath(out_path) catch unreachable;
        const out_file = std.mem.concat(b.allocator, u8, &.{ out_path, std.fs.path.sep_str ++ "index.html" }) catch unreachable;
        emlink.addArgs(&.{ "-o", out_file });

        if (self.args.exported_functions.items.len > 0) {
            var s = std.mem.join(self.b.allocator, "','", self.args.exported_functions.items) catch unreachable;
            var ss = std.fmt.allocPrint(self.b.allocator, "-sEXPORTED_FUNCTIONS=['{s}']", .{s}) catch unreachable;
            emlink.addArg(ss);
        }

        var op_it = self.args.options.iterator();
        while (op_it.next()) |opt| {
            if (opt.value_ptr.*.len > 0) {
                emlink.addArg(std.mem.join(b.allocator, "", &.{ "-s", opt.key_ptr.*, "=", opt.value_ptr.* }) catch unreachable);
            } else {
                emlink.addArg(std.mem.join(b.allocator, "", &.{ "-s", opt.key_ptr.* }) catch unreachable);
            }
        }

        emlink.addArgs(self.args.other_args.items);

        if (self.args.shell_file) |sf| emlink.addArgs(&.{ "--shell-file", sf });

        emlink.step.dependOn(&exe.step);
        self.link_step = emlink;
        self.lib_exe = exe;
    }
};

pub const EmscriptenArgs = struct {
    source_map_base: []const u8 = "./",
    shell_file: ?[]const u8 = null,

    exported_functions: std.ArrayList([]const u8),
    options: std.StringHashMap([]const u8), // in args formated as -sKEY=VALUE
    other_args: std.ArrayList([]const u8),

    pub fn init(alloc: std.mem.Allocator) @This() {
        return .{
            .exported_functions = std.ArrayList([]const u8).init(alloc),
            .options = std.StringHashMap([]const u8).init(alloc),
            .other_args = std.ArrayList([]const u8).init(alloc),
        };
    }

    /// set common args
    /// param max_optimize - use maximum optimizations which are much slower to compile
    pub fn setDefault(self: *@This(), build_mode: std.builtin.Mode, max_optimizations: bool) void {
        //_ = r.options.fetchPut("FILESYSTEM", "0") catch unreachable;
        _ = self.options.getOrPutValue("ASYNCIFY", "") catch unreachable;
        _ = self.options.getOrPutValue("EXIT_RUNTIME", "0") catch unreachable;
        _ = self.options.getOrPutValue("WASM_BIGINT", "") catch unreachable;
        _ = self.options.getOrPutValue("MALLOC", "emmalloc") catch unreachable;
        _ = self.options.getOrPutValue("ABORTING_MALLOC", "0") catch unreachable;
        _ = self.options.getOrPutValue("INITIAL_MEMORY", "64MB") catch unreachable;
        _ = self.options.getOrPutValue("ALLOW_MEMORY_GROWTH", "1") catch unreachable;

        self.shell_file = thisDir() ++ std.fs.path.sep_str ++ "shell_minimal.html";
        self.exported_functions.appendSlice(&.{ "_main", "_malloc", "_free" }) catch unreachable;

        self.other_args.append("-fno-rtti") catch unreachable;
        self.other_args.append("-fno-exceptions") catch unreachable;
        switch (build_mode) {
            .Debug => {
                self.other_args.append("-g") catch unreachable;
                self.other_args.appendSlice(&.{ "--closure", "0" }) catch unreachable;
                const source_map_base: []const u8 = "./";
                self.other_args.appendSlice(&.{ "-gsource-map", "--source-map-base", source_map_base }) catch unreachable;
            },
            .ReleaseSmall => {
                if (max_optimizations) self.other_args.append("-Oz") catch unreachable else self.other_args.append("-Os") catch unreachable;
            },
            .ReleaseSafe, .ReleaseFast => {
                if (max_optimizations) {
                    self.other_args.append("-O3") catch unreachable;
                    self.other_args.append("-flto") catch unreachable;
                } else {
                    self.other_args.append("-O2") catch unreachable;
                }
            },
        }
    }

    /// sets option argument to specific value if its not set or assert if value differs
    pub fn setOrAssertOption(self: *@This(), key: []const u8, value: []const u8) void {
        var v = self.options.getOrPut(key) catch unreachable;
        if (v.found_existing) {
            if (!std.ascii.eqlIgnoreCase(v.value_ptr.*, value)) {
                std.debug.panic("Emscripten argument conflict: want `{s}` to be `{s}` but `{s}` was already set", .{ key, value, v.key_ptr.* });
            }
        } else {
            v.value_ptr.* = value;
        }
    }

    pub fn overwriteOption(self: *@This(), key: []const u8, value: []const u8) void {
        _ = self.options.fetchPut(key, value);
    }
};

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
