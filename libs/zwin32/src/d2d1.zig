const std = @import("std");
const assert = std.debug.assert;
const w32 = @import("w32.zig");
const UINT = w32.UINT;
const IUnknown = w32.IUnknown;
const HRESULT = w32.HRESULT;
const GUID = w32.GUID;
const WINAPI = w32.WINAPI;
const FLOAT = w32.FLOAT;
const LPCWSTR = w32.LPCWSTR;
const UINT32 = w32.UINT32;
const UINT64 = w32.UINT64;
const POINT = w32.POINT;
const RECT = w32.RECT;
const dwrite = @import("dwrite.zig");
const dxgi = @import("dxgi.zig");

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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IUnknown.VTable,
        GetFactory: *anyopaque,
    };
};

pub const IImage = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
    };
};

pub const IBitmap = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IImage.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IImage.VTable,
        GetSize: *anyopaque,
        GetPixelSize: *anyopaque,
        GetPixelFormat: *anyopaque,
        GetPixelDpi: *anyopaque,
        CopyFromBitmap: *anyopaque,
        CopyFromRenderTarget: *anyopaque,
        CopyFromMemory: *anyopaque,
    };
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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetGradientStopCount: *anyopaque,
        GetGradientStops: *anyopaque,
        GetColorInterpolationGamma: *anyopaque,
        GetExtendMode: *anyopaque,
    };
};

pub const IBrush = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        SetOpacity: *anyopaque,
        SetTransform: *anyopaque,
        GetOpacity: *anyopaque,
        GetTransform: *anyopaque,
    };
};

pub const ISolidColorBrush = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IBrush.Methods(T);

            pub inline fn SetColor(self: *T, color: *const COLOR_F) void {
                @ptrCast(*const ISolidColorBrush.VTable, self.__v)
                    .SetColor(@ptrCast(*ISolidColorBrush, self), color);
            }
            pub inline fn GetColor(self: *T) COLOR_F {
                var color: COLOR_F = undefined;
                _ = @ptrCast(*const ISolidColorBrush.VTable, self.__v)
                    .GetColor(@ptrCast(*ISolidColorBrush, self), &color);
                return color;
            }
        };
    }

    pub const VTable = extern struct {
        base: IBrush.VTable,
        SetColor: *const fn (*ISolidColorBrush, *const COLOR_F) callconv(WINAPI) void,
        GetColor: *const fn (*ISolidColorBrush, *COLOR_F) callconv(WINAPI) *COLOR_F,
    };
};

pub const IRadialGradientBrush = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IBrush.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IBrush.VTable,
        SetCenter: *anyopaque,
        SetGradientOriginOffset: *anyopaque,
        SetRadiusX: *anyopaque,
        SetRadiusY: *anyopaque,
        GetCenter: *anyopaque,
        GetGradientOriginOffset: *anyopaque,
        GetRadiusX: *anyopaque,
        GetRadiusY: *anyopaque,
        GetGradientStopCollection: *anyopaque,
    };
};

pub const IStrokeStyle = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetStartCap: *anyopaque,
        GetEndCap: *anyopaque,
        GetDashCap: *anyopaque,
        GetMiterLimit: *anyopaque,
        GetLineJoin: *anyopaque,
        GetDashOffset: *anyopaque,
        GetDashStyle: *anyopaque,
        GetDashesCount: *anyopaque,
        GetDashes: *anyopaque,
    };
};

pub const IGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetBounds: *anyopaque,
        GetWidenedBounds: *anyopaque,
        StrokeContainsPoint: *anyopaque,
        FillContainsPoint: *anyopaque,
        CompareWithGeometry: *anyopaque,
        Simplify: *anyopaque,
        Tessellate: *anyopaque,
        CombineWithGeometry: *anyopaque,
        Outline: *anyopaque,
        ComputeArea: *anyopaque,
        ComputeLength: *anyopaque,
        ComputePointAtLength: *anyopaque,
        Widen: *anyopaque,
    };
};

pub const IRectangleGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IGeometry.VTable,
        GetRect: *anyopaque,
    };
};

pub const IRoundedRectangleGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IGeometry.VTable,
        GetRoundedRect: *anyopaque,
    };
};

pub const IEllipseGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IGeometry.VTable,
        GetEllipse: *anyopaque,
    };
};

pub const IGeometryGroup = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IGeometry.VTable,
        GetFillMode: *anyopaque,
        GetSourceGeometryCount: *anyopaque,
        GetSourceGeometries: *anyopaque,
    };
};

pub const ITransformedGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IGeometry.VTable,
        GetSourceGeometry: *anyopaque,
        GetTransform: *anyopaque,
    };
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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn SetFillMode(self: *T, mode: FILL_MODE) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .SetFillMode(@ptrCast(*ISimplifiedGeometrySink, self), mode);
            }
            pub inline fn SetSegmentFlags(self: *T, flags: PATH_SEGMENT) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .SetSegmentFlags(@ptrCast(*ISimplifiedGeometrySink, self), flags);
            }
            pub inline fn BeginFigure(self: *T, point: POINT_2F, begin: FIGURE_BEGIN) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .BeginFigure(@ptrCast(*ISimplifiedGeometrySink, self), point, begin);
            }
            pub inline fn AddLines(self: *T, points: [*]const POINT_2F, count: UINT32) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .AddLines(@ptrCast(*ISimplifiedGeometrySink, self), points, count);
            }
            pub inline fn AddBeziers(self: *T, segments: [*]const BEZIER_SEGMENT, count: UINT32) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .AddBeziers(@ptrCast(*ISimplifiedGeometrySink, self), segments, count);
            }
            pub inline fn EndFigure(self: *T, end: FIGURE_END) void {
                @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .EndFigure(@ptrCast(*ISimplifiedGeometrySink, self), end);
            }
            pub inline fn Close(self: *T) HRESULT {
                return @ptrCast(*const ISimplifiedGeometrySink.VTable, self.__v)
                    .Close(@ptrCast(*ISimplifiedGeometrySink, self));
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISimplifiedGeometrySink;
        base: IUnknown.VTable,
        SetFillMode: *const fn (*T, FILL_MODE) callconv(WINAPI) void,
        SetSegmentFlags: *const fn (*T, PATH_SEGMENT) callconv(WINAPI) void,
        BeginFigure: *const fn (*T, POINT_2F, FIGURE_BEGIN) callconv(WINAPI) void,
        AddLines: *const fn (*T, [*]const POINT_2F, UINT32) callconv(WINAPI) void,
        AddBeziers: *const fn (*T, [*]const BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
        EndFigure: *const fn (*T, FIGURE_END) callconv(WINAPI) void,
        Close: *const fn (*T) callconv(WINAPI) HRESULT,
    };
};

pub const IGeometrySink = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace ISimplifiedGeometrySink.Methods(T);

            pub inline fn AddLine(self: *T, point: POINT_2F) void {
                @ptrCast(*const IGeometrySink.VTable, self.__v).AddLine(@ptrCast(*IGeometrySink, self), point);
            }
            pub inline fn AddBezier(self: *T, segment: *const BEZIER_SEGMENT) void {
                @ptrCast(*const IGeometrySink.VTable, self.__v).AddBezier(@ptrCast(*IGeometrySink, self), segment);
            }
            pub inline fn AddQuadraticBezier(self: *T, segment: *const QUADRATIC_BEZIER_SEGMENT) void {
                @ptrCast(*const IGeometrySink.VTable, self.__v)
                    .AddQuadraticBezier(@ptrCast(*IGeometrySink, self), segment);
            }
            pub inline fn AddQuadraticBeziers(
                self: *T,
                segments: [*]const QUADRATIC_BEZIER_SEGMENT,
                count: UINT32,
            ) void {
                @ptrCast(*const IGeometrySink.VTable, self.__v)
                    .AddQuadraticBeziers(@ptrCast(*IGeometrySink, self), segments, count);
            }
            pub inline fn AddArc(self: *T, segment: *const ARC_SEGMENT) void {
                @ptrCast(*const IGeometrySink.VTable, self.__v).AddArc(@ptrCast(*IGeometrySink, self), segment);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IGeometrySink;
        base: ISimplifiedGeometrySink.VTable,
        AddLine: *const fn (*T, POINT_2F) callconv(WINAPI) void,
        AddBezier: *const fn (*T, *const BEZIER_SEGMENT) callconv(WINAPI) void,
        AddQuadraticBezier: *const fn (*T, *const QUADRATIC_BEZIER_SEGMENT) callconv(WINAPI) void,
        AddQuadraticBeziers: *const fn (*T, [*]const QUADRATIC_BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
        AddArc: *const fn (*T, *const ARC_SEGMENT) callconv(WINAPI) void,
    };
};

pub const IPathGeometry = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IGeometry.Methods(T);

            pub inline fn Open(self: *T, sink: *?*IGeometrySink) HRESULT {
                return @ptrCast(*const IPathGeometry.VTable, self.__v).Open(@ptrCast(*IPathGeometry, self), sink);
            }
            pub inline fn GetSegmentCount(self: *T, count: *UINT32) HRESULT {
                return @ptrCast(*const IPathGeometry.VTable, self.__v)
                    .GetSegmentCount(@ptrCast(*IPathGeometry, self), count);
            }
            pub inline fn GetFigureCount(self: *T, count: *UINT32) HRESULT {
                return @ptrCast(*const IPathGeometry.VTable, self.__v)
                    .GetFigureCount(@ptrCast(*IPathGeometry, self), count);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IPathGeometry;
        base: IGeometry.VTable,
        Open: *const fn (*T, *?*IGeometrySink) callconv(WINAPI) HRESULT,
        Stream: *anyopaque,
        GetSegmentCount: *const fn (*T, *UINT32) callconv(WINAPI) HRESULT,
        GetFigureCount: *const fn (*T, *UINT32) callconv(WINAPI) HRESULT,
    };
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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);

            pub inline fn CreateSolidColorBrush(
                self: *T,
                color: *const COLOR_F,
                properties: ?*const BRUSH_PROPERTIES,
                brush: *?*ISolidColorBrush,
            ) HRESULT {
                return @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .CreateSolidColorBrush(@ptrCast(*IRenderTarget, self), color, properties, brush);
            }
            pub inline fn CreateGradientStopCollection(
                self: *T,
                stops: [*]const GRADIENT_STOP,
                num_stops: UINT32,
                gamma: GAMMA,
                extend_mode: EXTEND_MODE,
                stop_collection: *?*IGradientStopCollection,
            ) HRESULT {
                return @ptrCast(*const IRenderTarget.VTable, self.__v).CreateGradientStopCollection(
                    @ptrCast(*IRenderTarget, self),
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
                return @ptrCast(*const IRenderTarget.VTable, self.__v).CreateRadialGradientBrush(
                    @ptrCast(*IRenderTarget, self),
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
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .DrawLine(@ptrCast(*IRenderTarget, self), p0, p1, brush, width, style);
            }
            pub inline fn DrawRectangle(
                self: *T,
                rect: *const RECT_F,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .DrawRectangle(@ptrCast(*IRenderTarget, self), rect, brush, width, stroke);
            }
            pub inline fn FillRectangle(self: *T, rect: *const RECT_F, brush: *IBrush) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .FillRectangle(@ptrCast(*IRenderTarget, self), rect, brush);
            }
            pub inline fn DrawRoundedRectangle(
                self: *T,
                rect: *const ROUNDED_RECT,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .DrawRoundedRectangle(@ptrCast(*IRenderTarget, self), rect, brush, width, stroke);
            }
            pub inline fn FillRoundedRectangle(self: *T, rect: *const ROUNDED_RECT, brush: *IBrush) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .FillRoundedRectangle(@ptrCast(*IRenderTarget, self), rect, brush);
            }
            pub inline fn DrawEllipse(
                self: *T,
                ellipse: *const ELLIPSE,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .DrawEllipse(@ptrCast(*IRenderTarget, self), ellipse, brush, width, stroke);
            }
            pub inline fn FillEllipse(self: *T, ellipse: *const ELLIPSE, brush: *IBrush) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .FillEllipse(@ptrCast(*IRenderTarget, self), ellipse, brush);
            }
            pub inline fn DrawGeometry(
                self: *T,
                geo: *IGeometry,
                brush: *IBrush,
                width: FLOAT,
                stroke: ?*IStrokeStyle,
            ) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .DrawGeometry(@ptrCast(*IRenderTarget, self), geo, brush, width, stroke);
            }
            pub inline fn FillGeometry(self: *T, geo: *IGeometry, brush: *IBrush, opacity_brush: ?*IBrush) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .FillGeometry(@ptrCast(*IRenderTarget, self), geo, brush, opacity_brush);
            }
            pub inline fn DrawBitmap(
                self: *T,
                bitmap: *IBitmap,
                dst_rect: ?*const RECT_F,
                opacity: FLOAT,
                interpolation_mode: BITMAP_INTERPOLATION_MODE,
                src_rect: ?*const RECT_F,
            ) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v).DrawBitmap(
                    @ptrCast(*IRenderTarget, self),
                    bitmap,
                    dst_rect,
                    opacity,
                    interpolation_mode,
                    src_rect,
                );
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
                @ptrCast(*const IRenderTarget.VTable, self.__v).DrawText(
                    @ptrCast(*IRenderTarget, self),
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
                @ptrCast(*const IRenderTarget.VTable, self.__v).SetTransform(@ptrCast(*IRenderTarget, self), m);
            }
            pub inline fn Clear(self: *T, color: ?*const COLOR_F) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v).Clear(@ptrCast(*IRenderTarget, self), color);
            }
            pub inline fn BeginDraw(self: *T) void {
                @ptrCast(*const IRenderTarget.VTable, self.__v).BeginDraw(@ptrCast(*IRenderTarget, self));
            }
            pub inline fn EndDraw(self: *T, tag1: ?*TAG, tag2: ?*TAG) HRESULT {
                return @ptrCast(*const IRenderTarget.VTable, self.__v)
                    .EndDraw(@ptrCast(*IRenderTarget, self), tag1, tag2);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IRenderTarget;
        base: IResource.VTable,
        CreateBitmap: *anyopaque,
        CreateBitmapFromWicBitmap: *anyopaque,
        CreateSharedBitmap: *anyopaque,
        CreateBitmapBrush: *anyopaque,
        CreateSolidColorBrush: *const fn (
            *T,
            *const COLOR_F,
            ?*const BRUSH_PROPERTIES,
            *?*ISolidColorBrush,
        ) callconv(WINAPI) HRESULT,
        CreateGradientStopCollection: *const fn (
            *T,
            [*]const GRADIENT_STOP,
            UINT32,
            GAMMA,
            EXTEND_MODE,
            *?*IGradientStopCollection,
        ) callconv(WINAPI) HRESULT,
        CreateLinearGradientBrush: *anyopaque,
        CreateRadialGradientBrush: *const fn (
            *T,
            *const RADIAL_GRADIENT_BRUSH_PROPERTIES,
            ?*const BRUSH_PROPERTIES,
            *IGradientStopCollection,
            *?*IRadialGradientBrush,
        ) callconv(WINAPI) HRESULT,
        CreateCompatibleRenderTarget: *anyopaque,
        CreateLayer: *anyopaque,
        CreateMesh: *anyopaque,
        DrawLine: *const fn (
            *T,
            POINT_2F,
            POINT_2F,
            *IBrush,
            FLOAT,
            ?*IStrokeStyle,
        ) callconv(WINAPI) void,
        DrawRectangle: *const fn (*T, *const RECT_F, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
        FillRectangle: *const fn (*T, *const RECT_F, *IBrush) callconv(WINAPI) void,
        DrawRoundedRectangle: *const fn (
            *T,
            *const ROUNDED_RECT,
            *IBrush,
            FLOAT,
            ?*IStrokeStyle,
        ) callconv(WINAPI) void,
        FillRoundedRectangle: *const fn (*T, *const ROUNDED_RECT, *IBrush) callconv(WINAPI) void,
        DrawEllipse: *const fn (*T, *const ELLIPSE, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
        FillEllipse: *const fn (*T, *const ELLIPSE, *IBrush) callconv(WINAPI) void,
        DrawGeometry: *const fn (*T, *IGeometry, *IBrush, FLOAT, ?*IStrokeStyle) callconv(WINAPI) void,
        FillGeometry: *const fn (*T, *IGeometry, *IBrush, ?*IBrush) callconv(WINAPI) void,
        FillMesh: *anyopaque,
        FillOpacityMask: *anyopaque,
        DrawBitmap: *const fn (
            *T,
            *IBitmap,
            ?*const RECT_F,
            FLOAT,
            BITMAP_INTERPOLATION_MODE,
            ?*const RECT_F,
        ) callconv(WINAPI) void,
        DrawText: *const fn (
            *T,
            LPCWSTR,
            UINT,
            *dwrite.ITextFormat,
            *const RECT_F,
            *IBrush,
            DRAW_TEXT_OPTIONS,
            dwrite.MEASURING_MODE,
        ) callconv(WINAPI) void,
        DrawTextLayout: *anyopaque,
        DrawGlyphRun: *anyopaque,
        SetTransform: *const fn (*T, *const MATRIX_3X2_F) callconv(WINAPI) void,
        GetTransform: *anyopaque,
        SetAntialiasMode: *anyopaque,
        GetAntialiasMode: *anyopaque,
        SetTextAntialiasMode: *anyopaque,
        GetTextAntialiasMode: *anyopaque,
        SetTextRenderingParams: *anyopaque,
        GetTextRenderingParams: *anyopaque,
        SetTags: *anyopaque,
        GetTags: *anyopaque,
        PushLayer: *anyopaque,
        PopLayer: *anyopaque,
        Flush: *anyopaque,
        SaveDrawingState: *anyopaque,
        RestoreDrawingState: *anyopaque,
        PushAxisAlignedClip: *anyopaque,
        PopAxisAlignedClip: *anyopaque,
        Clear: *const fn (*T, ?*const COLOR_F) callconv(WINAPI) void,
        BeginDraw: *const fn (*T) callconv(WINAPI) void,
        EndDraw: *const fn (*T, ?*TAG, ?*TAG) callconv(WINAPI) HRESULT,
        GetPixelFormat: *anyopaque,
        SetDpi: *anyopaque,
        GetDpi: *anyopaque,
        GetSize: *anyopaque,
        GetPixelSize: *anyopaque,
        GetMaximumBitmapSize: *anyopaque,
        IsSupported: *anyopaque,
    };
};

pub const IFactory = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn CreateRectangleGeometry(
                self: *T,
                rect: *const RECT_F,
                geo: *?*IRectangleGeometry,
            ) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateRectangleGeometry(@ptrCast(*IFactory, self), rect, geo);
            }
            pub inline fn CreateRoundedRectangleGeometry(
                self: *T,
                rect: *const ROUNDED_RECT,
                geo: *?*IRoundedRectangleGeometry,
            ) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateRoundedRectangleGeometry(@ptrCast(*IFactory, self), rect, geo);
            }
            pub inline fn CreateEllipseGeometry(
                self: *T,
                ellipse: *const ELLIPSE,
                geo: *?*IEllipseGeometry,
            ) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateEllipseGeometry(@ptrCast(*IFactory, self), ellipse, geo);
            }
            pub inline fn CreatePathGeometry(self: *T, geo: *?*IPathGeometry) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v).CreatePathGeometry(@ptrCast(*IFactory, self), geo);
            }
            pub inline fn CreateStrokeStyle(
                self: *T,
                properties: *const STROKE_STYLE_PROPERTIES,
                dashes: ?[*]const FLOAT,
                dashes_count: UINT32,
                stroke_style: *?*IStrokeStyle,
            ) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateStrokeStyle(@ptrCast(*IFactory, self), properties, dashes, dashes_count, stroke_style);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IFactory;
        base: IUnknown.VTable,
        ReloadSystemMetrics: *anyopaque,
        GetDesktopDpi: *anyopaque,
        CreateRectangleGeometry: *const fn (*T, *const RECT_F, *?*IRectangleGeometry) callconv(WINAPI) HRESULT,
        CreateRoundedRectangleGeometry: *const fn (
            *T,
            *const ROUNDED_RECT,
            *?*IRoundedRectangleGeometry,
        ) callconv(WINAPI) HRESULT,
        CreateEllipseGeometry: *const fn (*T, *const ELLIPSE, *?*IEllipseGeometry) callconv(WINAPI) HRESULT,
        CreateGeometryGroup: *anyopaque,
        CreateTransformedGeometry: *anyopaque,
        CreatePathGeometry: *const fn (*T, *?*IPathGeometry) callconv(WINAPI) HRESULT,
        CreateStrokeStyle: *const fn (
            *T,
            *const STROKE_STYLE_PROPERTIES,
            ?[*]const FLOAT,
            UINT32,
            *?*IStrokeStyle,
        ) callconv(WINAPI) HRESULT,
        CreateDrawingStateBlock: *anyopaque,
        CreateWicBitmapRenderTarget: *anyopaque,
        CreateHwndRenderTarget: *anyopaque,
        CreateDxgiSurfaceRenderTarget: *anyopaque,
        CreateDCRenderTarget: *anyopaque,
    };
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
    *?*anyopaque,
) callconv(WINAPI) HRESULT;

pub const IBitmap1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IBitmap.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IBitmap.VTable,
        GetColorContext: *anyopaque,
        GetOptions: *anyopaque,
        GetSurface: *anyopaque,
        Map: *anyopaque,
        Unmap: *anyopaque,
    };
};

pub const IColorContext = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetColorSpace: *anyopaque,
        GetProfileSize: *anyopaque,
        GetProfile: *anyopaque,
    };
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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IRenderTarget.Methods(T);

            pub inline fn CreateBitmapFromDxgiSurface(
                self: *T,
                surface: *dxgi.ISurface,
                properties: ?*const BITMAP_PROPERTIES1,
                bitmap: *?*IBitmap1,
            ) HRESULT {
                return @ptrCast(*const IDeviceContext.VTable, self.__v)
                    .CreateBitmapFromDxgiSurface(@ptrCast(*IDeviceContext, self), surface, properties, bitmap);
            }
            pub inline fn SetTarget(self: *T, image: ?*IImage) void {
                @ptrCast(*const IDeviceContext.VTable, self.__v).SetTarget(@ptrCast(*IDeviceContext, self), image);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IDeviceContext;
        base: IRenderTarget.VTable,
        CreateBitmap1: *anyopaque,
        CreateBitmapFromWicBitmap1: *anyopaque,
        CreateColorContext: *anyopaque,
        CreateColorContextFromFilename: *anyopaque,
        CreateColorContextFromWicColorContext: *anyopaque,
        CreateBitmapFromDxgiSurface: *const fn (
            *T,
            *dxgi.ISurface,
            ?*const BITMAP_PROPERTIES1,
            *?*IBitmap1,
        ) callconv(WINAPI) HRESULT,
        CreateEffect: *anyopaque,
        CreateGradientStopCollection1: *anyopaque,
        CreateImageBrush: *anyopaque,
        CreateBitmapBrush1: *anyopaque,
        CreateCommandList: *anyopaque,
        IsDxgiFormatSupported: *anyopaque,
        IsBufferPrecisionSupported: *anyopaque,
        GetImageLocalBounds: *anyopaque,
        GetImageWorldBounds: *anyopaque,
        GetGlyphRunWorldBounds: *anyopaque,
        GetDevice: *anyopaque,
        SetTarget: *const fn (*T, ?*IImage) callconv(WINAPI) void,
        GetTarget: *anyopaque,
        SetRenderingControls: *anyopaque,
        GetRenderingControls: *anyopaque,
        SetPrimitiveBlend: *anyopaque,
        GetPrimitiveBlend: *anyopaque,
        SetUnitMode: *anyopaque,
        GetUnitMode: *anyopaque,
        DrawGlyphRun1: *anyopaque,
        DrawImage: *anyopaque,
        DrawGdiMetafile: *anyopaque,
        DrawBitmap1: *anyopaque,
        PushLayer1: *anyopaque,
        InvalidateEffectInputRectangle: *anyopaque,
        GetEffectInvalidRectangleCount: *anyopaque,
        GetEffectInvalidRectangles: *anyopaque,
        GetEffectRequiredInputRectangles: *anyopaque,
        FillOpacityMask1: *anyopaque,
    };
};

pub const IFactory1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory.VTable,
        CreateDevice: *anyopaque,
        CreateStrokeStyle1: *anyopaque,
        CreatePathGeometry1: *anyopaque,
        CreateDrawingStateBlock1: *anyopaque,
        CreateGdiMetafile: *anyopaque,
        RegisterEffectFromStream: *anyopaque,
        RegisterEffectFromString: *anyopaque,
        UnregisterEffect: *anyopaque,
        GetRegisteredEffects: *anyopaque,
        GetEffectProperties: *anyopaque,
    };
};

pub const IDevice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        CreateDeviceContext: *anyopaque,
        CreatePrintControl: *anyopaque,
        SetMaximumTextureMemory: *anyopaque,
        GetMaximumTextureMemory: *anyopaque,
        ClearResources: *anyopaque,
    };
};

pub const IDeviceContext1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceContext.VTable,
        CreateFilledGeometryRealization: *anyopaque,
        CreateStrokedGeometryRealization: *anyopaque,
        DrawGeometryRealization: *anyopaque,
    };
};

pub const IFactory2 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory1.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory1.VTable,
        CreateDevice1: *anyopaque,
    };
};

pub const IDevice1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDevice.VTable,
        GetRenderingPriority: *anyopaque,
        SetRenderingPriority: *anyopaque,
        CreateDeviceContext1: *anyopaque,
    };
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
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);

            pub inline fn SetStartPoint(self: *T, point: *const INK_POINT) void {
                @ptrCast(*const IInk.VTable, self.__v).SetStartPoint(@ptrCast(*IInk, self), point);
            }
            pub inline fn GetStartPoint(self: *T) INK_POINT {
                var point: INK_POINT = undefined;
                _ = @ptrCast(*const IInk.VTable, self.__v).GetStartPoint(@ptrCast(*IInk, self), &point);
                return point;
            }
            pub inline fn AddSegments(self: *T, segments: [*]const INK_BEZIER_SEGMENT, count: UINT32) HRESULT {
                return @ptrCast(*const IInk.VTable, self.__v).AddSegments(@ptrCast(*IInk, self), segments, count);
            }
            pub inline fn RemoveSegmentsAtEnd(self: *T, count: UINT32) HRESULT {
                return @ptrCast(*const IInk.VTable, self.__v).RemoveSegmentsAtEnd(@ptrCast(*IInk, self), count);
            }
            pub inline fn SetSegments(
                self: *T,
                start_segment: UINT32,
                segments: [*]const INK_BEZIER_SEGMENT,
                count: UINT32,
            ) HRESULT {
                return @ptrCast(*const IInk.VTable, self.__v)
                    .SetSegments(@ptrCast(*IInk, self), start_segment, segments, count);
            }
            pub inline fn SetSegmentAtEnd(self: *T, segment: *const INK_BEZIER_SEGMENT) HRESULT {
                return @ptrCast(*const IInk.VTable, self.__v).SetSegmentAtEnd(@ptrCast(*IInk, self), segment);
            }
            pub inline fn GetSegmentCount(self: *T) UINT32 {
                return @ptrCast(*const IInk.VTable, self.__v).GetSegmentCount(@ptrCast(*IInk, self));
            }
            pub inline fn GetSegments(
                self: *T,
                start_segment: UINT32,
                segments: [*]const INK_BEZIER_SEGMENT,
                count: UINT32,
            ) HRESULT {
                return @ptrCast(*const IInk.VTable, self.__v)
                    .GetSegments(@ptrCast(*IInk, self), start_segment, segments, count);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IInk;
        base: IResource.VTable,
        SetStartPoint: *const fn (*T, *const INK_POINT) callconv(WINAPI) void,
        GetStartPoint: *const fn (*T, *INK_POINT) callconv(WINAPI) *INK_POINT,
        AddSegments: *const fn (*T, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
        RemoveSegmentsAtEnd: *const fn (*T, UINT32) callconv(WINAPI) HRESULT,
        SetSegments: *const fn (*T, UINT32, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
        SetSegmentAtEnd: *const fn (*T, *const INK_BEZIER_SEGMENT) callconv(WINAPI) HRESULT,
        GetSegmentCount: *const fn (*T) callconv(WINAPI) UINT32,
        GetSegments: *const fn (*T, UINT32, [*]const INK_BEZIER_SEGMENT, UINT32) callconv(WINAPI) HRESULT,
        StreamAsGeometry: *anyopaque,
        GetBounds: *anyopaque,
    };
};

pub const IInkStyle = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        SetNibTransform: *anyopaque,
        GetNibTransform: *anyopaque,
        SetNibShape: *anyopaque,
        GetNibShape: *anyopaque,
    };
};

pub const IDeviceContext2 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext1.Methods(T);

            pub inline fn CreateInk(self: *T, start_point: *const INK_POINT, ink: *?*IInk) HRESULT {
                return @ptrCast(*const IDeviceContext2.VTable, self.__v)
                    .CreateInk(@ptrCast(*IDeviceContext2, self), start_point, ink);
            }
            pub inline fn CreateInkStyle(
                self: *T,
                properties: ?*const INK_STYLE_PROPERTIES,
                ink_style: *?*IInkStyle,
            ) HRESULT {
                return @ptrCast(*const IDeviceContext2.VTable, self.__v)
                    .CreateInkStyle(@ptrCast(*IDeviceContext2, self), properties, ink_style);
            }
            pub inline fn DrawInk(self: *T, ink: *IInk, brush: *IBrush, style: ?*IInkStyle) void {
                return @ptrCast(*const IDeviceContext2.VTable, self.__v)
                    .DrawInk(@ptrCast(*IDeviceContext2, self), ink, brush, style);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IDeviceContext2;
        base: IDeviceContext1.VTable,
        CreateInk: *const fn (*T, *const INK_POINT, *?*IInk) callconv(WINAPI) HRESULT,
        CreateInkStyle: *const fn (*T, ?*const INK_STYLE_PROPERTIES, *?*IInkStyle) callconv(WINAPI) HRESULT,
        CreateGradientMesh: *anyopaque,
        CreateImageSourceFromWic: *anyopaque,
        CreateLookupTable3D: *anyopaque,
        CreateImageSourceFromDxgi: *anyopaque,
        GetGradientMeshWorldBounds: *anyopaque,
        DrawInk: *const fn (*T, *IInk, *IBrush, ?*IInkStyle) callconv(WINAPI) void,
        DrawGradientMesh: *anyopaque,
        DrawGdiMetafile1: *anyopaque,
        CreateTransformedImageSource: *anyopaque,
    };
};

pub const IDeviceContext3 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext2.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceContext2.VTable,
        CreateSpriteBatch: *anyopaque,
        DrawSpriteBatch: *anyopaque,
    };
};

pub const IDeviceContext4 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext3.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceContext3.VTable,
        CreateSvgGlyphStyle: *anyopaque,
        DrawText1: *anyopaque,
        DrawTextLayout1: *anyopaque,
        DrawColorBitmapGlyphRun: *anyopaque,
        DrawSvgGlyphRun: *anyopaque,
        GetColorBitmapGlyphImage: *anyopaque,
        GetSvgGlyphImage: *anyopaque,
    };
};

pub const IDeviceContext5 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext4.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceContext4.VTable,
        CreateSvgDocument: *anyopaque,
        DrawSvgDocument: *anyopaque,
        CreateColorContextFromDxgiColorSpace: *anyopaque,
        CreateColorContextFromSimpleColorProfile: *anyopaque,
    };
};

pub const IDeviceContext6 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceContext5.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceContext5.VTable,
        BlendImage: *anyopaque,
    };
};

pub const IFactory3 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory2.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory2.VTable,
        CreateDevice2: *anyopaque,
    };
};

pub const IFactory4 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory3.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory3.VTable,
        CreateDevice3: *anyopaque,
    };
};

pub const IFactory5 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory4.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory4.VTable,
        CreateDevice4: *anyopaque,
    };
};

pub const IFactory6 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory5.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IFactory5.VTable,
        CreateDevice5: *anyopaque,
    };
};

pub const IFactory7 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory6.Methods(T);

            pub inline fn CreateDevice6(self: *T, dxgi_device: *dxgi.IDevice, d2d_device6: *?*IDevice6) HRESULT {
                return @ptrCast(*const IFactory7.VTable, self.__v)
                    .CreateDevice6(@ptrCast(*IFactory7, self), dxgi_device, d2d_device6);
            }
        };
    }

    pub const VTable = extern struct {
        base: IFactory6.VTable,
        CreateDevice6: *const fn (*IFactory7, *dxgi.IDevice, *?*IDevice6) callconv(WINAPI) HRESULT,
    };
};

pub const IDevice2 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice1.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDevice1.VTable,
        CreateDeviceContext2: *anyopaque,
        FlushDeviceContexts: *anyopaque,
        GetDxgiDevice: *anyopaque,
    };
};

pub const IDevice3 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice2.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDevice2.VTable,
        CreateDeviceContext3: *anyopaque,
    };
};

pub const IDevice4 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice3.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDevice3.VTable,
        CreateDeviceContext4: *anyopaque,
        SetMaximumColorGlyphCacheMemory: *anyopaque,
        GetMaximumColorGlyphCacheMemory: *anyopaque,
    };
};

pub const IDevice5 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice4.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDevice4.VTable,
        CreateDeviceContext5: *anyopaque,
    };
};

pub const IDevice6 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice5.Methods(T);

            pub inline fn CreateDeviceContext6(
                self: *T,
                options: DEVICE_CONTEXT_OPTIONS,
                devctx: *?*IDeviceContext6,
            ) HRESULT {
                return @ptrCast(*const IDevice6.VTable, self.__v)
                    .CreateDeviceContext6(@ptrCast(*IDevice6, self), options, devctx);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDevice5.VTable,
        CreateDeviceContext6: *const fn (
            *IDevice6,
            DEVICE_CONTEXT_OPTIONS,
            *?*IDeviceContext6,
        ) callconv(WINAPI) HRESULT,
    };
};

pub const IID_IFactory7 = GUID{
    .Data1 = 0xbdc2bdd3,
    .Data2 = 0xb96c,
    .Data3 = 0x4de6,
    .Data4 = .{ 0xbd, 0xf7, 0x99, 0xd4, 0x74, 0x54, 0x54, 0xde },
};
