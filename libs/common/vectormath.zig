const std = @import("std");
const assert = std.debug.assert;
const math = std.math;

pub fn modAngle(in_angle: f32) f32 {
    const angle = in_angle + math.pi;
    var temp: f32 = math.fabs(angle);
    temp = temp - (2.0 * math.pi * @intToFloat(f32, @floatToInt(i32, temp / math.pi)));
    temp = temp - math.pi;
    if (angle < 0.0) {
        temp = -temp;
    }
    return temp;
}

pub const Vec2 = struct {
    v: [2]f32,

    pub fn init(x: f32, y: f32) Vec2 {
        return .{ .v = [_]f32{ x, y } };
    }
};

pub const Vec3 = struct {
    v: [3]f32,

    pub fn dot(a: Vec3, b: Vec3) f32 {
        return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
    }

    pub fn cross(a: Vec3, b: Vec3) Vec3 {
        return .{
            .v = [_]f32{
                a.v[1] * b.v[2] - a.v[2] * b.v[1],
                a.v[2] * b.v[0] - a.v[0] * b.v[2],
                a.v[0] * b.v[1] - a.v[1] * b.v[0],
            },
        };
    }

    pub fn add(a: Vec3, b: Vec3) Vec3 {
        return .{ .v = [_]f32{ a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2] } };
    }

    pub fn sub(a: Vec3, b: Vec3) Vec3 {
        return .{ .v = [_]f32{ a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2] } };
    }

    pub fn scale(a: Vec3, b: f32) Vec3 {
        return .{ .v = [_]f32{ a.v[0] * b, a.v[1] * b, a.v[2] * b } };
    }

    pub fn init(x: f32, y: f32, z: f32) Vec3 {
        return .{ .v = [_]f32{ x, y, z } };
    }

    pub fn length(a: Vec3) f32 {
        return math.sqrt(dot(a, a));
    }

    pub fn normalize(a: Vec3) Vec3 {
        const len = length(a);
        assert(!math.approxEq(f32, len, 0.0, 0.0001));
        const rcplen = 1.0 / len;
        return .{ .v = [_]f32{ rcplen * a.v[0], rcplen * a.v[1], rcplen * a.v[2] } };
    }

    pub fn transform(a: Vec3, b: Mat4) Vec3 {
        return .{
            .v = [_]f32{
                a.v[0] * b.v[0][0] + a.v[1] * b.v[1][0] + a.v[2] * b.v[2][0] + b.v[3][0],
                a.v[0] * b.v[0][1] + a.v[1] * b.v[1][1] + a.v[2] * b.v[2][1] + b.v[3][1],
                a.v[0] * b.v[0][2] + a.v[1] * b.v[1][2] + a.v[2] * b.v[2][2] + b.v[3][2],
            },
        };
    }

    pub fn transformNormal(a: Vec3, b: Mat4) Vec3 {
        return .{
            .v = [_]f32{
                a.v[0] * b.v[0][0] + a.v[1] * b.v[1][0] + a.v[2] * b.v[2][0],
                a.v[0] * b.v[0][1] + a.v[1] * b.v[1][1] + a.v[2] * b.v[2][1],
                a.v[0] * b.v[0][2] + a.v[1] * b.v[1][2] + a.v[2] * b.v[2][2],
            },
        };
    }
};

pub const Mat4 = struct {
    m: [4][4]f32,

    pub fn transpose(a: Mat4) Mat4 {
        return .{
            .m = [_][4]f32{
                [_]f32{ a.m[0][0], a.m[1][0], a.m[2][0], a.m[3][0] },
                [_]f32{ a.m[0][1], a.m[1][1], a.m[2][1], a.m[3][1] },
                [_]f32{ a.m[0][2], a.m[1][2], a.m[2][2], a.m[3][2] },
                [_]f32{ a.m[0][3], a.m[1][3], a.m[2][3], a.m[3][3] },
            },
        };
    }

    pub fn mul(a: Mat4, b: Mat4) Mat4 {
        return .{
            .m = [_][4]f32{
                [_]f32{
                    a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0],
                    a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1],
                    a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2],
                    a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3],
                },
                [_]f32{
                    a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0],
                    a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1],
                    a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2],
                    a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3],
                },
                [_]f32{
                    a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0],
                    a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1],
                    a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2],
                    a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3],
                },
                [_]f32{
                    a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0],
                    a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1],
                    a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2],
                    a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3],
                },
            },
        };
    }

    pub fn initRotationX(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        return .{
            .m = [_][4]f32{
                [_]f32{ 1.0, 0.0, 0.0, 0.0 },
                [_]f32{ 0.0, cosv, sinv, 0.0 },
                [_]f32{ 0.0, -sinv, cosv, 0.0 },
                [_]f32{ 0.0, 0.0, 0.0, 1.0 },
            },
        };
    }

    pub fn initRotationY(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        return .{
            .m = [_][4]f32{
                [_]f32{ cosv, 0.0, -sinv, 0.0 },
                [_]f32{ 0.0, 1.0, 0.0, 0.0 },
                [_]f32{ sinv, 0.0, cosv, 0.0 },
                [_]f32{ 0.0, 0.0, 0.0, 1.0 },
            },
        };
    }

    pub fn initRotationZ(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        return .{
            .m = [_][4]f32{
                [_]f32{ cosv, sinv, 0.0, 0.0 },
                [_]f32{ -sinv, cosv, 0.0, 0.0 },
                [_]f32{ 0.0, 0.0, 1.0, 0.0 },
                [_]f32{ 0.0, 0.0, 0.0, 1.0 },
            },
        };
    }

    pub fn initPerspectiveFovLh(fovy: f32, aspect: f32, near: f32, far: f32) Mat4 {
        const sinfov = math.sin(0.5 * fovy);
        const cosfov = math.cos(0.5 * fovy);

        assert(near > 0.0 and far > 0.0 and far > near);
        assert(!math.approxEq(f32, sinfov, 0.0, 0.0001));
        assert(!math.approxEq(f32, far, near, 0.001));
        assert(!math.approxEq(f32, aspect, 0.0, 0.01));

        const h = cosfov / sinfov;
        const w = h / aspect;
        const r = far / (far - near);
        return .{
            .m = [_][4]f32{
                [_]f32{ w, 0.0, 0.0, 0.0 },
                [_]f32{ 0.0, h, 0.0, 0.0 },
                [_]f32{ 0.0, 0.0, r, 1.0 },
                [_]f32{ 0.0, 0.0, -r * near, 0.0 },
            },
        };
    }

    pub inline fn initIdentity() Mat4 {
        const static = struct {
            const identity = Mat4{
                .m = [_][4]f32{
                    [_]f32{ 1.0, 0.0, 0.0, 0.0 },
                    [_]f32{ 0.0, 1.0, 0.0, 0.0 },
                    [_]f32{ 0.0, 0.0, 1.0, 0.0 },
                    [_]f32{ 0.0, 0.0, 0.0, 1.0 },
                },
            };
        };
        return static.identity;
    }

    pub fn initTranslation(a: Vec3) Mat4 {
        return .{
            .m = [_][4]f32{
                [_]f32{ 1.0, 0.0, 0.0, 0.0 },
                [_]f32{ 0.0, 1.0, 0.0, 0.0 },
                [_]f32{ 0.0, 0.0, 1.0, 0.0 },
                [_]f32{ a[0], a[1], a[2], 1.0 },
            },
        };
    }

    pub fn initLookAtLh(eye: Vec3, at: Vec3, up: Vec3) Mat4 {
        const az = Vec3.normalize(Vec3.sub(at, eye));
        const ax = Vec3.normalize(Vec3.cross(up, az));
        const ay = Vec3.normalize(Vec3.cross(az, ax));
        return .{
            .m = [_][4]f32{
                [_]f32{ ax.v[0], ay.v[0], az.v[0], 0.0 },
                [_]f32{ ax.v[1], ay.v[1], az.v[1], 0.0 },
                [_]f32{ ax.v[2], ay.v[2], az.v[2], 0.0 },
                [_]f32{ -Vec3.dot(ax, eye), -Vec3.dot(ay, eye), -Vec3.dot(az, eye), 1.0 },
            },
        };
    }

    pub fn initOrthoOffCenterLh(
        view_left: f32,
        view_right: f32,
        view_bottom: f32,
        view_top: f32,
        near_z: f32,
        far_z: f32,
    ) Mat4 {
        assert(!math.approxEq(f32, view_right, view_left, 0.00001));
        assert(!math.approxEq(f32, view_top, view_bottom, 0.00001));
        assert(!math.approxEq(f32, far_z, near_z, 0.00001));

        const rcp_width = 1.0 / (view_right - view_left);
        const rcp_height = 1.0 / (view_top - view_bottom);
        const range = 1.0 / (far_z - near_z);

        return .{
            .m = [_][4]f32{
                [_]f32{ rcp_width + rcp_width, 0.0, 0.0, 0.0 },
                [_]f32{ 0.0, rcp_height + rcp_height, 0.0, 0.0 },
                [_]f32{ 0.0, 0.0, range, 0.0 },
                [_]f32{ -(view_left + view_right) * rcp_width, -(view_top + view_bottom) * rcp_height, -range * near_z, 1.0 },
            },
        };
    }
};
