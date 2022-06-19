// 'zig build benchmark' in the root project directory will build and run 'ReleaseFast' configuration.
//
// Results on '11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz', Windows 11, Zig 0.10.0-dev.2620+0e9458a3f:
//
// 'zig build benchmark'
//                    matrix mul benchmark - scalar version: 2.2236s, zmath version: 0.9359s
//           cross3, scale, bias benchmark - scalar version: 1.0806s, zmath version: 0.5136s
//     cross3, dot3, scale, bias benchmark - scalar version: 1.6541s, zmath version: 0.9155s
//                quaternion mul benchmark - scalar version: 2.0128s, zmath version: 0.5862s
//
// 'zig build benchmark -Dcpu=x86_64'
//                    matrix mul benchmark - scalar version: 1.3909s, zmath version: 1.3477s
//           cross3, scale, bias benchmark - scalar version: 1.0801s, zmath version: 0.5884s
//     cross3, dot3, scale, bias benchmark - scalar version: 1.6541s, zmath version: 0.9695s
//                quaternion mul benchmark - scalar version: 1.6879s, zmath version: 0.7287s
//
// Notice that, when compiling for 'x86_64' target, compiler is able to auto-vectorize scalar
// matrix multiplication *but* 'zmath' version is still a bit faster *and* vectorized for all compile targets,
// giving consistent performance results in all cases.

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // m = mul(ma, mb); data set fits in L1 cache; AOS data layout.
    try mat4MulBenchmark(allocator, 100_000);

    // v = 0.01 * cross3(va, vb) + vec3(1.0); data set fits in L1 cache; AOS data layout.
    try cross3ScaleBiasBenchmark(allocator, 10_000);

    // v = dot3(va, vb) * (0.1 * cross3(va, vb) + vec3(1.0)); data set fits in L1 cache; AOS data layout.
    try cross3Dot3ScaleBiasBenchmark(allocator, 10_000);

    // q = qmul(qa, qb); data set fits in L1 cache; AOS data layout.
    try quatBenchmark(allocator, 10_000);
}

const std = @import("std");
const time = std.time;
const Timer = time.Timer;
const zm = @import("zmath");

var prng = std.rand.DefaultPrng.init(0);
const random = prng.random();

noinline fn mat4MulBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    std.debug.print("{s:>40} - ", .{"matrix mul benchmark"});

    var data0 = std.ArrayList([16]f32).init(allocator);
    defer data0.deinit();
    var data1 = std.ArrayList([16]f32).init(allocator);
    defer data1.deinit();

    var i: usize = 0;
    while (i < 64) : (i += 1) {
        try data0.append([16]f32{
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
        });
        try data1.append([16]f32{
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
            random.float(f32), random.float(f32), random.float(f32), random.float(f32),
        });
    }

    // Warmup, fills L1 cache.
    i = 0;
    while (i < 100) : (i += 1) {
        for (data1.items) |b| {
            for (data0.items) |a| {
                const ma = zm.loadMat(a[0..]);
                const mb = zm.loadMat(b[0..]);
                const r = zm.mul(ma, mb);
                std.mem.doNotOptimizeAway(&r);
            }
        }
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const r = [16]f32{
                        a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
                        a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
                        a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
                        a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
                        a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
                        a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
                        a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
                        a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
                        a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
                        a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
                        a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
                        a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
                        a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
                        a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
                        a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
                        a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],
                    };
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("scalar version: {d:.4}s, ", .{elapsed_s});
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const ma = zm.loadMat(a[0..]);
                    const mb = zm.loadMat(b[0..]);
                    const r = zm.mul(ma, mb);
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("zmath version: {d:.4}s\n", .{elapsed_s});
    }
}

noinline fn cross3ScaleBiasBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    std.debug.print("{s:>40} - ", .{"cross3, scale, bias benchmark"});

    var data0 = std.ArrayList([3]f32).init(allocator);
    defer data0.deinit();
    var data1 = std.ArrayList([3]f32).init(allocator);
    defer data1.deinit();

    var i: usize = 0;
    while (i < 256) : (i += 1) {
        try data0.append([3]f32{ random.float(f32), random.float(f32), random.float(f32) });
        try data1.append([3]f32{ random.float(f32), random.float(f32), random.float(f32) });
    }

    // Warmup, fills L1 cache.
    i = 0;
    while (i < 100) : (i += 1) {
        for (data1.items) |b| {
            for (data0.items) |a| {
                const va = zm.loadArr3(a);
                const vb = zm.loadArr3(b);
                const cp = zm.f32x4s(0.01) * zm.cross3(va, vb) + zm.f32x4s(1.0);
                std.mem.doNotOptimizeAway(&cp);
            }
        }
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const r = [3]f32{
                        0.01 * (a[1] * b[2] - a[2] * b[1]) + 1.0,
                        0.01 * (a[2] * b[0] - a[0] * b[2]) + 1.0,
                        0.01 * (a[0] * b[1] - a[1] * b[0]) + 1.0,
                    };
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("scalar version: {d:.4}s, ", .{elapsed_s});
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const va = zm.loadArr3(a);
                    const vb = zm.loadArr3(b);
                    const cp = zm.f32x4s(0.01) * zm.cross3(va, vb) + zm.f32x4s(1.0);
                    std.mem.doNotOptimizeAway(&cp);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("zmath version: {d:.4}s\n", .{elapsed_s});
    }
}

noinline fn cross3Dot3ScaleBiasBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    std.debug.print("{s:>40} - ", .{"cross3, dot3, scale, bias benchmark"});

    var data0 = std.ArrayList([3]f32).init(allocator);
    defer data0.deinit();
    var data1 = std.ArrayList([3]f32).init(allocator);
    defer data1.deinit();

    var i: usize = 0;
    while (i < 256) : (i += 1) {
        try data0.append([3]f32{ random.float(f32), random.float(f32), random.float(f32) });
        try data1.append([3]f32{ random.float(f32), random.float(f32), random.float(f32) });
    }

    // Warmup, fills L1 cache.
    i = 0;
    while (i < 100) : (i += 1) {
        for (data1.items) |b| {
            for (data0.items) |a| {
                const va = zm.loadArr3(a);
                const vb = zm.loadArr3(b);
                const r = (zm.dot3(va, vb) * (zm.f32x4s(0.1) * zm.cross3(va, vb) + zm.f32x4s(1.0)))[0];
                std.mem.doNotOptimizeAway(&r);
            }
        }
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const d = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
                    const r = [3]f32{
                        d * (0.1 * (a[1] * b[2] - a[2] * b[1]) + 1.0),
                        d * (0.1 * (a[2] * b[0] - a[0] * b[2]) + 1.0),
                        d * (0.1 * (a[0] * b[1] - a[1] * b[0]) + 1.0),
                    };
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("scalar version: {d:.4}s, ", .{elapsed_s});
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const va = zm.loadArr3(a);
                    const vb = zm.loadArr3(b);
                    const r = zm.dot3(va, vb) * (zm.f32x4s(0.1) * zm.cross3(va, vb) + zm.f32x4s(1.0));
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("zmath version: {d:.4}s\n", .{elapsed_s});
    }
}

noinline fn quatBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    std.debug.print("{s:>40} - ", .{"quaternion mul benchmark"});

    var data0 = std.ArrayList([4]f32).init(allocator);
    defer data0.deinit();
    var data1 = std.ArrayList([4]f32).init(allocator);
    defer data1.deinit();

    var i: usize = 0;
    while (i < 256) : (i += 1) {
        try data0.append([4]f32{ random.float(f32), random.float(f32), random.float(f32), random.float(f32) });
        try data1.append([4]f32{ random.float(f32), random.float(f32), random.float(f32), random.float(f32) });
    }

    // Warmup, fills L1 cache.
    i = 0;
    while (i < 100) : (i += 1) {
        for (data1.items) |b| {
            for (data0.items) |a| {
                const va = zm.loadArr4(a);
                const vb = zm.loadArr4(b);
                const r = zm.qmul(va, vb);
                std.mem.doNotOptimizeAway(&r);
            }
        }
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const r = [4]f32{
                        (b[3] * a[0]) + (b[0] * a[3]) + (b[1] * a[2]) - (b[2] * a[1]),
                        (b[3] * a[1]) - (b[0] * a[2]) + (b[1] * a[3]) + (b[2] * a[0]),
                        (b[3] * a[2]) + (b[0] * a[1]) - (b[1] * a[0]) + (b[2] * a[3]),
                        (b[3] * a[3]) - (b[0] * a[0]) - (b[1] * a[1]) - (b[2] * a[2]),
                    };
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("scalar version: {d:.4}s, ", .{elapsed_s});
    }

    {
        i = 0;
        var timer = try Timer.start();
        const start = timer.lap();
        while (i < count) : (i += 1) {
            for (data1.items) |b| {
                for (data0.items) |a| {
                    const va = zm.loadArr4(a);
                    const vb = zm.loadArr4(b);
                    const r = zm.qmul(va, vb);
                    std.mem.doNotOptimizeAway(&r);
                }
            }
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("zmath version: {d:.4}s\n", .{elapsed_s});
    }
}
