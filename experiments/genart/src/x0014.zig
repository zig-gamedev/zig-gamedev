const std = @import("std");
const math = std.math;
const sdl = @import("zsdl2");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0014";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

const Vec2 = [2]f32;

var fs_postprocess: gl.Uint = 0;
var angle: f32 = 0.0;
var accum_tex: gl.Uint = 0;
var accum_fbo: gl.Uint = 0;
var prng = std.rand.DefaultPrng.init(0);
var random = prng.random();

const bounds: f32 = 3.0;
var y = -bounds;
var pass: u32 = 1;

pub fn draw() void {
    gl.enable(gl.BLEND);
    gl.bindFramebuffer(gl.DRAW_FRAMEBUFFER, accum_fbo);
    gl.useProgram(0);

    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -bounds, bounds, -bounds, bounds, -1.0, 1.0);

    gl.loadIdentity();
    gl.rotatef(angle, 0, 0, 1);

    angle += 0.15;
    if (angle > 360.0) angle = 0.0;

    if (y <= bounds and pass == 0) {
        gl.begin(gl.POINTS);
        const step: f32 = 0.001;
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = random.floatNorm(f32) * 0.002;
                const yoff = random.floatNorm(f32) * 0.002;
                gl.color3f(0.002, 0.002, 0.002);
                gl.vertex2f(x + xoff, y + yoff);
            }
            y += step;
        }
        gl.end();
    } else if (y <= bounds and pass == 1) {
        gl.begin(gl.POINTS);
        const step: f32 = 0.001;
        var row: u32 = 0;
        while (row < 4) : (row += 1) {
            var x: f32 = -bounds;
            while (x <= bounds) : (x += step) {
                var v = Vec2{ x, y };
                const xoff = random.floatNorm(f32) * 0.01;
                const yoff = random.floatNorm(f32) * 0.01;
                v = pdj(v, 2.5);
                //v = julia(v, 1.5, random.float(f32));
                v = hyperbolic(v, 1.0);
                //v = sinusoidal(v, 2.2);
                gl.color3f(0.001, 0.001, 0.001);
                gl.vertex2f(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
    }

    if (y >= bounds) {
        y = -bounds;
        pass += 1;
    }

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

    gl.pointSize(3.0);

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
        \\      color = 1.0 - color;
        \\      color = pow(color, vec3(2.2));
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_postprocess, 0, accum_texh);
}

fn sinusoidal(v: Vec2, scale: f32) Vec2 {
    return .{ scale * math.sin(v[0]), scale * math.sin(v[1]) };
}

fn hyperbolic(v: Vec2, scale: f32) Vec2 {
    const r = @sqrt(v[0] * v[0] + v[1] * v[1]) + 0.0001;
    const theta = math.atan2(v[0], v[1]);
    const xx = scale * math.sin(theta) / r;
    const yy = scale * math.cos(theta) * r;
    return .{ xx, yy };
}

fn pdj(v: Vec2, scale: f32) Vec2 {
    const pdj_a = 0.1;
    const pdj_b = 1.9;
    const pdj_c = -0.8;
    const pdj_d = -1.2;
    //const pdj_a = 1.0111;
    //const pdj_b = -1.011;
    //const pdj_c = 2.08;
    //const pdj_d = 10.2;
    return .{
        scale * (math.sin(pdj_a * v[1]) - math.cos(pdj_b * v[0])),
        scale * (math.sin(pdj_c * v[0]) - math.cos(pdj_d * v[1])),
    };
}

fn julia(v: Vec2, scale: f32, rand01: f32) Vec2 {
    const r = scale * @sqrt(@sqrt(v[0] * v[0] + v[1] * v[1]));
    const theta = 0.5 * math.atan2(v[0], v[1]) +
        math.pi * @as(f32, @floatFromInt(@as(i32, @intFromFloat(2.0 * rand01))));
    const xx = r * math.cos(theta);
    const yy = r * math.sin(theta);
    return .{ xx, yy };
}
