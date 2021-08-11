const builtin = @import("builtin");
const std = @import("std");
const w = @import("../win32/win32.zig");
const c = @import("c.zig");
usingnamespace @import("vectormath.zig");
const assert = std.debug.assert;

pub inline fn vhr(hr: w.HRESULT) !void {
    if (hr != 0) {
        return error.HResult;
        //std.debug.panic("HRESULT function failed ({}).", .{hr});
    }
}

// TODO(mziulek): For now, we always transition *all* subresources.
const TransitionResourceBarrier = struct {
    state_before: w.D3D12_RESOURCE_STATES,
    state_after: w.D3D12_RESOURCE_STATES,
    resource: ResourceHandle,
};

pub const GraphicsContext = struct {
    const max_num_buffered_frames = 2;
    const num_swapbuffers = 4;
    const num_rtv_descriptors = 128;
    const num_dsv_descriptors = 128;
    const num_cbv_srv_uav_cpu_descriptors = 16 * 1024;
    const num_cbv_srv_uav_gpu_descriptors = 4 * 1024;
    const max_num_buffered_resource_barriers = 16;
    const upload_heap_capacity = 8 * 1024 * 1024;

    device: *w.ID3D12Device9,
    cmdqueue: *w.ID3D12CommandQueue,
    cmdlist: *w.ID3D12GraphicsCommandList6,
    cmdallocs: [max_num_buffered_frames]*w.ID3D12CommandAllocator,
    swapchain: *w.IDXGISwapChain3,
    swapchain_buffers: [num_swapbuffers]ResourceHandle,
    rtv_heap: DescriptorHeap,
    dsv_heap: DescriptorHeap,
    cbv_srv_uav_cpu_heap: DescriptorHeap,
    cbv_srv_uav_gpu_heaps: [max_num_buffered_frames]DescriptorHeap,
    upload_memory_heaps: [max_num_buffered_frames]GpuMemoryHeap,
    resource_pool: ResourcePool,
    pipeline: struct {
        pool: PipelinePool,
        map: std.AutoHashMapUnmanaged(u32, PipelineHandle),
        current: PipelineHandle,
    },
    transition_resource_barriers: []TransitionResourceBarrier,
    num_transition_resource_barriers: u32,
    viewport_width: u32,
    viewport_height: u32,
    frame_fence: *w.ID3D12Fence,
    frame_fence_event: w.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,
    window: w.HWND,
    is_cmdlist_opened: bool,
    d2d: struct {
        factory: *w.ID2D1Factory7,
        device: *w.ID2D1Device6,
        context: *w.ID2D1DeviceContext6,
        device11on12: *w.ID3D11On12Device2,
        device11: *w.ID3D11Device,
        context11: *w.ID3D11DeviceContext,
        swapbuffers11: [num_swapbuffers]*w.ID3D11Resource,
        targets: [num_swapbuffers]*w.ID2D1Bitmap1,
    },
    dwrite_factory: *w.IDWriteFactory,
    wic_factory: *w.IWICImagingFactory,

    pub fn init(window: w.HWND) !GraphicsContext {
        const wic_factory = blk: {
            var maybe_wic_factory: ?*w.IWICImagingFactory = null;
            try vhr(w.CoCreateInstance(
                &w.CLSID_WICImagingFactory,
                null,
                w.CLSCTX_INPROC_SERVER,
                &w.IID_IWICImagingFactory,
                @ptrCast(*?*c_void, &maybe_wic_factory),
            ));
            break :blk maybe_wic_factory.?;
        };
        errdefer _ = wic_factory.Release();

        const factory = blk: {
            var factory: *w.IDXGIFactory1 = undefined;
            try vhr(w.CreateDXGIFactory2(
                if (comptime builtin.mode == .Debug) w.DXGI_CREATE_FACTORY_DEBUG else 0,
                &w.IID_IDXGIFactory1,
                @ptrCast(*?*c_void, &factory),
            ));
            break :blk factory;
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
            var device: *w.ID3D12Device9 = undefined;
            try vhr(w.D3D12CreateDevice(null, ._11_1, &w.IID_ID3D12Device9, @ptrCast(*?*c_void, &device)));
            break :blk device;
        };
        errdefer _ = device.Release();

        const cmdqueue = blk: {
            var cmdqueue: *w.ID3D12CommandQueue = undefined;
            try vhr(device.CreateCommandQueue(&.{
                .Type = .DIRECT,
                .Priority = @enumToInt(w.D3D12_COMMAND_QUEUE_PRIORITY.NORMAL),
                .Flags = .{},
                .NodeMask = 0,
            }, &w.IID_ID3D12CommandQueue, @ptrCast(*?*c_void, &cmdqueue)));
            break :blk cmdqueue;
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
            try vhr(maybe_swapchain.?.QueryInterface(
                &w.IID_IDXGISwapChain3,
                @ptrCast(*?*c_void, &maybe_swapchain3),
            ));
            break :blk maybe_swapchain3.?;
        };
        errdefer _ = swapchain.Release();

        const d3d11 = blk: {
            var maybe_device11: ?*w.ID3D11Device = null;
            var maybe_device_context11: ?*w.ID3D11DeviceContext = null;
            try vhr(w.D3D11On12CreateDevice(
                @ptrCast(*w.IUnknown, device),
                if (comptime builtin.mode == .Debug) .{ .DEBUG = true, .BGRA_SUPPORT = true } else .{ .BGRA_SUPPORT = true },
                null,
                0,
                &[_]*w.IUnknown{@ptrCast(*w.IUnknown, cmdqueue)},
                1,
                0,
                &maybe_device11,
                &maybe_device_context11,
                null,
            ));
            break :blk .{ .device = maybe_device11.?, .device_context = maybe_device_context11.? };
        };
        errdefer {
            _ = d3d11.device.Release();
            _ = d3d11.device_context.Release();
        }

        const device11on12 = blk: {
            var device11on12: *w.ID3D11On12Device2 = undefined;
            try vhr(d3d11.device.QueryInterface(
                &w.IID_ID3D11On12Device2,
                @ptrCast(*?*c_void, &device11on12),
            ));
            break :blk device11on12;
        };
        errdefer _ = device11on12.Release();

        const d2d_factory = blk: {
            var d2d_factory: *w.ID2D1Factory7 = undefined;
            try vhr(w.D2D1CreateFactory(
                .SINGLE_THREADED,
                &w.IID_ID2D1Factory7,
                if (comptime builtin.mode == .Debug)
                    &w.D2D1_FACTORY_OPTIONS{ .debugLevel = .INFORMATION }
                else
                    &w.D2D1_FACTORY_OPTIONS{ .debugLevel = .NONE },
                @ptrCast(*?*c_void, &d2d_factory),
            ));
            break :blk d2d_factory;
        };
        errdefer _ = d2d_factory.Release();

        const dxgi_device = blk: {
            var maybe_dxgi_device: ?*w.IDXGIDevice = null;
            try vhr(device11on12.QueryInterface(&w.IID_IDXGIDevice, @ptrCast(*?*c_void, &maybe_dxgi_device)));
            break :blk maybe_dxgi_device.?;
        };
        defer _ = dxgi_device.Release();

        const d2d_device = blk: {
            var maybe_d2d_device: ?*w.ID2D1Device6 = null;
            try vhr(d2d_factory.CreateDevice6(dxgi_device, &maybe_d2d_device));
            break :blk maybe_d2d_device.?;
        };
        errdefer _ = d2d_device.Release();

        const d2d_device_context = blk: {
            var maybe_d2d_device_context: ?*w.ID2D1DeviceContext6 = null;
            try vhr(d2d_device.CreateDeviceContext6(.{}, &maybe_d2d_device_context));
            break :blk maybe_d2d_device_context.?;
        };
        errdefer _ = d2d_device_context.Release();

        const dwrite_factory = blk: {
            var dwrite_factory: *w.IDWriteFactory = undefined;
            try vhr(w.DWriteCreateFactory(.SHARED, &w.IID_IDWriteFactory, @ptrCast(*?*c_void, &dwrite_factory)));
            break :blk dwrite_factory;
        };
        errdefer _ = dwrite_factory.Release();

        var resource_pool = ResourcePool.init();
        errdefer resource_pool.deinit();

        var pipeline_pool = PipelinePool.init();
        errdefer pipeline_pool.deinit();

        var rtv_heap = try DescriptorHeap.init(device, num_rtv_descriptors, .RTV, .{});
        errdefer rtv_heap.deinit();

        var dsv_heap = try DescriptorHeap.init(device, num_dsv_descriptors, .DSV, .{});
        errdefer dsv_heap.deinit();

        var cbv_srv_uav_cpu_heap = try DescriptorHeap.init(
            device,
            num_cbv_srv_uav_cpu_descriptors,
            .CBV_SRV_UAV,
            .{},
        );
        errdefer cbv_srv_uav_cpu_heap.deinit();

        var cbv_srv_uav_gpu_heaps: [max_num_buffered_frames]DescriptorHeap = undefined;
        for (cbv_srv_uav_gpu_heaps) |_, heap_index| {
            cbv_srv_uav_gpu_heaps[heap_index] = DescriptorHeap.init(
                device,
                num_cbv_srv_uav_gpu_descriptors,
                .CBV_SRV_UAV,
                .{ .SHADER_VISIBLE = true },
            ) catch |err| {
                var i: u32 = 0;
                while (i < heap_index) : (i += 1) {
                    cbv_srv_uav_gpu_heaps[i].deinit();
                }
                return err;
            };
        }
        errdefer for (cbv_srv_uav_gpu_heaps) |*heap| heap.*.deinit();

        var upload_heaps: [max_num_buffered_frames]GpuMemoryHeap = undefined;
        for (upload_heaps) |_, heap_index| {
            upload_heaps[heap_index] = GpuMemoryHeap.init(device, upload_heap_capacity, .UPLOAD) catch |err| {
                var i: u32 = 0;
                while (i < heap_index) : (i += 1) {
                    upload_heaps[i].deinit();
                }
                return err;
            };
        }
        errdefer for (upload_heaps) |*heap| heap.*.deinit();

        const swapchain_buffers = blk: {
            var swapchain_buffers: [num_swapbuffers]ResourceHandle = undefined;
            var swapbuffers: [num_swapbuffers]*w.ID3D12Resource = undefined;
            for (swapbuffers) |_, buffer_index| {
                vhr(swapchain.GetBuffer(
                    @intCast(u32, buffer_index),
                    &w.IID_ID3D12Resource,
                    @ptrCast(*?*c_void, &swapbuffers[buffer_index]),
                )) catch |err| {
                    var i: u32 = 0;
                    while (i < buffer_index) : (i += 1) {
                        _ = swapbuffers[i].Release();
                    }
                    return err;
                };
                device.CreateRenderTargetView(
                    swapbuffers[buffer_index],
                    null,
                    rtv_heap.allocateDescriptors(1).cpu_handle,
                );
                swapchain_buffers[buffer_index] = resource_pool.addResource(swapbuffers[buffer_index], .{});
            }
            break :blk swapchain_buffers;
        };

        const swapbuffers11 = blk: {
            var swapbuffers11: [num_swapbuffers]*w.ID3D11Resource = undefined;
            for (swapbuffers11) |_, buffer_index| {
                vhr(device11on12.CreateWrappedResource(
                    @ptrCast(*w.IUnknown, resource_pool.getResource(swapchain_buffers[buffer_index]).raw.?),
                    &w.D3D11_RESOURCE_FLAGS{
                        .BindFlags = (w.D3D11_BIND_FLAG{ .RENDER_TARGET = true }).toInt(),
                        .MiscFlags = 0,
                        .CPUAccessFlags = 0,
                        .StructureByteStride = 0,
                    },
                    .{ .RENDER_TARGET = true },
                    w.D3D12_RESOURCE_STATE_PRESENT,
                    &w.IID_ID3D11Resource,
                    @ptrCast(*?*c_void, &swapbuffers11[buffer_index]),
                )) catch |err| {
                    var i: u32 = 0;
                    while (i < num_swapbuffers) : (i += 1) {
                        _ = swapbuffers11[i].Release();
                    }
                    return err;
                };
            }
            break :blk swapbuffers11;
        };
        errdefer {
            for (swapbuffers11) |swapbuffer11| _ = swapbuffer11.Release();
        }

        const d2d_targets = blk: {
            var maybe_d2d_targets = [_]?*w.ID2D1Bitmap1{null} ** num_swapbuffers;
            errdefer {
                for (maybe_d2d_targets) |target| {
                    if (target) |t| _ = t.Release();
                }
            }

            for (maybe_d2d_targets) |_, target_index| {
                const swapbuffer11 = swapbuffers11[target_index];

                var surface: *w.IDXGISurface = undefined;
                try vhr(swapbuffer11.QueryInterface(&w.IID_IDXGISurface, @ptrCast(*?*c_void, &surface)));
                defer _ = surface.Release();

                try vhr(d2d_device_context.CreateBitmapFromDxgiSurface(
                    surface,
                    &w.D2D1_BITMAP_PROPERTIES1{
                        .pixelFormat = .{ .format = .R8G8B8A8_UNORM, .alphaMode = .PREMULTIPLIED },
                        .dpiX = 96.0,
                        .dpiY = 96.0,
                        .bitmapOptions = .{ .TARGET = true, .CANNOT_DRAW = true },
                        .colorContext = null,
                    },
                    &maybe_d2d_targets[target_index],
                ));
            }

            var d2d_targets: [num_swapbuffers]*w.ID2D1Bitmap1 = undefined;
            for (d2d_targets) |_, i| d2d_targets[i] = maybe_d2d_targets[i].?;
            break :blk d2d_targets;
        };
        errdefer {
            for (d2d_targets) |target| _ = target.Release();
        }

        const frame_fence = blk: {
            var frame_fence: *w.ID3D12Fence = undefined;
            try vhr(device.CreateFence(0, .{}, &w.IID_ID3D12Fence, @ptrCast(*?*c_void, &frame_fence)));
            break :blk frame_fence;
        };
        errdefer _ = frame_fence.Release();

        const frame_fence_event = w.CreateEventEx(null, "frame_fence_event", 0, w.EVENT_ALL_ACCESS) catch unreachable;

        const cmdallocs = blk: {
            var cmdallocs: [max_num_buffered_frames]*w.ID3D12CommandAllocator = undefined;
            for (cmdallocs) |_, cmdalloc_index| {
                vhr(device.CreateCommandAllocator(
                    .DIRECT,
                    &w.IID_ID3D12CommandAllocator,
                    @ptrCast(*?*c_void, &cmdallocs[cmdalloc_index]),
                )) catch |err| {
                    var i: u32 = 0;
                    while (i < cmdalloc_index) : (i += 1) {
                        _ = cmdallocs[i].Release();
                    }
                    return err;
                };
            }
            break :blk cmdallocs;
        };
        errdefer {
            for (cmdallocs) |cmdalloc| _ = cmdalloc.Release();
        }

        const cmdlist = blk: {
            var cmdlist: *w.ID3D12GraphicsCommandList6 = undefined;
            try vhr(device.CreateCommandList(
                0,
                .DIRECT,
                cmdallocs[0],
                null,
                &w.IID_ID3D12GraphicsCommandList6,
                @ptrCast(*?*c_void, &cmdlist),
            ));
            break :blk cmdlist;
        };
        errdefer _ = cmdlist.Release();
        try vhr(cmdlist.Close());
        const is_cmdlist_opened = false;

        return GraphicsContext{
            .device = device,
            .cmdqueue = cmdqueue,
            .cmdlist = cmdlist,
            .cmdallocs = cmdallocs,
            .swapchain = swapchain,
            .swapchain_buffers = swapchain_buffers,
            .frame_fence = frame_fence,
            .frame_fence_event = frame_fence_event,
            .frame_fence_counter = 0,
            .rtv_heap = rtv_heap,
            .dsv_heap = dsv_heap,
            .cbv_srv_uav_cpu_heap = cbv_srv_uav_cpu_heap,
            .cbv_srv_uav_gpu_heaps = cbv_srv_uav_gpu_heaps,
            .upload_memory_heaps = upload_heaps,
            .resource_pool = resource_pool,
            .pipeline = .{
                .pool = pipeline_pool,
                .map = .{},
                .current = .{ .index = 0, .generation = 0 },
            },
            .transition_resource_barriers = std.heap.page_allocator.alloc(
                TransitionResourceBarrier,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
            .num_transition_resource_barriers = 0,
            .viewport_width = viewport_width,
            .viewport_height = viewport_height,
            .frame_index = 0,
            .back_buffer_index = swapchain.GetCurrentBackBufferIndex(),
            .window = window,
            .is_cmdlist_opened = is_cmdlist_opened,
            .d2d = .{
                .factory = d2d_factory,
                .device = d2d_device,
                .context = d2d_device_context,
                .device11on12 = device11on12,
                .device11 = d3d11.device,
                .context11 = d3d11.device_context,
                .swapbuffers11 = swapbuffers11,
                .targets = d2d_targets,
            },
            .dwrite_factory = dwrite_factory,
            .wic_factory = wic_factory,
        };
    }

    pub fn deinit(gr: *GraphicsContext, allocator: *std.mem.Allocator) void {
        gr.finishGpuCommands() catch unreachable;
        std.heap.page_allocator.free(gr.transition_resource_barriers);
        w.CloseHandle(gr.frame_fence_event);
        assert(gr.pipeline.map.count() == 0);
        gr.pipeline.map.deinit(allocator);
        gr.resource_pool.deinit();
        gr.rtv_heap.deinit();
        gr.dsv_heap.deinit();
        gr.cbv_srv_uav_cpu_heap.deinit();
        _ = gr.d2d.factory.Release();
        _ = gr.d2d.device.Release();
        _ = gr.d2d.context.Release();
        _ = gr.d2d.device11on12.Release();
        _ = gr.d2d.device11.Release();
        _ = gr.d2d.context11.Release();
        _ = gr.dwrite_factory.Release();
        for (gr.d2d.targets) |target| _ = target.Release();
        for (gr.d2d.swapbuffers11) |swapbuffer11| _ = swapbuffer11.Release();
        for (gr.cbv_srv_uav_gpu_heaps) |*heap| heap.*.deinit();
        for (gr.upload_memory_heaps) |*heap| heap.*.deinit();
        _ = gr.device.Release();
        _ = gr.cmdqueue.Release();
        _ = gr.swapchain.Release();
        _ = gr.frame_fence.Release();
        _ = gr.cmdlist.Release();
        for (gr.cmdallocs) |cmdalloc| _ = cmdalloc.Release();
        _ = gr.wic_factory.Release();
        gr.* = undefined;
    }

    pub fn beginFrame(gr: *GraphicsContext) !void {
        assert(!gr.is_cmdlist_opened);
        const cmdalloc = gr.cmdallocs[gr.frame_index];
        try vhr(cmdalloc.Reset());
        try vhr(gr.cmdlist.Reset(cmdalloc, null));
        gr.is_cmdlist_opened = true;
        gr.cmdlist.SetDescriptorHeaps(
            1,
            &[_]*w.ID3D12DescriptorHeap{gr.cbv_srv_uav_gpu_heaps[gr.frame_index].heap},
        );
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
        gr.pipeline.current = .{ .index = 0, .generation = 0 };
    }

    pub fn endFrame(gr: *GraphicsContext) !void {
        try gr.flushGpuCommands();

        gr.frame_fence_counter += 1;
        try vhr(gr.swapchain.Present(0, .{}));
        try vhr(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));

        const gpu_frame_counter = gr.frame_fence.GetCompletedValue();
        if ((gr.frame_fence_counter - gpu_frame_counter) >= max_num_buffered_frames) {
            try vhr(gr.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gr.frame_fence_event));
            w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
        }

        gr.frame_index = (gr.frame_index + 1) % max_num_buffered_frames;
        gr.back_buffer_index = gr.swapchain.GetCurrentBackBufferIndex();

        gr.cbv_srv_uav_gpu_heaps[gr.frame_index].size = 0;
        gr.upload_memory_heaps[gr.frame_index].size = 0;
    }

    pub fn beginDraw2d(gr: *GraphicsContext) !void {
        try gr.flushGpuCommands();

        gr.d2d.device11on12.AcquireWrappedResources(
            &[_]*w.ID3D11Resource{gr.d2d.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.context.SetTarget(@ptrCast(*w.ID2D1Image, gr.d2d.targets[gr.back_buffer_index]));
        gr.d2d.context.BeginDraw();
    }

    pub fn endDraw2d(gr: *GraphicsContext) !void {
        try vhr(gr.d2d.context.EndDraw(null, null));

        gr.d2d.device11on12.ReleaseWrappedResources(
            &[_]*w.ID3D11Resource{gr.d2d.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.context11.Flush();

        // Above calls will set back buffer state to PRESENT. We need to reflect this change
        // in 'resource_pool' by manually setting state.
        gr.resource_pool.editResource(gr.swapchain_buffers[gr.back_buffer_index]).*.state = w.D3D12_RESOURCE_STATE_PRESENT;
    }

    pub fn flushGpuCommands(gr: *GraphicsContext) !void {
        if (gr.is_cmdlist_opened) {
            gr.flushResourceBarriers();
            try vhr(gr.cmdlist.Close());
            gr.is_cmdlist_opened = false;
            gr.cmdqueue.ExecuteCommandLists(
                1,
                &[_]*w.ID3D12CommandList{@ptrCast(*w.ID3D12CommandList, gr.cmdlist)},
            );
        }
    }

    pub fn finishGpuCommands(gr: *GraphicsContext) !void {
        try gr.flushGpuCommands();

        gr.frame_fence_counter += 1;

        try vhr(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));
        try vhr(gr.frame_fence.SetEventOnCompletion(gr.frame_fence_counter, gr.frame_fence_event));
        w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;

        gr.cbv_srv_uav_gpu_heaps[gr.frame_index].size = 0;
        gr.upload_memory_heaps[gr.frame_index].size = 0;
    }

    pub fn getBackBuffer(gr: GraphicsContext) struct {
        resource_handle: ResourceHandle,
        descriptor_handle: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    } {
        return .{
            .resource_handle = gr.swapchain_buffers[gr.back_buffer_index],
            .descriptor_handle = .{
                .ptr = gr.rtv_heap.base.cpu_handle.ptr + gr.back_buffer_index * gr.rtv_heap.descriptor_size,
            },
        };
    }

    pub inline fn getResource(gr: GraphicsContext, handle: ResourceHandle) *w.ID3D12Resource {
        return gr.resource_pool.getResource(handle).raw.?;
    }

    pub fn getResourceSize(gr: GraphicsContext, handle: ResourceHandle) u64 {
        if (gr.resource_pool.isResourceValid(handle)) {
            const resource = gr.resource_pool.getResource(handle);
            assert(resource.desc.Dimension == .BUFFER);
            return resource.desc.Width;
        }
        return 0;
    }

    pub fn createCommittedResource(
        gr: *GraphicsContext,
        heap_type: w.D3D12_HEAP_TYPE,
        heap_flags: w.D3D12_HEAP_FLAGS,
        desc: *const w.D3D12_RESOURCE_DESC,
        initial_state: w.D3D12_RESOURCE_STATES,
        clear_value: ?*const w.D3D12_CLEAR_VALUE,
    ) !ResourceHandle {
        const resource = blk: {
            var maybe_resource: ?*w.ID3D12Resource = null;
            try vhr(gr.device.CreateCommittedResource(
                &w.D3D12_HEAP_PROPERTIES.initType(heap_type),
                heap_flags,
                desc,
                initial_state,
                clear_value,
                &w.IID_ID3D12Resource,
                @ptrCast(*?*c_void, &maybe_resource),
            ));
            break :blk maybe_resource.?;
        };
        return gr.resource_pool.addResource(resource, initial_state);
    }

    pub fn releaseResource(gr: *GraphicsContext, handle: ResourceHandle) u32 {
        if (gr.resource_pool.isResourceValid(handle)) {
            var resource = gr.resource_pool.editResource(handle);
            const refcount = resource.raw.?.Release();
            if (refcount == 0) {
                resource.* = .{ .raw = null, .state = .{}, .desc = w.D3D12_RESOURCE_DESC.initBuffer(0) };
            }
            return refcount;
        }
        return 0;
    }

    pub fn flushResourceBarriers(gr: *GraphicsContext) void {
        if (gr.num_transition_resource_barriers > 0) {
            var d3d12_barriers: [max_num_buffered_resource_barriers]w.D3D12_RESOURCE_BARRIER = undefined;

            var num_valid_barriers: u32 = 0;
            var barrier_index: u32 = 0;
            while (barrier_index < gr.num_transition_resource_barriers) : (barrier_index += 1) {
                const barrier = &gr.transition_resource_barriers[barrier_index];

                if (gr.resource_pool.isResourceValid(barrier.resource)) {
                    d3d12_barriers[num_valid_barriers] = .{
                        .Type = .TRANSITION,
                        .Flags = .{},
                        .u = .{
                            .Transition = .{
                                .pResource = gr.getResource(barrier.resource),
                                .Subresource = w.D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                                .StateBefore = barrier.state_before,
                                .StateAfter = barrier.state_after,
                            },
                        },
                    };
                    num_valid_barriers += 1;
                }
            }
            if (num_valid_barriers > 0) {
                gr.cmdlist.ResourceBarrier(num_valid_barriers, &d3d12_barriers);
            }
            gr.num_transition_resource_barriers = 0;
        }
    }

    pub fn addTransitionBarrier(
        gr: *GraphicsContext,
        handle: ResourceHandle,
        state_after: w.D3D12_RESOURCE_STATES,
    ) void {
        var resource = gr.resource_pool.editResource(handle);

        if (state_after.toInt() != resource.state.toInt()) {
            if (gr.num_transition_resource_barriers >= gr.transition_resource_barriers.len) {
                gr.flushResourceBarriers();
            }
            gr.transition_resource_barriers[gr.num_transition_resource_barriers] = .{
                .resource = handle,
                .state_before = resource.state,
                .state_after = state_after,
            };
        }
        gr.num_transition_resource_barriers += 1;
        resource.state = state_after;
    }

    pub fn createGraphicsShaderPipeline(
        gr: *GraphicsContext,
        allocator: *std.mem.Allocator,
        pso_desc: *w.D3D12_GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) !PipelineHandle {
        const vs_code = blk: {
            if (vs_cso_path) |path| {
                assert(pso_desc.VS.pShaderBytecode == null);
                const vs_file = try std.fs.cwd().openFile(path, .{});
                defer vs_file.close();
                const vs_code = try vs_file.reader().readAllAlloc(allocator, 256 * 1024);
                pso_desc.VS = .{ .pShaderBytecode = vs_code.ptr, .BytecodeLength = vs_code.len };
                break :blk vs_code;
            } else {
                assert(pso_desc.VS.pShaderBytecode != null);
                break :blk null;
            }
        };
        const ps_code = blk: {
            if (ps_cso_path) |path| {
                assert(pso_desc.PS.pShaderBytecode == null);
                const ps_file = try std.fs.cwd().openFile(path, .{});
                defer ps_file.close();
                const ps_code = try ps_file.reader().readAllAlloc(allocator, 256 * 1024);
                pso_desc.PS = .{ .pShaderBytecode = ps_code.ptr, .BytecodeLength = ps_code.len };
                break :blk ps_code;
            } else {
                assert(pso_desc.PS.pShaderBytecode != null);
                break :blk null;
            }
        };
        defer {
            if (vs_code) |code| allocator.free(code);
            if (ps_code) |code| allocator.free(code);
        }

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @ptrCast([*]const u8, pso_desc.VS.pShaderBytecode.?)[0..pso_desc.VS.BytecodeLength],
            );
            hasher.update(
                @ptrCast([*]const u8, pso_desc.PS.pShaderBytecode.?)[0..pso_desc.PS.BytecodeLength],
            );
            hasher.update(std.mem.asBytes(&pso_desc.BlendState));
            hasher.update(std.mem.asBytes(&pso_desc.SampleMask));
            hasher.update(std.mem.asBytes(&pso_desc.RasterizerState));
            hasher.update(std.mem.asBytes(&pso_desc.DepthStencilState));
            hasher.update(std.mem.asBytes(&pso_desc.IBStripCutValue));
            hasher.update(std.mem.asBytes(&pso_desc.PrimitiveTopologyType));
            hasher.update(std.mem.asBytes(&pso_desc.NumRenderTargets));
            hasher.update(std.mem.asBytes(&pso_desc.RTVFormats));
            hasher.update(std.mem.asBytes(&pso_desc.DSVFormat));
            hasher.update(std.mem.asBytes(&pso_desc.SampleDesc));
            // We don't support Stream Output.
            assert(pso_desc.StreamOutput.pSODeclaration == null);
            hasher.update(std.mem.asBytes(&pso_desc.InputLayout.NumElements));
            if (pso_desc.InputLayout.pInputElementDescs) |elements| {
                var i: u32 = 0;
                while (i < pso_desc.InputLayout.NumElements) : (i += 1) {
                    // TODO(mziulek): We ignore 'SemanticName' field here.
                    hasher.update(std.mem.asBytes(&elements[i].Format));
                    hasher.update(std.mem.asBytes(&elements[i].InputSlot));
                    hasher.update(std.mem.asBytes(&elements[i].AlignedByteOffset));
                    hasher.update(std.mem.asBytes(&elements[i].InputSlotClass));
                    hasher.update(std.mem.asBytes(&elements[i].InstanceDataStepRate));
                }
            }
            break :compute_hash hasher.final();
        };
        std.debug.print("PSO hash: {d}\n", .{hash});

        if (gr.pipeline.map.contains(hash)) {
            std.log.info("[graphics] Graphics pipeline hit detected.", .{});
            const handle = gr.pipeline.map.getEntry(hash).?.value_ptr.*;
            _ = incrementPipelineRefcount(gr.*, handle);
            return handle;
        }

        const rs = blk: {
            var maybe_rs: ?*w.ID3D12RootSignature = null;
            try vhr(gr.device.CreateRootSignature(
                0,
                pso_desc.VS.pShaderBytecode.?,
                pso_desc.VS.BytecodeLength,
                &w.IID_ID3D12RootSignature,
                @ptrCast(*?*c_void, &maybe_rs),
            ));
            break :blk maybe_rs.?;
        };
        errdefer _ = rs.Release();

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var maybe_pso: ?*w.ID3D12PipelineState = null;
            try vhr(gr.device.CreateGraphicsPipelineState(
                pso_desc,
                &w.IID_ID3D12PipelineState,
                @ptrCast(*?*c_void, &maybe_pso),
            ));
            break :blk maybe_pso.?;
        };
        errdefer _ = pso.Release();

        const handle = gr.pipeline.pool.addPipeline(pso, rs, .Graphics);
        gr.pipeline.map.put(allocator, hash, handle) catch unreachable;
        return handle;
    }

    pub fn setCurrentPipeline(gr: *GraphicsContext, pipeline_handle: PipelineHandle) void {
        assert(gr.is_cmdlist_opened);
        // TODO(mziulek): Do we need to unset pipeline state (null, null)?
        const pipeline = gr.pipeline.pool.getPipeline(pipeline_handle);

        if (pipeline_handle.index == gr.pipeline.current.index and
            pipeline_handle.generation == gr.pipeline.current.generation)
        {
            return;
        }

        gr.cmdlist.SetPipelineState(pipeline.pso.?);
        switch (pipeline.ptype.?) {
            .Graphics => gr.cmdlist.SetGraphicsRootSignature(pipeline.rs.?),
            .Compute => gr.cmdlist.SetComputeRootSignature(pipeline.rs.?),
        }

        gr.pipeline.current = pipeline_handle;
    }

    pub fn incrementPipelineRefcount(gr: GraphicsContext, handle: PipelineHandle) u32 {
        const pipeline = gr.pipeline.pool.getPipeline(handle);
        const refcount = pipeline.pso.?.AddRef();
        _ = pipeline.rs.?.AddRef();
        return refcount;
    }

    pub fn releasePipeline(gr: *GraphicsContext, handle: PipelineHandle) u32 {
        if (!gr.pipeline.pool.isPipelineValid(handle)) {
            return 0;
        }
        var pipeline = gr.pipeline.pool.editPipeline(handle);

        const refcount = pipeline.pso.?.Release();
        _ = pipeline.rs.?.Release();

        if (refcount == 0) {
            const hash_to_delete = blk: {
                var it = gr.pipeline.map.iterator();
                while (it.next()) |kv| {
                    if (kv.value_ptr.*.index == handle.index and
                        kv.value_ptr.*.generation == handle.generation)
                    {
                        break :blk kv.key_ptr.*;
                    }
                }
                unreachable;
            };
            _ = gr.pipeline.map.remove(hash_to_delete);
            pipeline.* = .{ .pso = null, .rs = null, .ptype = null };
        }
        return refcount;
    }

    pub fn allocateUploadMemory(
        gr: *GraphicsContext,
        size: u32,
    ) struct { cpu_slice: []u8, gpu_base: w.D3D12_GPU_VIRTUAL_ADDRESS } {
        assert(size > 0);
        var memory = gr.upload_memory_heaps[gr.frame_index].allocate(size);
        if (memory.cpu_slice == null or memory.gpu_base == null) {
            std.log.info("[graphics] Upload memory exhausted - waiting for a GPU... (cmdlist state is lost).", .{});

            gr.finishGpuCommands() catch unreachable;
            gr.beginFrame() catch unreachable;

            memory = gr.upload_memory_heaps[gr.frame_index].allocate(size);
        }
        return .{ .cpu_slice = memory.cpu_slice.?, .gpu_base = memory.gpu_base.? };
    }

    pub fn allocateUploadBufferRegion(
        gr: *GraphicsContext,
        comptime T: type,
        num_elements: u32,
    ) struct { cpu_slice: []T, buffer: *w.ID3D12Resource, buffer_offset: u64 } {
        assert(num_elements > 0);
        const size = num_elements * @sizeOf(T);
        const memory = gr.allocateUploadMemory(size);
        const aligned_size = (size + (GpuMemoryHeap.alloc_alignment - 1)) & ~(GpuMemoryHeap.alloc_alignment - 1);
        return .{
            .cpu_slice = std.mem.bytesAsSlice(T, @alignCast(@alignOf(T), memory.cpu_slice)),
            .buffer = gr.upload_memory_heaps[gr.frame_index].heap,
            .buffer_offset = gr.upload_memory_heaps[gr.frame_index].size - aligned_size,
        };
    }

    pub fn allocateCpuDescriptors(
        gr: *GraphicsContext,
        dtype: w.D3D12_DESCRIPTOR_HEAP_TYPE,
        num: u32,
    ) w.D3D12_CPU_DESCRIPTOR_HANDLE {
        assert(num > 0);
        switch (dtype) {
            .CBV_SRV_UAV => {
                assert(gr.cbv_srv_uav_cpu_heap.size_temp == 0);
                return gr.cbv_srv_uav_cpu_heap.allocateDescriptors(num).cpu_handle;
            },
            .RTV => {
                assert(gr.rtv_heap.size_temp == 0);
                return gr.rtv_heap.allocateDescriptors(num).cpu_handle;
            },
            .DSV => {
                assert(gr.dsv_heap.size_temp == 0);
                return gr.dsv_heap.allocateDescriptors(num).cpu_handle;
            },
            .SAMPLER => unreachable,
        }
    }

    pub inline fn allocateGpuDescriptors(gr: *GraphicsContext, num_descriptors: u32) Descriptor {
        return gr.cbv_srv_uav_gpu_heaps[gr.frame_index].allocateDescriptors(num_descriptors);
    }

    pub fn copyDescriptorsToGpuHeap(
        gr: *GraphicsContext,
        num: u32,
        src_base_handle: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    ) w.D3D12_GPU_DESCRIPTOR_HANDLE {
        const base = gr.allocateGpuDescriptors(num);
        gr.device.CopyDescriptorsSimple(num, base.cpu_handle, src_base_handle, .CBV_SRV_UAV);
        return base.gpu_handle;
    }

    pub fn updateTex2dSubresource(
        gr: *GraphicsContext,
        texture: ResourceHandle,
        subresource: u32,
        data: []const u8,
        row_pitch: u32,
    ) void {
        assert(gr.is_cmdlist_opened);
        const resource = gr.resource_pool.getResource(texture);
        assert(resource.desc.Dimension == .TEXTURE2D);

        var layout: [1]w.D3D12_PLACED_SUBRESOURCE_FOOTPRINT = undefined;
        var required_size: u64 = undefined;
        gr.device.GetCopyableFootprints(&resource.desc, subresource, layout.len, 0, &layout, null, null, &required_size);

        const upload = gr.allocateUploadBufferRegion(u8, @intCast(u32, required_size));
        layout[0].Offset = upload.buffer_offset;

        const pixel_size = resource.desc.Format.pixelSizeInBytes();
        var y: u32 = 0;
        while (y < layout[0].Footprint.Height) : (y += 1) {
            var x: u32 = 0;
            while (x < layout[0].Footprint.Width * pixel_size) : (x += 1) {
                upload.cpu_slice[y * layout[0].Footprint.RowPitch + x] = data[y * row_pitch + x];
            }
        }

        gr.addTransitionBarrier(texture, .{ .COPY_DEST = true });
        gr.flushResourceBarriers();

        gr.cmdlist.CopyTextureRegion(&w.D3D12_TEXTURE_COPY_LOCATION{
            .pResource = gr.getResource(texture),
            .Type = .SUBRESOURCE_INDEX,
            .u = .{
                .SubresourceIndex = subresource,
            },
        }, 0, 0, 0, &w.D3D12_TEXTURE_COPY_LOCATION{
            .pResource = upload.buffer,
            .Type = .PLACED_FOOTPRINT,
            .u = .{
                .PlacedFootprint = layout[0],
            },
        }, null);
    }

    pub fn createAndUploadTex2dFromFile(gr: *GraphicsContext, path: []const u16, num_mip_levels: i32) !ResourceHandle {
        // TODO(mziulek): Is this the correct way? We want to make sure that slice is ended with '0' (comes from [*:0] str).
        assert(path.ptr[path.len] == 0);
        _ = num_mip_levels;

        const bmp_decoder = blk: {
            var maybe_bmp_decoder: ?*w.IWICBitmapDecoder = undefined;
            try vhr(gr.wic_factory.CreateDecoderFromFilename(
                @ptrCast(w.LPCWSTR, path.ptr),
                null,
                w.GENERIC_READ,
                .MetadataCacheOnDemand,
                &maybe_bmp_decoder,
            ));
            break :blk maybe_bmp_decoder.?;
        };
        defer _ = bmp_decoder.Release();

        return ResourceHandle{ .index = 0, .generation = 0 };
    }
};

pub const GuiContext = struct {
    font: ResourceHandle,
    font_srv: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    pipeline: PipelineHandle,
    vb: [GraphicsContext.max_num_buffered_frames]ResourceHandle,
    ib: [GraphicsContext.max_num_buffered_frames]ResourceHandle,
    vb_cpu_addr: [GraphicsContext.max_num_buffered_frames][]align(8) u8,
    ib_cpu_addr: [GraphicsContext.max_num_buffered_frames][]align(8) u8,

    pub fn init(allocator: *std.mem.Allocator, gr: *GraphicsContext) !GuiContext {
        assert(gr.is_cmdlist_opened);
        assert(c.igGetCurrentContext() != null);

        var io = c.igGetIO().?;
        io.*.KeyMap[c.ImGuiKey_Tab] = w.VK_TAB;
        io.*.KeyMap[c.ImGuiKey_LeftArrow] = w.VK_LEFT;
        io.*.KeyMap[c.ImGuiKey_RightArrow] = w.VK_RIGHT;
        io.*.KeyMap[c.ImGuiKey_UpArrow] = w.VK_UP;
        io.*.KeyMap[c.ImGuiKey_DownArrow] = w.VK_DOWN;
        io.*.KeyMap[c.ImGuiKey_PageUp] = w.VK_PRIOR;
        io.*.KeyMap[c.ImGuiKey_PageDown] = w.VK_NEXT;
        io.*.KeyMap[c.ImGuiKey_Home] = w.VK_HOME;
        io.*.KeyMap[c.ImGuiKey_End] = w.VK_END;
        io.*.KeyMap[c.ImGuiKey_Delete] = w.VK_DELETE;
        io.*.KeyMap[c.ImGuiKey_Backspace] = w.VK_BACK;
        io.*.KeyMap[c.ImGuiKey_Enter] = w.VK_RETURN;
        io.*.KeyMap[c.ImGuiKey_Escape] = w.VK_ESCAPE;
        io.*.KeyMap[c.ImGuiKey_A] = 'A';
        io.*.KeyMap[c.ImGuiKey_C] = 'C';
        io.*.KeyMap[c.ImGuiKey_V] = 'V';
        io.*.KeyMap[c.ImGuiKey_X] = 'X';
        io.*.KeyMap[c.ImGuiKey_Y] = 'Y';
        io.*.KeyMap[c.ImGuiKey_Z] = 'Z';
        io.*.ImeWindowHandle = gr.window;
        io.*.DisplaySize = .{ .x = @intToFloat(f32, gr.viewport_width), .y = @intToFloat(f32, gr.viewport_height) };
        c.igGetStyle().?.*.WindowRounding = 0.0;

        _ = c.ImFontAtlas_AddFontFromFileTTF(io.*.Fonts, "content/Roboto-Medium.ttf", 24.0, null, null);
        const font_info = blk: {
            var pp: [*c]u8 = undefined;
            var ww: i32 = undefined;
            var hh: i32 = undefined;
            c.ImFontAtlas_GetTexDataAsRGBA32(io.*.Fonts, &pp, &ww, &hh, null);
            break :blk .{
                .pixels = pp[0..@intCast(usize, ww * hh * 4)],
                .width = @intCast(u32, ww),
                .height = @intCast(u32, hh),
            };
        };

        const font = try gr.createCommittedResource(
            .DEFAULT,
            .{},
            &w.D3D12_RESOURCE_DESC.initTex2d(font_info.width, font_info.height, .R8G8B8A8_UNORM, 1),
            .{ .COPY_DEST = true },
            null,
        );
        errdefer _ = gr.releaseResource(font);

        gr.updateTex2dSubresource(font, 0, font_info.pixels, font_info.width * 4);
        gr.addTransitionBarrier(font, .{ .PIXEL_SHADER_RESOURCE = true });

        const font_srv = gr.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gr.device.CreateShaderResourceView(gr.getResource(font), null, font_srv);

        const pipeline = blk: {
            const input_layout_desc = [_]w.D3D12_INPUT_ELEMENT_DESC{
                w.D3D12_INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                w.D3D12_INPUT_ELEMENT_DESC.init("_Uv", 0, .R32G32_FLOAT, 0, 8, .PER_VERTEX_DATA, 0),
                w.D3D12_INPUT_ELEMENT_DESC.init("_Color", 0, .R8G8B8A8_UNORM, 0, 16, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = w.D3D12_GRAPHICS_PIPELINE_STATE_DESC.initDefault();
            pso_desc.RasterizerState.CullMode = .NONE;
            pso_desc.DepthStencilState.DepthEnable = w.FALSE;
            pso_desc.BlendState.RenderTarget[0].BlendEnable = w.TRUE;
            pso_desc.BlendState.RenderTarget[0].SrcBlend = .SRC_ALPHA;
            pso_desc.BlendState.RenderTarget[0].DestBlend = .INV_SRC_ALPHA;
            pso_desc.BlendState.RenderTarget[0].BlendOp = .ADD;
            pso_desc.BlendState.RenderTarget[0].SrcBlendAlpha = .INV_SRC_ALPHA;
            pso_desc.BlendState.RenderTarget[0].DestBlendAlpha = .ZERO;
            pso_desc.BlendState.RenderTarget[0].BlendOpAlpha = .ADD;
            pso_desc.InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            };
            break :blk try gr.createGraphicsShaderPipeline(
                allocator,
                &pso_desc,
                "content/shaders/imgui.vs.cso",
                "content/shaders/imgui.ps.cso",
            );
        };
        errdefer _ = gr.releasePipeline(pipeline);

        return GuiContext{
            .font = font,
            .font_srv = font_srv,
            .pipeline = pipeline,
            .vb = [_]ResourceHandle{.{ .index = 0, .generation = 0 }} ** GraphicsContext.max_num_buffered_frames,
            .ib = [_]ResourceHandle{.{ .index = 0, .generation = 0 }} ** GraphicsContext.max_num_buffered_frames,
            .vb_cpu_addr = [_][]align(8) u8{&.{}} ** GraphicsContext.max_num_buffered_frames,
            .ib_cpu_addr = [_][]align(8) u8{&.{}} ** GraphicsContext.max_num_buffered_frames,
        };
    }

    pub fn deinit(gui: *GuiContext, gr: *GraphicsContext) void {
        gr.finishGpuCommands() catch unreachable;
        _ = gr.releasePipeline(gui.pipeline);
        _ = gr.releaseResource(gui.font);
        for (gui.vb) |vb| _ = gr.releaseResource(vb);
        for (gui.ib) |ib| _ = gr.releaseResource(ib);
        gui.* = undefined;
    }

    pub fn update(delta_time: f32) void {
        assert(c.igGetCurrentContext() != null);
        var io = c.igGetIO().?;
        io.*.KeyCtrl = w.GetAsyncKeyState(w.VK_CONTROL) < 0;
        io.*.KeyShift = w.GetAsyncKeyState(w.VK_SHIFT) < 0;
        io.*.KeyAlt = w.GetAsyncKeyState(w.VK_MENU) < 0;
        io.*.DeltaTime = delta_time;
        c.igNewFrame();
    }

    pub fn draw(gui: *GuiContext, gr: *GraphicsContext) !void {
        assert(gr.is_cmdlist_opened);
        assert(c.igGetCurrentContext() != null);

        c.igRender();
        const draw_data = c.igGetDrawData();
        if (draw_data == null or draw_data.?.*.TotalVtxCount == 0) {
            return;
        }
        const num_vertices = @intCast(u32, draw_data.?.*.TotalVtxCount);
        const num_indices = @intCast(u32, draw_data.?.*.TotalIdxCount);

        var vb = gui.vb[gr.frame_index];
        var ib = gui.ib[gr.frame_index];

        if (gr.getResourceSize(vb) < num_vertices * @sizeOf(c.ImDrawVert)) {
            _ = gr.releaseResource(vb);
            const new_size = 2 * num_vertices * @sizeOf(c.ImDrawVert);
            vb = try gr.createCommittedResource(
                .UPLOAD,
                .{},
                &w.D3D12_RESOURCE_DESC.initBuffer(new_size),
                w.D3D12_RESOURCE_STATE_GENERIC_READ,
                null,
            );
            gui.vb[gr.frame_index] = vb;
            gui.vb_cpu_addr[gr.frame_index] = blk: {
                var ptr: ?[*]align(8) u8 = null;
                try vhr(gr.getResource(vb).Map(0, &.{ .Begin = 0, .End = 0 }, @ptrCast(*?*c_void, &ptr)));
                break :blk ptr.?[0..new_size];
            };
        }
        if (gr.getResourceSize(ib) < num_indices * @sizeOf(c.ImDrawIdx)) {
            _ = gr.releaseResource(ib);
            const new_size = 2 * num_indices * @sizeOf(c.ImDrawIdx);
            ib = try gr.createCommittedResource(
                .UPLOAD,
                .{},
                &w.D3D12_RESOURCE_DESC.initBuffer(new_size),
                w.D3D12_RESOURCE_STATE_GENERIC_READ,
                null,
            );
            gui.ib[gr.frame_index] = ib;
            gui.ib_cpu_addr[gr.frame_index] = blk: {
                var ptr: ?[*]align(8) u8 = null;
                try vhr(gr.getResource(ib).Map(0, &.{ .Begin = 0, .End = 0 }, @ptrCast(*?*c_void, &ptr)));
                break :blk ptr.?[0..new_size];
            };
        }
        // Update vertex and index buffers.
        {
            var vb_slice = std.mem.bytesAsSlice(c.ImDrawVert, gui.vb_cpu_addr[gr.frame_index]);
            var ib_slice = std.mem.bytesAsSlice(c.ImDrawIdx, gui.ib_cpu_addr[gr.frame_index]);
            var vb_idx: u32 = 0;
            var ib_idx: u32 = 0;
            var cmdlist_idx: u32 = 0;
            const num_cmdlists = @intCast(u32, draw_data.?.*.CmdListsCount);
            while (cmdlist_idx < num_cmdlists) : (cmdlist_idx += 1) {
                const list = draw_data.?.*.CmdLists[cmdlist_idx];
                const list_vb_size = @intCast(u32, list.*.VtxBuffer.Size);
                const list_ib_size = @intCast(u32, list.*.IdxBuffer.Size);
                std.mem.copy(
                    c.ImDrawVert,
                    vb_slice[vb_idx .. vb_idx + list_vb_size],
                    list.*.VtxBuffer.Data[0..list_vb_size],
                );
                std.mem.copy(
                    c.ImDrawIdx,
                    ib_slice[ib_idx .. ib_idx + list_ib_size],
                    list.*.IdxBuffer.Data[0..list_ib_size],
                );
                vb_idx += list_vb_size;
                ib_idx += list_ib_size;
            }
        }

        const io = c.igGetIO().?;
        const vp_width = io.*.DisplaySize.x * io.*.DisplayFramebufferScale.x;
        const vp_height = io.*.DisplaySize.y * io.*.DisplayFramebufferScale.y;
        gr.cmdlist.RSSetViewports(1, &[_]w.D3D12_VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = vp_width,
            .Height = vp_height,
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        gr.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        gr.setCurrentPipeline(gui.pipeline);
        {
            const mem = gr.allocateUploadMemory(@sizeOf(Mat4));
            const xform = mat4.transpose(mat4.initOrthoOffCenterLh(0.0, vp_width, vp_height, 0.0, 0.0, 1.0));
            @memcpy(mem.cpu_slice.ptr, @ptrCast([*]const u8, &xform[0][0]), @sizeOf(Mat4));

            gr.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }
        gr.cmdlist.SetGraphicsRootDescriptorTable(1, gr.copyDescriptorsToGpuHeap(1, gui.font_srv));
        gr.cmdlist.IASetVertexBuffers(0, 1, &[_]w.D3D12_VERTEX_BUFFER_VIEW{.{
            .BufferLocation = gr.getResource(vb).GetGPUVirtualAddress(),
            .SizeInBytes = num_vertices * @sizeOf(c.ImDrawVert),
            .StrideInBytes = @sizeOf(c.ImDrawVert),
        }});
        gr.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = gr.getResource(ib).GetGPUVirtualAddress(),
            .SizeInBytes = num_indices * @sizeOf(c.ImDrawIdx),
            .Format = if (@sizeOf(c.ImDrawIdx) == 2) .R16_UINT else .R32_UINT,
        });

        var vertex_offset: i32 = 0;
        var index_offset: u32 = 0;

        var cmdlist_idx: u32 = 0;
        const num_cmdlists = @intCast(u32, draw_data.?.*.CmdListsCount);
        while (cmdlist_idx < num_cmdlists) : (cmdlist_idx += 1) {
            const cmdlist = draw_data.?.*.CmdLists[cmdlist_idx];

            var cmd_idx: u32 = 0;
            const num_cmds = cmdlist.*.CmdBuffer.Size;
            while (cmd_idx < num_cmds) : (cmd_idx += 1) {
                const cmd = &cmdlist.*.CmdBuffer.Data[cmd_idx];

                if (cmd.*.UserCallback != null) {
                    // TODO(mziulek): Call the callback.
                } else {
                    gr.cmdlist.RSSetScissorRects(1, &[_]w.D3D12_RECT{.{
                        .left = @floatToInt(i32, cmd.*.ClipRect.x),
                        .top = @floatToInt(i32, cmd.*.ClipRect.y),
                        .right = @floatToInt(i32, cmd.*.ClipRect.z),
                        .bottom = @floatToInt(i32, cmd.*.ClipRect.w),
                    }});
                    gr.cmdlist.DrawIndexedInstanced(cmd.*.ElemCount, 1, index_offset, vertex_offset, 0);
                }
                index_offset += cmd.*.ElemCount;
            }
            vertex_offset += cmdlist.*.VtxBuffer.Size;
        }
    }
};

pub const ResourceHandle = struct {
    index: u16 align(4),
    generation: u16,
};

const Resource = struct {
    raw: ?*w.ID3D12Resource,
    state: w.D3D12_RESOURCE_STATES,
    desc: w.D3D12_RESOURCE_DESC,
};

const ResourcePool = struct {
    const max_num_resources = 256;

    resources: []Resource,
    generations: []u16,

    fn init() ResourcePool {
        return .{
            .resources = blk: {
                var resources = std.heap.page_allocator.alloc(
                    Resource,
                    max_num_resources + 1,
                ) catch unreachable;
                for (resources) |*res| {
                    res.* = .{ .raw = null, .state = .{}, .desc = w.D3D12_RESOURCE_DESC.initBuffer(0) };
                }
                break :blk resources;
            },
            .generations = blk: {
                var generations = std.heap.page_allocator.alloc(
                    u16,
                    max_num_resources + 1,
                ) catch unreachable;
                for (generations) |*gen| gen.* = 0;
                break :blk generations;
            },
        };
    }

    fn deinit(pool: *ResourcePool) void {
        for (pool.resources) |resource, i| {
            if (i > 0 and i <= GraphicsContext.num_swapbuffers) {
                // Release internally created swapbuffers.
                if (resource.raw) |raw| {
                    _ = raw.Release();
                }
            } else if (i > GraphicsContext.num_swapbuffers) {
                // Verify that all resources has been released by a user.
                assert(resource.raw == null);
            }
        }
        std.heap.page_allocator.free(pool.resources);
        std.heap.page_allocator.free(pool.generations);
        pool.* = undefined;
    }

    fn addResource(
        pool: *ResourcePool,
        raw: *w.ID3D12Resource,
        state: w.D3D12_RESOURCE_STATES,
    ) ResourceHandle {
        var slot_idx: u32 = 1;
        while (slot_idx <= max_num_resources) : (slot_idx += 1) {
            if (pool.resources[slot_idx].raw == null)
                break;
        }
        assert(slot_idx <= max_num_resources);

        pool.resources[slot_idx] = .{ .raw = raw, .state = state, .desc = raw.GetDesc() };
        return .{
            .index = @intCast(u16, slot_idx),
            .generation = blk: {
                pool.generations[slot_idx] += 1;
                break :blk pool.generations[slot_idx];
            },
        };
    }

    fn isResourceValid(pool: ResourcePool, handle: ResourceHandle) bool {
        return handle.index > 0 and
            handle.index <= max_num_resources and
            handle.generation > 0 and
            handle.generation == pool.generations[handle.index] and
            pool.resources[handle.index].raw != null;
    }

    fn editResource(pool: *ResourcePool, handle: ResourceHandle) *Resource {
        assert(pool.isResourceValid(handle));
        return &pool.resources[handle.index];
    }

    fn getResource(pool: ResourcePool, handle: ResourceHandle) *const Resource {
        assert(pool.isResourceValid(handle));
        return &pool.resources[handle.index];
    }
};

pub const PipelineHandle = struct {
    index: u16 align(4),
    generation: u16,
};

const PipelineType = enum {
    Graphics,
    Compute,
};

const Pipeline = struct {
    pso: ?*w.ID3D12PipelineState,
    rs: ?*w.ID3D12RootSignature,
    ptype: ?PipelineType,
};

const PipelinePool = struct {
    const max_num_pipelines = 256;

    pipelines: []Pipeline,
    generations: []u16,

    fn init() PipelinePool {
        return .{
            .pipelines = blk: {
                var pipelines = std.heap.page_allocator.alloc(
                    Pipeline,
                    max_num_pipelines + 1,
                ) catch unreachable;
                for (pipelines) |*pipeline| {
                    pipeline.* = .{ .pso = null, .rs = null, .ptype = null };
                }
                break :blk pipelines;
            },
            .generations = blk: {
                var generations = std.heap.page_allocator.alloc(
                    u16,
                    max_num_pipelines + 1,
                ) catch unreachable;
                for (generations) |*gen| gen.* = 0;
                break :blk generations;
            },
        };
    }

    fn deinit(pool: *PipelinePool) void {
        for (pool.pipelines) |pipeline| {
            // Verify that all pipelines has been released by a user.
            assert(pipeline.pso == null);
            assert(pipeline.rs == null);
        }
        std.heap.page_allocator.free(pool.pipelines);
        std.heap.page_allocator.free(pool.generations);
        pool.* = undefined;
    }

    fn addPipeline(
        pool: *PipelinePool,
        pso: *w.ID3D12PipelineState,
        rs: *w.ID3D12RootSignature,
        ptype: PipelineType,
    ) PipelineHandle {
        var slot_idx: u32 = 1;
        while (slot_idx <= max_num_pipelines) : (slot_idx += 1) {
            if (pool.pipelines[slot_idx].pso == null)
                break;
        }
        assert(slot_idx <= max_num_pipelines);

        pool.pipelines[slot_idx] = .{ .pso = pso, .rs = rs, .ptype = ptype };
        return .{
            .index = @intCast(u16, slot_idx),
            .generation = blk: {
                pool.generations[slot_idx] += 1;
                break :blk pool.generations[slot_idx];
            },
        };
    }

    fn isPipelineValid(pool: PipelinePool, handle: PipelineHandle) bool {
        return handle.index > 0 and
            handle.index <= max_num_pipelines and
            handle.generation > 0 and
            handle.generation == pool.generations[handle.index] and
            pool.pipelines[handle.index].pso != null and
            pool.pipelines[handle.index].rs != null and
            pool.pipelines[handle.index].ptype != null;
    }

    fn editPipeline(pool: PipelinePool, handle: PipelineHandle) *Pipeline {
        assert(pool.isPipelineValid(handle));
        return &pool.pipelines[handle.index];
    }

    fn getPipeline(pool: PipelinePool, handle: PipelineHandle) *const Pipeline {
        assert(pool.isPipelineValid(handle));
        return &pool.pipelines[handle.index];
    }
};

const Descriptor = struct {
    cpu_handle: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    gpu_handle: w.D3D12_GPU_DESCRIPTOR_HANDLE,
};

const DescriptorHeap = struct {
    heap: *w.ID3D12DescriptorHeap,
    base: Descriptor,
    size: u32,
    size_temp: u32,
    capacity: u32,
    descriptor_size: u32,

    fn init(
        device: *w.ID3D12Device9,
        capacity: u32,
        heap_type: w.D3D12_DESCRIPTOR_HEAP_TYPE,
        flags: w.D3D12_DESCRIPTOR_HEAP_FLAGS,
    ) !DescriptorHeap {
        assert(capacity > 0);
        const heap = blk: {
            var maybe_heap: ?*w.ID3D12DescriptorHeap = null;
            try vhr(device.CreateDescriptorHeap(&.{
                .Type = heap_type,
                .NumDescriptors = capacity,
                .Flags = flags,
                .NodeMask = 0,
            }, &w.IID_ID3D12DescriptorHeap, @ptrCast(*?*c_void, &maybe_heap)));
            break :blk maybe_heap.?;
        };
        return DescriptorHeap{
            .heap = heap,
            .base = .{
                .cpu_handle = heap.GetCPUDescriptorHandleForHeapStart(),
                .gpu_handle = blk: {
                    if (flags.SHADER_VISIBLE == true)
                        break :blk heap.GetGPUDescriptorHandleForHeapStart();
                    break :blk w.D3D12_GPU_DESCRIPTOR_HANDLE{ .ptr = 0 };
                },
            },
            .size = 0,
            .size_temp = 0,
            .capacity = capacity,
            .descriptor_size = device.GetDescriptorHandleIncrementSize(heap_type),
        };
    }

    fn deinit(dheap: *DescriptorHeap) void {
        _ = dheap.heap.Release();
        dheap.* = undefined;
    }

    fn allocateDescriptors(dheap: *DescriptorHeap, num_descriptors: u32) Descriptor {
        assert(num_descriptors > 0);
        assert((dheap.size + num_descriptors) < dheap.capacity);

        const cpu_handle = w.D3D12_CPU_DESCRIPTOR_HANDLE{
            .ptr = dheap.base.cpu_handle.ptr + dheap.size * dheap.descriptor_size,
        };
        const gpu_handle = w.D3D12_GPU_DESCRIPTOR_HANDLE{
            .ptr = blk: {
                if (dheap.base.gpu_handle.ptr != 0)
                    break :blk dheap.base.gpu_handle.ptr + dheap.size * dheap.descriptor_size;
                break :blk 0;
            },
        };

        dheap.size += num_descriptors;
        return .{ .cpu_handle = cpu_handle, .gpu_handle = gpu_handle };
    }
};

const GpuMemoryHeap = struct {
    const alloc_alignment: u32 = 512;

    heap: *w.ID3D12Resource,
    cpu_slice: []u8,
    gpu_base: w.D3D12_GPU_VIRTUAL_ADDRESS,
    size: u32,
    capacity: u32,

    fn init(device: *w.ID3D12Device9, capacity: u32, heap_type: w.D3D12_HEAP_TYPE) !GpuMemoryHeap {
        assert(capacity > 0);
        const resource = blk: {
            var maybe_resource: ?*w.ID3D12Resource = null;
            try vhr(device.CreateCommittedResource(
                &w.D3D12_HEAP_PROPERTIES.initType(heap_type),
                .{},
                &w.D3D12_RESOURCE_DESC.initBuffer(capacity),
                w.D3D12_RESOURCE_STATE_GENERIC_READ,
                null,
                &w.IID_ID3D12Resource,
                @ptrCast(*?*c_void, &maybe_resource),
            ));
            break :blk maybe_resource.?;
        };
        errdefer _ = resource.Release();

        const cpu_base = blk: {
            var maybe_cpu_base: ?[*]u8 = null;
            try vhr(resource.Map(
                0,
                &w.D3D12_RANGE{ .Begin = 0, .End = 0 },
                @ptrCast(*?*c_void, &maybe_cpu_base),
            ));
            break :blk maybe_cpu_base.?;
        };
        return GpuMemoryHeap{
            .heap = resource,
            .cpu_slice = cpu_base[0..capacity],
            .gpu_base = resource.GetGPUVirtualAddress(),
            .size = 0,
            .capacity = capacity,
        };
    }

    fn deinit(mheap: *GpuMemoryHeap) void {
        _ = mheap.heap.Release();
        mheap.* = undefined;
    }

    fn allocate(
        mheap: *GpuMemoryHeap,
        size: u32,
    ) struct { cpu_slice: ?[]u8, gpu_base: ?w.D3D12_GPU_VIRTUAL_ADDRESS } {
        assert(size > 0);

        const aligned_size = (size + (alloc_alignment - 1)) & ~(alloc_alignment - 1);
        if ((mheap.size + aligned_size) >= mheap.capacity) {
            return .{ .cpu_slice = null, .gpu_base = null };
        }
        const cpu_slice = (mheap.cpu_slice.ptr + mheap.size)[0..size];
        const gpu_base = mheap.gpu_base + mheap.size;

        mheap.size += aligned_size;
        return .{ .cpu_slice = cpu_slice, .gpu_base = gpu_base };
    }
};
