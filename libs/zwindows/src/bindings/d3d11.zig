const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;
const HINSTANCE = windows.HINSTANCE;
const SIZE_T = windows.SIZE_T;
const LPCSTR = windows.LPCSTR;
const FLOAT = windows.FLOAT;
const BOOL = windows.BOOL;
const TRUE = windows.TRUE;
const FALSE = windows.FALSE;
const INT = windows.INT;
const UINT8 = windows.UINT8;

const d3dcommon = @import("d3dcommon.zig");
const FEATURE_LEVEL = d3dcommon.FEATURE_LEVEL;
const DRIVER_TYPE = d3dcommon.DRIVER_TYPE;

const dxgi = @import("dxgi.zig");

pub const CREATE_DEVICE_FLAG = packed struct(UINT) {
    SINGLETHREADED: bool = false,
    DEBUG: bool = false,
    SWITCH_TO_REF: bool = false,
    PREVENT_INTERNAL_THREADING_OPTIMIZATIONS: bool = false,
    __unused4: bool = false,
    BGRA_SUPPORT: bool = false,
    DEBUGGABLE: bool = false,
    PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY: bool = false,
    DISABLE_GPU_TIMEOUT: bool = false,
    __unused9: bool = false,
    __unused10: bool = false,
    VIDEO_SUPPORT: bool = false,
    __unused: u20 = 0,
};

pub const SDK_VERSION = 7;

pub const BIND_FLAG = packed struct(UINT) {
    VERTEX_BUFFER: bool = false,
    INDEX_BUFFER: bool = false,
    CONSTANT_BUFFER: bool = false,
    SHADER_RESOURCE: bool = false,
    STREAM_OUTPUT: bool = false,
    RENDER_TARGET: bool = false,
    DEPTH_STENCIL: bool = false,
    UNORDERED_ACCESS: bool = false,
    __unused8: bool = false,
    DECODER: bool = false,
    VIDEO_ENCODER: bool = false,
    __unused: u21 = 0,
};

pub const RECT = windows.RECT;

pub const RESOURCE_DIMENSION = enum(UINT) {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE2D = 3,
    TEXTURE3D = 4,
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

pub const BOX = extern struct {
    left: UINT,
    top: UINT,
    front: UINT,
    right: UINT,
    bottom: UINT,
    back: UINT,
};

pub const BUFFER_RTV = extern struct {
    u0: extern union {
        FirstElement: UINT,
        ElementOffset: UINT,
    },
    u1: extern union {
        NumElements: UINT,
        ElementWidth: UINT,
    },
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
};

pub const TEX2D_ARRAY_RTV = extern struct {
    MipSlice: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2DMS_RTV = extern struct {
    UnusedField_NothingToDefine: UINT = undefined,
};

pub const TEX2DMS_ARRAY_RTV = extern struct {
    FirstArraySlice: UINT,
    ArraySlice: UINT,
};

pub const TEX3D_RTV = extern struct {
    MipSlice: UINT,
    FirstWSlice: UINT,
    WSize: UINT,
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

pub const INPUT_CLASSIFICATION = enum(UINT) {
    INPUT_PER_VERTEX_DATA = 0,
    INPUT_PER_INSTANCE_DATA = 1,
};

pub const APPEND_ALIGNED_ELEMENT: UINT = 0xffffffff;

pub const INPUT_ELEMENT_DESC = extern struct {
    SemanticName: LPCSTR,
    SemanticIndex: UINT,
    Format: dxgi.FORMAT,
    InputSlot: UINT,
    AlignedByteOffset: UINT,
    InputSlotClass: INPUT_CLASSIFICATION,
    InstanceDataStepRate: UINT,
};

pub const SUBRESOURCE_DATA = extern struct {
    pSysMem: ?*const anyopaque,
    SysMemPitch: UINT = 0,
    SysMemSlicePitch: UINT = 0,
};

pub const USAGE = enum(UINT) {
    DEFAULT,
    IMMUTABLE,
    DYNAMIC,
    STAGING,
};

pub const CPU_ACCCESS_FLAG = packed struct(UINT) {
    __unused0: u16 = 0,
    WRITE: bool = false,
    READ: bool = false,
    __unused: u14 = 0,
};

pub const RESOURCE_MISC_FLAG = packed struct(UINT) {
    GENERATE_MIPS: bool = false,
    SHARED: bool = false,
    TEXTURECUBE: bool = false,
    __unused3: bool = false,
    DRAWINDIRECT_ARGS: bool = false,
    BUFFER_ALLOW_RAW_VIEWS: bool = false,
    BUFFER_STRUCTURED: bool = false,
    RESOURCE_CLAMP: bool = false,
    SHARED_KEYEDMUTEX: bool = false,
    GDI_COMPATIBLE: bool = false,
    __unused10: bool = false,
    SHARED_NTHANDLE: bool = false,
    RESTRICTED_CONTENT: bool = false,
    RESTRICT_SHARED_RESOURCE: bool = false,
    RESTRICT_SHARED_RESOURCE_DRIVER: bool = false,
    GUARDED: bool = false,
    __unused16: bool = false,
    TILE_POOL: bool = false,
    TILED: bool = false,
    HW_PROTECTED: bool = false,
    __unused: u12 = 0,
};

pub const BUFFER_DESC = extern struct {
    ByteWidth: UINT,
    Usage: USAGE,
    BindFlags: BIND_FLAG,
    CPUAccessFlags: CPU_ACCCESS_FLAG = .{},
    MiscFlags: RESOURCE_MISC_FLAG = .{},
    StructureByteStride: UINT = 0,
};

pub const VIEWPORT = extern struct {
    TopLeftX: FLOAT,
    TopLeftY: FLOAT,
    Width: FLOAT,
    Height: FLOAT,
    MinDepth: FLOAT,
    MaxDepth: FLOAT,
};

pub const CPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: SIZE_T,
};

pub const MAP = enum(UINT) {
    READ = 1,
    WRITE = 2,
    READ_WRITE = 3,
    WRITE_DISCARD = 4,
    WRITE_NO_OVERWRITE = 5,
};

pub const MAP_FLAG = packed struct(UINT) {
    DO_NOT_WAIT: bool = false,
    __unused: u31 = 0,
};

pub const MAPPED_SUBRESOURCE = extern struct {
    pData: *anyopaque,
    RowPitch: UINT,
    DepthPitch: UINT,
};

pub const PRIMITIVE_TOPOLOGY = d3dcommon.PRIMITIVE_TOPOLOGY;

pub const FILL_MODE = enum(UINT) {
    WIREFRAME = 2,
    SOLID = 3,
};

pub const CULL_MODE = enum(UINT) {
    NONE = 1,
    FRONT = 2,
    BACK = 3,
};

pub const RASTERIZER_DESC = extern struct {
    FillMode: FILL_MODE = .SOLID,
    CullMode: CULL_MODE = .BACK,
    FrontCounterClockwise: BOOL = FALSE,
    DepthBias: INT = 0,
    DepthBiasClamp: FLOAT = 0,
    SlopeScaledDepthBias: FLOAT = 0,
    DepthClipEnable: BOOL = TRUE,
    ScissorEnable: BOOL = FALSE,
    MultisampleEndable: BOOL = FALSE,
    AntialiasedLineEnable: BOOL = FALSE,
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

pub const COLOR_WRITE_ENABLE = packed struct(u8) {
    RED: bool = false,
    GREEN: bool = false,
    BLUE: bool = false,
    ALPHA: bool = false,
    __unused: u4 = 0,

    pub const ALL = COLOR_WRITE_ENABLE{ .RED = true, .GREEN = true, .BLUE = true, .ALPHA = true };
};

pub const RENDER_TARGET_BLEND_DESC = extern struct {
    BlendEnable: BOOL,
    SrcBlend: BLEND,
    DestBlend: BLEND,
    BlendOp: BLEND_OP,
    SrcBlendAlpha: BLEND,
    DestBlendAlpha: BLEND,
    BlendOpAlpha: BLEND_OP,
    RenderTargetWriteMask: COLOR_WRITE_ENABLE,
};

pub const BLEND_DESC = extern struct {
    AlphaToCoverageEnable: BOOL,
    IndependentBlendEnable: BOOL,
    RenderTarget: [8]RENDER_TARGET_BLEND_DESC,
};

pub const TEXTURE2D_DESC = struct {
    Width: UINT,
    Height: UINT,
    MipLevels: UINT,
    ArraySize: UINT,
    Format: dxgi.FORMAT,
    SampleDesc: dxgi.SAMPLE_DESC,
    Usage: USAGE,
    BindFlags: BIND_FLAG,
    CPUAccessFlags: CPU_ACCCESS_FLAG,
    MiscFlags: RESOURCE_MISC_FLAG,
};

pub const BUFFER_SRV = extern struct {
    FirstElement: UINT,
    NumElements: UINT,
};

pub const TEX1D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
};

pub const TEX1D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX2D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
};

pub const TEX2D_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const TEX3D_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
};

pub const TEXCUBE_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
};

pub const TEXCUBE_ARRAY_SRV = extern struct {
    MostDetailedMip: UINT,
    MipLevels: UINT,
    First2DArrayFace: UINT,
    NumCubes: UINT,
};

pub const TEX2DMS_SRV = extern struct {
    UnusedField_NothingToDefine: UINT,
};

pub const TEX2DMS_ARRAY_SRV = extern struct {
    FirstArraySlice: UINT,
    ArraySize: UINT,
};

pub const BUFFEREX_SRV_FLAG = packed struct(UINT) {
    RAW: bool = false,
    __unused: u31 = 0,
};

pub const BUFFEREX_SRV = extern struct {
    FirstElement: UINT,
    NumElements: UINT,
    Flags: BUFFEREX_SRV_FLAG,
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
    BUFFEREX = 11,
};

pub const SHADER_RESOURCE_VIEW_DESC = extern struct {
    Format: dxgi.FORMAT,
    ViewDimension: SRV_DIMENSION,
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
        BufferEx: BUFFEREX_SRV,
    },
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

pub const TEXTURE_ADDRESS_MODE = enum(UINT) {
    WRAP = 1,
    MIRROR = 2,
    CLAMP = 3,
    BORDER = 4,
    MIRROR_ONCE = 5,
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

pub const IID_IDeviceChild = GUID.parse("{1841e5c8-16b0-489b-bcc8-44cfb0d5deae}");
pub const IDeviceChild = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IUnknown.VTable,
        GetDevice: *anyopaque,
        GetPrivateData: *anyopaque,
        SetPrivateData: *anyopaque,
        SetPrivateDataInterface: *anyopaque,
    };
};

pub const IID_IClassLinkage = GUID.parse("{ddf57cba-9543-46e4-a12b-f207a0fe7fed}");
pub const IClassLinkage = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetClassInstance: *anyopaque,
        CreateClassInstance: *anyopaque,
    };
};

pub const IID_IClassInstance = GUID.parse("{a6cd7faa-b0b7-4a2f-9436-8662a65797cb}");
pub const IClassInstance = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetClassLinkage: *anyopaque,
        GetDesc: *anyopaque,
        GetInstanceName: *anyopaque,
        GetTypeName: *anyopaque,
    };
};

pub const IID_IResource = GUID.parse("{dc8e63f3-d12b-4952-b47b-5e45026a862d}");
pub const IResource = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetType: *anyopaque,
        SetEvictionPriority: *anyopaque,
        GetEvictionPriority: *anyopaque,
    };
};

pub const IID_IDeviceContext = GUID.parse("{c0bfa96c-e089-44fb-8eaf-26f8796190da}");
pub const IDeviceContext = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);

            pub inline fn VSSetConstantBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppConstantBuffers: ?[*]const *IBuffer,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).VSSetConstantBuffers(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumBuffers,
                    ppConstantBuffers,
                );
            }
            pub inline fn PSSetShaderResources(
                self: *T,
                StartSlot: UINT,
                NumViews: UINT,
                ppShaderResourceViews: ?[*]const *IShaderResourceView,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).PSSetShaderResources(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumViews,
                    ppShaderResourceViews,
                );
            }
            pub inline fn PSSetShader(
                self: *T,
                pPixelShader: ?*IPixelShader,
                ppClassInstance: ?[*]const *IClassInstance,
                NumClassInstances: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).PSSetShader(
                    @as(*IDeviceContext, @ptrCast(self)),
                    pPixelShader,
                    ppClassInstance,
                    NumClassInstances,
                );
            }
            pub inline fn PSSetSamplers(
                self: *T,
                StartSlot: UINT,
                NumSamplers: UINT,
                ppSamplers: ?[*]const *ISamplerState,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).PSSetSamplers(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumSamplers,
                    ppSamplers,
                );
            }
            pub inline fn VSSetShader(
                self: *T,
                pVertexShader: ?*IVertexShader,
                ppClassInstance: ?[*]const *IClassInstance,
                NumClassInstances: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).VSSetShader(
                    @as(*IDeviceContext, @ptrCast(self)),
                    pVertexShader,
                    ppClassInstance,
                    NumClassInstances,
                );
            }
            pub inline fn Draw(
                self: *T,
                VertexCount: UINT,
                StartVertexLocation: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .Draw(@as(*IDeviceContext, @ptrCast(self)), VertexCount, StartVertexLocation);
            }
            pub inline fn DrawIndexed(
                self: *T,
                IndexCount: UINT,
                StartIndexLocation: UINT,
                BaseVertexLocation: INT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .DrawIndexed(
                    @ptrCast(self),
                    IndexCount,
                    StartIndexLocation,
                    BaseVertexLocation,
                );
            }
            pub inline fn Map(
                self: *T,
                pResource: *IResource,
                Subresource: UINT,
                MapType: MAP,
                MapFlags: MAP_FLAG,
                pMappedResource: ?*MAPPED_SUBRESOURCE,
            ) HRESULT {
                return @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).Map(
                    @as(*IDeviceContext, @ptrCast(self)),
                    pResource,
                    Subresource,
                    MapType,
                    MapFlags,
                    pMappedResource,
                );
            }
            pub inline fn Unmap(self: *T, pResource: *IResource, Subresource: UINT) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .Unmap(@as(*IDeviceContext, @ptrCast(self)), pResource, Subresource);
            }
            pub inline fn PSSetConstantBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppConstantBuffers: ?[*]const *IBuffer,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).PSSetConstantBuffers(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumBuffers,
                    ppConstantBuffers,
                );
            }
            pub inline fn IASetInputLayout(self: *T, pInputLayout: ?*IInputLayout) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .IASetInputLayout(@as(*IDeviceContext, @ptrCast(self)), pInputLayout);
            }
            pub inline fn IASetVertexBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppVertexBuffers: ?[*]const *IBuffer,
                pStrides: ?[*]const UINT,
                pOffsets: ?[*]const UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).IASetVertexBuffers(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumBuffers,
                    ppVertexBuffers,
                    pStrides,
                    pOffsets,
                );
            }
            pub inline fn IASetIndexBuffer(
                self: *T,
                pIndexBuffer: ?*IBuffer,
                Format: dxgi.FORMAT,
                Offset: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).IASetIndexBuffer(
                    @ptrCast(self),
                    pIndexBuffer,
                    Format,
                    Offset,
                );
            }
            pub inline fn IASetPrimitiveTopology(self: *T, Topology: PRIMITIVE_TOPOLOGY) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .IASetPrimitiveTopology(@as(*IDeviceContext, @ptrCast(self)), Topology);
            }
            pub inline fn VSSetShaderResources(
                self: *T,
                StartSlot: UINT,
                NumViews: UINT,
                ppShaderResourceViews: ?[*]const *IShaderResourceView,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).VSSetShaderResources(
                    @as(*IDeviceContext, @ptrCast(self)),
                    StartSlot,
                    NumViews,
                    ppShaderResourceViews,
                );
            }
            pub inline fn OMSetRenderTargets(
                self: *T,
                NumViews: UINT,
                ppRenderTargetViews: ?[*]const *IRenderTargetView,
                pDepthStencilView: ?*IDepthStencilView,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).OMSetRenderTargets(
                    @as(*IDeviceContext, @ptrCast(self)),
                    NumViews,
                    ppRenderTargetViews,
                    pDepthStencilView,
                );
            }
            pub inline fn OMSetBlendState(
                self: *T,
                pBlendState: ?*IBlendState,
                BlendFactor: ?*const [4]FLOAT,
                SampleMask: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).OMSetBlendState(
                    @as(*IDeviceContext, @ptrCast(self)),
                    pBlendState,
                    BlendFactor,
                    SampleMask,
                );
            }
            pub inline fn RSSetState(self: *T, pRasterizerState: ?*IRasterizerState) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .RSSetState(@as(*IDeviceContext, @ptrCast(self)), pRasterizerState);
            }
            pub inline fn RSSetViewports(
                self: *T,
                NumViewports: UINT,
                pViewports: [*]const VIEWPORT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .RSSetViewports(@as(*IDeviceContext, @ptrCast(self)), NumViewports, pViewports);
            }
            pub inline fn RSSetScissorRects(self: *T, NumRects: UINT, pRects: ?[*]const RECT) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .RSSetScissorRects(@as(*IDeviceContext, @ptrCast(self)), NumRects, pRects);
            }
            pub inline fn ClearRenderTargetView(
                self: *T,
                pRenderTargetView: *IRenderTargetView,
                ColorRGBA: *const [4]FLOAT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .ClearRenderTargetView(@as(*IDeviceContext, @ptrCast(self)), pRenderTargetView, ColorRGBA);
            }
            pub inline fn Flush(self: *T) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v)).Flush(@as(*IDeviceContext, @ptrCast(self)));
            }
            pub inline fn UpdateSubresource(
                self: *T,
                pDstResource: *IResource,
                DstSubresource: UINT,
                pDstBox: ?*BOX,
                pSrcData: *anyopaque,
                SrcRowPitch: UINT,
                SrcDepthPitch: UINT,
            ) void {
                @as(*const IDeviceContext.VTable, @ptrCast(self.__v))
                    .UpdateSubresource(
                    @ptrCast(self),
                    pDstResource,
                    DstSubresource,
                    pDstBox,
                    pSrcData,
                    SrcRowPitch,
                    SrcDepthPitch,
                );
            }
        };
    }

    pub const VTable = extern struct {
        const T = IDeviceContext;
        base: IDeviceChild.VTable,
        VSSetConstantBuffers: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *IBuffer,
        ) callconv(WINAPI) void,
        PSSetShaderResources: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *IShaderResourceView,
        ) callconv(WINAPI) void,
        PSSetShader: *const fn (
            *T,
            ?*IPixelShader,
            ?[*]const *IClassInstance,
            UINT,
        ) callconv(WINAPI) void,
        PSSetSamplers: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *ISamplerState,
        ) callconv(WINAPI) void,
        VSSetShader: *const fn (
            *T,
            ?*IVertexShader,
            ?[*]const *IClassInstance,
            UINT,
        ) callconv(WINAPI) void,
        DrawIndexed: *const fn (*T, UINT, UINT, INT) callconv(WINAPI) void,
        Draw: *const fn (*T, UINT, UINT) callconv(WINAPI) void,
        Map: *const fn (
            *T,
            *IResource,
            UINT,
            MAP,
            MAP_FLAG,
            ?*MAPPED_SUBRESOURCE,
        ) callconv(WINAPI) HRESULT,
        Unmap: *const fn (*T, *IResource, UINT) callconv(WINAPI) void,
        PSSetConstantBuffers: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *IBuffer,
        ) callconv(WINAPI) void,
        IASetInputLayout: *const fn (*T, ?*IInputLayout) callconv(WINAPI) void,
        IASetVertexBuffers: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *IBuffer,
            ?[*]const UINT,
            ?[*]const UINT,
        ) callconv(WINAPI) void,
        IASetIndexBuffer: *const fn (
            *T,
            ?*IBuffer,
            dxgi.FORMAT,
            UINT,
        ) callconv(WINAPI) void,
        DrawIndexedInstanced: *anyopaque,
        DrawInstanced: *anyopaque,
        GSSetConstantBuffers: *anyopaque,
        GSSetShader: *anyopaque,
        IASetPrimitiveTopology: *const fn (*T, PRIMITIVE_TOPOLOGY) callconv(WINAPI) void,
        VSSetShaderResources: *const fn (
            *T,
            UINT,
            UINT,
            ?[*]const *IShaderResourceView,
        ) callconv(WINAPI) void,
        VSSetSamplers: *anyopaque,
        Begin: *anyopaque,
        End: *anyopaque,
        GetData: *anyopaque,
        SetPredication: *anyopaque,
        GSSetShaderResources: *anyopaque,
        GSSetSamplers: *anyopaque,
        OMSetRenderTargets: *const fn (
            *T,
            UINT,
            ?[*]const *IRenderTargetView,
            ?*IDepthStencilView,
        ) callconv(WINAPI) void,
        OMSetRenderTargetsAndUnorderedAccessViews: *anyopaque,
        OMSetBlendState: *const fn (
            *T,
            ?*IBlendState,
            ?*const [4]FLOAT,
            UINT,
        ) callconv(WINAPI) void,
        OMSetDepthStencilState: *anyopaque,
        SOSetTargets: *anyopaque,
        DrawAuto: *anyopaque,
        DrawIndexedInstancedIndirect: *anyopaque,
        DrawInstancedIndirect: *anyopaque,
        Dispatch: *anyopaque,
        DispatchIndirect: *anyopaque,
        RSSetState: *const fn (*T, ?*IRasterizerState) callconv(WINAPI) void,
        RSSetViewports: *const fn (*T, UINT, [*]const VIEWPORT) callconv(WINAPI) void,
        RSSetScissorRects: *const fn (*T, UINT, ?[*]const RECT) callconv(WINAPI) void,
        CopySubresourceRegion: *anyopaque,
        CopyResource: *anyopaque,
        UpdateSubresource: *const fn (*T, *IResource, UINT, ?*BOX, *anyopaque, UINT, UINT) callconv(WINAPI) void,
        CopyStructureCount: *anyopaque,
        ClearRenderTargetView: *const fn (*T, *IRenderTargetView, *const [4]FLOAT) callconv(WINAPI) void,
        ClearUnorderedAccessViewUint: *anyopaque,
        ClearUnorderedAccessViewFloat: *anyopaque,
        ClearDepthStencilView: *anyopaque,
        GenerateMips: *anyopaque,
        SetResourceMinLOD: *anyopaque,
        GetResourceMinLOD: *anyopaque,
        ResolveSubresource: *anyopaque,
        ExecuteCommandList: *anyopaque,
        HSSetShaderResources: *anyopaque,
        HSSetShader: *anyopaque,
        HSSetSamplers: *anyopaque,
        HSSetConstantBuffers: *anyopaque,
        DSSetShaderResources: *anyopaque,
        DSSetShader: *anyopaque,
        DSSetSamplers: *anyopaque,
        DSSetConstantBuffers: *anyopaque,
        CSSetShaderResources: *anyopaque,
        CSSetUnorderedAccessViews: *anyopaque,
        CSSetShader: *anyopaque,
        CSSetSamplers: *anyopaque,
        CSSetConstantBuffers: *anyopaque,
        VSGetConstantBuffers: *anyopaque,
        PSGetShaderResources: *anyopaque,
        PSGetShader: *anyopaque,
        PSGetSamplers: *anyopaque,
        VSGetShader: *anyopaque,
        PSGetConstantBuffers: *anyopaque,
        IAGetInputLayout: *anyopaque,
        IAGetVertexBuffers: *anyopaque,
        IAGetIndexBuffer: *anyopaque,
        GSGetConstantBuffers: *anyopaque,
        GSGetShader: *anyopaque,
        IAGetPrimitiveTopology: *anyopaque,
        VSGetShaderResources: *anyopaque,
        VSGetSamplers: *anyopaque,
        GetPredication: *anyopaque,
        GSGetShaderResources: *anyopaque,
        GSGetSamplers: *anyopaque,
        OMGetRenderTargets: *anyopaque,
        OMGetRenderTargetsAndUnorderedAccessViews: *anyopaque,
        OMGetBlendState: *anyopaque,
        OMGetDepthStencilState: *anyopaque,
        SOGetTargets: *anyopaque,
        RSGetState: *anyopaque,
        RSGetViewports: *anyopaque,
        RSGetScissorRects: *anyopaque,
        HSGetShaderResources: *anyopaque,
        HSGetShader: *anyopaque,
        HSGetSamplers: *anyopaque,
        HSGetConstantBuffers: *anyopaque,
        DSGetShaderResources: *anyopaque,
        DSGetShader: *anyopaque,
        DSGetSamplers: *anyopaque,
        DSGetConstantBuffers: *anyopaque,
        CSGetShaderResources: *anyopaque,
        CSGetUnorderedAccessViews: *anyopaque,
        CSGetShader: *anyopaque,
        CSGetSamplers: *anyopaque,
        CSGetConstantBuffers: *anyopaque,
        ClearState: *anyopaque,
        Flush: *const fn (*T) callconv(WINAPI) void,
        GetType: *anyopaque,
        GetContextFlags: *anyopaque,
        FinishCommandList: *anyopaque,
    };
};

pub const IID_IDevice = GUID.parse("{db6f6ddb-ac77-4e88-8253-819df9bbf140}");
pub const IDevice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn CreateBuffer(
                self: *T,
                pDesc: *const BUFFER_DESC,
                pInitialData: ?*const SUBRESOURCE_DATA,
                ppBuffer: *?*IBuffer,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateBuffer(
                    @as(*IDevice, @ptrCast(self)),
                    pDesc,
                    pInitialData,
                    ppBuffer,
                );
            }
            pub inline fn CreateTexture2D(
                self: *T,
                pDesc: *const TEXTURE2D_DESC,
                pInitialData: ?*const SUBRESOURCE_DATA,
                ppTexture2D: ?*?*ITexture2D,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateTexture2D(
                    @as(*IDevice, @ptrCast(self)),
                    pDesc,
                    pInitialData,
                    ppTexture2D,
                );
            }
            pub inline fn CreateShaderResourceView(
                self: *T,
                pResource: *IResource,
                pDesc: ?*const SHADER_RESOURCE_VIEW_DESC,
                ppSRView: ?*?*IShaderResourceView,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateShaderResourceView(
                    @as(*IDevice, @ptrCast(self)),
                    pResource,
                    pDesc,
                    ppSRView,
                );
            }
            pub inline fn CreateRenderTargetView(
                self: *T,
                pResource: ?*IResource,
                pDesc: ?*const RENDER_TARGET_VIEW_DESC,
                ppRTView: ?*?*IRenderTargetView,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateRenderTargetView(
                    @as(*IDevice, @ptrCast(self)),
                    pResource,
                    pDesc,
                    ppRTView,
                );
            }
            pub inline fn CreateInputLayout(
                self: *T,
                pInputElementDescs: ?[*]const INPUT_ELEMENT_DESC,
                NumElements: UINT,
                pShaderBytecodeWithInputSignature: *const anyopaque,
                BytecodeLength: SIZE_T,
                ppInputLayout: *?*IInputLayout,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateInputLayout(
                    @as(*IDevice, @ptrCast(self)),
                    pInputElementDescs,
                    NumElements,
                    pShaderBytecodeWithInputSignature,
                    BytecodeLength,
                    ppInputLayout,
                );
            }
            pub inline fn CreateVertexShader(
                self: *T,
                pShaderBytecode: *const anyopaque,
                BytecodeLength: SIZE_T,
                pClassLinkage: ?*IClassLinkage,
                ppVertexShader: ?*?*IVertexShader,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateVertexShader(
                    @as(*IDevice, @ptrCast(self)),
                    pShaderBytecode,
                    BytecodeLength,
                    pClassLinkage,
                    ppVertexShader,
                );
            }
            pub inline fn CreatePixelShader(
                self: *T,
                pShaderBytecode: *const anyopaque,
                BytecodeLength: SIZE_T,
                pClassLinkage: ?*IClassLinkage,
                ppPixelShader: ?*?*IPixelShader,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreatePixelShader(
                    @as(*IDevice, @ptrCast(self)),
                    pShaderBytecode,
                    BytecodeLength,
                    pClassLinkage,
                    ppPixelShader,
                );
            }
            pub inline fn CreateBlendState(
                self: *T,
                pBlendStateDesc: *const BLEND_DESC,
                ppBlendState: ?*?*IBlendState,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v))
                    .CreateBlendState(@as(*IDevice, @ptrCast(self)), pBlendStateDesc, ppBlendState);
            }
            pub inline fn CreateRasterizerState(
                self: *T,
                pRasterizerDesc: *const RASTERIZER_DESC,
                ppRasterizerState: ?*?*IRasterizerState,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateRasterizerState(
                    @as(*IDevice, @ptrCast(self)),
                    pRasterizerDesc,
                    ppRasterizerState,
                );
            }
            pub inline fn CreateSamplerState(
                self: *T,
                pSamplerDesc: *const SAMPLER_DESC,
                ppSamplerState: ?*?*ISamplerState,
            ) HRESULT {
                return @as(*const IDevice.VTable, @ptrCast(self.__v)).CreateSamplerState(
                    @as(*IDevice, @ptrCast(self)),
                    pSamplerDesc,
                    ppSamplerState,
                );
            }
        };
    }

    pub const VTable = extern struct {
        const T = IDevice;
        base: IUnknown.VTable,
        CreateBuffer: *const fn (
            *T,
            *const BUFFER_DESC,
            ?*const SUBRESOURCE_DATA,
            *?*IBuffer,
        ) callconv(WINAPI) HRESULT,
        CreateTexture1D: *anyopaque,
        CreateTexture2D: *const fn (
            *T,
            *const TEXTURE2D_DESC,
            ?*const SUBRESOURCE_DATA,
            ?*?*ITexture2D,
        ) callconv(WINAPI) HRESULT,
        CreateTexture3D: *anyopaque,
        CreateShaderResourceView: *const fn (
            *T,
            *IResource,
            ?*const SHADER_RESOURCE_VIEW_DESC,
            ?*?*IShaderResourceView,
        ) callconv(WINAPI) HRESULT,
        CreateUnorderedAccessView: *anyopaque,
        CreateRenderTargetView: *const fn (
            *T,
            ?*IResource,
            ?*const RENDER_TARGET_VIEW_DESC,
            ?*?*IRenderTargetView,
        ) callconv(WINAPI) HRESULT,
        CreateDepthStencilView: *anyopaque,
        CreateInputLayout: *const fn (
            *T,
            ?[*]const INPUT_ELEMENT_DESC,
            UINT,
            *const anyopaque,
            SIZE_T,
            *?*IInputLayout,
        ) callconv(WINAPI) HRESULT,
        CreateVertexShader: *const fn (
            *T,
            ?*const anyopaque,
            SIZE_T,
            ?*IClassLinkage,
            ?*?*IVertexShader,
        ) callconv(WINAPI) HRESULT,
        CreateGeometryShader: *anyopaque,
        CreateGeometryShaderWithStreamOutput: *anyopaque,
        CreatePixelShader: *const fn (
            *T,
            ?*const anyopaque,
            SIZE_T,
            ?*IClassLinkage,
            ?*?*IPixelShader,
        ) callconv(WINAPI) HRESULT,
        CreateHullShader: *anyopaque,
        CreateDomainShader: *anyopaque,
        CreateComputeShader: *anyopaque,
        CreateClassLinkage: *anyopaque,
        CreateBlendState: *const fn (
            *T,
            *const BLEND_DESC,
            ?*?*IBlendState,
        ) callconv(WINAPI) HRESULT,
        CreateDepthStencilState: *anyopaque,
        CreateRasterizerState: *const fn (
            *T,
            *const RASTERIZER_DESC,
            ?*?*IRasterizerState,
        ) callconv(WINAPI) HRESULT,
        CreateSamplerState: *const fn (
            *T,
            *const SAMPLER_DESC,
            ?*?*ISamplerState,
        ) callconv(WINAPI) HRESULT,
        CreateQuery: *anyopaque,
        CreatePredicate: *anyopaque,
        CreateCounter: *anyopaque,
        CreateDeferredContext: *anyopaque,
        OpenSharedResource: *anyopaque,
        CheckFormatSupport: *anyopaque,
        CheckMultisampleQualityLevels: *anyopaque,
        CheckCounterInfo: *anyopaque,
        CheckCounter: *anyopaque,
        CheckFeatureSupport: *anyopaque,
        GetPrivateData: *anyopaque,
        SetPrivateData: *anyopaque,
        SetPrivateDataInterface: *anyopaque,
        GetFeatureLevel: *anyopaque,
        GetCreationFlags: *anyopaque,
        GetDeviceRemovedReason: *anyopaque,
        GetImmediateContext: *anyopaque,
        SetExceptionMode: *anyopaque,
        GetExceptionMode: *anyopaque,
    };
};

pub const IID_IView = GUID.parse("{839d1216-bb2e-412b-b7f4-a9dbebe08ed1}");
pub const IView = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetResource: *anyopaque,
    };
};

pub const IID_IRenderTargetView = GUID.parse("{dfdba067-0b8d-4865-875b-d7b4516cc164}");
pub const IRenderTargetView = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IView.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IView.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IDepthStencilView = GUID.parse("{9fdac92a-1876-48c3-afad-25b94f84a9b6}");
pub const IDepthStencilView = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IView.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IView.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IShaderResourceView = GUID.parse("{b0e06fe0-8192-4e1a-b1ca-36d7414710b2}");
pub const IShaderResourceView = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IView.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IView.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IVertexShader = GUID.parse("{3b301d64-d678-4289-8897-22f8928b72f3}");
pub const IVertexShader = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IPixelShader = GUID.parse("{ea82e40d-51dc-4f33-93d4-db7c9125ae8c}");
pub const IPixelShader = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IInputLayout = GUID.parse("{e4819ddc-4cf0-4025-bd26-5de82a3e07b7}");
pub const IInputLayout = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IRasterizerState = GUID.parse("{9bb4ab81-ab1a-4d8f-b506-fc04200b6ee7}");
pub const IRasterizerState = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_BlendState = GUID.parse("{75b68faa-347d-4159-8f45-a0640f01cd9a}");
pub const IBlendState = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_SamplerState = GUID.parse("{da6fea51-564c-4487-9810-f0d0f9b4e3a5}");
pub const ISamplerState = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_IBuffer = GUID.parse("{48570b85-d1ee-4fcd-a250-eb350722b037}");
pub const IBuffer = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetDesc: *anyopaque,
    };
};

pub const IID_ITexture2D = GUID.parse("{6f15aaf2-d208-4e89-9ab4-489535d34f9c}");
pub const ITexture2D = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IResource.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IResource.VTable,
        GetDesc: *anyopaque,
    };
};

pub extern "d3d11" fn D3D11CreateDeviceAndSwapChain(
    pAdapter: ?*dxgi.IAdapter,
    DriverType: DRIVER_TYPE,
    Software: ?HINSTANCE,
    Flags: CREATE_DEVICE_FLAG,
    pFeatureLevels: ?[*]const FEATURE_LEVEL,
    FeatureLevels: UINT,
    SDKVersion: UINT,
    pSwapChainDesc: ?*const dxgi.SWAP_CHAIN_DESC,
    ppSwapChain: ?*?*dxgi.ISwapChain,
    ppDevice: ?*?*IDevice,
    pFeatureLevel: ?*FEATURE_LEVEL,
    ppImmediateContext: ?*?*IDeviceContext,
) callconv(WINAPI) HRESULT;

// Return codes as defined here:
// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d11-graphics-reference-returnvalues
pub const ERROR_FILE_NOT_FOUND = @as(HRESULT, @bitCast(@as(c_ulong, 0x887C0002)));
pub const ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS = @as(HRESULT, @bitCast(@as(c_ulong, 0x887C0001)));
pub const ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS = @as(HRESULT, @bitCast(@as(c_ulong, 0x887C0003)));
pub const ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD = @as(HRESULT, @bitCast(@as(c_ulong, 0x887C0004)));

// error set corresponding to the above return codes
pub const Error = error{
    FILE_NOT_FOUND,
    TOO_MANY_UNIQUE_STATE_OBJECTS,
    TOO_MANY_UNIQUE_VIEW_OBJECTS,
    DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
};
