const std = @import("std");
const expectEqual = std.testing.expectEqual;
const expectApproxEqAbs = std.testing.expectApproxEqAbs;
const math = std.math;

const tau = 2 * math.pi;

pub const Vertex = struct {
    position: [2]f32,
};

fn lerp(a: f32, b: f32, t: f32) f32 {
    return a * (1 - t) + b * t;
}

pub fn pill(segments: u32, vertex_data: []Vertex, index_data: []u32) void {
    var i: usize = 0;
    while (i <= segments) : (i += 1) {
        const angle = lerp(3.0 * tau / 4.0, tau / 4.0, @intToFloat(f32, i) / @intToFloat(f32, segments));
        vertex_data[i] = .{
            .position = .{ @cos(angle), @sin(angle) },
        };
    }
    i = 0;
    while (i <= segments) : (i += 1) {
        vertex_data[i + segments + 1] = .{
            .position = .{ -vertex_data[segments - i].position[0], vertex_data[segments - i].position[1] },
        };
    }

    var up = (segments + 1) / 2;
    var down = up - 1;
    i = 0;
    while (i < index_data.len) : (i += 2) {
        index_data[i] = up;
        if (i == index_data.len - 1) {
            break;
        }
        index_data[i + 1] = down;
        up += 1;
        if (down == 0) {
            down = @intCast(u32, index_data.len);
        }
        down -= 1;
    }
}

test "generate 6 segment pill" {
    var vertex_data: [14]Vertex = undefined;
    var index_data: [14]u32 = undefined;
    pill(6, &vertex_data, &index_data);
    try expectApproxEqAbs(vertex_data[0].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[0].position[1], -1.0, 0.1);
    try expectApproxEqAbs(vertex_data[3].position[0], -1.0, 0.1);
    try expectApproxEqAbs(vertex_data[3].position[1], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[6].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[6].position[1], 1.0, 0.1);

    try expectApproxEqAbs(vertex_data[7].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[7].position[1], 1.0, 0.1);
    try expectApproxEqAbs(vertex_data[10].position[0], 1.0, 0.1);
    try expectApproxEqAbs(vertex_data[10].position[1], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[13].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[13].position[1], -1.0, 0.1);

    try expectEqual(index_data, .{ 3, 2, 4, 1, 5, 0, 6, 13, 7, 12, 8, 11, 9, 10 });
}
