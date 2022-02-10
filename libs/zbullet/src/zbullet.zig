const std = @import("std");
const c = @cImport({
    @cInclude("cbullet.h");
});

test "bt" {
    const w = c.cbtWorldCreate();
    c.cbtWorldDestroy(w);

    std.debug.print("zbullet\n", .{});
}
