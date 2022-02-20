pub const std = @import("std");

pub const enable = @import("build_options").enable_tracy;

extern fn ___tracy_emit_zone_begin_callstack(
    srcloc: *const ___tracy_source_location_data,
    depth: c_int,
    active: c_int,
) ___tracy_c_zone_context;

extern fn ___tracy_emit_zone_end(ctx: ___tracy_c_zone_context) void;

extern fn ___tracy_emit_frame_mark(name: ?[*:0]const u8) void;

pub const ___tracy_source_location_data = extern struct {
    name: ?[*:0]const u8,
    function: [*:0]const u8,
    file: [*:0]const u8,
    line: u32,
    color: u32,
};

pub const ___tracy_c_zone_context = extern struct {
    id: u32,
    active: c_int,

    pub inline fn end(self: ___tracy_c_zone_context) void {
        ___tracy_emit_zone_end(self);
    }
};

pub const Ctx = if (enable) ___tracy_c_zone_context else struct {
    pub inline fn end(self: Ctx) void {
        _ = self;
    }
};

pub inline fn zone(
    comptime src: std.builtin.SourceLocation,
    comptime active: c_int,
) Ctx {
    if (!enable) return .{};

    const loc = ___tracy_source_location_data{
        .name = null,
        .function = src.fn_name.ptr,
        .file = src.file.ptr,
        .line = src.line,
        .color = 0,
    };
    return ___tracy_emit_zone_begin_callstack(&loc, 1, active);
}

pub inline fn zoneN(
    comptime src: std.builtin.SourceLocation,
    comptime name: ?[*:0]const u8,
    comptime active: c_int,
) Ctx {
    if (!enable) return .{};

    const loc = ___tracy_source_location_data{
        .name = name,
        .function = src.fn_name.ptr,
        .file = src.file.ptr,
        .line = src.line,
        .color = 0,
    };
    return ___tracy_emit_zone_begin_callstack(&loc, 1, active);
}

pub inline fn zoneNC(
    comptime src: std.builtin.SourceLocation,
    comptime name: ?[*:0]const u8,
    comptime color: u32,
    comptime active: c_int,
) Ctx {
    if (!enable) return .{};

    const loc = ___tracy_source_location_data{
        .name = name,
        .function = src.fn_name.ptr,
        .file = src.file.ptr,
        .line = src.line,
        .color = color,
    };
    return ___tracy_emit_zone_begin_callstack(&loc, 1, active);
}

pub inline fn frameMark() void {
    if (!enable) return;
    ___tracy_emit_frame_mark(null);
}

pub inline fn frameMarkNamed(comptime name: [*:0]const u8) void {
    if (!enable) return;
    ___tracy_emit_frame_mark(name);
}
