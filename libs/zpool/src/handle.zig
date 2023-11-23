const std = @import("std");

/// Returns a struct consisting of an array `index` and a semi-unique `cycle`,
/// which exists to distinguish handles with the same array `index`.
///
/// The `cycle` value is only unique within the incremental period of an
/// unsigned integer with `cycle_bits`, so a larger number of `cycle_bits`
/// provides a larger scope of identifiable conflicts between handles for the
/// same `index`.
///
/// `Handle` is generic because while the `{ index, cycle }` pattern is widely
/// applicable, a good distribution of bits between `index` and `cycle` and the
/// overall size of a handle are highly dependent on the lifecycle of the
/// resource being identified by a handle and the systems consuming handles.
///
/// Reasonable values for `index_bits` depend on the maximum number of
/// uniquely identifiable resources your API will to identify with handles.
/// Generally this is directly tied to the length of the array(s) in which
/// you will store data to be referenced by a handle's `index`.
///
/// Reasonable values for `cycle_bits` depend on the frequency with which your
/// API expects to be issuing handles, and how many cycles of your application
/// are likely to elapse before an expired handle will likely no longer be
/// retained by the API caller's data structures.
///
/// For example, a `Handle(16, 16)` may be sufficient for a GPU resource like
/// a texture or buffer, where 64k instances of that resource is a reasonable
/// upper bound.
///
/// A `Handle(22, 10)` may be more appropriate to identify an entity in a
/// system where we can safely assume that 4 million entities, is a lot, and
/// that API callers can discover and discard expired entity handles within
/// 1024 frames of an entity being destroyed and its handle's `index` being
/// reissued for use by a distinct entity.
///
/// `TResource` identifies type of resource referenced by a handle, and
/// provides a type-safe distinction between two otherwise equivalently
/// configured `Handle` types, such as:
/// * `const BufferHandle  = Handle(16, 16, Buffer);`
/// * `const TextureHandle = Handle(16, 16, Texture);`
///
/// The total size of a handle will always be the size of an addressable
/// unsigned integer of type `u8`, `u16`, `u32`, `u64`, `u128`, or `u256`.
pub fn Handle(
    comptime index_bits: u8,
    comptime cycle_bits: u8,
    comptime TResource: type,
) type {
    if (index_bits == 0) @compileError("index_bits must be greater than 0");
    if (cycle_bits == 0) @compileError("cycle_bits must be greater than 0");

    const id_bits: u16 = @as(u16, index_bits) + @as(u16, cycle_bits);
    const Id = switch (id_bits) {
        8 => u8,
        16 => u16,
        32 => u32,
        64 => u64,
        128 => u128,
        256 => u256,
        else => @compileError("index_bits + cycle_bits must sum to exactly " ++
            "8, 16, 32, 64, 128, or 256 bits"),
    };

    const field_bits = @max(index_bits, cycle_bits);

    const utils = @import("utils.zig");
    const UInt = utils.UInt;
    const AddressableUInt = utils.AddressableUInt;

    return extern struct {
        const Self = @This();

        const HandleType = Self;
        const IndexType = UInt(index_bits);
        const CycleType = UInt(cycle_bits);
        const HandleUnion = extern union {
            id: Id,
            bits: packed struct {
                cycle: CycleType, // least significant bits
                index: IndexType, // most significant bits
            },
        };

        pub const Resource = TResource;

        pub const AddressableCycle = AddressableUInt(field_bits);
        pub const AddressableIndex = AddressableUInt(field_bits);

        pub const max_cycle = ~@as(CycleType, 0);
        pub const max_index = ~@as(IndexType, 0);
        pub const max_count = @as(Id, max_index - 1) + 2;

        id: Id = 0,

        pub const nil = Self{ .id = 0 };

        pub fn init(i: IndexType, c: CycleType) Self {
            const u = HandleUnion{ .bits = .{
                .cycle = c,
                .index = i,
            } };
            return .{ .id = u.id };
        }

        pub fn cycle(self: Self) CycleType {
            const u = HandleUnion{ .id = self.id };
            return u.bits.cycle;
        }

        pub fn index(self: Self) IndexType {
            const u = HandleUnion{ .id = self.id };
            return u.bits.index;
        }

        /// Unpacks the `index` and `cycle` bit fields that comprise
        /// `Handle.id` into an `AddressableHandle`, which stores
        /// the `index` and `cycle` values in pointer-addressable fields.
        pub fn addressable(self: Self) AddressableHandle {
            const u = HandleUnion{ .id = self.id };
            return .{
                .cycle = u.bits.cycle,
                .index = u.bits.index,
            };
        }

        /// When you want to directly access the `index` and `cycle` of a
        /// handle, first convert it to an `AddressableHandle` by calling
        /// `Handle.addressable()`.
        /// An `AddressableHandle` can be converted back into a "compact"
        /// `Handle` by calling `AddressableHandle.compact()`.
        pub const AddressableHandle = struct {
            cycle: AddressableCycle = 0,
            index: AddressableIndex = 0,

            /// Returns the corresponding `Handle`
            pub fn handle(self: AddressableHandle) HandleType {
                const u = HandleUnion{ .bits = .{
                    .cycle = @as(CycleType, @intCast(self.cycle)),
                    .index = @as(IndexType, @intCast(self.index)),
                } };
                return .{ .id = u.id };
            }
        };

        pub fn format(
            self: Self,
            comptime fmt: []const u8,
            options: std.fmt.FormatOptions,
            writer: anytype,
        ) !void {
            _ = fmt;
            _ = options;
            const n = @typeName(Resource);
            const a = self.addressable();
            return writer.print("{s}[{}#{}]", .{ n, a.index, a.cycle });
        }
    };
}

////////////////////////////////////////////////////////////////////////////////

test "Handle sizes and alignments" {
    const expectEqual = std.testing.expectEqual;

    {
        const H = Handle(4, 4, void);
        try expectEqual(@sizeOf(u8), @sizeOf(H));
        try expectEqual(@alignOf(u8), @alignOf(H));
        try expectEqual(4, @bitSizeOf(H.IndexType));
        try expectEqual(4, @bitSizeOf(H.CycleType));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    {
        const H = Handle(6, 2, void);
        try expectEqual(@sizeOf(u8), @sizeOf(H));
        try expectEqual(@alignOf(u8), @alignOf(H));
        try expectEqual(6, @bitSizeOf(H.IndexType));
        try expectEqual(2, @bitSizeOf(H.CycleType));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    {
        const H = Handle(8, 8, void);
        try expectEqual(@sizeOf(u16), @sizeOf(H));
        try expectEqual(@alignOf(u16), @alignOf(H));
        try expectEqual(8, @bitSizeOf(H.IndexType));
        try expectEqual(8, @bitSizeOf(H.CycleType));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    {
        const H = Handle(12, 4, void);
        try expectEqual(@sizeOf(u16), @sizeOf(H));
        try expectEqual(@alignOf(u16), @alignOf(H));
        try expectEqual(12, @bitSizeOf(H.IndexType));
        try expectEqual(4, @bitSizeOf(H.CycleType));
        try expectEqual(16, @bitSizeOf(H.AddressableIndex));
        try expectEqual(16, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u32), @sizeOf(A));
        try expectEqual(@alignOf(u16), @alignOf(A));
    }

    {
        const H = Handle(16, 16, void);
        try expectEqual(@sizeOf(u32), @sizeOf(H));
        try expectEqual(@alignOf(u32), @alignOf(H));
        try expectEqual(16, @bitSizeOf(H.IndexType));
        try expectEqual(16, @bitSizeOf(H.CycleType));
        try expectEqual(16, @bitSizeOf(H.AddressableIndex));
        try expectEqual(16, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u32), @sizeOf(A));
        try expectEqual(@alignOf(u16), @alignOf(A));
    }

    {
        const H = Handle(22, 10, void);
        try expectEqual(@sizeOf(u32), @sizeOf(H));
        try expectEqual(@alignOf(u32), @alignOf(H));
        try expectEqual(22, @bitSizeOf(H.IndexType));
        try expectEqual(10, @bitSizeOf(H.CycleType));
        try expectEqual(32, @bitSizeOf(H.AddressableIndex));
        try expectEqual(32, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u64), @sizeOf(A));
        try expectEqual(@alignOf(u32), @alignOf(A));
    }
}

////////////////////////////////////////////////////////////////////////////////

test "Handle sort order" {
    const expect = std.testing.expect;

    const handle = Handle(4, 4, void).init;
    const a = handle(0, 3);
    const b = handle(1, 1);

    // id order is consistent with index order, even when cycle order is not
    try expect(a.id < b.id);
    try expect(a.index() < b.index());
    try expect(a.cycle() > b.cycle());
}

////////////////////////////////////////////////////////////////////////////////

test "Handle.format()" {
    const bufPrint = std.fmt.bufPrint;
    const expectEqualStrings = std.testing.expectEqualStrings;

    const Foo = struct {};
    const H = Handle(12, 4, Foo);
    const h = H.init(0, 1);

    var buffer = [_]u8{0} ** 128;
    const s = try bufPrint(buffer[0..], "{}", .{h});
    try expectEqualStrings("handle.test.Handle.format().Foo[0#1]", s);
}
