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

pub const BUFFER_DESC = extern struct {
    ByteWidth: UINT,
    Usage: USAGE,
    BindFlags: BIND_FLAG,
    CPUAccessFlags: UINT = 0,
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
            pub inline fn Flush(self: *T) void {
                self.v.devctx.Flush(self);
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
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            VSSetConstantBuffers: *anyopaque,
            PSSetShaderResources: *anyopaque,
            PSSetShader: *anyopaque,
            PSSetSamplers: *anyopaque,
            VSSetShader: *anyopaque,
            DrawIndexed: *anyopaque,
            Draw: *anyopaque,
            Map: *anyopaque,
            Unmap: *anyopaque,
            PSSetConstantBuffers: *anyopaque,
            IASetInputLayout: *anyopaque,
            IASetVertexBuffers: *anyopaque,
            IASetIndexBuffer: *anyopaque,
            DrawIndexedInstanced: *anyopaque,
            DrawInstanced: *anyopaque,
            GSSetConstantBuffers: *anyopaque,
            GSSetShader: *anyopaque,
            IASetPrimitiveTopology: *anyopaque,
            VSSetShaderResources: *anyopaque,
            VSSetSamplers: *anyopaque,
            Begin: *anyopaque,
            End: *anyopaque,
            GetData: *anyopaque,
            SetPredication: *anyopaque,
            GSSetShaderResources: *anyopaque,
            GSSetSamplers: *anyopaque,
            OMSetRenderTargets: *anyopaque,
            OMSetRenderTargetsAndUnorderedAccessViews: *anyopaque,
            OMSetBlendState: *anyopaque,
            OMSetDepthStencilState: *anyopaque,
            SOSetTargets: *anyopaque,
            DrawAuto: *anyopaque,
            DrawIndexedInstancedIndirect: *anyopaque,
            DrawInstancedIndirect: *anyopaque,
            Dispatch: *anyopaque,
            DispatchIndirect: *anyopaque,
            RSSetState: *anyopaque,
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
            CreateTexture2D: *anyopaque,
            CreateTexture3D: *anyopaque,
            CreateShaderResourceView: *anyopaque,
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
            CreateBlendState: *anyopaque,
            CreateDepthStencilState: *anyopaque,
            CreateRasterizerState: *anyopaque,
            CreateSamplerState: *anyopaque,
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
