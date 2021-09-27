const builtin = @import("builtin");
const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const dxgi = win32.dxgi;
const d3d12 = win32.d3d12;
const d3d12d = win32.d3d12d;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

pub fn hrPanicOnFail(hr: w.HRESULT) void {
    if (hr != 0) {
        // TODO(mziulek): In ReleaseMode display a MessageBox for the user.
        std.debug.panic("HRESULT error detected ({d}).", .{hr});
    }
}

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
        style + w.user32.WS_VISIBLE,
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

pub const GraphicsContext = struct {
    pub const max_num_buffered_frames = 2;
    pub const num_swapbuffers = 4;

    device: *d3d12.IDevice9,
    cmdqueue: *d3d12.ICommandQueue,
    cmdlist: *d3d12.IGraphicsCommandList6,
    cmdallocs: [max_num_buffered_frames]*d3d12.ICommandAllocator,
    swapchain: *dxgi.ISwapChain3,
    swapbuffers: [num_swapbuffers]*d3d12.IResource,
    rtv_descriptor_heap: *d3d12.IDescriptorHeap,
    viewport_width: u32,
    viewport_height: u32,
    frame_fence: *d3d12.IFence,
    frame_fence_event: w.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,

    pub fn init(window: w.HWND) GraphicsContext {
        const factory = blk: {
            var factory: *dxgi.IFactory1 = undefined;
            hrPanicOnFail(dxgi.CreateDXGIFactory2(0, &dxgi.IID_IFactory1, @ptrCast(*?*c_void, &factory)));
            break :blk factory;
        };
        defer _ = factory.Release();

        const device = blk: {
            var device: *d3d12.IDevice9 = undefined;
            hrPanicOnFail(d3d12.D3D12CreateDevice(
                null,
                .FL_11_1,
                &d3d12.IID_IDevice9,
                @ptrCast(*?*c_void, &device),
            ));
            break :blk device;
        };

        const cmdqueue = blk: {
            var cmdqueue: *d3d12.ICommandQueue = undefined;
            hrPanicOnFail(device.CreateCommandQueue(&.{
                .Type = .DIRECT,
                .Priority = @enumToInt(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
                .Flags = d3d12.COMMAND_QUEUE_FLAG_NONE,
                .NodeMask = 0,
            }, &d3d12.IID_ICommandQueue, @ptrCast(*?*c_void, &cmdqueue)));
            break :blk cmdqueue;
        };

        var rect: w.RECT = undefined;
        _ = w.GetClientRect(window, &rect);
        const viewport_width = @intCast(u32, rect.right - rect.left);
        const viewport_height = @intCast(u32, rect.bottom - rect.top);

        const swapchain = blk: {
            var swapchain: *dxgi.ISwapChain = undefined;
            hrPanicOnFail(factory.CreateSwapChain(
                @ptrCast(*w.IUnknown, cmdqueue),
                &dxgi.SWAP_CHAIN_DESC{
                    .BufferDesc = .{
                        .Width = viewport_width,
                        .Height = viewport_height,
                        .RefreshRate = .{ .Numerator = 0, .Denominator = 0 },
                        .Format = .R8G8B8A8_UNORM,
                        .ScanlineOrdering = .UNSPECIFIED,
                        .Scaling = .UNSPECIFIED,
                    },
                    .SampleDesc = .{ .Count = 1, .Quality = 0 },
                    .BufferUsage = dxgi.USAGE_RENDER_TARGET_OUTPUT,
                    .BufferCount = num_swapbuffers,
                    .OutputWindow = window,
                    .Windowed = w.TRUE,
                    .SwapEffect = .FLIP_DISCARD,
                    .Flags = 0,
                },
                @ptrCast(*?*dxgi.ISwapChain, &swapchain),
            ));
            defer _ = swapchain.Release();

            var swapchain3: *dxgi.ISwapChain3 = undefined;
            hrPanicOnFail(swapchain.QueryInterface(
                &dxgi.IID_ISwapChain3,
                @ptrCast(*?*c_void, &swapchain3),
            ));
            break :blk swapchain3;
        };

        const rtv_descriptor_heap = blk: {
            var heap: *d3d12.IDescriptorHeap = undefined;
            hrPanicOnFail(device.CreateDescriptorHeap(&.{
                .Type = .RTV,
                .NumDescriptors = num_swapbuffers,
                .Flags = d3d12.DESCRIPTOR_HEAP_FLAG_NONE,
                .NodeMask = 0,
            }, &d3d12.IID_IDescriptorHeap, @ptrCast(*?*c_void, &heap)));
            break :blk heap;
        };

        const swapbuffers = blk: {
            var descriptor = rtv_descriptor_heap.GetCPUDescriptorHandleForHeapStart();

            var swapbuffers: [num_swapbuffers]*d3d12.IResource = undefined;
            for (swapbuffers) |_, buffer_idx| {
                hrPanicOnFail(swapchain.GetBuffer(
                    @intCast(u32, buffer_idx),
                    &d3d12.IID_IResource,
                    @ptrCast(*?*c_void, &swapbuffers[buffer_idx]),
                ));
                device.CreateRenderTargetView(swapbuffers[buffer_idx], null, descriptor);
                descriptor.ptr += device.GetDescriptorHandleIncrementSize(.RTV);
            }
            break :blk swapbuffers;
        };

        const frame_fence = blk: {
            var frame_fence: *d3d12.IFence = undefined;
            hrPanicOnFail(device.CreateFence(
                0,
                d3d12.FENCE_FLAG_NONE,
                &d3d12.IID_IFence,
                @ptrCast(*?*c_void, &frame_fence),
            ));
            break :blk frame_fence;
        };

        const frame_fence_event = w.CreateEventEx(null, "frame_fence_event", 0, w.EVENT_ALL_ACCESS) catch unreachable;

        const cmdallocs = blk: {
            var cmdallocs: [max_num_buffered_frames]*d3d12.ICommandAllocator = undefined;
            for (cmdallocs) |_, cmdalloc_index| {
                hrPanicOnFail(device.CreateCommandAllocator(
                    .DIRECT,
                    &d3d12.IID_ICommandAllocator,
                    @ptrCast(*?*c_void, &cmdallocs[cmdalloc_index]),
                ));
            }
            break :blk cmdallocs;
        };

        const cmdlist = blk: {
            var cmdlist: *d3d12.IGraphicsCommandList6 = undefined;
            hrPanicOnFail(device.CreateCommandList(
                0,
                .DIRECT,
                cmdallocs[0],
                null,
                &d3d12.IID_IGraphicsCommandList6,
                @ptrCast(*?*c_void, &cmdlist),
            ));
            break :blk cmdlist;
        };
        hrPanicOnFail(cmdlist.Close());

        return GraphicsContext{
            .device = device,
            .cmdqueue = cmdqueue,
            .cmdlist = cmdlist,
            .cmdallocs = cmdallocs,
            .swapchain = swapchain,
            .swapbuffers = swapbuffers,
            .frame_fence = frame_fence,
            .frame_fence_event = frame_fence_event,
            .frame_fence_counter = 0,
            .rtv_descriptor_heap = rtv_descriptor_heap,
            .viewport_width = viewport_width,
            .viewport_height = viewport_height,
            .frame_index = 0,
            .back_buffer_index = swapchain.GetCurrentBackBufferIndex(),
        };
    }

    pub fn deinit(gr: *GraphicsContext) void {
        _ = gr.device.Release();
        _ = gr.cmdqueue.Release();
        _ = gr.swapchain.Release();
        _ = gr.frame_fence.Release();
        _ = gr.cmdlist.Release();
        _ = gr.rtv_descriptor_heap.Release();
        for (gr.cmdallocs) |cmdalloc| _ = cmdalloc.Release();
        for (gr.swapbuffers) |swapbuffer| _ = swapbuffer.Release();
        gr.* = undefined;
    }

    pub fn beginFrame(gr: *GraphicsContext) void {
        const cmdalloc = gr.cmdallocs[gr.frame_index];
        hrPanicOnFail(cmdalloc.Reset());
        hrPanicOnFail(gr.cmdlist.Reset(cmdalloc, null));

        gr.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = @intToFloat(f32, gr.viewport_width),
            .Height = @intToFloat(f32, gr.viewport_height),
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        gr.cmdlist.RSSetScissorRects(1, &[_]d3d12.RECT{.{
            .left = 0,
            .top = 0,
            .right = @intCast(c_long, gr.viewport_width),
            .bottom = @intCast(c_long, gr.viewport_height),
        }});
    }

    pub fn endFrame(gr: *GraphicsContext) void {
        hrPanicOnFail(gr.cmdlist.Close());
        gr.cmdqueue.ExecuteCommandLists(
            1,
            &[_]*d3d12.ICommandList{@ptrCast(*d3d12.ICommandList, gr.cmdlist)},
        );

        gr.frame_fence_counter += 1;
        hrPanicOnFail(gr.swapchain.Present(0, 0));
        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));

        const gpu_frame_counter = gr.frame_fence.GetCompletedValue();
        if ((gr.frame_fence_counter - gpu_frame_counter) >= max_num_buffered_frames) {
            hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gr.frame_fence_event));
            w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
        }

        gr.frame_index = (gr.frame_index + 1) % max_num_buffered_frames;
        gr.back_buffer_index = gr.swapchain.GetCurrentBackBufferIndex();
    }

    pub fn waitForGpu(gr: *GraphicsContext) void {
        gr.frame_fence_counter += 1;
        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));
        hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gr.frame_fence_counter, gr.frame_fence_event));
        w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
    }
};

pub fn main() !void {
    const window_name = "zig-gamedev: triangle";
    const window_width = 800;
    const window_height = 800;

    _ = w.SetProcessDPIAware();

    const window = initWindow(window_name, window_width, window_height) catch unreachable;
    var gr = GraphicsContext.init(window);
    defer gr.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }

    const pipeline = blk: {
        const vs_file = std.fs.cwd().openFile("content/shaders/triangle.vs.cso", .{}) catch unreachable;
        defer vs_file.close();
        const ps_file = std.fs.cwd().openFile("content/shaders/triangle.ps.cso", .{}) catch unreachable;
        defer ps_file.close();

        const allocator = &gpa.allocator;
        const vs_code = vs_file.reader().readAllAlloc(allocator, 256 * 1024) catch unreachable;
        defer allocator.free(vs_code);
        const ps_code = ps_file.reader().readAllAlloc(allocator, 256 * 1024) catch unreachable;
        defer allocator.free(ps_code);

        var rs: *d3d12.IRootSignature = undefined;
        hrPanicOnFail(gr.device.CreateRootSignature(
            0,
            vs_code.ptr,
            vs_code.len,
            &d3d12.IID_IRootSignature,
            @ptrCast(*?*c_void, &rs),
        ));

        const pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC{
            .pRootSignature = rs,
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
            .BlendState = d3d12.BLEND_DESC.initDefault(),
            .SampleMask = 0xffff_ffff,
            .RasterizerState = d3d12.RASTERIZER_DESC.initDefault(),
            .DepthStencilState = blk1: {
                var desc = d3d12.DEPTH_STENCIL_DESC.initDefault();
                desc.DepthEnable = w.FALSE;
                break :blk1 desc;
            },
            .InputLayout = .{ .pInputElementDescs = null, .NumElements = 0 },
            .IBStripCutValue = .DISABLED,
            .PrimitiveTopologyType = .TRIANGLE,
            .NumRenderTargets = 1,
            .RTVFormats = [_]dxgi.FORMAT{.R8G8B8A8_UNORM} ++ [_]dxgi.FORMAT{.UNKNOWN} ** 7,
            .DSVFormat = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .NodeMask = 0,
            .CachedPSO = .{ .pCachedBlob = null, .CachedBlobSizeInBytes = 0 },
            .Flags = d3d12.PIPELINE_STATE_FLAG_NONE,
        };

        var pso: *d3d12.IPipelineState = undefined;
        hrPanicOnFail(gr.device.CreateGraphicsPipelineState(
            &pso_desc,
            &d3d12.IID_IPipelineState,
            @ptrCast(*?*c_void, &pso),
        ));

        break :blk .{ .pso = pso, .rs = rs };
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

            gr.beginFrame();

            const back_buffer = gr.swapbuffers[gr.back_buffer_index];
            const back_buffer_rtv = blk: {
                var descriptor = gr.rtv_descriptor_heap.GetCPUDescriptorHandleForHeapStart();
                descriptor.ptr += gr.back_buffer_index * gr.device.GetDescriptorHandleIncrementSize(.RTV);
                break :blk descriptor;
            };

            gr.cmdlist.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
                .Type = .TRANSITION,
                .Flags = d3d12.RESOURCE_BARRIER_FLAG_NONE,
                .u = .{
                    .Transition = .{
                        .pResource = back_buffer,
                        .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = d3d12.RESOURCE_STATE_PRESENT,
                        .StateAfter = d3d12.RESOURCE_STATE_RENDER_TARGET,
                    },
                },
            }});
            gr.cmdlist.OMSetRenderTargets(1, &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer_rtv}, w.TRUE, null);
            gr.cmdlist.ClearRenderTargetView(back_buffer_rtv, &[4]f32{ 0.2, 0.4, 0.8, 1.0 }, 0, null);
            gr.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
            gr.cmdlist.SetPipelineState(pipeline.pso);
            gr.cmdlist.SetGraphicsRootSignature(pipeline.rs);
            gr.cmdlist.DrawInstanced(3, 1, 0, 0);
            gr.cmdlist.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
                .Type = .TRANSITION,
                .Flags = d3d12.RESOURCE_BARRIER_FLAG_NONE,
                .u = .{
                    .Transition = .{
                        .pResource = back_buffer,
                        .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = d3d12.RESOURCE_STATE_RENDER_TARGET,
                        .StateAfter = d3d12.RESOURCE_STATE_PRESENT,
                    },
                },
            }});

            gr.endFrame();
        }
    }

    gr.waitForGpu();

    std.debug.print("All OK!\n", .{});
}
