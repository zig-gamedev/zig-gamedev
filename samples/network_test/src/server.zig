const std = @import("std");
const zenet = @import("znetwork").enet;

pub fn main() !void {
    try zenet.initialize();
    defer zenet.deinitialize();

    var address: zenet.Address = std.mem.zeroes(zenet.Address);
    address.host = zenet.HOST_ANY; // localhost
    address.port = 7777;

    const server = try zenet.Host.create(address, 1, 1, 0, 0);
    defer server.destroy();

    std.log.debug("Server started!", .{});

    // game loop
    while (true) {
        var event: zenet.Event = std.mem.zeroes(zenet.Event);

        // wait 1000 ms (1 second) for an event
        while (try server.service(&event, 1000)) {
            if (event.peer == null)
                continue;

            switch (event.type) {
                .connect => {
                    std.log.debug(
                        "A new client connected from {d}:{d}.",
                        .{ event.peer.?.address.host, event.peer.?.address.port },
                    );
                },
                .receive => {
                    if (event.packet) |packet| {
                        std.log.debug(
                            "A packet of length {d} was received from {s} on channel {d}.",
                            .{ packet.dataLength, event.peer.?.data, event.channelID },
                        );
                        packet.destroy();
                    }
                },
                .disconnect => {
                    std.log.debug("{s} disconnected.", .{event.peer.?.data});
                    event.peer.?.data = null;
                },
                else => {
                    std.log.debug("ugh!", .{});
                },
            }
        }
    }
}
