const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");

pub const D3D12_GPU_BASED_VALIDATION_FLAGS = packed struct {
    DISABLE_STATE_TRACKING: bool align(4) = false, // 0x1
    __reserved1: bool = false,
    __reserved2: bool = false,
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
    std.debug.assert(@sizeOf(D3D12_GPU_BASED_VALIDATION_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_GPU_BASED_VALIDATION_FLAGS) == 4);
}

pub const ID3D12Debug = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnableDebugLayer(self: *T) void {
                self.v.debug.EnableDebugLayer(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            EnableDebugLayer: fn (*T) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Debug1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn EnableDebugLayer(self: *T) void {
                self.v.debug1.EnableDebugLayer(self);
            }
            pub inline fn SetEnableGPUBasedValidation(self: *T, enable: BOOL) void {
                self.v.debug1.SetEnableGPUBasedValidation(self, enable);
            }
            pub inline fn SetEnableSynchronizedCommandQueueValidation(self: *T, enable: BOOL) void {
                self.v.debug1.SetEnableSynchronizedCommandQueueValidation(self, enable);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            EnableDebugLayer: fn (*T) callconv(WINAPI) void,
            SetEnableGPUBasedValidation: fn (*T, BOOL) callconv(WINAPI) void,
            SetEnableSynchronizedCommandQueueValidation: fn (*T, BOOL) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Debug2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetGPUBasedValidationFlags(self: *T, flags: D3D12_GPU_BASED_VALIDATION_FLAGS) void {
                self.v.debug2.SetGPUBasedValidationFlags(self, flags);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetGPUBasedValidationFlags: fn (*T, D3D12_GPU_BASED_VALIDATION_FLAGS) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Debug3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug: ID3D12Debug.VTable(Self),
        debug3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Debug.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetEnableGPUBasedValidation(self: *T, enable: BOOL) void {
                self.v.debug3.SetEnableGPUBasedValidation(self, enable);
            }
            pub inline fn SetEnableSynchronizedCommandQueueValidation(self: *T, enable: BOOL) void {
                self.v.debug3.SetEnableSynchronizedCommandQueueValidation(self, enable);
            }
            pub inline fn SetGPUBasedValidationFlags(self: *T, flags: D3D12_GPU_BASED_VALIDATION_FLAGS) void {
                self.v.debug3.SetGPUBasedValidationFlags(self, flags);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetEnableGPUBasedValidation: fn (*T, BOOL) callconv(WINAPI) void,
            SetEnableSynchronizedCommandQueueValidation: fn (*T, BOOL) callconv(WINAPI) void,
            SetGPUBasedValidationFlags: fn (*T, D3D12_GPU_BASED_VALIDATION_FLAGS) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Debug4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug: ID3D12Debug.VTable(Self),
        debug3: ID3D12Debug3.VTable(Self),
        debug4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Debug.Methods(Self);
    usingnamespace ID3D12Debug3.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn DisableDebugLayer(self: *T) void {
                self.v.debug4.DisableDebugLayer(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            DisableDebugLayer: fn (*T) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Debug5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debug: ID3D12Debug.VTable(Self),
        debug3: ID3D12Debug3.VTable(Self),
        debug4: ID3D12Debug4.VTable(Self),
        debug5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Debug.Methods(Self);
    usingnamespace ID3D12Debug3.Methods(Self);
    usingnamespace ID3D12Debug4.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetEnableAutoName(self: *T, enable: BOOL) void {
                self.v.debug5.SetEnableAutoName(self, enable);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetEnableAutoName: fn (*T, BOOL) callconv(WINAPI) void,
        };
    }
};

pub const IID_ID3D12Debug = GUID{
    .Data1 = 0x344488b7,
    .Data2 = 0x6846,
    .Data3 = 0x474b,
    .Data4 = .{ 0xb9, 0x89, 0xf0, 0x27, 0x44, 0x82, 0x45, 0xe0 },
};
pub const IID_ID3D12Debug1 = GUID{
    .Data1 = 0xaffaa4ca,
    .Data2 = 0x63fe,
    .Data3 = 0x4d8e,
    .Data4 = .{ 0xb8, 0xad, 0x15, 0x90, 0x00, 0xaf, 0x43, 0x04 },
};
pub const IID_ID3D12Debug2 = GUID{
    .Data1 = 0x93a665c4,
    .Data2 = 0xa3b2,
    .Data3 = 0x4e5d,
    .Data4 = .{ 0xb6, 0x92, 0xa2, 0x6a, 0xe1, 0x4e, 0x33, 0x74 },
};
pub const IID_ID3D12Debug3 = GUID{
    .Data1 = 0x5cf4e58f,
    .Data2 = 0xf671,
    .Data3 = 0x4ff0,
    .Data4 = .{ 0xa5, 0x42, 0x36, 0x86, 0xe3, 0xd1, 0x53, 0xd1 },
};
pub const IID_ID3D12Debug4 = GUID{
    .Data1 = 0x014b816e,
    .Data2 = 0x9ec5,
    .Data3 = 0x4a2f,
    .Data4 = .{ 0xa8, 0x45, 0xff, 0xbe, 0x44, 0x1c, 0xe1, 0x3a },
};
pub const IID_ID3D12Debug5 = GUID{
    .Data1 = 0x548d6b12,
    .Data2 = 0x09fa,
    .Data3 = 0x40e0,
    .Data4 = .{ 0x90, 0x69, 0x5d, 0xcd, 0x58, 0x9a, 0x52, 0xc9 },
};
