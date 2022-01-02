const builtin = @import("builtin");
const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const expect = std.testing.expect;

const cpu_arch = builtin.cpu.arch;
const has_avx = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .avx) else false;

pub const Vec = @Vector(4, f32);
pub const VecBool = @Vector(4, bool);
pub const VecI32 = @Vector(4, i32);
pub const VecU32 = @Vector(4, u32);

pub const f32x4 = @Vector(4, f32);
pub const f32x8 = @Vector(8, f32);
pub const u32x4 = @Vector(4, u32);

//
// General Vec functions (always work on all vector components)
//
// vecZero() Vec
// vecI32Zero() VecI32
// vecU32Zero() VecU32
// vecSet(x: f32, y: f32, z: f32, w: f32) Vec
// vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec
// vecSplat(value: f32) Vec
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
// vecSwizzle( v: Vec, xyzw: VecComponent) Vec
// vecMod(v0: Vec, v1: Vec) Vec
// vecMulAdd(v0: Vec, v1: Vec, v2: Vec) Vec
// vecSin(v: Vec) Vec

// zig fmt: off
pub inline fn vecZero() Vec { return @splat(4, @as(f32, 0)); }
pub inline fn vecI32Zero() VecI32 { return @splat(4, @as(i32, 0)); }
pub inline fn vecU32Zero() VecU32 { return @splat(4, @as(u32, 0)); }
// zig fmt: on
test "zmath.vecZero" {
    const v = vecZero();
    const vi = vecI32Zero();
    const vu = vecU32Zero();
    try expect(v[0] == 0 and v[1] == 0 and v[2] == 0 and v[3] == 0);
    try expect(vi[0] == 0 and vi[1] == 0 and vi[2] == 0 and vi[3] == 0);
    try expect(vu[0] == 0 and vu[1] == 0 and vu[2] == 0 and vu[3] == 0);
}

pub inline fn vecSet(x: f32, y: f32, z: f32, w: f32) Vec {
    return [4]f32{ x, y, z, w };
}
test "zmath.vecSet" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSet(5.0, -6.0, 7.0, 8.0);
    try expect(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
    try expect(v1[0] == 5.0 and v1[1] == -6.0 and v1[2] == 7.0 and v1[3] == 8.0);
}

test "zmath.vecSet" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSet(5.0, -6.0, 7.0, 8.0);
    try expect(v0[0] == 1.0 and v0[1] == 2.0 and v0[2] == 3.0 and v0[3] == 4.0);
    try expect(v1[0] == 5.0 and v1[1] == -6.0 and v1[2] == 7.0 and v1[3] == 8.0);
}

pub inline fn vecSetInt(x: u32, y: u32, z: u32, w: u32) Vec {
    return @bitCast(Vec, [4]u32{ x, y, z, w });
}
test "zmath.vecSetInt" {
    const v = vecSetInt(0x3f80_0000, 0x4000_0000, 0x4040_0000, 0x4080_0000);
    try expect(vec4ApproxEqAbs(v, [4]f32{ 1.0, 2.0, 3.0, 4.0 }, 0.0));
}

pub inline fn vecSplat(value: f32) Vec {
    return @splat(4, value);
}
pub inline fn splat(comptime T: type, value: f32) T {
    return @splat(@typeInfo(T).Vector.len, value);
}
test "zmath.vecSplat" {
    const v = vecSplat(123.0);
    try expect(vec4ApproxEqAbs(v, [4]f32{ 123.0, 123.0, 123.0, 123.0 }, 0.0));
}

pub inline fn usplat(comptime T: type, value: u32) T {
    return @splat(@typeInfo(T).Vector.len, value);
}
pub inline fn splatInt(comptime T: type, value: u32) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, value));
}
pub inline fn vecSplatInt(value: u32) Vec {
    return @splat(4, @bitCast(f32, value));
}
test "zmath.vecSplatInt" {
    const v = vecSplatInt(0x4000_0000);
    try expect(vec4ApproxEqAbs(v, [4]f32{ 2.0, 2.0, 2.0, 2.0 }, 0.0));
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
    try expect(b[0] == true);
    try expect(b[1] == false);
    try expect(b[2] == false);
    try expect(b[3] == true);
    try expect(@reduce(.And, b == vecBoolSet(true, false, false, true)));
    try expect(vecBoolAllTrue(b == vecBoolSet(true, false, false, true)));
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
pub inline fn andInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    // andps
    return @bitCast(T, v0u & v1u);
}
test "zmath.vecAndInt" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v1 = vecSet(1.0, 2.0, 3.0, math.inf_f32);
    const v = vecAndInt(v0, v1);
    try expect(v[3] == math.inf_f32);
    try expect(vec3ApproxEqAbs(v, [4]f32{ 0.0, 2.0, 0.0, math.inf_f32 }, 0.0));
}

pub inline fn vecAndCInt(v0: Vec, v1: Vec) Vec {
    // andnps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u & ~v1u);
}
pub inline fn andCInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, v0u & ~v1u); // andnps
}
test "zmath.vecAndCInt" {
    const v0 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v1 = vecSetInt(0, ~@as(u32, 0), 0, ~@as(u32, 0));
    const v = vecAndCInt(v0, v1);
    try expect(vec4ApproxEqAbs(v, [4]f32{ 1.0, 0.0, 3.0, 0.0 }, 0.0));
}

pub inline fn vecOrInt(v0: Vec, v1: Vec) Vec {
    // orps
    const v0u = @bitCast(VecU32, v0);
    const v1u = @bitCast(VecU32, v1);
    return @bitCast(Vec, v0u | v1u);
}
pub inline fn orInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, v0u | v1u); // orps
}
test "zmath.vecOrInt" {
    const v0 = vecSetInt(0, ~@as(u32, 0), 0, 0);
    const v1 = vecSet(1.0, 2.0, 3.0, 4.0);
    const v = vecOrInt(v0, v1);
    try expect(v[0] == 1.0);
    try expect(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try expect(v[2] == 3.0);
    try expect(v[3] == 4.0);
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
    try expect(v[0] == 0.0);
    try expect(@bitCast(u32, v[1]) == ~@as(u32, 0));
    try expect(v[2] == 0.0);
    try expect(v[3] == 0.0);
}

pub inline fn vecIsNan(v: Vec) VecBool {
    return v != v;
}
test "zmath.vecIsNan" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsNan(v0);
    try expect(vecBoolEqual(b, vecBoolSet(false, true, true, false)));
}

pub inline fn vecIsInf(v: Vec) VecBool {
    return vecAbs(v) == f32x4_inf;
}
test "zmath.vecIsInf" {
    const v0 = vecSet(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
    const b = vecIsInf(v0);
    try expect(vecBoolEqual(b, vecBoolSet(true, false, false, false)));
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
        try expect(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMinFast(v0, v1);
        try expect(v[0] == 1.0);
        try expect(v[1] == 1.0);
        try expect(!math.isNan(v[1]));
        try expect(v[2] == 4.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
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
        try expect(vec4ApproxEqAbs(v, vecSet(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMaxFast(v0, v1);
        try expect(v[0] == 2.0);
        try expect(v[1] == 1.0);
        try expect(v[2] == 5.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
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
        try expect(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMin(v0, v1);
        try expect(v[0] == 1.0);
        try expect(v[1] == 1.0);
        try expect(!math.isNan(v[1]));
        try expect(v[2] == 4.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = vecSet(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecMin(v0, v1);
        try expect(v[0] == -math.inf_f32);
        try expect(v[1] == -math.inf_f32);
        try expect(v[2] == math.inf_f32);
        try expect(!math.isNan(v[2]));
        try expect(math.isNan(v[3]));
        try expect(!math.isInf(v[3]));
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
        try expect(vec4ApproxEqAbs(v, vecSet(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = vecSet(2.0, 1.0, 4.0, math.inf_f32);
        const v = vecMax(v0, v1);
        try expect(v[0] == 2.0);
        try expect(v[1] == 1.0);
        try expect(v[2] == 5.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = vecSet(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecMax(v0, v1);
        try expect(v[0] == -math.inf_f32);
        try expect(v[1] == math.inf_f32);
        try expect(v[2] == math.inf_f32);
        try expect(!math.isNan(v[2]));
        try expect(math.isNan(v[3]));
        try expect(!math.isInf(v[3]));
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
    try expect(vecBoolEqual(b0, vecBoolSet(true, true, true, true)));
    try expect(vecBoolEqual(b1, vecBoolSet(false, false, true, true)));
}

test "zmath.round" {
    {
        try expect(isEqual4(round(splat(f32x4, math.inf_f32)), splat(f32x4, math.inf_f32)));
        try expect(isEqual4(round(splat(f32x4, -math.inf_f32)), splat(f32x4, -math.inf_f32)));
        try expect(isNan4(round(splat(f32x4, math.nan_f32))));
        try expect(isNan4(round(splat(f32x4, -math.nan_f32))));
        try expect(isNan4(round(splat(f32x4, math.qnan_f32))));
        try expect(isNan4(round(splat(f32x4, -math.qnan_f32))));
    }
    var v = round(f32x4{ 1.1, -1.1, -1.5, 1.5 });
    try expect(approxEqAbs(v, f32x4{ 1.0, -1.0, -2.0, 2.0 }, 0.0));

    const v1 = f32x4{ -10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32 };
    v = round(v1);
    try expect(v[3] == math.inf_f32);
    try expect(approxEqAbs(v, f32x4{ -10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = f32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 };
    v = round(v2);
    try expect(math.isNan(v2[0]));
    try expect(math.isNan(v2[1]));
    try expect(math.isNan(v2[2]));
    try expect(v2[3] == -math.inf_f32);

    const v3 = f32x4{ 1001.5, -201.499, -10000.99, -101.5 };
    v = round(v3);
    try expect(approxEqAbs(v, f32x4{ 1002.0, -201.0, -10001.0, -102.0 }, 0.0));

    const v4 = f32x4{ -1_388_609.9, 1_388_609.5, 1_388_109.01, 2_388_609.5 };
    v = round(v4);
    try expect(approxEqAbs(v, f32x4{ -1_388_610.0, 1_388_610.0, 1_388_109.0, 2_388_610.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = round(splat(f32x4, f));
        const fr = @round(splat(f32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn trunc(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == f32x4) {
            return asm ("vroundps $3, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == f32x8) {
            return asm ("vroundps $3, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = @fabs(v) < splatNoFraction(T);
        const result = floatToIntAndBack(v);
        return @select(f32, mask, result, v);
    }
}
test "zmath.trunc" {
    {
        try expect(isEqual4(trunc(splat(f32x4, math.inf_f32)), splat(f32x4, math.inf_f32)));
        try expect(isEqual4(trunc(splat(f32x4, -math.inf_f32)), splat(f32x4, -math.inf_f32)));
        try expect(isNan4(trunc(splat(f32x4, math.nan_f32))));
        try expect(isNan4(trunc(splat(f32x4, -math.nan_f32))));
        try expect(isNan4(trunc(splat(f32x4, math.qnan_f32))));
        try expect(isNan4(trunc(splat(f32x4, -math.qnan_f32))));
    }
    var v = trunc(f32x4{ 1.1, -1.1, -1.5, 1.5 });
    try expect(approxEqAbs(v, f32x4{ 1.0, -1.0, -1.0, 1.0 }, 0.0));

    v = trunc(f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = trunc(f32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = trunc(f32x4{ 1000.5001, -201.499, -10000.99, 100.750001 });
    try expect(approxEqAbs(v, f32x4{ 1000.0, -201.0, -10000.0, 100.0 }, 0.0));

    v = trunc(f32x4{ -7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5 });
    try expect(approxEqAbs(v, f32x4{ -7_388_609.0, 7_388_609.0, 8_388_109.0, -8_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = trunc(splat(f32x4, f));
        const fr = @trunc(splat(f32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn floor(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == f32x4) {
            return asm ("vroundps $1, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == f32x8) {
            return asm ("vroundps $1, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = @fabs(v) < splatNoFraction(T);
        var result = floatToIntAndBack(v);
        const larger_mask = result > v;
        const larger = @select(f32, larger_mask, splat(T, -1.0), splat(T, 0.0));
        result = result + larger;
        return @select(f32, mask, result, v);
    }
}
test "zmath.floor" {
    {
        try expect(isEqual4(floor(splat(f32x4, math.inf_f32)), splat(f32x4, math.inf_f32)));
        try expect(isEqual4(floor(splat(f32x4, -math.inf_f32)), splat(f32x4, -math.inf_f32)));
        try expect(isNan4(floor(splat(f32x4, math.nan_f32))));
        try expect(isNan4(floor(splat(f32x4, -math.nan_f32))));
        try expect(isNan4(floor(splat(f32x4, math.qnan_f32))));
        try expect(isNan4(floor(splat(f32x4, -math.qnan_f32))));
    }
    var v = floor(f32x4{ 1.5, -1.5, -1.7, -2.1 });
    try expect(approxEqAbs(v, f32x4{ 1.0, -2.0, -2.0, -3.0 }, 0.0));

    v = floor(f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = floor(f32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = floor(f32x4{ 1000.5001, -201.499, -10000.99, 100.75001 });
    try expect(approxEqAbs(v, f32x4{ 1000.0, -202.0, -10001.0, 100.0 }, 0.0));

    v = floor(f32x4{ -7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5 });
    try expect(approxEqAbs(v, f32x4{ -7_388_610.0, 7_388_609.0, 8_388_109.0, -8_388_510.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = floor(splat(f32x4, f));
        const fr = @floor(splat(f32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn ceil(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == f32x4) {
            return asm ("vroundps $2, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == f32x8) {
            return asm ("vroundps $2, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = @fabs(v) < splatNoFraction(T);
        var result = floatToIntAndBack(v);
        const smaller_mask = result < v;
        const smaller = @select(f32, smaller_mask, splat(T, -1.0), splat(T, 0.0));
        result = result - smaller;
        return @select(f32, mask, result, v);
    }
}
test "zmath.vecCeil" {
    {
        try expect(isEqual4(ceil(splat(f32x4, math.inf_f32)), splat(f32x4, math.inf_f32)));
        try expect(isEqual4(ceil(splat(f32x4, -math.inf_f32)), splat(f32x4, -math.inf_f32)));
        try expect(isNan4(ceil(splat(f32x4, math.nan_f32))));
        try expect(isNan4(ceil(splat(f32x4, -math.nan_f32))));
        try expect(isNan4(ceil(splat(f32x4, math.qnan_f32))));
        try expect(isNan4(ceil(splat(f32x4, -math.qnan_f32))));
    }
    var v = ceil(f32x4{ 1.5, -1.5, -1.7, -2.1 });
    try expect(approxEqAbs(v, f32x4{ 2.0, -1.0, -1.0, -2.0 }, 0.0));

    v = ceil(f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, f32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = ceil(f32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = ceil(f32x4{ 1000.5001, -201.499, -10000.99, 100.75001 });
    try expect(approxEqAbs(v, f32x4{ 1001.0, -201.0, -10000.0, 101.0 }, 0.0));

    v = ceil(f32x4{ -1_388_609.5, 1_388_609.1, 1_388_109.9, -1_388_509.9 });
    try expect(approxEqAbs(v, f32x4{ -1_388_609.0, 1_388_610.0, 1_388_110.0, -1_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = ceil(splat(f32x4, f));
        const fr = @ceil(splat(f32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
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
        try expect(vec4ApproxEqAbs(v, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecClamp(v0, vecSet(-100.0, 0.0, -100.0, 0.0), vecSet(0.0, 100.0, 0.0, 100.0));
        try expect(vec4ApproxEqAbs(v, vecSet(-100.0, 100.0, -100.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecClamp(v0, vecSplat(-1.0), vecSplat(1.0));
        try expect(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, -1.0, -1.0), 0.0001));
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
        try expect(vec4ApproxEqAbs(v, vecSet(-0.5, 0.2, 0.5, -0.3), 0.0001));
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
        try expect(vec4ApproxEqAbs(v, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecSaturate(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecSaturate(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
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
        try expect(vec4ApproxEqAbs(v, vecSet(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = vecSaturateFast(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = vecSet(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = vecSaturateFast(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(1.0, 1.0, 0.0, 0.0), 0.0001));
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
        try expect(vec4ApproxEqAbs(
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
        try expect(vec3ApproxEqAbs(v, vecSet(0.0, 1.0 / math.sqrt(v0[1]), 1.0 / math.sqrt(v0[2]), 0.0), 0.0005));
        try expect(math.isNan(v[3]));
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
        try expect(vec4ApproxEqAbs(
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
        try expect(vec3ApproxEqAbs(v, vecSet(0.0, 1.0 / math.sqrt(v0[1]), 1.0 / math.sqrt(v0[2]), 0.0), 0.0005));
        try expect(math.isNan(v[3]));
    }
}

pub inline fn vecRcp(v: Vec) Vec {
    // load, divps
    return vecSplat(1.0) / v;
}
test "zmath.vecRcp" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcp(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(1.0 / v0[0], 1.0 / v0[1], 1.0 / v0[2], 1.0 / v0[3]), 0.0005));
    }
    {
        const v0 = vecSet(math.inf_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = vecRcp(v0);
        try expect(vec2ApproxEqAbs(v, vecSet(0.0, 0.0, 0.0, 0.0), 0.0005));
        try expect(math.isNan(v[2]));
        try expect(math.isNan(v[3]));
    }
}

pub inline fn vecRcpFast(v: Vec) Vec {
    // load, rcpps, 2 x mulps, addps, subps
    @setFloatMode(.Optimized);
    return vecSplat(1.0) / v;
}
test "zmath.vecRcpFast" {
    {
        const v0 = vecSet(1.0, 0.2, 123.1, 0.72);
        const v = vecRcpFast(v0);
        try expect(vec4ApproxEqAbs(v, vecSet(1.0 / v0[0], 1.0 / v0[1], 1.0 / v0[2], 1.0 / v0[3]), 0.001));
    }
}

pub inline fn vecLerp(v0: Vec, v1: Vec, t: f32) Vec {
    // subps, shufps, addps, mulps
    return v0 + (v1 - v0) * vecSplat(t);
}

pub inline fn vecLerpV(v0: Vec, v1: Vec, t: Vec) Vec {
    // subps, addps, mulps
    return v0 + (v1 - v0) * t;
}

pub const VecComponent = enum { x, y, z, w };

pub inline fn vecSwizzle(
    v: Vec,
    comptime x: VecComponent,
    comptime y: VecComponent,
    comptime z: VecComponent,
    comptime w: VecComponent,
) Vec {
    return @shuffle(f32, v, undefined, [4]i32{ @enumToInt(x), @enumToInt(y), @enumToInt(z), @enumToInt(w) });
}

pub inline fn mod(v0: anytype, v1: anytype) @TypeOf(v0) {
    // vdivps, vroundps, vmulps, vsubps
    return v0 - v1 * trunc(v0 / v1);
}
test "zmath.mod" {
    try expect(approxEqAbs(mod(splat(f32x4, 3.1), splat(f32x4, 1.7)), splat(f32x4, 1.4), 0.0005));
    try expect(approxEqAbs(mod(splat(f32x4, -3.0), splat(f32x4, 2.0)), splat(f32x4, -1.0), 0.0005));
    try expect(approxEqAbs(mod(splat(f32x4, -3.0), splat(f32x4, -2.0)), splat(f32x4, -1.0), 0.0005));
    try expect(approxEqAbs(mod(splat(f32x4, 3.0), splat(f32x4, -2.0)), splat(f32x4, 1.0), 0.0005));
    try expect(isNan4(mod(splat(f32x4, math.inf_f32), splat(f32x4, 1.0))));
    try expect(isNan4(mod(splat(f32x4, -math.inf_f32), splat(f32x4, 123.456))));
    try expect(isNan4(mod(splat(f32x4, math.nan_f32), splat(f32x4, 123.456))));
    try expect(isNan4(mod(splat(f32x4, math.qnan_f32), splat(f32x4, 123.456))));
    try expect(isNan4(mod(splat(f32x4, -math.qnan_f32), splat(f32x4, 123.456))));
    try expect(isNan4(mod(splat(f32x4, 123.456), splat(f32x4, math.inf_f32))));
    try expect(isNan4(mod(splat(f32x4, 123.456), splat(f32x4, -math.inf_f32))));
    try expect(isNan4(mod(splat(f32x4, 123.456), splat(f32x4, math.nan_f32))));
    try expect(isNan4(mod(splat(f32x4, math.inf_f32), splat(f32x4, math.inf_f32))));
    try expect(isNan4(mod(splat(f32x4, math.inf_f32), splat(f32x4, math.nan_f32))));
}

pub inline fn splatOne(comptime T: type) T {
    return splat(T, 1.0);
}
pub inline fn splatHalfPi(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, 0.5 * math.pi));
}
pub inline fn splatPi(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, math.pi));
}
pub inline fn splatTwoPi(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, math.tau));
}
pub inline fn splatRcpTwoPi(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, 1.0 / math.tau));
}
pub inline fn splatNegativeZero(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, @as(u32, 0x8000_0000)));
}
pub inline fn splatNoFraction(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, 8_388_608.0));
}
pub inline fn splatAbsMask(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, @as(u32, 0x7fff_ffff)));
}

pub inline fn round(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == f32x4) {
            return asm ("vroundps $0, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == f32x8) {
            return asm ("vroundps $0, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const sign = andInt(v, splatNegativeZero(T));
        const magic = orInt(splatNoFraction(T), sign);
        var r1 = v + magic;
        r1 = r1 - magic;
        const r2 = @fabs(v);
        const mask = r2 <= splatNoFraction(T);
        return @select(f32, mask, r1, v);
    }
}

pub inline fn modAngles(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    // 2 x vmulps, 2 x load, vroundps, vaddps
    return v - splatTwoPi(T) * round(v * splatRcpTwoPi(T));
}
test "zmath.modAngles" {
    try expect(approxEqAbs(modAngles(splat(f32x4, math.tau)), splat(f32x4, 0.0), 0.0005));
    try expect(approxEqAbs(modAngles(splat(f32x4, 0.0)), splat(f32x4, 0.0), 0.0005));
    try expect(approxEqAbs(modAngles(splat(f32x4, math.pi)), splat(f32x4, math.pi), 0.0005));
    try expect(vec4ApproxEqAbs(modAngles(splat(f32x4, 11 * math.pi)), splat(f32x4, math.pi), 0.0005));
    try expect(vec4ApproxEqAbs(modAngles(splat(f32x4, 3.5 * math.pi)), splat(f32x4, -0.5 * math.pi), 0.0005));
    try expect(vec4ApproxEqAbs(modAngles(splat(f32x4, 2.5 * math.pi)), splat(f32x4, 0.5 * math.pi), 0.0005));
}

pub inline fn mulAdd(v0: anytype, v1: anytype, v2: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    if (cpu_arch == .x86_64 and has_avx) {
        return @mulAdd(T, v0, v1, v2);
    } else {
        // NOTE(mziulek): On .x86_64 without HW fma instructions @mulAdd maps to really slow code!
        return v0 * v1 + v2;
    }
}

pub inline fn sin(v: anytype) @TypeOf(v) {
    // 11-degree minimax approximation
    // According to llvm-mca this routine will take on average:
    // * zen2 (AVX, SIMDx4, SIMDx8): ~51 cycles
    // * skylake (AVX, SIMDx4, SIMDx8): ~57 cycles
    // * x86_64 (SIMDx4): ~100 cycles
    const T = @TypeOf(v);
    var x = modAngles(v);

    const sign = andInt(x, splatNegativeZero(T));
    const c = orInt(sign, splatPi(T));
    const absx = andCInt(x, sign);
    const rflx = c - x;
    const comp = absx <= splatHalfPi(T);
    x = @select(f32, comp, x, rflx);
    const x2 = x * x;

    var result = mulAdd(splatSinC4(T), x2, splatSinC3(T));
    result = mulAdd(result, x2, splatSinC2(T));
    result = mulAdd(result, x2, splatSinC1(T));
    result = mulAdd(result, x2, splatSinC0(T));
    result = mulAdd(result, x2, splatOne(T));
    return x * result;
}
test "sin" {
    const epsilon = 0.0001;

    try expect(approxEqAbs(sin(splat(f32x4, 0.5 * math.pi)), splat(f32x4, 1.0), epsilon));
    try expect(approxEqAbs(sin(splat(f32x4, 0.0)), splat(f32x4, 0.0), epsilon));
    try expect(approxEqAbs(sin(splat(f32x4, -0.0)), splat(f32x4, -0.0), epsilon));
    try expect(approxEqAbs(sin(splat(f32x4, 89.123)), splat(f32x4, 0.916166), epsilon));
    try expect(isNan4(sin(splat(f32x4, math.inf_f32))) == true);
    try expect(isNan4(sin(splat(f32x4, -math.inf_f32))) == true);
    try expect(isNan4(sin(splat(f32x4, math.nan_f32))) == true);
    try expect(isNan4(sin(splat(f32x4, math.qnan_f32))) == true);

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = sin(splat(f32x4, f));
        const fr = @sin(splat(f32x4, f));
        try expect(approxEqAbs(vr, fr, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
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
    try expect(b[0] == true and b[1] == false and b[2] == false and b[3] == false);
}

pub inline fn vecBoolOr(b0: VecBool, b1: VecBool) VecBool {
    // orps
    return [4]bool{ b0[0] or b1[0], b0[1] or b1[1], b0[2] or b1[2], b0[3] or b1[3] };
}
test "zmath.vecBoolOr" {
    const b0 = vecBoolSet(true, false, true, false);
    const b1 = vecBoolSet(true, true, false, false);
    const b = vecBoolOr(b0, b1);
    try expect(b[0] == true and b[1] == true and b[2] == true and b[3] == false);
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
    try expect(vec4ApproxEqAbs(v0, [4]f32{ 1.0, 2.0, 0.0, 0.0 }, 0.0));
    i += 2;
    const v1 = vecLoadF32x2(a[i .. i + 2]);
    try expect(vec4ApproxEqAbs(v1, [4]f32{ 3.0, 4.0, 0.0, 0.0 }, 0.0));
    const v2 = vecLoadF32x2(a[5..7]);
    try expect(vec4ApproxEqAbs(v2, [4]f32{ 6.0, 7.0, 0.0, 0.0 }, 0.0));
    const v3 = vecLoadF32x2(ptr[1..]);
    try expect(vec4ApproxEqAbs(v3, [4]f32{ 2.0, 3.0, 0.0, 0.0 }, 0.0));
    i += 1;
    const v4 = vecLoadF32x2(ptr[i .. i + 2]);
    try expect(vec4ApproxEqAbs(v4, [4]f32{ 4.0, 5.0, 0.0, 0.0 }, 0.0));
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
    try expect(a[0] == 1.0);
    try expect(a[1] == 2.0);
    try expect(a[2] == 2.0);
    try expect(a[3] == 3.0);
    try expect(a[4] == 4.0);
    try expect(a[5] == 0.0);
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
// vec2IsNan(v: Vec) bool
// vec2IsInf(v: Vec) bool
// vec2Dot(v0: Vec, v1: Vec) Vec
// vec2LengthSq(v: Vec) Vec
// vec2RcpLengthFast(v: Vec) Vec
// vec2RcpLength(v: Vec) Vec
// vec2Length(v: Vec) Vec
// vec2NormalizeFast(v: Vec) Vec
// vec2Normalize(v: Vec) Vec

pub inline fn isEqual2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpeqps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ cmpeqps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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
test "zmath.isEqual2" {
    {
        const v0 = f32x4{ 1.0, math.inf_f32, -3.0, 1000.001 };
        const v1 = f32x4{ 1.0, math.inf_f32, -6.0, 4.0 };
        try expect(isEqual2(v0, v1));
    }
}

pub inline fn isEqualInt2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vpcmpeqd     %%xmm1, %%xmm0, %xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ pcmpeqd      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const v0u = @bitCast(u32x4, v0);
        const v1u = @bitCast(u32x4, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1];
    }
}

pub inline fn isNearEqual2(v0: f32x4, v1: f32x4, epsilon: f32x4) bool {
    // Won't handle inf & nan
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vsubps       %%xmm1, %%xmm0, %%xmm0  # xmm0 = delta
            \\ vxorps       %%xmm1, %%xmm1, %%xmm1  # xmm1 = 0
            \\ vsubps       %%xmm0, %%xmm1, %%xmm1  # xmm1 = 0 - delta
            \\ vmaxps       %%xmm1, %%xmm0, %%xmm0  # xmm0 = abs(delta)
            \\ vcmpleps     %%xmm2, %%xmm0, %%xmm0  # xmm0 = abs(delta) <= epsilon
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ subps        %%xmm1, %%xmm0          # xmm0 = delta
            \\ xorps        %%xmm1, %%xmm1          # xmm1 = 0
            \\ subps        %%xmm0, %%xmm1          # xmm1 = 0 - delta
            \\ maxps        %%xmm1, %%xmm0          # xmm0 = abs(delta)
            \\ cmpleps      %%xmm2, %%xmm0          # xmm0 = abs(delta) <= epsilon
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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
test "zmath.isNearEqual2" {
    {
        const v0 = f32x4{ 1.0, 2.0, -6.0001, 1000.001 };
        const v1 = f32x4{ 1.0, 2.001, -3.0, 4.0 };
        const v2 = f32x4{ 1.001, 2.001, -3.001, 4.001 };
        try expect(isNearEqual2(v0, v1, splat(f32x4, 0.01)));
        try expect(isNearEqual2(v2, v1, splat(f32x4, 0.01)));
    }
}

pub inline fn isLess2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpltps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ cmpltps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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
test "zmath.isLess2" {
    const v0 = f32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v1 = f32x4{ 4.0, 5.0, 6.0, 1.0 };
    try expect(isLess2(v0, v1) == true);

    const v2 = f32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v3 = f32x4{ 4.0, -5.0, 6.0, 1.0 };
    try expect(isLess2(v2, v3) == false);

    const v4 = f32x4{ 100.0, 200.0, 300.0, 50000.0 };
    const v5 = f32x4{ 400.0, 500.0, 600.0, 1.0 };
    try expect(isLess2(v4, v5) == true);

    const v6 = f32x4{ 100.0, -math.inf_f32, -math.inf_f32, 50000.0 };
    const v7 = f32x4{ 400.0, math.inf_f32, 600.0, 1.0 };
    try expect(isLess2(v6, v7) == true);
}

pub inline fn isLessEqual2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpleps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ cmpleps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ cmp          $3, %%al
            \\ sete         %%al
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

pub inline fn isGreater2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpgtps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ cmpgtps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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

pub inline fn isGreaterEqual2(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpgeps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ cmpgeps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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

pub inline fn isInBounds2(v: f32x4, bounds: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vmovaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ vxorps       %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\ vcmpleps     %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\ vcmpleps     %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\ vandps       %%xmm2, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
        else
            \\ movaps       %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ xorps        %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\ cmpleps      %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\ cmpleps      %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\ andps        %%xmm2, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $3, %%al
            \\ cmp          $3, %%al
            \\ sete         %%al
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
        const b0 = @select(u32, v <= bounds, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b1 = @select(u32, bounds * splat(f32x4, -1.0) <= v, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0;

        // I've also tried this (generated asm is different but still not great):
        // const b0 = v <= bounds;
        // const b1 = bounds * vecSplat(-1.0) <= v;
        // const b = vecBoolAnd(b0, b1);
        // return b[0] and b[1];
    }
}
test "zmath.isInBounds2" {
    {
        const v0 = f32x4{ 0.5, -2.0, -100.0, 1000.0 };
        const v1 = f32x4{ -1.6, -2.001, 1.0, 1.9 };
        const bounds = f32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds2(v0, bounds) == true);
        try expect(isInBounds2(v1, bounds) == false);
    }
    {
        const v0 = f32x4{ 10000.0, -1000.0, -10.0, 1000.0 };
        const bounds = f32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds2(v0, bounds) == true);
    }
}

pub inline fn isNan2(v: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpneqps    %%xmm0, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ test         $3, %%al
            \\ setne        %%al
        else
            \\ cmpneqps     %%xmm0, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ test         $3, %%al
            \\ setne        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b = v != v;
        return b[0] or b[1];
    }
}
test "zmath.isNan2" {
    try expect(isNan2(f32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan2(f32x4{ -1.0, 100.0, 3.0, math.nan_f32 }) == false);
    try expect(isNan2(f32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
    try expect(isNan2(f32x4{ -1.0, math.qnan_f32, 3.0, -2.0 }) == true);
    try expect(isNan2(f32x4{ -1.0, 1.0, 3.0, -2.0 }) == false);
    try expect(isNan2(f32x4{ -1.0, 1.0, 3.0, math.qnan_f32 }) == false);
}

pub inline fn isInf2(v: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vandps       %[x7fff_ffff], %%xmm0, %%xmm0
            \\ vcmpeqps     %[inf], %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ test         $3, %%al
            \\ setne        %%al
        else
            \\ andps        %[x7fff_ffff], %%xmm0
            \\ cmpeqps      %[inf], %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ test         $3, %%al
            \\ setne        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
              [x7fff_ffff] "{memory}" (f32x4_0x7fff_ffff),
              [inf] "{memory}" (f32x4_inf),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b = vecIsInf(v);
        return b[0] or b[1];
    }
}

pub inline fn dot2(v0: f32x4, v1: f32x4) f32x4 {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | -- | -- |
    var xmm1 = vecSwizzle(xmm0, .y, .x, .x, .x); // | y0*y1 | -- | -- | -- |
    xmm0 = f32x4{ xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[3] }; // | x0*x1 + y0*y1 | -- | -- | -- |
    return vecSwizzle(xmm0, .x, .x, .x, .x);
}
test "zmath.dot2" {
    const v0 = f32x4{ -1.0, 2.0, 300.0, -2.0 };
    const v1 = f32x4{ 4.0, 5.0, 600.0, 2.0 };
    var v = dot2(v0, v1);
    try expect(approxEqAbs(v, splat(f32x4, 6.0), 0.0001));
}

// zig fmt: off
pub inline fn lengthSq2(v: f32x4) f32x4 { return dot2(v, v); }
pub inline fn rcpLengthFast2(v: f32x4) f32x4 { return vecRcpSqrtFast(dot2(v, v)); }
pub inline fn rcpLength2(v: f32x4) f32x4 { return vecRcpSqrt(dot2(v, v)); }
pub inline fn length2(v: f32x4) f32x4 { return vecSqrt(dot2(v, v)); }
pub inline fn normalizeFast2(v: f32x4) f32x4 { return v * vecRcpSqrtFast(dot2(v, v)); }
pub inline fn normalize2(v: f32x4) f32x4 { return v * vecRcpSqrt(dot2(v, v)); }
// zig fmt: on

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
// vec3IsNan(v: Vec) bool
// vec3IsInf(v: Vec) bool
// vec3Dot(v0: Vec, v1: Vec) Vec
// vec3Cross(v0: Vec, v1: Vec) Vec
// vec3LengthSq(v: Vec) Vec
// vec3RcpLengthFast(v: Vec) Vec
// vec3RcpLength(v: Vec) Vec
// vec3Length(v: Vec) Vec
// vec3NormalizeFast(v: Vec) Vec
// vec3Normalize(v: Vec) Vec
// vec3LinePointDistance(line_pt0: Vec, line_pt1: Vec, pt: Vec) Vec

pub inline fn isEqual3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpeqps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ cmpeqps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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
test "zmath.isEqual3" {
    {
        const v0 = f32x4{ 1.0, math.inf_f32, -3.0, 1000.001 };
        const v1 = f32x4{ 1.0, math.inf_f32, -3.0, 4.0 };
        try expect(isEqual3(v0, v1) == true);
    }
    {
        const v0 = f32x4{ 1.0, math.inf_f32, -3.0, 4.0 };
        const v1 = f32x4{ 1.0, -math.inf_f32, -3.0, 4.0 };
        try expect(isEqual3(v0, v1) == false);
    }
}

pub inline fn isEqualInt3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vpcmpeqd     %%xmm1, %%xmm0, %xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ pcmpeqd      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v0] "{xmm0}" (v0),
              [v1] "{xmm1}" (v1),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const v0u = @bitCast(u32x4, v0);
        const v1u = @bitCast(u32x4, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn isNearEqual3(v0: f32x4, v1: f32x4, epsilon: f32x4) bool {
    // Won't handle inf & nan
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vsubps       %%xmm1, %%xmm0, %%xmm0  # xmm0 = delta
            \\ vxorps       %%xmm1, %%xmm1, %%xmm1  # xmm1 = 0
            \\ vsubps       %%xmm0, %%xmm1, %%xmm1  # xmm1 = 0 - delta
            \\ vmaxps       %%xmm1, %%xmm0, %%xmm0  # xmm0 = abs(delta)
            \\ vcmpleps     %%xmm2, %%xmm0, %%xmm0  # xmm0 = abs(delta) <= epsilon
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ subps        %%xmm1, %%xmm0          # xmm0 = delta
            \\ xorps        %%xmm1, %%xmm1          # xmm1 = 0
            \\ subps        %%xmm0, %%xmm1          # xmm1 = 0 - delta
            \\ maxps        %%xmm1, %%xmm0          # xmm0 = abs(delta)
            \\ cmpleps      %%xmm2, %%xmm0          # xmm0 = abs(delta) <= epsilon
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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
test "zmath.isNearEqual3" {
    {
        const v0 = f32x4{ 1.0, 2.0, -3.0001, 1000.001 };
        const v1 = f32x4{ 1.0, 2.001, -3.0, 4.0 };
        try expect(isNearEqual3(v0, v1, splat(f32x4, 0.01)));
    }
}

pub inline fn isLess3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpltps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ cmpltps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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
test "zmath.isLess3" {
    const v0 = f32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v1 = f32x4{ 4.0, 5.0, 6.0, 1.0 };
    try expect(isLess3(v0, v1) == true);

    const v2 = f32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v3 = f32x4{ 4.0, -5.0, 6.0, 1.0 };
    try expect(isLess3(v2, v3) == false);

    const v4 = f32x4{ 100.0, 200.0, 300.0, -500.0 };
    const v5 = f32x4{ 400.0, 500.0, 600.0, 1.0 };
    try expect(isLess3(v4, v5) == true);

    const v6 = f32x4{ 100.0, -math.inf_f32, -math.inf_f32, 50000.0 };
    const v7 = f32x4{ 400.0, math.inf_f32, 600.0, 1.0 };
    try expect(isLess3(v6, v7) == true);
}

pub inline fn isLessEqual3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpleps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ cmpleps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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

pub inline fn isGreater3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpgtps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ cmpgtps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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

pub inline fn isGreaterEqual3(v0: f32x4, v1: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpgeps     %%xmm1, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ cmpgeps      %%xmm1, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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

pub inline fn isInBounds3(v: f32x4, bounds: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vmovaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ vxorps       %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\ vcmpleps     %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\ vcmpleps     %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\ vandps       %%xmm2, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
        else
            \\ movaps       %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ xorps        %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\ cmpleps      %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\ cmpleps      %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\ andps        %%xmm2, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ and          $7, %%al
            \\ cmp          $7, %%al
            \\ sete         %%al
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
        const b0 = @select(u32, v <= bounds, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b1 = @select(u32, bounds * splat(f32x4, -1.0) <= v, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0 and b[2] > 0;

        // I've also tried this (generated asm is different but still not great):
        // const b0 = v <= bounds;
        // const b1 = bounds * vecSplat(-1.0) <= v;
        // const b = vecBoolAnd(b0, b1);
        // return b[0] and b[1] and b[2];
    }
}
test "zmath.isInBounds3" {
    {
        const v0 = f32x4{ 0.5, -2.0, -1.0, 1.0 };
        const v1 = f32x4{ -1.6, 1.1, -0.1, 2.9 };
        const v2 = f32x4{ 0.5, -2.0, -1.0, 10.0 };
        const bounds = f32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds3(v0, bounds) == true);
        try expect(isInBounds3(v1, bounds) == false);
        try expect(isInBounds3(v2, bounds) == true);
    }
    {
        const v0 = f32x4{ 10000.0, -1000.0, -1.0, 1000.0 };
        const bounds = f32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds3(v0, bounds) == true);
    }
}

pub inline fn isNan3(v: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpneqps    %%xmm0, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ test         $7, %%al
            \\ setne        %%al
        else
            \\ cmpneqps     %%xmm0, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ test         $7, %%al
            \\ setne        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b = v != v;
        return b[0] or b[1] or b[2];
    }
}
test "zmath.isNan3" {
    try expect(isNan3(f32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan3(f32x4{ -1.0, 100.0, 3.0, math.nan_f32 }) == false);
    try expect(isNan3(f32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
    try expect(isNan3(f32x4{ -1.0, math.qnan_f32, 3.0, -2.0 }) == true);
    try expect(isNan3(f32x4{ -1.0, 1.0, 3.0, -2.0 }) == false);
    try expect(isNan3(f32x4{ -1.0, 1.0, 3.0, math.qnan_f32 }) == false);
}

pub inline fn isInf3(v: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vandps       %[x7fff_ffff], %%xmm0, %%xmm0
            \\ vcmpeqps     %[inf], %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ test         $7, %%al
            \\ setne        %%al
        else
            \\ andps        %[x7fff_ffff], %%xmm0
            \\ cmpeqps      %[inf], %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ test         $7, %%al
            \\ setne        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
              [x7fff_ffff] "{memory}" (f32x4_0x7fff_ffff),
              [inf] "{memory}" (f32x4_inf),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b = vecIsInf(v);
        return b[0] or b[1] or b[2];
    }
}
test "zmath.isInf3" {
    try expect(isInf3(splat(f32x4, math.inf_f32)) == true);
    try expect(isInf3(splat(f32x4, -math.inf_f32)) == true);
    try expect(isInf3(splat(f32x4, math.nan_f32)) == false);
    try expect(isInf3(splat(f32x4, -math.nan_f32)) == false);
    try expect(isInf3(splat(f32x4, math.qnan_f32)) == false);
    try expect(isInf3(splat(f32x4, -math.qnan_f32)) == false);
    try expect(isInf3(f32x4{ 1.0, 2.0, 3.0, 4.0 }) == false);
    try expect(isInf3(f32x4{ 1.0, 2.0, 3.0, math.inf_f32 }) == false);
    try expect(isInf3(f32x4{ 1.0, 2.0, math.inf_f32, 1.0 }) == true);
    try expect(isInf3(f32x4{ -math.inf_f32, math.inf_f32, math.inf_f32, 1.0 }) == true);
    try expect(isInf3(f32x4{ -math.inf_f32, math.nan_f32, math.inf_f32, 1.0 }) == true);
}

pub inline fn dot3(v0: f32x4, v1: f32x4) f32x4 {
    var dot = v0 * v1;
    var temp = vecSwizzle(dot, .y, .z, .y, .z);
    dot = f32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    temp = vecSwizzle(temp, .y, .y, .y, .y);
    dot = f32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    return vecSwizzle(dot, .x, .x, .x, .x);
}
test "zmath.dot3" {
    const v0 = f32x4{ -1.0, 2.0, 3.0, 1.0 };
    const v1 = f32x4{ 4.0, 5.0, 6.0, 1.0 };
    var v = dot3(v0, v1);
    try expect(approxEqAbs(v, splat(f32x4, 24.0), 0.0001));
}

pub inline fn cross3(v0: f32x4, v1: f32x4) f32x4 {
    var xmm0 = vecSwizzle(v0, .y, .z, .x, .w);
    var xmm1 = vecSwizzle(v1, .z, .x, .y, .w);
    var result = xmm0 * xmm1;
    xmm0 = vecSwizzle(xmm0, .y, .z, .x, .w);
    xmm1 = vecSwizzle(xmm1, .z, .x, .y, .w);
    result = result - xmm0 * xmm1;
    return @bitCast(f32x4, @bitCast(u32x4, result) & u32x4_mask3);
}
test "zmath.cross3" {
    {
        const v0 = f32x4{ 1.0, 0.0, 0.0, 1.0 };
        const v1 = f32x4{ 0.0, 1.0, 0.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, f32x4{ 0.0, 0.0, 1.0, 0.0 }, 0.0001));
    }
    {
        const v0 = f32x4{ 1.0, 0.0, 0.0, 1.0 };
        const v1 = f32x4{ 0.0, -1.0, 0.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, f32x4{ 0.0, 0.0, -1.0, 0.0 }, 0.0001));
    }
    {
        const v0 = f32x4{ -3.0, 0, -2.0, 1.0 };
        const v1 = f32x4{ 5.0, -1.0, 2.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, f32x4{ -2.0, -4.0, 3.0, 0.0 }, 0.0001));
    }
}

// zig fmt: off
pub inline fn lengthSq3(v: Vec) Vec { return dot3(v, v); }
pub inline fn rcpLengthFast3(v: Vec) Vec { return vecRcpSqrtFast(dot3(v, v)); }
pub inline fn rcpLength3(v: Vec) Vec { return vecRcpSqrt(dot3(v, v)); }
pub inline fn length3(v: Vec) Vec { return vecSqrt(dot3(v, v)); }
pub inline fn normalizeFast3(v: Vec) Vec { return v * vecRcpSqrtFast(dot3(v, v)); }
pub inline fn normalize3(v: Vec) Vec { return v * vecRcpSqrt(dot3(v, v)); }
// zig fmt: on

test "zmath.rcpLengthFast3" {
    {
        var v = rcpLengthFast3(f32x4{ 1.0, -2.0, 3.0, 1000.0 });
        try expect(approxEqAbs(v, splat(f32x4, 1.0 / math.sqrt(14.0)), 0.001));
    }
    {
        var v = rcpLengthFast3(f32x4{ 1.0, math.nan_f32, math.inf_f32, 1000.0 });
        try expect(isNan4(v));
    }
    {
        var v = rcpLengthFast3(f32x4{ 1.0, math.inf_f32, 3.0, 1000.0 });
        try expect(approxEqAbs(v, splat(f32x4, 0.0), 0.001));
    }
    {
        var v = rcpLengthFast3(f32x4{ 3.0, 2.0, 1.0, math.nan_f32 });
        try expect(approxEqAbs(v, splat(f32x4, 1.0 / math.sqrt(14.0)), 0.001));
    }
}

test "zmath.length3" {
    {
        var v = length3(f32x4{ 1.0, -2.0, 3.0, 1000.0 });
        try expect(approxEqAbs(v, splat(f32x4, math.sqrt(14.0)), 0.001));
    }
    {
        var v = length3(f32x4{ 1.0, math.nan_f32, math.inf_f32, 1000.0 });
        try expect(isNan4(v));
    }
    {
        var v = length3(f32x4{ 1.0, math.inf_f32, 3.0, 1000.0 });
        try expect(isInf4(v));
    }
    {
        var v = length3(f32x4{ 3.0, 2.0, 1.0, math.nan_f32 });
        try expect(approxEqAbs(v, splat(f32x4, math.sqrt(14.0)), 0.001));
    }
}

test "zmath.normalizeFast3" {
    {
        const v0 = f32x4{ 1.0, -2.0, 3.0, 1000.0 };
        var v = normalizeFast3(v0);
        try expect(approxEqAbs(v, v0 * splat(f32x4, 1.0 / math.sqrt(14.0)), 0.05));
    }
    {
        try expect(isNan4(normalizeFast3(f32x4{ 1.0, math.inf_f32, 1.0, 1.0 })));
        try expect(isNan4(normalizeFast3(f32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 })));
        try expect(isNan4(normalizeFast3(f32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 })));
        try expect(isNan4(normalizeFast3(splat(f32x4, 0.0))));
    }
}

test "zmath.normalize3" {
    {
        const v0 = f32x4{ 1.0, -2.0, 3.0, 1000.0 };
        var v = normalize3(v0);
        try expect(vec4ApproxEqAbs(v, v0 * splat(f32x4, 1.0 / math.sqrt(14.0)), 0.0005));
    }
    {
        try expect(isNan4(normalize3(f32x4{ 1.0, math.inf_f32, 1.0, 1.0 })));
        try expect(isNan4(normalize3(f32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize3(f32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize3(splat(f32x4, 0.0))));
    }
}

pub inline fn linePointDistance(line_pt0: f32x4, line_pt1: f32x4, pt: f32x4) f32x4 {
    const pt_vec = pt - line_pt0;
    const line_vec = line_pt1 - line_pt0;
    const scale = dot3(pt_vec, line_vec) / lengthSq3(line_vec);
    return length3(pt_vec - line_vec * scale);
}

test "zmath.linePointDistance" {
    {
        const line_pt0 = f32x4{ -1.0, -2.0, -3.0, 1.0 };
        const line_pt1 = f32x4{ 1.0, 2.0, 3.0, 1.0 };
        const pt = f32x4{ 1.0, 1.0, 1.0, 1.0 };
        var v = linePointDistance(line_pt0, line_pt1, pt);
        try expect(approxEqAbs(v, splat(f32x4, 0.654), 0.001));
    }
}

//
// Vec4 functions
//
// isEqual4(v0: Vec, v1: Vec) bool
// isEqual4Int(v0: Vec, v1: Vec) bool
// vec4NearEqual(v0: Vec, v1: Vec, epsilon: Vec) bool
// vec4Less(v0: Vec, v1: Vec) bool
// vec4LessOrEqual(v0: Vec, v1: Vec) bool
// vec4Greater(v0: Vec, v1: Vec) bool
// vec4GreaterOrEqual(v0: Vec, v1: Vec) bool
// vec4InBounds(v: Vec, bounds: Vec) bool
// isNan4(v: Vec) bool
// isInf4(v: Vec) bool
// vec4Dot(v0: Vec, v1: Vec) Vec
// vec4LengthSq(v: Vec) Vec
// vec4RcpLengthFast(v: Vec) Vec
// vec4RcpLength(v: Vec) Vec
// vec4Length(v: Vec) Vec
// vec4NormalizeFast(v: Vec) Vec
// vec4Normalize(v: Vec) Vec

pub inline fn isEqual4(v0: f32x4, v1: f32x4) bool {
    const mask = v0 == v1;
    return @reduce(.And, mask);
}

pub inline fn isEqual4Int(v0: f32x4, v1: f32x4) bool {
    const v0u = @bitCast(u32x4, v0);
    const v1u = @bitCast(u32x4, v1);
    const mask = v0u == v1u;
    return @reduce(.And, mask);
}

pub inline fn isNearEqual4(v0: f32x4, v1: f32x4, epsilon: f32x4) bool {
    // Won't handle inf & nan
    const delta = v0 - v1;
    const temp = vecMaxFast(delta, splat(f32x4, 0.0) - delta);
    return @reduce(.And, temp <= epsilon);
}

pub inline fn isLess4(v0: f32x4, v1: f32x4) bool {
    const mask = v0 < v1;
    return @reduce(.And, mask);
}

pub inline fn isLessEqual4(v0: f32x4, v1: f32x4) bool {
    const mask = v0 <= v1;
    return @reduce(.And, mask);
}

pub inline fn isGreater4(v0: f32x4, v1: f32x4) bool {
    const mask = v0 > v1;
    return @reduce(.And, mask);
}

pub inline fn isGreaterEqual(v0: f32x4, v1: f32x4) bool {
    const mask = v0 >= v1;
    return @reduce(.And, mask);
}

pub inline fn isInBounds4(v: f32x4, bounds: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vmovaps      %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ vxorps       %[x8000_0000], %%xmm2, %%xmm2   # xmm2 = -bounds
            \\ vcmpleps     %%xmm0, %%xmm2, %%xmm2          # xmm2 = -bounds <= v
            \\ vcmpleps     %%xmm1, %%xmm0, %%xmm0          # xmm0 = v <= bounds
            \\ vandps       %%xmm2, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ cmp          $15, %%al
            \\ sete         %%al
        else
            \\ movaps       %%xmm1, %%xmm2                  # xmm2 = bounds
            \\ xorps        %[x8000_0000], %%xmm2           # xmm2 = -bounds
            \\ cmpleps      %%xmm0, %%xmm2                  # xmm2 = -bounds <= v
            \\ cmpleps      %%xmm1, %%xmm0                  # xmm0 = v <= bounds
            \\ andps        %%xmm2, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ cmp          $15, %%al
            \\ sete         %%al
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
        const b0 = @select(u32, v <= bounds, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b1 = @select(u32, bounds * splat(f32x4, -1.0) <= v, usplat(u32x4, ~@as(u32, 0)), usplat(u32x4, 0));
        const b = b0 & b1;
        return b[0] > 0 and b[1] > 0 and b[2] > 0 and b[3] > 0;
    }
}
test "zmath.isInBounds4" {
    {
        const v0 = f32x4{ 0.5, -2.0, -1.0, 1.9 };
        const v1 = f32x4{ -1.6, -2.001, -1.0, 1.9 };
        const bounds = f32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds4(v0, bounds) == true);
        try expect(isInBounds4(v1, bounds) == false);
    }
    {
        const v0 = f32x4{ 10000.0, -1000.0, -1.0, 0.0 };
        const bounds = f32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds4(v0, bounds) == true);
    }
}

pub inline fn isNan4(v: f32x4) bool {
    if (cpu_arch == .x86_64) {
        const code = if (has_avx)
            \\ vcmpneqps    %%xmm0, %%xmm0, %%xmm0
            \\ vmovmskps    %%xmm0, %%eax
            \\ test         $15, %%al
            \\ setne        %%al
        else
            \\ cmpneqps     %%xmm0, %%xmm0
            \\ movmskps     %%xmm0, %%eax
            \\ test         $15, %%al
            \\ setne        %%al
            ;
        return asm (code
            : [ret] "={rax}" (-> bool),
            : [v] "{xmm0}" (v),
        );
    } else {
        // NOTE(mziulek): Generated code is not optimal
        const b = v != v;
        return b[0] or b[1] or b[2] or b[3];
    }
}
test "zmath.isNan4" {
    try expect(isNan4(f32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan4(f32x4{ -1.0, 100.0, 3.0, -2.0 }) == false);
    try expect(isNan4(f32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
}

pub inline fn isInf4(v: f32x4) bool {
    // andps, cmpeqps, movmskps, test, setne
    const b = vecIsInf(v);
    return b[0] or b[1] or b[2] or b[3];
}

pub inline fn vec4Dot(v0: Vec, v1: Vec) Vec {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | z0*z1 | w0*w1 |
    var xmm1 = vecSwizzle(xmm0, .y, .x, .w, .x); // | y0*y1 | -- | w0*w1 | -- |
    xmm1 = xmm0 + xmm1; // | x0*x1 + y0*y1 | -- | z0*z1 + w0*w1 | -- |
    xmm0 = vecSwizzle(xmm1, .z, .x, .x, .x); // | z0*z1 + w0*w1 | -- | -- | -- |
    xmm0 = vecSet(xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[2]); // addss
    return vecSwizzle(xmm0, .x, .x, .x, .x);
}
test "zmath.vec4Dot" {
    const v0 = vecSet(-1.0, 2.0, 3.0, -2.0);
    const v1 = vecSet(4.0, 5.0, 6.0, 2.0);
    var v = vec4Dot(v0, v1);
    try expect(vec4ApproxEqAbs(v, vecSplat(20.0), 0.0001));
}

// zig fmt: off
pub inline fn vec4LengthSq(v: Vec) Vec { return vec4Dot(v, v); }
pub inline fn vec4RcpLengthFast(v: Vec) Vec { return vecRcpSqrtFast(vec4Dot(v, v)); }
pub inline fn vec4RcpLength(v: Vec) Vec { return vecRcpSqrt(vec4Dot(v, v)); }
pub inline fn vec4Length(v: Vec) Vec { return vecSqrt(vec4Dot(v, v)); }
pub inline fn vec4NormalizeFast(v: Vec) Vec { return v * vecRcpSqrtFast(vec4Dot(v, v)); }
pub inline fn vec4Normalize(v: Vec) Vec { return v * vecRcpSqrt(vec4Dot(v, v)); }
// zig fmt: on

test "zmath.vec4RcpLengthFast" {
    {
        const v0 = vecSet(1.0, -2.0, 3.0, 4.0);
        var v = vec4RcpLengthFast(v0);
        try expect(vec4ApproxEqAbs(v, vecSplat(1.0 / math.sqrt(30.0)), 0.001));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, math.inf_f32, 1000.0);
        var v = vec4RcpLengthFast(v0);
        try expect(isNan4(v));
    }
    {
        const v0 = vecSet(1.0, math.inf_f32, 3.0, 1000.0);
        var v = vec4RcpLengthFast(v0);
        try expect(vec4ApproxEqAbs(v, vecZero(), 0.001));
    }
}

test "zmath.vec4RcpLength" {
    {
        const v0 = vecSet(1.0, -2.0, 3.0, 4.0);
        var v = vec4RcpLength(v0);
        try expect(vec4ApproxEqAbs(v, vecSplat(1.0 / math.sqrt(30.0)), 0.001));
    }
    {
        const v0 = vecSet(1.0, math.nan_f32, math.inf_f32, 1000.0);
        var v = vec4RcpLength(v0);
        try expect(isNan4(v));
    }
    {
        const v0 = vecSet(1.0, math.inf_f32, 3.0, 1000.0);
        var v = vec4RcpLength(v0);
        try expect(vec4ApproxEqAbs(v, vecZero(), 0.001));
    }
}

test "zmath.vec4NormalizeFast" {
    {
        const v0 = vecSet(1.0, -2.0, 3.0, 10.0);
        var v = vec4NormalizeFast(v0);
        try expect(vec4ApproxEqAbs(v, v0 * vecSplat(1.0 / math.sqrt(114.0)), 0.001));
    }
    {
        try expect(isNan4(vec4NormalizeFast(vecSet(1.0, math.inf_f32, 1.0, 1.0))));
        try expect(isNan4(vec4NormalizeFast(vecSet(-math.inf_f32, math.inf_f32, 0.0, 0.0))));
        try expect(isNan4(vec4NormalizeFast(vecSet(-math.nan_f32, math.qnan_f32, 0.0, 0.0))));
        try expect(isNan4(vec4NormalizeFast(vecZero())));
    }
}

test "zmath.vec4Normalize" {
    {
        const v0 = vecSet(1.0, -2.0, 3.0, 10.0);
        var v = vec4Normalize(v0);
        try expect(vec4ApproxEqAbs(v, v0 * vecSplat(1.0 / math.sqrt(114.0)), 0.0005));
    }
    {
        try expect(isNan4(vec4Normalize(vecSet(1.0, math.inf_f32, 1.0, 1.0))));
        try expect(isNan4(vec4Normalize(vecSet(-math.inf_f32, math.inf_f32, 0.0, 0.0))));
        try expect(isNan4(vec4Normalize(vecSet(-math.nan_f32, math.qnan_f32, 0.0, 0.0))));
        try expect(isNan4(vec4Normalize(vecZero())));
    }
}

//
// Public constants
//

pub const u32x4_0xffff_ffff: VecU32 = @splat(4, ~@as(u32, 0));
pub const u32x4_0x8000_0000: VecU32 = @splat(4, @as(u32, 0x8000_0000));
pub const f32x4_0x8000_0000: Vec = vecSplatInt(0x8000_0000);
pub const f32x4_0x7fff_ffff: Vec = vecSplatInt(0x7fff_ffff);
pub const f32x4_inf: Vec = vecSplat(math.inf_f32);
pub const f32x4_nan: Vec = vecSplat(math.nan_f32);
pub const f32x4_qnan: Vec = vecSplat(math.qnan_f32);
pub const f32x4_epsilon: Vec = vecSplat(math.epsilon_f32);
pub const f32x4_one: Vec = vecSplat(1.0);
pub const f32x4_half_pi: Vec = vecSplat(0.5 * math.pi);
pub const f32x4_pi: Vec = vecSplat(math.pi);
pub const f32x4_two_pi: Vec = vecSplat(math.tau);
pub const f32x4_rcp_two_pi: Vec = vecSplat(1.0 / math.tau);
pub const u32x4_mask3: VecU32 = [4]u32{ 0xffff_ffff, 0xffff_ffff, 0xffff_ffff, 0 };

//
// Private functions and constants
//

const f32x4_8_388_608: Vec = vecSplat(8_388_608.0);
const f32x4_sin_c0123: Vec = vecSet(-0.16666667, 0.0083333310, -0.00019840874, 2.7525562e-06);
const f32x4_sin_c4567: Vec = vecSet(-2.3889859e-08, -0.16665852, 0.0083139502, -0.00018524670);

// zig fmt: off
pub inline fn splatSinC0(comptime T: type) T { return splat(T, -0.16666667); }
pub inline fn splatSinC1(comptime T: type) T { return splat(T, 0.0083333310); }
pub inline fn splatSinC2(comptime T: type) T { return splat(T, -0.00019840874); }
pub inline fn splatSinC3(comptime T: type) T { return splat(T, 2.7525562e-06); }
pub inline fn splatSinC4(comptime T: type) T { return splat(T, -2.3889859e-08); }
// zig fmt: on

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
inline fn floatToIntAndBack(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    // This won't handle nan, inf and numbers greater than 8_388_608.0
    @setRuntimeSafety(false);
    // TODO(mziulek): use inline while or something..
    if (T == f32x4) {
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
    } else if (T == f32x8) {
        // cvttps2dq
        const vi = [8]i32{
            @floatToInt(i32, v[0]),
            @floatToInt(i32, v[1]),
            @floatToInt(i32, v[2]),
            @floatToInt(i32, v[3]),
            @floatToInt(i32, v[4]),
            @floatToInt(i32, v[5]),
            @floatToInt(i32, v[6]),
            @floatToInt(i32, v[7]),
        };
        // cvtdq2ps
        return [8]f32{
            @intToFloat(f32, vi[0]),
            @intToFloat(f32, vi[1]),
            @intToFloat(f32, vi[2]),
            @intToFloat(f32, vi[3]),
            @intToFloat(f32, vi[4]),
            @intToFloat(f32, vi[5]),
            @intToFloat(f32, vi[6]),
            @intToFloat(f32, vi[7]),
        };
    }
}
test "zmath.vecFloatToIntAndBack" {
    const v0 = vecSet(1.1, 2.9, 3.0, -4.5);
    var v = vecFloatToIntAndBack(v0);
    try expect(v[0] == 1.0);
    try expect(v[1] == 2.0);
    try expect(v[2] == 3.0);
    try expect(v[3] == -4.0);

    const v1 = vecSet(math.inf_f32, 2.9, math.nan_f32, math.qnan_f32);
    v = vecFloatToIntAndBack(v1);
    try expect(v[1] == 2.0);
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

inline fn approxEqAbs(v0: anytype, v1: anytype, eps: f32) bool {
    const T = @TypeOf(v0);
    comptime var i: comptime_int = 0;
    inline while (i < @typeInfo(T).Vector.len) : (i += 1) {
        if (!math.approxEqAbs(f32, v0[i], v1[i], eps))
            return false;
    }
    return true;
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
