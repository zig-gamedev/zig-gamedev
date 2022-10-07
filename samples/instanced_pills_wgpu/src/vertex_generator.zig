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
        const angle = lerp(3.0 * tau / 4.0, tau / 4.0, @intToFloat(f32, i) / @intToFloat(f32, vertex_data.len - 1));
        vertex_data[i] = .{
            .position = .{ @round(@cos(angle) * 100) / 100.0, @round(@sin(angle) * 100) / 100.0 },
        };
    }
    var up = (segments + 1) / 2;
    var down = up - 1;
    i = 0;
    while (i <= segments) : (i += 2) {
        index_data[i] = up;
        if (i == segments) {
            break;
        }
        index_data[i + 1] = down;
        up += 1;
        if (down > 0) {
            down -= 1;
        }
    }
}

test "generate left disc" {
    const segments = 7;
    const vertex_count = segments + 1;
    var vertex_data: [vertex_count]Vertex = undefined;
    var index_data: [vertex_count]u32 = undefined;
    pill(segments, &vertex_data, &index_data);
    try expectApproxEqAbs(vertex_data[0].position[0], 0.0, 0.1);
    try expectApproxEqAbs(-vertex_data[0].position[1], 1.0, 0.1);
    try expectApproxEqAbs(vertex_data[segments].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[segments].position[1], 1.0, 0.1);
    try expectEqual(index_data, .{ 4, 3, 5, 2, 6, 1, 7, 0 });
}
