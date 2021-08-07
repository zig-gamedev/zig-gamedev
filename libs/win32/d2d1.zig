const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");

pub const ID2D1Resource = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetFactory: *c_void,
        };
    }
};

pub const ID2D1Geometry = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        geometry: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetBounds: *c_void,
            GetWidenedBounds: *c_void,
            StrokeContainsPoint: *c_void,
            FillContainsPoint: *c_void,
            CompareWithGeometry: *c_void,
            Simplify: *c_void,
            Tessellate: *c_void,
            CombineWithGeometry: *c_void,
            Outline: *c_void,
            ComputeArea: *c_void,
            ComputeLength: *c_void,
            ComputePointAtLength: *c_void,
            Widen: *c_void,
        };
    }
};

pub const ID2D1Image = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
};

pub const ID2D1Bitmap = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        resource: ID2D1Resource.VTable(Self),
        bitmap: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace ID2D1Resource.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetSize: *c_void,
            GetPixelSize: *c_void,
            GetPixelFormat: *c_void,
            GetPixelDpi: *c_void,
            CopyFromBitmap: *c_void,
            CopyFromRenderTarget: *c_void,
            CopyFromMemory: *c_void,
        };
    }
};
