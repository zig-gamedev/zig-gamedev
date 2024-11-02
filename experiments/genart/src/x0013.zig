const std = @import("std");
const math = std.math;
const sdl = @import("zsdl2");
const gl = @import("zopengl").bindings;
const xcommon = @import("xcommon");

pub const name = "generative art experiment: x0013";
pub const display_width = 1024 * 1;
pub const display_height = 1024 * 1;

var fs_postprocess: gl.Uint = 0;
var prng = std.Random.DefaultPrng.init(0);
var random = prng.random();

const Particle = struct {
    x: f32 = 0.0,
    y: f32 = 0.0,
    frame: u32 = 0,
    num_frames: u32 = 50,
    angle: f32 = 0.0,
    angle_delta: f32 = 0.0,
    color: [3]f32 = .{ 1, 1, 1 },
};

var particles = [_]Particle{.{}} ** 256;
var frame: u32 = 0;

pub fn draw() void {
    const angle_accel: f32 = if (frame % 240 < 100) 0.0 else 2.0;
    frame += 1;

    for (&particles) |*p| {
        p.angle += p.angle_delta + angle_accel;
        if (p.angle > 360.0 or p.angle < -360.0) p.angle = 0.0;

        if (p.frame == p.num_frames) {
            p.frame = 0;
            p.angle_delta = -p.angle_delta;
        }
        p.frame += 1;
    }

    gl.enable(gl.BLEND);
    gl.useProgram(0);
    gl.loadIdentity();
    for (particles) |p| {
        gl.pointSize(10.0 + 10.0 * random.float(f32));
        gl.pushMatrix();
        gl.rotatef(p.angle, 0, 0, 1);
        gl.begin(gl.POINTS);
        gl.color3fv(&p.color);
        gl.vertex2f(
            p.x + 0.025 * (-1.0 + 2.0 * random.float(f32)),
            p.y + 0.025 * (-1.0 + 2.0 * random.float(f32)),
        );
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
    for (&particles) |*p| {
        p.x = -0.9 + 1.8 * random.float(f32);
        p.y = -0.9 + 1.8 * random.float(f32);
        p.angle_delta = 0.5 + random.float(f32);
        p.num_frames = 20 + random.uintLessThan(u32, 100);
        p.color = switch (random.uintAtMost(u2, 3)) {
            0 => .{ 1, 0.005, 0 },
            1 => .{ 1, 1, 0.005 },
            2 => .{ 0, 1, 0.005 },
            3 => .{ 0, 0.02, 1 },
        };
    }

    try sdl.gl.setSwapInterval(1);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.0, 0.0, 0.0, 0.0 });
    gl.matrixLoadIdentityEXT(gl.PROJECTION);
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
