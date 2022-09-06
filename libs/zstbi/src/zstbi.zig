const std = @import("std");

pub fn Image(comptime ChannelType: type) type {
    return struct {
        const Self = @This();

        data: []ChannelType,
        width: u32,
        height: u32,
        bytes_per_row: u32,
        channels_in_memory: u32,
        channels_in_file: u32,

        pub fn init(
            filename: [*:0]const u8,
            desired_channels: u32,
        ) !Self {
            var x: c_int = undefined;
            var y: c_int = undefined;
            var ch: c_int = undefined;
            var data = switch (ChannelType) {
                u8 => stbi_load(filename, &x, &y, &ch, @intCast(c_int, desired_channels)),
                f16 => @ptrCast(?[*]f16, stbi_loadf(filename, &x, &y, &ch, @intCast(c_int, desired_channels))),
                f32 => stbi_loadf(filename, &x, &y, &ch, @intCast(c_int, desired_channels)),
                else => @compileError("[zgpu] stbi.Image: ChannelType can be u8, f16 or f32."),
            };
            if (data == null)
                return error.StbiLoadFailed;

            const channels_in_memory = if (desired_channels == 0) @intCast(u32, ch) else desired_channels;
            const width = @intCast(u32, x);
            const height = @intCast(u32, y);

            if (ChannelType == f16) {
                var data_f32 = @ptrCast([*]f32, data.?);
                const num = width * height * channels_in_memory;
                var i: u32 = 0;
                while (i < num) : (i += 1) {
                    data.?[i] = @floatCast(f16, data_f32[i]);
                }
            }

            return Self{
                .data = data.?[0 .. width * height * channels_in_memory],
                .width = width,
                .height = height,
                .bytes_per_row = width * channels_in_memory * @sizeOf(ChannelType),
                .channels_in_memory = channels_in_memory,
                .channels_in_file = @intCast(u32, ch),
            };
        }

        pub fn deinit(image: *Self) void {
            stbi_image_free(image.data.ptr);
            image.* = undefined;
        }
    };
}

pub const hdrToLdrScale = stbi_hdr_to_ldr_scale;
pub const hdrToLdrGamma = stbi_hdr_to_ldr_gamma;
pub const ldrToHdrScale = stbi_ldr_to_hdr_scale;
pub const ldrToHdrGamma = stbi_ldr_to_hdr_gamma;

pub fn isHdr(filename: [*:0]const u8) bool {
    return stbi_is_hdr(filename) == 1;
}

pub fn setFlipVerticallyOnLoad(should_flip: bool) void {
    stbi_set_flip_vertically_on_load(if (should_flip) 1 else 0);
}

extern fn stbi_load(
    filename: [*:0]const u8,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]u8;

extern fn stbi_loadf(
    filename: [*:0]const u8,
    x: *c_int,
    y: *c_int,
    channels_in_file: *c_int,
    desired_channels: c_int,
) ?[*]f32;

extern fn stbi_image_free(image_data: ?*anyopaque) void;

extern fn stbi_hdr_to_ldr_scale(scale: f32) void;
extern fn stbi_hdr_to_ldr_gamma(gamma: f32) void;
extern fn stbi_ldr_to_hdr_scale(scale: f32) void;
extern fn stbi_ldr_to_hdr_gamma(gamma: f32) void;

extern fn stbi_is_hdr(filename: [*:0]const u8) c_int;
extern fn stbi_set_flip_vertically_on_load(flag_true_if_should_flip: c_int) void;
