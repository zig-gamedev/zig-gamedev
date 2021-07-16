const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");

pub const DXGI_SAMPLE_DESC = extern struct {
    Count: UINT,
    Quality: UINT,
};
