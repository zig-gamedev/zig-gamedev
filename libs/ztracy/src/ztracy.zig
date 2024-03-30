const std = @import("std");
const builtin = @import("builtin");
const Src = std.builtin.SourceLocation;

// check for a decl named tracy_enabled in root or build_options
pub const enabled = blk: {
    var build_enable: ?bool = null;

    if (!builtin.is_test) {
        // Don't try to include build_options in tests.
        // Otherwise `zig test` doesn't work.
        const options = @import("ztracy_options");
        if (@hasDecl(options, "enable_ztracy")) {
            build_enable = options.enable_ztracy;
        }
    }

    break :blk build_enable orelse false;
};

const debug_verify_stack_order = false;

pub usingnamespace if (enabled) tracy_full else tracy_stub;

const tracy_stub = struct {
    pub const ZoneCtx = struct {
        pub inline fn Text(self: ZoneCtx, text: []const u8) void {
            _ = self;
            _ = text;
        }
        pub inline fn Name(self: ZoneCtx, name: []const u8) void {
            _ = self;
            _ = name;
        }
        pub inline fn Value(self: ZoneCtx, value: u64) void {
            _ = self;
            _ = value;
        }
        pub inline fn End(self: ZoneCtx) void {
            _ = self;
        }
    };

    pub inline fn SetThreadName(name: [*:0]const u8) void {
        _ = name;
    }

    pub inline fn Zone(comptime src: Src) ZoneCtx {
        _ = src;
        return .{};
    }
    pub inline fn ZoneN(comptime src: Src, name: [*:0]const u8) ZoneCtx {
        _ = src;
        _ = name;
        return .{};
    }
    pub inline fn ZoneC(comptime src: Src, color: u32) ZoneCtx {
        _ = src;
        _ = color;
        return .{};
    }
    pub inline fn ZoneNC(comptime src: Src, name: [*:0]const u8, color: u32) ZoneCtx {
        _ = src;
        _ = name;
        _ = color;
        return .{};
    }
    pub inline fn ZoneS(comptime src: Src, depth: i32) ZoneCtx {
        _ = src;
        _ = depth;
        return .{};
    }
    pub inline fn ZoneNS(comptime src: Src, name: [*:0]const u8, depth: i32) ZoneCtx {
        _ = src;
        _ = name;
        _ = depth;
        return .{};
    }
    pub inline fn ZoneCS(comptime src: Src, color: u32, depth: i32) ZoneCtx {
        _ = src;
        _ = color;
        _ = depth;
        return .{};
    }
    pub inline fn ZoneNCS(comptime src: Src, name: [*:0]const u8, color: u32, depth: i32) ZoneCtx {
        _ = src;
        _ = name;
        _ = color;
        _ = depth;
        return .{};
    }

    pub inline fn Alloc(ptr: ?*const anyopaque, size: usize) void {
        _ = ptr;
        _ = size;
    }
    pub inline fn Free(ptr: ?*const anyopaque) void {
        _ = ptr;
    }
    pub inline fn SecureAlloc(ptr: ?*const anyopaque, size: usize) void {
        _ = ptr;
        _ = size;
    }
    pub inline fn SecureFree(ptr: ?*const anyopaque) void {
        _ = ptr;
    }
    pub inline fn AllocS(ptr: ?*const anyopaque, size: usize, depth: c_int) void {
        _ = ptr;
        _ = size;
        _ = depth;
    }
    pub inline fn FreeS(ptr: ?*const anyopaque, depth: c_int) void {
        _ = ptr;
        _ = depth;
    }
    pub inline fn SecureAllocS(ptr: ?*const anyopaque, size: usize, depth: c_int) void {
        _ = ptr;
        _ = size;
        _ = depth;
    }
    pub inline fn SecureFreeS(ptr: ?*const anyopaque, depth: c_int) void {
        _ = ptr;
        _ = depth;
    }

    pub inline fn AllocN(ptr: ?*const anyopaque, size: usize, name: [*:0]const u8) void {
        _ = ptr;
        _ = size;
        _ = name;
    }
    pub inline fn FreeN(ptr: ?*const anyopaque, name: [*:0]const u8) void {
        _ = ptr;
        _ = name;
    }
    pub inline fn SecureAllocN(ptr: ?*const anyopaque, size: usize, name: [*:0]const u8) void {
        _ = ptr;
        _ = size;
        _ = name;
    }
    pub inline fn SecureFreeN(ptr: ?*const anyopaque, name: [*:0]const u8) void {
        _ = ptr;
        _ = name;
    }
    pub inline fn AllocNS(ptr: ?*const anyopaque, size: usize, depth: c_int, name: [*:0]const u8) void {
        _ = ptr;
        _ = size;
        _ = depth;
        _ = name;
    }
    pub inline fn FreeNS(ptr: ?*const anyopaque, depth: c_int, name: [*:0]const u8) void {
        _ = ptr;
        _ = depth;
        _ = name;
    }
    pub inline fn SecureAllocNS(ptr: ?*const anyopaque, size: usize, depth: c_int, name: [*:0]const u8) void {
        _ = ptr;
        _ = size;
        _ = depth;
        _ = name;
    }
    pub inline fn SecureFreeNS(ptr: ?*const anyopaque, depth: c_int, name: [*:0]const u8) void {
        _ = ptr;
        _ = depth;
        _ = name;
    }

    pub inline fn Message(text: []const u8) void {
        _ = text;
    }
    pub inline fn MessageL(text: [*:0]const u8) void {
        _ = text;
    }
    pub inline fn MessageC(text: []const u8, color: u32) void {
        _ = text;
        _ = color;
    }
    pub inline fn MessageLC(text: [*:0]const u8, color: u32) void {
        _ = text;
        _ = color;
    }
    pub inline fn MessageS(text: []const u8, depth: c_int) void {
        _ = text;
        _ = depth;
    }
    pub inline fn MessageLS(text: [*:0]const u8, depth: c_int) void {
        _ = text;
        _ = depth;
    }
    pub inline fn MessageCS(text: []const u8, color: u32, depth: c_int) void {
        _ = text;
        _ = color;
        _ = depth;
    }
    pub inline fn MessageLCS(text: [*:0]const u8, color: u32, depth: c_int) void {
        _ = text;
        _ = color;
        _ = depth;
    }

    pub inline fn FrameMark() void {}
    pub inline fn FrameMarkNamed(name: [*:0]const u8) void {
        _ = name;
    }
    pub inline fn FrameMarkStart(name: [*:0]const u8) void {
        _ = name;
    }
    pub inline fn FrameMarkEnd(name: [*:0]const u8) void {
        _ = name;
    }
    pub inline fn FrameImage(image: ?*const anyopaque, width: u16, height: u16, offset: u8, flip: c_int) void {
        _ = image;
        _ = width;
        _ = height;
        _ = offset;
        _ = flip;
    }

    pub inline fn FiberEnter(name: [*:0]const u8) void {
        _ = name;
    }
    pub inline fn FiberLeave() void {}

    pub inline fn PlotF(name: [*:0]const u8, val: f64) void {
        _ = name;
        _ = val;
    }
    pub inline fn PlotU(name: [*:0]const u8, val: u64) void {
        _ = name;
        _ = val;
    }
    pub inline fn PlotI(name: [*:0]const u8, val: i64) void {
        _ = name;
        _ = val;
    }
    pub inline fn AppInfo(text: []const u8) void {
        _ = text;
    }

    pub const TracyAllocator = struct {
        child_allocator: std.mem.Allocator,

        pub fn init(child_allocator: std.mem.Allocator) TracyAllocator {
            return .{
                .child_allocator = child_allocator,
            };
        }

        pub fn allocator(self: *TracyAllocator) std.mem.Allocator {
            return self.child_allocator;
        }
    };
};

const tracy_full = struct {
    const c = @cImport({
        //@cDefine("TRACY_CALLSTACK", "8"); Uncomment to enable callstacks. "8" is max depth (can be changed).
        @cDefine("TRACY_ENABLE", "");
        @cInclude("TracyC.h");
    });

    const has_callstack_support = @hasDecl(c, "TRACY_HAS_CALLSTACK") and @hasDecl(c, "TRACY_CALLSTACK");
    const callstack_depth: c_int = if (has_callstack_support) c.TRACY_CALLSTACK else 0;

    threadlocal var stack_depth: if (debug_verify_stack_order) usize else u0 = 0;

    pub const ZoneCtx = struct {
        _zone: c.___tracy_c_zone_context,
        _token: if (debug_verify_stack_order) usize else void,

        pub inline fn Text(self: ZoneCtx, text: []const u8) void {
            if (debug_verify_stack_order) {
                if (stack_depth != self._token) {
                    std.debug.panic(
                        "Error: expected Value() at stack depth {} but was {}\n",
                        .{ self._token, stack_depth },
                    );
                }
            }
            c.___tracy_emit_zone_text(self._zone, text.ptr, text.len);
        }
        pub inline fn Name(self: ZoneCtx, name: []const u8) void {
            if (debug_verify_stack_order) {
                if (stack_depth != self._token) {
                    std.debug.panic(
                        "Error: expected Value() at stack depth {} but was {}\n",
                        .{ self._token, stack_depth },
                    );
                }
            }
            c.___tracy_emit_zone_name(self._zone, name.ptr, name.len);
        }
        pub inline fn Value(self: ZoneCtx, val: u64) void {
            if (debug_verify_stack_order) {
                if (stack_depth != self._token) {
                    std.debug.panic(
                        "Error: expected Value() at stack depth {} but was {}\n",
                        .{ self._token, stack_depth },
                    );
                }
            }
            c.___tracy_emit_zone_value(self._zone, val);
        }
        pub inline fn End(self: ZoneCtx) void {
            if (debug_verify_stack_order) {
                if (stack_depth != self._token) {
                    std.debug.panic(
                        "Error: expected End() at stack depth {} but was {}\n",
                        .{ self._token, stack_depth },
                    );
                }
                stack_depth -= 1;
            }
            c.___tracy_emit_zone_end(self._zone);
        }
    };

    inline fn initZone(comptime src: Src, name: ?[*:0]const u8, color: u32, depth: c_int) ZoneCtx {
        // Tracy uses pointer identity to identify contexts.
        // The `src` parameter being comptime ensures that
        // each zone gets its own unique global location for this
        // struct.
        const static = struct {
            var loc: c.___tracy_source_location_data = undefined;

            // Ensure that a unique struct type is generated for each unique `src`. See
            // https://github.com/ziglang/zig/issues/18816
            comptime {
                // https://github.com/ziglang/zig/issues/19274
                _ = @sizeOf(@TypeOf(src));
            }
        };
        static.loc = .{
            .name = name,
            .function = src.fn_name.ptr,
            .file = src.file.ptr,
            .line = src.line,
            .color = color,
        };

        const zone = if (has_callstack_support)
            c.___tracy_emit_zone_begin_callstack(&static.loc, depth, 1)
        else
            c.___tracy_emit_zone_begin(&static.loc, 1);

        if (debug_verify_stack_order) {
            stack_depth += 1;
            return ZoneCtx{ ._zone = zone, ._token = stack_depth };
        } else {
            return ZoneCtx{ ._zone = zone, ._token = {} };
        }
    }

    pub inline fn SetThreadName(name: [*:0]const u8) void {
        c.___tracy_set_thread_name(name);
    }

    pub inline fn Zone(comptime src: Src) ZoneCtx {
        return initZone(src, null, 0, callstack_depth);
    }
    pub inline fn ZoneN(comptime src: Src, name: [*:0]const u8) ZoneCtx {
        return initZone(src, name, 0, callstack_depth);
    }
    pub inline fn ZoneC(comptime src: Src, color: u32) ZoneCtx {
        return initZone(src, null, color, callstack_depth);
    }
    pub inline fn ZoneNC(comptime src: Src, name: [*:0]const u8, color: u32) ZoneCtx {
        return initZone(src, name, color, callstack_depth);
    }
    pub inline fn ZoneS(comptime src: Src, depth: i32) ZoneCtx {
        return initZone(src, null, 0, depth);
    }
    pub inline fn ZoneNS(comptime src: Src, name: [*:0]const u8, depth: i32) ZoneCtx {
        return initZone(src, name, 0, depth);
    }
    pub inline fn ZoneCS(comptime src: Src, color: u32, depth: i32) ZoneCtx {
        return initZone(src, null, color, depth);
    }
    pub inline fn ZoneNCS(comptime src: Src, name: [*:0]const u8, color: u32, depth: i32) ZoneCtx {
        return initZone(src, name, color, depth);
    }

    pub inline fn Alloc(ptr: ?*const anyopaque, size: usize) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack(ptr, size, callstack_depth, 0);
        } else {
            c.___tracy_emit_memory_alloc(ptr, size, 0);
        }
    }
    pub inline fn Free(ptr: ?*const anyopaque) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack(ptr, callstack_depth, 0);
        } else {
            c.___tracy_emit_memory_free(ptr, 0);
        }
    }
    pub inline fn SecureAlloc(ptr: ?*const anyopaque, size: usize) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack(ptr, size, callstack_depth, 1);
        } else {
            c.___tracy_emit_memory_alloc(ptr, size, 1);
        }
    }
    pub inline fn SecureFree(ptr: ?*const anyopaque) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack(ptr, callstack_depth, 1);
        } else {
            c.___tracy_emit_memory_free(ptr, 1);
        }
    }
    pub inline fn AllocS(ptr: ?*const anyopaque, size: usize, depth: c_int) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack(ptr, size, depth, 0);
        } else {
            c.___tracy_emit_memory_alloc(ptr, size, 0);
        }
    }
    pub inline fn FreeS(ptr: ?*const anyopaque, depth: c_int) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack(ptr, depth, 0);
        } else {
            c.___tracy_emit_memory_free(ptr, 0);
        }
    }
    pub inline fn SecureAllocS(ptr: ?*const anyopaque, size: usize, depth: c_int) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack(ptr, size, depth, 1);
        } else {
            c.___tracy_emit_memory_alloc(ptr, size, 1);
        }
    }
    pub inline fn SecureFreeS(ptr: ?*const anyopaque, depth: c_int) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack(ptr, depth, 1);
        } else {
            c.___tracy_emit_memory_free(ptr, 1);
        }
    }

    pub inline fn AllocN(ptr: ?*const anyopaque, size: usize, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack_named(ptr, size, callstack_depth, 0, name);
        } else {
            c.___tracy_emit_memory_alloc_named(ptr, size, 0, name);
        }
    }
    pub inline fn FreeN(ptr: ?*const anyopaque, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack_named(ptr, callstack_depth, 0, name);
        } else {
            c.___tracy_emit_memory_free_named(ptr, 0, name);
        }
    }
    pub inline fn SecureAllocN(ptr: ?*const anyopaque, size: usize, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack_named(ptr, size, callstack_depth, 1, name);
        } else {
            c.___tracy_emit_memory_alloc_named(ptr, size, 1, name);
        }
    }
    pub inline fn SecureFreeN(ptr: ?*const anyopaque, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack_named(ptr, callstack_depth, 1, name);
        } else {
            c.___tracy_emit_memory_free_named(ptr, 1, name);
        }
    }
    pub inline fn AllocNS(ptr: ?*const anyopaque, size: usize, depth: c_int, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack_named(ptr, size, depth, 0, name);
        } else {
            c.___tracy_emit_memory_alloc_named(ptr, size, 0, name);
        }
    }
    pub inline fn FreeNS(ptr: ?*const anyopaque, depth: c_int, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack_named(ptr, depth, 0, name);
        } else {
            c.___tracy_emit_memory_free_named(ptr, 0, name);
        }
    }
    pub inline fn SecureAllocNS(ptr: ?*const anyopaque, size: usize, depth: c_int, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_alloc_callstack_named(ptr, size, depth, 1, name);
        } else {
            c.___tracy_emit_memory_alloc_named(ptr, size, 1, name);
        }
    }
    pub inline fn SecureFreeNS(ptr: ?*const anyopaque, depth: c_int, name: [*:0]const u8) void {
        if (has_callstack_support) {
            c.___tracy_emit_memory_free_callstack_named(ptr, depth, 1, name);
        } else {
            c.___tracy_emit_memory_free_named(ptr, 1, name);
        }
    }

    pub inline fn Message(text: []const u8) void {
        c.___tracy_emit_message(text.ptr, text.len, callstack_depth);
    }
    pub inline fn MessageL(text: [*:0]const u8, color: u32) void {
        c.___tracy_emit_messageL(text, color, callstack_depth);
    }
    pub inline fn MessageC(text: []const u8, color: u32) void {
        c.___tracy_emit_messageC(text.ptr, text.len, color, callstack_depth);
    }
    pub inline fn MessageLC(text: [*:0]const u8, color: u32) void {
        c.___tracy_emit_messageLC(text, color, callstack_depth);
    }
    pub inline fn MessageS(text: []const u8, depth: c_int) void {
        const inner_depth: c_int = if (has_callstack_support) depth else 0;
        c.___tracy_emit_message(text.ptr, text.len, inner_depth);
    }
    pub inline fn MessageLS(text: [*:0]const u8, depth: c_int) void {
        const inner_depth: c_int = if (has_callstack_support) depth else 0;
        c.___tracy_emit_messageL(text, inner_depth);
    }
    pub inline fn MessageCS(text: []const u8, color: u32, depth: c_int) void {
        const inner_depth: c_int = if (has_callstack_support) depth else 0;
        c.___tracy_emit_messageC(text.ptr, text.len, color, inner_depth);
    }
    pub inline fn MessageLCS(text: [*:0]const u8, color: u32, depth: c_int) void {
        const inner_depth: c_int = if (has_callstack_support) depth else 0;
        c.___tracy_emit_messageLC(text, color, inner_depth);
    }

    pub inline fn FrameMark() void {
        c.___tracy_emit_frame_mark(null);
    }
    pub inline fn FrameMarkNamed(name: [*:0]const u8) void {
        c.___tracy_emit_frame_mark(name);
    }
    pub inline fn FrameMarkStart(name: [*:0]const u8) void {
        c.___tracy_emit_frame_mark_start(name);
    }
    pub inline fn FrameMarkEnd(name: [*:0]const u8) void {
        c.___tracy_emit_frame_mark_end(name);
    }
    pub inline fn FrameImage(image: ?*const anyopaque, width: u16, height: u16, offset: u8, flip: c_int) void {
        c.___tracy_emit_frame_image(image, width, height, offset, flip);
    }

    pub inline fn FiberEnter(name: [*:0]const u8) void {
        c.___tracy_fiber_enter(name);
    }
    pub inline fn FiberLeave() void {
        c.___tracy_fiber_leave();
    }

    pub inline fn PlotF(name: [*:0]const u8, val: f64) void {
        c.___tracy_emit_plot(name, val);
    }
    pub inline fn PlotU(name: [*:0]const u8, val: u64) void {
        c.___tracy_emit_plot(name, @as(f64, @floatFromInt(val)));
    }
    pub inline fn PlotI(name: [*:0]const u8, val: i64) void {
        c.___tracy_emit_plot(name, @as(f64, @floatFromInt(val)));
    }
    pub inline fn AppInfo(text: []const u8) void {
        c.___tracy_emit_message_appinfo(text.ptr, text.len);
    }

    pub const TracyAllocator = struct {
        child_allocator: std.mem.Allocator,

        pub fn init(child_allocator: std.mem.Allocator) TracyAllocator {
            return .{
                .child_allocator = child_allocator,
            };
        }

        pub fn allocator(self: *TracyAllocator) std.mem.Allocator {
            return .{
                .ptr = self,
                .vtable = &.{
                    .alloc = alloc,
                    .resize = resize,
                    .free = free,
                },
            };
        }

        fn alloc(
            ctx: *anyopaque,
            len: usize,
            log2_ptr_align: u8,
            ra: usize,
        ) ?[*]u8 {
            const self: *TracyAllocator = @ptrCast(@alignCast(ctx));
            const result = self.child_allocator.rawAlloc(len, log2_ptr_align, ra);
            if (result) |addr| {
                Alloc(addr, len);
            } else {
                var buffer: [128]u8 = undefined;
                const msg = std.fmt.bufPrint(&buffer, "alloc failed requesting {d}", .{len}) catch return result;
                Message(msg);
            }
            return result;
        }

        fn resize(
            ctx: *anyopaque,
            buf: []u8,
            log2_ptr_align: u8,
            new_len: usize,
            ra: usize,
        ) bool {
            const self: *TracyAllocator = @ptrCast(@alignCast(ctx));
            const result = self.child_allocator.rawResize(buf, log2_ptr_align, new_len, ra);
            if (result) {
                Free(buf.ptr);
                Alloc(buf.ptr, new_len);
            } else {
                var buffer: [128]u8 = undefined;
                const msg = std.fmt.bufPrint(&buffer, "resize failed requesting {d} -> {d}", .{ buf.len, new_len }) catch return result;
                Message(msg);
            }
            return result;
        }

        fn free(
            ctx: *anyopaque,
            buf: []u8,
            log2_ptr_align: u8,
            ra: usize,
        ) void {
            const self: *TracyAllocator = @ptrCast(@alignCast(ctx));
            self.child_allocator.rawFree(buf, log2_ptr_align, ra);
            Free(buf.ptr);
        }
    };
};

test {
    std.testing.refAllDeclsRecursive(@This());
}
