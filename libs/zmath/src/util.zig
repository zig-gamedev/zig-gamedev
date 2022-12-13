// ==============================================================================
//
// Collection of useful functions building on top of, and extending, core zmath.
// https://github.com/michal-z/zig-gamedev/tree/main/libs/zmath
//
// ------------------------------------------------------------------------------
// 1. Matrix functions
// ------------------------------------------------------------------------------
//
// matTranslation(m: Mat) Vec
// matForward(m: Mat) Vec
// matUp(m: Mat) Vec
// matRight(m: Mat) Vec
//
//
// ------------------------------------------------------------------------------
// 2. Angle functions
// ------------------------------------------------------------------------------
//
// degToRad(angle: f32) f32
// radToDeg(angle: f32) f32
// angleMod(angle: f32) f32
//
// ==============================================================================

const zm = @import("zmath.zig");
const std = @import("std");
const math = std.math;
const expect = std.testing.expect;

pub fn matTranslation(m: zm.Mat) zm.Vec {
    return m[3];
}

pub fn matForward(m: zm.Mat) zm.Vec {
    return zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), m));
}

pub fn matUp(m: zm.Mat) zm.Vec {
    return zm.normalize3(zm.mul(zm.f32x4(0.0, 1.0, 0.0, 0.0), m));
}

pub fn matRight(m: zm.Mat) zm.Vec {
    return zm.normalize3(zm.mul(zm.f32x4(1.0, 0.0, 0.0, 0.0), m));
}

test "zmath.util.mat.translation" {
    // zig fmt: off
    const a = [18]f32{
        1.0,
        2.0, 3.0, 4.0, 5.0,
        6.0, 7.0, 8.0, 9.0,
        10.0,11.0, 12.0,13.0,
        14.0, 15.0, 16.0, 17.0,
        18.0,
    };
    // zig fmt: on
    const m = zm.loadMat(a[1..]);
    const translation = matTranslation(m);
    try expect(zm.approxEqAbs(translation, zm.f32x4(14.0, 15.0, 16.0, 17.0), 0.01));
}

test "zmath.util.mat.forward" {
    var a = zm.identity();
    var forward = matForward(a);
    try expect(zm.approxEqAbs(forward, zm.f32x4(0.0, 0.0, 1.0, 0), 0.01));
    const rot_yaw = zm.rotationY(degToRad(90));
    a = zm.mul(a, rot_yaw);
    forward = matForward(a);
    try expect(zm.approxEqAbs(forward, zm.f32x4(1.0, 0.0, 0.0, 0), 0.01));
}

test "zmath.util.mat.up" {
    var a = zm.identity();
    var up = matUp(a);
    try expect(zm.approxEqAbs(up, zm.f32x4(0.0, 1.0, 0.0, 0), 0.01));
    const rot_yaw = zm.rotationY(degToRad(90));
    a = zm.mul(a, rot_yaw);
    up = matUp(a);
    try expect(zm.approxEqAbs(up, zm.f32x4(0.0, 1.0, 0.0, 0), 0.01));
    const rot_pitch = zm.rotationX(degToRad(90));
    a = zm.mul(a, rot_pitch);
    up = matUp(a);
    try expect(zm.approxEqAbs(up, zm.f32x4(0.0, 0.0, 1.0, 0), 0.01));
}

test "zmath.util.mat.right" {
    var a = zm.identity();
    var right = matRight(a);
    try expect(zm.approxEqAbs(right, zm.f32x4(1.0, 0.0, 0.0, 0), 0.01));
    const rot_yaw = zm.rotationY(degToRad(90));
    a = zm.mul(a, rot_yaw);
    right = matRight(a);
    try expect(zm.approxEqAbs(right, zm.f32x4(0.0, 0.0, -1.0, 0), 0.01));
}

// ------------------------------------------------------------------------------
//
// 2. Angle functions
//
// ------------------------------------------------------------------------------

pub fn degToRad(angle: f32) f32 {
    return angle * math.pi / 180.0;
}

pub fn radToDeg(angle: f32) f32 {
    return angle * 180.0 / math.pi;
}

pub fn angleMod(angle: f32) f32 {
    const mod = @mod(angle, 2 * math.pi);
    return mod;
}

test "zmath.util.angle" {
    try expect(math.approxEqAbs(f32, degToRad(0), 0, 0.01));
    try expect(math.approxEqAbs(f32, degToRad(360), math.pi * 2, 0.01));
    try expect(math.approxEqAbs(f32, radToDeg(0), 0, 0.01));
    try expect(math.approxEqAbs(f32, radToDeg(math.pi * 2), 360, 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(0)), degToRad(0), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(0.5)), degToRad(0.5), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(1)), degToRad(1), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(361.5)), degToRad(1.5), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(721)), degToRad(1), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(-1.5)), degToRad(358.5), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(-359)), degToRad(1), 0.01));
    try expect(math.approxEqAbs(f32, angleMod(degToRad(-361)), degToRad(359), 0.01));
}

// ------------------------------------------------------------------------------
// This software is available under 2 licenses -- choose whichever you prefer.
// ------------------------------------------------------------------------------
// ALTERNATIVE A - MIT License
// Copyright (c) 2022 Michal Ziulek
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ------------------------------------------------------------------------------
// ALTERNATIVE B - Public Domain (www.unlicense.org)
// This is free and unencumbered software released into the public domain.
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
// software, either in source code form or as a compiled binary, for any purpose,
// commercial or non-commercial, and by any means.
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be an
// overt act of relinquishment in perpetuity of all present and future rights to
// this software under copyright law.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ------------------------------------------------------------------------------
