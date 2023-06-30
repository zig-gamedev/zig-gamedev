const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl");
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0022";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_postprocess: gl.Uint = 0;
var accum_tex: gl.Uint = 0;
var accum_fbo: gl.Uint = 0;

const a1 = -2.1;
const a2 = 1.4;
const a3 = 1.1;

const f1 = 0.4;
const f2 = 1.1;
const f3 = 1.0;

const a4 = 1.1;
const a5 = 1.2;
const a6 = 0.9;

const f4 = 1.1;
const f5 = 1.0;
const f6 = 0.7;

const v = 0.15;

var xn: f64 = 0.0;
var yn: f64 = 0.0;
var tn: f64 = 0.0;

var iter: u64 = 0;

pub fn draw() void {
    gl.enable(gl.BLEND);
    gl.bindFramebuffer(gl.DRAW_FRAMEBUFFER, accum_fbo);
    gl.useProgram(0);

    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -3.0, 3.0, -2.5, 3.5, -1.0, 1.0);

    gl.loadIdentity();

    gl.color3f(0.01, 0.01, 0.01);
    gl.begin(gl.POINTS);
    for (0..10_000) |_| {
        if (iter >= 2_000_000) break;

        gl.vertex2d(xn, yn);

        const xn1 = a1 * @sin(f1 * xn) + a2 * @cos(f2 * yn) + a3 * @sin(f3 * tn);
        const yn1 = a4 * @cos(f4 * xn) + a5 * @sin(f5 * yn) + a6 * @cos(f6 * tn);
        const tn1 = @as(f64, @floatFromInt(iter)) * v;

        xn = xn1;
        yn = yn1;
        tn = tn1;

        iter += 1;
    }
    gl.end();

    gl.disable(gl.BLEND);
    gl.bindFramebuffer(gl.DRAW_FRAMEBUFFER, xcommon.display_fbo);
    gl.useProgram(fs_postprocess);
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.loadIdentity();
    gl.begin(gl.TRIANGLES);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);
    gl.end();
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.pointSize(1.0);

    gl.blendFunc(gl.ONE, gl.ONE);
    gl.blendEquation(gl.FUNC_ADD);

    gl.createTextures(gl.TEXTURE_2D, 1, &accum_tex);
    gl.textureStorage2D(accum_tex, 1, gl.RGBA16F, display_width, display_height);
    gl.clearTexImage(accum_tex, 0, gl.RGBA, gl.FLOAT, null);

    gl.createFramebuffers(1, &accum_fbo);
    gl.namedFramebufferTexture(accum_fbo, gl.COLOR_ATTACHMENT0, accum_tex, 0);

    const accum_texh = gl.getTextureHandleNV(accum_tex);
    gl.makeTextureHandleResidentNV(accum_texh);

    fs_postprocess = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  #extension NV_bindless_texture : require
        \\
        \\  layout(location = 0) uniform sampler2D accum_texh;
        \\
        \\  void main() {
        \\      vec3 color = texelFetch(accum_texh, ivec2(gl_FragCoord.xy), 0).rgb;
        \\      color = color / (color + 1.0);
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_postprocess, 0, accum_texh);
}
