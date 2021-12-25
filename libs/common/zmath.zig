const builtin = @import("builtin");
const std = @import("std");
const math = std.math;

pub const Vec = @Vector(4, f32);
pub const VecBool = @Vector(4, bool);

const use_inline_asm = (builtin.target.cpu.arch == .x86_64);

pub inline fn vecZero() Vec {
    return @splat(4, @as(f32, 0));
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}

pub inline fn vecBoolAnd(b0: VecBool, b1: VecBool) VecBool {
    // andps
    return [4]bool{ b0[0] and b1[0], b0[1] and b1[1], b0[2] and b1[2], b0[3] and b1[3] };
}

pub inline fn vecBoolOr(b0: VecBool, b1: VecBool) VecBool {
    // orps
    return [4]bool{ b0[0] or b1[0], b0[1] or b1[1], b0[2] or b1[2], b0[3] or b1[3] };
}

pub inline fn vecBoolAllTrue(b: VecBool) bool {
    return @reduce(.And, b);
}

pub inline fn vecBoolEqual(b0: VecBool, b1: VecBool) bool {
    return vecBoolAllTrue(b0 == b1);
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    return @bitCast(Vec, [4]u32{ x, y, z, w });
}

pub inline fn vecSplat(value: f32) Vec {
    return @splat(4, value);
}

pub inline fn vecSplatX(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 0, 0, 0, 0 });
}

pub inline fn vecSplatY(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 1, 1, 1, 1 });
}

pub inline fn vecSplatZ(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 2, 2, 2, 2 });
}

pub inline fn vecSplatW(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 3, 3, 3, 3 });
}

// zig fmt: off
pub inline fn vecSplatInf() Vec { return vecSplat(math.inf_f32); }
pub inline fn vecSplatNan() Vec { return vecSplat(math.nan_f32); }
pub inline fn vecSplatQnan() Vec { return vecSplat(math.qnan_f32); }
pub inline fn vecSplatEpsilon() Vec { return vecSplat(math.epsilon_f32); }
pub inline fn vecSplatSignMask() Vec { return vecSplatInt(0x8000_0000); }
pub inline fn vecSplatAbsMask() Vec { return vecSplatInt(0x7fff_ffff); }
pub inline fn vecSplatNoFraction() Vec { return vecSplat(8_388_608.0); }
// zig fmt: on

pub inline fn vecSplatInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}

pub inline fn vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) VecBool {
    const delta = v0 - v1;
    const temp = vecMax(delta, vecZero() - delta);
    return temp <= epsilon;
}

pub inline fn vecAnd(v0: Vec, v1: Vec) Vec {
    // andps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u & v1u);
}

pub inline fn vecAndNot(v0: Vec, v1: Vec) Vec {
    // andnps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, ~v0u & v1u);
}

pub inline fn vecOr(v0: Vec, v1: Vec) Vec {
    // orps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u | v1u);
}

pub inline fn vecXor(v0: Vec, v1: Vec) Vec {
    // xorps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u ^ v1u);
}

pub inline fn vecIsNan(v: Vec) VecBool {
    return v != v;
}

pub inline fn vecIsInf(v: Vec) VecBool {
    return vecAnd(v, vecSplatAbsMask()) == vecSplatInf();
}

pub inline fn vecLoadF32(mem: []const f32) Vec {
    return [4]f32{ mem[0], 0, 0, 0 };
}

pub inline fn vecLoadF32x2(mem: []const f32) Vec {
    return [4]f32{ mem[0], mem[1], 0, 0 };
}

pub inline fn vecLoadF32x3(mem: []const f32) Vec {
    return vecSet(mem[0], mem[1], mem[2], 0);
}

pub inline fn vecLoadF32x4(mem: []const f32) Vec {
    return vecSet(mem[0], mem[1], mem[2], mem[4]);
}

pub inline fn vecStoreF32(mem: []f32, v: Vec) void {
    mem[0] = v[0];
}

pub inline fn vecStoreF32x2(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
}

pub inline fn vecStoreF32x3(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
    mem[2] = v[2];
}

pub inline fn vecStoreF32x4(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
    mem[2] = v[2];
    mem[3] = v[3];
}

pub inline fn vecMinFast(v0: Vec, v1: Vec) Vec {
    // minps
    return @select(f32, v0 < v1, v0, v1);
}

pub inline fn vecMaxFast(v0: Vec, v1: Vec) Vec {
    // maxps
    return @select(f32, v0 > v1, v0, v1);
}

pub inline fn vecMin(v0: Vec, v1: Vec) Vec {
    // This will handle inf & nan
    return @minimum(v0, v1);
}

pub inline fn vecMax(v0: Vec, v1: Vec) Vec {
    // This will handle inf & nan
    return @maximum(v0, v1);
}

pub inline fn vecSelect(b: VecBool, v0: Vec, v1: Vec) Vec {
    return @select(f32, b, v0, v1);
}

pub inline fn vecInBounds(v: Vec, bounds: Vec) VecBool {
    const b0 = v <= bounds;
    const b1 = (bounds * vecSplat(-1.0)) <= v;
    return vecBoolAnd(b0, b1);
}

pub inline fn vecRound(v: Vec) Vec {
    const sign = vecAnd(v, vecSplatSignMask());
    const magic = vecOr(vecSplatNoFraction(), sign);
    var r1 = v + magic;
    r1 = r1 - magic;
    const r2 = vecAbs(v);
    const mask = r2 <= vecSplatNoFraction();
    return vecSelect(mask, r1, v);
}

pub inline fn vecTrunc(v: Vec) Vec {
    const mask = vecAbs(v) < vecSplatNoFraction();
    const result = vecFloatToIntAndBack(v);
    return vecSelect(mask, result, v);
}

pub inline fn vecFloor(v: Vec) Vec {
    const mask = vecAbs(v) < vecSplatNoFraction();
    var result = vecFloatToIntAndBack(v);
    const larger_mask = result > v;
    const larger = vecSelect(larger_mask, vecSplat(-1.0), vecZero());
    result = result + larger;
    return vecSelect(mask, result, v);
}

pub inline fn vecCeil(v: Vec) Vec {
    const mask = vecAbs(v) < vecSplatNoFraction();
    var result = vecFloatToIntAndBack(v);
    const smaller_mask = result < v;
    const smaller = vecSelect(smaller_mask, vecSplat(-1.0), vecZero());
    result = result - smaller;
    return vecSelect(mask, result, v);
}

pub inline fn vecClamp(v: Vec, min: Vec, max: Vec) Vec {
    var result = vecMax(min, v);
    result = vecMin(max, result);
    return result;
}

pub inline fn vecClampFast(v: Vec, min: Vec, max: Vec) Vec {
    var result = vecMaxFast(min, v);
    result = vecMinFast(max, result);
    return result;
}

pub inline fn vecSaturate(v: Vec) Vec {
    var result = vecMax(v, vecZero());
    result = vecMin(result, vecSplat(1.0));
    return result;
}

pub inline fn vecSaturateFast(v: Vec) Vec {
    var result = vecMaxFast(v, vecZero());
    result = vecMinFast(result, vecSplat(1.0));
    return result;
}

pub inline fn vecAbs(v: Vec) Vec {
    // load, andps
    return @fabs(v);
}

pub inline fn vecSqrt(v: Vec) Vec {
    // sqrtps
    return @sqrt(v);
}

pub inline fn vecRcp(v: Vec) Vec {
    // load, divps
    return vecSplat(1.0) / v;
}

pub inline fn vecRcpFast(v: Vec) Vec {
    // load, rcpps, 2 x mulps, addps, subps
    @setFloatMode(.Optimized);
    return vecSplat(1.0) / v;
}

pub inline fn vecScale(v: Vec, s: f32) Vec {
    // shufps, mulps
    return v * vecSplat(s);
}

pub inline fn vec3Less(v0: Vec, v1: Vec) bool {
    if (use_inline_asm) {
        const b = asm (
            \\  cmpltps %[v1], %[v0]
            \\  movmskps %[v0], %%eax
            \\  cmp $7, %%al
            \\  sete %%al
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
        return b;
    } else {
        const mask = v0 < v1;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn vec3Dot(v0: Vec, v1: Vec) Vec {
    var dot = v0 * v1;
    var temp = @shuffle(f32, dot, undefined, [4]i32{ 1, 2, 1, 2 });
    dot = vecSet(dot[0] + temp[0], dot[1], dot[2], dot[2]); // addss
    temp = @shuffle(f32, temp, undefined, [4]i32{ 1, 1, 1, 1 });
    dot = vecSet(dot[0] + temp[0], dot[1], dot[2], dot[2]); // addss
    return vecSplatX(dot);
}

//
// Private types and functions
//
const VecU32 = @Vector(4, u32);

inline fn vecFloatToIntAndBack(v: Vec) Vec {
    // This won't handle nan, inf and numbers greater than 8_388_608.0
    @setRuntimeSafety(false);
    // cvttps2dq
    const vi = [4]i32{
        @floatToInt(i32, v[0]),
        @floatToInt(i32, v[1]),
        @floatToInt(i32, v[2]),
        @floatToInt(i32, v[3]),
    };
    // cvtdq2ps
    return [4]f32{
        @intToFloat(f32, vi[0]),
        @intToFloat(f32, vi[1]),
        @intToFloat(f32, vi[2]),
        @intToFloat(f32, vi[3]),
    };
}

inline fn vec3ApproxEqAbs(v0: Vec, v1: Vec, eps: f32) bool {
    return math.approxEqAbs(f32, v0[0], v1[0], eps) and
        math.approxEqAbs(f32, v0[1], v1[1], eps) and
        math.approxEqAbs(f32, v0[2], v1[2], eps);
}

inline fn vec4ApproxEqAbs(v0: Vec, v1: Vec, eps: f32) bool {
    return math.approxEqAbs(f32, v0[0], v1[0], eps) and
        math.approxEqAbs(f32, v0[1], v1[1], eps) and
        math.approxEqAbs(f32, v0[2], v1[2], eps) and
        math.approxEqAbs(f32, v0[3], v1[3], eps);
}

inline fn vecBoolSet(x: bool, y: bool, z: bool, w: bool) VecBool {
    return [4]bool{ x, y, z, w };
}

inline fn check(result: bool) !void {
    try std.testing.expectEqual(result, true);
}

//
// Tests
//
test "vecZero" {
    const v = vecZero();
    try check(vec4ApproxEqAbs(v, [4]f32{ 0.0, 0.0, 0.0, 0.0 }, 0.0));
}

test "vecSet" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSet(5.0, -6.0, 7.0, 8.0);
    try check(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
    try check(v1[0] == 5.0 and v1[1] == -6.0 and v1[2] == 7.0 and v1[3] == 8.0);
}

test "vecSetInt" {
    const v = vecSetInt(0x3f80_0000, 0x4000_0000, 0x4040_0000, 0x4080_0000);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, 2.0, 3.0, 4.0 }, 0.0));
}

test "vecSplat" {
    const v = vecSplat(123.0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 123.0, 123.0, 123.0, 123.0 }, 0.0));
}

test "vecSplatXYZW" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const vx = vecSplatX(v0);
    const vy = vecSplatY(v0);
    const vz = vecSplatZ(v0);
    const vw = vecSplatW(v0);
    try check(vec4ApproxEqAbs(vx, [4]f32{ 1.0, 1.0, 1.0, 1.0 }, 0.0));
    try check(vec4ApproxEqAbs(vy, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
    try check(vec4ApproxEqAbs(vz, [4]f32{ 3.0, 3.0, 3.0, 3.0 }, 0.0));
    try check(vec4ApproxEqAbs(vw, [4]f32{ 4.0, 4.0, 4.0, 4.0 }, 0.0));
}

test "vecSplatInt" {
    const v = vecSplatInt(0x4000_0000);
    try check(vec4ApproxEqAbs(v, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
}

test "vecMin and vecMax" {
    const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
    const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
    const vmin = vecMin(v0, v1);
    const vmax = vecMax(v0, v1);
    const less = v0 < v1;
    try check(vec4ApproxEqAbs(vmin, [4]f32{ 1.0, 1.0, 2.0, 7.0 }, 0.0));
    try check(vec4ApproxEqAbs(vmax, [4]f32{ 2.0, 3.0, 4.0, math.inf_f32 }, 0.0));
    try check(less[0] == true and less[1] == false and less[2] == true and less[3] == true);

    {
        const v2 = vecSet(2.0, math.nan_f32, 4.0, math.qnan_f32);
        const vmax_nan = vecMax(v2, v0);
        try check(vec4ApproxEqAbs(vmax_nan, [4]f32{ 2.0, 3.0, 4.0, 7.0 }, 0.0));

        const v3 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const vmin_nan = vecMin(v2, v3);
        try check(vmin_nan[0] == 1.0);
        try check(math.isNan(vmin_nan[1]));
        try check(vmin_nan[2] == 4.0);
        try check(math.isNan(vmin_nan[3]));
    }

    {
        const v4 = vecSet(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v5 = vecSet(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const vmin_nan = vecMin(v4, v5);
        try check(vmin_nan[0] == -math.inf_f32);
        try check(vmin_nan[1] == -math.inf_f32);
        try check(vmin_nan[2] == math.inf_f32);
        try check(!math.isNan(vmin_nan[2]));
        try check(math.isNan(vmin_nan[3]));
        try check(!math.isInf(vmin_nan[3]));

        const vmax_nan = vecMax(v4, v5);
        try check(vmax_nan[0] == -math.inf_f32);
        try check(vmax_nan[1] == math.inf_f32);
        try check(vmax_nan[2] == math.inf_f32);
        try check(!math.isNan(vmax_nan[2]));
        try check(math.isNan(vmax_nan[3]));
        try check(!math.isInf(vmax_nan[3]));
    }
}

test "vecIsNan" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsNan(v0);
    try check(vecBoolEqual(b, vecBoolSet(false, true, true, false)));
}

test "vecIsInf" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsInf(v0);
    try check(vecBoolEqual(b, vecBoolSet(true, false, false, false)));
}

test "vecLoadF32x2" {
    const a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    var ptr = &a;
    var i: u32 = 0;
    const v0 = vecLoadF32x2(a[i..]);
    try check(vec4ApproxEqAbs(v0, [4]f32{ 1.0, 2.0, 0.0, 0.0 }, 0.0));
    i += 2;
    const v1 = vecLoadF32x2(a[i .. i + 2]);
    try check(vec4ApproxEqAbs(v1, [4]f32{ 3.0, 4.0, 0.0, 0.0 }, 0.0));
    const v2 = vecLoadF32x2(a[5..7]);
    try check(vec4ApproxEqAbs(v2, [4]f32{ 6.0, 7.0, 0.0, 0.0 }, 0.0));
    const v3 = vecLoadF32x2(ptr[1..]);
    try check(vec4ApproxEqAbs(v3, [4]f32{ 2.0, 3.0, 0.0, 0.0 }, 0.0));
    i += 1;
    const v4 = vecLoadF32x2(ptr[i .. i + 2]);
    try check(vec4ApproxEqAbs(v4, [4]f32{ 4.0, 5.0, 0.0, 0.0 }, 0.0));
}

test "vecStoreF32x3" {
    var a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    const v = vecLoadF32x3(a[1..]);
    vecStoreF32x4(a[2..], v);
    try check(a[0] == 1.0);
    try check(a[1] == 2.0);
    try check(a[2] == 2.0);
    try check(a[3] == 3.0);
    try check(a[4] == 4.0);
    try check(a[5] == 0.0);
}

test "vecNearEqual" {
    const v0 = vecSet(1.0, 2.0, -3.0, 4.001);
    const v1 = vecSet(1.0, 2.1, 3.0, 4.0);
    const b = vecNearEqual(v0, v1, vecSplat(0.01));
    try check(b[0] == true);
    try check(b[1] == false);
    try check(b[2] == false);
    try check(b[3] == true);
    try check(@reduce(.And, b == vecBoolSet(true, false, false, true)));
    try check(vecBoolAllTrue(b == vecBoolSet(true, false, false, true)));
}

test "vecInBounds" {
    const v0 = vecSet(0.5, -2.0, -1.0, 1.9);
    const v1 = vecSet(-1.6, -2.001, -1.0, 1.9);
    const bounds = vecSet(1.0, 2.0, 1.0, 2.0);
    const b0 = vecInBounds(v0, bounds);
    const b1 = vecInBounds(v1, bounds);
    try check(vecBoolEqual(b0, vecBoolSet(true, true, true, true)));
    try check(vecBoolEqual(b1, vecBoolSet(false, false, true, true)));
}

test "vecBoolAnd" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolAnd(b0, b1);
    try check(b[0] == true and b[1] == false and b[2] == false and b[3] == false);
}

test "vecBoolOr" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolOr(b0, b1);
    try check(b[0] == true and b[1] == true and b[2] == true and b[3] == false);
}

test "vec @sin" {
    const v0 = vecSet(math.inf_f32, -math.inf_f32, math.inf_f32, 9_388_608.0);
    const v = @sin(v0);
    try check(math.isNan(v[0]));
    try check(math.isNan(v[1]));
    try check(math.isNan(v[2]));
    try check(math.approxEqAbs(f32, v[3], 0.7205, 0.001));
}

test "vecAnd" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v1 = vecSet(1.0, 2.0, 3.0, math.inf_f32);
    const v = vecAnd(v0, v1);
    try check(v[3] == math.inf_f32);
    try check(vec3ApproxEqAbs(v, [4]f32{ 0.0, 2.0, 0.0, math.inf_f32 }, 0.0));
}

test "vecAndNot" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v1 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v = vecAndNot(v0, v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, 0.0, 3.0, 0.0 }, 0.0));
}

test "vecOr" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, 0);
    const v1 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v = vecOr(v0, v1);
    try check(v[0] == 1.0);
    try check(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try check(v[2] == 3.0);
    try check(v[3] == 4.0);
}

test "vecXor" {
    const v0 = vecSetInt(@bitCast(u32, @as(f32, 1.0)), ~@as(u32, 0), 0, 0);
    const v1 = vecSet(1.0, 0, 0, 0);
    const v = vecXor(v0, v1);
    try check(v[0] == 0.0);
    try check(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try check(v[2] == 0.0);
    try check(v[3] == 0.0);
}

test "vecFloatToIntAndBack" {
    const v0 = vecSet(1.1, 2.9, 3.0, -4.5);
    var v = vecFloatToIntAndBack(v0);
    try check(v[0] == 1.0);
    try check(v[1] == 2.0);
    try check(v[2] == 3.0);
    try check(v[3] == -4.0);

    const v1 = vecSet(math.inf_f32, 2.9, math.nan_f32, math.qnan_f32);
    v = vecFloatToIntAndBack(v1);
    try check(v[1] == 2.0);
}

test "vecRound" {
    const v0 = vecSet(1.1, -1.1, -1.5, 1.5);
    var v = vecRound(v0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, -1.0, -2.0, 2.0 }, 0.0));

    const v1 = vecSet(-10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32);
    v = vecRound(v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ -10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = vecSet(-math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32);
    v = vecRound(v2);
    try check(math.isNan(v2[0]));
    try check(math.isNan(v2[1]));
    try check(math.isNan(v2[2]));
    try check(v2[3] == -math.inf_f32);

    const v3 = vecSet(1001.5, -201.499, -10000.99, -101.5);
    v = vecRound(v3);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1002.0, -201.0, -10001.0, -102.0 }, 0.0));

    const v4 = vecSet(-1_388_609.9, 1_388_609.5, 1_388_109.01, 2_388_609.5);
    v = vecRound(v4);
    try check(vec4ApproxEqAbs(v, [4]f32{ -1_388_610.0, 1_388_610.0, 1_388_109.0, 2_388_610.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = vecRound(vecSplat(f));
        const fr = @round(f);
        try check(vr[0] == fr);
        try check(vr[1] == fr);
        try check(vr[2] == fr);
        try check(vr[3] == fr);
        f += 0.12345 * @intToFloat(f32, i);
    }
}

test "vecTrunc" {
    const v0 = vecSet(1.1, -1.1, -1.5, 1.5);
    var v = vecTrunc(v0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, -1.0, -1.0, 1.0 }, 0.0));

    const v1 = vecSet(-10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32);
    v = vecTrunc(v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = vecSet(-math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32);
    v = vecTrunc(v2);
    try check(math.isNan(v2[0]));
    try check(math.isNan(v2[1]));
    try check(math.isNan(v2[2]));
    try check(v2[3] == -math.inf_f32);

    const v3 = vecSet(1000.5001, -201.499, -10000.99, 100.750001);
    v = vecTrunc(v3);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1000.0, -201.0, -10000.0, 100.0 }, 0.0));

    const v4 = vecSet(-7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5);
    v = vecTrunc(v4);
    try check(vec4ApproxEqAbs(v, [4]f32{ -7_388_609.0, 7_388_609.0, 8_388_109.0, -8_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = vecTrunc(vecSplat(f));
        const fr = @trunc(f);
        try check(vr[0] == fr);
        try check(vr[1] == fr);
        try check(vr[2] == fr);
        try check(vr[3] == fr);
        f += 0.12345 * @intToFloat(f32, i);
    }
}

test "vecFloor" {
    const v0 = vecSet(1.5, -1.5, -1.7, -2.1);
    var v = vecFloor(v0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, -2.0, -2.0, -3.0 }, 0.0));

    const v1 = vecSet(-10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32);
    v = vecFloor(v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = vecSet(-math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32);
    v = vecFloor(v2);
    try check(math.isNan(v2[0]));
    try check(math.isNan(v2[1]));
    try check(math.isNan(v2[2]));
    try check(v2[3] == -math.inf_f32);

    const v3 = vecSet(1000.5001, -201.499, -10000.99, 100.75001);
    v = vecFloor(v3);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1000.0, -202.0, -10001.0, 100.0 }, 0.0));

    const v4 = vecSet(-7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5);
    v = vecFloor(v4);
    try check(vec4ApproxEqAbs(v, [4]f32{ -7_388_610.0, 7_388_609.0, 8_388_109.0, -8_388_510.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = vecFloor(vecSplat(f));
        const fr = @floor(f);
        try check(vr[0] == fr);
        try check(vr[1] == fr);
        try check(vr[2] == fr);
        try check(vr[3] == fr);
        f += 0.12345 * @intToFloat(f32, i);
    }
}

test "vecCeil" {
    const v0 = vecSet(1.5, -1.5, -1.7, -2.1);
    var v = vecCeil(v0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 2.0, -1.0, -1.0, -2.0 }, 0.0));

    const v1 = vecSet(-10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32);
    v = vecCeil(v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = vecSet(-math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32);
    v = vecCeil(v2);
    try check(math.isNan(v2[0]));
    try check(math.isNan(v2[1]));
    try check(math.isNan(v2[2]));
    try check(v2[3] == -math.inf_f32);

    const v3 = vecSet(1000.5001, -201.499, -10000.99, 100.75001);
    v = vecCeil(v3);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1001.0, -201.0, -10000.0, 101.0 }, 0.0));

    const v4 = vecSet(-1_388_609.5, 1_388_609.1, 1_388_109.9, -1_388_509.9);
    v = vecCeil(v4);
    try check(vec4ApproxEqAbs(v, [4]f32{ -1_388_609.0, 1_388_610.0, 1_388_110.0, -1_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = vecCeil(vecSplat(f));
        const fr = @ceil(f);
        try check(vr[0] == fr);
        try check(vr[1] == fr);
        try check(vr[2] == fr);
        try check(vr[3] == fr);
        f += 0.12345 * @intToFloat(f32, i);
    }
}

test "vec3Dot" {
    const v0 = vecSet(-1.0, 2.0, 3.0, 1.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 1.0);
    var v = vec3Dot(v0, v1);
    try check(vec4ApproxEqAbs(v, vecSplat(24.0), 0.0001));
}

test "vec3Less" {
    const v0 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 1.0);
    try check(vec3Less(v0, v1) == true);

    const v2 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v3 = vecSet(4.0, -5.0, 6.0, 1.0);
    try check(vec3Less(v2, v3) == false);

    const v4 = vecSet(100.0, 200.0, 300.0, 50000.0);
    const v5 = vecSet(400.0, 500.0, 600.0, 1.0);
    try check(vec3Less(v4, v5) == true);

    const v6 = vecSet(100.0, -math.inf_f32, -math.inf_f32, 50000.0);
    const v7 = vecSet(400.0, math.inf_f32, 600.0, 1.0);
    try check(vec3Less(v6, v7) == true);
}

test "vecSaturate" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecSaturate(v0);
        const vf = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
        try check(vec4ApproxEqAbs(vf, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecSaturate(v0);
        const vf = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
        try check(vec4ApproxEqAbs(vf, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecSaturate(v0);
        const vf = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
        try check(vec4ApproxEqAbs(vf, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
    }
}

test "vecClamp" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecClamp(v0, vecSplat(-0.5), vecSplat(0.5));
        const vf = vecClampFast(v0, vecSplat(-0.5), vecSplat(0.5));
        try check(vec4ApproxEqAbs(v, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
        try check(vec4ApproxEqAbs(vf, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecClamp(v0, vecSet(-100.0, 0.0, -100.0, 0.0), vecSet(0.0, 100.0, 0.0, 100.0));
        try check(vec4ApproxEqAbs(v, vecSet(-100.0, 100.0, -100.0, 0.0), 0.0001));
    }

    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecClamp(v0, vecSplat(-1.0), vecSplat(1.0));
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, -1.0, -1.0), 0.0001));
    }
}
