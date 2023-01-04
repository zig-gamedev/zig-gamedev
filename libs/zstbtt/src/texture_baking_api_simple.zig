/// Simple Texture Baking API
/// NOTE: It's not recommended to ship with this but it's ok for tools or a quick start
const std = @import("std");
const assert = std.debug.assert;

/////////////////////////////////////////////////////////////////////////////////////////
// ZIGIFIED WRAPPER
//

/// bake a font to a bitmap for use as texture
///
/// if return is positive, the first unused row of the bitmap
/// if return is negative, returns the negative of the number of characters that fit
/// if return is 0, no characters fit and no rows were used
///
/// This uses a very crappy packing.
pub fn bakeFontBitmap(args: struct {
    ttf_bytes: []const u8,
    offset: u32 = 0,
    pixel_height: f32,
    first_char: u32,
    num_chars: u32,
    bitmap_width: u32,
    bitmap_height: u32,
    out_bitmap: []u8,
    out_chardata: []BakedChar,
}) void {
    const res = stbtt_BakeFontBitmap(
        args.ttf_bytes.ptr,
        @intCast(c_int, args.offset),
        args.pixel_height,
        args.out_bitmap.ptr,
        @intCast(c_int, args.bitmap_width),
        @intCast(c_int, args.bitmap_height),
        @intCast(c_int, args.first_char),
        @intCast(c_int, args.num_chars),
        args.out_chardata.ptr,
    );
    // TODO: better verify result
    assert(res != 0);
}

/// compute quad to draw for a given char
///
/// Call GetBakedQuad with char_index = 'character - first_char', and it
/// creates the quad you need to draw and advances the current position.
///
/// The coordinate system used assumes y increases downwards.
///
/// Characters will extend both above and below the current position;
/// see discussion of "BASELINE" above.
///
/// It's inefficient; you might want to replace with an optimised implementation
pub fn getBakedQuad(args: struct {
    baked_char: *const BakedChar,
    bitmap_width: u32,
    bitmap_height: u32,
    char_index: u32,
    opengl_fillrule: bool = true,
    in_out_xpos: *f32,
    in_out_ypos: *f32,
}) AlignedQuad {
    var out_quad = std.mem.zeros(AlignedQuad);
    stbtt_GetBakedQuad(
        args.baked_char,
        @intCast(c_int, args.bitmap_width),
        @intCast(c_int, args.bitmap_height),
        @intCast(c_int, args.char_index),
        args.in_out_xpos,
        args.in_out_ypos,
        &out_quad,
        if (args.opengl_fillrule) 1 else 0,
    );
    return out_quad;
}

/////////////////////////////////////////////////////////////////////////////////////////
// C BINDINGS
//

/// stbtt_bakedchar
pub const BakedChar = extern struct {
    x0: c_ushort,
    y0: c_ushort,
    x1: c_ushort,
    y1: c_ushort,
    xoff: f32,
    yoff: f32,
    xadvance: f32,
};

/// stbtt_aligned_quad
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
