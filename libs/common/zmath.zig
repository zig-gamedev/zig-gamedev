const std = @import("std");
const math = std.math;
const assert = std.debug.assert;

pub const Vec = @Vector(4, f32);

pub inline fn vecZero() Vec {
    // _mm_setzero_ps()
    return [4]f32{ 0.0, 0.0, 0.0, 0.0 };
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    const xx = @bitCast(f32, x);
    const yy = @bitCast(f32, y);
    const zz = @bitCast(f32, z);
    const ww = @bitCast(f32, w);
    return [4]f32{ xx, yy, zz, ww };
}

pub inline fn vecReplicate(value: f32) Vec {
    return @splat(4, value);
}

pub inline fn vecReplicatePtr(ptr: *const f32) Vec {
    return @splat(4, ptr.*);
}

pub inline fn vecReplicateInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}

pub inline fn vecReplicateIntPtr(ptr: *const u32) Vec {
    return @splat(4, @bitCast(f32, ptr.*));
}

pub inline fn vecTrueInt() Vec {
    const static = struct {
        const v = vecReplicateInt(0xffff_ffff);
    };
    return static.v;
}

pub inline fn vecFalseInt() Vec {
    return vecZero();
}

pub inline fn vecLess(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 < v1, vecTrueInt(), vecFalseInt());
}

pub inline fn vecLessOrEqual(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 <= v1, vecTrueInt(), vecFalseInt());
}

pub inline fn vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) Vec {
    const delta = v0 - v1;
    var temp = vecZero();
    temp = temp - delta;
    temp = @maximum(temp, delta);
    return vecLessOrEqual(temp, epsilon);
}

pub inline fn vecAnd(v0: Vec, v1: Vec) Vec {
    const v0u = @bitCast(@Vector(4, u32), v0);
    const v1u = @bitCast(@Vector(4, u32), v1);
    const vu = v0u & v1u;
    return @bitCast(Vec, vu);
}

pub inline fn vecAdd(v0: Vec, v1: Vec) Vec {
    return v0 + v1;
}

fn vecApproxEqAbs(v0: Vec, v1: Vec, eps: f32) bool {
    return math.approxEqAbs(f32, v0[0], v1[0], eps) and
        math.approxEqAbs(f32, v0[1], v1[1], eps) and
        math.approxEqAbs(f32, v0[2], v1[2], eps) and
        math.approxEqAbs(f32, v0[3], v1[3], eps);
}

test "basic" {
    {
        const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
        const v1 = vecSet(5.0, 6.0, 7.0, 8.0);
        assert(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
        assert(v1[0] == 5.0 and v1[1] == 6.0 and v1[2] == 7.0 and v1[3] == 8.0);
        const v = vecAdd(v0, v1);
        assert(vecApproxEqAbs(v, [4]f32{ 6.0, 8.0, 10.0, 12.0 }, 0.00001));
    }
    {
        const v = vecReplicate(123.0);
        assert(vecApproxEqAbs(v, [4]f32{ 123.0, 123.0, 123.0, 123.0 }, 0.0));
    }
    {
        const v = vecZero();
        assert(vecApproxEqAbs(v, [4]f32{ 0.0, 0.0, 0.0, 0.0 }, 0.0));
    }
    {
        const v = vecReplicateInt(0x4000_0000);
        assert(vecApproxEqAbs(v, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
    }
    {
        const f: f32 = 7.0;
        const v = vecReplicatePtr(&f);
        assert(vecApproxEqAbs(v, [4]f32{ 7.0, 7.0, 7.0, 7.0 }, 0.0));
    }
    {
        const u: u32 = 0x3f80_0000;
        const v = vecReplicateIntPtr(&u);
        assert(vecApproxEqAbs(v, [4]f32{ 1.0, 1.0, 1.0, 1.0 }, 0.0));
    }
    {
        const v = vecSetInt(0x3f80_0000, 0x4000_0000, 0x4040_0000, 0x4080_0000);
        assert(vecApproxEqAbs(v, [4]f32{ 1.0, 2.0, 3.0, 4.0 }, 0.0));
    }
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(2.0, 1.0, 4.0, 7.0);
        const vmin = @minimum(v0, v1);
        const vmax = @maximum(v0, v1);
        const less = v0 < v1;
        assert(vecApproxEqAbs(vmin, [4]f32{ 1.0, 1.0, 2.0, 7.0 }, 0.0));
        assert(vecApproxEqAbs(vmax, [4]f32{ 2.0, 3.0, 4.0, 7.0 }, 0.0));
        assert(less[0] == true and less[1] == false and less[2] == true and less[3] == false);
    }
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(1.0, 1.0, -2.0, 7.001);
        const v = vecNearEqual(v0, v1, vecReplicate(0.01));
        assert(@bitCast(u32, v[0]) == 0xffff_ffff);
        assert(@bitCast(u32, v[1]) == 0x0000_0000);
        assert(@bitCast(u32, v[2]) == 0x0000_0000);
        assert(@bitCast(u32, v[3]) == 0xffff_ffff);
        const vv = vecAnd(v0, v);
        assert(vv[0] == 1.0);
        assert(vv[1] == 0.0);
        assert(vv[2] == 0.0);
        assert(vv[3] == 7.0);
    }
}
