const std = @import("std");
const math = std.math;
const sdl = @import("zsdl2");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0010";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_postprocess: gl.Uint = 0;
var angle: f32 = 0.0;
var prng = std.Random.DefaultPrng.init(123);
var random = prng.random();

const Particle = struct {
    x: f32 = 0.0,
    y: f32 = 0.0,
    xy_delta: f32 = 0.01,
    frame: u32 = 0,
    num_frames: u32 = 50,
    step: u32 = 0,
    angle: f32 = 0.0,
    angle_delta: f32 = 0.0,
    color: [3]f32 = .{ 1.0, 1.0, 1.0 },
};

var particles = [_]Particle{
    .{ .x = 0.0, .y = 0.0, .num_frames = 50, .color = .{ 1.0, 0.01, 0.0 } },
    .{ .x = 0.0, .y = 0.0, .num_frames = 60, .color = .{ 0.01, 1.0, 0.0 } },
    .{ .x = 0.0, .y = 0.0, .num_frames = 70, .color = .{ 0.0, 0.01, 1.0 } },
    .{ .x = -0.7, .y = -0.7, .num_frames = 100, .color = .{ 1.0, 1.0, 0.01 } },
    .{ .x = -0.2, .y = -0.7, .num_frames = 60, .color = .{ 0.01, 1.0, 1.0 } },
    .{ .x = 0.0, .y = -0.7, .num_frames = 70, .angle_delta = 0.05, .color = .{ 1.0, 1.0, 1.0 } },
    .{ .x = -0.7, .y = -0.7, .num_frames = 110, .color = .{ 1.0, 0.01, 1.0 } },
    .{ .x = -0.7, .y = 0.1, .num_frames = 70, .angle_delta = 0.1, .color = .{ 1.0, 0.0, 0.01 } },
};

pub fn draw() void {
    for (&particles) |*p| {
        p.angle += p.angle_delta;
        if (p.angle > 360.0) p.angle = 0.0;

        if (p.step == 0) {
            p.x += p.xy_delta;
        } else if (p.step == 1) {
            p.y += p.xy_delta;
        } else if (p.step == 2) {
            p.x -= p.xy_delta;
        } else if (p.step == 3) {
            p.y -= p.xy_delta;
        }

        if (p.frame == p.num_frames) {
            p.frame = 0;
            p.step += 1;
            if (p.step == 4) p.step = 0;
        }
        p.frame += 1;
    }

    angle += 0.2;
    if (angle > 360.0) angle = 0.0;

    gl.enable(gl.BLEND);
    gl.useProgram(0);

    gl.loadIdentity();
    gl.rotatef(angle, 0, 0, 1);

    for (particles) |p| {
        gl.color3fv(&p.color);
        gl.pushMatrix();
        gl.rotatef(p.angle, 0, 0, 1);
        gl.begin(gl.POINTS);
        gl.vertex2f(p.x + 0.025 * random.float(f32), p.y + 0.025 * random.float(f32));
        gl.end();
        gl.popMatrix();
    }

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
    gl.pointSize(25.0);
    gl.blendFunc(gl.ONE, gl.ONE);
    //gl.disable(gl.MULTISAMPLE);

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
