const std = @import("std");
const sdl = @import("zsdl2");
const sdl_image = @import("zsdl2_image");

const content_dir = "sdl2_demo_content/";

var quit = false;
var window: *sdl.Window = undefined;
var renderer: *sdl.Renderer = undefined;
var sprite: Sprite = undefined;

const platform_rect = sdl.Rect{ .x = 160, .y = 380, .w = 300, .h = 50 };

pub fn init() !void {
    _ = sdl.setHint(sdl.hint_windows_dpi_awareness, "system");
    try sdl.init(.{ .audio = true, .video = true });

    const default_window_width = 640;
    const default_window_height = 480;

    window = try sdl.createWindow(
        "zig-gamedev: sdl2_demo",
        sdl.Window.pos_undefined,
        sdl.Window.pos_undefined,
        default_window_width,
        default_window_height,
        .{ .allow_highdpi = false },
    );

    renderer = try sdl.createRenderer(
        window,
        -1,
        .{ .present_vsync = true },
    );

    sprite = try Sprite.initWithFile(content_dir ++ "zero.png");
    sprite.x = default_window_width / 2;
    sprite.y = 220;
}

pub fn deinit() void {
    sprite.deinit();
    renderer.destroy();
    window.destroy();
    sdl.quit();
}

pub fn shouldQuit() bool {
    return quit;
}

pub fn updateAndRender() !void {
    var event: sdl.Event = undefined;
    while (sdl.pollEvent(&event)) {
        if (event.type == .quit) {
            quit = true;
        } else if (event.type == .keydown) {
            if (event.key.keysym.sym == .escape) {
                quit = true;
            }
        }
    }

    // Clear screen with colour
    try renderer.setDrawColor(.{ .r = 24, .g = 22, .b = 22, .a = 255 });
    try renderer.clear();

    // Draw platform
    try renderer.setDrawColor(.{ .r = 108, .g = 128, .b = 128, .a = 255 });
    try renderer.fillRect(platform_rect);

    // Draw Zero the Ziguana
    try sprite.draw();

    renderer.present();
}

const Sprite = struct {
    texture: *sdl.Texture,
    texture_width: u16,
    texture_height: u16,
    x: i32,
    y: i32,
    scale: f32,

    pub fn initWithFile(filepath: [:0]const u8) !Sprite {
        const surface = try sdl_image.load(filepath);
        defer surface.free();
        return .{
            .texture = try renderer.createTextureFromSurface(surface),
            .texture_width = @intCast(surface.w),
            .texture_height = @intCast(surface.h),
            .x = 0,
            .y = 0,
            .scale = 1.0,
        };
    }

    pub fn deinit(self: *Sprite) void {
        self.texture.destroy();
        self.texture = undefined;
    }

    pub fn draw(self: Sprite) !void {
        const width = @as(f32, @floatFromInt(self.texture_width)) * self.scale;
        const height = @as(f32, @floatFromInt(self.texture_height)) * self.scale;
        try renderer.copy(self.texture, null, &.{
            .x = self.x - @as(i32, @intFromFloat(@round(width / 2))),
            .y = self.y - @as(i32, @intFromFloat(@round(height / 2))),
            .w = @as(i32, @intFromFloat(@round(width))),
            .h = @as(i32, @intFromFloat(@round(height))),
        });
    }
};
