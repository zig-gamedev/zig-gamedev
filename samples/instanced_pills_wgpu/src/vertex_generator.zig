const std = @import("std");
const expectApproxEqAbs = std.testing.expectApproxEqAbs;
const math = std.math;

const tau = 2 * math.pi;

pub const Vertex = struct {
    position: [2]f32,
};

fn lerp(a: f32, b: f32, t: f32) f32 {
    return a * (1 - t) + b * t;
}

pub fn leftDisc(vertex_data: []Vertex, index_data: []u32) void {
    for (vertex_data) |*vertex, i| {
        const angle = lerp(3.0 * tau / 4.0, tau / 4.0, @intToFloat(f32, i) / @intToFloat(f32, vertex_data.len - 1));
        vertex.* = .{
            .position = .{ @round(@cos(angle) * 100) / 100.0, @round(@sin(angle) * 100) / 100.0 },
        };
    }
    index_data[0] = 1;
    index_data[1] = 0;
    index_data[2] = 2;
    index_data[3] = 6;
    index_data[4] = 3;
    index_data[5] = 5;
    index_data[6] = 4;
}

test "generate left disc" {
    const segments = 6;
    const vertex_count = segments + 1;
    var vertex_data: [vertex_count]Vertex = undefined;
    var index_data: [vertex_count]u32 = undefined;
    leftDisc(&vertex_data, &index_data);
    try expectApproxEqAbs(vertex_data[0].position[0], 0.0, 0.1);
    try expectApproxEqAbs(-vertex_data[0].position[1], 1.0, 0.1);
    try expectApproxEqAbs(vertex_data[segments].position[0], 0.0, 0.1);
    try expectApproxEqAbs(vertex_data[segments].position[1], 1.0, 0.1);
}
