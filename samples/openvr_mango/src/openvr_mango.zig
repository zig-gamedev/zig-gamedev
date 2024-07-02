const std = @import("std");
const OpenVR = @import("zopenvr");

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    const stderr = std.io.getStdErr().writer();

    var alloc = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer alloc.deinit();
    const allocator = alloc.allocator();

    const width = 128;
    const height = 128;

    try stdout.print("start\n", .{});

    const openvr = OpenVR.init(.overlay) catch |err| {
        const errDesc = OpenVR.initErrorAsEnglishDescription(err);
        const errSym = OpenVR.initErrorAsSymbol(err);
        try stderr.print("Error! Could not initialize OpenVR: {s} ({s})\n", .{ errDesc, errSym });
        std.process.exit(1);
    };
    defer openvr.deinit();

    const system = try openvr.system();
    const overlay = try openvr.overlay();

    try stdout.print("initing overlay\n", .{});
    const overlayID = try overlay.createOverlay("mangotest-overlay", "mango square overlay");
    try overlay.setOverlayWidthInMeters(overlayID, 0.1);
    try overlay.setOverlayColor(overlayID, 1, 0.8, 0.7);

    try overlay.setOverlayTextureBounds(overlayID, .{
        .u_min = 1,
        .v_min = 0,
        .u_max = 0,
        .v_max = 1,
    });
    try overlay.showOverlay(overlayID);

    {
        var mango = try allocator.alloc(u8, width * height * 4);
        for (0..height) |y| {
            for (0..width) |x| {
                mango[(x + y * width) * 4 + 0] = @as(u8, @intCast(x)) * 2;
                mango[(x + y * width) * 4 + 1] = @as(u8, @intCast(y)) * 2;
                mango[(x + y * width) * 4 + 2] = 0;
                mango[(x + y * width) * 4 + 3] = 255;
            }
        }

        try overlay.setOverlayRaw(u8, overlayID, mango.ptr, width, height, 4);
    }

    var overlay_associated = false;
    while (true) {
        defer std.time.sleep(50 * std.time.ns_per_ms);

        if (overlay_associated) {
            _ = overlay.pollNextOverlayEvent(overlayID);
            continue;
        }

        const index = system.getTrackedDeviceIndexForControllerRole(.left_hand);
        if (index == OpenVR.tracked_device_index_invalid or index == OpenVR.hmd) {
            try stdout.print("couldn't find a left controller to attach the overlay to", .{});
            continue;
        }

        var transform = std.mem.zeroes(OpenVR.Matrix34);
        transform.m[1][1] = 1;
        transform.m[0][2] = 1;
        transform.m[2][0] = 1;

        overlay.setOverlayTransformTrackedDeviceRelative(overlayID, index, transform) catch |err| {
            try stderr.print("Error connecting the mango to the device: {s}\n", .{overlay.getOverlayErrorNameFromError(err)});
            continue;
        };

        try stdout.print("Successfully associated your mango to the tracked device ({d} {x:0>8}).\n", .{ index, overlayID });

        overlay_associated = true;
    }
}
