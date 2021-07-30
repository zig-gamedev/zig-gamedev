const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
usingnamespace @import("vectormath");
const vhr = gr.vhr;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const FrameStats = struct {
    time: f64,
    delta_time: f32,
    fps: f32,
    average_cpu_time: f32,
    timer: std.time.Timer,
    previous_time_ns: u64,
    fps_refresh_time_ns: u64,
    frame_counter: u64,

    fn init() FrameStats {
        return .{
            .time = 0.0,
            .delta_time = 0.0,
            .fps = 0.0,
            .average_cpu_time = 0.0,
            .timer = std.time.Timer.start() catch unreachable,
            .previous_time_ns = 0,
            .fps_refresh_time_ns = 0,
            .frame_counter = 0,
        };
    }

    fn update(self: *FrameStats) void {
        const now_ns = self.timer.read();
        self.time = @intToFloat(f64, now_ns) / std.time.ns_per_s;
        self.delta_time = @intToFloat(f32, now_ns - self.previous_time_ns) / std.time.ns_per_s;
        self.previous_time_ns = now_ns;

        if ((now_ns - self.fps_refresh_time_ns) >= std.time.ns_per_s) {
            const t = @intToFloat(f64, now_ns - self.fps_refresh_time_ns) / std.time.ns_per_s;
            const fps = @intToFloat(f64, self.frame_counter) / t;
            const ms = (1.0 / fps) * 1000.0;

            self.fps = @floatCast(f32, fps);
            self.average_cpu_time = @floatCast(f32, ms);
            self.fps_refresh_time_ns = now_ns;
            self.frame_counter = 0;
        }
        self.frame_counter += 1;
    }
};

fn processWindowMessage(
    window: w.HWND,
    message: w.UINT,
    wparam: w.WPARAM,
    lparam: w.LPARAM,
) callconv(w.WINAPI) w.LRESULT {
    const processed = switch (message) {
        w.user32.WM_DESTROY => blk: {
            w.user32.PostQuitMessage(0);
            break :blk true;
        },
        w.user32.WM_KEYDOWN => blk: {
            if (wparam == w.VK_ESCAPE) {
                w.user32.PostQuitMessage(0);
                break :blk true;
            }
            break :blk false;
        },
        else => false,
    };
    return if (processed) 0 else w.user32.DefWindowProcA(window, message, wparam, lparam);
}

fn initWindow(name: [*:0]const u8, width: u32, height: u32) !w.HWND {
    const winclass = w.user32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w.HINSTANCE, w.kernel32.GetModuleHandleW(null)),
        .hIcon = null,
        .hCursor = w.LoadCursorA(null, @intToPtr(w.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = try w.user32.registerClassExA(&winclass);

    const style = w.user32.WS_OVERLAPPED +
        w.user32.WS_SYSMENU +
        w.user32.WS_CAPTION +
        w.user32.WS_MINIMIZEBOX;

    var rect = w.RECT{ .left = 0, .top = 0, .right = @intCast(i32, width), .bottom = @intCast(i32, height) };
    try w.user32.adjustWindowRectEx(&rect, style, false, 0);

    return try w.user32.createWindowExA(
        0,
        name,
        name,
        style + w.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    );
}

pub fn main() !void {
    const window_name = "zig-gamedev: simple3d";
    const window_width = 800;
    const window_height = 800;

    _ = w.SetProcessDPIAware();

    try w.dxgi_load_dll();
    try w.d3d12_load_dll();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }

    const window = try initWindow(window_name, window_width, window_height);
    var grctx = try gr.GraphicsContext.init(&gpa.allocator, window);
    defer grctx.deinit();

    const pipeline = blk: {
        const vs_file = try std.fs.cwd().openFile("content/shaders/simple3d.vs.cso", .{});
        defer vs_file.close();
        const ps_file = try std.fs.cwd().openFile("content/shaders/simple3d.ps.cso", .{});
        defer ps_file.close();

        const allocator = &gpa.allocator;
        const vs_code = try vs_file.reader().readAllAlloc(allocator, 256 * 1024);
        defer allocator.free(vs_code);
        const ps_code = try ps_file.reader().readAllAlloc(allocator, 256 * 1024);
        defer allocator.free(ps_code);

        var maybe_rs: ?*w.ID3D12RootSignature = null;
        try vhr(grctx.device.CreateRootSignature(
            0,
            vs_code.ptr,
            vs_code.len,
            &w.IID_ID3D12RootSignature,
            @ptrCast(*?*c_void, &maybe_rs),
        ));
        errdefer _ = maybe_rs.?.Release();

        const pso_desc = w.D3D12_GRAPHICS_PIPELINE_STATE_DESC{
            .pRootSignature = maybe_rs,
            .VS = .{ .pShaderBytecode = vs_code.ptr, .BytecodeLength = vs_code.len },
            .PS = .{ .pShaderBytecode = ps_code.ptr, .BytecodeLength = ps_code.len },
            .DS = .{ .pShaderBytecode = null, .BytecodeLength = 0 },
            .HS = .{ .pShaderBytecode = null, .BytecodeLength = 0 },
            .GS = .{ .pShaderBytecode = null, .BytecodeLength = 0 },
            .StreamOutput = .{
                .pSODeclaration = null,
                .NumEntries = 0,
                .pBufferStrides = null,
                .NumStrides = 0,
                .RasterizedStream = 0,
            },
            .BlendState = w.D3D12_BLEND_DESC.initDefault(),
            .SampleMask = 0xffff_ffff,
            .RasterizerState = w.D3D12_RASTERIZER_DESC.initDefault(),
            .DepthStencilState = blk1: {
                var desc = w.D3D12_DEPTH_STENCIL_DESC.initDefault();
                desc.DepthEnable = w.FALSE;
                break :blk1 desc;
            },
            .InputLayout = .{ .pInputElementDescs = null, .NumElements = 0 },
            .IBStripCutValue = .DISABLED,
            .PrimitiveTopologyType = .TRIANGLE,
            .NumRenderTargets = 1,
            .RTVFormats = [_]w.DXGI_FORMAT{.R8G8B8A8_UNORM} ++ [_]w.DXGI_FORMAT{.UNKNOWN} ** 7,
            .DSVFormat = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .NodeMask = 0,
            .CachedPSO = .{ .pCachedBlob = null, .CachedBlobSizeInBytes = 0 },
            .Flags = .{},
        };

        _ = grctx.createGraphicsShaderPipeline(pso_desc);

        var maybe_pso: ?*w.ID3D12PipelineState = null;
        try vhr(grctx.device.CreateGraphicsPipelineState(
            &pso_desc,
            &w.IID_ID3D12PipelineState,
            @ptrCast(*?*c_void, &maybe_pso),
        ));

        break :blk .{ .pso = maybe_pso.?, .rs = maybe_rs.? };
    };
    defer {
        _ = pipeline.pso.Release();
        _ = pipeline.rs.Release();
    }

    var stats = FrameStats.init();

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        if (w.user32.PeekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) > 0) {
            _ = w.user32.DispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT)
                break;
        } else {
            stats.update();
            {
                var buffer = [_]u8{0} ** 64;
                const text = std.fmt.bufPrint(
                    buffer[0..],
                    "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                    .{ stats.fps, stats.average_cpu_time, window_name },
                ) catch unreachable;
                _ = w.SetWindowTextA(window, @ptrCast([*:0]const u8, text.ptr));
            }

            try grctx.beginFrame();

            const back_buffer = grctx.getBackBuffer();

            grctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            grctx.flushResourceBarriers();

            grctx.cmdlist.OMSetRenderTargets(
                1,
                &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
                w.TRUE,
                null,
            );
            grctx.cmdlist.ClearRenderTargetView(
                back_buffer.descriptor_handle,
                &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
                0,
                null,
            );
            grctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
            grctx.cmdlist.SetPipelineState(pipeline.pso);
            grctx.cmdlist.SetGraphicsRootSignature(pipeline.rs);
            grctx.cmdlist.DrawInstanced(3, 1, 0, 0);

            grctx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATES.PRESENT);
            grctx.flushResourceBarriers();

            try grctx.endFrame();
        }
    }

    try grctx.waitForGpu();

    std.debug.print("All OK!\n", .{});
}
