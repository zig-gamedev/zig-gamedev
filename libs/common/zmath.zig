// ==============================================================================
//
// zmath v0.2 - https://github.com/michal-z/zig-gamedev/blob/main/libs/common/zmath.zig
//
// ------------------------------------------------------------------------------
// 1. Initialization functions
// ------------------------------------------------------------------------------
//
// f32x4(e0: f32, e1: f32, e2: f32, e3: f32) F32x4
// f32x8(e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32) F32x8
// f32x16(e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32,
//        e8: f32, e9: f32, ea: f32, eb: f32, ec: f32, ed: f32, ee: f32, ef: f32) F32x16
//
// u32x4(e0: u32, e1: u32, e2: u32, e3: u32) U32x4
// u32x8(e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32) U32x8
// u32x16(e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32,
//        e8: u32, e9: u32, ea: u32, eb: u32, ec: u32, ed: u32, ee: u32, ef: u32) U32x16
//
// boolx4(e0: bool, e1: bool, e2: bool, e3: bool) Boolx4
// boolx8(e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool) Boolx8
// boolx16(e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool,
//         e8: bool, e9: bool, ea: bool, eb: bool, ec: bool, ed: bool, ee: bool, ef: bool) Boolx16
//
// load(mem: []const f32, comptime T: type, comptime len: u32) T
// store(mem: []f32, v: anytype, comptime len: u32) void
//
// splat(comptime T: type, value: f32) T
// splatInt(comptime T: type, value: u32) T
// usplat(comptime T: type, value: u32) T
//
// ------------------------------------------------------------------------------
// 2. Functions that work on all vector components (F32xN = F32x4 or F32x8 or F32x16)
// ------------------------------------------------------------------------------
//
// all(comptime len: u32, vb: anytype) bool
// any(comptime len: u32, vb: anytype) bool
//
// isNearEqual(v0: F32xN, v1: F32xN, epsilon: F32xN) BoolxN
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
// 3. 2D, 3D, 4D vector functions
// ------------------------------------------------------------------------------
//
// swizzle(v: Vec, c, c, c, c) Vec (comptime c = .x | .y | .z | .w)
// dot2(v0: Vec, v1: Vec) F32x4
// dot3(v0: Vec, v1: Vec) F32x4
// dot4(v0: Vec, v1: Vec) F32x4
// cross3(v0: Vec, v1: Vec) Vec
// lengthSq2(v: Vec) F32x4
// lengthSq3(v: Vec) F32x4
// lengthSq4(v: Vec) F32x4
// length2(v: Vec) F32x4
// length3(v: Vec) F32x4
// length4(v: Vec) F32x4
// normalize2(v: Vec) Vec
// normalize3(v: Vec) Vec
// normalize4(v: Vec) Vec
//
// ------------------------------------------------------------------------------
// 4. Matrix functions
// ------------------------------------------------------------------------------
//
// mul(m0: Mat, m1: Mat) Mat
// transpose(m: Mat) Mat
// rotationX(angle: f32) Mat
// rotationY(angle: f32) Mat
// rotationZ(angle: f32) Mat
// translation(x: f32, y: f32, z: f32) Mat
// translationV(v: Vec) Mat
// scaling(x: f32, y: f32, z: f32) Mat
// scalingV(v: Vec) Mat
// lookToLh(eye_pos: Vec, eye_dir: Vec, up_dir: Vec) Mat
// lookAtLh(eye_pos: Vec, focus_pos: Vec, up_dir: Vec) Mat
// perspectiveFovLh(fovy: f32, aspect: f32, near: f32, far: f32) Mat
// determinant(m: Mat) F32x4
// inverse(m: Mat) Mat
// inverseDet(m: Mat, out_det: ?*F32x4) Mat {
// matFromAxisAngle(axis: Vec, angle: f32) Mat
// matFromNormAxisAngle(axis: Vec, angle: f32) Mat
// matFromQuat(quat: Quat) Mat
//
// ------------------------------------------------------------------------------
// 5. Quaternion functions
// ------------------------------------------------------------------------------
//
// quatToMat(quat: Quat) Mat
// mul(q0: Quat, q1: Quat) Quat
// quatFromAxisAngle(axis: Vec, angle: f32) Quat
// quatFromNormAxisAngle(axis: Vec, angle: f32) Quat
// inverse(q: Quat) Quat
//
// ------------------------------------------------------------------------------
// X. Misc functions
// ------------------------------------------------------------------------------
//
// linePointDistance(line_pt0: Vec, line_pt1: Vec, pt: Vec) F32x4
// sincos(v: f32) [2]f32
//
// ==============================================================================

// Fundamental types
pub const F32x4 = @Vector(4, f32);
pub const F32x8 = @Vector(8, f32);
pub const F32x16 = @Vector(16, f32);
pub const Boolx4 = @Vector(4, bool);
pub const Boolx8 = @Vector(8, bool);
pub const Boolx16 = @Vector(16, bool);

// Higher-level 'geometric' types
pub const Vec = F32x4;
pub const Mat = [4]F32x4;
pub const Quat = F32x4;

// Helper types
pub const U32x4 = @Vector(4, u32);
pub const U32x8 = @Vector(8, u32);
pub const U32x16 = @Vector(16, u32);

const builtin = @import("builtin");
const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const expect = std.testing.expect;

const cpu_arch = builtin.cpu.arch;
const has_avx = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .avx) else false;
const has_avx512f = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .avx512f) else false;
const has_fma = if (cpu_arch == .x86_64) std.Target.x86.featureSetHas(builtin.cpu.features, .fma) else false;

// ------------------------------------------------------------------------------
//
// 1. Initialization functions
//
// ------------------------------------------------------------------------------

pub inline fn f32x4(e0: f32, e1: f32, e2: f32, e3: f32) F32x4 {
    return .{ e0, e1, e2, e3 };
}
pub inline fn f32x8(e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32) F32x8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}
// zig fmt: off
pub inline fn f32x16(
    e0: f32, e1: f32, e2: f32, e3: f32, e4: f32, e5: f32, e6: f32, e7: f32,
    e8: f32, e9: f32, ea: f32, eb: f32, ec: f32, ed: f32, ee: f32, ef: f32) F32x16 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ea, eb, ec, ed, ee, ef };
}
// zig fmt: on

pub inline fn u32x4(e0: u32, e1: u32, e2: u32, e3: u32) U32x4 {
    return .{ e0, e1, e2, e3 };
}
pub inline fn u32x8(e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32) U32x8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}
// zig fmt: off
pub inline fn u32x16(
    e0: u32, e1: u32, e2: u32, e3: u32, e4: u32, e5: u32, e6: u32, e7: u32,
    e8: u32, e9: u32, ea: u32, eb: u32, ec: u32, ed: u32, ee: u32, ef: u32) U32x16 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ea, eb, ec, ed, ee, ef };
}
// zig fmt: on

pub inline fn boolx4(e0: bool, e1: bool, e2: bool, e3: bool) Boolx4 {
    return .{ e0, e1, e2, e3 };
}
pub inline fn boolx8(e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool) Boolx8 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7 };
}
// zig fmt: off
pub inline fn boolx16(
    e0: bool, e1: bool, e2: bool, e3: bool, e4: bool, e5: bool, e6: bool, e7: bool,
    e8: bool, e9: bool, ea: bool, eb: bool, ec: bool, ed: bool, ee: bool, ef: bool) Boolx16 {
    return .{ e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ea, eb, ec, ed, ee, ef };
}
// zig fmt: on

pub fn veclen(comptime T: type) comptime_int {
    return @typeInfo(T).Vector.len;
}

pub inline fn splat(comptime T: type, value: f32) T {
    return @splat(veclen(T), value);
}
pub inline fn splatInt(comptime T: type, value: u32) T {
    return @splat(veclen(T), @bitCast(f32, value));
}
pub inline fn usplat(comptime T: type, value: u32) T {
    return @splat(veclen(T), value);
}

pub fn load(mem: []const f32, comptime T: type, comptime len: u32) T {
    var v = splat(T, 0.0);
    comptime var loop_len = if (len == 0) veclen(T) else len;
    comptime var i: u32 = 0;
    inline while (i < loop_len) : (i += 1) {
        v[i] = mem[i];
    }
    return v;
}
test "zmath.load" {
    const a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    var ptr = &a;
    var i: u32 = 0;
    const v0 = load(a[i..], F32x4, 2);
    try expect(approxEqAbs(v0, [4]f32{ 1.0, 2.0, 0.0, 0.0 }, 0.0));
    i += 2;
    const v1 = load(a[i .. i + 2], F32x4, 2);
    try expect(approxEqAbs(v1, [4]f32{ 3.0, 4.0, 0.0, 0.0 }, 0.0));
    const v2 = load(a[5..7], F32x4, 2);
    try expect(approxEqAbs(v2, [4]f32{ 6.0, 7.0, 0.0, 0.0 }, 0.0));
    const v3 = load(ptr[1..], F32x4, 2);
    try expect(approxEqAbs(v3, [4]f32{ 2.0, 3.0, 0.0, 0.0 }, 0.0));
    i += 1;
    const v4 = load(ptr[i .. i + 2], F32x4, 2);
    try expect(approxEqAbs(v4, [4]f32{ 4.0, 5.0, 0.0, 0.0 }, 0.0));
}

pub fn store(mem: []f32, v: anytype, comptime len: u32) void {
    const T = @TypeOf(v);
    comptime var loop_len = if (len == 0) veclen(T) else len;
    comptime var i: u32 = 0;
    inline while (i < loop_len) : (i += 1) {
        mem[i] = v[i];
    }
}
test "zmath.store" {
    var a = [7]f32{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    const v = load(a[1..], F32x4, 3);
    store(a[2..], v, 4);
    try expect(a[0] == 1.0);
    try expect(a[1] == 2.0);
    try expect(a[2] == 2.0);
    try expect(a[3] == 3.0);
    try expect(a[4] == 4.0);
    try expect(a[5] == 0.0);
}

// ------------------------------------------------------------------------------
//
// 2. Functions that work on all vector components (F32xN = F32x4 or F32x8)
//
// ------------------------------------------------------------------------------

pub fn all(comptime len: u32, vb: anytype) bool {
    const T = @TypeOf(vb);
    if (len > veclen(T)) {
        @compileError("zmath.all(): 'len' is greater than vector len of type " ++ @typeName(T));
    }
    comptime var loop_len = if (len == 0) veclen(T) else len;
    const ab: [veclen(T)]bool = vb;
    comptime var i: u32 = 0;
    var result = true;
    inline while (i < loop_len) : (i += 1) {
        result = result and ab[i];
    }
    return result;
}
test "zmath.all" {
    try expect(all(5, boolx8(true, true, true, true, true, false, true, false)) == true);
    try expect(all(6, boolx8(true, true, true, true, true, false, true, false)) == false);
    try expect(all(4, boolx8(true, true, true, true, false, false, false, false)) == true);
    try expect(all(3, boolx4(true, true, true, false)) == true);
    try expect(all(1, boolx4(true, true, true, false)) == true);
    try expect(all(1, boolx4(true, false, false, false)) == true);
    try expect(all(1, boolx4(false, true, false, false)) == false);
    try expect(all(0, boolx8(true, true, true, true, true, false, true, false)) == false);
    try expect(all(0, boolx4(false, true, false, false)) == false);
    try expect(all(0, boolx4(true, true, true, true)) == true);
}

pub fn any(comptime len: u32, vb: anytype) bool {
    const T = @TypeOf(vb);
    if (len > veclen(T)) {
        @compileError("zmath.any(): 'len' is greater than vector len of type " ++ @typeName(T));
    }
    comptime var loop_len = if (len == 0) veclen(T) else len;
    const ab: [veclen(T)]bool = vb;
    comptime var i: u32 = 0;
    var result = false;
    inline while (i < loop_len) : (i += 1) {
        result = result or ab[i];
    }
    return result;
}
test "zmath.any" {
    try expect(any(0, boolx8(true, true, true, true, true, false, true, false)) == true);
    try expect(any(3, boolx8(false, false, false, true, true, false, true, false)) == false);
    try expect(any(4, boolx8(false, false, false, false, false, true, false, false)) == false);
}

pub inline fn isNearEqual(
    v0: anytype,
    v1: anytype,
    epsilon: anytype,
) @Vector(veclen(@TypeOf(v0)), bool) {
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

pub inline fn isNan(
    v: anytype,
) @Vector(veclen(@TypeOf(v)), bool) {
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
) @Vector(veclen(@TypeOf(v)), bool) {
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

pub inline fn isInBounds(
    v: anytype,
    bounds: anytype,
) @Vector(veclen(@TypeOf(v)), bool) {
    const T = @TypeOf(v);
    const Tu = @Vector(veclen(T), u1);
    const Tr = @Vector(veclen(T), bool);

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

pub inline fn andInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(veclen(T), u32);
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
    const Tu = @Vector(veclen(T), u32);
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
    const Tu = @Vector(veclen(T), u32);
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
    const Tu = @Vector(veclen(T), u32);
    const v0u = @bitCast(Tu, v0);
    const v1u = @bitCast(Tu, v1);
    return @bitCast(T, ~(v0u | v1u)); // por, pcmpeqd, pxor
}

pub inline fn xorInt(v0: anytype, v1: anytype) @TypeOf(v0) {
    const T = @TypeOf(v0);
    const Tu = @Vector(veclen(T), u32);
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

pub fn round(v: anytype) @TypeOf(v) {
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
        } else if (T == F32x16 and has_avx512f) {
            return asm ("vrndscaleps $0, %%zmm0, %%zmm0"
                : [ret] "={zmm0}" (-> T),
                : [v] "{zmm0}" (v),
            );
        } else if (T == F32x16 and !has_avx512f) {
            const arr: [16]f32 = v;
            var ymm0 = @as(F32x8, arr[0..8].*);
            var ymm1 = @as(F32x8, arr[8..16].*);
            ymm0 = asm ("vroundps $0, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> F32x8),
                : [v] "{ymm0}" (ymm0),
            );
            ymm1 = asm ("vroundps $0, %%ymm1, %%ymm1"
                : [ret] "={ymm1}" (-> F32x8),
                : [v] "{ymm1}" (ymm1),
            );
            return @shuffle(f32, ymm0, ymm1, [16]i32{ 0, 1, 2, 3, 4, 5, 6, 7, -1, -2, -3, -4, -5, -6, -7, -8 });
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
test "zmath.round" {
    {
        try expect(all(0, round(splat(F32x4, math.inf_f32)) == splat(F32x4, math.inf_f32)));
        try expect(all(0, round(splat(F32x4, -math.inf_f32)) == splat(F32x4, -math.inf_f32)));
        try expect(all(0, isNan(round(splat(F32x4, math.nan_f32)))));
        try expect(all(0, isNan(round(splat(F32x4, -math.nan_f32)))));
        try expect(all(0, isNan(round(splat(F32x4, math.qnan_f32)))));
        try expect(all(0, isNan(round(splat(F32x4, -math.qnan_f32)))));
    }
    {
        var v = round(F32x16{ 1.1, -1.1, -1.5, 1.5, 2.1, 2.8, 2.9, 4.1, 5.8, 6.1, 7.9, 8.9, 10.1, 11.2, 12.7, 13.1 });
        try expect(approxEqAbs(
            v,
            F32x16{ 1.0, -1.0, -2.0, 2.0, 2.0, 3.0, 3.0, 4.0, 6.0, 6.0, 8.0, 9.0, 10.0, 11.0, 13.0, 13.0 },
            0.0,
        ));
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
        const vr8 = round(splat(F32x8, f));
        const fr8 = @round(splat(F32x8, f));
        const vr16 = round(splat(F32x16, f));
        const fr16 = @round(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        try expect(approxEqAbs(vr8, fr8, 0.0));
        try expect(approxEqAbs(vr16, fr16, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn trunc(v: anytype) @TypeOf(v) {
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
        } else if (T == F32x16 and has_avx512f) {
            return asm ("vrndscaleps $3, %%zmm0, %%zmm0"
                : [ret] "={zmm0}" (-> T),
                : [v] "{zmm0}" (v),
            );
        } else if (T == F32x16 and !has_avx512f) {
            const arr: [16]f32 = v;
            var ymm0 = @as(F32x8, arr[0..8].*);
            var ymm1 = @as(F32x8, arr[8..16].*);
            ymm0 = asm ("vroundps $3, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> F32x8),
                : [v] "{ymm0}" (ymm0),
            );
            ymm1 = asm ("vroundps $3, %%ymm1, %%ymm1"
                : [ret] "={ymm1}" (-> F32x8),
                : [v] "{ymm1}" (ymm1),
            );
            return @shuffle(f32, ymm0, ymm1, [16]i32{ 0, 1, 2, 3, 4, 5, 6, 7, -1, -2, -3, -4, -5, -6, -7, -8 });
        }
    } else {
        const mask = abs(v) < splatNoFraction(T);
        const result = floatToIntAndBack(v);
        return select(mask, result, v);
    }
}
test "zmath.trunc" {
    {
        try expect(all(0, trunc(splat(F32x4, math.inf_f32)) == splat(F32x4, math.inf_f32)));
        try expect(all(0, trunc(splat(F32x4, -math.inf_f32)) == splat(F32x4, -math.inf_f32)));
        try expect(all(0, isNan(trunc(splat(F32x4, math.nan_f32)))));
        try expect(all(0, isNan(trunc(splat(F32x4, -math.nan_f32)))));
        try expect(all(0, isNan(trunc(splat(F32x4, math.qnan_f32)))));
        try expect(all(0, isNan(trunc(splat(F32x4, -math.qnan_f32)))));
    }
    {
        var v = trunc(F32x16{ 1.1, -1.1, -1.5, 1.5, 2.1, 2.8, 2.9, 4.1, 5.8, 6.1, 7.9, 8.9, 10.1, 11.2, 12.7, 13.1 });
        try expect(approxEqAbs(
            v,
            F32x16{ 1.0, -1.0, -1.0, 1.0, 2.0, 2.0, 2.0, 4.0, 5.0, 6.0, 7.0, 8.0, 10.0, 11.0, 12.0, 13.0 },
            0.0,
        ));
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
        const vr8 = trunc(splat(F32x8, f));
        const fr8 = @trunc(splat(F32x8, f));
        const vr16 = trunc(splat(F32x16, f));
        const fr16 = @trunc(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        try expect(approxEqAbs(vr8, fr8, 0.0));
        try expect(approxEqAbs(vr16, fr16, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn floor(v: anytype) @TypeOf(v) {
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
        } else if (T == F32x16 and has_avx512f) {
            return asm ("vrndscaleps $1, %%zmm0, %%zmm0"
                : [ret] "={zmm0}" (-> T),
                : [v] "{zmm0}" (v),
            );
        } else if (T == F32x16 and !has_avx512f) {
            const arr: [16]f32 = v;
            var ymm0 = @as(F32x8, arr[0..8].*);
            var ymm1 = @as(F32x8, arr[8..16].*);
            ymm0 = asm ("vroundps $1, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> F32x8),
                : [v] "{ymm0}" (ymm0),
            );
            ymm1 = asm ("vroundps $1, %%ymm1, %%ymm1"
                : [ret] "={ymm1}" (-> F32x8),
                : [v] "{ymm1}" (ymm1),
            );
            return @shuffle(f32, ymm0, ymm1, [16]i32{ 0, 1, 2, 3, 4, 5, 6, 7, -1, -2, -3, -4, -5, -6, -7, -8 });
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
        try expect(all(0, floor(splat(F32x4, math.inf_f32)) == splat(F32x4, math.inf_f32)));
        try expect(all(0, floor(splat(F32x4, -math.inf_f32)) == splat(F32x4, -math.inf_f32)));
        try expect(all(0, isNan(floor(splat(F32x4, math.nan_f32)))));
        try expect(all(0, isNan(floor(splat(F32x4, -math.nan_f32)))));
        try expect(all(0, isNan(floor(splat(F32x4, math.qnan_f32)))));
        try expect(all(0, isNan(floor(splat(F32x4, -math.qnan_f32)))));
    }
    {
        var v = floor(F32x16{ 1.1, -1.1, -1.5, 1.5, 2.1, 2.8, 2.9, 4.1, 5.8, 6.1, 7.9, 8.9, 10.1, 11.2, 12.7, 13.1 });
        try expect(approxEqAbs(
            v,
            F32x16{ 1.0, -2.0, -2.0, 1.0, 2.0, 2.0, 2.0, 4.0, 5.0, 6.0, 7.0, 8.0, 10.0, 11.0, 12.0, 13.0 },
            0.0,
        ));
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
        const vr8 = floor(splat(F32x8, f));
        const fr8 = @floor(splat(F32x8, f));
        const vr16 = floor(splat(F32x16, f));
        const fr16 = @floor(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        try expect(approxEqAbs(vr8, fr8, 0.0));
        try expect(approxEqAbs(vr16, fr16, 0.0));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn ceil(v: anytype) @TypeOf(v) {
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
        } else if (T == F32x16 and has_avx512f) {
            return asm ("vrndscaleps $2, %%zmm0, %%zmm0"
                : [ret] "={zmm0}" (-> T),
                : [v] "{zmm0}" (v),
            );
        } else if (T == F32x16 and !has_avx512f) {
            const arr: [16]f32 = v;
            var ymm0 = @as(F32x8, arr[0..8].*);
            var ymm1 = @as(F32x8, arr[8..16].*);
            ymm0 = asm ("vroundps $2, %%ymm0, %%ymm0"
                : [ret] "={ymm0}" (-> F32x8),
                : [v] "{ymm0}" (ymm0),
            );
            ymm1 = asm ("vroundps $2, %%ymm1, %%ymm1"
                : [ret] "={ymm1}" (-> F32x8),
                : [v] "{ymm1}" (ymm1),
            );
            return @shuffle(f32, ymm0, ymm1, [16]i32{ 0, 1, 2, 3, 4, 5, 6, 7, -1, -2, -3, -4, -5, -6, -7, -8 });
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
        try expect(all(0, ceil(splat(F32x4, math.inf_f32)) == splat(F32x4, math.inf_f32)));
        try expect(all(0, ceil(splat(F32x4, -math.inf_f32)) == splat(F32x4, -math.inf_f32)));
        try expect(all(0, isNan(ceil(splat(F32x4, math.nan_f32)))));
        try expect(all(0, isNan(ceil(splat(F32x4, -math.nan_f32)))));
        try expect(all(0, isNan(ceil(splat(F32x4, math.qnan_f32)))));
        try expect(all(0, isNan(ceil(splat(F32x4, -math.qnan_f32)))));
    }
    {
        var v = ceil(F32x16{ 1.1, -1.1, -1.5, 1.5, 2.1, 2.8, 2.9, 4.1, 5.8, 6.1, 7.9, 8.9, 10.1, 11.2, 12.7, 13.1 });
        try expect(approxEqAbs(
            v,
            F32x16{ 2.0, -1.0, -1.0, 2.0, 3.0, 3.0, 3.0, 5.0, 6.0, 7.0, 8.0, 9.0, 11.0, 12.0, 13.0, 14.0 },
            0.0,
        ));
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
        const vr8 = ceil(splat(F32x8, f));
        const fr8 = @ceil(splat(F32x8, f));
        const vr16 = ceil(splat(F32x16, f));
        const fr16 = @ceil(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, 0.0));
        try expect(approxEqAbs(vr8, fr8, 0.0));
        try expect(approxEqAbs(vr16, fr16, 0.0));
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

pub inline fn swizzle(
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
    try expect(all(0, isNan(mod(splat(F32x4, math.inf_f32), splat(F32x4, 1.0)))));
    try expect(all(0, isNan(mod(splat(F32x4, -math.inf_f32), splat(F32x4, 123.456)))));
    try expect(all(0, isNan(mod(splat(F32x4, math.nan_f32), splat(F32x4, 123.456)))));
    try expect(all(0, isNan(mod(splat(F32x4, math.qnan_f32), splat(F32x4, 123.456)))));
    try expect(all(0, isNan(mod(splat(F32x4, -math.qnan_f32), splat(F32x4, 123.456)))));
    try expect(all(0, isNan(mod(splat(F32x4, 123.456), splat(F32x4, math.inf_f32)))));
    try expect(all(0, isNan(mod(splat(F32x4, 123.456), splat(F32x4, -math.inf_f32)))));
    try expect(all(0, isNan(mod(splat(F32x4, 123.456), splat(F32x4, math.nan_f32)))));
    try expect(all(0, isNan(mod(splat(F32x4, math.inf_f32), splat(F32x4, math.inf_f32)))));
    try expect(all(0, isNan(mod(splat(F32x4, math.inf_f32), splat(F32x4, math.nan_f32)))));
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
    try expect(approxEqAbs(sin(splat(F32x16, 89.123)), splat(F32x16, 0.916166), epsilon));
    try expect(all(0, isNan(sin(splat(F32x4, math.inf_f32)))) == true);
    try expect(all(0, isNan(sin(splat(F32x4, -math.inf_f32)))) == true);
    try expect(all(0, isNan(sin(splat(F32x4, math.nan_f32)))) == true);
    try expect(all(0, isNan(sin(splat(F32x4, math.qnan_f32)))) == true);

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = sin(splat(F32x4, f));
        const fr = @sin(splat(F32x4, f));
        const vr8 = sin(splat(F32x8, f));
        const fr8 = @sin(splat(F32x8, f));
        const vr16 = sin(splat(F32x16, f));
        const fr16 = @sin(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, epsilon));
        try expect(approxEqAbs(vr8, fr8, epsilon));
        try expect(approxEqAbs(vr16, fr16, epsilon));
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
    try expect(all(0, isNan(cos(splat(F32x4, math.inf_f32)))) == true);
    try expect(all(0, isNan(cos(splat(F32x4, -math.inf_f32)))) == true);
    try expect(all(0, isNan(cos(splat(F32x4, math.nan_f32)))) == true);
    try expect(all(0, isNan(cos(splat(F32x4, math.qnan_f32)))) == true);

    var f: f32 = -100.0;
    var i: u32 = 0;
    while (i < 100) : (i += 1) {
        const vr = cos(splat(F32x4, f));
        const fr = @cos(splat(F32x4, f));
        const vr8 = cos(splat(F32x8, f));
        const fr8 = @cos(splat(F32x8, f));
        const vr16 = cos(splat(F32x16, f));
        const fr16 = @cos(splat(F32x16, f));
        try expect(approxEqAbs(vr, fr, epsilon));
        try expect(approxEqAbs(vr8, fr8, epsilon));
        try expect(approxEqAbs(vr16, fr16, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

pub fn sincos(v: anytype) [2]@TypeOf(v) {
    const T = @TypeOf(v);
    return switch (T) {
        f32 => sincos32(v),
        F32x4, F32x8, F32x16 => sincos32xN(v),
        else => @compileError("zmath.sincos() not implemented for " ++ @typeName(T)),
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
        const sc16 = sincos(splat(F32x16, f));
        const s4 = @sin(splat(F32x4, f));
        const s8 = @sin(splat(F32x8, f));
        const s16 = @sin(splat(F32x16, f));
        const c4 = @cos(splat(F32x4, f));
        const c8 = @cos(splat(F32x8, f));
        const c16 = @cos(splat(F32x16, f));
        try expect(approxEqAbs(sc[0], s4, epsilon));
        try expect(approxEqAbs(sc8[0], s8, epsilon));
        try expect(approxEqAbs(sc16[0], s16, epsilon));
        try expect(approxEqAbs(sc[1], c4, epsilon));
        try expect(approxEqAbs(sc8[1], c8, epsilon));
        try expect(approxEqAbs(sc16[1], c16, epsilon));
        f += 0.12345 * @intToFloat(f32, i);
    }
}

// ------------------------------------------------------------------------------
//
// 3. 2D, 3D, 4D vector functions
//
// ------------------------------------------------------------------------------

pub inline fn dot2(v0: Vec, v1: Vec) F32x4 {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | -- | -- |
    var xmm1 = swizzle(xmm0, .y, .x, .x, .x); // | y0*y1 | -- | -- | -- |
    xmm0 = F32x4{ xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[3] }; // | x0*x1 + y0*y1 | -- | -- | -- |
    return swizzle(xmm0, .x, .x, .x, .x);
}
test "zmath.dot2" {
    const v0 = F32x4{ -1.0, 2.0, 300.0, -2.0 };
    const v1 = F32x4{ 4.0, 5.0, 600.0, 2.0 };
    var v = dot2(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 6.0), 0.0001));
}

pub inline fn dot3(v0: Vec, v1: Vec) F32x4 {
    var dot = v0 * v1;
    var temp = swizzle(dot, .y, .z, .y, .z);
    dot = F32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    temp = swizzle(temp, .y, .y, .y, .y);
    dot = F32x4{ dot[0] + temp[0], dot[1], dot[2], dot[2] }; // addss
    return swizzle(dot, .x, .x, .x, .x);
}
test "zmath.dot3" {
    const v0 = F32x4{ -1.0, 2.0, 3.0, 1.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 1.0 };
    var v = dot3(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 24.0), 0.0001));
}

pub inline fn dot4(v0: Vec, v1: Vec) F32x4 {
    var xmm0 = v0 * v1; // | x0*x1 | y0*y1 | z0*z1 | w0*w1 |
    var xmm1 = swizzle(xmm0, .y, .x, .w, .x); // | y0*y1 | -- | w0*w1 | -- |
    xmm1 = xmm0 + xmm1; // | x0*x1 + y0*y1 | -- | z0*z1 + w0*w1 | -- |
    xmm0 = swizzle(xmm1, .z, .x, .x, .x); // | z0*z1 + w0*w1 | -- | -- | -- |
    xmm0 = F32x4{ xmm0[0] + xmm1[0], xmm0[1], xmm0[2], xmm0[2] }; // addss
    return swizzle(xmm0, .x, .x, .x, .x);
}
test "zmath.dot4" {
    const v0 = F32x4{ -1.0, 2.0, 3.0, -2.0 };
    const v1 = F32x4{ 4.0, 5.0, 6.0, 2.0 };
    var v = dot4(v0, v1);
    try expect(approxEqAbs(v, splat(F32x4, 20.0), 0.0001));
}

pub inline fn cross3(v0: Vec, v1: Vec) Vec {
    var xmm0 = swizzle(v0, .y, .z, .x, .w);
    var xmm1 = swizzle(v1, .z, .x, .y, .w);
    var result = xmm0 * xmm1;
    xmm0 = swizzle(xmm0, .y, .z, .x, .w);
    xmm1 = swizzle(xmm1, .z, .x, .y, .w);
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

pub inline fn lengthSq2(v: Vec) F32x4 {
    return dot2(v, v);
}
pub inline fn lengthSq3(v: Vec) F32x4 {
    return dot3(v, v);
}
pub inline fn lengthSq4(v: Vec) F32x4 {
    return dot4(v, v);
}

pub inline fn length2(v: Vec) F32x4 {
    return sqrt(dot2(v, v));
}
pub inline fn length3(v: Vec) F32x4 {
    return sqrt(dot3(v, v));
}
pub inline fn length4(v: Vec) F32x4 {
    return sqrt(dot4(v, v));
}
test "zmath.length3" {
    {
        var v = length3(F32x4{ 1.0, -2.0, 3.0, 1000.0 });
        try expect(approxEqAbs(v, splat(F32x4, math.sqrt(14.0)), 0.001));
    }
    {
        var v = length3(F32x4{ 1.0, math.nan_f32, math.inf_f32, 1000.0 });
        try expect(all(0, isNan(v)));
    }
    {
        var v = length3(F32x4{ 1.0, math.inf_f32, 3.0, 1000.0 });
        try expect(all(0, isInf(v)));
    }
    {
        var v = length3(F32x4{ 3.0, 2.0, 1.0, math.nan_f32 });
        try expect(approxEqAbs(v, splat(F32x4, math.sqrt(14.0)), 0.001));
    }
}

pub inline fn normalize2(v: Vec) Vec {
    return v * splat(F32x4, 1.0) / sqrt(dot2(v, v));
}
pub inline fn normalize3(v: Vec) Vec {
    return v * splat(F32x4, 1.0) / sqrt(dot3(v, v));
}
pub inline fn normalize4(v: Vec) Vec {
    return v * splat(F32x4, 1.0) / sqrt(dot4(v, v));
}
test "zmath.normalize3" {
    {
        const v0 = F32x4{ 1.0, -2.0, 3.0, 1000.0 };
        var v = normalize3(v0);
        try expect(approxEqAbs(v, v0 * splat(F32x4, 1.0 / math.sqrt(14.0)), 0.0005));
    }
    {
        try expect(any(0, isNan(normalize3(F32x4{ 1.0, math.inf_f32, 1.0, 1.0 }))));
        try expect(any(0, isNan(normalize3(F32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 }))));
        try expect(any(0, isNan(normalize3(F32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 }))));
        try expect(any(0, isNan(normalize3(splat(F32x4, 0.0)))));
    }
}
test "zmath.normalize4" {
    {
        const v0 = F32x4{ 1.0, -2.0, 3.0, 10.0 };
        var v = normalize4(v0);
        try expect(approxEqAbs(v, v0 * splat(F32x4, 1.0 / math.sqrt(114.0)), 0.0005));
    }
    {
        try expect(any(0, isNan(normalize4(F32x4{ 1.0, math.inf_f32, 1.0, 1.0 }))));
        try expect(any(0, isNan(normalize4(F32x4{ -math.inf_f32, math.inf_f32, 0.0, 0.0 }))));
        try expect(any(0, isNan(normalize4(F32x4{ -math.nan_f32, math.qnan_f32, 0.0, 0.0 }))));
        try expect(any(0, isNan(normalize4(splat(F32x4, 0.0)))));
    }
}

// ------------------------------------------------------------------------------
//
// 4. Matrix functions
//
// ------------------------------------------------------------------------------

pub fn mul(a: anytype, b: anytype) @TypeOf(a) {
    const T = @TypeOf(a);
    if (T == Mat) {
        return mulMat(a, b);
    } else if (T == Quat) {
        return mulQuat(a, b);
    } else {
        @compileError("zmath.mul() not implemented for " ++ @typeName(T));
    }
}

fn mulMat(m0: Mat, m1: Mat) Mat {
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
pub fn translationV(v: Vec) Mat {
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
pub fn scalingV(v: Vec) Mat {
    return scaling(v[0], v[1], v[2]);
}

pub fn lookToLh(eye_pos: Vec, eye_dir: Vec, up_dir: Vec) Mat {
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
pub fn lookAtLh(eye_pos: Vec, focus_pos: Vec, up_dir: Vec) Mat {
    return lookToLh(eye_pos, focus_pos - eye_pos, up_dir);
}
test "zmath.matrix.lookToLh" {
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

pub fn determinant(m: Mat) F32x4 {
    var v0 = swizzle(m[2], .y, .x, .x, .x);
    var v1 = swizzle(m[3], .z, .z, .y, .y);
    var v2 = swizzle(m[2], .y, .x, .x, .x);
    var v3 = swizzle(m[3], .w, .w, .w, .z);
    var v4 = swizzle(m[2], .z, .z, .y, .y);
    var v5 = swizzle(m[3], .w, .w, .w, .z);

    var p0 = v0 * v1;
    var p1 = v2 * v3;
    var p2 = v4 * v5;

    v0 = swizzle(m[2], .z, .z, .y, .y);
    v1 = swizzle(m[3], .y, .x, .x, .x);
    v2 = swizzle(m[2], .w, .w, .w, .z);
    v3 = swizzle(m[3], .y, .x, .x, .x);
    v4 = swizzle(m[2], .w, .w, .w, .z);
    v5 = swizzle(m[3], .z, .z, .y, .y);

    p0 = mulAdd(-v0, v1, p0);
    p1 = mulAdd(-v2, v3, p1);
    p2 = mulAdd(-v4, v5, p2);

    v0 = swizzle(m[1], .w, .w, .w, .z);
    v1 = swizzle(m[1], .z, .z, .y, .y);
    v2 = swizzle(m[1], .y, .x, .x, .x);

    var s = m[0] * f32x4(1.0, -1.0, 1.0, -1.0);
    var r = v0 * p0;
    r = mulAdd(-v1, p1, r);
    r = mulAdd(v2, p2, r);
    return dot4(s, r);
}
test "zmath.matrix.determinant" {
    const m = Mat{
        f32x4(10.0, -9.0, -12.0, 1.0),
        f32x4(7.0, -12.0, 11.0, 1.0),
        f32x4(-10.0, 10.0, 3.0, 1.0),
        f32x4(1.0, 2.0, 3.0, 4.0),
    };
    try expect(approxEqAbs(determinant(m), splat(F32x4, 2939.0), 0.0001));
}

pub fn inverse(a: anytype) @TypeOf(a) {
    const T = @TypeOf(a);
    return switch (T) {
        Mat => inverseMat(a),
        Quat => inverseQuat(a),
        else => @compileError("zmath.inverse() not implemented for " ++ @typeName(T)),
    };
}

fn inverseMat(m: Mat) Mat {
    return inverseDet(m, null);
}

pub fn inverseDet(m: Mat, out_det: ?*F32x4) Mat {
    const mt = transpose(m);
    var v0: [4]F32x4 = undefined;
    var v1: [4]F32x4 = undefined;

    v0[0] = swizzle(mt[2], .x, .x, .y, .y);
    v1[0] = swizzle(mt[3], .z, .w, .z, .w);
    v0[1] = swizzle(mt[0], .x, .x, .y, .y);
    v1[1] = swizzle(mt[1], .z, .w, .z, .w);
    v0[2] = @shuffle(f32, mt[2], mt[0], [4]i32{ 0, 2, ~@as(i32, 0), ~@as(i32, 2) });
    v1[2] = @shuffle(f32, mt[3], mt[1], [4]i32{ 1, 3, ~@as(i32, 1), ~@as(i32, 3) });

    var d0 = v0[0] * v1[0];
    var d1 = v0[1] * v1[1];
    var d2 = v0[2] * v1[2];

    v0[0] = swizzle(mt[2], .z, .w, .z, .w);
    v1[0] = swizzle(mt[3], .x, .x, .y, .y);
    v0[1] = swizzle(mt[0], .z, .w, .z, .w);
    v1[1] = swizzle(mt[1], .x, .x, .y, .y);
    v0[2] = @shuffle(f32, mt[2], mt[0], [4]i32{ 1, 3, ~@as(i32, 1), ~@as(i32, 3) });
    v1[2] = @shuffle(f32, mt[3], mt[1], [4]i32{ 0, 2, ~@as(i32, 0), ~@as(i32, 2) });

    d0 = mulAdd(-v0[0], v1[0], d0);
    d1 = mulAdd(-v0[1], v1[1], d1);
    d2 = mulAdd(-v0[2], v1[2], d2);

    v0[0] = swizzle(mt[1], .y, .z, .x, .y);
    v1[0] = @shuffle(f32, d0, d2, [4]i32{ ~@as(i32, 1), 1, 3, 0 });
    v0[1] = swizzle(mt[0], .z, .x, .y, .x);
    v1[1] = @shuffle(f32, d0, d2, [4]i32{ 3, ~@as(i32, 1), 1, 2 });
    v0[2] = swizzle(mt[3], .y, .z, .x, .y);
    v1[2] = @shuffle(f32, d1, d2, [4]i32{ ~@as(i32, 3), 1, 3, 0 });
    v0[3] = swizzle(mt[2], .z, .x, .y, .x);
    v1[3] = @shuffle(f32, d1, d2, [4]i32{ 3, ~@as(i32, 3), 1, 2 });

    var c0 = v0[0] * v1[0];
    var c2 = v0[1] * v1[1];
    var c4 = v0[2] * v1[2];
    var c6 = v0[3] * v1[3];

    v0[0] = swizzle(mt[1], .z, .w, .y, .z);
    v1[0] = @shuffle(f32, d0, d2, [4]i32{ 3, 0, 1, ~@as(i32, 0) });
    v0[1] = swizzle(mt[0], .w, .z, .w, .y);
    v1[1] = @shuffle(f32, d0, d2, [4]i32{ 2, 1, ~@as(i32, 0), 0 });
    v0[2] = swizzle(mt[3], .z, .w, .y, .z);
    v1[2] = @shuffle(f32, d1, d2, [4]i32{ 3, 0, 1, ~@as(i32, 2) });
    v0[3] = swizzle(mt[2], .w, .z, .w, .y);
    v1[3] = @shuffle(f32, d1, d2, [4]i32{ 2, 1, ~@as(i32, 2), 0 });

    c0 = mulAdd(-v0[0], v1[0], c0);
    c2 = mulAdd(-v0[1], v1[1], c2);
    c4 = mulAdd(-v0[2], v1[2], c4);
    c6 = mulAdd(-v0[3], v1[3], c6);

    v0[0] = swizzle(mt[1], .w, .x, .w, .x);
    v1[0] = @shuffle(f32, d0, d2, [4]i32{ 2, ~@as(i32, 1), ~@as(i32, 0), 2 });
    v0[1] = swizzle(mt[0], .y, .w, .x, .z);
    v1[1] = @shuffle(f32, d0, d2, [4]i32{ ~@as(i32, 1), 0, 3, ~@as(i32, 0) });
    v0[2] = swizzle(mt[3], .w, .x, .w, .x);
    v1[2] = @shuffle(f32, d1, d2, [4]i32{ 2, ~@as(i32, 3), ~@as(i32, 2), 2 });
    v0[3] = swizzle(mt[2], .y, .w, .x, .z);
    v1[3] = @shuffle(f32, d1, d2, [4]i32{ ~@as(i32, 3), 0, 3, ~@as(i32, 2) });

    const c1 = mulAdd(-v0[0], v1[0], c0);
    const c3 = mulAdd(v0[1], v1[1], c2);
    const c5 = mulAdd(-v0[2], v1[2], c4);
    const c7 = mulAdd(v0[3], v1[3], c6);

    c0 = mulAdd(v0[0], v1[0], c0);
    c2 = mulAdd(-v0[1], v1[1], c2);
    c4 = mulAdd(v0[2], v1[2], c4);
    c6 = mulAdd(-v0[3], v1[3], c6);

    var mr = Mat{
        f32x4(c0[0], c1[1], c0[2], c1[3]),
        f32x4(c2[0], c3[1], c2[2], c3[3]),
        f32x4(c4[0], c5[1], c4[2], c5[3]),
        f32x4(c6[0], c7[1], c6[2], c7[3]),
    };

    const det = dot4(mr[0], mt[0]);
    if (out_det != null) {
        out_det.?.* = det;
    }

    if (math.approxEqAbs(f32, det[0], 0.0, 0.0001)) {
        return .{
            f32x4(0.0, 0.0, 0.0, 0.0),
            f32x4(0.0, 0.0, 0.0, 0.0),
            f32x4(0.0, 0.0, 0.0, 0.0),
            f32x4(0.0, 0.0, 0.0, 0.0),
        };
    }

    const scale = splat(F32x4, 1.0) / det;
    mr[0] *= scale;
    mr[1] *= scale;
    mr[2] *= scale;
    mr[3] *= scale;
    return mr;
}
test "zmath.matrix.inverse" {
    const m = Mat{
        f32x4(10.0, -9.0, -12.0, 1.0),
        f32x4(7.0, -12.0, 11.0, 1.0),
        f32x4(-10.0, 10.0, 3.0, 1.0),
        f32x4(1.0, 2.0, 3.0, 4.0),
    };
    var det: F32x4 = undefined;
    const mi = inverseDet(m, &det);
    try expect(approxEqAbs(det, splat(F32x4, 2939.0), 0.0001));

    try expect(approxEqAbs(mi[0], f32x4(-0.170806, -0.13576, -0.349439, 0.164001), 0.0001));
    try expect(approxEqAbs(mi[1], f32x4(-0.163661, -0.14801, -0.253147, 0.141204), 0.0001));
    try expect(approxEqAbs(mi[2], f32x4(-0.0871045, 0.00646478, -0.0785982, 0.0398095), 0.0001));
    try expect(approxEqAbs(mi[3], f32x4(0.18986, 0.103096, 0.272882, 0.10854), 0.0001));
}

pub fn matFromNormAxisAngle(axis: Vec, angle: f32) Mat {
    const sincos_angle = sincos(angle);

    const c2 = splat(F32x4, 1.0 - sincos_angle[1]);
    const c1 = splat(F32x4, sincos_angle[1]);
    const c0 = splat(F32x4, sincos_angle[0]);

    const n0 = swizzle(axis, .y, .z, .x, .w);
    const n1 = swizzle(axis, .z, .x, .y, .w);

    var v0 = c2 * n0 * n1;
    const r0 = c2 * axis * axis + c1;
    const r1 = c0 * axis + v0;
    var r2 = v0 - c0 * axis;

    v0 = @bitCast(F32x4, @bitCast(U32x4, r0) & u32x4_mask3);

    var v1 = @shuffle(f32, r1, r2, [4]i32{ 0, 2, ~@as(i32, 1), ~@as(i32, 2) });
    v1 = swizzle(v1, .y, .z, .w, .x);

    var v2 = @shuffle(f32, r1, r2, [4]i32{ 1, 1, ~@as(i32, 0), ~@as(i32, 0) });
    v2 = swizzle(v2, .x, .z, .x, .z);

    r2 = @shuffle(f32, v0, v1, [4]i32{ 0, 3, ~@as(i32, 0), ~@as(i32, 1) });
    r2 = swizzle(r2, .x, .z, .w, .y);

    var m: Mat = undefined;
    m[0] = r2;

    r2 = @shuffle(f32, v0, v1, [4]i32{ 1, 3, ~@as(i32, 2), ~@as(i32, 3) });
    r2 = swizzle(r2, .z, .x, .w, .y);
    m[1] = r2;

    v2 = @shuffle(f32, v2, v0, [4]i32{ 0, 1, ~@as(i32, 2), ~@as(i32, 3) });
    m[2] = v2;
    m[3] = f32x4(0.0, 0.0, 0.0, 1.0);
    return m;
}
pub fn matFromAxisAngle(axis: Vec, angle: f32) Mat {
    assert(!all(3, axis == splat(F32x4, 0.0)));
    assert(!all(3, isInf(axis)));
    const normal = normalize3(axis);
    return matFromNormAxisAngle(normal, angle);
}
test "zmath.matrix.matFromAxisAngle" {
    {
        const m0 = matFromAxisAngle(f32x4(1.0, 0.0, 0.0, 0.0), math.pi * 0.25);
        const m1 = rotationX(math.pi * 0.25);
        try expect(approxEqAbs(m0[0], m1[0], 0.001));
        try expect(approxEqAbs(m0[1], m1[1], 0.001));
        try expect(approxEqAbs(m0[2], m1[2], 0.001));
        try expect(approxEqAbs(m0[3], m1[3], 0.001));
    }
    {
        const m0 = matFromAxisAngle(f32x4(0.0, 1.0, 0.0, 0.0), math.pi * 0.125);
        const m1 = rotationY(math.pi * 0.125);
        try expect(approxEqAbs(m0[0], m1[0], 0.001));
        try expect(approxEqAbs(m0[1], m1[1], 0.001));
        try expect(approxEqAbs(m0[2], m1[2], 0.001));
        try expect(approxEqAbs(m0[3], m1[3], 0.001));
    }
    {
        const m0 = matFromAxisAngle(f32x4(0.0, 0.0, 1.0, 0.0), math.pi * 0.333);
        const m1 = rotationZ(math.pi * 0.333);
        try expect(approxEqAbs(m0[0], m1[0], 0.001));
        try expect(approxEqAbs(m0[1], m1[1], 0.001));
        try expect(approxEqAbs(m0[2], m1[2], 0.001));
        try expect(approxEqAbs(m0[3], m1[3], 0.001));
    }
}

pub fn matFromQuat(quat: Quat) Mat {
    var q0 = quat + quat;
    var q1 = quat * q0;

    var v0 = swizzle(q1, .y, .x, .x, .w);
    v0 = @bitCast(F32x4, @bitCast(U32x4, v0) & u32x4_mask3);

    var v1 = swizzle(q1, .z, .z, .y, .w);
    v1 = @bitCast(F32x4, @bitCast(U32x4, v1) & u32x4_mask3);

    var r0 = (f32x4(1.0, 1.0, 1.0, 0.0) - v0) - v1;

    v0 = swizzle(quat, .x, .x, .y, .w);
    v1 = swizzle(q0, .z, .y, .z, .w);
    v0 = v0 * v1;

    v1 = swizzle(quat, .w, .w, .w, .w);
    var v2 = swizzle(q0, .y, .z, .x, .w);
    v1 = v1 * v2;

    var r1 = v0 + v1;
    var r2 = v0 - v1;

    v0 = @shuffle(f32, r1, r2, [4]i32{ 1, 2, ~@as(i32, 0), ~@as(i32, 1) });
    v0 = swizzle(v0, .x, .z, .w, .y);
    v1 = @shuffle(f32, r1, r2, [4]i32{ 0, 0, ~@as(i32, 2), ~@as(i32, 2) });
    v1 = swizzle(v1, .x, .z, .x, .z);

    q1 = @shuffle(f32, r0, v0, [4]i32{ 0, 3, ~@as(i32, 0), ~@as(i32, 1) });
    q1 = swizzle(q1, .x, .z, .w, .y);

    var m: Mat = undefined;
    m[0] = q1;

    q1 = @shuffle(f32, r0, v0, [4]i32{ 1, 3, ~@as(i32, 2), ~@as(i32, 3) });
    q1 = swizzle(q1, .z, .x, .w, .y);
    m[1] = q1;

    q1 = @shuffle(f32, v1, r0, [4]i32{ 0, 1, ~@as(i32, 2), ~@as(i32, 3) });
    m[2] = q1;
    m[3] = f32x4(0.0, 0.0, 0.0, 1.0);
    return m;
}
test "zmath.matrix.matFromQuat" {
    {
        const m = matFromQuat(f32x4(0.0, 0.0, 0.0, 1.0));
        try expect(approxEqAbs(m[0], f32x4(1.0, 0.0, 0.0, 0.0), 0.0001));
        try expect(approxEqAbs(m[1], f32x4(0.0, 1.0, 0.0, 0.0), 0.0001));
        try expect(approxEqAbs(m[2], f32x4(0.0, 0.0, 1.0, 0.0), 0.0001));
        try expect(approxEqAbs(m[3], f32x4(0.0, 0.0, 0.0, 1.0), 0.0001));
    }
}

// ------------------------------------------------------------------------------
//
// 5. Quaternion functions
//
// ------------------------------------------------------------------------------

pub fn quatToMat(quat: Quat) Mat {
    return matFromQuat(quat);
}

fn mulQuat(q0: Quat, q1: Quat) Quat {
    var result = swizzle(q1, .w, .w, .w, .w);
    var q1x = swizzle(q1, .x, .x, .x, .x);
    var q1y = swizzle(q1, .y, .y, .y, .y);
    var q1z = swizzle(q1, .z, .z, .z, .z);
    result = result * q0;
    var q0_shuf = swizzle(q0, .w, .z, .y, .x);
    q1x = q1x * q0_shuf;
    q0_shuf = swizzle(q0_shuf, .y, .x, .w, .z);
    result = mulAdd(q1x, f32x4(1.0, -1.0, 1.0, -1.0), result);
    q1y = q1y * q0_shuf;
    q0_shuf = swizzle(q0_shuf, .w, .z, .y, .x);
    q1y = q1y * f32x4(1.0, 1.0, -1.0, -1.0);
    q1z = q1z * q0_shuf;
    q1y = mulAdd(q1z, f32x4(-1.0, 1.0, 1.0, -1.0), q1y);
    return result + q1y;
}
test "zmath.quaternion.mul" {
    {
        const q0 = f32x4(2.0, 3.0, 4.0, 1.0);
        const q1 = f32x4(3.0, 2.0, 1.0, 4.0);
        try expect(approxEqAbs(mul(q0, q1), f32x4(16.0, 4.0, 22.0, -12.0), 0.0001));
    }
}

pub fn quatFromNormAxisAngle(axis: Vec, angle: f32) Quat {
    var n = f32x4(axis[0], axis[1], axis[2], 1.0);
    const sc = sincos(0.5 * angle);
    return n * f32x4(sc[0], sc[0], sc[0], sc[1]);
}
pub fn quatFromAxisAngle(axis: Vec, angle: f32) Quat {
    assert(!all(3, axis == splat(F32x4, 0.0)));
    assert(!all(3, isInf(axis)));
    const normal = normalize3(axis);
    return quatFromNormAxisAngle(normal, angle);
}
test "zmath.quaternion.quatFromNormAxisAngle" {
    {
        const q0 = quatFromAxisAngle(f32x4(1.0, 0.0, 0.0, 0.0), 0.25 * math.pi);
        const q1 = quatFromAxisAngle(f32x4(0.0, 1.0, 0.0, 0.0), 0.125 * math.pi);
        const m0 = rotationX(0.25 * math.pi);
        const m1 = rotationY(0.125 * math.pi);
        const mr0 = quatToMat(mul(q0, q1));
        const mr1 = mul(m0, m1);
        try expect(approxEqAbs(mr0[0], mr1[0], 0.0001));
        try expect(approxEqAbs(mr0[1], mr1[1], 0.0001));
        try expect(approxEqAbs(mr0[2], mr1[2], 0.0001));
        try expect(approxEqAbs(mr0[3], mr1[3], 0.0001));
    }
    {
        const m0 = quatToMat(quatFromAxisAngle(f32x4(1.0, 2.0, 0.5, 0.0), 0.25 * math.pi));
        const m1 = matFromAxisAngle(f32x4(1.0, 2.0, 0.5, 0.0), 0.25 * math.pi);
        try expect(approxEqAbs(m0[0], m1[0], 0.0001));
        try expect(approxEqAbs(m0[1], m1[1], 0.0001));
        try expect(approxEqAbs(m0[2], m1[2], 0.0001));
        try expect(approxEqAbs(m0[3], m1[3], 0.0001));
    }
}

pub fn conjugate(quat: Quat) Quat {
    return quat * f32x4(-1.0, -1.0, -1.0, 1.0);
}

fn inverseQuat(quat: Quat) Quat {
    const l = lengthSq4(quat);
    const conj = conjugate(quat);
    return select(l <= splat(F32x4, math.f32_epsilon), splat(F32x4, 0.0), conj / l);
}
test "zmath.quaternion.inverse" {
    try expect(approxEqAbs(
        inverse(f32x4(2.0, 3.0, 4.0, 1.0)),
        f32x4(-1.0 / 15.0, -1.0 / 10.0, -2.0 / 15.0, 1.0 / 30.0),
        0.0001,
    ));
}

// ------------------------------------------------------------------------------
//
// X. Misc functions
//
// ------------------------------------------------------------------------------

pub inline fn linePointDistance(line_pt0: Vec, line_pt1: Vec, pt: Vec) F32x4 {
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

// ------------------------------------------------------------------------------
//
// Private functions and constants
//
// ------------------------------------------------------------------------------

const f32x4_0x8000_0000: F32x4 = splatInt(F32x4, 0x8000_0000);
const f32x4_0x7fff_ffff: F32x4 = splatInt(F32x4, 0x7fff_ffff);
const f32x4_inf: F32x4 = splat(F32x4, math.inf_f32);
const u32x4_mask3: U32x4 = U32x4{ 0xffff_ffff, 0xffff_ffff, 0xffff_ffff, 0 };

inline fn splatNegativeZero(comptime T: type) T {
    return @splat(veclen(T), @bitCast(f32, @as(u32, 0x8000_0000)));
}
inline fn splatNoFraction(comptime T: type) T {
    return @splat(veclen(T), @as(f32, 8_388_608.0));
}
inline fn splatAbsMask(comptime T: type) T {
    return @splat(veclen(T), @bitCast(f32, @as(u32, 0x7fff_ffff)));
}

fn floatToIntAndBack(v: anytype) @TypeOf(v) {
    // This routine won't handle nan, inf and numbers greater than 8_388_608.0 (will generate undefined values)
    @setRuntimeSafety(false);

    const T = @TypeOf(v);
    const len = veclen(T);

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

fn approxEqAbs(v0: anytype, v1: anytype, eps: f32) bool {
    const T = @TypeOf(v0);
    comptime var i: comptime_int = 0;
    inline while (i < veclen(T)) : (i += 1) {
        if (!math.approxEqAbs(f32, v0[i], v1[i], eps)) {
            return false;
        }
    }
    return true;
}
