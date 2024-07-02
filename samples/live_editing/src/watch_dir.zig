const std = @import("std");

const kernel32 = std.os.windows.kernel32;

pub fn getVLAFileName(comptime T: type, t: *T) []std.os.windows.WCHAR {
    comptime var file_name_offset = 0;
    switch (@typeInfo(T)) {
        .Struct => |struct_info| {
            if (struct_info.layout != .@"extern") {
                @compileError("expected extern struct but was " ++ @tagName(struct_info.layout));
            }
            inline for (struct_info.fields) |field| {
                if (std.mem.eql(u8, field.name, "FileName")) {
                    break;
                }
                file_name_offset += @sizeOf(field.type);
            }
        },
        else => @compileError("expected T to be a struct but was " ++ @typeName(T)),
    }
    const file_name_ptr: [*]std.os.windows.WCHAR = @ptrFromInt(@intFromPtr(t) + file_name_offset);
    const file_name_length_in_bytes = @field(t, "FileNameLength");
    const file_name_length_in_wchars = file_name_length_in_bytes / @sizeOf(std.os.windows.WCHAR);
    return file_name_ptr[0..file_name_length_in_wchars];
}

// workaround https://github.com/ziglang/zig/issues/19946
pub extern "kernel32" fn CreateEventExW(
    lpEventAttributes: ?*std.os.windows.SECURITY_ATTRIBUTES,
    lpName: ?std.os.windows.LPCWSTR,
    dwFlags: std.os.windows.DWORD,
    dwDesiredAccess: std.os.windows.DWORD,
) callconv(std.os.windows.WINAPI) ?std.os.windows.HANDLE;

pub fn WorkaroundCreateEventExW(attributes: ?*std.os.windows.SECURITY_ATTRIBUTES, nameW: ?std.os.windows.LPCWSTR, flags: std.os.windows.DWORD, desired_access: std.os.windows.DWORD) !std.os.windows.HANDLE {
    const handle = CreateEventExW(attributes, nameW, flags, desired_access);
    if (handle) |h| {
        return h;
    } else {
        switch (kernel32.GetLastError()) {
            else => |err| return std.os.windows.unexpectedError(err),
        }
    }
}

notify_filter: std.os.windows.DWORD,
dir_handle: std.os.windows.HANDLE,
file_notify_info_buffer: []align(@sizeOf(std.os.windows.DWORD)) u8,
overlapped: *std.os.windows.OVERLAPPED,

const Self = @This();

pub fn init(allocator: std.mem.Allocator, dir: []const u8, notify_filter: std.os.windows.DWORD) !Self {
    const dir_w = try std.unicode.utf8ToUtf16LeAllocZ(allocator, dir);
    defer allocator.free(dir_w);

    const dir_handle = kernel32.CreateFileW(
        dir_w,
        std.os.windows.FILE_LIST_DIRECTORY,
        std.os.windows.FILE_SHARE_READ | std.os.windows.FILE_SHARE_WRITE | std.os.windows.FILE_SHARE_DELETE,
        null,
        std.os.windows.OPEN_EXISTING,
        std.os.windows.FILE_FLAG_BACKUP_SEMANTICS | // for ReadDirectoryChangesW
            std.os.windows.FILE_FLAG_OVERLAPPED, // for GetOverlappedResult
        null,
    );

    const file_notify_info_buffer = try allocator.alignedAlloc(u8, @sizeOf(std.os.windows.DWORD), 1024);

    var overlapped = try allocator.create(std.os.windows.OVERLAPPED);
    overlapped.hEvent = try WorkaroundCreateEventExW(
        null,
        null,
        0,
        std.os.windows.SYNCHRONIZE | // for WaitForSingleObject
            std.os.windows.EVENT_MODIFY_STATE, // for ReadDirectoryChangesW
    );

    std.debug.assert(kernel32.ReadDirectoryChangesW(
        dir_handle,
        file_notify_info_buffer.ptr,
        @intCast(file_notify_info_buffer.len),
        std.os.windows.FALSE,
        notify_filter,
        null,
        overlapped,
        null,
    ) == std.os.windows.TRUE);

    return .{
        .notify_filter = notify_filter,
        .dir_handle = dir_handle,
        .file_notify_info_buffer = file_notify_info_buffer,
        .overlapped = overlapped,
    };
}

pub fn deinit(self: Self, allocator: std.mem.Allocator) void {
    _ = std.os.windows.CloseHandle(self.overlapped.hEvent.?);
    allocator.destroy(self.overlapped);
    allocator.free(self.file_notify_info_buffer);
    _ = std.os.windows.CloseHandle(self.dir_handle);
}

pub fn waitForIterator(self: *Self, milliseconds: std.os.windows.DWORD) !Iterator {
    if (std.os.windows.WaitForSingleObject(self.overlapped.hEvent.?, milliseconds)) {
        const bytes_transfered = try std.os.windows.GetOverlappedResult(self.dir_handle, self.overlapped, false);
        if (bytes_transfered > 0) {
            return .{
                .self = self,
            };
        } else {
            return .{
                .self = self,
                .index = null,
            };
        }
    } else |err| switch (err) {
        error.WaitTimeOut => return .{
            .self = self,
            .index = null,
        },
        else => return err,
    }
}

pub const Entry = struct {
    file_notify_info: *align(@sizeOf(std.os.windows.DWORD)) std.os.windows.FILE_NOTIFY_INFORMATION,

    pub fn getFileName(entry: Entry) []const std.os.windows.WCHAR {
        return getVLAFileName(std.os.windows.FILE_NOTIFY_INFORMATION, entry.file_notify_info);
    }
};

pub const Iterator = struct {
    self: *Self,
    index: ?std.os.windows.DWORD = 0,

    pub fn deinit(it: *Iterator) void {
        std.debug.assert(kernel32.ReadDirectoryChangesW(
            it.self.dir_handle,
            it.self.file_notify_info_buffer.ptr,
            @intCast(it.self.file_notify_info_buffer.len),
            std.os.windows.FALSE,
            it.self.notify_filter,
            null,
            it.self.overlapped,
            null,
        ) == std.os.windows.TRUE);
    }

    pub fn next(it: *Iterator) ?Entry {
        if (it.index) |index| {
            const file_notify_info: *align(@sizeOf(std.os.windows.DWORD)) std.os.windows.FILE_NOTIFY_INFORMATION = @ptrFromInt(@intFromPtr(it.self.file_notify_info_buffer.ptr) + index);

            if (file_notify_info.NextEntryOffset == 0) {
                it.index = null;
            } else {
                it.index = file_notify_info.NextEntryOffset;
            }
            return .{
                .file_notify_info = file_notify_info,
            };
        } else {
            return null;
        }
    }
};
