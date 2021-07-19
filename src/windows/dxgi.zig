const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("dxgitype.zig");
usingnamespace @import("dxgiformat.zig");

pub const DXGI_USAGE = packed struct {
    __reserved0: bool align(4) = false,
    __reserved1: bool = false,
    __reserved2: bool = false,
    __reserved3: bool = false,
    SHADER_INPUT: bool = false, // 0x10
    RENDER_TARGET_OUTPUT: bool = false, // 0x20
    BACK_BUFFER: bool = false, // 0x40
    SHARED: bool = false, // 0x80
    READ_ONLY: bool = false, // 0x100
    DISCARD_ON_PRESENT: bool = false, // 0x200
    UNORDERED_ACCESS: bool = false, // 0x400
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
};
comptime {
    std.debug.assert(@sizeOf(DXGI_USAGE) == 4);
    std.debug.assert(@alignOf(DXGI_USAGE) == 4);
}

pub const DXGI_FRAME_STATISTICS = extern struct {
    PresentCount: UINT,
    PresentRefreshCount: UINT,
    SyncRefreshCount: UINT,
    SyncQPCTime: LARGE_INTEGER,
    SyncGPUTime: LARGE_INTEGER,
};

pub const DXGI_MAPPED_RECT = extern struct {
    Pitch: INT,
    pBits: *BYTE,
};

pub const DXGI_ADAPTER_DESC = extern struct {
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

pub const DXGI_OUTPUT_DESC = extern struct {
    DeviceName: [32]WCHAR,
    DesktopCoordinates: RECT,
    AttachedToDesktop: BOOL,
    Rotation: DXGI_MODE_ROTATION,
    Monitor: HMONITOR,
};

pub const DXGI_SHARED_RESOURCE = extern struct {
    Handle: HANDLE,
};

pub const DXGI_RESOURCE_PRIORITY = enum(UINT) {
    MINIMUM = 0x28000000,
    LOW = 0x50000000,
    NORMAL = 0x78000000,
    HIGH = 0xa0000000,
    MAXIMUM = 0xc8000000,
};

pub const DXGI_RESIDENCY = enum(UINT) {
    FULLY_RESIDENT = 1,
    RESIDENT_IN_SHARED_MEMORY = 2,
    EVICTED_TO_DISK = 3,
};

pub const DXGI_SURFACE_DESC = extern struct {
    Width: UINT,
    Height: UINT,
    Format: DXGI_FORMAT,
    SampleDesc: DXGI_SAMPLE_DESC,
};

pub const DXGI_SWAP_EFFECT = enum(UINT) {
    DISCARD = 0,
    SEQUENTIAL = 1,
    FLIP_SEQUENTIAL = 3,
    FLIP_DISCARD = 4,
};

pub const DXGI_SWAP_CHAIN_FLAG = packed struct {
    NONPREROTATED: bool align(4) = false, // 0x1
    ALLOW_MODE_SWITCH: bool = false, // 0x2
    GDI_COMPATIBLE: bool = false, // 0x4
    RESTRICTED_CONTENT: bool = false, // 0x8
    RESTRICT_SHARED_RESOURCE_DRIVER: bool = false, // 0x10
    DISPLAY_ONLY: bool = false, // 0x20
    FRAME_LATENCY_WAITABLE_OBJECT: bool = false, // 0x40
    FOREGROUND_LAYER: bool = false, // 0x80
    FULLSCREEN_VIDEO: bool = false, // 0x100
    YUV_VIDEO: bool = false, // 0x200
    HW_PROTECTED: bool = false, // 0x400
    ALLOW_TEARING: bool = false, // 0x800
    RESTRICTED_TO_ALL_HOLOGRAPHIC_DISPLAYS: bool = false, // 0x1000
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
};
comptime {
    std.debug.assert(@sizeOf(DXGI_SWAP_CHAIN_FLAG) == 4);
    std.debug.assert(@alignOf(DXGI_SWAP_CHAIN_FLAG) == 4);
}

pub const DXGI_SWAP_CHAIN_DESC = extern struct {
    BufferDesc: DXGI_MODE_DESC,
    SampleDesc: DXGI_SAMPLE_DESC,
    BufferUsage: DXGI_USAGE,
    BufferCount: UINT,
    OutputWindow: HWND,
    Windowed: BOOL,
    SwapEffect: DXGI_SWAP_EFFECT,
    Flags: UINT,
};
