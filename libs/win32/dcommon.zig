const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dxgiformat.zig");

pub const D2D1_POINT_2F = D2D_POINT_2F;
pub const D2D1_POINT_2U = D2D_POINT_2U;
pub const D2D1_POINT_2L = D2D_POINT_2L;
pub const D2D1_RECT_F = D2D_RECT_F;
pub const D2D1_RECT_U = D2D_RECT_U;
pub const D2D1_RECT_L = D2D_RECT_L;
pub const D2D1_SIZE_F = D2D_SIZE_F;
pub const D2D1_SIZE_U = D2D_SIZE_U;
pub const D2D1_MATRIX_3X2_F = D2D_MATRIX_3X2_F;

pub const D2D1_COLOR_F = extern struct {
    r: FLOAT,
    g: FLOAT,
    b: FLOAT,
    a: FLOAT,

    pub const Black = D2D1_COLOR_F{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 };

    fn toSrgb(s: FLOAT) FLOAT {
        var l: FLOAT = undefined;
        if (s > 0.0031308) {
            l = 1.055 * (std.math.pow(FLOAT, s, (1.0 / 2.4))) - 0.055;
        } else {
            l = 12.92 * s;
        }
        return l;
    }

    pub fn linearToSrgb(r: FLOAT, g: FLOAT, b: FLOAT, a: FLOAT) D2D1_COLOR_F {
        return D2D1_COLOR_F{
            .r = toSrgb(r),
            .g = toSrgb(g),
            .b = toSrgb(b),
            .a = a,
        };
    }
};

pub const D2D1_ALPHA_MODE = enum(UINT) {
    UNKNOWN = 0,
    PREMULTIPLIED = 1,
    STRAIGHT = 2,
    IGNORE = 3,
};

pub const D2D1_PIXEL_FORMAT = extern struct {
    format: DXGI_FORMAT,
    alphaMode: D2D1_ALPHA_MODE,
};

pub const D2D_POINT_2U = extern struct {
    x: UINT32,
    y: UINT32,
};

pub const D2D_POINT_2F = extern struct {
    x: FLOAT,
    y: FLOAT,
};

pub const D2D_POINT_2L = POINT;

pub const D2D_VECTOR_2F = extern struct {
    x: FLOAT,
    y: FLOAT,
};

pub const D2D_VECTOR_3F = extern struct {
    x: FLOAT,
    y: FLOAT,
    z: FLOAT,
};

pub const D2D_VECTOR_4F = extern struct {
    x: FLOAT,
    y: FLOAT,
    z: FLOAT,
    w: FLOAT,
};

pub const D2D_RECT_F = extern struct {
    left: FLOAT,
    top: FLOAT,
    right: FLOAT,
    bottom: FLOAT,
};

pub const D2D_RECT_U = extern struct {
    left: UINT32,
    top: UINT32,
    right: UINT32,
    bottom: UINT32,
};

pub const D2D_RECT_L = RECT;

pub const D2D_SIZE_F = extern struct {
    width: FLOAT,
    height: FLOAT,
};

pub const D2D_SIZE_U = extern struct {
    width: UINT32,
    height: UINT32,
};

pub const D2D_MATRIX_3X2_F = extern struct {
    m: [3][2]FLOAT,

    pub fn initTranslation(x: FLOAT, y: FLOAT) D2D_MATRIX_3X2_F {
        return .{
            .m = [_][2]FLOAT{
                [2]FLOAT{ 1.0, 0.0 },
                [2]FLOAT{ 0.0, 1.0 },
                [2]FLOAT{ x, y },
            },
        };
    }

    pub fn initIdentity() D2D_MATRIX_3X2_F {
        return .{
            .m = [_][2]FLOAT{
                [2]FLOAT{ 1.0, 0.0 },
                [2]FLOAT{ 0.0, 1.0 },
                [2]FLOAT{ 0.0, 0.0 },
            },
        };
    }
};

pub const D2D_MATRIX_4X3_F = extern struct {
    m: [4][3]FLOAT,
};

pub const D2D_MATRIX_4X4_F = extern struct {
    m: [4][4]FLOAT,
};

pub const D2D_MATRIX_5X4_F = extern struct {
    m: [5][4]FLOAT,
};

pub const DWRITE_MEASURING_MODE = enum(UINT) {
    NATURAL = 0,
    GDI_CLASSIC = 1,
    GDI_NATURAL = 2,
};
