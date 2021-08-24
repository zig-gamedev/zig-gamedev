const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const utf8ToUtf16LeStringLiteral = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: simple2d";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *w.ID2D1SolidColorBrush,
    textformat: *w.IDWriteTextFormat,
    ellipse: *w.ID2D1EllipseGeometry,
    stroke_style: *w.ID2D1StrokeStyle,
    path: *w.ID2D1PathGeometry,
    ink: *w.ID2D1Ink,
    ink_style: *w.ID2D1InkStyle,
    bezier_lines_path: *w.ID2D1PathGeometry,
    noise_path: *w.ID2D1PathGeometry,

    ink_points: std.ArrayList(w.D2D1_POINT_2F),
};

fn init(allocator: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

    var ink_points = std.ArrayList(w.D2D1_POINT_2F).init(allocator);

    const brush = blk: {
        var maybe_brush: ?*w.ID2D1SolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &w.D2D1_COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &maybe_brush,
        ));
        break :blk maybe_brush.?;
    };
    const textformat = blk: {
        var maybe_textformat: ?*w.IDWriteTextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            utf8ToUtf16LeStringLiteral("Verdana"),
            null,
            w.DWRITE_FONT_WEIGHT.NORMAL,
            w.DWRITE_FONT_STYLE.NORMAL,
            w.DWRITE_FONT_STRETCH.NORMAL,
            32.0,
            utf8ToUtf16LeStringLiteral("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
    hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

    const ellipse = blk: {
        var maybe_ellipse: ?*w.ID2D1EllipseGeometry = null;
        hrPanicOnFail(grfx.d2d.factory.CreateEllipseGeometry(
            &.{ .point = .{ .x = 1210.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
            &maybe_ellipse,
        ));
        break :blk maybe_ellipse.?;
    };
    const stroke_style = blk: {
        var maybe_stroke_style: ?*w.ID2D1StrokeStyle = null;
        hrPanicOnFail(grfx.d2d.factory.CreateStrokeStyle(
            &.{
                .startCap = .ROUND,
                .endCap = .ROUND,
                .dashCap = .FLAT,
                .lineJoin = .MITER,
                .miterLimit = 0.0,
                .dashStyle = .SOLID,
                .dashOffset = 0.0,
            },
            null,
            0,
            &maybe_stroke_style,
        ));
        break :blk maybe_stroke_style.?;
    };
    const path = blk: {
        var path: *w.ID2D1PathGeometry = undefined;
        hrPanicOnFail(grfx.d2d.factory.CreatePathGeometry(@ptrCast(*?*w.ID2D1PathGeometry, &path)));

        var sink: *w.ID2D1GeometrySink = undefined;
        hrPanicOnFail(path.Open(@ptrCast(*?*w.ID2D1GeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.BeginFigure(.{ .x = 500.0, .y = 400.0 }, .FILLED);
        sink.AddLine(.{ .x = 600.0, .y = 300.0 });
        sink.AddLine(.{ .x = 700.0, .y = 400.0 });
        sink.AddLine(.{ .x = 800.0, .y = 300.0 });
        sink.AddBezier(&.{
            .point1 = .{ .x = 850.0, .y = 350.0 },
            .point2 = .{ .x = 850.0, .y = 350.0 },
            .point3 = .{ .x = 900.0, .y = 300.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 920.0, .y = 280.0 },
            .point2 = .{ .x = 950.0, .y = 320.0 },
            .point3 = .{ .x = 1000.0, .y = 300.0 },
        });
        sink.EndFigure(.OPEN);
        break :blk path;
    };
    const ink_style = blk: {
        var ink_style: *w.ID2D1InkStyle = undefined;
        hrPanicOnFail(grfx.d2d.context.CreateInkStyle(
            &.{ .nibShape = .ROUND, .nibTransform = w.D2D1_MATRIX_3X2_F.initIdentity() },
            @ptrCast(*?*w.ID2D1InkStyle, &ink_style),
        ));
        break :blk ink_style;
    };
    const ink = blk: {
        var ink: *w.ID2D1Ink = undefined;
        hrPanicOnFail(grfx.d2d.context.CreateInk(
            &.{ .x = 0.0, .y = 0.0, .radius = 0.0 },
            @ptrCast(*?*w.ID2D1Ink, &ink),
        ));

        var p0 = w.D2D1_POINT_2F{ .x = 0.0, .y = 0.0 };
        ink_points.append(p0) catch unreachable;
        ink.SetStartPoint(&.{ .x = p0.x, .y = p0.y, .radius = 1.0 });
        const sp = ink.GetStartPoint();
        assert(sp.x == p0.x and sp.y == p0.y and sp.radius == 1.0);
        assert(ink.GetSegmentCount() == 0);

        {
            const p1 = w.D2D1_POINT_2F{ .x = 200.0, .y = 0.0 };
            const cp1 = w.D2D1_POINT_2F{ .x = p0.x - 40.0, .y = p0.y + 140.0 };
            const cp2 = w.D2D1_POINT_2F{ .x = p1.x + 40.0, .y = p1.y - 140.0 };
            ink_points.append(cp1) catch unreachable;
            ink_points.append(cp2) catch unreachable;
            hrPanicOnFail(ink.AddSegments(&[_]w.D2D1_INK_BEZIER_SEGMENT{.{
                .point1 = .{ .x = cp1.x, .y = cp1.y, .radius = 12.5 },
                .point2 = .{ .x = cp2.x, .y = cp2.y, .radius = 12.5 },
                .point3 = .{ .x = p1.x, .y = p1.y, .radius = 9.0 },
            }}, 1));
            ink_points.append(p1) catch unreachable;
        }

        p0 = ink_points.items[ink_points.items.len - 1];
        {
            const p1 = w.D2D1_POINT_2F{ .x = 400.0, .y = 0.0 };
            const cp1 = w.D2D1_POINT_2F{ .x = p0.x - 40.0, .y = p0.y + 140.0 };
            const cp2 = w.D2D1_POINT_2F{ .x = p1.x + 40.0, .y = p1.y - 140.0 };
            ink_points.append(cp1) catch unreachable;
            ink_points.append(cp2) catch unreachable;
            hrPanicOnFail(ink.AddSegments(&[_]w.D2D1_INK_BEZIER_SEGMENT{.{
                .point1 = .{ .x = cp1.x, .y = cp1.y, .radius = 6.25 },
                .point2 = .{ .x = cp2.x, .y = cp2.y, .radius = 6.25 },
                .point3 = .{ .x = p1.x, .y = p1.y, .radius = 1.0 },
            }}, 1));
            ink_points.append(p1) catch unreachable;
        }

        break :blk ink;
    };
    const bezier_lines_path = blk: {
        var bezier_lines_path: *w.ID2D1PathGeometry = undefined;
        hrPanicOnFail(grfx.d2d.factory.CreatePathGeometry(@ptrCast(*?*w.ID2D1PathGeometry, &bezier_lines_path)));

        var sink: *w.ID2D1GeometrySink = undefined;
        hrPanicOnFail(bezier_lines_path.Open(@ptrCast(*?*w.ID2D1GeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.BeginFigure(.{ .x = 0.0, .y = 0.0 }, .FILLED);
        sink.AddLines(ink_points.items.ptr + 1, @intCast(u32, ink_points.items.len - 1));
        sink.EndFigure(.OPEN);
        break :blk bezier_lines_path;
    };
    const noise_path = blk: {
        var points = std.ArrayList(w.D2D1_POINT_2F).init(allocator);
        defer points.deinit();

        var i: u32 = 0;
        while (i < 100) : (i += 1) {
            const frac = @intToFloat(f32, (i + 1)) / 100.0;
            const y = 150.0 * c.stb_perlin_fbm_noise3(4.0 * frac, 0.0, 0.0, 2.0, 0.5, 2);
            points.append(.{ .x = 400.0 * frac, .y = y }) catch unreachable;
        }
        points.append(.{ .x = 400.0, .y = 100.0 }) catch unreachable;

        var noise_path: *w.ID2D1PathGeometry = undefined;
        hrPanicOnFail(grfx.d2d.factory.CreatePathGeometry(@ptrCast(*?*w.ID2D1PathGeometry, &noise_path)));

        var sink: *w.ID2D1GeometrySink = undefined;
        hrPanicOnFail(noise_path.Open(@ptrCast(*?*w.ID2D1GeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.BeginFigure(.{ .x = 0.0, .y = 100.0 }, .FILLED);
        sink.AddLines(points.items.ptr, @intCast(u32, points.items.len));
        sink.EndFigure(.CLOSED);
        break :blk noise_path;
    };

    grfx.beginFrame();

    var gui = gr.GuiContext.init(allocator, &grfx);

    grfx.finishGpuCommands();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .textformat = textformat,
        .ellipse = ellipse,
        .stroke_style = stroke_style,
        .path = path,
        .ink_style = ink_style,
        .ink = ink,
        .ink_points = ink_points,
        .bezier_lines_path = bezier_lines_path,
        .noise_path = noise_path,
    };
}

fn drawShapes(demo: DemoState) void {
    var grfx = &demo.grfx;

    grfx.d2d.context.DrawLine(
        .{ .x = 20.0, .y = 200.0 },
        .{ .x = 120.0, .y = 300.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    grfx.d2d.context.DrawLine(
        .{ .x = 160.0, .y = 300.0 },
        .{ .x = 260.0, .y = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    grfx.d2d.context.DrawLine(
        .{ .x = 300.0, .y = 200.0 },
        .{ .x = 400.0, .y = 300.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );

    grfx.d2d.context.DrawRectangle(
        &.{ .left = 500.0, .top = 100.0, .right = 600.0, .bottom = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        5.0,
        null,
    );
    grfx.d2d.context.FillRectangle(
        &.{ .left = 610.0, .top = 100.0, .right = 710.0, .bottom = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );
    grfx.d2d.context.FillRoundedRectangle(
        &.{
            .rect = .{ .left = 830.0, .top = 100.0, .right = 930.0, .bottom = 200.0 },
            .radiusX = 20.0,
            .radiusY = 20.0,
        },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );

    grfx.d2d.context.DrawEllipse(
        &.{ .point = .{ .x = 990.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        5.0,
        null,
    );
    grfx.d2d.context.FillEllipse(
        &.{ .point = .{ .x = 1100.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );
    grfx.d2d.context.DrawGeometry(
        @ptrCast(*w.ID2D1Geometry, demo.ellipse),
        @ptrCast(*w.ID2D1Brush, demo.brush),
        7.0,
        null,
    );
    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(110.0, 0.0));
    grfx.d2d.context.FillGeometry(@ptrCast(*w.ID2D1Geometry, demo.ellipse), @ptrCast(*w.ID2D1Brush, demo.brush), null);
    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initIdentity());

    demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 0.2, .g = 0.4, .b = 0.8, .a = 1.0 });
    var i: u32 = 0;
    while (i < 5) : (i += 1) {
        grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(0.0, @intToFloat(f32, i) * 50.0));
        grfx.d2d.context.DrawGeometry(
            @ptrCast(*w.ID2D1Geometry, demo.path),
            @ptrCast(*w.ID2D1Brush, demo.brush),
            15.0,
            null,
        );
    }

    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(500.0, 800.0));
    grfx.d2d.context.DrawInk(demo.ink, @ptrCast(*w.ID2D1Brush, demo.brush), demo.ink_style);

    demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
    grfx.d2d.context.DrawGeometry(
        @ptrCast(*w.ID2D1Geometry, demo.bezier_lines_path),
        @ptrCast(*w.ID2D1Brush, demo.brush),
        3.0,
        null,
    );

    demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 0.8, .g = 0.0, .b = 0.0, .a = 1.0 });
    for (demo.ink_points.items) |cp| {
        grfx.d2d.context.FillEllipse(
            &.{ .point = cp, .radiusX = 9.0, .radiusY = 9.0 },
            @ptrCast(*w.ID2D1Brush, demo.brush),
        );
    }

    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(750.0, 900.0));
    grfx.d2d.context.DrawInk(demo.ink, @ptrCast(*w.ID2D1Brush, demo.brush), demo.ink_style);

    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(1000.0, 600.0));

    demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 0.2, .g = 0.4, .b = 0.8, .a = 1.0 });
    grfx.d2d.context.FillGeometry(
        @ptrCast(*w.ID2D1Geometry, demo.noise_path),
        @ptrCast(*w.ID2D1Brush, demo.brush),
        null,
    );
    demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 0.8, .g = 0.0, .b = 0.0, .a = 1.0 });
    grfx.d2d.context.DrawGeometry(
        @ptrCast(*w.ID2D1Geometry, demo.noise_path),
        @ptrCast(*w.ID2D1Brush, demo.brush),
        8.0,
        null,
    );

    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initIdentity());
}

fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    _ = demo.ellipse.Release();
    _ = demo.stroke_style.Release();
    _ = demo.path.Release();
    _ = demo.ink.Release();
    _ = demo.ink_style.Release();
    _ = demo.bezier_lines_path.Release();
    _ = demo.noise_path.Release();
    demo.ink_points.deinit();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(allocator);
    lib.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    lib.updateWindow(demo.frame_stats.delta_time);

    c.igShowDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    demo.gui.draw(grfx);

    grfx.beginDraw2d();
    {
        const stats = &demo.frame_stats;
        var buffer = [_]u8{0} ** 64;
        const text = std.fmt.bufPrint(
            buffer[0..],
            "FPS: {d:.1}\nCPU time: {d:.3} ms",
            .{ stats.fps, stats.average_cpu_time },
        ) catch unreachable;

        demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        grfx.d2d.context.DrawTextSimple(
            text,
            demo.textformat,
            &w.D2D1_RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, grfx.viewport_width),
                .bottom = @intToFloat(f32, grfx.viewport_height),
            },
            @ptrCast(*w.ID2D1Brush, demo.brush),
        );

        drawShapes(demo.*);
    }
    grfx.endDraw2d();

    grfx.endFrame();
}

pub fn main() !void {
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = init(allocator);
    defer deinit(&demo, allocator);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch unreachable;
        if (has_message) {
            _ = w.user32.translateMessage(&message);
            _ = w.user32.dispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT) {
                break;
            }
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
