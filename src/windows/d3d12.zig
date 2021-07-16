const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");
usingnamespace @import("dxgi.zig");
usingnamespace @import("d3dcommon.zig");

pub const D3D12_GPU_VIRTUAL_ADDRESS = UINT64;

pub const D3D12_HEAP_TYPE = enum(UINT) {
    DEFAULT = 1,
    UPLOAD = 2,
    READBACK = 3,
    CUSTOM = 4,
};

pub const D3D12_CPU_PAGE_PROPERTY = enum(UINT) {
    UNKNOWN = 0,
    NOT_AVAILABLE = 1,
    WRITE_COMBINE = 2,
    WRITE_BACK = 3,
};

pub const D3D12_MEMORY_POOL = enum(UINT) {
    UNKNOWN = 0,
    L0 = 1,
    L1 = 2,
};

pub const D3D12_HEAP_PROPERTIES = extern struct {
    Type: D3D12_HEAP_TYPE,
    CPUPageProperty: D3D12_CPU_PAGE_PROPERTY,
    MemoryPoolPreference: D3D12_MEMORY_POOL,
    CreationNodeMask: UINT,
    VisibleNodeMask: UINT,
};

pub const D3D12_HEAP_FLAGS = packed struct {
    SHARED: bool align(4) = false, // 0x1
    __reserved1: bool = false, // 0x2
    DENY_BUFFERS: bool = false, // 0x4
    ALLOW_DISPLAY: bool = false, // 0x8
    __reserved4: bool = false, // 0x10
    SHARED_CROSS_ADAPTER: bool = false, // 0x20
    DENY_RT_DS_TEXTURES: bool = false, // 0x40
    DENY_NON_RT_DS_TEXTURES: bool = false, // 0x80
    HARDWARE_PROTECTED: bool = false, // 0x100
    ALLOW_WRITE_WATCH: bool = false, // 0x200
    ALLOW_SHADER_ATOMICS: bool = false, // 0x400
    CREATE_NOT_RESIDENT: bool = false, // 0x800
    CREATE_NOT_ZEROED: bool = false, // 0x1000
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

    pub fn allowAllBuffersAndTextures() D3D12_HEAP_FLAGS {
        return .{};
    }
    pub fn allowOnlyBuffers() D3D12_HEAP_FLAGS {
        return .{ .DENY_RT_DS_TEXTURES = true, .DENY_NON_RT_DS_TEXTURES = true };
    }
    pub fn allowOnlyNonRtDsTextures() D3D12_HEAP_FLAGS {
        return .{ .DENY_BUFFERS = true, .DENY_RT_DS_TEXTURES = true };
    }
    pub fn allowOnlyRtDsTextures() D3D12_HEAP_FLAGS {
        return .{ .DENY_BUFFERS = true, .DENY_NON_RT_DS_TEXTURES = true };
    }
};
comptime {
    std.debug.assert(@sizeOf(D3D12_HEAP_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_HEAP_FLAGS) == 4);
}

pub const D3D12_HEAP_DESC = extern struct {
    SizeInBytes: UINT64,
    Properties: D3D12_HEAP_PROPERTIES,
    Alignment: UINT64,
    Flags: D3D12_HEAP_FLAGS,
};

pub const D3D12_RANGE = extern struct {
    Begin: UINT64,
    End: UINT64,
};

pub const D3D12_BOX = extern struct {
    left: UINT,
    top: UINT,
    front: UINT,
    right: UINT,
    bottom: UINT,
    back: UINT,
};

pub const D3D12_RESOURCE_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE2D = 3,
    TEXTURE3D = 4,
};

pub const D3D12_TEXTURE_LAYOUT = enum(UINT) {
    UNKNOWN = 0,
    ROW_MAJOR = 1,
    _64KB_UNDEFINED_SWIZZLE = 2,
    _64KB_STANDARD_SWIZZLE = 3,
};

pub const D3D12_RESOURCE_FLAGS = packed struct {
    ALLOW_RENDER_TARGET: bool align(4) = false, // 0x1
    ALLOW_DEPTH_STENCIL: bool = false, // 0x2
    ALLOW_UNORDERED_ACCESS: bool = false, // 0x4
    DENY_SHADER_RESOURCE: bool = false, // 0x8
    ALLOW_CROSS_ADAPTER: bool = false, // 0x10
    ALLOW_SIMULTANEOUS_ACCESS: bool = false, // 0x20
    VIDEO_DECODE_REFERENCE_ONLY: bool = false, // 0x40
    VIDEO_ENCODE_REFERENCE_ONLY: bool = false, // 0x80
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
    std.debug.assert(@sizeOf(D3D12_RESOURCE_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_RESOURCE_FLAGS) == 4);
}

pub const D3D12_RESOURCE_DESC = extern struct {
    Dimension: D3D12_RESOURCE_DIMENSION,
    Alignment: UINT64,
    Width: UINT64,
    Height: UINT,
    DepthOrArraySize: UINT16,
    MipLevels: UINT16,
    Format: DXGI_FORMAT,
    SampleDesc: DXGI_SAMPLE_DESC,
    Layout: D3D12_TEXTURE_LAYOUT,
    Flags: D3D12_RESOURCE_FLAGS,

    pub fn buffer(width: UINT64) D3D12_RESOURCE_DESC {
        return .{
            .Dimension = .BUFFER,
            .Alignment = 0,
            .Width = width,
            .Height = 1,
            .DepthOrArraySize = 1,
            .MipLevels = 1,
            .Format = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .Layout = .ROW_MAJOR,
            .Flags = .{},
        };
    }
};

pub const ID3D12Object = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: ?*c_void) HRESULT {
                return self.v.object.GetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateData(self: *T, guid: *const GUID, data_size: UINT, data: ?*const c_void) HRESULT {
                return self.v.object.SetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const os.IUnknown) HRESULT {
                return self.v.object.SetPrivateDataInterface(self, guid, data);
            }
            pub inline fn SetName(self: *T, name: ?os.LPCWSTR) HRESULT {
                return self.v.object.SetName(self, name);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetPrivateData: fn (*T, *const GUID, *UINT, ?*c_void) callconv(WINAPI) HRESULT,
            SetPrivateData: fn (*T, *const GUID, UINT, ?*const c_void) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            SetName: fn (*T, ?LPCWSTR) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12DeviceChild = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDevice(self: *T, guid: *const GUID, device: **c_void) HRESULT {
                return self.v.devchild.GetDevice(self, guid, device);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, **c_void) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12RootSignature = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
};

pub const ID3D12QueryHeap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
};

pub const ID3D12CommandSignature = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
};

pub const ID3D12Pageable = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
};

pub const ID3D12Heap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        heap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) D3D12_HEAP_DESC {
                var desc: D3D12_HEAP_DESC = undefined;
                self.v.heap.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *D3D12_HEAP_DESC) callconv(WINAPI) *D3D12_HEAP_DESC,
        };
    }
};

pub const ID3D12Resource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Map(self: *T, subresource: UINT, read_range: *const D3D12_RANGE, data: **c_void) HRESULT {
                return self.v.resource.Map(self, subresource, read_range, data);
            }
            pub inline fn Unmap(self: *T, subresource: UINT, written_range: *const D3D12_RANGE) void {
                self.v.resource.Unmap(self, subresource, written_range);
            }
            pub inline fn GetDesc(self: *T) D3D12_RESOURCE_DESC {
                var desc: D3D12_RESOURCE_DESC = undefined;
                _ = self.v.resource.GetDesc(self, &desc);
                return desc;
            }
            pub inline fn GetGPUVirtualAddress(self: *T) D3D12_GPU_VIRTUAL_ADDRESS {
                return self.v.resource.GetGPUVirtualAddress(self);
            }
            pub inline fn WriteToSubresource(
                self: *T,
                dst_subresource: UINT,
                dst_box: *const D3D12_BOX,
                src_data: *const c_void,
                src_row_pitch: UINT,
                src_depth_pitch: UINT,
            ) HRESULT {
                return self.v.resource.WriteToSubresource(
                    self,
                    dst_subresource,
                    dst_box,
                    src_data,
                    src_row_pitch,
                    src_depth_pitch,
                );
            }
            pub inline fn ReadFromSubresource(
                self: *T,
                dst_data: *c_void,
                dst_row_pitch: UINT,
                dst_depth_pitch: UINT,
                src_subresource: UINT,
                src_box: *const D3D12_BOX,
            ) HRESULT {
                return self.v.resource.ReadFromSubresource(
                    self,
                    dst_data,
                    dst_row_pitch,
                    dst_depth_pitch,
                    src_subresource,
                    src_box,
                );
            }
            pub inline fn GetHeapProperties(self: *T, properties: *D3D12_HEAP_PROPERTIES, flags: *D3D12_HEAP_FLAGS) HRESULT {
                return self.v.resource.GetHeapProperties(self, properties, flags);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Map: fn (*T, UINT, *const D3D12_RANGE, **c_void) callconv(WINAPI) HRESULT,
            Unmap: fn (*T, UINT, *const D3D12_RANGE) callconv(WINAPI) void,
            GetDesc: fn (*T, *D3D12_RESOURCE_DESC) callconv(WINAPI) *D3D12_RESOURCE_DESC,
            GetGPUVirtualAddress: fn (*T) callconv(WINAPI) D3D12_GPU_VIRTUAL_ADDRESS,
            WriteToSubresource: fn (*T, UINT, *const D3D12_BOX, *const c_void, UINT, UINT) callconv(WINAPI) HRESULT,
            ReadFromSubresource: fn (*T, *c_void, UINT, UINT, UINT, *const D3D12_BOX) callconv(WINAPI) HRESULT,
            GetHeapProperties: fn (*T, *D3D12_HEAP_PROPERTIES, *D3D12_HEAP_FLAGS) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12CommandAllocator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        alloc: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Reset(self: *T) HRESULT {
                return self.v.alloc.Reset(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Reset: fn (*T) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12Fence = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        fence: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCompletedValue(self: *T) UINT64 {
                return self.v.fence.GetCompletedValue(self);
            }
            pub inline fn SetEventOnCompletion(self: *T, value: UINT64, event: HANDLE) HRESULT {
                return self.v.fence.SetEventOnCompletion(self, value, event);
            }
            pub inline fn Signal(self: *T, value: UINT64) HRESULT {
                return self.v.fence.Signal(self, value);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetCompletedValue: fn (*T) callconv(WINAPI) UINT64,
            SetEventOnCompletion: fn (*Self, UINT64, HANDLE) callconv(WINAPI) HRESULT,
            Signal: fn (*Self, UINT64) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12PipelineState = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        pstate: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCachedBlob(self: *T, blob: **ID3DBlob) HRESULT {
                return self.v.pstate.GetCachedBlob(self, blob);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetCachedBlob: fn (*T, **ID3DBlob) callconv(WINAPI) HRESULT,
        };
    }
};

pub var D3D12CreateDevice: fn (
    ?*IUnknown,
    u32,
    *const GUID,
    **c_void,
) callconv(WINAPI) HRESULT = undefined;

pub const IID_ID3D12Device = GUID{
    .Data1 = 0x189819f1,
    .Data2 = 0x1db6,
    .Data3 = 0x4b57,
    .Data4 = .{ 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7 },
};

pub fn d3d12_load_dll() !void {
    var d3d12_dll = try std.DynLib.openZ("d3d12.dll");
    D3D12CreateDevice = d3d12_dll.lookup(@TypeOf(D3D12CreateDevice), "D3D12CreateDevice").?;
}
