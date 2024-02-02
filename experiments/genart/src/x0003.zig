const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0003";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_draw: gl.Uint = 0;
var fs_postprocess: gl.Uint = 0;
var rot: f32 = 0;

pub fn draw() void {
    const bounds: f32 = 6.0;
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -bounds, bounds, -bounds, bounds, -1.0, 1.0);

    gl.loadIdentity();
    gl.rotatef(rot, 0.0, 0.0, 1.0);
    rot += 0.2;
    if (rot > 360.0) rot = 0.0;

    gl.useProgram(fs_draw);
    gl.begin(gl.TRIANGLES);

    gl.color3f(1.0, 0.0, 0.0);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);

    gl.color3f(0.0, 1.0, 0.0);
    gl.vertex2f(0.0, -1.0);
    gl.vertex2f(4.0, -1.0);
    gl.vertex2f(0.0, 3.0);

    gl.color3f(0.0, 0.0, 1.0);
    gl.vertex2f(1.0, -1.0);
    gl.vertex2f(5.0, -1.0);
    gl.vertex2f(1.0, 3.0);

    gl.end();
    gl.textureBarrier();

    gl.loadIdentity();
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.useProgram(fs_postprocess);
    gl.begin(gl.TRIANGLES);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);
    gl.end();
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });

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
