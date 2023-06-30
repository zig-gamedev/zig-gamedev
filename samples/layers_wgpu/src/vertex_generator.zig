const std = @import("std");
const expectApproxEqAbs = std.testing.expectApproxEqAbs;
const expectEqualSlices = std.testing.expectEqualSlices;
const math = std.math;
const Vertex = @import("pill.zig").Vertex;

const tau = 2 * math.pi;

fn lerp(a: f32, b: f32, t: f32) f32 {
    return a * (1 - t) + b * t;
}

pub fn generateVertices(segments: u16, verticies: *std.ArrayList(Vertex)) !void {
    {
        var i: usize = 0;
        while (i <= segments) : (i += 1) {
            const angle = lerp(3.0 * tau / 4.0, tau / 4.0, @as(f32, @floatFromInt(i)) / @as(f32, @floatFromInt(segments)));
            try verticies.append(.{
                .position = .{ @cos(angle), @sin(angle) },
                .side = -1.0,
            });
        }
    }
    {
        const total = 2 * (segments + 1);
        var i: usize = segments + 1;
        while (i < total) : (i += 1) {
            const v = verticies.items[total - i - 1];
            try verticies.append(.{
                .position = .{ -v.position[0], v.position[1] },
                .side = 1.0,
            });
        }
    }
}

pub fn generateIndices(segments: u16, indicies: *std.ArrayList(u16)) !void {
    const total = 2 * (segments + 1);
    var up = (segments + 1) / 2;
    var down = up - 1;
    var i: usize = 0;
    while (i < total) : (i += 2) {
        try indicies.append(up);
        if (i == total - 1) {
            break;
        }
        try indicies.append(down);
        up += 1;
        if (down == 0) {
            down = total;
        }
        down -= 1;
    }
}

test "generate 6 segment vertices" {
    var verticies = std.ArrayList(Vertex).init(std.testing.allocator);
    defer verticies.deinit();
    try generateVertices(6, &verticies);

    try expectApproxEqAbs(verticies.items[0].position[0], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[0].position[1], -1.0, 0.1);
    try expectApproxEqAbs(verticies.items[3].position[0], -1.0, 0.1);
    try expectApproxEqAbs(verticies.items[3].position[1], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[6].position[0], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[6].position[1], 1.0, 0.1);

    try expectApproxEqAbs(verticies.items[6].side, -1.0, 0.1);
    try expectApproxEqAbs(verticies.items[7].side, 1.0, 0.1);

    try expectApproxEqAbs(verticies.items[7].position[0], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[7].position[1], 1.0, 0.1);
    try expectApproxEqAbs(verticies.items[10].position[0], 1.0, 0.1);
    try expectApproxEqAbs(verticies.items[10].position[1], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[13].position[0], 0.0, 0.1);
    try expectApproxEqAbs(verticies.items[13].position[1], -1.0, 0.1);
}

test "generate 6 segment indices" {
    var indices = std.ArrayList(u16).init(std.testing.allocator);
    defer indices.deinit();
    try generateIndices(6, &indices);
    const expected = [_]u16{ 3, 2, 4, 1, 5, 0, 6, 13, 7, 12, 8, 11, 9, 10 };
    try expectEqualSlices(u16, indices.items, expected[0..]);
}
