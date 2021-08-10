const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");
const IDWriteTextFormat = @import("dwrite.zig").IDWriteTextFormat;
const assert = std.debug.assert;

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

pub const ID2D1SimplifiedGeometrySink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        simgeosink: VTable(Self),
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
            SetFillMode: *c_void,
            SetSegmentFlags: *c_void,
            BeginFigure: *c_void,
            AddLines: *c_void,
            AddBeziers: *c_void,
            EndFigure: *c_void,
            Close: *c_void,
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
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            AddLine: *c_void,
            AddBezier: *c_void,
            AddQuadraticBezier: *c_void,
            AddQuadraticBeziers: *c_void,
            AddArc: *c_void,
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
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            Open: *c_void,
            Stream: *c_void,
            GetSegmentCount: *c_void,
            GetFigureCount: *c_void,
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

pub const D2D1_BITMAP_INTERPOLATION_MODE = enum(UINT) {
    NEAREST_NEIGHBOR = 0,
    LINEAR = 1,
};

pub const D2D1_DRAW_TEXT_OPTIONS = packed struct {
    NO_SNAP: bool align(4) = false, // 0x1
    CLIP: bool = false, // 0x2
    ENABLE_COLOR_FONT: bool = false, // 0x4
    DISABLE_COLOR_BITMAP_SNAPPING: bool = false, // 0x8
    __reserved4: bool = false,
    __reserved5: bool = false,
    __reserved6: bool = false,
    __reserved7: bool = false,
    __reserved8: bool = false,
    __reserved9: bool = false,
    __reserved10: bool = false,
    __reserved11: bool = false,
    __reserved12: bool = false,
    __reserved13: bool = false,
    __reserved14: bool = false,
    __reserved15: bool = false,
    __reserved16: bool = false,
    __reserved17: bool = false,
    __reserved18: bool = false,
    __reserved19: bool = false,
    __reserved20: bool = false,
    __reserved21: bool = false,
    __reserved22: bool = false,
    __reserved23: bool = false,
    __reserved24: bool = false,
    __reserved25: bool = false,
    __reserved26: bool = false,
    __reserved27: bool = false,
    __reserved28: bool = false,
    __reserved29: bool = false,
    __reserved30: bool = false,
    __reserved31: bool = false,
    pub usingnamespace FlagsMixin(@This());
};

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
            pub inline fn FillRectangle(self: *T, rect: *const D2D1_RECT_F, brush: *ID2D1Brush) void {
                self.v.rendertarget.FillRectangle(self, rect, brush);
            }
            pub inline fn FillEllipse(self: *T, ellipse: *const D2D1_ELLIPSE, brush: *ID2D1Brush) void {
                self.v.rendertarget.FillEllipse(self, ellipse, brush);
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
                DrawText(self, &utf16, @intCast(u32, len), format, layout_rect, brush, .{}, .NATURAL);
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
            CreateGradientStopCollection: *c_void,
            CreateLinearGradientBrush: *c_void,
            CreateRadialGradientBrush: *c_void,
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
            DrawRectangle: *c_void,
            FillRectangle: fn (*T, *const D2D1_RECT_F, *ID2D1Brush) callconv(WINAPI) void,
            DrawRoundedRectangle: *c_void,
            FillRoundedRectangle: *c_void,
            DrawEllipse: *c_void,
            FillEllipse: fn (*T, *const D2D1_ELLIPSE, *ID2D1Brush) callconv(WINAPI) void,
            DrawGeometry: *c_void,
            FillGeometry: *c_void,
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
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            ReloadSystemMetrics: *c_void,
            GetDesktopDpi: *c_void,
            CreateRectangleGeometry: *c_void,
            CreateRoundedRectangleGeometry: *c_void,
            CreateEllipseleGeometry: *c_void,
            CreateGeometryGroup: *c_void,
            CreateTransformedGeometry: *c_void,
            CreatePathGeometry: *c_void,
            CreateStrokeStyle: *c_void,
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
