const std = @import("std");
const windows = @import("windows.zig");
const dxgi = @import("dxgi.zig");
const d3d = @import("d3dcommon.zig");
const UINT = windows.UINT;
const IUnknown = windows.IUnknown;
const HRESULT = windows.HRESULT;
const GUID = windows.GUID;
const LUID = windows.LUID;
const WINAPI = windows.WINAPI;
const FLOAT = windows.FLOAT;
const LPCWSTR = windows.LPCWSTR;
const LPCSTR = windows.LPCSTR;
const UINT8 = windows.UINT8;
const UINT16 = windows.UINT16;
const UINT32 = windows.UINT32;
const UINT64 = windows.UINT64;
const INT = windows.INT;
const INT8 = windows.INT8;
const BYTE = windows.BYTE;
const DWORD = windows.DWORD;
const SIZE_T = windows.SIZE_T;
const HANDLE = windows.HANDLE;
const SECURITY_ATTRIBUTES = windows.SECURITY_ATTRIBUTES;
const BOOL = windows.BOOL;
const FALSE = windows.FALSE;
const TRUE = windows.TRUE;

pub const RESOURCE_BARRIER_ALL_SUBRESOURCES = 0xffff_ffff;

pub const SHADER_IDENTIFIER_SIZE_IN_BYTES = 32;

pub const GPU_VIRTUAL_ADDRESS = UINT64;

pub const PRIMITIVE_TOPOLOGY = d3d.PRIMITIVE_TOPOLOGY;

pub const CPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: UINT64,
};

pub const GPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: UINT64,
};

pub const PRIMITIVE_TOPOLOGY_TYPE = enum(UINT) {
    UNDEFINED = 0,
    POINT = 1,
    LINE = 2,
    TRIANGLE = 3,
    PATCH = 4,
};

pub const HEAP_TYPE = enum(UINT) {
    DEFAULT = 1,
    UPLOAD = 2,
    READBACK = 3,
    CUSTOM = 4,
};

pub const CPU_PAGE_PROPERTY = enum(UINT) {
    UNKNOWN = 0,
    NOT_AVAILABLE = 1,
    WRITE_COMBINE = 2,
    WRITE_BACK = 3,
};

pub const MEMORY_POOL = enum(UINT) {
    UNKNOWN = 0,
    L0 = 1,
    L1 = 2,
};

pub const HEAP_PROPERTIES = extern struct {
    Type: HEAP_TYPE,
    CPUPageProperty: CPU_PAGE_PROPERTY,
    MemoryPoolPreference: MEMORY_POOL,
    CreationNodeMask: UINT,
    VisibleNodeMask: UINT,

    pub fn initType(heap_type: HEAP_TYPE) HEAP_PROPERTIES {
        var v = std.mem.zeroes(@This());
        v = HEAP_PROPERTIES{
            .Type = heap_type,
            .CPUPageProperty = .UNKNOWN,
            .MemoryPoolPreference = .UNKNOWN,
            .CreationNodeMask = 0,
            .VisibleNodeMask = 0,
        };
        return v;
    }
};

pub const HEAP_FLAGS = UINT;
pub const HEAP_FLAG_NONE = 0;
pub const HEAP_FLAG_SHARED = 0x1;
pub const HEAP_FLAG_DENY_BUFFERS = 0x4;
pub const HEAP_FLAG_ALLOW_DISPLAY = 0x8;
pub const HEAP_FLAG_SHARED_CROSS_ADAPTER = 0x20;
pub const HEAP_FLAG_DENY_RT_DS_TEXTURES = 0x40;
pub const HEAP_FLAG_DENY_NON_RT_DS_TEXTURES = 0x80;
pub const HEAP_FLAG_HARDWARE_PROTECTED = 0x100;
pub const HEAP_FLAG_ALLOW_WRITE_WATCH = 0x200;
pub const HEAP_FLAG_ALLOW_SHADER_ATOMICS = 0x400;
pub const HEAP_FLAG_CREATE_NOT_RESIDENT = 0x800;
pub const HEAP_FLAG_CREATE_NOT_ZEROED = 0x1000;
pub const HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES = 0;
pub const HEAP_FLAG_ALLOW_ONLY_BUFFERS = 0xc0;
pub const HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES = 0x44;
pub const HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES = 0x84;

pub const HEAP_DESC = extern struct {
    SizeInBytes: UINT64,
    Properties: HEAP_PROPERTIES,
    Alignment: UINT64,
    Flags: HEAP_FLAGS,
};

pub const RANGE = extern struct {
    Begin: UINT64,
    End: UINT64,
};

pub const BOX = extern struct {
    left: UINT,
    top: UINT,
    front: UINT,
    right: UINT,
    bottom: UINT,
    back: UINT,
};

pub const RESOURCE_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE2D = 3,
    TEXTURE3D = 4,
};

pub const TEXTURE_LAYOUT = enum(UINT) {
    UNKNOWN = 0,
    ROW_MAJOR = 1,
    _64KB_UNDEFINED_SWIZZLE = 2,
    _64KB_STANDARD_SWIZZLE = 3,
};

pub const RESOURCE_FLAGS = UINT;
pub const RESOURCE_FLAG_NONE = 0;
pub const RESOURCE_FLAG_ALLOW_RENDER_TARGET = 0x1;
pub const RESOURCE_FLAG_ALLOW_DEPTH_STENCIL = 0x2;
pub const RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS = 0x4;
pub const RESOURCE_FLAG_DENY_SHADER_RESOURCE = 0x8;
pub const RESOURCE_FLAG_ALLOW_CROSS_ADAPTER = 0x10;
pub const RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS = 0x20;
pub const RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY = 0x40;
pub const RESOURCE_FLAG_VIDEO_ENCODE_REFERENCE_ONLY = 0x80;

pub const RESOURCE_DESC = extern struct {
    Dimension: RESOURCE_DIMENSION,
    Alignment: UINT64,
    Width: UINT64,
    Height: UINT,
    DepthOrArraySize: UINT16,
    MipLevels: UINT16,
    Format: dxgi.FORMAT,
    SampleDesc: dxgi.SAMPLE_DESC,
    Layout: TEXTURE_LAYOUT,
    Flags: RESOURCE_FLAGS,

    pub fn initBuffer(width: UINT64) RESOURCE_DESC {
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
            .Flags = RESOURCE_FLAG_NONE,
        };
        return v;
    }

    pub fn initTex2d(format: dxgi.FORMAT, width: UINT64, height: UINT, mip_levels: u32) RESOURCE_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .Dimension = .TEXTURE2D,
            .Alignment = 0,
            .Width = width,
            .Height = height,
            .DepthOrArraySize = 1,
            .MipLevels = @intCast(u16, mip_levels),
            .Format = format,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .Layout = .UNKNOWN,
            .Flags = RESOURCE_FLAG_NONE,
        };
        return v;
    }
};

pub const FENCE_FLAGS = UINT;
pub const FENCE_FLAG_NONE = 0;
pub const FENCE_FLAG_SHARED = 0x1;
pub const FENCE_FLAG_SHARED_CROSS_ADAPTER = 0x2;
pub const FENCE_FLAG_NON_MONITORED = 0x4;

pub const DESCRIPTOR_HEAP_TYPE = enum(UINT) {
    CBV_SRV_UAV = 0,
    SAMPLER = 1,
    RTV = 2,
    DSV = 3,
};

pub const DESCRIPTOR_HEAP_FLAGS = UINT;
pub const DESCRIPTOR_HEAP_FLAG_NONE = 0;
pub const DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE = 0x1;

pub const DESCRIPTOR_HEAP_DESC = extern struct {
    Type: DESCRIPTOR_HEAP_TYPE,
    NumDescriptors: UINT,
    Flags: DESCRIPTOR_HEAP_FLAGS,
    NodeMask: UINT,
};

pub const DESCRIPTOR_RANGE_TYPE = enum(UINT) {
    SRV = 0,
    UAV = 1,
    CBV = 2,
    SAMPLER = 3,
};

pub const DESCRIPTOR_RANGE = extern struct {
    RangeType: DESCRIPTOR_RANGE_TYPE,
    NumDescriptors: UINT,
    BaseShaderRegister: UINT,
    RegisterSpace: UINT,
    OffsetInDescriptorsFromStart: UINT,
};

pub const ROOT_DESCRIPTOR_TABLE = extern struct {
    NumDescriptorRanges: UINT,
    pDescriptorRanges: ?[*]const DESCRIPTOR_RANGE,
};

pub const ROOT_CONSTANTS = extern struct {
    ShaderRegister: UINT,
    RegisterSpace: UINT,
    Num32BitValues: UINT,
};

pub const ROOT_DESCRIPTOR = extern struct {
    ShaderRegister: UINT,
    RegisterSpace: UINT,
};

pub const ROOT_PARAMETER_TYPE = enum(UINT) {
    DESCRIPTOR_TABLE = 0,
    _32BIT_CONSTANTS = 1,
    CBV = 2,
    SRV = 3,
    UAV = 4,
};

pub const SHADER_VISIBILITY = enum(UINT) {
    ALL = 0,
    VERTEX = 1,
    HULL = 2,
    DOMAIN = 3,
    GEOMETRY = 4,
    PIXEL = 5,
    AMPLIFICATION = 6,
    MESH = 7,
};

pub const ROOT_PARAMETER = extern struct {
    ParameterType: ROOT_PARAMETER_TYPE,
    u: extern union {
        DescriptorTable: ROOT_DESCRIPTOR_TABLE,
        Constants: ROOT_CONSTANTS,
        Descriptor: ROOT_DESCRIPTOR,
    },
    ShaderVisibility: SHADER_VISIBILITY,
};

pub const STATIC_BORDER_COLOR = enum(UINT) {
    TRANSPARENT_BLACK = 0,
    OPAQUE_BLACK = 1,
    OPAQUE_WHITE = 2,
};

pub const STATIC_SAMPLER_DESC = extern struct {
    Filter: FILTER,
    AddressU: TEXTURE_ADDRESS_MODE,
    AddressV: TEXTURE_ADDRESS_MODE,
    AddressW: TEXTURE_ADDRESS_MODE,
    MipLODBias: FLOAT,
    MaxAnisotropy: UINT,
    ComparisonFunc: COMPARISON_FUNC,
    BorderColor: STATIC_BORDER_COLOR,
    MinLOD: FLOAT,
    MaxLOD: FLOAT,
    ShaderRegister: UINT,
    RegisterSpace: UINT,
    ShaderVisibility: SHADER_VISIBILITY,
};

pub const ROOT_SIGNATURE_FLAGS = UINT;
pub const ROOT_SIGNATURE_FLAG_NONE: ROOT_SIGNATURE_FLAGS = 0;
pub const ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT: ROOT_SIGNATURE_FLAGS = 0x1;
pub const ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x2;
pub const ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x4;
pub const ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x8;
pub const ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x10;
pub const ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x20;
pub const ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT: ROOT_SIGNATURE_FLAGS = 0x40;
pub const ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE: ROOT_SIGNATURE_FLAGS = 0x80;
pub const ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x100;
pub const ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS: ROOT_SIGNATURE_FLAGS = 0x200;
pub const ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED: ROOT_SIGNATURE_FLAGS = 0x400;
pub const ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED: ROOT_SIGNATURE_FLAGS = 0x800;

pub const ROOT_SIGNATURE_DESC = extern struct {
    NumParamenters: UINT,
    pParameters: ?[*]const ROOT_PARAMETER,
    NumStaticSamplers: UINT,
    pStaticSamplers: ?[*]const STATIC_SAMPLER_DESC,
    Flags: ROOT_SIGNATURE_FLAGS,
};

pub const DESCRIPTOR_RANGE_FLAGS = UINT;
pub const DESCRIPTOR_RANGE_FLAG_NONE: DESCRIPTOR_RANGE_FLAGS = 0;
pub const DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE: DESCRIPTOR_RANGE_FLAGS = 0x1;
pub const DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE: DESCRIPTOR_RANGE_FLAGS = 0x2;
pub const DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE: DESCRIPTOR_RANGE_FLAGS = 0x4;
pub const DESCRIPTOR_RANGE_FLAG_DATA_STATIC: DESCRIPTOR_RANGE_FLAGS = 0x8;
pub const DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_STATIC_KEEPING_BUFFER_BOUNDS_CHECKS: DESCRIPTOR_RANGE_FLAGS = 0x10000;

pub const DESCRIPTOR_RANGE1 = extern struct {
    RangeType: DESCRIPTOR_RANGE_TYPE,
    NumDescriptors: UINT,
    BaseShaderRegister: UINT,
    RegisterSpace: UINT,
    Flags: DESCRIPTOR_RANGE_FLAGS,
    OffsetInDescriptorsFromTableStart: UINT,
};

pub const ROOT_DESCRIPTOR_TABLE1 = extern struct {
    NumDescriptorRanges: UINT,
    pDescriptorRanges: ?[*]const DESCRIPTOR_RANGE1,
};

pub const ROOT_DESCRIPTOR_FLAGS = UINT;
pub const ROOT_DESCRIPTOR_FLAG_NONE: ROOT_DESCRIPTOR_FLAGS = 0;
pub const ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE: ROOT_DESCRIPTOR_FLAGS = 0x2;
pub const ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE: ROOT_DESCRIPTOR_FLAGS = 0x4;
pub const ROOT_DESCRIPTOR_FLAG_DATA_STATIC: ROOT_DESCRIPTOR_FLAGS = 0x8;

pub const ROOT_DESCRIPTOR1 = extern struct {
    ShaderRegister: UINT,
    RegisterSpace: UINT,
    Flags: ROOT_DESCRIPTOR_FLAGS,
};

pub const ROOT_PARAMETER1 = extern struct {
    ParameterType: ROOT_PARAMETER_TYPE,
    u: extern union {
        DescriptorTable: ROOT_DESCRIPTOR_TABLE1,
        Constants: ROOT_CONSTANTS,
        Descriptor: ROOT_DESCRIPTOR1,
    },
    ShaderVisibility: SHADER_VISIBILITY,
};

pub const ROOT_SIGNATURE_DESC1 = extern struct {
    NumParamenters: UINT,
    pParameters: ?[*]const ROOT_PARAMETER1,
    NumStaticSamplers: UINT,
    pStaticSamplers: ?[*]const STATIC_SAMPLER_DESC,
    Flags: ROOT_SIGNATURE_FLAGS,
};

pub const ROOT_SIGNATURE_VERSION = enum(UINT) {
    VERSION_1_0 = 0x1,
    VERSION_1_1 = 0x2,
};

pub const VERSIONED_ROOT_SIGNATURE_DESC = extern struct {
    Version: ROOT_SIGNATURE_VERSION,
    u: extern union {
        Desc_1_0: ROOT_SIGNATURE_DESC,
        Desc_1_1: ROOT_SIGNATURE_DESC1,
    },
};

pub const COMMAND_LIST_TYPE = enum(UINT) {
    DIRECT = 0,
    BUNDLE = 1,
    COMPUTE = 2,
    COPY = 3,
    VIDEO_DECODE = 4,
    VIDEO_PROCESS = 5,
    VIDEO_ENCODE = 6,
};

pub const RESOURCE_BARRIER_TYPE = enum(UINT) {
    TRANSITION = 0,
    ALIASING = 1,
    UAV = 2,
};

pub const RESOURCE_TRANSITION_BARRIER = extern struct {
    pResource: *IResource,
    Subresource: UINT,
    StateBefore: RESOURCE_STATES,
    StateAfter: RESOURCE_STATES,
};

pub const RESOURCE_ALIASING_BARRIER = extern struct {
    pResourceBefore: *IResource,
    pResourceAfter: *IResource,
};

pub const RESOURCE_UAV_BARRIER = extern struct {
    pResource: *IResource,
};

pub const RESOURCE_BARRIER_FLAGS = UINT;
pub const RESOURCE_BARRIER_FLAG_NONE = 0;
pub const RESOURCE_BARRIER_FLAG_BEGIN_ONLY = 0x1;
pub const RESOURCE_BARRIER_FLAG_END_ONLY = 0x2;

pub const RESOURCE_BARRIER = extern struct {
    Type: RESOURCE_BARRIER_TYPE,
    Flags: RESOURCE_BARRIER_FLAGS,
    u: extern union {
        Transition: RESOURCE_TRANSITION_BARRIER,
        Aliasing: RESOURCE_ALIASING_BARRIER,
        UAV: RESOURCE_UAV_BARRIER,
    },

    pub fn initUav(resource: *IResource) RESOURCE_BARRIER {
        var v = std.mem.zeroes(@This());
        v = .{ .Type = .UAV, .Flags = 0, .u = .{ .UAV = .{ .pResource = resource } } };
        return v;
    }
};

pub const SUBRESOURCE_FOOTPRINT = extern struct {
    Format: dxgi.FORMAT,
    Width: UINT,
    Height: UINT,
    Depth: UINT,
    RowPitch: UINT,
};

pub const PLACED_SUBRESOURCE_FOOTPRINT = extern struct {
    Offset: UINT64,
    Footprint: SUBRESOURCE_FOOTPRINT,
};

pub const TEXTURE_COPY_TYPE = enum(UINT) {
    SUBRESOURCE_INDEX = 0,
    PLACED_FOOTPRINT = 1,
};

pub const TEXTURE_COPY_LOCATION = extern struct {
    pResource: *IResource,
    Type: TEXTURE_COPY_TYPE,
    u: extern union {
        PlacedFootprint: PLACED_SUBRESOURCE_FOOTPRINT,
        SubresourceIndex: UINT,
    },
};

pub const TILED_RESOURCE_COORDINATE = extern struct {
    X: UINT,
    Y: UINT,
    Z: UINT,
    Subresource: UINT,
};

pub const TILE_REGION_SIZE = extern struct {
    NumTiles: UINT,
    UseBox: BOOL,
    Width: UINT,
    Height: UINT16,
    Depth: UINT16,
};

pub const TILE_RANGE_FLAGS = UINT;
pub const TILE_RANGE_FLAG_NONE = 0;
pub const TILE_RANGE_FLAG_NULL = 0x1;
pub const TILE_RANGE_FLAG_SKIP = 0x2;
pub const TILE_RANGE_FLAG_REUSE_SINGLE_TILE = 0x4;

pub const SUBRESOURCE_TILING = extern struct {
    WidthInTiles: UINT,
    HeightInTiles: UINT16,
    DepthInTiles: UINT16,
    StartTileIndexInOverallResource: UINT,
};

pub const TILE_SHAPE = extern struct {
    WidthInTexels: UINT,
    HeightInTexels: UINT,
    DepthInTexels: UINT,
};

pub const TILE_MAPPING_FLAGS = UINT;
pub const TILE_MAPPING_FLAG_NONE = 0;
pub const TILE_MAPPING_FLAG_NO_HAZARD = 0x1;

pub const TILE_COPY_FLAGS = UINT;
pub const TILE_COPY_FLAG_NONE = 0;
pub const TILE_COPY_FLAG_NO_HAZARD = 0x1;
pub const TILE_COPY_FLAG_LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE = 0x2;
pub const TILE_COPY_FLAG_SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER = 0x4;

pub const VIEWPORT = extern struct {
    TopLeftX: FLOAT,
    TopLeftY: FLOAT,
    Width: FLOAT,
    Height: FLOAT,
    MinDepth: FLOAT,
    MaxDepth: FLOAT,
};

pub const RECT = windows.RECT;

pub const RESOURCE_STATES = UINT;
pub const RESOURCE_STATE_COMMON = 0;
pub const RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER = 0x1;
pub const RESOURCE_STATE_INDEX_BUFFER = 0x2;
pub const RESOURCE_STATE_RENDER_TARGET = 0x4;
pub const RESOURCE_STATE_UNORDERED_ACCESS = 0x8;
pub const RESOURCE_STATE_DEPTH_WRITE = 0x10;
pub const RESOURCE_STATE_DEPTH_READ = 0x20;
pub const RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE = 0x40;
pub const RESOURCE_STATE_PIXEL_SHADER_RESOURCE = 0x80;
pub const RESOURCE_STATE_STREAM_OUT = 0x100;
pub const RESOURCE_STATE_INDIRECT_ARGUMENT = 0x200;
pub const RESOURCE_STATE_COPY_DEST = 0x400;
pub const RESOURCE_STATE_COPY_SOURCE = 0x800;
pub const RESOURCE_STATE_RESOLVE_DEST = 0x1000;
pub const RESOURCE_STATE_RESOLVE_SOURCE = 0x2000;
pub const RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE = 0x400000;
pub const RESOURCE_STATE_SHADING_RATE_SOURCE = 0x1000000;
pub const RESOURCE_STATE_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800);
pub const RESOURCE_STATE_ALL_SHADER_RESOURCE = (0x40 | 0x80);
pub const RESOURCE_STATE_PRESENT = 0;
pub const RESOURCE_STATE_PREDICATION = 0x200;
pub const RESOURCE_STATE_VIDEO_DECODE_READ = 0x10000;
pub const RESOURCE_STATE_VIDEO_DECODE_WRITE = 0x20000;
pub const RESOURCE_STATE_VIDEO_PROCESS_READ = 0x40000;
pub const RESOURCE_STATE_VIDEO_PROCESS_WRITE = 0x80000;
pub const RESOURCE_STATE_VIDEO_ENCODE_READ = 0x200000;
pub const RESOURCE_STATE_VIDEO_ENCODE_WRITE = 0x800000;

pub const INDEX_BUFFER_STRIP_CUT_VALUE = enum(UINT) {
    DISABLED = 0,
    _0xFFFF = 1,
    _0xFFFFFFFF = 2,
};

pub const VERTEX_BUFFER_VIEW = extern struct {
    BufferLocation: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
    StrideInBytes: UINT,
};

pub const INDEX_BUFFER_VIEW = extern struct {
    BufferLocation: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
    Format: dxgi.FORMAT,
};

pub const STREAM_OUTPUT_BUFFER_VIEW = extern struct {
    BufferLocation: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
    BufferFilledSizeLocation: GPU_VIRTUAL_ADDRESS,
};

pub const CLEAR_FLAGS = UINT;
pub const CLEAR_FLAG_DEPTH: CLEAR_FLAGS = 0x1;
pub const CLEAR_FLAG_STENCIL: CLEAR_FLAGS = 0x2;

pub const DISCARD_REGION = extern struct {
    NumRects: UINT,
    pRects: *const RECT,
    FirstSubresource: UINT,
    NumSubresources: UINT,
};

pub const QUERY_HEAP_TYPE = enum(UINT) {
    OCCLUSION = 0,
    TIMESTAMP = 1,
    PIPELINE_STATISTICS = 2,
    SO_STATISTICS = 3,
};

pub const QUERY_HEAP_DESC = extern struct {
    Type: QUERY_HEAP_TYPE,
    Count: UINT,
    NodeMask: UINT,
};

pub const QUERY_TYPE = enum(UINT) {
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

pub const PREDICATION_OP = enum(UINT) {
    EQUAL_ZERO = 0,
    NOT_EQUAL_ZERO = 1,
};

pub const INDIRECT_ARGUMENT_TYPE = enum(UINT) {
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

pub const INDIRECT_ARGUMENT_DESC = extern struct {
    Type: INDIRECT_ARGUMENT_TYPE,
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

pub const COMMAND_SIGNATURE_DESC = extern struct {
    ByteStride: UINT,
    NumArgumentDescs: UINT,
    pArgumentDescs: *const INDIRECT_ARGUMENT_DESC,
    NodeMask: UINT,
};

pub const PACKED_MIP_INFO = extern struct {
    NumStandardMips: UINT8,
    NumPackedMips: UINT8,
    NumTilesForPackedMips: UINT,
    StartTileIndexInOverallResource: UINT,
};

pub const COMMAND_QUEUE_FLAGS = UINT;
pub const COMMAND_QUEUE_FLAG_NONE = 0;
pub const COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT = 0x1;

pub const COMMAND_QUEUE_PRIORITY = enum(UINT) {
    NORMAL = 0,
    HIGH = 100,
    GLOBAL_REALTIME = 10000,
};

pub const COMMAND_QUEUE_DESC = extern struct {
    Type: COMMAND_LIST_TYPE,
    Priority: INT,
    Flags: COMMAND_QUEUE_FLAGS,
    NodeMask: UINT,
};

pub const SHADER_BYTECODE = extern struct {
    pShaderBytecode: ?*const anyopaque,
    BytecodeLength: UINT64,

    pub inline fn initZero() SHADER_BYTECODE {
        return std.mem.zeroes(@This());
    }
};

pub const SO_DECLARATION_ENTRY = extern struct {
    Stream: UINT,
    SemanticName: LPCSTR,
    SemanticIndex: UINT,
    StartComponent: UINT8,
    ComponentCount: UINT8,
    OutputSlot: UINT8,
};

pub const STREAM_OUTPUT_DESC = extern struct {
    pSODeclaration: ?[*]const SO_DECLARATION_ENTRY,
    NumEntries: UINT,
    pBufferStrides: ?[*]const UINT,
    NumStrides: UINT,
    RasterizedStream: UINT,

    pub inline fn initZero() STREAM_OUTPUT_DESC {
        return std.mem.zeroes(@This());
    }
};

pub const BLEND = enum(UINT) {
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

pub const BLEND_OP = enum(UINT) {
    ADD = 1,
    SUBTRACT = 2,
    REV_SUBTRACT = 3,
    MIN = 4,
    MAX = 5,
};

pub const COLOR_WRITE_ENABLE = UINT;
pub const COLOR_WRITE_ENABLE_RED = 0x1;
pub const COLOR_WRITE_ENABLE_GREEN = 0x2;
pub const COLOR_WRITE_ENABLE_BLUE = 0x4;
pub const COLOR_WRITE_ENABLE_ALPHA = 0x8;
pub const COLOR_WRITE_ENABLE_ALL =
    COLOR_WRITE_ENABLE_RED |
    COLOR_WRITE_ENABLE_GREEN |
    COLOR_WRITE_ENABLE_BLUE |
    COLOR_WRITE_ENABLE_ALPHA;

pub const LOGIC_OP = enum(UINT) {
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

pub const RENDER_TARGET_BLEND_DESC = extern struct {
    BlendEnable: BOOL,
    LogicOpEnable: BOOL,
    SrcBlend: BLEND,
    DestBlend: BLEND,
    BlendOp: BLEND_OP,
    SrcBlendAlpha: BLEND,
    DestBlendAlpha: BLEND,
    BlendOpAlpha: BLEND_OP,
    LogicOp: LOGIC_OP,
    RenderTargetWriteMask: UINT8,

    pub fn initDefault() RENDER_TARGET_BLEND_DESC {
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
            .RenderTargetWriteMask = 0x0,
        };
        return v;
    }
};

pub const BLEND_DESC = extern struct {
    AlphaToCoverageEnable: BOOL,
    IndependentBlendEnable: BOOL,
    RenderTarget: [8]RENDER_TARGET_BLEND_DESC,

    pub fn initDefault() BLEND_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .AlphaToCoverageEnable = FALSE,
            .IndependentBlendEnable = FALSE,
            .RenderTarget = [_]RENDER_TARGET_BLEND_DESC{RENDER_TARGET_BLEND_DESC.initDefault()} ** 8,
        };
        return v;
    }
};

pub const RASTERIZER_DESC = extern struct {
    FillMode: FILL_MODE,
    CullMode: CULL_MODE,
    FrontCounterClockwise: BOOL,
    DepthBias: INT,
    DepthBiasClamp: FLOAT,
    SlopeScaledDepthBias: FLOAT,
    DepthClipEnable: BOOL,
    MultisampleEnable: BOOL,
    AntialiasedLineEnable: BOOL,
    ForcedSampleCount: UINT,
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE,

    pub fn initDefault() RASTERIZER_DESC {
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

pub const FILL_MODE = enum(UINT) {
    WIREFRAME = 2,
    SOLID = 3,
};

pub const CULL_MODE = enum(UINT) {
    NONE = 1,
    FRONT = 2,
    BACK = 3,
};

pub const CONSERVATIVE_RASTERIZATION_MODE = enum(UINT) {
    OFF = 0,
    ON = 1,
};

pub const COMPARISON_FUNC = enum(UINT) {
    NEVER = 1,
    LESS = 2,
    EQUAL = 3,
    LESS_EQUAL = 4,
    GREATER = 5,
    NOT_EQUAL = 6,
    GREATER_EQUAL = 7,
    ALWAYS = 8,
};

pub const DEPTH_WRITE_MASK = enum(UINT) {
    ZERO = 0,
    ALL = 1,
};

pub const STENCIL_OP = enum(UINT) {
    KEEP = 1,
    ZERO = 2,
    REPLACE = 3,
    INCR_SAT = 4,
    DECR_SAT = 5,
    INVERT = 6,
    INCR = 7,
    DECR = 8,
};

pub const DEPTH_STENCILOP_DESC = extern struct {
    StencilFailOp: STENCIL_OP,
    StencilDepthFailOp: STENCIL_OP,
    StencilPassOp: STENCIL_OP,
    StencilFunc: COMPARISON_FUNC,

    pub fn initDefault() DEPTH_STENCILOP_DESC {
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

pub const DEPTH_STENCIL_DESC = extern struct {
    DepthEnable: BOOL,
    DepthWriteMask: DEPTH_WRITE_MASK,
    DepthFunc: COMPARISON_FUNC,
    StencilEnable: BOOL,
    StencilReadMask: UINT8,
    StencilWriteMask: UINT8,
    FrontFace: DEPTH_STENCILOP_DESC,
    BackFace: DEPTH_STENCILOP_DESC,

    pub fn initDefault() DEPTH_STENCIL_DESC {
        var desc = std.mem.zeroes(@This());
        desc = .{
            .DepthEnable = TRUE,
            .DepthWriteMask = .ALL,
            .DepthFunc = .LESS,
            .StencilEnable = FALSE,
            .StencilReadMask = 0xff,
            .StencilWriteMask = 0xff,
            .FrontFace = DEPTH_STENCILOP_DESC.initDefault(),
            .BackFace = DEPTH_STENCILOP_DESC.initDefault(),
        };
        return desc;
    }
};

pub const DEPTH_STENCIL_DESC1 = extern struct {
    DepthEnable: BOOL,
    DepthWriteMask: DEPTH_WRITE_MASK,
    DepthFunc: COMPARISON_FUNC,
    StencilEnable: BOOL,
    StencilReadMask: UINT8,
    StencilWriteMask: UINT8,
    FrontFace: DEPTH_STENCILOP_DESC,
    BackFace: DEPTH_STENCILOP_DESC,
    DepthBoundsTestEnable: BOOL,

    pub fn initDefault() DEPTH_STENCIL_DESC1 {
        var desc = std.mem.zeroes(@This());
        desc = .{
            .DepthEnable = TRUE,
            .DepthWriteMask = .ALL,
            .DepthFunc = .LESS,
            .StencilEnable = FALSE,
            .StencilReadMask = 0xff,
            .StencilWriteMask = 0xff,
            .FrontFace = DEPTH_STENCILOP_DESC.initDefault(),
            .BackFace = DEPTH_STENCILOP_DESC.initDefault(),
            .DepthBoundsTestEnable = FALSE,
        };
        return desc;
    }
};

pub const INPUT_LAYOUT_DESC = extern struct {
    pInputElementDescs: ?[*]const INPUT_ELEMENT_DESC,
    NumElements: UINT,

    pub inline fn initZero() INPUT_LAYOUT_DESC {
        return std.mem.zeroes(@This());
    }
};

pub const INPUT_CLASSIFICATION = enum(UINT) {
    PER_VERTEX_DATA = 0,
    PER_INSTANCE_DATA = 1,
};

pub const INPUT_ELEMENT_DESC = extern struct {
    SemanticName: LPCSTR,
    SemanticIndex: UINT,
    Format: dxgi.FORMAT,
    InputSlot: UINT,
    AlignedByteOffset: UINT,
    InputSlotClass: INPUT_CLASSIFICATION,
    InstanceDataStepRate: UINT,

    pub inline fn init(
        semanticName: LPCSTR,
        semanticIndex: UINT,
        format: dxgi.FORMAT,
        inputSlot: UINT,
        alignedByteOffset: UINT,
        inputSlotClass: INPUT_CLASSIFICATION,
        instanceDataStepRate: UINT,
    ) INPUT_ELEMENT_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .SemanticName = semanticName,
            .SemanticIndex = semanticIndex,
            .Format = format,
            .InputSlot = inputSlot,
            .AlignedByteOffset = alignedByteOffset,
            .InputSlotClass = inputSlotClass,
            .InstanceDataStepRate = instanceDataStepRate,
        };
        return v;
    }
};

pub const CACHED_PIPELINE_STATE = extern struct {
    pCachedBlob: ?*const anyopaque,
    CachedBlobSizeInBytes: UINT64,

    pub inline fn initZero() CACHED_PIPELINE_STATE {
        return std.mem.zeroes(@This());
    }
};

pub const PIPELINE_STATE_FLAGS = UINT;
pub const PIPELINE_STATE_FLAG_NONE = 0;
pub const PIPELINE_STATE_FLAG_TOOL_DEBUG = 0x1;

pub const GRAPHICS_PIPELINE_STATE_DESC = extern struct {
    pRootSignature: ?*IRootSignature,
    VS: SHADER_BYTECODE,
    PS: SHADER_BYTECODE,
    DS: SHADER_BYTECODE,
    HS: SHADER_BYTECODE,
    GS: SHADER_BYTECODE,
    StreamOutput: STREAM_OUTPUT_DESC,
    BlendState: BLEND_DESC,
    SampleMask: UINT,
    RasterizerState: RASTERIZER_DESC,
    DepthStencilState: DEPTH_STENCIL_DESC,
    InputLayout: INPUT_LAYOUT_DESC,
    IBStripCutValue: INDEX_BUFFER_STRIP_CUT_VALUE,
    PrimitiveTopologyType: PRIMITIVE_TOPOLOGY_TYPE,
    NumRenderTargets: UINT,
    RTVFormats: [8]dxgi.FORMAT,
    DSVFormat: dxgi.FORMAT,
    SampleDesc: dxgi.SAMPLE_DESC,
    NodeMask: UINT,
    CachedPSO: CACHED_PIPELINE_STATE,
    Flags: PIPELINE_STATE_FLAGS,

    pub fn initDefault() GRAPHICS_PIPELINE_STATE_DESC {
        var v = std.mem.zeroes(@This());
        v = GRAPHICS_PIPELINE_STATE_DESC{
            .pRootSignature = null,
            .VS = SHADER_BYTECODE.initZero(),
            .PS = SHADER_BYTECODE.initZero(),
            .DS = SHADER_BYTECODE.initZero(),
            .HS = SHADER_BYTECODE.initZero(),
            .GS = SHADER_BYTECODE.initZero(),
            .StreamOutput = STREAM_OUTPUT_DESC.initZero(),
            .BlendState = BLEND_DESC.initDefault(),
            .SampleMask = 0xffff_ffff,
            .RasterizerState = RASTERIZER_DESC.initDefault(),
            .DepthStencilState = DEPTH_STENCIL_DESC.initDefault(),
            .InputLayout = INPUT_LAYOUT_DESC.initZero(),
            .IBStripCutValue = .DISABLED,
            .PrimitiveTopologyType = .UNDEFINED,
            .NumRenderTargets = 0,
            .RTVFormats = [_]dxgi.FORMAT{.UNKNOWN} ** 8,
            .DSVFormat = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .NodeMask = 0,
            .CachedPSO = CACHED_PIPELINE_STATE.initZero(),
            .Flags = PIPELINE_STATE_FLAG_NONE,
        };
        return v;
    }
};

pub const COMPUTE_PIPELINE_STATE_DESC = extern struct {
    pRootSignature: ?*IRootSignature,
    CS: SHADER_BYTECODE,
    NodeMask: UINT,
    CachedPSO: CACHED_PIPELINE_STATE,
    Flags: PIPELINE_STATE_FLAGS,

    pub fn initDefault() COMPUTE_PIPELINE_STATE_DESC {
        var v = std.mem.zeroes(@This());
        v = COMPUTE_PIPELINE_STATE_DESC{
            .pRootSignature = null,
            .CS = SHADER_BYTECODE.initZero(),
            .NodeMask = 0,
            .CachedPSO = CACHED_PIPELINE_STATE.initZero(),
            .Flags = PIPELINE_STATE_FLAG_NONE,
        };
        return v;
    }
};

pub const FEATURE = enum(UINT) {
    OPTIONS = 0,
    ARCHITECTURE = 1,
    FEATURE_LEVELS = 2,
    FORMAT_SUPPORT = 3,
    MULTISAMPLE_QUALITY_LEVELS = 4,
    FORMAT_INFO = 5,
    GPU_VIRTUAL_ADDRESS_SUPPORT = 6,
    SHADER_MODEL = 7,
    OPTIONS1 = 8,
    PROTECTED_RESOURCE_SESSION_SUPPORT = 10,
    ROOT_SIGNATURE = 12,
    ARCHITECTURE1 = 16,
    OPTIONS2 = 18,
    SHADER_CACHE = 19,
    COMMAND_QUEUE_PRIORITY = 20,
    OPTIONS3 = 21,
    EXISTING_HEAPS = 22,
    OPTIONS4 = 23,
    SERIALIZATION = 24,
    CROSS_NODE = 25,
    OPTIONS5 = 27,
    DISPLAYABLE = 28,
    OPTIONS6 = 30,
    QUERY_META_COMMAND = 31,
    OPTIONS7 = 32,
    PROTECTED_RESOURCE_SESSION_TYPE_COUNT = 33,
    PROTECTED_RESOURCE_SESSION_TYPES = 34,
    OPTIONS8 = 36,
    OPTIONS9 = 37,
    OPTIONS10 = 39,
    OPTIONS11 = 40,
};

pub const SHADER_MODEL = enum(UINT) {
    SM_5_1 = 0x51,
    SM_6_0 = 0x60,
    SM_6_1 = 0x61,
    SM_6_2 = 0x62,
    SM_6_3 = 0x63,
    SM_6_4 = 0x64,
    SM_6_5 = 0x65,
    SM_6_6 = 0x66,
    SM_6_7 = 0x67,
};

pub const RESOURCE_BINDING_TIER = enum(UINT) {
    TIER_1 = 1,
    TIER_2 = 2,
    TIER_3 = 3,
};

pub const RESOURCE_HEAP_TIER = enum(UINT) {
    TIER_1 = 1,
    TIER_2 = 2,
};

pub const SHADER_MIN_PRECISION_SUPPORT = UINT;
pub const SHADER_MIN_PRECISION_SUPPORT_NONE: SHADER_MIN_PRECISION_SUPPORT = 0;
pub const SHADER_MIN_PRECISION_SUPPORT_10_BIT: SHADER_MIN_PRECISION_SUPPORT = 0x1;
pub const SHADER_MIN_PRECISION_SUPPORT_16_BIT: SHADER_MIN_PRECISION_SUPPORT = 0x2;

pub const TILED_RESOURCES_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1 = 1,
    TIER_2 = 2,
    TIER_3 = 3,
    TIER_4 = 4,
};

pub const CONSERVATIVE_RASTERIZATION_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1 = 1,
    TIER_2 = 2,
    TIER_3 = 3,
};

pub const CROSS_NODE_SHARING_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1_EMULATED = 1,
    TIER_1 = 2,
    TIER_2 = 3,
    TIER_3 = 4,
};

pub const FEATURE_DATA_D3D12_OPTIONS = extern struct {
    DoublePrecisionFloatShaderOps: BOOL,
    OutputMergerLogicOp: BOOL,
    MinPrecisionSupport: SHADER_MIN_PRECISION_SUPPORT,
    TiledResourcesTier: TILED_RESOURCES_TIER,
    ResourceBindingTier: RESOURCE_BINDING_TIER,
    PSSpecifiedStencilRefSupported: BOOL,
    TypedUAVLoadAdditionalFormats: BOOL,
    ROVsSupported: BOOL,
    ConservativeRasterizationTier: CONSERVATIVE_RASTERIZATION_TIER,
    MaxGPUVirtualAddressBitsPerResource: UINT,
    StandardSwizzle64KBSupported: BOOL,
    CrossNodeSharingTier: CROSS_NODE_SHARING_TIER,
    CrossAdapterRowMajorTextureSupported: BOOL,
    VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation: BOOL,
    ResourceHeapTier: RESOURCE_HEAP_TIER,
};

pub const FEATURE_DATA_SHADER_MODEL = extern struct {
    HighestShaderModel: SHADER_MODEL,
};

pub const RENDER_PASS_TIER = enum(UINT) {
    TIER_0 = 0,
    TIER_1 = 1,
    TIER_2 = 2,
};

pub const RAYTRACING_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1_0 = 10,
    TIER_1_1 = 11,
};

pub const MESH_SHADER_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1 = 10,
};

pub const SAMPLER_FEEDBACK_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_0_9 = 90,
    TIER_1_0 = 100,
};

pub const FEATURE_DATA_D3D12_OPTIONS7 = extern struct {
    MeshShaderTier: MESH_SHADER_TIER,
    SamplerFeedbackTier: SAMPLER_FEEDBACK_TIER,
};

pub const COMMAND_LIST_SUPPORT_FLAGS = UINT;
pub const COMMAND_LIST_SUPPORT_FLAG_NONE: COMMAND_LIST_SUPPORT_FLAGS = 0x0;
pub const COMMAND_LIST_SUPPORT_FLAG_DIRECT: COMMAND_LIST_SUPPORT_FLAGS = 0x1;
pub const COMMAND_LIST_SUPPORT_FLAG_BUNDLE: COMMAND_LIST_SUPPORT_FLAGS = 0x2;
pub const COMMAND_LIST_SUPPORT_FLAG_COMPUTE: COMMAND_LIST_SUPPORT_FLAGS = 0x4;
pub const COMMAND_LIST_SUPPORT_FLAG_COPY: COMMAND_LIST_SUPPORT_FLAGS = 0x8;
pub const COMMAND_LIST_SUPPORT_FLAG_VIDEO_DECODE: COMMAND_LIST_SUPPORT_FLAGS = 0x10;
pub const COMMAND_LIST_SUPPORT_FLAG_VIDEO_PROCESS: COMMAND_LIST_SUPPORT_FLAGS = 0x20;
pub const COMMAND_LIST_SUPPORT_FLAG_VIDEO_ENCODE: COMMAND_LIST_SUPPORT_FLAGS = 0x40;

pub const VIEW_INSTANCING_TIER = enum(UINT) {
    NOT_SUPPORTED = 0,
    TIER_1 = 1,
    TIER_2 = 2,
    TIER_3 = 3,
};

pub const FEATURE_DATA_D3D12_OPTIONS3 = extern struct {
    CopyQueueTimestampQueriesSupported: BOOL,
    CastingFullyTypedFormatSupported: BOOL,
    WriteBufferImmediateSupportFlags: COMMAND_LIST_SUPPORT_FLAGS,
    ViewInstancingTier: VIEW_INSTANCING_TIER,
    BarycentricsSupported: BOOL,
};

pub const FEATURE_DATA_D3D12_OPTIONS5 = extern struct {
    SRVOnlyTiledResourceTier3: BOOL,
    RenderPassesTier: RENDER_PASS_TIER,
    RaytracingTier: RAYTRACING_TIER,
};

pub const CONSTANT_BUFFER_VIEW_DESC = extern struct {
    BufferLocation: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT,
};

pub inline fn encodeShader4ComponentMapping(src0: UINT, src1: UINT, src2: UINT, src3: UINT) UINT {
    return (src0 & 0x7) | ((src1 & 0x7) << 3) | ((src2 & 0x7) << (3 * 2)) | ((src3 & 0x7) << (3 * 3)) | (1 << (3 * 4));
}
pub const DEFAULT_SHADER_4_COMPONENT_MAPPING = encodeShader4ComponentMapping(0, 1, 2, 3);

pub const BUFFER_SRV_FLAGS = UINT;
pub const BUFFER_SRV_FLAG_NONE = 0;
pub const BUFFER_SRV_FLAG_RAW = 0x1;

pub const BUFFER_SRV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
    StructureByteStride: UINT,
    Flags: BUFFER_SRV_FLAGS,
};

pub const TEX1D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEX1D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEX2D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    PlaneSlice: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEX2D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEX3D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEXCUBE_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEXCUBE_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    First2DArrayFace: UINT,
    NumCubes: UINT,
    ResourceMinLODClamp: FLOAT,
};

pub const TEX2DMS_SRV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const TEX2DMS_ARRAY_SRV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const SRV_DIMENSION = enum(UINT) {
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

pub const SHADER_RESOURCE_VIEW_DESC = extern struct {
    Format: dxgi.FORMAT,
    ViewDimension: SRV_DIMENSION,
    Shader4ComponentMapping: UINT,
    u: extern union {
        Buffer: BUFFER_SRV,
        Texture1D: TEX1D_SRV,
        Texture1DArray: TEX1D_ARRAY_SRV,
        Texture2D: TEX2D_SRV,
        Texture2DArray: TEX2D_ARRAY_SRV,
        Texture2DMS: TEX2DMS_SRV,
        Texture2DMSArray: TEX2DMS_ARRAY_SRV,
        Texture3D: TEX3D_SRV,
        TextureCube: TEXCUBE_SRV,
        TextureCubeArray: TEXCUBE_ARRAY_SRV,
    },

    pub fn initTypedBuffer(
        format: dxgi.FORMAT,
        first_element: UINT64,
        num_elements: UINT,
    ) SHADER_RESOURCE_VIEW_DESC {
        var desc = std.mem.zeroes(@This());
        desc = .{
            .Format = format,
            .ViewDimension = .BUFFER,
            .Shader4ComponentMapping = DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = 0,
                    .Flags = BUFFER_SRV_FLAG_NONE,
                },
            },
        };
        return desc;
    }

    pub fn initStructuredBuffer(
        first_element: UINT64,
        num_elements: UINT,
        stride: UINT,
    ) SHADER_RESOURCE_VIEW_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = .UNKNOWN,
            .ViewDimension = .BUFFER,
            .Shader4ComponentMapping = DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = stride,
                    .Flags = BUFFER_SRV_FLAG_NONE,
                },
            },
        };
        return v;
    }
};

pub const FILTER = enum(UINT) {
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

pub const FILTER_TYPE = enum(UINT) {
    POINT = 0,
    LINEAR = 1,
};

pub const FILTER_REDUCTION_TYPE = enum(UINT) {
    STANDARD = 0,
    COMPARISON = 1,
    MINIMUM = 2,
    MAXIMUM = 3,
};

pub const TEXTURE_ADDRESS_MODE = enum(UINT) {
    WRAP = 1,
    MIRROR = 2,
    CLAMP = 3,
    BORDER = 4,
    MIRROR_ONCE = 5,
};

pub const SAMPLER_DESC = extern struct {
    Filter: FILTER,
    AddressU: TEXTURE_ADDRESS_MODE,
    AddressV: TEXTURE_ADDRESS_MODE,
    AddressW: TEXTURE_ADDRESS_MODE,
    MipLODBias: FLOAT,
    MaxAnisotropy: UINT,
    ComparisonFunc: COMPARISON_FUNC,
    BorderColor: [4]FLOAT,
    MinLOD: FLOAT,
    MaxLOD: FLOAT,
};

pub const BUFFER_UAV_FLAGS = UINT;
pub const BUFFER_UAV_FLAG_NONE = 0;
pub const BUFFER_UAV_FLAG_RAW = 0x1;

pub const BUFFER_UAV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
    StructureByteStride: UINT,
    CounterOffsetInBytes: UINT64,
    Flags: BUFFER_UAV_FLAGS,
};

pub const TEX1D_UAV = extern struct {
    MipSlice: UINT,
};

pub const TEX1D_ARRAY_UAV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2D_UAV = extern struct {
    MipSlice: UINT,
    PlaneSlice: UINT,
};

pub const TEX2D_ARRAY_UAV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
};

pub const TEX3D_UAV = extern struct {
    MipSlice: UINT,
    FirstWSlice: UINT,
    WSize: UINT,
};

pub const UAV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE1DARRAY = 3,
    TEXTURE2D = 4,
    TEXTURE2DARRAY = 5,
    TEXTURE3D = 8,
};

pub const UNORDERED_ACCESS_VIEW_DESC = extern struct {
    Format: dxgi.FORMAT,
    ViewDimension: UAV_DIMENSION,
    u: extern union {
        Buffer: BUFFER_UAV,
        Texture1D: TEX1D_UAV,
        Texture1DArray: TEX1D_ARRAY_UAV,
        Texture2D: TEX2D_UAV,
        Texture2DArray: TEX2D_ARRAY_UAV,
        Texture3D: TEX3D_UAV,
    },

    pub fn initTypedBuffer(
        format: dxgi.FORMAT,
        first_element: UINT64,
        num_elements: UINT,
        counter_offset: UINT64,
    ) UNORDERED_ACCESS_VIEW_DESC {
        var desc = std.mem.zeroes(@This());
        desc = .{
            .Format = format,
            .ViewDimension = .BUFFER,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = 0,
                    .CounterOffsetInBytes = counter_offset,
                    .Flags = BUFFER_SRV_FLAG_NONE,
                },
            },
        };
        return desc;
    }

    pub fn initStructuredBuffer(
        first_element: UINT64,
        num_elements: UINT,
        stride: UINT,
        counter_offset: UINT64,
    ) UNORDERED_ACCESS_VIEW_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = .UNKNOWN,
            .ViewDimension = .BUFFER,
            .u = .{
                .Buffer = .{
                    .FirstElement = first_element,
                    .NumElements = num_elements,
                    .StructureByteStride = stride,
                    .CounterOffsetInBytes = counter_offset,
                    .Flags = BUFFER_SRV_FLAG_NONE,
                },
            },
        };
        return v;
    }
};

pub const BUFFER_RTV = extern struct {
    FirstElement: UINT64,
    NumElements: UINT,
};

pub const TEX1D_RTV = extern struct {
    MipSlice: UINT,
};

pub const TEX1D_ARRAY_RTV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2D_RTV = extern struct {
    MipSlice: UINT,
    PlaneSlice: UINT,
};

pub const TEX2DMS_RTV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const TEX2D_ARRAY_RTV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
    PlaneSlice: UINT,
};

pub const TEX2DMS_ARRAY_RTV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX3D_RTV = extern struct {
    MipSlice: UINT,
    FirstWSlice: UINT,
    WSize: UINT,
};

pub const RTV_DIMENSION = enum(UINT) {
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

pub const RENDER_TARGET_VIEW_DESC = extern struct {
    Format: dxgi.FORMAT,
    ViewDimension: RTV_DIMENSION,
    u: extern union {
        Buffer: BUFFER_RTV,
        Texture1D: TEX1D_RTV,
        Texture1DArray: TEX1D_ARRAY_RTV,
        Texture2D: TEX2D_RTV,
        Texture2DArray: TEX2D_ARRAY_RTV,
        Texture2DMS: TEX2DMS_RTV,
        Texture2DMSArray: TEX2DMS_ARRAY_RTV,
        Texture3D: TEX3D_RTV,
    },
};

pub const TEX1D_DSV = extern struct {
    MipSlice: UINT,
};

pub const TEX1D_ARRAY_DSV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2D_DSV = extern struct {
    MipSlice: UINT,
};

pub const TEX2D_ARRAY_DSV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2DMS_DSV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const TEX2DMS_ARRAY_DSV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const DSV_FLAGS = UINT;
pub const DSV_FLAG_NONE = 0;
pub const DSV_FLAG_READ_ONLY_DEPTH = 0x1;
pub const DSV_FLAG_READ_ONLY_STENCIL = 0x2;

pub const DSV_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    TEXTURE1D = 1,
    TEXTURE1DARRAY = 2,
    TEXTURE2D = 3,
    TEXTURE2DARRAY = 4,
    TEXTURE2DMS = 5,
    TEXTURE2DMSARRAY = 6,
};

pub const DEPTH_STENCIL_VIEW_DESC = extern struct {
    Format: dxgi.FORMAT,
    ViewDimension: DSV_DIMENSION,
    Flags: DSV_FLAGS,
    u: extern union {
        Texture1D: TEX1D_DSV,
        Texture1DArray: TEX1D_ARRAY_DSV,
        Texture2D: TEX2D_DSV,
        Texture2DArray: TEX2D_ARRAY_DSV,
        Texture2DMS: TEX2DMS_DSV,
        Texture2DMSArray: TEX2DMS_ARRAY_DSV,
    },
};

pub const RESOURCE_ALLOCATION_INFO = extern struct {
    SizeInBytes: UINT64,
    Alignment: UINT64,
};

pub const DEPTH_STENCIL_VALUE = extern struct {
    Depth: FLOAT,
    Stencil: UINT8,
};

pub const CLEAR_VALUE = extern struct {
    Format: dxgi.FORMAT,
    u: extern union {
        Color: [4]FLOAT,
        DepthStencil: DEPTH_STENCIL_VALUE,
    },

    pub fn initColor(format: dxgi.FORMAT, in_color: *const [4]FLOAT) CLEAR_VALUE {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = format,
            .u = .{ .Color = in_color.* },
        };
        return v;
    }

    pub fn initDepthStencil(format: dxgi.FORMAT, depth: FLOAT, stencil: UINT8) CLEAR_VALUE {
        var v = std.mem.zeroes(@This());
        v = .{
            .Format = format,
            .u = .{ .DepthStencil = .{ .Depth = depth, .Stencil = stencil } },
        };
        return v;
    }
};

pub const IObject = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: ?*anyopaque) HRESULT {
                return self.v.object.GetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateData(self: *T, guid: *const GUID, data_size: UINT, data: ?*const anyopaque) HRESULT {
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
            GetPrivateData: fn (*T, *const GUID, *UINT, ?*anyopaque) callconv(WINAPI) HRESULT,
            SetPrivateData: fn (*T, *const GUID, UINT, ?*const anyopaque) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            SetName: fn (*T, LPCWSTR) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDeviceChild = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDevice(self: *T, guid: *const GUID, device: *?*anyopaque) HRESULT {
                return self.v.devchild.GetDevice(self, guid, device);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IRootSignature = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        rs: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IQueryHeap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        qheap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const ICommandSignature = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        cmdsig: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IPageable = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IHeap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        heap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) HEAP_DESC {
                var desc: HEAP_DESC = undefined;
                _ = self.v.heap.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *HEAP_DESC) callconv(WINAPI) *HEAP_DESC,
        };
    }
};

pub const IResource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Map(self: *T, subresource: UINT, read_range: ?*const RANGE, data: *?*anyopaque) HRESULT {
                return self.v.resource.Map(self, subresource, read_range, data);
            }
            pub inline fn Unmap(self: *T, subresource: UINT, written_range: ?*const RANGE) void {
                self.v.resource.Unmap(self, subresource, written_range);
            }
            pub inline fn GetDesc(self: *T) RESOURCE_DESC {
                var desc: RESOURCE_DESC = undefined;
                _ = self.v.resource.GetDesc(self, &desc);
                return desc;
            }
            pub inline fn GetGPUVirtualAddress(self: *T) GPU_VIRTUAL_ADDRESS {
                return self.v.resource.GetGPUVirtualAddress(self);
            }
            pub inline fn WriteToSubresource(
                self: *T,
                dst_subresource: UINT,
                dst_box: ?*const BOX,
                src_data: *const anyopaque,
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
                dst_data: *anyopaque,
                dst_row_pitch: UINT,
                dst_depth_pitch: UINT,
                src_subresource: UINT,
                src_box: ?*const BOX,
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
                properties: ?*HEAP_PROPERTIES,
                flags: ?*HEAP_FLAGS,
            ) HRESULT {
                return self.v.resource.GetHeapProperties(self, properties, flags);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Map: fn (*T, UINT, ?*const RANGE, *?*anyopaque) callconv(WINAPI) HRESULT,
            Unmap: fn (*T, UINT, ?*const RANGE) callconv(WINAPI) void,
            GetDesc: fn (*T, *RESOURCE_DESC) callconv(WINAPI) *RESOURCE_DESC,
            GetGPUVirtualAddress: fn (*T) callconv(WINAPI) GPU_VIRTUAL_ADDRESS,
            WriteToSubresource: fn (*T, UINT, ?*const BOX, *const anyopaque, UINT, UINT) callconv(WINAPI) HRESULT,
            ReadFromSubresource: fn (*T, *anyopaque, UINT, UINT, UINT, ?*const BOX) callconv(WINAPI) HRESULT,
            GetHeapProperties: fn (*T, ?*HEAP_PROPERTIES, ?*HEAP_FLAGS) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IResource1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        resource: IResource.VTable(Self),
        resource1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetProtectedResourceSession(self: *T, guid: *const GUID, session: *?*anyopaque) HRESULT {
                return self.v.resource1.GetProtectedResourceSession(self, guid, session);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetProtectedResourceSession: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ICommandAllocator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        alloc: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
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

pub const IFence = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        fence: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
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

pub const IFence1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        fence: IFence.VTable(Self),
        fence1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace IFence.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCreationFlags(self: *T) FENCE_FLAGS {
                return self.v.fence1.GetCreationFlags(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetCreationFlags: fn (*T) callconv(WINAPI) FENCE_FLAGS,
        };
    }
};

pub const IPipelineState = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        pstate: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCachedBlob(self: *T, blob: **d3d.IBlob) HRESULT {
                return self.v.pstate.GetCachedBlob(self, blob);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetCachedBlob: fn (*T, **d3d.IBlob) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IDescriptorHeap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        dheap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) DESCRIPTOR_HEAP_DESC {
                var desc: DESCRIPTOR_HEAP_DESC = undefined;
                _ = self.v.dheap.GetDesc(self, &desc);
                return desc;
            }
            pub inline fn GetCPUDescriptorHandleForHeapStart(self: *T) CPU_DESCRIPTOR_HANDLE {
                var handle: CPU_DESCRIPTOR_HANDLE = undefined;
                _ = self.v.dheap.GetCPUDescriptorHandleForHeapStart(self, &handle);
                return handle;
            }
            pub inline fn GetGPUDescriptorHandleForHeapStart(self: *T) GPU_DESCRIPTOR_HANDLE {
                var handle: GPU_DESCRIPTOR_HANDLE = undefined;
                _ = self.v.dheap.GetGPUDescriptorHandleForHeapStart(self, &handle);
                return handle;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (*T, *DESCRIPTOR_HEAP_DESC) callconv(WINAPI) *DESCRIPTOR_HEAP_DESC,
            GetCPUDescriptorHandleForHeapStart: fn (
                *T,
                *CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) *CPU_DESCRIPTOR_HANDLE,
            GetGPUDescriptorHandleForHeapStart: fn (
                *T,
                *GPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) *GPU_DESCRIPTOR_HANDLE,
        };
    }
};

pub const ICommandList = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetType(self: *T) COMMAND_LIST_TYPE {
                return self.v.cmdlist.GetType(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetType: fn (*T) callconv(WINAPI) COMMAND_LIST_TYPE,
        };
    }
};

pub const IGraphicsCommandList = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Close(self: *T) HRESULT {
                return self.v.grcmdlist.Close(self);
            }
            pub inline fn Reset(self: *T, alloc: *ICommandAllocator, initial_state: ?*IPipelineState) HRESULT {
                return self.v.grcmdlist.Reset(self, alloc, initial_state);
            }
            pub inline fn ClearState(self: *T, pso: ?*IPipelineState) void {
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
                dst_buffer: *IResource,
                dst_offset: UINT64,
                src_buffer: *IResource,
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
                dst: *const TEXTURE_COPY_LOCATION,
                dst_x: UINT,
                dst_y: UINT,
                dst_z: UINT,
                src: *const TEXTURE_COPY_LOCATION,
                src_box: ?*const BOX,
            ) void {
                self.v.grcmdlist.CopyTextureRegion(self, dst, dst_x, dst_y, dst_z, src, src_box);
            }
            pub inline fn CopyResource(self: *T, dst: *IResource, src: *IResource) void {
                self.v.grcmdlist.CopyResource(self, dst, src);
            }
            pub inline fn CopyTiles(
                self: *T,
                tiled_resource: *IResource,
                tile_region_start_coordinate: *const TILED_RESOURCE_COORDINATE,
                tile_region_size: *const TILE_REGION_SIZE,
                buffer: *IResource,
                buffer_start_offset_in_bytes: UINT64,
                flags: TILE_COPY_FLAGS,
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
                dst_resource: *IResource,
                dst_subresource: UINT,
                src_resource: *IResource,
                src_subresource: UINT,
                format: dxgi.FORMAT,
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
            pub inline fn IASetPrimitiveTopology(self: *T, topology: PRIMITIVE_TOPOLOGY) void {
                self.v.grcmdlist.IASetPrimitiveTopology(self, topology);
            }
            pub inline fn RSSetViewports(self: *T, num: UINT, viewports: [*]const VIEWPORT) void {
                self.v.grcmdlist.RSSetViewports(self, num, viewports);
            }
            pub inline fn RSSetScissorRects(self: *T, num: UINT, rects: [*]const RECT) void {
                self.v.grcmdlist.RSSetScissorRects(self, num, rects);
            }
            pub inline fn OMSetBlendFactor(self: *T, blend_factor: *const [4]FLOAT) void {
                self.v.grcmdlist.OMSetBlendFactor(self, blend_factor);
            }
            pub inline fn OMSetStencilRef(self: *T, stencil_ref: UINT) void {
                self.v.grcmdlist.OMSetStencilRef(self, stencil_ref);
            }
            pub inline fn SetPipelineState(self: *T, pso: *IPipelineState) void {
                self.v.grcmdlist.SetPipelineState(self, pso);
            }
            pub inline fn ResourceBarrier(self: *T, num: UINT, barriers: [*]const RESOURCE_BARRIER) void {
                self.v.grcmdlist.ResourceBarrier(self, num, barriers);
            }
            pub inline fn ExecuteBundle(self: *T, cmdlist: *IGraphicsCommandList) void {
                self.v.grcmdlist.ExecuteBundle(self, cmdlist);
            }
            pub inline fn SetDescriptorHeaps(self: *T, num: UINT, heaps: [*]const *IDescriptorHeap) void {
                self.v.grcmdlist.SetDescriptorHeaps(self, num, heaps);
            }
            pub inline fn SetComputeRootSignature(self: *T, root_signature: ?*IRootSignature) void {
                self.v.grcmdlist.SetComputeRootSignature(self, root_signature);
            }
            pub inline fn SetGraphicsRootSignature(self: *T, root_signature: ?*IRootSignature) void {
                self.v.grcmdlist.SetGraphicsRootSignature(self, root_signature);
            }
            pub inline fn SetComputeRootDescriptorTable(
                self: *T,
                root_index: UINT,
                base_descriptor: GPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.grcmdlist.SetComputeRootDescriptorTable(self, root_index, base_descriptor);
            }
            pub inline fn SetGraphicsRootDescriptorTable(
                self: *T,
                root_index: UINT,
                base_descriptor: GPU_DESCRIPTOR_HANDLE,
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
                data: *const anyopaque,
                offset: UINT,
            ) void {
                self.v.grcmdlist.SetComputeRoot32BitConstants(self, root_index, num, data, offset);
            }
            pub inline fn SetGraphicsRoot32BitConstants(
                self: *T,
                root_index: UINT,
                num: UINT,
                data: *const anyopaque,
                offset: UINT,
            ) void {
                self.v.grcmdlist.SetGraphicsRoot32BitConstants(self, root_index, num, data, offset);
            }
            pub inline fn SetComputeRootConstantBufferView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootConstantBufferView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootConstantBufferView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootConstantBufferView(self, index, buffer_location);
            }
            pub inline fn SetComputeRootShaderResourceView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootShaderResourceView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootShaderResourceView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootShaderResourceView(self, index, buffer_location);
            }
            pub inline fn SetComputeRootUnorderedAccessView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetComputeRootUnorderedAccessView(self, index, buffer_location);
            }
            pub inline fn SetGraphicsRootUnorderedAccessView(
                self: *T,
                index: UINT,
                buffer_location: GPU_VIRTUAL_ADDRESS,
            ) void {
                self.v.grcmdlist.SetGraphicsRootUnorderedAccessView(self, index, buffer_location);
            }
            pub inline fn IASetIndexBuffer(self: *T, view: ?*const INDEX_BUFFER_VIEW) void {
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
                views: ?[*]const STREAM_OUTPUT_BUFFER_VIEW,
            ) void {
                self.v.grcmdlist.SOSetTargets(self, start_slot, num_views, views);
            }
            pub inline fn OMSetRenderTargets(
                self: *T,
                num_rt_descriptors: UINT,
                rt_descriptors: ?[*]const CPU_DESCRIPTOR_HANDLE,
                single_handle: BOOL,
                ds_descriptors: ?*const CPU_DESCRIPTOR_HANDLE,
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
                ds_view: CPU_DESCRIPTOR_HANDLE,
                clear_flags: CLEAR_FLAGS,
                depth: FLOAT,
                stencil: UINT8,
                num_rects: UINT,
                rects: ?[*]const RECT,
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
                rt_view: CPU_DESCRIPTOR_HANDLE,
                rgba: *const [4]FLOAT,
                num_rects: UINT,
                rects: ?[*]const RECT,
            ) void {
                self.v.grcmdlist.ClearRenderTargetView(self, rt_view, rgba, num_rects, rects);
            }
            pub inline fn ClearUnorderedAccessViewUint(
                self: *T,
                gpu_view: GPU_DESCRIPTOR_HANDLE,
                cpu_view: CPU_DESCRIPTOR_HANDLE,
                resource: *IResource,
                values: *const [4]UINT,
                num_rects: UINT,
                rects: ?[*]const RECT,
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
                gpu_view: GPU_DESCRIPTOR_HANDLE,
                cpu_view: CPU_DESCRIPTOR_HANDLE,
                resource: *IResource,
                values: *const [4]FLOAT,
                num_rects: UINT,
                rects: ?[*]const RECT,
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
            pub inline fn DiscardResource(self: *T, resource: *IResource, region: ?*const DISCARD_REGION) void {
                self.v.grcmdlist.DiscardResource(self, resource, region);
            }
            pub inline fn BeginQuery(self: *T, query: *IQueryHeap, query_type: QUERY_TYPE, index: UINT) void {
                self.v.grcmdlist.BeginQuery(self, query, query_type, index);
            }
            pub inline fn EndQuery(self: *T, query: *IQueryHeap, query_type: QUERY_TYPE, index: UINT) void {
                self.v.grcmdlist.EndQuery(self, query, query_type, index);
            }
            pub inline fn ResolveQueryData(
                self: *T,
                query: *IQueryHeap,
                query_type: QUERY_TYPE,
                start_index: UINT,
                num_queries: UINT,
                dst_resource: *IResource,
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
                buffer: ?*IResource,
                buffer_offset: UINT64,
                operation: PREDICATION_OP,
            ) void {
                self.v.grcmdlist.SetPredication(self, buffer, buffer_offset, operation);
            }
            pub inline fn SetMarker(self: *T, metadata: UINT, data: ?*const anyopaque, size: UINT) void {
                self.v.grcmdlist.SetMarker(self, metadata, data, size);
            }
            pub inline fn BeginEvent(self: *T, metadata: UINT, data: ?*const anyopaque, size: UINT) void {
                self.v.grcmdlist.BeginEvent(self, metadata, data, size);
            }
            pub inline fn EndEvent(self: *T) void {
                self.v.grcmdlist.EndEvent(self);
            }
            pub inline fn ExecuteIndirect(
                self: *T,
                command_signature: *ICommandSignature,
                max_command_count: UINT,
                arg_buffer: *IResource,
                arg_buffer_offset: UINT64,
                count_buffer: ?*IResource,
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
            Reset: fn (*T, *ICommandAllocator, ?*IPipelineState) callconv(WINAPI) HRESULT,
            ClearState: fn (*T, ?*IPipelineState) callconv(WINAPI) void,
            DrawInstanced: fn (*T, UINT, UINT, UINT, UINT) callconv(WINAPI) void,
            DrawIndexedInstanced: fn (*T, UINT, UINT, UINT, INT, UINT) callconv(WINAPI) void,
            Dispatch: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            CopyBufferRegion: fn (*T, *IResource, UINT64, *IResource, UINT64, UINT64) callconv(WINAPI) void,
            CopyTextureRegion: fn (
                *T,
                *const TEXTURE_COPY_LOCATION,
                UINT,
                UINT,
                UINT,
                *const TEXTURE_COPY_LOCATION,
                ?*const BOX,
            ) callconv(WINAPI) void,
            CopyResource: fn (*T, *IResource, *IResource) callconv(WINAPI) void,
            CopyTiles: fn (
                *T,
                *IResource,
                *const TILED_RESOURCE_COORDINATE,
                *const TILE_REGION_SIZE,
                *IResource,
                buffer_start_offset_in_bytes: UINT64,
                TILE_COPY_FLAGS,
            ) callconv(WINAPI) void,
            ResolveSubresource: fn (*T, *IResource, UINT, *IResource, UINT, dxgi.FORMAT) callconv(WINAPI) void,
            IASetPrimitiveTopology: fn (*T, PRIMITIVE_TOPOLOGY) callconv(WINAPI) void,
            RSSetViewports: fn (*T, UINT, [*]const VIEWPORT) callconv(WINAPI) void,
            RSSetScissorRects: fn (*T, UINT, [*]const RECT) callconv(WINAPI) void,
            OMSetBlendFactor: fn (*T, *const [4]FLOAT) callconv(WINAPI) void,
            OMSetStencilRef: fn (*T, UINT) callconv(WINAPI) void,
            SetPipelineState: fn (*T, *IPipelineState) callconv(WINAPI) void,
            ResourceBarrier: fn (*T, UINT, [*]const RESOURCE_BARRIER) callconv(WINAPI) void,
            ExecuteBundle: fn (*T, *IGraphicsCommandList) callconv(WINAPI) void,
            SetDescriptorHeaps: fn (*T, UINT, [*]const *IDescriptorHeap) callconv(WINAPI) void,
            SetComputeRootSignature: fn (*T, ?*IRootSignature) callconv(WINAPI) void,
            SetGraphicsRootSignature: fn (*T, ?*IRootSignature) callconv(WINAPI) void,
            SetComputeRootDescriptorTable: fn (*T, UINT, GPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            SetGraphicsRootDescriptorTable: fn (*T, UINT, GPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            SetComputeRoot32BitConstant: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            SetGraphicsRoot32BitConstant: fn (*T, UINT, UINT, UINT) callconv(WINAPI) void,
            SetComputeRoot32BitConstants: fn (*T, UINT, UINT, *const anyopaque, UINT) callconv(WINAPI) void,
            SetGraphicsRoot32BitConstants: fn (*T, UINT, UINT, *const anyopaque, UINT) callconv(WINAPI) void,
            SetComputeRootConstantBufferView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootConstantBufferView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetComputeRootShaderResourceView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootShaderResourceView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetComputeRootUnorderedAccessView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            SetGraphicsRootUnorderedAccessView: fn (*T, UINT, GPU_VIRTUAL_ADDRESS) callconv(WINAPI) void,
            IASetIndexBuffer: fn (*T, ?*const INDEX_BUFFER_VIEW) callconv(WINAPI) void,
            IASetVertexBuffers: fn (*T, UINT, UINT, ?[*]const VERTEX_BUFFER_VIEW) callconv(WINAPI) void,
            SOSetTargets: fn (*T, UINT, UINT, ?[*]const STREAM_OUTPUT_BUFFER_VIEW) callconv(WINAPI) void,
            OMSetRenderTargets: fn (
                *T,
                UINT,
                ?[*]const CPU_DESCRIPTOR_HANDLE,
                BOOL,
                ?*const CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            ClearDepthStencilView: fn (
                *T,
                CPU_DESCRIPTOR_HANDLE,
                CLEAR_FLAGS,
                FLOAT,
                UINT8,
                UINT,
                ?[*]const RECT,
            ) callconv(WINAPI) void,
            ClearRenderTargetView: fn (
                *T,
                CPU_DESCRIPTOR_HANDLE,
                *const [4]FLOAT,
                UINT,
                ?[*]const RECT,
            ) callconv(WINAPI) void,
            ClearUnorderedAccessViewUint: fn (
                *T,
                GPU_DESCRIPTOR_HANDLE,
                CPU_DESCRIPTOR_HANDLE,
                *IResource,
                *const [4]UINT,
                UINT,
                ?[*]const RECT,
            ) callconv(WINAPI) void,
            ClearUnorderedAccessViewFloat: fn (
                *T,
                GPU_DESCRIPTOR_HANDLE,
                CPU_DESCRIPTOR_HANDLE,
                *IResource,
                *const [4]FLOAT,
                UINT,
                ?[*]const RECT,
            ) callconv(WINAPI) void,
            DiscardResource: fn (*T, *IResource, ?*const DISCARD_REGION) callconv(WINAPI) void,
            BeginQuery: fn (*T, *IQueryHeap, QUERY_TYPE, UINT) callconv(WINAPI) void,
            EndQuery: fn (*T, *IQueryHeap, QUERY_TYPE, UINT) callconv(WINAPI) void,
            ResolveQueryData: fn (
                *T,
                *IQueryHeap,
                QUERY_TYPE,
                UINT,
                UINT,
                *IResource,
                UINT64,
            ) callconv(WINAPI) void,
            SetPredication: fn (*T, ?*IResource, UINT64, PREDICATION_OP) callconv(WINAPI) void,
            SetMarker: fn (*T, UINT, ?*const anyopaque, UINT) callconv(WINAPI) void,
            BeginEvent: fn (*T, UINT, ?*const anyopaque, UINT) callconv(WINAPI) void,
            EndEvent: fn (*T) callconv(WINAPI) void,
            ExecuteIndirect: fn (
                *T,
                *ICommandSignature,
                UINT,
                *IResource,
                UINT64,
                ?*IResource,
                UINT64,
            ) callconv(WINAPI) void,
        };
    }
};

pub const RANGE_UINT64 = extern struct {
    Begin: UINT64,
    End: UINT64,
};

pub const SUBRESOURCE_RANGE_UINT64 = extern struct {
    Subresource: UINT,
    Range: RANGE_UINT64,
};

pub const SAMPLE_POSITION = extern struct {
    X: INT8,
    Y: INT8,
};

pub const RESOLVE_MODE = enum(UINT) {
    DECOMPRESS = 0,
    MIN = 1,
    MAX = 2,
    AVERAGE = 3,
    ENCODE_SAMPLER_FEEDBACK = 4,
    DECODE_SAMPLER_FEEDBACK = 5,
};

pub const IGraphicsCommandList1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AtomicCopyBufferUINT(
                self: *T,
                dst_buffer: *IResource,
                dst_offset: UINT64,
                src_buffer: *IResource,
                src_offset: UINT64,
                dependencies: UINT,
                dependent_resources: [*]const *IResource,
                dependent_subresource_ranges: [*]const SUBRESOURCE_RANGE_UINT64,
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
                dst_buffer: *IResource,
                dst_offset: UINT64,
                src_buffer: *IResource,
                src_offset: UINT64,
                dependencies: UINT,
                dependent_resources: [*]const *IResource,
                dependent_subresource_ranges: [*]const SUBRESOURCE_RANGE_UINT64,
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
                sample_positions: *SAMPLE_POSITION,
            ) void {
                self.v.grcmdlist1.SetSamplePositions(self, num_samples, num_pixels, sample_positions);
            }
            pub inline fn ResolveSubresourceRegion(
                self: *T,
                dst_resource: *IResource,
                dst_subresource: UINT,
                dst_x: UINT,
                dst_y: UINT,
                src_resource: *IResource,
                src_subresource: UINT,
                src_rect: *RECT,
                format: dxgi.FORMAT,
                resolve_mode: RESOLVE_MODE,
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
                *IResource,
                UINT64,
                *IResource,
                UINT64,
                UINT,
                [*]const *IResource,
                [*]const SUBRESOURCE_RANGE_UINT64,
            ) callconv(WINAPI) void,
            AtomicCopyBufferUINT64: fn (
                *T,
                *IResource,
                UINT64,
                *IResource,
                UINT64,
                UINT,
                [*]const *IResource,
                [*]const SUBRESOURCE_RANGE_UINT64,
            ) callconv(WINAPI) void,
            OMSetDepthBounds: fn (*T, FLOAT, FLOAT) callconv(WINAPI) void,
            SetSamplePositions: fn (*T, UINT, UINT, *SAMPLE_POSITION) callconv(WINAPI) void,
            ResolveSubresourceRegion: fn (
                *T,
                *IResource,
                UINT,
                UINT,
                UINT,
                *IResource,
                UINT,
                *RECT,
                dxgi.FORMAT,
                RESOLVE_MODE,
            ) callconv(WINAPI) void,
            SetViewInstanceMask: fn (*T, UINT) callconv(WINAPI) void,
        };
    }
};

pub const WRITEBUFFERIMMEDIATE_PARAMETER = extern struct {
    Dest: GPU_VIRTUAL_ADDRESS,
    Value: UINT32,
};

pub const WRITEBUFFERIMMEDIATE_MODE = enum(UINT) {
    DEFAULT = 0,
    MARKER_IN = 0x1,
    MARKER_OUT = 0x2,
};

pub const IGraphicsCommandList2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: IGraphicsCommandList1.VTable(Self),
        grcmdlist2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace IGraphicsCommandList1.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn WriteBufferImmediate(
                self: *T,
                count: UINT,
                params: [*]const WRITEBUFFERIMMEDIATE_PARAMETER,
                modes: ?[*]const WRITEBUFFERIMMEDIATE_MODE,
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
                [*]const WRITEBUFFERIMMEDIATE_PARAMETER,
                ?[*]const WRITEBUFFERIMMEDIATE_MODE,
            ) callconv(WINAPI) void,
        };
    }
};

pub const IGraphicsCommandList3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: IGraphicsCommandList1.VTable(Self),
        grcmdlist2: IGraphicsCommandList2.VTable(Self),
        grcmdlist3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace IGraphicsCommandList1.Methods(Self);
    usingnamespace IGraphicsCommandList2.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetProtectedResourceSession(self: *T, prsession: ?*IProtectedResourceSession) void {
                self.v.grcmdlist3.SetProtectedResourceSession(self, prsession);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetProtectedResourceSession: fn (*T, ?*IProtectedResourceSession) callconv(WINAPI) void,
        };
    }
};

pub const RENDER_PASS_BEGINNING_ACCESS_TYPE = enum(UINT) {
    DISCARD = 0,
    PRESERVE = 1,
    CLEAR = 2,
    NO_ACCESS = 3,
};

pub const RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS = extern struct {
    ClearValue: CLEAR_VALUE,
};

pub const RENDER_PASS_BEGINNING_ACCESS = extern struct {
    Type: RENDER_PASS_BEGINNING_ACCESS_TYPE,
    u: extern union {
        Clear: RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS,
    },
};

pub const RENDER_PASS_ENDING_ACCESS_TYPE = enum(UINT) {
    DISCARD = 0,
    PRESERVE = 1,
    RESOLVE = 2,
    NO_ACCESS = 3,
};

pub const RENDER_PASS_ENDING_ACCESS_RESOLVE_SUBRESOURCE_PARAMETERS = extern struct {
    SrcSubresource: UINT,
    DstSubresource: UINT,
    DstX: UINT,
    DstY: UINT,
    SrcRect: RECT,
};

pub const RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS = extern struct {
    pSrcResource: *IResource,
    pDstResource: *IResource,
    SubresourceCount: UINT,
    pSubresourceParameters: [*]const RENDER_PASS_ENDING_ACCESS_RESOLVE_SUBRESOURCE_PARAMETERS,
    Format: dxgi.FORMAT,
    ResolveMode: RESOLVE_MODE,
    PreserveResolveSource: BOOL,
};

pub const RENDER_PASS_ENDING_ACCESS = extern struct {
    Type: RENDER_PASS_ENDING_ACCESS_TYPE,
    u: extern union {
        Resolve: RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS,
    },
};

pub const RENDER_PASS_RENDER_TARGET_DESC = extern struct {
    cpuDescriptor: CPU_DESCRIPTOR_HANDLE,
    BeginningAccess: RENDER_PASS_BEGINNING_ACCESS,
    EndingAccess: RENDER_PASS_ENDING_ACCESS,
};

pub const RENDER_PASS_DEPTH_STENCIL_DESC = extern struct {
    cpuDescriptor: CPU_DESCRIPTOR_HANDLE,
    DepthBeginningAccess: RENDER_PASS_BEGINNING_ACCESS,
    StencilBeginningAccess: RENDER_PASS_BEGINNING_ACCESS,
    DepthEndingAccess: RENDER_PASS_ENDING_ACCESS,
    StencilEndingAccess: RENDER_PASS_ENDING_ACCESS,
};

pub const RENDER_PASS_FLAGS = UINT;
pub const RENDER_PASS_FLAG_NONE = 0;
pub const RENDER_PASS_FLAG_ALLOW_UAV_WRITES = 0x1;
pub const RENDER_PASS_FLAG_SUSPENDING_PASS = 0x2;
pub const RENDER_PASS_FLAG_RESUMING_PASS = 0x4;

pub const META_COMMAND_PARAMETER_TYPE = enum(UINT) {
    FLOAT = 0,
    UINT64 = 1,
    GPU_VIRTUAL_ADDRESS = 2,
    CPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV = 3,
    GPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV = 4,
};

pub const META_COMMAND_PARAMETER_FLAGS = UINT;
pub const META_COMMAND_PARAMETER_FLAG_INPUT = 0x1;
pub const META_COMMAND_PARAMETER_FLAG_OUTPUT = 0x2;

pub const META_COMMAND_PARAMETER_STAGE = enum(UINT) {
    CREATION = 0,
    INITIALIZATION = 1,
    EXECUTION = 2,
};

pub const META_COMMAND_PARAMETER_DESC = extern struct {
    Name: LPCWSTR,
    Type: META_COMMAND_PARAMETER_TYPE,
    Flags: META_COMMAND_PARAMETER_FLAGS,
    RequiredResourceState: RESOURCE_STATES,
    StructureOffset: UINT,
};

pub const GRAPHICS_STATES = UINT;
pub const GRAPHICS_STATE_NONE = 0;
pub const GRAPHICS_STATE_IA_VERTEX_BUFFERS = (1 << 0);
pub const GRAPHICS_STATE_IA_INDEX_BUFFER = (1 << 1);
pub const GRAPHICS_STATE_IA_PRIMITIVE_TOPOLOGY = (1 << 2);
pub const GRAPHICS_STATE_DESCRIPTOR_HEAP = (1 << 3);
pub const GRAPHICS_STATE_GRAPHICS_ROOT_SIGNATURE = (1 << 4);
pub const GRAPHICS_STATE_COMPUTE_ROOT_SIGNATURE = (1 << 5);
pub const GRAPHICS_STATE_RS_VIEWPORTS = (1 << 6);
pub const GRAPHICS_STATE_RS_SCISSOR_RECTS = (1 << 7);
pub const GRAPHICS_STATE_PREDICATION = (1 << 8);
pub const GRAPHICS_STATE_OM_RENDER_TARGETS = (1 << 9);
pub const GRAPHICS_STATE_OM_STENCIL_REF = (1 << 10);
pub const GRAPHICS_STATE_OM_BLEND_FACTOR = (1 << 11);
pub const GRAPHICS_STATE_PIPELINE_STATE = (1 << 12);
pub const GRAPHICS_STATE_SO_TARGETS = (1 << 13);
pub const GRAPHICS_STATE_OM_DEPTH_BOUNDS = (1 << 14);
pub const GRAPHICS_STATE_SAMPLE_POSITIONS = (1 << 15);
pub const GRAPHICS_STATE_VIEW_INSTANCE_MASK = (1 << 16);

pub const META_COMMAND_DESC = extern struct {
    Id: GUID,
    Name: LPCWSTR,
    InitializationDirtyState: GRAPHICS_STATES,
    ExecutionDirtyState: GRAPHICS_STATES,
};

pub const IMetaCommand = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        metacmd: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetRequiredParameterResourceSize(
                self: *T,
                stage: META_COMMAND_PARAMETER_STAGE,
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
                META_COMMAND_PARAMETER_STAGE,
                UINT,
            ) callconv(WINAPI) UINT64,
        };
    }
};

pub const STATE_SUBOBJECT_TYPE = enum(UINT) {
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
    MAX_VALID,
};

pub const STATE_SUBOBJECT = extern struct {
    Type: STATE_SUBOBJECT_TYPE,
    pDesc: *const anyopaque,
};

pub const STATE_OBJECT_FLAGS = UINT;
pub const STATE_OBJECT_FLAG_NONE = 0;
pub const STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS = 0x1;
pub const STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS = 0x2;
pub const STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS = 0x4;

pub const STATE_OBJECT_CONFIG = extern struct {
    Flags: STATE_OBJECT_FLAGS,
};

pub const GLOBAL_ROOT_SIGNATURE = extern struct {
    pGlobalRootSignature: *IRootSignature,
};

pub const LOCAL_ROOT_SIGNATURE = extern struct {
    pLocalRootSignature: *IRootSignature,
};

pub const NODE_MASK = extern struct {
    NodeMask: UINT,
};

pub const EXPORT_FLAGS = UINT;

pub const EXPORT_DESC = extern struct {
    Name: LPCWSTR,
    ExportToRename: LPCWSTR,
    Flags: EXPORT_FLAGS,
};

pub const DXIL_LIBRARY_DESC = extern struct {
    DXILLibrary: SHADER_BYTECODE,
    NumExports: UINT,
    pExports: ?[*]EXPORT_DESC,
};

pub const EXISTING_COLLECTION_DESC = extern struct {
    pExistingCollection: *IStateObject,
    NumExports: UINT,
    pExports: [*]EXPORT_DESC,
};

pub const SUBOBJECT_TO_EXPORTS_ASSOCIATION = extern struct {
    pSubobjectToAssociate: *const STATE_SUBOBJECT,
    NumExports: UINT,
    pExports: [*]LPCWSTR,
};

pub const DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION = extern struct {
    SubobjectToAssociate: LPCWSTR,
    NumExports: UINT,
    pExports: [*]LPCWSTR,
};

pub const HIT_GROUP_TYPE = enum(UINT) {
    TRIANGLES = 0,
    PROCEDURAL_PRIMITIVE = 0x1,
};

pub const HIT_GROUP_DESC = extern struct {
    HitGroupExport: LPCWSTR,
    Type: HIT_GROUP_TYPE,
    AnyHitShaderImport: LPCWSTR,
    ClosestHitShaderImport: LPCWSTR,
    IntersectionShaderImport: LPCWSTR,
};

pub const RAYTRACING_SHADER_CONFIG = extern struct {
    MaxPayloadSizeInBytes: UINT,
    MaxAttributeSizeInBytes: UINT,
};

pub const RAYTRACING_PIPELINE_CONFIG = extern struct {
    MaxTraceRecursionDepth: UINT,
};

pub const RAYTRACING_PIPELINE_FLAGS = UINT;
pub const RAYTRACING_PIPELINE_FLAG_NONE = 0;
pub const RAYTRACING_PIPELINE_FLAG_SKIP_TRIANGLES = 0x100;
pub const RAYTRACING_PIPELINE_FLAG_SKIP_PROCEDURAL_PRIMITIVES = 0x200;

pub const RAYTRACING_PIPELINE_CONFIG1 = extern struct {
    MaxTraceRecursionDepth: UINT,
    Flags: RAYTRACING_PIPELINE_FLAGS,
};

pub const STATE_OBJECT_TYPE = enum(UINT) {
    COLLECTION = 0,
    RAYTRACING_PIPELINE = 3,
};

pub const STATE_OBJECT_DESC = extern struct {
    Type: STATE_OBJECT_TYPE,
    NumSubobjects: UINT,
    pSubobjects: [*]const STATE_SUBOBJECT,
};

pub const RAYTRACING_GEOMETRY_FLAGS = UINT;
pub const RAYTRACING_GEOMETRY_FLAG_NONE = 0;
pub const RAYTRACING_GEOMETRY_FLAG_OPAQUE = 0x1;
pub const RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION = 0x2;

pub const RAYTRACING_GEOMETRY_TYPE = enum(UINT) {
    TRIANGLES = 0,
    PROCEDURAL_PRIMITIVE_AABBS = 1,
};

pub const RAYTRACING_INSTANCE_FLAGS = UINT;
pub const RAYTRACING_INSTANCE_FLAG_NONE = 0;
pub const RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE = 0x1;
pub const RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE = 0x2;
pub const RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE = 0x4;
pub const RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE = 0x8;

pub const GPU_VIRTUAL_ADDRESS_AND_STRIDE = extern struct {
    StartAddress: GPU_VIRTUAL_ADDRESS,
    StrideInBytes: UINT64,
};

pub const GPU_VIRTUAL_ADDRESS_RANGE = extern struct {
    StartAddress: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
};

pub const GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE = extern struct {
    StartAddress: GPU_VIRTUAL_ADDRESS,
    SizeInBytes: UINT64,
    StrideInBytes: UINT64,
};

pub const RAYTRACING_GEOMETRY_TRIANGLES_DESC = extern struct {
    Transform3x4: GPU_VIRTUAL_ADDRESS,
    IndexFormat: dxgi.FORMAT,
    VertexFormat: dxgi.FORMAT,
    IndexCount: UINT,
    VertexCount: UINT,
    IndexBuffer: GPU_VIRTUAL_ADDRESS,
    VertexBuffer: GPU_VIRTUAL_ADDRESS_AND_STRIDE,
};

pub const RAYTRACING_AABB = extern struct {
    MinX: FLOAT,
    MinY: FLOAT,
    MinZ: FLOAT,
    MaxX: FLOAT,
    MaxY: FLOAT,
    MaxZ: FLOAT,
};

pub const RAYTRACING_GEOMETRY_AABBS_DESC = extern struct {
    AABBCount: UINT64,
    AABBs: GPU_VIRTUAL_ADDRESS_AND_STRIDE,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS = UINT;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE = 0;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE = 0x1;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION = 0x2;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE = 0x4;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD = 0x8;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY = 0x10;
pub const RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE = 0x20;

pub const RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE = enum(UINT) {
    CLONE = 0,
    COMPACT = 0x1,
    VISUALIZATION_DECODE_FOR_TOOLS = 0x2,
    SERIALIZE = 0x3,
    DESERIALIZE = 0x4,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_TYPE = enum(UINT) {
    TOP_LEVEL = 0,
    BOTTOM_LEVEL = 0x1,
};

pub const ELEMENTS_LAYOUT = enum(UINT) {
    ARRAY = 0,
    ARRAY_OF_POINTERS = 0x1,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE = enum(UINT) {
    COMPACTED_SIZE = 0,
    TOOLS_VISUALIZATION = 0x1,
    SERIALIZATION = 0x2,
    CURRENT_SIZE = 0x3,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC = extern struct {
    DestBuffer: GPU_VIRTUAL_ADDRESS,
    InfoType: RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE_DESC = extern struct {
    CompactedSizeInBytes: UINT64,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TOOLS_VISUALIZATION_DESC = extern struct {
    DecodedSizeInBytes: UINT64,
};

pub const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_TOOLS_VISUALIZATION_HEADER = extern struct {
    Type: RAYTRACING_ACCELERATION_STRUCTURE_TYPE,
    NumDescs: UINT,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_SERIALIZATION_DESC = extern struct {
    SerializedSizeInBytes: UINT64,
    NumBottomLevelAccelerationStructurePointers: UINT64,
};

pub const SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER = extern struct {
    DriverOpaqueGUID: GUID,
    DriverOpaqueVersioningData: [16]BYTE,
};

pub const SERIALIZED_DATA_TYPE = enum(UINT) {
    RAYTRACING_ACCELERATION_STRUCTURE = 0,
};

pub const DRIVER_MATCHING_IDENTIFIER_STATUS = enum(UINT) {
    COMPATIBLE_WITH_DEVICE = 0,
    UNSUPPORTED_TYPE = 0x1,
    UNRECOGNIZED = 0x2,
    INCOMPATIBLE_VERSION = 0x3,
    INCOMPATIBLE_TYPE = 0x4,
};

pub const SERIALIZED_RAYTRACING_ACCELERATION_STRUCTURE_HEADER = extern struct {
    DriverMatchingIdentifier: SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
    SerializedSizeInBytesIncludingHeader: UINT64,
    DeserializedSizeInBytes: UINT64,
    NumBottomLevelAccelerationStructurePointersAfterHeader: UINT64,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_CURRENT_SIZE_DESC = extern struct {
    CurrentSizeInBytes: UINT64,
};

pub const RAYTRACING_INSTANCE_DESC = packed struct {
    Transform: [3][4]FLOAT,
    InstanceID: u24,
    InstanceMask: u8,
    InstanceContributionToHitGroupIndex: u24,
    Flags: u8,
    AccelerationStructure: GPU_VIRTUAL_ADDRESS,
};
comptime {
    std.debug.assert(@sizeOf(RAYTRACING_INSTANCE_DESC) == 64);
    //std.debug.assert(@alignOf(RAYTRACING_INSTANCE_DESC) == 16);
}

pub const RAYTRACING_GEOMETRY_DESC = extern struct {
    Type: RAYTRACING_GEOMETRY_TYPE,
    Flags: RAYTRACING_GEOMETRY_FLAGS,
    u: extern union {
        Triangles: RAYTRACING_GEOMETRY_TRIANGLES_DESC,
        AABBs: RAYTRACING_GEOMETRY_AABBS_DESC,
    },
};

pub const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS = extern struct {
    Type: RAYTRACING_ACCELERATION_STRUCTURE_TYPE,
    Flags: RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS,
    NumDescs: UINT,
    DescsLayout: ELEMENTS_LAYOUT,
    u: extern union {
        InstanceDescs: GPU_VIRTUAL_ADDRESS,
        pGeometryDescs: [*]const RAYTRACING_GEOMETRY_DESC,
        ppGeometryDescs: [*]const *RAYTRACING_GEOMETRY_DESC,
    },
};

pub const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC = extern struct {
    DestAccelerationStructureData: GPU_VIRTUAL_ADDRESS,
    Inputs: BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
    SourceAccelerationStructureData: GPU_VIRTUAL_ADDRESS,
    ScratchAccelerationStructureData: GPU_VIRTUAL_ADDRESS,
};

pub const RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO = extern struct {
    ResultDataMaxSizeInBytes: UINT64,
    ScratchDataSizeInBytes: UINT64,
    UpdateScratchDataSizeInBytes: UINT64,
};

pub const IID_IStateObject = GUID.parse("{47016943-fca8-4594-93ea-af258b55346d}");
pub const IStateObject = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        stateobj: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IID_IStateObjectProperties = GUID.parse("{de5fa827-9bf9-4f26-89ff-d7f56fde3860}");
pub const IStateObjectProperties = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        properties: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetShaderIdentifier(self: *T, export_name: LPCWSTR) *anyopaque {
                return self.v.properties.GetShaderIdentifier(self, export_name);
            }
            pub inline fn GetShaderStackSize(self: *T, export_name: LPCWSTR) UINT64 {
                return self.v.properties.GetShaderStackSize(self, export_name);
            }
            pub inline fn GetPipelineStackSize(self: *T) UINT64 {
                return self.v.properties.GetPipelineStackSize(self);
            }
            pub inline fn SetPipelineStackSize(self: *T, stack_size: UINT64) void {
                self.v.properties.SetPipelineStackSize(self, stack_size);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetShaderIdentifier: fn (*T, LPCWSTR) callconv(WINAPI) *anyopaque,
            GetShaderStackSize: fn (*T, LPCWSTR) callconv(WINAPI) UINT64,
            GetPipelineStackSize: fn (*T) callconv(WINAPI) UINT64,
            SetPipelineStackSize: fn (*T, UINT64) callconv(WINAPI) void,
        };
    }
};

pub const DISPATCH_RAYS_DESC = extern struct {
    RayGenerationShaderRecord: GPU_VIRTUAL_ADDRESS_RANGE,
    MissShaderTable: GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    HitGroupTable: GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    CallableShaderTable: GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE,
    Width: UINT,
    Height: UINT,
    Depth: UINT,
};

pub const IGraphicsCommandList4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: IGraphicsCommandList1.VTable(Self),
        grcmdlist2: IGraphicsCommandList2.VTable(Self),
        grcmdlist3: IGraphicsCommandList3.VTable(Self),
        grcmdlist4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace IGraphicsCommandList1.Methods(Self);
    usingnamespace IGraphicsCommandList2.Methods(Self);
    usingnamespace IGraphicsCommandList3.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn BeginRenderPass(
                self: *T,
                num_render_targets: UINT,
                render_targets: ?[*]const RENDER_PASS_RENDER_TARGET_DESC,
                depth_stencil: ?*const RENDER_PASS_DEPTH_STENCIL_DESC,
                flags: RENDER_PASS_FLAGS,
            ) void {
                self.v.grcmdlist4.BeginRenderPass(self, num_render_targets, render_targets, depth_stencil, flags);
            }
            pub inline fn EndRenderPass(self: *T) void {
                self.v.grcmdlist4.EndRenderPass(self);
            }
            pub inline fn InitializeMetaCommand(
                self: *T,
                meta_cmd: *IMetaCommand,
                init_param_data: ?*const anyopaque,
                data_size: SIZE_T,
            ) void {
                self.v.grcmdlist4.InitializeMetaCommand(self, meta_cmd, init_param_data, data_size);
            }
            pub inline fn ExecuteMetaCommand(
                self: *T,
                meta_cmd: *IMetaCommand,
                exe_param_data: ?*const anyopaque,
                data_size: SIZE_T,
            ) void {
                self.v.grcmdlist4.InitializeMetaCommand(self, meta_cmd, exe_param_data, data_size);
            }
            pub inline fn BuildRaytracingAccelerationStructure(
                self: *T,
                desc: *const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC,
                num_post_build_descs: UINT,
                post_build_descs: ?[*]const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
            ) void {
                self.v.grcmdlist4.BuildRaytracingAccelerationStructure(self, desc, num_post_build_descs, post_build_descs);
            }
            pub inline fn EmitRaytracingAccelerationStructurePostbuildInfo(
                self: *T,
                desc: *const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
                num_src_accel_structs: UINT,
                src_accel_struct_data: [*]const GPU_VIRTUAL_ADDRESS,
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
                dst_data: GPU_VIRTUAL_ADDRESS,
                src_data: GPU_VIRTUAL_ADDRESS,
                mode: RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE,
            ) void {
                self.v.grcmdlist4.CopyRaytracingAccelerationStructure(self, dst_data, src_data, mode);
            }
            pub inline fn SetPipelineState1(self: *T, state_obj: *IStateObject) void {
                self.v.grcmdlist4.SetPipelineState1(self, state_obj);
            }
            pub inline fn DispatchRays(self: *T, desc: *const DISPATCH_RAYS_DESC) void {
                self.v.grcmdlist4.DispatchRays(self, desc);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            BeginRenderPass: fn (
                *T,
                UINT,
                ?[*]const RENDER_PASS_RENDER_TARGET_DESC,
                ?*const RENDER_PASS_DEPTH_STENCIL_DESC,
                RENDER_PASS_FLAGS,
            ) callconv(WINAPI) void,
            EndRenderPass: fn (*T) callconv(WINAPI) void,
            InitializeMetaCommand: fn (*T, *IMetaCommand, ?*const anyopaque, SIZE_T) callconv(WINAPI) void,
            ExecuteMetaCommand: fn (*T, *IMetaCommand, ?*const anyopaque, SIZE_T) callconv(WINAPI) void,
            BuildRaytracingAccelerationStructure: fn (
                *T,
                *const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC,
                UINT,
                ?[*]const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
            ) callconv(WINAPI) void,
            EmitRaytracingAccelerationStructurePostbuildInfo: fn (
                *T,
                *const RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC,
                UINT,
                [*]const GPU_VIRTUAL_ADDRESS,
            ) callconv(WINAPI) void,
            CopyRaytracingAccelerationStructure: fn (
                *T,
                GPU_VIRTUAL_ADDRESS,
                GPU_VIRTUAL_ADDRESS,
                RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE,
            ) callconv(WINAPI) void,
            SetPipelineState1: fn (*T, *IStateObject) callconv(WINAPI) void,
            DispatchRays: fn (*T, *const DISPATCH_RAYS_DESC) callconv(WINAPI) void,
        };
    }
};

pub const RS_SET_SHADING_RATE_COMBINER_COUNT = 2;

pub const SHADING_RATE = enum(UINT) {
    _1X1 = 0,
    _1X2 = 0x1,
    _2X1 = 0x4,
    _2X2 = 0x5,
    _2X4 = 0x6,
    _4X2 = 0x9,
    _4X4 = 0xa,
};

pub const SHADING_RATE_COMBINER = enum(UINT) {
    PASSTHROUGH = 0,
    OVERRIDE = 1,
    COMBINER_MIN = 2,
    COMBINER_MAX = 3,
    COMBINER_SUM = 4,
};

pub const IGraphicsCommandList5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: IGraphicsCommandList1.VTable(Self),
        grcmdlist2: IGraphicsCommandList2.VTable(Self),
        grcmdlist3: IGraphicsCommandList3.VTable(Self),
        grcmdlist4: IGraphicsCommandList4.VTable(Self),
        grcmdlist5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace IGraphicsCommandList1.Methods(Self);
    usingnamespace IGraphicsCommandList2.Methods(Self);
    usingnamespace IGraphicsCommandList3.Methods(Self);
    usingnamespace IGraphicsCommandList4.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn RSSetShadingRate(
                self: *T,
                base_shading_rate: SHADING_RATE,
                combiners: ?[RS_SET_SHADING_RATE_COMBINER_COUNT]SHADING_RATE_COMBINER,
            ) void {
                self.v.grcmdlist5.RSSetShadingRate(self, base_shading_rate, combiners);
            }
            pub inline fn RSSetShadingRateImage(self: *T, shading_rate_img: ?*IResource) void {
                self.v.grcmdlist5.RSSetShadingRateImage(self, shading_rate_img);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            RSSetShadingRate: fn (
                *T,
                SHADING_RATE,
                ?[RS_SET_SHADING_RATE_COMBINER_COUNT]SHADING_RATE_COMBINER,
            ) callconv(WINAPI) void,
            RSSetShadingRateImage: fn (*T, ?*IResource) callconv(WINAPI) void,
        };
    }
};

pub const IGraphicsCommandList6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdlist: ICommandList.VTable(Self),
        grcmdlist: IGraphicsCommandList.VTable(Self),
        grcmdlist1: IGraphicsCommandList1.VTable(Self),
        grcmdlist2: IGraphicsCommandList2.VTable(Self),
        grcmdlist3: IGraphicsCommandList3.VTable(Self),
        grcmdlist4: IGraphicsCommandList4.VTable(Self),
        grcmdlist5: IGraphicsCommandList5.VTable(Self),
        grcmdlist6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace ICommandList.Methods(Self);
    usingnamespace IGraphicsCommandList.Methods(Self);
    usingnamespace IGraphicsCommandList1.Methods(Self);
    usingnamespace IGraphicsCommandList2.Methods(Self);
    usingnamespace IGraphicsCommandList3.Methods(Self);
    usingnamespace IGraphicsCommandList4.Methods(Self);
    usingnamespace IGraphicsCommandList5.Methods(Self);
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

pub const ICommandQueue = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        cmdqueue: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn UpdateTileMappings(
                self: *T,
                resource: *IResource,
                num_resource_regions: UINT,
                resource_region_start_coordinates: ?[*]const TILED_RESOURCE_COORDINATE,
                resource_region_sizes: ?[*]const TILE_REGION_SIZE,
                heap: ?*IHeap,
                num_ranges: UINT,
                range_flags: ?[*]const TILE_RANGE_FLAGS,
                heap_range_start_offsets: ?[*]const UINT,
                range_tile_counts: ?[*]const UINT,
                flags: TILE_MAPPING_FLAGS,
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
                dst_resource: *IResource,
                dst_region_start_coordinate: *const TILED_RESOURCE_COORDINATE,
                src_resource: *IResource,
                src_region_start_coordinate: *const TILED_RESOURCE_COORDINATE,
                region_size: *const TILE_REGION_SIZE,
                flags: TILE_MAPPING_FLAGS,
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
            pub inline fn ExecuteCommandLists(self: *T, num: UINT, cmdlists: [*]const *ICommandList) void {
                self.v.cmdqueue.ExecuteCommandLists(self, num, cmdlists);
            }
            pub inline fn SetMarker(self: *T, metadata: UINT, data: ?*const anyopaque, size: UINT) void {
                self.v.cmdqueue.SetMarker(self, metadata, data, size);
            }
            pub inline fn BeginEvent(self: *T, metadata: UINT, data: ?*const anyopaque, size: UINT) void {
                self.v.cmdqueue.BeginEvent(self, metadata, data, size);
            }
            pub inline fn EndEvent(self: *T) void {
                self.v.cmdqueue.EndEvent(self);
            }
            pub inline fn Signal(self: *T, fence: *IFence, value: UINT64) HRESULT {
                return self.v.cmdqueue.Signal(self, fence, value);
            }
            pub inline fn Wait(self: *T, fence: *IFence, value: UINT64) HRESULT {
                return self.v.cmdqueue.Wait(self, fence, value);
            }
            pub inline fn GetTimestampFrequency(self: *T, frequency: *UINT64) HRESULT {
                return self.v.cmdqueue.GetTimestampFrequency(self, frequency);
            }
            pub inline fn GetClockCalibration(self: *T, gpu_timestamp: *UINT64, cpu_timestamp: *UINT64) HRESULT {
                return self.v.cmdqueue.GetClockCalibration(self, gpu_timestamp, cpu_timestamp);
            }
            pub inline fn GetDesc(self: *T) COMMAND_QUEUE_DESC {
                var desc: COMMAND_QUEUE_DESC = undefined;
                _ = self.v.cmdqueue.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            UpdateTileMappings: fn (
                *T,
                *IResource,
                UINT,
                ?[*]const TILED_RESOURCE_COORDINATE,
                ?[*]const TILE_REGION_SIZE,
                *IHeap,
                UINT,
                ?[*]const TILE_RANGE_FLAGS,
                ?[*]const UINT,
                ?[*]const UINT,
                TILE_MAPPING_FLAGS,
            ) callconv(WINAPI) void,
            CopyTileMappings: fn (
                *T,
                *IResource,
                *const TILED_RESOURCE_COORDINATE,
                *IResource,
                *const TILED_RESOURCE_COORDINATE,
                *const TILE_REGION_SIZE,
                TILE_MAPPING_FLAGS,
            ) callconv(WINAPI) void,
            ExecuteCommandLists: fn (*T, UINT, [*]const *ICommandList) callconv(WINAPI) void,
            SetMarker: fn (*T, UINT, ?*const anyopaque, UINT) callconv(WINAPI) void,
            BeginEvent: fn (*T, UINT, ?*const anyopaque, UINT) callconv(WINAPI) void,
            EndEvent: fn (*T) callconv(WINAPI) void,
            Signal: fn (*T, *IFence, UINT64) callconv(WINAPI) HRESULT,
            Wait: fn (*T, *IFence, UINT64) callconv(WINAPI) HRESULT,
            GetTimestampFrequency: fn (*T, *UINT64) callconv(WINAPI) HRESULT,
            GetClockCalibration: fn (*T, *UINT64, *UINT64) callconv(WINAPI) HRESULT,
            GetDesc: fn (*T, *COMMAND_QUEUE_DESC) callconv(WINAPI) *COMMAND_QUEUE_DESC,
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

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetNodeCount(self: *T) UINT {
                return self.v.device.GetNodeCount(self);
            }
            pub inline fn CreateCommandQueue(
                self: *T,
                desc: *const COMMAND_QUEUE_DESC,
                guid: *const GUID,
                obj: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateCommandQueue(self, desc, guid, obj);
            }
            pub inline fn CreateCommandAllocator(
                self: *T,
                cmdlist_type: COMMAND_LIST_TYPE,
                guid: *const GUID,
                obj: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateCommandAllocator(self, cmdlist_type, guid, obj);
            }
            pub inline fn CreateGraphicsPipelineState(
                self: *T,
                desc: *const GRAPHICS_PIPELINE_STATE_DESC,
                guid: *const GUID,
                pso: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateGraphicsPipelineState(self, desc, guid, pso);
            }
            pub inline fn CreateComputePipelineState(
                self: *T,
                desc: *const COMPUTE_PIPELINE_STATE_DESC,
                guid: *const GUID,
                pso: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateComputePipelineState(self, desc, guid, pso);
            }
            pub inline fn CreateCommandList(
                self: *T,
                node_mask: UINT,
                cmdlist_type: COMMAND_LIST_TYPE,
                cmdalloc: *ICommandAllocator,
                initial_state: ?*IPipelineState,
                guid: *const GUID,
                cmdlist: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateCommandList(self, node_mask, cmdlist_type, cmdalloc, initial_state, guid, cmdlist);
            }
            pub inline fn CheckFeatureSupport(self: *T, feature: FEATURE, data: *anyopaque, data_size: UINT) HRESULT {
                return self.v.device.CheckFeatureSupport(self, feature, data, data_size);
            }
            pub inline fn CreateDescriptorHeap(
                self: *T,
                desc: *const DESCRIPTOR_HEAP_DESC,
                guid: *const GUID,
                heap: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateDescriptorHeap(self, desc, guid, heap);
            }
            pub inline fn GetDescriptorHandleIncrementSize(self: *T, heap_type: DESCRIPTOR_HEAP_TYPE) UINT {
                return self.v.device.GetDescriptorHandleIncrementSize(self, heap_type);
            }
            pub inline fn CreateRootSignature(
                self: *T,
                node_mask: UINT,
                blob: *const anyopaque,
                blob_size: UINT64,
                guid: *const GUID,
                signature: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateRootSignature(self, node_mask, blob, blob_size, guid, signature);
            }
            pub inline fn CreateConstantBufferView(
                self: *T,
                desc: ?*const CONSTANT_BUFFER_VIEW_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateConstantBufferView(self, desc, dst_descriptor);
            }
            pub inline fn CreateShaderResourceView(
                self: *T,
                resource: ?*IResource,
                desc: ?*const SHADER_RESOURCE_VIEW_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateShaderResourceView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateUnorderedAccessView(
                self: *T,
                resource: ?*IResource,
                counter_resource: ?*IResource,
                desc: ?*const UNORDERED_ACCESS_VIEW_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
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
                resource: ?*IResource,
                desc: ?*const RENDER_TARGET_VIEW_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateRenderTargetView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateDepthStencilView(
                self: *T,
                resource: ?*IResource,
                desc: ?*const DEPTH_STENCIL_VIEW_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateDepthStencilView(self, resource, desc, dst_descriptor);
            }
            pub inline fn CreateSampler(
                self: *T,
                desc: *const SAMPLER_DESC,
                dst_descriptor: CPU_DESCRIPTOR_HANDLE,
            ) void {
                self.v.device.CreateSampler(self, desc, dst_descriptor);
            }
            pub inline fn CopyDescriptors(
                self: *T,
                num_dst_ranges: UINT,
                dst_range_starts: [*]const CPU_DESCRIPTOR_HANDLE,
                dst_range_sizes: ?[*]const UINT,
                num_src_ranges: UINT,
                src_range_starts: [*]const CPU_DESCRIPTOR_HANDLE,
                src_range_sizes: ?[*]const UINT,
                heap_type: DESCRIPTOR_HEAP_TYPE,
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
                dst_range_start: CPU_DESCRIPTOR_HANDLE,
                src_range_start: CPU_DESCRIPTOR_HANDLE,
                heap_type: DESCRIPTOR_HEAP_TYPE,
            ) void {
                self.v.device.CopyDescriptorsSimple(self, num, dst_range_start, src_range_start, heap_type);
            }
            pub inline fn GetResourceAllocationInfo(
                self: *T,
                visible_mask: UINT,
                num_descs: UINT,
                descs: [*]const RESOURCE_DESC,
            ) RESOURCE_ALLOCATION_INFO {
                var info: RESOURCE_ALLOCATION_INFO = undefined;
                self.v.device.GetResourceAllocationInfo(self, &info, visible_mask, num_descs, descs);
                return info;
            }
            pub inline fn GetCustomHeapProperties(
                self: *T,
                node_mask: UINT,
                heap_type: HEAP_TYPE,
            ) HEAP_PROPERTIES {
                var props: HEAP_PROPERTIES = undefined;
                self.v.device.GetCustomHeapProperties(self, &props, node_mask, heap_type);
                return props;
            }
            pub inline fn CreateCommittedResource(
                self: *T,
                heap_props: *const HEAP_PROPERTIES,
                heap_flags: HEAP_FLAGS,
                desc: *const RESOURCE_DESC,
                state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
            pub inline fn CreateHeap(self: *T, desc: *const HEAP_DESC, guid: *const GUID, heap: ?*?*anyopaque) HRESULT {
                return self.v.device.CreateHeap(self, desc, guid, heap);
            }
            pub inline fn CreatePlacedResource(
                self: *T,
                heap: *IHeap,
                heap_offset: UINT64,
                desc: *const RESOURCE_DESC,
                state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
                desc: *const RESOURCE_DESC,
                state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateReservedResource(self, desc, state, clear_value, guid, resource);
            }
            pub inline fn CreateSharedHandle(
                self: *T,
                object: *IDeviceChild,
                attributes: ?*const SECURITY_ATTRIBUTES,
                access: DWORD,
                name: ?LPCWSTR,
                handle: ?*HANDLE,
            ) HRESULT {
                return self.v.device.CreateSharedHandle(self, object, attributes, access, name, handle);
            }
            pub inline fn OpenSharedHandle(self: *T, handle: HANDLE, guid: *const GUID, object: ?*?*anyopaque) HRESULT {
                return self.v.device.OpenSharedHandle(self, handle, guid, object);
            }
            pub inline fn OpenSharedHandleByName(self: *T, name: LPCWSTR, access: DWORD, handle: ?*HANDLE) HRESULT {
                return self.v.device.OpenSharedHandleByName(self, name, access, handle);
            }
            pub inline fn MakeResident(self: *T, num: UINT, objects: [*]const *IPageable) HRESULT {
                return self.v.device.MakeResident(self, num, objects);
            }
            pub inline fn Evict(self: *T, num: UINT, objects: [*]const *IPageable) HRESULT {
                return self.v.device.Evict(self, num, objects);
            }
            pub inline fn CreateFence(
                self: *T,
                initial_value: UINT64,
                flags: FENCE_FLAGS,
                guid: *const GUID,
                fence: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateFence(self, initial_value, flags, guid, fence);
            }
            pub inline fn GetDeviceRemovedReason(self: *T) HRESULT {
                return self.v.device.GetDeviceRemovedReason(self);
            }
            pub inline fn GetCopyableFootprints(
                self: *T,
                desc: *const RESOURCE_DESC,
                first_subresource: UINT,
                num_subresources: UINT,
                base_offset: UINT64,
                layouts: ?[*]PLACED_SUBRESOURCE_FOOTPRINT,
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
                desc: *const QUERY_HEAP_DESC,
                guid: *const GUID,
                query_heap: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateQueryHeap(self, desc, guid, query_heap);
            }
            pub inline fn SetStablePowerState(self: *T, enable: BOOL) HRESULT {
                return self.v.device.SetStablePowerState(self, enable);
            }
            pub inline fn CreateCommandSignature(
                self: *T,
                desc: *const COMMAND_SIGNATURE_DESC,
                root_signature: ?*IRootSignature,
                guid: *const GUID,
                cmd_signature: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateCommandSignature(self, desc, root_signature, guid, cmd_signature);
            }
            pub inline fn GetResourceTiling(
                self: *T,
                resource: *IResource,
                num_resource_tiles: ?*UINT,
                packed_mip_desc: ?*PACKED_MIP_INFO,
                std_tile_shape_non_packed_mips: ?*TILE_SHAPE,
                num_subresource_tilings: ?*UINT,
                first_subresource: UINT,
                subresource_tiling_for_non_packed_mips: [*]SUBRESOURCE_TILING,
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
            CreateCommandQueue: fn (*T, *const COMMAND_QUEUE_DESC, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            CreateCommandAllocator: fn (*T, COMMAND_LIST_TYPE, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            CreateGraphicsPipelineState: fn (
                *T,
                *const GRAPHICS_PIPELINE_STATE_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateComputePipelineState: fn (
                *T,
                *const COMPUTE_PIPELINE_STATE_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateCommandList: fn (
                *T,
                UINT,
                COMMAND_LIST_TYPE,
                *ICommandAllocator,
                ?*IPipelineState,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CheckFeatureSupport: fn (*T, FEATURE, *anyopaque, UINT) callconv(WINAPI) HRESULT,
            CreateDescriptorHeap: fn (
                *T,
                *const DESCRIPTOR_HEAP_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            GetDescriptorHandleIncrementSize: fn (*T, DESCRIPTOR_HEAP_TYPE) callconv(WINAPI) UINT,
            CreateRootSignature: fn (*T, UINT, *const anyopaque, UINT64, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            CreateConstantBufferView: fn (
                *T,
                ?*const CONSTANT_BUFFER_VIEW_DESC,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateShaderResourceView: fn (
                *T,
                ?*IResource,
                ?*const SHADER_RESOURCE_VIEW_DESC,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateUnorderedAccessView: fn (
                *T,
                ?*IResource,
                ?*IResource,
                ?*const UNORDERED_ACCESS_VIEW_DESC,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateRenderTargetView: fn (
                *T,
                ?*IResource,
                ?*const RENDER_TARGET_VIEW_DESC,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateDepthStencilView: fn (
                *T,
                ?*IResource,
                ?*const DEPTH_STENCIL_VIEW_DESC,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            CreateSampler: fn (*T, *const SAMPLER_DESC, CPU_DESCRIPTOR_HANDLE) callconv(WINAPI) void,
            CopyDescriptors: fn (
                *T,
                UINT,
                [*]const CPU_DESCRIPTOR_HANDLE,
                ?[*]const UINT,
                UINT,
                [*]const CPU_DESCRIPTOR_HANDLE,
                ?[*]const UINT,
                DESCRIPTOR_HEAP_TYPE,
            ) callconv(WINAPI) void,
            CopyDescriptorsSimple: fn (
                *T,
                UINT,
                CPU_DESCRIPTOR_HANDLE,
                CPU_DESCRIPTOR_HANDLE,
                DESCRIPTOR_HEAP_TYPE,
            ) callconv(WINAPI) void,
            GetResourceAllocationInfo: fn (
                *T,
                *RESOURCE_ALLOCATION_INFO,
                UINT,
                UINT,
                [*]const RESOURCE_DESC,
            ) callconv(WINAPI) *RESOURCE_ALLOCATION_INFO,
            GetCustomHeapProperties: fn (
                *T,
                *HEAP_PROPERTIES,
                UINT,
                HEAP_TYPE,
            ) callconv(WINAPI) *HEAP_PROPERTIES,
            CreateCommittedResource: fn (
                *T,
                *const HEAP_PROPERTIES,
                HEAP_FLAGS,
                *const RESOURCE_DESC,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateHeap: fn (*T, *const HEAP_DESC, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            CreatePlacedResource: fn (
                *T,
                *IHeap,
                UINT64,
                *const RESOURCE_DESC,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateReservedResource: fn (
                *T,
                *const RESOURCE_DESC,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateSharedHandle: fn (
                *T,
                *IDeviceChild,
                ?*const SECURITY_ATTRIBUTES,
                DWORD,
                ?LPCWSTR,
                ?*HANDLE,
            ) callconv(WINAPI) HRESULT,
            OpenSharedHandle: fn (*T, HANDLE, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            OpenSharedHandleByName: fn (*T, LPCWSTR, DWORD, ?*HANDLE) callconv(WINAPI) HRESULT,
            MakeResident: fn (*T, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
            Evict: fn (*T, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
            CreateFence: fn (*T, UINT64, FENCE_FLAGS, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            GetDeviceRemovedReason: fn (*T) callconv(WINAPI) HRESULT,
            GetCopyableFootprints: fn (
                *T,
                *const RESOURCE_DESC,
                UINT,
                UINT,
                UINT64,
                ?[*]PLACED_SUBRESOURCE_FOOTPRINT,
                ?[*]UINT,
                ?[*]UINT64,
                ?*UINT64,
            ) callconv(WINAPI) void,
            CreateQueryHeap: fn (*T, *const QUERY_HEAP_DESC, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            SetStablePowerState: fn (*T, BOOL) callconv(WINAPI) HRESULT,
            CreateCommandSignature: fn (
                *T,
                *const COMMAND_SIGNATURE_DESC,
                ?*IRootSignature,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            GetResourceTiling: fn (
                *T,
                *IResource,
                ?*UINT,
                ?*PACKED_MIP_INFO,
                ?*TILE_SHAPE,
                ?*UINT,
                UINT,
                [*]SUBRESOURCE_TILING,
            ) callconv(WINAPI) void,
            GetAdapterLuid: fn (*T, *LUID) callconv(WINAPI) *LUID,
        };
    }
};

pub const MULTIPLE_FENCE_WAIT_FLAGS = enum(UINT) {
    ALL = 0,
    ANY = 1,
};

pub const RESIDENCY_PRIORITY = enum(UINT) {
    MINIMUM = 0x28000000,
    LOW = 0x50000000,
    NORMAL = 0x78000000,
    HIGH = 0xa0010000,
    MAXIMUM = 0xc8000000,
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

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreatePipelineLibrary(
                self: *T,
                blob: *const anyopaque,
                blob_length: SIZE_T,
                guid: *const GUID,
                library: *?*anyopaque,
            ) HRESULT {
                return self.v.device1.CreatePipelineLibrary(self, blob, blob_length, guid, library);
            }
            pub inline fn SetEventOnMultipleFenceCompletion(
                self: *T,
                fences: [*]const *IFence,
                fence_values: [*]const UINT64,
                num_fences: UINT,
                flags: MULTIPLE_FENCE_WAIT_FLAGS,
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
                objects: [*]const *IPageable,
                priorities: [*]const RESIDENCY_PRIORITY,
            ) HRESULT {
                return self.v.device1.SetResidencyPriority(self, num_objects, objects, priorities);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreatePipelineLibrary: fn (*T, *const anyopaque, SIZE_T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            SetEventOnMultipleFenceCompletion: fn (
                *T,
                [*]const *IFence,
                [*]const UINT64,
                UINT,
                MULTIPLE_FENCE_WAIT_FLAGS,
                HANDLE,
            ) callconv(WINAPI) HRESULT,
            SetResidencyPriority: fn (
                *T,
                UINT,
                [*]const *IPageable,
                [*]const RESIDENCY_PRIORITY,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const PIPELINE_STATE_SUBOBJECT_TYPE = enum(UINT) {
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
    MAX_VALID,
};

pub const RT_FORMAT_ARRAY = extern struct {
    RTFormats: [8]dxgi.FORMAT,
    NumRenderTargets: UINT,
};

pub const PIPELINE_STATE_STREAM_DESC = extern struct {
    SizeInBytes: SIZE_T,
    pPipelineStateSubobjectStream: *anyopaque,
};

// NOTE(mziulek): Helper structures for defining Mesh Shaders.
pub const MESH_SHADER_PIPELINE_STATE_DESC = extern struct {
    pRootSignature: ?*IRootSignature,
    AS: SHADER_BYTECODE,
    MS: SHADER_BYTECODE,
    PS: SHADER_BYTECODE,
    BlendState: BLEND_DESC,
    SampleMask: UINT,
    RasterizerState: RASTERIZER_DESC,
    DepthStencilState: DEPTH_STENCIL_DESC1,
    PrimitiveTopologyType: PRIMITIVE_TOPOLOGY_TYPE,
    NumRenderTargets: UINT,
    RTVFormats: [8]dxgi.FORMAT,
    DSVFormat: dxgi.FORMAT,
    SampleDesc: dxgi.SAMPLE_DESC,
    NodeMask: UINT,
    CachedPSO: CACHED_PIPELINE_STATE,
    Flags: PIPELINE_STATE_FLAGS,

    pub fn initDefault() MESH_SHADER_PIPELINE_STATE_DESC {
        var v = std.mem.zeroes(@This());
        v = .{
            .pRootSignature = null,
            .AS = SHADER_BYTECODE.initZero(),
            .MS = SHADER_BYTECODE.initZero(),
            .PS = SHADER_BYTECODE.initZero(),
            .BlendState = BLEND_DESC.initDefault(),
            .SampleMask = 0xffff_ffff,
            .RasterizerState = RASTERIZER_DESC.initDefault(),
            .DepthStencilState = DEPTH_STENCIL_DESC1.initDefault(),
            .PrimitiveTopologyType = .UNDEFINED,
            .NumRenderTargets = 0,
            .RTVFormats = [_]dxgi.FORMAT{.UNKNOWN} ** 8,
            .DSVFormat = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .NodeMask = 0,
            .CachedPSO = CACHED_PIPELINE_STATE.initZero(),
            .Flags = PIPELINE_STATE_FLAG_NONE,
        };
        return v;
    }
};

pub const PIPELINE_MESH_STATE_STREAM = extern struct {
    Flags_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .FLAGS,
    Flags: PIPELINE_STATE_FLAGS,
    NodeMask_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .NODE_MASK,
    NodeMask: UINT,
    pRootSignature_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .ROOT_SIGNATURE,
    pRootSignature: ?*IRootSignature,
    PS_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .PS,
    PS: SHADER_BYTECODE,
    AS_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .AS,
    AS: SHADER_BYTECODE,
    MS_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .MS,
    MS: SHADER_BYTECODE,
    BlendState_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .BLEND,
    BlendState: BLEND_DESC,
    DepthStencilState_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .DEPTH_STENCIL1,
    DepthStencilState: DEPTH_STENCIL_DESC1,
    DSVFormat_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .DEPTH_STENCIL_FORMAT,
    DSVFormat: dxgi.FORMAT,
    RasterizerState_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .RASTERIZER,
    RasterizerState: RASTERIZER_DESC,
    RTVFormats_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .RENDER_TARGET_FORMATS,
    RTVFormats: RT_FORMAT_ARRAY,
    SampleDesc_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .SAMPLE_DESC,
    SampleDesc: dxgi.SAMPLE_DESC,
    SampleMask_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .SAMPLE_MASK,
    SampleMask: UINT,
    CachedPSO_type: PIPELINE_STATE_SUBOBJECT_TYPE align(8) = .CACHED_PSO,
    CachedPSO: CACHED_PIPELINE_STATE,

    pub fn init(desc: MESH_SHADER_PIPELINE_STATE_DESC) PIPELINE_MESH_STATE_STREAM {
        const stream = PIPELINE_MESH_STATE_STREAM{
            .Flags = desc.Flags,
            .NodeMask = desc.NodeMask,
            .pRootSignature = desc.pRootSignature,
            .PS = desc.PS,
            .AS = desc.AS,
            .MS = desc.MS,
            .BlendState = desc.BlendState,
            .DepthStencilState = desc.DepthStencilState,
            .DSVFormat = desc.DSVFormat,
            .RasterizerState = desc.RasterizerState,
            .RTVFormats = .{ .RTFormats = desc.RTVFormats, .NumRenderTargets = desc.NumRenderTargets },
            .SampleDesc = desc.SampleDesc,
            .SampleMask = desc.SampleMask,
            .CachedPSO = desc.CachedPSO,
        };
        return stream;
    }
};

pub const IDevice2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreatePipelineState(
                self: *T,
                desc: *const PIPELINE_STATE_STREAM_DESC,
                guid: *const GUID,
                pso: *?*anyopaque,
            ) HRESULT {
                return self.v.device2.CreatePipelineState(self, desc, guid, pso);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreatePipelineState: fn (
                *T,
                *const PIPELINE_STATE_STREAM_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const RESIDENCY_FLAGS = UINT;
pub const RESIDENCY_FLAG_NONE = 0;
pub const RESIDENCY_FLAG_DENY_OVERBUDGET = 0x1;

pub const IDevice3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OpenExistingHeapFromAddress(
                self: *T,
                address: *const anyopaque,
                guid: *const GUID,
                heap: *?*anyopaque,
            ) HRESULT {
                return self.v.device3.OpenExistingHeapFromAddress(self, address, guid, heap);
            }
            pub inline fn OpenExistingHeapFromFileMapping(
                self: *T,
                file_mapping: HANDLE,
                guid: *const GUID,
                heap: *?*anyopaque,
            ) HRESULT {
                return self.v.device3.OpenExistingHeapFromFileMapping(self, file_mapping, guid, heap);
            }
            pub inline fn EnqueueMakeResident(
                self: *T,
                flags: RESIDENCY_FLAGS,
                num_objects: UINT,
                objects: [*]const *IPageable,
                fence_to_signal: *IFence,
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
            OpenExistingHeapFromAddress: fn (*T, *const anyopaque, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            OpenExistingHeapFromFileMapping: fn (*T, HANDLE, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            EnqueueMakeResident: fn (
                *T,
                RESIDENCY_FLAGS,
                UINT,
                [*]const *IPageable,
                *IFence,
                UINT64,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const COMMAND_LIST_FLAGS = UINT;

pub const RESOURCE_ALLOCATION_INFO1 = extern struct {
    Offset: UINT64,
    Alignment: UINT64,
    SizeInBytes: UINT64,
};

pub const IDevice4 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateCommandList1(
                self: *T,
                node_mask: UINT,
                cmdlist_type: COMMAND_LIST_TYPE,
                flags: COMMAND_LIST_FLAGS,
                guid: *const GUID,
                cmdlist: *?*anyopaque,
            ) HRESULT {
                return self.v.device4.CreateCommandList1(self, node_mask, cmdlist_type, flags, guid, cmdlist);
            }
            pub inline fn CreateProtectedResourceSession(
                self: *T,
                desc: *const PROTECTED_RESOURCE_SESSION_DESC,
                guid: *const GUID,
                session: *?*anyopaque,
            ) HRESULT {
                return self.v.device4.CreateProtectedResourceSession(self, desc, guid, session);
            }
            pub inline fn CreateCommittedResource1(
                self: *T,
                heap_properties: *const HEAP_PROPERTIES,
                heap_flags: HEAP_FLAGS,
                desc: *const RESOURCE_DESC,
                initial_state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                psession: ?*IProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
                desc: *const HEAP_DESC,
                psession: ?*IProtectedResourceSession,
                guid: *const GUID,
                heap: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device4.CreateHeap1(self, desc, psession, guid, heap);
            }
            pub inline fn CreateReservedResource1(
                self: *T,
                desc: *const RESOURCE_DESC,
                initial_state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                psession: ?*IProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
                resource_descs: [*]const RESOURCE_DESC,
                alloc_info: ?[*]RESOURCE_ALLOCATION_INFO1,
            ) RESOURCE_ALLOCATION_INFO {
                var desc: RESOURCE_ALLOCATION_INFO = undefined;
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
                COMMAND_LIST_TYPE,
                COMMAND_LIST_FLAGS,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateProtectedResourceSession: fn (
                *T,
                *const PROTECTED_RESOURCE_SESSION_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateCommittedResource1: fn (
                *T,
                *const HEAP_PROPERTIES,
                HEAP_FLAGS,
                *const RESOURCE_DESC,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                ?*IProtectedResourceSession,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateHeap1: fn (
                *T,
                *const HEAP_DESC,
                ?*IProtectedResourceSession,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateReservedResource1: fn (
                *T,
                *const RESOURCE_DESC,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                ?*IProtectedResourceSession,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            GetResourceAllocationInfo1: fn (
                *T,
                *RESOURCE_ALLOCATION_INFO,
                UINT,
                UINT,
                [*]const RESOURCE_DESC,
                ?[*]RESOURCE_ALLOCATION_INFO1,
            ) callconv(WINAPI) *RESOURCE_ALLOCATION_INFO,
        };
    }
};

pub const LIFETIME_STATE = enum(UINT) {
    IN_USE = 0,
    NOT_IN_USE = 1,
};

pub const ILifetimeOwner = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        ltowner: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn LifetimeStateUpdated(self: *T, new_state: LIFETIME_STATE) void {
                self.v.ltowner.LifetimeStateUpdated(self, new_state);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            LifetimeStateUpdated: fn (*T, LIFETIME_STATE) callconv(WINAPI) void,
        };
    }
};

pub const IDevice5 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateLifetimeTracker(
                self: *T,
                owner: *ILifetimeOwner,
                guid: *const GUID,
                tracker: *?*anyopaque,
            ) HRESULT {
                return self.v.device5.CreateLifetimeTracker(self, owner, guid, tracker);
            }
            pub inline fn RemoveDevice(self: *T) void {
                self.v.device5.RemoveDevice(self);
            }
            pub inline fn EnumerateMetaCommands(
                self: *T,
                num_meta_cmds: *UINT,
                descs: ?[*]META_COMMAND_DESC,
            ) HRESULT {
                return self.v.device5.EnumerateMetaCommands(self, num_meta_cmds, descs);
            }
            pub inline fn EnumerateMetaCommandParameters(
                self: *T,
                cmd_id: *const GUID,
                stage: META_COMMAND_PARAMETER_STAGE,
                total_size: ?*UINT,
                param_count: *UINT,
                param_descs: ?[*]META_COMMAND_PARAMETER_DESC,
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
                creation_param_data: ?*const anyopaque,
                creation_param_data_size: SIZE_T,
                guid: *const GUID,
                meta_cmd: *?*anyopaque,
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
                desc: *const STATE_OBJECT_DESC,
                guid: *const GUID,
                state_object: *?*anyopaque,
            ) HRESULT {
                return self.v.device5.CreateStateObject(self, desc, guid, state_object);
            }
            pub inline fn GetRaytracingAccelerationStructurePrebuildInfo(
                self: *T,
                desc: *const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
                info: *RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO,
            ) void {
                self.v.device5.GetRaytracingAccelerationStructurePrebuildInfo(self, desc, info);
            }
            pub inline fn CheckDriverMatchingIdentifier(
                self: *T,
                serialized_data_type: SERIALIZED_DATA_TYPE,
                identifier_to_check: *const SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
            ) DRIVER_MATCHING_IDENTIFIER_STATUS {
                return self.v.device5.CheckDriverMatchingIdentifier(self, serialized_data_type, identifier_to_check);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateLifetimeTracker: fn (*T, *ILifetimeOwner, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            RemoveDevice: fn (self: *T) callconv(WINAPI) void,
            EnumerateMetaCommands: fn (*T, *UINT, ?[*]META_COMMAND_DESC) callconv(WINAPI) HRESULT,
            EnumerateMetaCommandParameters: fn (
                *T,
                *const GUID,
                META_COMMAND_PARAMETER_STAGE,
                ?*UINT,
                *UINT,
                ?[*]META_COMMAND_PARAMETER_DESC,
            ) callconv(WINAPI) HRESULT,
            CreateMetaCommand: fn (
                *T,
                *const GUID,
                UINT,
                ?*const anyopaque,
                SIZE_T,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateStateObject: fn (
                *T,
                *const STATE_OBJECT_DESC,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            GetRaytracingAccelerationStructurePrebuildInfo: fn (
                *T,
                *const BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS,
                *RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO,
            ) callconv(WINAPI) void,
            CheckDriverMatchingIdentifier: fn (
                *T,
                SERIALIZED_DATA_TYPE,
                *const SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER,
            ) callconv(WINAPI) DRIVER_MATCHING_IDENTIFIER_STATUS,
        };
    }
};

pub const BACKGROUND_PROCESSING_MODE = enum(UINT) {
    ALLOWED = 0,
    ALLOW_INTRUSIVE_MEASUREMENTS = 1,
    DISABLE_BACKGROUND_WORK = 2,
    DISABLE_PROFILING_BY_SYSTEM = 3,
};

pub const MEASUREMENTS_ACTION = enum(UINT) {
    KEEP_ALL = 0,
    COMMIT_RESULTS = 1,
    COMMIT_RESULTS_HIGH_PRIORITY = 2,
    DISCARD_PREVIOUS = 3,
};

pub const IDevice6 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: IDevice5.VTable(Self),
        device6: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace IDevice5.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetBackgroundProcessingMode(
                self: *T,
                mode: BACKGROUND_PROCESSING_MODE,
                measurements_action: MEASUREMENTS_ACTION,
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
                BACKGROUND_PROCESSING_MODE,
                MEASUREMENTS_ACTION,
                ?HANDLE,
                ?*BOOL,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const PROTECTED_RESOURCE_SESSION_DESC1 = extern struct {
    NodeMask: UINT,
    Flags: PROTECTED_RESOURCE_SESSION_FLAGS,
    ProtectionType: GUID,
};

pub const IDevice7 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: IDevice5.VTable(Self),
        device6: IDevice6.VTable(Self),
        device7: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace IDevice5.Methods(Self);
    usingnamespace IDevice6.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn AddToStateObject(
                self: *T,
                addition: *const STATE_OBJECT_DESC,
                state_object: *IStateObject,
                guid: *const GUID,
                new_state_object: *?*anyopaque,
            ) HRESULT {
                return self.v.device7.AddToStateObject(self, addition, state_object, guid, new_state_object);
            }
            pub inline fn CreateProtectedResourceSession1(
                self: *T,
                desc: *const PROTECTED_RESOURCE_SESSION_DESC1,
                guid: *const GUID,
                session: *?*anyopaque,
            ) HRESULT {
                return self.v.device7.CreateProtectedResourceSession1(self, desc, guid, session);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            AddToStateObject: fn (
                *T,
                *const STATE_OBJECT_DESC,
                *IStateObject,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateProtectedResourceSession1: fn (
                *T,
                *const PROTECTED_RESOURCE_SESSION_DESC1,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const MIP_REGION = extern struct {
    Width: UINT,
    Height: UINT,
    Depth: UINT,
};

pub const RESOURCE_DESC1 = extern struct {
    Dimension: RESOURCE_DIMENSION,
    Alignment: UINT64,
    Width: UINT64,
    Height: UINT,
    DepthOrArraySize: UINT16,
    MipLevels: UINT16,
    Format: dxgi.FORMAT,
    SampleDesc: dxgi.SAMPLE_DESC,
    Layout: TEXTURE_LAYOUT,
    Flags: RESOURCE_FLAGS,
    SamplerFeedbackMipRegion: MIP_REGION,
};

pub const IDevice8 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: IDevice5.VTable(Self),
        device6: IDevice6.VTable(Self),
        device7: IDevice7.VTable(Self),
        device8: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace IDevice5.Methods(Self);
    usingnamespace IDevice6.Methods(Self);
    usingnamespace IDevice7.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetResourceAllocationInfo2(
                self: *T,
                visible_mask: UINT,
                num_resource_descs: UINT,
                resource_descs: *const RESOURCE_DESC1,
                alloc_info: ?[*]RESOURCE_ALLOCATION_INFO1,
            ) RESOURCE_ALLOCATION_INFO {
                var desc: RESOURCE_ALLOCATION_INFO = undefined;
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
                heap_properties: *const HEAP_PROPERTIES,
                heap_flags: HEAP_FLAGS,
                desc: *const RESOURCE_DESC1,
                initial_state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                prsession: ?*IProtectedResourceSession,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
                heap: *IHeap,
                heap_offset: UINT64,
                desc: *const RESOURCE_DESC1,
                initial_state: RESOURCE_STATES,
                clear_value: ?*const CLEAR_VALUE,
                guid: *const GUID,
                resource: ?*?*anyopaque,
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
                targeted_resource: ?*IResource,
                feedback_resource: ?*IResource,
                dest_descriptor: CPU_DESCRIPTOR_HANDLE,
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
                desc: *const RESOURCE_DESC1,
                first_subresource: UINT,
                num_subresources: UINT,
                base_offset: UINT64,
                layouts: ?[*]PLACED_SUBRESOURCE_FOOTPRINT,
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
                *const RESOURCE_DESC1,
                ?[*]RESOURCE_ALLOCATION_INFO1,
            ) callconv(WINAPI) RESOURCE_ALLOCATION_INFO,
            CreateCommittedResource2: fn (
                *T,
                *const HEAP_PROPERTIES,
                HEAP_FLAGS,
                *const RESOURCE_DESC1,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                ?*IProtectedResourceSession,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreatePlacedResource1: fn (
                *T,
                *IHeap,
                UINT64,
                *const RESOURCE_DESC1,
                RESOURCE_STATES,
                ?*const CLEAR_VALUE,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateSamplerFeedbackUnorderedAccessView: fn (
                *T,
                ?*IResource,
                ?*IResource,
                CPU_DESCRIPTOR_HANDLE,
            ) callconv(WINAPI) void,
            GetCopyableFootprints1: fn (
                *T,
                *const RESOURCE_DESC1,
                UINT,
                UINT,
                UINT64,
                ?[*]PLACED_SUBRESOURCE_FOOTPRINT,
                ?[*]UINT,
                ?[*]UINT64,
                ?*UINT64,
            ) callconv(WINAPI) void,
        };
    }
};

pub const SHADER_CACHE_KIND_FLAGS = UINT;
pub const SHADER_CACHE_KIND_FLAG_IMPLICIT_D3D_CACHE_FOR_DRIVER = 0x1;
pub const SHADER_CACHE_KIND_FLAG_IMPLICIT_D3D_CONVERSIONS = 0x2;
pub const SHADER_CACHE_KIND_FLAG_IMPLICIT_DRIVER_MANAGED = 0x4;
pub const SHADER_CACHE_KIND_FLAG_APPLICATION_MANAGED = 0x8;

pub const SHADER_CACHE_CONTROL_FLAGS = UINT;
pub const SHADER_CACHE_CONTROL_FLAG_DISABLE = 0x1;
pub const SHADER_CACHE_CONTROL_FLAG_ENABLE = 0x2;
pub const SHADER_CACHE_CONTROL_FLAG_CLEAR = 0x4;

pub const SHADER_CACHE_MODE = enum(UINT) {
    MEMORY = 0,
    DISK = 1,
};

pub const SHADER_CACHE_FLAGS = UINT;
pub const SHADER_CACHE_FLAG_NONE = 0;
pub const SHADER_CACHE_FLAG_DRIVER_VERSIONED = 0x1;
pub const SHADER_CACHE_FLAG_USE_WORKING_DIR = 0x2;

pub const SHADER_CACHE_SESSION_DESC = extern struct {
    Identifier: GUID,
    Mode: SHADER_CACHE_MODE,
    Flags: SHADER_CACHE_FLAGS,
    MaximumInMemoryCacheSizeBytes: UINT,
    MaximumInMemoryCacheEntries: UINT,
    MaximumValueFileSizeBytes: UINT,
    Version: UINT64,
};

pub const IDevice9 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: IDevice1.VTable(Self),
        device2: IDevice2.VTable(Self),
        device3: IDevice3.VTable(Self),
        device4: IDevice4.VTable(Self),
        device5: IDevice5.VTable(Self),
        device6: IDevice6.VTable(Self),
        device7: IDevice7.VTable(Self),
        device8: IDevice8.VTable(Self),
        device9: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace IDevice2.Methods(Self);
    usingnamespace IDevice3.Methods(Self);
    usingnamespace IDevice4.Methods(Self);
    usingnamespace IDevice5.Methods(Self);
    usingnamespace IDevice6.Methods(Self);
    usingnamespace IDevice7.Methods(Self);
    usingnamespace IDevice8.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateShaderCacheSession(
                self: *T,
                desc: *const SHADER_CACHE_SESSION_DESC,
                guid: *const GUID,
                session: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device9.CreateShaderCacheSession(self, desc, guid, session);
            }
            pub inline fn ShaderCacheControl(
                self: *T,
                kinds: SHADER_CACHE_KIND_FLAGS,
                control: SHADER_CACHE_CONTROL_FLAGS,
            ) HRESULT {
                return self.v.device9.ShaderCacheControl(self, kinds, control);
            }
            pub inline fn CreateCommandQueue1(
                self: *T,
                desc: *const COMMAND_QUEUE_DESC,
                creator_id: *const GUID,
                guid: *const GUID,
                cmdqueue: *?*anyopaque,
            ) HRESULT {
                return self.v.device9.CreateCommandQueue1(self, desc, creator_id, guid, cmdqueue);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CreateShaderCacheSession: fn (
                *T,
                *const SHADER_CACHE_SESSION_DESC,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            ShaderCacheControl: fn (
                *T,
                SHADER_CACHE_KIND_FLAGS,
                SHADER_CACHE_CONTROL_FLAGS,
            ) callconv(WINAPI) HRESULT,
            CreateCommandQueue1: fn (
                *T,
                *const COMMAND_QUEUE_DESC,
                *const GUID,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const PROTECTED_SESSION_STATUS = enum(UINT) {
    OK = 0,
    INVALID = 1,
};

pub const IProtectedSession = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        psession: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetStatusFence(self: *T, guid: *const GUID, fence: ?*?*anyopaque) HRESULT {
                return self.v.psession.GetStatusFence(self, guid, fence);
            }
            pub inline fn GetSessionStatus(self: *T) PROTECTED_SESSION_STATUS {
                return self.v.psession.GetSessionStatus(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetStatusFence: fn (*T, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            GetSessionStatus: fn (*T) callconv(WINAPI) PROTECTED_SESSION_STATUS,
        };
    }
};

pub const PROTECTED_RESOURCE_SESSION_FLAGS = UINT;

pub const PROTECTED_RESOURCE_SESSION_DESC = extern struct {
    NodeMask: UINT,
    Flags: PROTECTED_RESOURCE_SESSION_FLAGS,
};

pub const IProtectedResourceSession = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        psession: IProtectedSession.VTable(Self),
        prsession: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IProtectedSession.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc(self: *T) PROTECTED_RESOURCE_SESSION_DESC {
                var desc: PROTECTED_RESOURCE_SESSION_DESC = undefined;
                _ = self.v.prsession.GetDesc(self, &desc);
                return desc;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc: fn (
                *T,
                *PROTECTED_RESOURCE_SESSION_DESC,
            ) callconv(WINAPI) *PROTECTED_RESOURCE_SESSION_DESC,
        };
    }
};

pub extern "d3d12" fn D3D12GetDebugInterface(*const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT;
pub extern "d3d12" fn D3D12CreateDevice(
    ?*IUnknown,
    d3d.FEATURE_LEVEL,
    *const GUID,
    ?*?*anyopaque,
) callconv(WINAPI) HRESULT;
pub extern "d3d12" fn D3D12SerializeVersionedRootSignature(
    *const VERSIONED_ROOT_SIGNATURE_DESC,
    ?*?*d3d.IBlob,
    ?*?*d3d.IBlob,
) callconv(WINAPI) HRESULT;

pub const IID_IDevice = GUID{
    .Data1 = 0x189819f1,
    .Data2 = 0x1db6,
    .Data3 = 0x4b57,
    .Data4 = .{ 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7 },
};
pub const IID_IDevice1 = GUID{
    .Data1 = 0x77acce80,
    .Data2 = 0x638e,
    .Data3 = 0x4e65,
    .Data4 = .{ 0x88, 0x95, 0xc1, 0xf2, 0x33, 0x86, 0x86, 0x3e },
};
pub const IID_IDevice2 = GUID{
    .Data1 = 0x30baa41e,
    .Data2 = 0xb15b,
    .Data3 = 0x475c,
    .Data4 = .{ 0xa0, 0xbb, 0x1a, 0xf5, 0xc5, 0xb6, 0x43, 0x28 },
};
pub const IID_IDevice3 = GUID{
    .Data1 = 0x81dadc15,
    .Data2 = 0x2bad,
    .Data3 = 0x4392,
    .Data4 = .{ 0x93, 0xc5, 0x10, 0x13, 0x45, 0xc4, 0xaa, 0x98 },
};
pub const IID_IDevice4 = GUID{
    .Data1 = 0xe865df17,
    .Data2 = 0xa9ee,
    .Data3 = 0x46f9,
    .Data4 = .{ 0xa4, 0x63, 0x30, 0x98, 0x31, 0x5a, 0xa2, 0xe5 },
};
pub const IID_IDevice5 = GUID{
    .Data1 = 0x8b4f173a,
    .Data2 = 0x2fea,
    .Data3 = 0x4b80,
    .Data4 = .{ 0x8f, 0x58, 0x43, 0x07, 0x19, 0x1a, 0xb9, 0x5d },
};
pub const IID_IDevice6 = GUID{
    .Data1 = 0xc70b221b,
    .Data2 = 0x40e4,
    .Data3 = 0x4a17,
    .Data4 = .{ 0x89, 0xaf, 0x02, 0x5a, 0x07, 0x27, 0xa6, 0xdc },
};
pub const IID_IDevice7 = GUID{
    .Data1 = 0x5c014b53,
    .Data2 = 0x68a1,
    .Data3 = 0x4b9b,
    .Data4 = .{ 0x8b, 0xd1, 0xdd, 0x60, 0x46, 0xb9, 0x35, 0x8b },
};
pub const IID_IDevice8 = GUID{
    .Data1 = 0x9218E6BB,
    .Data2 = 0xF944,
    .Data3 = 0x4F7E,
    .Data4 = .{ 0xA7, 0x5C, 0xB1, 0xB2, 0xC7, 0xB7, 0x01, 0xF3 },
};
pub const IID_IDevice9 = GUID{
    .Data1 = 0x4c80e962,
    .Data2 = 0xf032,
    .Data3 = 0x4f60,
    .Data4 = .{ 0xbc, 0x9e, 0xeb, 0xc2, 0xcf, 0xa1, 0xd8, 0x3c },
};
pub const IID_ICommandQueue = GUID{
    .Data1 = 0x0ec870a6,
    .Data2 = 0x5d7e,
    .Data3 = 0x4c22,
    .Data4 = .{ 0x8c, 0xfc, 0x5b, 0xaa, 0xe0, 0x76, 0x16, 0xed },
};
pub const IID_IFence = GUID{
    .Data1 = 0x0a753dcf,
    .Data2 = 0xc4d8,
    .Data3 = 0x4b91,
    .Data4 = .{ 0xad, 0xf6, 0xbe, 0x5a, 0x60, 0xd9, 0x5a, 0x76 },
};
pub const IID_ICommandAllocator = GUID{
    .Data1 = 0x6102dee4,
    .Data2 = 0xaf59,
    .Data3 = 0x4b09,
    .Data4 = .{ 0xb9, 0x99, 0xb4, 0x4d, 0x73, 0xf0, 0x9b, 0x24 },
};
pub const IID_IPipelineState = GUID{
    .Data1 = 0x765a30f3,
    .Data2 = 0xf624,
    .Data3 = 0x4c6f,
    .Data4 = .{ 0xa8, 0x28, 0xac, 0xe9, 0x48, 0x62, 0x24, 0x45 },
};
pub const IID_IDescriptorHeap = GUID{
    .Data1 = 0x8efb471d,
    .Data2 = 0x616c,
    .Data3 = 0x4f49,
    .Data4 = .{ 0x90, 0xf7, 0x12, 0x7b, 0xb7, 0x63, 0xfa, 0x51 },
};
pub const IID_IResource = GUID{
    .Data1 = 0x696442be,
    .Data2 = 0xa72e,
    .Data3 = 0x4059,
    .Data4 = .{ 0xbc, 0x79, 0x5b, 0x5c, 0x98, 0x04, 0x0f, 0xad },
};
pub const IID_IRootSignature = GUID{
    .Data1 = 0xc54a6b66,
    .Data2 = 0x72df,
    .Data3 = 0x4ee8,
    .Data4 = .{ 0x8b, 0xe5, 0xa9, 0x46, 0xa1, 0x42, 0x92, 0x14 },
};
pub const IID_IGraphicsCommandList = GUID{
    .Data1 = 0x5b160d0f,
    .Data2 = 0xac1b,
    .Data3 = 0x4185,
    .Data4 = .{ 0x8b, 0xa8, 0xb3, 0xae, 0x42, 0xa5, 0xa4, 0x55 },
};
pub const IID_IGraphicsCommandList1 = GUID{
    .Data1 = 0x553103fb,
    .Data2 = 0x1fe7,
    .Data3 = 0x4557,
    .Data4 = .{ 0xbb, 0x38, 0x94, 0x6d, 0x7d, 0x0e, 0x7c, 0xa7 },
};
pub const IID_IGraphicsCommandList2 = GUID{
    .Data1 = 0x38C3E584,
    .Data2 = 0xFF17,
    .Data3 = 0x412C,
    .Data4 = .{ 0x91, 0x50, 0x4F, 0xC6, 0xF9, 0xD7, 0x2A, 0x28 },
};
pub const IID_IGraphicsCommandList3 = GUID{
    .Data1 = 0x6FDA83A7,
    .Data2 = 0xB84C,
    .Data3 = 0x4E38,
    .Data4 = .{ 0x9A, 0xC8, 0xC7, 0xBD, 0x22, 0x01, 0x6B, 0x3D },
};
pub const IID_IGraphicsCommandList4 = GUID{
    .Data1 = 0x8754318e,
    .Data2 = 0xd3a9,
    .Data3 = 0x4541,
    .Data4 = .{ 0x98, 0xcf, 0x64, 0x5b, 0x50, 0xdc, 0x48, 0x74 },
};
pub const IID_IGraphicsCommandList5 = GUID{
    .Data1 = 0x55050859,
    .Data2 = 0x4024,
    .Data3 = 0x474c,
    .Data4 = .{ 0x87, 0xf5, 0x64, 0x72, 0xea, 0xee, 0x44, 0xea },
};
pub const IID_IGraphicsCommandList6 = GUID{
    .Data1 = 0xc3827890,
    .Data2 = 0xe548,
    .Data3 = 0x4cfa,
    .Data4 = .{ 0x96, 0xcf, 0x56, 0x89, 0xa9, 0x37, 0x0f, 0x80 },
};

// Error return codes from https://docs.microsoft.com/en-us/windows/win32/direct3d12/d3d12-graphics-reference-returnvalues
pub const ERROR_ADAPTER_NOT_FOUND = @bitCast(HRESULT, @as(c_ulong, 0x887E0001));
pub const ERROR_DRIVER_VERSION_MISMATCH = @bitCast(HRESULT, @as(c_ulong, 0x887E0002));

// Error set corresponding to the above error return codes
pub const Error = error{
    ADAPTER_NOT_FOUND,
    DRIVER_VERSION_MISMATCH,
};
