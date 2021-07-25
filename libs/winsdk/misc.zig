const std = @import("std");
usingnamespace std.os.windows;

pub const INT8 = i8;
pub const UINT8 = u8;
pub const UINT16 = c_ushort;
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

pub const VK_ESCAPE = 0x001B;
pub const WS_VISIBLE = 0x10000000;
