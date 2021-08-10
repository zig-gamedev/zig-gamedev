const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");

pub const WICPixelFormatGUID = GUID;

pub const Rect = extern struct {
    X: INT,
    Y: INT,
    Width: INT,
    Height: INT,
};

pub const IWICBitmapSource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        bmpsource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetSize(self: *T, width: *UINT, height: *UINT) HRESULT {
                return self.v.bmpsource.GetSize(self, width, height);
            }
            pub inline fn GetPixelFormat(self: *T, guid: *WICPixelFormatGUID) HRESULT {
                return self.v.bmpsource.GetPixelFormat(self, guid);
            }
            pub inline fn CopyPixels(self: *T, rect: ?*const Rect, stride: UINT, size: UINT, buffer: [*]BYTE) HRESULT {
                return self.v.bmpsource.CopyPixels(self, rect, stride, size, buffer);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetSize: fn (*T, *UINT, *UINT) callconv(WINAPI) HRESULT,
            GetPixelFormat: fn (*T, *GUID) callconv(WINAPI) HRESULT,
            GetResolution: *c_void,
            CopyPalette: *c_void,
            CopyPixels: fn (*T, ?*const Rect, UINT, UINT, [*]BYTE) callconv(WINAPI) HRESULT,
        };
    }
};
