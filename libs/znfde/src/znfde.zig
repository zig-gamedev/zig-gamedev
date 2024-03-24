const std = @import("std");
const c = @import("c.zig");
const log = std.log.scoped(.znfde);

pub const Error = error{
    ZnfdeError,
};

pub fn makeError() Error {
    if (c.NFD_GetError()) |ptr| {
        log.err("{s}\n", .{
            std.mem.sliceTo(ptr, 0),
        });
    }
    return error.ZnfdeError;
}

const FilterItem = extern struct {
    name: [*:0]const u8,
    spec: [*:0]const u8,
};

/// Open single file dialog
pub fn openFileDialog(allocator: std.mem.Allocator, filter: ?[]const FilterItem, default_path: ?[:0]const u8) !?[:0]const u8 {
    var out_path: [*c]u8 = null;

    const result = c.NFD_OpenDialog(
        &out_path,
        if (filter) |f| @ptrCast(f.ptr) else null,
        if (filter) |f| @intCast(f.len) else 0,
        if (default_path != null) default_path.? else null,
    );

    return switch (result) {
        c.NFD_OKAY => {
            if (out_path == null) {
                return null;
            }

            defer std.c.free(out_path);
            return try allocator.dupeZ(u8, std.mem.sliceTo(out_path, 0));
        },
        c.NFD_ERROR => makeError(),
        else => null,
    };
}

/// Open save dialog
pub fn saveFileDialog(allocator: std.mem.Allocator, filter: ?[]const FilterItem, default_path: ?[:0]const u8, default_name: ?[:0]const u8) !?[:0]const u8 {
    var out_path: [*c]u8 = null;

    const result = c.NFD_SaveDialog(
        &out_path,
        if (filter) |f| @ptrCast(f.ptr) else null,
        if (filter) |f| @intCast(f.len) else 0,
        if (default_path != null) default_path.? else null,
        if (default_name != null) default_name.? else null,
    );

    return switch (result) {
        c.NFD_OKAY => {
            if (out_path == null) {
                return null;
            }

            defer std.c.free(out_path);
            return try allocator.dupeZ(u8, std.mem.sliceTo(out_path, 0));
        },
        c.NFD_ERROR => makeError(),
        else => null,
    };
}

/// Open folder dialog
pub fn openFolderDialog(allocator: std.mem.Allocator, default_path: ?[:0]const u8) !?[:0]const u8 {
    var out_path: [*c]u8 = null;

    const result = c.NFD_PickFolder(&out_path, if (default_path != null) default_path.?.ptr else null);

    return switch (result) {
        c.NFD_OKAY => {
            if (out_path == null) {
                return null;
            }
            defer std.c.free(out_path);
            return try allocator.dupeZ(u8, std.mem.sliceTo(out_path, 0));
        },
        c.NFD_ERROR => makeError(),
        else => null,
    };
}

pub fn init() !void {
    const result = c.NFD_Init();
    return switch (result) {
        c.NFD_ERROR => makeError(),
        else => {},
    };
}

pub fn deinit() void {
    c.NFD_Quit();
}
