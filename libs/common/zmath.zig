const builtin = @import("builtin");
const std = @import("std");
const math = std.math;

const cpu_arch = builtin.cpu.arch;
const has_avx = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .avx) else false;

pub const Vec = @Vector(4, f32);
pub const VecBool = @Vector(4, bool);
const VecU32 = @Vector(4, u32);

//
// General Vec functions (always work on all vector components)
//
// vecZero() Vec
// vecSet(x: f32, y: f32, z: f32, w: f32) Vec
// vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec
// vecSplat(value: f32) Vec
// vecSplatX(v: Vec) Vec
// vecSplatY(v: Vec) Vec
// vecSplatZ(v: Vec) Vec
// vecSplatW(v: Vec) Vec
// vecSplatInt(value: u32) Vec
// vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) VecBool
// vecEqualInt(v0: Vec, v1: Vec) VecBool
// vecNotEqualInt(v0: Vec, v1: Vec) VecBool
// vecAndInt(v0: Vec, v1: Vec) Vec
// vecAndCInt(v0: Vec, v1: Vec) Vec
// vecOrInt(v0: Vec, v1: Vec) Vec
// vecNorInt(v0: Vec, v1: Vec) Vec
// vecXorInt(v0: Vec, v1: Vec) Vec
// vecIsNan(v: Vec) VecBool
// vecIsInf(v: Vec) VecBool
// vecMinFast(v0: Vec, v1: Vec) Vec
// vecMaxFast(v0: Vec, v1: Vec) Vec
// vecMin(v0: Vec, v1: Vec) Vec
// vecMax(v0: Vec, v1: Vec) Vec
// vecSelect(b: VecBool, v0: Vec, v1: Vec) Vec
// vecInBounds(v: Vec, bounds: Vec) VecBool
// vecRound(v: Vec) Vec
// vecTrunc(v: Vec) Vec
// vecFloor(v: Vec) Vec
// vecCeil(v: Vec) Vec
// vecClamp(v: Vec, min: Vec, max: Vec) Vec
// vecClampFast(v: Vec, min: Vec, max: Vec) Vec
// vecSaturate(v: Vec) Vec
// vecSaturateFast(v: Vec) Vec
// vecAbs(v: Vec) Vec
// vecSqrt(v: Vec) Vec
// vecRcpSqrt(v: Vec) Vec
// vecRcpSqrtFast(v: Vec) Vec
// vecRcp(v: Vec) Vec
// vecRcpFast(v: Vec) Vec
// vecScale(v: Vec, s: f32) Vec
// vecLerp(v0: Vec, v1: Vec, t: f32) Vec
// vecLerpV(v0: Vec, v1: Vec, t: Vec) Vec

pub inline fn vecZero() Vec {
    return @splat(4, @as(f32, 0));
}
test "zmath.vecZero" {
    const v = vecZero();
    try check(vec4ApproxEqAbs(v, [4]f32{ 0.0, 0.0, 0.0, 0.0 }, 0.0));
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}
test "zmath.vecSet" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSet(5.0, -6.0, 7.0, 8.0);
    try check(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
    try check(v1[0] == 5.0 and v1[1] == -6.0 and v1[2] == 7.0 and v1[3] == 8.0);
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    return @bitCast(Vec, [4]u32{ x, y, z, w });
}
test "zmath.vecSetInt" {
    const v = vecSetInt(0x3f80_0000, 0x4000_0000, 0x4040_0000, 0x4080_0000);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, 2.0, 3.0, 4.0 }, 0.0));
}

pub inline fn vecSplat(value: f32) Vec {
    return @splat(4, value);
}
test "zmath.vecSplat" {
    const v = vecSplat(123.0);
    try check(vec4ApproxEqAbs(v, [4]f32{ 123.0, 123.0, 123.0, 123.0 }, 0.0));
}

pub inline fn vecSplatX(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 0, 0, 0, 0 });
}
test "zmath.vecSplatX" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const vx = vecSplatX(v0);
    try check(vec4ApproxEqAbs(vx, [4]f32{ 1.0, 1.0, 1.0, 1.0 }, 0.0));
}

pub inline fn vecSplatY(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 1, 1, 1, 1 });
}
test "zmath.vecSplatY" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const vy = vecSplatY(v0);
    try check(vec4ApproxEqAbs(vy, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
}

pub inline fn vecSplatZ(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 2, 2, 2, 2 });
}
test "zmath.vecSplatZ" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const vz = vecSplatZ(v0);
    try check(vec4ApproxEqAbs(vz, [4]f32{ 3.0, 3.0, 3.0, 3.0 }, 0.0));
}

pub inline fn vecSplatW(v: Vec) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ 3, 3, 3, 3 });
}
test "zmath.vecSplatW" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const vw = vecSplatW(v0);
    try check(vec4ApproxEqAbs(vw, [4]f32{ 4.0, 4.0, 4.0, 4.0 }, 0.0));
}

pub inline fn vecSplatInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}
test "zmath.vecSplatInt" {
    const v = vecSplatInt(0x4000_0000);
    try check(vec4ApproxEqAbs(v, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
}

pub inline fn vecNearEqual(v0: Vec, v1: Vec, epsilon: Vec) VecBool {
    // Won't handle inf & nan
    const delta = v0 - v1;
    const temp = vecMaxFast(delta, vecZero() - delta);
    return temp <= epsilon;
}
test "zmath.vecNearEqual" {
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

pub inline fn vecEqualInt(v0: Vec, v1: Vec) VecBool {
    // pcmpeqd
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return v0u == v1u;
}

pub inline fn vecNotEqualInt(v0: Vec, v1: Vec) VecBool {
    // 2 x pcmpeqd, pxor
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return v0u != v1u;
}

pub inline fn vecAndInt(v0: Vec, v1: Vec) Vec {
    // andps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u & v1u);
}
test "zmath.vecAndInt" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v1 = vecSet(1.0, 2.0, 3.0, math.inf_f32);
    const v = vecAndInt(v0, v1);
    try check(v[3] == math.inf_f32);
    try check(vec3ApproxEqAbs(v, [4]f32{ 0.0, 2.0, 0.0, math.inf_f32 }, 0.0));
}

pub inline fn vecAndCInt(v0: Vec, v1: Vec) Vec {
    // andnps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u & ~v1u);
}
test "zmath.vecAndCInt" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v = vecAndCInt(v0, v1);
    try check(vec4ApproxEqAbs(v, [4]f32{ 1.0, 0.0, 3.0, 0.0 }, 0.0));
}

pub inline fn vecOrInt(v0: Vec, v1: Vec) Vec {
    // orps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u | v1u);
}
test "zmath.vecOrInt" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, 0);
    const v1 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v = vecOrInt(v0, v1);
    try check(v[0] == 1.0);
    try check(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try check(v[2] == 3.0);
    try check(v[3] == 4.0);
}

pub inline fn vecNorInt(v0: Vec, v1: Vec) Vec {
    // por, pcmpeqd, pxor
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, ~(v0u | v1u));
}

pub inline fn vecXorInt(v0: Vec, v1: Vec) Vec {
    // xorps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u ^ v1u);
}
test "zmath.vecXorInt" {
    const v0 = vecSetInt(@bitCast(u32, @as(f32, 1.0)), ~@as(u32, 0), 0, 0);
    const v1 = vecSet(1.0, 0, 0, 0);
    const v = vecXorInt(v0, v1);
    try check(v[0] == 0.0);
    try check(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try check(v[2] == 0.0);
    try check(v[3] == 0.0);
}

pub inline fn vecIsNan(v: Vec) VecBool {
    return v != v;
}
test "zmath.vecIsNan" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsNan(v0);
    try check(vecBoolEqual(b, vecBoolSet(false, true, true, false)));
}

pub inline fn vecIsInf(v: Vec) VecBool {
    return vecAndInt(v, f32x4_0x7fff_ffff) == f32x4_inf;
}
test "zmath.vecIsInf" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsInf(v0);
    try check(vecBoolEqual(b, vecBoolSet(true, false, false, false)));
}

pub inline fn vecMinFast(v0: Vec, v1: Vec) Vec {
    // minps
    return @select(f32, v0 < v1, v0, v1);
}
test "zmath.vecMinFast" {
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMinFast(v0, v1);
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMinFast(v0, v1);
        try check(v[0] == 1.0);
        try check(v[1] == 1.0);
        try check(!math.isNan(v[1]));
        try check(v[2] == 4.0);
        try check(v[3] == math.inf_f32);
        try check(!math.isNan(v[3]));
    }
}

pub inline fn vecMaxFast(v0: Vec, v1: Vec) Vec {
    // maxps
    return @select(f32, v0 > v1, v0, v1);
}
test "zmath.vecMaxFast" {
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMaxFast(v0, v1);
        try check(vec4ApproxEqAbs(v, vecSet(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMaxFast(v0, v1);
        try check(v[0] == 2.0);
        try check(v[1] == 1.0);
        try check(v[2] == 5.0);
        try check(v[3] == math.inf_f32);
        try check(!math.isNan(v[3]));
    }
}

pub inline fn vecMin(v0: Vec, v1: Vec) Vec {
    // This will handle inf & nan
    // minps, cmpunordps, andps, andnps, orps
    return @minimum(v0, v1);
}
test "zmath.vecMin" {
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMin(v0, v1);
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMin(v0, v1);
        try check(v[0] == 1.0);
        try check(v[1] == 1.0);
        try check(!math.isNan(v[1]));
        try check(v[2] == 4.0);
        try check(v[3] == math.inf_f32);
        try check(!math.isNan(v[3]));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = vecSet(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecMin(v0, v1);
        try check(v[0] == -math.inf_f32);
        try check(v[1] == -math.inf_f32);
        try check(v[2] == math.inf_f32);
        try check(!math.isNan(v[2]));
        try check(math.isNan(v[3]));
        try check(!math.isInf(v[3]));
    }
}

pub inline fn vecMax(v0: Vec, v1: Vec) Vec {
    // This will handle inf & nan
    // maxps, cmpunordps, andps, andnps, orps
    return @maximum(v0, v1);
}
test "zmath.vecMax" {
    {
        const v0 = vecSet(1.0, 3.0, 2.0, 7.0);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMax(v0, v1);
        try check(vec4ApproxEqAbs(v, vecSet(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMax(v0, v1);
        try check(v[0] == 2.0);
        try check(v[1] == 1.0);
        try check(v[2] == 5.0);
        try check(v[3] == math.inf_f32);
        try check(!math.isNan(v[3]));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = vecSet(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecMax(v0, v1);
        try check(v[0] == -math.inf_f32);
        try check(v[1] == math.inf_f32);
        try check(v[2] == math.inf_f32);
        try check(!math.isNan(v[2]));
        try check(math.isNan(v[3]));
        try check(!math.isInf(v[3]));
    }
}

pub inline fn vecSelect(b: VecBool, v0: Vec, v1: Vec) Vec {
    return @select(f32, b, v0, v1);
}

pub inline fn vecInBounds(v: Vec, bounds: Vec) VecBool {
    // 2 x cmpleps, xorps, load, andps
    const b0 = v <= bounds;
    const b1 = (bounds * vecSplat(-1.0)) <= v;
    return vecBoolAnd(b0, b1);
}
test "zmath.vecInBounds" {
    const v0 = vecSet(0.5, -2.0, -1.0, 1.9);
    const v1 = vecSet(-1.6, -2.001, -1.0, 1.9);
    const bounds = vecSet(1.0, 2.0, 1.0, 2.0);
    const b0 = vecInBounds(v0, bounds);
    const b1 = vecInBounds(v1, bounds);
    try check(vecBoolEqual(b0, vecBoolSet(true, true, true, true)));
    try check(vecBoolEqual(b1, vecBoolSet(false, false, true, true)));
}

pub inline fn vecRound(v: Vec) Vec {
    const sign = vecAndInt(v, f32x4_0x8000_0000);
    const magic = vecOrInt(f32x4_8_388_608, sign);
    var r1 = v + magic;
    r1 = r1 - magic;
    const r2 = vecAbs(v);
    const mask = r2 <= f32x4_8_388_608;
    return vecSelect(mask, r1, v);
}
test "zmath.vecRound" {
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

pub inline fn vecTrunc(v: Vec) Vec {
    const mask = vecAbs(v) < f32x4_8_388_608;
    const result = vecFloatToIntAndBack(v);
    return vecSelect(mask, result, v);
}
test "zmath.vecTrunc" {
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

pub inline fn vecFloor(v: Vec) Vec {
    const mask = vecAbs(v) < f32x4_8_388_608;
    var result = vecFloatToIntAndBack(v);
    const larger_mask = result > v;
    const larger = vecSelect(larger_mask, vecSplat(-1.0), vecZero());
    result = result + larger;
    return vecSelect(mask, result, v);
}
test "zmath.vecFloor" {
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

pub inline fn vecCeil(v: Vec) Vec {
    const mask = vecAbs(v) < f32x4_8_388_608;
    var result = vecFloatToIntAndBack(v);
    const smaller_mask = result < v;
    const smaller = vecSelect(smaller_mask, vecSplat(-1.0), vecZero());
    result = result - smaller;
    return vecSelect(mask, result, v);
}
test "zmath.vecCeil" {
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

pub inline fn vecClamp(v: Vec, min: Vec, max: Vec) Vec {
    var result = vecMax(min, v);
    result = vecMin(max, result);
    return result;
}
test "zmath.vecClamp" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecClamp(v0, vecSplat(-0.5), vecSplat(0.5));
        try check(vec4ApproxEqAbs(v, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
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

pub inline fn vecClampFast(v: Vec, min: Vec, max: Vec) Vec {
    var result = vecMaxFast(min, v);
    result = vecMinFast(max, result);
    return result;
}
test "zmath.vecClampFast" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecClampFast(v0, vecSplat(-0.5), vecSplat(0.5));
        try check(vec4ApproxEqAbs(v, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
    }
}

pub inline fn vecSaturate(v: Vec) Vec {
    var result = vecMax(v, vecZero());
    result = vecMin(result, vecSplat(1.0));
    return result;
}
test "zmath.vecSaturate" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecSaturate(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecSaturate(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecSaturate(v0);
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
    }
}

pub inline fn vecSaturateFast(v: Vec) Vec {
    var result = vecMaxFast(v, vecZero());
    result = vecMinFast(result, vecSplat(1.0));
    return result;
}
test "zmath.vecSaturateFast" {
    {
        const v0 = vecSet(-1.0, 0.2, 1.1, -0.3);
        const v = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecSaturateFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
    }
}

pub inline fn vecAbs(v: Vec) Vec {
    // load, andps
    return @fabs(v);
}

pub inline fn vecSqrt(v: Vec) Vec {
    // sqrtps
    return @sqrt(v);
}

pub inline fn vecRcpSqrt(v: Vec) Vec {
    // load, divps, sqrtps
    return vecSplat(1.0) / vecSqrt(v);
}
test "zmath.vecRcpSqrt" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcpSqrt(v0);
        try check(vec4ApproxEqAbs(
            v,
            vecSet(
                1.0 / math.sqrt(v0[0]),
                1.0 / math.sqrt(v0[1]),
                1.0 / math.sqrt(v0[2]),
                1.0 / math.sqrt(v0[3]),
            ),
            0.0005,
        ));
    }
    {
        const v0 = vecSet(math.inf_f32, 0.2, 123.1, math.nan_f32);
        const v = vecRcpSqrt(v0);
        try check(vec3ApproxEqAbs(v, vecSet(0.0, 1.0 / math.sqrt(v0[1]), 1.0 / math.sqrt(v0[2]), 0.0), 0.0005));
        try check(math.isNan(v[3]));
    }
}

pub inline fn vecRcpSqrtFast(v: Vec) Vec {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vrsqrtps    %%xmm0, %%xmm0
        else
            \\  rsqrtps     %%xmm0, %%xmm0
            ;
        return asm (code
            : [ret] "={xmm0}" (-> Vec),
            : [v] "{xmm0}" (v),
        );
    } else {
        return vecSplat(1.0) / vecSqrt(v);
    }
}
test "zmath.vecRcpSqrtFast" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcpSqrtFast(v0);
        try check(vec4ApproxEqAbs(
            v,
            vecSet(
                1.0 / math.sqrt(v0[0]),
                1.0 / math.sqrt(v0[1]),
                1.0 / math.sqrt(v0[2]),
                1.0 / math.sqrt(v0[3]),
            ),
            0.0005,
        ));
    }
    {
        const v0 = vecSet(math.inf_f32, 0.2, 123.1, math.nan_f32);
        const v = vecRcpSqrtFast(v0);
        try check(vec3ApproxEqAbs(v, vecSet(0.0, 1.0 / math.sqrt(v0[1]), 1.0 / math.sqrt(v0[2]), 0.0), 0.0005));
        try check(math.isNan(v[3]));
    }
}

pub inline fn vecRcp(v: Vec) Vec {
    // Will handle inf & nan
    // load, divps
    return vecSplat(1.0) / v;
}
test "zmath.vecRcp" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcp(v0);
        try check(vec4ApproxEqAbs(v, vecSet(1.0 / v0[0], 1.0 / v0[1], 1.0 / v0[2], 1.0 / v0[3]), 0.0005));
    }
    {
        const v0 = vecSet(math.inf_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecRcp(v0);
        try check(vec2ApproxEqAbs(v, vecSet(0.0, 0.0, 0.0, 0.0), 0.0005));
        try check(math.isNan(v[2]));
        try check(math.isNan(v[3]));
    }
}

pub inline fn vecRcpFast(v: Vec) Vec {
    // Will not handle inf & nan
    // load, rcpps, 2 x mulps, addps, subps
    @setFloatMode(.Optimized);
    return vecSplat(1.0) / v;
}
test "zmath.vecRcpFast" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcpFast(v0);
        try check(vec4ApproxEqAbs(v, vecSet(1.0 / v0[0], 1.0 / v0[1], 1.0 / v0[2], 1.0 / v0[3]), 0.0005));
    }
}

pub inline fn vecScale(v: Vec, s: f32) Vec {
    // shufps, mulps
    return v * vecSplat(s);
}

pub inline fn vecLerp(v0: Vec, v1: Vec, t: f32) Vec {
    // subps, shufps, addps, mulps
    return v0 + (v1 - v0) * vecSplat(t);
}

pub inline fn vecLerpV(v0: Vec, v1: Vec, t: Vec) Vec {
    // subps, addps, mulps
    return v0 + (v1 - v0) * t;
}

//
// VecBool functions
//
// vecBoolAnd(b0: VecBool, b1: VecBool) VecBool
// vecBoolOr(b0: VecBool, b1: VecBool) VecBool
// vecBoolAllTrue(b: VecBool) bool
// vecBoolEqual(b: VecBool) bool

pub inline fn vecBoolAnd(b0: VecBool, b1: VecBool) VecBool {
    // andps
    return [4]bool{ b0[0] and b1[0], b0[1] and b1[1], b0[2] and b1[2], b0[3] and b1[3] };
}
test "zmath.vecBoolAnd" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolAnd(b0, b1);
    try check(b[0] == true and b[1] == false and b[2] == false and b[3] == false);
}

pub inline fn vecBoolOr(b0: VecBool, b1: VecBool) VecBool {
    // orps
    return [4]bool{ b0[0] or b1[0], b0[1] or b1[1], b0[2] or b1[2], b0[3] or b1[3] };
}
test "zmath.vecBoolOr" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolOr(b0, b1);
    try check(b[0] == true and b[1] == true and b[2] == true and b[3] == false);
}

pub inline fn vecBoolAllTrue(b: VecBool) bool {
    return @reduce(.And, b);
}

pub inline fn vecBoolEqual(b0: VecBool, b1: VecBool) bool {
    return vecBoolAllTrue(b0 == b1);
}

//
// Load/store functions
//
// vecLoadF32(mem: []const f32) Vec
// vecLoadF32x2(mem: []const f32) Vec
// vecLoadF32x3(mem: []const f32) Vec
// vecLoadF32x4(mem: []const f32) Vec
// vecStoreF32(mem: []f32, v: Vec) void
// vecStoreF32x2(mem: []f32, v: Vec) void
// vecStoreF32x3(mem: []f32, v: Vec) void
// vecStoreF32x4(mem: []f32, v: Vec) void

pub inline fn vecLoadF32(mem: []const f32) Vec {
    return [4]f32{ mem[0], 0, 0, 0 };
}

pub inline fn vecLoadF32x2(mem: []const f32) Vec {
    return [4]f32{ mem[0], mem[1], 0, 0 };
}
test "zmath.vecLoadF32x2" {
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
test "zmath.vecStoreF32x3" {
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

pub inline fn vecStoreF32x4(mem: []f32, v: Vec) void {
    mem[0] = v[0];
    mem[1] = v[1];
    mem[2] = v[2];
    mem[3] = v[3];
}

//
// Vec2 functions
//
// vec2Equal(v0: Vec, v1: Vec) bool
// vec2EqualInt(v0: Vec, v1: Vec) bool
// vec2NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool
// vec2Less(v0: Vec, v1: Vec) bool
// vec2LessOrEqual(v0: Vec, v1: Vec) bool
// vec2Greater(v0: Vec, v1: Vec) bool
// vec2GreaterOrEqual(v0: Vec, v1: Vec) bool
// vec2InBounds(v: Vec, bounds: Vec) bool
// vec2Dot(v0: Vec, v1: Vec) Vec

pub inline fn vec2Equal(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpeqps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  cmpeqps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 == v1;
        return mask[0] and mask[1];
    }
}
test "zmath.vec2Equal" {
    {
        const v0 = vecSet(1.0, math.inf_f32, -3.0, 1000.001);
        const v1 = vecSet(1.0, math.inf_f32, -6.0, 4.0);
        try check(vec2Equal(v0, v1));
    }
}

pub inline fn vec2EqualInt(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vpcmpeqd    %%xmm1, %%xmm0, %xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  pcmpeqd     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const v0u = @bitCast(VecU32, v0);
        const v1u = @bitCast(VecU32, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1];
    }
}

pub inline fn vec2NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool {
    // Won't handle inf & nan
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vsubps      %%xmm1, %%xmm0, %%xmm0  # xmm0 = delta
            \\  vxorps      %%xmm1, %%xmm1, %%xmm1  # xmm1 = 0
            \\  vsubps      %%xmm0, %%xmm1, %%xmm1  # xmm1 = 0 - delta
            \\  vmaxps      %%xmm1, %%xmm0, %%xmm0  # xmm0 = abs(delta)
            \\  vcmpleps    %%xmm2, %%xmm0, %%xmm0  # xmm0 = abs(delta) <= epsilon
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  subps       %%xmm1, %%xmm0          # xmm0 = delta
            \\  xorps       %%xmm1, %%xmm1          # xmm1 = 0
            \\  subps       %%xmm0, %%xmm1          # xmm1 = 0 - delta
            \\  maxps       %%xmm1, %%xmm0          # xmm0 = abs(delta)
            \\  cmpleps     %%xmm2, %%xmm0          # xmm0 = abs(delta) <= epsilon
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
              [epsilon] "{xmm2}" (epsilon),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = vecNearEqual(v0, v1, epsilon);
        return mask[0] and mask[1];
    }
}
test "zmath.vec2NearEqual" {
    {
        const v0 = vecSet(1.0, 2.0, -6.0001, 1000.001);
        const v1 = vecSet(1.0, 2.001, -3.0, 4.0);
        const v2 = vecSet(1.001, 2.001, -3.001, 4.001);
        try check(vec2NearEqual(v0, v1, vecSplat(0.01)));
        try check(vec2NearEqual(v2, v1, vecSplat(0.01)));
    }
}

pub inline fn vec2Less(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpltps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  cmpltps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 < v1;
        return mask[0] and mask[1];
    }
}
test "zmath.vec2Less" {
    const v0 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 1.0);
    try check(vec2Less(v0, v1) == true);

    const v2 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v3 = vecSet(4.0, -5.0, 6.0, 1.0);
    try check(vec2Less(v2, v3) == false);

    const v4 = vecSet(100.0, 200.0, 300.0, 50000.0);
    const v5 = vecSet(400.0, 500.0, 600.0, 1.0);
    try check(vec2Less(v4, v5) == true);

    const v6 = vecSet(100.0, -math.inf_f32, -math.inf_f32, 50000.0);
    const v7 = vecSet(400.0, math.inf_f32, 600.0, 1.0);
    try check(vec2Less(v6, v7) == true);
}

pub inline fn vec2LessOrEqual(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpleps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  cmpleps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 <= v1;
        return mask[0] and mask[1];
    }
}

pub inline fn vec2Greater(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpgtps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  cmpgtps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 > v1;
        return mask[0] and mask[1];
    }
}

pub inline fn vec2GreaterOrEqual(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpgeps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  cmpgeps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 >= v1;
        return mask[0] and mask[1];
    }
}

pub inline fn vec2InBounds(v: Vec, bounds: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vmovaps     %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  vxorps      %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\  vcmpleps    %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\  vcmpleps    %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\  vandps      %%xmm2, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
        else
            \\  movaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  xorps       %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\  cmpleps     %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\  cmpleps     %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\  andps       %%xmm2, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $3, %%al
            \\  cmp         $3, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
              [bounds] "{xmm1}" (bounds),
              [x8000_0000] "{memory}" (f32x4_0x8000_0000),
            : "xmm2"
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b0 = @select(u32, v <= bounds, u32x4_0xffff_ffff, vecU32Zero());
        const b1 = @select(u32, bounds * vecSplat(-1.0) <= v, u32x4_0xffff_ffff, vecU32Zero());
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0;

        // I've also tried this (generated asm is different but still not great):
        // const b0 = v <= bounds;
        // const b1 = bounds * vecSplat(-1.0) <= v;
        // const b = vecBoolAnd(b0, b1);
        // return b[0] and b[1];
    }
}
test "zmath.vec2InBounds" {
    {
        const v0 = vecSet(0.5, -2.0, -100.0, 1000.0);
        const v1 = vecSet(-1.6, -2.001, 1.0, 1.9);
        const bounds = vecSet(1.0, 2.0, 1.0, 2.0);
        try check(vec2InBounds(v0, bounds) == true);
        try check(vec2InBounds(v1, bounds) == false);
    }
    {
        const v0 = vecSet(10000.0, -1000.0, -10.0, 1000.0);
        const bounds = vecSet(math.inf_f32, math.inf_f32, 1.0, 2.0);
        try check(vec2InBounds(v0, bounds) == true);
    }
}

pub inline fn vec2Dot(v0: Vec, v1: Vec) Vec {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | -- | -- |
    var xmm1 = @shuffle(f32, xmm0, undefined, [4]i32{ 1, 0, 0, 0 }); // | y0*y1 | -- | -- | -- |
    xmm0 = vecSet(xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[3]); // | x0*x1 + y0*y1 | -- | -- | -- |
    return vecSplatX(xmm0);
}
test "zmath.vec2Dot" {
    const v0 = vecSet(-1.0, 2.0, 300.0, -2.0);
    const v1 = vecSet(4.0, 5.0, 600.0, 2.0);
    var v = vec2Dot(v0, v1);
    try check(vec4ApproxEqAbs(v, vecSplat(6.0), 0.0001));
}

//
// Vec3 functions
//
// vec3Equal(v0: Vec, v1: Vec) bool
// vec3EqualInt(v0: Vec, v1: Vec) bool
// vec3NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool
// vec3Less(v0: Vec, v1: Vec) bool
// vec3LessOrEqual(v0: Vec, v1: Vec) bool
// vec3Greater(v0: Vec, v1: Vec) bool
// vec3GreaterOrEqual(v0: Vec, v1: Vec) bool
// vec3InBounds(v: Vec, bounds: Vec) bool
// vec3Dot(v0: Vec, v1: Vec) Vec

pub inline fn vec3Equal(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpeqps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  cmpeqps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 == v1;
        return mask[0] and mask[1] and mask[2];
    }
}
test "zmath.vec3Equal" {
    {
        const v0 = vecSet(1.0, math.inf_f32, -3.0, 1000.001);
        const v1 = vecSet(1.0, math.inf_f32, -3.0, 4.0);
        try check(vec3Equal(v0, v1));
    }
}

pub inline fn vec3EqualInt(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vpcmpeqd    %%xmm1, %%xmm0, %xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  pcmpeqd     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const v0u = @bitCast(VecU32, v0);
        const v1u = @bitCast(VecU32, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn vec3NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool {
    // Won't handle inf & nan
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vsubps      %%xmm1, %%xmm0, %%xmm0  # xmm0 = delta
            \\  vxorps      %%xmm1, %%xmm1, %%xmm1  # xmm1 = 0
            \\  vsubps      %%xmm0, %%xmm1, %%xmm1  # xmm1 = 0 - delta
            \\  vmaxps      %%xmm1, %%xmm0, %%xmm0  # xmm0 = abs(delta)
            \\  vcmpleps    %%xmm2, %%xmm0, %%xmm0  # xmm0 = abs(delta) <= epsilon
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  subps       %%xmm1, %%xmm0          # xmm0 = delta
            \\  xorps       %%xmm1, %%xmm1          # xmm1 = 0
            \\  subps       %%xmm0, %%xmm1          # xmm1 = 0 - delta
            \\  maxps       %%xmm1, %%xmm0          # xmm0 = abs(delta)
            \\  cmpleps     %%xmm2, %%xmm0          # xmm0 = abs(delta) <= epsilon
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
              [epsilon] "{xmm2}" (epsilon),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = vecNearEqual(v0, v1, epsilon);
        return mask[0] and mask[1] and mask[2];
    }
}
test "zmath.vec3NearEqual" {
    {
        const v0 = vecSet(1.0, 2.0, -3.0001, 1000.001);
        const v1 = vecSet(1.0, 2.001, -3.0, 4.0);
        try check(vec3NearEqual(v0, v1, vecSplat(0.01)));
    }
}

pub inline fn vec3Less(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpltps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  cmpltps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 < v1;
        return mask[0] and mask[1] and mask[2];
    }
}
test "zmath.vec3Less" {
    const v0 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 1.0);
    try check(vec3Less(v0, v1) == true);

    const v2 = vecSet(-1.0, 2.0, 3.0, 5.0);
    const v3 = vecSet(4.0, -5.0, 6.0, 1.0);
    try check(vec3Less(v2, v3) == false);

    const v4 = vecSet(100.0, 200.0, 300.0, -500.0);
    const v5 = vecSet(400.0, 500.0, 600.0, 1.0);
    try check(vec3Less(v4, v5) == true);

    const v6 = vecSet(100.0, -math.inf_f32, -math.inf_f32, 50000.0);
    const v7 = vecSet(400.0, math.inf_f32, 600.0, 1.0);
    try check(vec3Less(v6, v7) == true);
}

pub inline fn vec3LessOrEqual(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpleps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  cmpleps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 <= v1;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn vec3Greater(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpgtps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  cmpgtps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 > v1;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn vec3GreaterOrEqual(v0: Vec, v1: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vcmpgeps    %%xmm1, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  cmpgeps     %%xmm1, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const mask = v0 >= v1;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn vec3InBounds(v: Vec, bounds: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vmovaps     %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  vxorps      %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\  vcmpleps    %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\  vcmpleps    %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\  vandps      %%xmm2, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
        else
            \\  movaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  xorps       %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\  cmpleps     %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\  cmpleps     %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\  andps       %%xmm2, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  and         $7, %%al
            \\  cmp         $7, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
              [bounds] "{xmm1}" (bounds),
              [x8000_0000] "{memory}" (f32x4_0x8000_0000),
            : "xmm2"
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b0 = @select(u32, v <= bounds, u32x4_0xffff_ffff, vecU32Zero());
        const b1 = @select(u32, bounds * vecSplat(-1.0) <= v, u32x4_0xffff_ffff, vecU32Zero());
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0 and b[2] > 0;

        // I've also tried this (generated asm is different but still not great):
        // const b0 = v <= bounds;
        // const b1 = bounds * vecSplat(-1.0) <= v;
        // const b = vecBoolAnd(b0, b1);
        // return b[0] and b[1] and b[2];
    }
}
test "zmath.vec3InBounds" {
    {
        const v0 = vecSet(0.5, -2.0, -1.0, 1.0);
        const v1 = vecSet(-1.6, 1.1, -0.1, 2.9);
        const v2 = vecSet(0.5, -2.0, -1.0, 10.0);
        const bounds = vecSet(1.0, 2.0, 1.0, 2.0);
        try check(vec3InBounds(v0, bounds) == true);
        try check(vec3InBounds(v1, bounds) == false);
        try check(vec3InBounds(v2, bounds) == true);
    }
    {
        const v0 = vecSet(10000.0, -1000.0, -1.0, 1000.0);
        const bounds = vecSet(math.inf_f32, math.inf_f32, 1.0, 2.0);
        try check(vec3InBounds(v0, bounds) == true);
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
test "zmath.vec3Dot" {
    const v0 = vecSet(-1.0, 2.0, 3.0, 1.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 1.0);
    var v = vec3Dot(v0, v1);
    try check(vec4ApproxEqAbs(v, vecSplat(24.0), 0.0001));
}

//
// Vec4 functions
//
// vec4Equal(v0: Vec, v1: Vec) bool
// vec4EqualInt(v0: Vec, v1: Vec) bool
// vec4NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool
// vec4Less(v0: Vec, v1: Vec) bool
// vec4LessOrEqual(v0: Vec, v1: Vec) bool
// vec4Greater(v0: Vec, v1: Vec) bool
// vec4GreaterOrEqual(v0: Vec, v1: Vec) bool
// vec4InBounds(v: Vec, bounds: Vec) bool
// vec4Dot(v0: Vec, v1: Vec) Vec

pub inline fn vec4Equal(v0: Vec, v1: Vec) bool {
    const mask = v0 == v1;
    return @reduce(.And, mask);
}

pub inline fn vec4EqualInt(v0: Vec, v1: Vec) bool {
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    const mask = v0u == v1u;
    return @reduce(.And, mask);
}

pub inline fn vec4NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool {
    // Won't handle inf & nan
    const delta = v0 - v1;
    const temp = vecMaxFast(delta, vecZero() - delta);
    return @reduce(.And, temp <= epsilon);
}

pub inline fn vec4Less(v0: Vec, v1: Vec) bool {
    const mask = v0 < v1;
    return @reduce(.And, mask);
}

pub inline fn vec4LessOrEqual(v0: Vec, v1: Vec) bool {
    const mask = v0 <= v1;
    return @reduce(.And, mask);
}

pub inline fn vec4Greater(v0: Vec, v1: Vec) bool {
    const mask = v0 > v1;
    return @reduce(.And, mask);
}

pub inline fn vec4GreaterOrEqual(v0: Vec, v1: Vec) bool {
    const mask = v0 >= v1;
    return @reduce(.And, mask);
}

pub inline fn vec4InBounds(v: Vec, bounds: Vec) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\  vmovaps     %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  vxorps      %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\  vcmpleps    %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\  vcmpleps    %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\  vandps      %%xmm2, %%xmm0, %%xmm0
            \\  vmovmskps   %%xmm0, %%eax
            \\  cmp         $15, %%al
            \\  sete        %%al
        else
            \\  movaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\  xorps       %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\  cmpleps     %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\  cmpleps     %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\  andps       %%xmm2, %%xmm0
            \\  movmskps    %%xmm0, %%eax
            \\  cmp         $15, %%al
            \\  sete        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
              [bounds] "{xmm1}" (bounds),
              [x8000_0000] "{memory}" (f32x4_0x8000_0000),
            : "xmm2"
        );
    } else {
        // 2 x cmpleps, movmskps, andps, xorps, pslld, load
        const b0 = @select(u32, v <= bounds, u32x4_0xffff_ffff, vecU32Zero());
        const b1 = @select(u32, bounds * vecSplat(-1.0) <= v, u32x4_0xffff_ffff, vecU32Zero());
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0 and b[2] > 0 and b[3] > 0;
    }
}
test "zmath.vec4InBounds" {
    {
        const v0 = vecSet(0.5, -2.0, -1.0, 1.9);
        const v1 = vecSet(-1.6, -2.001, -1.0, 1.9);
        const bounds = vecSet(1.0, 2.0, 1.0, 2.0);
        try check(vec4InBounds(v0, bounds) == true);
        try check(vec4InBounds(v1, bounds) == false);
    }
    {
        const v0 = vecSet(10000.0, -1000.0, -1.0, 0.0);
        const bounds = vecSet(math.inf_f32, math.inf_f32, 1.0, 2.0);
        try check(vec4InBounds(v0, bounds) == true);
    }
}

pub inline fn vec4Dot(v0: Vec, v1: Vec) Vec {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | z0*z1 | w0*w1 |
    var xmm1 = @shuffle(f32, xmm0, undefined, [4]i32{ 1, 0, 3, 0 }); // | y0*y1 | -- | w0*w1 | -- |
    xmm1 = xmm0 + xmm1; // | x0*x1 + y0*y1 | -- | z0*z1 + w0*w1 | -- |
    xmm0 = @shuffle(f32, xmm1, undefined, [4]i32{ 2, 0, 0, 0 }); // | z0*z1 + w0*w1 | -- | -- | -- |
    xmm0 = vecSet(xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[2]); // addss
    return vecSplatX(xmm0);
}
test "zmath.vec4Dot" {
    const v0 = vecSet(-1.0, 2.0, 3.0, -2.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 2.0);
    var v = vec4Dot(v0, v1);
    try check(vec4ApproxEqAbs(v, vecSplat(20.0), 0.0001));
}

//
// Private functions and constants
//

const u32x4_0xffff_ffff: VecU32 = @splat(4, ~@as(u32, 0));
const f32x4_0x8000_0000: Vec = vecSplatInt(0x8000_0000);
const f32x4_0x7fff_ffff: Vec = vecSplatInt(0x7fff_ffff);
const f32x4_inf: Vec = vecSplat(math.inf_f32);
const f32x4_nan: Vec = vecSplat(math.nan_f32);
const f32x4_qnan: Vec = vecSplat(math.qnan_f32);
const f32x4_epsilon: Vec = vecSplat(math.epsilon_f32);
const f32x4_8_388_608: Vec = vecSplat(8_388_608.0);

inline fn vecU32Zero() VecU32 {
    return @splat(4, @as(u32, 0));
}

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
test "zmath.vecFloatToIntAndBack" {
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

inline fn vec2ApproxEqAbs(v0: Vec, v1: Vec, eps: f32) bool {
    return math.approxEqAbs(f32, v0[0], v1[0], eps) and
        math.approxEqAbs(f32, v0[1], v1[1], eps);
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
