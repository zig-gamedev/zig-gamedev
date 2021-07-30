const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("dxgitype.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgi.zig");
usingnamespace @import("dxgi1_2.zig");
usingnamespace @import("dxgi1_3.zig");

pub const IDXGISwapChain3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        swapchain: IDXGISwapChain.VTable(Self),
        swapchain1: IDXGISwapChain1.VTable(Self),
        swapchain2: IDXGISwapChain2.VTable(Self),
        swapchain3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace IDXGISwapChain.Methods(Self);
    usingnamespace IDXGISwapChain1.Methods(Self);
    usingnamespace IDXGISwapChain2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetCurrentBackBufferIndex(self: *T) UINT {
                return self.v.swapchain3.GetCurrentBackBufferIndex(self);
            }
            pub inline fn CheckColorSpaceSupport(self: *T, space: DXGI_COLOR_SPACE_TYPE, support: *UINT) HRESULT {
                return self.v.swapchain3.CheckColorSpaceSupport(self, space, support);
            }
            pub inline fn SetColorSpace1(self: *T, space: DXGI_COLOR_SPACE_TYPE) HRESULT {
                return self.v.swapchain3.SetColorSpace1(self, space);
            }
            pub inline fn ResizeBuffers1(
                self: *T,
                buffer_count: UINT,
                width: UINT,
                height: UINT,
                format: DXGI_FORMAT,
                swap_chain_flags: UINT,
                creation_node_mask: [*]const UINT,
                present_queue: [*]const *IUnknown,
            ) HRESULT {
                return self.v.swapchain3.ResizeBuffers1(
                    self,
                    buffer_count,
                    width,
                    height,
                    format,
                    swap_chain_flags,
                    creation_node_mask,
                    present_queue,
                );
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetCurrentBackBufferIndex: fn (*T) callconv(WINAPI) UINT,
            CheckColorSpaceSupport: fn (*T, DXGI_COLOR_SPACE_TYPE, *UINT) callconv(WINAPI) HRESULT,
            SetColorSpace1: fn (*T, DXGI_COLOR_SPACE_TYPE) callconv(WINAPI) HRESULT,
            ResizeBuffers1: fn (
                *T,
                UINT,
                UINT,
                UINT,
                DXGI_FORMAT,
                UINT,
                [*]const UINT,
                [*]const *IUnknown,
            ) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IDXGISwapChain3 = GUID{
    .Data1 = 0x94d99bdb,
    .Data2 = 0xf1f8,
    .Data3 = 0x4ab0,
    .Data4 = .{ 0xb2, 0x36, 0x7d, 0xa0, 0x17, 0x0e, 0xda, 0xb1 },
};
