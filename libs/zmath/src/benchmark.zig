// -------------------------------------------------------------------------------------------------
// zmath - benchmarks
// -------------------------------------------------------------------------------------------------
// 'zig build benchmark' in the root project directory will build and run 'ReleaseFast' configuration.
//
// -------------------------------------------------------------------------------------------------
// 'AMD Ryzen 9 3950X 16-Core Processor', Windows 11, Zig 0.10.0-dev.2620+0e9458a3f
// -------------------------------------------------------------------------------------------------
//                matrix mul benchmark (AOS) - scalar version: 1.5880s, zmath version: 1.0642s
//       cross3, scale, bias benchmark (AOS) - scalar version: 0.9318s, zmath version: 0.6888s
// cross3, dot3, scale, bias benchmark (AOS) - scalar version: 1.2258s, zmath version: 1.1095s
//            quaternion mul benchmark (AOS) - scalar version: 1.4123s, zmath version: 0.6958s
//                      wave benchmark (SOA) - scalar version: 4.8165s, zmath version: 0.7338s
//
// -------------------------------------------------------------------------------------------------
// 'AMD Ryzen 7 5800X 8-Core Processer', Linux 5.17.14, Zig 0.10.0-dev.2624+d506275a0
// -------------------------------------------------------------------------------------------------
//                matrix mul benchmark (AOS) - scalar version: 1.3672s, zmath version: 0.8617s
//       cross3, scale, bias benchmark (AOS) - scalar version: 0.6586s, zmath version: 0.4803s
// cross3, dot3, scale, bias benchmark (AOS) - scalar version: 1.0620s, zmath version: 0.8942s
//            quaternion mul benchmark (AOS) - scalar version: 1.1324s, zmath version: 0.6064s
//                      wave benchmark (SOA) - scalar version: 3.6598s, zmath version: 0.4231s
//
// -------------------------------------------------------------------------------------------------
// 'Apple M1 Max', macOS Version 12.4, Zig 0.10.0-dev.2657+74442f350
// -------------------------------------------------------------------------------------------------
//                matrix mul benchmark (AOS) - scalar version: 1.0297s, zmath version: 1.0538s
//       cross3, scale, bias benchmark (AOS) - scalar version: 0.6294s, zmath version: 0.6532s
// cross3, dot3, scale, bias benchmark (AOS) - scalar version: 0.9807s, zmath version: 1.0988s
//            quaternion mul benchmark (AOS) - scalar version: 1.5413s, zmath version: 0.7800s
//                      wave benchmark (SOA) - scalar version: 3.4220s, zmath version: 1.0255s
//
// -------------------------------------------------------------------------------------------------
// '11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz', Windows 11, Zig 0.10.0-dev.2620+0e9458a3f
// -------------------------------------------------------------------------------------------------
//                matrix mul benchmark (AOS) - scalar version: 2.2308s, zmath version: 0.9376s
//       cross3, scale, bias benchmark (AOS) - scalar version: 1.0821s, zmath version: 0.5110s
// cross3, dot3, scale, bias benchmark (AOS) - scalar version: 1.6580s, zmath version: 0.9167s
//            quaternion mul benchmark (AOS) - scalar version: 2.0139s, zmath version: 0.5856s
//                      wave benchmark (SOA) - scalar version: 3.7832s, zmath version: 0.3642s
//
// -------------------------------------------------------------------------------------------------

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

    // d = sqrt(x * x + z * z); y = sin(d - t); SOA layout.
    try waveBenchmark(allocator, 1_000);
}

const std = @import("std");
const time = std.time;
const Timer = time.Timer;
const zm = @import("zmath");

var prng = std.rand.DefaultPrng.init(0);
const random = prng.random();

noinline fn mat4MulBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    std.debug.print("\n", .{});
    std.debug.print("{s:>42} - ", .{"matrix mul benchmark (AOS)"});

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
    std.debug.print("{s:>42} - ", .{"cross3, scale, bias benchmark (AOS)"});

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
    std.debug.print("{s:>42} - ", .{"cross3, dot3, scale, bias benchmark (AOS)"});

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
    std.debug.print("{s:>42} - ", .{"quaternion mul benchmark (AOS)"});

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

noinline fn waveBenchmark(allocator: std.mem.Allocator, comptime count: comptime_int) !void {
    _ = allocator;
    std.debug.print("{s:>42} - ", .{"wave benchmark (SOA)"});

    const grid_size = 1024;
    {
        var t: f32 = 0.0;

        const scale: f32 = 0.05;

        var timer = try Timer.start();
        const start = timer.lap();

        var iter: usize = 0;
        while (iter < count) : (iter += 1) {
            var z_index: i32 = 0;
            while (z_index < grid_size) : (z_index += 1) {
                const z = scale * @intToFloat(f32, z_index - grid_size / 2);

                var x_index: i32 = 0;
                while (x_index < grid_size) : (x_index += 4) {
                    const x0 = scale * @intToFloat(f32, x_index + 0 - grid_size / 2);
                    const x1 = scale * @intToFloat(f32, x_index + 1 - grid_size / 2);
                    const x2 = scale * @intToFloat(f32, x_index + 2 - grid_size / 2);
                    const x3 = scale * @intToFloat(f32, x_index + 3 - grid_size / 2);

                    const d0 = zm.sqrt(x0 * x0 + z * z);
                    const d1 = zm.sqrt(x1 * x1 + z * z);
                    const d2 = zm.sqrt(x2 * x2 + z * z);
                    const d3 = zm.sqrt(x3 * x3 + z * z);

                    const y0 = zm.sin(d0 - t);
                    const y1 = zm.sin(d1 - t);
                    const y2 = zm.sin(d2 - t);
                    const y3 = zm.sin(d3 - t);

                    std.mem.doNotOptimizeAway(&y0);
                    std.mem.doNotOptimizeAway(&y1);
                    std.mem.doNotOptimizeAway(&y2);
                    std.mem.doNotOptimizeAway(&y3);
                }
            }
            t += 0.001;
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("scalar version: {d:.4}s, ", .{elapsed_s});
    }

    {
        const T = zm.F32x16;

        const static = struct {
            const offsets = [16]f32{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        };
        const voffset = zm.load(static.offsets[0..], T, 0);
        var vt = zm.splat(T, 0.0);

        const scale: f32 = 0.05;

        var timer = try Timer.start();
        const start = timer.lap();

        var iter: usize = 0;
        while (iter < count) : (iter += 1) {
            var z_index: i32 = 0;
            while (z_index < grid_size) : (z_index += 1) {
                const z = scale * @intToFloat(f32, z_index - grid_size / 2);
                const vz = zm.splat(T, z);

                var x_index: i32 = 0;
                while (x_index < grid_size) : (x_index += zm.veclen(T)) {
                    const x = scale * @intToFloat(f32, x_index - grid_size / 2);
                    const vx = zm.splat(T, x) + voffset * zm.splat(T, scale);

                    const d = zm.sqrt(vx * vx + vz * vz);

                    const vy = zm.sin(d - vt);

                    std.mem.doNotOptimizeAway(&vy);
                }
            }
            vt += zm.splat(T, 0.001);
        }
        const end = timer.read();
        const elapsed_s = @intToFloat(f64, end - start) / time.ns_per_s;

        std.debug.print("zmath version: {d:.4}s\n", .{elapsed_s});
    }
}
