const std = @import("std");
const math = std.math;
const assert = std.debug.assert;

pub const Vec = @Vector(4, f32);
pub const VecBool = @Vector(4, bool);

pub inline fn vecZero() Vec {
    return @splat(4, @as(f32, 0));
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}

pub inline fn vecBoolSet(x: bool, y: bool, z: bool, w: bool) VecBool {
    return [4]bool{ x, y, z, w };
}

pub inline fn vecBoolAnd(b0: VecBool, b1: VecBool) VecBool {
    return vecBoolSet(b0[0] and b1[0], b0[1] and b1[1], b0[2] and b1[2], b0[3] and b1[3]);
}

pub inline fn vecBoolOr(b0: VecBool, b1: VecBool) VecBool {
    return vecBoolSet(b0[0] or b1[0], b0[1] or b1[1], b0[2] or b1[2], b0[3] or b1[3]);
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    return @bitCast(Vec, [4]u32{ x, y, z, w });
}

pub inline fn vecSplat(value: f32) Vec {
    return @splat(4, value);
}

pub inline fn vecSplatInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}

pub inline fn vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) VecBool {
    const delta = v0 - v1;
    var temp = vecZero();
    temp = temp - delta;
    temp = vecMax(temp, delta);
    return temp <= epsilon;
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

pub inline fn vecMin(v0: Vec, v1: Vec) Vec {
    return @minimum(v0, v1);
}

pub inline fn vecMax(v0: Vec, v1: Vec) Vec {
    return @maximum(v0, v1);
}

test "vecZero" {
    const v = vecZero();
    assert(vecApproxEqAbs(v, [4]f32{ 0.0, 0.0, 0.0, 0.0 }, 0.0));
}

test "vecSet" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSet(5.0, -6.0, 7.0, 8.0);
    assert(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
    assert(v1[0] == 5.0 and v1[1] == -6.0 and v1[2] == 7.0 and v1[3] == 8.0);
}

test "vecSetInt" {
    const v = vecSetInt(0x3f80_0000, 0x4000_0000, 0x4040_0000, 0x4080_0000);
    assert(vecApproxEqAbs(v, [4]f32{ 1.0, 2.0, 3.0, 4.0 }, 0.0));
}

test "vecSplat" {
    const v = vecSplat(123.0);
    assert(vecApproxEqAbs(v, [4]f32{ 123.0, 123.0, 123.0, 123.0 }, 0.0));
}

test "vecSplatInt" {
    const v = vecSplatInt(0x4000_0000);
    assert(vecApproxEqAbs(v, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
}

test "vecMin and vecMax" {
    const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
    const v1 = vecSet(2.0, 1.0, 4.0, 7.0);
    const vmin = vecMin(v0, v1);
    const vmax = vecMax(v0, v1);
    const less = v0 < v1;
    assert(vecApproxEqAbs(vmin, [4]f32{ 1.0, 1.0, 2.0, 7.0 }, 0.0));
    assert(vecApproxEqAbs(vmax, [4]f32{ 2.0, 3.0, 4.0, 7.0 }, 0.0));
    assert(less[0] == true and less[1] == false and less[2] == true and less[3] == false);
}

test "vecLoadFloat2" {
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

test "vecStoreFloat3" {
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

test "vecNearEqual" {
    const v0 = vecSet(1.0, 2.0, -3.0, 4.001);
    const v1 = vecSet(1.0, 2.1, 3.0, 4.0);
    const b = vecNearEqual(v0, v1, vecSplat(0.01));
    assert(b[0] == true);
    assert(b[1] == false);
    assert(b[2] == false);
    assert(b[3] == true);
    assert(@reduce(.And, b == vecBoolSet(true, false, false, true)));
}

test "vecBoolAnd" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolAnd(b0, b1);
    assert(b[0] == true and b[1] == false and b[2] == false and b[3] == false);
}

test "vecBoolOr" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolOr(b0, b1);
    assert(b[0] == true and b[1] == true and b[2] == true and b[3] == false);
}

test "vec @sin" {
    const v0 = vecSplat(0.5 * math.pi);
    const v = @sin(v0);
    assert(vecApproxEqAbs(v, [4]f32{ 1.0, 1.0, 1.0, 1.0 }, 0.001));
}
