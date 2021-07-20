const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");
usingnamespace @import("dxgicommon.zig");
usingnamespace @import("dxgitype.zig");
usingnamespace @import("dxgiformat.zig");
usingnamespace @import("dxgi.zig");

pub const DXGI_SCALING = enum(UINT) {
    STRETCH = 0,
    NONE = 1,
    ASPECT_RATIO_STRETCH = 2,
};

pub const DXGI_ALPHA_MODE = enum(UINT) {
    UNSPECIFIED = 0,
    PREMULTIPLIED = 1,
    STRAIGHT = 2,
    IGNORE = 3,
};

pub const DXGI_SWAP_CHAIN_DESC1 = extern struct {
    Width: UINT,
    Height: UINT,
    Format: DXGI_FORMAT,
    Stereo: BOOL,
    SampleDesc: DXGI_SAMPLE_DESC,
    BufferUsage: DXGI_USAGE,
    BufferCount: UINT,
    Scaling: DXGI_SCALING,
    SwapEffect: DXGI_SWAP_EFFECT,
    AlphaMode: DXGI_ALPHA_MODE,
    Flags: DXGI_SWAP_CHAIN_FLAG,
};

pub const DXGI_SWAP_CHAIN_FULLSCREEN_DESC = extern struct {
    RefreshRate: DXGI_RATIONAL,
    ScanlineOrdering: DXGI_MODE_SCANLINE_ORDER,
    Scaling: DXGI_MODE_SCALING,
    Windowed: BOOL,
};

pub const DXGI_PRESENT_PARAMETERS = extern struct {
    DirtyRectsCount: UINT,
    pDirtyRects: ?*RECT,
    pScrollRect: *RECT,
    pScrollOffset: *POINT,
};

pub const IDXGISwapChain1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IDXGIObject.VTable(Self),
        devsubobj: IDXGIDeviceSubObject.VTable(Self),
        swapchain: IDXGISwapChain.VTable(Self),
        swapchain1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IDXGIObject.Methods(Self);
    usingnamespace IDXGIDeviceSubObject.Methods(Self);
    usingnamespace IDXGISwapChain.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDesc1(self: *T, desc: *DXGI_SWAP_CHAIN_DESC1) HRESULT {
                return self.v.swapchain1.GetDesc1(self, desc);
            }
            pub inline fn GetFullscreenDesc(self: *T, desc: *DXGI_SWAP_CHAIN_FULLSCREEN_DESC) HRESULT {
                return self.v.swapchain1.GetFullscreenDesc(self, desc);
            }
            pub inline fn GetHwnd(self: *T, hwnd: *HWND) HRESULT {
                return self.v.swapchain1.GetHwnd(self, hwnd);
            }
            pub inline fn GetCoreWindow(self: *T, guid: *const GUID, unknown: *?*c_void) HRESULT {
                return self.v.swapchain1.GetCoreWindow(self, guid, unknown);
            }
            pub inline fn Present1(
                self: *T,
                sync_interval: UINT,
                flags: DXGI_PRESENT,
                params: *const DXGI_PRESENT_PARAMETERS,
            ) HRESULT {
                return self.v.swapchain1.Present1(self, sync_interval, flags, params);
            }
            pub inline fn IsTemporaryMonoSupported(self: *T) BOOL {
                return self.v.swapchain1.IsTemporaryMonoSupported(self);
            }
            pub inline fn GetRestrictToOutput(self: *T, output: *?*IDXGIOutput) HRESULT {
                return self.v.swapchain1.GetRestrictToOutput(self, output);
            }
            pub inline fn SetBackgroundColor(self: *T, color: *const DXGI_RGBA) HRESULT {
                return self.v.swapchain1.SetBackgroundColor(self, color);
            }
            pub inline fn GetBackgroundColor(self: *T, color: *DXGI_RGBA) HRESULT {
                return self.v.swapchain1.GetBackgroundColor(self, color);
            }
            pub inline fn SetRotation(self: *T, rotation: DXGI_MODE_ROTATION) HRESULT {
                return self.v.swapchain1.SetRotation(self, rotation);
            }
            pub inline fn GetRotation(self: *T, rotation: *DXGI_MODE_ROTATION) HRESULT {
                return self.v.swapchain1.GetRotation(self, rotation);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDesc1: fn (*T, *DXGI_SWAP_CHAIN_DESC1) callconv(WINAPI) HRESULT,
            GetFullscreenDesc: fn (*T, *DXGI_SWAP_CHAIN_FULLSCREEN_DESC) callconv(WINAPI) HRESULT,
            GetHwnd: fn (*T, *HWND) callconv(WINAPI) HRESULT,
            GetCoreWindow: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
            Present1: fn (*T, UINT, DXGI_PRESENT, *const DXGI_PRESENT_PARAMETERS) callconv(WINAPI) HRESULT,
            IsTemporaryMonoSupported: fn (*T) callconv(WINAPI) BOOL,
            GetRestrictToOutput: fn (*T, *?*IDXGIOutput) callconv(WINAPI) HRESULT,
            SetBackgroundColor: fn (*T, *const DXGI_RGBA) callconv(WINAPI) HRESULT,
            GetBackgroundColor: fn (*T, *DXGI_RGBA) callconv(WINAPI) HRESULT,
            SetRotation: fn (*T, DXGI_MODE_ROTATION) callconv(WINAPI) HRESULT,
            GetRotation: fn (*T, *DXGI_MODE_ROTATION) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IDXGISwapChain1 = GUID{
    .Data1 = 0x790a45f7,
    .Data2 = 0x0d41,
    .Data3 = 0x4876,
    .Data4 = .{ 0x98, 0x3a, 0x0a, 0x55, 0xcf, 0xe6, 0xf4, 0xaa },
};

pub const IID_IDXGIFactory2 = GUID{
    .Data1 = 0x50c83a1c,
    .Data2 = 0xe072,
    .Data3 = 0x4c48,
    .Data4 = .{ 0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0 },
};
