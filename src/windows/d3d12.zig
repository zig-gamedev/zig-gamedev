const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("d3dcommon.zig");
usingnamespace @import("d3d12sdklayers.zig");

pub const D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES = 0xffff_ffff;

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

pub const D3D12_INDIRECT_ARGUMENT_DESC = extern struct {
    Type: D3D12_INDIRECT_ARGUMENT_TYPE,
    u: extern union {
        VertexBuffer: extern struct {
            Slot: UINT,
        },
        Constant: extern struct {
            RootParameterIndex: UINT,
            DestOffsetIn32BitValues: UINT,
            Num32BitValuesToSet: UINT,
        },
        ConstantBufferView: extern struct {
            RootParameterIndex: UINT,
        },
        ShaderResourceView: extern struct {
            RootParameterIndex: UINT,
        },
        UnorderedAccessView: extern struct {
            RootParameterIndex: UINT,
        },
    },
};

pub const D3D12_COMMAND_SIGNATURE_DESC = extern struct {
    ByteStride: UINT,
    NumArgumentDescs: UINT,
    pArgumentDescs: *const D3D12_INDIRECT_ARGUMENT_DESC,
    NodeMask: UINT,
};

pub const D3D12_PACKED_MIP_INFO = extern struct {
    NumStandardMips: UINT8,
    NumPackedMips: UINT8,
    NumTilesForPackedMips: UINT,
    StartTileIndexInOverallResource: UINT,
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

pub const D3D12_SHADER_BYTECODE = extern struct {
    pShaderBytecode: ?*const c_void,
    BytecodeLength: UINT64,
};

pub const D3D12_SO_DECLARATION_ENTRY = extern struct {
    Stream: UINT,
    SemanticName: LPCSTR,
    SemanticIndex: UINT,
    StartComponent: UINT8,
    ComponentCount: UINT8,
    OutputSlot: UINT8,
};

pub const D3D12_STREAM_OUTPUT_DESC = extern struct {
    pSODeclaration: ?[*]const D3D12_SO_DECLARATION_ENTRY,
    NumEntries: UINT,
    pBufferStrides: ?[*]const UINT,
    NumStrides: UINT,
    RasterizedStream: UINT,
};

pub const D3D12_BLEND = enum(UINT) {
    ZERO = 1,
    ONE = 2,
    SRC_COLOR = 3,
    INV_SRC_COLOR = 4,
    SRC_ALPHA = 5,
    INV_SRC_ALPHA = 6,
    DEST_ALPHA = 7,
    INV_DEST_ALPHA = 8,
    DEST_COLOR = 9,
    INV_DEST_COLOR = 10,
    SRC_ALPHA_SAT = 11,
    BLEND_FACTOR = 14,
    INV_BLEND_FACTOR = 15,
    SRC1_COLOR = 16,
    INV_SRC1_COLOR = 17,
    SRC1_ALPHA = 18,
    INV_SRC1_ALPHA = 19,
};

pub const D3D12_BLEND_OP = enum(UINT) {
    ADD = 1,
    SUBTRACT = 2,
    REV_SUBTRACT = 3,
    MIN = 4,
    MAX = 5,
};

pub const D3D12_COLOR_WRITE_ENABLE = packed struct {
    RED: bool align(4) = false, // 0x1
    GREEN: bool = false, // 0x2
    BLUE: bool = false, // 0x4
    ALPHA: bool = false, // 0x8
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

    pub fn all() D3D12_COLOR_WRITE_ENABLE {
        return .{ .RED = true, .GREEN = true, .BLUE = true, .ALPHA = true };
    }
};
comptime {
    std.debug.assert(@sizeOf(D3D12_COLOR_WRITE_ENABLE) == 4);
    std.debug.assert(@alignOf(D3D12_COLOR_WRITE_ENABLE) == 4);
}

pub const D3D12_LOGIC_OP = enum(UINT) {
    CLEAR = 0,
    SET = 1,
    COPY = 2,
    COPY_INVERTED = 3,
    NOOP = 4,
    INVERT = 5,
    AND = 6,
    NAND = 7,
    OR = 8,
    NOR = 9,
    XOR = 10,
    EQUIV = 11,
    AND_REVERSE = 12,
    AND_INVERTED = 13,
    OR_REVERSE = 14,
    OR_INVERTED = 15,
};

pub const D3D12_RENDER_TARGET_BLEND_DESC = extern struct {
    BlendEnable: BOOL,
    LogicOpEnable: BOOL,
    SrcBlend: D3D12_BLEND,
    DestBlend: D3D12_BLEND,
    BlendOp: D3D12_BLEND_OP,
    SrcBlendAlpha: D3D12_BLEND,
    DestBlendAlpha: D3D12_BLEND,
    BlendOpAlpha: D3D12_BLEND_OP,
    LogicOp: D3D12_LOGIC_OP,
    RenderTargetWriteMask: UINT8,
};

pub const D3D12_BLEND_DESC = extern struct {
    AlphaToCoverageEnable: BOOL,
    IndependentBlendEnable: BOOL,
    RenderTarget: [8]D3D12_RENDER_TARGET_BLEND_DESC,
};

pub const D3D12_RASTERIZER_DESC = extern struct {
    FillMode: D3D12_FILL_MODE,
    CullMode: D3D12_CULL_MODE,
    FrontCounterClockwise: BOOL,
    DepthBias: INT,
    DepthBiasClamp: FLOAT,
    SlopeScaledDepthBias: FLOAT,
    DepthClipEnable: BOOL,
    MultisampleEnable: BOOL,
    AntialiasedLineEnable: BOOL,
    ForcedSampleCount: UINT,
    ConservativeRaster: D3D12_CONSERVATIVE_RASTERIZATION_MODE,
};

pub const D3D12_FILL_MODE = enum(UINT) {
    WIREFRAME = 2,
    SOLID = 3,
};

pub const D3D12_CULL_MODE = enum(UINT) {
    NONE = 1,
    FRONT = 2,
    BACK = 3,
};

pub const D3D12_CONSERVATIVE_RASTERIZATION_MODE = enum(UINT) {
    OFF = 0,
    ON = 1,
};

pub const D3D12_COMPARISON_FUNC = enum(UINT) {
    NEVER = 1,
    LESS = 2,
    EQUAL = 3,
    LESS_EQUAL = 4,
    GREATER = 5,
    NOT_EQUAL = 6,
    GREATER_EQUAL = 7,
    ALWAYS = 8,
};

pub const D3D12_DEPTH_WRITE_MASK = enum(UINT) {
    ZERO = 0,
    ALL = 1,
};

pub const D3D12_STENCIL_OP = enum(UINT) {
    KEEP = 1,
    ZERO = 2,
    REPLACE = 3,
    INCR_SAT = 4,
    DECR_SAT = 5,
    INVERT = 6,
    INCR = 7,
    DECR = 8,
};

pub const D3D12_DEPTH_STENCILOP_DESC = extern struct {
    StencilFailOp: D3D12_STENCIL_OP,
    StencilDepthFailOp: D3D12_STENCIL_OP,
    StencilPassOp: D3D12_STENCIL_OP,
    StencilFunc: D3D12_COMPARISON_FUNC,
};

pub const D3D12_DEPTH_STENCIL_DESC = extern struct {
    DepthEnable: BOOL,
    DepthWriteMask: D3D12_DEPTH_WRITE_MASK,
    DepthFunc: D3D12_COMPARISON_FUNC,
    StencilEnable: BOOL,
    StencilReadMask: UINT8,
    StencilWriteMask: UINT8,
    FrontFace: D3D12_DEPTH_STENCILOP_DESC,
    BackFace: D3D12_DEPTH_STENCILOP_DESC,
};

pub const D3D12_INPUT_LAYOUT_DESC = extern struct {
    pInputElementDescs: ?[*]const D3D12_INPUT_ELEMENT_DESC,
    NumElements: UINT,
};

pub const D3D12_INPUT_CLASSIFICATION = enum(UINT) {
    PER_VERTEX_DATA = 0,
    PER_INSTANCE_DATA = 1,
};

pub const D3D12_INPUT_ELEMENT_DESC = extern struct {
    SemanticName: LPCSTR,
    SemanticIndex: UINT,
    Format: DXGI_FORMAT,
    InputSlot: UINT,
    AlignedByteOffset: UINT,
    InputSlotClass: D3D12_INPUT_CLASSIFICATION,
    InstanceDataStepRate: UINT,
};

pub const D3D12_CACHED_PIPELINE_STATE = extern struct {
    pCachedBlob: ?*const c_void,
    CachedBlobSizeInBytes: UINT64,
};

pub const D3D12_PIPELINE_STATE_FLAGS = packed struct {
    TOOL_DEBUG: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_PIPELINE_STATE_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_PIPELINE_STATE_FLAGS) == 4);
}

pub const D3D12_GRAPHICS_PIPELINE_STATE_DESC = extern struct {
    pRootSignature: ?*ID3D12RootSignature,
    VS: D3D12_SHADER_BYTECODE,
    PS: D3D12_SHADER_BYTECODE,
    DS: D3D12_SHADER_BYTECODE,
    HS: D3D12_SHADER_BYTECODE,
    GS: D3D12_SHADER_BYTECODE,
    StreamOutput: D3D12_STREAM_OUTPUT_DESC,
    BlendState: D3D12_BLEND_DESC,
    SampleMask: UINT,
    RasterizerState: D3D12_RASTERIZER_DESC,
    DepthStencilState: D3D12_DEPTH_STENCIL_DESC,
    InputLayout: D3D12_INPUT_LAYOUT_DESC,
    IBStripCutValue: D3D12_INDEX_BUFFER_STRIP_CUT_VALUE,
    PrimitiveTopologyType: D3D12_PRIMITIVE_TOPOLOGY_TYPE,
    NumRenderTargets: UINT,
    RTVFormats: [8]DXGI_FORMAT,
    DSVFormat: DXGI_FORMAT,
    SampleDesc: DXGI_SAMPLE_DESC,
    NodeMask: UINT,
    CachedPSO: D3D12_CACHED_PIPELINE_STATE,
    Flags: D3D12_PIPELINE_STATE_FLAGS,
};

pub const D3D12_COMPUTE_PIPELINE_STATE_DESC = extern struct {
    pRootSignature: ?*ID3D12RootSignature,
    CS: D3D12_SHADER_BYTECODE,
    NodeMask: UINT,
    CachedPSO: D3D12_CACHED_PIPELINE_STATE,
    Flags: D3D12_PIPELINE_STATE_FLAGS,
};

pub const D3D12_FEATURE = enum(UINT) {
    D3D12_OPTIONS = 0,
    ARCHITECTURE = 1,
    FEATURE_LEVELS = 2,
    FORMAT_SUPPORT = 3,
    MULTISAMPLE_QUALITY_LEVELS = 4,
    FORMAT_INFO = 5,
    GPU_VIRTUAL_ADDRESS_SUPPORT = 6,
    SHADER_MODEL = 7,
    D3D12_OPTIONS1 = 8,
    PROTECTED_RESOURCE_SESSION_SUPPORT = 10,
    ROOT_SIGNATURE = 12,
    ARCHITECTURE1 = 16,
    D3D12_OPTIONS2 = 18,
    SHADER_CACHE = 19,
    COMMAND_QUEUE_PRIORITY = 20,
    D3D12_OPTIONS3 = 21,
    EXISTING_HEAPS = 22,
    D3D12_OPTIONS4 = 23,
    SERIALIZATION = 24,
    CROSS_NODE = 25,
    D3D12_OPTIONS5 = 27,
    DISPLAYABLE = 28,
    D3D12_OPTIONS6 = 30,
    QUERY_META_COMMAND = 31,
    D3D12_OPTIONS7 = 32,
    PROTECTED_RESOURCE_SESSION_TYPE_COUNT = 33,
    PROTECTED_RESOURCE_SESSION_TYPES = 34,
    D3D12_OPTIONS8 = 36,
    D3D12_OPTIONS9 = 37,
    D3D12_OPTIONS10 = 39,
    D3D12_OPTIONS11 = 40,
};

pub const D3D12_CONSTANT_BUFFER_VIEW_DESC = extern struct {
    BufferLocation: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
};

pub inline fn D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(src0: UINT, src1: UINT, src2: UINT, src3: UINT) UINT {
    return (src0 & 0x7) | ((src1 & 0x7) << 3) | ((src2 & 0x7) << (3 * 2)) | ((src3 & 0x7) << (3 * 3)) | (1 << (3 * 4));
}
pub const D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING = D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 1, 2, 3);

pub const D3D12_BUFFER_SRV_FLAGS = packed struct {
    RAW: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_BUFFER_SRV_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_BUFFER_SRV_FLAGS) == 4);
}

pub const D3D12_BUFFER_SRV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
    StructureByteStride: UINT,
    Flags: D3D12_BUFFER_SRV_FLAGS,
};

pub const D3D12_TEX1D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEX1D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEX2D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    PlaneSlice: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEX2D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEX3D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEXCUBE_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEXCUBE_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    First2DArrayFace: UINT,
    NumCubes: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const D3D12_TEX2DMS_SRV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const D3D12_TEX2DMS_ARRAY_SRV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_SRV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE1DARRAY = 3,
    TEXTURE2D = 4,
    TEXTURE2DARRAY = 5,
    TEXTURE2DMS = 6,
    TEXTURE2DMSARRAY = 7,
    TEXTURE3D = 8,
    TEXTURECUBE = 9,
    TEXTURECUBEARRAY = 10,
};

pub const D3D12_SHADER_RESOURCE_VIEW_DESC = extern struct {
    Format: DXGI_FORMAT,
    ViewDimension: D3D12_SRV_DIMENSION,
    Shader4ComponentMapping: UINT,
    u: extern union {
        Buffer: D3D12_BUFFER_SRV,
        Texture1D: D3D12_TEX1D_SRV,
        Texture1DArray: D3D12_TEX1D_ARRAY_SRV,
        Texture2D: D3D12_TEX2D_SRV,
        Texture2DArray: D3D12_TEX2D_ARRAY_SRV,
        Texture2DMS: D3D12_TEX2DMS_SRV,
        Texture2DMSArray: D3D12_TEX2DMS_ARRAY_SRV,
        Texture3D: D3D12_TEX3D_SRV,
        TextureCube: D3D12_TEXCUBE_SRV,
        TextureCubeArray: D3D12_TEXCUBE_ARRAY_SRV,
    },

    pub fn typedBuffer(format: DXGI_FORMAT, first_element: UINT64, num_elements: UINT) D3D12_SHADER_RESOURCE_VIEW_DESC {
        return .{
            .Format = format,
            .ViewDimension = .BUFFER,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = 0,
                },
            },
        };
    }

    pub fn structuredBuffer(first_element: UINT64, num_elements: UINT, stride: UINT) D3D12_SHADER_RESOURCE_VIEW_DESC {
        return .{
            .ViewDimension = .BUFFER,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = stride,
                },
            },
        };
    }
};

pub const D3D12_FILTER = enum(UINT) {
    MIN_MAG_MIP_POINT = 0,
    MIN_MAG_POINT_MIP_LINEAR = 0x1,
    MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
    MIN_POINT_MAG_MIP_LINEAR = 0x5,
    MIN_LINEAR_MAG_MIP_POINT = 0x10,
    MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
    MIN_MAG_LINEAR_MIP_POINT = 0x14,
    MIN_MAG_MIP_LINEAR = 0x15,
    ANISOTROPIC = 0x55,
    COMPARISON_MIN_MAG_MIP_POINT = 0x80,
    COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
    COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
    COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
    COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
    COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
    COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
    COMPARISON_ANISOTROPIC = 0xd5,
    MINIMUM_MIN_MAG_MIP_POINT = 0x100,
    MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
    MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
    MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
    MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
    MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
    MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
    MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
    MINIMUM_ANISOTROPIC = 0x155,
    MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
    MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
    MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
    MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
    MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
    MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
    MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
    MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
    MAXIMUM_ANISOTROPIC = 0x1d5,
};

pub const D3D12_FILTER_TYPE = enum(UINT) {
    POINT = 0,
    LINEAR = 1,
};

pub const D3D12_FILTER_REDUCTION_TYPE = enum(UINT) {
    STANDARD = 0,
    COMPARISON = 1,
    MINIMUM = 2,
    MAXIMUM = 3,
};

pub const D3D12_TEXTURE_ADDRESS_MODE = enum(UINT) {
    WRAP = 1,
    MIRROR = 2,
    CLAMP = 3,
    BORDER = 4,
    MIRROR_ONCE = 5,
};

pub const D3D12_SAMPLER_DESC = extern struct {
    Filter: D3D12_FILTER,
    AddressU: D3D12_TEXTURE_ADDRESS_MODE,
    AddressV: D3D12_TEXTURE_ADDRESS_MODE,
    AddressW: D3D12_TEXTURE_ADDRESS_MODE,
    MipLODBias: FLOAT,
    MaxAnisotropy: UINT,
    ComparisonFunc: D3D12_COMPARISON_FUNC,
    BorderColor: [4]FLOAT,
    MinLOD: FLOAT,
    MaxLOD: FLOAT,
};

pub const D3D12_BUFFER_UAV_FLAGS = packed struct {
    RAW: bool align(4) = false, // 0x1
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
    std.debug.assert(@sizeOf(D3D12_BUFFER_UAV_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_BUFFER_UAV_FLAGS) == 4);
}

pub const D3D12_BUFFER_UAV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
    StructureByteStride: UINT,
    CounterOffsetInBytes: UINT64,
    Flags: D3D12_BUFFER_UAV_FLAGS,
};

pub const D3D12_TEX1D_UAV = extern struct {
    MipSlice: UINT,
};

pub const D3D12_TEX1D_ARRAY_UAV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_TEX2D_UAV = extern struct {
    MipSlice: UINT,
    PlaneSlice: UINT,
};

pub const D3D12_TEX2D_ARRAY_UAV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
};

pub const D3D12_TEX3D_UAV = extern struct {
    MipSlice: UINT,
    FirstWSlice: UINT,
    WSize: UINT,
};

pub const D3D12_UAV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE1DARRAY = 3,
    TEXTURE2D = 4,
    TEXTURE2DARRAY = 5,
    TEXTURE3D = 8,
};

pub const D3D12_UNORDERED_ACCESS_VIEW_DESC = extern struct {
    Format: DXGI_FORMAT,
    ViewDimension: D3D12_UAV_DIMENSION,
    u: extern union {
        Buffer: D3D12_BUFFER_UAV,
        Texture1D: D3D12_TEX1D_UAV,
        Texture1DArray: D3D12_TEX1D_ARRAY_UAV,
        Texture2D: D3D12_TEX2D_UAV,
        Texture2DArray: D3D12_TEX2D_ARRAY_UAV,
        Texture3D: D3D12_TEX3D_UAV,
    },
};

pub const D3D12_BUFFER_RTV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
};

pub const D3D12_TEX1D_RTV = extern struct {
    MipSlice: UINT,
};

pub const D3D12_TEX1D_ARRAY_RTV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_TEX2D_RTV = extern struct {
    MipSlice: UINT,
    PlaneSlice: UINT,
};

pub const D3D12_TEX2DMS_RTV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const D3D12_TEX2D_ARRAY_RTV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
};

pub const D3D12_TEX2DMS_ARRAY_RTV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_TEX3D_RTV = extern struct {
    MipSlice: UINT,
    FirstWSlice: UINT,
    WSize: UINT,
};

pub const D3D12_RTV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE1DARRAY = 3,
    TEXTURE2D = 4,
    TEXTURE2DARRAY = 5,
    TEXTURE2DMS = 6,
    TEXTURE2DMSARRAY = 7,
    TEXTURE3D = 8,
};

pub const D3D12_RENDER_TARGET_VIEW_DESC = extern struct {
    Format: DXGI_FORMAT,
    ViewDimension: D3D12_RTV_DIMENSION,
    u: extern union {
        Buffer: D3D12_BUFFER_RTV,
        Texture1D: D3D12_TEX1D_RTV,
        Texture1DArray: D3D12_TEX1D_ARRAY_RTV,
        Texture2D: D3D12_TEX2D_RTV,
        Texture2DArray: D3D12_TEX2D_ARRAY_RTV,
        Texture2DMS: D3D12_TEX2DMS_RTV,
        Texture2DMSArray: D3D12_TEX2DMS_ARRAY_RTV,
        Texture3D: D3D12_TEX3D_RTV,
    },
};

pub const D3D12_TEX1D_DSV = extern struct {
    MipSlice: UINT,
};

pub const D3D12_TEX1D_ARRAY_DSV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_TEX2D_DSV = extern struct {
    MipSlice: UINT,
};

pub const D3D12_TEX2D_ARRAY_DSV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_TEX2DMS_DSV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const D3D12_TEX2DMS_ARRAY_DSV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const D3D12_DSV_FLAGS = packed struct {
    READ_ONLY_DEPTH: bool align(4) = false, // 0x1
    READ_ONLY_STENCIL: bool = false, // 0x2
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
    std.debug.assert(@sizeOf(D3D12_DSV_FLAGS) == 4);
    std.debug.assert(@alignOf(D3D12_DSV_FLAGS) == 4);
}

pub const D3D12_DSV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    TEXTURE1D = 1,
    TEXTURE1DARRAY = 2,
    TEXTURE2D = 3,
    TEXTURE2DARRAY = 4,
    TEXTURE2DMS = 5,
    TEXTURE2DMSARRAY = 6,
};

pub const D3D12_DEPTH_STENCIL_VIEW_DESC = extern struct {
    Format: DXGI_FORMAT,
    ViewDimension: D3D12_DSV_DIMENSION,
    Flags: D3D12_DSV_FLAGS,
    u: extern union {
        Texture1D: D3D12_TEX1D_DSV,
        Texture1DArray: D3D12_TEX1D_ARRAY_DSV,
        Texture2D: D3D12_TEX2D_DSV,
        Texture2DArray: D3D12_TEX2D_ARRAY_DSV,
        Texture2DMS: D3D12_TEX2DMS_DSV,
        Texture2DMSArray: D3D12_TEX2DMS_ARRAY_DSV,
    },
};

pub const D3D12_RESOURCE_ALLOCATION_INFO = extern struct {
    SizeInBytes: UINT64,
    Alignment: UINT64,
};

pub const D3D12_DEPTH_STENCIL_VALUE = extern struct {
    Depth: FLOAT,
    Stencil: UINT8,
};

pub const D3D12_CLEAR_VALUE = extern struct {
    Format: DXGI_FORMAT,
    u: extern union {
        Color: [4]FLOAT,
        DepthStencil: D3D12_DEPTH_STENCIL_VALUE,
    },

    pub fn color(format: DXGI_FORMAT, in_color: [4]FLOAT) D3D12_CLEAR_VALUE {
        return .{
            .Format = format,
            .u = .{ .Color = in_color },
        };
    }

    pub fn depthStencil(format: DXGI_FORMAT, depth: FLOAT, stencil: UINT8) D3D12_CLEAR_VALUE {
        return .{
            .Format = format,
            .u = .{ .DepthStencil = .{ .Depth = depth, .Stencil = stencil } },
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
        devchild: ID3D12DeviceChild.VTable(Self),
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
                var desc: D3D12_COMMAND_QUEUE_DESC = undefined;
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

pub const ID3D12Device = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetNodeCount(self: *T) UINT {
                return self.v.device.GetNodeCount(self);
            }
            pub inline fn CreateCommandQueue(
                self: *T,
                desc: *const D3D12_COMMAND_QUEUE_DESC,
                guid: *const GUID,
                obj: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateCommandQueue(self, desc, guid, obj);
            }
            pub inline fn CreateCommandAllocator(
                self: *T,
                cmdlist_type: D3D12_COMMAND_LIST_TYPE,
                guid: *const GUID,
                obj: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateCommandAllocator(self, cmdlist_type, guid, obj);
            }
            pub inline fn CreateGraphicsPipelineState(
                self: *T,
                desc: *const D3D12_GRAPHICS_PIPELINE_STATE_DESC,
                guid: *const GUID,
                pso: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateGraphicsPipelineState(self, desc, guid, pso);
            }
            pub inline fn CreateComputePipelineState(
                self: *T,
                desc: *const D3D12_COMPUTE_PIPELINE_STATE_DESC,
                guid: *const GUID,
                pso: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateComputePipelineState(self, desc, guid, pso);
            }
            pub inline fn CreateCommandList(
                self: *T,
                node_mask: UINT,
                cmdlist_type: D3D12_COMMAND_LIST_TYPE,
                cmdalloc: *ID3D12CommandAllocator,
                initial_state: ?*ID3D12PipelineState,
                guid: *const GUID,
                cmdlist: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateCommandList(self, node_mask, cmdlist_type, cmdalloc, initial_state, guid, cmdlist);
            }
            pub inline fn CheckFeatureSupport(self: *T, feature: D3D12_FEATURE, data: *c_void, data_size: UINT) HRESULT {
                return self.vtbl.CheckFeatureSupport(self, feature, data, data_size);
            }
            pub inline fn CreateDescriptorHeap(
                self: *T,
                desc: *const D3D12_DESCRIPTOR_HEAP_DESC,
                guid: *const GUID,
                heap: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateDescriptorHeap(self, desc, guid, heap);
            }
            pub inline fn GetDescriptorHandleIncrementSize(self: *T, heap_type: D3D12_DESCRIPTOR_HEAP_TYPE) UINT {
                return self.v.device.GetDescriptorHandleIncrementSize(self, heap_type);
            }
            pub inline fn CreateRootSignature(
                self: *T,
                node_mask: UINT,
                blob: *const c_void,
                blob_size: UINT64,
                guid: *const GUID,
                signature: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateRootSignature(self, node_mask, blob, blob_size, guid, signature);
            }
            pub inline fn CreateConstantBufferView(
                self: *T,
                desc: ?*const D3D12_CONSTANT_BUFFER_VIEW_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateConstantBufferView(self, desc, dst_descriptor);
            }
            pub inline fn CreateShaderResourceView(
                self: *T,
                resource: ?*ID3D12Resource,
                desc: ?*const D3D12_SHADER_RESOURCE_VIEW_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateShaderResourceView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateUnorderedAccessView(
                self: *T,
                resource: ?*ID3D12Resource,
                counter_resource: ?*ID3D12Resource,
                desc: ?*const D3D12_UNORDERED_ACCESS_VIEW_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateUnorderedAccessView(
                    self,
                    resource,
                    counter_resource,
                    desc,
                    dst_descriptor,
                );
            }
            pub inline fn CreateRenderTargetView(
                self: *T,
                resource: ?*ID3D12Resource,
                desc: ?*const D3D12_RENDER_TARGET_VIEW_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateRenderTargetView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateDepthStencilView(
                self: *T,
                resource: ?*ID3D12Resource,
                desc: ?*const D3D12_DEPTH_STENCIL_VIEW_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateDepthStencilView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateSampler(
                self: *T,
                desc: *const D3D12_SAMPLER_DESC,
                dst_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateSampler(self, desc, dst_descriptor);
            }
            pub inline fn CopyDescriptors(
                self: *T,
                num_dst_ranges: UINT,
                dst_range_starts: [*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                dst_range_sizes: ?[*]const UINT,
                num_src_ranges: UINT,
                src_range_starts: [*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                src_range_sizes: ?[*]const UINT,
                heap_type: D3D12_DESCRIPTOR_HEAP_TYPE,
            ) void {
                self.v.device.CopyDescriptors(
                    self,
                    num_dst_ranges,
                    dst_range_starts,
                    dst_range_sizes,
                    num_src_ranges,
                    src_range_starts,
                    src_range_sizes,
                    heap_type,
                );
            }
            pub inline fn CopyDescriptorsSimple(
                self: *T,
                num: UINT,
                dst_range_start: D3D12_CPU_DESCRIPTOR_HANDLE,
                src_range_start: D3D12_CPU_DESCRIPTOR_HANDLE,
                heap_type: D3D12_DESCRIPTOR_HEAP_TYPE,
            ) void {
                self.v.device.CopyDescriptorsSimple(self, num, dst_range_start, src_range_start, heap_type);
            }
            pub inline fn GetResourceAllocationInfo(
                self: *T,
                visible_mask: UINT,
                num_descs: UINT,
                descs: [*]const D3D12_RESOURCE_DESC,
            ) D3D12_RESOURCE_ALLOCATION_INFO {
                var info: D3D12_RESOURCE_ALLOCATION_INFO = undefined;
                self.v.device.GetResourceAllocationInfo(self, &info, visible_mask, num_descs, descs);
                return info;
            }
            pub inline fn GetCustomHeapProperties(
                self: *T,
                node_mask: UINT,
                heap_type: D3D12_HEAP_TYPE,
            ) D3D12_HEAP_PROPERTIES {
                var props: D3D12_HEAP_PROPERTIES = undefined;
                self.v.device.GetCustomHeapProperties(self, &props, node_mask, heap_type);
                return props;
            }
            pub inline fn CreateCommittedResource(
                self: *T,
                heap_props: *const D3D12_HEAP_PROPERTIES,
                heap_flags: D3D12_HEAP_FLAGS,
                desc: *const D3D12_RESOURCE_DESC,
                state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device.CreateCommittedResource(
                    self,
                    heap_props,
                    heap_flags,
                    desc,
                    state,
                    clear_value,
                    guid,
                    resource,
                );
            }
            pub inline fn CreateHeap(self: *T, desc: *const D3D12_HEAP_DESC, guid: *const GUID, heap: ?*?*c_void) HRESULT {
                return self.v.device.CreateHeap(self, desc, guid, heap);
            }
            pub inline fn CreatePlacedResource(
                self: *T,
                heap: *ID3D12Heap,
                heap_offset: UINT64,
                desc: *const D3D12_RESOURCE_DESC,
                state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device.CreatePlacedResource(
                    self,
                    heap,
                    heap_offset,
                    desc,
                    state,
                    clear_value,
                    guid,
                    resource,
                );
            }
            pub inline fn CreateReservedResource(
                self: *T,
                desc: *const D3D12_RESOURCE_DESC,
                state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device.CreateReservedResource(self, desc, state, clear_value, guid, resource);
            }
            pub inline fn CreateSharedHandle(
                self: *T,
                object: *ID3D12DeviceChild,
                attributes: ?*const SECURITY_ATTRIBUTES,
                access: DWORD,
                name: ?LPCWSTR,
                handle: ?*HANDLE,
            ) HRESULT {
                return self.v.device.CreateSharedHandle(self, object, attributes, access, name, handle);
            }
            pub inline fn OpenSharedHandle(self: *T, handle: HANDLE, guid: *const GUID, object: ?*?*c_void) HRESULT {
                return self.v.device.OpenSharedHandle(self, handle, guid, object);
            }
            pub inline fn OpenSharedHandleByName(self: *T, name: LPCWSTR, access: DWORD, handle: ?*HANDLE) HRESULT {
                return self.v.device.OpenSharedHandleByName(self, name, access, handle);
            }
            pub inline fn MakeResident(self: *T, num: UINT, objects: [*]const *ID3D12Pageable) HRESULT {
                return self.v.device.MakeResident(self, num, objects);
            }
            pub inline fn Evict(self: *T, num: UINT, objects: [*]const *ID3D12Pageable) HRESULT {
                return self.v.device.Evict(self, num, objects);
            }
            pub inline fn CreateFence(
                self: *T,
                initial_value: UINT64,
                flags: D3D12_FENCE_FLAGS,
                guid: *const GUID,
                fence: *?*c_void,
            ) HRESULT {
                return self.v.device.CreateFence(self, initial_value, flags, guid, fence);
            }
            pub inline fn GetDeviceRemovedReason(self: *T) HRESULT {
                return self.v.device.GetDeviceRemovedReason(self);
            }
            pub inline fn GetCopyableFootprints(
                self: *T,
                desc: *const D3D12_RESOURCE_DESC,
                first_subresource: UINT,
                num_subresources: UINT,
                base_offset: UINT64,
                layouts: ?[*]D3D12_PLACED_SUBRESOURCE_FOOTPRINT,
                num_rows: ?[*]UINT,
                row_size: ?[*]UINT64,
                total_sizie: ?*UINT64,
            ) void {
                self.v.device.GetCopyableFootprints(
                    self,
                    desc,
                    first_subresource,
                    num_subresources,
                    base_offset,
                    layouts,
                    num_rows,
                    row_size,
                    total_sizie,
                );
            }
            pub inline fn CreateQueryHeap(
                self: *T,
                desc: *const D3D12_QUERY_HEAP_DESC,
                guid: *const GUID,
                query_heap: ?*?*c_void,
            ) HRESULT {
                return self.v.device.CreateQueryHeap(self, desc, guid, query_heap);
            }
            pub inline fn SetStablePowerState(self: *T, enable: BOOL) HRESULT {
                return self.v.device.SetStablePowerState(self, enable);
            }
            pub inline fn CreateCommandSignature(
                self: *T,
                desc: *const D3D12_COMMAND_SIGNATURE_DESC,
                root_signature: ?*ID3D12RootSignature,
                guid: *const GUID,
                cmd_signature: ?*?*c_void,
            ) HRESULT {
                return self.v.device.CreateCommandSignature(self, desc, root_signature, guid, cmd_signature);
            }
            pub inline fn GetResourceTiling(
                self: *T,
                resource: *ID3D12Resource,
                num_resource_tiles: ?*UINT,
                packed_mip_desc: ?*D3D12_PACKED_MIP_INFO,
                std_tile_shape_non_packed_mips: ?*D3D12_TILE_SHAPE,
                num_subresource_tilings: ?*UINT,
                first_subresource: UINT,
                subresource_tiling_for_non_packed_mips: [*]D3D12_SUBRESOURCE_TILING,
            ) void {
                self.v.device.GetResourceTiling(
                    self,
                    resource,
                    num_resource_tiles,
                    packed_mip_desc,
                    std_tile_shape_non_packed_mips,
                    num_subresource_tilings,
                    first_subresource,
                    subresource_tiling_for_non_packed_mips,
                );
            }
            pub inline fn GetAdapterLuid(self: *T) LUID {
                var luid: LUID = undefined;
                self.v.device.GetAdapterLuid(self, &luid);
                return luid;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetNodeCount: fn (*T) callconv(WINAPI) UINT,
            CreateCommandQueue: fn (*T, *const D3D12_COMMAND_QUEUE_DESC, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            CreateCommandAllocator: fn (*T, D3D12_COMMAND_LIST_TYPE, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            CreateGraphicsPipelineState: fn (
                *T,
                *const D3D12_GRAPHICS_PIPELINE_STATE_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateComputePipelineState: fn (
                *T,
                *const D3D12_COMPUTE_PIPELINE_STATE_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateCommandList: fn (
                *T,
                UINT,
                D3D12_COMMAND_LIST_TYPE,
                *ID3D12CommandAllocator,
                ?*ID3D12PipelineState,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CheckFeatureSupport: fn (*T, D3D12_FEATURE, *c_void, UINT) callconv(WINAPI) HRESULT,
            CreateDescriptorHeap: fn (
                *T,
                *const D3D12_DESCRIPTOR_HEAP_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            GetDescriptorHandleIncrementSize: fn (*T, D3D12_DESCRIPTOR_HEAP_TYPE) callconv(WINAPI) UINT,
            CreateRootSignature: fn (*T, UINT, *const c_void, UINT64, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            CreateConstantBufferView: fn (
                *T,
                ?*const D3D12_CONSTANT_BUFFER_VIEW_DESC,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateShaderResourceView: fn (
                *T,
                ?*ID3D12Resource,
                ?*const D3D12_SHADER_RESOURCE_VIEW_DESC,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateUnorderedAccessView: fn (
                *T,
                ?*ID3D12Resource,
                ?*ID3D12Resource,
                ?*const D3D12_UNORDERED_ACCESS_VIEW_DESC,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateRenderTargetView: fn (
                *T,
                ?*ID3D12Resource,
                ?*const D3D12_RENDER_TARGET_VIEW_DESC,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateDepthStencilView: fn (
                *T,
                ?*ID3D12Resource,
                ?*const D3D12_DEPTH_STENCIL_VIEW_DESC,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateSampler: fn (*T, *const D3D12_SAMPLER_DESC, D3D12_CPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            CopyDescriptors: fn (
                *T,
                UINT,
                [*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                ?[*]const UINT,
                UINT,
                [*]const D3D12_CPU_DESCRIPTOR_HANDLE,
                ?[*]const UINT,
                D3D12_DESCRIPTOR_HEAP_TYPE,
            ) callconv(WINAPI) void,
            CopyDescriptorsSimple: fn (
                *T,
                UINT,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                D3D12_CPU_DESCRIPTOR_HANDLE,
                D3D12_DESCRIPTOR_HEAP_TYPE,
            ) callconv(WINAPI) void,
            GetResourceAllocationInfo: fn (
                *T,
                *D3D12_RESOURCE_ALLOCATION_INFO,
                UINT,
                UINT,
                [*]const D3D12_RESOURCE_DESC,
            ) callconv(WINAPI) *D3D12_RESOURCE_ALLOCATION_INFO,
            GetCustomHeapProperties: fn (
                *T,
                *D3D12_HEAP_PROPERTIES,
                UINT,
                D3D12_HEAP_TYPE,
            ) callconv(WINAPI) *D3D12_HEAP_PROPERTIES,
            CreateCommittedResource: fn (
                *T,
                *const D3D12_HEAP_PROPERTIES,
                D3D12_HEAP_FLAGS,
                *const D3D12_RESOURCE_DESC,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateHeap: fn (*T, *const D3D12_HEAP_DESC, *const GUID, ?*?*c_void) callconv(WINAPI) HRESULT,
            CreatePlacedResource: fn (
                *T,
                *ID3D12Heap,
                UINT64,
                *const D3D12_RESOURCE_DESC,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateReservedResource: fn (
                *T,
                *const D3D12_RESOURCE_DESC,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateSharedHandle: fn (
                *T,
                *ID3D12DeviceChild,
                ?*const SECURITY_ATTRIBUTES,
                DWORD,
                ?LPCWSTR,
                ?*HANDLE,
            ) callconv(WINAPI) HRESULT,
            OpenSharedHandle: fn (*T, HANDLE, *const GUID, ?*?*c_void) callconv(WINAPI) HRESULT,
            OpenSharedHandleByName: fn (*T, LPCWSTR, DWORD, ?*HANDLE) callconv(WINAPI) HRESULT,
            MakeResident: fn (*T, UINT, [*]const *ID3D12Pageable) callconv(WINAPI) HRESULT,
            Evict: fn (*T, UINT, [*]const *ID3D12Pageable) callconv(WINAPI) HRESULT,
            CreateFence: fn (*T, UINT64, D3D12_FENCE_FLAGS, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            GetDeviceRemovedReason: fn (*T) callconv(WINAPI) HRESULT,
            GetCopyableFootprints: fn (
                *T,
                *const D3D12_RESOURCE_DESC,
                UINT,
                UINT,
                UINT64,
                ?[*]D3D12_PLACED_SUBRESOURCE_FOOTPRINT,
                ?[*]UINT,
                ?[*]UINT64,
                ?*UINT64,
            ) callconv(WINAPI) void,
            CreateQueryHeap: fn (*T, *const D3D12_QUERY_HEAP_DESC, *const GUID, ?*?*c_void) callconv(WINAPI) HRESULT,
            SetStablePowerState: fn (*T, BOOL) callconv(WINAPI) HRESULT,
            CreateCommandSignature: fn (
                *T,
                *const D3D12_COMMAND_SIGNATURE_DESC,
                ?*ID3D12RootSignature,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            GetResourceTiling: fn (
                *T,
                *ID3D12Resource,
                ?*UINT,
                ?*D3D12_PACKED_MIP_INFO,
                ?*D3D12_TILE_SHAPE,
                ?*UINT,
                UINT,
                [*]D3D12_SUBRESOURCE_TILING,
            ) callconv(WINAPI) void,
            GetAdapterLuid: fn (*T, *LUID) callconv(WINAPI) *LUID,
        };
    }
};

pub var D3D12GetDebugInterface: fn (*const GUID, ?*?*c_void) callconv(WINAPI) HRESULT = undefined;
pub var D3D12CreateDevice: fn (
    ?*IUnknown,
    D3D_FEATURE_LEVEL,
    *const GUID,
    ?*?*c_void,
) callconv(WINAPI) HRESULT = undefined;

pub const IID_ID3D12Device = GUID{
    .Data1 = 0x189819f1,
    .Data2 = 0x1db6,
    .Data3 = 0x4b57,
    .Data4 = .{ 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7 },
};
pub const IID_ID3D12CommandQueue = GUID{
    .Data1 = 0x0ec870a6,
    .Data2 = 0x5d7e,
    .Data3 = 0x4c22,
    .Data4 = .{ 0x8c, 0xfc, 0x5b, 0xaa, 0xe0, 0x76, 0x16, 0xed },
};
pub const IID_ID3D12Fence = GUID{
    .Data1 = 0x0a753dcf,
    .Data2 = 0xc4d8,
    .Data3 = 0x4b91,
    .Data4 = .{ 0xad, 0xf6, 0xbe, 0x5a, 0x60, 0xd9, 0x5a, 0x76 },
};
pub const IID_ID3D12CommandAllocator = GUID{
    .Data1 = 0x6102dee4,
    .Data2 = 0xaf59,
    .Data3 = 0x4b09,
    .Data4 = .{ 0xb9, 0x99, 0xb4, 0x4d, 0x73, 0xf0, 0x9b, 0x24 },
};
pub const IID_ID3D12PipelineState = GUID{
    .Data1 = 0x765a30f3,
    .Data2 = 0xf624,
    .Data3 = 0x4c6f,
    .Data4 = .{ 0xa8, 0x28, 0xac, 0xe9, 0x48, 0x62, 0x24, 0x45 },
};
pub const IID_ID3D12InfoQueue = GUID{
    .Data1 = 0x0742a90b,
    .Data2 = 0xc387,
    .Data3 = 0x483f,
    .Data4 = .{ 0xb9, 0x46, 0x30, 0xa7, 0xe4, 0xe6, 0x14, 0x58 },
};
pub const IID_ID3D12DescriptorHeap = GUID{
    .Data1 = 0x8efb471d,
    .Data2 = 0x616c,
    .Data3 = 0x4f49,
    .Data4 = .{ 0x90, 0xf7, 0x12, 0x7b, 0xb7, 0x63, 0xfa, 0x51 },
};
pub const IID_ID3D12Resource = GUID{
    .Data1 = 0x696442be,
    .Data2 = 0xa72e,
    .Data3 = 0x4059,
    .Data4 = .{ 0xbc, 0x79, 0x5b, 0x5c, 0x98, 0x04, 0x0f, 0xad },
};
pub const IID_ID3D12RootSignature = GUID{
    .Data1 = 0xc54a6b66,
    .Data2 = 0x72df,
    .Data3 = 0x4ee8,
    .Data4 = .{ 0x8b, 0xe5, 0xa9, 0x46, 0xa1, 0x42, 0x92, 0x14 },
};
pub const IID_ID3D12GraphicsCommandList = GUID{
    .Data1 = 0x5b160d0f,
    .Data2 = 0xac1b,
    .Data3 = 0x4185,
    .Data4 = .{ 0x8b, 0xa8, 0xb3, 0xae, 0x42, 0xa5, 0xa4, 0x55 },
};

pub fn d3d12_load_dll() !void {
    // TODO(mziulek): Better error handling.
    var d3d12_dll = try std.DynLib.openZ("d3d12.dll");
    D3D12CreateDevice = d3d12_dll.lookup(@TypeOf(D3D12CreateDevice), "D3D12CreateDevice").?;
    D3D12GetDebugInterface = d3d12_dll.lookup(@TypeOf(D3D12GetDebugInterface), "D3D12GetDebugInterface").?;
}
