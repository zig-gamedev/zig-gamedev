const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");
usingnamespace @import("d2d1.zig");
usingnamespace @import("dxgi.zig");

pub const ID2D1Bitmap1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        image: ID2D1Image.VTable(Self),
        bitmap: ID2D1Bitmap.VTable(Self),
        bitmap1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Image.Methods(Self);
    usingnamespace ID2D1Bitmap.Methods(Self);
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

pub const ID2D1ColorContext = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        colorctx: VTable(Self),
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
            GetColorSpace: *c_void,
            GetProfileSize: *c_void,
            GetProfile: *c_void,
        };
    }
};

pub const D2D1_BITMAP_OPTIONS = packed struct {
    TARGET: bool align(4) = false, // 0x1
    CANNOT_DRAW: bool = false, // 0x2
    CPU_READ: bool = false, // 0x4
    GDI_COMPATIBLE: bool = false, // 0x8
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

pub const D2D1_BITMAP_PROPERTIES1 = extern struct {
    pixelFormat: D2D1_PIXEL_FORMAT,
    dpiX: FLOAT,
    dpiY: FLOAT,
    bitmapOptions: D2D1_BITMAP_OPTIONS,
    colorContext: ?*ID2D1ColorContext,
};

pub const ID2D1DeviceContext = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        rendertarget: ID2D1RenderTarget.VTable(Self),
        devctx: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1RenderTarget.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateBitmapFromDxgiSurface(
                self: *T,
                surface: *IDXGISurface,
                properties: ?*const D2D1_BITMAP_PROPERTIES1,
                bitmap: *?*ID2D1Bitmap1,
            ) HRESULT {
                return self.v.devctx.CreateBitmapFromDxgiSurface(self, surface, properties, bitmap);
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
                *IDXGISurface,
                ?*const D2D1_BITMAP_PROPERTIES1,
                *?*ID2D1Bitmap1,
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
            SetTarget: fn (*T, ?*ID2D1Image) callconv(WINAPI) void,
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
