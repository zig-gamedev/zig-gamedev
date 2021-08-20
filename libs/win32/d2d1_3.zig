const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");
usingnamespace @import("d2d1.zig");
usingnamespace @import("d2d1_1.zig");
usingnamespace @import("d2d1_2.zig");
usingnamespace @import("dxgi.zig");

pub const D2D1_INK_NIB_SHAPE = enum(UINT) {
    ROUND = 0,
    SQUARE = 1,
};

pub const D2D1_INK_POINT = extern struct {
    x: FLOAT,
    y: FLOAT,
    radius: FLOAT,
};

pub const D2D1_INK_BEZIER_SEGMENT = extern struct {
    point1: D2D1_INK_POINT,
    point2: D2D1_INK_POINT,
    point3: D2D1_INK_POINT,
};

pub const D2D1_INK_STYLE_PROPERTIES = extern struct {
    nibShape: D2D1_INK_NIB_SHAPE,
    nibTransform: D2D1_MATRIX_3X2_F,
};

pub const ID2D1Ink = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        ink: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetStartPoint(self: *T, point: *const D2D1_INK_POINT) void {
                self.v.ink.SetStartPoint(self, point);
            }
            pub inline fn GetStartPoint(self: *T) D2D1_INK_POINT {
                var point: D2D1_INK_POINT = undefined;
                self.v.ink.GetStartPoint(self, &point);
                return point;
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetStartPoint: fn (*T, *const D2D1_INK_POINT) callconv(WINAPI) void,
            GetStartPoint: fn (*T, *D2D1_INK_POINT) callconv(WINAPI) *D2D1_INK_POINT,
            AddSegments: *c_void,
            RemoveSegmentsAtEnd: *c_void,
            SetSegments: *c_void,
            SetSegmentAtEnd: *c_void,
            GetSegmentCount: *c_void,
            GetSegments: *c_void,
            StreamAsGeometry: *c_void,
            GetBounds: *c_void,
        };
    }
};

pub const ID2D1DeviceContext2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: ID2D1DeviceContext.VTable(Self),
        devctx1: ID2D1DeviceContext1.VTable(Self),
        devctx2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace ID2D1DeviceContext.Methods(Self);
    usingnamespace ID2D1DeviceContext1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateInk(self: *T, start_point: *const D2D1_INK_POINT, ink: *?*ID2D1Ink) HRESULT {
                return self.devctx2.CreateInk(self, start_point, ink);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateInk: fn (*T, *const D2D1_INK_POINT, *?*ID2D1Ink) callconv(WINAPI) HRESULT,
            CreateInkStyle: *c_void,
            CreateGradientMesh: *c_void,
            CreateImageSourceFromWic: *c_void,
            CreateLookupTable3D: *c_void,
            CreateImageSourceFromDxgi: *c_void,
            GetGradientMeshWorldBounds: *c_void,
            DrawInk: *c_void,
            DrawGradientMesh: *c_void,
            DrawGdiMetafile1: *c_void,
            CreateTransformedImageSource: *c_void,
        };
    }
};

pub const ID2D1DeviceContext3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: ID2D1DeviceContext.VTable(Self),
        devctx1: ID2D1DeviceContext1.VTable(Self),
        devctx2: ID2D1DeviceContext2.VTable(Self),
        devctx3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace ID2D1DeviceContext.Methods(Self);
    usingnamespace ID2D1DeviceContext1.Methods(Self);
    usingnamespace ID2D1DeviceContext2.Methods(Self);
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

pub const ID2D1DeviceContext4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: ID2D1DeviceContext.VTable(Self),
        devctx1: ID2D1DeviceContext1.VTable(Self),
        devctx2: ID2D1DeviceContext2.VTable(Self),
        devctx3: ID2D1DeviceContext3.VTable(Self),
        devctx4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace ID2D1DeviceContext.Methods(Self);
    usingnamespace ID2D1DeviceContext1.Methods(Self);
    usingnamespace ID2D1DeviceContext2.Methods(Self);
    usingnamespace ID2D1DeviceContext3.Methods(Self);
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

pub const ID2D1DeviceContext5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: ID2D1DeviceContext.VTable(Self),
        devctx1: ID2D1DeviceContext1.VTable(Self),
        devctx2: ID2D1DeviceContext2.VTable(Self),
        devctx3: ID2D1DeviceContext3.VTable(Self),
        devctx4: ID2D1DeviceContext4.VTable(Self),
        devctx5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace ID2D1DeviceContext.Methods(Self);
    usingnamespace ID2D1DeviceContext1.Methods(Self);
    usingnamespace ID2D1DeviceContext2.Methods(Self);
    usingnamespace ID2D1DeviceContext3.Methods(Self);
    usingnamespace ID2D1DeviceContext4.Methods(Self);
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

pub const ID2D1DeviceContext6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: ID2D1DeviceContext.VTable(Self),
        devctx1: ID2D1DeviceContext1.VTable(Self),
        devctx2: ID2D1DeviceContext2.VTable(Self),
        devctx3: ID2D1DeviceContext3.VTable(Self),
        devctx4: ID2D1DeviceContext4.VTable(Self),
        devctx5: ID2D1DeviceContext5.VTable(Self),
        devctx6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace ID2D1DeviceContext.Methods(Self);
    usingnamespace ID2D1DeviceContext1.Methods(Self);
    usingnamespace ID2D1DeviceContext2.Methods(Self);
    usingnamespace ID2D1DeviceContext3.Methods(Self);
    usingnamespace ID2D1DeviceContext4.Methods(Self);
    usingnamespace ID2D1DeviceContext5.Methods(Self);
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

pub const ID2D1Factory3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: ID2D1Factory.VTable(Self),
        factory1: ID2D1Factory1.VTable(Self),
        factory2: ID2D1Factory2.VTable(Self),
        factory3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Factory.Methods(Self);
    usingnamespace ID2D1Factory1.Methods(Self);
    usingnamespace ID2D1Factory2.Methods(Self);
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

pub const ID2D1Factory4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: ID2D1Factory.VTable(Self),
        factory1: ID2D1Factory1.VTable(Self),
        factory2: ID2D1Factory2.VTable(Self),
        factory3: ID2D1Factory3.VTable(Self),
        factory4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Factory.Methods(Self);
    usingnamespace ID2D1Factory1.Methods(Self);
    usingnamespace ID2D1Factory2.Methods(Self);
    usingnamespace ID2D1Factory3.Methods(Self);
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

pub const ID2D1Factory5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: ID2D1Factory.VTable(Self),
        factory1: ID2D1Factory1.VTable(Self),
        factory2: ID2D1Factory2.VTable(Self),
        factory3: ID2D1Factory3.VTable(Self),
        factory4: ID2D1Factory4.VTable(Self),
        factory5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Factory.Methods(Self);
    usingnamespace ID2D1Factory1.Methods(Self);
    usingnamespace ID2D1Factory2.Methods(Self);
    usingnamespace ID2D1Factory3.Methods(Self);
    usingnamespace ID2D1Factory4.Methods(Self);
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

pub const ID2D1Factory6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: ID2D1Factory.VTable(Self),
        factory1: ID2D1Factory1.VTable(Self),
        factory2: ID2D1Factory2.VTable(Self),
        factory3: ID2D1Factory3.VTable(Self),
        factory4: ID2D1Factory4.VTable(Self),
        factory5: ID2D1Factory5.VTable(Self),
        factory6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Factory.Methods(Self);
    usingnamespace ID2D1Factory1.Methods(Self);
    usingnamespace ID2D1Factory2.Methods(Self);
    usingnamespace ID2D1Factory3.Methods(Self);
    usingnamespace ID2D1Factory4.Methods(Self);
    usingnamespace ID2D1Factory5.Methods(Self);
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

pub const ID2D1Factory7 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        factory: ID2D1Factory.VTable(Self),
        factory1: ID2D1Factory1.VTable(Self),
        factory2: ID2D1Factory2.VTable(Self),
        factory3: ID2D1Factory3.VTable(Self),
        factory4: ID2D1Factory4.VTable(Self),
        factory5: ID2D1Factory5.VTable(Self),
        factory6: ID2D1Factory6.VTable(Self),
        factory7: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Factory.Methods(Self);
    usingnamespace ID2D1Factory1.Methods(Self);
    usingnamespace ID2D1Factory2.Methods(Self);
    usingnamespace ID2D1Factory3.Methods(Self);
    usingnamespace ID2D1Factory4.Methods(Self);
    usingnamespace ID2D1Factory5.Methods(Self);
    usingnamespace ID2D1Factory6.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateDevice6(self: *T, dxgi_device: *IDXGIDevice, d2d_device6: *?*ID2D1Device6) HRESULT {
                return self.v.factory7.CreateDevice6(self, dxgi_device, d2d_device6);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateDevice6: fn (*T, *IDXGIDevice, *?*ID2D1Device6) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID2D1Device2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        device: ID2D1Device.VTable(Self),
        device1: ID2D1Device1.VTable(Self),
        device2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Device.Methods(Self);
    usingnamespace ID2D1Device1.Methods(Self);
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

pub const ID2D1Device3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        device: ID2D1Device.VTable(Self),
        device1: ID2D1Device1.VTable(Self),
        device2: ID2D1Device2.VTable(Self),
        device3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Device.Methods(Self);
    usingnamespace ID2D1Device1.Methods(Self);
    usingnamespace ID2D1Device2.Methods(Self);
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

pub const ID2D1Device4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        device: ID2D1Device.VTable(Self),
        device1: ID2D1Device1.VTable(Self),
        device2: ID2D1Device2.VTable(Self),
        device3: ID2D1Device3.VTable(Self),
        device4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Device.Methods(Self);
    usingnamespace ID2D1Device1.Methods(Self);
    usingnamespace ID2D1Device2.Methods(Self);
    usingnamespace ID2D1Device3.Methods(Self);
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

pub const ID2D1Device5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        device: ID2D1Device.VTable(Self),
        device1: ID2D1Device1.VTable(Self),
        device2: ID2D1Device2.VTable(Self),
        device3: ID2D1Device3.VTable(Self),
        device4: ID2D1Device4.VTable(Self),
        device5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Device.Methods(Self);
    usingnamespace ID2D1Device1.Methods(Self);
    usingnamespace ID2D1Device2.Methods(Self);
    usingnamespace ID2D1Device3.Methods(Self);
    usingnamespace ID2D1Device4.Methods(Self);
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

pub const ID2D1Device6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        device: ID2D1Device.VTable(Self),
        device1: ID2D1Device1.VTable(Self),
        device2: ID2D1Device2.VTable(Self),
        device3: ID2D1Device3.VTable(Self),
        device4: ID2D1Device4.VTable(Self),
        device5: ID2D1Device5.VTable(Self),
        device6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Device.Methods(Self);
    usingnamespace ID2D1Device1.Methods(Self);
    usingnamespace ID2D1Device2.Methods(Self);
    usingnamespace ID2D1Device3.Methods(Self);
    usingnamespace ID2D1Device4.Methods(Self);
    usingnamespace ID2D1Device5.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateDeviceContext6(
                self: *T,
                options: D2D1_DEVICE_CONTEXT_OPTIONS,
                devctx: *?*ID2D1DeviceContext6,
            ) HRESULT {
                return self.v.device6.CreateDeviceContext6(self, options, devctx);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateDeviceContext6: fn (
                *T,
                D2D1_DEVICE_CONTEXT_OPTIONS,
                *?*ID2D1DeviceContext6,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_ID2D1Factory7 = GUID{
    .Data1 = 0xbdc2bdd3,
    .Data2 = 0xb96c,
    .Data3 = 0x4de6,
    .Data4 = .{ 0xbd, 0xf7, 0x99, 0xd4, 0x74, 0x54, 0x54, 0xde },
};
