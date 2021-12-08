// Most of below code is ported from DirectXMath: https://github.com/microsoft/DirectXMath
const std = @import("std");
const assert = std.debug.assert;
const math = std.math;

const epsilon: f32 = 0.00001;

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
    c: [2]f32,

    pub inline fn init(x: f32, y: f32) Vec2 {
        return .{ .c = [_]f32{ x, y } };
    }

    pub inline fn initS(s: f32) Vec2 {
        return init(s, s);
    }

    pub inline fn initZero() Vec2 {
        const static = struct {
            const zero = init(0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn toVec3(v: Vec2) Vec3 {
        return Vec3.init(v.c[0], v.c[1], 0.0);
    }

    pub inline fn toVec4(v: Vec2) Vec4 {
        return Vec4.init(v.c[0], v.c[1], 0.0, 1.0);
    }

    pub inline fn approxEq(v0: Vec2, v1: Vec2, eps: f32) bool {
        return math.approxEqAbs(f32, v0.c[0], v1.c[0], eps) and
            math.approxEqAbs(f32, v0.c[1], v1.c[1], eps);
    }

    pub inline fn add(v0: Vec2, v1: Vec2) Vec2 {
        return init(
            v0.c[0] + v1.c[0],
            v0.c[1] + v1.c[1],
        );
    }

    pub inline fn sub(v0: Vec2, v1: Vec2) Vec2 {
        return init(
            v0.c[0] - v1.c[0],
            v0.c[1] - v1.c[1],
        );
    }

    pub inline fn mul(v0: Vec2, v1: Vec2) Vec2 {
        return init(
            v0.c[0] * v1.c[0],
            v0.c[1] * v1.c[1],
        );
    }

    pub inline fn div(v0: Vec2, v1: Vec2) Vec2 {
        assert(!approxEq(v1, initZero(), epsilon));
        return init(
            v0.c[0] / v1.c[0],
            v0.c[1] / v1.c[1],
        );
    }

    pub inline fn scale(v: Vec2, s: f32) Vec2 {
        return init(
            v.c[0] * s,
            v.c[1] * s,
        );
    }

    pub inline fn neg(v: Vec2) Vec2 {
        return init(
            -v.c[0],
            -v.c[1],
        );
    }

    pub inline fn mulAdd(v0: Vec2, v1: Vec2, v2: Vec2) Vec2 {
        return init(
            v0.c[0] * v1.c[0] + v2.c[0],
            v0.c[1] * v1.c[1] + v2.c[1],
        );
    }

    pub inline fn negMulAdd(v0: Vec2, v1: Vec2, v2: Vec2) Vec2 {
        return init(
            -v0.c[0] * v1.c[0] + v2.c[0],
            -v0.c[1] * v1.c[1] + v2.c[1],
        );
    }

    pub inline fn mulSub(v0: Vec2, v1: Vec2, v2: Vec2) Vec2 {
        return init(
            v0.c[0] * v1.c[0] - v2.c[0],
            v0.c[1] * v1.c[1] - v2.c[1],
        );
    }

    pub inline fn negMulSub(v0: Vec2, v1: Vec2, v2: Vec2) Vec2 {
        return init(
            -v0.c[0] * v1.c[0] - v2.c[0],
            -v0.c[1] * v1.c[1] - v2.c[1],
        );
    }

    pub inline fn lerp(v0: Vec2, v1: Vec2, t: f32) Vec2 {
        const tt = initS(t);
        const v0v1 = v1.sub(v0);
        return v0v1.mulAdd(tt, v0);
    }

    pub inline fn rcp(v: Vec2) Vec2 {
        assert(!approxEq(v, initZero(), epsilon));
        return init(
            1.0 / v.c[0],
            1.0 / v.c[1],
        );
    }

    pub inline fn dot(v0: Vec2, v1: Vec2) f32 {
        return v0.c[0] * v1.c[0] +
            v0.c[1] * v1.c[1];
    }

    pub inline fn length(v: Vec2) f32 {
        return math.sqrt(dot(v, v));
    }

    pub inline fn lengthSq(v: Vec2) f32 {
        return dot(v, v);
    }

    pub inline fn normalize(v: Vec2) Vec2 {
        const len = length(v);
        assert(!math.approxEqAbs(f32, len, 0.0, epsilon));
        const rcplen = 1.0 / len;
        return v.scale(rcplen);
    }

    pub inline fn transform(v: Vec2, m: Mat4) Vec2 {
        return init(
            v.c[0] * m.r[0].c[0] + v.c[1] * m.r[1].c[0] + m.r[3].c[0],
            v.c[0] * m.r[0].c[1] + v.c[1] * m.r[1].c[1] + m.r[3].c[1],
        );
    }

    pub inline fn transformNormal(v: Vec2, m: Mat4) Vec2 {
        return init(
            v.c[0] * m.r[0].c[0] + v.c[1] * m.r[1].c[0],
            v.c[0] * m.r[0].c[1] + v.c[1] * m.r[1].c[1],
        );
    }
};

pub const Vec3 = extern struct {
    c: [3]f32,

    pub inline fn init(x: f32, y: f32, z: f32) Vec3 {
        return .{ .c = [_]f32{ x, y, z } };
    }

    pub inline fn initS(s: f32) Vec3 {
        return init(s, s, s);
    }

    pub inline fn initZero() Vec3 {
        const static = struct {
            const zero = init(0.0, 0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn toVec2(v: Vec3) Vec2 {
        return Vec2.init(v.c[0], v.c[1]);
    }

    pub inline fn toVec4(v: Vec3) Vec4 {
        return Vec4.init(v.c[0], v.c[1], v.c[2], 1.0);
    }

    pub inline fn approxEq(v0: Vec3, v1: Vec3, eps: f32) bool {
        return math.approxEqAbs(f32, v0.c[0], v1.c[0], eps) and
            math.approxEqAbs(f32, v0.c[1], v1.c[1], eps) and
            math.approxEqAbs(f32, v0.c[2], v1.c[2], eps);
    }

    pub inline fn dot(v0: Vec3, v1: Vec3) f32 {
        return v0.c[0] * v1.c[0] +
            v0.c[1] * v1.c[1] +
            v0.c[2] * v1.c[2];
    }

    pub inline fn cross(v0: Vec3, v1: Vec3) Vec3 {
        return init(
            v0.c[1] * v1.c[2] - v0.c[2] * v1.c[1],
            v0.c[2] * v1.c[0] - v0.c[0] * v1.c[2],
            v0.c[0] * v1.c[1] - v0.c[1] * v1.c[0],
        );
    }

    pub inline fn add(v0: Vec3, v1: Vec3) Vec3 {
        return init(
            v0.c[0] + v1.c[0],
            v0.c[1] + v1.c[1],
            v0.c[2] + v1.c[2],
        );
    }

    pub inline fn sub(v0: Vec3, v1: Vec3) Vec3 {
        return init(
            v0.c[0] - v1.c[0],
            v0.c[1] - v1.c[1],
            v0.c[2] - v1.c[2],
        );
    }

    pub inline fn mul(v0: Vec3, v1: Vec3) Vec3 {
        return init(
            v0.c[0] * v1.c[0],
            v0.c[1] * v1.c[1],
            v0.c[2] * v1.c[2],
        );
    }

    pub inline fn div(v0: Vec3, v1: Vec3) Vec3 {
        assert(!approxEq(v1, initZero(), epsilon));
        return init(
            v0.c[0] / v1.c[0],
            v0.c[1] / v1.c[1],
            v0.c[2] / v1.c[2],
        );
    }

    pub inline fn scale(v: Vec3, s: f32) Vec3 {
        return init(
            v.c[0] * s,
            v.c[1] * s,
            v.c[2] * s,
        );
    }

    pub inline fn neg(v: Vec3) Vec3 {
        return init(
            -v.c[0],
            -v.c[1],
            -v.c[2],
        );
    }

    pub inline fn mulAdd(v0: Vec3, v1: Vec3, v2: Vec3) Vec3 {
        return init(
            v0.c[0] * v1.c[0] + v2.c[0],
            v0.c[1] * v1.c[1] + v2.c[1],
            v0.c[2] * v1.c[2] + v2.c[2],
        );
    }

    pub inline fn negMulAdd(v0: Vec3, v1: Vec3, v2: Vec3) Vec3 {
        return init(
            -v0.c[0] * v1.c[0] + v2.c[0],
            -v0.c[1] * v1.c[1] + v2.c[1],
            -v0.c[2] * v1.c[2] + v2.c[2],
        );
    }

    pub inline fn mulSub(v0: Vec3, v1: Vec3, v2: Vec3) Vec3 {
        return init(
            v0.c[0] * v1.c[0] - v2.c[0],
            v0.c[1] * v1.c[1] - v2.c[1],
            v0.c[2] * v1.c[2] - v2.c[2],
        );
    }

    pub inline fn negMulSub(v0: Vec3, v1: Vec3, v2: Vec3) Vec3 {
        return init(
            -v0.c[0] * v1.c[0] - v2.c[0],
            -v0.c[1] * v1.c[1] - v2.c[1],
            -v0.c[2] * v1.c[2] - v2.c[2],
        );
    }

    pub inline fn lerp(v0: Vec3, v1: Vec3, t: f32) Vec3 {
        const ttt = Vec3.initS(t);
        const v0v1 = v1.sub(v0);
        return v0v1.mulAdd(ttt, v0);
    }

    pub inline fn rcp(v: Vec3) Vec3 {
        assert(!approxEq(v, initZero(), epsilon));
        return init(
            1.0 / v.c[0],
            1.0 / v.c[1],
            1.0 / v.c[2],
        );
    }

    pub inline fn length(v: Vec3) f32 {
        return math.sqrt(dot(v, v));
    }

    pub inline fn lengthSq(v: Vec3) f32 {
        return dot(v, v);
    }

    pub inline fn normalize(v: Vec3) Vec3 {
        const len = length(v);
        assert(!math.approxEqAbs(f32, len, 0.0, epsilon));
        const rcplen = 1.0 / len;
        return v.scale(rcplen);
    }

    pub inline fn transform(v: Vec3, m: Mat4) Vec3 {
        return init(
            v.c[0] * m.r[0].c[0] + v.c[1] * m.r[1].c[0] + v.c[2] * m.r[2].c[0] + m.r[3].c[0],
            v.c[0] * m.r[0].c[1] + v.c[1] * m.r[1].c[1] + v.c[2] * m.r[2].c[1] + m.r[3].c[1],
            v.c[0] * m.r[0].c[2] + v.c[1] * m.r[1].c[2] + v.c[2] * m.r[2].c[2] + m.r[3].c[2],
        );
    }

    pub inline fn transformNormal(v: Vec3, m: Mat4) Vec3 {
        return init(
            v.c[0] * m.r[0].c[0] + v.c[1] * m.r[1].c[0] + v.c[2] * m.r[2].c[0],
            v.c[0] * m.r[0].c[1] + v.c[1] * m.r[1].c[1] + v.c[2] * m.r[2].c[1],
            v.c[0] * m.r[0].c[2] + v.c[1] * m.r[1].c[2] + v.c[2] * m.r[2].c[2],
        );
    }
};

pub const Vec4 = extern struct {
    c: [4]f32,

    pub inline fn init(x: f32, y: f32, z: f32, w: f32) Vec4 {
        return .{ .c = [_]f32{ x, y, z, w } };
    }

    pub inline fn initS(s: f32) Vec4 {
        return init(s, s, s, s);
    }

    pub inline fn initZero() Vec4 {
        const static = struct {
            const zero = init(0.0, 0.0, 0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn toVec2(v: Vec4) Vec2 {
        return Vec2.init(v.c[0], v.c[1]);
    }

    pub inline fn toVec3(v: Vec4) Vec3 {
        return Vec3.init(v.c[0], v.c[1], v.c[2]);
    }

    pub inline fn approxEq(v0: Vec4, v1: Vec4, eps: f32) bool {
        return math.approxEqAbs(f32, v0.c[0], v1.c[0], eps) and
            math.approxEqAbs(f32, v0.c[1], v1.c[1], eps) and
            math.approxEqAbs(f32, v0.c[2], v1.c[2], eps) and
            math.approxEqAbs(f32, v0.c[3], v1.c[3], eps);
    }

    pub inline fn add(v0: Vec4, v1: Vec4) Vec4 {
        return init(
            v0.c[0] + v1.c[0],
            v0.c[1] + v1.c[1],
            v0.c[2] + v1.c[2],
            v0.c[3] + v1.c[3],
        );
    }

    pub inline fn sub(v0: Vec4, v1: Vec4) Vec4 {
        return init(
            v0.c[0] - v1.c[0],
            v0.c[1] - v1.c[1],
            v0.c[2] - v1.c[2],
            v0.c[3] - v1.c[3],
        );
    }

    pub inline fn mul(v0: Vec4, v1: Vec4) Vec4 {
        return init(
            v0.c[0] * v1.c[0],
            v0.c[1] * v1.c[1],
            v0.c[2] * v1.c[2],
            v0.c[3] * v1.c[3],
        );
    }

    pub inline fn div(v0: Vec4, v1: Vec4) Vec4 {
        assert(!approxEq(v1, initZero(), epsilon));
        return init(
            v0.c[0] / v1.c[0],
            v0.c[1] / v1.c[1],
            v0.c[2] / v1.c[2],
            v0.c[3] / v1.c[3],
        );
    }

    pub inline fn scale(v: Vec4, s: f32) Vec4 {
        return init(
            v.c[0] * s,
            v.c[1] * s,
            v.c[2] * s,
            v.c[3] * s,
        );
    }

    pub inline fn neg(v: Vec4) Vec4 {
        return init(
            -v.c[0],
            -v.c[1],
            -v.c[2],
            -v.c[3],
        );
    }

    pub inline fn mulAdd(v0: Vec4, v1: Vec4, v2: Vec4) Vec4 {
        return init(
            v0.c[0] * v1.c[0] + v2.c[0],
            v0.c[1] * v1.c[1] + v2.c[1],
            v0.c[2] * v1.c[2] + v2.c[2],
            v0.c[3] * v1.c[3] + v2.c[3],
        );
    }

    pub inline fn negMulAdd(v0: Vec4, v1: Vec4, v2: Vec4) Vec4 {
        return init(
            -v0.c[0] * v1.c[0] + v2.c[0],
            -v0.c[1] * v1.c[1] + v2.c[1],
            -v0.c[2] * v1.c[2] + v2.c[2],
            -v0.c[3] * v1.c[3] + v2.c[3],
        );
    }

    pub inline fn mulSub(v0: Vec4, v1: Vec4, v2: Vec4) Vec4 {
        return init(
            v0.c[0] * v1.c[0] - v2.c[0],
            v0.c[1] * v1.c[1] - v2.c[1],
            v0.c[2] * v1.c[2] - v2.c[2],
            v0.c[3] * v1.c[3] - v2.c[3],
        );
    }

    pub inline fn negMulSub(v0: Vec4, v1: Vec4, v2: Vec4) Vec4 {
        return init(
            -v0.c[0] * v1.c[0] - v2.c[0],
            -v0.c[1] * v1.c[1] - v2.c[1],
            -v0.c[2] * v1.c[2] - v2.c[2],
            -v0.c[3] * v1.c[3] - v2.c[3],
        );
    }

    pub inline fn lerp(v0: Vec4, v1: Vec4, t: f32) Vec4 {
        const tttt = Vec4.initS(t);
        const v0v1 = v1.sub(v0);
        return v0v1.mulAdd(tttt, v0);
    }

    pub inline fn rcp(v: Vec4) Vec4 {
        assert(!approxEq(v, initZero(), epsilon));
        return init(
            1.0 / v.c[0],
            1.0 / v.c[1],
            1.0 / v.c[2],
            1.0 / v.c[3],
        );
    }

    pub inline fn dot(v0: Vec4, v1: Vec4) f32 {
        return v0.c[0] * v1.c[0] +
            v0.c[1] * v1.c[1] +
            v0.c[2] * v1.c[2] +
            v0.c[3] * v1.c[3];
    }

    pub inline fn length(v: Vec4) f32 {
        return math.sqrt(dot(v, v));
    }

    pub inline fn lengthSq(v: Vec4) f32 {
        return dot(v, v);
    }

    pub inline fn normalize(v: Vec4) Vec4 {
        const len = length(v);
        assert(!math.approxEqAbs(f32, len, 0.0, epsilon));
        const rcplen = 1.0 / len;
        return v.scale(rcplen);
    }

    pub inline fn transform(v: Vec4, m: Mat4) Vec4 {
        return init(
            v.c[0] * m.r[0].c[0] + v.c[1] * m.r[1].c[0] + v.c[2] * m.r[2].c[0] + v.c[3] * m.r[3].c[0],
            v.c[0] * m.r[0].c[1] + v.c[1] * m.r[1].c[1] + v.c[2] * m.r[2].c[1] + v.c[3] * m.r[3].c[1],
            v.c[0] * m.r[0].c[2] + v.c[1] * m.r[1].c[2] + v.c[2] * m.r[2].c[2] + v.c[3] * m.r[3].c[2],
            v.c[0] * m.r[0].c[3] + v.c[1] * m.r[1].c[3] + v.c[2] * m.r[2].c[3] + v.c[3] * m.r[3].c[3],
        );
    }
};

pub const Quat = extern struct {
    q: [4]f32,

    pub inline fn init(x: f32, y: f32, z: f32, w: f32) Quat {
        return .{ .q = [_]f32{ x, y, z, w } };
    }

    pub inline fn initZero() Quat {
        const static = struct {
            const zero = init(0.0, 0.0, 0.0, 0.0);
        };
        return static.zero;
    }

    pub inline fn initIdentity() Quat {
        const static = struct {
            const identity = init(0.0, 0.0, 0.0, 1.0);
        };
        return static.identity;
    }

    pub inline fn approxEq(a: Quat, b: Quat, eps: f32) bool {
        return math.approxEqAbs(f32, a.q[0], b.q[0], eps) and
            math.approxEqAbs(f32, a.q[1], b.q[1], eps) and
            math.approxEqAbs(f32, a.q[2], b.q[2], eps) and
            math.approxEqAbs(f32, a.q[3], b.q[3], eps);
    }

    pub inline fn add(a: Quat, b: Quat) Quat {
        return init(
            a.q[0] + b.q[0],
            a.q[1] + b.q[1],
            a.q[2] + b.q[2],
            a.q[3] + b.q[3],
        );
    }

    pub inline fn mul(a: Quat, b: Quat) Quat {
        // Returns the product b * a (which is the concatenation of a rotation 'a' followed by the rotation 'b').
        return init(
            (b.q[3] * a.q[0]) + (b.q[0] * a.q[3]) + (b.q[1] * a.q[2]) - (b.q[2] * a.q[1]),
            (b.q[3] * a.q[1]) - (b.q[0] * a.q[2]) + (b.q[1] * a.q[3]) + (b.q[2] * a.q[0]),
            (b.q[3] * a.q[2]) + (b.q[0] * a.q[1]) - (b.q[1] * a.q[0]) + (b.q[2] * a.q[3]),
            (b.q[3] * a.q[3]) - (b.q[0] * a.q[0]) - (b.q[1] * a.q[1]) - (b.q[2] * a.q[2]),
        );
    }

    pub inline fn scale(a: Quat, b: f32) Quat {
        return init(
            a.q[0] * b,
            a.q[1] * b,
            a.q[2] * b,
            a.q[3] * b,
        );
    }

    pub inline fn dot(a: Quat, b: Quat) f32 {
        return a.q[0] * b.q[0] +
            a.q[1] * b.q[1] +
            a.q[2] * b.q[2] +
            a.q[3] * b.q[3];
    }

    pub inline fn length(a: Quat) f32 {
        return math.sqrt(dot(a, a));
    }

    pub inline fn lengthSq(a: Quat) f32 {
        return dot(a, a);
    }

    pub inline fn normalize(a: Quat) Quat {
        const len = length(a);
        assert(!math.approxEqAbs(f32, len, 0.0, epsilon));
        const rcplen = 1.0 / len;
        return a.scale(rcplen);
    }

    pub inline fn conj(a: Quat) Quat {
        return init(
            -a.q[0],
            -a.q[1],
            -a.q[2],
            a.q[3],
        );
    }

    pub inline fn inv(a: Quat) Quat {
        const lensq = lengthSq(a);
        const con = conj(a);
        assert(!math.approxEqAbs(f32, lensq, 0.0, epsilon));
        const rcp_lensq = 1.0 / lensq;
        return con.scale(rcp_lensq);
    }

    pub inline fn slerp(a: Quat, b: Quat, t: f32) Quat {
        const cos_angle = dot(a, b);
        const angle = math.acos(cos_angle);

        const fa = math.sin((1.0 - t) * angle);
        const fb = math.sin(t * angle);

        const sin_angle = math.sin(angle);
        assert(!math.approxEqAbs(f32, sin_angle, 0.0, epsilon));
        const rcp_sin_angle = 1.0 / sin_angle;

        const ra = a.scale(fa);
        const rb = b.scale(fb);
        return ra.add(rb).scale(rcp_sin_angle);
    }

    pub fn initRotationMat4(m: Mat4) Quat {
        var q: Quat = undefined;
        const r22 = m.r[2].c[2];
        if (r22 <= 0.0) // x^2 + y^2 >= z^2 + w^2
        {
            const dif10 = m.r[1].c[1] - m.r[0].c[0];
            const omr22 = 1.0 - r22;
            if (dif10 <= 0.0) // x^2 >= y^2
            {
                const fourXSqr = omr22 - dif10;
                const inv4x = 0.5 / math.sqrt(fourXSqr);
                q.q[0] = fourXSqr * inv4x;
                q.q[1] = (m.r[0].c[1] + m.r[1].c[0]) * inv4x;
                q.q[2] = (m.r[0].c[2] + m.r[2].c[0]) * inv4x;
                q.q[3] = (m.r[1].c[2] - m.r[2].c[1]) * inv4x;
            } else // y^2 >= x^2
            {
                const fourYSqr = omr22 + dif10;
                const inv4y = 0.5 / math.sqrt(fourYSqr);
                q.q[0] = (m.r[0].c[1] + m.r[1].c[0]) * inv4y;
                q.q[1] = fourYSqr * inv4y;
                q.q[2] = (m.r[1].c[2] + m.r[2].c[1]) * inv4y;
                q.q[3] = (m.r[2].c[0] - m.r[0].c[2]) * inv4y;
            }
        } else // z^2 + w^2 >= x^2 + y^2
        {
            const sum10 = m.r[1].c[1] + m.r[0].c[0];
            const opr22 = 1.0 + r22;
            if (sum10 <= 0.0) // z^2 >= w^2
            {
                const fourZSqr = opr22 - sum10;
                const inv4z = 0.5 / math.sqrt(fourZSqr);
                q.q[0] = (m.r[0].c[2] + m.r[2].c[0]) * inv4z;
                q.q[1] = (m.r[1].c[2] + m.r[2].c[1]) * inv4z;
                q.q[2] = fourZSqr * inv4z;
                q.q[3] = (m.r[0].c[1] - m.r[1].c[0]) * inv4z;
            } else // w^2 >= z^2
            {
                const fourWSqr = opr22 + sum10;
                const inv4w = 0.5 / math.sqrt(fourWSqr);
                q.q[0] = (m.r[1].c[2] - m.r[2].c[1]) * inv4w;
                q.q[1] = (m.r[2].c[0] - m.r[0].c[2]) * inv4w;
                q.q[2] = (m.r[0].c[1] - m.r[1].c[0]) * inv4w;
                q.q[3] = fourWSqr * inv4w;
            }
        }
        return q;
    }

    pub fn initRotationNormal(normal_axis: Vec3, angle: f32) Quat {
        const half_angle = 0.5 * angle;
        const sinv = math.sin(half_angle);
        const cosv = math.cos(half_angle);

        return init(
            normal_axis.c[0] * sinv,
            normal_axis.c[1] * sinv,
            normal_axis.c[2] * sinv,
            cosv,
        );
    }

    pub fn initRotationAxis(axis: Vec3, angle: f32) Quat {
        assert(!axis.approxEq(Vec3.initZero(), epsilon));
        const n = axis.normalize();
        return initRotationNormal(n, angle);
    }
};

pub const Mat4 = extern struct {
    r: [4]Vec4,

    pub fn init(
        // zig fmt: off
        r0x: f32, r0y: f32, r0z: f32, r0w: f32,
        r1x: f32, r1y: f32, r1z: f32, r1w: f32,
        r2x: f32, r2y: f32, r2z: f32, r2w: f32,
        r3x: f32, r3y: f32, r3z: f32, r3w: f32,
        // zig fmt: on
    ) Mat4 {
        return initVec4(
            Vec4.init(r0x, r0y, r0z, r0w),
            Vec4.init(r1x, r1y, r1z, r1w),
            Vec4.init(r2x, r2y, r2z, r2w),
            Vec4.init(r3x, r3y, r3z, r3w),
        );
    }

    pub fn initVec4(r0: Vec4, r1: Vec4, r2: Vec4, r3: Vec4) Mat4 {
        return .{ .r = [4]Vec4{ r0, r1, r2, r3 } };
    }

    pub fn initArray4x3(a: [4][3]f32) Mat4 {
        // zig fmt: off
        return init(
            a[0][0], a[0][1], a[0][2], 0.0,
            a[1][0], a[1][1], a[1][2], 0.0,
            a[2][0], a[2][1], a[2][2], 0.0,
            a[3][0], a[3][1], a[3][2], 1.0,
        );
        // zig fmt: on
    }

    pub fn toArray4x3(m: Mat4) [4][3]f32 {
        return [4][3]f32{
            [3]f32{ m.r[0].c[0], m.r[0].c[1], m.r[0].c[2] },
            [3]f32{ m.r[1].c[0], m.r[1].c[1], m.r[1].c[2] },
            [3]f32{ m.r[2].c[0], m.r[2].c[1], m.r[2].c[2] },
            [3]f32{ m.r[3].c[0], m.r[3].c[1], m.r[3].c[2] },
        };
    }

    pub inline fn approxEq(a: Mat4, b: Mat4, eps: f32) bool {
        return math.approxEqAbs(f32, a.r[0].c[0], b.r[0].c[0], eps) and
            math.approxEqAbs(f32, a.r[0].c[1], b.r[0].c[1], eps) and
            math.approxEqAbs(f32, a.r[0].c[2], b.r[0].c[2], eps) and
            math.approxEqAbs(f32, a.r[0].c[3], b.r[0].c[3], eps) and
            math.approxEqAbs(f32, a.r[1].c[0], b.r[1].c[0], eps) and
            math.approxEqAbs(f32, a.r[1].c[1], b.r[1].c[1], eps) and
            math.approxEqAbs(f32, a.r[1].c[2], b.r[1].c[2], eps) and
            math.approxEqAbs(f32, a.r[1].c[3], b.r[1].c[3], eps) and
            math.approxEqAbs(f32, a.r[2].c[0], b.r[2].c[0], eps) and
            math.approxEqAbs(f32, a.r[2].c[1], b.r[2].c[1], eps) and
            math.approxEqAbs(f32, a.r[2].c[2], b.r[2].c[2], eps) and
            math.approxEqAbs(f32, a.r[2].c[3], b.r[2].c[3], eps) and
            math.approxEqAbs(f32, a.r[3].c[0], b.r[3].c[0], eps) and
            math.approxEqAbs(f32, a.r[3].c[1], b.r[3].c[1], eps) and
            math.approxEqAbs(f32, a.r[3].c[2], b.r[3].c[2], eps) and
            math.approxEqAbs(f32, a.r[3].c[3], b.r[3].c[3], eps);
    }

    pub fn transpose(a: Mat4) Mat4 {
        return initVec4(
            Vec4.init(a.r[0].c[0], a.r[1].c[0], a.r[2].c[0], a.r[3].c[0]),
            Vec4.init(a.r[0].c[1], a.r[1].c[1], a.r[2].c[1], a.r[3].c[1]),
            Vec4.init(a.r[0].c[2], a.r[1].c[2], a.r[2].c[2], a.r[3].c[2]),
            Vec4.init(a.r[0].c[3], a.r[1].c[3], a.r[2].c[3], a.r[3].c[3]),
        );
    }

    pub fn scale(m: Mat4, s: f32) Mat4 {
        return initVec4(
            m.r[0].scale(s),
            m.r[1].scale(s),
            m.r[2].scale(s),
            m.r[3].scale(s),
        );
    }

    pub fn mul(a: Mat4, b: Mat4) Mat4 {
        // zig fmt: off
        return init(
        a.r[0].c[0] * b.r[0].c[0] + a.r[0].c[1] * b.r[1].c[0] + a.r[0].c[2] * b.r[2].c[0] + a.r[0].c[3] * b.r[3].c[0],
        a.r[0].c[0] * b.r[0].c[1] + a.r[0].c[1] * b.r[1].c[1] + a.r[0].c[2] * b.r[2].c[1] + a.r[0].c[3] * b.r[3].c[1],
        a.r[0].c[0] * b.r[0].c[2] + a.r[0].c[1] * b.r[1].c[2] + a.r[0].c[2] * b.r[2].c[2] + a.r[0].c[3] * b.r[3].c[2],
        a.r[0].c[0] * b.r[0].c[3] + a.r[0].c[1] * b.r[1].c[3] + a.r[0].c[2] * b.r[2].c[3] + a.r[0].c[3] * b.r[3].c[3],
        a.r[1].c[0] * b.r[0].c[0] + a.r[1].c[1] * b.r[1].c[0] + a.r[1].c[2] * b.r[2].c[0] + a.r[1].c[3] * b.r[3].c[0],
        a.r[1].c[0] * b.r[0].c[1] + a.r[1].c[1] * b.r[1].c[1] + a.r[1].c[2] * b.r[2].c[1] + a.r[1].c[3] * b.r[3].c[1],
        a.r[1].c[0] * b.r[0].c[2] + a.r[1].c[1] * b.r[1].c[2] + a.r[1].c[2] * b.r[2].c[2] + a.r[1].c[3] * b.r[3].c[2],
        a.r[1].c[0] * b.r[0].c[3] + a.r[1].c[1] * b.r[1].c[3] + a.r[1].c[2] * b.r[2].c[3] + a.r[1].c[3] * b.r[3].c[3],
        a.r[2].c[0] * b.r[0].c[0] + a.r[2].c[1] * b.r[1].c[0] + a.r[2].c[2] * b.r[2].c[0] + a.r[2].c[3] * b.r[3].c[0],
        a.r[2].c[0] * b.r[0].c[1] + a.r[2].c[1] * b.r[1].c[1] + a.r[2].c[2] * b.r[2].c[1] + a.r[2].c[3] * b.r[3].c[1],
        a.r[2].c[0] * b.r[0].c[2] + a.r[2].c[1] * b.r[1].c[2] + a.r[2].c[2] * b.r[2].c[2] + a.r[2].c[3] * b.r[3].c[2],
        a.r[2].c[0] * b.r[0].c[3] + a.r[2].c[1] * b.r[1].c[3] + a.r[2].c[2] * b.r[2].c[3] + a.r[2].c[3] * b.r[3].c[3],
        a.r[3].c[0] * b.r[0].c[0] + a.r[3].c[1] * b.r[1].c[0] + a.r[3].c[2] * b.r[2].c[0] + a.r[3].c[3] * b.r[3].c[0],
        a.r[3].c[0] * b.r[0].c[1] + a.r[3].c[1] * b.r[1].c[1] + a.r[3].c[2] * b.r[2].c[1] + a.r[3].c[3] * b.r[3].c[1],
        a.r[3].c[0] * b.r[0].c[2] + a.r[3].c[1] * b.r[1].c[2] + a.r[3].c[2] * b.r[2].c[2] + a.r[3].c[3] * b.r[3].c[2],
        a.r[3].c[0] * b.r[0].c[3] + a.r[3].c[1] * b.r[1].c[3] + a.r[3].c[2] * b.r[2].c[3] + a.r[3].c[3] * b.r[3].c[3],
        );
        // zig fmt: on
    }

    pub fn inv(m: Mat4, out_det: ?*f32) Mat4 {
        const mt = m.transpose();
        var v0: [4]Vec4 = undefined;
        var v1: [4]Vec4 = undefined;

        v0[0] = Vec4.init(mt.r[2].c[0], mt.r[2].c[0], mt.r[2].c[1], mt.r[2].c[1]);
        v1[0] = Vec4.init(mt.r[3].c[2], mt.r[3].c[3], mt.r[3].c[2], mt.r[3].c[3]);
        v0[1] = Vec4.init(mt.r[0].c[0], mt.r[0].c[0], mt.r[0].c[1], mt.r[0].c[1]);
        v1[1] = Vec4.init(mt.r[1].c[2], mt.r[1].c[3], mt.r[1].c[2], mt.r[1].c[3]);
        v0[2] = Vec4.init(mt.r[2].c[0], mt.r[2].c[2], mt.r[0].c[0], mt.r[0].c[2]);
        v1[2] = Vec4.init(mt.r[3].c[1], mt.r[3].c[3], mt.r[1].c[1], mt.r[1].c[3]);

        var d0 = v0[0].mul(v1[0]);
        var d1 = v0[1].mul(v1[1]);
        var d2 = v0[2].mul(v1[2]);

        v0[0] = Vec4.init(mt.r[2].c[2], mt.r[2].c[3], mt.r[2].c[2], mt.r[2].c[3]);
        v1[0] = Vec4.init(mt.r[3].c[0], mt.r[3].c[0], mt.r[3].c[1], mt.r[3].c[1]);
        v0[1] = Vec4.init(mt.r[0].c[2], mt.r[0].c[3], mt.r[0].c[2], mt.r[0].c[3]);
        v1[1] = Vec4.init(mt.r[1].c[0], mt.r[1].c[0], mt.r[1].c[1], mt.r[1].c[1]);
        v0[2] = Vec4.init(mt.r[2].c[1], mt.r[2].c[3], mt.r[0].c[1], mt.r[0].c[3]);
        v1[2] = Vec4.init(mt.r[3].c[0], mt.r[3].c[2], mt.r[1].c[0], mt.r[1].c[2]);

        d0 = v0[0].negMulAdd(v1[0], d0);
        d1 = v0[1].negMulAdd(v1[1], d1);
        d2 = v0[2].negMulAdd(v1[2], d2);

        v0[0] = Vec4.init(mt.r[1].c[1], mt.r[1].c[2], mt.r[1].c[0], mt.r[1].c[1]);
        v1[0] = Vec4.init(d2.c[1], d0.c[1], d0.c[3], d0.c[0]);
        v0[1] = Vec4.init(mt.r[0].c[2], mt.r[0].c[0], mt.r[0].c[1], mt.r[0].c[0]);
        v1[1] = Vec4.init(d0.c[3], d2.c[1], d0.c[1], d0.c[2]);
        v0[2] = Vec4.init(mt.r[3].c[1], mt.r[3].c[2], mt.r[3].c[0], mt.r[3].c[1]);
        v1[2] = Vec4.init(d2.c[3], d1.c[1], d1.c[3], d1.c[0]);
        v0[3] = Vec4.init(mt.r[2].c[2], mt.r[2].c[0], mt.r[2].c[1], mt.r[2].c[0]);
        v1[3] = Vec4.init(d1.c[3], d2.c[3], d1.c[1], d1.c[2]);

        var c0 = v0[0].mul(v1[0]);
        var c2 = v0[1].mul(v1[1]);
        var c4 = v0[2].mul(v1[2]);
        var c6 = v0[3].mul(v1[3]);

        v0[0] = Vec4.init(mt.r[1].c[2], mt.r[1].c[3], mt.r[1].c[1], mt.r[1].c[2]);
        v1[0] = Vec4.init(d0.c[3], d0.c[0], d0.c[1], d2.c[0]);
        v0[1] = Vec4.init(mt.r[0].c[3], mt.r[0].c[2], mt.r[0].c[3], mt.r[0].c[1]);
        v1[1] = Vec4.init(d0.c[2], d0.c[1], d2.c[0], d0.c[0]);
        v0[2] = Vec4.init(mt.r[3].c[2], mt.r[3].c[3], mt.r[3].c[1], mt.r[3].c[2]);
        v1[2] = Vec4.init(d1.c[3], d1.c[0], d1.c[1], d2.c[2]);
        v0[3] = Vec4.init(mt.r[2].c[3], mt.r[2].c[2], mt.r[2].c[3], mt.r[2].c[1]);
        v1[3] = Vec4.init(d1.c[2], d1.c[1], d2.c[2], d1.c[0]);

        c0 = v0[0].negMulAdd(v1[0], c0);
        c2 = v0[1].negMulAdd(v1[1], c2);
        c4 = v0[2].negMulAdd(v1[2], c4);
        c6 = v0[3].negMulAdd(v1[3], c6);

        v0[0] = Vec4.init(mt.r[1].c[3], mt.r[1].c[0], mt.r[1].c[3], mt.r[1].c[0]);
        v1[0] = Vec4.init(d0.c[2], d2.c[1], d2.c[0], d0.c[2]);
        v0[1] = Vec4.init(mt.r[0].c[1], mt.r[0].c[3], mt.r[0].c[0], mt.r[0].c[2]);
        v1[1] = Vec4.init(d2.c[1], d0.c[0], d0.c[3], d2.c[0]);
        v0[2] = Vec4.init(mt.r[3].c[3], mt.r[3].c[0], mt.r[3].c[3], mt.r[3].c[0]);
        v1[2] = Vec4.init(d1.c[2], d2.c[3], d2.c[2], d1.c[2]);
        v0[3] = Vec4.init(mt.r[2].c[1], mt.r[2].c[3], mt.r[2].c[0], mt.r[2].c[2]);
        v1[3] = Vec4.init(d2.c[3], d1.c[0], d1.c[3], d2.c[2]);

        const c1 = v0[0].negMulAdd(v1[0], c0);
        c0 = v0[0].mulAdd(v1[0], c0);

        const c3 = v0[1].mulAdd(v1[1], c2);
        c2 = v0[1].negMulAdd(v1[1], c2);

        const c5 = v0[2].negMulAdd(v1[2], c4);
        c4 = v0[2].mulAdd(v1[2], c4);

        const c7 = v0[3].mulAdd(v1[3], c6);
        c6 = v0[3].negMulAdd(v1[3], c6);

        var mr = Mat4.initVec4(
            Vec4.init(c0.c[0], c1.c[1], c0.c[2], c1.c[3]),
            Vec4.init(c2.c[0], c3.c[1], c2.c[2], c3.c[3]),
            Vec4.init(c4.c[0], c5.c[1], c4.c[2], c5.c[3]),
            Vec4.init(c6.c[0], c7.c[1], c6.c[2], c7.c[3]),
        );

        const d = mr.r[0].dot(mt.r[0]);
        if (out_det != null) {
            out_det.?.* = d;
        }

        if (math.approxEqAbs(f32, d, 0.0, epsilon)) {
            return initZero();
        }

        return mr.scale(1.0 / d);
    }

    pub fn det(a: Mat4) f32 {
        const static = struct {
            const sign = Vec4.init(1.0, -1.0, 1.0, -1.0);
        };

        var v0 = Vec4.init(a.r[2].c[1], a.r[2].c[0], a.r[2].c[0], a.r[2].c[0]);
        var v1 = Vec4.init(a.r[3].c[2], a.r[3].c[2], a.r[3].c[1], a.r[3].c[1]);
        var v2 = Vec4.init(a.r[2].c[1], a.r[2].c[0], a.r[2].c[0], a.r[2].c[0]);
        var v3 = Vec4.init(a.r[3].c[3], a.r[3].c[3], a.r[3].c[3], a.r[3].c[2]);
        var v4 = Vec4.init(a.r[2].c[2], a.r[2].c[2], a.r[2].c[1], a.r[2].c[1]);
        var v5 = Vec4.init(a.r[3].c[3], a.r[3].c[3], a.r[3].c[3], a.r[3].c[2]);

        var p0 = v0.mul(v1);
        var p1 = v2.mul(v3);
        var p2 = v4.mul(v5);

        v0 = Vec4.init(a.r[2].c[2], a.r[2].c[2], a.r[2].c[1], a.r[2].c[1]);
        v1 = Vec4.init(a.r[3].c[1], a.r[3].c[0], a.r[3].c[0], a.r[3].c[0]);
        v2 = Vec4.init(a.r[2].c[3], a.r[2].c[3], a.r[2].c[3], a.r[2].c[2]);
        v3 = Vec4.init(a.r[3].c[1], a.r[3].c[0], a.r[3].c[0], a.r[3].c[0]);
        v4 = Vec4.init(a.r[2].c[3], a.r[2].c[3], a.r[2].c[3], a.r[2].c[2]);
        v5 = Vec4.init(a.r[3].c[2], a.r[3].c[2], a.r[3].c[1], a.r[3].c[1]);

        p0 = v0.negMulAdd(v1, p0);
        p1 = v2.negMulAdd(v3, p1);
        p2 = v4.negMulAdd(v5, p2);

        v0 = Vec4.init(a.r[1].c[3], a.r[1].c[3], a.r[1].c[3], a.r[1].c[2]);
        v1 = Vec4.init(a.r[1].c[2], a.r[1].c[2], a.r[1].c[1], a.r[1].c[1]);
        v2 = Vec4.init(a.r[1].c[1], a.r[1].c[0], a.r[1].c[0], a.r[1].c[0]);

        var s = a.r[0].mul(static.sign);
        var r = v0.mul(p0);
        r = v1.negMulAdd(p1, r);
        r = v2.mulAdd(p2, r);

        return s.dot(r);
    }

    pub fn initRotationX(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        // zig fmt: off
        return init(
            1.0, 0.0, 0.0, 0.0,
            0.0, cosv, sinv, 0.0,
            0.0, -sinv, cosv, 0.0,
            0.0, 0.0, 0.0, 1.0,
        );
        // zig fmt: on
    }

    pub fn initRotationY(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        // zig fmt: off
        return init(
            cosv, 0.0, -sinv, 0.0,
            0.0, 1.0, 0.0, 0.0,
            sinv, 0.0, cosv, 0.0,
            0.0, 0.0, 0.0, 1.0,
        );
        // zig fmt: on
    }

    pub fn initRotationZ(angle: f32) Mat4 {
        const sinv = math.sin(angle);
        const cosv = math.cos(angle);
        // zig fmt: off
        return init(
            cosv, sinv, 0.0, 0.0,
            -sinv, cosv, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0,
        );
        // zig fmt: on
    }

    pub fn initRotationQuat(q: Quat) Mat4 {
        const static = struct {
            const const1110 = Vec4.init(1.0, 1.0, 1.0, 0.0);
        };
        const vin = Vec4.init(q.q[0], q.q[1], q.q[2], q.q[3]);

        const q0 = vin.add(vin);
        const q1 = vin.mul(q0);

        var v0 = Vec4.init(q1.c[1], q1.c[0], q1.c[0], static.const1110.c[3]);
        var v1 = Vec4.init(q1.c[2], q1.c[2], q1.c[1], static.const1110.c[3]);
        var r0 = static.const1110.sub(v0);
        r0 = r0.sub(v1);

        v0 = Vec4.init(vin.c[0], vin.c[0], vin.c[1], vin.c[3]);
        v1 = Vec4.init(q0.c[2], q0.c[1], q0.c[2], q0.c[3]);
        v0 = v0.mul(v1);

        v1 = Vec4.init(vin.c[3], vin.c[3], vin.c[3], vin.c[3]);
        var v2 = Vec4.init(q0.c[1], q0.c[2], q0.c[0], q0.c[3]);
        v1 = v1.mul(v2);

        var r1 = v0.add(v1);
        var r2 = v0.sub(v1);

        v0 = Vec4.init(r1.c[1], r2.c[0], r2.c[1], r1.c[2]);
        v1 = Vec4.init(r1.c[0], r2.c[2], r1.c[0], r2.c[2]);

        return Mat4.initVec4(
            Vec4.init(r0.c[0], v0.c[0], v0.c[1], r0.c[3]),
            Vec4.init(v0.c[2], r0.c[1], v0.c[3], r0.c[3]),
            Vec4.init(v1.c[0], v1.c[1], r0.c[2], r0.c[3]),
            Vec4.init(0.0, 0.0, 0.0, 1.0),
        );
    }

    pub fn initPerspectiveFovLh(fovy: f32, aspect: f32, near: f32, far: f32) Mat4 {
        const sinfov = math.sin(0.5 * fovy);
        const cosfov = math.cos(0.5 * fovy);

        assert(near > 0.0 and far > 0.0 and far > near);
        assert(!math.approxEqAbs(f32, sinfov, 0.0, 0.001));
        assert(!math.approxEqAbs(f32, far, near, 0.001));
        assert(!math.approxEqAbs(f32, aspect, 0.0, 0.01));

        const h = cosfov / sinfov;
        const w = h / aspect;
        const r = far / (far - near);
        // zig fmt: off
        return init(
            w, 0.0, 0.0, 0.0,
            0.0, h, 0.0, 0.0,
            0.0, 0.0, r, 1.0,
            0.0, 0.0, -r * near, 0.0,
        );
        // zig fmt: on
    }

    pub inline fn initZero() Mat4 {
        const static = struct {
            // zig fmt: off
            const zero = init(
                0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0,
            );
            // zig fmt: on
        };
        return static.zero;
    }

    pub inline fn initIdentity() Mat4 {
        const static = struct {
            // zig fmt: off
            const identity = init(
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0,
            );
            // zig fmt: on
        };
        return static.identity;
    }

    pub fn initTranslation(v: Vec3) Mat4 {
        // zig fmt: off
        return init(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            v.c[0], v.c[1], v.c[2], 1.0,
        );
        // zig fmt: on
    }

    pub fn initScaling(v: Vec3) Mat4 {
        // zig fmt: off
        return init(
            v.c[0], 0.0, 0.0, 0.0,
            0.0, v.c[1], 0.0, 0.0,
            0.0, 0.0, v.c[2], 0.0,
            0.0, 0.0, 0.0, 1.0,
        );
        // zig fmt: on
    }

    pub fn initDiagonal(s: f32) Mat4 {
        // zig fmt: off
        return init(
            s, 0.0, 0.0, 0.0,
            0.0, s, 0.0, 0.0,
            0.0, 0.0, s, 0.0,
            0.0, 0.0, 0.0, s,
        );
        // zig fmt: on
    }

    pub fn initLookToLh(eye_pos: Vec3, eye_dir: Vec3, up_dir: Vec3) Mat4 {
        const az = Vec3.normalize(eye_dir);
        const ax = Vec3.normalize(Vec3.cross(up_dir, az));
        const ay = Vec3.normalize(Vec3.cross(az, ax));
        // zig fmt: off
        return init(
            ax.c[0], ay.c[0], az.c[0], 0.0,
            ax.c[1], ay.c[1], az.c[1], 0.0,
            ax.c[2], ay.c[2], az.c[2], 0.0,
            -Vec3.dot(ax, eye_pos), -Vec3.dot(ay, eye_pos), -Vec3.dot(az, eye_pos), 1.0,
        );
        // zig fmt: on
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
        assert(!math.approxEqAbs(f32, view_right, view_left, 0.001));
        assert(!math.approxEqAbs(f32, view_top, view_bottom, 0.001));
        assert(!math.approxEqAbs(f32, far_z, near_z, 0.001));

        const rcp_w = 1.0 / (view_right - view_left);
        const rcp_h = 1.0 / (view_top - view_bottom);
        const range = 1.0 / (far_z - near_z);

        // zig fmt: off
        return init(
            rcp_w + rcp_w, 0.0, 0.0, 0.0,
            0.0, rcp_h + rcp_h, 0.0, 0.0,
            0.0, 0.0, range, 0.0,
            -(view_left + view_right) * rcp_w, -(view_top + view_bottom) * rcp_h, -range * near_z, 1.0,
        );
        // zig fmt: on
    }
};

test "dot" {
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(math.approxEqAbs(f32, a.dot(b), 11.0, epsilon));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(4.0, 5.0, 6.0);
        assert(math.approxEqAbs(f32, a.dot(b), 32.0, epsilon));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, 4.0);
        const b = Vec4.init(5.0, 6.0, 7.0, 8.0);
        assert(math.approxEqAbs(f32, a.dot(b), 70.0, epsilon));
    }
}

test "cross" {
    {
        const a = Vec3.init(1.0, 0.0, 0.0);
        const b = Vec3.init(0.0, 1.0, 0.0);
        assert(a.cross(b).approxEq(Vec3.init(0.0, 0.0, 1.0), epsilon));
    }
    {
        const a = Vec3.init(0.0, 0.0, -1.0);
        const b = Vec3.init(1.0, 0.0, 0.0);
        assert(a.cross(b).approxEq(Vec3.init(0.0, -1.0, 0.0), epsilon));
    }
}

test "VecN add, sub, scale" {
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(a.add(b).approxEq(Vec2.init(4.0, 6.0), epsilon));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(3.0, 4.0, 5.0);
        assert(a.add(b).approxEq(Vec3.init(4.0, 6.0, 8.0), epsilon));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        const b = Vec4.init(3.0, 4.0, 5.0, 2.0);
        assert(a.add(b).approxEq(Vec4.init(4.0, 6.0, 8.0, 1.0), epsilon));
    }
    {
        const a = Vec2.init(1.0, 2.0);
        const b = Vec2.init(3.0, 4.0);
        assert(a.sub(b).approxEq(Vec2.init(-2.0, -2.0), epsilon));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        const b = Vec3.init(3.0, 4.0, 5.0);
        assert(a.sub(b).approxEq(Vec3.init(-2.0, -2.0, -2.0), epsilon));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        const b = Vec4.init(3.0, 4.0, 5.0, 2.0);
        assert(a.sub(b).approxEq(Vec4.init(-2.0, -2.0, -2.0, -3.0), epsilon));
    }
    {
        const a = Vec2.init(1.0, 2.0);
        assert(a.scale(2.0).approxEq(Vec2.init(2.0, 4.0), epsilon));
    }
    {
        const a = Vec3.init(1.0, 2.0, 3.0);
        assert(a.scale(-1.0).approxEq(Vec3.init(-1.0, -2.0, -3.0), epsilon));
    }
    {
        const a = Vec4.init(1.0, 2.0, 3.0, -1.0);
        assert(a.scale(3.0).approxEq(Vec4.init(3.0, 6.0, 9.0, -3.0), epsilon));
    }
}

test "length, normalize" {
    {
        const a = Vec2.init(2.0, 3.0).length();
        assert(math.approxEqAbs(f32, a, 3.60555, epsilon));
    }
    {
        const a = Vec3.init(1.0, 1.0, 1.0).length();
        assert(math.approxEqAbs(f32, a, 1.73205, epsilon));
    }
    {
        const a = Vec4.init(1.0, 1.0, 1.0, 1.0).length();
        assert(math.approxEqAbs(f32, a, 2.0, epsilon));
    }
    {
        const a = Vec2.init(2.0, 4.0).normalize();
        assert(Vec2.approxEq(a, Vec2.init(0.447214, 0.894427), epsilon));
    }
    {
        const a = Vec3.init(2.0, -5.0, 4.0).normalize();
        assert(Vec3.approxEq(a, Vec3.init(0.298142, -0.745356, 0.596285), epsilon));
    }
    {
        const a = Vec4.init(-1.0, 2.0, -5.0, 4.0).normalize();
        assert(Vec4.approxEq(a, Vec4.init(-0.147442, 0.294884, -0.73721, 0.589768), epsilon));
    }
    {
        const a = Quat.init(-1.0, 2.0, -5.0, 4.0).normalize();
        assert(Quat.approxEq(a, Quat.init(-0.147442, 0.294884, -0.73721, 0.589768), epsilon));
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
            epsilon,
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
        epsilon,
    ));
}

test "Mat4 inv, det" {
    var m = Mat4.initVec4(
        Vec4.init(10.0, -9.0, -12.0, 1.0),
        Vec4.init(7.0, -12.0, 11.0, 1.0),
        Vec4.init(-10.0, 10.0, 3.0, 1.0),
        Vec4.init(1.0, 2.0, 3.0, 4.0),
    );
    assert(math.approxEqAbs(f32, m.det(), 2939.0, epsilon));

    var det: f32 = 0.0;
    m = m.inv(&det);
    assert(math.approxEqAbs(f32, det, 2939.0, epsilon));
    assert(m.approxEq(
        Mat4.initVec4(
            Vec4.init(-0.170806, -0.13576, -0.349439, 0.164001),
            Vec4.init(-0.163661, -0.14801, -0.253147, 0.141204),
            Vec4.init(-0.0871045, 0.00646478, -0.0785982, 0.0398095),
            Vec4.init(0.18986, 0.103096, 0.272882, 0.10854),
        ),
        epsilon,
    ));
}

test "Quat mul, inv" {
    {
        const a = Quat.init(2.0, 3.0, 4.0, 1.0);
        const b = Quat.init(6.0, 7.0, 8.0, 5.0);
        assert(a.mul(b).approxEq(Quat.init(20.0, 14.0, 32.0, -60.0), epsilon));
        assert(b.mul(a).approxEq(Quat.init(12.0, 30.0, 24.0, -60.0), epsilon));
    }
    {
        const a = Quat.init(2.0, 3.0, 4.0, 1.0);
        const b = a.inv();
        assert(a.approxEq(Quat.init(2.0, 3.0, 4.0, 1.0), epsilon));
        assert(b.approxEq(Quat.init(-0.0666667, -0.1, -0.133333, 0.0333333), epsilon));
    }
}

test "Mat4 transforms" {
    const a = Mat4.initTranslation(Vec3.init(1.0, 0.0, 0.0));
    const b = Mat4.initRotationY(math.pi * 0.5);
    const c = Vec3.init(1.0, 0.0, 0.0);
    const e = Mat4.initTranslation(Vec3.init(0.0, 1.0, 0.0));
    const d = c.transform(a.mul(b).mul(e));
    assert(d.approxEq(Vec3.init(0.0, 1.0, -2.0), epsilon));
}

test "Mat4 <-> Quat" {
    {
        const a = Quat.initRotationMat4(Mat4.initIdentity());
        assert(a.approxEq(Quat.initIdentity(), epsilon));
        const b = Mat4.initRotationQuat(a);
        assert(b.approxEq(Mat4.initIdentity(), epsilon));
    }
    {
        const a = Quat.initRotationMat4(Mat4.initTranslation(Vec3.init(1.0, 2.0, 3.0)));
        assert(a.approxEq(Quat.initIdentity(), epsilon));
        const b = Mat4.initRotationQuat(a);
        assert(b.approxEq(Mat4.initIdentity(), epsilon));
    }
    {
        const a = Quat.initRotationMat4(Mat4.initRotationY(math.pi * 0.5));
        const b = Mat4.initRotationQuat(a);
        assert(b.approxEq(Mat4.initRotationY(math.pi * 0.5), epsilon));
    }
    {
        const a = Quat.initRotationMat4(Mat4.initRotationY(math.pi * 0.25));
        const b = Quat.initRotationAxis(Vec3.init(0.0, 1.0, 0.0), math.pi * 0.25);
        assert(a.approxEq(b, epsilon));
    }
    {
        const m0 = Mat4.initRotationX(math.pi * 0.125);
        const m1 = Mat4.initRotationY(math.pi * 0.25);
        const m2 = Mat4.initRotationZ(math.pi * 0.5);

        const q0 = Quat.initRotationMat4(m0);
        const q1 = Quat.initRotationMat4(m1);
        const q2 = Quat.initRotationMat4(m2);

        const mr = m0.mul(m1).mul(m2);
        const qr = q0.mul(q1).mul(q2);

        assert(mr.approxEq(Mat4.initRotationQuat(qr), epsilon));
        assert(qr.approxEq(Quat.initRotationMat4(mr), epsilon));
    }
}

test "slerp" {
    const from = Quat.init(0.0, 0.0, 0.0, 1.0);
    const to = Quat.init(0.5, 0.5, -0.5, 0.5);
    const result = from.slerp(to, 0.5);
    assert(result.approxEq(Quat.init(0.28867513, 0.28867513, -0.28867513, 0.86602540), epsilon));
}
