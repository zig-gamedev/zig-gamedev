/// "New" texture baking API
/// This provides options for packing multiple fonts into one atlas, not
/// perfectly but better than nothing.

// TODO: zigified wrapper

/// stbrp_coord
pub const STBRPCoord = c_int;

/// stbrp_context
const STBRPContext = struct {
    width: c_int,
    height: c_int,
    x: c_int,
    y: c_int,
    bottom_y: c_int,
};

/// stbrp_rect
const STBRPRect = struct {
    x: STBRPCoord,
    y: STBRPCoord,
    id: c_int,
    w: c_int,
    h: c_int,
    was_packed: c_int,
};

/// stbtt_pack_context
const PackContext = extern struct {
    user_allocator_context: ?*anyopaque,
    pack_info: ?*anyopaque,
    width: c_int,
    height: c_int,
    stride_in_bytes: c_int,
    padding: c_int,
    skip_missing: c_int,
    h_oversample: c_int,
    v_oversample: c_int,
    pixels: [*]u8,
    nodes: ?*anyopaque,
};

/// stbtt_packedchar
const PackedChar = extern struct {
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

/// stbtt_pack_range
const PackRange = extern struct {
    font_size: f32,
    first_unicode_codepoint_in_range: c_int,
    array_of_unicode_codepoints: *c_int,
    num_chars: c_int,
    chardata_for_range: *PackedChar,
    h_oversample: u8,
    v_oversample: u8,
};

/// stbtt_aligned_quad
const AlignedQuad = extern struct {
    x0: f32,
    y0: f32,
    s0: f32,
    t0: f32,
    x1: f32,
    y1: f32,
    s1: f32,
    t1: f32,
};

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

extern fn stbtt_PackBegin(
    spc: *PackContext,
    pixels: [*]u8,
    width: c_int,
    height: c_int,
    stride_in_bytes: c_int,
    padding: c_int,
    alloc_context: ?*anyopaque,
) c_int;

extern fn stbtt_PackEnd(spc: *PackContext) void;

extern fn stbtt_PackFontRange(
    spc: *PackContext,
    fontdata: [*]const u8,
    font_index: c_int,
    font_size: f32,
    first_unicode_char_in_range: c_int,
    num_chars_in_range: c_int,
    chardata_for_range: *PackedChar,
) c_int;

extern fn stbtt_PackFontRanges(
    spc: *PackContext,
    fontdata: [*]const u8,
    font_index: c_int,
    ranges: [*]PackRange,
    num_ranges: c_int,
) c_int;

extern fn stbtt_PackSetOversampling(
    spc: *PackContext,
    h_oversample: c_uint,
    v_oversample: c_uint,
) void;

extern fn stbtt_PackSetSkipMissingCodepoints(spc: *PackContext, skip: c_int) void;

extern fn stbtt_GetPackedQuad(
    chardata: [*]const PackedChar,
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
    ranges: [*]PackRange,
    num_ranges: c_int,
    rects: [*]STBRPRect,
) c_int;

extern fn stbtt_PackFontRangesPackRects(
    spc: *PackContext,
    rects: [*]STBRPRect,
    num_rects: c_int,
) void;

extern fn stbtt_PackFontRangesRenderIntoRects(
    spc: *PackContext,
    info: *const FontInfo,
    ranges: [*]PackRange,
    num_ranges: c_int,
    rects: [*]STBRPRect,
) c_int;
