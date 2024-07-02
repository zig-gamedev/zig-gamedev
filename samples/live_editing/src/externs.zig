const std = @import("std");

const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");

const build_options = @import("build_options");
const reloadable_files = build_options.reloadable_files;
const exports_lib_path = build_options.exports_lib_path;

const ErrorSetEnum = @import("error_set_enum.zig");

pub const ExternEntry = struct {
    const Entry = @import("entry.zig");
    pub const Input = Entry.Input;

    pub const init_error = ErrorSetEnum.initFn(@TypeOf(Entry.init));
    pub const renderFrameD3d12_error = ErrorSetEnum.initFn(@TypeOf(Entry.renderFrameD3d12));

    dyn_lib: std.DynLib,

    function_table: struct {
        init: *const fn (allocator: *const std.mem.Allocator, gctx: *zd3d12.GraphicsContext, error_enum: *init_error.Enum, result: *Entry) callconv(.C) bool,
        deinit: *const fn (self: *Entry) callconv(.C) void,
        inputUpdated: *const fn (self: *Entry, input: Entry.Input) callconv(.C) void,
        renderFrameD3d12: *const fn (self: *Entry, error_enum: *renderFrameD3d12_error.Enum) callconv(.C) bool,
        postRenderFrame: *const fn (self: *Entry) callconv(.C) void,
    },

    entry: *Entry,

    const Self = @This();

    pub fn init(allocator: std.mem.Allocator, gctx: *zd3d12.GraphicsContext) (init_error.ErrorSet || std.DynLib.Error)!Self {
        var install_dir = install_dir: {
            const install_path = std.fs.path.dirname(exports_lib_path).?;
            break :install_dir try std.fs.cwd().openDir(install_path, .{});
        };
        defer install_dir.close();
        const exports_lib_file_name = std.fs.path.basename(exports_lib_path);

        const dll_path = try install_dir.readFileAlloc(allocator, exports_lib_file_name, 256);
        defer allocator.free(dll_path);

        var dyn_lib = try std.DynLib.open(dll_path);
        const function_table = .{
            .init = dyn_lib.lookup(*const fn (allocator: *const std.mem.Allocator, gctx: *zd3d12.GraphicsContext, error_enum: *init_error.Enum, result: *Entry) callconv(.C) bool, "Entry.init").?,
            .deinit = dyn_lib.lookup(*const fn (self: *Entry) callconv(.C) void, "Entry.deinit").?,
            .inputUpdated = dyn_lib.lookup(*const fn (self: *Entry, input: Entry.Input) callconv(.C) void, "Entry.inputUpdated").?,
            .renderFrameD3d12 = dyn_lib.lookup(*const fn (self: *Entry, error_enum: *renderFrameD3d12_error.Enum) callconv(.C) bool, "Entry.renderFrameD3d12").?,
            .postRenderFrame = dyn_lib.lookup(*const fn (self: *Entry) callconv(.C) void, "Entry.postRenderFrame").?,
        };

        var error_enum: init_error.Enum = undefined;
        const entry = try allocator.create(Entry);
        errdefer allocator.destroy(entry);

        if (!function_table.init(&allocator, gctx, &error_enum, entry)) {
            return init_error.enumToError(error_enum);
        }

        return .{
            .dyn_lib = dyn_lib,
            .function_table = function_table,
            .entry = entry,
        };
    }

    pub fn deinit(self: *Self, allocator: std.mem.Allocator) void {
        self.function_table.deinit(self.entry);
        allocator.destroy(self.entry);
        self.dyn_lib.close();
    }

    pub fn reload(self: *Self, allocator: std.mem.Allocator) !Self {
        const gctx = self.entry.gctx;
        self.deinit(allocator);
        return try init(allocator, gctx);
    }

    pub fn inputUpdated(self: *Self, input: Entry.Input) void {
        self.function_table.inputUpdated(self.entry, input);
    }

    pub fn renderFrameD3d12(self: *Self) renderFrameD3d12_error.ErrorSet!void {
        var error_enum: renderFrameD3d12_error.Enum = undefined;
        if (self.function_table.renderFrameD3d12(self.entry, &error_enum)) {
            return;
        } else {
            return renderFrameD3d12_error.enumToError(error_enum);
        }
    }

    pub fn postRenderFrame(self: *Self) void {
        self.function_table.postRenderFrame(self.entry);
    }
};

const WatchDir = @import("watch_dir.zig");
pub const Reloader = struct {
    watch_thread: std.Thread,
    done: *std.Thread.ResetEvent,
    new_update: bool = false,
    file_changed_at: std.StringHashMap(i64),

    pub fn initAlloc(allocator: std.mem.Allocator) !*Reloader {
        const self = try allocator.create(Reloader);
        const done = try allocator.create(std.Thread.ResetEvent);
        self.* = .{
            .watch_thread = try std.Thread.spawn(.{}, Reloader.watch, .{ allocator, self }),
            .done = done,
            .file_changed_at = std.StringHashMap(i64).init(allocator),
        };
        return self;
    }

    pub fn deinit(self: *Reloader, allocator: std.mem.Allocator) void {
        self.done.set();
        self.watch_thread.join();
        self.file_changed_at.deinit();
        allocator.destroy(self.done);
        allocator.destroy(self);
    }

    fn watch(allocator: std.mem.Allocator, self: *Reloader) !void {
        const src_path = comptime (std.fs.path.dirname(@src().file).?);
        const cwd_path = try std.fs.path.resolve(allocator, &.{ src_path, ".." });
        defer allocator.free(cwd_path);

        var src_dir = try std.fs.cwd().openDir(src_path, .{});
        defer src_dir.close();

        var watch_dir = try WatchDir.init(
            allocator,
            src_path,
            std.os.windows.FILE_NOTIFY_CHANGE_LAST_WRITE,
        );
        defer watch_dir.deinit(allocator);

        const reloadable_file_set = comptime std.StaticStringMap(void).initComptime(reloadable_file_list: {
            var reloadable_file_list: [reloadable_files.len]struct { []const u8, void } = undefined;
            for (reloadable_files, 0..) |reloadable_file, i| {
                reloadable_file_list[i] = .{ reloadable_file, {} };
            }
            break :reloadable_file_list reloadable_file_list;
        });
        while (!self.done.isSet()) {
            var watch_dir_it = try watch_dir.waitForIterator(100);
            defer watch_dir_it.deinit();
            var i: usize = 0;
            while (watch_dir_it.next()) |e| : (i += 1) {
                const file_name_w = e.getFileName();
                const file_name = try std.unicode.wtf16LeToWtf8Alloc(allocator, file_name_w);

                if (reloadable_file_set.has(file_name)) {
                    if (self.file_changed_at.get(file_name)) |_| {
                        allocator.free(file_name);
                    } else {
                        try self.file_changed_at.put(file_name, std.time.milliTimestamp());
                    }
                }
            }

            var handled_files = std.ArrayList([]const u8).init(allocator);
            defer handled_files.deinit();

            const debounce = 300;
            var file_changed_at_it = self.file_changed_at.iterator();
            while (file_changed_at_it.next()) |file_changed_at_entry| {
                const start = file_changed_at_entry.value_ptr.*;
                if (std.time.milliTimestamp() - start < debounce) {
                    continue;
                }
                const file_name = file_changed_at_entry.key_ptr.*;
                try handled_files.append(file_name);

                const result = try std.ChildProcess.run(.{
                    .allocator = allocator,
                    .argv = &.{ "zig", "build", "-Dlive-editing-exports-only=true", "live_editing" },
                });
                defer {
                    allocator.free(result.stdout);
                    allocator.free(result.stderr);
                }
                switch (result.term) {
                    .Exited => |exit_code| if (exit_code == 0) {} else {
                        std.debug.print("{s} changed. Build failed: {s}", .{ file_name, result.stderr });
                        continue;
                    },
                    else => continue,
                }

                @atomicStore(bool, &self.new_update, true, .release);
                std.debug.print("{s} changed. Updated in {} ms\n", .{ file_name, std.time.milliTimestamp() - start });
            }

            for (handled_files.items) |file_name| {
                _ = self.file_changed_at.remove(file_name);
                allocator.free(file_name);
            }
        }
    }

    pub fn update(self: *Reloader, allocator: std.mem.Allocator, extern_entry: *ExternEntry) !ExternEntry {
        if (@atomicLoad(bool, &self.new_update, .acquire)) {
            @setCold(true);
            defer {
                @atomicStore(bool, &self.new_update, false, .release);
            }
            return extern_entry.reload(allocator);
        } else {
            return extern_entry.*;
        }
    }
};
