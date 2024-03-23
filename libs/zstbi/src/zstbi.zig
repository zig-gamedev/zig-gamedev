const std = @import("std");
const testing = std.testing;
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

    setFlipVerticallyOnLoad(false);
    setFlipVerticallyOnWrite(false);

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
            .width = @as(u32, @intCast(w)),
            .height = @as(u32, @intCast(h)),
            .num_components = @as(u32, @intCast(c)),
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
                @as(c_int, @intCast(forced_num_components)),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @as(u32, @intCast(ch)) else forced_num_components;
            width = @as(u32, @intCast(x));
            height = @as(u32, @intCast(y));
            bytes_per_component = 2;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = true;

            // Convert each component from f32 to f16.
            var ptr_f16 = @as([*]f16, @ptrCast(ptr.?));
            const num = width * height * num_components;
            var i: u32 = 0;
            while (i < num) : (i += 1) {
                ptr_f16[i] = @as(f16, @floatCast(ptr.?[i]));
            }
            break :data @as([*]u8, @ptrCast(ptr_f16))[0 .. height * bytes_per_row];
        } else data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const is_16bit = is16bit(pathname);
            const ptr = if (is_16bit) @as(?[*]u8, @ptrCast(stbi_load_16(
                pathname,
                &x,
                &y,
                &ch,
                @as(c_int, @intCast(forced_num_components)),
            ))) else stbi_load(
                pathname,
                &x,
                &y,
                &ch,
                @as(c_int, @intCast(forced_num_components)),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @as(u32, @intCast(ch)) else forced_num_components;
            width = @as(u32, @intCast(x));
            height = @as(u32, @intCast(y));
            bytes_per_component = if (is_16bit) 2 else 1;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = false;

            break :data @as([*]u8, @ptrCast(ptr))[0 .. height * bytes_per_row];
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
                @as(c_int, @intCast(data.len)),
                &x,
                &y,
                &ch,
                @as(c_int, @intCast(forced_num_components)),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @as(u32, @intCast(ch)) else forced_num_components;
            width = @as(u32, @intCast(x));
            height = @as(u32, @intCast(y));
            bytes_per_component = 2;
            bytes_per_row = width * num_components * bytes_per_component;
            is_hdr = true;

            // Convert each component from f32 to f16.
            var ptr_f16 = @as([*]f16, @ptrCast(ptr.?));
            const num = width * height * num_components;
            var i: u32 = 0;
            while (i < num) : (i += 1) {
                ptr_f16[i] = @as(f16, @floatCast(ptr.?[i]));
            }
            break :data @as([*]u8, @ptrCast(ptr_f16))[0 .. height * bytes_per_row];
        } else data: {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            const ptr = stbi_load_from_memory(
                data.ptr,
                @as(c_int, @intCast(data.len)),
                &x,
                &y,
                &ch,
                @as(c_int, @intCast(forced_num_components)),
            );
            if (ptr == null) return error.ImageInitFailed;

            num_components = if (forced_num_components == 0) @as(u32, @intCast(ch)) else forced_num_components;
            width = @as(u32, @intCast(x));
            height = @as(u32, @intCast(y));
            bytes_per_component = 1;
            bytes_per_row = width * num_components * bytes_per_component;

            break :data @as([*]u8, @ptrCast(ptr))[0 .. height * bytes_per_row];
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

        const data = @as([*]u8, @ptrCast(zstbiMalloc(size)));
        @memset(data[0..size], 0);

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
        const new_data = @as([*]u8, @ptrCast(zstbiMalloc(new_size)));
        stbir_resize_uint8(
            image.data.ptr,
            @as(c_int, @intCast(image.width)),
            @as(c_int, @intCast(image.height)),
            0,
            new_data,
            @as(c_int, @intCast(new_width)),
            @as(c_int, @intCast(new_height)),
            0,
            @as(c_int, @intCast(image.num_components)),
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
        image: Image,
        filename: [:0]const u8,
        image_format: ImageWriteFormat,
    ) ImageWriteError!void {
        assert(mem_allocator != null);

        const w = @as(c_int, @intCast(image.width));
        const h = @as(c_int, @intCast(image.height));
        const comp = @as(c_int, @intCast(image.num_components));
        const result = switch (image_format) {
            .png => stbi_write_png(filename.ptr, w, h, comp, image.data.ptr, 0),
            .jpg => |settings| stbi_write_jpg(
                filename.ptr,
                w,
                h,
                comp,
                image.data.ptr,
                @as(c_int, @intCast(settings.quality)),
            ),
        };
        // if the result is 0 then it means an error occured (per stb image write docs)
        if (result == 0) {
            return ImageWriteError.CouldNotWriteImage;
        }
    }

    pub fn writeToFn(
        image: Image,
        write_fn: *const fn (ctx: ?*anyopaque, data: ?*anyopaque, size: c_int) callconv(.C) void,
        context: ?*anyopaque,
        image_format: ImageWriteFormat,
    ) ImageWriteError!void {
        assert(mem_allocator != null);

        const w = @as(c_int, @intCast(image.width));
        const h = @as(c_int, @intCast(image.height));
        const comp = @as(c_int, @intCast(image.num_components));
        const result = switch (image_format) {
            .png => stbi_write_png_to_func(write_fn, context, w, h, comp, image.data.ptr, 0),
            .jpg => |settings| stbi_write_jpg_to_func(
                write_fn,
                context,
                w,
                h,
                comp,
                image.data.ptr,
                @as(c_int, @intCast(settings.quality)),
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
    return stbi_is_hdr_from_memory(buffer.ptr, @as(c_int, @intCast(buffer.len))) != 0;
}

pub fn is16bit(filename: [:0]const u8) bool {
    return stbi_is_16_bit(filename) != 0;
}

pub fn setFlipVerticallyOnLoad(should_flip: bool) void {
    stbi_set_flip_vertically_on_load(if (should_flip) 1 else 0);
}

pub fn setFlipVerticallyOnWrite(should_flip: bool) void {
    stbi_flip_vertically_on_write(if (should_flip) 1 else 0);
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

    mem_allocations.?.put(@intFromPtr(mem.ptr), size) catch @panic("zstbi: out of memory");

    return mem.ptr;
}

extern var zstbiReallocPtr: ?*const fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque;
extern var zstbiwReallocPtr: ?*const fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque;

fn zstbiRealloc(ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const old_size = if (ptr != null) mem_allocations.?.get(@intFromPtr(ptr.?)).? else 0;
    const old_mem = if (old_size > 0)
        @as([*]align(mem_alignment) u8, @ptrCast(@alignCast(ptr)))[0..old_size]
    else
        @as([*]align(mem_alignment) u8, undefined)[0..0];

    const new_mem = mem_allocator.?.realloc(old_mem, size) catch @panic("zstbi: out of memory");

    if (ptr != null) {
        const removed = mem_allocations.?.remove(@intFromPtr(ptr.?));
        std.debug.assert(removed);
    }

    mem_allocations.?.put(@intFromPtr(new_mem.ptr), size) catch @panic("zstbi: out of memory");

    return new_mem.ptr;
}

extern var zstbiFreePtr: ?*const fn (maybe_ptr: ?*anyopaque) callconv(.C) void;
extern var zstbiwFreePtr: ?*const fn (maybe_ptr: ?*anyopaque) callconv(.C) void;

fn zstbiFree(maybe_ptr: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const size = mem_allocations.?.fetchRemove(@intFromPtr(ptr)).?.value;
        const mem = @as([*]align(mem_alignment) u8, @ptrCast(@alignCast(ptr)))[0..size];
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
extern fn stbi_flip_vertically_on_write(flag: c_int) void; // flag is non-zero to flip data vertically

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

test "zstbi basic" {
    init(testing.allocator);
    defer deinit();

    var im1 = try Image.createEmpty(8, 6, 4, .{});
    defer im1.deinit();

    try testing.expect(im1.width == 8);
    try testing.expect(im1.height == 6);
    try testing.expect(im1.num_components == 4);
}

test "zstbi resize" {
    init(testing.allocator);
    defer deinit();

    var im1 = try Image.createEmpty(32, 32, 4, .{});
    defer im1.deinit();

    var im2 = im1.resize(8, 6);
    defer im2.deinit();

    try testing.expect(im2.width == 8);
    try testing.expect(im2.height == 6);
    try testing.expect(im2.num_components == 4);
}

test "zstbi write and load file" {
    init(testing.allocator);
    defer deinit();

    const pth = try std.fs.selfExeDirPathAlloc(testing.allocator);
    defer testing.allocator.free(pth);
    try std.posix.chdir(pth);

    var img = try Image.createEmpty(8, 6, 4, .{});
    defer img.deinit();

    try img.writeToFile("test_img.png", ImageWriteFormat.png);
    try img.writeToFile("test_img.jpg", .{ .jpg = .{ .quality = 80 } });

    var img_png = try Image.loadFromFile("test_img.png", 0);
    defer img_png.deinit();

    try testing.expect(img_png.width == img.width);
    try testing.expect(img_png.height == img.height);
    try testing.expect(img_png.num_components == img.num_components);

    var img_jpg = try Image.loadFromFile("test_img.jpg", 0);
    defer img_jpg.deinit();

    try testing.expect(img_jpg.width == img.width);
    try testing.expect(img_jpg.height == img.height);
    try testing.expect(img_jpg.num_components == 3); // RGB JPEG

    try std.fs.cwd().deleteFile("test_img.png");
    try std.fs.cwd().deleteFile("test_img.jpg");
}
