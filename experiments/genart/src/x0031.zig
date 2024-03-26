const std = @import("std");
const math = std.math;
const sdl = @import("zsdl2");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0031";
pub const display_width = 1024 * res_mul;
pub const display_height = 1024 * res_mul;

const res_mul = 1; // 1 (1024x1024) or 2 (2048x2048)

const step: f64 = if (res_mul == 2) 0.001 else 0.002;
const scale: f32 = 0.3;
const rand_factor = step * 0.05 * res_mul;
const luminance = 0.0005;

const Vec2 = [2]f64;

var fs_postprocess: gl.Uint = 0;
var accum_tex: gl.Uint = 0;
var accum_fbo: gl.Uint = 0;
var prng = std.rand.DefaultPrng.init(0);
var random = prng.random();

const bounds: f64 = 3.0;
var y = -bounds;
var pass: u32 = 0;

pub fn draw() void {
    gl.enable(gl.BLEND);
    gl.bindFramebuffer(gl.FRAMEBUFFER, accum_fbo);
    gl.useProgram(0);

    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -3.0, 3.0, -3.0, 3.0, -1.0, 1.0);

    gl.loadIdentity();

    if (y <= bounds and pass == 0) {
        gl.pointSize(3.0);
        const s = 0.0025 * res_mul * res_mul;
        gl.color3f(s * (1.0 - 0.44), s * (1.0 - 0.26), s * (1.0 - 0.08));
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += 0.001) {
                const xoff = 0.00025 * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = 0.00025 * (-1.0 + 2.0 * random.floatNorm(f64));
                const v = Vec2{ x, y };

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += 0.001;
        }
        gl.end();
        gl.pointSize(1.0);
        gl.color3f(luminance, luminance, luminance);
    }

    if (y <= bounds and pass == 1) {
        gl.pushMatrix();
        gl.translatef(-2.0, -2.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 2) {
        gl.pushMatrix();
        gl.translatef(0.0, -2.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = hyperbolic(v, 1.0);
                v = julia(v, 2.0, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 3) {
        gl.pushMatrix();
        gl.translatef(2.0, -2.0, 0.0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = hyperbolic(v, 1.0);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 4) {
        gl.pushMatrix();
        gl.translatef(-2.0, 0.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 5) {
        gl.pushMatrix();
        gl.translatef(0.0, 0.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[1], v[0] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 6) {
        gl.pushMatrix();
        gl.translatef(2.0, 0.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[1], v[0] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 7) {
        gl.pushMatrix();
        gl.translatef(-2.0, 2.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.0, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[1], v[0] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[1], v[0] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 8) {
        gl.pushMatrix();
        gl.translatef(0.0, 2.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = julia(v, 2.2, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = sinusoidal(v, 2.8);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.1, random.float(f64));
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
    }

    if (y <= bounds and pass == 9) {
        gl.pushMatrix();
        gl.translatef(2.0, 2.0, 0);
        gl.scalef(scale, scale, 1);
        gl.color3f(luminance, luminance, luminance);
        gl.begin(gl.POINTS);
        var row: u32 = 0;
        while (row < 16) : (row += 1) {
            var x: f64 = -bounds;
            while (x <= bounds) : (x += step) {
                const xoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                const yoff = rand_factor * (-1.0 + 2.0 * random.floatNorm(f64));
                var v = Vec2{ x, y };

                v = sinusoidal(v, 2.8);
                v = julia(v, 2.1, random.float(f64));
                v = hyperbolic(v, 1.0);
                v = hyperbolic(.{ v[0], v[1] }, 1.0);
                v = sinusoidal(v, 2.8);
                v = julia(.{ v[0], v[1] }, 2.0, random.float(f64));
                v = sinusoidal(v, 2.8);
                v = sinusoidal(v, 2.8);

                gl.vertex2d(v[0] + xoff, v[1] + yoff);
            }
            y += step;
        }
        gl.end();
        gl.popMatrix();
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
        \\      color = clamp(color, 0.0, 1.0);
        \\      color = 1.0 - color;
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
    gl.programUniformHandleui64NV(fs_postprocess, 0, accum_texh);
}

fn sinusoidal(v: Vec2, s: f64) Vec2 {
    return .{ s * math.sin(v[0]), s * math.sin(v[1]) };
}

fn julia(v: Vec2, s: f64, rand01: f64) Vec2 {
    const r = s * @sqrt(@sqrt(v[0] * v[0] + v[1] * v[1]));
    const theta = 0.5 * math.atan2(v[0], v[1]) +
        math.pi * @as(f64, @floatFromInt(@as(i32, @intFromFloat(2.0 * rand01))));
    const xx = r * math.cos(theta);
    const yy = r * math.sin(theta);
    return .{ xx, yy };
}

fn hyperbolic(v: Vec2, s: f64) Vec2 {
    const r = @sqrt(v[0] * v[0] + v[1] * v[1]) + 0.0001;
    const theta = math.atan2(v[0], v[1]);
    const xx = s * math.sin(theta) / r;
    const yy = s * math.cos(theta) * r;
    return .{ xx, yy };
}
