const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0009";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_postprocess: gl.Uint = 0;

var x0: f32 = 0;
var y0: f32 = 0;
var frame0: u32 = 0;
var step0: u32 = 0;
var angle0: f32 = 0.0;

var x1: f32 = 0;
var y1: f32 = 0;
var frame1: u32 = 0;
var step1: u32 = 0;
var angle1: f32 = 30.0;

pub fn draw() void {
    angle0 += 0.2;
    if (angle0 > 360.0) angle0 = 0.0;

    angle1 += 0.5;
    if (angle1 > 360.0) angle1 = 0.0;

    if (step0 == 0) x0 += 0.01;
    //if (step == 1) y += 0.005;
    if (step0 == 1) x0 -= 0.01;
    //if (step == 3) y -= 0.005;

    if (step1 == 0) x1 += 0.01;
    if (step1 == 1) x1 -= 0.01;

    frame0 += 1;
    if (frame0 == 100) {
        frame0 = 0;
        step0 += 1;
        if (step0 == 2) step0 = 0;
    }

    frame1 += 1;
    if (frame1 == 50) {
        frame1 = 0;
        step1 += 1;
        if (step1 == 2) step1 = 0;
    }

    gl.pointSize(17.0);
    gl.enable(gl.BLEND);
    gl.useProgram(0);
    gl.loadIdentity();

    gl.color3f(1.0, 0.01, 0.0);
    gl.pushMatrix();
    gl.rotatef(angle0, 0, 0, 1);
    gl.begin(gl.POINTS);
    gl.vertex2f(x0, y0);
    gl.end();
    gl.popMatrix();

    gl.color3f(0.01, 1.0, 0.0);
    gl.pushMatrix();
    gl.rotatef(angle0 + 120.0, 0, 0, 1);
    gl.begin(gl.POINTS);
    gl.vertex2f(x0, y0);
    gl.end();
    gl.popMatrix();

    gl.color3f(1.0, 1.0, 0.01);
    gl.pushMatrix();
    gl.rotatef(angle0 + 240.0, 0, 0, 1);
    gl.begin(gl.POINTS);
    gl.vertex2f(x0, y0);
    gl.end();
    gl.popMatrix();

    gl.color3f(0.0, 0.01, 1.0);
    gl.pushMatrix();
    gl.rotatef(-angle1, 0, 0, 1);
    gl.begin(gl.POINTS);
    gl.vertex2f(x1, y1);
    gl.end();
    gl.popMatrix();

    gl.textureBarrier();

    gl.disable(gl.BLEND);
    gl.useProgram(fs_postprocess);
    gl.loadIdentity();
    gl.begin(gl.TRIANGLES);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);
    gl.end();
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.pointSize(17.0);
    gl.blendFunc(gl.ONE, gl.ONE);

    fs_postprocess = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  #extension NV_bindless_texture : require
        \\
        \\  layout(location = 0) uniform sampler2DMS display_texh;
        \\
        \\  void main() {
        \\      vec3 color = texelFetch(display_texh, ivec2(gl_FragCoord.xy), gl_SampleID).rgb;
        \\      color = color / (color + 1.0);
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_postprocess, 0, xcommon.display_texh);
}
