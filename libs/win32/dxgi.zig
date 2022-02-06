const windows = @import("windows.zig");
const UINT = windows.UINT;
const UINT64 = windows.UINT64;
const DWORD = windows.DWORD;
const FLOAT = windows.FLOAT;
const BOOL = windows.BOOL;
const GUID = windows.GUID;
const WINAPI = windows.WINAPI;
const IUnknown = windows.IUnknown;
const HRESULT = windows.HRESULT;
const WCHAR = windows.WCHAR;
const RECT = windows.RECT;
const INT = windows.INT;
const BYTE = windows.BYTE;
const HMONITOR = windows.HMONITOR;
const LARGE_INTEGER = windows.LARGE_INTEGER;
const HWND = windows.HWND;
const SIZE_T = windows.SIZE_T;
const LUID = windows.LUID;
const HANDLE = windows.HANDLE;
const POINT = windows.POINT;

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
    _420_OPAQUE = 106,
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
            ._420_OPAQUE,
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
pub const STANDARD_MULTISAMPLE_QUALITY_PATTERN: UINT = 0xffffffff;
pub const CENTER_MULTISAMPLE_QUALITY_PATTERN: UINT = 0xfffffffe;

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

pub const USAGE = UINT;
pub const USAGE_SHADER_INPUT: USAGE = 0x00000010;
pub const USAGE_RENDER_TARGET_OUTPUT: USAGE = 0x00000020;
pub const USAGE_BACK_BUFFER: USAGE = 0x00000040;
pub const USAGE_SHARED: USAGE = 0x00000080;
pub const USAGE_READ_ONLY: USAGE = 0x00000100;
pub const USAGE_DISCARD_ON_PRESENT: USAGE = 0x00000200;
pub const USAGE_UNORDERED_ACCESS: USAGE = 0x00000400;

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

pub const SWAP_CHAIN_FLAG = UINT;
pub const SWAP_CHAIN_FLAG_NONPREROTATED: SWAP_CHAIN_FLAG = 1;
pub const SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH: SWAP_CHAIN_FLAG = 2;
pub const SWAP_CHAIN_FLAG_GDI_COMPATIBLE: SWAP_CHAIN_FLAG = 4;
pub const SWAP_CHAIN_FLAG_RESTRICTED_CONTENT: SWAP_CHAIN_FLAG = 8;
pub const SWAP_CHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER: SWAP_CHAIN_FLAG = 16;
pub const SWAP_CHAIN_FLAG_DISPLAY_ONLY: SWAP_CHAIN_FLAG = 32;
pub const SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT: SWAP_CHAIN_FLAG = 64;
pub const SWAP_CHAIN_FLAG_FOREGROUND_LAYER: SWAP_CHAIN_FLAG = 128;
pub const SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO: SWAP_CHAIN_FLAG = 256;
pub const SWAP_CHAIN_FLAG_YUV_VIDEO: SWAP_CHAIN_FLAG = 512;
pub const SWAP_CHAIN_FLAG_HW_PROTECTED: SWAP_CHAIN_FLAG = 1024;
pub const SWAP_CHAIN_FLAG_ALLOW_TEARING: SWAP_CHAIN_FLAG = 2048;
pub const SWAP_CHAIN_FLAG_RESTRICTED_TO_ALL_HOLOGRAPHIC_DISPLAYS: SWAP_CHAIN_FLAG = 4096;

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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetPrivateData(self: *T, guid: *const GUID, data_size: UINT, data: *const anyopaque) HRESULT {
                return self.v.object.SetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const IUnknown) HRESULT {
                return self.v.object.SetPrivateDataInterface(self, guid, data);
            }
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: *anyopaque) HRESULT {
                return self.v.object.GetPrivateData(self, guid, data_size, data);
            }
            pub inline fn GetParent(self: *T, guid: *const GUID, parent: *?*anyopaque) HRESULT {
                return self.v.object.GetParent(self, guid, parent);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetPrivateData: fn (*T, *const GUID, UINT, *const anyopaque) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            GetPrivateData: fn (*T, *const GUID, *UINT, *anyopaque) callconv(WINAPI) HRESULT,
            GetParent: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDeviceSubObject = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDevice(self: *T, guid: *const GUID, parent: *?*anyopaque) HRESULT {
                return self.v.devsubobj.GetDevice(self, guid, parent);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IResource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetSharedHandle(self: *T, handle: *HANDLE) HRESULT {
                return self.v.resource.GetSharedHandle(self, handle);
            }
            pub inline fn GetUsage(self: *T, usage: *USAGE) HRESULT {
                return self.v.resource.GetUsage(self, usage);
            }
            pub inline fn SetEvictionPriority(self: *T, priority: UINT) HRESULT {
                return self.v.resource.SetEvictionPriority(self, priority);
            }
            pub inline fn GetEvictionPriority(self: *T, priority: *UINT) HRESULT {
                return self.v.resource.GetEvictionPriority(self, priority);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetSharedHandle: fn (*T, *HANDLE) callconv(WINAPI) HRESULT,
            GetUsage: fn (*T, *USAGE) callconv(WINAPI) HRESULT,
            SetEvictionPriority: fn (*T, UINT) callconv(WINAPI) HRESULT,
            GetEvictionPriority: fn (*T, *UINT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IKeyedMutex = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        mutex: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AcquireSync(self: *T, key: UINT64, milliseconds: DWORD) HRESULT {
                return self.v.mutex.AcquireSync(self, key, milliseconds);
            }
            pub inline fn ReleaseSync(self: *T, key: UINT64) HRESULT {
                return self.v.mutex.ReleaseSync(self, key);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            AcquireSync: fn (*T, UINT64, DWORD) callconv(WINAPI) HRESULT,
            ReleaseSync: fn (*T, UINT64) callconv(WINAPI) HRESULT,
        };
    }
};

pub const MAP_READ: UINT = 0x1;
pub const MAP_WRITE: UINT = 0x2;
pub const MAP_DISCARD: UINT = 0x4;

pub const ISurface = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        surface: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T, desc: *SURFACE_DESC) HRESULT {
                return self.v.surface.GetDesc(self, desc);
            }
            pub inline fn Map(self: *T, locked_rect: *MAPPED_RECT, flags: UINT) HRESULT {
                return self.v.surface.Map(self, locked_rect, flags);
            }
            pub inline fn Unmap(self: *T) HRESULT {
                return self.v.surface.Unmap(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *SURFACE_DESC) callconv(WINAPI) HRESULT,
            Map: fn (*T, *MAPPED_RECT, UINT) callconv(WINAPI) HRESULT,
            Unmap: fn (*T) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IAdapter = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        adapter: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumOutputs(self: *T, index: UINT, output: *?*IOutput) HRESULT {
                return self.v.adapter.EnumOutputs(self, index, output);
            }
            pub inline fn GetDesc(self: *T, desc: *ADAPTER_DESC) HRESULT {
                return self.v.adapter.GetDesc(self, desc);
            }
            pub inline fn CheckInterfaceSupport(self: *T, guid: *const GUID, umd_ver: *LARGE_INTEGER) HRESULT {
                return self.v.adapter.CheckInterfaceSupport(self, guid, umd_ver);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumOutputs: fn (*T, UINT, *?*IOutput) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *ADAPTER_DESC) callconv(WINAPI) HRESULT,
            CheckInterfaceSupport: fn (*T, *const GUID, *LARGE_INTEGER) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ENUM_MODES_INTERLACED: UINT = 0x1;
pub const ENUM_MODES_SCALING: UINT = 0x2;
pub const ENUM_MODES_STEREO: UINT = 0x4;
pub const ENUM_MODES_DISABLED_STEREO: UINT = 0x8;

pub const IOutput = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        output: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T, desc: *OUTPUT_DESC) HRESULT {
                return self.v.output.GetDesc(self, desc);
            }
            pub inline fn GetDisplayModeList(
                self: *T,
                enum_format: FORMAT,
                flags: UINT,
                num_nodes: *UINT,
                desc: ?*MODE_DESC,
            ) HRESULT {
                return self.v.output.GetDisplayModeList(self, enum_format, flags, num_nodes, desc);
            }
            pub inline fn FindClosestMatchingMode(
                self: *T,
                mode_to_match: *const MODE_DESC,
                closest_match: *MODE_DESC,
                concerned_device: ?*IUnknown,
            ) HRESULT {
                return self.v.output.FindClosestMatchingMode(self, mode_to_match, closest_match, concerned_device);
            }
            pub inline fn WaitForVBlank(self: *T) HRESULT {
                return self.v.output.WaitForVBlank(self);
            }
            pub inline fn TakeOwnership(self: *T, device: *IUnknown, exclusive: BOOL) HRESULT {
                return self.v.output.TakeOwnership(self, device, exclusive);
            }
            pub inline fn ReleaseOwnership(self: *T) void {
                self.v.output.ReleaseOwnership(self);
            }
            pub inline fn GetGammaControlCapabilities(self: *T, gamma_caps: *GAMMA_CONTROL_CAPABILITIES) HRESULT {
                return self.v.output.GetGammaControlCapabilities(self, gamma_caps);
            }
            pub inline fn SetGammaControl(self: *T, array: *const GAMMA_CONTROL) HRESULT {
                return self.v.output.SetGammaControl(self, array);
            }
            pub inline fn GetGammaControl(self: *T, array: *GAMMA_CONTROL) HRESULT {
                return self.v.output.GetGammaControl(self, array);
            }
            pub inline fn SetDisplaySurface(self: *T, scanout_surface: *ISurface) HRESULT {
                return self.v.output.SetDisplaySurface(self, scanout_surface);
            }
            pub inline fn GetDisplaySurfaceData(self: *T, destination: *ISurface) HRESULT {
                return self.v.output.GetDisplaySurfaceData(self, destination);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *FRAME_STATISTICS) HRESULT {
                return self.v.output.GetFrameStatistics(self, stats);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (self: *T, desc: *OUTPUT_DESC) callconv(WINAPI) HRESULT,
            GetDisplayModeList: fn (*T, FORMAT, UINT, *UINT, ?*MODE_DESC) callconv(WINAPI) HRESULT,
            FindClosestMatchingMode: fn (*T, *const MODE_DESC, *MODE_DESC, ?*IUnknown) callconv(WINAPI) HRESULT,
            WaitForVBlank: fn (*T) callconv(WINAPI) HRESULT,
            TakeOwnership: fn (*T, *IUnknown, BOOL) callconv(WINAPI) HRESULT,
            ReleaseOwnership: fn (*T) callconv(WINAPI) void,
            GetGammaControlCapabilities: fn (*T, *GAMMA_CONTROL_CAPABILITIES) callconv(WINAPI) HRESULT,
            SetGammaControl: fn (*T, *const GAMMA_CONTROL) callconv(WINAPI) HRESULT,
            GetGammaControl: fn (*T, *GAMMA_CONTROL) callconv(WINAPI) HRESULT,
            SetDisplaySurface: fn (*T, *ISurface) callconv(WINAPI) HRESULT,
            GetDisplaySurfaceData: fn (*T, *ISurface) callconv(WINAPI) HRESULT,
            GetFrameStatistics: fn (*T, *FRAME_STATISTICS) callconv(WINAPI) HRESULT,
        };
    }
};

pub const MAX_SWAP_CHAIN_BUFFERS = 16;

pub const PRESENT_TEST: UINT = 0x00000001;
pub const PRESENT_DO_NOT_SEQUENCE: UINT = 0x00000002;
pub const PRESENT_RESTART: UINT = 0x00000004;
pub const PRESENT_DO_NOT_WAIT: UINT = 0x00000008;
pub const PRESENT_STEREO_PREFER_RIGHT: UINT = 0x00000010;
pub const PRESENT_STEREO_TEMPORARY_MONO: UINT = 0x00000020;
pub const PRESENT_RESTRICT_TO_OUTPUT: UINT = 0x00000040;
pub const PRESENT_USE_DURATION: UINT = 0x00000100;
pub const PRESENT_ALLOW_TEARING: UINT = 0x00000200;

pub const ISwapChain = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        swapchain: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Present(self: *T, sync_interval: UINT, flags: UINT) HRESULT {
                return self.v.swapchain.Present(self, sync_interval, flags);
            }
            pub inline fn GetBuffer(self: *T, index: u32, guid: *const GUID, surface: *?*anyopaque) HRESULT {
                return self.v.swapchain.GetBuffer(self, index, guid, surface);
            }
            pub inline fn SetFullscreenState(self: *T, target: ?*IOutput) HRESULT {
                return self.v.swapchain.SetFullscreenState(self, target);
            }
            pub inline fn GetFullscreenState(self: *T, fullscreen: ?*BOOL, target: ?*?*IOutput) HRESULT {
                return self.v.swapchain.GetFullscreenState(self, fullscreen, target);
            }
            pub inline fn GetDesc(self: *T, desc: *SWAP_CHAIN_DESC) HRESULT {
                return self.v.swapchain.GetDesc(self, desc);
            }
            pub inline fn ResizeBuffers(
                self: *T,
                count: UINT,
                width: UINT,
                height: UINT,
                format: FORMAT,
                flags: SWAP_CHAIN_FLAG,
            ) HRESULT {
                return self.v.swapchain.ResizeBuffers(self, count, width, height, format, flags);
            }
            pub inline fn ResizeTarget(self: *T, params: *const MODE_DESC) HRESULT {
                return self.v.swapchain.ResizeTarget(self, params);
            }
            pub inline fn GetContainingOutput(self: *T, output: *?*IOutput) HRESULT {
                return self.v.swapchain.GetContainingOutput(self, output);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *FRAME_STATISTICS) HRESULT {
                return self.v.swapchain.GetFrameStatistics(self, stats);
            }
            pub inline fn GetLastPresentCount(self: *T, count: *UINT) HRESULT {
                return self.v.swapchain.GetLastPresentCount(self, count);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Present: fn (*T, UINT, UINT) callconv(WINAPI) HRESULT,
            GetBuffer: fn (*T, u32, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            SetFullscreenState: fn (*T, ?*IOutput) callconv(WINAPI) HRESULT,
            GetFullscreenState: fn (*T, ?*BOOL, ?*?*IOutput) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *SWAP_CHAIN_DESC) callconv(WINAPI) HRESULT,
            ResizeBuffers: fn (*T, UINT, UINT, UINT, FORMAT, SWAP_CHAIN_FLAG) callconv(WINAPI) HRESULT,
            ResizeTarget: fn (*T, *const MODE_DESC) callconv(WINAPI) HRESULT,
            GetContainingOutput: fn (*T, *?*IOutput) callconv(WINAPI) HRESULT,
            GetFrameStatistics: fn (*T, *FRAME_STATISTICS) callconv(WINAPI) HRESULT,
            GetLastPresentCount: fn (*T, *UINT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IFactory = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumAdapters(self: *T, index: UINT, adapter: *?*IAdapter) HRESULT {
                return self.v.factory.EnumAdapters(self, index, adapter);
            }
            pub inline fn MakeWindowAssociation(self: *T, window: HWND, flags: UINT) HRESULT {
                return self.v.factory.MakeWindowAssociation(self, window, flags);
            }
            pub inline fn GetWindowAssociation(self: *T, window: *HWND) HRESULT {
                return self.v.factory.GetWindowAssociation(self, window);
            }
            pub inline fn CreateSwapChain(
                self: *T,
                device: *IUnknown,
                desc: *SWAP_CHAIN_DESC,
                swapchain: *?*ISwapChain,
            ) HRESULT {
                return self.v.factory.CreateSwapChain(self, device, desc, swapchain);
            }
            pub inline fn CreateSoftwareAdapter(self: *T, adapter: *?*IAdapter) HRESULT {
                return self.v.factory.CreateSoftwareAdapter(self, adapter);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAdapters: fn (*T, UINT, *?*IAdapter) callconv(WINAPI) HRESULT,
            MakeWindowAssociation: fn (*T, HWND, UINT) callconv(WINAPI) HRESULT,
            GetWindowAssociation: fn (*T, *HWND) callconv(WINAPI) HRESULT,
            CreateSwapChain: fn (*T, *IUnknown, *SWAP_CHAIN_DESC, *?*ISwapChain) callconv(WINAPI) HRESULT,
            CreateSoftwareAdapter: fn (*T, *?*IAdapter) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetAdapter(self: *T, adapter: *?*IAdapter) HRESULT {
                return self.v.device.GetAdapter(self, adapter);
            }
            pub inline fn CreateSurface(
                self: *T,
                desc: *const SURFACE_DESC,
                num_surfaces: UINT,
                usage: USAGE,
                shared_resource: ?*const SHARED_RESOURCE,
                surface: *?*ISurface,
            ) HRESULT {
                return self.v.device.CreateSurface(self, desc, num_surfaces, usage, shared_resource, surface);
            }
            pub inline fn QueryResourceResidency(
                self: *T,
                resources: *const *IUnknown,
                status: [*]RESIDENCY,
                num_resources: UINT,
            ) HRESULT {
                return self.v.device.QueryResourceResidency(self, resources, status, num_resources);
            }
            pub inline fn SetGPUThreadPriority(self: *T, priority: INT) HRESULT {
                return self.v.device.SetGPUThreadPriority(self, priority);
            }
            pub inline fn GetGPUThreadPriority(self: *T, priority: *INT) HRESULT {
                return self.v.device.GetGPUThreadPriority(self, priority);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetAdapter: fn (self: *T, adapter: *?*IAdapter) callconv(WINAPI) HRESULT,
            CreateSurface: fn (
                *T,
                *const SURFACE_DESC,
                UINT,
                USAGE,
                ?*const SHARED_RESOURCE,
                *?*ISurface,
            ) callconv(WINAPI) HRESULT,
            QueryResourceResidency: fn (
                *T,
                *const *IUnknown,
                [*]RESIDENCY,
                UINT,
            ) callconv(WINAPI) HRESULT,
            SetGPUThreadPriority: fn (self: *T, priority: INT) callconv(WINAPI) HRESULT,
            GetGPUThreadPriority: fn (self: *T, priority: *INT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ADAPTER_FLAGS = UINT;
pub const ADAPTER_FLAG_NONE: ADAPTER_FLAGS = 0;
pub const ADAPTER_FLAG_REMOTE: ADAPTER_FLAGS = 0x1;
pub const ADAPTER_FLAG_SOFTWARE: ADAPTER_FLAGS = 0x2;

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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumAdapters1(self: *T, index: UINT, adapter: *?*IAdapter1) HRESULT {
                return self.v.factory1.EnumAdapters1(self, index, adapter);
            }
            pub inline fn IsCurrent(self: *T) BOOL {
                return self.v.factory1.IsCurrent(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAdapters1: fn (*T, UINT, *?*IAdapter1) callconv(WINAPI) HRESULT,
            IsCurrent: fn (*T) callconv(WINAPI) BOOL,
        };
    }
};

pub const IFactory2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
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
    }
};

pub const IFactory3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
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
            GetCreationFlags: *anyopaque,
        };
    }
};

pub const IFactory4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
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
            EnumAdapterByLuid: *anyopaque,
            EnumWarpAdapter: *anyopaque,
        };
    }
};

pub const FEATURE = enum(UINT) {
    PRESENT_ALLOW_TEARING = 0,
};

pub const IID_IFactory5 = GUID.parse("{7632e1f5-ee65-4dca-87fd-84cd75f8838d}");
pub const IFactory5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: IFactory4.VTable(Self),
        factory5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace IFactory4.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CheckFeatureSupport(
                self: *T,
                feature: FEATURE,
                support_data: *anyopaque,
                support_data_size: UINT,
            ) HRESULT {
                return self.v.factory5.CheckFeatureSupport(self, feature, support_data, support_data_size);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CheckFeatureSupport: fn (*T, FEATURE, *anyopaque, UINT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const GPU_PREFERENCE = UINT;
pub const GPU_PREFERENCE_UNSPECIFIED: GPU_PREFERENCE = 0;
pub const GPU_PREFERENCE_MINIMUM: GPU_PREFERENCE = 1;
pub const GPU_PREFERENCE_HIGH_PERFORMANCE: GPU_PREFERENCE = 2;

pub const IID_IFactory6 = GUID.parse("{c1b6694f-ff09-44a9-b03c-77900a0a1d17}");
pub const IFactory6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        factory: IFactory.VTable(Self),
        factory1: IFactory1.VTable(Self),
        factory2: IFactory2.VTable(Self),
        factory3: IFactory3.VTable(Self),
        factory4: IFactory4.VTable(Self),
        factory5: IFactory5.VTable(Self),
        factory6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IFactory.Methods(Self);
    usingnamespace IFactory1.Methods(Self);
    usingnamespace IFactory2.Methods(Self);
    usingnamespace IFactory3.Methods(Self);
    usingnamespace IFactory4.Methods(Self);
    usingnamespace IFactory5.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumAdapterByGpuPreference(
                self: *T,
                adapter_index: UINT,
                gpu_preference: GPU_PREFERENCE,
                riid: *const GUID,
                adapter: *?*IAdapter1,
            ) HRESULT {
                return self.v.factory6.EnumAdapterByGpuPreference(self, adapter_index, gpu_preference, riid, adapter);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAdapterByGpuPreference: fn (*T, UINT, GPU_PREFERENCE, *const GUID, *?*IAdapter1) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IAdapter1 = GUID.parse("{29038f61-3839-4626-91fd-086879011a05}");
pub const IAdapter1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        adapter: IAdapter.VTable(Self),
        adapter1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IAdapter.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc1(self: *T, desc: *ADAPTER_DESC1) HRESULT {
                return self.v.adapter1.GetDesc1(self, desc);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc1: fn (*T, *ADAPTER_DESC1) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDevice1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetMaximumFrameLatency(self: *T, max_latency: UINT) HRESULT {
                return self.v.device1.SetMaximumFrameLatency(self, max_latency);
            }
            pub inline fn GetMaximumFrameLatency(self: *T, max_latency: *UINT) HRESULT {
                return self.v.device1.GetMaximumFrameLatency(self, max_latency);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetMaximumFrameLatency: fn (self: *T, max_latency: UINT) callconv(WINAPI) HRESULT,
            GetMaximumFrameLatency: fn (self: *T, max_latency: *UINT) callconv(WINAPI) HRESULT,
        };
    }
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

pub const CREATE_FACTORY_DEBUG: UINT = 0x1;
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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        swapchain: ISwapChain.VTable(Self),
        swapchain1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace ISwapChain.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc1(self: *T, desc: *SWAP_CHAIN_DESC1) HRESULT {
                return self.v.swapchain1.GetDesc1(self, desc);
            }
            pub inline fn GetFullscreenDesc(self: *T, desc: *SWAP_CHAIN_FULLSCREEN_DESC) HRESULT {
                return self.v.swapchain1.GetFullscreenDesc(self, desc);
            }
            pub inline fn GetHwnd(self: *T, hwnd: *HWND) HRESULT {
                return self.v.swapchain1.GetHwnd(self, hwnd);
            }
            pub inline fn GetCoreWindow(self: *T, guid: *const GUID, unknown: *?*anyopaque) HRESULT {
                return self.v.swapchain1.GetCoreWindow(self, guid, unknown);
            }
            pub inline fn Present1(
                self: *T,
                sync_interval: UINT,
                flags: UINT,
                params: *const PRESENT_PARAMETERS,
            ) HRESULT {
                return self.v.swapchain1.Present1(self, sync_interval, flags, params);
            }
            pub inline fn IsTemporaryMonoSupported(self: *T) BOOL {
                return self.v.swapchain1.IsTemporaryMonoSupported(self);
            }
            pub inline fn GetRestrictToOutput(self: *T, output: *?*IOutput) HRESULT {
                return self.v.swapchain1.GetRestrictToOutput(self, output);
            }
            pub inline fn SetBackgroundColor(self: *T, color: *const RGBA) HRESULT {
                return self.v.swapchain1.SetBackgroundColor(self, color);
            }
            pub inline fn GetBackgroundColor(self: *T, color: *RGBA) HRESULT {
                return self.v.swapchain1.GetBackgroundColor(self, color);
            }
            pub inline fn SetRotation(self: *T, rotation: MODE_ROTATION) HRESULT {
                return self.v.swapchain1.SetRotation(self, rotation);
            }
            pub inline fn GetRotation(self: *T, rotation: *MODE_ROTATION) HRESULT {
                return self.v.swapchain1.GetRotation(self, rotation);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc1: fn (*T, *SWAP_CHAIN_DESC1) callconv(WINAPI) HRESULT,
            GetFullscreenDesc: fn (*T, *SWAP_CHAIN_FULLSCREEN_DESC) callconv(WINAPI) HRESULT,
            GetHwnd: fn (*T, *HWND) callconv(WINAPI) HRESULT,
            GetCoreWindow: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            Present1: fn (*T, UINT, UINT, *const PRESENT_PARAMETERS) callconv(WINAPI) HRESULT,
            IsTemporaryMonoSupported: fn (*T) callconv(WINAPI) BOOL,
            GetRestrictToOutput: fn (*T, *?*IOutput) callconv(WINAPI) HRESULT,
            SetBackgroundColor: fn (*T, *const RGBA) callconv(WINAPI) HRESULT,
            GetBackgroundColor: fn (*T, *RGBA) callconv(WINAPI) HRESULT,
            SetRotation: fn (*T, MODE_ROTATION) callconv(WINAPI) HRESULT,
            GetRotation: fn (*T, *MODE_ROTATION) callconv(WINAPI) HRESULT,
        };
    }
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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        swapchain: ISwapChain.VTable(Self),
        swapchain1: ISwapChain1.VTable(Self),
        swapchain2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace ISwapChain.Methods(Self);
    usingnamespace ISwapChain1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetSourceSize(self: *T, width: UINT, height: UINT) HRESULT {
                return self.v.swapchain2.SetSourceSize(self, width, height);
            }
            pub inline fn GetSourceSize(self: *T, width: *UINT, height: *UINT) HRESULT {
                return self.v.swapchain2.GetSourceSize(self, width, height);
            }
            pub inline fn SetMaximumFrameLatency(self: *T, max_latency: UINT) HRESULT {
                return self.v.swapchain2.SetMaximumFrameLatency(self, max_latency);
            }
            pub inline fn GetMaximumFrameLatency(self: *T, max_latency: *UINT) HRESULT {
                return self.v.swapchain2.GetMaximumFrameLatency(self, max_latency);
            }
            pub inline fn GetFrameLatencyWaitableObject(self: *T) HANDLE {
                return self.v.swapchain2.GetFrameLatencyWaitableObject(self);
            }
            pub inline fn SetMatrixTransform(self: *T, matrix: *const MATRIX_3X2_F) HRESULT {
                return self.v.swapchain2.SetMatrixTransform(self, matrix);
            }
            pub inline fn GetMatrixTransform(self: *T, matrix: *MATRIX_3X2_F) HRESULT {
                return self.v.swapchain2.GetMatrixTransform(self, matrix);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetSourceSize: fn (*T, UINT, UINT) callconv(WINAPI) HRESULT,
            GetSourceSize: fn (*T, *UINT, *UINT) callconv(WINAPI) HRESULT,
            SetMaximumFrameLatency: fn (*T, UINT) callconv(WINAPI) HRESULT,
            GetMaximumFrameLatency: fn (*T, *UINT) callconv(WINAPI) HRESULT,
            GetFrameLatencyWaitableObject: fn (*T) callconv(WINAPI) HANDLE,
            SetMatrixTransform: fn (*T, *const MATRIX_3X2_F) callconv(WINAPI) HRESULT,
            GetMatrixTransform: fn (*T, *MATRIX_3X2_F) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_ISwapChain2 = GUID{
    .Data1 = 0xa8be2ac4,
    .Data2 = 0x199f,
    .Data3 = 0x4946,
    .Data4 = .{ 0xb3, 0x31, 0x79, 0x59, 0x9f, 0xb9, 0x8d, 0xe7 },
};

pub const ISwapChain3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devsubobj: IDeviceSubObject.VTable(Self),
        swapchain: ISwapChain.VTable(Self),
        swapchain1: ISwapChain1.VTable(Self),
        swapchain2: ISwapChain2.VTable(Self),
        swapchain3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceSubObject.Methods(Self);
    usingnamespace ISwapChain.Methods(Self);
    usingnamespace ISwapChain1.Methods(Self);
    usingnamespace ISwapChain2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCurrentBackBufferIndex(self: *T) UINT {
                return self.v.swapchain3.GetCurrentBackBufferIndex(self);
            }
            pub inline fn CheckColorSpaceSupport(self: *T, space: COLOR_SPACE_TYPE, support: *UINT) HRESULT {
                return self.v.swapchain3.CheckColorSpaceSupport(self, space, support);
            }
            pub inline fn SetColorSpace1(self: *T, space: COLOR_SPACE_TYPE) HRESULT {
                return self.v.swapchain3.SetColorSpace1(self, space);
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
                return self.v.swapchain3.ResizeBuffers1(
                    self,
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

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetCurrentBackBufferIndex: fn (*T) callconv(WINAPI) UINT,
            CheckColorSpaceSupport: fn (*T, COLOR_SPACE_TYPE, *UINT) callconv(WINAPI) HRESULT,
            SetColorSpace1: fn (*T, COLOR_SPACE_TYPE) callconv(WINAPI) HRESULT,
            ResizeBuffers1: fn (
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
    }
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
