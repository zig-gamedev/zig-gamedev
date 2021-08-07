const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");
usingnamespace @import("d2d1.zig");

pub const ID2D1Bitmap1 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        image: ID2D1Image.VTable(Self),
        bitmap: ID2D1Bitmap.VTable(Self),
        bitmap1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace ID2D1Image.Methods(Self);
    usingnamespace ID2D1Bitmap.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetColorContext: *c_void,
            GetOptions: *c_void,
            GetSurface: *c_void,
            Map: *c_void,
            Unmap: *c_void,
        };
    }
};
