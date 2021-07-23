const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("win.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("dxgitype.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgi.zig");
usingnamespace @import("dxgi1_2.zig");

pub const DXGI_MATRIX_3X2_F = extern struct {
    _11: FLOAT,
    _12: FLOAT,
    _21: FLOAT,
    _22: FLOAT,
    _31: FLOAT,
    _32: FLOAT,
};

pub const IDXGISwapChain2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        swapchain: IDXGISwapChain.VTable(Self),
        swapchain1: IDXGISwapChain1.VTable(Self),
        swapchain2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace IDXGISwapChain.Methods(Self);
    usingnamespace IDXGISwapChain1.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetSourceSize(self: *T, width: UINT, height: UINT) HRESULT {
                return self.v.swapchain2.SetSourceSize(self, width, height);
            }
            pub inline fn GetSourceSize(self: *T, width: *UINT, height: *UINT) HRESULT {
                return self.v.swapchain2.GetSourceSize(self, width, height);
            }
            pub inline fn SetMaximumFrameLatency(self: *T, max_latency: UINT) HRESULT {
                return self.v.swapchain2.SetMaximumFrameLatency(self, max_latency);
            }
            pub inline fn GetMaximumFrameLatency(self: *T, max_latency: *UINT) HRESULT {
                return self.v.swapchain2.GetMaximumFrameLatency(self, max_latency);
            }
            pub inline fn GetFrameLatencyWaitableObject(self: *T) HANDLE {
                return self.v.swapchain2.GetFrameLatencyWaitableObject(self);
            }
            pub inline fn SetMatrixTransform(self: *T, matrix: *const DXGI_MATRIX_3X2_F) HRESULT {
                return self.v.swapchain2.SetMatrixTransform(self, matrix);
            }
            pub inline fn GetMatrixTransform(self: *T, matrix: *DXGI_MATRIX_3X2_F) HRESULT {
                return self.v.swapchain2.GetMatrixTransform(self, matrix);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetSourceSize: fn (*T, UINT, UINT) callconv(WINAPI) HRESULT,
            GetSourceSize: fn (*T, *UINT, *UINT) callconv(WINAPI) HRESULT,
            SetMaximumFrameLatency: fn (*T, UINT) callconv(WINAPI) HRESULT,
            GetMaximumFrameLatency: fn (*T, *UINT) callconv(WINAPI) HRESULT,
            GetFrameLatencyWaitableObject: fn (*T) callconv(WINAPI) HANDLE,
            SetMatrixTransform: fn (*T, *const DXGI_MATRIX_3X2_F) callconv(WINAPI) HRESULT,
            GetMatrixTransform: fn (*T, *DXGI_MATRIX_3X2_F) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IDXGISwapChain2 = GUID{
    .Data1 = 0xa8be2ac4,
    .Data2 = 0x199f,
    .Data3 = 0x4946,
    .Data4 = .{ 0xb3, 0x31, 0x79, 0x59, 0x9f, 0xb9, 0x8d, 0xe7 },
};
