const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");
const IDWriteTextFormat = @import("dwrite.zig").IDWriteTextFormat;
const assert = std.debug.assert;

pub const D2D1_CAP_STYLE = enum(UINT) {
    FLAT = 0,
    SQUARE = 1,
    ROUND = 2,
    TRIANGLE = 3,
};

pub const D2D1_DASH_STYLE = enum(UINT) {
    SOLID = 0,
    DASH = 1,
    DOT = 2,
    DASH_DOT = 3,
    DASH_DOT_DOT = 4,
    CUSTOM = 5,
};

pub const D2D1_LINE_JOIN = enum(UINT) {
    MITER = 0,
    BEVEL = 1,
    ROUND = 2,
    MITER_OR_BEVEL = 3,
};

pub const D2D1_STROKE_STYLE_PROPERTIES = extern struct {
    startCap: D2D1_CAP_STYLE,
    endCap: D2D1_CAP_STYLE,
    dashCap: D2D1_CAP_STYLE,
    lineJoin: D2D1_LINE_JOIN,
    miterLimit: FLOAT,
    dashStyle: D2D1_DASH_STYLE,
    dashOffset: FLOAT,
};

pub const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES = extern struct {
    center: D2D1_POINT_2F,
    gradientOriginOffset: D2D1_POINT_2F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const ID2D1Resource = extern struct {
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

pub const ID2D1Image = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        image: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1Bitmap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        image: ID2D1Image.VTable(Self),
        bitmap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Image.Methods(Self);
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

pub const D2D1_GAMMA = enum(UINT) {
    _2_2 = 0,
    _1_0 = 1,
};

pub const D2D1_EXTEND_MODE = enum(UINT) {
    CLAMP = 0,
    WRAP = 1,
    MIRROR = 2,
};

pub const D2D1_GRADIENT_STOP = extern struct {
    position: FLOAT,
    color: D2D1_COLOR_F,
};

pub const ID2D1GradientStopCollection = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        gradsc: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1Brush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        brush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1BitmapBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        brush: ID2D1Brush.VTable(Self),
        bmpbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Brush.Methods(Self);
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

pub const ID2D1SolidColorBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        brush: ID2D1Brush.VTable(Self),
        scbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Brush.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetColor(self: *T, color: *const D2D1_COLOR_F) void {
                self.v.scbrush.SetColor(self, color);
            }
            pub inline fn GetColor(self: *T) D2D1_COLOR_F {
                var color: D2D1_COLOR_F = undefined;
                _ = self.v.scbrush.GetColor(self, &color);
                return color;
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetColor: fn (*T, *const D2D1_COLOR_F) callconv(WINAPI) void,
            GetColor: fn (*T, *D2D1_COLOR_F) callconv(WINAPI) *D2D1_COLOR_F,
        };
    }
};

pub const ID2D1LinearGradientBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        brush: ID2D1Brush.VTable(Self),
        lgbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Brush.Methods(Self);
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

pub const ID2D1RadialGradientBrush = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        brush: ID2D1Brush.VTable(Self),
        rgbrush: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Brush.Methods(Self);
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

pub const ID2D1StrokeStyle = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        strokestyle: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1Geometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1RectangleGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        rectgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
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

pub const ID2D1RoundedRectangleGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        roundedrectgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
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

pub const ID2D1EllipseGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        ellipsegeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
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

pub const ID2D1GeometryGroup = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        geogroup: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
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

pub const ID2D1TransformedGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        transgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
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

pub const D2D1_FIGURE_BEGIN = enum(UINT) {
    FILLED = 0,
    HOLLOW = 1,
};

pub const D2D1_FIGURE_END = enum(UINT) {
    OPEN = 0,
    CLOSED = 1,
};

pub const D2D1_BEZIER_SEGMENT = extern struct {
    point1: D2D1_POINT_2F,
    point2: D2D1_POINT_2F,
    point3: D2D1_POINT_2F,
};

pub const D2D1_TRIANGLE = extern struct {
    point1: D2D1_POINT_2F,
    point2: D2D1_POINT_2F,
    point3: D2D1_POINT_2F,
};

pub const D2D1_PATH_SEGMENT = UINT;
pub const D2D1_PATH_SEGMENT_NONE = 0x00000000;
pub const D2D1_PATH_SEGMENT_FORCE_UNSTROKED = 0x00000001;
pub const D2D1_PATH_SEGMENT_FORCE_ROUND_LINE_JOIN = 0x00000002;

pub const D2D1_SWEEP_DIRECTION = enum(UINT) {
    COUNTER_CLOCKWISE = 0,
    CLOCKWISE = 1,
};

pub const D2D1_FILL_MODE = enum(UINT) {
    ALTERNATE = 0,
    WINDING = 1,
};

pub const D2D1_ARC_SIZE = enum(UINT) {
    SMALL = 0,
    LARGE = 1,
};

pub const D2D1_ARC_SEGMENT = extern struct {
    point: D2D1_POINT_2F,
    size: D2D1_SIZE_F,
    rotationAngle: FLOAT,
    sweepDirection: D2D1_SWEEP_DIRECTION,
    arcSize: D2D1_ARC_SIZE,
};

pub const D2D1_QUADRATIC_BEZIER_SEGMENT = extern struct {
    point1: D2D1_POINT_2F,
    point2: D2D1_POINT_2F,
};

pub const ID2D1SimplifiedGeometrySink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        simgeosink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetFillMode(self: *T, mode: D2D1_FILL_MODE) void {
                self.v.simgeosink.SetFillMode(self, mode);
            }
            pub inline fn SetSegmentFlags(self: *T, flags: D2D1_PATH_SEGMENT) void {
                self.v.simgeosink.SetSegmentFlags(self, flags);
            }
            pub inline fn BeginFigure(self: *T, point: D2D1_POINT_2F, begin: D2D1_FIGURE_BEGIN) void {
                self.v.simgeosink.BeginFigure(self, point, begin);
            }
            pub inline fn AddLines(self: *T, points: [*]const D2D1_POINT_2F, count: UINT32) void {
                self.v.simgeosink.AddLines(self, points, count);
            }
            pub inline fn AddBeziers(self: *T, segments: [*]const D2D1_BEZIER_SEGMENT, count: UINT32) void {
                self.v.simgeosink.AddBeziers(self, segments, count);
            }
            pub inline fn EndFigure(self: *T, end: D2D1_FIGURE_END) void {
                self.v.simgeosink.EndFigure(self, end);
            }
            pub inline fn Close(self: *T) HRESULT {
                return self.v.simgeosink.Close(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetFillMode: fn (*T, D2D1_FILL_MODE) callconv(WINAPI) void,
            SetSegmentFlags: fn (*T, D2D1_PATH_SEGMENT) callconv(WINAPI) void,
            BeginFigure: fn (*T, D2D1_POINT_2F, D2D1_FIGURE_BEGIN) callconv(WINAPI) void,
            AddLines: fn (*T, [*]const D2D1_POINT_2F, UINT32) callconv(WINAPI) void,
            AddBeziers: fn (*T, [*]const D2D1_BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
            EndFigure: fn (*T, D2D1_FIGURE_END) callconv(WINAPI) void,
            Close: fn (*T) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID2D1GeometrySink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        simgeosink: ID2D1SimplifiedGeometrySink.VTable(Self),
        geosink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1SimplifiedGeometrySink.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AddLine(self: *T, point: D2D1_POINT_2F) void {
                self.v.geosink.AddLine(self, point);
            }
            pub inline fn AddBezier(self: *T, segment: *const D2D1_BEZIER_SEGMENT) void {
                self.v.geosink.AddBezier(self, segment);
            }
            pub inline fn AddQuadraticBezier(self: *T, segment: *const D2D1_QUADRATIC_BEZIER_SEGMENT) void {
                self.v.geosink.AddQuadraticBezier(self, segment);
            }
            pub inline fn AddQuadraticBeziers(self: *T, segments: [*]const D2D1_QUADRATIC_BEZIER_SEGMENT, count: UINT32) void {
                self.v.geosink.AddQuadraticBeziers(self, segments, count);
            }
            pub inline fn AddArc(self: *T, segment: *const D2D1_ARC_SEGMENT) void {
                self.v.geosink.AddArc(self, segment);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            AddLine: fn (*T, D2D1_POINT_2F) callconv(WINAPI) void,
            AddBezier: fn (*T, *const D2D1_BEZIER_SEGMENT) callconv(WINAPI) void,
            AddQuadraticBezier: fn (*T, *const D2D1_QUADRATIC_BEZIER_SEGMENT) callconv(WINAPI) void,
            AddQuadraticBeziers: fn (*T, [*]const D2D1_QUADRATIC_BEZIER_SEGMENT, UINT32) callconv(WINAPI) void,
            AddArc: fn (*T, *const D2D1_ARC_SEGMENT) callconv(WINAPI) void,
        };
    }
};

pub const ID2D1TessellationSink = extern struct {
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

pub const ID2D1PathGeometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: ID2D1Geometry.VTable(Self),
        pathgeo: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Geometry.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Open(self: *T, sink: *?*ID2D1GeometrySink) HRESULT {
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
            Open: fn (*T, *?*ID2D1GeometrySink) callconv(WINAPI) HRESULT,
            Stream: *c_void,
            GetSegmentCount: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
            GetFigureCount: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID2D1Mesh = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        mesh: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1Layer = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        layer: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const ID2D1DrawingStateBlock = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        stateblock: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
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

pub const D2D1_BRUSH_PROPERTIES = extern struct {
    opacity: FLOAT,
    transform: D2D1_MATRIX_3X2_F,
};

pub const D2D1_ELLIPSE = extern struct {
    point: D2D1_POINT_2F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const D2D1_ROUNDED_RECT = extern struct {
    rect: D2D1_RECT_F,
    radiusX: FLOAT,
    radiusY: FLOAT,
};

pub const D2D1_BITMAP_INTERPOLATION_MODE = enum(UINT) {
    NEAREST_NEIGHBOR = 0,
    LINEAR = 1,
};

pub const D2D1_DRAW_TEXT_OPTIONS = UINT;
pub const D2D1_DRAW_TEXT_OPTIONS_NONE = 0;
pub const D2D1_DRAW_TEXT_OPTIONS_NO_SNAP = 0x1;
pub const D2D1_DRAW_TEXT_OPTIONS_CLIP = 0x2;
pub const D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT = 0x4;
pub const D2D1_DRAW_TEXT_OPTIONS_DISABLE_COLOR_BITMAP_SNAPPING = 0x8;

pub const D2D1_TAG = UINT64;

pub const ID2D1RenderTarget = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateSolidColorBrush(
                self: *T,
                color: *const D2D1_COLOR_F,
                properties: ?*const D2D1_BRUSH_PROPERTIES,
                brush: *?*ID2D1SolidColorBrush,
            ) HRESULT {
                return self.v.rendertarget.CreateSolidColorBrush(self, color, properties, brush);
            }
            pub inline fn CreateGradientStopCollection(
                self: *T,
                stops: [*]const D2D1_GRADIENT_STOP,
                num_stops: UINT32,
                gamma: D2D1_GAMMA,
                extend_mode: D2D1_EXTEND_MODE,
                stop_collection: *?*ID2D1GradientStopCollection,
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
                gradient_properties: *const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES,
                brush_properties: ?*const D2D1_BRUSH_PROPERTIES,
                stop_collection: *ID2D1GradientStopCollection,
                brush: *?*ID2D1RadialGradientBrush,
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
                p0: D2D1_POINT_2F,
                p1: D2D1_POINT_2F,
                brush: *ID2D1Brush,
                width: FLOAT,
                style: ?*ID2D1StrokeStyle,
            ) void {
                self.v.rendertarget.DrawLine(self, p0, p1, brush, width, style);
            }
            pub inline fn DrawRectangle(
                self: *T,
                rect: *const D2D1_RECT_F,
                brush: *ID2D1Brush,
                width: FLOAT,
                stroke: ?*ID2D1StrokeStyle,
            ) void {
                self.v.rendertarget.DrawRectangle(self, rect, brush, width, stroke);
            }
            pub inline fn FillRectangle(self: *T, rect: *const D2D1_RECT_F, brush: *ID2D1Brush) void {
                self.v.rendertarget.FillRectangle(self, rect, brush);
            }
            pub inline fn DrawRoundedRectangle(
                self: *T,
                rect: *const D2D1_ROUNDED_RECT,
                brush: *ID2D1Brush,
                width: FLOAT,
                stroke: ?*ID2D1StrokeStyle,
            ) void {
                self.v.rendertarget.DrawRoundedRectangle(self, rect, brush, width, stroke);
            }
            pub inline fn FillRoundedRectangle(self: *T, rect: *const D2D1_ROUNDED_RECT, brush: *ID2D1Brush) void {
                self.v.rendertarget.FillRoundedRectangle(self, rect, brush);
            }
            pub inline fn DrawEllipse(
                self: *T,
                ellipse: *const D2D1_ELLIPSE,
                brush: *ID2D1Brush,
                width: FLOAT,
                stroke: ?*ID2D1StrokeStyle,
            ) void {
                self.v.rendertarget.DrawEllipse(self, ellipse, brush, width, stroke);
            }
            pub inline fn FillEllipse(self: *T, ellipse: *const D2D1_ELLIPSE, brush: *ID2D1Brush) void {
                self.v.rendertarget.FillEllipse(self, ellipse, brush);
            }
            pub inline fn DrawGeometry(
                self: *T,
                geo: *ID2D1Geometry,
                brush: *ID2D1Brush,
                width: FLOAT,
                stroke: ?*ID2D1StrokeStyle,
            ) void {
                self.v.rendertarget.DrawGeometry(self, geo, brush, width, stroke);
            }
            pub inline fn FillGeometry(self: *T, geo: *ID2D1Geometry, brush: *ID2D1Brush, opacity_brush: ?*ID2D1Brush) void {
                self.v.rendertarget.FillGeometry(self, geo, brush, opacity_brush);
            }
            pub inline fn DrawBitmap(
                self: *T,
                bitmap: *ID2D1Bitmap,
                dst_rect: ?*const D2D1_RECT_F,
                opacity: FLOAT,
                interpolation_mode: D2D1_BITMAP_INTERPOLATION_MODE,
                src_rect: ?*const D2D1_RECT_F,
            ) void {
                self.v.rendertarget.DrawBitmap(self, bitmap, dst_rect, opacity, interpolation_mode, src_rect);
            }
            pub inline fn DrawText(
                self: *T,
                string: LPCWSTR,
                length: UINT,
                format: *IDWriteTextFormat,
                layout_rect: *const D2D1_RECT_F,
                brush: *ID2D1Brush,
                options: D2D1_DRAW_TEXT_OPTIONS,
                measuring_mode: DWRITE_MEASURING_MODE,
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
            pub inline fn SetTransform(self: *T, m: *const D2D1_MATRIX_3X2_F) void {
                self.v.rendertarget.SetTransform(self, m);
            }
            pub inline fn Clear(self: *T, color: ?*const D2D1_COLOR_F) void {
                self.v.rendertarget.Clear(self, color);
            }
            pub inline fn BeginDraw(self: *T) void {
                self.v.rendertarget.BeginDraw(self);
            }
            pub inline fn EndDraw(self: *T, tag1: ?*D2D1_TAG, tag2: ?*D2D1_TAG) HRESULT {
                return self.v.rendertarget.EndDraw(self, tag1, tag2);
            }

            // NOTE(mziulek): This is a helper method to draw short utf8 strings (not part of D2D1 API).
            pub fn DrawTextSimple(
                self: *T,
                text: []const u8,
                format: *IDWriteTextFormat,
                layout_rect: *const D2D1_RECT_F,
                brush: *ID2D1Brush,
            ) void {
                var utf16: [128:0]u16 = undefined;
                assert(text.len < utf16.len);
                const len = std.unicode.utf8ToUtf16Le(utf16[0..], text) catch unreachable;
                utf16[len] = 0;
                DrawText(
                    self,
                    &utf16,
                    @intCast(u32, len),
                    format,
                    layout_rect,
                    brush,
                    D2D1_DRAW_TEXT_OPTIONS_NONE,
                    .NATURAL,
                );
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
                *const D2D1_COLOR_F,
                ?*const D2D1_BRUSH_PROPERTIES,
                *?*ID2D1SolidColorBrush,
            ) callconv(WINAPI) HRESULT,
            CreateGradientStopCollection: fn (
                *T,
                [*]const D2D1_GRADIENT_STOP,
                UINT32,
                D2D1_GAMMA,
                D2D1_EXTEND_MODE,
                *?*ID2D1GradientStopCollection,
            ) callconv(WINAPI) HRESULT,
            CreateLinearGradientBrush: *c_void,
            CreateRadialGradientBrush: fn (
                *T,
                *const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES,
                ?*const D2D1_BRUSH_PROPERTIES,
                *ID2D1GradientStopCollection,
                *?*ID2D1RadialGradientBrush,
            ) callconv(WINAPI) HRESULT,
            CreateCompatibleRenderTarget: *c_void,
            CreateLayer: *c_void,
            CreateMesh: *c_void,
            DrawLine: fn (
                *T,
                D2D1_POINT_2F,
                D2D1_POINT_2F,
                *ID2D1Brush,
                FLOAT,
                ?*ID2D1StrokeStyle,
            ) callconv(WINAPI) void,
            DrawRectangle: fn (*T, *const D2D1_RECT_F, *ID2D1Brush, FLOAT, ?*ID2D1StrokeStyle) callconv(WINAPI) void,
            FillRectangle: fn (*T, *const D2D1_RECT_F, *ID2D1Brush) callconv(WINAPI) void,
            DrawRoundedRectangle: fn (
                *T,
                *const D2D1_ROUNDED_RECT,
                *ID2D1Brush,
                FLOAT,
                ?*ID2D1StrokeStyle,
            ) callconv(WINAPI) void,
            FillRoundedRectangle: fn (*T, *const D2D1_ROUNDED_RECT, *ID2D1Brush) callconv(WINAPI) void,
            DrawEllipse: fn (*T, *const D2D1_ELLIPSE, *ID2D1Brush, FLOAT, ?*ID2D1StrokeStyle) callconv(WINAPI) void,
            FillEllipse: fn (*T, *const D2D1_ELLIPSE, *ID2D1Brush) callconv(WINAPI) void,
            DrawGeometry: fn (*T, *ID2D1Geometry, *ID2D1Brush, FLOAT, ?*ID2D1StrokeStyle) callconv(WINAPI) void,
            FillGeometry: fn (*T, *ID2D1Geometry, *ID2D1Brush, ?*ID2D1Brush) callconv(WINAPI) void,
            FillMesh: *c_void,
            FillOpacityMask: *c_void,
            DrawBitmap: fn (
                *T,
                *ID2D1Bitmap,
                ?*const D2D1_RECT_F,
                FLOAT,
                D2D1_BITMAP_INTERPOLATION_MODE,
                ?*const D2D1_RECT_F,
            ) callconv(WINAPI) void,
            DrawText: fn (
                *T,
                LPCWSTR,
                UINT,
                *IDWriteTextFormat,
                *const D2D1_RECT_F,
                *ID2D1Brush,
                D2D1_DRAW_TEXT_OPTIONS,
                DWRITE_MEASURING_MODE,
            ) callconv(WINAPI) void,
            DrawTextLayout: *c_void,
            DrawGlyphRun: *c_void,
            SetTransform: fn (*T, *const D2D1_MATRIX_3X2_F) callconv(WINAPI) void,
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
            Clear: fn (*T, ?*const D2D1_COLOR_F) callconv(WINAPI) void,
            BeginDraw: fn (*T) callconv(WINAPI) void,
            EndDraw: fn (*T, ?*D2D1_TAG, ?*D2D1_TAG) callconv(WINAPI) HRESULT,
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

pub const ID2D1Factory = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateRectangleGeometry(self: *T, rect: *const D2D1_RECT_F, geo: *?*ID2D1RectangleGeometry) HRESULT {
                return self.v.factory.CreateRectangleGeometry(self, rect, geo);
            }
            pub inline fn CreateRoundedRectangleGeometry(
                self: *T,
                rect: *const D2D1_ROUNDED_RECT,
                geo: *?*ID2D1RoundedRectangleGeometry,
            ) HRESULT {
                return self.v.factory.CreateRoundedRectangleGeometry(self, rect, geo);
            }
            pub inline fn CreateEllipseGeometry(self: *T, ellipse: *const D2D1_ELLIPSE, geo: *?*ID2D1EllipseGeometry) HRESULT {
                return self.v.factory.CreateEllipseGeometry(self, ellipse, geo);
            }
            pub inline fn CreatePathGeometry(self: *T, geo: *?*ID2D1PathGeometry) HRESULT {
                return self.v.factory.CreatePathGeometry(self, geo);
            }
            pub inline fn CreateStrokeStyle(
                self: *T,
                properties: *const D2D1_STROKE_STYLE_PROPERTIES,
                dashes: ?[*]const FLOAT,
                dashes_count: UINT32,
                stroke_style: *?*ID2D1StrokeStyle,
            ) HRESULT {
                return self.v.factory.CreateStrokeStyle(self, properties, dashes, dashes_count, stroke_style);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            ReloadSystemMetrics: *c_void,
            GetDesktopDpi: *c_void,
            CreateRectangleGeometry: fn (*T, *const D2D1_RECT_F, *?*ID2D1RectangleGeometry) callconv(WINAPI) HRESULT,
            CreateRoundedRectangleGeometry: fn (
                *T,
                *const D2D1_ROUNDED_RECT,
                *?*ID2D1RoundedRectangleGeometry,
            ) callconv(WINAPI) HRESULT,
            CreateEllipseGeometry: fn (*T, *const D2D1_ELLIPSE, *?*ID2D1EllipseGeometry) callconv(WINAPI) HRESULT,
            CreateGeometryGroup: *c_void,
            CreateTransformedGeometry: *c_void,
            CreatePathGeometry: fn (*T, *?*ID2D1PathGeometry) callconv(WINAPI) HRESULT,
            CreateStrokeStyle: fn (
                *T,
                *const D2D1_STROKE_STYLE_PROPERTIES,
                ?[*]const FLOAT,
                UINT32,
                *?*ID2D1StrokeStyle,
            ) callconv(WINAPI) HRESULT,
            CreateDrawingStateBlock: *c_void,
            CreateWicBitmapRenderTarget: *c_void,
            CreateHwndRenderTarget: *c_void,
            CreateDxgiSurfaceRenderTarget: *c_void,
            CreateDCRenderTarget: *c_void,
        };
    }
};

pub const D2D1_FACTORY_TYPE = enum(UINT) {
    SINGLE_THREADED = 0,
    MULTI_THREADED = 1,
};

pub const D2D1_DEBUG_LEVEL = enum(UINT) {
    NONE = 0,
    ERROR = 1,
    WARNING = 2,
    INFORMATION = 3,
};

pub const D2D1_FACTORY_OPTIONS = extern struct {
    debugLevel: D2D1_DEBUG_LEVEL,
};

pub extern "d2d1" fn D2D1CreateFactory(
    D2D1_FACTORY_TYPE,
    *const GUID,
    ?*const D2D1_FACTORY_OPTIONS,
    *?*c_void,
) callconv(WINAPI) HRESULT;
