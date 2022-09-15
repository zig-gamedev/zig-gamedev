// znetwork

pub usingnamespace @import("network.zig");

test "znetwork" {
    const net = @import("network.zig");
    try net.init();
    defer net.deinit();
}
