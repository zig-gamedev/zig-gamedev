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

pub const CREATE_DEVICE_FLAG = UINT;
pub const CREATE_DEVICE_SINGLETHREADED = 0x1;
pub const CREATE_DEVICE_DEBUG = 0x2;
pub const CREATE_DEVICE_SWITCH_TO_REF = 0x4;
pub const CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8;
pub const CREATE_DEVICE_BGRA_SUPPORT = 0x20;
pub const CREATE_DEVICE_DEBUGGABLE = 0x40;
pub const CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80;
pub const CREATE_DEVICE_DISABLE_GPU_TIMEOUT = 0x100;
pub const CREATE_DEVICE_VIDEO_SUPPORT = 0x800;

pub const SDK_VERSION: UINT = 7;

pub const BIND_FLAG = UINT;
pub const BIND_VERTEX_BUFFER = 0x1;
pub const BIND_INDEX_BUFFER = 0x2;
pub const BIND_CONSTANT_BUFFER = 0x4;
pub const BIND_SHADER_RESOURCE = 0x8;
pub const BIND_STREAM_OUTPUT = 0x10;
pub const BIND_RENDER_TARGET = 0x20;
pub const BIND_DEPTH_STENCIL = 0x40;
pub const BIND_UNORDERED_ACCESS = 0x80;
pub const BIND_DECODER = 0x200;
pub const BIND_VIDEO_ENCODER = 0x400;

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
    INPUT_PER_INSTNACE_DATA = 1,
};

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

pub const USAGE = UINT;
pub const USAGE_DEFAULT = 0;
pub const USAGE_IMMUTABLE = 1;
pub const USAGE_DYNAMIC = 2;
pub const USAGE_STAGING = 3;

pub const CPU_ACCCESS_FLAG = UINT;
pub const CPU_ACCESS_WRITE = 0x10000;
pub const CPU_ACCESS_READ = 0x20000;

pub const BUFFER_DESC = extern struct {
    ByteWidth: UINT,
    Usage: USAGE,
    BindFlags: BIND_FLAG,
    CPUAccessFlags: CPU_ACCCESS_FLAG = 0,
    MiscFlags: UINT = 0,
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

pub const MAP_FLAG = UINT;
pub const MAP_FLAG_DO_NOT_WAIT = 0x100000;

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
    FillMode: FILL_MODE = FILL_MODE.SOLID,
    CullMode: CULL_MODE = CULL_MODE.BACK,
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

pub const COLOR_WRITE_ENABLE = UINT;
pub const COLOR_WRITE_ENABLE_RED = 1;
pub const COLOR_WRITE_ENABLE_GREEN = 2;
pub const COLOR_WRITE_ENABLE_BLUE = 4;
pub const COLOR_WRITE_ENABLE_ALPHA = 8;
pub const COLOR_WRITE_ENABLE_ALL = COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN | COLOR_WRITE_ENABLE_BLUE | COLOR_WRITE_ENABLE_ALPHA;

pub const RENDER_TARGET_BLEND_DESC = extern struct {
    BlendEnable: BOOL,
    SrcBlend: BLEND,
    DestBlend: BLEND,
    BlendOp: BLEND_OP,
    SrcBlendAlpha: BLEND,
    DestBlendAlpha: BLEND,
    BlendOpAlpha: BLEND_OP,
    RenderTargetWriteMask: UINT8,
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
    MiscFlags: UINT,
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

pub const BUFFEREX_SRV_FLAG = UINT;
pub const BUFFEREX_SRV_FLAG_RAW = 0x1;

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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDevice: *anyopaque,
            GetPrivateData: *anyopaque,
            SetPrivateData: *anyopaque,
            SetPrivateDataInterface: *anyopaque,
        };
    }
};

pub const IID_IClassLinkage = GUID.parse("{ddf57cba-9543-46e4-a12b-f207a0fe7fed}");
pub const IClassLinkage = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        classlinkage: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetClassInstance: *anyopaque,
            CreateClassInstance: *anyopaque,
        };
    }
};

pub const IID_IClassInstance = GUID.parse("{a6cd7faa-b0b7-4a2f-9436-8662a65797cb}");
pub const IClassInstance = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        classinst: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetClassLinkage: *anyopaque,
            GetDesc: *anyopaque,
            GetInstanceName: *anyopaque,
            GetTypeName: *anyopaque,
        };
    }
};

pub const IID_IResource = GUID.parse("{dc8e63f3-d12b-4952-b47b-5e45026a862d}");
pub const IResource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetType: *anyopaque,
            SetEvictionPriority: *anyopaque,
            GetEvictionPriority: *anyopaque,
        };
    }
};

pub const IID_IDeviceContext = GUID.parse("{c0bfa96c-e089-44fb-8eaf-26f8796190da}");
pub const IDeviceContext = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        devctx: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn VSSetConstantBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppConstantBuffers: ?[*]const *IBuffer,
            ) void {
                self.v.devctx.VSSetConstantBuffers(
                    self,
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
                self.v.devctx.PSSetShaderResources(
                    self,
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
                self.v.devctx.PSSetShader(
                    self,
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
                self.v.devctx.PSSetSamplers(
                    self,
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
                self.v.devctx.VSSetShader(
                    self,
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
                self.v.devctx.Draw(self, VertexCount, StartVertexLocation);
            }
            pub inline fn Map(
                self: *T,
                pResource: *IResource,
                Subresource: UINT,
                MapType: MAP,
                MapFlags: MAP_FLAG,
                pMappedResource: ?*MAPPED_SUBRESOURCE,
            ) HRESULT {
                return self.v.devctx.Map(
                    self,
                    pResource,
                    Subresource,
                    MapType,
                    MapFlags,
                    pMappedResource,
                );
            }
            pub inline fn Unmap(self: *T, pResource: *IResource, Subresource: UINT) void {
                self.v.devctx.Unmap(self, pResource, Subresource);
            }
            pub inline fn PSSetConstantBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppConstantBuffers: ?[*]const *IBuffer,
            ) void {
                self.v.devctx.PSSetConstantBuffers(
                    self,
                    StartSlot,
                    NumBuffers,
                    ppConstantBuffers,
                );
            }
            pub inline fn IASetInputLayout(self: *T, pInputLayout: ?*IInputLayout) void {
                self.v.devctx.IASetInputLayout(self, pInputLayout);
            }
            pub inline fn IASetVertexBuffers(
                self: *T,
                StartSlot: UINT,
                NumBuffers: UINT,
                ppVertexBuffers: ?[*]const *IBuffer,
                pStrides: ?[*]const UINT,
                pOffsets: ?[*]const UINT,
            ) void {
                self.v.devctx.IASetVertexBuffers(
                    self,
                    StartSlot,
                    NumBuffers,
                    ppVertexBuffers,
                    pStrides,
                    pOffsets,
                );
            }
            pub inline fn IASetPrimitiveTopology(self: *T, Topology: PRIMITIVE_TOPOLOGY) void {
                self.v.devctx.IASetPrimitiveTopology(self, Topology);
            }
            pub inline fn VSSetShaderResources(
                self: *T,
                StartSlot: UINT,
                NumViews: UINT,
                ppShaderResourceViews: ?[*]const *IShaderResourceView,
            ) void {
                self.v.devctx.VSSetShaderResources(
                    self,
                    StartSlot,
                    NumViews,
                    ppShaderResourceViews,
                );
            }
            pub inline fn OMSetRenderTargets(
                self: *T,
                NumViews: UINT,
                ppRenderTargetViews: ?[*]const IRenderTargetView,
                pDepthStencilView: ?*IDepthStencilView,
            ) void {
                self.v.devctx.OMSetRenderTargets(
                    self,
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
                self.v.devctx.OMSetBlendState(
                    self,
                    pBlendState,
                    BlendFactor,
                    SampleMask,
                );
            }
            pub inline fn RSSetState(self: *T, pRasterizerState: ?*IRasterizerState) void {
                self.v.devctx.RSSetState(self, pRasterizerState);
            }
            pub inline fn RSSetViewports(
                self: *T,
                NumViewports: UINT,
                pViewports: [*]const VIEWPORT,
            ) void {
                self.v.devctx.RSSetViewports(self, NumViewports, pViewports);
            }
            pub inline fn ClearRenderTargetView(
                self: *T,
                pRenderTargetView: *IRenderTargetView,
                ColorRGBA: *const [4]FLOAT,
            ) void {
                self.v.devctx.ClearRenderTargetView(self, pRenderTargetView, ColorRGBA);
            }
            pub inline fn Flush(self: *T) void {
                self.v.devctx.Flush(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            VSSetConstantBuffers: fn (
                *T,
                UINT,
                UINT,
                ?[*]const *IBuffer,
            ) callconv(WINAPI) void,
            PSSetShaderResources: fn (
                *T,
                UINT,
                UINT,
                ?[*]const *IShaderResourceView,
            ) callconv(WINAPI) void,
            PSSetShader: fn (
                *T,
                ?*IPixelShader,
                ?[*]const *IClassInstance,
                UINT,
            ) callconv(WINAPI) void,
            PSSetSamplers: fn (
                *T,
                UINT,
                UINT,
                ?[*]const *ISamplerState,
            ) callconv(WINAPI) void,
            VSSetShader: fn (
                *T,
                ?*IVertexShader,
                ?[*]const *IClassInstance,
                UINT,
            ) callconv(WINAPI) void,
            DrawIndexed: *anyopaque,
            Draw: fn (*T, UINT, UINT) callconv(WINAPI) void,
            Map: fn (
                *T,
                *IResource,
                UINT,
                MAP,
                MAP_FLAG,
                ?*MAPPED_SUBRESOURCE,
            ) callconv(WINAPI) HRESULT,
            Unmap: fn (*T, *IResource, UINT) callconv(WINAPI) void,
            PSSetConstantBuffers: fn (
                *T,
                UINT,
                UINT,
                ?[*]const *IBuffer,
            ) callconv(WINAPI) void,
            IASetInputLayout: fn (*T, ?*IInputLayout) callconv(WINAPI) void,
            IASetVertexBuffers: fn (
                *T,
                UINT,
                UINT,
                ?[*]const *IBuffer,
                ?[*]const UINT,
                ?[*]const UINT,
            ) callconv(WINAPI) void,
            IASetIndexBuffer: *anyopaque,
            DrawIndexedInstanced: *anyopaque,
            DrawInstanced: *anyopaque,
            GSSetConstantBuffers: *anyopaque,
            GSSetShader: *anyopaque,
            IASetPrimitiveTopology: fn (*T, PRIMITIVE_TOPOLOGY) callconv(WINAPI) void,
            VSSetShaderResources: fn (
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
            OMSetRenderTargets: fn (
                *T,
                UINT,
                ?[*]const IRenderTargetView,
                ?*IDepthStencilView,
            ) callconv(WINAPI) void,
            OMSetRenderTargetsAndUnorderedAccessViews: *anyopaque,
            OMSetBlendState: fn (
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
            RSSetState: fn (*T, ?*IRasterizerState) callconv(WINAPI) void,
            RSSetViewports: fn (*T, UINT, [*]const VIEWPORT) callconv(WINAPI) void,
            RSSetScissorRects: *anyopaque,
            CopySubresourceRegion: *anyopaque,
            CopyResource: *anyopaque,
            UpdateSubresource: *anyopaque,
            CopyStructureCount: *anyopaque,
            ClearRenderTargetView: fn (*T, *IRenderTargetView, *const [4]FLOAT) callconv(WINAPI) void,
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
            Flush: fn (*T) callconv(WINAPI) void,
            GetType: *anyopaque,
            GetContextFlags: *anyopaque,
            FinishCommandList: *anyopaque,
        };
    }
};

pub const IID_IDevice = GUID.parse("{db6f6ddb-ac77-4e88-8253-819df9bbf140}");
pub const IDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateBuffer(
                self: *T,
                pDesc: *const BUFFER_DESC,
                pInitialData: ?*const SUBRESOURCE_DATA,
                ppBuffer: *?*IBuffer,
            ) HRESULT {
                return self.v.device.CreateBuffer(
                    self,
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
                return self.v.device.CreateTexture2D(
                    self,
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
                return self.v.device.CreateShaderResourceView(
                    self,
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
                return self.v.device.CreateRenderTargetView(
                    self,
                    pResource,
                    pDesc,
                    ppRTView,
                );
            }
            pub inline fn CreateInputLayout(
                self: *T,
                pInputElementDescs: *const INPUT_ELEMENT_DESC,
                NumElements: UINT,
                pShaderBytecodeWithInputSignature: *anyopaque,
                BytecodeLength: SIZE_T,
                ppInputLayout: *?*IInputLayout,
            ) HRESULT {
                return self.v.device.CreateInputLayout(
                    self,
                    pInputElementDescs,
                    NumElements,
                    pShaderBytecodeWithInputSignature,
                    BytecodeLength,
                    ppInputLayout,
                );
            }
            pub inline fn CreateVertexShader(
                self: *T,
                pShaderBytecode: *anyopaque,
                BytecodeLength: SIZE_T,
                pClassLinkage: ?*IClassLinkage,
                ppVertexShader: ?*?*IVertexShader,
            ) HRESULT {
                return self.v.device.CreateVertexShader(
                    self,
                    pShaderBytecode,
                    BytecodeLength,
                    pClassLinkage,
                    ppVertexShader,
                );
            }
            pub inline fn CreatePixelShader(
                self: *T,
                pShaderBytecode: *anyopaque,
                BytecodeLength: SIZE_T,
                pClassLinkage: ?*IClassLinkage,
                ppPixelShader: ?*?*IPixelShader,
            ) HRESULT {
                return self.v.device.CreatePixelShader(
                    self,
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
                return self.v.device.CreateBlendState(self, pBlendStateDesc, ppBlendState);
            }
            pub inline fn CreateRasterizerState(
                self: *T,
                pRasterizerDesc: *const RASTERIZER_DESC,
                ppRasterizerState: ?*?*IRasterizerState,
            ) HRESULT {
                return self.v.device.CreateRasterizerState(
                    self,
                    pRasterizerDesc,
                    ppRasterizerState,
                );
            }
            pub inline fn CreateSamplerState(
                self: *T,
                pSamplerDesc: *const SAMPLER_DESC,
                ppSamplerState: ?*?*ISamplerState,
            ) HRESULT {
                return self.v.device.CreateSamplerState(
                    self,
                    pSamplerDesc,
                    ppSamplerState,
                );
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateBuffer: fn (
                *T,
                *const BUFFER_DESC,
                ?*const SUBRESOURCE_DATA,
                *?*IBuffer,
            ) callconv(WINAPI) HRESULT,
            CreateTexture1D: *anyopaque,
            CreateTexture2D: fn (
                *T,
                *const TEXTURE2D_DESC,
                ?*const SUBRESOURCE_DATA,
                ?*?*ITexture2D,
            ) callconv(WINAPI) HRESULT,
            CreateTexture3D: *anyopaque,
            CreateShaderResourceView: fn (
                *T,
                *IResource,
                ?*const SHADER_RESOURCE_VIEW_DESC,
                ?*?*IShaderResourceView,
            ) callconv(WINAPI) HRESULT,
            CreateUnorderedAccessView: *anyopaque,
            CreateRenderTargetView: fn (
                *T,
                ?*IResource,
                ?*const RENDER_TARGET_VIEW_DESC,
                ?*?*IRenderTargetView,
            ) callconv(WINAPI) HRESULT,
            CreateDepthStencilView: *anyopaque,
            CreateInputLayout: fn (
                *T,
                *const INPUT_ELEMENT_DESC,
                UINT,
                *anyopaque,
                SIZE_T,
                *?*IInputLayout,
            ) callconv(WINAPI) HRESULT,
            CreateVertexShader: fn (
                *T,
                ?*anyopaque,
                SIZE_T,
                ?*IClassLinkage,
                ?*?*IVertexShader,
            ) callconv(WINAPI) HRESULT,
            CreateGeometryShader: *anyopaque,
            CreateGeometryShaderWithStreamOutput: *anyopaque,
            CreatePixelShader: fn (
                *T,
                ?*anyopaque,
                SIZE_T,
                ?*IClassLinkage,
                ?*?*IPixelShader,
            ) callconv(WINAPI) HRESULT,
            CreateHullShader: *anyopaque,
            CreateDomainShader: *anyopaque,
            CreateComputeShader: *anyopaque,
            CreateClassLinkage: *anyopaque,
            CreateBlendState: fn (
                *T,
                *const BLEND_DESC,
                ?*?*IBlendState,
            ) callconv(WINAPI) HRESULT,
            CreateDepthStencilState: *anyopaque,
            CreateRasterizerState: fn (
                *T,
                *const RASTERIZER_DESC,
                ?*?*IRasterizerState,
            ) callconv(WINAPI) HRESULT,
            CreateSamplerState: fn (
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
    }
};

pub const IID_IView = GUID.parse("{839d1216-bb2e-412b-b7f4-a9dbebe08ed1}");
pub const IView = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        view: VTable(Self),
    },

    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetResource: *anyopaque,
        };
    }
};

pub const IID_IRenderTargetView = GUID.parse("{dfdba067-0b8d-4865-875b-d7b4516cc164}");
pub const IRenderTargetView = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        view: IView.VTable(Self),
        rendertargetview: VTable(Self),
    },

    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IView.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_IDepthStencilView = GUID.parse("{9fdac92a-1876-48c3-afad-25b94f84a9b6}");
pub const IDepthStencilView = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        view: IView.VTable(Self),
        dsv: VTable(Self),
    },

    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IView.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_IShaderResourceView = GUID.parse("{b0e06fe0-8192-4e1a-b1ca-36d7414710b}");
pub const IShaderResourceView = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        view: IView.VTable(Self),
        shader_res_view: VTable(Self),
    },

    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IView.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_IVertexShader = GUID("{3b301d64-d678-4289-8897-22f8928b72f3}");
pub const IVertexShader = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        vertexshader: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
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

pub const IID_IPixelShader = GUID("{ea82e40d-51dc-4f33-93d4-db7c9125ae8c}");
pub const IPixelShader = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pixelshader: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
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

pub const IID_IInputLayout = GUID.parse("{e4819ddc-4cf0-4025-bd26-5de82a3e07b7}");
pub const IInputLayout = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        inputlayout: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
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

pub const IID_IRasterizerState = GUID.parse("{9bb4ab81-ab1a-4d8f-b506-fc04200b6ee7}");
pub const IRasterizerState = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        state: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_BlendState = GUID.parse("{75b68faa-347d-4159-8f45-a0640f01cd9a}");
pub const IBlendState = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        state: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_SamplerState = GUID.parse("{da6fea51-564c-4487-9810-f0d0f9b4e3a5}");
pub const ISamplerState = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        state: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.VTable(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_IBuffer = GUID.parse("{48570b85-d1ee-4fcd-a250-eb350722b037}");
pub const IBuffer = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        resource: IResource.VTable(Self),
        buffer: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
};

pub const IID_ITexture2D = GUID.parse("{6f15aaf2-d208-4e89-9ab4-489535d34f9c}");
pub const ITexture2D = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        resource: IResource.VTable(Self),
        texture: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IResource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetDesc: *anyopaque,
        };
    }
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

// Return codes as defined here: https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d11-graphics-reference-returnvalues
pub const ERROR_FILE_NOT_FOUND = @bitCast(HRESULT, @as(c_ulong, 0x887C0002));
pub const ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS = @bitCast(HRESULT, @as(c_ulong, 0x887C0001));
pub const ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS = @bitCast(HRESULT, @as(c_ulong, 0x887C0003));
pub const ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD = @bitCast(HRESULT, @as(c_ulong, 0x887C0004));

// error set corresponding to the above return codes
pub const Error = error{
    FILE_NOT_FOUND,
    TOO_MANY_UNIQUE_STATE_OBJECTS,
    TOO_MANY_UNIQUE_VIEW_OBJECTS,
    DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
};
