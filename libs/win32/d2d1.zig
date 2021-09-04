const std = @import("std");
const windows = @import("windows.zig");
const assert = std.debug.assert;
const dwrite = @import("dwrite.zig");
const dxgi = @import("dxgi.zig");
const UINT = windows.UINT;
const IUnknown = windows.IUnknown;
const HRESULT = windows.HRESULT;
const GUID = windows.GUID;
const WINAPI = windows.WINAPI;
const FLOAT = windows.FLOAT;
const LPCWSTR = windows.LPCWSTR;
const UINT32 = windows.UINT32;
const UINT64 = windows.UINT64;
const POINT = windows.POINT;
const RECT = windows.RECT;

pub const POINT_2F = D2D_POINT_2F;
pub const POINT_2U = D2D_POINT_2U;
pub const POINT_2L = D2D_POINT_2L;
pub const RECT_F = D2D_RECT_F;
pub const RECT_U = D2D_RECT_U;
pub const RECT_L = D2D_RECT_L;
pub const SIZE_F = D2D_SIZE_F;
pub const SIZE_U = D2D_SIZE_U;
pub const MATRIX_3X2_F = D2D_MATRIX_3X2_F;

pub const colorf = struct {
    pub const OliveDrab = COLOR_F{ .r = 0.419607878, .g = 0.556862772, .b = 0.137254909, .a = 1.0 };
    pub const Black = COLOR_F{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 };
    pub const White = COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 };
    pub const YellowGreen = COLOR_F{ .r = 0.603921592, .g = 0.803921640, .b = 0.196078449, .a = 1.0 };
    pub const Yellow = COLOR_F{ .r = 1.0, .g = 1.0, .b = 0.0, .a = 1.0 };
    pub const LightSkyBlue = COLOR_F{ .r = 0.529411793, .g = 0.807843208, .b = 0.980392218, .a = 1.000000000 };
    pub const DarkOrange = COLOR_F{ .r = 1.000000000, .g = 0.549019635, .b = 0.000000000, .a = 1.000000000 };
};

pub const COLOR_F = extern struct {
    r: FLOAT,
    g: FLOAT,
    b: FLOAT,
    a: FLOAT,

    pub const Black = COLOR_F{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 };

    fn toSrgb(s: FLOAT) FLOAT {
        var l: FLOAT = undefined;
        if (s > 0.0031308) {
            l = 1.055 * (std.math.pow(FLOAT, s, (1.0 / 2.4))) - 0.055;
        } else {
            l = 12.92 * s;
        }
        return l;
    }

    pub fn linearToSrgb(r: FLOAT, g: FLOAT, b: FLOAT, a: FLOAT) COLOR_F {
        return COLOR_F{
            .r = toSrgb(r),
            .g = toSrgb(g),
            .b = toSrgb(b),
            .a = a,
        };
    }
};

pub const ALPHA_MODE = enum(UINT) {
    UNKNOWN = 0,
    PREMULTIPLIED = 1,
    STRAIGHT = 2,
    IGNORE = 3,
};

pub const PIXEL_FORMAT = extern struct {
    format: dxgi.FORMAT,
    alphaMode: ALPHA_MODE,
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

pub const CAP_STYLE = enum(UINT) {
    FLAT = 0,
    SQUARE = 1,
    ROUND = 2,
    TRIANGLE = 3,
};

pub const DASH_STYLE = enum(UINT) {
    SOLID = 0,
    DASH = 1,
    DOT = 2,
    DASH_DOT = 3,
    DASH_DOT_DOT = 4,
    CUSTOM = 5,
};

pub const LINE_JOIN = enum(UINT) {
    MITER = 0,
    BEVEL = 1,
    ROUND = 2,
    MITER_OR_BEVEL = 3,
};

pub const STROKE_STYLE_PROPERTIES = extern struct {
    startCap: CAP_STYLE,
    endCap: CAP_STYLE,
    dashCap: CAP_STYLE,
    lineJoin: LINE_JOIN,
    miterLimit: FLOAT,
    dashStyle: DASH_STYLE,
    dashOffset: FLOAT,
};

pub const RADIAL_GRADIENT_BRUSH_PROPERTIES = extern struct {
    center: POINT_2F,
    gradientOriginOffset: POINT_2F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const IResource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetFactory: *c_void,
        };
    }
};

pub const IImage = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        image: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IBitmap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        image: IImage.VTable(Self),
        bitmap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IImage.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetSize: *c_void,
            GetPixelSize: *c_void,
            GetPixelFormat: *c_void,
            GetPixelDpi: *c_void,
            CopyFromBitmap: *c_void,
            CopyFromRenderTarget: *c_void,
            CopyFromMemory: *c_void,
        };
    }
};

pub const GAMMA = enum(UINT) {
    _2_2 = 0,
    _1_0 = 1,
};

pub const EXTEND_MODE = enum(UINT) {
    CLAMP = 0,
    WRAP = 1,
    MIRROR = 2,
};

pub const GRADIENT_STOP = extern struct {
    position: FLOAT,
    color: COLOR_F,
};

pub const IGradientStopCollection = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        gradsc: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetGradientStopCount: *c_void,
            GetGradientStops: *c_void,
            GetColorInterpolationGamma: *c_void,
            GetExtendMode: *c_void,
        };
    }
};

pub const IBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        brush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            SetOpacity: *c_void,
            SetTransform: *c_void,
            GetOpacity: *c_void,
            GetTransform: *c_void,
        };
    }
};

pub const IBitmapBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        brush: IBrush.VTable(Self),
        bmpbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IBrush.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            SetExtendModeX: *c_void,
            SetExtendModeY: *c_void,
            SetInterpolationMode: *c_void,
            SetBitmap: *c_void,
            GetExtendModeX: *c_void,
            GetExtendModeY: *c_void,
            GetInterpolationMode: *c_void,
            GetBitmap: *c_void,
        };
    }
};

pub const ISolidColorBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        brush: IBrush.VTable(Self),
        scbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IBrush.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetColor(self: *T, color: *const COLOR_F) void {
                self.v.scbrush.SetColor(self, color);
            }
            pub inline fn GetColor(self: *T) COLOR_F {
                var color: COLOR_F = undefined;
                _ = self.v.scbrush.GetColor(self, &color);
                return color;
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetColor: fn (*T, *const COLOR_F) callconv(WINAPI) void,
            GetColor: fn (*T, *COLOR_F) callconv(WINAPI) *COLOR_F,
        };
    }
};

pub const ILinearGradientBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        brush: IBrush.VTable(Self),
        lgbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IBrush.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            SetStartPoint: *c_void,
            SetEndPoint: *c_void,
            GetStartPoint: *c_void,
            GetEndPoint: *c_void,
            GetGradientStopCollection: *c_void,
        };
    }
};

pub const IRadialGradientBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        brush: IBrush.VTable(Self),
        rgbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IBrush.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            SetCenter: *c_void,
            SetGradientOriginOffset: *c_void,
            SetRadiusX: *c_void,
            SetRadiusY: *c_void,
            GetCenter: *c_void,
            GetGradientOriginOffset: *c_void,
            GetRadiusX: *c_void,
            GetRadiusY: *c_void,
            GetGradientStopCollection: *c_void,
        };
    }
};

pub const IStrokeStyle = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        strokestyle: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetStartCap: *c_void,
            GetEndCap: *c_void,
            GetDashCap: *c_void,
            GetMiterLimit: *c_void,
            GetLineJoin: *c_void,
            GetDashOffset: *c_void,
            GetDashStyle: *c_void,
            GetDashesCount: *c_void,
            GetDashes: *c_void,
        };
    }
};

pub const IGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetBounds: *c_void,
            GetWidenedBounds: *c_void,
            StrokeContainsPoint: *c_void,
            FillContainsPoint: *c_void,
            CompareWithGeometry: *c_void,
            Simplify: *c_void,
            Tessellate: *c_void,
            CombineWithGeometry: *c_void,
            Outline: *c_void,
            ComputeArea: *c_void,
            ComputeLength: *c_void,
            ComputePointAtLength: *c_void,
            Widen: *c_void,
        };
    }
};

pub const IRectangleGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        rectgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetRect: *c_void,
        };
    }
};

pub const IRoundedRectangleGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        roundedrectgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetRoundedRect: *c_void,
        };
    }
};

pub const IEllipseGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        ellipsegeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetEllipse: *c_void,
        };
    }
};

pub const IGeometryGroup = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        geogroup: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetFillMode: *c_void,
            GetSourceGeometryCount: *c_void,
            GetSourceGeometries: *c_void,
        };
    }
};

pub const ITransformedGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        transgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetSourceGeometry: *c_void,
            GetTransform: *c_void,
        };
    }
};

pub const FIGURE_BEGIN = enum(UINT) {
    FILLED = 0,
    HOLLOW = 1,
};

pub const FIGURE_END = enum(UINT) {
    OPEN = 0,
    CLOSED = 1,
};

pub const BEZIER_SEGMENT = extern struct {
    point1: POINT_2F,
    point2: POINT_2F,
    point3: POINT_2F,
};

pub const TRIANGLE = extern struct {
    point1: POINT_2F,
    point2: POINT_2F,
    point3: POINT_2F,
};

pub const PATH_SEGMENT = UINT;
pub const PATH_SEGMENT_NONE = 0x00000000;
pub const PATH_SEGMENT_FORCE_UNSTROKED = 0x00000001;
pub const PATH_SEGMENT_FORCE_ROUND_LINE_JOIN = 0x00000002;

pub const SWEEP_DIRECTION = enum(UINT) {
    COUNTER_CLOCKWISE = 0,
    CLOCKWISE = 1,
};

pub const FILL_MODE = enum(UINT) {
    ALTERNATE = 0,
    WINDING = 1,
};

pub const ARC_SIZE = enum(UINT) {
    SMALL = 0,
    LARGE = 1,
};

pub const ARC_SEGMENT = extern struct {
    point: POINT_2F,
    size: SIZE_F,
    rotationAngle: FLOAT,
    sweepDirection: SWEEP_DIRECTION,
    arcSize: ARC_SIZE,
};

pub const QUADRATIC_BEZIER_SEGMENT = extern struct {
    point1: POINT_2F,
    point2: POINT_2F,
};

pub const ISimplifiedGeometrySink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        simgeosink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetFillMode(self: *T, mode: FILL_MODE) void {
                self.v.simgeosink.SetFillMode(self, mode);
            }
            pub inline fn SetSegmentFlags(self: *T, flags: PATH_SEGMENT) void {
                self.v.simgeosink.SetSegmentFlags(self, flags);
            }
            pub inline fn BeginFigure(self: *T, point: POINT_2F, begin: FIGURE_BEGIN) void {
                self.v.simgeosink.BeginFigure(self, point, begin);
            }
            pub inline fn AddLines(self: *T, points: [*]const POINT_2F, count: UINT32) void {
                self.v.simgeosink.AddLines(self, points, count);
            }
            pub inline fn AddBeziers(self: *T, segments: [*]const BEZIER_SEGMENT, count: UINT32) void {
                self.v.simgeosink.AddBeziers(self, segments, count);
            }
            pub inline fn EndFigure(self: *T, end: FIGURE_END) void {
                self.v.simgeosink.EndFigure(self, end);
            }
            pub inline fn Close(self: *T) HRESULT {
                return self.v.simgeosink.Close(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetFillMode: fn (*T, FILL_MODE) callconv(WINAPI) void,
            SetSegmentFlags: fn (*T, PATH_SEGMENT) callconv(WINAPI) void,
            BeginFigure: fn (*T, POINT_2F, FIGURE_BEGIN) callconv(WINAPI) void,
            AddLines: fn (*T, [*]const POINT_2F, UINT32) callconv(WINAPI) void,
            AddBeziers: fn (*T, [*]const BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
            EndFigure: fn (*T, FIGURE_END) callconv(WINAPI) void,
            Close: fn (*T) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IGeometrySink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        simgeosink: ISimplifiedGeometrySink.VTable(Self),
        geosink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ISimplifiedGeometrySink.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AddLine(self: *T, point: POINT_2F) void {
                self.v.geosink.AddLine(self, point);
            }
            pub inline fn AddBezier(self: *T, segment: *const BEZIER_SEGMENT) void {
                self.v.geosink.AddBezier(self, segment);
            }
            pub inline fn AddQuadraticBezier(self: *T, segment: *const QUADRATIC_BEZIER_SEGMENT) void {
                self.v.geosink.AddQuadraticBezier(self, segment);
            }
            pub inline fn AddQuadraticBeziers(self: *T, segments: [*]const QUADRATIC_BEZIER_SEGMENT, count: UINT32) void {
                self.v.geosink.AddQuadraticBeziers(self, segments, count);
            }
            pub inline fn AddArc(self: *T, segment: *const ARC_SEGMENT) void {
                self.v.geosink.AddArc(self, segment);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            AddLine: fn (*T, POINT_2F) callconv(WINAPI) void,
            AddBezier: fn (*T, *const BEZIER_SEGMENT) callconv(WINAPI) void,
            AddQuadraticBezier: fn (*T, *const QUADRATIC_BEZIER_SEGMENT) callconv(WINAPI) void,
            AddQuadraticBeziers: fn (*T, [*]const QUADRATIC_BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
            AddArc: fn (*T, *const ARC_SEGMENT) callconv(WINAPI) void,
        };
    }
};

pub const ITessellationSink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        tesssink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            AddTriangles: *c_void,
            Close: *c_void,
        };
    }
};

pub const IPathGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        geometry: IGeometry.VTable(Self),
        pathgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IGeometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Open(self: *T, sink: *?*IGeometrySink) HRESULT {
                return self.v.pathgeo.Open(self, sink);
            }
            pub inline fn GetSegmentCount(self: *T, count: *UINT32) HRESULT {
                return self.v.pathgeo.GetSegmentCount(self, count);
            }
            pub inline fn GetFigureCount(self: *T, count: *UINT32) HRESULT {
                return self.v.pathgeo.GetFigureCount(self, count);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Open: fn (*T, *?*IGeometrySink) callconv(WINAPI) HRESULT,
            Stream: *c_void,
            GetSegmentCount: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
            GetFigureCount: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IMesh = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        mesh: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            Open: *c_void,
        };
    }
};

pub const ILayer = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        layer: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetSize: *c_void,
        };
    }
};

pub const IDrawingStateBlock = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        stateblock: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDescription: *c_void,
            SetDescription: *c_void,
            SetTextRenderingParams: *c_void,
            GetTextRenderingParams: *c_void,
        };
    }
};

pub const BRUSH_PROPERTIES = extern struct {
    opacity: FLOAT,
    transform: MATRIX_3X2_F,
};

pub const ELLIPSE = extern struct {
    point: POINT_2F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const ROUNDED_RECT = extern struct {
    rect: RECT_F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const BITMAP_INTERPOLATION_MODE = enum(UINT) {
    NEAREST_NEIGHBOR = 0,
    LINEAR = 1,
};

pub const DRAW_TEXT_OPTIONS = UINT;
pub const DRAW_TEXT_OPTIONS_NONE = 0;
pub const DRAW_TEXT_OPTIONS_NO_SNAP = 0x1;
pub const DRAW_TEXT_OPTIONS_CLIP = 0x2;
pub const DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT = 0x4;
pub const DRAW_TEXT_OPTIONS_DISABLE_COLOR_BITMAP_SNAPPING = 0x8;

pub const TAG = UINT64;

pub const IRenderTarget = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateSolidColorBrush(
                self: *T,
                color: *const COLOR_F,
                properties: ?*const BRUSH_PROPERTIES,
                brush: *?*ISolidColorBrush,
            ) HRESULT {
                return self.v.rendertarget.CreateSolidColorBrush(self, color, properties, brush);
            }
            pub inline fn CreateGradientStopCollection(
                self: *T,
                stops: [*]const GRADIENT_STOP,
                num_stops: UINT32,
                gamma: GAMMA,
                extend_mode: EXTEND_MODE,
                stop_collection: *?*IGradientStopCollection,
            ) HRESULT {
                return self.v.rendertarget.CreateGradientStopCollection(
                    self,
                    stops,
                    num_stops,
                    gamma,
                    extend_mode,
                    stop_collection,
                );
            }
            pub inline fn CreateRadialGradientBrush(
                self: *T,
                gradient_properties: *const RADIAL_GRADIENT_BRUSH_PROPERTIES,
                brush_properties: ?*const BRUSH_PROPERTIES,
                stop_collection: *IGradientStopCollection,
                brush: *?*IRadialGradientBrush,
            ) HRESULT {
                return self.v.rendertarget.CreateRadialGradientBrush(
                    self,
                    gradient_properties,
                    brush_properties,
                    stop_collection,
                    brush,
                );
            }
            pub inline fn DrawLine(
                self: *T,
                p0: POINT_2F,
                p1: POINT_2F,
                brush: *IBrush,
                width: FLOAT,
                style: ?*IStrokeStyle,
            ) void {
                self.v.rendertarget.DrawLine(self, p0, p1, brush, width, style);
            }
            pub inline fn DrawRectangle(
                self: *T,
                rect: *const RECT_F,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                self.v.rendertarget.DrawRectangle(self, rect, brush, width, stroke);
            }
            pub inline fn FillRectangle(self: *T, rect: *const RECT_F, brush: *IBrush) void {
                self.v.rendertarget.FillRectangle(self, rect, brush);
            }
            pub inline fn DrawRoundedRectangle(
                self: *T,
                rect: *const ROUNDED_RECT,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                self.v.rendertarget.DrawRoundedRectangle(self, rect, brush, width, stroke);
            }
            pub inline fn FillRoundedRectangle(self: *T, rect: *const ROUNDED_RECT, brush: *IBrush) void {
                self.v.rendertarget.FillRoundedRectangle(self, rect, brush);
            }
            pub inline fn DrawEllipse(
                self: *T,
                ellipse: *const ELLIPSE,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                self.v.rendertarget.DrawEllipse(self, ellipse, brush, width, stroke);
            }
            pub inline fn FillEllipse(self: *T, ellipse: *const ELLIPSE, brush: *IBrush) void {
                self.v.rendertarget.FillEllipse(self, ellipse, brush);
            }
            pub inline fn DrawGeometry(
                self: *T,
                geo: *IGeometry,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                self.v.rendertarget.DrawGeometry(self, geo, brush, width, stroke);
            }
            pub inline fn FillGeometry(self: *T, geo: *IGeometry, brush: *IBrush, opacity_brush: ?*IBrush) void {
                self.v.rendertarget.FillGeometry(self, geo, brush, opacity_brush);
            }
            pub inline fn DrawBitmap(
                self: *T,
                bitmap: *IBitmap,
                dst_rect: ?*const RECT_F,
                opacity: FLOAT,
                interpolation_mode: BITMAP_INTERPOLATION_MODE,
                src_rect: ?*const RECT_F,
            ) void {
                self.v.rendertarget.DrawBitmap(self, bitmap, dst_rect, opacity, interpolation_mode, src_rect);
            }
            pub inline fn DrawText(
                self: *T,
                string: LPCWSTR,
                length: UINT,
                format: *dwrite.ITextFormat,
                layout_rect: *const RECT_F,
                brush: *IBrush,
                options: DRAW_TEXT_OPTIONS,
                measuring_mode: dwrite.MEASURING_MODE,
            ) void {
                self.v.rendertarget.DrawText(
                    self,
                    string,
                    length,
                    format,
                    layout_rect,
                    brush,
                    options,
                    measuring_mode,
                );
            }
            pub inline fn SetTransform(self: *T, m: *const MATRIX_3X2_F) void {
                self.v.rendertarget.SetTransform(self, m);
            }
            pub inline fn Clear(self: *T, color: ?*const COLOR_F) void {
                self.v.rendertarget.Clear(self, color);
            }
            pub inline fn BeginDraw(self: *T) void {
                self.v.rendertarget.BeginDraw(self);
            }
            pub inline fn EndDraw(self: *T, tag1: ?*TAG, tag2: ?*TAG) HRESULT {
                return self.v.rendertarget.EndDraw(self, tag1, tag2);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateBitmap: *c_void,
            CreateBitmapFromWicBitmap: *c_void,
            CreateSharedBitmap: *c_void,
            CreateBitmapBrush: *c_void,
            CreateSolidColorBrush: fn (
                *T,
                *const COLOR_F,
                ?*const BRUSH_PROPERTIES,
                *?*ISolidColorBrush,
            ) callconv(WINAPI) HRESULT,
            CreateGradientStopCollection: fn (
                *T,
                [*]const GRADIENT_STOP,
                UINT32,
                GAMMA,
                EXTEND_MODE,
                *?*IGradientStopCollection,
            ) callconv(WINAPI) HRESULT,
            CreateLinearGradientBrush: *c_void,
            CreateRadialGradientBrush: fn (
                *T,
                *const RADIAL_GRADIENT_BRUSH_PROPERTIES,
                ?*const BRUSH_PROPERTIES,
                *IGradientStopCollection,
                *?*IRadialGradientBrush,
            ) callconv(WINAPI) HRESULT,
            CreateCompatibleRenderTarget: *c_void,
            CreateLayer: *c_void,
            CreateMesh: *c_void,
            DrawLine: fn (
                *T,
                POINT_2F,
                POINT_2F,
                *IBrush,
                FLOAT,
                ?*IStrokeStyle,
            ) callconv(WINAPI) void,
            DrawRectangle: fn (*T, *const RECT_F, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
            FillRectangle: fn (*T, *const RECT_F, *IBrush) callconv(WINAPI) void,
            DrawRoundedRectangle: fn (
                *T,
                *const ROUNDED_RECT,
                *IBrush,
                FLOAT,
                ?*IStrokeStyle,
            ) callconv(WINAPI) void,
            FillRoundedRectangle: fn (*T, *const ROUNDED_RECT, *IBrush) callconv(WINAPI) void,
            DrawEllipse: fn (*T, *const ELLIPSE, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
            FillEllipse: fn (*T, *const ELLIPSE, *IBrush) callconv(WINAPI) void,
            DrawGeometry: fn (*T, *IGeometry, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
            FillGeometry: fn (*T, *IGeometry, *IBrush, ?*IBrush) callconv(WINAPI) void,
            FillMesh: *c_void,
            FillOpacityMask: *c_void,
            DrawBitmap: fn (
                *T,
                *IBitmap,
                ?*const RECT_F,
                FLOAT,
                BITMAP_INTERPOLATION_MODE,
                ?*const RECT_F,
            ) callconv(WINAPI) void,
            DrawText: fn (
                *T,
                LPCWSTR,
                UINT,
                *dwrite.ITextFormat,
                *const RECT_F,
                *IBrush,
                DRAW_TEXT_OPTIONS,
                dwrite.MEASURING_MODE,
            ) callconv(WINAPI) void,
            DrawTextLayout: *c_void,
            DrawGlyphRun: *c_void,
            SetTransform: fn (*T, *const MATRIX_3X2_F) callconv(WINAPI) void,
            GetTransform: *c_void,
            SetAntialiasMode: *c_void,
            GetAntialiasMode: *c_void,
            SetTextAntialiasMode: *c_void,
            GetTextAntialiasMode: *c_void,
            SetTextRenderingParams: *c_void,
            GetTextRenderingParams: *c_void,
            SetTags: *c_void,
            GetTags: *c_void,
            PushLayer: *c_void,
            PopLayer: *c_void,
            Flush: *c_void,
            SaveDrawingState: *c_void,
            RestoreDrawingState: *c_void,
            PushAxisAlignedClip: *c_void,
            PopAxisAlignedClip: *c_void,
            Clear: fn (*T, ?*const COLOR_F) callconv(WINAPI) void,
            BeginDraw: fn (*T) callconv(WINAPI) void,
            EndDraw: fn (*T, ?*TAG, ?*TAG) callconv(WINAPI) HRESULT,
            GetPixelFormat: *c_void,
            SetDpi: *c_void,
            GetDpi: *c_void,
            GetSize: *c_void,
            GetPixelSize: *c_void,
            GetMaximumBitmapSize: *c_void,
            IsSupported: *c_void,
        };
    }
};

pub const IFactory = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateRectangleGeometry(self: *T, rect: *const RECT_F, geo: *?*IRectangleGeometry) HRESULT {
                return self.v.factory.CreateRectangleGeometry(self, rect, geo);
            }
            pub inline fn CreateRoundedRectangleGeometry(
                self: *T,
                rect: *const ROUNDED_RECT,
                geo: *?*IRoundedRectangleGeometry,
            ) HRESULT {
                return self.v.factory.CreateRoundedRectangleGeometry(self, rect, geo);
            }
            pub inline fn CreateEllipseGeometry(self: *T, ellipse: *const ELLIPSE, geo: *?*IEllipseGeometry) HRESULT {
                return self.v.factory.CreateEllipseGeometry(self, ellipse, geo);
            }
            pub inline fn CreatePathGeometry(self: *T, geo: *?*IPathGeometry) HRESULT {
                return self.v.factory.CreatePathGeometry(self, geo);
            }
            pub inline fn CreateStrokeStyle(
                self: *T,
                properties: *const STROKE_STYLE_PROPERTIES,
                dashes: ?[*]const FLOAT,
                dashes_count: UINT32,
                stroke_style: *?*IStrokeStyle,
            ) HRESULT {
                return self.v.factory.CreateStrokeStyle(self, properties, dashes, dashes_count, stroke_style);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            ReloadSystemMetrics: *c_void,
            GetDesktopDpi: *c_void,
            CreateRectangleGeometry: fn (*T, *const RECT_F, *?*IRectangleGeometry) callconv(WINAPI) HRESULT,
            CreateRoundedRectangleGeometry: fn (
                *T,
                *const ROUNDED_RECT,
                *?*IRoundedRectangleGeometry,
            ) callconv(WINAPI) HRESULT,
            CreateEllipseGeometry: fn (*T, *const ELLIPSE, *?*IEllipseGeometry) callconv(WINAPI) HRESULT,
            CreateGeometryGroup: *c_void,
            CreateTransformedGeometry: *c_void,
            CreatePathGeometry: fn (*T, *?*IPathGeometry) callconv(WINAPI) HRESULT,
            CreateStrokeStyle: fn (
                *T,
                *const STROKE_STYLE_PROPERTIES,
                ?[*]const FLOAT,
                UINT32,
                *?*IStrokeStyle,
            ) callconv(WINAPI) HRESULT,
            CreateDrawingStateBlock: *c_void,
            CreateWicBitmapRenderTarget: *c_void,
            CreateHwndRenderTarget: *c_void,
            CreateDxgiSurfaceRenderTarget: *c_void,
            CreateDCRenderTarget: *c_void,
        };
    }
};

pub const FACTORY_TYPE = enum(UINT) {
    SINGLE_THREADED = 0,
    MULTI_THREADED = 1,
};

pub const DEBUG_LEVEL = enum(UINT) {
    NONE = 0,
    ERROR = 1,
    WARNING = 2,
    INFORMATION = 3,
};

pub const FACTORY_OPTIONS = extern struct {
    debugLevel: DEBUG_LEVEL,
};

pub extern "d2d1" fn D2D1CreateFactory(
    FACTORY_TYPE,
    *const GUID,
    ?*const FACTORY_OPTIONS,
    *?*c_void,
) callconv(WINAPI) HRESULT;

pub const IBitmap1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        image: IImage.VTable(Self),
        bitmap: IBitmap.VTable(Self),
        bitmap1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IImage.Methods(Self);
    usingnamespace IBitmap.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetColorContext: *c_void,
            GetOptions: *c_void,
            GetSurface: *c_void,
            Map: *c_void,
            Unmap: *c_void,
        };
    }
};

pub const IColorContext = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        colorctx: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetColorSpace: *c_void,
            GetProfileSize: *c_void,
            GetProfile: *c_void,
        };
    }
};

pub const DEVICE_CONTEXT_OPTIONS = UINT;
pub const DEVICE_CONTEXT_OPTIONS_NONE = 0;
pub const DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS = 0x1;

pub const BITMAP_OPTIONS = UINT;
pub const BITMAP_OPTIONS_NONE = 0;
pub const BITMAP_OPTIONS_TARGET = 0x1;
pub const BITMAP_OPTIONS_CANNOT_DRAW = 0x2;
pub const BITMAP_OPTIONS_CPU_READ = 0x4;
pub const BITMAP_OPTIONS_GDI_COMPATIBLE = 0x8;

pub const BITMAP_PROPERTIES1 = extern struct {
    pixelFormat: PIXEL_FORMAT,
    dpiX: FLOAT,
    dpiY: FLOAT,
    bitmapOptions: BITMAP_OPTIONS,
    colorContext: ?*IColorContext,
};

pub const IDeviceContext = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateBitmapFromDxgiSurface(
                self: *T,
                surface: *dxgi.ISurface,
                properties: ?*const BITMAP_PROPERTIES1,
                bitmap: *?*IBitmap1,
            ) HRESULT {
                return self.v.devctx.CreateBitmapFromDxgiSurface(self, surface, properties, bitmap);
            }
            pub inline fn SetTarget(self: *T, image: ?*IImage) void {
                self.v.devctx.SetTarget(self, image);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateBitmap1: *c_void,
            CreateBitmapFromWicBitmap1: *c_void,
            CreateColorContext: *c_void,
            CreateColorContextFromFilename: *c_void,
            CreateColorContextFromWicColorContext: *c_void,
            CreateBitmapFromDxgiSurface: fn (
                *T,
                *dxgi.ISurface,
                ?*const BITMAP_PROPERTIES1,
                *?*IBitmap1,
            ) callconv(WINAPI) HRESULT,
            CreateEffect: *c_void,
            CreateGradientStopCollection1: *c_void,
            CreateImageBrush: *c_void,
            CreateBitmapBrush1: *c_void,
            CreateCommandList: *c_void,
            IsDxgiFormatSupported: *c_void,
            IsBufferPrecisionSupported: *c_void,
            GetImageLocalBounds: *c_void,
            GetImageWorldBounds: *c_void,
            GetGlyphRunWorldBounds: *c_void,
            GetDevice: *c_void,
            SetTarget: fn (*T, ?*IImage) callconv(WINAPI) void,
            GetTarget: *c_void,
            SetRenderingControls: *c_void,
            GetRenderingControls: *c_void,
            SetPrimitiveBlend: *c_void,
            GetPrimitiveBlend: *c_void,
            SetUnitMode: *c_void,
            GetUnitMode: *c_void,
            DrawGlyphRun1: *c_void,
            DrawImage: *c_void,
            DrawGdiMetafile: *c_void,
            DrawBitmap1: *c_void,
            PushLayer1: *c_void,
            InvalidateEffectInputRectangle: *c_void,
            GetEffectInvalidRectangleCount: *c_void,
            GetEffectInvalidRectangles: *c_void,
            GetEffectRequiredInputRectangles: *c_void,
            FillOpacityMask1: *c_void,
        };
    }
};

pub const IFactory1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice: *c_void,
            CreateStrokeStyle1: *c_void,
            CreatePathGeometry1: *c_void,
            CreateDrawingStateBlock1: *c_void,
            CreateGdiMetafile: *c_void,
            RegisterEffectFromStream: *c_void,
            RegisterEffectFromString: *c_void,
            UnregisterEffect: *c_void,
            GetRegisteredEffects: *c_void,
            GetEffectProperties: *c_void,
        };
    }
};

pub const IDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDeviceContext: *c_void,
            CreatePrintControl: *c_void,
            SetMaximumTextureMemory: *c_void,
            GetMaximumTextureMemory: *c_void,
            ClearResources: *c_void,
        };
    }
};

pub const IDeviceContext1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateFilledGeometryRealization: *c_void,
            CreateStrokedGeometryRealization: *c_void,
            DrawGeometryRealization: *c_void,
        };
    }
};

pub const IFactory2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice1: *c_void,
        };
    }
};

pub const IDevice1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetRenderingPriority: *c_void,
            SetRenderingPriority: *c_void,
            CreateDeviceContext1: *c_void,
        };
    }
};

pub const INK_NIB_SHAPE = enum(UINT) {
    ROUND = 0,
    SQUARE = 1,
};

pub const INK_POINT = extern struct {
    x: FLOAT,
    y: FLOAT,
    radius: FLOAT,
};

pub const INK_BEZIER_SEGMENT = extern struct {
    point1: INK_POINT,
    point2: INK_POINT,
    point3: INK_POINT,
};

pub const INK_STYLE_PROPERTIES = extern struct {
    nibShape: INK_NIB_SHAPE,
    nibTransform: MATRIX_3X2_F,
};

pub const IInk = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        ink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetStartPoint(self: *T, point: *const INK_POINT) void {
                self.v.ink.SetStartPoint(self, point);
            }
            pub inline fn GetStartPoint(self: *T) INK_POINT {
                var point: INK_POINT = undefined;
                _ = self.v.ink.GetStartPoint(self, &point);
                return point;
            }
            pub inline fn AddSegments(self: *T, segments: [*]const INK_BEZIER_SEGMENT, count: UINT32) HRESULT {
                return self.v.ink.AddSegments(self, segments, count);
            }
            pub inline fn RemoveSegmentsAtEnd(self: *T, count: UINT32) HRESULT {
                return self.v.ink.RemoveSegmentsAtEnd(self, count);
            }
            pub inline fn SetSegments(
                self: *T,
                start_segment: UINT32,
                segments: [*]const INK_BEZIER_SEGMENT,
                count: UINT32,
            ) HRESULT {
                return self.v.ink.SetSegments(self, start_segment, segments, count);
            }
            pub inline fn SetSegmentAtEnd(self: *T, segment: *const INK_BEZIER_SEGMENT) HRESULT {
                return self.v.ink.SetSegmentAtEnd(self, segment);
            }
            pub inline fn GetSegmentCount(self: *T) UINT32 {
                return self.v.ink.GetSegmentCount(self);
            }
            pub inline fn GetSegments(
                self: *T,
                start_segment: UINT32,
                segments: [*]const INK_BEZIER_SEGMENT,
                count: UINT32,
            ) HRESULT {
                return self.v.ink.GetSegments(self, start_segment, segments, count);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetStartPoint: fn (*T, *const INK_POINT) callconv(WINAPI) void,
            GetStartPoint: fn (*T, *INK_POINT) callconv(WINAPI) *INK_POINT,
            AddSegments: fn (*T, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
            RemoveSegmentsAtEnd: fn (*T, UINT32) callconv(WINAPI) HRESULT,
            SetSegments: fn (*T, UINT32, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
            SetSegmentAtEnd: fn (*T, *const INK_BEZIER_SEGMENT) callconv(WINAPI) HRESULT,
            GetSegmentCount: fn (*T) callconv(WINAPI) UINT32,
            GetSegments: fn (*T, UINT32, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
            StreamAsGeometry: *c_void,
            GetBounds: *c_void,
        };
    }
};

pub const IInkStyle = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        inkstyle: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            SetNibTransform: *c_void,
            GetNibTransform: *c_void,
            SetNibShape: *c_void,
            GetNibShape: *c_void,
        };
    }
};

pub const IDeviceContext2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: IDeviceContext1.VTable(Self),
        devctx2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace IDeviceContext1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateInk(self: *T, start_point: *const INK_POINT, ink: *?*IInk) HRESULT {
                return self.v.devctx2.CreateInk(self, start_point, ink);
            }
            pub inline fn CreateInkStyle(
                self: *T,
                properties: ?*const INK_STYLE_PROPERTIES,
                ink_style: *?*IInkStyle,
            ) HRESULT {
                return self.v.devctx2.CreateInkStyle(self, properties, ink_style);
            }
            pub inline fn DrawInk(self: *T, ink: *IInk, brush: *IBrush, style: ?*IInkStyle) void {
                return self.v.devctx2.DrawInk(self, ink, brush, style);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateInk: fn (*T, *const INK_POINT, *?*IInk) callconv(WINAPI) HRESULT,
            CreateInkStyle: fn (*T, ?*const INK_STYLE_PROPERTIES, *?*IInkStyle) callconv(WINAPI) HRESULT,
            CreateGradientMesh: *c_void,
            CreateImageSourceFromWic: *c_void,
            CreateLookupTable3D: *c_void,
            CreateImageSourceFromDxgi: *c_void,
            GetGradientMeshWorldBounds: *c_void,
            DrawInk: fn (*T, *IInk, *IBrush, ?*IInkStyle) callconv(WINAPI) void,
            DrawGradientMesh: *c_void,
            DrawGdiMetafile1: *c_void,
            CreateTransformedImageSource: *c_void,
        };
    }
};

pub const IDeviceContext3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: IDeviceContext1.VTable(Self),
        devctx2: IDeviceContext2.VTable(Self),
        devctx3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace IDeviceContext1.Methods(Self);
    usingnamespace IDeviceContext2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateSpriteBatch: *c_void,
            DrawSpriteBatch: *c_void,
        };
    }
};

pub const IDeviceContext4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: IDeviceContext1.VTable(Self),
        devctx2: IDeviceContext2.VTable(Self),
        devctx3: IDeviceContext3.VTable(Self),
        devctx4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace IDeviceContext1.Methods(Self);
    usingnamespace IDeviceContext2.Methods(Self);
    usingnamespace IDeviceContext3.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateSvgGlyphStyle: *c_void,
            DrawText1: *c_void,
            DrawTextLayout1: *c_void,
            DrawColorBitmapGlyphRun: *c_void,
            DrawSvgGlyphRun: *c_void,
            GetColorBitmapGlyphImage: *c_void,
            GetSvgGlyphImage: *c_void,
        };
    }
};

pub const IDeviceContext5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: IDeviceContext1.VTable(Self),
        devctx2: IDeviceContext2.VTable(Self),
        devctx3: IDeviceContext3.VTable(Self),
        devctx4: IDeviceContext4.VTable(Self),
        devctx5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace IDeviceContext1.Methods(Self);
    usingnamespace IDeviceContext2.Methods(Self);
    usingnamespace IDeviceContext3.Methods(Self);
    usingnamespace IDeviceContext4.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateSvgDocument: *c_void,
            DrawSvgDocument: *c_void,
            CreateColorContextFromDxgiColorSpace: *c_void,
            CreateColorContextFromSimpleColorProfile: *c_void,
        };
    }
};

pub const IDeviceContext6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        rendertarget: IRenderTarget.VTable(Self),
        devctx: IDeviceContext.VTable(Self),
        devctx1: IDeviceContext1.VTable(Self),
        devctx2: IDeviceContext2.VTable(Self),
        devctx3: IDeviceContext3.VTable(Self),
        devctx4: IDeviceContext4.VTable(Self),
        devctx5: IDeviceContext5.VTable(Self),
        devctx6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IRenderTarget.Methods(Self);
    usingnamespace IDeviceContext.Methods(Self);
    usingnamespace IDeviceContext1.Methods(Self);
    usingnamespace IDeviceContext2.Methods(Self);
    usingnamespace IDeviceContext3.Methods(Self);
    usingnamespace IDeviceContext4.Methods(Self);
    usingnamespace IDeviceContext5.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            BlendImage: *c_void,
        };
    }
};

pub const IFactory3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice2: *c_void,
        };
    }
};

pub const IFactory4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice3: *c_void,
        };
    }
};

pub const IFactory5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: IFactory4.VTable(Self),
        factory5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace IFactory4.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice4: *c_void,
        };
    }
};

pub const IFactory6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: IFactory4.VTable(Self),
        factory5: IFactory5.VTable(Self),
        factory6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace IFactory4.Methods(Self);
    usingnamespace IFactory5.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDevice5: *c_void,
        };
    }
};

pub const IFactory7 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: IFactory4.VTable(Self),
        factory5: IFactory5.VTable(Self),
        factory6: IFactory6.VTable(Self),
        factory7: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace IFactory4.Methods(Self);
    usingnamespace IFactory5.Methods(Self);
    usingnamespace IFactory6.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateDevice6(self: *T, dxgi_device: *dxgi.IDevice, d2d_device6: *?*IDevice6) HRESULT {
                return self.v.factory7.CreateDevice6(self, dxgi_device, d2d_device6);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateDevice6: fn (*T, *dxgi.IDevice, *?*IDevice6) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDevice2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDeviceContext2: *c_void,
            FlushDeviceContexts: *c_void,
            GetDxgiDevice: *c_void,
        };
    }
};

pub const IDevice3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDeviceContext3: *c_void,
        };
    }
};

pub const IDevice4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDeviceContext4: *c_void,
            SetMaximumColorGlyphCacheMemory: *c_void,
            GetMaximumColorGlyphCacheMemory: *c_void,
        };
    }
};

pub const IDevice5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            CreateDeviceContext5: *c_void,
        };
    }
};

pub const IDevice6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: IResource.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: IDevice5.VTable(Self),
        device6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace IDevice5.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateDeviceContext6(
                self: *T,
                options: DEVICE_CONTEXT_OPTIONS,
                devctx: *?*IDeviceContext6,
            ) HRESULT {
                return self.v.device6.CreateDeviceContext6(self, options, devctx);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateDeviceContext6: fn (
                *T,
                DEVICE_CONTEXT_OPTIONS,
                *?*IDeviceContext6,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IFactory7 = GUID{
    .Data1 = 0xbdc2bdd3,
    .Data2 = 0xb96c,
    .Data3 = 0x4de6,
    .Data4 = .{ 0xbd, 0xf7, 0x99, 0xd4, 0x74, 0x54, 0x54, 0xde },
};
