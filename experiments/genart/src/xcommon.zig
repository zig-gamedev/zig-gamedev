const std = @import("std");
const gl = @import("zopengl");
const zstbi = @import("zstbi");

pub var display_fbo: gl.Uint = undefined;
pub var display_tex: gl.Uint = undefined;
pub var display_texh: gl.Uint64 = undefined;

pub var frame_time: f64 = undefined;
pub var frame_delta_time: f32 = undefined;

pub var allocator: std.mem.Allocator = undefined;

pub fn saveScreenshot(alloc: std.mem.Allocator, filename: [:0]const u8) void {
    var viewport: [4]gl.Int = undefined;
    gl.getIntegerv(gl.VIEWPORT, &viewport);

    const mem = alloc.alloc(u8, @as(usize, @intCast(viewport[2] * viewport[3] * 3))) catch @panic("OOM");
    defer alloc.free(mem);

    gl.readPixels(viewport[0], viewport[1], viewport[2], viewport[3], gl.RGB, gl.UNSIGNED_BYTE, mem.ptr);

    zstbi.init(alloc);
    defer zstbi.deinit();

    zstbi.setFlipVerticallyOnWrite(true);

    const image = zstbi.Image{
        .data = mem,
        .width = @as(u32, @intCast(viewport[2])),
        .height = @as(u32, @intCast(viewport[3])),
        .num_components = 3,
        .bytes_per_component = 1,
        .bytes_per_row = @as(u32, @intCast(viewport[2])) * 3,
        .is_hdr = false,
    };
    image.writeToFile(filename, .png) catch {};

    std.debug.print("{s} saved\n", .{filename});
}
