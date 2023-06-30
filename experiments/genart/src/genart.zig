const std = @import("std");
const sdl = @import("zsdl");
const gl = @import("zopengl");
const ximpl = @import("ximpl");
const xcommon = @import("xcommon");

pub export var NvOptimusEnablement: u32 = 1;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    xcommon.allocator = gpa.allocator();

    _ = sdl.setHint(sdl.hint_windows_dpi_awareness, "system");

    try sdl.init(.{ .audio = true, .video = true });
    defer sdl.quit();

    try sdl.gl.setAttribute(.context_profile_mask, @intFromEnum(sdl.gl.Profile.compatibility));
    try sdl.gl.setAttribute(.context_major_version, 4);
    try sdl.gl.setAttribute(.context_minor_version, 6);

    const errmsg = "Sorry but this application requires modern NVIDIA GPU to run.";

    const window = sdl.Window.create(
        ximpl.name,
        sdl.Window.pos_undefined,
        sdl.Window.pos_undefined,
        ximpl.display_width,
        ximpl.display_height,
        .{ .opengl = true, .allow_highdpi = true },
    ) catch |err| {
        sdl.showSimpleMessageBox(.{ .information = true }, "OpenGL info", errmsg, null) catch unreachable;
        return err;
    };
    defer window.destroy();

    const gl_context = sdl.gl.createContext(window) catch |err| {
        sdl.showSimpleMessageBox(.{ .information = true }, "OpenGL info", errmsg, null) catch unreachable;
        return err;
    };
    defer sdl.gl.deleteContext(gl_context);

    try sdl.gl.makeCurrent(window, gl_context);
    try sdl.gl.setSwapInterval(1);

    if (!sdl.gl.isExtensionSupported("GL_NV_path_rendering") or
        !sdl.gl.isExtensionSupported("GL_NV_bindless_texture") or
        !sdl.gl.isExtensionSupported("GL_NV_shader_buffer_load") or
        !sdl.gl.isExtensionSupported("GL_NV_mesh_shader"))
    {
        sdl.showSimpleMessageBox(.{ .information = true }, "OpenGL info", errmsg, null) catch unreachable;
        return;
    }

    try gl.loadCompatProfileExt(sdl.gl.getProcAddress);
    try gl.loadExtension(sdl.gl.getProcAddress, .NV_bindless_texture);
    try gl.loadExtension(sdl.gl.getProcAddress, .NV_shader_buffer_load);

    std.log.info("OpenGL vendor: {s}", .{gl.getString(gl.VENDOR)});
    std.log.info("OpenGL renderer: {s}", .{gl.getString(gl.RENDERER)});
    std.log.info("OpenGL version: {s}", .{gl.getString(gl.VERSION)});

    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(
        gl.PROJECTION,
        -ximpl.display_width * 0.5,
        ximpl.display_width * 0.5,
        -ximpl.display_height * 0.5,
        ximpl.display_height * 0.5,
        -1.0,
        1.0,
    );
    gl.enable(gl.FRAMEBUFFER_SRGB);
    gl.enable(gl.MULTISAMPLE);

    gl.createTextures(gl.TEXTURE_2D_MULTISAMPLE, 1, &xcommon.display_tex);
    defer gl.deleteTextures(1, &xcommon.display_tex);
    gl.textureStorage2DMultisample(
        xcommon.display_tex,
        if (@hasDecl(ximpl, "display_num_samples")) ximpl.display_num_samples else 8,
        if (@hasDecl(ximpl, "display_format")) ximpl.display_format else gl.RGBA16F,
        ximpl.display_width,
        ximpl.display_height,
        gl.FALSE,
    );

    xcommon.display_texh = gl.getTextureHandleNV(xcommon.display_tex);
    gl.makeTextureHandleResidentNV(xcommon.display_texh);

    gl.createFramebuffers(1, &xcommon.display_fbo);
    defer gl.deleteFramebuffers(1, &xcommon.display_fbo);
    gl.namedFramebufferTexture(xcommon.display_fbo, gl.COLOR_ATTACHMENT0, xcommon.display_tex, 0);
    gl.clearNamedFramebufferfv(xcommon.display_fbo, gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });

    try ximpl.init();
    defer if (@hasDecl(ximpl, "deinit")) ximpl.deinit();

    main_loop: while (true) {
        var event: sdl.Event = undefined;
        while (sdl.pollEvent(&event)) {
            if (event.type == .quit) {
                break :main_loop;
            } else if (event.type == .keydown) {
                switch (event.key.keysym.sym) {
                    .escape => break :main_loop,
                    .f12 => {
                        xcommon.saveScreenshot(xcommon.allocator, "screenshot.png");
                    },
                    else => {},
                }
            }
        }

        const stats = updateFrameStats(window, ximpl.name);
        xcommon.frame_time = stats.time;
        xcommon.frame_delta_time = stats.delta_time;

        gl.bindFramebuffer(gl.FRAMEBUFFER, xcommon.display_fbo);
        ximpl.draw();
        gl.bindFramebuffer(gl.FRAMEBUFFER, 0);

        gl.blitNamedFramebuffer(
            xcommon.display_fbo,
            0, // default fbo
            0,
            0,
            ximpl.display_width,
            ximpl.display_height,
            0,
            0,
            ximpl.display_width,
            ximpl.display_height,
            gl.COLOR_BUFFER_BIT,
            gl.LINEAR,
        );
        sdl.gl.swapWindow(window);

        if (@import("builtin").mode == .Debug and gl.getError() != gl.NO_ERROR) {
            std.debug.panic("OpenGL error detected!", .{});
        }
    }
}

fn updateFrameStats(window: *sdl.Window, name: [:0]const u8) struct { time: f64, delta_time: f32 } {
    const state = struct {
        var timer: std.time.Timer = undefined;
        var previous_time_ns: u64 = 0;
        var header_refresh_time_ns: u64 = 0;
        var frame_count: u64 = ~@as(u64, 0);
    };

    if (state.frame_count == ~@as(u64, 0)) {
        state.timer = std.time.Timer.start() catch unreachable;
        state.previous_time_ns = 0;
        state.header_refresh_time_ns = 0;
        state.frame_count = 0;
    }

    const now_ns = now_ns: {
        const now_ns = state.timer.read();
        const this_frame_ns = now_ns - state.previous_time_ns;
        const wanted_per_frame_ns = @as(u64, @intFromFloat(1.0 / 60.0 * std.time.ns_per_s));

        if (this_frame_ns < wanted_per_frame_ns) {
            std.time.sleep(wanted_per_frame_ns - this_frame_ns);
            break :now_ns state.timer.read();
        }
        break :now_ns now_ns;
    };

    const time = @as(f64, @floatFromInt(now_ns)) / std.time.ns_per_s;
    const delta_time = @as(f32, @floatFromInt(now_ns - state.previous_time_ns)) / std.time.ns_per_s;
    state.previous_time_ns = now_ns;

    if ((now_ns - state.header_refresh_time_ns) >= std.time.ns_per_s) {
        const t = @as(f64, @floatFromInt(now_ns - state.header_refresh_time_ns)) / std.time.ns_per_s;
        const fps = @as(f64, @floatFromInt(state.frame_count)) / t;
        const ms = (1.0 / fps) * 1000.0;

        var buffer = [_]u8{0} ** 128;
        const buffer_slice = buffer[0 .. buffer.len - 1];
        const header = std.fmt.bufPrintZ(
            buffer_slice,
            "[{d:.1} fps  {d:.3} ms] {s}",
            .{ fps, ms, name.ptr },
        ) catch name;

        window.setTitle(header);

        state.header_refresh_time_ns = now_ns;
        state.frame_count = 0;
    }
    state.frame_count += 1;

    return .{ .time = time, .delta_time = delta_time };
}
