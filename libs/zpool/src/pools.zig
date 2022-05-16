const std = @import("std");

/// Returns a struct that maintains a pool of data.  Handles returned by
/// `Pool.add()` can be used to get/set the data in zero or more columns.
///
/// See `handles.zig` for more information on `index_bits` and `cycle_bits`,
/// and handles in general.
///
/// `TResource` identifies type of resource referenced by a handle, and
/// provides a type-safe distinction between two otherwise equivalently
/// configured `Handle` types, such as:
/// * `const BufferHandle  = Handle(16, 16, Buffer);`
/// * `const TextureHandle = Handle(16, 16, Texture);`
///
/// `TColumns` is a struct that defines the column names, and the element types
/// of the column arrays.
///
/// ```zig
/// const Texture = gpu.Texture;
///
/// const TexturePool = Pool(16, 16, Texture, struct { obj:Texture, desc:Texture.Descriptor });
/// const TextureHandle = TexturePool.Handle;
///
/// const GPA = std.heap.GeneralPurposeAllocator;
/// var gpa = GPA(.{}){};
/// var pool = try TestPool.initMaxCapacity(gpa.allocator());
/// defer pool.deinit();
///
/// // creating a texture and adding it to the pool returns a handle
/// const desc : Texture.Descriptor = .{ ... };
/// const obj = device.createTexture(desc);
/// const handle : TextureHandle = pool.add(.{ .obj = obj, .desc = desc });
/// 
/// // elsewhere, use the handle to get `obj` or `desc` as needed
/// const obj = pool.getColumn(handle, .obj);
/// const desc = pool.getColumn(handle, .desc);
/// ```
pub fn Pool(
    comptime index_bits : u8,
    comptime cycle_bits : u8,
    comptime TResource  : type,
    comptime TColumns   : type,
) type {
    // Handle performs compile time checks on index_bits & cycle_bits
    const handles = @import("handles.zig");
    const THandle = handles.Handle(index_bits, cycle_bits, TResource);

    const utils = @import("utils.zig");
    const isStruct = utils.isStruct;

    if (!isStruct(TColumns)) @compileError("TColumns must be a struct");

    const meta = std.meta;
    const Allocator = std.mem.Allocator;
    const MultiArrayList = std.MultiArrayList;
    const StructOfSlices = utils.StructOfSlices;

    return struct {
        const Self = @This();

        pub const Error = error {
            PoolIsFull,
            HandleIsUnacquired,
            HandleIsOutOfBounds,
            HandleIsReleased,
        };

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub const Resource = TResource;
        pub const Handle   = THandle;

        pub const AddressableHandle = Handle.AddressableHandle;
        pub const AddressableIndex  = Handle.AddressableIndex;
        pub const AddressableCycle  = Handle.AddressableCycle;

        pub const max_index    : usize = Handle.max_index;
        pub const max_cycle    : usize = Handle.max_cycle;
        pub const max_capacity : usize = Handle.max_count;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub const Columns      = TColumns;
        pub const ColumnSlices = StructOfSlices(Columns);
        pub const Column       = meta.FieldEnum(Columns);

        pub const column_fields = meta.fields(Columns);
        pub const column_count = column_fields.len;

        pub fn ColumnType(comptime column: Column) type {
            return meta.fieldInfo(Columns, column).field_type;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const private_fields = meta.fields(struct {
            @"Pool._free_stack" : AddressableIndex,
            @"Pool._curr_cycle" : AddressableCycle,
        });

        const Storage = MultiArrayList(@Type(.{.Struct = .{
            .layout   = std.builtin.Type.ContainerLayout.Auto,
            .fields   = private_fields ++ column_fields,
            .decls    = &.{},
            .is_tuple = false,
        }}));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        _allocator  : Allocator          = undefined,
        _storage    : Storage            = .{},
        _free_count : usize              = 0,
        _free_stack : []AddressableIndex = &.{},
        _curr_cycle : []AddressableCycle = &.{},
        columns     : ColumnSlices       = undefined,

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn init(allocator: Allocator) Self {
            var self = Self { ._allocator = allocator };
            updateSlices(self);
            return self;
        }

        pub fn initCapacity(allocator: Allocator, min_capacity: usize) !Self {
            var self = Self { ._allocator = allocator };
            try self.reserve(min_capacity);
            return self;
        }

        pub fn initMaxCapacity(allocator: Allocator) !Self {
            return initCapacity(allocator, max_capacity);
        }

        pub fn deinit(self: *Self) void {
            self.clear();
            self._storage.deinit(self._allocator);
            self.* = .{};
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn capacity(self: Self) usize {
            return self._storage.capacity;
        }

        pub fn reserve(self: *Self, min_capacity: usize) !void {
            const old_capacity = self._storage.capacity;
            if (min_capacity <= old_capacity)
                return;

            if (min_capacity > max_capacity)
                return Error.PoolIsFull;

            try self._storage.setCapacity(self._allocator, min_capacity);
            updateSlices(self);
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn liveHandleCount(self: Self) usize {
            return self._storage.len - self._free_count;
        }

        pub fn isLiveHandle(self: Self, handle: Handle) bool {
            return isLiveAddressableHandle(self, handle.addressable());
        }

        pub fn validateHandle(self: Self, handle: Handle) Error!void {
            try validateAddressableHandle(self, handle.addressable());
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub const LiveIndexIterator = struct {
            curr_cycle: []const AddressableCycle = &.{},
            next_index: AddressableIndex = 0,

            pub fn next(self: *LiveIndexIterator) ?AddressableIndex {
                while (self.next_index < self.curr_cycle.len) {
                    const curr_index = self.next_index;
                    self.next_index += 1;
                    if (isLiveCycle(self.curr_cycle[curr_index]))
                        return curr_index;
                }
                return null;
            }
        };

        pub fn liveIndices(self: Self) LiveIndexIterator {
            return .{ .curr_cycle = self._curr_cycle };
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub const LiveHandleIterator = struct {
            live_indices : LiveIndexIterator = .{},

            pub fn next(self: *LiveHandleIterator) ?Handle {
                if (self.live_indices.next()) |index| {
                    const ahandle = AddressableHandle {
                        .index = index,
                        .cycle = self.live_indices.curr_cycle[index],
                    };
                    return ahandle.compact();
                }
                return null;
            }
        };

        pub fn liveHandles(self: Self) LiveHandleIterator {
            return .{ .live_indices = liveIndices(self) };
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn clear(self: *Self) void {
            var ahandle = AddressableHandle { .index = 0 };
            for (self._curr_cycle) |cycle| {
                if (isLiveCycle(cycle)) {
                    ahandle.cycle = cycle;
                    releaseAddressableHandleUnchecked(self, ahandle);
                }
                ahandle.index += 1;
            }
        }

        pub fn add(self: *Self, values: Columns) !Handle {
            const ahandle = try acquireAddressableHandle(self);
            inline for (column_fields) |field| {
                @field(self.columns, field.name)[ahandle.index] =
                     @field(values, field.name);
            }
            return ahandle.compact();
        }

        pub fn remove(self: *Self, handle: Handle) !void {
            try releaseAddressableHandle(self, handle.addressable());
        }

        pub fn removeIfLive(self: *Self, handle: Handle) void {
            var ahandle = handle.addressable();
            if (isLiveAddressableHandle(self.*, ahandle)) {
                releaseAddressableHandleUnchecked(self, ahandle);
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn getColumnPtr(
            self: Self,
            handle: Handle,
            comptime column: Column
        ) !*ColumnType(column) {
            const ahandle = handle.addressable();
            try validateAddressableHandle(self, ahandle);
            const column_field = meta.fieldInfo(Columns, column);
            return &@field(self.columns, column_field.name)[ahandle.index];
        }

        pub fn getColumn(
            self: Self,
            handle: Handle,
            comptime column: Column
        ) !ColumnType(column) {
            return (try getColumnPtr(self, handle, column)).*;
        }

        pub fn getColumns(self: Self, handle: Handle) !Columns {
            const ahandle = handle.addressable();
            try validateAddressableHandle(self, ahandle);
            var values : Columns = undefined;
            inline for (column_fields) |column_field| {
                @field(values, column_field.name) =
                    @field(self.columns, column_field.name)[ahandle.index];
            }
            return values;
        }

        pub fn setColumn(
            self: Self,
            handle: Handle,
            comptime column: Column,
            value: ColumnType(column)
        ) !void {
            const ahandle = handle.addressable();
            try validateAddressableHandle(self, ahandle);
            const column_field = meta.fieldInfo(Columns, column);
            deinitColumn(self, ahandle.index, column_field);
            @field(self.columns, column_field.name)[ahandle.index] = value;
        }

        pub fn setColumns(self: Self, handle:Handle, values: Columns) !void {
            const ahandle = handle.addressable();
            try validateAddressableHandle(self, ahandle);
            deinitColumns(self, ahandle.index);
            inline for (column_fields) |column_field| {
                @field(self.columns, column_field.name)[ahandle.index] =
                    @field(values, column_field.name);
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const StructField = std.builtin.Type.StructField;

        /// Call `value.deinit()` if defined.
        fn deinitColumn(
            self: Self,
            index: AddressableIndex,
            comptime column_field: StructField
        ) void {
            switch (@typeInfo(column_field.field_type)) {
                .Struct, .Enum, .Union, .Opaque => {
                    if (@hasDecl(column_field.field_type, "deinit")) {
                        @field(self.columns, column_field.name)[index].deinit();
                    }
                },
                else => {},
            }
        }

        /// Call `values.deinit()` if defined.
        fn deinitColumns(self: Self, index: AddressableIndex) void {
            if (@hasDecl(Columns, "deinit")) {
                var values : Columns = undefined;
                inline for (column_fields) |column_field| {
                    @field(values, column_field.name) =
                        @field(self.columns, column_field.name)[index];
                }
                values.deinit();
                inline for (column_fields) |column_field| {
                    @field(self.columns, column_field.name)[index] =
                        @field(values, column_field.name);
                }
            } else {
                inline for (column_fields) |column_field| {
                    deinitColumn(self, index, column_field);
                }
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn updateSlices(self: *Self) void {
            var slice = self._storage.slice();
            self._free_stack = slice.items(.@"Pool._free_stack");
            self._curr_cycle = slice.items(.@"Pool._curr_cycle");
            inline for (column_fields) |column_field, i| {
                const F = column_field.field_type;
                const p = slice.ptrs[private_fields.len + i];
                const f = @ptrCast([*]F, @alignCast(@alignOf(F), p));
                @field(self.columns, column_field.name) = f[0..slice.len];
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn isLiveAddressableHandle(
            self: Self,
            handle: AddressableHandle,
        ) bool {
            return isLiveCycle(handle.cycle)
               and handle.index < self._curr_cycle.len
               and handle.cycle == self._curr_cycle[handle.index];
        }

        fn validateAddressableHandle(
            self: Self,
            handle: AddressableHandle,
        ) Error!void {
            if (isFreeCycle(handle.cycle))
                return Error.HandleIsUnacquired;
            if (handle.index >= self._curr_cycle.len)
                return Error.HandleIsOutOfBounds;
            if (handle.cycle != self._curr_cycle[handle.index])
                return Error.HandleIsReleased;
        }

        fn acquireAddressableHandle(self: *Self) !AddressableHandle {
            var handle = AddressableHandle{};
            if (tryPopFreeIndex(self, &handle.index)) {
                handle.cycle = incrementAndReturnCycle(self, handle.index);
                try validateAddressableHandle(self.*, handle);
                return handle;
            }
            else if (self._storage.len < max_capacity) {
                handle.index = try issueNewIndex(self);
                handle.cycle = 1;
                try validateAddressableHandle(self.*, handle);
                return handle;
            }
            return Error.PoolIsFull;
        }

        fn releaseAddressableHandle(
            self: *Self,
            handle: AddressableHandle,
        ) !void {
            try validateAddressableHandle(self.*, handle);
            releaseAddressableHandleUnchecked(self, handle);
        }

        fn releaseAddressableHandleUnchecked(
            self: *Self,
            handle: AddressableHandle,
        ) void {
            deinitColumns(self.*, handle.index);
            incrementCycle(self, handle.index);
            pushFreeIndex(self, handle.index);
        }

        fn tryReleaseAddressableHandle(
            self: *Self,
            handle: AddressableHandle,
        ) bool {
            if (isLiveAddressableHandle(self.*, handle)) {
                releaseAddressableHandleUnchecked(self, handle);
                return true;
            }
            return false;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn isFreeCycle(cycle: AddressableCycle) bool {
            return (cycle & @as(AddressableCycle,1)) == @as(AddressableCycle,0);
        }

        fn isLiveCycle(cycle: AddressableCycle) bool {
            return (cycle & @as(AddressableCycle,1)) == @as(AddressableCycle,1);
        }

        fn incrementCycle(self: *Self, index: AddressableIndex) void {
            const new_cycle = self._curr_cycle[index] +% 1;
            self._curr_cycle[index] = new_cycle;
        }

        fn incrementAndReturnCycle(
            self: *Self,
            index: AddressableIndex,
        ) AddressableCycle {
            const new_cycle = self._curr_cycle[index] +% 1;
            self._curr_cycle[index] = new_cycle;
            return new_cycle;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn pushFreeIndex(self: *Self, index: AddressableIndex) void {
            const assert = std.debug.assert;
            assert(self._free_count < self._free_stack.len);
            assert(self._free_count < max_capacity);
            const addressable_index = @intCast(AddressableIndex, index);
            self._free_stack[self._free_count] = addressable_index;
            self._free_count += 1;
        }

        fn tryPopFreeIndex(self: *Self, index: *AddressableIndex) bool {
            if (self._free_count > 0) {
                self._free_count -= 1;
                index.* = self._free_stack[self._free_count];
                return true;
            }
            return false;
        }

        fn issueNewIndex(self: *Self) !AddressableIndex {
            const new_index = try self._storage.addOne(self._allocator);
            updateSlices(self);
            self._curr_cycle[new_index] = 1;
            return @intCast(AddressableIndex, new_index);
        }

    };
}

const expect = std.testing.expect;
const expectEqual = std.testing.expectEqual;
const expectError = std.testing.expectError;

const DeinitCounter = struct {
    const Self = @This();

    counter: *u32,

    fn init(_counter: *u32) Self {
        return Self { .counter = _counter };
    }

    fn deinit(self: *Self) void { self.counter.* += 1; }
};

test "Pool with no columns" {
    const TestPool = Pool(8,8, void, struct {});
    try expectEqual(@as(usize, 0), TestPool.column_count);
    try expectEqual(@as(usize, 0), @sizeOf(TestPool.ColumnSlices));

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    const handle = try pool.add(.{});
    defer pool.removeIfLive(handle);

    try pool.validateHandle(handle);
    try expect(pool.isLiveHandle(handle));
    try expectEqual(@as(u8, 0), handle.compact.index);
    try expectEqual(@as(u8, 1), handle.compact.cycle);
    try expectEqual(@as(usize, 1), pool.liveHandleCount());
}

test "Pool with one column" {
    const TestPool = Pool(8,8, void, struct { a: u32 });
    try expectEqual(@as(usize, 1), TestPool.column_count);
    try expectEqual(@sizeOf([]u32), @sizeOf(TestPool.ColumnSlices));

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    const handle = try pool.add(.{ .a = 123 });
    defer pool.removeIfLive(handle);

    try pool.validateHandle(handle);
    try expect(pool.isLiveHandle(handle));
    try expectEqual(@as(usize, 1), pool.liveHandleCount());
    try expectEqual(@as(u8, 0), handle.compact.index);
    try expectEqual(@as(u8, 1), handle.compact.cycle);

    try expectEqual(@as(u32, 123), try pool.getColumn(handle, .a));
    try pool.setColumn(handle, .a, 456);
    try expectEqual(@as(u32, 456), try pool.getColumn(handle, .a));
}

test "Pool with two columns" {
    const TestPool = Pool(8,8, void, struct { a: u32, b: u64 });
    try expectEqual(@as(usize, 2), TestPool.column_count);
    try expectEqual(@sizeOf([]u32) * 2, @sizeOf(TestPool.ColumnSlices));

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    const handle = try pool.add(.{ .a = 123, .b = 456 });
    defer pool.removeIfLive(handle);

    try pool.validateHandle(handle);
    try expect(pool.isLiveHandle(handle));
    try expectEqual(@as(usize, 1), pool.liveHandleCount());
    try expectEqual(@as(u8, 0), handle.compact.index);
    try expectEqual(@as(u8, 1), handle.compact.cycle);

    try expectEqual(@as(u32, 123), try pool.getColumn(handle, .a));
    try pool.setColumn(handle, .a, 456);
    try expectEqual(@as(u32, 456), try pool.getColumn(handle, .a));

    try expectEqual(@as(u64, 456), try pool.getColumn(handle, .b));
    try pool.setColumn(handle, .b, 123);
    try expectEqual(@as(u64, 123), try pool.getColumn(handle, .b));
}

test "Pool.liveHandleCount()" {
    const TestPool = Pool(8,8, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    try expectEqual(@as(usize, 1), pool.liveHandleCount());

    const handle1 = try pool.add(.{});
    try expectEqual(@as(usize, 2), pool.liveHandleCount());

    try pool.remove(handle0);
    try expectEqual(@as(usize, 1), pool.liveHandleCount());

    try pool.remove(handle1);
    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle2 = try pool.add(.{});
    try expectEqual(@as(usize, 1), pool.liveHandleCount());

    try pool.remove(handle2);
    try expectEqual(@as(usize, 0), pool.liveHandleCount());
}

test "Pool.isLiveHandle()" {
    const TestPool = Pool(8,8, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const unacquiredHandle = TestPool.Handle.init(0, 0);
    try expect(!pool.isLiveHandle(unacquiredHandle));

    const outOfBoundsHandle = TestPool.Handle.init(1, 1);
    try expect(!pool.isLiveHandle(outOfBoundsHandle));

    const handle = try pool.add(.{});
    try expect(pool.isLiveHandle(handle));

    try pool.remove(handle);
    try expect(!pool.isLiveHandle(handle));
}

test "Pool.validateHandle()" {
    const TestPool = Pool(8,8, void, struct {});
    try expectEqual(@as(usize, 0), TestPool.column_count);
    try expectEqual(@as(usize, 0), @sizeOf(TestPool.ColumnSlices));

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const unacquiredHandle = TestPool.Handle.init(0, 0);
    try expectError(
        TestPool.Error.HandleIsUnacquired,
        pool.validateHandle(unacquiredHandle));

    const outOfBoundsHandle = TestPool.Handle.init(1, 1);
    try expectError(
        TestPool.Error.HandleIsOutOfBounds,
        pool.validateHandle(outOfBoundsHandle));

    const handle = try pool.add(.{});
    try pool.validateHandle(handle);

    try pool.remove(handle);
    try expectError(
        TestPool.Error.HandleIsReleased,
        pool.validateHandle(handle));
}

test "Pool.liveIndices()" {
    const TestPool = Pool(8,8, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    try expectEqual(@as(usize, 3), pool.liveHandleCount());

    var live_indices = pool.liveIndices();
    try expectEqual(handle0.index(), live_indices.next().?);
    try expectEqual(handle1.index(), live_indices.next().?);
    try expectEqual(handle2.index(), live_indices.next().?);
    try expect(null == live_indices.next());
}

test "Pool.liveHandles()" {
    const TestPool = Pool(8,8, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    try expectEqual(@as(usize, 3), pool.liveHandleCount());

    var live_handles = pool.liveHandles();
    try expectEqual(handle0.value, live_handles.next().?.value);
    try expectEqual(handle1.value, live_handles.next().?.value);
    try expectEqual(handle2.value, live_handles.next().?.value);
    try expect(null == live_handles.next());
}

test "Pool.clear()" {
    const TestPool = Pool(8,8, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    try expectEqual(@as(usize, 3), pool.liveHandleCount());
    try expect(pool.isLiveHandle(handle0));
    try expect(pool.isLiveHandle(handle1));
    try expect(pool.isLiveHandle(handle2));

    pool.clear();
    try expectEqual(@as(usize, 0), pool.liveHandleCount());
    try expect(!pool.isLiveHandle(handle0));
    try expect(!pool.isLiveHandle(handle1));
    try expect(!pool.isLiveHandle(handle2));
}

test "Pool.add()" {
    const TestPool = Pool(2,6, void, struct {});
    try expectEqual(@sizeOf(u8), @sizeOf(TestPool.Handle));
    try expectEqual(@as(usize, 4), TestPool.max_capacity);

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    const handle3 = try pool.add(.{});
    try expectEqual(@as(usize, 4), pool.liveHandleCount());
    try expect(pool.isLiveHandle(handle0));
    try expect(pool.isLiveHandle(handle1));
    try expect(pool.isLiveHandle(handle2));
    try expect(pool.isLiveHandle(handle3));
    try expectError(TestPool.Error.PoolIsFull, pool.add(.{}));
}

test "Pool.remove()" {
    const TestPool = Pool(2,6, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    const handle3 = try pool.add(.{});
    try pool.remove(handle0);
    try pool.remove(handle1);
    try pool.remove(handle2);
    try pool.remove(handle3);
    try expectError(TestPool.Error.HandleIsReleased, pool.remove(handle0));
    try expectError(TestPool.Error.HandleIsReleased, pool.remove(handle1));
    try expectError(TestPool.Error.HandleIsReleased, pool.remove(handle2));
    try expectError(TestPool.Error.HandleIsReleased, pool.remove(handle3));
}

test "Pool.removeIfLive()" {
    const TestPool = Pool(2,6, void, struct {});

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{});
    const handle1 = try pool.add(.{});
    const handle2 = try pool.add(.{});
    const handle3 = try pool.add(.{});
    try expect(pool.isLiveHandle(handle0));
    try expect(pool.isLiveHandle(handle1));
    try expect(pool.isLiveHandle(handle2));
    try expect(pool.isLiveHandle(handle3));

    pool.removeIfLive(handle0);
    pool.removeIfLive(handle1);
    pool.removeIfLive(handle2);
    pool.removeIfLive(handle3);
    try expect(!pool.isLiveHandle(handle0));
    try expect(!pool.isLiveHandle(handle1));
    try expect(!pool.isLiveHandle(handle2));
    try expect(!pool.isLiveHandle(handle3));

    pool.removeIfLive(handle0);
    pool.removeIfLive(handle1);
    pool.removeIfLive(handle2);
    pool.removeIfLive(handle3);
}

test "Pool.getColumnPtr()" {
    const TestPool = Pool(2,6, void, struct { a: u32 });

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{ .a = 0 });
    const handle1 = try pool.add(.{ .a = 1 });
    const handle2 = try pool.add(.{ .a = 2 });
    const handle3 = try pool.add(.{ .a = 3 });

    const a0ptr : *u32 = try pool.getColumnPtr(handle0, .a);
    const a1ptr : *u32 = try pool.getColumnPtr(handle1, .a);
    const a2ptr : *u32 = try pool.getColumnPtr(handle2, .a);
    const a3ptr : *u32 = try pool.getColumnPtr(handle3, .a);
    try expectEqual(@as(u32, 0), a0ptr.*);
    try expectEqual(@as(u32, 1), a1ptr.*);
    try expectEqual(@as(u32, 2), a2ptr.*);
    try expectEqual(@as(u32, 3), a3ptr.*);

    try pool.remove(handle0);
    try pool.remove(handle1);
    try pool.remove(handle2);
    try pool.remove(handle3);
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumnPtr(handle0, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumnPtr(handle1, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumnPtr(handle2, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumnPtr(handle3, .a));
}

test "Pool.getColumn()" {
    const TestPool = Pool(2,6, void, struct { a: u32 });

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{ .a = 0 });
    const handle1 = try pool.add(.{ .a = 1 });
    const handle2 = try pool.add(.{ .a = 2 });
    const handle3 = try pool.add(.{ .a = 3 });

    try expectEqual(@as(u32, 0), try pool.getColumn(handle0, .a));
    try expectEqual(@as(u32, 1), try pool.getColumn(handle1, .a));
    try expectEqual(@as(u32, 2), try pool.getColumn(handle2, .a));
    try expectEqual(@as(u32, 3), try pool.getColumn(handle3, .a));

    try pool.remove(handle0);
    try pool.remove(handle1);
    try pool.remove(handle2);
    try pool.remove(handle3);
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumn(handle0, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumn(handle1, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumn(handle2, .a));
    try expectError(TestPool.Error.HandleIsReleased, pool.getColumn(handle3, .a));
}

test "Pool.setColumn()" {
    const TestPool = Pool(2,6, void, struct { a: u32 });

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{ .a = 0 });
    const handle1 = try pool.add(.{ .a = 1 });
    const handle2 = try pool.add(.{ .a = 2 });
    const handle3 = try pool.add(.{ .a = 3 });

    try expectEqual(@as(u32, 0), try pool.getColumn(handle0, .a));
    try expectEqual(@as(u32, 1), try pool.getColumn(handle1, .a));
    try expectEqual(@as(u32, 2), try pool.getColumn(handle2, .a));
    try expectEqual(@as(u32, 3), try pool.getColumn(handle3, .a));

    try pool.setColumn(handle0, .a, 10);
    try pool.setColumn(handle1, .a, 11);
    try pool.setColumn(handle2, .a, 12);
    try pool.setColumn(handle3, .a, 13);

    try expectEqual(@as(u32, 10), try pool.getColumn(handle0, .a));
    try expectEqual(@as(u32, 11), try pool.getColumn(handle1, .a));
    try expectEqual(@as(u32, 12), try pool.getColumn(handle2, .a));
    try expectEqual(@as(u32, 13), try pool.getColumn(handle3, .a));

    try pool.remove(handle0);
    try pool.remove(handle1);
    try pool.remove(handle2);
    try pool.remove(handle3);
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle0, .a, 20));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle1, .a, 21));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle2, .a, 22));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle3, .a, 23));
}

test "Pool.setColumn() calls deinit" {
    const TestPool = Pool(2,6, void, struct { d: DeinitCounter });

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    var deinit_count : u32 = 0;
    const handle = try pool.add(.{ .d = DeinitCounter.init(&deinit_count)});
    try expectEqual(@as(u32, 0), deinit_count);
    try pool.setColumn(handle, .d, DeinitCounter.init(&deinit_count));
    try expectEqual(@as(u32, 1), deinit_count);
    try pool.remove(handle);
    try expectEqual(@as(u32, 2), deinit_count);
}

test "Pool.setColumns()" {
    const TestPool = Pool(2,6, void, struct { a: u32 });

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    try expectEqual(@as(usize, 0), pool.liveHandleCount());

    const handle0 = try pool.add(.{ .a = 0 });
    const handle1 = try pool.add(.{ .a = 1 });
    const handle2 = try pool.add(.{ .a = 2 });
    const handle3 = try pool.add(.{ .a = 3 });

    try expectEqual(@as(u32, 0), try pool.getColumn(handle0, .a));
    try expectEqual(@as(u32, 1), try pool.getColumn(handle1, .a));
    try expectEqual(@as(u32, 2), try pool.getColumn(handle2, .a));
    try expectEqual(@as(u32, 3), try pool.getColumn(handle3, .a));

    try pool.setColumns(handle0, .{ .a = 10 });
    try pool.setColumns(handle1, .{ .a = 11 });
    try pool.setColumns(handle2, .{ .a = 12 });
    try pool.setColumns(handle3, .{ .a = 13 });

    try expectEqual(@as(u32, 10), try pool.getColumn(handle0, .a));
    try expectEqual(@as(u32, 11), try pool.getColumn(handle1, .a));
    try expectEqual(@as(u32, 12), try pool.getColumn(handle2, .a));
    try expectEqual(@as(u32, 13), try pool.getColumn(handle3, .a));

    try pool.remove(handle0);
    try pool.remove(handle1);
    try pool.remove(handle2);
    try pool.remove(handle3);
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle0, .a, 20));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle1, .a, 21));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle2, .a, 22));
    try expectError(TestPool.Error.HandleIsReleased, pool.setColumn(handle3, .a, 23));
}

test "Pool.setColumns() calls deinit" {
    const TestPool = Pool(2,6, void, DeinitCounter);

    const GPA = std.heap.GeneralPurposeAllocator;
    var gpa = GPA(.{}){};
    var pool = try TestPool.initMaxCapacity(gpa.allocator());
    defer pool.deinit();

    var deinit_count : u32 = 0;
    const handle = try pool.add(DeinitCounter.init(&deinit_count));
    try expectEqual(@as(u32, 0), deinit_count);
    try pool.setColumns(handle, DeinitCounter.init(&deinit_count));
    try expectEqual(@as(u32, 1), deinit_count);
    try pool.remove(handle);
    try expectEqual(@as(u32, 2), deinit_count);
}
