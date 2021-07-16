const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");

pub const ID3DBlob = extern struct {
    const Self = @This();
    vtbl: *const extern struct {
        unknown: IUnknown.VTable(Self),
        blob: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetBufferPointer(self: *T) *c_void {
                return self.v.blob.GetBufferPointer(self);
            }
            pub inline fn GetBufferSize(self: *T) SIZE_T {
                return self.v.blob.GetBufferSize(self);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetBufferPointer: fn (*T) callconv(WINAPI) *c_void,
            GetBufferSize: fn (*T) callconv(WINAPI) SIZE_T,
        };
    }
};
