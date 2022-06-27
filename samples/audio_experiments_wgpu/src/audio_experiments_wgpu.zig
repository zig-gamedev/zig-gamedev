const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const glfw = @import("glfw");
const gpu = @import("gpu");
const zgpu = @import("zgpu");
const zgui = zgpu.zgui;
const zm = @import("zmath");
const zaudio = @import("zaudio");
const wgsl = @import("audio_experiments_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: audio experiments (wgpu)";

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    audio_engine: zaudio.Engine,
    music: zaudio.Sound,
};

fn init(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    //const arena = arena_state.allocator();

    const depth = createDepthTexture(gctx);

    const audio_engine = try zaudio.Engine.init(allocator);

    const music = try zaudio.Sound.initFile(
        allocator,
        audio_engine,
        content_dir ++ "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    try music.start();

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .audio_engine = audio_engine,
        .music = music,
    };

    return demo;
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.music.deinit(allocator);
    demo.audio_engine.deinit(allocator);
    demo.gctx.deinit(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) !void {
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    if (zgui.begin("Demo Settings", null, .{ .no_move = true, .no_resize = true })) {
        zgui.bulletText(
            "Average :  {d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );
        zgui.bulletText("Right Mouse Button + drag :  rotate camera", .{});
        zgui.bulletText("W, A, S, D :  move camera", .{});

        zgui.spacing();
        zgui.text("Music:", .{});
        const music_is_playing = demo.music.isPlaying();
        if (zgui.button(if (music_is_playing) "  Pause  " else "  Play  ", .{})) {
            if (music_is_playing) {
                try demo.music.stop();
            } else {
                try demo.music.start();
            }
        }
        zgui.sameLine(.{});
        if (zgui.button("  Rewind  ", .{})) {
            try demo.music.seekToPcmFrame(0);
        }

        zgui.spacing();
        zgui.text("Sounds:", .{});
        if (zgui.button("  Play Sound 1  ", .{})) {
            try demo.audio_engine.playSound(content_dir ++ "drum_bass_hard.flac", null);
        }
        zgui.sameLine(.{});
        if (zgui.button("  Play Sound 2  ", .{})) {
            try demo.audio_engine.playSound(content_dir ++ "tabla_tas1.flac", null);
        }
        zgui.sameLine(.{});
        if (zgui.button("  Play Sound 3  ", .{})) {
            try demo.audio_engine.playSound(content_dir ++ "loop_mika.flac", null);
        }
    }
    zgui.end();
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    //const fb_width = gctx.swapchain_descriptor.width;
    //const fb_height = gctx.swapchain_descriptor.height;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Gui pass.
        {
            const pass = zgpu.util.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.util.endRelease(pass);
            zgpu.gui.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});

    if (gctx.present() == .swap_chain_resized) {
        // Release old depth texture.
        gctx.releaseResource(demo.depth_texv);
        gctx.destroyResource(demo.depth_tex);

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(gctx);
        demo.depth_tex = depth.tex;
        demo.depth_texv = depth.texv;
    }
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    tex: zgpu.TextureHandle,
    texv: zgpu.TextureViewHandle,
} {
    const tex = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .dimension_2d,
        .size = .{
            .width = gctx.swapchain_descriptor.width,
            .height = gctx.swapchain_descriptor.height,
            .depth_or_array_layers = 1,
        },
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const texv = gctx.createTextureView(tex, .{});
    return .{ .tex = tex, .texv = texv };
}

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    zgpu.checkSystem(content_dir) catch {
        // In case of error zgpu.checkSystem() will print error message.
        return;
    };

    const window = try glfw.Window.create(1600, 1000, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 400, .height = 400 }, .{ .width = null, .height = null });

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try init(allocator, window);
    defer deinit(allocator, demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir, "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
