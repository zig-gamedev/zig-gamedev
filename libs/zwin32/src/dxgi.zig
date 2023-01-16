const w32 = @import("w32.zig");
const UINT = w32.UINT;
const UINT64 = w32.UINT64;
const DWORD = w32.DWORD;
const FLOAT = w32.FLOAT;
const BOOL = w32.BOOL;
const GUID = w32.GUID;
const WINAPI = w32.WINAPI;
const IUnknown = w32.IUnknown;
const HRESULT = w32.HRESULT;
const WCHAR = w32.WCHAR;
const RECT = w32.RECT;
const INT = w32.INT;
const BYTE = w32.BYTE;
const HMONITOR = w32.HMONITOR;
const LARGE_INTEGER = w32.LARGE_INTEGER;
const HWND = w32.HWND;
const SIZE_T = w32.SIZE_T;
const LUID = w32.LUID;
const HANDLE = w32.HANDLE;
const POINT = w32.POINT;

pub const FORMAT = enum(UINT) {
    UNKNOWN = 0,
    R32G32B32A32_TYPELESS = 1,
    R32G32B32A32_FLOAT = 2,
    R32G32B32A32_UINT = 3,
    R32G32B32A32_SINT = 4,
    R32G32B32_TYPELESS = 5,
    R32G32B32_FLOAT = 6,
    R32G32B32_UINT = 7,
    R32G32B32_SINT = 8,
    R16G16B16A16_TYPELESS = 9,
    R16G16B16A16_FLOAT = 10,
    R16G16B16A16_UNORM = 11,
    R16G16B16A16_UINT = 12,
    R16G16B16A16_SNORM = 13,
    R16G16B16A16_SINT = 14,
    R32G32_TYPELESS = 15,
    R32G32_FLOAT = 16,
    R32G32_UINT = 17,
    R32G32_SINT = 18,
    R32G8X24_TYPELESS = 19,
    D32_FLOAT_S8X24_UINT = 20,
    R32_FLOAT_X8X24_TYPELESS = 21,
    X32_TYPELESS_G8X24_UINT = 22,
    R10G10B10A2_TYPELESS = 23,
    R10G10B10A2_UNORM = 24,
    R10G10B10A2_UINT = 25,
    R11G11B10_FLOAT = 26,
    R8G8B8A8_TYPELESS = 27,
    R8G8B8A8_UNORM = 28,
    R8G8B8A8_UNORM_SRGB = 29,
    R8G8B8A8_UINT = 30,
    R8G8B8A8_SNORM = 31,
    R8G8B8A8_SINT = 32,
    R16G16_TYPELESS = 33,
    R16G16_FLOAT = 34,
    R16G16_UNORM = 35,
    R16G16_UINT = 36,
    R16G16_SNORM = 37,
    R16G16_SINT = 38,
    R32_TYPELESS = 39,
    D32_FLOAT = 40,
    R32_FLOAT = 41,
    R32_UINT = 42,
    R32_SINT = 43,
    R24G8_TYPELESS = 44,
    D24_UNORM_S8_UINT = 45,
    R24_UNORM_X8_TYPELESS = 46,
    X24_TYPELESS_G8_UINT = 47,
    R8G8_TYPELESS = 48,
    R8G8_UNORM = 49,
    R8G8_UINT = 50,
    R8G8_SNORM = 51,
    R8G8_SINT = 52,
    R16_TYPELESS = 53,
    R16_FLOAT = 54,
    D16_UNORM = 55,
    R16_UNORM = 56,
    R16_UINT = 57,
    R16_SNORM = 58,
    R16_SINT = 59,
    R8_TYPELESS = 60,
    R8_UNORM = 61,
    R8_UINT = 62,
    R8_SNORM = 63,
    R8_SINT = 64,
    A8_UNORM = 65,
    R1_UNORM = 66,
    R9G9B9E5_SHAREDEXP = 67,
    R8G8_B8G8_UNORM = 68,
    G8R8_G8B8_UNORM = 69,
    BC1_TYPELESS = 70,
    BC1_UNORM = 71,
    BC1_UNORM_SRGB = 72,
    BC2_TYPELESS = 73,
    BC2_UNORM = 74,
    BC2_UNORM_SRGB = 75,
    BC3_TYPELESS = 76,
    BC3_UNORM = 77,
    BC3_UNORM_SRGB = 78,
    BC4_TYPELESS = 79,
    BC4_UNORM = 80,
    BC4_SNORM = 81,
    BC5_TYPELESS = 82,
    BC5_UNORM = 83,
    BC5_SNORM = 84,
    B5G6R5_UNORM = 85,
    B5G5R5A1_UNORM = 86,
    B8G8R8A8_UNORM = 87,
    B8G8R8X8_UNORM = 88,
    R10G10B10_XR_BIAS_A2_UNORM = 89,
    B8G8R8A8_TYPELESS = 90,
    B8G8R8A8_UNORM_SRGB = 91,
    B8G8R8X8_TYPELESS = 92,
    B8G8R8X8_UNORM_SRGB = 93,
    BC6H_TYPELESS = 94,
    BC6H_UF16 = 95,
    BC6H_SF16 = 96,
    BC7_TYPELESS = 97,
    BC7_UNORM = 98,
    BC7_UNORM_SRGB = 99,
    AYUV = 100,
    Y410 = 101,
    Y416 = 102,
    NV12 = 103,
    P010 = 104,
    P016 = 105,
    @"420_OPAQUE" = 106,
    YUY2 = 107,
    Y210 = 108,
    Y216 = 109,
    NV11 = 110,
    AI44 = 111,
    IA44 = 112,
    P8 = 113,
    A8P8 = 114,
    B4G4R4A4_UNORM = 115,
    P208 = 130,
    V208 = 131,
    V408 = 132,
    SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
    SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,

    pub fn pixelSizeInBytes(format: FORMAT) u32 {
        return switch (format) {
            .R32G32B32A32_TYPELESS,
            .R32G32B32A32_FLOAT,
            .R32G32B32A32_UINT,
            .R32G32B32A32_SINT,
            => 128 / 8,

            .R32G32B32_TYPELESS,
            .R32G32B32_FLOAT,
            .R32G32B32_UINT,
            .R32G32B32_SINT,
            => 96 / 8,

            .R16G16B16A16_TYPELESS,
            .R16G16B16A16_FLOAT,
            .R16G16B16A16_UNORM,
            .R16G16B16A16_UINT,
            .R16G16B16A16_SNORM,
            .R16G16B16A16_SINT,
            .R32G32_TYPELESS,
            .R32G32_FLOAT,
            .R32G32_UINT,
            .R32G32_SINT,
            .R32G8X24_TYPELESS,
            .D32_FLOAT_S8X24_UINT,
            .R32_FLOAT_X8X24_TYPELESS,
            .X32_TYPELESS_G8X24_UINT,
            .Y416,
            .Y210,
            .Y216,
            => 64 / 8,

            .R10G10B10A2_TYPELESS,
            .R10G10B10A2_UNORM,
            .R10G10B10A2_UINT,
            .R11G11B10_FLOAT,
            .R8G8B8A8_TYPELESS,
            .R8G8B8A8_UNORM,
            .R8G8B8A8_UNORM_SRGB,
            .R8G8B8A8_UINT,
            .R8G8B8A8_SNORM,
            .R8G8B8A8_SINT,
            .R16G16_TYPELESS,
            .R16G16_FLOAT,
            .R16G16_UNORM,
            .R16G16_UINT,
            .R16G16_SNORM,
            .R16G16_SINT,
            .R32_TYPELESS,
            .D32_FLOAT,
            .R32_FLOAT,
            .R32_UINT,
            .R32_SINT,
            .R24G8_TYPELESS,
            .D24_UNORM_S8_UINT,
            .R24_UNORM_X8_TYPELESS,
            .X24_TYPELESS_G8_UINT,
            .R9G9B9E5_SHAREDEXP,
            .R8G8_B8G8_UNORM,
            .G8R8_G8B8_UNORM,
            .B8G8R8A8_UNORM,
            .B8G8R8X8_UNORM,
            .R10G10B10_XR_BIAS_A2_UNORM,
            .B8G8R8A8_TYPELESS,
            .B8G8R8A8_UNORM_SRGB,
            .B8G8R8X8_TYPELESS,
            .B8G8R8X8_UNORM_SRGB,
            .AYUV,
            .Y410,
            .YUY2,
            => 32 / 8,

            .P010,
            .P016,
            => 24 / 8,

            .R8G8_TYPELESS,
            .R8G8_UNORM,
            .R8G8_UINT,
            .R8G8_SNORM,
            .R8G8_SINT,
            .R16_TYPELESS,
            .R16_FLOAT,
            .D16_UNORM,
            .R16_UNORM,
            .R16_UINT,
            .R16_SNORM,
            .R16_SINT,
            .B5G6R5_UNORM,
            .B5G5R5A1_UNORM,
            .A8P8,
            .B4G4R4A4_UNORM,
            => 16 / 8,

            .R8_TYPELESS,
            .R8_UNORM,
            .R8_UINT,
            .R8_SNORM,
            .R8_SINT,
            .A8_UNORM,
            .AI44,
            .IA44,
            .P8,
            => 8 / 8,

            .BC2_TYPELESS,
            .BC2_UNORM,
            .BC2_UNORM_SRGB,
            .BC3_TYPELESS,
            .BC3_UNORM,
            .BC3_UNORM_SRGB,
            .BC5_TYPELESS,
            .BC5_UNORM,
            .BC5_SNORM,
            .BC6H_TYPELESS,
            .BC6H_UF16,
            .BC6H_SF16,
            .BC7_TYPELESS,
            .BC7_UNORM,
            .BC7_UNORM_SRGB,
            => 8 / 8,

            .UNKNOWN,
            .R1_UNORM,
            .BC1_TYPELESS,
            .BC1_UNORM,
            .BC1_UNORM_SRGB,
            .BC4_TYPELESS,
            .BC4_UNORM,
            .BC4_SNORM,
            .@"420_OPAQUE",
            .NV11,
            .NV12,
            .P208,
            .V208,
            .V408,
            .SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE,
            .SAMPLER_FEEDBACK_MIN_MIP_OPAQUE,
            => unreachable,
        };
    }
};

pub const RATIONAL = extern struct {
    Numerator: UINT,
    Denominator: UINT,
};

// The following values are used with SAMPLE_DESC::Quality:
pub const STANDARD_MULTISAMPLE_QUALITY_PATTERN = 0xffffffff;
pub const CENTER_MULTISAMPLE_QUALITY_PATTERN = 0xfffffffe;

pub const SAMPLE_DESC = extern struct {
    Count: UINT,
    Quality: UINT,
};

pub const COLOR_SPACE_TYPE = enum(UINT) {
    RGB_FULL_G22_NONE_P709 = 0,
    RGB_FULL_G10_NONE_P709 = 1,
    RGB_STUDIO_G22_NONE_P709 = 2,
    RGB_STUDIO_G22_NONE_P2020 = 3,
    RESERVED = 4,
    YCBCR_FULL_G22_NONE_P709_X601 = 5,
    YCBCR_STUDIO_G22_LEFT_P601 = 6,
    YCBCR_FULL_G22_LEFT_P601 = 7,
    YCBCR_STUDIO_G22_LEFT_P709 = 8,
    YCBCR_FULL_G22_LEFT_P709 = 9,
    YCBCR_STUDIO_G22_LEFT_P2020 = 10,
    YCBCR_FULL_G22_LEFT_P2020 = 11,
    RGB_FULL_G2084_NONE_P2020 = 12,
    YCBCR_STUDIO_G2084_LEFT_P2020 = 13,
    RGB_STUDIO_G2084_NONE_P2020 = 14,
    YCBCR_STUDIO_G22_TOPLEFT_P2020 = 15,
    YCBCR_STUDIO_G2084_TOPLEFT_P2020 = 16,
    RGB_FULL_G22_NONE_P2020 = 17,
    YCBCR_STUDIO_GHLG_TOPLEFT_P2020 = 18,
    YCBCR_FULL_GHLG_TOPLEFT_P2020 = 19,
    RGB_STUDIO_G24_NONE_P709 = 20,
    RGB_STUDIO_G24_NONE_P2020 = 21,
    YCBCR_STUDIO_G24_LEFT_P709 = 22,
    YCBCR_STUDIO_G24_LEFT_P2020 = 23,
    YCBCR_STUDIO_G24_TOPLEFT_P2020 = 24,
    CUSTOM = 0xFFFFFFFF,
};

pub const CPU_ACCESS = enum(UINT) {
    NONE = 0,
    DYNAMIC = 1,
    READ_WRITE = 2,
    SCRATCH = 3,
    FIELD = 15,
};

pub const RGB = extern struct {
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

pub const RGBA = D3DCOLORVALUE;

pub const GAMMA_CONTROL = extern struct {
    Scale: RGB,
    Offset: RGB,
    GammaCurve: [1025]RGB,
};

pub const GAMMA_CONTROL_CAPABILITIES = extern struct {
    ScaleAndOffsetSupported: BOOL,
    MaxConvertedValue: FLOAT,
    MinConvertedValue: FLOAT,
    NumGammaControlPoints: UINT,
    ControlPointPositions: [1025]FLOAT,
};

pub const MODE_SCANLINE_ORDER = enum(UINT) {
    UNSPECIFIED = 0,
    PROGRESSIVE = 1,
    UPPER_FIELD_FIRST = 2,
    LOWER_FIELD_FIRST = 3,
};

pub const MODE_SCALING = enum(UINT) {
    UNSPECIFIED = 0,
    CENTERED = 1,
    STRETCHED = 2,
};

pub const MODE_ROTATION = enum(UINT) {
    UNSPECIFIED = 0,
    IDENTITY = 1,
    ROTATE90 = 2,
    ROTATE180 = 3,
    ROTATE270 = 4,
};

pub const MODE_DESC = extern struct {
    Width: UINT,
    Height: UINT,
    RefreshRate: RATIONAL,
    Format: FORMAT,
    ScanlineOrdering: MODE_SCANLINE_ORDER,
    Scaling: MODE_SCALING,
};

pub const USAGE = packed struct(UINT) {
    __unused0: bool = false,
    __unused1: bool = false,
    __unused2: bool = false,
    __unused3: bool = false,
    SHADER_INPUT: bool = false,
    RENDER_TARGET_OUTPUT: bool = false,
    BACK_BUFFER: bool = false,
    SHARED: bool = false,
    READ_ONLY: bool = false,
    DISCARD_ON_PRESENT: bool = false,
    UNORDERED_ACCESS: bool = false,
    __unused: u21 = 0,
};

pub const FRAME_STATISTICS = extern struct {
    PresentCount: UINT,
    PresentRefreshCount: UINT,
    SyncRefreshCount: UINT,
    SyncQPCTime: LARGE_INTEGER,
    SyncGPUTime: LARGE_INTEGER,
};

pub const MAPPED_RECT = extern struct {
    Pitch: INT,
    pBits: *BYTE,
};

pub const ADAPTER_DESC = extern struct {
    Description: [128]WCHAR,
    VendorId: UINT,
    DeviceId: UINT,
    SubSysId: UINT,
    Revision: UINT,
    DedicatedVideoMemory: SIZE_T,
    DedicatedSystemMemory: SIZE_T,
    SharedSystemMemory: SIZE_T,
    AdapterLuid: LUID,
};

pub const OUTPUT_DESC = extern struct {
    DeviceName: [32]WCHAR,
    DesktopCoordinates: RECT,
    AttachedToDesktop: BOOL,
    Rotation: MODE_ROTATION,
    Monitor: HMONITOR,
};

pub const SHARED_RESOURCE = extern struct {
    Handle: HANDLE,
};

pub const RESOURCE_PRIORITY = enum(UINT) {
    MINIMUM = 0x28000000,
    LOW = 0x50000000,
    NORMAL = 0x78000000,
    HIGH = 0xa0000000,
    MAXIMUM = 0xc8000000,
};

pub const RESIDENCY = enum(UINT) {
    FULLY_RESIDENT = 1,
    RESIDENT_IN_SHARED_MEMORY = 2,
    EVICTED_TO_DISK = 3,
};

pub const SURFACE_DESC = extern struct {
    Width: UINT,
    Height: UINT,
    Format: FORMAT,
    SampleDesc: SAMPLE_DESC,
};

pub const SWAP_EFFECT = enum(UINT) {
    DISCARD = 0,
    SEQUENTIAL = 1,
    FLIP_SEQUENTIAL = 3,
    FLIP_DISCARD = 4,
};

pub const SWAP_CHAIN_FLAG = packed struct(UINT) {
    NONPREROTATED: bool = false,
    ALLOW_MODE_SWITCH: bool = false,
    GDI_COMPATIBLE: bool = false,
    RESTRICTED_CONTENT: bool = false,
    RESTRICT_SHARED_RESOURCE_DRIVER: bool = false,
    DISPLAY_ONLY: bool = false,
    FRAME_LATENCY_WAITABLE_OBJECT: bool = false,
    FOREGROUND_LAYER: bool = false,
    FULLSCREEN_VIDEO: bool = false,
    YUV_VIDEO: bool = false,
    HW_PROTECTED: bool = false,
    ALLOW_TEARING: bool = false,
    RESTRICTED_TO_ALL_HOLOGRAPHIC_DISPLAYS: bool = false,
    __unused: u19 = 0,
};

pub const SWAP_CHAIN_DESC = extern struct {
    BufferDesc: MODE_DESC,
    SampleDesc: SAMPLE_DESC,
    BufferUsage: USAGE,
    BufferCount: UINT,
    OutputWindow: HWND,
    Windowed: BOOL,
    SwapEffect: SWAP_EFFECT,
    Flags: SWAP_CHAIN_FLAG,
};

pub const IObject = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn SetPrivateData(
                self: *T,
                guid: *const GUID,
                data_size: UINT,
                data: *const anyopaque,
            ) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .SetPrivateData(@ptrCast(*IObject, self), guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const IUnknown) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .SetPrivateDataInterface(@ptrCast(*IObject, self), guid, data);
            }
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: *anyopaque) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .GetPrivateData(@ptrCast(*IObject, self), guid, data_size, data);
            }
            pub inline fn GetParent(self: *T, guid: *const GUID, parent: *?*anyopaque) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v).GetParent(@ptrCast(*IObject, self), guid, parent);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IObject;
        base: IUnknown.VTable,
        SetPrivateData: *const fn (*T, *const GUID, UINT, *const anyopaque) callconv(WINAPI) HRESULT,
        SetPrivateDataInterface: *const fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
        GetPrivateData: *const fn (*T, *const GUID, *UINT, *anyopaque) callconv(WINAPI) HRESULT,
        GetParent: *const fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
    };
};

pub const IDeviceSubObject = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn GetDevice(self: *T, guid: *const GUID, parent: *?*anyopaque) HRESULT {
                return @ptrCast(*const IDeviceSubObject.VTable, self.__v)
                    .GetDevice(@ptrCast(*IDeviceSubObject, self), guid, parent);
            }
        };
    }

    pub const VTable = extern struct {
        base: IObject.VTable,
        GetDevice: *const fn (*IDeviceSubObject, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
    };
};

pub const IResource = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceSubObject.Methods(T);

            pub inline fn GetSharedHandle(self: *T, handle: *HANDLE) HRESULT {
                return @ptrCast(*const IResource.VTable, self.__v)
                    .GetSharedHandle(@ptrCast(*IResource, self), handle);
            }
            pub inline fn GetUsage(self: *T, usage: *USAGE) HRESULT {
                return @ptrCast(*const IResource.VTable, self.__v).GetUsage(@ptrCast(*IResource, self), usage);
            }
            pub inline fn SetEvictionPriority(self: *T, priority: UINT) HRESULT {
                return @ptrCast(*const IResource.VTable, self.__v)
                    .SetEvictionPriority(@ptrCast(*IResource, self), priority);
            }
            pub inline fn GetEvictionPriority(self: *T, priority: *UINT) HRESULT {
                return @ptrCast(*const IResource.VTable, self.__v)
                    .GetEvictionPriority(@ptrCast(*IResource, self), priority);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IResource;
        base: IDeviceSubObject.VTable,
        GetSharedHandle: *const fn (*T, *HANDLE) callconv(WINAPI) HRESULT,
        GetUsage: *const fn (*T, *USAGE) callconv(WINAPI) HRESULT,
        SetEvictionPriority: *const fn (*T, UINT) callconv(WINAPI) HRESULT,
        GetEvictionPriority: *const fn (*T, *UINT) callconv(WINAPI) HRESULT,
    };
};

pub const IKeyedMutex = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceSubObject.Methods(T);

            pub inline fn AcquireSync(self: *T, key: UINT64, milliseconds: DWORD) HRESULT {
                return @ptrCast(*const IKeyedMutex.VTable, self.__v)
                    .AcquireSync(@ptrCast(*IKeyedMutex, self), key, milliseconds);
            }
            pub inline fn ReleaseSync(self: *T, key: UINT64) HRESULT {
                return @ptrCast(*const IKeyedMutex.VTable, self.__v).ReleaseSync(@ptrCast(*IKeyedMutex, self), key);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDeviceSubObject.VTable,
        AcquireSync: *const fn (*IKeyedMutex, UINT64, DWORD) callconv(WINAPI) HRESULT,
        ReleaseSync: *const fn (*IKeyedMutex, UINT64) callconv(WINAPI) HRESULT,
    };
};

pub const MAP_FLAG = packed struct(UINT) {
    READ: bool = false,
    WRITE: bool = false,
    DISCARD: bool = false,
    __unused: u29 = 0,
};

pub const ISurface = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceSubObject.Methods(T);

            pub inline fn GetDesc(self: *T, desc: *SURFACE_DESC) HRESULT {
                return @ptrCast(*const ISurface.VTable, self.__v).GetDesc(@ptrCast(*ISurface, self), desc);
            }
            pub inline fn Map(self: *T, locked_rect: *MAPPED_RECT, flags: MAP_FLAG) HRESULT {
                return @ptrCast(*const ISurface.VTable, self.__v).Map(@ptrCast(*ISurface, self), locked_rect, flags);
            }
            pub inline fn Unmap(self: *T) HRESULT {
                return @ptrCast(*const ISurface.VTable, self.__v).Unmap(@ptrCast(*ISurface, self));
            }
        };
    }

    pub const VTable = extern struct {
        base: IDeviceSubObject.VTable,
        GetDesc: *const fn (*ISurface, *SURFACE_DESC) callconv(WINAPI) HRESULT,
        Map: *const fn (*ISurface, *MAPPED_RECT, MAP_FLAG) callconv(WINAPI) HRESULT,
        Unmap: *const fn (*ISurface) callconv(WINAPI) HRESULT,
    };
};

pub const IAdapter = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn EnumOutputs(self: *T, index: UINT, output: *?*IOutput) HRESULT {
                return @ptrCast(*const IAdapter.VTable, self.__v)
                    .EnumOutputs(@ptrCast(*IAdapter, self), index, output);
            }
            pub inline fn GetDesc(self: *T, desc: *ADAPTER_DESC) HRESULT {
                return @ptrCast(*const IAdapter.VTable, self.__v).GetDesc(@ptrCast(*IAdapter, self), desc);
            }
            pub inline fn CheckInterfaceSupport(self: *T, guid: *const GUID, umd_ver: *LARGE_INTEGER) HRESULT {
                return @ptrCast(*const IAdapter.VTable, self.__v)
                    .CheckInterfaceSupport(@ptrCast(*IAdapter, self), guid, umd_ver);
            }
        };
    }

    pub const VTable = extern struct {
        base: IObject.VTable,
        EnumOutputs: *const fn (*IAdapter, UINT, *?*IOutput) callconv(WINAPI) HRESULT,
        GetDesc: *const fn (*IAdapter, *ADAPTER_DESC) callconv(WINAPI) HRESULT,
        CheckInterfaceSupport: *const fn (*IAdapter, *const GUID, *LARGE_INTEGER) callconv(WINAPI) HRESULT,
    };
};

pub const ENUM_MODES = packed struct(UINT) {
    INTERLACED: bool = false,
    SCALING: bool = false,
    STEREO: bool = false,
    DISABLED_STEREO: bool = false,
    __unused: u28 = 0,
};

pub const IOutput = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn GetDesc(self: *T, desc: *OUTPUT_DESC) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).GetDesc(@ptrCast(*IOutput, self), desc);
            }
            pub inline fn GetDisplayModeList(
                self: *T,
                enum_format: FORMAT,
                flags: ENUM_MODES,
                num_nodes: *UINT,
                desc: ?*MODE_DESC,
            ) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v)
                    .GetDisplayModeList(@ptrCast(*IOutput, self), enum_format, flags, num_nodes, desc);
            }
            pub inline fn FindClosestMatchingMode(
                self: *T,
                mode_to_match: *const MODE_DESC,
                closest_match: *MODE_DESC,
                concerned_device: ?*IUnknown,
            ) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).FindClosestMatchingMode(
                    @ptrCast(*IOutput, self),
                    mode_to_match,
                    closest_match,
                    concerned_device,
                );
            }
            pub inline fn WaitForVBlank(self: *T) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).WaitForVBlank(@ptrCast(*IOutput, self));
            }
            pub inline fn TakeOwnership(self: *T, device: *IUnknown, exclusive: BOOL) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v)
                    .TakeOwnership(@ptrCast(*IOutput, self), device, exclusive);
            }
            pub inline fn ReleaseOwnership(self: *T) void {
                @ptrCast(*const IOutput.VTable, self.__v).ReleaseOwnership(@ptrCast(*IOutput, self));
            }
            pub inline fn GetGammaControlCapabilities(self: *T, gamma_caps: *GAMMA_CONTROL_CAPABILITIES) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v)
                    .GetGammaControlCapabilities(@ptrCast(*IOutput, self), gamma_caps);
            }
            pub inline fn SetGammaControl(self: *T, array: *const GAMMA_CONTROL) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).SetGammaControl(@ptrCast(*IOutput, self), array);
            }
            pub inline fn GetGammaControl(self: *T, array: *GAMMA_CONTROL) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).GetGammaControl(@ptrCast(*IOutput, self), array);
            }
            pub inline fn SetDisplaySurface(self: *T, scanout_surface: *ISurface) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v)
                    .SetDisplaySurface(@ptrCast(*IOutput, self), scanout_surface);
            }
            pub inline fn GetDisplaySurfaceData(self: *T, destination: *ISurface) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v)
                    .GetDisplaySurfaceData(@ptrCast(*IOutput, self), destination);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *FRAME_STATISTICS) HRESULT {
                return @ptrCast(*const IOutput.VTable, self.__v).GetFrameStatistics(@ptrCast(*IOutput, self), stats);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IOutput;
        base: IObject.VTable,
        GetDesc: *const fn (self: *T, desc: *OUTPUT_DESC) callconv(WINAPI) HRESULT,
        GetDisplayModeList: *const fn (*T, FORMAT, ENUM_MODES, *UINT, ?*MODE_DESC) callconv(WINAPI) HRESULT,
        FindClosestMatchingMode: *const fn (
            *T,
            *const MODE_DESC,
            *MODE_DESC,
            ?*IUnknown,
        ) callconv(WINAPI) HRESULT,
        WaitForVBlank: *const fn (*T) callconv(WINAPI) HRESULT,
        TakeOwnership: *const fn (*T, *IUnknown, BOOL) callconv(WINAPI) HRESULT,
        ReleaseOwnership: *const fn (*T) callconv(WINAPI) void,
        GetGammaControlCapabilities: *const fn (*T, *GAMMA_CONTROL_CAPABILITIES) callconv(WINAPI) HRESULT,
        SetGammaControl: *const fn (*T, *const GAMMA_CONTROL) callconv(WINAPI) HRESULT,
        GetGammaControl: *const fn (*T, *GAMMA_CONTROL) callconv(WINAPI) HRESULT,
        SetDisplaySurface: *const fn (*T, *ISurface) callconv(WINAPI) HRESULT,
        GetDisplaySurfaceData: *const fn (*T, *ISurface) callconv(WINAPI) HRESULT,
        GetFrameStatistics: *const fn (*T, *FRAME_STATISTICS) callconv(WINAPI) HRESULT,
    };
};

pub const MAX_SWAP_CHAIN_BUFFERS = 16;

pub const PRESENT_FLAG = packed struct(UINT) {
    TEST: bool = false,
    DO_NOT_SEQUENCE: bool = false,
    RESTART: bool = false,
    DO_NOT_WAIT: bool = false,
    STEREO_PREFER_RIGHT: bool = false,
    STEREO_TEMPORARY_MONO: bool = false,
    RESTRICT_TO_OUTPUT: bool = false,
    __unused7: bool = false,
    USE_DURATION: bool = false,
    ALLOW_TEARING: bool = false,
    __unused: u22 = 0,
};

pub const ISwapChain = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceSubObject.Methods(T);

            pub inline fn Present(self: *T, sync_interval: UINT, flags: PRESENT_FLAG) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .Present(@ptrCast(*ISwapChain, self), sync_interval, flags);
            }
            pub inline fn GetBuffer(self: *T, index: u32, guid: *const GUID, surface: *?*anyopaque) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .GetBuffer(@ptrCast(*ISwapChain, self), index, guid, surface);
            }
            pub inline fn SetFullscreenState(self: *T, target: ?*IOutput) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .SetFullscreenState(@ptrCast(*ISwapChain, self), target);
            }
            pub inline fn GetFullscreenState(self: *T, fullscreen: ?*BOOL, target: ?*?*IOutput) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .GetFullscreenState(@ptrCast(*ISwapChain, self), fullscreen, target);
            }
            pub inline fn GetDesc(self: *T, desc: *SWAP_CHAIN_DESC) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v).GetDesc(@ptrCast(*ISwapChain, self), desc);
            }
            pub inline fn ResizeBuffers(
                self: *T,
                count: UINT,
                width: UINT,
                height: UINT,
                format: FORMAT,
                flags: SWAP_CHAIN_FLAG,
            ) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .ResizeBuffers(@ptrCast(*ISwapChain, self), count, width, height, format, flags);
            }
            pub inline fn ResizeTarget(self: *T, params: *const MODE_DESC) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v).ResizeTarget(@ptrCast(*ISwapChain, self), params);
            }
            pub inline fn GetContainingOutput(self: *T, output: *?*IOutput) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .GetContainingOutput(@ptrCast(*ISwapChain, self), output);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *FRAME_STATISTICS) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .GetFrameStatistics(@ptrCast(*ISwapChain, self), stats);
            }
            pub inline fn GetLastPresentCount(self: *T, count: *UINT) HRESULT {
                return @ptrCast(*const ISwapChain.VTable, self.__v)
                    .GetLastPresentCount(@ptrCast(*ISwapChain, self), count);
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISwapChain;
        base: IDeviceSubObject.VTable,
        Present: *const fn (*T, UINT, PRESENT_FLAG) callconv(WINAPI) HRESULT,
        GetBuffer: *const fn (*T, u32, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        SetFullscreenState: *const fn (*T, ?*IOutput) callconv(WINAPI) HRESULT,
        GetFullscreenState: *const fn (*T, ?*BOOL, ?*?*IOutput) callconv(WINAPI) HRESULT,
        GetDesc: *const fn (*T, *SWAP_CHAIN_DESC) callconv(WINAPI) HRESULT,
        ResizeBuffers: *const fn (*T, UINT, UINT, UINT, FORMAT, SWAP_CHAIN_FLAG) callconv(WINAPI) HRESULT,
        ResizeTarget: *const fn (*T, *const MODE_DESC) callconv(WINAPI) HRESULT,
        GetContainingOutput: *const fn (*T, *?*IOutput) callconv(WINAPI) HRESULT,
        GetFrameStatistics: *const fn (*T, *FRAME_STATISTICS) callconv(WINAPI) HRESULT,
        GetLastPresentCount: *const fn (*T, *UINT) callconv(WINAPI) HRESULT,
    };
};

pub const IFactory = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn EnumAdapters(self: *T, index: UINT, adapter: *?*IAdapter) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .EnumAdapters(@ptrCast(*IFactory, self), index, adapter);
            }
            pub inline fn MakeWindowAssociation(self: *T, window: HWND, flags: UINT) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .MakeWindowAssociation(@ptrCast(*IFactory, self), window, flags);
            }
            pub inline fn GetWindowAssociation(self: *T, window: *HWND) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .GetWindowAssociation(@ptrCast(*IFactory, self), window);
            }
            pub inline fn CreateSwapChain(
                self: *T,
                device: *IUnknown,
                desc: *SWAP_CHAIN_DESC,
                swapchain: *?*ISwapChain,
            ) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateSwapChain(@ptrCast(*IFactory, self), device, desc, swapchain);
            }
            pub inline fn CreateSoftwareAdapter(self: *T, adapter: *?*IAdapter) HRESULT {
                return @ptrCast(*const IFactory.VTable, self.__v)
                    .CreateSoftwareAdapter(@ptrCast(*IFactory, self), adapter);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IFactory;
        base: IObject.VTable,
        EnumAdapters: *const fn (*T, UINT, *?*IAdapter) callconv(WINAPI) HRESULT,
        MakeWindowAssociation: *const fn (*T, HWND, UINT) callconv(WINAPI) HRESULT,
        GetWindowAssociation: *const fn (*T, *HWND) callconv(WINAPI) HRESULT,
        CreateSwapChain: *const fn (*T, *IUnknown, *SWAP_CHAIN_DESC, *?*ISwapChain) callconv(WINAPI) HRESULT,
        CreateSoftwareAdapter: *const fn (*T, *?*IAdapter) callconv(WINAPI) HRESULT,
    };
};

pub const IDevice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn GetAdapter(self: *T, adapter: *?*IAdapter) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v).GetAdapter(@ptrCast(*IDevice, self), adapter);
            }
            pub inline fn CreateSurface(
                self: *T,
                desc: *const SURFACE_DESC,
                num_surfaces: UINT,
                usage: USAGE,
                shared_resource: ?*const SHARED_RESOURCE,
                surface: *?*ISurface,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v).CreateSurface(
                    @ptrCast(*IDevice, self),
                    desc,
                    num_surfaces,
                    usage,
                    shared_resource,
                    surface,
                );
            }
            pub inline fn QueryResourceResidency(
                self: *T,
                resources: *const *IUnknown,
                status: [*]RESIDENCY,
                num_resources: UINT,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .QueryResourceResidency(@ptrCast(*IDevice, self), resources, status, num_resources);
            }
            pub inline fn SetGPUThreadPriority(self: *T, priority: INT) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .SetGPUThreadPriority(@ptrCast(*IDevice, self), priority);
            }
            pub inline fn GetGPUThreadPriority(self: *T, priority: *INT) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .GetGPUThreadPriority(@ptrCast(*IDevice, self), priority);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IDevice;
        base: IObject.VTable,
        GetAdapter: *const fn (self: *T, adapter: *?*IAdapter) callconv(WINAPI) HRESULT,
        CreateSurface: *const fn (
            *T,
            *const SURFACE_DESC,
            UINT,
            USAGE,
            ?*const SHARED_RESOURCE,
            *?*ISurface,
        ) callconv(WINAPI) HRESULT,
        QueryResourceResidency: *const fn (
            *T,
            *const *IUnknown,
            [*]RESIDENCY,
            UINT,
        ) callconv(WINAPI) HRESULT,
        SetGPUThreadPriority: *const fn (self: *T, priority: INT) callconv(WINAPI) HRESULT,
        GetGPUThreadPriority: *const fn (self: *T, priority: *INT) callconv(WINAPI) HRESULT,
    };
};

pub const ADAPTER_FLAGS = packed struct(UINT) {
    REMOTE: bool = false,
    SOFTWARE: bool = false,
    __unused: u30 = 0,
};

pub const ADAPTER_DESC1 = extern struct {
    Description: [128]WCHAR,
    VendorId: UINT,
    DeviceId: UINT,
    SubSysId: UINT,
    Revision: UINT,
    DedicatedVideoMemory: SIZE_T,
    DedicatedSystemMemory: SIZE_T,
    SharedSystemMemory: SIZE_T,
    AdapterLuid: LUID,
    Flags: ADAPTER_FLAGS,
};

pub const IFactory1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory.Methods(T);

            pub inline fn EnumAdapters1(self: *T, index: UINT, adapter: *?*IAdapter1) HRESULT {
                return @ptrCast(*const IFactory1.VTable, self.__v)
                    .EnumAdapters1(@ptrCast(*IFactory1, self), index, adapter);
            }
            pub inline fn IsCurrent(self: *T) BOOL {
                return @ptrCast(*const IFactory1.VTable, self.__v)
                    .IsCurrent(@ptrCast(*IFactory1, self));
            }
        };
    }

    pub const VTable = extern struct {
        base: IFactory.VTable,
        EnumAdapters1: *const fn (*IFactory1, UINT, *?*IAdapter1) callconv(WINAPI) HRESULT,
        IsCurrent: *const fn (*IFactory1) callconv(WINAPI) BOOL,
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
        IsWindowedStereoEnabled: *anyopaque,
        CreateSwapChainForHwnd: *anyopaque,
        CreateSwapChainForCoreWindow: *anyopaque,
        GetSharedResourceAdapterLuid: *anyopaque,
        RegisterStereoStatusWindow: *anyopaque,
        RegisterStereoStatusEvent: *anyopaque,
        UnregisterStereoStatus: *anyopaque,
        RegisterOcclusionStatusWindow: *anyopaque,
        RegisterOcclusionStatusEvent: *anyopaque,
        UnregisterOcclusionStatus: *anyopaque,
        CreateSwapChainForComposition: *anyopaque,
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
        GetCreationFlags: *anyopaque,
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
        EnumAdapterByLuid: *anyopaque,
        EnumWarpAdapter: *anyopaque,
    };
};

pub const FEATURE = enum(UINT) {
    PRESENT_ALLOW_TEARING = 0,
};

pub const IID_IFactory5 = GUID.parse("{7632e1f5-ee65-4dca-87fd-84cd75f8838d}");
pub const IFactory5 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory4.Methods(T);

            pub inline fn CheckFeatureSupport(
                self: *T,
                feature: FEATURE,
                support_data: *anyopaque,
                support_data_size: UINT,
            ) HRESULT {
                return @ptrCast(*const IFactory5.VTable, self.__v).CheckFeatureSupport(
                    @ptrCast(*IFactory5, self),
                    feature,
                    support_data,
                    support_data_size,
                );
            }
        };
    }

    pub const VTable = extern struct {
        base: IFactory4.VTable,
        CheckFeatureSupport: *const fn (*IFactory5, FEATURE, *anyopaque, UINT) callconv(WINAPI) HRESULT,
    };
};

pub const GPU_PREFERENCE = enum(UINT) {
    UNSPECIFIED,
    MINIMUM,
    HIGH_PERFORMANCE,
};

pub const IID_IFactory6 = GUID.parse("{c1b6694f-ff09-44a9-b03c-77900a0a1d17}");
pub const IFactory6 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IFactory5.Methods(T);

            pub inline fn EnumAdapterByGpuPreference(
                self: *T,
                adapter_index: UINT,
                gpu_preference: GPU_PREFERENCE,
                riid: *const GUID,
                adapter: *?*IAdapter1,
            ) HRESULT {
                return @ptrCast(*const IFactory6.VTable, self.__v).EnumAdapterByGpuPreference(
                    @ptrCast(*IFactory6, self),
                    adapter_index,
                    gpu_preference,
                    riid,
                    adapter,
                );
            }
        };
    }

    pub const VTable = extern struct {
        base: IFactory5.VTable,
        EnumAdapterByGpuPreference: *const fn (
            *IFactory6,
            UINT,
            GPU_PREFERENCE,
            *const GUID,
            *?*IAdapter1,
        ) callconv(WINAPI) HRESULT,
    };
};

pub const IID_IAdapter1 = GUID.parse("{29038f61-3839-4626-91fd-086879011a05}");
pub const IAdapter1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IAdapter.Methods(T);

            pub inline fn GetDesc1(self: *T, desc: *ADAPTER_DESC1) HRESULT {
                return @ptrCast(*const IAdapter1.VTable, self.__v)
                    .GetDesc1(@ptrCast(*IAdapter1, self), desc);
            }
        };
    }

    pub const VTable = extern struct {
        base: IAdapter.VTable,
        GetDesc1: *const fn (*IAdapter1, *ADAPTER_DESC1) callconv(WINAPI) HRESULT,
    };
};

pub const IDevice1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice.Methods(T);

            pub inline fn SetMaximumFrameLatency(self: *T, max_latency: UINT) HRESULT {
                return @ptrCast(*const IDevice1.VTable, self.__v)
                    .SetMaximumFrameLatency(@ptrCast(*IDevice1, self), max_latency);
            }
            pub inline fn GetMaximumFrameLatency(self: *T, max_latency: *UINT) HRESULT {
                return @ptrCast(*const IDevice1.VTable, self.__v)
                    .GetMaximumFrameLatency(@ptrCast(*IDevice1, self), max_latency);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDevice.VTable,
        SetMaximumFrameLatency: *const fn (self: *IDevice1, max_latency: UINT) callconv(WINAPI) HRESULT,
        GetMaximumFrameLatency: *const fn (self: *IDevice1, max_latency: *UINT) callconv(WINAPI) HRESULT,
    };
};

pub const IID_IFactory1 = GUID{
    .Data1 = 0x770aae78,
    .Data2 = 0xf26f,
    .Data3 = 0x4dba,
    .Data4 = .{ 0xa8, 0x29, 0x25, 0x3c, 0x83, 0xd1, 0xb3, 0x87 },
};

pub const IID_IDevice = GUID{
    .Data1 = 0x54ec77fa,
    .Data2 = 0x1377,
    .Data3 = 0x44e6,
    .Data4 = .{ 0x8c, 0x32, 0x88, 0xfd, 0x5f, 0x44, 0xc8, 0x4c },
};

pub const IID_ISurface = GUID{
    .Data1 = 0xcafcb56c,
    .Data2 = 0x6ac3,
    .Data3 = 0x4889,
    .Data4 = .{ 0xbf, 0x47, 0x9e, 0x23, 0xbb, 0xd2, 0x60, 0xec },
};

pub const CREATE_FACTORY_DEBUG = 0x1;
pub extern "dxgi" fn CreateDXGIFactory2(UINT, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT;

pub const SCALING = enum(UINT) {
    STRETCH = 0,
    NONE = 1,
    ASPECT_RATIO_STRETCH = 2,
};

pub const ALPHA_MODE = enum(UINT) {
    UNSPECIFIED = 0,
    PREMULTIPLIED = 1,
    STRAIGHT = 2,
    IGNORE = 3,
};

pub const SWAP_CHAIN_DESC1 = extern struct {
    Width: UINT,
    Height: UINT,
    Format: FORMAT,
    Stereo: BOOL,
    SampleDesc: SAMPLE_DESC,
    BufferUsage: USAGE,
    BufferCount: UINT,
    Scaling: SCALING,
    SwapEffect: SWAP_EFFECT,
    AlphaMode: ALPHA_MODE,
    Flags: SWAP_CHAIN_FLAG,
};

pub const SWAP_CHAIN_FULLSCREEN_DESC = extern struct {
    RefreshRate: RATIONAL,
    ScanlineOrdering: MODE_SCANLINE_ORDER,
    Scaling: MODE_SCALING,
    Windowed: BOOL,
};

pub const PRESENT_PARAMETERS = extern struct {
    DirtyRectsCount: UINT,
    pDirtyRects: ?*RECT,
    pScrollRect: *RECT,
    pScrollOffset: *POINT,
};

pub const ISwapChain1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace ISwapChain.Methods(T);

            pub inline fn GetDesc1(self: *T, desc: *SWAP_CHAIN_DESC1) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v).GetDesc1(@ptrCast(*ISwapChain1, self), desc);
            }
            pub inline fn GetFullscreenDesc(self: *T, desc: *SWAP_CHAIN_FULLSCREEN_DESC) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .GetFullscreenDesc(@ptrCast(*ISwapChain1, self), desc);
            }
            pub inline fn GetHwnd(self: *T, hwnd: *HWND) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v).GetHwnd(@ptrCast(*ISwapChain1, self), hwnd);
            }
            pub inline fn GetCoreWindow(self: *T, guid: *const GUID, unknown: *?*anyopaque) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .GetCoreWindow(@ptrCast(*ISwapChain1, self), guid, unknown);
            }
            pub inline fn Present1(
                self: *T,
                sync_interval: UINT,
                flags: PRESENT_FLAG,
                params: *const PRESENT_PARAMETERS,
            ) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .Present1(@ptrCast(*ISwapChain1, self), sync_interval, flags, params);
            }
            pub inline fn IsTemporaryMonoSupported(self: *T) BOOL {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .IsTemporaryMonoSupported(@ptrCast(*ISwapChain1, self));
            }
            pub inline fn GetRestrictToOutput(self: *T, output: *?*IOutput) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .GetRestrictToOutput(@ptrCast(*ISwapChain1, self), output);
            }
            pub inline fn SetBackgroundColor(self: *T, color: *const RGBA) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .SetBackgroundColor(@ptrCast(*ISwapChain1, self), color);
            }
            pub inline fn GetBackgroundColor(self: *T, color: *RGBA) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .GetBackgroundColor(@ptrCast(*ISwapChain1, self), color);
            }
            pub inline fn SetRotation(self: *T, rotation: MODE_ROTATION) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .SetRotation(@ptrCast(*ISwapChain1, self), rotation);
            }
            pub inline fn GetRotation(self: *T, rotation: *MODE_ROTATION) HRESULT {
                return @ptrCast(*const ISwapChain1.VTable, self.__v)
                    .GetRotation(@ptrCast(*ISwapChain1, self), rotation);
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISwapChain1;
        base: ISwapChain.VTable,
        GetDesc1: *const fn (*T, *SWAP_CHAIN_DESC1) callconv(WINAPI) HRESULT,
        GetFullscreenDesc: *const fn (*T, *SWAP_CHAIN_FULLSCREEN_DESC) callconv(WINAPI) HRESULT,
        GetHwnd: *const fn (*T, *HWND) callconv(WINAPI) HRESULT,
        GetCoreWindow: *const fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        Present1: *const fn (*T, UINT, PRESENT_FLAG, *const PRESENT_PARAMETERS) callconv(WINAPI) HRESULT,
        IsTemporaryMonoSupported: *const fn (*T) callconv(WINAPI) BOOL,
        GetRestrictToOutput: *const fn (*T, *?*IOutput) callconv(WINAPI) HRESULT,
        SetBackgroundColor: *const fn (*T, *const RGBA) callconv(WINAPI) HRESULT,
        GetBackgroundColor: *const fn (*T, *RGBA) callconv(WINAPI) HRESULT,
        SetRotation: *const fn (*T, MODE_ROTATION) callconv(WINAPI) HRESULT,
        GetRotation: *const fn (*T, *MODE_ROTATION) callconv(WINAPI) HRESULT,
    };
};

pub const IID_ISwapChain1 = GUID{
    .Data1 = 0x790a45f7,
    .Data2 = 0x0d41,
    .Data3 = 0x4876,
    .Data4 = .{ 0x98, 0x3a, 0x0a, 0x55, 0xcf, 0xe6, 0xf4, 0xaa },
};

pub const MATRIX_3X2_F = extern struct {
    _11: FLOAT,
    _12: FLOAT,
    _21: FLOAT,
    _22: FLOAT,
    _31: FLOAT,
    _32: FLOAT,
};

pub const ISwapChain2 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace ISwapChain1.Methods(T);

            pub inline fn SetSourceSize(self: *T, width: UINT, height: UINT) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .SetSourceSize(@ptrCast(*ISwapChain2, self), width, height);
            }
            pub inline fn GetSourceSize(self: *T, width: *UINT, height: *UINT) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .GetSourceSize(@ptrCast(*ISwapChain2, self), width, height);
            }
            pub inline fn SetMaximumFrameLatency(self: *T, max_latency: UINT) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .SetMaximumFrameLatency(@ptrCast(*ISwapChain2, self), max_latency);
            }
            pub inline fn GetMaximumFrameLatency(self: *T, max_latency: *UINT) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .GetMaximumFrameLatency(@ptrCast(*ISwapChain2, self), max_latency);
            }
            pub inline fn GetFrameLatencyWaitableObject(self: *T) HANDLE {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .GetFrameLatencyWaitableObject(@ptrCast(*ISwapChain2, self));
            }
            pub inline fn SetMatrixTransform(self: *T, matrix: *const MATRIX_3X2_F) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .SetMatrixTransform(@ptrCast(*ISwapChain2, self), matrix);
            }
            pub inline fn GetMatrixTransform(self: *T, matrix: *MATRIX_3X2_F) HRESULT {
                return @ptrCast(*const ISwapChain2.VTable, self.__v)
                    .GetMatrixTransform(@ptrCast(*ISwapChain2, self), matrix);
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISwapChain2;
        base: ISwapChain1.VTable,
        SetSourceSize: *const fn (*T, UINT, UINT) callconv(WINAPI) HRESULT,
        GetSourceSize: *const fn (*T, *UINT, *UINT) callconv(WINAPI) HRESULT,
        SetMaximumFrameLatency: *const fn (*T, UINT) callconv(WINAPI) HRESULT,
        GetMaximumFrameLatency: *const fn (*T, *UINT) callconv(WINAPI) HRESULT,
        GetFrameLatencyWaitableObject: *const fn (*T) callconv(WINAPI) HANDLE,
        SetMatrixTransform: *const fn (*T, *const MATRIX_3X2_F) callconv(WINAPI) HRESULT,
        GetMatrixTransform: *const fn (*T, *MATRIX_3X2_F) callconv(WINAPI) HRESULT,
    };
};

pub const IID_ISwapChain2 = GUID{
    .Data1 = 0xa8be2ac4,
    .Data2 = 0x199f,
    .Data3 = 0x4946,
    .Data4 = .{ 0xb3, 0x31, 0x79, 0x59, 0x9f, 0xb9, 0x8d, 0xe7 },
};

pub const ISwapChain3 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace ISwapChain2.Methods(T);

            pub inline fn GetCurrentBackBufferIndex(self: *T) UINT {
                return @ptrCast(*const ISwapChain3.VTable, self.__v)
                    .GetCurrentBackBufferIndex(@ptrCast(*ISwapChain3, self));
            }
            pub inline fn CheckColorSpaceSupport(self: *T, space: COLOR_SPACE_TYPE, support: *UINT) HRESULT {
                return @ptrCast(*const ISwapChain3.VTable, self.__v)
                    .CheckColorSpaceSupport(@ptrCast(*const ISwapChain3.VTable, self.__v), space, support);
            }
            pub inline fn SetColorSpace1(self: *T, space: COLOR_SPACE_TYPE) HRESULT {
                return @ptrCast(*const ISwapChain3.VTable, self.__v)
                    .SetColorSpace1(@ptrCast(*const ISwapChain3.VTable, self.__v), space);
            }
            pub inline fn ResizeBuffers1(
                self: *T,
                buffer_count: UINT,
                width: UINT,
                height: UINT,
                format: FORMAT,
                swap_chain_flags: UINT,
                creation_node_mask: [*]const UINT,
                present_queue: [*]const *IUnknown,
            ) HRESULT {
                return @ptrCast(*const ISwapChain3.VTable, self.__v).ResizeBuffers1(
                    @ptrCast(*const ISwapChain3.VTable, self.__v),
                    buffer_count,
                    width,
                    height,
                    format,
                    swap_chain_flags,
                    creation_node_mask,
                    present_queue,
                );
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISwapChain3;
        base: ISwapChain2.VTable,
        GetCurrentBackBufferIndex: *const fn (*T) callconv(WINAPI) UINT,
        CheckColorSpaceSupport: *const fn (*T, COLOR_SPACE_TYPE, *UINT) callconv(WINAPI) HRESULT,
        SetColorSpace1: *const fn (*T, COLOR_SPACE_TYPE) callconv(WINAPI) HRESULT,
        ResizeBuffers1: *const fn (
            *T,
            UINT,
            UINT,
            UINT,
            FORMAT,
            UINT,
            [*]const UINT,
            [*]const *IUnknown,
        ) callconv(WINAPI) HRESULT,
    };
};

pub const IID_ISwapChain3 = GUID{
    .Data1 = 0x94d99bdb,
    .Data2 = 0xf1f8,
    .Data3 = 0x4ab0,
    .Data4 = .{ 0xb2, 0x36, 0x7d, 0xa0, 0x17, 0x0e, 0xda, 0xb1 },
};

// Status return codes as defined here: https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-status
pub const STATUS_OCCLUDED = @bitCast(HRESULT, @as(c_ulong, 0x087A0001));
pub const STATUS_MODE_CHANGED = @bitCast(HRESULT, @as(c_ulong, 0x087A0007));
pub const STATUS_MODE_CHANGE_IN_PROGRESS = @bitCast(HRESULT, @as(c_ulong, 0x087A0008));

// Return codes as defined here: https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-error
pub const ERROR_ACCESS_DENIED = @bitCast(HRESULT, @as(c_ulong, 0x887A002B));
pub const ERROR_ACCESS_LOST = @bitCast(HRESULT, @as(c_ulong, 0x887A0026));
pub const ERROR_ALREADY_EXISTS = @bitCast(HRESULT, @as(c_ulong, 0x887A0036));
pub const ERROR_CANNOT_PROTECT_CONTENT = @bitCast(HRESULT, @as(c_ulong, 0x887A002A));
pub const ERROR_DEVICE_HUNG = @bitCast(HRESULT, @as(c_ulong, 0x887A0006));
pub const ERROR_DEVICE_REMOVED = @bitCast(HRESULT, @as(c_ulong, 0x887A0005));
pub const ERROR_DEVICE_RESET = @bitCast(HRESULT, @as(c_ulong, 0x887A0007));
pub const ERROR_DRIVER_INTERNAL_ERROR = @bitCast(HRESULT, @as(c_ulong, 0x887A0020));
pub const ERROR_FRAME_STATISTICS_DISJOINT = @bitCast(HRESULT, @as(c_ulong, 0x887A000B));
pub const ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE = @bitCast(HRESULT, @as(c_ulong, 0x887A000C));
pub const ERROR_INVALID_CALL = @bitCast(HRESULT, @as(c_ulong, 0x887A0001));
pub const ERROR_MORE_DATA = @bitCast(HRESULT, @as(c_ulong, 0x887A0003));
pub const ERROR_NAME_ALREADY_EXISTS = @bitCast(HRESULT, @as(c_ulong, 0x887A002C));
pub const ERROR_NONEXCLUSIVE = @bitCast(HRESULT, @as(c_ulong, 0x887A0021));
pub const ERROR_NOT_CURRENTLY_AVAILABLE = @bitCast(HRESULT, @as(c_ulong, 0x887A0022));
pub const ERROR_NOT_FOUND = @bitCast(HRESULT, @as(c_ulong, 0x887A0002));
pub const ERROR_REMOTE_CLIENT_DISCONNECTED = @bitCast(HRESULT, @as(c_ulong, 0x887A0023));
pub const ERROR_REMOTE_OUTOFMEMORY = @bitCast(HRESULT, @as(c_ulong, 0x887A0024));
pub const ERROR_RESTRICT_TO_OUTPUT_STALE = @bitCast(HRESULT, @as(c_ulong, 0x887A0029));
pub const ERROR_SDK_COMPONENT_MISSING = @bitCast(HRESULT, @as(c_ulong, 0x887A002D));
pub const ERROR_SESSION_DISCONNECTED = @bitCast(HRESULT, @as(c_ulong, 0x887A0028));
pub const ERROR_UNSUPPORTED = @bitCast(HRESULT, @as(c_ulong, 0x887A0004));
pub const ERROR_WAIT_TIMEOUT = @bitCast(HRESULT, @as(c_ulong, 0x887A0027));
pub const ERROR_WAS_STILL_DRAWING = @bitCast(HRESULT, @as(c_ulong, 0x887A000A));

// Error set corresponding to the above error return codes
pub const Error = error{
    ACCESS_DENIED,
    ACCESS_LOST,
    ALREADY_EXISTS,
    CANNOT_PROTECT_CONTENT,
    DEVICE_HUNG,
    DEVICE_REMOVED,
    DEVICE_RESET,
    DRIVER_INTERNAL_ERROR,
    FRAME_STATISTICS_DISJOINT,
    GRAPHICS_VIDPN_SOURCE_IN_USE,
    INVALID_CALL,
    MORE_DATA,
    NAME_ALREADY_EXISTS,
    NONEXCLUSIVE,
    NOT_CURRENTLY_AVAILABLE,
    NOT_FOUND,
    REMOTE_CLIENT_DISCONNECTED,
    REMOTE_OUTOFMEMORY,
    RESTRICT_TO_OUTPUT_STALE,
    SDK_COMPONENT_MISSING,
    SESSION_DISCONNECTED,
    UNSUPPORTED,
    WAIT_TIMEOUT,
    WAS_STILL_DRAWING,
};
