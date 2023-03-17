const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl");
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0001";
pub const viewport_width = 1600;
pub const viewport_height = 1600;

var rot: f32 = 0.0;

pub fn draw() void {
    gl.loadIdentity();
    gl.rotatef(rot, 0, 0, 1);
    rot += 0.1;
    rot = @mod(rot, 360.0);

    gl.begin(gl.LINE_LOOP);
    gl.color4f(1.0, 0.0, 0.0, 0.01);
    gl.vertex2f(-50.0, -50.0);
    gl.color4f(0.0, 1.0, 0.0, 0.01);
    gl.vertex2f(50.0, -50.0);
    gl.color4f(0.0, 0.0, 1.0, 0.01);
    gl.vertex2f(0.0, 50.0);
    gl.end();
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.lineWidth(15.0);

    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.05, 0.05, 0.05, 1.0 });
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
}
