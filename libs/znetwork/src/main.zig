// znetwork v0.1

// TODO: Re-enable zenet when stage3 compiler is more stable
//pub const enet = @import("zenet.zig");
pub usingnamespace @import("network.zig");

test "znetwork" {
    const net = @import("network.zig");
    try net.init();
    defer net.deinit();
}
