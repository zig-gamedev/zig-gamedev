pub const F32x4 = @Vector(4, f32);
pub const F32x8 = @Vector(8, f32);
pub const U32x4 = @Vector(4, u32);
pub const U32x8 = @Vector(8, u32);
pub const Boolx4 = @Vector(4, bool);
pub const Boolx8 = @Vector(8, bool);
pub const Mat = [4]F32x4;

// ------------------------------------------------------------------------------
// 1. Initialization functions
// ------------------------------------------------------------------------------
//
// f32x4(e0: f32, e1: f32, e2: f32, e3: f32) F32x4
// f32x8(e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32) F32x8
// u32x4(e0: u32, e1: u32, e2: u32, e3: u32) U32x4
// u32x8(e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32) U32x8
// boolx4(e0: bool, e1: bool, e2: bool, e3: bool) Boolx4
// boolx8(e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool) Boolx8
//
// loadF32x4(mem: []const f32, comptime len: u32) F32x4
// loadF32x8(mem: []const f32, comptime len: u32) F32x4
// storeF32x4(mem: []f32, v: F32x4, comptime len: u32) void
// storeF32x8(mem: []f32, v: F32x4, comptime len: u32) void
//
// splat(comptime T: type, value: f32) T
// splatInt(comptime T: type, value: u32) T
// usplat(comptime T: type, value: u32) T
//
// ------------------------------------------------------------------------------
// 2. Functions that work on all vector components (F32xN = F32x4 | F32x8)
// ------------------------------------------------------------------------------
//
// isNearEqual(v0: F32xN, v1: F32xN, epsilon: F32xN) BoolxN
// isEqualInt(v0: F32xN, v1: F32xN, epsilon: F32xN) BoolxN
// isNotEqualInt(v0: F32xN, v1: F32xN, epsilon: F32xN) BoolxN
// isNan(v: F32xN) BoolxN
// isInf(v: F32xN) BoolxN
// isInBounds(v: F32xN, bounds: F32xN) BoolxN
//
// andInt(v0: F32xN, v1: F32xN) F32xN
// andNotInt(v0: F32xN, v1: F32xN) F32xN
// orInt(v0: F32xN, v1: F32xN) F32xN
// norInt(v0: F32xN, v1: F32xN) F32xN
// xorInt(v0: F32xN, v1: F32xN) F32xN
//
// minFast(v0: F32xN, v1: F32xN) F32xN
// maxFast(v0: F32xN, v1: F32xN) F32xN
// min(v0: F32xN, v1: F32xN) F32xN
// max(v0: F32xN, v1: F32xN) F32xN
// round(v: F32xN) F32xN
// floor(v: F32xN) F32xN
// trunc(v: F32xN) F32xN
// ceil(v: F32xN) F32xN
// clamp(v0: F32xN, v1: F32xN) F32xN
// clampFast(v0: F32xN, v1: F32xN) F32xN
// saturate(v: F32xN) F32xN
// saturateFast(v: F32xN) F32xN
// lerp(v0: F32xN, v1: F32xN, t: f32) F32xN
// lerpV(v0: F32xN, v1: F32xN, t: F32xN) F32xN
// sqrt(v: F32xN) F32xN
// abs(v: F32xN) F32xN
// mod(v0: F32xN, v1: F32xN) F32xN
// modAngles(v: F32xN) F32xN
// mulAdd(v0: F32xN, v1: F32xN, v2: F32xN) F32xN
// sin(v: F32xN) F32xN
// cos(v: F32xN) F32xN
// sincos(v: F32xN) [2]F32xN
// select(mask: BoolxN, v0: F32xN, v1: F32xN)
//
// ------------------------------------------------------------------------------
// 3. Functions that process N components of F32x4 (N = function name suffix)
// ------------------------------------------------------------------------------
//
// swizzle4(v: F32x4, c, c, c, c) F32x4 (c = .x | .y | .z | .w)
// isEqual2(v0: F32x4, v1: F32x4) bool
// isEqual3(v0: F32x4, v1: F32x4) bool
// isEqual4(v0: F32x4, v1: F32x4) bool
// isEqualInt2(v0: F32x4, v1: F32x4) bool
// isEqualInt3(v0: F32x4, v1: F32x4) bool
// isEqualInt4(v0: F32x4, v1: F32x4) bool
// isNearEqual2(v0: F32x4, v1: F32x4, epsilon: F32x4) bool
// isNearEqual3(v0: F32x4, v1: F32x4, epsilon: F32x4) bool
// isNearEqual4(v0: F32x4, v1: F32x4, epsilon: F32x4) bool
// isLess2(v0: F32x4, v1: F32x4) bool
// isLess3(v0: F32x4, v1: F32x4) bool
// isLess4(v0: F32x4, v1: F32x4) bool
// isLessEqual2(v0: F32x4, v1: F32x4) bool
// isLessEqual3(v0: F32x4, v1: F32x4) bool
// isLessEqual4(v0: F32x4, v1: F32x4) bool
// isGreater2(v0: F32x4, v1: F32x4) bool
// isGreater3(v0: F32x4, v1: F32x4) bool
// isGreater4(v0: F32x4, v1: F32x4) bool
// isGreaterEqual2(v0: F32x4, v1: F32x4) bool
// isGreaterEqual3(v0: F32x4, v1: F32x4) bool
// isGreaterEqual4(v0: F32x4, v1: F32x4) bool
// isInBounds2(v: F32x4, bounds: F32x4) bool
// isInBounds3(v: F32x4, bounds: F32x4) bool
// isInBounds4(v: F32x4, bounds: F32x4) bool
// isNan2(v: F32x4) bool
// isNan3(v: F32x4) bool
// isNan4(v: F32x4) bool
// isInf2(v: F32x4) bool
// isInf3(v: F32x4) bool
// isInf4(v: F32x4) bool
// dot2(v0: F32x4, v1: F32x4) F32x4
// dot3(v0: F32x4, v1: F32x4) F32x4
// dot4(v0: F32x4, v1: F32x4) F32x4
// lengthSq2(v: F32x4) F32x4
// lengthSq3(v: F32x4) F32x4
// lengthSq4(v: F32x4) F32x4
// length2(v: F32x4) F32x4
// length3(v: F32x4) F32x4
// length4(v: F32x4) F32x4
// normalize2(v: F32x4) F32x4
// normalize3(v: F32x4) F32x4
// normalize4(v: F32x4) F32x4
//
// linePointDistance(line_pt0: F32x4, line_pt1: F32x4, pt: F32x4) F32x4

const builtin = @import("builtin");
const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const expect = std.testing.expect;

const U1x4 = @Vector(4, u1);

const cpu_arch = builtin.cpu.arch;
const has_avx = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .avx) else false;
const has_fma = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .fma) else false;

pub inline fn f32x4(e0: f32, e1: f32, e2: f32, e3: f32) F32x4 {
    return .{ e0, e1, e2, e3 };
}
pub inline fn u32x4(e0: u32, e1: u32, e2: u32, e3: u32) U32x4 {
    return .{ e0, e1, e2, e3 };
}
pub inline fn boolx4(e0: bool, e1: bool, e2: bool, e3: bool) Boolx4 {
    return .{ e0, e1, e2, e3 };
}

pub inline fn f32x8(e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32) F32x8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}
pub inline fn u32x8(e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32) U32x8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}
pub inline fn boolx8(e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool) Boolx8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}

pub inline fn splat(comptime T: type, value: f32) T {
    return @splat(@typeInfo(T).Vector.len, value);
}
pub inline fn splatInt(comptime T: type, value: u32) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, value));
}
pub inline fn usplat(comptime T: type, value: u32) T {
    return @splat(@typeInfo(T).Vector.len, value);
}

pub inline fn isNearEqual(
    v0: anytype,
    v1: anytype,
    epsilon: anytype,
) @Vector(@typeInfo(@TypeOf(v0)).Vector.len, bool) {
    // Won't handle inf & nan
    const T = @TypeOf(v0);
    const delta = v0 - v1;
    const temp = maxFast(delta, splat(T, 0.0) - delta);
    return temp <= epsilon;
}
test "zmath.isNearEqual" {
    {
        const v0 = f32x4(1.0, 2.0, -3.0, 4.001);
        const v1 = f32x4(1.0, 2.1, 3.0, 4.0);
        const b = isNearEqual(v0, v1, splat(F32x4, 0.01));
        try expect(@reduce(.And, b == Boolx4{ true, false, false, true }));
    }
    {
        const v0 = f32x8(1.0, 2.0, -3.0, 4.001, 1.001, 2.3, -0.0, 0.0);
        const v1 = f32x8(1.0, 2.1, 3.0, 4.0, -1.001, 2.1, 0.0, 0.0);
        const b = isNearEqual(v0, v1, splat(F32x8, 0.01));
        try expect(@reduce(.And, b == Boolx8{ true, false, false, true, false, false, true, true }));
    }
}

pub inline fn isEqualInt(
    v0: anytype,
    v1: anytype,
) @Vector(@typeInfo(@TypeOf(v0)).Vector.len, bool) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return v0u == v1u; // pcmpeqd
}
test "zmath.isEqualInt" {
    {
        const v0 = f32x4(1.0, -0.0, 3.0, 4.001);
        const v1 = f32x4(1.0, 0.0, -3.0, 4.0);
        const b0 = isEqualInt(v0, v1);
        const b1 = v0 == v1;
        try expect(@reduce(.And, b0 == boolx4(true, false, false, false)));
        try expect(@reduce(.And, b1 == boolx4(true, true, false, false)));
    }
    {
        const v0 = f32x8(1.0, 2.0, -3.0, 4.001, 1.001, 2.3, -0.0, 0.0);
        const v1 = f32x8(1.0, 2.1, 3.0, 4.0, -1.001, 2.1, 0.0, 0.0);
        const b0 = isEqualInt(v0, v1);
        const b1 = v0 == v1;
        try expect(@reduce(.And, b0 == boolx8(true, false, false, false, false, false, false, true)));
        try expect(@reduce(.And, b1 == boolx8(true, false, false, false, false, false, true, true)));
    }
}

pub inline fn isNotEqualInt(
    v0: anytype,
    v1: anytype,
) @Vector(@typeInfo(@TypeOf(v0)).Vector.len, bool) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return v0u != v1u; // 2 x pcmpeqd, pxor
}
test "zmath.isNotEqualInt" {
    {
        const v0 = f32x4(1.0, -0.0, 3.0, 4.001);
        const v1 = f32x4(1.0, 0.0, -3.0, 4.0);
        const b0 = isNotEqualInt(v0, v1);
        const b1 = v0 != v1;
        try expect(@reduce(.And, b0 == boolx4(false, true, true, true)));
        try expect(@reduce(.And, b1 == boolx4(false, false, true, true)));
    }
    {
        const v0 = f32x8(1.0, 2.0, -3.0, 4.001, 1.001, 2.3, -0.0, 0.0);
        const v1 = f32x8(1.0, 2.1, 3.0, 4.0, -1.001, 2.1, 0.0, 0.0);
        const b0 = isNotEqualInt(v0, v1);
        const b1 = v0 != v1;
        try expect(@reduce(.And, b0 == boolx8(false, true, true, true, true, true, true, false)));
        try expect(@reduce(.And, b1 == boolx8(false, true, true, true, true, true, false, false)));
    }
}

pub inline fn andInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, v0u & v1u); // andps
}
test "zmath.andInt" {
    {
        const v0 = f32x4(0, @bitCast(f32, ~@as(u32, 0)), 0, @bitCast(f32, ~@as(u32, 0)));
        const v1 = f32x4(1.0, 2.0, 3.0, math.inf_f32);
        const v = andInt(v0, v1);
        try expect(v[3] == math.inf_f32);
        try expect(approxEqAbs(v, f32x4(0.0, 2.0, 0.0, math.inf_f32), 0.0));
    }
    {
        const v0 = f32x8(0, 0, 0, 0, 0, @bitCast(f32, ~@as(u32, 0)), 0, @bitCast(f32, ~@as(u32, 0)));
        const v1 = f32x8(0, 0, 0, 0, 1.0, 2.0, 3.0, math.inf_f32);
        const v = andInt(v0, v1);
        try expect(v[7] == math.inf_f32);
        try expect(approxEqAbs(v, f32x8(0, 0, 0, 0, 0.0, 2.0, 0.0, math.inf_f32), 0.0));
    }
}

pub inline fn andNotInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, ~v0u & v1u); // andnps
}
test "zmath.andNotInt" {
    {
        const v0 = F32x4{ 1.0, 2.0, 3.0, 4.0 };
        const v1 = F32x4{ 0, @bitCast(f32, ~@as(u32, 0)), 0, @bitCast(f32, ~@as(u32, 0)) };
        const v = andNotInt(v1, v0);
        try expect(approxEqAbs(v, F32x4{ 1.0, 0.0, 3.0, 0.0 }, 0.0));
    }
    {
        const v0 = F32x8{ 0, 0, 0, 0, 1.0, 2.0, 3.0, 4.0 };
        const v1 = F32x8{ 0, 0, 0, 0, 0, @bitCast(f32, ~@as(u32, 0)), 0, @bitCast(f32, ~@as(u32, 0)) };
        const v = andNotInt(v1, v0);
        try expect(approxEqAbs(v, F32x8{ 0, 0, 0, 0, 1.0, 0.0, 3.0, 0.0 }, 0.0));
    }
}

pub inline fn orInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, v0u | v1u); // orps
}
test "zmath.orInt" {
    {
        const v0 = F32x4{ 0, @bitCast(f32, ~@as(u32, 0)), 0, 0 };
        const v1 = F32x4{ 1.0, 2.0, 3.0, 4.0 };
        const v = orInt(v0, v1);
        try expect(v[0] == 1.0);
        try expect(@bitCast(u32, v[1]) == ~@as(u32, 0));
        try expect(v[2] == 3.0);
        try expect(v[3] == 4.0);
    }
    {
        const v0 = F32x8{ 0, 0, 0, 0, 0, @bitCast(f32, ~@as(u32, 0)), 0, 0 };
        const v1 = F32x8{ 0, 0, 0, 0, 1.0, 2.0, 3.0, 4.0 };
        const v = orInt(v0, v1);
        try expect(v[4] == 1.0);
        try expect(@bitCast(u32, v[5]) == ~@as(u32, 0));
        try expect(v[6] == 3.0);
        try expect(v[7] == 4.0);
    }
}

pub inline fn norInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, ~(v0u | v1u)); // por, pcmpeqd, pxor
}

pub inline fn xorInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(@typeInfo(T).Vector.len, u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, v0u ^ v1u); // xorps
}
test "zmath.xorInt" {
    {
        const v0 = F32x4{ 1.0, @bitCast(f32, ~@as(u32, 0)), 0, 0 };
        const v1 = F32x4{ 1.0, 0, 0, 0 };
        const v = xorInt(v0, v1);
        try expect(v[0] == 0.0);
        try expect(@bitCast(u32, v[1]) == ~@as(u32, 0));
        try expect(v[2] == 0.0);
        try expect(v[3] == 0.0);
    }
    {
        const v0 = F32x8{ 0, 0, 0, 0, 1.0, @bitCast(f32, ~@as(u32, 0)), 0, 0 };
        const v1 = F32x8{ 0, 0, 0, 0, 1.0, 0, 0, 0 };
        const v = xorInt(v0, v1);
        try expect(v[4] == 0.0);
        try expect(@bitCast(u32, v[5]) == ~@as(u32, 0));
        try expect(v[6] == 0.0);
        try expect(v[7] == 0.0);
    }
}

pub inline fn isNan(
    v: anytype,
) @Vector(@typeInfo(@TypeOf(v)).Vector.len, bool) {
    return v != v;
}
test "zmath.isNan" {
    {
        const v0 = F32x4{ math.inf_f32, math.nan_f32, math.qnan_f32, 7.0 };
        const b = isNan(v0);
        try expect(@reduce(.And, b == Boolx4{ false, true, true, false }));
    }
    {
        const v0 = F32x8{ 0, math.nan_f32, 0, 0, math.inf_f32, math.nan_f32, math.qnan_f32, 7.0 };
        const b = isNan(v0);
        try expect(@reduce(.And, b == Boolx8{ false, true, false, false, false, true, true, false }));
    }
}

pub inline fn isInf(
    v: anytype,
) @Vector(@typeInfo(@TypeOf(v)).Vector.len, bool) {
    const T = @TypeOf(v);
    return abs(v) == splat(T, math.inf_f32);
}
test "zmath.isInf" {
    {
        const v0 = f32x4(math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
        const b = isInf(v0);
        try expect(@reduce(.And, b == boolx4(true, false, false, false)));
    }
    {
        const v0 = f32x8(0, math.inf_f32, 0, 0, math.inf_f32, math.nan_f32, math.qnan_f32, 7.0);
        const b = isInf(v0);
        try expect(@reduce(.And, b == boolx8(false, true, false, false, true, false, false, false)));
    }
}

pub inline fn minFast(v0: anytype, v1: anytype) @TypeOf(v0) {
    return select(v0 < v1, v0, v1); // minps
}
test "zmath.minFast" {
    {
        const v0 = f32x4(1.0, 3.0, 2.0, 7.0);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = minFast(v0, v1);
        try expect(approxEqAbs(v, f32x4(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = f32x4(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = minFast(v0, v1);
        try expect(v[0] == 1.0);
        try expect(v[1] == 1.0);
        try expect(!math.isNan(v[1]));
        try expect(v[2] == 4.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
}

pub inline fn maxFast(v0: anytype, v1: anytype) @TypeOf(v0) {
    return select(v0 > v1, v0, v1); // maxps
}
test "zmath.maxFast" {
    {
        const v0 = f32x4(1.0, 3.0, 2.0, 7.0);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = maxFast(v0, v1);
        try expect(approxEqAbs(v, f32x4(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = f32x4(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = maxFast(v0, v1);
        try expect(v[0] == 2.0);
        try expect(v[1] == 1.0);
        try expect(v[2] == 5.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
}

pub inline fn min(v0: anytype, v1: anytype) @TypeOf(v0) {
    // This will handle inf & nan
    return @minimum(v0, v1); // minps, cmpunordps, andps, andnps, orps
}
test "zmath.min" {
    {
        const v0 = f32x4(1.0, 3.0, 2.0, 7.0);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = min(v0, v1);
        try expect(approxEqAbs(v, f32x4(1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = f32x8(0, 0, -2.0, 0, 1.0, 3.0, 2.0, 7.0);
        const v1 = f32x8(0, 1.0, 0, 0, 2.0, 1.0, 4.0, math.inf_f32);
        const v = min(v0, v1);
        try expect(approxEqAbs(v, f32x8(0.0, 0.0, -2.0, 0.0, 1.0, 1.0, 2.0, 7.0), 0.0));
    }
    {
        const v0 = f32x4(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = min(v0, v1);
        try expect(v[0] == 1.0);
        try expect(v[1] == 1.0);
        try expect(!math.isNan(v[1]));
        try expect(v[2] == 4.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
    {
        const v0 = f32x4(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = f32x4(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = min(v0, v1);
        try expect(v[0] == -math.inf_f32);
        try expect(v[1] == -math.inf_f32);
        try expect(v[2] == math.inf_f32);
        try expect(!math.isNan(v[2]));
        try expect(math.isNan(v[3]));
        try expect(!math.isInf(v[3]));
    }
}

pub inline fn max(v0: anytype, v1: anytype) @TypeOf(v0) {
    // This will handle inf & nan
    return @maximum(v0, v1); // maxps, cmpunordps, andps, andnps, orps
}
test "zmath.max" {
    {
        const v0 = f32x4(1.0, 3.0, 2.0, 7.0);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = max(v0, v1);
        try expect(approxEqAbs(v, f32x4(2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = f32x8(0, 0, -2.0, 0, 1.0, 3.0, 2.0, 7.0);
        const v1 = f32x8(0, 1.0, 0, 0, 2.0, 1.0, 4.0, math.inf_f32);
        const v = max(v0, v1);
        try expect(approxEqAbs(v, f32x8(0.0, 1.0, 0.0, 0.0, 2.0, 3.0, 4.0, math.inf_f32), 0.0));
    }
    {
        const v0 = f32x4(1.0, math.nan_f32, 5.0, math.qnan_f32);
        const v1 = f32x4(2.0, 1.0, 4.0, math.inf_f32);
        const v = max(v0, v1);
        try expect(v[0] == 2.0);
        try expect(v[1] == 1.0);
        try expect(v[2] == 5.0);
        try expect(v[3] == math.inf_f32);
        try expect(!math.isNan(v[3]));
    }
    {
        const v0 = f32x4(-math.inf_f32, math.inf_f32, math.inf_f32, math.qnan_f32);
        const v1 = f32x4(math.qnan_f32, -math.inf_f32, math.qnan_f32, math.nan_f32);
        const v = max(v0, v1);
        try expect(v[0] == -math.inf_f32);
        try expect(v[1] == math.inf_f32);
        try expect(v[2] == math.inf_f32);
        try expect(!math.isNan(v[2]));
        try expect(math.isNan(v[3]));
        try expect(!math.isInf(v[3]));
    }
}

pub inline fn isInBounds(
    v: anytype,
    bounds: anytype,
) @Vector(@typeInfo(@TypeOf(v)).Vector.len, bool) {
    const T = @TypeOf(v);
    const Tu = @Vector(@typeInfo(T).Vector.len, u1);
    const Tr = @Vector(@typeInfo(T).Vector.len, bool);

    // 2 x cmpleps, xorps, load, andps
    const b0 = v <= bounds;
    const b1 = (bounds * splat(T, -1.0)) <= v;
    const b0u = @bitCast(Tu, b0);
    const b1u = @bitCast(Tu, b1);
    return @bitCast(Tr, b0u & b1u);
}
test "zmath.isInBounds" {
    {
        const v0 = f32x4(0.5, -2.0, -1.0, 1.9);
        const v1 = f32x4(-1.6, -2.001, -1.0, 1.9);
        const bounds = f32x4(1.0, 2.0, 1.0, 2.0);
        const b0 = isInBounds(v0, bounds);
        const b1 = isInBounds(v1, bounds);
        try expect(@reduce(.And, b0 == boolx4(true, true, true, true)));
        try expect(@reduce(.And, b1 == boolx4(false, false, true, true)));
    }
    {
        const v0 = f32x8(2.0, 1.0, 2.0, 1.0, 0.5, -2.0, -1.0, 1.9);
        const bounds = f32x8(1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 2.0);
        const b0 = isInBounds(v0, bounds);
        try expect(@reduce(.And, b0 == boolx8(false, true, false, true, true, true, true, true)));
    }
}

test "zmath.round" {
    {
        try expect(isEqual4(round(splat(F32x4, math.inf_f32)), splat(F32x4, math.inf_f32)));
        try expect(isEqual4(round(splat(F32x4, -math.inf_f32)), splat(F32x4, -math.inf_f32)));
        try expect(isNan4(round(splat(F32x4, math.nan_f32))));
        try expect(isNan4(round(splat(F32x4, -math.nan_f32))));
        try expect(isNan4(round(splat(F32x4, math.qnan_f32))));
        try expect(isNan4(round(splat(F32x4, -math.qnan_f32))));
    }
    var v = round(F32x4{ 1.1, -1.1, -1.5, 1.5 });
    try expect(approxEqAbs(v, F32x4{ 1.0, -1.0, -2.0, 2.0 }, 0.0));

    const v1 = F32x4{ -10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32 };
    v = round(v1);
    try expect(v[3] == math.inf_f32);
    try expect(approxEqAbs(v, F32x4{ -10_000_000.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    const v2 = F32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 };
    v = round(v2);
    try expect(math.isNan(v2[0]));
    try expect(math.isNan(v2[1]));
    try expect(math.isNan(v2[2]));
    try expect(v2[3] == -math.inf_f32);

    const v3 = F32x4{ 1001.5, -201.499, -10000.99, -101.5 };
    v = round(v3);
    try expect(approxEqAbs(v, F32x4{ 1002.0, -201.0, -10001.0, -102.0 }, 0.0));

    const v4 = F32x4{ -1_388_609.9, 1_388_609.5, 1_388_109.01, 2_388_609.5 };
    v = round(v4);
    try expect(approxEqAbs(v, F32x4{ -1_388_610.0, 1_388_610.0, 1_388_109.0, 2_388_610.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = round(splat(F32x4, f));
        const fr = @round(splat(F32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn trunc(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == F32x4) {
            return asm ("vroundps $3, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == F32x8) {
            return asm ("vroundps $3, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = abs(v) < splatNoFraction(T);
        const result = floatToIntAndBack(v);
        return select(mask, result, v);
    }
}
test "zmath.trunc" {
    {
        try expect(isEqual4(trunc(splat(F32x4, math.inf_f32)), splat(F32x4, math.inf_f32)));
        try expect(isEqual4(trunc(splat(F32x4, -math.inf_f32)), splat(F32x4, -math.inf_f32)));
        try expect(isNan4(trunc(splat(F32x4, math.nan_f32))));
        try expect(isNan4(trunc(splat(F32x4, -math.nan_f32))));
        try expect(isNan4(trunc(splat(F32x4, math.qnan_f32))));
        try expect(isNan4(trunc(splat(F32x4, -math.qnan_f32))));
    }
    var v = trunc(F32x4{ 1.1, -1.1, -1.5, 1.5 });
    try expect(approxEqAbs(v, F32x4{ 1.0, -1.0, -1.0, 1.0 }, 0.0));

    v = trunc(F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = trunc(F32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = trunc(F32x4{ 1000.5001, -201.499, -10000.99, 100.750001 });
    try expect(approxEqAbs(v, F32x4{ 1000.0, -201.0, -10000.0, 100.0 }, 0.0));

    v = trunc(F32x4{ -7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5 });
    try expect(approxEqAbs(v, F32x4{ -7_388_609.0, 7_388_609.0, 8_388_109.0, -8_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = trunc(splat(F32x4, f));
        const fr = @trunc(splat(F32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn floor(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == F32x4) {
            return asm ("vroundps $1, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == F32x8) {
            return asm ("vroundps $1, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = abs(v) < splatNoFraction(T);
        var result = floatToIntAndBack(v);
        const larger_mask = result > v;
        const larger = select(larger_mask, splat(T, -1.0), splat(T, 0.0));
        result = result + larger;
        return select(mask, result, v);
    }
}
test "zmath.floor" {
    {
        try expect(isEqual4(floor(splat(F32x4, math.inf_f32)), splat(F32x4, math.inf_f32)));
        try expect(isEqual4(floor(splat(F32x4, -math.inf_f32)), splat(F32x4, -math.inf_f32)));
        try expect(isNan4(floor(splat(F32x4, math.nan_f32))));
        try expect(isNan4(floor(splat(F32x4, -math.nan_f32))));
        try expect(isNan4(floor(splat(F32x4, math.qnan_f32))));
        try expect(isNan4(floor(splat(F32x4, -math.qnan_f32))));
    }
    var v = floor(F32x4{ 1.5, -1.5, -1.7, -2.1 });
    try expect(approxEqAbs(v, F32x4{ 1.0, -2.0, -2.0, -3.0 }, 0.0));

    v = floor(F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = floor(F32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = floor(F32x4{ 1000.5001, -201.499, -10000.99, 100.75001 });
    try expect(approxEqAbs(v, F32x4{ 1000.0, -202.0, -10001.0, 100.0 }, 0.0));

    v = floor(F32x4{ -7_388_609.5, 7_388_609.1, 8_388_109.5, -8_388_509.5 });
    try expect(approxEqAbs(v, F32x4{ -7_388_610.0, 7_388_609.0, 8_388_109.0, -8_388_510.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = floor(splat(F32x4, f));
        const fr = @floor(splat(F32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn ceil(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == F32x4) {
            return asm ("vroundps $2, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == F32x8) {
            return asm ("vroundps $2, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> T),
                : [v] "{ymm0}" (v),
            );
        }
    } else {
        const mask = abs(v) < splatNoFraction(T);
        var result = floatToIntAndBack(v);
        const smaller_mask = result < v;
        const smaller = select(smaller_mask, splat(T, -1.0), splat(T, 0.0));
        result = result - smaller;
        return select(mask, result, v);
    }
}
test "zmath.ceil" {
    {
        try expect(isEqual4(ceil(splat(F32x4, math.inf_f32)), splat(F32x4, math.inf_f32)));
        try expect(isEqual4(ceil(splat(F32x4, -math.inf_f32)), splat(F32x4, -math.inf_f32)));
        try expect(isNan4(ceil(splat(F32x4, math.nan_f32))));
        try expect(isNan4(ceil(splat(F32x4, -math.nan_f32))));
        try expect(isNan4(ceil(splat(F32x4, math.qnan_f32))));
        try expect(isNan4(ceil(splat(F32x4, -math.qnan_f32))));
    }
    var v = ceil(F32x4{ 1.5, -1.5, -1.7, -2.1 });
    try expect(approxEqAbs(v, F32x4{ 2.0, -1.0, -1.0, -2.0 }, 0.0));

    v = ceil(F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 });
    try expect(approxEqAbs(v, F32x4{ -10_000_002.1, -math.inf_f32, 10_000_001.5, math.inf_f32 }, 0.0));

    v = ceil(F32x4{ -math.qnan_f32, math.qnan_f32, math.nan_f32, -math.inf_f32 });
    try expect(math.isNan(v[0]));
    try expect(math.isNan(v[1]));
    try expect(math.isNan(v[2]));
    try expect(v[3] == -math.inf_f32);

    v = ceil(F32x4{ 1000.5001, -201.499, -10000.99, 100.75001 });
    try expect(approxEqAbs(v, F32x4{ 1001.0, -201.0, -10000.0, 101.0 }, 0.0));

    v = ceil(F32x4{ -1_388_609.5, 1_388_609.1, 1_388_109.9, -1_388_509.9 });
    try expect(approxEqAbs(v, F32x4{ -1_388_609.0, 1_388_610.0, 1_388_110.0, -1_388_509.0 }, 0.0));

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = ceil(splat(F32x4, f));
        const fr = @ceil(splat(F32x4, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub inline fn clamp(v: anytype, vmin: anytype, vmax: anytype) @TypeOf(v) {
    var result = max(vmin, v);
    result = min(vmax, result);
    return result;
}
test "zmath.clamp" {
    {
        const v0 = f32x4(-1.0, 0.2, 1.1, -0.3);
        const v = clamp(v0, splat(F32x4, -0.5), splat(F32x4, 0.5));
        try expect(approxEqAbs(v, f32x4(-0.5, 0.2, 0.5, -0.3), 0.0001));
    }
    {
        const v0 = f32x8(-2.0, 0.25, -0.25, 100.0, -1.0, 0.2, 1.1, -0.3);
        const v = clamp(v0, splat(F32x8, -0.5), splat(F32x8, 0.5));
        try expect(approxEqAbs(v, f32x8(-0.5, 0.25, -0.25, 0.5, -0.5, 0.2, 0.5, -0.3), 0.0001));
    }
    {
        const v0 = f32x4(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = clamp(v0, f32x4(-100.0, 0.0, -100.0, 0.0), f32x4(0.0, 100.0, 0.0, 100.0));
        try expect(approxEqAbs(v, f32x4(-100.0, 100.0, -100.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x4(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = clamp(v0, splat(F32x4, -1.0), splat(F32x4, 1.0));
        try expect(approxEqAbs(v, f32x4(1.0, 1.0, -1.0, -1.0), 0.0001));
    }
}

pub inline fn clampFast(v: anytype, vmin: anytype, vmax: anytype) @TypeOf(v) {
    var result = maxFast(vmin, v);
    result = minFast(vmax, result);
    return result;
}
test "zmath.clampFast" {
    {
        const v0 = f32x4(-1.0, 0.2, 1.1, -0.3);
        const v = clampFast(v0, splat(F32x4, -0.5), splat(F32x4, 0.5));
        try expect(approxEqAbs(v, f32x4(-0.5, 0.2, 0.5, -0.3), 0.0001));
    }
}

pub inline fn saturate(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    var result = max(v, splat(T, 0.0));
    result = min(result, splat(T, 1.0));
    return result;
}
test "zmath.saturate" {
    {
        const v0 = f32x4(-1.0, 0.2, 1.1, -0.3);
        const v = saturate(v0);
        try expect(approxEqAbs(v, f32x4(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x8(0.0, 0.0, 2.0, -2.0, -1.0, 0.2, 1.1, -0.3);
        const v = saturate(v0);
        try expect(approxEqAbs(v, f32x8(0.0, 0.0, 1.0, 0.0, 0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x4(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = saturate(v0);
        try expect(approxEqAbs(v, f32x4(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x4(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = saturate(v0);
        try expect(approxEqAbs(v, f32x4(1.0, 1.0, 0.0, 0.0), 0.0001));
    }
}

pub inline fn saturateFast(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    var result = maxFast(v, splat(T, 0.0));
    result = minFast(result, splat(T, 1.0));
    return result;
}
test "zmath.saturateFast" {
    {
        const v0 = f32x4(-1.0, 0.2, 1.1, -0.3);
        const v = saturateFast(v0);
        try expect(approxEqAbs(v, f32x4(0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x8(0.0, 0.0, 2.0, -2.0, -1.0, 0.2, 1.1, -0.3);
        const v = saturateFast(v0);
        try expect(approxEqAbs(v, f32x8(0.0, 0.0, 1.0, 0.0, 0.0, 0.2, 1.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x4(-math.inf_f32, math.inf_f32, math.nan_f32, math.qnan_f32);
        const v = saturateFast(v0);
        try expect(approxEqAbs(v, f32x4(0.0, 1.0, 0.0, 0.0), 0.0001));
    }
    {
        const v0 = f32x4(math.inf_f32, math.inf_f32, -math.nan_f32, -math.qnan_f32);
        const v = saturateFast(v0);
        try expect(approxEqAbs(v, f32x4(1.0, 1.0, 0.0, 0.0), 0.0001));
    }
}

pub inline fn sqrt(v: anytype) @TypeOf(v) {
    return @sqrt(v); // sqrtps
}

pub inline fn abs(v: anytype) @TypeOf(v) {
    return @fabs(v); // load, andps
}

pub inline fn select(mask: anytype, v0: anytype, v1: anytype) @TypeOf(v0) {
    return @select(f32, mask, v0, v1);
}

pub inline fn lerp(v0: anytype, v1: anytype, t: f32) @TypeOf(v0) {
    const T = @TypeOf(v0);
    return v0 + (v1 - v0) * splat(T, t); // subps, shufps, addps, mulps
}

pub inline fn lerpV(v0: anytype, v1: anytype, t: anytype) @TypeOf(v0) {
    return v0 + (v1 - v0) * t; // subps, addps, mulps
}

pub const F32x4Component = enum { x, y, z, w };

pub inline fn swizzle4(
    v: F32x4,
    comptime x: F32x4Component,
    comptime y: F32x4Component,
    comptime z: F32x4Component,
    comptime w: F32x4Component,
) F32x4 {
    return @shuffle(f32, v, undefined, [4]i32{ @enumToInt(x), @enumToInt(y), @enumToInt(z), @enumToInt(w) });
}

pub inline fn mod(v0: anytype, v1: anytype) @TypeOf(v0) {
    // vdivps, vroundps, vmulps, vsubps
    return v0 - v1 * trunc(v0 / v1);
}
test "zmath.mod" {
    try expect(approxEqAbs(mod(splat(F32x4, 3.1), splat(F32x4, 1.7)), splat(F32x4, 1.4), 0.0005));
    try expect(approxEqAbs(mod(splat(F32x4, -3.0), splat(F32x4, 2.0)), splat(F32x4, -1.0), 0.0005));
    try expect(approxEqAbs(mod(splat(F32x4, -3.0), splat(F32x4, -2.0)), splat(F32x4, -1.0), 0.0005));
    try expect(approxEqAbs(mod(splat(F32x4, 3.0), splat(F32x4, -2.0)), splat(F32x4, 1.0), 0.0005));
    try expect(isNan4(mod(splat(F32x4, math.inf_f32), splat(F32x4, 1.0))));
    try expect(isNan4(mod(splat(F32x4, -math.inf_f32), splat(F32x4, 123.456))));
    try expect(isNan4(mod(splat(F32x4, math.nan_f32), splat(F32x4, 123.456))));
    try expect(isNan4(mod(splat(F32x4, math.qnan_f32), splat(F32x4, 123.456))));
    try expect(isNan4(mod(splat(F32x4, -math.qnan_f32), splat(F32x4, 123.456))));
    try expect(isNan4(mod(splat(F32x4, 123.456), splat(F32x4, math.inf_f32))));
    try expect(isNan4(mod(splat(F32x4, 123.456), splat(F32x4, -math.inf_f32))));
    try expect(isNan4(mod(splat(F32x4, 123.456), splat(F32x4, math.nan_f32))));
    try expect(isNan4(mod(splat(F32x4, math.inf_f32), splat(F32x4, math.inf_f32))));
    try expect(isNan4(mod(splat(F32x4, math.inf_f32), splat(F32x4, math.nan_f32))));
}

pub inline fn round(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    if (cpu_arch == .x86_64 and has_avx) {
        if (T == F32x4) {
            return asm ("vroundps $0, %%xmm0, %%xmm0"
                : [ret] "={xmm0}" (-> T),
                : [v] "{xmm0}" (v),
            );
        } else if (T == F32x8) {
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
        const r2 = abs(v);
        const mask = r2 <= splatNoFraction(T);
        return select(mask, r1, v);
    }
}

pub inline fn modAngles(v: anytype) @TypeOf(v) {
    const T = @TypeOf(v);
    return v - splat(T, math.tau) * round(v * splat(T, 1.0 / math.tau)); // 2 x vmulps, 2 x load, vroundps, vaddps
}
test "zmath.modAngles" {
    try expect(approxEqAbs(modAngles(splat(F32x4, math.tau)), splat(F32x4, 0.0), 0.0005));
    try expect(approxEqAbs(modAngles(splat(F32x4, 0.0)), splat(F32x4, 0.0), 0.0005));
    try expect(approxEqAbs(modAngles(splat(F32x4, math.pi)), splat(F32x4, math.pi), 0.0005));
    try expect(approxEqAbs(modAngles(splat(F32x4, 11 * math.pi)), splat(F32x4, math.pi), 0.0005));
    try expect(approxEqAbs(modAngles(splat(F32x4, 3.5 * math.pi)), splat(F32x4, -0.5 * math.pi), 0.0005));
    try expect(approxEqAbs(modAngles(splat(F32x4, 2.5 * math.pi)), splat(F32x4, 0.5 * math.pi), 0.0005));
}

pub inline fn mulAdd(v0: anytype, v1: anytype, v2: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    if (cpu_arch == .x86_64 and has_avx and has_fma) {
        return @mulAdd(T, v0, v1, v2);
    } else {
        // NOTE(mziulek): On .x86_64 without HW fma instructions @mulAdd maps to really slow code!
        return v0 * v1 + v2;
    }
}

pub fn sin(v: anytype) @TypeOf(v) {
    // 11-degree minimax approximation
    const T = @TypeOf(v);

    var x = modAngles(v);
    const sign = andInt(x, splatNegativeZero(T));
    const c = orInt(sign, splat(T, math.pi));
    const absx = andNotInt(sign, x);
    const rflx = c - x;
    const comp = absx <= splat(T, 0.5 * math.pi);
    x = select(comp, x, rflx);
    const x2 = x * x;

    var result = mulAdd(splat(T, -2.3889859e-08), x2, splat(T, 2.7525562e-06));
    result = mulAdd(result, x2, splat(T, -0.00019840874));
    result = mulAdd(result, x2, splat(T, 0.0083333310));
    result = mulAdd(result, x2, splat(T, -0.16666667));
    result = mulAdd(result, x2, splat(T, 1.0));
    return x * result;
}
test "sin" {
    const epsilon = 0.0001;

    try expect(approxEqAbs(sin(splat(F32x4, 0.5 * math.pi)), splat(F32x4, 1.0), epsilon));
    try expect(approxEqAbs(sin(splat(F32x4, 0.0)), splat(F32x4, 0.0), epsilon));
    try expect(approxEqAbs(sin(splat(F32x4, -0.0)), splat(F32x4, -0.0), epsilon));
    try expect(approxEqAbs(sin(splat(F32x4, 89.123)), splat(F32x4, 0.916166), epsilon));
    try expect(approxEqAbs(sin(splat(F32x8, 89.123)), splat(F32x8, 0.916166), epsilon));
    try expect(isNan4(sin(splat(F32x4, math.inf_f32))) == true);
    try expect(isNan4(sin(splat(F32x4, -math.inf_f32))) == true);
    try expect(isNan4(sin(splat(F32x4, math.nan_f32))) == true);
    try expect(isNan4(sin(splat(F32x4, math.qnan_f32))) == true);

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = sin(splat(F32x4, f));
        const fr = @sin(splat(F32x4, f));
        const vr8 = sin(splat(F32x8, f));
        const fr8 = @sin(splat(F32x8, f));
        try expect(approxEqAbs(vr, fr, epsilon));
        try expect(approxEqAbs(vr8, fr8, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn cos(v: anytype) @TypeOf(v) {
    // 10-degree minimax approximation
    const T = @TypeOf(v);

    var x = modAngles(v);
    var sign = andInt(x, splatNegativeZero(T));
    const c = orInt(sign, splat(T, math.pi));
    const absx = andNotInt(sign, x);
    const rflx = c - x;
    const comp = absx <= splat(T, 0.5 * math.pi);
    x = select(comp, x, rflx);
    sign = select(comp, splat(T, 1.0), splat(T, -1.0));
    const x2 = x * x;

    var result = mulAdd(splat(T, -2.6051615e-07), x2, splat(T, 2.4760495e-05));
    result = mulAdd(result, x2, splat(T, -0.0013888378));
    result = mulAdd(result, x2, splat(T, 0.041666638));
    result = mulAdd(result, x2, splat(T, -0.5));
    result = mulAdd(result, x2, splat(T, 1.0));
    return sign * result;
}
test "zmath.cos" {
    const epsilon = 0.0001;

    try expect(approxEqAbs(cos(splat(F32x4, 0.5 * math.pi)), splat(F32x4, 0.0), epsilon));
    try expect(approxEqAbs(cos(splat(F32x4, 0.0)), splat(F32x4, 1.0), epsilon));
    try expect(approxEqAbs(cos(splat(F32x4, -0.0)), splat(F32x4, 1.0), epsilon));
    try expect(isNan4(cos(splat(F32x4, math.inf_f32))) == true);
    try expect(isNan4(cos(splat(F32x4, -math.inf_f32))) == true);
    try expect(isNan4(cos(splat(F32x4, math.nan_f32))) == true);
    try expect(isNan4(cos(splat(F32x4, math.qnan_f32))) == true);

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = cos(splat(F32x4, f));
        const fr = @cos(splat(F32x4, f));
        const vr8 = cos(splat(F32x8, f));
        const fr8 = @cos(splat(F32x8, f));
        try expect(approxEqAbs(vr, fr, epsilon));
        try expect(approxEqAbs(vr8, fr8, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn sincos(v: anytype) [2]@TypeOf(v) {
    const T = @TypeOf(v);
    return switch (T) {
        f32 => sincos32(v),
        F32x4, F32x8 => sincos32xN(v),
        else => @compileError("zmath.sincos not implemented for " ++ @typeName(T)),
    };
}

fn sincos32xN(v: anytype) [2]@TypeOf(v) {
    const T = @TypeOf(v);

    var x = modAngles(v);
    var sign = andInt(x, splatNegativeZero(T));
    const c = orInt(sign, splat(T, math.pi));
    const absx = andNotInt(sign, x);
    const rflx = c - x;
    const comp = absx <= splat(T, 0.5 * math.pi);
    x = select(comp, x, rflx);
    sign = select(comp, splat(T, 1.0), splat(T, -1.0));
    const x2 = x * x;

    var sresult = mulAdd(splat(T, -2.3889859e-08), x2, splat(T, 2.7525562e-06));
    sresult = mulAdd(sresult, x2, splat(T, -0.00019840874));
    sresult = mulAdd(sresult, x2, splat(T, 0.0083333310));
    sresult = mulAdd(sresult, x2, splat(T, -0.16666667));
    sresult = x * mulAdd(sresult, x2, splat(T, 1.0));

    var cresult = mulAdd(splat(T, -2.6051615e-07), x2, splat(T, 2.4760495e-05));
    cresult = mulAdd(cresult, x2, splat(T, -0.0013888378));
    cresult = mulAdd(cresult, x2, splat(T, 0.041666638));
    cresult = mulAdd(cresult, x2, splat(T, -0.5));
    cresult = sign * mulAdd(cresult, x2, splat(T, 1.0));

    return .{ sresult, cresult };
}
test "zmath.sincos32xN" {
    const epsilon = 0.0001;

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const sc = sincos(splat(F32x4, f));
        const sc8 = sincos(splat(F32x8, f));
        const s4 = @sin(splat(F32x4, f));
        const s8 = @sin(splat(F32x8, f));
        const c4 = @cos(splat(F32x4, f));
        const c8 = @cos(splat(F32x8, f));
        try expect(approxEqAbs(sc[0], s4, epsilon));
        try expect(approxEqAbs(sc8[0], s8, epsilon));
        try expect(approxEqAbs(sc[1], c4, epsilon));
        try expect(approxEqAbs(sc8[1], c8, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

fn sincos32(v: f32) [2]f32 {
    var quotient = 1.0 / math.tau * v;
    if (v >= 0.0) {
        quotient = @intToFloat(f32, @floatToInt(i32, quotient + 0.5));
    } else {
        quotient = @intToFloat(f32, @floatToInt(i32, quotient - 0.5));
    }
    var y = v - math.tau * quotient;

    const sign = blk: {
        if (y > 0.5 * math.pi) {
            y = math.pi - y;
            break :blk @as(f32, -1.0);
        } else if (y < -math.pi * 0.5) {
            y = -math.pi - y;
            break :blk @as(f32, -1.0);
        } else {
            break :blk @as(f32, 1.0);
        }
    };
    const y2 = y * y;

    // 11-degree minimax approximation
    var sinv = mulAdd(@as(f32, -2.3889859e-08), y2, 2.7525562e-06);
    sinv = mulAdd(sinv, y2, -0.00019840874);
    sinv = mulAdd(sinv, y2, 0.0083333310);
    sinv = mulAdd(sinv, y2, -0.16666667);
    sinv = y * mulAdd(sinv, y2, 1.0);

    // 10-degree minimax approximation
    var cosv = mulAdd(@as(f32, -2.6051615e-07), y2, 2.4760495e-05);
    cosv = mulAdd(cosv, y2, -0.0013888378);
    cosv = mulAdd(cosv, y2, 0.041666638);
    cosv = mulAdd(cosv, y2, -0.5);
    cosv = sign * mulAdd(cosv, y2, 1.0);

    return .{ sinv, cosv };
}
test "zmath.sincos32" {
    const epsilon = 0.0001;

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const sc = sincos32(f);
        const s = @sin(f);
        const c = @cos(f);
        try expect(math.approxEqAbs(f32, sc[0], s, epsilon));
        try expect(math.approxEqAbs(f32, sc[1], c, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

//
// Load/store functions
//

pub inline fn loadF32x4(mem: []const f32, comptime len: u32) F32x4 {
    switch (len) {
        1 => return f32x4(mem[0], 0, 0, 0),
        2 => return f32x4(mem[0], mem[1], 0, 0),
        3 => return f32x4(mem[0], mem[1], mem[2], 0),
        4 => return f32x4(mem[0], mem[1], mem[2], mem[3]),
        else => unreachable,
    }
}
test "zmath.loadF32x4" {
    const a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    var ptr = &a;
    var i: u32 = 0;
    const v0 = loadF32x4(a[i..], 2);
    try expect(approxEqAbs(v0, [4]f32{ 1.0, 2.0, 0.0, 0.0 }, 0.0));
    i += 2;
    const v1 = loadF32x4(a[i .. i + 2], 2);
    try expect(approxEqAbs(v1, [4]f32{ 3.0, 4.0, 0.0, 0.0 }, 0.0));
    const v2 = loadF32x4(a[5..7], 2);
    try expect(approxEqAbs(v2, [4]f32{ 6.0, 7.0, 0.0, 0.0 }, 0.0));
    const v3 = loadF32x4(ptr[1..], 2);
    try expect(approxEqAbs(v3, [4]f32{ 2.0, 3.0, 0.0, 0.0 }, 0.0));
    i += 1;
    const v4 = loadF32x4(ptr[i .. i + 2], 2);
    try expect(approxEqAbs(v4, [4]f32{ 4.0, 5.0, 0.0, 0.0 }, 0.0));
}

pub inline fn loadF32x8(mem: []const f32, comptime len: u32) F32x8 {
    switch (len) {
        1 => return f32x8(mem[0], 0, 0, 0, 0, 0, 0, 0),
        2 => return f32x8(mem[0], mem[1], 0, 0, 0, 0, 0, 0),
        3 => return f32x8(mem[0], mem[1], mem[2], 0, 0, 0, 0, 0),
        4 => return f32x8(mem[0], mem[1], mem[2], mem[3], 0, 0, 0, 0),
        5 => return f32x8(mem[0], mem[1], mem[2], mem[3], mem[4], 0, 0, 0),
        6 => return f32x8(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], 0, 0),
        7 => return f32x8(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], 0),
        8 => return f32x8(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]),
        else => unreachable,
    }
}

pub inline fn storeF32x4(mem: []f32, v: F32x4, comptime len: u32) void {
    assert(len >= 1 and len <= 4);
    comptime var i: u32 = 0;
    inline while (i < len) : (i += 1) {
        mem[i] = v[i];
    }
}
test "zmath.storeF32x4" {
    var a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    const v = loadF32x4(a[1..], 3);
    storeF32x4(a[2..], v, 4);
    try expect(a[0] == 1.0);
    try expect(a[1] == 2.0);
    try expect(a[2] == 2.0);
    try expect(a[3] == 3.0);
    try expect(a[4] == 4.0);
    try expect(a[5] == 0.0);
}

pub inline fn storeF32x8(mem: []f32, v: F32x8, comptime len: u32) void {
    assert(len >= 1 and len <= 8);
    comptime var i: u32 = 0;
    inline while (i < len) : (i += 1) {
        mem[i] = v[i];
    }
}

//
// Functions working on 2 components
//

pub inline fn isEqual2(v0: F32x4, v1: F32x4) bool {
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
        const v0 = F32x4{ 1.0, math.inf_f32, -3.0, 1000.001 };
        const v1 = F32x4{ 1.0, math.inf_f32, -6.0, 4.0 };
        try expect(isEqual2(v0, v1));
    }
}

pub inline fn isEqualInt2(v0: F32x4, v1: F32x4) bool {
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
        const v0u = @bitCast(U32x4, v0);
        const v1u = @bitCast(U32x4, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1];
    }
}

pub inline fn isNearEqual2(v0: F32x4, v1: F32x4, epsilon: F32x4) bool {
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
        const mask = isNearEqual(v0, v1, epsilon);
        return mask[0] and mask[1];
    }
}
test "zmath.isNearEqual2" {
    {
        const v0 = F32x4{ 1.0, 2.0, -6.0001, 1000.001 };
        const v1 = F32x4{ 1.0, 2.001, -3.0, 4.0 };
        const v2 = F32x4{ 1.001, 2.001, -3.001, 4.001 };
        try expect(isNearEqual2(v0, v1, splat(F32x4, 0.01)));
        try expect(isNearEqual2(v2, v1, splat(F32x4, 0.01)));
    }
}

pub inline fn isLess2(v0: F32x4, v1: F32x4) bool {
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
    const v0 = F32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 1.0 };
    try expect(isLess2(v0, v1) == true);

    const v2 = F32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v3 = F32x4{ 4.0, -5.0, 6.0, 1.0 };
    try expect(isLess2(v2, v3) == false);

    const v4 = F32x4{ 100.0, 200.0, 300.0, 50000.0 };
    const v5 = F32x4{ 400.0, 500.0, 600.0, 1.0 };
    try expect(isLess2(v4, v5) == true);

    const v6 = F32x4{ 100.0, -math.inf_f32, -math.inf_f32, 50000.0 };
    const v7 = F32x4{ 400.0, math.inf_f32, 600.0, 1.0 };
    try expect(isLess2(v6, v7) == true);
}

pub inline fn isLessEqual2(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isGreater2(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isGreaterEqual2(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isInBounds2(v: F32x4, bounds: F32x4) bool {
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
        const b0 = v <= bounds;
        const b1 = bounds * splat(F32x4, -1.0) <= v;
        const b = @bitCast(Boolx4, (@bitCast(U1x4, b0) & @bitCast(U1x4, b1)) | U1x4{ 0, 0, 1, 1 });
        return @reduce(.And, b);
    }
}
test "zmath.isInBounds2" {
    {
        const v0 = F32x4{ 0.5, -2.0, -100.0, 1000.0 };
        const v1 = F32x4{ -1.6, -2.001, 1.0, 1.9 };
        const bounds = F32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds2(v0, bounds) == true);
        try expect(isInBounds2(v1, bounds) == false);
    }
    {
        const v0 = F32x4{ 10000.0, -1000.0, -10.0, 1000.0 };
        const bounds = F32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds2(v0, bounds) == true);
    }
}

pub inline fn isNan2(v: F32x4) bool {
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
    try expect(isNan2(F32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan2(F32x4{ -1.0, 100.0, 3.0, math.nan_f32 }) == false);
    try expect(isNan2(F32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
    try expect(isNan2(F32x4{ -1.0, math.qnan_f32, 3.0, -2.0 }) == true);
    try expect(isNan2(F32x4{ -1.0, 1.0, 3.0, -2.0 }) == false);
    try expect(isNan2(F32x4{ -1.0, 1.0, 3.0, math.qnan_f32 }) == false);
}

pub inline fn isInf2(v: F32x4) bool {
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
        const b = isInf(v);
        return b[0] or b[1];
    }
}

pub inline fn dot2(v0: F32x4, v1: F32x4) F32x4 {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | -- | -- |
    var xmm1 = swizzle4(xmm0, .y, .x, .x, .x); // | y0*y1 | -- | -- | -- |
    xmm0 = F32x4{ xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[3] }; // | x0*x1 + y0*y1 | -- | -- | -- |
    return swizzle4(xmm0, .x, .x, .x, .x);
}
test "zmath.dot2" {
    const v0 = F32x4{ -1.0, 2.0, 300.0, -2.0 };
    const v1 = F32x4{ 4.0, 5.0, 600.0, 2.0 };
    var v = dot2(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 6.0), 0.0001));
}

pub inline fn lengthSq2(v: F32x4) F32x4 {
    return dot2(v, v);
}

pub inline fn length2(v: F32x4) F32x4 {
    return sqrt(dot2(v, v));
}

pub inline fn normalize2(v: F32x4) F32x4 {
    return v * splat(F32x4, 1.0) / sqrt(dot2(v, v));
}

//
// Functions working on 3 components
//

pub inline fn isEqual3(v0: F32x4, v1: F32x4) bool {
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
        const v0 = F32x4{ 1.0, math.inf_f32, -3.0, 1000.001 };
        const v1 = F32x4{ 1.0, math.inf_f32, -3.0, 4.0 };
        try expect(isEqual3(v0, v1) == true);
    }
    {
        const v0 = F32x4{ 1.0, math.inf_f32, -3.0, 4.0 };
        const v1 = F32x4{ 1.0, -math.inf_f32, -3.0, 4.0 };
        try expect(isEqual3(v0, v1) == false);
    }
}

pub inline fn isEqualInt3(v0: F32x4, v1: F32x4) bool {
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
        const v0u = @bitCast(U32x4, v0);
        const v1u = @bitCast(U32x4, v1);
        const mask = v0u == v1u;
        return mask[0] and mask[1] and mask[2];
    }
}

pub inline fn isNearEqual3(v0: F32x4, v1: F32x4, epsilon: F32x4) bool {
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
        const mask = isNearEqual(v0, v1, epsilon);
        return mask[0] and mask[1] and mask[2];
    }
}
test "zmath.isNearEqual3" {
    {
        const v0 = F32x4{ 1.0, 2.0, -3.0001, 1000.001 };
        const v1 = F32x4{ 1.0, 2.001, -3.0, 4.0 };
        try expect(isNearEqual3(v0, v1, splat(F32x4, 0.01)));
    }
}

pub inline fn isLess3(v0: F32x4, v1: F32x4) bool {
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
    const v0 = F32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 1.0 };
    try expect(isLess3(v0, v1) == true);

    const v2 = F32x4{ -1.0, 2.0, 3.0, 5.0 };
    const v3 = F32x4{ 4.0, -5.0, 6.0, 1.0 };
    try expect(isLess3(v2, v3) == false);

    const v4 = F32x4{ 100.0, 200.0, 300.0, -500.0 };
    const v5 = F32x4{ 400.0, 500.0, 600.0, 1.0 };
    try expect(isLess3(v4, v5) == true);

    const v6 = F32x4{ 100.0, -math.inf_f32, -math.inf_f32, 50000.0 };
    const v7 = F32x4{ 400.0, math.inf_f32, 600.0, 1.0 };
    try expect(isLess3(v6, v7) == true);
}

pub inline fn isLessEqual3(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isGreater3(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isGreaterEqual3(v0: F32x4, v1: F32x4) bool {
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

pub inline fn isInBounds3(v: F32x4, bounds: F32x4) bool {
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
        const b0 = v <= bounds;
        const b1 = bounds * splat(F32x4, -1.0) <= v;
        const b = @bitCast(Boolx4, (@bitCast(U1x4, b0) & @bitCast(U1x4, b1)) | U1x4{ 0, 0, 0, 1 });
        return @reduce(.And, b);
    }
}
test "zmath.isInBounds3" {
    {
        const v0 = F32x4{ 0.5, -2.0, -1.0, 1.0 };
        const v1 = F32x4{ -1.6, 1.1, -0.1, 2.9 };
        const v2 = F32x4{ 0.5, -2.0, -1.0, 10.0 };
        const bounds = F32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds3(v0, bounds) == true);
        try expect(isInBounds3(v1, bounds) == false);
        try expect(isInBounds3(v2, bounds) == true);
    }
    {
        const v0 = F32x4{ 10000.0, -1000.0, -1.0, 1000.0 };
        const bounds = F32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds3(v0, bounds) == true);
    }
}

pub inline fn isNan3(v: F32x4) bool {
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
    try expect(isNan3(F32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan3(F32x4{ -1.0, 100.0, 3.0, math.nan_f32 }) == false);
    try expect(isNan3(F32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
    try expect(isNan3(F32x4{ -1.0, math.qnan_f32, 3.0, -2.0 }) == true);
    try expect(isNan3(F32x4{ -1.0, 1.0, 3.0, -2.0 }) == false);
    try expect(isNan3(F32x4{ -1.0, 1.0, 3.0, math.qnan_f32 }) == false);
}

pub inline fn isInf3(v: F32x4) bool {
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
        const b = isInf(v);
        return b[0] or b[1] or b[2];
    }
}
test "zmath.isInf3" {
    try expect(isInf3(splat(F32x4, math.inf_f32)) == true);
    try expect(isInf3(splat(F32x4, -math.inf_f32)) == true);
    try expect(isInf3(splat(F32x4, math.nan_f32)) == false);
    try expect(isInf3(splat(F32x4, -math.nan_f32)) == false);
    try expect(isInf3(splat(F32x4, math.qnan_f32)) == false);
    try expect(isInf3(splat(F32x4, -math.qnan_f32)) == false);
    try expect(isInf3(F32x4{ 1.0, 2.0, 3.0, 4.0 }) == false);
    try expect(isInf3(F32x4{ 1.0, 2.0, 3.0, math.inf_f32 }) == false);
    try expect(isInf3(F32x4{ 1.0, 2.0, math.inf_f32, 1.0 }) == true);
    try expect(isInf3(F32x4{ -math.inf_f32, math.inf_f32, math.inf_f32, 1.0 }) == true);
    try expect(isInf3(F32x4{ -math.inf_f32, math.nan_f32, math.inf_f32, 1.0 }) == true);
}

pub inline fn dot3(v0: F32x4, v1: F32x4) F32x4 {
    var dot = v0 * v1;
    var temp = swizzle4(dot, .y, .z, .y, .z);
    dot = F32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    temp = swizzle4(temp, .y, .y, .y, .y);
    dot = F32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    return swizzle4(dot, .x, .x, .x, .x);
}
test "zmath.dot3" {
    const v0 = F32x4{ -1.0, 2.0, 3.0, 1.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 1.0 };
    var v = dot3(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 24.0), 0.0001));
}

pub inline fn cross3(v0: F32x4, v1: F32x4) F32x4 {
    var xmm0 = swizzle4(v0, .y, .z, .x, .w);
    var xmm1 = swizzle4(v1, .z, .x, .y, .w);
    var result = xmm0 * xmm1;
    xmm0 = swizzle4(xmm0, .y, .z, .x, .w);
    xmm1 = swizzle4(xmm1, .z, .x, .y, .w);
    result = result - xmm0 * xmm1;
    return @bitCast(F32x4, @bitCast(U32x4, result) & u32x4_mask3);
}
test "zmath.cross3" {
    {
        const v0 = F32x4{ 1.0, 0.0, 0.0, 1.0 };
        const v1 = F32x4{ 0.0, 1.0, 0.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, F32x4{ 0.0, 0.0, 1.0, 0.0 }, 0.0001));
    }
    {
        const v0 = F32x4{ 1.0, 0.0, 0.0, 1.0 };
        const v1 = F32x4{ 0.0, -1.0, 0.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, F32x4{ 0.0, 0.0, -1.0, 0.0 }, 0.0001));
    }
    {
        const v0 = F32x4{ -3.0, 0, -2.0, 1.0 };
        const v1 = F32x4{ 5.0, -1.0, 2.0, 1.0 };
        var v = cross3(v0, v1);
        try expect(approxEqAbs(v, F32x4{ -2.0, -4.0, 3.0, 0.0 }, 0.0001));
    }
}

pub inline fn lengthSq3(v: F32x4) F32x4 {
    return dot3(v, v);
}

pub inline fn length3(v: F32x4) F32x4 {
    return sqrt(dot3(v, v));
}
test "zmath.length3" {
    {
        var v = length3(F32x4{ 1.0, -2.0, 3.0, 1000.0 });
        try expect(approxEqAbs(v, splat(F32x4, math.sqrt(14.0)), 0.001));
    }
    {
        var v = length3(F32x4{ 1.0, math.nan_f32, math.inf_f32, 1000.0 });
        try expect(isNan4(v));
    }
    {
        var v = length3(F32x4{ 1.0, math.inf_f32, 3.0, 1000.0 });
        try expect(isInf4(v));
    }
    {
        var v = length3(F32x4{ 3.0, 2.0, 1.0, math.nan_f32 });
        try expect(approxEqAbs(v, splat(F32x4, math.sqrt(14.0)), 0.001));
    }
}

pub inline fn normalize3(v: F32x4) F32x4 {
    return v * splat(F32x4, 1.0) / sqrt(dot3(v, v));
}
test "zmath.normalize3" {
    {
        const v0 = F32x4{ 1.0, -2.0, 3.0, 1000.0 };
        var v = normalize3(v0);
        try expect(approxEqAbs(v, v0 * splat(F32x4, 1.0 / math.sqrt(14.0)), 0.0005));
    }
    {
        try expect(isNan4(normalize3(F32x4{ 1.0, math.inf_f32, 1.0, 1.0 })));
        try expect(isNan4(normalize3(F32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize3(F32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize3(splat(F32x4, 0.0))));
    }
}

pub inline fn linePointDistance(line_pt0: F32x4, line_pt1: F32x4, pt: F32x4) F32x4 {
    const pt_vec = pt - line_pt0;
    const line_vec = line_pt1 - line_pt0;
    const scale = dot3(pt_vec, line_vec) / lengthSq3(line_vec);
    return length3(pt_vec - line_vec * scale);
}

test "zmath.linePointDistance" {
    {
        const line_pt0 = F32x4{ -1.0, -2.0, -3.0, 1.0 };
        const line_pt1 = F32x4{ 1.0, 2.0, 3.0, 1.0 };
        const pt = F32x4{ 1.0, 1.0, 1.0, 1.0 };
        var v = linePointDistance(line_pt0, line_pt1, pt);
        try expect(approxEqAbs(v, splat(F32x4, 0.654), 0.001));
    }
}

//
// Functions working on 4 components
//

pub inline fn isEqual4(v0: F32x4, v1: F32x4) bool {
    const mask = v0 == v1;
    return @reduce(.And, mask);
}

pub inline fn isEqual4Int(v0: F32x4, v1: F32x4) bool {
    const v0u = @bitCast(U32x4, v0);
    const v1u = @bitCast(U32x4, v1);
    const mask = v0u == v1u;
    return @reduce(.And, mask);
}

pub inline fn isNearEqual4(v0: F32x4, v1: F32x4, epsilon: F32x4) bool {
    // Won't handle inf & nan
    const delta = v0 - v1;
    const temp = maxFast(delta, splat(F32x4, 0.0) - delta);
    return @reduce(.And, temp <= epsilon);
}

pub inline fn isLess4(v0: F32x4, v1: F32x4) bool {
    const mask = v0 < v1;
    return @reduce(.And, mask);
}

pub inline fn isLessEqual4(v0: F32x4, v1: F32x4) bool {
    const mask = v0 <= v1;
    return @reduce(.And, mask);
}

pub inline fn isGreater4(v0: F32x4, v1: F32x4) bool {
    const mask = v0 > v1;
    return @reduce(.And, mask);
}

pub inline fn isGreaterEqual(v0: F32x4, v1: F32x4) bool {
    const mask = v0 >= v1;
    return @reduce(.And, mask);
}

pub inline fn isInBounds4(v: F32x4, bounds: F32x4) bool {
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
        const b0 = v <= bounds;
        const b1 = bounds * splat(F32x4, -1.0) <= v;
        const b = @bitCast(Boolx4, @bitCast(U1x4, b0) & @bitCast(U1x4, b1));
        return @reduce(.And, b);
    }
}
test "zmath.isInBounds4" {
    {
        const v0 = F32x4{ 0.5, -2.0, -1.0, 1.9 };
        const v1 = F32x4{ -1.6, -2.001, -1.0, 1.9 };
        const bounds = F32x4{ 1.0, 2.0, 1.0, 2.0 };
        try expect(isInBounds4(v0, bounds) == true);
        try expect(isInBounds4(v1, bounds) == false);
    }
    {
        const v0 = F32x4{ 10000.0, -1000.0, -1.0, 0.0 };
        const bounds = F32x4{ math.inf_f32, math.inf_f32, 1.0, 2.0 };
        try expect(isInBounds4(v0, bounds) == true);
    }
}

pub inline fn isNan4(v: F32x4) bool {
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
    try expect(isNan4(F32x4{ -1.0, math.nan_f32, 3.0, -2.0 }) == true);
    try expect(isNan4(F32x4{ -1.0, 100.0, 3.0, -2.0 }) == false);
    try expect(isNan4(F32x4{ -1.0, math.inf_f32, 3.0, -2.0 }) == false);
}

pub inline fn isInf4(v: F32x4) bool {
    // andps, cmpeqps, movmskps, test, setne
    const b = isInf(v);
    return b[0] or b[1] or b[2] or b[3];
}

pub inline fn dot4(v0: F32x4, v1: F32x4) F32x4 {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | z0*z1 | w0*w1 |
    var xmm1 = swizzle4(xmm0, .y, .x, .w, .x); // | y0*y1 | -- | w0*w1 | -- |
    xmm1 = xmm0 + xmm1; // | x0*x1 + y0*y1 | -- | z0*z1 + w0*w1 | -- |
    xmm0 = swizzle4(xmm1, .z, .x, .x, .x); // | z0*z1 + w0*w1 | -- | -- | -- |
    xmm0 = F32x4{ xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[2] }; // addss
    return swizzle4(xmm0, .x, .x, .x, .x);
}
test "zmath.dot4" {
    const v0 = F32x4{ -1.0, 2.0, 3.0, -2.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 2.0 };
    var v = dot4(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 20.0), 0.0001));
}

pub inline fn lengthSq4(v: F32x4) F32x4 {
    return dot4(v, v);
}

pub inline fn length4(v: F32x4) F32x4 {
    return sqrt(dot4(v, v));
}

pub inline fn normalize4(v: F32x4) F32x4 {
    return v * splat(F32x4, 1.0) / sqrt(dot4(v, v));
}
test "zmath.normalize4" {
    {
        const v0 = F32x4{ 1.0, -2.0, 3.0, 10.0 };
        var v = normalize4(v0);
        try expect(approxEqAbs(v, v0 * splat(F32x4, 1.0 / math.sqrt(114.0)), 0.0005));
    }
    {
        try expect(isNan4(normalize4(F32x4{ 1.0, math.inf_f32, 1.0, 1.0 })));
        try expect(isNan4(normalize4(F32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize4(F32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 })));
        try expect(isNan4(normalize4(splat(F32x4, 0.0))));
    }
}

pub fn mul(m0: Mat, m1: Mat) Mat {
    var result: Mat = undefined;
    comptime var row: u32 = 0;
    inline while (row < 4) : (row += 1) {
        var vx = @shuffle(f32, m0[row], undefined, [4]i32{ 0, 0, 0, 0 });
        var vy = @shuffle(f32, m0[row], undefined, [4]i32{ 1, 1, 1, 1 });
        var vz = @shuffle(f32, m0[row], undefined, [4]i32{ 2, 2, 2, 2 });
        var vw = @shuffle(f32, m0[row], undefined, [4]i32{ 3, 3, 3, 3 });
        vx = vx * m1[0];
        vy = vy * m1[1];
        vz = vz * m1[2];
        vw = vw * m1[3];
        vx = vx + vz;
        vy = vy + vw;
        vx = vx + vy;
        result[row] = vx;
    }
    return result;
}
test "zmath.matrix.mul" {
    const a = Mat{
        f32x4(0.1, 0.2, 0.3, 0.4),
        f32x4(0.5, 0.6, 0.7, 0.8),
        f32x4(0.9, 1.0, 1.1, 1.2),
        f32x4(1.3, 1.4, 1.5, 1.6),
    };
    const b = Mat{
        f32x4(1.7, 1.8, 1.9, 2.0),
        f32x4(2.1, 2.2, 2.3, 2.4),
        f32x4(2.5, 2.6, 2.7, 2.8),
        f32x4(2.9, 3.0, 3.1, 3.2),
    };
    const c = mul(a, b);
    try expect(approxEqAbs(c[0], f32x4(2.5, 2.6, 2.7, 2.8), 0.0001));
    try expect(approxEqAbs(c[1], f32x4(6.18, 6.44, 6.7, 6.96), 0.0001));
    try expect(approxEqAbs(c[2], f32x4(9.86, 10.28, 10.7, 11.12), 0.0001));
    try expect(approxEqAbs(c[3], f32x4(13.54, 14.12, 14.7, 15.28), 0.0001));
}

pub fn transpose(m: Mat) Mat {
    const temp1 = @shuffle(f32, m[0], m[1], [4]i32{ 0, 1, ~@as(i32, 0), ~@as(i32, 1) });
    const temp3 = @shuffle(f32, m[0], m[1], [4]i32{ 2, 3, ~@as(i32, 2), ~@as(i32, 3) });
    const temp2 = @shuffle(f32, m[2], m[3], [4]i32{ 0, 1, ~@as(i32, 0), ~@as(i32, 1) });
    const temp4 = @shuffle(f32, m[2], m[3], [4]i32{ 2, 3, ~@as(i32, 2), ~@as(i32, 3) });
    return .{
        @shuffle(f32, temp1, temp2, [4]i32{ 0, 2, ~@as(i32, 0), ~@as(i32, 2) }),
        @shuffle(f32, temp1, temp2, [4]i32{ 1, 3, ~@as(i32, 1), ~@as(i32, 3) }),
        @shuffle(f32, temp3, temp4, [4]i32{ 0, 2, ~@as(i32, 0), ~@as(i32, 2) }),
        @shuffle(f32, temp3, temp4, [4]i32{ 1, 3, ~@as(i32, 1), ~@as(i32, 3) }),
    };
}
test "zmath.matrix.transpose" {
    const m = Mat{
        f32x4(1.0, 2.0, 3.0, 4.0),
        f32x4(5.0, 6.0, 7.0, 8.0),
        f32x4(9.0, 10.0, 11.0, 12.0),
        f32x4(13.0, 14.0, 15.0, 16.0),
    };
    const mt = transpose(m);
    try expect(approxEqAbs(mt[0], f32x4(1.0, 5.0, 9.0, 13.0), 0.0001));
    try expect(approxEqAbs(mt[1], f32x4(2.0, 6.0, 10.0, 14.0), 0.0001));
    try expect(approxEqAbs(mt[2], f32x4(3.0, 7.0, 11.0, 15.0), 0.0001));
    try expect(approxEqAbs(mt[3], f32x4(4.0, 8.0, 12.0, 16.0), 0.0001));
}

pub fn rotationX(angle: f32) Mat {
    const sc = sincos(angle);
    return .{
        f32x4(1.0, 0.0, 0.0, 0.0),
        f32x4(0.0, sc[1], sc[0], 0.0),
        f32x4(0.0, -sc[0], sc[1], 0.0),
        f32x4(0.0, 0.0, 0.0, 1.0),
    };
}

pub fn rotationY(angle: f32) Mat {
    const sc = sincos(angle);
    return .{
        f32x4(sc[1], 0.0, -sc[0], 0.0),
        f32x4(0.0, 1.0, 0.0, 0.0),
        f32x4(sc[0], 0.0, sc[1], 0.0),
        f32x4(0.0, 0.0, 0.0, 1.0),
    };
}

pub fn rotationZ(angle: f32) Mat {
    const sc = sincos(angle);
    return .{
        f32x4(sc[1], sc[0], 0.0, 0.0),
        f32x4(-sc[0], sc[1], 0.0, 0.0),
        f32x4(0.0, 0.0, 1.0, 0.0),
        f32x4(0.0, 0.0, 0.0, 1.0),
    };
}

pub fn translation(x: f32, y: f32, z: f32) Mat {
    return .{
        f32x4(1.0, 0.0, 0.0, 0.0),
        f32x4(0.0, 1.0, 0.0, 0.0),
        f32x4(0.0, 0.0, 1.0, 0.0),
        f32x4(x, y, z, 1.0),
    };
}
pub fn translationV(v: F32x4) Mat {
    return translation(v[0], v[1], v[2]);
}

pub fn scaling(x: f32, y: f32, z: f32) Mat {
    return .{
        f32x4(x, 0.0, 0.0, 0.0),
        f32x4(0.0, y, 0.0, 0.0),
        f32x4(0.0, 0.0, z, 0.0),
        f32x4(0.0, 0.0, 0.0, 1.0),
    };
}
pub fn scalingV(v: F32x4) Mat {
    return scaling(v[0], v[1], v[2]);
}

pub fn lookToLh(eye_pos: F32x4, eye_dir: F32x4, up_dir: F32x4) Mat {
    const az = normalize3(eye_dir);
    const ax = normalize3(cross3(up_dir, az));
    const ay = normalize3(cross3(az, ax));
    return transpose(.{
        f32x4(ax[0], ax[1], ax[2], -dot3(ax, eye_pos)[0]),
        f32x4(ay[0], ay[1], ay[2], -dot3(ay, eye_pos)[0]),
        f32x4(az[0], az[1], az[2], -dot3(az, eye_pos)[0]),
        f32x4(0.0, 0.0, 0.0, 1.0),
    });
}
pub fn lookAtLh(eye_pos: F32x4, focus_pos: F32x4, up_dir: F32x4) Mat {
    return lookToLh(eye_pos, focus_pos - eye_pos, up_dir);
}
test "zmath.lookToLh" {
    const m = lookToLh(f32x4(0.0, 0.0, -3.0, 1.0), f32x4(0.0, 0.0, 1.0, 0.0), f32x4(0.0, 1.0, 0.0, 0.0));
    try expect(approxEqAbs(m[0], f32x4(1.0, 0.0, 0.0, 0.0), 0.001));
    try expect(approxEqAbs(m[1], f32x4(0.0, 1.0, 0.0, 0.0), 0.001));
    try expect(approxEqAbs(m[2], f32x4(0.0, 0.0, 1.0, 0.0), 0.001));
    try expect(approxEqAbs(m[3], f32x4(0.0, 0.0, 3.0, 1.0), 0.001));
}

pub fn perspectiveFovLh(fovy: f32, aspect: f32, near: f32, far: f32) Mat {
    const scfov = sincos(0.5 * fovy);

    assert(near > 0.0 and far > 0.0 and far > near);
    assert(!math.approxEqAbs(f32, scfov[0], 0.0, 0.001));
    assert(!math.approxEqAbs(f32, far, near, 0.001));
    assert(!math.approxEqAbs(f32, aspect, 0.0, 0.01));

    const h = scfov[1] / scfov[0];
    const w = h / aspect;
    const r = far / (far - near);
    return .{
        f32x4(w, 0.0, 0.0, 0.0),
        f32x4(0.0, h, 0.0, 0.0),
        f32x4(0.0, 0.0, r, 1.0),
        f32x4(0.0, 0.0, -r * near, 0.0),
    };
}

//
// Constants
//

const f32x4_0x8000_0000: F32x4 = splatInt(F32x4, 0x8000_0000);
const f32x4_0x7fff_ffff: F32x4 = splatInt(F32x4, 0x7fff_ffff);
const f32x4_inf: F32x4 = splat(F32x4, math.inf_f32);
const u32x4_mask3: U32x4 = U32x4{ 0xffff_ffff, 0xffff_ffff, 0xffff_ffff, 0 };

//
// Private functions and constants
//

inline fn splatNegativeZero(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, @as(u32, 0x8000_0000)));
}
inline fn splatNoFraction(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @as(f32, 8_388_608.0));
}
inline fn splatAbsMask(comptime T: type) T {
    return @splat(@typeInfo(T).Vector.len, @bitCast(f32, @as(u32, 0x7fff_ffff)));
}

inline fn floatToIntAndBack(v: anytype) @TypeOf(v) {
    // This routine won't handle nan, inf and numbers greater than 8_388_608.0 (will generate undefined values)
    @setRuntimeSafety(false);

    const T = @TypeOf(v);
    const len = @typeInfo(T).Vector.len;

    var vi32: [len]i32 = undefined;
    comptime var i: u32 = 0;
    // vcvttps2dq
    inline while (i < len) : (i += 1) {
        vi32[i] = @floatToInt(i32, v[i]);
    }

    var vf32: [len]f32 = undefined;
    i = 0;
    // vcvtdq2ps
    inline while (i < len) : (i += 1) {
        vf32[i] = @intToFloat(f32, vi32[i]);
    }

    return vf32;
}
test "zmath.floatToIntAndBack" {
    {
        const v = floatToIntAndBack(F32x4{ 1.1, 2.9, 3.0, -4.5 });
        try expect(approxEqAbs(v, F32x4{ 1.0, 2.0, 3.0, -4.0 }, 0.0));
    }
    {
        const v = floatToIntAndBack(F32x8{ 1.1, 2.9, 3.0, -4.5, 2.5, -2.5, 1.1, -100.2 });
        try expect(approxEqAbs(v, F32x8{ 1.0, 2.0, 3.0, -4.0, 2.0, -2.0, 1.0, -100.0 }, 0.0));
    }
    {
        const v = floatToIntAndBack(F32x4{ math.inf_f32, 2.9, math.nan_f32, math.qnan_f32 });
        try expect(v[1] == 2.0);
    }
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
