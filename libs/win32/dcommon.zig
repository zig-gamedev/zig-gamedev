const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dxgiformat.zig");

pub const D2D1_POINT_2F = D2D_POINT_2F;
pub const D2D1_POINT_2U = D2D_POINT_2U;
pub const D2D1_POINT_2L = D2D_POINT_2L;
pub const D2D1_RECT_2F = D2D_RECT_2F;
pub const D2D1_RECT_2U = D2D_RECT_2U;
pub const D2D1_RECT_2L = D2D_RECT_2L;
pub const D2D1_SIZE_2F = D2D_SIZE_2F;
pub const D2D1_SIZE_2U = D2D_SIZE_2U;
pub const D2D1_MATRIX_3X2_F = D2D_MATRIX_3X2_F;

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
