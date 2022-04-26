const std = @import("std");
const Mutex = std.Thread.Mutex;

pub fn init(alloc: std.mem.Allocator) void {
    std.debug.assert(allocator == null and allocations == null);
    allocator = alloc;
    allocations = std.AutoHashMap(usize, usize).init(allocator.?);
    allocations.?.ensureTotalCapacity(32) catch unreachable;
    zmesh_setAllocator(zmeshAlloc, zmeshClearAlloc, zmeshReAlloc, zmeshFree);
    meshopt_setAllocator(zmeshAlloc, zmeshFree);
}

pub fn deinit() void {
    allocations.?.deinit();
    allocations = null;
    allocator = null;
}

extern fn zmesh_setAllocator(
    malloc: fn (size: usize) callconv(.C) ?*anyopaque,
    calloc: fn (num: usize, size: usize) callconv(.C) ?*anyopaque,
    realloc: fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque,
    free: fn (ptr: ?*anyopaque) callconv(.C) void,
) void;

extern fn meshopt_setAllocator(
    allocate: fn (size: usize) callconv(.C) ?*anyopaque,
    deallocate: fn (ptr: ?*anyopaque) callconv(.C) void,
) void;

var allocator: ?std.mem.Allocator = null;
var allocations: ?std.AutoHashMap(usize, usize) = null;
var mutex: Mutex = .{};

export fn zmeshAlloc(size: usize) callconv(.C) ?*anyopaque {
    mutex.lock();
    defer mutex.unlock();

    var slice = allocator.?.allocBytes(
        @sizeOf(usize),
        size,
        0,
        @returnAddress(),
    ) catch @panic("zmesh: out of memory");

    allocations.?.put(@ptrToInt(slice.ptr), size) catch
        @panic("zmesh: out of memory");

    return slice.ptr;
}

export fn zmeshClearAlloc(num: usize, size: usize) callconv(.C) ?*anyopaque {
    const ptr = zmeshAlloc(num * size);
    if (ptr != null) {
        @memset(@ptrCast([*]u8, ptr), 0, num * size);
        return ptr;
    }
    return null;
}

pub export fn zmeshAllocUser(user: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque {
    _ = user;
    return zmeshAlloc(size);
}

export fn zmeshReAlloc(ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque {
    mutex.lock();
    defer mutex.unlock();

    const old_len = if (ptr != null)
        allocations.?.get(@ptrToInt(ptr.?)).?
    else
        0;

    var old_mem = if (old_len > 0)
        @ptrCast([*]u8, ptr)[0..old_len]
    else
        @as([*]u8, undefined)[0..0];

    var slice = allocator.?.reallocBytes(
        old_mem,
        @sizeOf(usize),
        size,
        @sizeOf(usize),
        0,
        @returnAddress(),
    ) catch @panic("zmesh: out of memory");

    if (ptr != null) {
        const removed = allocations.?.remove(@ptrToInt(ptr.?));
        std.debug.assert(removed);
    }

    allocations.?.put(@ptrToInt(slice.ptr), size) catch
        @panic("zmesh: out of memory");

    return slice.ptr;
}

export fn zmeshFree(ptr: ?*anyopaque) callconv(.C) void {
    if (ptr != null) {
        mutex.lock();
        defer mutex.unlock();

        const size = allocations.?.fetchRemove(@ptrToInt(ptr.?)).?.value;
        const slice = @ptrCast([*]u8, ptr.?)[0..size];
        allocator.?.free(slice);
    }
}

pub export fn zmeshFreeUser(user: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void {
    _ = user;
    zmeshFree(ptr);
}
