const std = @import("std");
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const d2d1 = zwin32.d2d1;
const dwrite = zwin32.dwrite;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: vector graphics test";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    frame_stats: common.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    radial_gradient_brush: *d2d1.IRadialGradientBrush,
    textformat: *dwrite.ITextFormat,
    ellipse: *d2d1.IEllipseGeometry,
    stroke_style: *d2d1.IStrokeStyle,
    path: *d2d1.IPathGeometry,
    ink: *d2d1.IInk,
    ink_style: *d2d1.IInkStyle,
    bezier_lines_path: *d2d1.IPathGeometry,

    ink_points: std.ArrayList(d2d1.POINT_2F),

    left_mountain_geo: *d2d1.IGeometry,
    right_mountain_geo: *d2d1.IGeometry,
    sun_geo: *d2d1.IGeometry,
    river_geo: *d2d1.IGeometry,

    custom_txtfmt: *dwrite.ITextFormat,
};

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);
    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    gctx.present_flags = .{};
    gctx.present_interval = 1;

    var ink_points = std.ArrayList(d2d1.POINT_2F).init(allocator);

    const brush = blk: {
        var maybe_brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(gctx.d2d.?.context.CreateSolidColorBrush(
            &d2d1.COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &maybe_brush,
        ));
        break :blk maybe_brush.?;
    };

    const textformat = blk: {
        var maybe_textformat: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(gctx.d2d.?.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
    hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

    const custom_txtfmt = blk: {
        var maybe_textformat: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(gctx.d2d.?.dwrite_factory.CreateTextFormat(
            L("Ink Free"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            64.0,
            L("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(custom_txtfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(custom_txtfmt.SetParagraphAlignment(.NEAR));

    const ellipse = blk: {
        var maybe_ellipse: ?*d2d1.IEllipseGeometry = null;
        hrPanicOnFail(gctx.d2d.?.factory.CreateEllipseGeometry(
            &.{ .point = .{ .x = 1210.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
            &maybe_ellipse,
        ));
        break :blk maybe_ellipse.?;
    };

    const stroke_style = blk: {
        var maybe_stroke_style: ?*d2d1.IStrokeStyle = null;
        hrPanicOnFail(gctx.d2d.?.factory.CreateStrokeStyle(
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
        var path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
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
        var ink_style: *d2d1.IInkStyle = undefined;
        hrPanicOnFail(gctx.d2d.?.context.CreateInkStyle(
            &.{ .nibShape = .ROUND, .nibTransform = d2d1.MATRIX_3X2_F.initIdentity() },
            @ptrCast(*?*d2d1.IInkStyle, &ink_style),
        ));
        break :blk ink_style;
    };

    const ink = blk: {
        var ink: *d2d1.IInk = undefined;
        hrPanicOnFail(gctx.d2d.?.context.CreateInk(
            &.{ .x = 0.0, .y = 0.0, .radius = 0.0 },
            @ptrCast(*?*d2d1.IInk, &ink),
        ));

        var p0 = d2d1.POINT_2F{ .x = 0.0, .y = 0.0 };
        ink_points.append(p0) catch unreachable;
        ink.SetStartPoint(&.{ .x = p0.x, .y = p0.y, .radius = 1.0 });
        const sp = ink.GetStartPoint();
        assert(sp.x == p0.x and sp.y == p0.y and sp.radius == 1.0);
        assert(ink.GetSegmentCount() == 0);

        {
            const p1 = d2d1.POINT_2F{ .x = 200.0, .y = 0.0 };
            const cp1 = d2d1.POINT_2F{ .x = p0.x - 40.0, .y = p0.y + 140.0 };
            const cp2 = d2d1.POINT_2F{ .x = p1.x + 40.0, .y = p1.y - 140.0 };
            ink_points.append(cp1) catch unreachable;
            ink_points.append(cp2) catch unreachable;
            hrPanicOnFail(ink.AddSegments(&[_]d2d1.INK_BEZIER_SEGMENT{.{
                .point1 = .{ .x = cp1.x, .y = cp1.y, .radius = 12.5 },
                .point2 = .{ .x = cp2.x, .y = cp2.y, .radius = 12.5 },
                .point3 = .{ .x = p1.x, .y = p1.y, .radius = 9.0 },
            }}, 1));
            ink_points.append(p1) catch unreachable;
        }

        p0 = ink_points.items[ink_points.items.len - 1];
        {
            const p1 = d2d1.POINT_2F{ .x = 400.0, .y = 0.0 };
            const cp1 = d2d1.POINT_2F{ .x = p0.x - 40.0, .y = p0.y + 140.0 };
            const cp2 = d2d1.POINT_2F{ .x = p1.x + 40.0, .y = p1.y - 140.0 };
            ink_points.append(cp1) catch unreachable;
            ink_points.append(cp2) catch unreachable;
            hrPanicOnFail(ink.AddSegments(&[_]d2d1.INK_BEZIER_SEGMENT{.{
                .point1 = .{ .x = cp1.x, .y = cp1.y, .radius = 6.25 },
                .point2 = .{ .x = cp2.x, .y = cp2.y, .radius = 6.25 },
                .point3 = .{ .x = p1.x, .y = p1.y, .radius = 1.0 },
            }}, 1));
            ink_points.append(p1) catch unreachable;
        }

        break :blk ink;
    };

    const bezier_lines_path = blk: {
        var bezier_lines_path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &bezier_lines_path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(bezier_lines_path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.BeginFigure(.{ .x = 0.0, .y = 0.0 }, .FILLED);
        sink.AddLines(ink_points.items.ptr + 1, @intCast(u32, ink_points.items.len - 1));
        sink.EndFigure(.OPEN);
        break :blk bezier_lines_path;
    };

    const left_mountain_geo = blk: {
        var left_mountain_path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &left_mountain_path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(left_mountain_path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.SetFillMode(.WINDING);

        sink.BeginFigure(.{ .x = 346.0, .y = 255.0 }, .FILLED);
        const points = [_]d2d1.POINT_2F{
            .{ .x = 267.0, .y = 177.0 },
            .{ .x = 236.0, .y = 192.0 },
            .{ .x = 212.0, .y = 160.0 },
            .{ .x = 156.0, .y = 255.0 },
            .{ .x = 346.0, .y = 255.0 },
        };
        sink.AddLines(&points, points.len);
        sink.EndFigure(.CLOSED);
        break :blk @ptrCast(*d2d1.IGeometry, left_mountain_path);
    };

    const right_mountain_geo = blk: {
        var right_mountain_path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &right_mountain_path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(right_mountain_path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.SetFillMode(.WINDING);

        sink.BeginFigure(.{ .x = 575.0, .y = 263.0 }, .FILLED);
        const points = [_]d2d1.POINT_2F{
            .{ .x = 481.0, .y = 146.0 },
            .{ .x = 449.0, .y = 181.0 },
            .{ .x = 433.0, .y = 159.0 },
            .{ .x = 401.0, .y = 214.0 },
            .{ .x = 381.0, .y = 199.0 },
            .{ .x = 323.0, .y = 263.0 },
            .{ .x = 575.0, .y = 263.0 },
        };
        sink.AddLines(&points, points.len);
        sink.EndFigure(.CLOSED);
        break :blk @ptrCast(*d2d1.IGeometry, right_mountain_path);
    };

    const sun_geo = blk: {
        var sun_path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &sun_path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(sun_path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.SetFillMode(.WINDING);

        sink.BeginFigure(.{ .x = 270.0, .y = 255.0 }, .FILLED);
        sink.AddArc(&.{
            .point = .{ .x = 440.0, .y = 255.0 },
            .size = .{ .width = 85.0, .height = 85.0 },
            .rotationAngle = 0.0, // rotation angle
            .sweepDirection = .CLOCKWISE,
            .arcSize = .SMALL,
        });
        sink.EndFigure(.CLOSED);

        sink.BeginFigure(.{ .x = 299.0, .y = 182.0 }, .HOLLOW);
        sink.AddBezier(&.{
            .point1 = .{ .x = 299.0, .y = 182.0 },
            .point2 = .{ .x = 294.0, .y = 176.0 },
            .point3 = .{ .x = 285.0, .y = 178.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 276.0, .y = 179.0 },
            .point2 = .{ .x = 272.0, .y = 173.0 },
            .point3 = .{ .x = 272.0, .y = 173.0 },
        });
        sink.EndFigure(.OPEN);

        sink.BeginFigure(.{ .x = 354.0, .y = 156.0 }, .HOLLOW);
        sink.AddBezier(&.{
            .point1 = .{ .x = 354.0, .y = 156.0 },
            .point2 = .{ .x = 358.0, .y = 149.0 },
            .point3 = .{ .x = 354.0, .y = 142.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 349.0, .y = 134.0 },
            .point2 = .{ .x = 354.0, .y = 127.0 },
            .point3 = .{ .x = 354.0, .y = 127.0 },
        });
        sink.EndFigure(.OPEN);

        sink.BeginFigure(.{ .x = 322.0, .y = 164.0 }, .HOLLOW);
        sink.AddBezier(&.{
            .point1 = .{ .x = 322.0, .y = 164.0 },
            .point2 = .{ .x = 322.0, .y = 156.0 },
            .point3 = .{ .x = 314.0, .y = 152.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 306.0, .y = 149.0 },
            .point2 = .{ .x = 305.0, .y = 141.0 },
            .point3 = .{ .x = 305.0, .y = 141.0 },
        });
        sink.EndFigure(.OPEN);

        sink.BeginFigure(.{ .x = 385.0, .y = 164.0 }, .HOLLOW);
        sink.AddBezier(&.{
            .point1 = .{ .x = 385.0, .y = 164.0 },
            .point2 = .{ .x = 392.0, .y = 161.0 },
            .point3 = .{ .x = 394.0, .y = 152.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 395.0, .y = 144.0 },
            .point2 = .{ .x = 402.0, .y = 141.0 },
            .point3 = .{ .x = 402.0, .y = 142.0 },
        });
        sink.EndFigure(.OPEN);

        sink.BeginFigure(.{ .x = 408.0, .y = 182.0 }, .HOLLOW);
        sink.AddBezier(&.{
            .point1 = .{ .x = 408.0, .y = 182.0 },
            .point2 = .{ .x = 416.0, .y = 184.0 },
            .point3 = .{ .x = 422.0, .y = 178.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 428.0, .y = 171.0 },
            .point2 = .{ .x = 435.0, .y = 173.0 },
            .point3 = .{ .x = 435.0, .y = 173.0 },
        });
        sink.EndFigure(.OPEN);

        break :blk @ptrCast(*d2d1.IGeometry, sun_path);
    };

    const river_geo = blk: {
        var river_path: *d2d1.IPathGeometry = undefined;
        hrPanicOnFail(gctx.d2d.?.factory.CreatePathGeometry(@ptrCast(*?*d2d1.IPathGeometry, &river_path)));

        var sink: *d2d1.IGeometrySink = undefined;
        hrPanicOnFail(river_path.Open(@ptrCast(*?*d2d1.IGeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.SetFillMode(.WINDING);

        sink.BeginFigure(.{ .x = 183.0, .y = 392.0 }, .FILLED);
        sink.AddBezier(&.{
            .point1 = .{ .x = 238.0, .y = 284.0 },
            .point2 = .{ .x = 472.0, .y = 345.0 },
            .point3 = .{ .x = 356.0, .y = 303.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 237.0, .y = 261.0 },
            .point2 = .{ .x = 333.0, .y = 256.0 },
            .point3 = .{ .x = 333.0, .y = 256.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 335.0, .y = 257.0 },
            .point2 = .{ .x = 241.0, .y = 261.0 },
            .point3 = .{ .x = 411.0, .y = 306.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 574.0, .y = 350.0 },
            .point2 = .{ .x = 288.0, .y = 324.0 },
            .point3 = .{ .x = 296.0, .y = 392.0 },
        });
        sink.EndFigure(.CLOSED);
        break :blk @ptrCast(*d2d1.IGeometry, river_path);
    };

    const radial_gradient_brush = blk: {
        const stops = [_]d2d1.GRADIENT_STOP{
            .{ .color = d2d1.colorf.YellowGreen, .position = 0.0 },
            .{ .color = d2d1.colorf.LightSkyBlue, .position = 1.0 },
        };
        var stop_collection: *d2d1.IGradientStopCollection = undefined;
        hrPanicOnFail(gctx.d2d.?.context.CreateGradientStopCollection(
            &stops,
            2,
            ._2_2,
            .CLAMP,
            @ptrCast(*?*d2d1.IGradientStopCollection, &stop_collection),
        ));
        defer _ = stop_collection.Release();

        var radial_gradient_brush: *d2d1.IRadialGradientBrush = undefined;
        hrPanicOnFail(gctx.d2d.?.context.CreateRadialGradientBrush(
            &.{
                .center = .{ .x = 75.0, .y = 75.0 },
                .gradientOriginOffset = .{ .x = 0.0, .y = 0.0 },
                .radiusX = 75.0,
                .radiusY = 75.0,
            },
            null,
            stop_collection,
            @ptrCast(*?*d2d1.IRadialGradientBrush, &radial_gradient_brush),
        ));
        break :blk radial_gradient_brush;
    };

    return DemoState{
        .gctx = gctx,
        .frame_stats = common.FrameStats.init(),
        .brush = brush,
        .textformat = textformat,
        .ellipse = ellipse,
        .stroke_style = stroke_style,
        .path = path,
        .ink_style = ink_style,
        .ink = ink,
        .ink_points = ink_points,
        .bezier_lines_path = bezier_lines_path,
        .left_mountain_geo = left_mountain_geo,
        .right_mountain_geo = right_mountain_geo,
        .sun_geo = sun_geo,
        .river_geo = river_geo,
        .radial_gradient_brush = radial_gradient_brush,
        .custom_txtfmt = custom_txtfmt,
    };
}

fn drawShapes(demo: DemoState) void {
    var gctx = &demo.gctx;

    gctx.d2d.?.context.DrawLine(
        .{ .x = 20.0, .y = 200.0 },
        .{ .x = 120.0, .y = 300.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    gctx.d2d.?.context.DrawLine(
        .{ .x = 160.0, .y = 300.0 },
        .{ .x = 260.0, .y = 200.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    gctx.d2d.?.context.DrawLine(
        .{ .x = 300.0, .y = 200.0 },
        .{ .x = 400.0, .y = 300.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
        33.0,
        demo.stroke_style,
    );

    gctx.d2d.?.context.DrawRectangle(
        &.{ .left = 500.0, .top = 100.0, .right = 600.0, .bottom = 200.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
        5.0,
        null,
    );
    gctx.d2d.?.context.FillRectangle(
        &.{ .left = 610.0, .top = 100.0, .right = 710.0, .bottom = 200.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
    );
    gctx.d2d.?.context.FillRoundedRectangle(
        &.{
            .rect = .{ .left = 830.0, .top = 100.0, .right = 930.0, .bottom = 200.0 },
            .radiusX = 20.0,
            .radiusY = 20.0,
        },
        @ptrCast(*d2d1.IBrush, demo.brush),
    );

    gctx.d2d.?.context.DrawEllipse(
        &.{ .point = .{ .x = 990.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
        5.0,
        null,
    );
    gctx.d2d.?.context.FillEllipse(
        &.{ .point = .{ .x = 1100.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
    );
    gctx.d2d.?.context.DrawGeometry(
        @ptrCast(*d2d1.IGeometry, demo.ellipse),
        @ptrCast(*d2d1.IBrush, demo.brush),
        7.0,
        null,
    );
    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initTranslation(110.0, 0.0));
    gctx.d2d.?.context.FillGeometry(
        @ptrCast(*d2d1.IGeometry, demo.ellipse),
        @ptrCast(*d2d1.IBrush, demo.brush),
        null,
    );
    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initIdentity());

    demo.brush.SetColor(&d2d1.COLOR_F{ .r = 0.2, .g = 0.4, .b = 0.8, .a = 1.0 });
    var i: u32 = 0;
    while (i < 5) : (i += 1) {
        gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initTranslation(0.0, @intToFloat(f32, i) * 50.0));
        gctx.d2d.?.context.DrawGeometry(
            @ptrCast(*d2d1.IGeometry, demo.path),
            @ptrCast(*d2d1.IBrush, demo.brush),
            15.0,
            null,
        );
    }

    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initTranslation(500.0, 800.0));
    gctx.d2d.?.context.DrawInk(demo.ink, @ptrCast(*d2d1.IBrush, demo.brush), demo.ink_style);

    demo.brush.SetColor(&d2d1.COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
    gctx.d2d.?.context.DrawGeometry(
        @ptrCast(*d2d1.IGeometry, demo.bezier_lines_path),
        @ptrCast(*d2d1.IBrush, demo.brush),
        3.0,
        null,
    );

    demo.brush.SetColor(&d2d1.COLOR_F{ .r = 0.8, .g = 0.0, .b = 0.0, .a = 1.0 });
    for (demo.ink_points.items) |cp| {
        gctx.d2d.?.context.FillEllipse(
            &.{ .point = cp, .radiusX = 9.0, .radiusY = 9.0 },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }

    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initTranslation(750.0, 900.0));
    gctx.d2d.?.context.DrawInk(demo.ink, @ptrCast(*d2d1.IBrush, demo.brush), demo.ink_style);

    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initTranslation(1080.0, 640.0));

    // Draw background.
    demo.brush.SetColor(&d2d1.colorf.White);
    gctx.d2d.?.context.FillRectangle(
        &.{ .left = 100.0, .top = 100.0, .right = 620.0, .bottom = 420.0 },
        @ptrCast(*d2d1.IBrush, demo.brush),
    );

    // NOTE(mziulek): D2D1 is slow. It creates and destroys resources every frame (see graphics.endDraw2d()).
    // TODO(mziulek): Check if ID2D1GeometryRealization helps.

    // Draw sun.
    // NOTE(mziulek): Using 'demo.radial_gradient_brush' causes GPU-Based Validation errors (D3D11on12 bug?).
    // As a workaround we use 'demo.brush' (solid color brush).
    demo.brush.SetColor(&d2d1.colorf.DarkOrange);
    gctx.d2d.?.context.FillGeometry(demo.sun_geo, @ptrCast(*d2d1.IBrush, demo.brush), null);

    demo.brush.SetColor(&d2d1.colorf.Black);
    gctx.d2d.?.context.DrawGeometry(demo.sun_geo, @ptrCast(*d2d1.IBrush, demo.brush), 5.0, null);

    // Draw left mountain geometry.
    demo.brush.SetColor(&d2d1.colorf.OliveDrab);
    gctx.d2d.?.context.FillGeometry(demo.left_mountain_geo, @ptrCast(*d2d1.IBrush, demo.brush), null);

    demo.brush.SetColor(&d2d1.colorf.Black);
    gctx.d2d.?.context.DrawGeometry(demo.left_mountain_geo, @ptrCast(*d2d1.IBrush, demo.brush), 5.0, null);

    // Draw river geometry.
    demo.brush.SetColor(&d2d1.colorf.LightSkyBlue);
    gctx.d2d.?.context.FillGeometry(demo.river_geo, @ptrCast(*d2d1.IBrush, demo.brush), null);

    demo.brush.SetColor(&d2d1.colorf.Black);
    gctx.d2d.?.context.DrawGeometry(demo.river_geo, @ptrCast(*d2d1.IBrush, demo.brush), 5.0, null);

    // Draw right mountain geometry.
    demo.brush.SetColor(&d2d1.colorf.YellowGreen);
    gctx.d2d.?.context.FillGeometry(demo.right_mountain_geo, @ptrCast(*d2d1.IBrush, demo.brush), null);

    demo.brush.SetColor(&d2d1.colorf.Black);
    gctx.d2d.?.context.DrawGeometry(demo.right_mountain_geo, @ptrCast(*d2d1.IBrush, demo.brush), 5.0, null);

    gctx.d2d.?.context.SetTransform(&d2d1.MATRIX_3X2_F.initIdentity());
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    _ = demo.custom_txtfmt.Release();
    _ = demo.ellipse.Release();
    _ = demo.stroke_style.Release();
    _ = demo.path.Release();
    _ = demo.ink.Release();
    _ = demo.ink_style.Release();
    _ = demo.bezier_lines_path.Release();
    _ = demo.left_mountain_geo.Release();
    _ = demo.right_mountain_geo.Release();
    _ = demo.sun_geo.Release();
    _ = demo.river_geo.Release();
    _ = demo.radial_gradient_brush.Release();
    demo.ink_points.deinit();
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.gctx.window, window_name);
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const back_buffer = gctx.getBackBuffer();

    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        null,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    gctx.beginDraw2d();
    {
        const stats = &demo.frame_stats;
        var buffer = [_]u8{0} ** 64;
        const text = std.fmt.bufPrint(
            buffer[0..],
            "FPS: {d:.1}\nCPU time: {d:.3} ms",
            .{ stats.fps, stats.average_cpu_time },
        ) catch unreachable;

        demo.brush.SetColor(&d2d1.COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        common.drawText(
            gctx.d2d.?.context,
            text,
            demo.textformat,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, gctx.viewport_width),
                .bottom = @intToFloat(f32, gctx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );

        demo.brush.SetColor(&d2d1.COLOR_F{ .r = 0.6, .g = 0.0, .b = 1.0, .a = 1.0 });
        common.drawText(
            gctx.d2d.?.context,
            \\Lorem ipsum dolor sit amet,
            \\consectetur adipiscing elit,
            \\sed do eiusmod tempor incididunt
            \\ut labore et dolore magna aliqua.
        ,
            demo.custom_txtfmt,
            &d2d1.RECT_F{
                .left = 1030.0,
                .top = 220.0,
                .right = @intToFloat(f32, gctx.viewport_width),
                .bottom = @intToFloat(f32, gctx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );

        demo.brush.SetColor(&d2d1.COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        drawShapes(demo.*);
    }
    gctx.endDraw2d();

    gctx.endFrame();
}

pub fn main() !void {
    common.init();
    defer common.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try init(allocator);
    defer deinit(&demo, allocator);

    while (common.handleWindowEvents()) {
        update(&demo);
        draw(&demo);
    }
}
