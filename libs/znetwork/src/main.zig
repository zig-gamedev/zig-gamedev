// znetwork v0.1

pub const enet = @import("zenet.zig");
pub usingnamespace @import("network.zig");

test "znetwork" {
    _ = enet;
    const net = @import("network.zig");
    try net.init();
    defer net.deinit();
}
