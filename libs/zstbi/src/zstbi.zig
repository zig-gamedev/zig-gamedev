pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 9, .patch = 3 };
const std = @import("std");
const assert = std.debug.assert;

pub fn init(allocator: std.mem.Allocator) void {
    assert(mem_allocator == null);
    mem_allocator = allocator;
    mem_allocations = std.AutoHashMap(usize, usize).init(allocator);

    // stb image
    zstbiMallocPtr = zstbiMalloc;
    zstbiReallocPtr = zstbiRealloc;
    zstbiFreePtr = zstbiFree;
    // stb image resize
    zstbirMallocPtr = zstbirMalloc;
    zstbirFreePtr = zstbirFree;
    // stb image write
    zstbiwMallocPtr = zstbiMalloc;
    zstbiwReallocPtr = zstbiRealloc;
    zstbiwFreePtr = zstbiFree;
}

pub fn deinit() void {
    assert(mem_allocator != null);
    assert(mem_allocations.?.count() == 0);
    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}

pub const JpgWriteSettings = struct {
    quality: u32,
};

pub const ImageWriteFormat = union(enum) {
    png,
    jpg: JpgWriteSettings,
};

pub const ImageWriteError = error{
    CouldNotWriteImage,
};

pub const Image = struct {
    data: []u8,
    width: u32,
    height: u32,
    num_components: u32,
    bytes_per_component: u32,
    bytes_per_row: u32,
    is_hdr: bool,

    pub fn info(pathname: [:0]const u8) struct {
        is_supported: bool,
        width: u32,
        height: u32,
        num_components: u32,
    } {
        assert(mem_allocator != null);

        var w: c_int = 0;
        var h: c_int = 0;
        var c: c_int = 0;
        const is_supported = stbi_info(pathname, &w, &h, &c);
        return .{
            .is_supported = if (is_supported == 1) true else false,
            .width = @intCast(u32, w),
            .height = @intCast(u32, h),
            .num_components = @intCast(u32, c),
        };
    }

    pub fn loadFromFile(pathname: [:0]const u8, forced_num_components: u32) !Image {
        assert(mem_allocator != null);

        var width: u32 = 0;
        var height: u32 = 0;
        var num_components: u32 = 0;
        var bytes_per_component: u32 = 0;
        var bytes_per_row: u32 = 0;
        var is_hdr = false;

        const data = if (isHdr(pathname)) data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const ptr = stbi_loadf(
                pathname,
                &x,
                &y,
                &ch,
                @intCast(c_int, forced_num_components),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @intCast(u32, ch) else forced_num_components;
            width = @intCast(u32, x);
            height = @intCast(u32, y);
            bytes_per_component = 2;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = true;

            // Convert each component from f32 to f16.
            var ptr_f16 = @ptrCast([*]f16, ptr.?);
            const num = width * height * num_components;
            var i: u32 = 0;
            while (i < num) : (i += 1) {
                ptr_f16[i] = @floatCast(f16, ptr.?[i]);
            }
            break :data @ptrCast([*]u8, ptr_f16)[0 .. height * bytes_per_row];
        } else data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const is_16bit = is16bit(pathname);
            const ptr = if (is_16bit) @ptrCast(?[*]u8, stbi_load_16(
                pathname,
                &x,
                &y,
                &ch,
                @intCast(c_int, forced_num_components),
            )) else stbi_load(
                pathname,
                &x,
                &y,
                &ch,
                @intCast(c_int, forced_num_components),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @intCast(u32, ch) else forced_num_components;
            width = @intCast(u32, x);
            height = @intCast(u32, y);
            bytes_per_component = if (is_16bit) 2 else 1;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = false;

            break :data @ptrCast([*]u8, ptr)[0 .. height * bytes_per_row];
        };

        return Image{
            .data = data,
            .width = width,
            .height = height,
            .num_components = num_components,
            .bytes_per_component = bytes_per_component,
            .bytes_per_row = bytes_per_row,
            .is_hdr = is_hdr,
        };
    }

    pub fn loadFromMemory(data: []const u8, forced_num_components: u32) !Image {
        assert(mem_allocator != null);

        var width: u32 = 0;
        var height: u32 = 0;
        var num_components: u32 = 0;
        var bytes_per_component: u32 = 0;
        var bytes_per_row: u32 = 0;
        var is_hdr = false;

        const image_data = if (isHdrFromMem(data)) data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const ptr = stbi_loadf_from_memory(
                data.ptr,
                @intCast(c_int, data.len),
                &x,
                &y,
                &ch,
                @intCast(c_int, forced_num_components),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @intCast(u32, ch) else forced_num_components;
            width = @intCast(u32, x);
            height = @intCast(u32, y);
            bytes_per_component = 2;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = true;

            // Convert each component from f32 to f16.
            var ptr_f16 = @ptrCast([*]f16, ptr.?);
            const num = width * height * num_components;
            var i: u32 = 0;
            while (i < num) : (i += 1) {
                ptr_f16[i] = @floatCast(f16, ptr.?[i]);
            }
            break :data @ptrCast([*]u8, ptr_f16)[0 .. height * bytes_per_row];
        } else data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const ptr = stbi_load_from_memory(
                data.ptr,
                @intCast(c_int, data.len),
                &x,
                &y,
                &ch,
                @intCast(c_int, forced_num_components),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @intCast(u32, ch) else forced_num_components;
            width = @intCast(u32, x);
            height = @intCast(u32, y);
            bytes_per_component = 1;
            bytes_per_row = width * num_components * bytes_per_component;

            break :data @ptrCast([*]u8, ptr)[0 .. height * bytes_per_row];
        };

        return Image{
            .data = image_data,
            .width = width,
            .height = height,
            .num_components = num_components,
            .bytes_per_component = bytes_per_component,
            .bytes_per_row = bytes_per_row,
            .is_hdr = is_hdr,
        };
    }

    pub fn createEmpty(width: u32, height: u32, num_components: u32, args: struct {
        bytes_per_component: u32 = 0,
        bytes_per_row: u32 = 0,
    }) !Image {
        assert(mem_allocator != null);

        const bytes_per_component = if (args.bytes_per_component == 0) 1 else args.bytes_per_component;
        const bytes_per_row = if (args.bytes_per_row == 0)
            width * num_components * bytes_per_component
        else
            args.bytes_per_row;

        const size = height * bytes_per_row;

        const data = @ptrCast([*]u8, zstbiMalloc(size));
        @memset(data, 0, size);

        return Image{
            .data = data[0..size],
            .width = width,
            .height = height,
            .num_components = num_components,
            .bytes_per_component = bytes_per_component,
            .bytes_per_row = bytes_per_row,
            .is_hdr = false,
        };
    }

    pub fn resize(image: *const Image, new_width: u32, new_height: u32) Image {
        assert(mem_allocator != null);

        // TODO: Add support for HDR images
        const new_bytes_per_row = new_width * image.num_components * image.bytes_per_component;
        const new_size = new_height * new_bytes_per_row;
        const new_data = @ptrCast([*]u8, zstbiMalloc(new_size));
        stbir_resize_uint8(
            image.data.ptr,
            @intCast(c_int, image.width),
            @intCast(c_int, image.height),
            0,
            new_data,
            @intCast(c_int, new_width),
            @intCast(c_int, new_height),
            0,
            @intCast(c_int, image.num_components),
        );
        return .{
            .data = new_data[0..new_size],
            .width = new_width,
            .height = new_height,
            .num_components = image.num_components,
            .bytes_per_component = image.bytes_per_component,
            .bytes_per_row = new_bytes_per_row,
            .is_hdr = image.is_hdr,
        };
    }

    pub fn writeToFile(
        image: *const Image,
        filename: [:0]const u8,
        image_format: ImageWriteFormat,
    ) ImageWriteError!void {
        assert(mem_allocator != null);

        const w = @intCast(c_int, image.width);
        const h = @intCast(c_int, image.height);
        const comp = @intCast(c_int, image.num_components);
        const result = switch (image_format) {
            .png => stbi_write_png(filename.ptr, w, h, comp, image.data.ptr, 0),
            .jpg => |settings| stbi_write_jpg(
                filename.ptr,
                w,
                h,
                comp,
                image.data.ptr,
                @intCast(c_int, settings.quality),
            ),
        };
        // if the result is 0 then it means an error occured (per stb image write docs)
        if (result == 0) {
            return ImageWriteError.CouldNotWriteImage;
        }
    }

    pub fn writeToFn(
        image: *const Image,
        write_fn: *const fn (ctx: ?*anyopaque, data: ?*anyopaque, size: c_int) callconv(.C) void,
        context: ?*anyopaque,
        image_format: ImageWriteFormat,
    ) ImageWriteError!void {
        assert(mem_allocator != null);

        const w = @intCast(c_int, image.width);
        const h = @intCast(c_int, image.height);
        const comp = @intCast(c_int, image.num_components);
        const result = switch (image_format) {
            .png => stbi_write_png_to_func(write_fn, context, w, h, comp, image.data.ptr, 0),
            .jpg => |settings| stbi_write_jpg_to_func(
                write_fn,
                context,
                w,
                h,
                comp,
                image.data.ptr,
                @intCast(c_int, settings.quality),
            ),
        };
        // if the result is 0 then it means an error occured (per stb image write docs)
        if (result == 0) {
            return ImageWriteError.CouldNotWriteImage;
        }
    }

    pub fn deinit(image: *Image) void {
        stbi_image_free(image.data.ptr);
        image.* = undefined;
    }
};

/// `pub fn setHdrToLdrScale(scale: f32) void`
pub const setHdrToLdrScale = stbi_hdr_to_ldr_scale;

/// `pub fn setHdrToLdrGamma(gamma: f32) void`
pub const setHdrToLdrGamma = stbi_hdr_to_ldr_gamma;

/// `pub fn setLdrToHdrScale(scale: f32) void`
pub const setLdrToHdrScale = stbi_ldr_to_hdr_scale;

/// `pub fn setLdrToHdrGamma(gamma: f32) void`
pub const setLdrToHdrGamma = stbi_ldr_to_hdr_gamma;

pub fn isHdr(filename: [:0]const u8) bool {
    return stbi_is_hdr(filename) != 0;
}

pub fn isHdrFromMem(buffer: []const u8) bool {
    return stbi_is_hdr_from_memory(buffer.ptr, @intCast(c_int, buffer.len)) != 0;
}

pub fn is16bit(filename: [:0]const u8) bool {
    return stbi_is_16_bit(filename) != 0;
}

pub fn setFlipVerticallyOnLoad(should_flip: bool) void {
    stbi_set_flip_vertically_on_load(if (should_flip) 1 else 0);
}

var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, usize) = null;
var mem_mutex: std.Thread.Mutex = .{};
const mem_alignment = 16;

extern var zstbiMallocPtr: ?*const fn (size: usize) callconv(.C) ?*anyopaque;
extern var zstbiwMallocPtr: ?*const fn (size: usize) callconv(.C) ?*anyopaque;

fn zstbiMalloc(size: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const mem = mem_allocator.?.alignedAlloc(
        u8,
        mem_alignment,
        size,
    ) catch @panic("zstbi: out of memory");

    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch @panic("zstbi: out of memory");

    return mem.ptr;
}

extern var zstbiReallocPtr: ?*const fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque;
extern var zstbiwReallocPtr: ?*const fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque;

fn zstbiRealloc(ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const old_size = if (ptr != null) mem_allocations.?.get(@ptrToInt(ptr.?)).? else 0;
    const old_mem = if (old_size > 0)
        @ptrCast([*]align(mem_alignment) u8, @alignCast(mem_alignment, ptr))[0..old_size]
    else
        @as([*]align(mem_alignment) u8, undefined)[0..0];

    const new_mem = mem_allocator.?.realloc(old_mem, size) catch @panic("zstbi: out of memory");

    if (ptr != null) {
        const removed = mem_allocations.?.remove(@ptrToInt(ptr.?));
        std.debug.assert(removed);
    }

    mem_allocations.?.put(@ptrToInt(new_mem.ptr), size) catch @panic("zstbi: out of memory");

    return new_mem.ptr;
}

extern var zstbiFreePtr: ?*const fn (maybe_ptr: ?*anyopaque) callconv(.C) void;
extern var zstbiwFreePtr: ?*const fn (maybe_ptr: ?*anyopaque) callconv(.C) void;

fn zstbiFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const size = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;
        const mem = @ptrCast([*]align(mem_alignment) u8, @alignCast(mem_alignment, ptr))[0..size];
        mem_allocator.?.free(mem);
    }
}

extern var zstbirMallocPtr: ?*const fn (size: usize, maybe_context: ?*anyopaque) callconv(.C) ?*anyopaque;

fn zstbirMalloc(size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque {
    return zstbiMalloc(size);
}

extern var zstbirFreePtr: ?*const fn (maybe_ptr: ?*anyopaque, maybe_context: ?*anyopaque) callconv(.C) void;

fn zstbirFree(maybe_ptr: ?*anyopaque, _: ?*anyopaque) callconv(.C) void {
    zstbiFree(maybe_ptr);
}

extern fn stbi_info(filename: [*:0]const u8, x: *c_int, y: *c_int, comp: *c_int) c_int;

extern fn stbi_load(
    filename: [*:0]const u8,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]u8;

extern fn stbi_load_16(
    filename: [*:0]const u8,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]u16;

extern fn stbi_loadf(
    filename: [*:0]const u8,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]f32;

pub extern fn stbi_load_from_memory(
    buffer: [*]const u8,
    len: c_int,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]u8;

pub extern fn stbi_loadf_from_memory(
    buffer: [*]const u8,
    len: c_int,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]f32;

extern fn stbi_image_free(image_data: ?[*]u8) void;

extern fn stbi_hdr_to_ldr_scale(scale: f32) void;
extern fn stbi_hdr_to_ldr_gamma(gamma: f32) void;
extern fn stbi_ldr_to_hdr_scale(scale: f32) void;
extern fn stbi_ldr_to_hdr_gamma(gamma: f32) void;

extern fn stbi_is_16_bit(filename: [*:0]const u8) c_int;
extern fn stbi_is_hdr(filename: [*:0]const u8) c_int;
extern fn stbi_is_hdr_from_memory(buffer: [*]const u8, len: c_int) c_int;

extern fn stbi_set_flip_vertically_on_load(flag_true_if_should_flip: c_int) void;

extern fn stbir_resize_uint8(
    input_pixels: [*]const u8,
    input_w: c_int,
    input_h: c_int,
    input_stride_in_bytes: c_int,
    output_pixels: [*]u8,
    output_w: c_int,
    output_h: c_int,
    output_stride_in_bytes: c_int,
    num_channels: c_int,
) void;

extern fn stbi_write_jpg(
    filename: [*:0]const u8,
    w: c_int,
    h: c_int,
    comp: c_int,
    data: [*]const u8,
    quality: c_int,
) c_int;
extern fn stbi_write_png(
    filename: [*:0]const u8,
    w: c_int,
    h: c_int,
    comp: c_int,
    data: [*]const u8,
    stride_in_bytes: c_int,
) c_int;

extern fn stbi_write_png_to_func(
    func: *const fn (?*anyopaque, ?*anyopaque, c_int) callconv(.C) void,
    context: ?*anyopaque,
    w: c_int,
    h: c_int,
    comp: c_int,
    data: [*]const u8,
    stride_in_bytes: c_int,
) c_int;

extern fn stbi_write_jpg_to_func(
    func: *const fn (?*anyopaque, ?*anyopaque, c_int) callconv(.C) void,
    context: ?*anyopaque,
    x: c_int,
    y: c_int,
    comp: c_int,
    data: [*]const u8,
    quality: c_int,
) c_int;

test "zstbi.basic" {
    init(std.testing.allocator);
    defer deinit();

    var image = try Image.createEmpty(64, 64, 4, .{});
    defer image.deinit();
}
