const RESOURCE_STATES = @import("d3d12.zig").RESOURCE_STATES;
const windows = @import("windows.zig");
const d3d = @import("d3dcommon.zig");
const d3d11 = @import("d3d11.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;

pub const RESOURCE_FLAGS = extern struct {
    BindFlags: UINT,
    MiscFlags: UINT,
    CPUAccessFlags: UINT,
    StructureByteStride: UINT,
};

pub const IDevice = extern struct {
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
                flags11: *const RESOURCE_FLAGS,
                in_state: RESOURCE_STATES,
                out_state: RESOURCE_STATES,
                guid: *const GUID,
                resource11: ?*?*anyopaque,
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
                resources: [*]const *d3d11.IResource,
                num_resources: UINT,
            ) void {
                self.v.on12dev.ReleaseWrappedResources(self, resources, num_resources);
            }
            pub inline fn AcquireWrappedResources(
                self: *T,
                resources: [*]const *d3d11.IResource,
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
                *const RESOURCE_FLAGS,
                RESOURCE_STATES,
                RESOURCE_STATES,
                *const GUID,
                ?*?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            ReleaseWrappedResources: fn (*T, [*]const *d3d11.IResource, UINT) callconv(WINAPI) void,
            AcquireWrappedResources: fn (*T, [*]const *d3d11.IResource, UINT) callconv(WINAPI) void,
        };
    }
};

pub const IDevice1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        on12dev: IDevice.VTable(Self),
        on12dev1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetD3D12Device: *anyopaque,
        };
    }
};

pub const IDevice2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        on12dev: IDevice.VTable(Self),
        on12dev1: IDevice1.VTable(Self),
        on12dev2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace IDevice1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            UnwrapUnderlyingResource: *anyopaque,
            ReturnUnderlyingResource: *anyopaque,
        };
    }
};

pub const IID_IDevice2 = GUID{
    .Data1 = 0xdc90f331,
    .Data2 = 0x4740,
    .Data3 = 0x43fa,
    .Data4 = .{ 0x86, 0x6e, 0x67, 0xf1, 0x2c, 0xb5, 0x82, 0x23 },
};

pub extern "d3d11" fn D3D11On12CreateDevice(
    device12: *IUnknown,
    flags11: d3d11.CREATE_DEVICE_FLAG,
    feature_levels: ?[*]const d3d.FEATURE_LEVEL,
    num_feature_levels: UINT,
    cmd_queues: [*]const *IUnknown,
    num_cmd_queues: UINT,
    node_mask: UINT,
    device11: ?*?*d3d11.IDevice,
    device_ctx11: ?*?*d3d11.IDeviceContext,
    ?*d3d.FEATURE_LEVEL,
) callconv(WINAPI) HRESULT;
