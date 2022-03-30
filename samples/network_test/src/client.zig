const std = @import("std");
const zenet = @import("zenet");

pub fn main() !void {
    try zenet.initialize();
    defer zenet.deinitialize();

    var address: zenet.Address = std.mem.zeroes(zenet.Address);
    var event: zenet.Event = std.mem.zeroes(zenet.Event);

    const client = try zenet.Host.create(null, 1, 1, 0, 0);
    defer client.destroy();

    try address.set_host("127.0.0.1");
    address.port = 7777;

    const peer = try client.connect(address, 1, 0);

    if (try client.service(&event, 5000)) {
        if (event.type == zenet.EventType.connect) {
            std.log.debug("Connection to 127.0.0.1:7777 succeeded!", .{});
        }
    }

    while (try client.service(&event, 1000)) {
        switch (event.type) {
            .receive => {
                if (event.packet) |packet| {
                    std.log.debug("A packet of length {d} was received from {d}:{d} on channel {d}.", .{
                        packet.dataLength,
                        event.peer.?.address.host,
                        event.peer.?.address.port,
                        event.channelID,
                    });
                }
            },
            else => {},
        }
    }

    peer.disconnect(0);

    while (try client.service(&event, 3000)) {
        switch (event.type) {
            .receive => {
                if (event.packet) |packet| {
                    packet.destroy();
                }
            },
            .disconnect => {
                std.log.debug("Disconnect succeeded!", .{});
            },
            else => {},
        }
    }
}
