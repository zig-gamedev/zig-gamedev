const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");

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
