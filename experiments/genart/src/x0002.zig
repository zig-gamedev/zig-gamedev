const std = @import("std");
const math = std.math;
const sdl = @import("zsdl2");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0002";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

const Vec2 = [2]f32;
const bounds: f32 = 3.0;

var prng = std.rand.DefaultPrng.init(0);
var random = prng.random();
var pass: u32 = 0;
var y: f32 = -bounds;
var fs_postprocess: gl.Uint = 0;

pub fn draw() void {
    gl.enable(gl.BLEND);

    if (y <= bounds and pass <= 3) {
        gl.begin(gl.POINTS);
        const step: f32 = 0.001;
        var row: u32 = 0;
        while (row < 4) : (row += 1) {
            var x: f32 = -bounds;
            while (x <= bounds) : (x += step) {
                var v = Vec2{ x, y };

                v = hyperbolic(v, 1.0);
                v = julia(v, 2.5, random.float(f32));
                v = pdj(v, 2.0);
                v = sinusoidal(v, 2.5);

                gl.color4f(0.002 * x * x, 0.0002, 0.001 * y * y, 0.0);
                gl.vertex2f(v[0], v[1]);
            }
            y += step;
        }
        gl.end();
    }

    if (y >= bounds) {
        y = -bounds;
        pass += 1;
    }

    gl.textureBarrier();

    gl.disable(gl.BLEND);

    gl.useProgram(fs_postprocess);
    gl.bindTextureUnit(0, xcommon.display_tex);
    gl.begin(gl.QUADS);
    gl.vertex2f(-bounds, -bounds);
    gl.vertex2f(bounds, -bounds);
    gl.vertex2f(bounds, bounds);
    gl.vertex2f(-bounds, bounds);
    gl.end();
    gl.bindTextureUnit(0, 0);
    gl.useProgram(0);
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });
    gl.pointSize(5.0);
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.ONE, gl.ONE);
    gl.blendEquation(gl.FUNC_ADD);
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -bounds, bounds, -bounds, bounds, -1.0, 1.0);

    fs_postprocess = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  layout(binding = 0) uniform sampler2DMS display_tex;
        \\
        \\  void main() {
        \\      vec3 color = texelFetch(display_tex, ivec2(gl_FragCoord.xy), gl_SampleID).rgb;
        \\      color = color / (color + 1.0);
        \\      gl_FragColor = vec4(color, 1.0);
        \\  }
    ));
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
