/// Simple Texture Baking API
/// NOTE: It's not recommended to ship with this but it's ok for tools or a quick start

// TODO: zigified wrapper

/// stbtt_bakedchar
const BakedChar = extern struct {
    x0: c_ushort,
    y0: c_ushort,
    x1: c_ushort,
    y1: c_ushort,
    xoff: f32,
    yoff: f32,
    xadvance: f32,
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

extern fn stbtt_BakeFontBitmap(
    data: [*]const u8,
    offset: c_int,
    pixel_height: f32,
    pixels: [*]u8,
    pw: c_int,
    ph: c_int,
    first_char: c_int,
    num_chars: c_int,
    chardata: [*]BakedChar,
) c_int;

extern fn stbtt_GetBakedQuad(
    chardata: *const BakedChar,
    pw: c_int,
    ph: c_int,
    char_index: c_int,
    xpos: *f32,
    ypos: *f32,
    q: *AlignedQuad,
    opengl_fillrule: c_int,
) void;
