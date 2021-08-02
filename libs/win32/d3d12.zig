const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
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

    pub fn initType(heap_type: D3D12_HEAP_TYPE) D3D12_HEAP_PROPERTIES {
        var v = std.mem.zeroes(@This());
        v = D3D12_HEAP_PROPERTIES{
            .Type = heap_type,
            .CPUPageProperty = .UNKNOWN,
            .MemoryPoolPreference = .UNKNOWN,
            .CreationNodeMask = 0,
            .VisibleNodeMask = 0,
        };
        return v;
    }
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES = D3D12_HEAP_FLAGS{};
pub const D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS = D3D12_HEAP_FLAGS{ .DENY_RT_DS_TEXTURES = true, .DENY_NON_RT_DS_TEXTURES = true };
pub const D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES = D3D12_HEAP_FLAGS{ .DENY_BUFFERS = true, .DENY_RT_DS_TEXTURES = true };
pub const D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES = D3D12_HEAP_FLAGS{ .DENY_BUFFERS = true, .DENY_NON_RT_DS_TEXTURES = true };

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
    pub usingnamespace FlagsMixin(@This());
};

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

    pub fn initBuffer(width: UINT64) D3D12_RESOURCE_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
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
        return v;
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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_RESOURCE_STATE_GENERIC_READ = D3D12_RESOURCE_STATES{
    .VERTEX_AND_CONSTANT_BUFFER = true,
    .INDEX_BUFFER = true,
    .NON_PIXEL_SHADER_RESOURCE = true,
    .PIXEL_SHADER_RESOURCE = true,
    .INDIRECT_ARGUMENT = true,
    .COPY_SOURCE = true,
};
pub const D3D12_RESOURCE_STATE_PRESENT = D3D12_RESOURCE_STATES{};
pub const D3D12_RESOURCE_STATE_PREDICATION = D3D12_RESOURCE_STATES{ .INDIRECT_ARGUMENT = true };
pub const D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE = D3D12_RESOURCE_STATES{
    .NON_PIXEL_SHADER_RESOURCE = true,
    .PIXEL_SHADER_RESOURCE = true,
};

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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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

    pub inline fn initZero() D3D12_SHADER_BYTECODE {
        return std.mem.zeroes(@This());
    }
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

    pub inline fn initZero() D3D12_STREAM_OUTPUT_DESC {
        return std.mem.zeroes(@This());
    }
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_COLOR_WRITE_ENABLE_ALL = D3D12_COLOR_WRITE_ENABLE{ .RED = true, .GREEN = true, .BLUE = true, .ALPHA = true };

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

    pub fn initDefault() D3D12_RENDER_TARGET_BLEND_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .BlendEnable = FALSE,
            .LogicOpEnable = FALSE,
            .SrcBlend = .ONE,
            .DestBlend = .ZERO,
            .BlendOp = .ADD,
            .SrcBlendAlpha = .ONE,
            .DestBlendAlpha = .ZERO,
            .BlendOpAlpha = .ADD,
            .LogicOp = .NOOP,
            .RenderTargetWriteMask = 0xf,
        };
        return v;
    }
};

pub const D3D12_BLEND_DESC = extern struct {
    AlphaToCoverageEnable: BOOL,
    IndependentBlendEnable: BOOL,
    RenderTarget: [8]D3D12_RENDER_TARGET_BLEND_DESC,

    pub fn initDefault() D3D12_BLEND_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .AlphaToCoverageEnable = FALSE,
            .IndependentBlendEnable = FALSE,
            .RenderTarget = [_]D3D12_RENDER_TARGET_BLEND_DESC{D3D12_RENDER_TARGET_BLEND_DESC.initDefault()} ** 8,
        };
        return v;
    }
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

    pub fn initDefault() D3D12_RASTERIZER_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .FillMode = .SOLID,
            .CullMode = .BACK,
            .FrontCounterClockwise = FALSE,
            .DepthBias = 0,
            .DepthBiasClamp = 0.0,
            .SlopeScaledDepthBias = 0.0,
            .DepthClipEnable = TRUE,
            .MultisampleEnable = FALSE,
            .AntialiasedLineEnable = FALSE,
            .ForcedSampleCount = 0,
            .ConservativeRaster = .OFF,
        };
        return v;
    }
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

    pub fn initDefault() D3D12_DEPTH_STENCILOP_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .StencilFailOp = .KEEP,
            .StencilDepthFailOp = .KEEP,
            .StencilPassOp = .KEEP,
            .StencilFunc = .ALWAYS,
        };
        return v;
    }
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

    pub fn initDefault() D3D12_DEPTH_STENCIL_DESC {
        var desc = std.mem.zeroes(@This());
        desc = .{
            .DepthEnable = TRUE,
            .DepthWriteMask = .ALL,
            .DepthFunc = .LESS,
            .StencilEnable = FALSE,
            .StencilReadMask = 0xff,
            .StencilWriteMask = 0xff,
            .FrontFace = D3D12_DEPTH_STENCILOP_DESC.initDefault(),
            .BackFace = D3D12_DEPTH_STENCILOP_DESC.initDefault(),
        };
        return desc;
    }
};

pub const D3D12_INPUT_LAYOUT_DESC = extern struct {
    pInputElementDescs: ?[*]const D3D12_INPUT_ELEMENT_DESC,
    NumElements: UINT,

    pub inline fn initZero() D3D12_INPUT_LAYOUT_DESC {
        return std.mem.zeroes(@This());
    }
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

    pub inline fn initZero() D3D12_CACHED_PIPELINE_STATE {
        return std.mem.zeroes(@This());
    }
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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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

    pub fn initTypedBuffer(
        format: DXGI_FORMAT,
        first_element: UINT64,
        num_elements: UINT,
    ) D3D12_SHADER_RESOURCE_VIEW_DESC {
        var desc = std.mem.zeroes(@This());
        desc = .{
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
        return desc;
    }

    pub fn initStructuredBuffer(
        first_element: UINT64,
        num_elements: UINT,
        stride: UINT,
    ) D3D12_SHADER_RESOURCE_VIEW_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .ViewDimension = .BUFFER,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = stride,
                },
            },
        };
        return v;
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
    pub usingnamespace FlagsMixin(@This());
};

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
    pub usingnamespace FlagsMixin(@This());
};

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

    pub fn initColor(format: DXGI_FORMAT, in_color: *const [4]FLOAT) D3D12_CLEAR_VALUE {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = format,
            .u = .{ .Color = in_color.* },
        };
        return v;
    }

    pub fn initDepthStencil(format: DXGI_FORMAT, depth: FLOAT, stencil: UINT8) D3D12_CLEAR_VALUE {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = format,
            .u = .{ .DepthStencil = .{ .Depth = depth, .Stencil = stencil } },
        };
        return v;
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

pub const D3D12_RANGE_UINT64 = extern struct {
    Begin: UINT64,
    End: UINT64,
};

pub const D3D12_SUBRESOURCE_RANGE_UINT64 = extern struct {
    Subresource: UINT,
    Range: D3D12_RANGE_UINT64,
};

pub const D3D12_SAMPLE_POSITION = extern struct {
    X: INT8,
    Y: INT8,
};

pub const D3D12_RESOLVE_MODE = enum(UINT) {
    DECOMPRESS = 0,
    MIN = 1,
    MAX = 2,
    AVERAGE = 3,
    ENCODE_SAMPLER_FEEDBACK = 4,
    DECODE_SAMPLER_FEEDBACK = 5,
};

pub const ID3D12GraphicsCommandList1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AtomicCopyBufferUINT(
                self: *T,
                dst_buffer: *ID3D12Resource,
                dst_offset: UINT64,
                src_buffer: *ID3D12Resource,
                src_offset: UINT64,
                dependencies: UINT,
                dependent_resources: [*]const *ID3D12Resource,
                dependent_subresource_ranges: [*]const D3D12_SUBRESOURCE_RANGE_UINT64,
            ) void {
                self.v.grcmdlist1.AtomicCopyBufferUINT(
                    self,
                    dst_buffer,
                    dst_offset,
                    src_buffer,
                    src_offset,
                    dependencies,
                    dependent_resources,
                    dependent_subresource_ranges,
                );
            }
            pub inline fn AtomicCopyBufferUINT64(
                self: *T,
                dst_buffer: *ID3D12Resource,
                dst_offset: UINT64,
                src_buffer: *ID3D12Resource,
                src_offset: UINT64,
                dependencies: UINT,
                dependent_resources: [*]const *ID3D12Resource,
                dependent_subresource_ranges: [*]const D3D12_SUBRESOURCE_RANGE_UINT64,
            ) void {
                self.v.grcmdlist1.AtomicCopyBufferUINT64(
                    self,
                    dst_buffer,
                    dst_offset,
                    src_buffer,
                    src_offset,
                    dependencies,
                    dependent_resources,
                    dependent_subresource_ranges,
                );
            }
            pub inline fn OMSetDepthBounds(self: *T, min: FLOAT, max: FLOAT) void {
                self.v.grcmdlist1.OMSetDepthBounds(self, min, max);
            }
            pub inline fn SetSamplePositions(
                self: *T,
                num_samples: UINT,
                num_pixels: UINT,
                sample_positions: *D3D12_SAMPLE_POSITION,
            ) void {
                self.v.grcmdlist1.SetSamplePositions(self, num_samples, num_pixels, sample_positions);
            }
            pub inline fn ResolveSubresourceRegion(
                self: *T,
                dst_resource: *ID3D12Resource,
                dst_subresource: UINT,
                dst_x: UINT,
                dst_y: UINT,
                src_resource: *ID3D12Resource,
                src_subresource: UINT,
                src_rect: *D3D12_RECT,
                format: DXGI_FORMAT,
                resolve_mode: D3D12_RESOLVE_MODE,
            ) void {
                self.v.grcmdlist1.ResolveSubresourceRegion(
                    self,
                    dst_resource,
                    dst_subresource,
                    dst_x,
                    dst_y,
                    src_resource,
                    src_subresource,
                    src_rect,
                    format,
                    resolve_mode,
                );
            }
            pub inline fn SetViewInstanceMask(self: *T, mask: UINT) void {
                self.v.grcmdlist1.SetViewInstanceMask(self, mask);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            AtomicCopyBufferUINT: fn (
                *T,
                *ID3D12Resource,
                UINT64,
                *ID3D12Resource,
                UINT64,
                UINT,
                [*]const *ID3D12Resource,
                [*]const D3D12_SUBRESOURCE_RANGE_UINT64,
            ) callconv(WINAPI) void,
            AtomicCopyBufferUINT64: fn (
                *T,
                *ID3D12Resource,
                UINT64,
                *ID3D12Resource,
                UINT64,
                UINT,
                [*]const *ID3D12Resource,
                [*]const D3D12_SUBRESOURCE_RANGE_UINT64,
            ) callconv(WINAPI) void,
            OMSetDepthBounds: fn (*T, FLOAT, FLOAT) callconv(WINAPI) void,
            SetSamplePositions: fn (*T, UINT, UINT, *D3D12_SAMPLE_POSITION) callconv(WINAPI) void,
            ResolveSubresourceRegion: fn (
                *T,
                *ID3D12Resource,
                UINT,
                UINT,
                UINT,
                *ID3D12Resource,
                UINT,
                *D3D12_RECT,
                DXGI_FORMAT,
                D3D12_RESOLVE_MODE,
            ) callconv(WINAPI) void,
            SetViewInstanceMask: fn (*T, UINT) callconv(WINAPI) void,
        };
    }
};

pub const D3D12_WRITEBUFFERIMMEDIATE_PARAMETER = extern struct {
    Dest: D3D12_GPU_VIRTUAL_ADDRESS,
    Value: UINT32,
};

pub const D3D12_WRITEBUFFERIMMEDIATE_MODE = enum(UINT) {
    DEFAULT = 0,
    MARKER_IN = 0x1,
    MARKER_OUT = 0x2,
};

pub const ID3D12GraphicsCommandList2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: ID3D12GraphicsCommandList1.VTable(Self),
        grcmdlist2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList1.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn WriteBufferImmediate(
                self: *T,
                count: UINT,
                params: [*]const D3D12_WRITEBUFFERIMMEDIATE_PARAMETER,
                modes: ?[*]const D3D12_WRITEBUFFERIMMEDIATE_MODE,
            ) void {
                self.v.grcmdlist2.WriteBufferImmediate(self, count, params, modes);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            WriteBufferImmediate: fn (
                *T,
                UINT,
                [*]const D3D12_WRITEBUFFERIMMEDIATE_PARAMETER,
                ?[*]const D3D12_WRITEBUFFERIMMEDIATE_MODE,
            ) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12GraphicsCommandList3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: ID3D12GraphicsCommandList1.VTable(Self),
        grcmdlist2: ID3D12GraphicsCommandList2.VTable(Self),
        grcmdlist3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList1.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList2.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetProtectedResourceSession(self: *T, prsession: ?*ID3D12ProtectedResourceSession) void {
                self.v.grcmdlist3.SetProtectedResourceSession(self, prsession);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetProtectedResourceSession: fn (*T, ?*ID3D12ProtectedResourceSession) callconv(WINAPI) void,
        };
    }
};

pub const D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE = enum(UINT) {
    DISCARD = 0,
    PRESERVE = 1,
    CLEAR = 2,
    NO_ACCESS = 3,
};

pub const D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS = extern struct {
    ClearValue: D3D12_CLEAR_VALUE,
};

pub const D3D12_RENDER_PASS_BEGINNING_ACCESS = extern struct {
    Type: D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE,
    u: extern union {
        Clear: D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS,
    },
};

pub const D3D12_RENDER_PASS_ENDING_ACCESS_TYPE = enum(UINT) {
    DISCARD = 0,
    PRESERVE = 1,
    RESOLVE = 2,
    NO_ACCESS = 3,
};

pub const D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_SUBRESOURCE_PARAMETERS = extern struct {
    SrcSubresource: UINT,
    DstSubresource: UINT,
    DstX: UINT,
    DstY: UINT,
    SrcRect: D3D12_RECT,
};

pub const D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS = extern struct {
    pSrcResource: *ID3D12Resource,
    pDstResource: *ID3D12Resource,
    SubresourceCount: UINT,
    pSubresourceParameters: [*]const D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_SUBRESOURCE_PARAMETERS,
    Format: DXGI_FORMAT,
    ResolveMode: D3D12_RESOLVE_MODE,
    PreserveResolveSource: BOOL,
};

pub const D3D12_RENDER_PASS_ENDING_ACCESS = extern struct {
    Type: D3D12_RENDER_PASS_ENDING_ACCESS_TYPE,
    u: extern union {
        Resolve: D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS,
    },
};

pub const D3D12_RENDER_PASS_RENDER_TARGET_DESC = extern struct {
    cpuDescriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
    BeginningAccess: D3D12_RENDER_PASS_BEGINNING_ACCESS,
    EndingAccess: D3D12_RENDER_PASS_ENDING_ACCESS,
};

pub const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC = extern struct {
    cpuDescriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
    DepthBeginningAccess: D3D12_RENDER_PASS_BEGINNING_ACCESS,
    StencilBeginningAccess: D3D12_RENDER_PASS_BEGINNING_ACCESS,
    DepthEndingAccess: D3D12_RENDER_PASS_ENDING_ACCESS,
    StencilEndingAccess: D3D12_RENDER_PASS_ENDING_ACCESS,
};

pub const D3D12_RENDER_PASS_FLAGS = packed struct {
    ALLOW_UAV_WRITES: bool align(4) = false, // 0x1
    SUSPENDING_PASS: bool = false, // 0x2
    RESUMING_PASS: bool = false, // 0x4
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_META_COMMAND_PARAMETER_TYPE = enum(UINT) {
    FLOAT = 0,
    UINT64 = 1,
    GPU_VIRTUAL_ADDRESS = 2,
    CPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV = 3,
    GPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV = 4,
};

pub const D3D12_META_COMMAND_PARAMETER_FLAGS = packed struct {
    INPUT: bool align(4) = false, // 0x1
    OUTPUT: bool = false, // 0x2
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_META_COMMAND_PARAMETER_STAGE = enum(UINT) {
    CREATION = 0,
    INITIALIZATION = 1,
    EXECUTION = 2,
};

pub const D3D12_META_COMMAND_PARAMETER_DESC = extern struct {
    Name: LPCWSTR,
    Type: D3D12_META_COMMAND_PARAMETER_TYPE,
    Flags: D3D12_META_COMMAND_PARAMETER_FLAGS,
    RequiredResourceState: D3D12_RESOURCE_STATES,
    StructureOffset: UINT,
};

pub const D3D12_GRAPHICS_STATES = packed struct {
    IA_VERTEX_BUFFERS: bool align(4) = false, // ( 1 << 0 )
    IA_INDEX_BUFFER: bool = false, // ( 1 << 1 )
    IA_PRIMITIVE_TOPOLOGY: bool = false, // ( 1 << 2 )
    DESCRIPTOR_HEAP: bool = false, // ( 1 << 3 )
    GRAPHICS_ROOT_SIGNATURE: bool = false, // ( 1 << 4 )
    COMPUTE_ROOT_SIGNATURE: bool = false, // ( 1 << 5 )
    RS_VIEWPORTS: bool = false, // ( 1 << 6 )
    RS_SCISSOR_RECTS: bool = false, // ( 1 << 7 )
    PREDICATION: bool = false, // ( 1 << 8 )
    OM_RENDER_TARGETS: bool = false, // ( 1 << 9 )
    OM_STENCIL_REF: bool = false, //  ( 1 << 10 )
    OM_BLEND_FACTOR: bool = false, // ( 1 << 11 )
    PIPELINE_STATE: bool = false, // ( 1 << 12 )
    SO_TARGETS: bool = false, // ( 1 << 13 )
    OM_DEPTH_BOUNDS: bool = false, //  ( 1 << 14 )
    SAMPLE_POSITIONS: bool = false, // ( 1 << 15 )
    VIEW_INSTANCE_MASK: bool = false, // ( 1 << 16 )
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

pub const D3D12_META_COMMAND_DESC = extern struct {
    Id: GUID,
    Name: LPCWSTR,
    InitializationDirtyState: D3D12_GRAPHICS_STATES,
    ExecutionDirtyState: D3D12_GRAPHICS_STATES,
};

pub const ID3D12MetaCommand = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        metacmd: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetRequiredParameterResourceSize(
                self: *T,
                stage: D3D12_META_COMMAND_PARAMETER_STAGE,
                param_index: UINT,
            ) UINT64 {
                return self.v.metacmd.GetRequiredParameterResourceSize(self, stage, param_index);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetRequiredParameterResourceSize: fn (
                *T,
                D3D12_META_COMMAND_PARAMETER_STAGE,
                UINT,
            ) callconv(WINAPI) UINT64,
        };
    }
};

pub const D3D12_STATE_SUBOBJECT_TYPE = enum(UINT) {
    STATE_OBJECT_CONFIG = 0,
    GLOBAL_ROOT_SIGNATURE = 1,
    LOCAL_ROOT_SIGNATURE = 2,
    NODE_MASK = 3,
    DXIL_LIBRARY = 5,
    EXISTING_COLLECTION = 6,
    SUBOBJECT_TO_EXPORTS_ASSOCIATION = 7,
    DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION = 8,
    RAYTRACING_SHADER_CONFIG = 9,
    RAYTRACING_PIPELINE_CONFIG = 10,
    HIT_GROUP = 11,
    RAYTRACING_PIPELINE_CONFIG1 = 12,
    MAX_VALID = 13,
};

pub const D3D12_STATE_SUBOBJECT = extern struct {
    Type: D3D12_STATE_SUBOBJECT_TYPE,
    desc: *const c_void,
};

pub const D3D12_STATE_OBJECT_FLAGS = packed struct {
    ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS: bool align(4) = false, // 0x1
    ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS: bool = false, // 0x2
    ALLOW_STATE_OBJECT_ADDITIONS: bool = false, // 0x4
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_STATE_OBJECT_CONFIG = extern struct {
    Flags: D3D12_STATE_OBJECT_FLAGS,
};

pub const D3D12_GLOBAL_ROOT_SIGNATURE = extern struct {
    pGlobalRootSignature: *ID3D12RootSignature,
};

pub const D3D12_LOCAL_ROOT_SIGNATURE = extern struct {
    pLocalRootSignature: *ID3D12RootSignature,
};

pub const D3D12_NODE_MASK = extern struct {
    NodeMask: UINT,
};

pub const D3D12_EXPORT_FLAGS = packed struct {
    __reserved0: bool align(4) = false,
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_EXPORT_DESC = extern struct {
    Name: LPCWSTR,
    ExportToRename: LPCWSTR,
    Flags: D3D12_EXPORT_FLAGS,
};

pub const D3D12_DXIL_LIBRARY_DESC = extern struct {
    DXILLibrary: D3D12_SHADER_BYTECODE,
    NumExports: UINT,
    pExports: [*]D3D12_EXPORT_DESC,
};

pub const D3D12_EXISTING_COLLECTION_DESC = extern struct {
    pExistingCollection: *ID3D12StateObject,
    NumExports: UINT,
    pExports: [*]D3D12_EXPORT_DESC,
};

pub const D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION = extern struct {
    pSubobjectToAssociate: *const D3D12_STATE_SUBOBJECT,
    NumExports: UINT,
    pExports: [*]LPCWSTR,
};

pub const D3D12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION = extern struct {
    SubobjectToAssociate: LPCWSTR,
    NumExports: UINT,
    pExports: [*]LPCWSTR,
};

pub const D3D12_HIT_GROUP_TYPE = enum(UINT) {
    TRIANGLES = 0,
    PROCEDURAL_PRIMITIVE = 0x1,
};

pub const D3D12_HIT_GROUP_DESC = extern struct {
    HitGroupExport: LPCWSTR,
    Type: D3D12_HIT_GROUP_TYPE,
    AnyHitShaderImport: LPCWSTR,
    ClosestHitShaderImport: LPCWSTR,
    IntersectionShaderImport: LPCWSTR,
};

pub const D3D12_RAYTRACING_SHADER_CONFIG = extern struct {
    MaxPayloadSizeInBytes: UINT,
    MaxAttributeSizeInBytes: UINT,
};

pub const D3D12_RAYTRACING_PIPELINE_CONFIG = extern struct {
    MaxTraceRecursionDepth: UINT,
};

pub const D3D12_RAYTRACING_PIPELINE_FLAGS = packed struct {
    __reserved0: bool align(4) = false, // 0x1
    __reserved1: bool = false, // 0x2
    __reserved2: bool = false, // 0x4
    __reserved3: bool = false, // 0x8
    __reserved4: bool = false, // 0x10
    __reserved5: bool = false, // 0x20
    __reserved6: bool = false, // 0x40
    __reserved7: bool = false, // 0x80
    SKIP_TRIANGLES: bool = false, // 0x100
    SKIP_PROCEDURAL_PRIMITIVES: bool = false, // 0x200
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

pub const D3D12_RAYTRACING_PIPELINE_CONFIG1 = extern struct {
    MaxTraceRecursionDepth: UINT,
    Flags: D3D12_RAYTRACING_PIPELINE_FLAGS,
};

pub const D3D12_STATE_OBJECT_TYPE = enum(UINT) {
    COLLECTION = 0,
    RAYTRACING_PIPELINE = 3,
};

pub const D3D12_STATE_OBJECT_DESC = extern struct {
    Type: D3D12_STATE_OBJECT_TYPE,
    NumSubobjects: UINT,
    pSubobjects: [*]const D3D12_STATE_SUBOBJECT,
};

pub const D3D12_RAYTRACING_GEOMETRY_FLAGS = packed struct {
    OPAQUE: bool align(4) = false, // 0x1
    NO_DUPLICATE_ANYHIT_INVOCATION: bool = false, // 0x2
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_RAYTRACING_GEOMETRY_TYPE = enum(UINT) {
    TRIANGLES = 0,
    PROCEDURAL_PRIMITIVE_AABBS = 1,
};

pub const D3D12_RAYTRACING_INSTANCE_FLAGS = packed struct {
    TRIANGLE_CULL_DISABLE: bool align(4) = false, // 0x1
    TRIANGLE_FRONT_COUNTERCLOCKWISE: bool = false, // 0x2
    FORCE_OPAQUE: bool = false, // 0x4
    FORCE_NON_OPAQUE: bool = false, // 0x8
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

pub const D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE = extern struct {
    StartAddress: D3D12_GPU_VIRTUAL_ADDRESS,
    StrideInBytes: UINT64,
};

pub const D3D12_GPU_VIRTUAL_ADDRESS_RANGE = extern struct {
    StartAddress: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
};

pub const D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE = extern struct {
    StartAddress: D3D12_GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
    StrideInBytes: UINT64,
};

pub const D3D12_RAYTRACING_GEOMETRY_TRIANGLES_DESC = extern struct {
    Transform3x4: D3D12_GPU_VIRTUAL_ADDRESS,
    IndexFormat: DXGI_FORMAT,
    VertexFormat: DXGI_FORMAT,
    IndexCount: UINT,
    VertexCount: UINT,
    IndexBuffer: D3D12_GPU_VIRTUAL_ADDRESS,
    VertexBuffer: D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE,
};

pub const D3D12_RAYTRACING_AABB = extern struct {
    MinX: FLOAT,
    MinY: FLOAT,
    MinZ: FLOAT,
    MaxX: FLOAT,
    MaxY: FLOAT,
    MaxZ: FLOAT,
};

pub const D3D12_RAYTRACING_GEOMETRY_AABBS_DESC = extern struct {
    AABBCount: UINT64,
    AABBs: D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS = packed struct {
    ALLOW_UPDATE: bool align(4) = false, // 0x1
    ALLOW_COMPACTION: bool = false, // 0x2
    PREFER_FAST_TRACE: bool = false, //	0x4
    PREFER_FAST_BUILD: bool = false, // 0x8
    MINIMIZE_MEMORY: bool = false, // 0x10
    PERFORM_UPDATE: bool = false, // 0x20
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

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE = enum(UINT) {
    CLONE = 0,
    COMPACT = 0x1,
    VISUALIZATION_DECODE_FOR_TOOLS = 0x2,
    SERIALIZE = 0x3,
    DESERIALIZE = 0x4,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE = enum(UINT) {
    TOP_LEVEL = 0,
    BOTTOM_LEVEL = 0x1,
};

pub const D3D12_ELEMENTS_LAYOUT = enum(UINT) {
    ARRAY = 0,
    ARRAY_OF_POINTERS = 0x1,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE = enum(UINT) {
    COMPACTED_SIZE = 0,
    TOOLS_VISUALIZATION = 0x1,
    SERIALIZATION = 0x2,
    CURRENT_SIZE = 0x3,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC = extern struct {
    DestBuffer: D3D12_GPU_VIRTUAL_ADDRESS,
    InfoType: D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE_DESC = extern struct {
    CompactedSizeInBytes: UINT64,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TOOLS_VISUALIZATION_DESC = extern struct {
    DecodedSizeInBytes: UINT64,
};

pub const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_TOOLS_VISUALIZATION_HEADER = extern struct {
    Type: D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE,
    NumDescs: UINT,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_SERIALIZATION_DESC = extern struct {
    SerializedSizeInBytes: UINT64,
    NumBottomLevelAccelerationStructurePointers: UINT64,
};

pub const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER = extern struct {
    DriverOpaqueGUID: GUID,
    DriverOpaqueVersioningData: [16]BYTE,
};

pub const D3D12_SERIALIZED_DATA_TYPE = enum(UINT) {
    RAYTRACING_ACCELERATION_STRUCTURE = 0,
};

pub const D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS = enum(UINT) {
    COMPATIBLE_WITH_DEVICE = 0,
    UNSUPPORTED_TYPE = 0x1,
    UNRECOGNIZED = 0x2,
    INCOMPATIBLE_VERSION = 0x3,
    INCOMPATIBLE_TYPE = 0x4,
};

pub const D3D12_SERIALIZED_RAYTRACING_ACCELERATION_STRUCTURE_HEADER = extern struct {
    DriverMatchingIdentifier: D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
    SerializedSizeInBytesIncludingHeader: UINT64,
    DeserializedSizeInBytes: UINT64,
    NumBottomLevelAccelerationStructurePointersAfterHeader: UINT64,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_CURRENT_SIZE_DESC = extern struct {
    CurrentSizeInBytes: UINT64,
};

pub const D3D12_RAYTRACING_INSTANCE_DESC = packed struct {
    Transform: [3][4]FLOAT align(8), // TODO(mziulek): Is alignment 8 correct?
    InstanceID: u24,
    InstanceMask: u8,
    InstanceContributionToHitGroupIndex: u24,
    Flags: u8,
    AccelerationStructure: D3D12_GPU_VIRTUAL_ADDRESS,
};
comptime {
    std.debug.assert(@sizeOf(D3D12_RAYTRACING_INSTANCE_DESC) == 64);
    std.debug.assert(@alignOf(D3D12_RAYTRACING_INSTANCE_DESC) == 8);
}

pub const D3D12_RAYTRACING_GEOMETRY_DESC = extern struct {
    Type: D3D12_RAYTRACING_GEOMETRY_TYPE,
    Flags: D3D12_RAYTRACING_GEOMETRY_FLAGS,
    u: extern union {
        Triangles: D3D12_RAYTRACING_GEOMETRY_TRIANGLES_DESC,
        AABBs: D3D12_RAYTRACING_GEOMETRY_AABBS_DESC,
    },
};

pub const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS = extern struct {
    Type: D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE,
    Flags: D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS,
    NumDescs: UINT,
    DescsLayout: D3D12_ELEMENTS_LAYOUT,
    u: extern union {
        InstanceDescs: D3D12_GPU_VIRTUAL_ADDRESS,
        pGeometryDescs: [*]const D3D12_RAYTRACING_GEOMETRY_DESC,
        ppGeometryDescs: [*]const *D3D12_RAYTRACING_GEOMETRY_DESC,
    },
};

pub const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC = extern struct {
    DestAccelerationStructureData: D3D12_GPU_VIRTUAL_ADDRESS,
    Inputs: D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
    SourceAccelerationStructureData: D3D12_GPU_VIRTUAL_ADDRESS,
    ScratchAccelerationStructureData: D3D12_GPU_VIRTUAL_ADDRESS,
};

pub const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO = extern struct {
    ResultDataMaxSizeInBytes: UINT64,
    ScratchDataSizeInBytes: UINT64,
    UpdateScratchDataSizeInBytes: UINT64,
};

pub const ID3D12StateObject = extern struct {
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

pub const D3D12_DISPATCH_RAYS_DESC = extern struct {
    RayGenerationShaderRecord: D3D12_GPU_VIRTUAL_ADDRESS_RANGE,
    MissShaderTable: D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    HitGroupTable: D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    CallableShaderTable: D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    Width: UINT,
    Height: UINT,
    Depth: UINT,
};

pub const ID3D12GraphicsCommandList4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: ID3D12GraphicsCommandList1.VTable(Self),
        grcmdlist2: ID3D12GraphicsCommandList2.VTable(Self),
        grcmdlist3: ID3D12GraphicsCommandList3.VTable(Self),
        grcmdlist4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList1.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList2.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList3.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn BeginRenderPass(
                self: *T,
                num_render_targets: UINT,
                render_targets: ?[*]const D3D12_RENDER_PASS_RENDER_TARGET_DESC,
                depth_stencil: ?*const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC,
                flags: D3D12_RENDER_PASS_FLAGS,
            ) void {
                self.v.grcmdlist4.BeginRenderPass(self, num_render_targets, render_targets, depth_stencil, flags);
            }
            pub inline fn EndRenderPass(self: *T) void {
                self.v.grcmdlist4.EndRenderPass(self);
            }
            pub inline fn InitializeMetaCommand(
                self: *T,
                meta_cmd: *ID3D12MetaCommand,
                init_param_data: ?*const c_void,
                data_size: SIZE_T,
            ) void {
                self.v.grcmdlist4.InitializeMetaCommand(self, meta_cmd, init_param_data, data_size);
            }
            pub inline fn ExecuteMetaCommand(
                self: *T,
                meta_cmd: *ID3D12MetaCommand,
                exe_param_data: ?*const c_void,
                data_size: SIZE_T,
            ) void {
                self.v.grcmdlist4.InitializeMetaCommand(self, meta_cmd, exe_param_data, data_size);
            }
            pub inline fn BuildRaytracingAccelerationStructure(
                self: *T,
                desc: *const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC,
                num_post_build_descs: UINT,
                post_build_descs: ?[*]const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
            ) void {
                self.v.grcmdlist4.BuildRaytracingAccelerationStructure(self, desc, num_post_build_descs, post_build_descs);
            }
            pub inline fn EmitRaytracingAccelerationStructurePostbuildInfo(
                self: *T,
                desc: *const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
                num_src_accel_structs: UINT,
                src_accel_struct_data: [*]const D3D12_GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist4.EmitRaytracingAccelerationStructurePostbuildInfo(
                    self,
                    desc,
                    num_src_accel_structs,
                    src_accel_struct_data,
                );
            }
            pub inline fn CopyRaytracingAccelerationStructure(
                self: *T,
                dst_data: D3D12_GPU_VIRTUAL_ADDRESS,
                src_data: D3D12_GPU_VIRTUAL_ADDRESS,
                mode: D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE,
            ) void {
                self.v.grcmdlist4.CopyRaytracingAccelerationStructure(self, dst_data, src_data, mode);
            }
            pub inline fn SetPipelineState1(self: *T, state_obj: *ID3D12StateObject) void {
                self.v.grcmdlist4.SetPipelineState1(self, state_obj);
            }
            pub inline fn DispatchRays(self: *T, desc: *const D3D12_DISPATCH_RAYS_DESC) void {
                self.v.grcmdlist4.DispatchRays(self, desc);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            BeginRenderPass: fn (
                *T,
                UINT,
                ?[*]const D3D12_RENDER_PASS_RENDER_TARGET_DESC,
                ?*const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC,
                D3D12_RENDER_PASS_FLAGS,
            ) callconv(WINAPI) void,
            EndRenderPass: fn (*T) callconv(WINAPI) void,
            InitializeMetaCommand: fn (*T, *ID3D12MetaCommand, ?*const c_void, SIZE_T) callconv(WINAPI) void,
            ExecuteMetaCommand: fn (*T, *ID3D12MetaCommand, ?*const c_void, SIZE_T) callconv(WINAPI) void,
            BuildRaytracingAccelerationStructure: fn (
                *T,
                *const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC,
                UINT,
                ?[*]const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
            ) callconv(WINAPI) void,
            EmitRaytracingAccelerationStructurePostbuildInfo: fn (
                *T,
                *const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
                UINT,
                [*]const D3D12_GPU_VIRTUAL_ADDRESS,
            ) callconv(WINAPI) void,
            CopyRaytracingAccelerationStructure: fn (
                *T,
                D3D12_GPU_VIRTUAL_ADDRESS,
                D3D12_GPU_VIRTUAL_ADDRESS,
                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE,
            ) callconv(WINAPI) void,
            SetPipelineState1: fn (*T, *ID3D12StateObject) callconv(WINAPI) void,
            DispatchRays: fn (*T, *const D3D12_DISPATCH_RAYS_DESC) callconv(WINAPI) void,
        };
    }
};

pub const D3D12_RS_SET_SHADING_RATE_COMBINER_COUNT = 2;

pub const D3D12_SHADING_RATE = enum(UINT) {
    _1X1 = 0,
    _1X2 = 0x1,
    _2X1 = 0x4,
    _2X2 = 0x5,
    _2X4 = 0x6,
    _4X2 = 0x9,
    _4X4 = 0xa,
};

pub const D3D12_SHADING_RATE_COMBINER = enum(UINT) {
    PASSTHROUGH = 0,
    OVERRIDE = 1,
    COMBINER_MIN = 2,
    COMBINER_MAX = 3,
    COMBINER_SUM = 4,
};

pub const ID3D12GraphicsCommandList5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: ID3D12GraphicsCommandList1.VTable(Self),
        grcmdlist2: ID3D12GraphicsCommandList2.VTable(Self),
        grcmdlist3: ID3D12GraphicsCommandList3.VTable(Self),
        grcmdlist4: ID3D12GraphicsCommandList4.VTable(Self),
        grcmdlist5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList1.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList2.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList3.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList4.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn RSSetShadingRate(
                self: *T,
                base_shading_rate: D3D12_SHADING_RATE,
                combiners: ?[D3D12_RS_SET_SHADING_RATE_COMBINER_COUNT]D3D12_SHADING_RATE_COMBINER,
            ) void {
                self.v.grcmdlist5.RSSetShadingRate(self, base_shading_rate, combiners);
            }
            pub inline fn RSSetShadingRateImage(self: *T, shading_rate_img: ?*ID3D12Resource) void {
                self.v.grcmdlist5.RSSetShadingRateImage(self, shading_rate_img);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            RSSetShadingRate: fn (
                *T,
                D3D12_SHADING_RATE,
                ?[D3D12_RS_SET_SHADING_RATE_COMBINER_COUNT]D3D12_SHADING_RATE_COMBINER,
            ) callconv(WINAPI) void,
            RSSetShadingRateImage: fn (*T, ?*ID3D12Resource) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12GraphicsCommandList6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        cmdlist: ID3D12CommandList.VTable(Self),
        grcmdlist: ID3D12GraphicsCommandList.VTable(Self),
        grcmdlist1: ID3D12GraphicsCommandList1.VTable(Self),
        grcmdlist2: ID3D12GraphicsCommandList2.VTable(Self),
        grcmdlist3: ID3D12GraphicsCommandList3.VTable(Self),
        grcmdlist4: ID3D12GraphicsCommandList4.VTable(Self),
        grcmdlist5: ID3D12GraphicsCommandList5.VTable(Self),
        grcmdlist6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12CommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList1.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList2.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList3.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList4.Methods(Self);
    usingnamespace ID3D12GraphicsCommandList5.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn DispatchMesh(
                self: *T,
                thread_group_count_x: UINT,
                thread_group_count_y: UINT,
                thread_group_count_z: UINT,
            ) void {
                self.v.grcmdlist6.DispatchMesh(self, thread_group_count_x, thread_group_count_y, thread_group_count_z);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            DispatchMesh: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
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

pub const D3D12_MULTIPLE_FENCE_WAIT_FLAGS = enum(UINT) {
    ALL = 0,
    ANY = 1,
};

pub const D3D12_RESIDENCY_PRIORITY = enum(UINT) {
    MINIMUM = 0x28000000,
    LOW = 0x50000000,
    NORMAL = 0x78000000,
    HIGH = 0xa0010000,
    MAXIMUM = 0xc8000000,
};

pub const ID3D12Device1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreatePipelineLibrary(
                self: *T,
                blob: *const c_void,
                blob_length: SIZE_T,
                guid: *const GUID,
                library: *?*c_void,
            ) HRESULT {
                return self.v.device1.CreatePipelineLibrary(self, blob, blob_length, guid, library);
            }
            pub inline fn SetEventOnMultipleFenceCompletion(
                self: *T,
                fences: [*]const *ID3D12Fence,
                fence_values: [*]const UINT64,
                num_fences: UINT,
                flags: D3D12_MULTIPLE_FENCE_WAIT_FLAGS,
                event: HANDLE,
            ) HRESULT {
                return self.v.device1.SetEventOnMultipleFenceCompletion(
                    self,
                    fences,
                    fence_values,
                    num_fences,
                    flags,
                    event,
                );
            }
            pub inline fn SetResidencyPriority(
                self: *T,
                num_objects: UINT,
                objects: [*]const *ID3D12Pageable,
                priorities: [*]const D3D12_RESIDENCY_PRIORITY,
            ) HRESULT {
                return self.v.device1.SetResidencyPriority(self, num_objects, objects, priorities);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreatePipelineLibrary: fn (*T, *const c_void, SIZE_T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            SetEventOnMultipleFenceCompletion: fn (
                *T,
                [*]const *ID3D12Fence,
                [*]const UINT64,
                UINT,
                D3D12_MULTIPLE_FENCE_WAIT_FLAGS,
                HANDLE,
            ) callconv(WINAPI) HRESULT,
            SetResidencyPriority: fn (
                *T,
                UINT,
                [*]const *ID3D12Pageable,
                [*]const D3D12_RESIDENCY_PRIORITY,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_PIPELINE_STATE_SUBOBJECT_TYPE = enum(UINT) {
    ROOT_SIGNATURE = 0,
    VS = 1,
    PS = 2,
    DS = 3,
    HS = 4,
    GS = 5,
    CS = 6,
    STREAM_OUTPUT = 7,
    BLEND = 8,
    SAMPLE_MASK = 9,
    RASTERIZER = 10,
    DEPTH_STENCIL = 11,
    INPUT_LAYOUT = 12,
    IB_STRIP_CUT_VALUE = 13,
    PRIMITIVE_TOPOLOGY = 14,
    RENDER_TARGET_FORMATS = 15,
    DEPTH_STENCIL_FORMAT = 16,
    SAMPLE_DESC = 17,
    NODE_MASK = 18,
    CACHED_PSO = 19,
    FLAGS = 20,
    DEPTH_STENCIL1 = 21,
    VIEW_INSTANCING = 22,
    AS = 24,
    MS = 25,
    MAX_VALID = 26,
};

pub const D3D12_PIPELINE_STATE_STREAM_DESC = extern struct {
    SizeInBytes: SIZE_T,
    pPipelineStateSubobjectStream: *c_void,
};

pub const ID3D12Device2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreatePipelineState(
                self: *T,
                desc: *const D3D12_PIPELINE_STATE_STREAM_DESC,
                guid: *const GUID,
                pso: *?*c_void,
            ) HRESULT {
                return self.v.device2.CreatePipelineState(self, desc, guid, pso);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreatePipelineState: fn (
                *T,
                *const D3D12_PIPELINE_STATE_STREAM_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_RESIDENCY_FLAGS = packed struct {
    DENY_OVERBUDGET: bool align(4) = false,
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
    pub usingnamespace FlagsMixin(@This());
};

pub const ID3D12Device3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OpenExistingHeapFromAddress(
                self: *T,
                address: *const c_void,
                guid: *const GUID,
                heap: *?*c_void,
            ) HRESULT {
                return self.v.device3.OpenExistingHeapFromAddress(self, address, guid, heap);
            }
            pub inline fn OpenExistingHeapFromFileMapping(
                self: *T,
                file_mapping: HANDLE,
                guid: *const GUID,
                heap: *?*c_void,
            ) HRESULT {
                return self.v.device3.OpenExistingHeapFromFileMapping(self, file_mapping, guid, heap);
            }
            pub inline fn EnqueueMakeResident(
                self: *T,
                flags: D3D12_RESIDENCY_FLAGS,
                num_objects: UINT,
                objects: [*]const *ID3D12Pageable,
                fence_to_signal: *ID3D12Fence,
                fence_value_to_signal: UINT64,
            ) HRESULT {
                return self.v.device3.EnqueueMakeResident(
                    self,
                    flags,
                    num_objects,
                    objects,
                    fence_to_signal,
                    fence_value_to_signal,
                );
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            OpenExistingHeapFromAddress: fn (*T, *const c_void, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            OpenExistingHeapFromFileMapping: fn (*T, HANDLE, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            EnqueueMakeResident: fn (
                *T,
                D3D12_RESIDENCY_FLAGS,
                UINT,
                [*]const *ID3D12Pageable,
                *ID3D12Fence,
                UINT64,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_COMMAND_LIST_FLAGS = packed struct {
    __reserved0: bool align(4) = false,
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_RESOURCE_ALLOCATION_INFO1 = extern struct {
    Offset: UINT64,
    Alignment: UINT64,
    SizeInBytes: UINT64,
};

pub const ID3D12Device4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateCommandList1(
                self: *T,
                node_mask: UINT,
                cmdlist_type: D3D12_COMMAND_LIST_TYPE,
                flags: D3D12_COMMAND_LIST_FLAGS,
                guid: *const GUID,
                cmdlist: *?*c_void,
            ) HRESULT {
                return self.v.device4.CreateCommandList1(self, node_mask, cmdlist_type, flags, guid, cmdlist);
            }
            pub inline fn CreateProtectedResourceSession(
                self: *T,
                desc: *const D3D12_PROTECTED_RESOURCE_SESSION_DESC,
                guid: *const GUID,
                session: *?*c_void,
            ) HRESULT {
                return self.v.device4.CreateProtectedResourceSession(self, desc, guid, session);
            }
            pub inline fn CreateCommittedResource1(
                self: *T,
                heap_properties: *const D3D12_HEAP_PROPERTIES,
                heap_flags: D3D12_HEAP_FLAGS,
                desc: *const D3D12_RESOURCE_DESC,
                initial_state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                psession: ?*ID3D12ProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device4.CreateCommittedResource1(
                    self,
                    heap_properties,
                    heap_flags,
                    desc,
                    initial_state,
                    clear_value,
                    psession,
                    guid,
                    resource,
                );
            }
            pub inline fn CreateHeap1(
                self: *T,
                desc: *const D3D12_HEAP_DESC,
                psession: ?*ID3D12ProtectedResourceSession,
                guid: *const GUID,
                heap: ?*?*c_void,
            ) HRESULT {
                return self.v.device4.CreateHeap1(self, desc, psession, guid, heap);
            }
            pub inline fn CreateReservedResource1(
                self: *T,
                desc: *const D3D12_RESOURCE_DESC,
                initial_state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                psession: ?*ID3D12ProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device4.CreateReservedResource1(
                    self,
                    desc,
                    initial_state,
                    clear_value,
                    psession,
                    guid,
                    resource,
                );
            }
            pub inline fn GetResourceAllocationInfo1(
                self: *T,
                visible_mask: UINT,
                num_resource_descs: UINT,
                resource_descs: [*]const D3D12_RESOURCE_DESC,
                alloc_info: ?[*]D3D12_RESOURCE_ALLOCATION_INFO1,
            ) D3D12_RESOURCE_ALLOCATION_INFO {
                var desc: D3D12_RESOURCE_ALLOCATION_INFO = undefined;
                self.v.device4.GetResourceAllocationInfo1(
                    self,
                    &desc,
                    visible_mask,
                    num_resource_descs,
                    resource_descs,
                    alloc_info,
                );
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateCommandList1: fn (
                *T,
                UINT,
                D3D12_COMMAND_LIST_TYPE,
                D3D12_COMMAND_LIST_FLAGS,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateProtectedResourceSession: fn (
                *T,
                *const D3D12_PROTECTED_RESOURCE_SESSION_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateCommittedResource1: fn (
                *T,
                *const D3D12_HEAP_PROPERTIES,
                D3D12_HEAP_FLAGS,
                *const D3D12_RESOURCE_DESC,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                ?*ID3D12ProtectedResourceSession,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateHeap1: fn (
                *T,
                *const D3D12_HEAP_DESC,
                ?*ID3D12ProtectedResourceSession,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateReservedResource1: fn (
                *T,
                *const D3D12_RESOURCE_DESC,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                ?*ID3D12ProtectedResourceSession,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            GetResourceAllocationInfo1: fn (
                *T,
                *D3D12_RESOURCE_ALLOCATION_INFO,
                UINT,
                UINT,
                [*]const D3D12_RESOURCE_DESC,
                ?[*]D3D12_RESOURCE_ALLOCATION_INFO1,
            ) callconv(WINAPI) *D3D12_RESOURCE_ALLOCATION_INFO,
        };
    }
};

pub const D3D12_LIFETIME_STATE = enum(UINT) {
    IN_USE = 0,
    NOT_IN_USE = 1,
};

pub const ID3D12LifetimeOwner = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        ltowner: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn LifetimeStateUpdated(self: *T, new_state: D3D12_LIFETIME_STATE) void {
                self.v.ltowner.LifetimeStateUpdated(self, new_state);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            LifetimeStateUpdated: fn (*T, D3D12_LIFETIME_STATE) callconv(WINAPI) void,
        };
    }
};

pub const ID3D12Device5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: ID3D12Device4.VTable(Self),
        device5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace ID3D12Device4.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateLifetimeTracker(
                self: *T,
                owner: *ID3D12LifetimeOwner,
                guid: *const GUID,
                tracker: *?*c_void,
            ) HRESULT {
                return self.v.device5.CreateLifetimeTracker(self, owner, guid, tracker);
            }
            pub inline fn RemoveDevice(self: *T) void {
                self.v.device5.RemoveDevice(self);
            }
            pub inline fn EnumerateMetaCommands(
                self: *T,
                num_meta_cmds: *UINT,
                descs: ?[*]D3D12_META_COMMAND_DESC,
            ) HRESULT {
                return self.v.device5.EnumerateMetaCommands(self, num_meta_cmds, descs);
            }
            pub inline fn EnumerateMetaCommandParameters(
                self: *T,
                cmd_id: *const GUID,
                stage: D3D12_META_COMMAND_PARAMETER_STAGE,
                total_size: ?*UINT,
                param_count: *UINT,
                param_descs: ?[*]D3D12_META_COMMAND_PARAMETER_DESC,
            ) HRESULT {
                return self.v.device5.EnumerateMetaCommandParameters(
                    self,
                    cmd_id,
                    stage,
                    total_size,
                    param_count,
                    param_descs,
                );
            }
            pub inline fn CreateMetaCommand(
                self: *T,
                cmd_id: *const GUID,
                node_mask: UINT,
                creation_param_data: ?*const c_void,
                creation_param_data_size: SIZE_T,
                guid: *const GUID,
                meta_cmd: *?*c_void,
            ) HRESULT {
                return self.v.device5.CreateMetaCommand(
                    self,
                    cmd_id,
                    node_mask,
                    creation_param_data,
                    creation_param_data_size,
                    guid,
                    meta_cmd,
                );
            }
            pub inline fn CreateStateObject(
                self: *T,
                desc: *const D3D12_STATE_OBJECT_DESC,
                guid: *const GUID,
                state_object: *?*c_void,
            ) HRESULT {
                return self.v.device5.CreateStateObject(self, desc, guid, state_object);
            }
            pub inline fn GetRaytracingAccelerationStructurePrebuildInfo(
                self: *T,
                desc: *const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
                info: *D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO,
            ) void {
                self.v.device5.GetRaytracingAccelerationStructurePrebuildInfo(self, desc, info);
            }
            pub inline fn CheckDriverMatchingIdentifier(
                self: *T,
                serialized_data_type: D3D12_SERIALIZED_DATA_TYPE,
                identifier_to_check: *const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
            ) D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS {
                return self.v.device5.CheckDriverMatchingIdentifier(self, serialized_data_type, identifier_to_check);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateLifetimeTracker: fn (*T, *ID3D12LifetimeOwner, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            RemoveDevice: fn (self: *T) callconv(WINAPI) void,
            EnumerateMetaCommands: fn (*T, *UINT, ?[*]D3D12_META_COMMAND_DESC) callconv(WINAPI) HRESULT,
            EnumerateMetaCommandParameters: fn (
                *T,
                *const GUID,
                D3D12_META_COMMAND_PARAMETER_STAGE,
                ?*UINT,
                *UINT,
                ?[*]D3D12_META_COMMAND_PARAMETER_DESC,
            ) callconv(WINAPI) HRESULT,
            CreateMetaCommand: fn (
                *T,
                *const GUID,
                UINT,
                ?*const c_void,
                SIZE_T,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateStateObject: fn (
                *T,
                *const D3D12_STATE_OBJECT_DESC,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            GetRaytracingAccelerationStructurePrebuildInfo: fn (
                *T,
                *const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
                *D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO,
            ) callconv(WINAPI) void,
            CheckDriverMatchingIdentifier: fn (
                *T,
                D3D12_SERIALIZED_DATA_TYPE,
                *const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
            ) callconv(WINAPI) D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS,
        };
    }
};

pub const D3D12_BACKGROUND_PROCESSING_MODE = enum(UINT) {
    ALLOWED = 0,
    ALLOW_INTRUSIVE_MEASUREMENTS = 1,
    DISABLE_BACKGROUND_WORK = 2,
    DISABLE_PROFILING_BY_SYSTEM = 3,
};

pub const D3D12_MEASUREMENTS_ACTION = enum(UINT) {
    KEEP_ALL = 0,
    COMMIT_RESULTS = 1,
    COMMIT_RESULTS_HIGH_PRIORITY = 2,
    DISCARD_PREVIOUS = 3,
};

pub const ID3D12Device6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: ID3D12Device4.VTable(Self),
        device5: ID3D12Device5.VTable(Self),
        device6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace ID3D12Device4.Methods(Self);
    usingnamespace ID3D12Device5.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetBackgroundProcessingMode(
                self: *T,
                mode: D3D12_BACKGROUND_PROCESSING_MODE,
                measurements_action: D3D12_MEASUREMENTS_ACTION,
                event_to_signal_upon_completion: ?HANDLE,
                further_measurements_desired: ?*BOOL,
            ) HRESULT {
                return self.v.device6.SetBackgroundProcessingMode(
                    self,
                    mode,
                    measurements_action,
                    event_to_signal_upon_completion,
                    further_measurements_desired,
                );
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetBackgroundProcessingMode: fn (
                *T,
                D3D12_BACKGROUND_PROCESSING_MODE,
                D3D12_MEASUREMENTS_ACTION,
                ?HANDLE,
                ?*BOOL,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_PROTECTED_RESOURCE_SESSION_DESC1 = extern struct {
    NodeMask: UINT,
    Flags: D3D12_PROTECTED_RESOURCE_SESSION_FLAGS,
    ProtectionType: GUID,
};

pub const ID3D12Device7 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: ID3D12Device4.VTable(Self),
        device5: ID3D12Device5.VTable(Self),
        device6: ID3D12Device6.VTable(Self),
        device7: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace ID3D12Device4.Methods(Self);
    usingnamespace ID3D12Device5.Methods(Self);
    usingnamespace ID3D12Device6.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AddToStateObject(
                self: *T,
                addition: *const D3D12_STATE_OBJECT_DESC,
                state_object: *ID3D12StateObject,
                guid: *const GUID,
                new_state_object: *?*c_void,
            ) HRESULT {
                return self.v.device7.AddToStateObject(self, addition, state_object, guid, new_state_object);
            }
            pub inline fn CreateProtectedResourceSession1(
                self: *T,
                desc: *const D3D12_PROTECTED_RESOURCE_SESSION_DESC1,
                guid: *const GUID,
                session: *?*c_void,
            ) HRESULT {
                return self.v.device7.CreateProtectedResourceSession1(self, desc, guid, session);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            AddToStateObject: fn (
                *T,
                *const D3D12_STATE_OBJECT_DESC,
                *ID3D12StateObject,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateProtectedResourceSession1: fn (
                *T,
                *const D3D12_PROTECTED_RESOURCE_SESSION_DESC1,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_MIP_REGION = extern struct {
    Width: UINT,
    Height: UINT,
    Depth: UINT,
};

pub const D3D12_RESOURCE_DESC1 = extern struct {
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
    SamplerFeedbackMipRegion: D3D12_MIP_REGION,
};

pub const ID3D12Device8 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: ID3D12Device4.VTable(Self),
        device5: ID3D12Device5.VTable(Self),
        device6: ID3D12Device6.VTable(Self),
        device7: ID3D12Device7.VTable(Self),
        device8: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace ID3D12Device4.Methods(Self);
    usingnamespace ID3D12Device5.Methods(Self);
    usingnamespace ID3D12Device6.Methods(Self);
    usingnamespace ID3D12Device7.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetResourceAllocationInfo2(
                self: *T,
                visible_mask: UINT,
                num_resource_descs: UINT,
                resource_descs: *const D3D12_RESOURCE_DESC1,
                alloc_info: ?[*]D3D12_RESOURCE_ALLOCATION_INFO1,
            ) D3D12_RESOURCE_ALLOCATION_INFO {
                var desc: D3D12_RESOURCE_ALLOCATION_INFO = undefined;
                self.v.device8.GetResourceAllocationInfo2(
                    self,
                    &desc,
                    visible_mask,
                    num_resource_descs,
                    resource_descs,
                    alloc_info,
                );
                return desc;
            }
            pub inline fn CreateCommittedResource2(
                self: *T,
                heap_properties: *const D3D12_HEAP_PROPERTIES,
                heap_flags: D3D12_HEAP_FLAGS,
                desc: *const D3D12_RESOURCE_DESC1,
                initial_state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                prsession: ?*ID3D12ProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device8.CreateCommittedResource2(
                    self,
                    heap_properties,
                    heap_flags,
                    desc,
                    initial_state,
                    clear_value,
                    prsession,
                    guid,
                    resource,
                );
            }
            pub inline fn CreatePlacedResource1(
                self: *T,
                heap: *ID3D12Heap,
                heap_offset: UINT64,
                desc: *const D3D12_RESOURCE_DESC1,
                initial_state: D3D12_RESOURCE_STATES,
                clear_value: ?*const D3D12_CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*c_void,
            ) HRESULT {
                return self.v.device8.CreatePlacedResource1(
                    self,
                    heap,
                    heap_offset,
                    desc,
                    initial_state,
                    clear_value,
                    guid,
                    resource,
                );
            }
            pub inline fn CreateSamplerFeedbackUnorderedAccessView(
                self: *T,
                targeted_resource: ?*ID3D12Resource,
                feedback_resource: ?*ID3D12Resource,
                dest_descriptor: D3D12_CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device8.CreateSamplerFeedbackUnorderedAccessView(
                    self,
                    targeted_resource,
                    feedback_resource,
                    dest_descriptor,
                );
            }
            pub inline fn GetCopyableFootprints1(
                self: *T,
                desc: *const D3D12_RESOURCE_DESC1,
                first_subresource: UINT,
                num_subresources: UINT,
                base_offset: UINT64,
                layouts: ?[*]D3D12_PLACED_SUBRESOURCE_FOOTPRINT,
                num_rows: ?[*]UINT,
                row_size_in_bytes: ?[*]UINT64,
                total_bytes: ?*UINT64,
            ) void {
                self.v.device8.GetCopyableFootprints1(
                    self,
                    desc,
                    first_subresource,
                    num_subresources,
                    base_offset,
                    layouts,
                    num_rows,
                    row_size_in_bytes,
                    total_bytes,
                );
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetResourceAllocationInfo2: fn (
                *T,
                UINT,
                UINT,
                *const D3D12_RESOURCE_DESC1,
                ?[*]D3D12_RESOURCE_ALLOCATION_INFO1,
            ) callconv(WINAPI) D3D12_RESOURCE_ALLOCATION_INFO,
            CreateCommittedResource2: fn (
                *T,
                *const D3D12_HEAP_PROPERTIES,
                D3D12_HEAP_FLAGS,
                *const D3D12_RESOURCE_DESC1,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                ?*ID3D12ProtectedResourceSession,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreatePlacedResource1: fn (
                *T,
                *ID3D12Heap,
                UINT64,
                *const D3D12_RESOURCE_DESC1,
                D3D12_RESOURCE_STATES,
                ?*const D3D12_CLEAR_VALUE,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            CreateSamplerFeedbackUnorderedAccessView: fn (
                *T,
                ?*ID3D12Resource,
                ?*ID3D12Resource,
                D3D12_CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            GetCopyableFootprints1: fn (
                *T,
                *const D3D12_RESOURCE_DESC1,
                UINT,
                UINT,
                UINT64,
                ?[*]D3D12_PLACED_SUBRESOURCE_FOOTPRINT,
                ?[*]UINT,
                ?[*]UINT64,
                ?*UINT64,
            ) callconv(WINAPI) void,
        };
    }
};

pub const D3D12_SHADER_CACHE_KIND_FLAGS = packed struct {
    IMPLICIT_D3D_CACHE_FOR_DRIVER: bool align(4) = false, // 0x1
    IMPLICIT_D3D_CONVERSIONS: bool = false, // 0x2
    IMPLICIT_DRIVER_MANAGED: bool = false, // 0x4
    APPLICATION_MANAGED: bool = false, // 0x8
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

pub const D3D12_SHADER_CACHE_CONTROL_FLAGS = packed struct {
    DISABLE: bool align(4) = false, // 0x1
    ENABLE: bool = false, // 0x2
    CLEAR: bool = false, // 0x4
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_SHADER_CACHE_MODE = enum(UINT) {
    MEMORY = 0,
    DISK = 1,
};

pub const D3D12_SHADER_CACHE_FLAGS = packed struct {
    DRIVER_VERSIONED: bool align(4) = false, // 0x1
    USE_WORKING_DIR: bool = false, // 0x2
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_SHADER_CACHE_SESSION_DESC = extern struct {
    Identifier: GUID,
    Mode: D3D12_SHADER_CACHE_MODE,
    Flags: D3D12_SHADER_CACHE_FLAGS,
    MaximumInMemoryCacheSizeBytes: UINT,
    MaximumInMemoryCacheEntries: UINT,
    MaximumValueFileSizeBytes: UINT,
    Version: UINT64,
};

pub const ID3D12Device9 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        device: ID3D12Device.VTable(Self),
        device1: ID3D12Device1.VTable(Self),
        device2: ID3D12Device2.VTable(Self),
        device3: ID3D12Device3.VTable(Self),
        device4: ID3D12Device4.VTable(Self),
        device5: ID3D12Device5.VTable(Self),
        device6: ID3D12Device6.VTable(Self),
        device7: ID3D12Device7.VTable(Self),
        device8: ID3D12Device8.VTable(Self),
        device9: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12Device.Methods(Self);
    usingnamespace ID3D12Device1.Methods(Self);
    usingnamespace ID3D12Device2.Methods(Self);
    usingnamespace ID3D12Device3.Methods(Self);
    usingnamespace ID3D12Device4.Methods(Self);
    usingnamespace ID3D12Device5.Methods(Self);
    usingnamespace ID3D12Device6.Methods(Self);
    usingnamespace ID3D12Device7.Methods(Self);
    usingnamespace ID3D12Device8.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateShaderCacheSession(
                self: *T,
                desc: *const D3D12_SHADER_CACHE_SESSION_DESC,
                guid: *const GUID,
                session: ?*?*c_void,
            ) HRESULT {
                return self.v.device9.CreateShaderCacheSession(self, desc, guid, session);
            }
            pub inline fn ShaderCacheControl(
                self: *T,
                kinds: D3D12_SHADER_CACHE_KIND_FLAGS,
                control: D3D12_SHADER_CACHE_CONTROL_FLAGS,
            ) HRESULT {
                return self.v.device9.ShaderCacheControl(self, kinds, control);
            }
            pub inline fn CreateCommandQueue1(
                self: *T,
                desc: *const D3D12_COMMAND_QUEUE_DESC,
                creator_id: *const GUID,
                guid: *const GUID,
                cmdqueue: *?*c_void,
            ) HRESULT {
                return self.v.device9.CreateCommandQueue1(self, desc, creator_id, guid, cmdqueue);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateShaderCacheSession: fn (
                *T,
                *const D3D12_SHADER_CACHE_SESSION_DESC,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            ShaderCacheControl: fn (
                *T,
                D3D12_SHADER_CACHE_KIND_FLAGS,
                D3D12_SHADER_CACHE_CONTROL_FLAGS,
            ) callconv(WINAPI) HRESULT,
            CreateCommandQueue1: fn (
                *T,
                *const D3D12_COMMAND_QUEUE_DESC,
                *const GUID,
                *const GUID,
                *?*c_void,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const D3D12_PROTECTED_SESSION_STATUS = enum(UINT) {
    OK = 0,
    INVALID = 1,
};

pub const ID3D12ProtectedSession = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        psession: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetStatusFence(self: *T, guid: *const GUID, fence: ?*?*c_void) HRESULT {
                return self.v.psession.GetStatusFence(self, guid, fence);
            }
            pub inline fn GetSessionStatus(self: *T) D3D12_PROTECTED_SESSION_STATUS {
                return self.v.psession.GetSessionStatus(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetStatusFence: fn (*T, *const GUID, ?*?*c_void) callconv(WINAPI) HRESULT,
            GetSessionStatus: fn (*T) callconv(WINAPI) D3D12_PROTECTED_SESSION_STATUS,
        };
    }
};

pub const D3D12_PROTECTED_RESOURCE_SESSION_FLAGS = packed struct {
    __reserved0: bool align(4) = false,
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
    pub usingnamespace FlagsMixin(@This());
};

pub const D3D12_PROTECTED_RESOURCE_SESSION_DESC = extern struct {
    NodeMask: UINT,
    Flags: D3D12_PROTECTED_RESOURCE_SESSION_FLAGS,
};

pub const ID3D12ProtectedResourceSession = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: ID3D12Object.VTable(Self),
        devchild: ID3D12DeviceChild.VTable(Self),
        psession: ID3D12ProtectedSession.VTable(Self),
        prsession: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D12Object.Methods(Self);
    usingnamespace ID3D12DeviceChild.Methods(Self);
    usingnamespace ID3D12ProtectedSession.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) D3D12_PROTECTED_RESOURCE_SESSION_DESC {
                var desc: D3D12_PROTECTED_RESOURCE_SESSION_DESC = undefined;
                self.v.prsession.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (
                *T,
                *D3D12_PROTECTED_RESOURCE_SESSION_DESC,
            ) callconv(WINAPI) *D3D12_PROTECTED_RESOURCE_SESSION_DESC,
        };
    }
};

pub extern "d3d12" fn D3D12GetDebugInterface(*const GUID, ?*?*c_void) callconv(WINAPI) HRESULT;
pub extern "d3d12" fn D3D12CreateDevice(
    ?*IUnknown,
    D3D_FEATURE_LEVEL,
    *const GUID,
    ?*?*c_void,
) callconv(WINAPI) HRESULT;

pub const IID_ID3D12Device = GUID{
    .Data1 = 0x189819f1,
    .Data2 = 0x1db6,
    .Data3 = 0x4b57,
    .Data4 = .{ 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7 },
};
pub const IID_ID3D12Device1 = GUID{
    .Data1 = 0x77acce80,
    .Data2 = 0x638e,
    .Data3 = 0x4e65,
    .Data4 = .{ 0x88, 0x95, 0xc1, 0xf2, 0x33, 0x86, 0x86, 0x3e },
};
pub const IID_ID3D12Device2 = GUID{
    .Data1 = 0x30baa41e,
    .Data2 = 0xb15b,
    .Data3 = 0x475c,
    .Data4 = .{ 0xa0, 0xbb, 0x1a, 0xf5, 0xc5, 0xb6, 0x43, 0x28 },
};
pub const IID_ID3D12Device3 = GUID{
    .Data1 = 0x81dadc15,
    .Data2 = 0x2bad,
    .Data3 = 0x4392,
    .Data4 = .{ 0x93, 0xc5, 0x10, 0x13, 0x45, 0xc4, 0xaa, 0x98 },
};
pub const IID_ID3D12Device4 = GUID{
    .Data1 = 0xe865df17,
    .Data2 = 0xa9ee,
    .Data3 = 0x46f9,
    .Data4 = .{ 0xa4, 0x63, 0x30, 0x98, 0x31, 0x5a, 0xa2, 0xe5 },
};
pub const IID_ID3D12Device5 = GUID{
    .Data1 = 0x8b4f173a,
    .Data2 = 0x2fea,
    .Data3 = 0x4b80,
    .Data4 = .{ 0x8f, 0x58, 0x43, 0x07, 0x19, 0x1a, 0xb9, 0x5d },
};
pub const IID_ID3D12Device6 = GUID{
    .Data1 = 0xc70b221b,
    .Data2 = 0x40e4,
    .Data3 = 0x4a17,
    .Data4 = .{ 0x89, 0xaf, 0x02, 0x5a, 0x07, 0x27, 0xa6, 0xdc },
};
pub const IID_ID3D12Device7 = GUID{
    .Data1 = 0x5c014b53,
    .Data2 = 0x68a1,
    .Data3 = 0x4b9b,
    .Data4 = .{ 0x8b, 0xd1, 0xdd, 0x60, 0x46, 0xb9, 0x35, 0x8b },
};
pub const IID_ID3D12Device8 = GUID{
    .Data1 = 0x9218E6BB,
    .Data2 = 0xF944,
    .Data3 = 0x4F7E,
    .Data4 = .{ 0xA7, 0x5C, 0xB1, 0xB2, 0xC7, 0xB7, 0x01, 0xF3 },
};
pub const IID_ID3D12Device9 = GUID{
    .Data1 = 0x4c80e962,
    .Data2 = 0xf032,
    .Data3 = 0x4f60,
    .Data4 = .{ 0xbc, 0x9e, 0xeb, 0xc2, 0xcf, 0xa1, 0xd8, 0x3c },
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
pub const IID_ID3D12GraphicsCommandList1 = GUID{
    .Data1 = 0x553103fb,
    .Data2 = 0x1fe7,
    .Data3 = 0x4557,
    .Data4 = .{ 0xbb, 0x38, 0x94, 0x6d, 0x7d, 0x0e, 0x7c, 0xa7 },
};
pub const IID_ID3D12GraphicsCommandList2 = GUID{
    .Data1 = 0x38C3E584,
    .Data2 = 0xFF17,
    .Data3 = 0x412C,
    .Data4 = .{ 0x91, 0x50, 0x4F, 0xC6, 0xF9, 0xD7, 0x2A, 0x28 },
};
pub const IID_ID3D12GraphicsCommandList3 = GUID{
    .Data1 = 0x6FDA83A7,
    .Data2 = 0xB84C,
    .Data3 = 0x4E38,
    .Data4 = .{ 0x9A, 0xC8, 0xC7, 0xBD, 0x22, 0x01, 0x6B, 0x3D },
};
pub const IID_ID3D12GraphicsCommandList4 = GUID{
    .Data1 = 0x8754318e,
    .Data2 = 0xd3a9,
    .Data3 = 0x4541,
    .Data4 = .{ 0x98, 0xcf, 0x64, 0x5b, 0x50, 0xdc, 0x48, 0x74 },
};
pub const IID_ID3D12GraphicsCommandList5 = GUID{
    .Data1 = 0x55050859,
    .Data2 = 0x4024,
    .Data3 = 0x474c,
    .Data4 = .{ 0x87, 0xf5, 0x64, 0x72, 0xea, 0xee, 0x44, 0xea },
};
pub const IID_ID3D12GraphicsCommandList6 = GUID{
    .Data1 = 0xc3827890,
    .Data2 = 0xe548,
    .Data3 = 0x4cfa,
    .Data4 = .{ 0x96, 0xcf, 0x56, 0x89, 0xa9, 0x37, 0x0f, 0x80 },
};
