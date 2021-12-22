const std = @import("std");
const math = std.math;
const assert = std.debug.assert;

pub const Vec = @Vector(4, f32);
const VecU32 = @Vector(4, u32);

pub inline fn vecZero() Vec {
    return @splat(4, @as(f32, 0));
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    const vu = [4]u32{ x, y, z, w };
    return @bitCast(Vec, vu);
}

pub inline fn vecReplicate(value: f32) Vec {
    return @splat(4, value);
}

pub inline fn vecReplicateInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}

pub inline fn vecTrueInt() Vec {
    return @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff)));
}

pub inline fn vecFalseInt() Vec {
    return @splat(4, @as(f32, 0));
}

pub inline fn vecEqual(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 == v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecEqualInt(v0: Vec, v1: Vec) Vec {
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @select(f32, v0u == v1u, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) Vec {
    const delta = v0 - v1;
    var temp = vecZero();
    temp = temp - delta;
    temp = @maximum(temp, delta);
    return vecLessOrEqual(temp, epsilon);
}

pub inline fn vecNotEqual(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 != v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecNotEqualInt(v0: Vec, v1: Vec) Vec {
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @select(f32, v0u != v1u, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecGreater(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 > v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecGreaterOrEqual(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 >= v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecLess(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 < v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
}

pub inline fn vecLessOrEqual(v0: Vec, v1: Vec) Vec {
    return @select(f32, v0 <= v1, @bitCast(Vec, @splat(4, @as(u32, 0xffff_ffff))), @splat(4, @as(f32, 0)));
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

pub inline fn vecLoadFloat(mem: []const f32) Vec {
    return [4]f32{ mem[0], 0, 0, 0 };
}

pub inline fn vecLoadFloat2(mem: []const f32) Vec {
    return [4]f32{ mem[0], mem[1], 0, 0 };
}

pub inline fn vecLoadFloat3(mem: []const f32) Vec {
    return vecSet(mem[0], mem[1], mem[2], 0);
}

pub inline fn vecLoadFloat4(mem: []const f32) Vec {
    return vecSet(mem[0], mem[1], mem[2], mem[4]);
}

pub inline fn vecStoreFloat(mem: []f32, v: Vec) void {
    mem[0] = v[0];
}

pub inline fn vecStoreFloat2(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
}

pub inline fn vecStoreFloat3(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
    mem[2] = v[2];
}

pub inline fn vecStoreFloat4(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
    mem[2] = v[2];
    mem[3] = v[3];
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
        const v0 = vecSet(1.0, 3.0, -2.0, 7.0);
        const v1 = vecSet(1.0, 1.0, 2.0, 7.001);
        var v = vecEqual(v0, v1);
        assert(@bitCast(u32, v[0]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[1]) == 0);
        assert(@bitCast(u32, v[2]) == 0);
        assert(@bitCast(u32, v[3]) == 0);
        v = vecEqualInt(v, vecSetInt(~@as(u32, 0), 0, 0, 0));
        assert(@bitCast(u32, v[0]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[1]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[2]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[3]) == ~@as(u32, 0));
        assert(@reduce(.And, @bitCast(VecU32, v)) == ~@as(u32, 0));
    }
    {
        const v0 = vecSet(1.0, 3.0, -2.0, 7.0);
        const v1 = vecSet(1.0, 1.0, 2.0, 7.001);
        var v = vecNotEqual(v0, v1);
        assert(@bitCast(u32, v[0]) == 0);
        assert(@bitCast(u32, v[1]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[2]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[3]) == ~@as(u32, 0));
        v = vecNotEqualInt(v, vecSetInt(~@as(u32, 0), 0, 0, 0));
        assert(@bitCast(u32, v[0]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[1]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[2]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[3]) == ~@as(u32, 0));
        assert(@reduce(.And, @bitCast(VecU32, v)) == ~@as(u32, 0));
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
        assert(vecApproxEqAbs(vv, [4]f32{ 1.0, 0.0, 0.0, 7.0 }, 0.0));
    }
    {
        const v0 = vecSet(1.0, 3.0, -2.0, 7.0);
        const v1 = vecSet(1.0, 1.0, 2.0, 7.001);
        const v = vecLess(v0, v1);
        assert(@bitCast(u32, v[0]) == 0x0000_0000);
        assert(@bitCast(u32, v[1]) == 0x0000_0000);
        assert(@bitCast(u32, v[2]) == 0xffff_ffff);
        assert(@bitCast(u32, v[3]) == 0xffff_ffff);
    }
    {
        const v0 = vecSet(1.0, 3.0, -2.0, 7.002);
        const v1 = vecSet(1.0, 1.0, 2.0, 7.001);
        const v = vecGreater(v0, v1);
        assert(@bitCast(u32, v[0]) == 0);
        assert(@bitCast(u32, v[1]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[2]) == 0);
        assert(@bitCast(u32, v[3]) == ~@as(u32, 0));
    }
    {
        const v0 = vecSet(1.0, 3.0, -2.0, 7.002);
        const v1 = vecSet(1.0, 1.0, 2.0, 7.001);
        const v = vecGreaterOrEqual(v0, v1);
        assert(@bitCast(u32, v[0]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[1]) == ~@as(u32, 0));
        assert(@bitCast(u32, v[2]) == 0);
        assert(@bitCast(u32, v[3]) == ~@as(u32, 0));
    }
    {
        const a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
        var ptr = &a;
        var i: u32 = 0;
        const v0 = vecLoadFloat2(a[i..]);
        assert(vecApproxEqAbs(v0, [4]f32{ 1.0, 2.0, 0.0, 0.0 }, 0.0));
        i += 2;
        const v1 = vecLoadFloat2(a[i .. i + 2]);
        assert(vecApproxEqAbs(v1, [4]f32{ 3.0, 4.0, 0.0, 0.0 }, 0.0));
        const v2 = vecLoadFloat2(a[5..7]);
        assert(vecApproxEqAbs(v2, [4]f32{ 6.0, 7.0, 0.0, 0.0 }, 0.0));
        const v3 = vecLoadFloat2(ptr[1..]);
        assert(vecApproxEqAbs(v3, [4]f32{ 2.0, 3.0, 0.0, 0.0 }, 0.0));
        i += 1;
        const v4 = vecLoadFloat2(ptr[i .. i + 2]);
        assert(vecApproxEqAbs(v4, [4]f32{ 4.0, 5.0, 0.0, 0.0 }, 0.0));
    }
    {
        var a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
        const v = vecLoadFloat3(a[1..]);
        vecStoreFloat4(a[2..], v);
        assert(a[0] == 1.0);
        assert(a[1] == 2.0);
        assert(a[2] == 2.0);
        assert(a[3] == 3.0);
        assert(a[4] == 4.0);
        assert(a[5] == 0.0);
    }
}
