const std = @import("std");
const assert = std.debug.assert;

pub const version = std.SemanticVersion{ .major = 0, .minor = 1, .patch = 0 };

pub const Error = error{
    InvalidFontFormat,
};

var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, usize) = null;
const mem_alignment = 16;

// TODO: So that libc dependency can be removed, define remaining fns listed by TODO in zstbtt.c

extern var zstbttFreePtr: ?*const fn (
    maybe_ptr: ?*anyopaque,
    userdata: ?*anyopaque,
) callconv(.C) void;

extern var zstbttMallocPtr: ?*const fn (
    size: usize,
    userdata: ?*anyopaque,
) callconv(.C) ?*anyopaque;

extern var zstbttAssertPtr: ?*const fn (condition: c_int) callconv(.C) void;

fn zstbttMalloc(size: c_ulonglong, _: ?*anyopaque) callconv(.C) ?*anyopaque {
    const mem = mem_allocator.?.alignedAlloc(u8, mem_alignment, @intCast(usize, size)) catch {
        @panic("zstbtt: out of memory");
    };
    mem_allocations.?.put(@ptrToInt(mem.ptr), @intCast(usize, size)) catch {
        @panic("zstbtt: out of memory");
    };
    return mem.ptr;
}

fn zstbttFree(maybe_ptr: ?*anyopaque, _: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        const size = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;
        const aligned_slice = @ptrCast(
            [*]align(mem_alignment) u8,
            @alignCast(mem_alignment, ptr),
        );
        const mem = aligned_slice[0..size];
        mem_allocator.?.free(mem);
    }
}

fn zstbttAssert(condition: c_int) callconv(.C) void {
    assert(condition != 0);
}

pub fn init(allocator: std.mem.Allocator) void {
    assert(mem_allocator == null);
    mem_allocator = allocator;
    mem_allocations = std.AutoHashMap(usize, usize).init(allocator);

    zstbttMallocPtr = zstbttMalloc;
    zstbttFreePtr = zstbttFree;
    zstbttAssertPtr = zstbttAssert;
}

pub fn deinit() void {
    assert(mem_allocator != null);
    assert(mem_allocations.?.count() == 0);
    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}

/// NOTE: It's not recommended to ship code using the simple texture baking API but it's
/// ok for tools or a quick start
pub usingnamespace @import("texture_baking_api_simple.zig");

/// The improved texture baking API
pub usingnamespace @import("texture_baking_api.zig");

/////////////////////////////////////////////////////////////////////////////////////////
// ZIGIFIED WRAPPER

pub const FontVMetrics = struct {
    ascent: i16,
    descent: i16,
    line_gap: i16,
};

pub const GlyphHMetrics = struct {
    advance: i16,
    bearing: i16,
};

pub fn getNumberOfFonts(ttf_bytes: []const u8) Error!u32 {
    const num = stbtt_GetNumberOfFonts(ttf_bytes.ptr);
    return if (num == 0) Error.InvalidFontFormat else @intCast(u32, num);
}

pub fn getFontOffsetForIndex(ttf_bytes: []const u8, index: u32) Error!usize {
    const offset = stbtt_GetFontOffsetForIndex(ttf_bytes.ptr, @intCast(c_int, index));
    return if (offset < 0) return Error.InvalidFontFormat else @intCast(usize, offset);
}

pub fn initFont(ttf_bytes: []const u8, offset: usize) Error!FontInfo {
    var info = std.mem.zeroes(FontInfo);
    if (stbtt_InitFont(&info, ttf_bytes.ptr, @intCast(c_int, offset)) == 0) {
        return Error.InvalidFontFormat;
    }
    return info;
}

pub fn getFontVMetrics(font_info: *const FontInfo) FontVMetrics {
    var ascent: c_int = undefined;
    var descent: c_int = undefined;
    var line_gap: c_int = undefined;
    stbtt_GetFontVMetrics(font_info, &ascent, &descent, &line_gap);
    return .{
        .ascent = @intCast(i16, ascent),
        .descent = @intCast(i16, descent),
        .line_gap = @intCast(i16, line_gap),
    };
}

pub fn getCodepointHMetrics(font_info: *const FontInfo, codepoint: u32) GlyphHMetrics {
    var advance: c_int = undefined;
    var bearing: c_int = undefined;
    stbtt_GetCodepointHMetrics(font_info, @intCast(c_int, codepoint), &advance, &bearing);
    return .{
        .advance = @intCast(i16, advance),
        .bearing = @intCast(i16, bearing),
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
// C BINDINGS

/// stbtt_fontinfo
const FontInfo = extern struct {
    userdata: *anyopaque,
    data: [*]const u8,
    fontstart: c_int,
    numGlyphs: c_int,
    loca: c_int,
    head: c_int,
    hhea: c_int,
    hmtx: c_int,
    kern: c_int,
    gpos: c_int,
    svg: c_int,
    index_map: c_int,
    indexToLocFormat: c_int,
};

/// stbtt_kerningentry
const KerningEntry = extern struct {
    glyph1: c_int,
    glyph2: c_int,
    advance: c_int,
};

/// stbtt_vertex
const Vertex = extern struct {
    const vertex_type = c_short;

    x: vertex_type,
    y: vertex_type,
    cx: vertex_type,
    cy: vertex_type,
    cx1: vertex_type,
    cy1: vertex_type,
    type: u8,
    padding: u8,
};

extern fn stbtt_GetNumberOfFonts(data: [*]const u8) c_int;
extern fn stbtt_GetFontOffsetForIndex(data: [*]const u8, index: c_int) c_int;
extern fn stbtt_InitFont(info: *FontInfo, data: [*]const u8, offset: c_int) c_int;
extern fn stbtt_FindGlyphIndex(
    info: *const FontInfo,
    unicode_codepoint: c_int,
) c_int;
extern fn stbtt_ScaleForPixelHeight(info: *const FontInfo, pixels: f32) f32;
extern fn stbtt_ScaleForMappingEmToPixels(info: *const FontInfo, pixels: f32) f32;
extern fn stbtt_GetFontVMetrics(
    info: *const FontInfo,
    ascent: *c_int,
    descent: *c_int,
    lineGap: *c_int,
) void;
extern fn stbtt_GetFontVMetricsOS2(
    info: *const FontInfo,
    typoAscent: *c_int,
    typoDescent: *c_int,
    typoLineGap: *c_int,
) c_int;
extern fn stbtt_GetFontBoundingBox(
    info: *const FontInfo,
    x0: *c_int,
    y0: *c_int,
    x1: *c_int,
    y1: *c_int,
) void;
extern fn stbtt_GetCodepointHMetrics(
    info: *const FontInfo,
    codepoint: c_int,
    advanceWidth: *c_int,
    leftSideBearing: *c_int,
) void;
extern fn stbtt_GetCodepointKernAdvance(
    info: *const FontInfo,
    ch1: c_int,
    ch2: c_int,
) c_int;
extern fn stbtt_GetCodepointBox(
    info: *const FontInfo,
    codepoint: c_int,
    x0: *c_int,
    y0: *c_int,
    x1: *c_int,
    y1: *c_int,
) c_int;
extern fn stbtt_GetGlyphHMetrics(
    info: *const FontInfo,
    glyph_index: c_int,
    advanceWidth: *c_int,
    leftSideBearing: *c_int,
) void;
extern fn stbtt_GetGlyphKernAdvance(
    info: *const FontInfo,
    glyph1: c_int,
    glyph2: c_int,
) c_int;
extern fn stbtt_GetGlyphBox(
    info: *const FontInfo,
    glyph_index: c_int,
    x0: *c_int,
    y0: *c_int,
    x1: *c_int,
    y1: *c_int,
) c_int;
extern fn stbtt_GetKerningTableLength(info: *const FontInfo) c_int;
extern fn stbtt_GetKerningTable(
    info: *const FontInfo,
    table: *KerningEntry,
    table_length: c_int,
) c_int;
extern fn stbtt_IsGlyphEmpty(info: *const FontInfo, glyph_index: c_int) c_int;
extern fn stbtt_GetCodepointShape(
    info: *const FontInfo,
    unicode_codepoint: c_int,
    vertices: [*]Vertex,
) c_int;
extern fn stbtt_GetGlyphShape(
    info: *const FontInfo,
    glyph_index: c_int,
    vertices: [*]Vertex,
) c_int;
extern fn stbtt_FreeShape(
    info: *const FontInfo,
    vertices: [*]Vertex,
) void;
extern fn stbtt_FindSVGDoc(info: *const FontInfo, gl: c_int) ?[*]u8;
extern fn stbtt_GetCodepointSVG(
    info: *const FontInfo,
    unicode_codepoint: c_int,
    svg: [*]const u8,
) c_int;
extern fn stbtt_GetGlyphSVG(
    info: *const FontInfo,
    gl: c_int,
    svg: [*]const u8,
) c_int;

extern fn stbtt_FreeBitmap(bitmap: [*]u8, userdata: ?*anyopaque) void;
extern fn stbtt_GetCodepointBitmap(
    info: *const FontInfo,
    scale_x: f32,
    scale_y: f32,
    codepoint: c_int,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;
extern fn stbtt_GetCodepointBitmapSubpixel(
    info: *const FontInfo,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    codepoint: c_int,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;
extern fn stbtt_MakeCodepointBitmap(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    codepoint: c_int,
) void;
extern fn stbtt_MakeCodepointBitmapSubpixel(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    codepoint: c_int,
) void;
extern fn stbtt_MakeCodepointBitmapSubpixelPrefilter(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    oversample_x: c_int,
    oversample_y: c_int,
    sub_x: *f32,
    sub_y: *f32,
    codepoint: c_int,
) void;
extern fn stbtt_GetCodepointBitmapBox(
    info: *const FontInfo,
    codepoint: c_int,
    scale_x: f32,
    scale_y: f32,
    ix0: *c_int,
    iy0: *c_int,
    ix1: *c_int,
    iy1: *c_int,
) void;
extern fn stbtt_GetCodepointBitmapBoxSubpixel(
    info: *const FontInfo,
    codepoint: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    ix0: *c_int,
    iy0: *c_int,
    ix1: *c_int,
    iy1: *c_int,
) void;
extern fn stbtt_GetGlyphBitmap(
    info: *const FontInfo,
    scale_x: f32,
    scale_y: f32,
    glyph: c_int,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;
extern fn stbtt_GetGlyphBitmapSubpixel(
    info: *const FontInfo,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    glyph: c_int,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;
extern fn stbtt_MakeGlyphBitmap(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    glyph: c_int,
) void;
extern fn stbtt_MakeGlyphBitmapSubpixel(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    glyph: c_int,
) void;
extern fn stbtt_MakeGlyphBitmapSubpixelPrefilter(
    info: *const FontInfo,
    output: [*]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    oversample_x: c_int,
    oversample_y: c_int,
    sub_x: *f32,
    sub_y: *f32,
    glyph: c_int,
) void;
extern fn stbtt_GetGlyphBitmapBox(
    info: *const FontInfo,
    glyph: c_int,
    scale_x: f32,
    scale_y: f32,
    ix0: *c_int,
    iy0: *c_int,
    ix1: *c_int,
    iy1: *c_int,
) void;
extern fn stbtt_GetGlyphBitmapBoxSubpixel(
    info: *const FontInfo,
    glyph: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    ix0: *c_int,
    iy0: *c_int,
    ix1: *c_int,
    iy1: *c_int,
) void;

extern fn stbtt_FreeSDF(bitmap: [*]u8, userdata: ?*anyopaque) void;
extern fn stbtt_GetGlyphSDF(
    info: *const FontInfo,
    scale: f32,
    glyph: c_int,
    padding: c_int,
    onedge_value: c_uint,
    pixel_dist_scale: f32,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;
extern fn stbtt_GetCodepointSDF(
    info: *const FontInfo,
    scale: f32,
    codepoint: c_int,
    padding: c_int,
    onedge_value: c_uint,
    pixel_dist_scale: f32,
    width: *c_int,
    height: *c_int,
    xoff: *c_int,
    yoff: *c_int,
) ?[*]u8;

/////////////////////////////////////////////////////////////////////////////////////////
// TESTS

test {
    std.testing.refAllDecls(@This());

    init(std.testing.allocator);
    defer deinit();
}
