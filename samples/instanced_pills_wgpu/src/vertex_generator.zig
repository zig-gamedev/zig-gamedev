const std = @import("std");
const expectEqual = std.testing.expectEqual;
const expectApproxEqAbs = std.testing.expectApproxEqAbs;
const math = std.math;

const tau = 2 * math.pi;

pub const Vertex = struct {
    position: [2]f32,
    side: f32,
};

fn lerp(a: f32, b: f32, t: f32) f32 {
    return a * (1 - t) + b * t;
}

pub fn pill(segments: u16, vertex_data: []Vertex, index_data: []u16) void {
    {
        var i: usize = 0;
        while (i <= segments) : (i += 1) {
            const angle = lerp(3.0 * tau / 4.0, tau / 4.0, @as(f32, @floatFromInt(i)) / @as(f32, @floatFromInt(segments)));
            vertex_data[i] = .{
                .position = .{ @cos(angle), @sin(angle) },
                .side = -1.0,
            };
        }
    }
    for (vertex_data[0..(segments + 1)], 0..) |v, i| {
        vertex_data[vertex_data.len - 1 - i] = .{
            .position = .{ -v.position[0], v.position[1] },
            .side = 1.0,
        };
    }
    {
        var up = (segments + 1) / 2;
        var down = up - 1;
        var i: usize = 0;
        while (i < index_data.len) : (i += 2) {
            index_data[i] = up;
            if (i == index_data.len - 1) {
                break;
            }
            index_data[i + 1] = down;
            up += 1;
            if (down == 0) {
                down = @as(u16, @intCast(index_data.len));
            }
            down -= 1;
        }
    }
}

test "generate 6 segment pill" {
    var vertex_data: [14]Vertex = undefined;
    var index_data: [14]u16 = undefined;
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
