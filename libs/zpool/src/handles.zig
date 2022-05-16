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

    const value_bits : u16 = @as(u16, index_bits) + @as(u16, cycle_bits);
    const Value = switch (value_bits) {
          8 => u8,
         16 => u16,
         32 => u32,
         64 => u64,
        128 => u128,
        256 => u256,
        else => @compileError(
            "index_bits + cycle_bits should sum to exactly " ++
            "8, 16, 32, 64, 128, or 256 bits"),
    };

    const utils = @import("utils.zig");
    const UInt = utils.UInt;
    const AddressableUInt = utils.AddressableUInt;

    return extern union {
        const Self = @This();

        pub const Resource = TResource;
        pub const Index    = UInt(index_bits);
        pub const Cycle    = UInt(cycle_bits);

        pub const AddressableIndex = AddressableUInt(index_bits);
        pub const AddressableCycle = AddressableUInt(cycle_bits);

        pub const max_index = ~@as(Index,0);
        pub const max_cycle = ~@as(Cycle,0);
        pub const max_count =  @as(Value,max_index-1)+2;

        value   : Value,
        compact : packed struct { index: Index, cycle: Cycle },

        pub fn init(_index: Index, _cycle: Cycle) Self {
            return .{ .compact = .{ .index = _index, .cycle = _cycle } };
        }

        pub fn addressable(self: Self) AddressableHandle {
            return .{
                .index = self.compact.index,
                .cycle = self.compact.cycle,
            };
        }

        pub fn index(self: Self) AddressableIndex { return self.compact.index; }

        pub fn cycle(self: Self) AddressableCycle { return self.compact.cycle; }

        pub fn eql(a: Self, b: Self) bool { return a.value == b.value; }

        pub const AddressableHandle = struct {
            const Compact = Self;
            index : AddressableIndex = 0,
            cycle : AddressableCycle = 0,

            pub fn compact(handle: AddressableHandle) Compact {
                const _index = @intCast(Index, handle.index);
                const _cycle = @intCast(Cycle, handle.cycle);
                return Compact.init(_index, _cycle);
            }

            pub fn eql(a: AddressableHandle, b: AddressableHandle) bool {
                return a.index == b.index and a.cycle == b.cycle;
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

test "Handle sizes and alignments"
{
    const testing = std.testing;
    const expectEqual = testing.expectEqual;

    { const H = Handle(4, 4, void);
        try expectEqual(@sizeOf(u8), @sizeOf(H));
        try expectEqual(@alignOf(u8), @alignOf(H));
        try expectEqual(4, @bitSizeOf(H.Index));
        try expectEqual(4, @bitSizeOf(H.Cycle));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    { const H = Handle(6, 2, void);
        try expectEqual(@sizeOf(u8), @sizeOf(H));
        try expectEqual(@alignOf(u8), @alignOf(H));
        try expectEqual(6, @bitSizeOf(H.Index));
        try expectEqual(2, @bitSizeOf(H.Cycle));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    { const H = Handle(8, 8, void);
        try expectEqual(@sizeOf(u16), @sizeOf(H));
        try expectEqual(@alignOf(u16), @alignOf(H));
        try expectEqual(8, @bitSizeOf(H.Index));
        try expectEqual(8, @bitSizeOf(H.Cycle));
        try expectEqual(8, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u16), @sizeOf(A));
        try expectEqual(@alignOf(u8), @alignOf(A));
    }

    { const H = Handle(12, 4, void);
        try expectEqual(@sizeOf(u16), @sizeOf(H));
        try expectEqual(@alignOf(u16), @alignOf(H));
        try expectEqual(12, @bitSizeOf(H.Index));
        try expectEqual(4, @bitSizeOf(H.Cycle));
        try expectEqual(16, @bitSizeOf(H.AddressableIndex));
        try expectEqual(8, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u32), @sizeOf(A));
        try expectEqual(@alignOf(u16), @alignOf(A));
    }

    { const H = Handle(16, 16, void);
        try expectEqual(@sizeOf(u32), @sizeOf(H));
        try expectEqual(@alignOf(u32), @alignOf(H));
        try expectEqual(16, @bitSizeOf(H.Index));
        try expectEqual(16, @bitSizeOf(H.Cycle));
        try expectEqual(16, @bitSizeOf(H.AddressableIndex));
        try expectEqual(16, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u32), @sizeOf(A));
        try expectEqual(@alignOf(u16), @alignOf(A));
    }

    { const H = Handle(22, 10, void);
        try expectEqual(@sizeOf(u32), @sizeOf(H));
        try expectEqual(@alignOf(u32), @alignOf(H));
        try expectEqual(22, @bitSizeOf(H.Index));
        try expectEqual(10, @bitSizeOf(H.Cycle));
        try expectEqual(32, @bitSizeOf(H.AddressableIndex));
        try expectEqual(16, @bitSizeOf(H.AddressableCycle));

        const A = H.AddressableHandle;
        try expectEqual(@sizeOf(u64), @sizeOf(A));
        try expectEqual(@alignOf(u32), @alignOf(A));
    }
}

////////////////////////////////////////////////////////////////////////////////

test "Handle.format()" {
    const bufPrint = std.fmt.bufPrint;
    const expectEqualStrings = std.testing.expectEqualStrings;

    const Foo = struct {};
    const H = Handle(12,4, Foo);
    const h = H.init(0, 1);

    var buffer = [_]u8{0} ** 128;
    const s = try bufPrint(buffer[0..], "{}", .{ h });
    try expectEqualStrings("Foo[0#1]", s);
}