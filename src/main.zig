const builtin = @import("builtin");
const std = @import("std");
const w = struct {
    usingnamespace std.os.windows;
    usingnamespace @import("windows/windows.zig");
    usingnamespace @import("windows/d3d12.zig");
    usingnamespace @import("windows/d3d12sdklayers.zig");
    usingnamespace @import("windows/d3dcommon.zig");
    usingnamespace @import("windows/dxgi.zig");
    usingnamespace @import("windows/dxgi1_2.zig");
    usingnamespace @import("windows/dxgi1_4.zig");
};

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*c]const u8 = ".\\D3D12\\";

inline fn vhr(hr: w.HRESULT) !void {
    if (hr != 0) {
        return error.HResult;
        //std.debug.panic("HRESULT function failed ({}).", .{hr});
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

pub const GraphicsContext = struct {
    pub const max_num_buffered_frames = 2;
    pub const num_swapbuffers = 4;

    device: *w.ID3D12Device,
    cmdqueue: *w.ID3D12CommandQueue,
    cmdlist: *w.ID3D12GraphicsCommandList,
    cmdallocs: [max_num_buffered_frames]*w.ID3D12CommandAllocator,
    swapchain: *w.IDXGISwapChain3,
    swapbuffers: [num_swapbuffers]*w.ID3D12Resource,
    rtv_descriptor_heap: *w.ID3D12DescriptorHeap,
    viewport_width: u32,
    viewport_height: u32,
    frame_fence: *w.ID3D12Fence,
    frame_fence_event: w.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,

    pub fn init(window: w.HWND) !GraphicsContext {
        const factory = blk: {
            var maybe_factory: ?*w.IDXGIFactory1 = null;
            try vhr(w.CreateDXGIFactory2(
                if (comptime builtin.mode == .Debug) w.DXGI_CREATE_FACTORY_DEBUG else 0,
                &w.IID_IDXGIFactory1,
                @ptrCast(*?*c_void, &maybe_factory),
            ));
            break :blk maybe_factory.?;
        };
        defer _ = factory.Release();

        if (comptime builtin.mode == .Debug) {
            var maybe_debug: ?*w.ID3D12Debug1 = null;
            _ = w.D3D12GetDebugInterface(&w.IID_ID3D12Debug1, @ptrCast(*?*c_void, &maybe_debug));
            if (maybe_debug) |debug| {
                debug.EnableDebugLayer();
                debug.SetEnableGPUBasedValidation(w.TRUE);
                _ = debug.Release();
            }
        }

        const device = blk: {
            var maybe_device: ?*w.ID3D12Device = null;
            try vhr(w.D3D12CreateDevice(null, ._11_1, &w.IID_ID3D12Device, @ptrCast(*?*c_void, &maybe_device)));
            break :blk maybe_device.?;
        };
        errdefer _ = device.Release();

        const cmdqueue = blk: {
            var maybe_cmdqueue: ?*w.ID3D12CommandQueue = null;
            try vhr(device.CreateCommandQueue(&.{
                .Type = .DIRECT,
                .Priority = @enumToInt(w.D3D12_COMMAND_QUEUE_PRIORITY.NORMAL),
                .Flags = .{},
                .NodeMask = 0,
            }, &w.IID_ID3D12CommandQueue, @ptrCast(*?*c_void, &maybe_cmdqueue)));
            break :blk maybe_cmdqueue.?;
        };
        errdefer _ = cmdqueue.Release();

        var rect: w.RECT = undefined;
        _ = w.GetClientRect(window, &rect);
        const viewport_width = @intCast(u32, rect.right - rect.left);
        const viewport_height = @intCast(u32, rect.bottom - rect.top);

        const swapchain = blk: {
            var maybe_swapchain: ?*w.IDXGISwapChain = null;
            try vhr(factory.CreateSwapChain(
                @ptrCast(*w.IUnknown, cmdqueue),
                &w.DXGI_SWAP_CHAIN_DESC{
                    .BufferDesc = .{
                        .Width = viewport_width,
                        .Height = viewport_height,
                        .RefreshRate = .{ .Numerator = 0, .Denominator = 0 },
                        .Format = .R8G8B8A8_UNORM,
                        .ScanlineOrdering = .UNSPECIFIED,
                        .Scaling = .UNSPECIFIED,
                    },
                    .SampleDesc = .{ .Count = 1, .Quality = 0 },
                    .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                    .BufferCount = num_swapbuffers,
                    .OutputWindow = window,
                    .Windowed = w.TRUE,
                    .SwapEffect = .FLIP_DISCARD,
                    .Flags = .{},
                },
                &maybe_swapchain,
            ));
            defer _ = maybe_swapchain.?.Release();
            var maybe_swapchain3: ?*w.IDXGISwapChain3 = null;
            try vhr(maybe_swapchain.?.QueryInterface(&w.IID_IDXGISwapChain3, @ptrCast(*?*c_void, &maybe_swapchain3)));
            break :blk maybe_swapchain3.?;
        };
        errdefer _ = swapchain.Release();

        const rtv_descriptor_heap = blk: {
            var maybe_heap: ?*w.ID3D12DescriptorHeap = null;
            try vhr(device.CreateDescriptorHeap(&.{
                .Type = .RTV,
                .NumDescriptors = num_swapbuffers,
                .Flags = .{},
                .NodeMask = 0,
            }, &w.IID_ID3D12DescriptorHeap, @ptrCast(*?*c_void, &maybe_heap)));
            break :blk maybe_heap.?;
        };
        errdefer _ = rtv_descriptor_heap.Release();

        const swapbuffers = blk: {
            var maybe_swapbuffers = [_]?*w.ID3D12Resource{null} ** num_swapbuffers;
            errdefer {
                for (maybe_swapbuffers) |swapbuffer| {
                    if (swapbuffer) |sb| _ = sb.Release();
                }
            }
            var descriptor = rtv_descriptor_heap.GetCPUDescriptorHandleForHeapStart();
            for (maybe_swapbuffers) |*swapbuffer, buffer_idx| {
                try vhr(swapchain.GetBuffer(
                    @intCast(u32, buffer_idx),
                    &w.IID_ID3D12Resource,
                    @ptrCast(*?*c_void, &swapbuffer.*),
                ));
                device.CreateRenderTargetView(swapbuffer.*, null, descriptor);
                descriptor.ptr += device.GetDescriptorHandleIncrementSize(.RTV);
            }
            var swapbuffers: [num_swapbuffers]*w.ID3D12Resource = undefined;
            for (maybe_swapbuffers) |swapbuffer, i| swapbuffers[i] = swapbuffer.?;
            break :blk swapbuffers;
        };
        errdefer {
            for (swapbuffers) |swapbuffer| _ = swapbuffer.Release();
        }

        const frame_fence = blk: {
            var maybe_frame_fence: ?*w.ID3D12Fence = null;
            try vhr(device.CreateFence(0, .{}, &w.IID_ID3D12Fence, @ptrCast(*?*c_void, &maybe_frame_fence)));
            break :blk maybe_frame_fence.?;
        };
        errdefer _ = frame_fence.Release();

        const frame_fence_event = w.CreateEventEx(null, "frame_fence_event", 0, w.EVENT_ALL_ACCESS) catch unreachable;

        const cmdallocs = blk: {
            var maybe_cmdallocs = [_]?*w.ID3D12CommandAllocator{null} ** max_num_buffered_frames;
            errdefer {
                for (maybe_cmdallocs) |cmdalloc| {
                    if (cmdalloc) |ca| _ = ca.Release();
                }
            }
            for (maybe_cmdallocs) |*cmdalloc| {
                try vhr(device.CreateCommandAllocator(
                    .DIRECT,
                    &w.IID_ID3D12CommandAllocator,
                    @ptrCast(*?*c_void, &cmdalloc.*),
                ));
            }
            var cmdallocs: [max_num_buffered_frames]*w.ID3D12CommandAllocator = undefined;
            for (maybe_cmdallocs) |cmdalloc, i| cmdallocs[i] = cmdalloc.?;
            break :blk cmdallocs;
        };
        errdefer {
            for (cmdallocs) |cmdalloc| _ = cmdalloc.Release();
        }

        const cmdlist = blk: {
            var maybe_cmdlist: ?*w.ID3D12GraphicsCommandList = null;
            try vhr(device.CreateCommandList(
                0,
                .DIRECT,
                cmdallocs[0],
                null,
                &w.IID_ID3D12GraphicsCommandList,
                @ptrCast(*?*c_void, &maybe_cmdlist),
            ));
            break :blk maybe_cmdlist.?;
        };
        errdefer _ = cmdlist.Release();
        try vhr(cmdlist.Close());

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
};

pub fn main() !void {
    const window_name = "zig-gamedev: triangle";
    const window_width = 800;
    const window_height = 800;

    _ = w.SetProcessDPIAware();

    try w.dxgi_load_dll();
    try w.d3d12_load_dll();

    const window = try initWindow(window_name, window_width, window_height);
    var gr = try GraphicsContext.init(window);
    defer gr.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
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
                    "FPS: {d:.1}  CPU time: {d:.3} ms",
                    .{ stats.fps, stats.average_cpu_time },
                ) catch unreachable;
                _ = w.SetWindowTextA(window, @ptrCast([*:0]const u8, text.ptr));
            }

            const cmdalloc = gr.cmdallocs[gr.frame_index];
            try vhr(cmdalloc.Reset());
            try vhr(gr.cmdlist.Reset(cmdalloc, null));

            gr.cmdlist.RSSetViewports(1, &[_]w.D3D12_VIEWPORT{.{
                .TopLeftX = 0.0,
                .TopLeftY = 0.0,
                .Width = @intToFloat(f32, gr.viewport_width),
                .Height = @intToFloat(f32, gr.viewport_height),
                .MinDepth = 0.0,
                .MaxDepth = 1.0,
            }});
            gr.cmdlist.RSSetScissorRects(1, &[_]w.D3D12_RECT{.{
                .left = 0,
                .top = 0,
                .right = @intCast(c_long, gr.viewport_width),
                .bottom = @intCast(c_long, gr.viewport_height),
            }});

            const back_buffer = gr.swapbuffers[gr.back_buffer_index];
            const back_buffer_rtv = blk: {
                var descriptor = gr.rtv_descriptor_heap.GetCPUDescriptorHandleForHeapStart();
                descriptor.ptr += gr.back_buffer_index * gr.device.GetDescriptorHandleIncrementSize(.RTV);
                break :blk descriptor;
            };

            gr.cmdlist.ResourceBarrier(1, &[_]w.D3D12_RESOURCE_BARRIER{.{
                .Type = .TRANSITION,
                .Flags = .{},
                .u = .{
                    .Transition = .{
                        .pResource = back_buffer,
                        .Subresource = w.D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = .{},
                        .StateAfter = .{ .RENDER_TARGET = true },
                    },
                },
            }});
            gr.cmdlist.OMSetRenderTargets(1, &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer_rtv}, w.TRUE, null);
            gr.cmdlist.ClearRenderTargetView(back_buffer_rtv, &[4]f32{ 0.2, 0.4, 0.8, 1.0 }, 0, null);
            gr.cmdlist.ResourceBarrier(1, &[_]w.D3D12_RESOURCE_BARRIER{.{
                .Type = .TRANSITION,
                .Flags = .{},
                .u = .{
                    .Transition = .{
                        .pResource = back_buffer,
                        .Subresource = w.D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = .{ .RENDER_TARGET = true },
                        .StateAfter = .{},
                    },
                },
            }});

            try vhr(gr.cmdlist.Close());
            gr.cmdqueue.ExecuteCommandLists(
                1,
                &[_]*w.ID3D12CommandList{@ptrCast(*w.ID3D12CommandList, gr.cmdlist)},
            );

            gr.frame_fence_counter += 1;
            try vhr(gr.swapchain.Present(0, .{}));
            try vhr(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));

            const gpu_frame_counter = gr.frame_fence.GetCompletedValue();
            if ((gr.frame_fence_counter - gpu_frame_counter) >= GraphicsContext.max_num_buffered_frames) {
                try vhr(gr.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gr.frame_fence_event));
                w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
            }

            gr.frame_index = (gr.frame_index + 1) % GraphicsContext.max_num_buffered_frames;
            gr.back_buffer_index = gr.swapchain.GetCurrentBackBufferIndex();
        }
    }

    gr.frame_fence_counter += 1;
    try vhr(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));
    try vhr(gr.frame_fence.SetEventOnCompletion(gr.frame_fence_counter, gr.frame_fence_event));
    w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;

    std.debug.print("All OK!\n", .{});
}
