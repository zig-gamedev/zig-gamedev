const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl");
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0008";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_draw: gl.Uint = 0;
var fs_postprocess: gl.Uint = 0;
var rot: f32 = 0;

pub fn draw() void {
    rot += 0.3;
    if (rot > 360.0) rot = 0.0;

    gl.useProgram(fs_draw);

    const t = @as(f32, @floatCast(xcommon.frame_time));
    const r = @sin(t);

    gl.loadIdentity();
    gl.rotatef(0.5 * rot, 0.0, 0.0, 1.0);

    gl.color3f(1.0, 0.1, 0.05);
    gl.pushMatrix();
    gl.rotatef(rot, 0.0, 0.0, 1.0);
    gl.begin(gl.POINTS);
    for (0..10) |i| {
        const fract = @as(f32, @floatFromInt(i)) / 10.0;
        const x = r * @cos(math.tau * fract);
        const y = r * @sin(math.tau * fract);
        gl.vertex2f(x, y);
    }
    gl.end();
    gl.popMatrix();

    gl.textureBarrier();

    gl.color3f(0.05, 0.1, 1.0);
    gl.pushMatrix();
    gl.rotatef(90.0 * @sin(t), 0.0, 0.0, 1.0);
    gl.begin(gl.POINTS);
    for (0..10) |i| {
        const fract = @as(f32, @floatFromInt(i)) / 10.0;
        const x = r * @cos(math.tau * fract);
        const y = r * @sin(math.tau * fract);
        gl.vertex2f(x, y);
    }
    gl.end();
    gl.popMatrix();

    gl.textureBarrier();

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
    gl.pointSize(15.0);
    gl.matrixLoadIdentityEXT(gl.PROJECTION);

    fs_draw = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  #extension NV_bindless_texture : require
        \\
        \\  in gl_PerFragment {
        \\      vec4 gl_Color;
        \\  };
        \\
        \\  layout(location = 0) uniform sampler2DMS display_texh;
        \\
        \\  void main() {
        \\      vec4 color = texelFetch(display_texh, ivec2(gl_FragCoord.xy), gl_SampleID);
        \\      gl_FragColor = color + gl_Color;
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_draw, 0, xcommon.display_texh);

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
