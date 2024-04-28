const std = @import("std");

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pub fn asTypeId(comptime typeInfo: std.builtin.Type) std.builtin.TypeId {
    return @as(std.builtin.TypeId, typeInfo);
}

pub fn typeIdOf(comptime T: type) std.builtin.TypeId {
    return asTypeId(@typeInfo(T));
}

pub fn isStruct(comptime T: type) bool {
    return typeIdOf(T) == std.builtin.TypeId.Struct;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// UInt(bits) returns an unsigned integer type of the requested bit width.
pub fn UInt(comptime bits: u8) type {
    const unsigned = std.builtin.Signedness.unsigned;
    return @Type(.{ .Int = .{ .signedness = unsigned, .bits = bits } });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Returns an unsigned integer type with ***at least*** `min_bits`,
/// that is also large enough to be addressable by a normal pointer.
/// The returned type will always be one of the following:
/// * `u8`
/// * `u16`
/// * `u32`
/// * `u64`
/// * `u128`
/// * `u256`
pub fn AddressableUInt(comptime min_bits: u8) type {
    return switch (min_bits) {
        0...8 => u8,
        9...16 => u16,
        17...32 => u32,
        33...64 => u64,
        65...128 => u128,
        129...255 => u256,
    };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Given: `Struct = struct { foo: u32, bar: u64 }`
/// Returns: `StructOfSlices = struct { foo: []u32, bar: []u64 }`
pub fn StructOfSlices(comptime Struct: type) type {
    const StructField = std.builtin.Type.StructField;

    // same number of fields in the new struct
    const struct_fields = @typeInfo(Struct).Struct.fields;

    comptime var struct_of_slices_fields: []const StructField = &.{};
    inline for (struct_fields) |struct_field| {
        // u32 -> []u32
        const element_type = struct_field.type;

        const slice_type_info = std.builtin.Type{
            .Pointer = .{
                .child = element_type,
                .alignment = @alignOf(element_type),
                .size = .Slice,
                .is_const = false,
                .is_volatile = false,
                .address_space = .generic,
                .is_allowzero = false,
                .sentinel = null,
            },
        };

        const FieldType = @Type(slice_type_info);

        // Struct.foo: u32 -> StructOfSlices.foo : []u32
        const slice_field = std.builtin.Type.StructField{
            .name = struct_field.name,
            .type = FieldType,
            .default_value = null,
            .is_comptime = false,
            .alignment = @alignOf(FieldType),
        };

        // Struct.foo: u32 -> StructOfSlices.foo : []u32
        struct_of_slices_fields = struct_of_slices_fields ++ [1]StructField{slice_field};
    }

    return @Type(.{ .Struct = .{
        .layout = .auto,
        .fields = struct_of_slices_fields,
        .decls = &.{},
        .is_tuple = false,
    } });
}

test "StructOfSlices" {
    const expectEqual = std.testing.expectEqual;

    const Struct = struct { a: u16, b: u16, c: u16 };
    try expectEqual(@sizeOf(u16) * 3, @sizeOf(Struct));

    const SOS = StructOfSlices(Struct);
    try expectEqual(@sizeOf([]u16) * 3, @sizeOf(SOS));
}
