const std = @import("std");
const math = std.math;
const sdl = @import("zsdl");
const gl = @import("zopengl");
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0003";
pub const viewport_width = 1024 * 1;
pub const viewport_height = 1024 * 1;

const Vec2 = [2]f32;
const bounds: f32 = 6.0;

var prng = std.rand.DefaultPrng.init(0);
var random = prng.random();
var pass: u32 = 0;
var y: f32 = -bounds;
var fs_postprocess: gl.Uint = 0;
var rot: f32 = 0;

pub fn draw() void {
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.matrixOrthoEXT(gl.PROJECTION, -bounds, bounds, -bounds, bounds, -1.0, 1.0);
    gl.enable(gl.BLEND);

    gl.loadIdentity();
    gl.rotatef(rot, 0.0, 0.0, 1.0);
    rot += 0.2;
    if (rot > 360.0) rot = 0.0;

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

    gl.disable(gl.BLEND);

    gl.loadIdentity();
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
    gl.useProgram(fs_postprocess);
    gl.begin(gl.TRIANGLES);
    gl.vertex2f(-1.0, -1.0);
    gl.vertex2f(3.0, -1.0);
    gl.vertex2f(-1.0, 3.0);
    gl.end();
    gl.useProgram(0);
}

pub fn init() !void {
    try sdl.gl.setSwapInterval(1);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });
    gl.pointSize(1.0);
    gl.blendFunc(gl.ONE, gl.ONE);
    gl.blendEquation(gl.FUNC_ADD);

    fs_postprocess = gl.createShaderProgramv(gl.FRAGMENT_SHADER, 1, &@as([*:0]const gl.Char, 
        \\  #version 460 compatibility
        \\  #extension NV_bindless_texture : require
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

fn sinusoidal(v: Vec2, scale: f32) Vec2 {
    return .{ scale * math.sin(v[0]), scale * math.sin(v[1]) };
}

fn hyperbolic(v: Vec2, scale: f32) Vec2 {
    const r = @sqrt(v[0] * v[0] + v[1] * v[1]) + 0.0001;
    const theta = math.atan2(f32, v[0], v[1]);
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
    const theta = 0.5 * math.atan2(f32, v[0], v[1]) +
        math.pi * @intToFloat(f32, @floatToInt(i32, 2.0 * rand01));
    const xx = r * math.cos(theta);
    const yy = r * math.sin(theta);
    return .{ xx, yy };
}
