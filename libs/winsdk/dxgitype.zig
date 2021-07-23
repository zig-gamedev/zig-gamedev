const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgicommon.zig");

pub const DXGI_CPU_ACCESS = enum(UINT) {
    NONE = 0,
    DYNAMIC = 1,
    READ_WRITE = 2,
    SCRATCH = 3,
    FIELD = 15,
};

pub const DXGI_RGB = extern struct {
    Red: FLOAT,
    Green: FLOAT,
    Blue: FLOAT,
};

pub const D3DCOLORVALUE = extern struct {
    r: FLOAT,
    g: FLOAT,
    b: FLOAT,
    a: FLOAT,
};

pub const DXGI_RGBA = D3DCOLORVALUE;

pub const DXGI_GAMMA_CONTROL = extern struct {
    Scale: DXGI_RGB,
    Offset: DXGI_RGB,
    GammaCurve: [1025]DXGI_RGB,
};

pub const DXGI_GAMMA_CONTROL_CAPABILITIES = extern struct {
    ScaleAndOffsetSupported: BOOL,
    MaxConvertedValue: FLOAT,
    MinConvertedValue: FLOAT,
    NumGammaControlPoints: UINT,
    ControlPointPositions: [1025]FLOAT,
};

pub const DXGI_MODE_SCANLINE_ORDER = enum(UINT) {
    UNSPECIFIED = 0,
    PROGRESSIVE = 1,
    UPPER_FIELD_FIRST = 2,
    LOWER_FIELD_FIRST = 3,
};

pub const DXGI_MODE_SCALING = enum(UINT) {
    UNSPECIFIED = 0,
    CENTERED = 1,
    STRETCHED = 2,
};

pub const DXGI_MODE_ROTATION = enum(UINT) {
    UNSPECIFIED = 0,
    IDENTITY = 1,
    ROTATE90 = 2,
    ROTATE180 = 3,
    ROTATE270 = 4,
};

pub const DXGI_MODE_DESC = extern struct {
    Width: UINT,
    Height: UINT,
    RefreshRate: DXGI_RATIONAL,
    Format: DXGI_FORMAT,
    ScanlineOrdering: DXGI_MODE_SCANLINE_ORDER,
    Scaling: DXGI_MODE_SCALING,
};
