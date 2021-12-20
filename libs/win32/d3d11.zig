const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;

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
            RSSetViewports: *anyopaque,
            RSSetScissorRects: *anyopaque,
            CopySubresourceRegion: *anyopaque,
            CopyResource: *anyopaque,
            UpdateSubresource: *anyopaque,
            CopyStructureCount: *anyopaque,
            ClearRenderTargetView: *anyopaque,
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

pub const IDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        device: VTable(Self),
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
            CreateBuffer: *anyopaque,
            CreateTexture1D: *anyopaque,
            CreateTexture2D: *anyopaque,
            CreateTexture3D: *anyopaque,
            CreateShaderResourceView: *anyopaque,
            CreateUnorderedAccessView: *anyopaque,
            CreateRenderTargetView: *anyopaque,
            CreateDepthStencilView: *anyopaque,
            CreateInputLayout: *anyopaque,
            CreateVertexShader: *anyopaque,
            CreateGeometryShader: *anyopaque,
            CreateGeometryShaderWithStreamOutput: *anyopaque,
            CreatePixelShader: *anyopaque,
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

pub const IID_IResource = GUID{
    .Data1 = 0xdc8e63f3,
    .Data2 = 0xd12b,
    .Data3 = 0x4952,
    .Data4 = .{ 0xb4, 0x7b, 0x5e, 0x45, 0x02, 0x6a, 0x86, 0x2d },
};
