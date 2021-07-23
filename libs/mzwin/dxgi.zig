const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("win.zig");
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
    Flags: DXGI_SWAP_CHAIN_FLAG,
};

pub const IDXGIObject = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetPrivateData(self: *T, guid: *const GUID, data_size: UINT, data: *const c_void) HRESULT {
                return self.v.object.SetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const IUnknown) HRESULT {
                return self.v.object.SetPrivateDataInterface(self, guid, data);
            }
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: *c_void) HRESULT {
                return self.v.object.GetPrivateData(self, guid, data_size, data);
            }
            pub inline fn GetParent(self: *T, guid: *const GUID, parent: *?*c_void) HRESULT {
                return self.v.object.GetParent(self, guid, parent);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetPrivateData: fn (*T, *const GUID, UINT, *const c_void) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            GetPrivateData: fn (*T, *const GUID, *UINT, *c_void) callconv(WINAPI) HRESULT,
            GetParent: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIDeviceSubObject = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDevice(self: *T, guid: *const GUID, parent: *?*c_void) HRESULT {
                return self.v.devsubobj.GetDevice(self, guid, parent);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIResource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetSharedHandle(self: *T, handle: *HANDLE) HRESULT {
                return self.v.resource.GetSharedHandle(self, handle);
            }
            pub inline fn GetUsage(self: *T, usage: *DXGI_USAGE) HRESULT {
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
            GetUsage: fn (*T, *DXGI_USAGE) callconv(WINAPI) HRESULT,
            SetEvictionPriority: fn (*T, UINT) callconv(WINAPI) HRESULT,
            GetEvictionPriority: fn (*T, *UINT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIKeyedMutex = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        mutex: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
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

pub const DXGI_MAP = packed struct {
    READ: bool align(4) = false, // 0x1
    WRITE: bool = false, // 0x2
    DISCARD: bool = false, // 0x4
    __reserved3: bool = false,
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
};
comptime {
    std.debug.assert(@sizeOf(DXGI_MAP) == 4);
    std.debug.assert(@alignOf(DXGI_MAP) == 4);
}

pub const IDXGISurface = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        surface: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T, desc: *DXGI_SURFACE_DESC) HRESULT {
                return self.v.surface.GetDesc(self, desc);
            }
            pub inline fn Map(self: *T, locked_rect: *DXGI_MAPPED_RECT, flags: DXGI_MAP) HRESULT {
                return self.v.surface.Map(self, locked_rect, flags);
            }
            pub inline fn Unmap(self: *T) HRESULT {
                return self.v.surface.Unmap(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *DXGI_SURFACE_DESC) callconv(WINAPI) HRESULT,
            Map: fn (*T, *DXGI_MAPPED_RECT, DXGI_MAP) callconv(WINAPI) HRESULT,
            Unmap: fn (*T) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIAdapter = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        adapter: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumOutputs(self: *T, index: UINT, output: *?*IDXGIOutput) HRESULT {
                return self.v.adapter.EnumOutputs(self, index, output);
            }
            pub inline fn GetDesc(self: *T, desc: *DXGI_ADAPTER_DESC) HRESULT {
                return self.v.adapter.GetDesc(self, desc);
            }
            pub inline fn CheckInterfaceSupport(self: *T, guid: *const GUID, umd_ver: *LARGE_INTEGER) HRESULT {
                return self.v.adapter.CheckInterfaceSupport(self, guid, umd_ver);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumOutputs: fn (*T, UINT, *?*IDXGIOutput) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *DXGI_ADAPTER_DESC) callconv(WINAPI) HRESULT,
            CheckInterfaceSupport: fn (*T, *const GUID, *LARGE_INTEGER) callconv(WINAPI) HRESULT,
        };
    }
};

pub const DXGI_ENUM_MODES = packed struct {
    INTERLACED: bool align(4) = false, // 0x1
    SCALING: bool = false, // 0x2
    STEREO: bool = false, // 0x4
    DISABLED_STEREO: bool = false, // 0x8
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
};
comptime {
    std.debug.assert(@sizeOf(DXGI_ENUM_MODES) == 4);
    std.debug.assert(@alignOf(DXGI_ENUM_MODES) == 4);
}

pub const IDXGIOutput = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        output: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T, desc: *DXGI_OUTPUT_DESC) HRESULT {
                return self.v.output.GetDesc(self, desc);
            }
            pub inline fn GetDisplayModeList(
                self: *T,
                enum_format: DXGI_FORMAT,
                flags: DXGI_ENUM_MODES,
                num_nodes: *UINT,
                desc: ?*DXGI_MODE_DESC,
            ) HRESULT {
                return self.v.output.GetDisplayModeList(self, enum_format, flags, num_nodes, desc);
            }
            pub inline fn FindClosestMatchingMode(
                self: *T,
                mode_to_match: *const DXGI_MODE_DESC,
                closest_match: *DXGI_MODE_DESC,
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
            pub inline fn GetGammaControlCapabilities(self: *T, gamma_caps: *DXGI_GAMMA_CONTROL_CAPABILITIES) HRESULT {
                return self.v.output.GetGammaControlCapabilities(self, gamma_caps);
            }
            pub inline fn SetGammaControl(self: *T, array: *const DXGI_GAMMA_CONTROL) HRESULT {
                return self.v.output.SetGammaControl(self, array);
            }
            pub inline fn GetGammaControl(self: *T, array: *DXGI_GAMMA_CONTROL) HRESULT {
                return self.v.output.GetGammaControl(self, array);
            }
            pub inline fn SetDisplaySurface(self: *T, scanout_surface: *IDXGISurface) HRESULT {
                return self.v.output.SetDisplaySurface(self, scanout_surface);
            }
            pub inline fn GetDisplaySurfaceData(self: *T, destination: *IDXGISurface) HRESULT {
                return self.v.output.GetDisplaySurfaceData(self, destination);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *DXGI_FRAME_STATISTICS) HRESULT {
                return self.v.output.GetFrameStatistics(self, stats);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (self: *T, desc: *DXGI_OUTPUT_DESC) callconv(WINAPI) HRESULT,
            GetDisplayModeList: fn (*T, DXGI_FORMAT, DXGI_ENUM_MODES, *UINT, ?*DXGI_MODE_DESC) callconv(WINAPI) HRESULT,
            FindClosestMatchingMode: fn (*T, *const DXGI_MODE_DESC, *DXGI_MODE_DESC, ?*IUnknown) callconv(WINAPI) HRESULT,
            WaitForVBlank: fn (*T) callconv(WINAPI) HRESULT,
            TakeOwnership: fn (*T, *IUnknown, BOOL) callconv(WINAPI) HRESULT,
            ReleaseOwnership: fn (*T) callconv(WINAPI) void,
            GetGammaControlCapabilities: fn (*T, *DXGI_GAMMA_CONTROL_CAPABILITIES) callconv(WINAPI) HRESULT,
            SetGammaControl: fn (*T, *const DXGI_GAMMA_CONTROL) callconv(WINAPI) HRESULT,
            GetGammaControl: fn (*T, *DXGI_GAMMA_CONTROL) callconv(WINAPI) HRESULT,
            SetDisplaySurface: fn (*T, *IDXGISurface) callconv(WINAPI) HRESULT,
            GetDisplaySurfaceData: fn (*T, *IDXGISurface) callconv(WINAPI) HRESULT,
            GetFrameStatistics: fn (*T, *DXGI_FRAME_STATISTICS) callconv(WINAPI) HRESULT,
        };
    }
};

pub const DXGI_MAX_SWAP_CHAIN_BUFFERS = 16;

pub const DXGI_PRESENT = packed struct {
    TEST: bool align(4) = false, // 0x1
    DO_NOT_SEQUENCE: bool = false, // 0x2
    RESTART: bool = false, // 0x4
    DO_NOT_WAIT: bool = false, // 0x8
    STEREO_PREFER_RIGHT: bool = false, // 0x10
    STEREO_TEMPORARY_MONO: bool = false, // 0x20
    RESTRICT_TO_OUTPUT: bool = false, // 0x40
    __reserved7: bool = false, // 0x80
    USE_DURATION: bool = false, // 0x100
    ALLOW_TEARING: bool = false, // 0x200
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
};
comptime {
    std.debug.assert(@sizeOf(DXGI_PRESENT) == 4);
    std.debug.assert(@alignOf(DXGI_PRESENT) == 4);
}

pub const IDXGISwapChain = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        swapchain: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Present(self: *T, sync_interval: UINT, flags: DXGI_PRESENT) HRESULT {
                return self.v.swapchain.Present(self, sync_interval, flags);
            }
            pub inline fn GetBuffer(self: *T, index: u32, guid: *const GUID, surface: *?*c_void) HRESULT {
                return self.v.swapchain.GetBuffer(self, index, guid, surface);
            }
            pub inline fn SetFullscreenState(self: *T, target: ?*IDXGIOutput) HRESULT {
                return self.v.swapchain.SetFullscreenState(self, target);
            }
            pub inline fn GetFullscreenState(self: *T, fullscreen: ?*BOOL, target: ?*?*IDXGIOutput) HRESULT {
                return self.v.swapchain.GetFullscreenState(self, fullscreen, target);
            }
            pub inline fn GetDesc(self: *T, desc: *DXGI_SWAP_CHAIN_DESC) HRESULT {
                return self.v.swapchain.GetDesc(self, desc);
            }
            pub inline fn ResizeBuffers(
                self: *T,
                count: UINT,
                width: UINT,
                height: UINT,
                format: DXGI_FORMAT,
                flags: DXGI_SWAP_CHAIN_FLAG,
            ) HRESULT {
                return self.v.swapchain.ResizeBuffers(self, count, width, height, format, flags);
            }
            pub inline fn ResizeTarget(self: *T, params: *const DXGI_MODE_DESC) HRESULT {
                return self.v.swapchain.ResizeTarget(self, params);
            }
            pub inline fn GetContainingOutput(self: *T, output: *?*pIDXGIOutput) HRESULT {
                return self.v.swapchain.GetContainingOutput(self, output);
            }
            pub inline fn GetFrameStatistics(self: *T, stats: *DXGI_FRAME_STATISTICS) HRESULT {
                return self.v.swapchain.GetFrameStatistics(self, stats);
            }
            pub inline fn GetLastPresentCount(self: *T, count: *UINT) HRESULT {
                return self.v.swapchain.GetLastPresentCount(self, count);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Present: fn (*T, UINT, DXGI_PRESENT) callconv(WINAPI) HRESULT,
            GetBuffer: fn (*T, u32, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            SetFullscreenState: fn (*T, ?*IDXGIOutput) callconv(WINAPI) HRESULT,
            GetFullscreenState: fn (*T, ?*BOOL, ?*?*IDXGIOutput) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *DXGI_SWAP_CHAIN_DESC) callconv(WINAPI) HRESULT,
            ResizeBuffers: fn (*T, UINT, UINT, UINT, DXGI_FORMAT, DXGI_SWAP_CHAIN_FLAG) callconv(WINAPI) HRESULT,
            ResizeTarget: fn (*T, *const DXGI_MODE_DESC) callconv(WINAPI) HRESULT,
            GetContainingOutput: fn (*T, *?*IDXGIOutput) callconv(WINAPI) HRESULT,
            GetFrameStatistics: fn (*T, *DXGI_FRAME_STATISTICS) callconv(WINAPI) HRESULT,
            GetLastPresentCount: fn (*T, *UINT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIFactory = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        factory: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumAdapters(self: *T, index: UINT, adapter: *?*IDXGIAdapter) HRESULT {
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
                desc: *DXGI_SWAP_CHAIN_DESC,
                swapchain: *?*IDXGISwapChain,
            ) HRESULT {
                return self.v.factory.CreateSwapChain(self, device, desc, swapchain);
            }
            pub inline fn CreateSoftwareAdapter(self: *T, adapter: *?*IDXGIAdapter) HRESULT {
                return self.v.factory.CreateSoftwareAdapter(self, adapter);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAdapters: fn (*T, UINT, *?*IDXGIAdapter) callconv(WINAPI) HRESULT,
            MakeWindowAssociation: fn (*T, HWND, UINT) callconv(WINAPI) HRESULT,
            GetWindowAssociation: fn (*T, *HWND) callconv(WINAPI) HRESULT,
            CreateSwapChain: fn (*T, *IUnknown, *DXGI_SWAP_CHAIN_DESC, *?*IDXGISwapChain) callconv(WINAPI) HRESULT,
            CreateSoftwareAdapter: fn (*T, *?*IDXGIAdapter) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetAdapter(self: *T, adapter: *?*IDXGIAdapter) HRESULT {
                return self.v.device.GetAdapter(self, adapter);
            }
            pub inline fn CreateSurface(
                self: *T,
                desc: *const DXGI_SURFACE_DESC,
                num_surfaces: UINT,
                usage: DXGI_USAGE,
                shared_resource: ?*const DXGI_SHARED_RESOURCE,
                surface: *?*IDXGISurface,
            ) HRESULT {
                return self.v.device.CreateSurface(self, desc, num_surfaces, usage, shared_resource, surface);
            }
            pub inline fn QueryResourceResidency(
                self: *T,
                resources: *const *IUnknown,
                status: [*]DXGI_RESIDENCY,
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
            GetAdapter: fn (self: *T, adapter: *?*IDXGIAdapter) callconv(WINAPI) HRESULT,
            CreateSurface: fn (
                *T,
                *const DXGI_SURFACE_DESC,
                UINT,
                DXGI_USAGE,
                ?*const DXGI_SHARED_RESOURCE,
                *?*IDXGISurface,
            ) callconv(WINAPI) HRESULT,
            QueryResourceResidency: fn (
                *T,
                *const *IUnknown,
                [*]DXGI_RESIDENCY,
                UINT,
            ) callconv(WINAPI) HRESULT,
            SetGPUThreadPriority: fn (self: *T, priority: INT) callconv(WINAPI) HRESULT,
            GetGPUThreadPriority: fn (self: *T, priority: *INT) callconv(WINAPI) HRESULT,
        };
    }
};

pub const DXGI_ADAPTER_DESC1 = extern struct {
    Description: [128]WCHAR,
    VendorId: UINT,
    DeviceId: UINT,
    SubSysId: UINT,
    Revision: UINT,
    DedicatedVideoMemory: SIZE_T,
    DedicatedSystemMemory: SIZE_T,
    SharedSystemMemory: SIZE_T,
    AdapterLuid: LUID,
    Flags: UINT,
};

pub const IDXGIFactory1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        factory: IDXGIFactory.VTable(Self),
        factory1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIFactory.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnumAdapters1(self: *T, index: UINT, adapter: *?*IDXGIAdapter1) HRESULT {
                return self.v.factory1.EnumAdapters1(self, index, adapter);
            }
            pub inline fn IsCurrent(self: *T) BOOL {
                return self.v.factory1.IsCurrent(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAdapters1: fn (*T, UINT, *?*IDXGIAdapter) callconv(WINAPI) HRESULT,
            IsCurrent: fn (*T) callconv(WINAPI) BOOL,
        };
    }
};

pub const IDXGIAdapter1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        adapter: IDXGIAdapter.VTable(Self),
        adapter1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIAdapter.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc1(self: *T, desc: *DXGI_ADAPTER_DESC1) HRESULT {
                return self.v.adapter1.GetDesc1(self, desc);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc1: fn (*T, *DXGI_ADAPTER_DESC1) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDXGIDevice1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        device: IDXGIDevice.VTable(Self),
        device1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDevice.Methods(Self);
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

pub const IID_IDXGIFactory1 = GUID{
    .Data1 = 0x770aae78,
    .Data2 = 0xf26f,
    .Data3 = 0x4dba,
    .Data4 = .{ 0xa8, 0x29, 0x25, 0x3c, 0x83, 0xd1, 0xb3, 0x87 },
};

pub const DXGI_CREATE_FACTORY_DEBUG = 0x1;
pub var CreateDXGIFactory2: fn (UINT, *const GUID, *?*c_void) callconv(WINAPI) HRESULT = undefined;

pub fn dxgi_load_dll() !void {
    // TODO(mziulek): Better error handling.
    var dxgi_dll = try std.DynLib.openZ("dxgi.dll");
    CreateDXGIFactory2 = dxgi_dll.lookup(@TypeOf(CreateDXGIFactory2), "CreateDXGIFactory2").?;
}
