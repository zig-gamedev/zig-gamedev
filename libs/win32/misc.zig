const std = @import("std");
const assert = std.debug.assert;
usingnamespace std.os.windows;

pub const INT8 = i8;
pub const UINT8 = u8;
pub const UINT16 = c_ushort;
pub const UINT32 = c_uint;
pub const UINT64 = c_ulonglong;
pub const HMONITOR = HANDLE;
pub const LUID = extern struct {
    LowPart: DWORD,
    HighPart: LONG,
};

pub const IUnknown = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: VTable(Self),
    },
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn QueryInterface(self: *T, guid: *const GUID, outobj: ?*?*c_void) HRESULT {
                return self.v.unknown.QueryInterface(self, guid, outobj);
            }
            pub inline fn AddRef(self: *T) ULONG {
                return self.v.unknown.AddRef(self);
            }
            pub inline fn Release(self: *T) ULONG {
                return self.v.unknown.Release(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            QueryInterface: fn (*T, *const GUID, ?*?*c_void) callconv(WINAPI) HRESULT,
            AddRef: fn (*T) callconv(WINAPI) ULONG,
            Release: fn (*T) callconv(WINAPI) ULONG,
        };
    }
};

pub extern "user32" fn SetProcessDPIAware() callconv(WINAPI) BOOL;

pub extern "user32" fn LoadCursorA(
    hInstance: ?HINSTANCE,
    lpCursorName: LPCSTR,
) callconv(WINAPI) HCURSOR;

pub extern "user32" fn GetClientRect(HWND, *RECT) callconv(WINAPI) BOOL;

pub extern "user32" fn SetWindowTextA(hWnd: ?HWND, lpString: LPCSTR) callconv(WINAPI) BOOL;

pub extern "user32" fn GetAsyncKeyState(vKey: c_int) callconv(WINAPI) SHORT;

pub const CLSCTX_INPROC_SERVER = 0x1;

pub extern "ole32" fn CoCreateInstance(
    rclsid: *const GUID,
    pUnkOuter: ?*IUnknown,
    dwClsContext: DWORD,
    riid: *const GUID,
    ppv: *?*c_void,
) callconv(WINAPI) HRESULT;

pub const VK_TAB = 0x09;
pub const VK_ESCAPE = 0x1B;
pub const VK_LEFT = 0x25;
pub const VK_UP = 0x26;
pub const VK_RIGHT = 0x27;
pub const VK_DOWN = 0x28;
pub const VK_PRIOR = 0x21;
pub const VK_NEXT = 0x22;
pub const VK_END = 0x23;
pub const VK_HOME = 0x24;
pub const VK_DELETE = 0x2E;
pub const VK_BACK = 0x08;
pub const VK_RETURN = 0x0D;
pub const VK_CONTROL = 0x11;
pub const VK_SHIFT = 0x10;
pub const VK_MENU = 0x12;

pub const D3D12_ERROR_ADAPTER_NOT_FOUND = @bitCast(HRESULT, @as(c_ulong, 0x887E0001));
pub const D3D12_ERROR_DRIVER_VERSION_MISMATCH = @bitCast(HRESULT, @as(c_ulong, 0x887E0002));
pub const DXGI_ERROR_INVALID_CALL = @bitCast(HRESULT, @as(c_ulong, 0x887A0001));
pub const DXGI_ERROR_WAS_STILL_DRAWING = @bitCast(HRESULT, @as(c_ulong, 0x887A000A));
pub const DWRITE_E_FILEFORMAT = @bitCast(HRESULT, @as(c_ulong, 0x88985000));

pub const DXGI_STATUS_MODE_CHANGED = @bitCast(HRESULT, @as(c_ulong, 0x087A0007));
