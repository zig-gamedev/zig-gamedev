const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");

pub const WICPixelFormatGUID = GUID;

pub const Rect = extern struct {
    X: INT,
    Y: INT,
    Width: INT,
    Height: INT,
};

pub const WICDecodeOptions = enum(UINT) {
    MetadataCacheOnDemand = 0,
    MetadataCacheOnLoad = 0x1,
};

pub const WICBitmapPaletteType = enum(UINT) {
    Custom = 0,
    MedianCut = 0x1,
    FixedBW = 0x2,
    FixedHalftone8 = 0x3,
    FixedHalftone27 = 0x4,
    FixedHalftone64 = 0x5,
    FixedHalftone125 = 0x6,
    FixedHalftone216 = 0x7,
    FixedHalftone252 = 0x8,
    FixedHalftone256 = 0x9,
    FixedGray4 = 0xa,
    FixedGray16 = 0xb,
    FixedGray256 = 0xc,
};

pub const IWICPalette = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        palette: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            InitializePredefined: *c_void,
            InitializeCustom: *c_void,
            InitializeFromBitmap: *c_void,
            InitializeFromPalette: *c_void,
            GetType: *c_void,
            GetColorCount: *c_void,
            GetColors: *c_void,
            IsBlackWhite: *c_void,
            IsGrayscale: *c_void,
            HasAlpha: *c_void,
        };
    }
};

pub const IWICBitmapDecoder = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpdecoder: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetFrame(self: *T, index: UINT, frame: ?*?*IWICBitmapFrameDecode) HRESULT {
                return self.v.bmpdecoder.GetFrame(self, index, frame);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            QueryCapability: *c_void,
            Initialize: *c_void,
            GetContainerFormat: *c_void,
            GetDecoderInfo: *c_void,
            CopyPalette: *c_void,
            GetMetadataQueryReader: *c_void,
            GetPreview: *c_void,
            GetColorContexts: *c_void,
            GetThumbnail: *c_void,
            GetFrameCount: *c_void,
            GetFrame: fn (*T, UINT, ?*?*IWICBitmapFrameDecode) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IWICBitmapSource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpsource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetSize(self: *T, width: *UINT, height: *UINT) HRESULT {
                return self.v.bmpsource.GetSize(self, width, height);
            }
            pub inline fn GetPixelFormat(self: *T, guid: *WICPixelFormatGUID) HRESULT {
                return self.v.bmpsource.GetPixelFormat(self, guid);
            }
            pub inline fn CopyPixels(self: *T, rect: ?*const Rect, stride: UINT, size: UINT, buffer: [*]BYTE) HRESULT {
                return self.v.bmpsource.CopyPixels(self, rect, stride, size, buffer);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetSize: fn (*T, *UINT, *UINT) callconv(WINAPI) HRESULT,
            GetPixelFormat: fn (*T, *GUID) callconv(WINAPI) HRESULT,
            GetResolution: *c_void,
            CopyPalette: *c_void,
            CopyPixels: fn (*T, ?*const Rect, UINT, UINT, [*]BYTE) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IWICBitmapFrameDecode = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpsource: IWICBitmapSource.VTable(Self),
        bmpframedecode: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IWICBitmapSource.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetMetadataQueryReader: *c_void,
            GetColorContexts: *c_void,
            GetThumbnail: *c_void,
        };
    }
};

pub const IWICBitmap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpsource: IWICBitmapSource.VTable(Self),
        bmp: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IWICBitmapSource.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            Lock: *c_void,
            SetPalette: *c_void,
            SetResolution: *c_void,
        };
    }
};

pub const WICBitmapDitherType = enum(UINT) {
    None = 0,
    Ordered4x4 = 0x1,
    Ordered8x8 = 0x2,
    Ordered16x16 = 0x3,
    Spiral4x4 = 0x4,
    Spiral8x8 = 0x5,
    DualSpiral4x4 = 0x6,
    DualSpiral8x8 = 0x7,
    ErrorDiffusion = 0x8,
};

pub const IWICFormatConverter = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpsource: IWICBitmapSource.VTable(Self),
        fmtconv: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IWICBitmapSource.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Initialize(
                self: *T,
                source: ?*IWICBitmapSource,
                dest_format: *const WICPixelFormatGUID,
                dither: WICBitmapDitherType,
                palette: ?*IWICPalette,
                alpha_threshold_percent: f64,
                palette_translate: WICBitmapPaletteType,
            ) HRESULT {
                return self.v.fmtconv.Initialize(
                    self,
                    source,
                    dest_format,
                    dither,
                    palette,
                    alpha_threshold_percent,
                    palette_translate,
                );
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Initialize: fn (
                *T,
                ?*IWICBitmapSource,
                *const WICPixelFormatGUID,
                WICBitmapDitherType,
                ?*IWICPalette,
                f64,
                WICBitmapPaletteType,
            ) callconv(WINAPI) HRESULT,
            CanConvert: *c_void,
        };
    }
};

pub const IWICImagingFactory = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        imgfactory: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateDecoderFromFilename(
                self: *T,
                filename: LPCWSTR,
                vendor: ?*const GUID,
                access: DWORD,
                metadata: WICDecodeOptions,
                decoder: ?*?*IWICBitmapDecoder,
            ) HRESULT {
                return self.v.imgfactory.CreateDecoderFromFilename(self, filename, vendor, access, metadata, decoder);
            }
            pub inline fn CreateFormatConverter(self: *T, converter: ?*?*IWICFormatConverter) HRESULT {
                return self.v.imgfactory.CreateFormatConverter(self, converter);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateDecoderFromFilename: fn (
                *T,
                LPCWSTR,
                ?*const GUID,
                DWORD,
                WICDecodeOptions,
                ?*?*IWICBitmapDecoder,
            ) callconv(WINAPI) HRESULT,
            CreateDecoderFromStream: *c_void,
            CreateDecoderFromFileHandle: *c_void,
            CreateComponentInfo: *c_void,
            CreateDecoder: *c_void,
            CreateEncoder: *c_void,
            CreatePalette: *c_void,
            CreateFormatConverter: fn (*T, ?*?*IWICFormatConverter) callconv(WINAPI) HRESULT,
            CreateBitmapScaler: *c_void,
            CreateBitmapClipper: *c_void,
            CreateBitmapFlipRotator: *c_void,
            CreateStream: *c_void,
            CreateColorContext: *c_void,
            CreateColorTransformer: *c_void,
            CreateBitmap: *c_void,
            CreateBitmapFromSource: *c_void,
            CreateBitmapFromSourceRect: *c_void,
            CreateBitmapFromMemory: *c_void,
            CreateBitmapFromHBITMAP: *c_void,
            CreateBitmapFromHICON: *c_void,
            CreateComponentEnumerator: *c_void,
            CreateFastMetadataEncoderFromDecoder: *c_void,
            CreateFastMetadataEncoderFromFrameDecode: *c_void,
            CreateQueryWriter: *c_void,
            CreateQueryWriterFromReader: *c_void,
        };
    }
};

pub const CLSID_WICImagingFactory = GUID{
    .Data1 = 0xcacaf262,
    .Data2 = 0x9370,
    .Data3 = 0x4615,
    .Data4 = .{ 0xa1, 0x3b, 0x9f, 0x55, 0x39, 0xda, 0x4c, 0xa },
};

pub const IID_IWICImagingFactory = GUID{
    .Data1 = 0xec5ec8a9,
    .Data2 = 0xc395,
    .Data3 = 0x4314,
    .Data4 = .{ 0x9c, 0x77, 0x54, 0xd7, 0xa9, 0x35, 0xff, 0x70 },
};

pub const GUID_WICPixelFormat24bppRGB = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0d },
};
pub const GUID_WICPixelFormat32bppRGB = WICPixelFormatGUID{
    .Data1 = 0xd98c6b95,
    .Data2 = 0x3efe,
    .Data3 = 0x47d6,
    .Data4 = .{ 0xbb, 0x25, 0xeb, 0x17, 0x48, 0xab, 0x0c, 0xf1 },
};
pub const GUID_WICPixelFormat32bppRGBA = WICPixelFormatGUID{
    .Data1 = 0xf5c7ad2d,
    .Data2 = 0x6a8d,
    .Data3 = 0x43dd,
    .Data4 = .{ 0xa7, 0xa8, 0xa2, 0x99, 0x35, 0x26, 0x1a, 0xe9 },
};
pub const GUID_WICPixelFormat32bppPRGBA = WICPixelFormatGUID{
    .Data1 = 0x3cc4a650,
    .Data2 = 0xa527,
    .Data3 = 0x4d37,
    .Data4 = .{ 0xa9, 0x16, 0x31, 0x42, 0xc7, 0xeb, 0xed, 0xba },
};

pub const GUID_WICPixelFormat24bppBGR = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0c },
};
pub const GUID_WICPixelFormat32bppBGR = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0e },
};
pub const GUID_WICPixelFormat32bppBGRA = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0f },
};
pub const GUID_WICPixelFormat32bppPBGRA = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x10 },
};

pub const GUID_WICPixelFormat8bppGray = WICPixelFormatGUID{
    .Data1 = 0x6fddc324,
    .Data2 = 0x4e03,
    .Data3 = 0x4bfe,
    .Data4 = .{ 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x08 },
};
pub const GUID_WICPixelFormat8bppAlpha = WICPixelFormatGUID{
    .Data1 = 0xe6cd0116,
    .Data2 = 0xeeba,
    .Data3 = 0x4161,
    .Data4 = .{ 0xaa, 0x85, 0x27, 0xdd, 0x9f, 0xb3, 0xa8, 0x95 },
};
