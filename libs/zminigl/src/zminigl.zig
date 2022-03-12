// zminigl - version 0.1

pub const FbBuffer = enum(u32) {
    color = 0x1800,
    depth = 0x1801,
    stencil = 0x1802,
    depth_stencil = 0x84f9,
};

pub const Buffer = extern struct {
    name: u32,
};

pub const Texture = extern struct {
    name: u32,
};

pub const Framebuffer = extern struct {
    name: u32,
};

pub const default_framebuffer = Framebuffer{ .name = 0 };

pub var createFramebuffers: fn (n: i32, framebuffers: [*]Framebuffer) callconv(.C) void = undefined;
pub var deleteFramebuffers: fn (n: i32, framebuffers: [*]const Framebuffer) callconv(.C) void = undefined;

pub var clearNamedFramebufferfv: fn (
    framebuffer: Framebuffer,
    buffer: FbBuffer,
    drawbuffer: i32,
    value: *const [4]f32,
) callconv(.C) void = undefined;

pub fn init(
    getProcAddr: fn (name: [*:0]const u8) callconv(.C) ?fn () callconv(.C) void,
) void {
    clearNamedFramebufferfv = @ptrCast(@TypeOf(clearNamedFramebufferfv), getProcAddr("glClearNamedFramebufferfv").?);
}

test "zminigl.framebuffer.basic" {
    clearNamedFramebufferfv(default_framebuffer, .color, 0, &.{ 0.0, 0.0, 0.0, 1.0 });
}
