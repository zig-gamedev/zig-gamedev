const std = @import("std");

pub fn ErrorUnionErrorSet(comptime T: type) type {
    return switch (@typeInfo(T)) {
        .ErrorUnion => |error_union_info| error_union_info.error_set,
        else => @compileError("expected ErrorUnion but was " ++ @typeName(T)),
    };
}

pub fn ErrorUnionReturnType(comptime T: type) type {
    return switch (@typeInfo(T)) {
        .ErrorUnion => |error_union_info| error_union_info.payload,
        else => @compileError("expected ErrorUnion but was " ++ @typeName(T)),
    };
}

pub fn ReturnType(comptime T: type) type {
    return switch (@typeInfo(T)) {
        .Fn => |fn_info| fn_info.return_type.?,
        else => @compileError("expected Fn but was " ++ @typeName(T)),
    };
}

fn compareStrings(_: void, left: [:0]const u8, right: [:0]const u8) bool {
    return std.mem.order(u8, left, right).compare(std.math.CompareOperator.lt);
}

fn ErrorEnum(comptime ErrorSet: type) type {
    const error_set_info = @typeInfo(ErrorSet);
    switch (error_set_info) {
        .ErrorSet => |error_set| if (error_set) |es| {
            comptime var error_names: [es.len][:0]const u8 = undefined;
            inline for (es, 0..) |err, i| {
                error_names[i] = err.name;
            }
            std.mem.sort([:0]const u8, &error_names, {}, compareStrings);

            const EnumTagType = std.math.IntFittingRange(0, es.len - 1);
            comptime var enum_fields: [es.len]std.builtin.Type.EnumField = undefined;

            inline for (&error_names, 0..) |error_name, i| {
                enum_fields[i] = .{
                    .name = error_name,
                    .value = i,
                };
            }
            return @Type(.{
                .Enum = .{
                    .tag_type = EnumTagType,
                    .fields = &enum_fields,
                    .decls = &.{},
                    .is_exhaustive = true,
                },
            });
        } else {
            @compileError(@typeName(ErrorSet) ++ " must not be empty");
        },
        else => |info| @compileError(@typeName(ErrorSet) ++ " must be an ErrorSet, but was " ++ info),
    }
}

fn ErrorSetEnum(comptime ErrorSet: type) type {
    return struct {
        ErrorSet: type,
        Enum: type,
        enum_to_error: std.StaticStringMap(ErrorSet),
        error_to_enum: std.StaticStringMap(ErrorEnum(ErrorSet)),

        pub fn errorToEnum(self: @This(), err: ErrorSet) ErrorEnum(ErrorSet) {
            return self.error_to_enum.get(@errorName(err)).?;
        }

        pub fn enumToError(self: @This(), error_enum: ErrorEnum(ErrorSet)) ErrorSet {
            return self.enum_to_error.get(@tagName(error_enum)).?;
        }
    };
}

pub fn initFn(comptime Fn: type) ErrorSetEnum(ErrorUnionErrorSet(ReturnType(Fn))) {
    return init(ErrorUnionErrorSet(ReturnType(Fn)));
}

pub fn init(comptime ErrorSet: type) ErrorSetEnum(ErrorSet) {
    @setEvalBranchQuota(20000);

    const error_set_info = @typeInfo(ErrorSet);
    switch (error_set_info) {
        .ErrorSet => |error_set| if (error_set) |es| {
            const enum_to_error = enum_to_error: {
                const EnumErrorItem = struct { []const u8, ErrorSet };
                comptime var enum_to_error_list: [es.len]EnumErrorItem = undefined;
                inline for (es, 0..) |err, i| {
                    enum_to_error_list[i] = .{
                        err.name,
                        @field(ErrorSet, err.name),
                    };
                }

                break :enum_to_error std.StaticStringMap(ErrorSet).initComptime(&enum_to_error_list);
            };

            const Enum = ErrorEnum(ErrorSet);
            const error_to_enum = error_to_enum: {
                const ErrorEnumItem = struct { []const u8, Enum };
                comptime var error_to_enum_list: [es.len]ErrorEnumItem = undefined;
                inline for (es, 0..) |err, i| {
                    error_to_enum_list[i] = .{
                        err.name,
                        @field(Enum, err.name),
                    };
                }

                break :error_to_enum std.StaticStringMap(ErrorEnum(ErrorSet)).initComptime(&error_to_enum_list);
            };

            return .{
                .ErrorSet = ErrorSet,
                .Enum = Enum,
                .enum_to_error = enum_to_error,
                .error_to_enum = error_to_enum,
            };
        } else {
            @compileError(@typeName(ErrorSet) ++ " must not be empty");
        },
        else => |info| @compileError(@typeName(ErrorSet) ++ " must be an ErrorSet, but was " ++ info),
    }
}
