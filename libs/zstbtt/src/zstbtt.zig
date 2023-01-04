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

extern var zstbttMallocPtr: ?*const fn (
    size: usize,
    userdata: ?*anyopaque,
) callconv(.C) ?*anyopaque;
extern var zstbttFreePtr: ?*const fn (
    maybe_ptr: ?*anyopaque,
    userdata: ?*anyopaque,
) callconv(.C) void;

fn zstbttMalloc(size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque {
    const mem = mem_allocator.?.alignedAlloc(u8, mem_alignment, size) catch {
        @panic("zstbtt: out of memory");
    };
    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch {
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

pub fn init(allocator: std.mem.Allocator) void {
    std.debug.assert(mem_allocator == null);
    mem_allocator = allocator;
    mem_allocations = std.AutoHashMap(usize, usize).init(allocator);

    zstbttMallocPtr = zstbttMalloc;
    zstbttFreePtr = zstbttFree;
}

pub fn deinit() void {
    std.debug.assert(mem_allocator != null);
    std.debug.assert(mem_allocations.?.count() == 0);
    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}

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

pub const STBRP_Coord = c_int;

pub const STBRPContext = struct {
    width: c_int,
    height: c_int,
    x: c_int,
    y: c_int,
    bottom_y: c_int,
};

pub const STBRPRect = struct {
    x: STBRP_Coord,
    y: STBRP_Coord,
    id: c_int,
    w: c_int,
    h: c_int,
    was_packed: c_int,
};

pub const PackContext = extern struct {
    user_allocator_context: ?*anyopaque,
    pack_info: ?*anyopaque,
    width: c_int,
    height: c_int,
    stride_in_bytes: c_int,
    padding: c_int,
    skip_missing: c_int,
    h_oversample: c_int,
    v_oversample: c_int,
    pixels: [*c]u8,
    nodes: ?*anyopaque,
};

pub const PackedChar = extern struct {
    x0: c_ushort,
    y0: c_ushort,
    x1: c_ushort,
    y1: c_ushort,
    xoff: f32,
    yoff: f32,
    xadvance: f32,
    xoff2: f32,
    yoff2: f32,
};

pub const PackRange = extern struct {
    font_size: f32,
    first_unicode_codepoint_in_range: c_int,
    array_of_unicode_codepoints: [*c]c_int,
    num_chars: c_int,
    chardata_for_range: *PackedChar,
    h_oversample: u8,
    v_oversample: u8,
};

pub const AlignedQuad = extern struct {
    x0: f32,
    y0: f32,
    s0: f32,
    t0: f32,
    x1: f32,
    y1: f32,
    s1: f32,
    t1: f32,
};

extern fn stbtt_PackBegin(
    spc: *PackContext,
    pixels: [*c]u8,
    width: c_int,
    height: c_int,
    stride_in_bytes: c_int,
    padding: c_int,
    alloc_context: ?*anyopaque,
) c_int;
extern fn stbtt_PackEnd(spc: *PackContext) void;
extern fn stbtt_PackFontRange(
    spc: *PackContext,
    fontdata: [*c]const u8,
    font_index: c_int,
    font_size: f32,
    first_unicode_char_in_range: c_int,
    num_chars_in_range: c_int,
    chardata_for_range: *PackedChar,
) c_int;
extern fn stbtt_PackFontRanges(
    spc: *PackContext,
    fontdata: [*c]const u8,
    font_index: c_int,
    ranges: [*c]PackRange,
    num_ranges: c_int,
) c_int;
extern fn stbtt_PackSetOversampling(
    spc: *PackContext,
    h_oversample: c_uint,
    v_oversample: c_uint,
) void;
extern fn stbtt_PackSetSkipMissingCodepoints(spc: *PackContext, skip: c_int) void;
extern fn stbtt_GetPackedQuad(
    chardata: [*c]const PackedChar,
    pw: c_int,
    ph: c_int,
    char_index: c_int,
    xpos: *f32,
    ypos: *f32,
    q: *AlignedQuad,
    align_to_integer: c_int,
) void;
extern fn stbtt_PackFontRangesGatherRects(
    spc: *PackContext,
    info: *const FontInfo,
    ranges: [*c]PackRange,
    num_ranges: c_int,
    rects: [*c]STBRPRect,
) c_int;
extern fn stbtt_PackFontRangesPackRects(
    spc: *PackContext,
    rects: [*c]STBRPRect,
    num_rects: c_int,
) void;
extern fn stbtt_PackFontRangesRenderIntoRects(
    spc: *PackContext,
    info: *const FontInfo,
    ranges: [*c]PackRange,
    num_ranges: c_int,
    rects: [*c]STBRPRect,
) c_int;

pub const FontInfo = extern struct {
    userdata: *anyopaque,
    data: [*c]const u8,
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

pub const KerningEntry = extern struct {
    glyph1: c_int,
    glyph2: c_int,
    advance: c_int,
};

// TODO: Provide a way to override vertex definition like underlying API allows stbtt_vertex to be overidden
pub const Vertex = extern struct {
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

extern fn stbtt_GetNumberOfFonts(data: [*c]const u8) c_int;
extern fn stbtt_GetFontOffsetForIndex(data: [*c]const u8, index: c_int) c_int;
extern fn stbtt_InitFont(info: *FontInfo, data: [*c]const u8, offset: c_int) c_int;
extern fn stbtt_FindGlyphIndex(
    info: [*c]const FontInfo,
    unicode_codepoint: c_int,
) c_int;
extern fn stbtt_ScaleForPixelHeight(info: [*c]const FontInfo, pixels: f32) f32;
extern fn stbtt_ScaleForMappingEmToPixels(info: [*c]const FontInfo, pixels: f32) f32;
extern fn stbtt_GetFontVMetrics(
    info: [*c]const FontInfo,
    ascent: [*c]c_int,
    descent: [*c]c_int,
    lineGap: [*c]c_int,
) void;
extern fn stbtt_GetFontVMetricsOS2(
    info: [*c]const FontInfo,
    typoAscent: [*c]c_int,
    typoDescent: [*c]c_int,
    typoLineGap: [*c]c_int,
) c_int;
extern fn stbtt_GetFontBoundingBox(
    info: [*c]const FontInfo,
    x0: [*c]c_int,
    y0: [*c]c_int,
    x1: [*c]c_int,
    y1: [*c]c_int,
) void;
extern fn stbtt_GetCodepointHMetrics(
    info: [*c]const FontInfo,
    codepoint: c_int,
    advanceWidth: [*c]c_int,
    leftSideBearing: [*c]c_int,
) void;
extern fn stbtt_GetCodepointKernAdvance(
    info: [*c]const FontInfo,
    ch1: c_int,
    ch2: c_int,
) c_int;
extern fn stbtt_GetCodepointBox(
    info: [*c]const FontInfo,
    codepoint: c_int,
    x0: [*c]c_int,
    y0: [*c]c_int,
    x1: [*c]c_int,
    y1: [*c]c_int,
) c_int;
extern fn stbtt_GetGlyphHMetrics(
    info: [*c]const FontInfo,
    glyph_index: c_int,
    advanceWidth: [*c]c_int,
    leftSideBearing: [*c]c_int,
) void;
extern fn stbtt_GetGlyphKernAdvance(
    info: [*c]const FontInfo,
    glyph1: c_int,
    glyph2: c_int,
) c_int;
extern fn stbtt_GetGlyphBox(
    info: [*c]const FontInfo,
    glyph_index: c_int,
    x0: [*c]c_int,
    y0: [*c]c_int,
    x1: [*c]c_int,
    y1: [*c]c_int,
) c_int;
extern fn stbtt_GetKerningTableLength(info: [*c]const FontInfo) c_int;
extern fn stbtt_GetKerningTable(
    info: [*c]const FontInfo,
    table: *KerningEntry,
    table_length: c_int,
) c_int;
extern fn stbtt_IsGlyphEmpty(info: [*c]const FontInfo, glyph_index: c_int) c_int;
extern fn stbtt_GetCodepointShape(
    info: [*c]const FontInfo,
    unicode_codepoint: c_int,
    vertices: [*c][*c]Vertex,
) c_int;
extern fn stbtt_GetGlyphShape(
    info: [*c]const FontInfo,
    glyph_index: c_int,
    vertices: [*c][*c]Vertex,
) c_int;
extern fn stbtt_FreeShape(
    info: [*c]const FontInfo,
    vertices: [*]*Vertex,
) void;
extern fn stbtt_FindSVGDoc(info: [*c]const FontInfo, gl: c_int) ?[*c]u8;
extern fn stbtt_GetCodepointSVG(
    info: [*c]const FontInfo,
    unicode_codepoint: c_int,
    svg: [*c]const [*c]const u8,
) c_int;
extern fn stbtt_GetGlyphSVG(
    info: [*c]const FontInfo,
    gl: c_int,
    svg: [*c]const [*c]const u8,
) c_int;

extern fn stbtt_FreeBitmap(bitmap: [*c]u8, userdata: ?[*c]anyopaque) void;
extern fn stbtt_GetCodepointBitmap(
    info: [*c]const FontInfo,
    scale_x: f32,
    scale_y: f32,
    codepoint: c_int,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;
extern fn stbtt_GetCodepointBitmapSubpixel(
    info: [*c]const FontInfo,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    codepoint: c_int,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;
extern fn stbtt_MakeCodepointBitmap(
    info: [*c]const FontInfo,
    output: [*c]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    codepoint: c_int,
) void;
extern fn stbtt_MakeCodepointBitmapSubpixel(
    info: [*c]const FontInfo,
    output: [*c]u8,
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
    info: [*c]const FontInfo,
    output: [*c]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    oversample_x: c_int,
    oversample_y: c_int,
    sub_x: [*c]f32,
    sub_y: [*c]f32,
    codepoint: c_int,
) void;
extern fn stbtt_GetCodepointBitmapBox(
    info: [*c]const FontInfo,
    codepoint: c_int,
    scale_x: f32,
    scale_y: f32,
    ix0: [*c]c_int,
    iy0: [*c]c_int,
    ix1: [*c]c_int,
    iy1: [*c]c_int,
) void;
extern fn stbtt_GetCodepointBitmapBoxSubpixel(
    info: [*c]const FontInfo,
    codepoint: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    ix0: [*c]c_int,
    iy0: [*c]c_int,
    ix1: [*c]c_int,
    iy1: [*c]c_int,
) void;
extern fn stbtt_GetGlyphBitmap(
    info: [*c]const FontInfo,
    scale_x: f32,
    scale_y: f32,
    glyph: c_int,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;
extern fn stbtt_GetGlyphBitmapSubpixel(
    info: [*c]const FontInfo,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    glyph: c_int,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;
extern fn stbtt_MakeGlyphBitmap(
    info: [*c]const FontInfo,
    output: [*c]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    glyph: c_int,
) void;
extern fn stbtt_MakeGlyphBitmapSubpixel(
    info: [*c]const FontInfo,
    output: [*c]u8,
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
    info: [*c]const FontInfo,
    output: [*c]u8,
    out_w: c_int,
    out_h: c_int,
    out_stride: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    oversample_x: c_int,
    oversample_y: c_int,
    sub_x: [*c]f32,
    sub_y: [*c]f32,
    glyph: c_int,
) void;
extern fn stbtt_GetGlyphBitmapBox(
    info: [*c]const FontInfo,
    glyph: c_int,
    scale_x: f32,
    scale_y: f32,
    ix0: [*c]c_int,
    iy0: [*c]c_int,
    ix1: [*c]c_int,
    iy1: [*c]c_int,
) void;
extern fn stbtt_GetGlyphBitmapBoxSubpixel(
    info: [*c]const FontInfo,
    glyph: c_int,
    scale_x: f32,
    scale_y: f32,
    shift_x: f32,
    shift_y: f32,
    ix0: [*c]c_int,
    iy0: [*c]c_int,
    ix1: [*c]c_int,
    iy1: [*c]c_int,
) void;

extern fn stbtt_FreeSDF(bitmap: [*c]u8, userdata: ?[*c]anyopaque) void;
extern fn stbtt_GetGlyphSDF(
    info: [*c]const FontInfo,
    scale: f32,
    glyph: c_int,
    padding: c_int,
    onedge_value: c_uint,
    pixel_dist_scale: f32,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;
extern fn stbtt_GetCodepointSDF(
    info: [*c]const FontInfo,
    scale: f32,
    codepoint: c_int,
    padding: c_int,
    onedge_value: c_uint,
    pixel_dist_scale: f32,
    width: [*c]c_int,
    height: [*c]c_int,
    xoff: [*c]c_int,
    yoff: [*c]c_int,
) ?[*c]u8;

test {
    std.testing.refAllDecls(@This());

    init(std.testing.allocator);
    defer deinit();
}
