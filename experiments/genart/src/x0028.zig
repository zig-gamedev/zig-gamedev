const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl");
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0028";
pub const display_width = 1024 * res_mul;
pub const display_height = 1024 * res_mul;

const res_mul = 1; // 1 (1024x1024) or 2 (2048x2048)

var prng = std.rand.DefaultPrng.init(0);
var random = prng.random();

var fs_postprocess: gl.Uint = 0;
var accum_tex: gl.Uint = 0;
var accum_fbo: gl.Uint = 0;

const a1 = 1.1;
const a2 = 1.4;
const a3 = 1.1;

const f1 = 0.4;
const f2 = 1.1;
const f3 = 1.0;

const a4 = 1.7;
const a5 = 1.2;
const a6 = 0.9;

const f4 = 0.75;
const f5 = 0.1;
const f6 = 0.7;

var v: f64 = 0.15;

var xn: f64 = 0.0;
var yn: f64 = 0.0;
var tn: f64 = 0.0;

const max_iter = 25_000_000 * res_mul * res_mul;
var iter: u64 = 0;
var frame: u32 = 0;

pub fn draw() void {
    gl.bindFramebuffer(gl.FRAMEBUFFER, accum_fbo);
    gl.useProgram(0);

    if (false and iter >= max_iter) {
        iter = 0;
        frame = 0;
        v += 0.001;
        xn = 0;
        yn = 0;
        tn = 0;

        gl.color3f(0.0, 0.0, 0.0);
        gl.rectf(-1.0, -1.0, 1.0, 1.0);

        const static = struct {
            var num: u32 = 0;
        };
        var buffer = [_]u8{0} ** 128;
        const filename = std.fmt.bufPrintZ(
            buffer[0..],
            "x0028_f{d:0>4}.png",
            .{static.num},
        ) catch unreachable;
        static.num += 1;

        gl.bindFramebuffer(gl.FRAMEBUFFER, 0);
        xcommon.saveScreenshot(xcommon.allocator, filename);
        gl.bindFramebuffer(gl.FRAMEBUFFER, accum_fbo);
    }

    gl.enable(gl.BLEND);

    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.loadIdentity();

    gl.enable(gl.SCISSOR_TEST);
    gl.scissor(
        100 * res_mul,
        100 * res_mul,
        display_width - 2 * 100 * res_mul,
        display_height - 2 * 100 * res_mul,
    );

    if (frame == 0) {
        gl.color3f(0.25, 0.25, 0.25);
        gl.rectf(-1.0, -1.0, 1.0, 1.0);
    }

    gl.matrixOrthoEXT(gl.PROJECTION, -3.5, 3.5, -3.5, 3.5, -1.0, 1.0);

    gl.begin(gl.POINTS);
    for (0..50_000) |_| {
        if (iter >= max_iter) break;

        if (xn > yn)
            gl.color3f(0.01, 0.01, 0.01)
        else
            gl.color3f(0.01, 0.01, 0.0001);

        gl.vertex2d(xn, yn);

        const r0 = random.float(f64) * 0.000;
        const r1 = random.float(f64) * 0.000;

        const xn1 = a1 * @sin((f1 + r0) * xn) + a2 * @cos((f2 + r0) * yn) + a3 * @sin(f3 * tn);
        const yn1 = a4 * @cos((f4 + r1) * xn) + a5 * @sin((f5 + r1) * yn) + a6 * @cos(f6 * tn);
        const tn1 = @as(f64, @floatFromInt(iter)) * v;

        xn = xn1;
        yn = yn1;
        tn = tn1;

        iter += 1;
    }
    gl.end();
    gl.disable(gl.SCISSOR_TEST);

    gl.disable(gl.BLEND);
    gl.bindFramebuffer(gl.FRAMEBUFFER, xcommon.display_fbo);
    gl.useProgram(fs_postprocess);
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.loadIdentity();
    gl.begin(gl.TRIANGLES);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);
    gl.end();

    frame += 1;
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.pointSize(1.0);
    gl.lineWidth(3.0);

    gl.blendFunc(gl.ONE, gl.ONE);
    gl.blendEquation(gl.FUNC_ADD);

    gl.createTextures(gl.TEXTURE_2D_MULTISAMPLE, 1, &accum_tex);
    gl.textureStorage2DMultisample(accum_tex, 8, gl.RGBA16F, display_width, display_height, gl.FALSE);
    gl.clearTexImage(accum_tex, 0, gl.RGBA, gl.FLOAT, null);

    gl.createFramebuffers(1, &accum_fbo);
    gl.namedFramebufferTexture(accum_fbo, gl.COLOR_ATTACHMENT0, accum_tex, 0);

    const accum_texh = gl.getTextureHandleNV(accum_tex);
    gl.makeTextureHandleResidentNV(accum_texh);

    fs_postprocess = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  #extension NV_bindless_texture : require
        \\
        \\  layout(location = 0) uniform sampler2DMS accum_texh;
        \\
        \\  void main() {
        \\      vec3 color = texelFetch(accum_texh, ivec2(gl_FragCoord.xy), gl_SampleID).rgb;
        \\      color = color / (color + 1.0);
        \\      color = 1.0 - color;
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_postprocess, 0, accum_texh);
}
