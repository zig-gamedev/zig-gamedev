const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("d3dcommon.zig");
usingnamespace @import("d3d12sdklayers.zig");

pub const D3D12_GPU_VIRTUAL_ADDRESS = UINT64;

pub const D3D12_PRIMITIVE_TOPOLOGY = D3D_PRIMITIVE_TOPOLOGY;

pub const D3D12_CPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: UINT64,
};

pub const D3D12_GPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: UINT64,
};

pub const D3D12_PRIMITIVE_TOPOLOGY_TYPE = enum(UINT) {
    UNDEFINED = 0,
    POINT = 1,
    LINE = 2,
    TRIANGLE = 3,
    PATCH = 4,
};

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

pub const D3D12_FENCE_FLAGS = packed struct {
    SHARED: bool align(4) = false, // 0x1
    SHARED_CROSS_ADAPTER: bool = false, // 0x2
    NON_MONITORED: bool = false, // 0x4
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
    std.debug.assert(@sizeOf(D3D12_FENCE_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_FENCE_FLAGS) == 4);
}

pub const D3D12_DESCRIPTOR_HEAP_TYPE = enum(UINT) {
    CBV_SRV_UAV = 0,
    SAMPLER = 1,
    RTV = 2,
    DSV = 3,
};

pub const D3D12_DESCRIPTOR_HEAP_FLAGS = packed struct {
    SHADER_VISIBLE: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_DESCRIPTOR_HEAP_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_DESCRIPTOR_HEAP_FLAGS) == 4);
}

pub const D3D12_DESCRIPTOR_HEAP_DESC = extern struct {
    Type: D3D12_DESCRIPTOR_HEAP_TYPE,
    NumDescriptors: UINT,
    Flags: D3D12_DESCRIPTOR_HEAP_FLAGS,
    NodeMask: UINT,
};

pub const D3D12_COMMAND_LIST_TYPE = enum(UINT) {
    DIRECT = 0,
    BUNDLE = 1,
    COMPUTE = 2,
    COPY = 3,
    VIDEO_DECODE = 4,
    VIDEO_PROCESS = 5,
    VIDEO_ENCODE = 6,
};

pub const D3D12_RESOURCE_BARRIER_TYPE = enum(UINT) {
    TRANSITION = 0,
    ALIASING = 1,
    UAV = 2,
};

pub const D3D12_RESOURCE_TRANSITION_BARRIER = extern struct {
    pResource: *ID3D12Resource,
    Subresource: UINT,
    StateBefore: D3D12_RESOURCE_STATES,
    StateAfter: D3D12_RESOURCE_STATES,
};

pub const D3D12_RESOURCE_ALIASING_BARRIER = extern struct {
    pResourceBefore: *ID3D12Resource,
    pResourceAfter: *ID3D12Resource,
};

pub const D3D12_RESOURCE_UAV_BARRIER = extern struct {
    pResource: *ID3D12Resource,
};

pub const D3D12_RESOURCE_BARRIER_FLAGS = packed struct {
    BEGIN_ONLY: bool align(4) = false, // 0x1
    END_ONLY: bool = false, // 0x2
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
    std.debug.assert(@sizeOf(D3D12_RESOURCE_BARRIER_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_RESOURCE_BARRIER_FLAGS) == 4);
}

pub const D3D12_RESOURCE_BARRIER = extern struct {
    Type: D3D12_RESOURCE_BARRIER_TYPE,
    Flags: D3D12_RESOURCE_BARRIER_FLAGS,
    u: extern union {
        Transition: D3D12_RESOURCE_TRANSITION_BARRIER,
        Aliasing: D3D12_RESOURCE_ALIASING_BARRIER,
        UAV: D3D12_RESOURCE_UAV_BARRIER,
    },
};

pub const D3D12_SUBRESOURCE_FOOTPRINT = extern struct {
    Format: DXGI_FORMAT,
    Width: UINT,
    Height: UINT,
    Depth: UINT,
    RowPitch: UINT,
};

pub const D3D12_PLACED_SUBRESOURCE_FOOTPRINT = extern struct {
    Offset: UINT64,
    Footprint: D3D12_SUBRESOURCE_FOOTPRINT,
};

pub const D3D12_TEXTURE_COPY_TYPE = enum(UINT) {
    D3D12_SUBRESOURCE_INDEX = 0,
    D3D12_PLACED_FOOTPRINT = 1,
};

pub const D3D12_TEXTURE_COPY_LOCATION = extern struct {
    pResource: *ID3D12Resource,
    Type: D3D12_TEXTURE_COPY_TYPE,
    u: extern union {
        PlacedFootprint: D3D12_PLACED_SUBRESOURCE_FOOTPRINT,
        SubresourceIndex: UINT,
    },
};

pub const D3D12_TILED_RESOURCE_COORDINATE = extern struct {
    X: UINT,
    Y: UINT,
    Z: UINT,
    Subresource: UINT,
};

pub const D3D12_TILE_REGION_SIZE = extern struct {
    NumTiles: UINT,
    UseBox: BOOL,
    Width: UINT,
    Height: UINT16,
    Depth: UINT16,
};

pub const D3D12_TILE_RANGE_FLAGS = packed struct {
    NULL: bool align(4) = false, // 0x1
    SKIP: bool = false, // 0x2
    REUSE_SINGLE_TILE: bool = false, // 0x4
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
    std.debug.assert(@sizeOf(D3D12_TILE_RANGE_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_TILE_RANGE_FLAGS) == 4);
}

pub const D3D12_SUBRESOURCE_TILING = extern struct {
    WidthInTiles: UINT,
    HeightInTiles: UINT16,
    DepthInTiles: UINT16,
    StartTileIndexInOverallResource: UINT,
};

pub const D3D12_TILE_SHAPE = extern struct {
    WidthInTexels: UINT,
    HeightInTexels: UINT,
    DepthInTexels: UINT,
};

pub const D3D12_TILE_MAPPING_FLAGS = packed struct {
    NO_HAZARD: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_TILE_MAPPING_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_TILE_MAPPING_FLAGS) == 4);
}

pub const D3D12_TILE_COPY_FLAGS = packed struct {
    NO_HAZARD: bool align(4) = false, // 0x1
    LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE: bool = false, // 0x2
    SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER: bool = false, // 0x4
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
    std.debug.assert(@sizeOf(D3D12_TILE_COPY_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_TILE_COPY_FLAGS) == 4);
}

pub const D3D12_VIEWPORT = extern struct {
    TopLeftX: FLOAT,
    TopLeftY: FLOAT,
    Width: FLOAT,
    Height: FLOAT,
    MinDepth: FLOAT,
    MaxDepth: FLOAT,
};

pub const D3D12_RECT = RECT;

pub const D3D12_RESOURCE_STATES = packed struct {
    VERTEX_AND_CONSTANT_BUFFER: bool align(4) = false, // 0x1
    INDEX_BUFFER: bool = false, // 0x2
    RENDER_TARGET: bool = false, // 0x4
    UNORDERED_ACCESS: bool = false, // 0x8
    DEPTH_WRITE: bool = false, // 0x10
    DEPTH_READ: bool = false, // 0x20
    NON_PIXEL_SHADER_RESOURCE: bool = false, // 0x40
    PIXEL_SHADER_RESOURCE: bool = false, // 0x80
    STREAM_OUT: bool = false, // 0x100
    INDIRECT_ARGUMENT: bool = false, // 0x200
    COPY_DEST: bool = false, // 0x400
    COPY_SOURCE: bool = false, // 0x800
    RESOLVE_DEST: bool = false, // 0x1000
    RESOLVE_SOURCE: bool = false, // 0x2000
    __reserved14: bool = false, // 0x4000
    __reserved15: bool = false, // 0x8000
    VIDEO_DECODE_READ: bool = false, // 0x10000
    VIDEO_DECODE_WRITE: bool = false, // 0x20000
    VIDEO_PROCESS_READ: bool = false, // 0x40000
    VIDEO_PROCESS_WRITE: bool = false, // 0x80000
    __reserved20: bool = false, // 0x100000
    VIDEO_ENCODE_READ: bool = false, // 0x200000
    RAYTRACING_ACCELERATION_STRUCTURE: bool = false, // 0x400000
    VIDEO_ENCODE_WRITE: bool = false, // 0x800000
    SHADING_RATE_SOURCE: bool = false, // 0x1000000
    __reserved25: bool = false, // 0x2000000
    __reserved26: bool = false, // 0x4000000
    __reserved27: bool = false, // 0x8000000
    __reserved28: bool = false, // 0x10000000
    __reserved29: bool = false, // 0x20000000
    __reserved30: bool = false, // 0x40000000
    __reserved31: bool = false, // 0x80000000

    pub fn genericRead() D3D12_RESOURCE_STATES {
        return .{
            .VERTEX_AND_CONSTANT_BUFFER = true,
            .INDEX_BUFFER = true,
            .NON_PIXEL_SHADER_RESOURCE = true,
            .PIXEL_SHADER_RESOURCE = true,
            .INDIRECT_ARGUMENT = true,
            .COPY_SOURCE = true,
        };
    }
    pub fn predication() D3D12_RESOURCE_STATES {
        return .{ .INDIRECT_ARGUMENT = true };
    }
    pub fn allShaderResource() D3D12_RESOURCE_STATES {
        return .{ .NON_PIXEL_SHADER_RESOURCE = true, .PIXEL_SHADER_RESOURCE = true };
    }
};
comptime {
    std.debug.assert(@sizeOf(D3D12_RESOURCE_STATES) == 4);
    std.debug.assert(@alignOf(D3D12_RESOURCE_STATES) == 4);
}

pub const D3D12_INDEX_BUFFER_STRIP_CUT_VALUE = enum(UINT) {
    DISABLED = 0,
    _0xFFFF = 1,
    _0xFFFFFFFF = 2,
};

pub const D3D12_VERTEX_BUFFER_VIEW = extern struct {
    BufferLocation: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
    StrideInBytes: UINT,
};

pub const D3D12_INDEX_BUFFER_VIEW = extern struct {
    BufferLocation: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
    Format: DXGI_FORMAT,
};

pub const D3D12_STREAM_OUTPUT_BUFFER_VIEW = extern struct {
    BufferLocation: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
    BufferFilledSizeLocation: D3D12_GPU_VIRTUAL_ADDRESS,
};

pub const D3D12_CLEAR_FLAGS = packed struct {
    DEPTH: bool align(4) = false,
    STENCIL: bool = false,
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
    std.debug.assert(@sizeOf(D3D12_CLEAR_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_CLEAR_FLAGS) == 4);
}

pub const D3D12_DISCARD_REGION = extern struct {
    NumRects: UINT,
    pRects: *const D3D12_RECT,
    FirstSubresource: UINT,
    NumSubresources: UINT,
};

pub const D3D12_QUERY_HEAP_TYPE = enum(UINT) {
    OCCLUSION = 0,
    TIMESTAMP = 1,
    PIPELINE_STATISTICS = 2,
    SO_STATISTICS = 3,
};

pub const D3D12_QUERY_HEAP_DESC = extern struct {
    Type: D3D12_QUERY_HEAP_TYPE,
    Count: UINT,
    NodeMask: UINT,
};

pub const D3D12_QUERY_TYPE = enum(UINT) {
    OCCLUSION = 0,
    BINARY_OCCLUSION = 1,
    TIMESTAMP = 2,
    PIPELINE_STATISTICS = 3,
    SO_STATISTICS_STREAM0 = 4,
    SO_STATISTICS_STREAM1 = 5,
    SO_STATISTICS_STREAM2 = 6,
    SO_STATISTICS_STREAM3 = 7,
    VIDEO_DECODE_STATISTICS = 8,
    PIPELINE_STATISTICS1 = 10,
};

pub const D3D12_PREDICATION_OP = enum(UINT) {
    EQUAL_ZERO = 0,
    NOT_EQUAL_ZERO = 1,
};

pub const D3D12_INDIRECT_ARGUMENT_TYPE = enum(UINT) {
    DRAW = 0,
    DRAW_INDEXED = 1,
    DISPATCH = 2,
    VERTEX_BUFFER_VIEW = 3,
    INDEX_BUFFER_VIEW = 4,
    CONSTANT = 5,
    CONSTANT_BUFFER_VIEW = 6,
    SHADER_RESOURCE_VIEW = 7,
    UNORDERED_ACCESS_VIEW = 8,
    DISPATCH_RAYS = 9,
    DISPATCH_MESH = 10,
};

pub const D3D12_COMMAND_QUEUE_FLAGS = packed struct {
    DISABLE_GPU_TIMEOUT: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_COMMAND_QUEUE_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_COMMAND_QUEUE_FLAGS) == 4);
}

pub const D3D12_COMMAND_QUEUE_PRIORITY = enum(UINT) {
    NORMAL = 0,
    HIGH = 100,
    GLOBAL_REALTIME = 10000,
};

pub const D3D12_COMMAND_QUEUE_DESC = extern struct {
    Type: D3D12_COMMAND_LIST_TYPE,
    Priority: INT,
    Flags: D3D12_COMMAND_QUEUE_FLAGS,
    NodeMask: UINT,
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
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const IUnknown) HRESULT {
                return self.v.object.SetPrivateDataInterface(self, guid, data);
            }
            pub inline fn SetName(self: *T, name: LPCWSTR) HRESULT {
                return self.v.object.SetName(self, name);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetPrivateData: fn (*T, *const GUID, *UINT, ?*c_void) callconv(WINAPI) HRESULT,
            SetPrivateData: fn (*T, *const GUID, UINT, ?*const c_void) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            SetName: fn (*T, LPCWSTR) callconv(WINAPI) HRESULT,
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
            pub inline fn GetDevice(self: *T, guid: *const GUID, device: *?*c_void) HRESULT {
                return self.v.devchild.GetDevice(self, guid, device);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
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
            pub inline fn Map(self: *T, subresource: UINT, read_range: ?*const D3D12_RANGE, data: *?*c_void) HRESULT {
                return self.v.resource.Map(self, subresource, read_range, data);
            }
            pub inline fn Unmap(self: *T, subresource: UINT, written_range: ?*const D3D12_RANGE) void {
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
                dst_box: ?*const D3D12_BOX,
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
                src_box: ?*const D3D12_BOX,
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
            pub inline fn GetHeapProperties(
                self: *T,
                properties: ?*D3D12_HEAP_PROPERTIES,
                flags: ?*D3D12_HEAP_FLAGS,
            ) HRESULT {
                return self.v.resource.GetHeapProperties(self, properties, flags);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Map: fn (*T, UINT, ?*const D3D12_RANGE, *?*c_void) callconv(WINAPI) HRESULT,
            Unmap: fn (*T, UINT, ?*const D3D12_RANGE) callconv(WINAPI) void,
            GetDesc: fn (*T, *D3D12_RESOURCE_DESC) callconv(WINAPI) *D3D12_RESOURCE_DESC,
            GetGPUVirtualAddress: fn (*T) callconv(WINAPI) D3D12_GPU_VIRTUAL_ADDRESS,
            WriteToSubresource: fn (*T, UINT, ?*const D3D12_BOX, *const c_void, UINT, UINT) callconv(WINAPI) HRESULT,
            ReadFromSubresource: fn (*T, *c_void, UINT, UINT, UINT, ?*const D3D12_BOX) callconv(WINAPI) HRESULT,
            GetHeapProperties: fn (*T, ?*D3D12_HEAP_PROPERTIES, ?*D3D12_HEAP_FLAGS) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12Resource1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        resource: ID3D12Resource.VTable(Self),
        resource1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12Resource.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetProtectedResourceSession(self: *T, guid: *const GUID, session: *?*c_void) HRESULT {
                return self.v.resource1.GetProtectedResourceSession(self, guid, session);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetProtectedResourceSession: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
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
            SetEventOnCompletion: fn (*T, UINT64, HANDLE) callconv(WINAPI) HRESULT,
            Signal: fn (*T, UINT64) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ID3D12Fence1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        fence: ID3D12Fence.VTable(Self),
        fence1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12Fence.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCreationFlags(self: *T) D3D12_FENCE_FLAGS {
                return self.v.fence1.GetCreationFlags(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetCreationFlags: fn (*T) callconv(WINAPI) D3D12_FENCE_FLAGS,
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

pub const ID3D12DescriptorHeap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        dheap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) D3D12_DESCRIPTOR_HEAP_DESC {
                var desc: D3D12_DESCRIPTOR_HEAP_DESC = undefined;
                self.v.dheap.GetDesc(self, &desc);
                return desc;
            }
            pub inline fn GetCPUDescriptorHandleForHeapStart(self: *T) D3D12_CPU_DESCRIPTOR_HANDLE {
                var handle: D3D12_CPU_DESCRIPTOR_HANDLE = undefined;
                _ = self.v.dheap.GetCPUDescriptorHandleForHeapStart(self, &handle);
                return handle;
            }
            pub inline fn GetGPUDescriptorHandleForHeapStart(self: *T) D3D12_GPU_DESCRIPTOR_HANDLE {
                var handle: D3D12_GPU_DESCRIPTOR_HANDLE = undefined;
                _ = self.v.dheap.GetGPUDescriptorHandleForHeapStart(self, &handle);
                return handle;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *D3D12_DESCRIPTOR_HEAP_DESC) callconv(WINAPI) *D3D12_DESCRIPTOR_HEAP_DESC,
            GetCPUDescriptorHandleForHeapStart: fn (
                *T,
                *D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) *D3D12_CPU_DESCRIPTOR_HANDLE,
            GetGPUDescriptorHandleForHeapStart: fn (
                *T,
                *D3D12_GPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) *D3D12_GPU_DESCRIPTOR_HANDLE,
        };
    }
};

pub const ID3D12CommandList = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetType(self: *T) D3D12_COMMAND_LIST_TYPE {
                return self.v.cmdlist.GetType(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetType: fn (*T) callconv(WINAPI) D3D12_COMMAND_LIST_TYPE,
        };
    }
};

pub const ID3D12GraphicsCommandList = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Close(self: *T) HRESULT {
                return self.v.grcmdlist.Close(self);
            }
            pub inline fn Reset(self: *T, alloc: *ID3D12CommandAllocator, initial_state: ?*ID3D12PipelineState) HRESULT {
                return self.v.grcmdlist.Reset(self, alloc, initial_state);
            }
            pub inline fn ClearState(self: *T, pso: ?*ID3D12PipelineState) void {
                self.v.grcmdlist.ClearState(self, pso);
            }
            pub inline fn DrawInstanced(
                self: *T,
                vertex_count_per_instance: UINT,
                instance_count: UINT,
                start_vertex_location: UINT,
                start_instance_location: UINT,
            ) void {
                self.v.grcmdlist.DrawInstanced(
                    self,
                    vertex_count_per_instance,
                    instance_count,
                    start_vertex_location,
                    start_instance_location,
                );
            }
            pub inline fn DrawIndexedInstanced(
                self: *T,
                index_count_per_instance: UINT,
                instance_count: UINT,
                start_index_location: UINT,
                base_vertex_location: INT,
                start_instance_location: UINT,
            ) void {
                self.v.grcmdlist.DrawIndexedInstanced(
                    self,
                    index_count_per_instance,
                    instance_count,
                    start_index_location,
                    base_vertex_location,
                    start_instance_location,
                );
            }
            pub inline fn Dispatch(self: *T, count_x: UINT, count_y: UINT, count_z: UINT) void {
                self.v.grcmdlist.Dispatch(self, count_x, count_y, count_z);
            }
            pub inline fn CopyBufferRegion(
                self: *T,
                dst_buffer: *ID3D12Resource,
                dst_offset: UINT64,
                src_buffer: *ID3D12Resource,
                src_offset: UINT64,
                num_bytes: UINT64,
            ) void {
                self.v.grcmdlist.CopyBufferRegion(
                    self,
                    dst_buffer,
                    dst_offset,
                    src_buffer,
                    src_offset,
                    num_bytes,
                );
            }
            pub inline fn CopyTextureRegion(
                self: *T,
                dst: *const D3D12_TEXTURE_COPY_LOCATION,
                dst_x: UINT,
                dst_y: UINT,
                dst_z: UINT,
                src: *const D3D12_TEXTURE_COPY_LOCATION,
                src_box: ?*const D3D12_BOX,
            ) void {
                self.v.grcmdlist.CopyTextureRegion(self, dst, dst_x, dst_y, dst_z, src, src_box);
            }
            pub inline fn CopyResource(self: *T, dst: *ID3D12Resource, src: *ID3D12Resource) void {
                self.v.grcmdlist.CopyResource(self, dst, src);
            }
            pub inline fn CopyTiles(
                self: *T,
                tiled_resource: *ID3D12Resource,
                tile_region_start_coordinate: *const D3D12_TILED_RESOURCE_COORDINATE,
                tile_region_size: *const D3D12_TILE_REGION_SIZE,
                buffer: *ID3D12Resource,
                buffer_start_offset_in_bytes: UINT64,
                flags: D3D12_TILE_COPY_FLAGS,
            ) void {
                self.v.grcmdlist.CopyTiles(
                    self,
                    tiled_resource,
                    tile_region_start_coordinate,
                    tile_region_size,
                    buffer,
                    buffer_start_offset_in_bytes,
                    flags,
                );
            }
            pub inline fn ResolveSubresource(
                self: *T,
                dst_resource: *ID3D12Resource,
                dst_subresource: UINT,
                src_resource: *ID3D12Resource,
                src_subresource: UINT,
                format: DXGI_FORMAT,
            ) void {
                self.v.grcmdlist.ResolveSubresource(
                    self,
                    dst_resource,
                    dst_subresource,
                    src_resource,
                    src_subresource,
                    format,
                );
            }
            pub inline fn IASetPrimitiveTopology(self: *T, topology: D3D12_PRIMITIVE_TOPOLOGY) void {
                self.v.grcmdlist.IASetPrimitiveTopology(self, topology);
            }
            pub inline fn RSSetViewports(self: *T, num: UINT, viewports: [*]const D3D12_VIEWPORT) void {
                self.v.grcmdlist.RSSetViewports(self, num, viewports);
            }
            pub inline fn RSSetScissorRects(self: *T, num: UINT, rects: [*]const D3D12_RECT) void {
                self.v.grcmdlist.RSSetScissorRects(self, num, rects);
            }
            pub inline fn OMSetBlendFactor(self: *T, blend_factor: *const [4]FLOAT) void {
                self.v.grcmdlist.OMSetBlendFactor(self, blend_factor);
            }
            pub inline fn OMSetStencilRef(self: *T, stencil_ref: UINT) void {
                self.v.grcmdlist.OMSetStencilRef(self, stencil_ref);
            }
            pub inline fn SetPipelineState(self: *T, pso: *ID3D12PipelineState) void {
                self.v.grcmdlist.SetPipelineState(self, pso);
            }
            pub inline fn ResourceBarrier(self: *T, num: UINT, barriers: [*]const D3D12_RESOURCE_BARRIER) void {
                self.v.grcmdlist.ResourceBarrier(self, num, barriers);
            }
            pub inline fn ExecuteBundle(self: *T, cmdlist: *ID3D12GraphicsCommandList) void {
                self.v.grcmdlist.ExecuteBundle(self, cmdlist);
            }
            pub inline fn SetDescriptorHeaps(self: *T, num: UINT, heaps: [*]const *ID3D12DescriptorHeap) void {
                self.v.grcmdlist.SetDescriptorHeaps(self, num, heaps);
            }
            pub inline fn SetComputeRootSignature(self: *T, root_signature: ?*ID3D12RootSignature) void {
                self.v.grcmdlist.SetComputeRootSignature(self, root_signature);
            }
            pub inline fn SetGraphicsRootSignature(self: *T, root_signature: ?*ID3D12RootSignature) void {
                self.v.grcmdlist.SetGraphicsRootSignature(self, root_signature);
            }
            pub inline fn SetComputeRootDescriptorTable(
                self: *T,
                root_index: UINT,
                base_descriptor: D3D12_GPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.grcmdlist.SetComputeRootDescriptorTable(self, root_index, base_descriptor);
            }
            pub inline fn SetGraphicsRootDescriptorTable(
                self: *T,
                root_index: UINT,
                base_descriptor: D3D12_GPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.grcmdlist.SetGraphicsRootDescriptorTable(self, root_index, base_descriptor);
            }
            pub inline fn SetComputeRoot32BitConstant(self: *T, index: UINT, data: UINT, off: UINT) void {
                self.v.grcmdlist.SetComputeRoot32BitConstant(self, index, data, off);
            }
            pub inline fn SetGraphicsRoot32BitConstant(self: *T, index: UINT, data: UINT, off: UINT) void {
                self.v.grcmdlist.SetGraphicsRoot32BitConstant(self, index, data, off);
            }
            pub inline fn SetComputeRoot32BitConstants(
                self: *T,
                root_index: UINT,
                num: UINT,
                data: *const c_void,
                offset: UINT,
            ) void {
                self.v.grcmdlist.SetComputeRoot32BitConstants(self, root_index, num, data, offset);
            }
            pub inline fn SetGraphicsRoot32BitConstants(
                self: *T,
                root_index: UINT,
                num: UINT,
                data: *const c_void,
                offset: UINT,
            ) void {
                self.v.grcmdlist.SetGraphicsRoot32BitConstants(self, root_index, num, data, offset);
            }
            pub inline fn SetComputeRootConstantBufferView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootConstantBufferView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootConstantBufferView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootConstantBufferView(self, index, buffer_location);
            }
            pub inline fn SetComputeRootShaderResourceView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootShaderResourceView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootShaderResourceView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootShaderResourceView(self, index, buffer_location);
            }
            pub inline fn SetComputeRootUnorderedAccessView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootUnorderedAccessView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootUnorderedAccessView(
                self: *T,
                index: UINT,
                buffer_location: D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootUnorderedAccessView(self, index, buffer_location);
            }
            pub inline fn IASetIndexBuffer(self: *T, view: ?*const D3D12_INDEX_BUFFER_VIEW) void {
                self.v.grcmdlist.IASetIndexBuffer(self, view);
            }
            pub inline fn IASetVertexBuffers(
                self: *T,
                start_slot: UINT,
                num_views: UINT,
                views: ?[*]const VERTEX_BUFFER_VIEW,
            ) void {
                self.v.grcmdlist.IASetVertexBuffers(self, start_slot, num_views, views);
            }
            pub inline fn SOSetTargets(
                self: *T,
                start_slot: UINT,
                num_views: UINT,
                views: ?[*]const D3D12_STREAM_OUTPUT_BUFFER_VIEW,
            ) void {
                self.v.grcmdlist.SOSetTargets(self, start_slot, num_views, views);
            }
            pub inline fn OMSetRenderTargets(
                self: *T,
                num_rt_descriptors: UINT,
                rt_descriptors: ?[*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                single_handle: BOOL,
                ds_descriptors: ?*const D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.grcmdlist.OMSetRenderTargets(
                    self,
                    num_rt_descriptors,
                    rt_descriptors,
                    single_handle,
                    ds_descriptors,
                );
            }
            pub inline fn ClearDepthStencilView(
                self: *T,
                ds_view: D3D12_CPU_DESCRIPTOR_HANDLE,
                clear_flags: D3D12_CLEAR_FLAGS,
                depth: FLOAT,
                stencil: UINT8,
                num_rects: UINT,
                rects: ?[*]const D3D12_RECT,
            ) void {
                self.v.grcmdlist.ClearDepthStencilView(
                    self,
                    ds_view,
                    clear_flags,
                    depth,
                    stencil,
                    num_rects,
                    rects,
                );
            }
            pub inline fn ClearRenderTargetView(
                self: *T,
                rt_view: D3D12_CPU_DESCRIPTOR_HANDLE,
                rgba: *const [4]FLOAT,
                num_rects: UINT,
                rects: ?[*]const D3D12_RECT,
            ) void {
                self.v.grcmdlist.ClearRenderTargetView(self, rt_view, rgba, num_rects, rects);
            }
            pub inline fn ClearUnorderedAccessViewUint(
                self: *T,
                gpu_view: D3D12_GPU_DESCRIPTOR_HANDLE,
                cpu_view: D3D12_CPU_DESCRIPTOR_HANDLE,
                resource: *ID3D12Resource,
                values: *const [4]UINT,
                num_rects: UINT,
                rects: ?[*]const D3D12_RECT,
            ) void {
                self.v.grcmdlist.ClearUnorderedAccessViewUint(
                    self,
                    gpu_view,
                    cpu_view,
                    resource,
                    values,
                    num_rects,
                    rects,
                );
            }
            pub inline fn ClearUnorderedAccessViewFloat(
                self: *T,
                gpu_view: D3D12_GPU_DESCRIPTOR_HANDLE,
                cpu_view: D3D12_CPU_DESCRIPTOR_HANDLE,
                resource: *ID3D12Resource,
                values: *const [4]FLOAT,
                num_rects: UINT,
                rects: ?[*]const D3D12_RECT,
            ) void {
                self.v.grcmdlist.ClearUnorderedAccessViewFloat(
                    self,
                    gpu_view,
                    cpu_view,
                    resource,
                    values,
                    num_rects,
                    rects,
                );
            }
            pub inline fn DiscardResource(self: *T, resource: *ID3D12Resource, region: ?*const D3D12_DISCARD_REGION) void {
                self.v.grcmdlist.DiscardResource(self, resource, region);
            }
            pub inline fn BeginQuery(self: *T, query: *ID3D12QueryHeap, query_type: D3D12_QUERY_TYPE, index: UINT) void {
                self.v.grcmdlist.BeginQuery(self, query, query_type, index);
            }
            pub inline fn EndQuery(self: *T, query: *ID3D12QueryHeap, query_type: D3D12_QUERY_TYPE, index: UINT) void {
                self.v.grcmdlist.EndQuery(self, query, query_type, index);
            }
            pub inline fn ResolveQueryData(
                self: *T,
                query: *ID3D12QueryHeap,
                query_type: D3D12_QUERY_TYPE,
                start_index: UINT,
                num_queries: UINT,
                dst_resource: *ID3D12Resource,
                buffer_offset: UINT64,
            ) void {
                self.v.grcmdlist.ResolveQueryData(
                    self,
                    query,
                    query_type,
                    start_index,
                    num_queries,
                    dst_resource,
                    buffer_offset,
                );
            }
            pub inline fn SetPredication(
                self: *T,
                buffer: ?*ID3D12Resource,
                buffer_offset: UINT64,
                operation: D3D12_PREDICATION_OP,
            ) void {
                self.v.grcmdlist.SetPredication(self, buffer, buffer_offset, operation);
            }
            pub inline fn SetMarker(self: *T, metadata: UINT, data: ?*const c_void, size: UINT) void {
                self.v.grcmdlist.SetMarker(self, metadata, data, size);
            }
            pub inline fn BeginEvent(self: *T, metadata: UINT, data: ?*const c_void, size: UINT) void {
                self.v.grcmdlist.BeginEvent(self, metadata, data, size);
            }
            pub inline fn EndEvent(self: *T) void {
                self.v.grcmdlist.EndEvent(self);
            }
            pub inline fn ExecuteIndirect(
                self: *T,
                command_signature: *ID3D12CommandSignature,
                max_command_count: UINT,
                arg_buffer: *ID3D12Resource,
                arg_buffer_offset: UINT64,
                count_buffer: ?*ID3D12Resource,
                count_buffer_offset: UINT64,
            ) void {
                self.v.grcmdlist.ExecuteIndirect(
                    self,
                    command_signature,
                    max_command_count,
                    arg_buffer,
                    arg_buffer_offset,
                    count_buffer,
                    count_buffer_offset,
                );
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Close: fn (*T) callconv(.C) HRESULT,
            Reset: fn (*T, *ID3D12CommandAllocator, ?*ID3D12PipelineState) callconv(WINAPI) HRESULT,
            ClearState: fn (*T, ?*ID3D12PipelineState) callconv(WINAPI) void,
            DrawInstanced: fn (*T, UINT, UINT, UINT, UINT) callconv(WINAPI) void,
            DrawIndexedInstanced: fn (*T, UINT, UINT, UINT, INT, UINT) callconv(WINAPI) void,
            Dispatch: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            CopyBufferRegion: fn (*T, *ID3D12Resource, UINT64, *ID3D12Resource, UINT64, UINT64) callconv(WINAPI) void,
            CopyTextureRegion: fn (
                *T,
                *const D3D12_TEXTURE_COPY_LOCATION,
                UINT,
                UINT,
                UINT,
                *const D3D12_TEXTURE_COPY_LOCATION,
                ?*const D3D12_BOX,
            ) callconv(WINAPI) void,
            CopyResource: fn (*T, *ID3D12Resource, *ID3D12Resource) callconv(WINAPI) void,
            CopyTiles: fn (
                *T,
                *ID3D12Resource,
                *const D3D12_TILED_RESOURCE_COORDINATE,
                *const D3D12_TILE_REGION_SIZE,
                *ID3D12Resource,
                buffer_start_offset_in_bytes: UINT64,
                D3D12_TILE_COPY_FLAGS,
            ) callconv(WINAPI) void,
            ResolveSubresource: fn (*T, *ID3D12Resource, UINT, *ID3D12Resource, UINT, DXGI_FORMAT) callconv(WINAPI) void,
            IASetPrimitiveTopology: fn (*T, D3D12_PRIMITIVE_TOPOLOGY) callconv(WINAPI) void,
            RSSetViewports: fn (*T, UINT, [*]const D3D12_VIEWPORT) callconv(WINAPI) void,
            RSSetScissorRects: fn (*T, UINT, [*]const D3D12_RECT) callconv(WINAPI) void,
            OMSetBlendFactor: fn (*T, *const [4]FLOAT) callconv(WINAPI) void,
            OMSetStencilRef: fn (*T, UINT) callconv(WINAPI) void,
            SetPipelineState: fn (*T, *ID3D12PipelineState) callconv(WINAPI) void,
            ResourceBarrier: fn (*T, UINT, [*]const D3D12_RESOURCE_BARRIER) callconv(WINAPI) void,
            ExecuteBundle: fn (*T, *ID3D12GraphicsCommandList) callconv(WINAPI) void,
            SetDescriptorHeaps: fn (*T, UINT, [*]const *ID3D12DescriptorHeap) callconv(WINAPI) void,
            SetComputeRootSignature: fn (*T, ?*ID3D12RootSignature) callconv(WINAPI) void,
            SetGraphicsRootSignature: fn (*T, ?*ID3D12RootSignature) callconv(WINAPI) void,
            SetComputeRootDescriptorTable: fn (*T, UINT, D3D12_GPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            SetGraphicsRootDescriptorTable: fn (*T, UINT, D3D12_GPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            SetComputeRoot32BitConstant: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            SetGraphicsRoot32BitConstant: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            SetComputeRoot32BitConstants: fn (*T, UINT, UINT, *const c_void, UINT) callconv(WINAPI) void,
            SetGraphicsRoot32BitConstants: fn (*T, UINT, UINT, *const c_void, UINT) callconv(WINAPI) void,
            SetComputeRootConstantBufferView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootConstantBufferView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetComputeRootShaderResourceView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootShaderResourceView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetComputeRootUnorderedAccessView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootUnorderedAccessView: fn (*T, UINT, D3D12_GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            IASetIndexBuffer: fn (*T, ?*const D3D12_INDEX_BUFFER_VIEW) callconv(WINAPI) void,
            IASetVertexBuffers: fn (*T, UINT, UINT, ?[*]const D3D12_VERTEX_BUFFER_VIEW) callconv(WINAPI) void,
            SOSetTargets: fn (*T, UINT, UINT, ?[*]const D3D12_STREAM_OUTPUT_BUFFER_VIEW) callconv(WINAPI) void,
            OMSetRenderTargets: fn (
                *T,
                UINT,
                ?[*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                BOOL,
                ?*const D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            ClearDepthStencilView: fn (
                *T,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                D3D12_CLEAR_FLAGS,
                FLOAT,
                UINT8,
                UINT,
                ?[*]const D3D12_RECT,
            ) callconv(WINAPI) void,
            ClearRenderTargetView: fn (
                *T,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                *const [4]FLOAT,
                UINT,
                ?[*]const D3D12_RECT,
            ) callconv(WINAPI) void,
            ClearUnorderedAccessViewUint: fn (
                *T,
                D3D12_GPU_DESCRIPTOR_HANDLE,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                *ID3D12Resource,
                *const [4]UINT,
                UINT,
                ?[*]const D3D12_RECT,
            ) callconv(WINAPI) void,
            ClearUnorderedAccessViewFloat: fn (
                *T,
                D3D12_GPU_DESCRIPTOR_HANDLE,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                *ID3D12Resource,
                *const [4]FLOAT,
                UINT,
                ?[*]const D3D12_RECT,
            ) callconv(WINAPI) void,
            DiscardResource: fn (*T, *ID3D12Resource, ?*const D3D12_DISCARD_REGION) callconv(WINAPI) void,
            BeginQuery: fn (*T, *ID3D12QueryHeap, D3D12_QUERY_TYPE, UINT) callconv(WINAPI) void,
            EndQuery: fn (*T, *ID3D12QueryHeap, D3D12_QUERY_TYPE, UINT) callconv(WINAPI) void,
            ResolveQueryData: fn (
                *T,
                *ID3D12QueryHeap,
                D3D12_QUERY_TYPE,
                UINT,
                UINT,
                *ID3D12Resource,
                UINT64,
            ) callconv(WINAPI) void,
            SetPredication: fn (*T, ?*ID3D12Resource, UINT64, D3D12_PREDICATION_OP) callconv(WINAPI) void,
            SetMarker: fn (*T, UINT, ?*const c_void, UINT) callconv(WINAPI) void,
            BeginEvent: fn (*T, UINT, ?*const c_void, UINT) callconv(WINAPI) void,
            EndEvent: fn (*T) callconv(WINAPI) void,
            ExecuteIndirect: fn (
                *T,
                *ID3D12CommandSignature,
                UINT,
                *ID3D12Resource,
                UINT64,
                ?*ID3D12Resource,
                UINT64,
            ) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12CommandQueue = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12Object.VTable(Self),
        cmdqueue: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn UpdateTileMappings(
                self: *T,
                resource: *ID3D12Resource,
                num_resource_regions: UINT,
                resource_region_start_coordinates: ?[*]const D3D12_TILED_RESOURCE_COORDINATE,
                resource_region_sizes: ?[*]const D3D12_TILE_REGION_SIZE,
                heap: ?*ID3D12Heap,
                num_ranges: UINT,
                range_flags: ?[*]const D3D12_TILE_RANGE_FLAGS,
                heap_range_start_offsets: ?[*]const UINT,
                range_tile_counts: ?[*]const UINT,
                flags: D3D12_TILE_MAPPING_FLAGS,
            ) void {
                self.v.cmdqueue.UpdateTileMappings(
                    self,
                    resource,
                    num_resource_regions,
                    resource_region_start_coordinates,
                    resource_region_sizes,
                    heap,
                    num_ranges,
                    range_flags,
                    heap_range_start_offsets,
                    range_tile_counts,
                    flags,
                );
            }
            pub inline fn CopyTileMappings(
                self: *T,
                dst_resource: *ID3D12Resource,
                dst_region_start_coordinate: *const D3D12_TILED_RESOURCE_COORDINATE,
                src_resource: *IResource,
                src_region_start_coordinate: *const D3D12_TILED_RESOURCE_COORDINATE,
                region_size: *const D3D12_TILE_REGION_SIZE,
                flags: D3D12_TILE_MAPPING_FLAGS,
            ) void {
                self.v.cmdqueue.CopyTileMappings(
                    self,
                    dst_resource,
                    dst_region_start_coordinate,
                    src_resource,
                    src_region_start_coordinate,
                    region_size,
                    flags,
                );
            }
            pub inline fn ExecuteCommandLists(self: *T, num: UINT, cmdlists: [*]const *ID3D12CommandList) void {
                self.v.cmdqueue.ExecuteCommandLists(self, num, cmdlists);
            }
            pub inline fn SetMarker(self: *T, metadata: UINT, data: ?*const c_void, size: UINT) void {
                self.v.cmdqueue.SetMarker(self, metadata, data, size);
            }
            pub inline fn BeginEvent(self: *T, metadata: UINT, data: ?*const c_void, size: UINT) void {
                self.v.cmdqueue.BeginEvent(self, metadata, data, size);
            }
            pub inline fn EndEvent(self: *T) void {
                self.v.cmdqueue.EndEvent(self);
            }
            pub inline fn Signal(self: *T, fence: *ID3D12Fence, value: UINT64) HRESULT {
                return self.v.cmdqueue.Signal(self, fence, value);
            }
            pub inline fn Wait(self: *T, fence: *ID3D12Fence, value: UINT64) HRESULT {
                return self.v.cmdqueue.Wait(self, fence, value);
            }
            pub inline fn GetTimestampFrequency(self: *T, frequency: *UINT64) HRESULT {
                return self.v.cmdqueue.GetTimestampFrequency(self, frequency);
            }
            pub inline fn GetClockCalibration(self: *T, gpu_timestamp: *UINT64, cpu_timestamp: *UINT64) HRESULT {
                return self.v.cmdqueue.GetClockCalibration(self, gpu_timestamp, cpu_timestamp);
            }
            pub inline fn GetDesc(self: *T) D3D12_COMMAND_QUEUE_DESC {
                var desc: COMMAND_QUEUE_DESC = undefined;
                self.v.cmdqueue.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            UpdateTileMappings: fn (
                *T,
                *ID3D12Resource,
                UINT,
                ?[*]const D3D12_TILED_RESOURCE_COORDINATE,
                ?[*]const D3D12_TILE_REGION_SIZE,
                *ID3D12Heap,
                UINT,
                ?[*]const D3D12_TILE_RANGE_FLAGS,
                ?[*]const UINT,
                ?[*]const UINT,
                D3D12_TILE_MAPPING_FLAGS,
            ) callconv(WINAPI) void,
            CopyTileMappings: fn (
                *T,
                *ID3D12Resource,
                *const D3D12_TILED_RESOURCE_COORDINATE,
                *ID3D12Resource,
                *const D3D12_TILED_RESOURCE_COORDINATE,
                *const D3D12_TILE_REGION_SIZE,
                D3D12_TILE_MAPPING_FLAGS,
            ) callconv(WINAPI) void,
            ExecuteCommandLists: fn (*T, UINT, [*]const *ID3D12CommandList) callconv(WINAPI) void,
            SetMarker: fn (*T, UINT, ?*const c_void, UINT) callconv(WINAPI) void,
            BeginEvent: fn (*T, UINT, ?*const c_void, UINT) callconv(WINAPI) void,
            EndEvent: fn (*T) callconv(WINAPI) void,
            Signal: fn (*T, *ID3D12Fence, UINT64) callconv(WINAPI) HRESULT,
            Wait: fn (*T, *ID3D12Fence, UINT64) callconv(WINAPI) HRESULT,
            GetTimestampFrequency: fn (*T, *UINT64) callconv(WINAPI) HRESULT,
            GetClockCalibration: fn (*T, *UINT64, *UINT64) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *D3D12_COMMAND_QUEUE_DESC) callconv(WINAPI) *D3D12_COMMAND_QUEUE_DESC,
        };
    }
};

pub var D3D12GetDebugInterface: fn (*const GUID, *?*c_void) callconv(WINAPI) HRESULT = undefined;
pub var D3D12CreateDevice: fn (
    ?*IUnknown,
    u32,
    *const GUID,
    *?*c_void,
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
    D3D12GetDebugInterface = d3d12_dll.lookup(@TypeOf(D3D12GetDebugInterface), "D3D12GetDebugInterface").?;
}
