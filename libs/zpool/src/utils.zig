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
    const alignment = std.meta.alignment;

    // initialize a basic slice-typed field, no name, zero sized elements
    const Template = struct { @"": []u0 };
    var slice_field: StructField = @typeInfo(Template).Struct.fields[0];
    var slice_type_info = @typeInfo(slice_field.field_type);

    // same number of fields in the new struct
    const struct_fields = @typeInfo(Struct).Struct.fields;
    var struct_of_slices_fields: [struct_fields.len]StructField = undefined;

    inline for (struct_fields) |struct_field, i| {
        // u32 -> []u32
        const element_type = struct_field.field_type;
        slice_type_info.Pointer.child = element_type;
        slice_type_info.Pointer.alignment = alignment(element_type);

        // Struct.foo: u32 -> StructOfSlices.foo : []u32
        slice_field.name = struct_field.name;
        slice_field.field_type = @Type(slice_type_info);

        // Struct.foo: u32 -> StructOfSlices.foo : []u32
        struct_of_slices_fields[i] = slice_field;
    }

    return @Type(.{ .Struct = .{
        .layout = std.builtin.Type.ContainerLayout.Auto,
        .fields = &struct_of_slices_fields,
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
