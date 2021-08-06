const std = @import("std");
const assert = std.debug.assert;
const math = std.math;

pub const Vec3 = [3]f32;
pub const Mat4 = [4][4]f32;

pub fn scalarModAngle(in_angle: f32) f32 {
    const angle = in_angle + math.pi;
    var temp: f32 = math.fabs(angle);
    temp = temp - (2.0 * math.pi * @intToFloat(f32, @floatToInt(i32, temp / math.pi)));
    temp = temp - math.pi;
    if (angle < 0.0) {
        temp = -temp;
    }
    return temp;
}

pub fn vec3Dot(a: Vec3, b: Vec3) f32 {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

pub fn vec3Cross(a: Vec3, b: Vec3) Vec3 {
    return .{
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    };
}

pub fn vec3Add(a: Vec3, b: Vec3) Vec3 {
    return .{ a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

pub fn vec3Sub(a: Vec3, b: Vec3) Vec3 {
    return .{ a[0] - b[0], a[1] - b[1], a[2] - b[2] };
}

pub fn vec3Scale(a: Vec3, b: f32) Vec3 {
    return .{ a[0] * b, a[1] * b, a[2] * b };
}

pub fn vec3Init(x: f32, y: f32, z: f32) Vec3 {
    return .{ x, y, z };
}

pub fn vec3Length(a: Vec3) f32 {
    return math.sqrt(vec3Dot(a, a));
}

pub fn vec3Normalize(a: Vec3) Vec3 {
    const len = vec3Length(a);
    assert(!math.approxEq(f32, len, 0.0, 0.0001));
    const rcplen = 1.0 / len;
    return .{ rcplen * a[0], rcplen * a[1], rcplen * a[2] };
}

pub fn vec3Transform(a: Vec3, b: Mat4) Vec3 {
    return .{
        a[0] * b[0][0] + a[1] * b[1][0] + a[2] * b[2][0] + b[3][0],
        a[0] * b[0][1] + a[1] * b[1][1] + a[2] * b[2][1] + b[3][1],
        a[0] * b[0][2] + a[1] * b[1][2] + a[2] * b[2][2] + b[3][2],
    };
}

pub fn vec3TransformNormal(a: Vec3, b: Mat4) Vec3 {
    return .{
        a[0] * b[0][0] + a[1] * b[1][0] + a[2] * b[2][0],
        a[0] * b[0][1] + a[1] * b[1][1] + a[2] * b[2][1],
        a[0] * b[0][2] + a[1] * b[1][2] + a[2] * b[2][2],
    };
}

pub fn mat4Transpose(a: Mat4) Mat4 {
    return .{
        [_]f32{ a[0][0], a[1][0], a[2][0], a[3][0] },
        [_]f32{ a[0][1], a[1][1], a[2][1], a[3][1] },
        [_]f32{ a[0][2], a[1][2], a[2][2], a[3][2] },
        [_]f32{ a[0][3], a[1][3], a[2][3], a[3][3] },
    };
}

pub fn mat4Mul(a: Mat4, b: Mat4) Mat4 {
    return .{
        [_]f32{
            a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
            a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
            a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
            a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3],
        },
        [_]f32{
            a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
            a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
            a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
            a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3],
        },
        [_]f32{
            a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
            a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
            a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
            a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3],
        },
        [_]f32{
            a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0],
            a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1],
            a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2],
            a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3],
        },
    };
}

pub fn mat4InitRotationX(angle: f32) Mat4 {
    const sinv = math.sin(angle);
    const cosv = math.cos(angle);
    return .{
        [_]f32{ 1.0, 0.0, 0.0, 0.0 },
        [_]f32{ 0.0, cosv, sinv, 0.0 },
        [_]f32{ 0.0, -sinv, cosv, 0.0 },
        [_]f32{ 0.0, 0.0, 0.0, 1.0 },
    };
}

pub fn mat4InitRotationY(angle: f32) Mat4 {
    const sinv = math.sin(angle);
    const cosv = math.cos(angle);
    return .{
        [_]f32{ cosv, 0.0, -sinv, 0.0 },
        [_]f32{ 0.0, 1.0, 0.0, 0.0 },
        [_]f32{ sinv, 0.0, cosv, 0.0 },
        [_]f32{ 0.0, 0.0, 0.0, 1.0 },
    };
}

pub fn mat4InitRotationZ(angle: f32) Mat4 {
    const sinv = math.sin(angle);
    const cosv = math.cos(angle);
    return .{
        [_]f32{ cosv, sinv, 0.0, 0.0 },
        [_]f32{ -sinv, cosv, 0.0, 0.0 },
        [_]f32{ 0.0, 0.0, 1.0, 0.0 },
        [_]f32{ 0.0, 0.0, 0.0, 1.0 },
    };
}

pub fn mat4InitPerspectiveFovLh(fovy: f32, aspect: f32, near: f32, far: f32) Mat4 {
    const sinfov = math.sin(0.5 * fovy);
    const cosfov = math.cos(0.5 * fovy);

    assert(near > 0.0 and far > 0.0 and far > near);
    assert(!math.approxEq(f32, sinfov, 0.0, 0.0001));
    assert(!math.approxEq(f32, far, near, 0.001));
    assert(!math.approxEq(f32, aspect, 0.0, 0.01));

    const h = cosfov / sinfov;
    const w = h / aspect;
    const r = far / (far - near);
    return .{
        [_]f32{ w, 0.0, 0.0, 0.0 },
        [_]f32{ 0.0, h, 0.0, 0.0 },
        [_]f32{ 0.0, 0.0, r, 1.0 },
        [_]f32{ 0.0, 0.0, -r * near, 0.0 },
    };
}

pub fn mat4InitIdentity() Mat4 {
    return .{
        [_]f32{ 1.0, 0.0, 0.0, 0.0 },
        [_]f32{ 0.0, 1.0, 0.0, 0.0 },
        [_]f32{ 0.0, 0.0, 1.0, 0.0 },
        [_]f32{ 0.0, 0.0, 0.0, 1.0 },
    };
}

pub fn mat4InitTranslation(a: Vec3) Mat4 {
    return .{
        [_]f32{ 1.0, 0.0, 0.0, 0.0 },
        [_]f32{ 0.0, 1.0, 0.0, 0.0 },
        [_]f32{ 0.0, 0.0, 1.0, 0.0 },
        [_]f32{ a[0], a[1], a[2], 1.0 },
    };
}

pub fn mat4InitLookAt(eye: Vec3, at: Vec3, up: Vec3) Mat4 {
    const az = vec3Normalize(vec3Sub(at, eye));
    const ax = vec3Normalize(vec3Cross(up, az));
    const ay = vec3Normalize(vec3Cross(az, ax));
    return .{
        [_]f32{ ax[0], ay[0], az[0], 0.0 },
        [_]f32{ ax[1], ay[1], az[1], 0.0 },
        [_]f32{ ax[2], ay[2], az[2], 0.0 },
        [_]f32{ -vec3Dot(ax, eye), -vec3Dot(ay, eye), -vec3Dot(az, eye), 1.0 },
    };
}

pub fn mat4InitOrthoOffCenterLh(
    view_left: f32,
    view_right: f32,
    view_bottom: f32,
    view_top: f32,
    near_z: f32,
    far_z: f32,
) Mat4 {
    assert(!math.approxEq(f32, view_right, view_left, 0.00001));
    assert(!math.approxEq(f32, view_top, view_bottom, 0.00001));
    assert(!math.approxEq(f32, far_z, near_z, 0.00001));

    const rcp_width = 1.0 / (view_right - view_left);
    const rcp_height = 1.0 / (view_top - view_bottom);
    const range = 1.0 / (far_z - near_z);

    return .{
        [_]f32{ rcp_width + rcp_width, 0.0, 0.0, 0.0 },
        [_]f32{ 0.0, rcp_height + rcp_height, 0.0, 0.0 },
        [_]f32{ 0.0, 0.0, range, 0.0 },
        [_]f32{ -(view_left + view_right) * rcp_width, -(view_top + view_bottom) * rcp_height, -range * near_z, 1.0 },
    };
}
