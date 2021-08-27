const std = @import("std");
const D3D12_RESOURCE_STATES = @import("d3d12.zig").D3D12_RESOURCE_STATES;
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
const d3d = @import("d3dcommon.zig");
usingnamespace @import("d3d11.zig");

pub const D3D11_RESOURCE_FLAGS = extern struct {
    BindFlags: UINT,
    MiscFlags: UINT,
    CPUAccessFlags: UINT,
    StructureByteStride: UINT,
};

pub const ID3D11On12Device = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        on12dev: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CreateWrappedResource(
                self: *T,
                resource12: *IUnknown,
                flags11: *const D3D11_RESOURCE_FLAGS,
                in_state: D3D12_RESOURCE_STATES,
                out_state: D3D12_RESOURCE_STATES,
                guid: *const GUID,
                resource11: ?*?*c_void,
            ) HRESULT {
                return self.v.on12dev.CreateWrappedResource(
                    self,
                    resource12,
                    flags11,
                    in_state,
                    out_state,
                    guid,
                    resource11,
                );
            }
            pub inline fn ReleaseWrappedResources(
                self: *T,
                resources: [*]const *ID3D11Resource,
                num_resources: UINT,
            ) void {
                self.v.on12dev.ReleaseWrappedResources(self, resources, num_resources);
            }
            pub inline fn AcquireWrappedResources(
                self: *T,
                resources: [*]const *ID3D11Resource,
                num_resources: UINT,
            ) void {
                self.v.on12dev.AcquireWrappedResources(self, resources, num_resources);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            CreateWrappedResource: fn (
                *T,
                *IUnknown,
                *const D3D11_RESOURCE_FLAGS,
                D3D12_RESOURCE_STATES,
                D3D12_RESOURCE_STATES,
                *const GUID,
                ?*?*c_void,
            ) callconv(WINAPI) HRESULT,
            ReleaseWrappedResources: fn (*T, [*]const *ID3D11Resource, UINT) callconv(WINAPI) void,
            AcquireWrappedResources: fn (*T, [*]const *ID3D11Resource, UINT) callconv(WINAPI) void,
        };
    }
};

pub const ID3D11On12Device1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        on12dev: ID3D11On12Device.VTable(Self),
        on12dev1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D11On12Device.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetD3D12Device: *c_void,
        };
    }
};

pub const ID3D11On12Device2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        on12dev: ID3D11On12Device.VTable(Self),
        on12dev1: ID3D11On12Device1.VTable(Self),
        on12dev2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID3D11On12Device.Methods(Self);
    usingnamespace ID3D11On12Device1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            UnwrapUnderlyingResource: *c_void,
            ReturnUnderlyingResource: *c_void,
        };
    }
};

pub const IID_ID3D11On12Device2 = GUID{
    .Data1 = 0xdc90f331,
    .Data2 = 0x4740,
    .Data3 = 0x43fa,
    .Data4 = .{ 0x86, 0x6e, 0x67, 0xf1, 0x2c, 0xb5, 0x82, 0x23 },
};

pub extern "d3d11" fn D3D11On12CreateDevice(
    device12: *IUnknown,
    flags11: D3D11_CREATE_DEVICE_FLAG,
    feature_levels: ?[*]const d3d.FEATURE_LEVEL,
    num_feature_levels: UINT,
    cmd_queues: [*]const *IUnknown,
    num_cmd_queues: UINT,
    node_mask: UINT,
    device11: ?*?*ID3D11Device,
    device_ctx11: ?*?*ID3D11DeviceContext,
    ?*d3d.FEATURE_LEVEL,
) callconv(WINAPI) HRESULT;
