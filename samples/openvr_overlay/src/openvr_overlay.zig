const std = @import("std");
const glfw = @import("zglfw");
const zopengl = @import("zopengl");
const zopenvr = @import("zopenvr");

const gl_major = 4;
const gl_minor = 0;

const width = 512;
const height = 512;
const overlayWidth = 0.25; // meters
const fps = 10;

pub fn main() !void {
    try glfw.init();
    defer glfw.terminate();

    glfw.windowHintTyped(.context_version_major, gl_major);
    glfw.windowHintTyped(.context_version_minor, gl_minor);
    glfw.windowHintTyped(.opengl_profile, .opengl_core_profile);
    glfw.windowHintTyped(.opengl_forward_compat, true);
    glfw.windowHintTyped(.client_api, .opengl_api);
    // glfw.windowHintTyped(.doublebuffer, .opengl_api);
    glfw.windowHintTyped(.visible, false);

    const window = try glfw.Window.create(600, 600, "zig-gamedev: openvr_overlay", null);
    defer window.destroy();

    glfw.makeContextCurrent(window);

    try zopengl.loadCoreProfile(glfw.getProcAddress, gl_major, gl_minor);
    const gl = zopengl.bindings;
    glfw.swapInterval(1);

    const openvr = openvr: {
        // if wireless headset not connected then retry till its present
        while (true) {
            const openvr = zopenvr.init(.overlay) catch |err| {
                const errDesc = zopenvr.initErrorAsEnglishDescription(err);
                const errSym = zopenvr.initErrorAsSymbol(err);
                std.log.err("Could not initialize OpenVR: {s} ({s})\n", .{ errDesc, errSym });

                switch (err) {
                    zopenvr.InitError.DriverWirelessHmdNotConnected => {
                        std.time.sleep(2 * std.time.ns_per_s);
                        continue;
                    },
                    else => std.process.exit(1),
                }
            };
            break :openvr openvr;
        }
    };
    defer openvr.deinit();

    const system = try openvr.system();
    const overlay = try openvr.overlay();

    const overlayID = try overlay.createOverlay("zopenvr-overlay", "test overlay with zgui");
    try overlay.setOverlayWidthInMeters(overlayID, overlayWidth);
    try overlay.setOverlayColor(overlayID, 0.3, 0.8, 0.9);
    try overlay.setOverlayTextureBounds(overlayID, .{
        .u_min = 1,
        .v_min = 0,
        .u_max = 0,
        .v_max = 1,
    });

    try overlay.showOverlay(overlayID);

    var overlayTexture: gl.Uint = undefined;
    {
        gl.genTextures(1, &overlayTexture);
        gl.bindTexture(gl.TEXTURE_2D, overlayTexture);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
    }

    var overlay_associated = false;
    while (!window.shouldClose()) {
        glfw.pollEvents();

        gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.2, 0.6, 0.4, 1.0 });

        ///////

        if (!overlay_associated) blk: {
            const index = system.getTrackedDeviceIndexForControllerRole(.left_hand);
            if (index == zopenvr.tracked_device_index_invalid or index == zopenvr.hmd) {
                std.log.warn("couldn't find a left controller to attach the overlay to", .{});
                break :blk;
            }

            var transform = std.mem.zeroes(zopenvr.Matrix34);
            transform.m[0][0] = 1;
            transform.m[1][2] = 1;
            transform.m[2][1] = 1;

            transform.m[2][3] = -0.12;

            overlay.setOverlayTransformTrackedDeviceRelative(overlayID, index, transform) catch |err| {
                std.log.err("Error connecting the overlay to the device: {s}\n", .{overlay.getOverlayErrorNameFromError(err)});
                break :blk;
            };

            std.log.info("Successfully associated the overlay to the tracked device ({d} {x:0>8}).\n", .{ index, overlayID });

            overlay_associated = true;
        }

        gl.bindTexture(gl.TEXTURE_2D, overlayTexture);
        gl.copyTexImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 0, 0, width, height, 0);

        try overlay.setOverlayTexture(overlayID, .{
            .handle = @ptrFromInt(overlayTexture),
            .color_space = .auto,
            .texture_type = .opengl,
        });

        if (overlay_associated) {
            while (overlay.pollNextOverlayEvent(overlayID)) |event| {
                _ = event;
            }
        }

        // window.swapBuffers();

        overlay.waitFrameSync(1000 / fps) catch |err| switch (err) {
            zopenvr.OverlayError.TimedOut => std.time.sleep(200 * std.time.ns_per_ms),
            else => return err,
        };
    }
}
