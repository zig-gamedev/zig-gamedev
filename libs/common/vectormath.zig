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

pub const Vec2 = extern struct {
    v: [2]f32,

    pub inline fn init(x: f32, y: f32) Vec2 {
        return .{ .v = [_]f32{ x, y } };
    }

    pub inline fn initZero() Vec2 {
        const static = struct {
            const zero = init(0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn approxEq(a: Vec2, b: Vec2, eps: f32) bool {
        return math.approxEq(f32, a.v[0], b.v[0], eps) and
            math.approxEq(f32, a.v[1], b.v[1], eps);
    }

    pub inline fn add(a: Vec2, b: Vec2) Vec2 {
        return .{ .v = [_]f32{ a.v[0] + b.v[0], a.v[1] + b.v[1] } };
    }

    pub inline fn sub(a: Vec2, b: Vec2) Vec2 {
        return .{ .v = [_]f32{ a.v[0] - b.v[0], a.v[1] - b.v[1] } };
    }

    pub inline fn scale(a: Vec2, b: f32) Vec2 {
        return .{ .v = [_]f32{ a.v[0] * b, a.v[1] * b } };
    }

    pub inline fn neg(a: Vec2) Vec2 {
        return .{ .v = [_]f32{ -a.v[0], -a.v[1] } };
    }

    pub inline fn mulAdd(a: Vec2, b: Vec2, c: Vec2) Vec2 {
        return .{ .v = [_]f32{
            a.v[0] * b.v[0] + c.v[0],
            a.v[1] * b.v[1] + c.v[1],
        } };
    }

    pub inline fn negMulAdd(a: Vec2, b: Vec2, c: Vec2) Vec2 {
        return .{ .v = [_]f32{
            a.v[0] * (-b.v[0]) + c.v[0],
            a.v[1] * (-b.v[1]) + c.v[1],
        } };
    }

    pub inline fn rcp(a: Vec2) Vec2 {
        assert(!math.approxEq(f32, a.v[0], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[1], 0.0, 0.0001));
        return .{ .v = [_]f32{ 1.0 / a.v[0], 1.0 / a.v[1] } };
    }

    pub inline fn dot(a: Vec2, b: Vec2) f32 {
        return a.v[0] * b.v[0] + a.v[1] * b.v[1];
    }

    pub inline fn length(a: Vec2) f32 {
        return math.sqrt(dot(a, a));
    }

    pub inline fn lengthSq(a: Vec2) f32 {
        return dot(a, a);
    }

    pub inline fn normalize(a: Vec2) Vec2 {
        const len = length(a);
        assert(!math.approxEq(f32, len, 0.0, 0.0001));
        const rcplen = 1.0 / len;
        return .{ .v = [_]f32{ rcplen * a.v[0], rcplen * a.v[1] } };
    }

    pub inline fn transform(a: Vec2, b: Mat4) Vec2 {
        return .{
            .v = [_]f32{
                a.v[0] * b.m[0][0] + a.v[1] * b.m[1][0] + b.m[3][0],
                a.v[0] * b.m[0][1] + a.v[1] * b.m[1][1] + b.m[3][1],
            },
        };
    }

    pub inline fn transformNormal(a: Vec2, b: Mat4) Vec2 {
        return .{
            .v = [_]f32{
                a.v[0] * b.m[0][0] + a.v[1] * b.m[1][0],
                a.v[0] * b.m[0][1] + a.v[1] * b.m[1][1],
            },
        };
    }
};

pub const Vec3 = extern struct {
    v: [3]f32,

    pub inline fn init(x: f32, y: f32, z: f32) Vec3 {
        return .{ .v = [_]f32{ x, y, z } };
    }

    pub inline fn initZero() Vec3 {
        const static = struct {
            const zero = init(0.0, 0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn approxEq(a: Vec3, b: Vec3, eps: f32) bool {
        return math.approxEq(f32, a.v[0], b.v[0], eps) and
            math.approxEq(f32, a.v[1], b.v[1], eps) and
            math.approxEq(f32, a.v[2], b.v[2], eps);
    }

    pub inline fn dot(a: Vec3, b: Vec3) f32 {
        return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
    }

    pub inline fn cross(a: Vec3, b: Vec3) Vec3 {
        return .{
            .v = [_]f32{
                a.v[1] * b.v[2] - a.v[2] * b.v[1],
                a.v[2] * b.v[0] - a.v[0] * b.v[2],
                a.v[0] * b.v[1] - a.v[1] * b.v[0],
            },
        };
    }

    pub inline fn add(a: Vec3, b: Vec3) Vec3 {
        return .{ .v = [_]f32{ a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2] } };
    }

    pub inline fn sub(a: Vec3, b: Vec3) Vec3 {
        return .{ .v = [_]f32{ a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2] } };
    }

    pub inline fn scale(a: Vec3, b: f32) Vec3 {
        return .{ .v = [_]f32{ a.v[0] * b, a.v[1] * b, a.v[2] * b } };
    }

    pub inline fn neg(a: Vec3) Vec3 {
        return .{ .v = [_]f32{ -a.v[0], -a.v[1], -a.v[2] } };
    }

    pub inline fn mulAdd(a: Vec3, b: Vec3, c: Vec3) Vec3 {
        return .{ .v = [_]f32{
            a.v[0] * b.v[0] + c.v[0],
            a.v[1] * b.v[1] + c.v[1],
            a.v[2] * b.v[2] + c.v[2],
        } };
    }

    pub inline fn negMulAdd(a: Vec3, b: Vec3, c: Vec3) Vec3 {
        return .{ .v = [_]f32{
            a.v[0] * (-b.v[0]) + c.v[0],
            a.v[1] * (-b.v[1]) + c.v[1],
            a.v[2] * (-b.v[2]) + c.v[2],
        } };
    }

    pub inline fn rcp(a: Vec3) Vec3 {
        assert(!math.approxEq(f32, a.v[0], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[1], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[2], 0.0, 0.0001));
        return .{ .v = [_]f32{ 1.0 / a.v[0], 1.0 / a.v[1], 1.0 / a.v[2] } };
    }

    pub inline fn length(a: Vec3) f32 {
        return math.sqrt(dot(a, a));
    }

    pub inline fn lengthSq(a: Vec3) f32 {
        return dot(a, a);
    }

    pub inline fn normalize(a: Vec3) Vec3 {
        const len = length(a);
        assert(!math.approxEq(f32, len, 0.0, 0.0001));
        const rcplen = 1.0 / len;
        return .{ .v = [_]f32{ rcplen * a.v[0], rcplen * a.v[1], rcplen * a.v[2] } };
    }

    pub inline fn transform(a: Vec3, b: Mat4) Vec3 {
        return .{
            .v = [_]f32{
                a.v[0] * b.m[0][0] + a.v[1] * b.m[1][0] + a.v[2] * b.m[2][0] + b.m[3][0],
                a.v[0] * b.m[0][1] + a.v[1] * b.m[1][1] + a.v[2] * b.m[2][1] + b.m[3][1],
                a.v[0] * b.m[0][2] + a.v[1] * b.m[1][2] + a.v[2] * b.m[2][2] + b.m[3][2],
            },
        };
    }

    pub inline fn transformNormal(a: Vec3, b: Mat4) Vec3 {
        return .{
            .v = [_]f32{
                a.v[0] * b.m[0][0] + a.v[1] * b.m[1][0] + a.v[2] * b.m[2][0],
                a.v[0] * b.m[0][1] + a.v[1] * b.m[1][1] + a.v[2] * b.m[2][1],
                a.v[0] * b.m[0][2] + a.v[1] * b.m[1][2] + a.v[2] * b.m[2][2],
            },
        };
    }
};

pub const Vec4 = extern struct {
    v: [4]f32,

    pub inline fn init(x: f32, y: f32, z: f32, w: f32) Vec4 {
        return .{ .v = [_]f32{ x, y, z, w } };
    }

    pub inline fn initZero() Vec4 {
        const static = struct {
            const zero = init(0.0, 0.0, 0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn approxEq(a: Vec4, b: Vec4, eps: f32) bool {
        return math.approxEq(f32, a.v[0], b.v[0], eps) and
            math.approxEq(f32, a.v[1], b.v[1], eps) and
            math.approxEq(f32, a.v[2], b.v[2], eps) and
            math.approxEq(f32, a.v[3], b.v[3], eps);
    }

    pub inline fn add(a: Vec4, b: Vec4) Vec4 {
        return .{ .v = [_]f32{ a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2], a.v[3] + b.v[3] } };
    }

    pub inline fn sub(a: Vec4, b: Vec4) Vec4 {
        return .{ .v = [_]f32{ a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2], a.v[3] - b.v[3] } };
    }

    pub inline fn mul(a: Vec4, b: Vec4) Vec4 {
        return .{ .v = [_]f32{ a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2], a.v[3] * b.v[3] } };
    }

    pub inline fn scale(a: Vec4, b: f32) Vec4 {
        return .{ .v = [_]f32{ a.v[0] * b, a.v[1] * b, a.v[2] * b, a.v[3] * b } };
    }

    pub inline fn neg(a: Vec4) Vec4 {
        return .{ .v = [_]f32{ -a.v[0], -a.v[1], -a.v[2], -a.v[3] } };
    }

    pub inline fn mulAdd(a: Vec4, b: Vec4, c: Vec4) Vec4 {
        return .{ .v = [_]f32{
            a.v[0] * b.v[0] + c.v[0],
            a.v[1] * b.v[1] + c.v[1],
            a.v[2] * b.v[2] + c.v[2],
            a.v[3] * b.v[3] + c.v[3],
        } };
    }

    pub inline fn negMulAdd(a: Vec4, b: Vec4, c: Vec4) Vec4 {
        return .{ .v = [_]f32{
            a.v[0] * (-b.v[0]) + c.v[0],
            a.v[1] * (-b.v[1]) + c.v[1],
            a.v[2] * (-b.v[2]) + c.v[2],
            a.v[3] * (-b.v[3]) + c.v[3],
        } };
    }

    pub inline fn rcp(a: Vec4) Vec4 {
        assert(!math.approxEq(f32, a.v[0], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[1], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[2], 0.0, 0.0001));
        assert(!math.approxEq(f32, a.v[3], 0.0, 0.0001));
        return .{ .v = [_]f32{ 1.0 / a.v[0], 1.0 / a.v[1], 1.0 / a.v[2], 1.0 / a.v[3] } };
    }

    pub inline fn dot(a: Vec4, b: Vec4) f32 {
        return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2] + a.v[3] * b.v[3];
    }

    pub inline fn length(a: Vec4) f32 {
        return math.sqrt(dot(a, a));
    }

    pub inline fn lengthSq(a: Vec4) f32 {
        return dot(a, a);
    }

    pub inline fn normalize(a: Vec4) Vec4 {
        const len = length(a);
        assert(!math.approxEq(f32, len, 0.0, 0.0001));
        const rcplen = 1.0 / len;
        return .{ .v = [_]f32{ rcplen * a.v[0], rcplen * a.v[1], rcplen * a.v[2], rcplen * a.v[3] } };
    }

    pub inline fn transform(a: Vec4, b: Mat4) Vec4 {
        return .{
            .v = [_]f32{
                a.v[0] * b.m[0][0] + a.v[1] * b.m[1][0] + a.v[2] * b.m[2][0] + a.v[3] * b.m[3][0],
                a.v[0] * b.m[0][1] + a.v[1] * b.m[1][1] + a.v[2] * b.m[2][1] + a.v[3] * b.m[3][1],
                a.v[0] * b.m[0][2] + a.v[1] * b.m[1][2] + a.v[2] * b.m[2][2] + a.v[3] * b.m[3][2],
                a.v[0] * b.m[0][3] + a.v[1] * b.m[1][3] + a.v[2] * b.m[2][3] + a.v[3] * b.m[3][3],
            },
        };
    }
};

pub const Mat4 = extern struct {
    m: [4][4]f32,

    pub fn init(
        // zig fmt: off
        r0x: f32, r0y: f32, r0z: f32, r0w: f32,
        r1x: f32, r1y: f32, r1z: f32, r1w: f32,
        r2x: f32, r2y: f32, r2z: f32, r2w: f32,
        r3x: f32, r3y: f32, r3z: f32, r3w: f32,
        // zig fmt: on
    ) Mat4 {
        return .{
            .m = [_][4]f32{
                [_]f32{ r0x, r0y, r0z, r0w },
                [_]f32{ r1x, r1y, r1z, r1w },
                [_]f32{ r2x, r2y, r2z, r2w },
                [_]f32{ r3x, r3y, r3z, r3w },
            },
        };
    }

    pub fn initVec4(r0: Vec4, r1: Vec4, r2: Vec4, r3: Vec4) Mat4 {
        return .{
            .m = [_][4]f32{
                [_]f32{ r0.v[0], r0.v[1], r0.v[2], r0.v[3] },
                [_]f32{ r1.v[0], r1.v[1], r1.v[2], r1.v[3] },
                [_]f32{ r2.v[0], r2.v[1], r2.v[2], r2.v[3] },
                [_]f32{ r3.v[0], r3.v[1], r3.v[2], r3.v[3] },
            },
        };
    }

    pub inline fn approxEq(a: Mat4, b: Mat4, eps: f32) bool {
        return math.approxEq(f32, a.m[0][0], b.m[0][0], eps) and
            math.approxEq(f32, a.m[0][1], b.m[0][1], eps) and
            math.approxEq(f32, a.m[0][2], b.m[0][2], eps) and
            math.approxEq(f32, a.m[0][3], b.m[0][3], eps) and
            math.approxEq(f32, a.m[1][0], b.m[1][0], eps) and
            math.approxEq(f32, a.m[1][1], b.m[1][1], eps) and
            math.approxEq(f32, a.m[1][2], b.m[1][2], eps) and
            math.approxEq(f32, a.m[1][3], b.m[1][3], eps) and
            math.approxEq(f32, a.m[2][0], b.m[2][0], eps) and
            math.approxEq(f32, a.m[2][1], b.m[2][1], eps) and
            math.approxEq(f32, a.m[2][2], b.m[2][2], eps) and
            math.approxEq(f32, a.m[2][3], b.m[2][3], eps) and
            math.approxEq(f32, a.m[3][0], b.m[3][0], eps) and
            math.approxEq(f32, a.m[3][1], b.m[3][1], eps) and
            math.approxEq(f32, a.m[3][2], b.m[3][2], eps) and
            math.approxEq(f32, a.m[3][3], b.m[3][3], eps);
    }

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

    pub fn inv(a: Mat4, det: ?*f32) Mat4 {
        const mt = a.transpose();
        var v0: [4]Vec4 = undefined;
        var v1: [4]Vec4 = undefined;

        v0[0] = Vec4.init(mt.m[2][0], mt.m[2][0], mt.m[2][1], mt.m[2][1]);
        v1[0] = Vec4.init(mt.m[3][2], mt.m[3][3], mt.m[3][2], mt.m[3][3]);
        v0[1] = Vec4.init(mt.m[0][0], mt.m[0][0], mt.m[0][1], mt.m[0][1]);
        v1[1] = Vec4.init(mt.m[1][2], mt.m[1][3], mt.m[1][2], mt.m[1][3]);
        v0[2] = Vec4.init(mt.m[2][0], mt.m[2][2], mt.m[0][0], mt.m[0][2]);
        v1[2] = Vec4.init(mt.m[3][1], mt.m[3][3], mt.m[1][1], mt.m[1][3]);

        var d0 = v0[0].mul(v1[0]);
        var d1 = v0[1].mul(v1[1]);
        var d2 = v0[2].mul(v1[2]);

        v0[0] = Vec4.init(mt.m[2][2], mt.m[2][3], mt.m[2][2], mt.m[2][3]);
        v1[0] = Vec4.init(mt.m[3][0], mt.m[3][0], mt.m[3][1], mt.m[3][1]);
        v0[1] = Vec4.init(mt.m[0][2], mt.m[0][3], mt.m[0][2], mt.m[0][3]);
        v1[1] = Vec4.init(mt.m[1][0], mt.m[1][0], mt.m[1][1], mt.m[1][1]);
        v0[2] = Vec4.init(mt.m[2][1], mt.m[2][3], mt.m[0][1], mt.m[0][3]);
        v1[2] = Vec4.init(mt.m[3][0], mt.m[3][2], mt.m[1][0], mt.m[1][2]);

        d0 = v0[0].negMulAdd(v1[0], d0);
        d1 = v0[1].negMulAdd(v1[1], d1);
        d2 = v0[2].negMulAdd(v1[2], d2);

        v0[0] = Vec4.init(mt.m[1][1], mt.m[1][2], mt.m[1][0], mt.m[1][1]);
        v1[0] = Vec4.init(d2.v[1], d0.v[1], d0.v[3], d0.v[0]);
        v0[1] = Vec4.init(mt.m[0][2], mt.m[0][0], mt.m[0][1], mt.m[0][0]);
        v1[1] = Vec4.init(d0.v[3], d2.v[1], d0.v[1], d0.v[2]);
        v0[2] = Vec4.init(mt.m[3][1], mt.m[3][2], mt.m[3][0], mt.m[3][1]);
        v1[2] = Vec4.init(d2.v[3], d1.v[1], d1.v[3], d1.v[0]);
        v0[3] = Vec4.init(mt.m[2][2], mt.m[2][0], mt.m[2][1], mt.m[2][0]);
        v1[3] = Vec4.init(d1.v[3], d2.v[3], d1.v[1], d1.v[2]);

        var c0 = v0[0].mul(v1[0]);
        var c2 = v0[1].mul(v1[1]);
        var c4 = v0[2].mul(v1[2]);
        var c6 = v0[3].mul(v1[3]);

        v0[0] = Vec4.init(mt.m[1][2], mt.m[1][3], mt.m[1][1], mt.m[1][2]);
        v1[0] = Vec4.init(d0.v[3], d0.v[0], d0.v[1], d2.v[0]);
        v0[1] = Vec4.init(mt.m[0][3], mt.m[0][2], mt.m[0][3], mt.m[0][1]);
        v1[1] = Vec4.init(d0.v[2], d0.v[1], d2.v[0], d0.v[0]);
        v0[2] = Vec4.init(mt.m[3][2], mt.m[3][3], mt.m[3][1], mt.m[3][2]);
        v1[2] = Vec4.init(d1.v[3], d1.v[0], d1.v[1], d2.v[2]);
        v0[3] = Vec4.init(mt.m[2][3], mt.m[2][2], mt.m[2][3], mt.m[2][1]);
        v1[3] = Vec4.init(d1.v[2], d1.v[1], d2.v[2], d1.v[0]);

        c0 = v0[0].negMulAdd(v1[0], c0);
        c2 = v0[1].negMulAdd(v1[1], c2);
        c4 = v0[2].negMulAdd(v1[2], c4);
        c6 = v0[3].negMulAdd(v1[3], c6);

        v0[0] = Vec4.init(mt.m[1][3], mt.m[1][0], mt.m[1][3], mt.m[1][0]);
        v1[0] = Vec4.init(d0.v[2], d2.v[1], d2.v[0], d0.v[2]);
        v0[1] = Vec4.init(mt.m[0][1], mt.m[0][3], mt.m[0][0], mt.m[0][2]);
        v1[1] = Vec4.init(d2.v[1], d0.v[0], d0.v[3], d2.v[0]);
        v0[2] = Vec4.init(mt.m[3][3], mt.m[3][0], mt.m[3][3], mt.m[3][0]);
        v1[2] = Vec4.init(d1.v[2], d2.v[3], d2.v[2], d1.v[2]);
        v0[3] = Vec4.init(mt.m[2][1], mt.m[2][3], mt.m[2][0], mt.m[2][2]);
        v1[3] = Vec4.init(d2.v[3], d1.v[0], d1.v[3], d2.v[2]);

        const c1 = v0[0].negMulAdd(v1[0], c0);
        c0 = v0[0].mulAdd(v1[0], c0);

        const c3 = v0[1].mulAdd(v1[1], c2);
        c2 = v0[1].negMulAdd(v1[1], c2);

        const c5 = v0[2].negMulAdd(v1[2], c4);
        c4 = v0[2].mulAdd(v1[2], c4);

        const c7 = v0[3].mulAdd(v1[3], c6);
        c6 = v0[3].negMulAdd(v1[3], c6);

        var r = Mat4.initVec4(
            Vec4.init(c0.v[0], c1.v[1], c0.v[2], c1.v[3]),
            Vec4.init(c2.v[0], c3.v[1], c2.v[2], c3.v[3]),
            Vec4.init(c4.v[0], c5.v[1], c4.v[2], c5.v[3]),
            Vec4.init(c6.v[0], c7.v[1], c6.v[2], c7.v[3]),
        );

        const d = r.m[0][0] * mt.m[0][0] + r.m[0][1] * mt.m[0][1] + r.m[0][2] * mt.m[0][2] + r.m[0][3] * mt.m[0][3];
        if (det != null) {
            det.?.* = d;
        }
        const rcp_d = if (math.approxEq(f32, d, 0.0, 0.00001)) 0.0 else 1.0 / d;

        r.m[0][0] *= rcp_d;
        r.m[0][1] *= rcp_d;
        r.m[0][2] *= rcp_d;
        r.m[0][3] *= rcp_d;
        r.m[1][0] *= rcp_d;
        r.m[1][1] *= rcp_d;
        r.m[1][2] *= rcp_d;
        r.m[1][3] *= rcp_d;
        r.m[2][0] *= rcp_d;
        r.m[2][1] *= rcp_d;
        r.m[2][2] *= rcp_d;
        r.m[2][3] *= rcp_d;
        r.m[3][0] *= rcp_d;
        r.m[3][1] *= rcp_d;
        r.m[3][2] *= rcp_d;
        r.m[3][3] *= rcp_d;

        return r;
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

    pub inline fn initZero() Mat4 {
        const static = struct {
            const zero = Mat4{
                .m = [_][4]f32{
                    [_]f32{ 0.0, 0.0, 0.0, 0.0 },
                    [_]f32{ 0.0, 0.0, 0.0, 0.0 },
                    [_]f32{ 0.0, 0.0, 0.0, 0.0 },
                    [_]f32{ 0.0, 0.0, 0.0, 0.0 },
                },
            };
        };
        return static.zero;
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

    pub fn initLookToLh(eye_pos: Vec3, eye_dir: Vec3, up_dir: Vec3) Mat4 {
        const az = Vec3.normalize(eye_dir);
        const ax = Vec3.normalize(Vec3.cross(up_dir, az));
        const ay = Vec3.normalize(Vec3.cross(az, ax));
        return .{
            .m = [_][4]f32{
                [_]f32{ ax.v[0], ay.v[0], az.v[0], 0.0 },
                [_]f32{ ax.v[1], ay.v[1], az.v[1], 0.0 },
                [_]f32{ ax.v[2], ay.v[2], az.v[2], 0.0 },
                [_]f32{ -Vec3.dot(ax, eye_pos), -Vec3.dot(ay, eye_pos), -Vec3.dot(az, eye_pos), 1.0 },
            },
        };
    }

    pub inline fn initLookAtLh(eye_pos: Vec3, focus_pos: Vec3, up_dir: Vec3) Mat4 {
        return initLookToLh(eye_pos, focus_pos.sub(eye_pos), up_dir);
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

        const rcp_w = 1.0 / (view_right - view_left);
        const rcp_h = 1.0 / (view_top - view_bottom);
        const range = 1.0 / (far_z - near_z);

        return .{
            .m = [_][4]f32{
                [_]f32{ rcp_w + rcp_w, 0.0, 0.0, 0.0 },
                [_]f32{ 0.0, rcp_h + rcp_h, 0.0, 0.0 },
                [_]f32{ 0.0, 0.0, range, 0.0 },
                [_]f32{ -(view_left + view_right) * rcp_w, -(view_top + view_bottom) * rcp_h, -range * near_z, 1.0 },
            },
        };
    }
};

test "dot" {
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(math.approxEq(f32, a.dot(b), 11.0, 0.0001));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(4.0, 5.0, 6.0);
        assert(math.approxEq(f32, a.dot(b), 32.0, 0.0001));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, 4.0);
        const b = Vec4.init(5.0, 6.0, 7.0, 8.0);
        assert(math.approxEq(f32, a.dot(b), 70.0, 0.0001));
    }
}

test "cross" {
    {
        const a = Vec3.init(1.0, 0.0, 0.0);
        const b = Vec3.init(0.0, 1.0, 0.0);
        assert(a.cross(b).approxEq(Vec3.init(0.0, 0.0, 1.0), 0.00001));
    }
    {
        const a = Vec3.init(0.0, 0.0, -1.0);
        const b = Vec3.init(1.0, 0.0, 0.0);
        assert(a.cross(b).approxEq(Vec3.init(0.0, -1.0, 0.0), 0.00001));
    }
}

test "VecN add, sub, scale" {
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(a.add(b).approxEq(Vec2.init(4.0, 6.0), 0.00001));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(3.0, 4.0, 5.0);
        assert(a.add(b).approxEq(Vec3.init(4.0, 6.0, 8.0), 0.00001));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        const b = Vec4.init(3.0, 4.0, 5.0, 2.0);
        assert(a.add(b).approxEq(Vec4.init(4.0, 6.0, 8.0, 1.0), 0.00001));
    }
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(a.sub(b).approxEq(Vec2.init(-2.0, -2.0), 0.00001));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(3.0, 4.0, 5.0);
        assert(a.sub(b).approxEq(Vec3.init(-2.0, -2.0, -2.0), 0.00001));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        const b = Vec4.init(3.0, 4.0, 5.0, 2.0);
        assert(a.sub(b).approxEq(Vec4.init(-2.0, -2.0, -2.0, -3.0), 0.00001));
    }
    {
        const a = Vec2.init(1.0, 2.0);
        assert(a.scale(2.0).approxEq(Vec2.init(2.0, 4.0), 0.00001));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        assert(a.scale(-1.0).approxEq(Vec3.init(-1.0, -2.0, -3.0), 0.00001));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        assert(a.scale(3.0).approxEq(Vec4.init(3.0, 6.0, 9.0, -3.0), 0.00001));
    }
}

test "length, normalize" {
    {
        const a = Vec2.init(2.0, 3.0).length();
        assert(math.approxEq(f32, a, 3.60555, 0.0001));
    }
    {
        const a = Vec3.init(1.0, 1.0, 1.0).length();
        assert(math.approxEq(f32, a, 1.73205, 0.0001));
    }
    {
        const a = Vec4.init(1.0, 1.0, 1.0, 1.0).length();
        assert(math.approxEq(f32, a, 2.0, 0.0001));
    }
    {
        const a = Vec2.init(2.0, 4.0).normalize();
        assert(Vec2.approxEq(a, Vec2.init(0.447214, 0.894427), 0.0001));
    }
    {
        const a = Vec3.init(2.0, -5.0, 4.0).normalize();
        assert(Vec3.approxEq(a, Vec3.init(0.298142, -0.745356, 0.596285), 0.0001));
    }
    {
        const a = Vec4.init(-1.0, 2.0, -5.0, 4.0).normalize();
        assert(Vec4.approxEq(a, Vec4.init(-0.147442, 0.294884, -0.73721, 0.589768), 0.0001));
    }
}

test "Mat4 transpose" {
    const m = Mat4.initVec4(
        Vec4.init(1.0, 2.0, 3.0, 4.0),
        Vec4.init(5.0, 6.0, 7.0, 8.0),
        Vec4.init(9.0, 10.0, 11.0, 12.0),
        Vec4.init(13.0, 14.0, 15.0, 16.0),
    );
    const mt = m.transpose();
    assert(
        mt.approxEq(
            Mat4.init(1.0, 5.0, 9.0, 13.0, 2.0, 6.0, 10.0, 14.0, 3.0, 7.0, 11.0, 15.0, 4.0, 8.0, 12.0, 16.0),
            0.00001,
        ),
    );
}

test "Mat4 mul" {
    const a = Mat4.initVec4(
        Vec4.init(0.1, 0.2, 0.3, 0.4),
        Vec4.init(0.5, 0.6, 0.7, 0.8),
        Vec4.init(0.9, 1.0, 1.1, 1.2),
        Vec4.init(1.3, 1.4, 1.5, 1.6),
    );
    const b = Mat4.initVec4(
        Vec4.init(1.7, 1.8, 1.9, 2.0),
        Vec4.init(2.1, 2.2, 2.3, 2.4),
        Vec4.init(2.5, 2.6, 2.7, 2.8),
        Vec4.init(2.9, 3.0, 3.1, 3.2),
    );
    const c = a.mul(b);
    assert(c.approxEq(
        Mat4.initVec4(
            Vec4.init(2.5, 2.6, 2.7, 2.8),
            Vec4.init(6.18, 6.44, 6.7, 6.96),
            Vec4.init(9.86, 10.28, 10.7, 11.12),
            Vec4.init(13.54, 14.12, 14.7, 15.28),
        ),
        0.0001,
    ));
}

test "Mat4 inverse" {
    var m = Mat4.initVec4(
        Vec4.init(10.0, -9.0, -12.0, 1.0),
        Vec4.init(7.0, -12.0, 11.0, 1.0),
        Vec4.init(-10.0, 10.0, 3.0, 1.0),
        Vec4.init(1.0, 2.0, 3.0, 4.0),
    );
    var det: f32 = 0.0;
    m = m.inv(&det);
    assert(math.approxEq(f32, det, 2939.0, 0.0001));
    assert(m.approxEq(
        Mat4.initVec4(
            Vec4.init(-0.170806, -0.13576, -0.349439, 0.164001),
            Vec4.init(-0.163661, -0.14801, -0.253147, 0.141204),
            Vec4.init(-0.0871045, 0.00646478, -0.0785982, 0.0398095),
            Vec4.init(0.18986, 0.103096, 0.272882, 0.10854),
        ),
        0.0001,
    ));
}
