const builtin = @import("builtin");
const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const dwrite = win32.dwrite;
const dxgi = win32.dxgi;
const d3d11 = win32.d3d11;
const d3d12 = win32.d3d12;
const d3d12d = win32.d3d12d;
const d2d1 = win32.d2d1;
const d3d11on12 = win32.d3d11on12;
const wic = win32.wic;
const c = @import("c.zig");
const lib = @import("library.zig");
const vm = @import("vectormath.zig");
const tracy = @import("tracy.zig");
const assert = std.debug.assert;
const HResultError = lib.HResultError;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const hrErrorOnFail = lib.hrErrorOnFail;

const enable_dx_debug = @import("build_options").enable_dx_debug;
const enable_dx_gpu_debug = @import("build_options").enable_dx_gpu_debug;

// TODO(mziulek): For now, we always transition *all* subresources.
const TransitionResourceBarrier = struct {
    state_before: d3d12.RESOURCE_STATES,
    state_after: d3d12.RESOURCE_STATES,
    resource: ResourceHandle,
};

const ResourceWithCounter = struct {
    resource: ResourceHandle,
    counter: u32,
};

pub const GraphicsContext = struct {
    const max_num_buffered_frames = 2;
    const num_swapbuffers = 4;
    const num_rtv_descriptors = 128;
    const num_dsv_descriptors = 128;
    const num_cbv_srv_uav_cpu_descriptors = 16 * 1024;
    const num_cbv_srv_uav_gpu_descriptors = 4 * 1024;
    const max_num_buffered_resource_barriers = 16;
    const upload_heap_capacity = 18 * 1024 * 1024;

    device: *d3d12.IDevice9,
    cmdqueue: *d3d12.ICommandQueue,
    cmdlist: *d3d12.IGraphicsCommandList6,
    cmdallocs: [max_num_buffered_frames]*d3d12.ICommandAllocator,
    swapchain: *dxgi.ISwapChain3,
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
    resources_to_release: std.ArrayList(ResourceWithCounter),
    viewport_width: u32,
    viewport_height: u32,
    frame_fence: *d3d12.IFence,
    frame_fence_event: w.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,
    window: w.HWND,
    is_cmdlist_opened: bool,
    d2d: struct {
        factory: *d2d1.IFactory7,
        device: *d2d1.IDevice6,
        context: *d2d1.IDeviceContext6,
        device11on12: *d3d11on12.IDevice2,
        device11: *d3d11.IDevice,
        context11: *d3d11.IDeviceContext,
        swapbuffers11: [num_swapbuffers]*d3d11.IResource,
        targets: [num_swapbuffers]*d2d1.IBitmap1,
    },
    dwrite_factory: *dwrite.IFactory,
    wic_factory: *wic.IImagingFactory,
    present_flags: w.UINT,
    present_interval: w.UINT,

    pub fn init(window: w.HWND) GraphicsContext {
        const wic_factory = blk: {
            var wic_factory: *wic.IImagingFactory = undefined;
            hrPanicOnFail(w.CoCreateInstance(
                &wic.CLSID_ImagingFactory,
                null,
                w.CLSCTX_INPROC_SERVER,
                &wic.IID_IImagingFactory,
                @ptrCast(*?*c_void, &wic_factory),
            ));
            break :blk wic_factory;
        };

        const factory = blk: {
            var factory: *dxgi.IFactory6 = undefined;
            hrPanicOnFail(dxgi.CreateDXGIFactory2(
                if (enable_dx_debug) dxgi.CREATE_FACTORY_DEBUG else 0,
                &dxgi.IID_IFactory6,
                @ptrCast(*?*c_void, &factory),
            ));
            break :blk factory;
        };
        defer _ = factory.Release();

        var present_flags: w.UINT = 0;
        var present_interval: w.UINT = 0;
        {
            var allow_tearing: w.BOOL = w.FALSE;
            var hr = factory.CheckFeatureSupport(
                .PRESENT_ALLOW_TEARING,
                &allow_tearing,
                @sizeOf(@TypeOf(allow_tearing)),
            );

            if (hr == w.S_OK and allow_tearing == w.TRUE) {
                present_flags |= dxgi.PRESENT_ALLOW_TEARING;
            }
        }

        if (enable_dx_debug) {
            var maybe_debug: ?*d3d12d.IDebug1 = null;
            _ = d3d12.D3D12GetDebugInterface(&d3d12d.IID_IDebug1, @ptrCast(*?*c_void, &maybe_debug));
            if (maybe_debug) |debug| {
                debug.EnableDebugLayer();
                if (enable_dx_gpu_debug) {
                    debug.SetEnableGPUBasedValidation(w.TRUE);
                }
                _ = debug.Release();
            }
        }

        const suitable_adapter = blk: {
            var adapter: ?*dxgi.IAdapter1 = null;

            var adapter_index: u32 = 0;
            var optional_adapter1: ?*dxgi.IAdapter1 = undefined;

            while (factory.EnumAdapterByGpuPreference(adapter_index, dxgi.DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, &dxgi.IID_IAdapter1, &optional_adapter1) == w.S_OK) {
                if (optional_adapter1) |adapter1| {
                    var adapter1_desc: dxgi.ADAPTER_DESC1 = undefined;
                    if (adapter1.GetDesc1(&adapter1_desc) == w.S_OK) {
                        if ((adapter1_desc.Flags & dxgi.DXGI_ADAPTER_FLAG_SOFTWARE) != 0) {
                            // Don't select the Basic Render Driver adapter.
                            continue;
                        }

                        var hr = d3d12.D3D12CreateDevice(@ptrCast(*w.IUnknown, adapter1), .FL_11_1, &d3d12.IID_IDevice9, null);
                        // NOTE(gmodarelli): D3D12CreateDevice returns S_FALSE when the output device is null.
                        // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-d3d12createdevice#return-value
                        if (hr == w.S_OK or hr == w.S_FALSE) {
                            adapter = adapter1;
                            break;
                        }
                    }
                }

                adapter_index += 1;
            }

            break :blk adapter;
        };

        const device = blk: {
            var device: *d3d12.IDevice9 = undefined;
            const hr = innerblk: {
                if (suitable_adapter) |adapter| {
                    break :innerblk d3d12.D3D12CreateDevice(
                        @ptrCast(*w.IUnknown, adapter),
                        .FL_11_1,
                        &d3d12.IID_IDevice9,
                        @ptrCast(*?*c_void, &device),
                    );
                } else {
                    break :innerblk d3d12.D3D12CreateDevice(
                        null,
                        .FL_11_1,
                        &d3d12.IID_IDevice9,
                        @ptrCast(*?*c_void, &device),
                    );
                }
            };

            if (hr != w.S_OK) {
                _ = w.user32.messageBoxA(
                    window,
                    "Failed to create Direct3D 12 Device. This applications requires graphics card with DirectX 12 support.",
                    "Your graphics card driver may be old",
                    w.user32.MB_OK | w.user32.MB_ICONERROR,
                ) catch 0;
                w.kernel32.ExitProcess(0);
            }
            break :blk device;
        };

        // Check for Shader Model 6.3 support.
        {
            var data: d3d12.FEATURE_DATA_SHADER_MODEL = .{ .HighestShaderModel = .SM_6_7 };
            const hr = device.CheckFeatureSupport(.SHADER_MODEL, &data, @sizeOf(d3d12.FEATURE_DATA_SHADER_MODEL));
            if (hr != w.S_OK or @enumToInt(data.HighestShaderModel) < @enumToInt(d3d12.SHADER_MODEL.SM_6_3)) {
                _ = w.user32.messageBoxA(
                    window,
                    "This applications requires graphics card driver that supports Shader Model 6.3. Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    w.user32.MB_OK | w.user32.MB_ICONERROR,
                ) catch 0;
                w.kernel32.ExitProcess(0);
            }
        }

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
                    .Flags = if ((present_flags & dxgi.PRESENT_ALLOW_TEARING) != 0)
                        dxgi.SWAP_CHAIN_FLAG_ALLOW_TEARING
                    else
                        0,
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


        const dx11 = blk: {
            var device11: *d3d11.IDevice = undefined;
            var device_context11: *d3d11.IDeviceContext = undefined;
            hrPanicOnFail(d3d11on12.D3D11On12CreateDevice(
                @ptrCast(*w.IUnknown, device),
                if (enable_dx_debug)
                    d3d11.CREATE_DEVICE_DEBUG | d3d11.CREATE_DEVICE_BGRA_SUPPORT
                else
                    d3d11.CREATE_DEVICE_BGRA_SUPPORT,
                null,
                0,
                &[_]*w.IUnknown{@ptrCast(*w.IUnknown, cmdqueue)},
                1,
                0,
                @ptrCast(*?*d3d11.IDevice, &device11),
                @ptrCast(*?*d3d11.IDeviceContext, &device_context11),
                null,
            ));
            break :blk .{ .device = device11, .device_context = device_context11 };
        };

        const device11on12 = blk: {
            var device11on12: *d3d11on12.IDevice2 = undefined;
            hrPanicOnFail(dx11.device.QueryInterface(
                &d3d11on12.IID_IDevice2,
                @ptrCast(*?*c_void, &device11on12),
            ));
            break :blk device11on12;
        };

        const d2d_factory = blk: {
            var d2d_factory: *d2d1.IFactory7 = undefined;
            hrPanicOnFail(d2d1.D2D1CreateFactory(
                .SINGLE_THREADED,
                &d2d1.IID_IFactory7,
                if (enable_dx_debug)
                    &d2d1.FACTORY_OPTIONS{ .debugLevel = .INFORMATION }
                else
                    &d2d1.FACTORY_OPTIONS{ .debugLevel = .NONE },
                @ptrCast(*?*c_void, &d2d_factory),
            ));
            break :blk d2d_factory;
        };

        const dxgi_device = blk: {
            var dxgi_device: *dxgi.IDevice = undefined;
            hrPanicOnFail(device11on12.QueryInterface(
                &dxgi.IID_IDevice,
                @ptrCast(*?*c_void, &dxgi_device),
            ));
            break :blk dxgi_device;
        };
        defer _ = dxgi_device.Release();

        const d2d_device = blk: {
            var d2d_device: *d2d1.IDevice6 = undefined;
            hrPanicOnFail(d2d_factory.CreateDevice6(
                dxgi_device,
                @ptrCast(*?*d2d1.IDevice6, &d2d_device),
            ));
            break :blk d2d_device;
        };

        const d2d_device_context = blk: {
            var d2d_device_context: *d2d1.IDeviceContext6 = undefined;
            hrPanicOnFail(d2d_device.CreateDeviceContext6(
                d2d1.DEVICE_CONTEXT_OPTIONS_NONE,
                @ptrCast(*?*d2d1.IDeviceContext6, &d2d_device_context),
            ));
            break :blk d2d_device_context;
        };

        const dwrite_factory = blk: {
            var dwrite_factory: *dwrite.IFactory = undefined;
            hrPanicOnFail(dwrite.DWriteCreateFactory(
                .SHARED,
                &dwrite.IID_IFactory,
                @ptrCast(*?*c_void, &dwrite_factory),
            ));
            break :blk dwrite_factory;
        };

        var resource_pool = ResourcePool.init();
        var pipeline_pool = PipelinePool.init();

        var rtv_heap = DescriptorHeap.init(device, num_rtv_descriptors, .RTV, d3d12.DESCRIPTOR_HEAP_FLAG_NONE);
        var dsv_heap = DescriptorHeap.init(device, num_dsv_descriptors, .DSV, d3d12.DESCRIPTOR_HEAP_FLAG_NONE);

        var cbv_srv_uav_cpu_heap = DescriptorHeap.init(
            device,
            num_cbv_srv_uav_cpu_descriptors,
            .CBV_SRV_UAV,
            d3d12.DESCRIPTOR_HEAP_FLAG_NONE,
        );

        var cbv_srv_uav_gpu_heaps: [max_num_buffered_frames]DescriptorHeap = undefined;
        for (cbv_srv_uav_gpu_heaps) |_, heap_index| {
            cbv_srv_uav_gpu_heaps[heap_index] = DescriptorHeap.init(
                device,
                num_cbv_srv_uav_gpu_descriptors,
                .CBV_SRV_UAV,
                d3d12.DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
            );
        }

        var upload_heaps: [max_num_buffered_frames]GpuMemoryHeap = undefined;
        for (upload_heaps) |_, heap_index| {
            upload_heaps[heap_index] = GpuMemoryHeap.init(device, upload_heap_capacity, .UPLOAD);
        }

        const swapchain_buffers = blk: {
            var swapchain_buffers: [num_swapbuffers]ResourceHandle = undefined;
            var swapbuffers: [num_swapbuffers]*d3d12.IResource = undefined;
            for (swapbuffers) |_, buffer_index| {
                hrPanicOnFail(swapchain.GetBuffer(
                    @intCast(u32, buffer_index),
                    &d3d12.IID_IResource,
                    @ptrCast(*?*c_void, &swapbuffers[buffer_index]),
                ));
                device.CreateRenderTargetView(
                    swapbuffers[buffer_index],
                    &d3d12.RENDER_TARGET_VIEW_DESC{
                        .Format = .R8G8B8A8_UNORM, // TODO(mziulek): .R8G8B8A8_UNORM_SRGB?
                        .ViewDimension = .TEXTURE2D,
                        .u = .{
                            .Texture2D = .{
                                .MipSlice = 0,
                                .PlaneSlice = 0,
                            },
                        },
                    },
                    rtv_heap.allocateDescriptors(1).cpu_handle,
                );
                swapchain_buffers[buffer_index] = resource_pool.addResource(
                    swapbuffers[buffer_index],
                    d3d12.RESOURCE_STATE_PRESENT,
                );
            }
            break :blk swapchain_buffers;
        };

        const swapbuffers11 = blk: {
            var swapbuffers11: [num_swapbuffers]*d3d11.IResource = undefined;
            for (swapbuffers11) |_, buffer_index| {
                hrPanicOnFail(device11on12.CreateWrappedResource(
                    @ptrCast(*w.IUnknown, resource_pool.getResource(swapchain_buffers[buffer_index]).raw.?),
                    &d3d11on12.RESOURCE_FLAGS{
                        .BindFlags = d3d11.BIND_RENDER_TARGET,
                        .MiscFlags = 0,
                        .CPUAccessFlags = 0,
                        .StructureByteStride = 0,
                    },
                    d3d12.RESOURCE_STATE_RENDER_TARGET,
                    d3d12.RESOURCE_STATE_PRESENT,
                    &d3d11.IID_IResource,
                    @ptrCast(*?*c_void, &swapbuffers11[buffer_index]),
                ));
            }
            break :blk swapbuffers11;
        };

        const d2d_targets = blk: {
            var d2d_targets: [num_swapbuffers]*d2d1.IBitmap1 = undefined;
            for (d2d_targets) |_, target_index| {
                const swapbuffer11 = swapbuffers11[target_index];

                var surface: *dxgi.ISurface = undefined;
                hrPanicOnFail(swapbuffer11.QueryInterface(
                    &dxgi.IID_ISurface,
                    @ptrCast(*?*c_void, &surface),
                ));
                defer _ = surface.Release();

                hrPanicOnFail(d2d_device_context.CreateBitmapFromDxgiSurface(
                    surface,
                    &d2d1.BITMAP_PROPERTIES1{
                        .pixelFormat = .{ .format = .R8G8B8A8_UNORM, .alphaMode = .PREMULTIPLIED },
                        .dpiX = 96.0,
                        .dpiY = 96.0,
                        .bitmapOptions = d2d1.BITMAP_OPTIONS_TARGET | d2d1.BITMAP_OPTIONS_CANNOT_DRAW,
                        .colorContext = null,
                    },
                    @ptrCast(*?*d2d1.IBitmap1, &d2d_targets[target_index]),
                ));
            }
            break :blk d2d_targets;
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

        const frame_fence_event = w.CreateEventEx(
            null,
            "frame_fence_event",
            0,
            w.EVENT_ALL_ACCESS,
        ) catch unreachable;

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
                .map = blk: {
                    var hm: std.AutoHashMapUnmanaged(u32, PipelineHandle) = .{};
                    hm.ensureTotalCapacity(std.heap.page_allocator, PipelinePool.max_num_pipelines) catch unreachable;
                    break :blk hm;
                },
                .current = .{ .index = 0, .generation = 0 },
            },
            .transition_resource_barriers = std.heap.page_allocator.alloc(
                TransitionResourceBarrier,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
            .resources_to_release = std.ArrayList(ResourceWithCounter).initCapacity(std.heap.page_allocator, 64) catch unreachable,
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
                .device11 = dx11.device,
                .context11 = dx11.device_context,
                .swapbuffers11 = swapbuffers11,
                .targets = d2d_targets,
            },
            .dwrite_factory = dwrite_factory,
            .wic_factory = wic_factory,
            .present_flags = present_flags,
            .present_interval = present_interval,
        };
    }

    pub fn deinit(gr: *GraphicsContext) void {
        gr.finishGpuCommands();
        std.heap.page_allocator.free(gr.transition_resource_barriers);
        gr.resources_to_release.deinit();
        w.CloseHandle(gr.frame_fence_event);
        assert(gr.pipeline.map.count() == 0);
        gr.pipeline.map.deinit(std.heap.page_allocator);
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

    pub fn beginFrame(gr: *GraphicsContext) void {
        assert(!gr.is_cmdlist_opened);
        const cmdalloc = gr.cmdallocs[gr.frame_index];
        hrPanicOnFail(cmdalloc.Reset());
        hrPanicOnFail(gr.cmdlist.Reset(cmdalloc, null));
        gr.is_cmdlist_opened = true;
        gr.cmdlist.SetDescriptorHeaps(
            1,
            &[_]*d3d12.IDescriptorHeap{gr.cbv_srv_uav_gpu_heaps[gr.frame_index].heap},
        );
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
        gr.pipeline.current = .{ .index = 0, .generation = 0 };
    }

    pub fn endFrame(gr: *GraphicsContext) void {
        gr.flushGpuCommands();

        gr.frame_fence_counter += 1;
        hrPanicOnFail(gr.swapchain.Present(gr.present_interval, gr.present_flags));
        // TODO(mziulek):
        // Handle DXGI_ERROR_DEVICE_REMOVED and DXGI_ERROR_DEVICE_RESET codes here - we need to re-create
        // all resources in that case.
        // Take a look at:
        // https://github.com/microsoft/DirectML/blob/master/Samples/DirectMLSuperResolution/DeviceResources.cpp

        tracy.frameMark();
        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));

        const gpu_frame_counter = gr.frame_fence.GetCompletedValue();
        if ((gr.frame_fence_counter - gpu_frame_counter) >= max_num_buffered_frames) {
            hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gr.frame_fence_event));
            w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
        }

        gr.frame_index = (gr.frame_index + 1) % max_num_buffered_frames;
        gr.back_buffer_index = gr.swapchain.GetCurrentBackBufferIndex();

        gr.cbv_srv_uav_gpu_heaps[gr.frame_index].size = 0;
        gr.upload_memory_heaps[gr.frame_index].size = 0;

        for (gr.resources_to_release.items) |*res, i| {
            assert(res.counter > 0);
            res.counter -= 1;
            if (res.counter == 0) {
                _ = gr.releaseResource(res.resource);
                _ = gr.resources_to_release.swapRemove(i);
            }
        }
    }

    pub fn beginDraw2d(gr: *GraphicsContext) void {
        gr.flushGpuCommands();

        gr.d2d.device11on12.AcquireWrappedResources(
            &[_]*d3d11.IResource{gr.d2d.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.context.SetTarget(@ptrCast(*d2d1.IImage, gr.d2d.targets[gr.back_buffer_index]));
        gr.d2d.context.BeginDraw();
    }

    pub fn endDraw2d(gr: *GraphicsContext) void {
        var info_queue: *d3d12d.IInfoQueue = undefined;
        const mute_d2d_completely = true;
        if (enable_dx_debug) {
            // NOTE(mziulek): D2D1 is slow. It creates and destroys resources every frame. To see create/destroy
            // messages in debug output set 'mute_d2d_completely' to 'false'.
            hrPanicOnFail(gr.device.QueryInterface(&d3d12d.IID_IInfoQueue, @ptrCast(*?*c_void, &info_queue)));

            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(w.TRUE);
            } else {
                var filter: d3d12.INFO_QUEUE_FILTER = std.mem.zeroes(d3d12.INFO_QUEUE_FILTER);
                hrPanicOnFail(info_queue.PushStorageFilter(&filter));

                var hide = [_]d3d12.MESSAGE_ID{
                    .CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
                    .COMMAND_LIST_DRAW_VERTEX_BUFFER_STRIDE_TOO_SMALL,
                    .CREATEGRAPHICSPIPELINESTATE_DEPTHSTENCILVIEW_NOT_SET,
                };
                hrPanicOnFail(info_queue.AddStorageFilterEntries(&d3d12.INFO_QUEUE_FILTER{
                    .AllowList = .{
                        .NumCategories = 0,
                        .pCategoryList = null,
                        .NumSeverities = 0,
                        .pSeverityList = null,
                        .NumIDs = 0,
                        .pIDList = null,
                    },
                    .DenyList = .{
                        .NumCategories = 0,
                        .pCategoryList = null,
                        .NumSeverities = 0,
                        .pSeverityList = null,
                        .NumIDs = hide.len,
                        .pIDList = &hide,
                    },
                }));
            }
        }
        hrPanicOnFail(gr.d2d.context.EndDraw(null, null));

        gr.d2d.device11on12.ReleaseWrappedResources(
            &[_]*d3d11.IResource{gr.d2d.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.context11.Flush();

        if (enable_dx_debug) {
            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(w.FALSE);
            } else {
                info_queue.PopStorageFilter();
            }
            _ = info_queue.Release();
        }

        // Above calls will set back buffer state to PRESENT. We need to reflect this change
        // in 'resource_pool' by manually setting state.
        gr.resource_pool.editResource(gr.swapchain_buffers[gr.back_buffer_index]).*.state =
            d3d12.RESOURCE_STATE_PRESENT;
    }

    pub fn flushGpuCommands(gr: *GraphicsContext) void {
        if (gr.is_cmdlist_opened) {
            gr.flushResourceBarriers();
            hrPanicOnFail(gr.cmdlist.Close());
            gr.is_cmdlist_opened = false;
            gr.cmdqueue.ExecuteCommandLists(
                1,
                &[_]*d3d12.ICommandList{@ptrCast(*d3d12.ICommandList, gr.cmdlist)},
            );
        }
    }

    pub fn finishGpuCommands(gr: *GraphicsContext) void {
        gr.flushGpuCommands();

        gr.frame_fence_counter += 1;

        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));
        hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gr.frame_fence_counter, gr.frame_fence_event));
        w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;

        gr.cbv_srv_uav_gpu_heaps[gr.frame_index].size = 0;
        gr.upload_memory_heaps[gr.frame_index].size = 0;

        if (gr.resources_to_release.items.len > 0) {
            for (gr.resources_to_release.items) |res| {
                _ = gr.releaseResource(res.resource);
            }
            gr.resources_to_release.resize(0) catch unreachable;
        }
    }

    pub fn getBackBuffer(gr: GraphicsContext) struct {
        resource_handle: ResourceHandle,
        descriptor_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    } {
        return .{
            .resource_handle = gr.swapchain_buffers[gr.back_buffer_index],
            .descriptor_handle = .{
                .ptr = gr.rtv_heap.base.cpu_handle.ptr + gr.back_buffer_index * gr.rtv_heap.descriptor_size,
            },
        };
    }

    pub inline fn getResource(gr: GraphicsContext, handle: ResourceHandle) *d3d12.IResource {
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

    pub fn getResourceDesc(gr: GraphicsContext, handle: ResourceHandle) d3d12.RESOURCE_DESC {
        const resource = gr.resource_pool.getResource(handle);
        return resource.desc;
    }

    pub fn createCommittedResource(
        gr: *GraphicsContext,
        heap_type: d3d12.HEAP_TYPE,
        heap_flags: d3d12.HEAP_FLAGS,
        desc: *const d3d12.RESOURCE_DESC,
        initial_state: d3d12.RESOURCE_STATES,
        clear_value: ?*const d3d12.CLEAR_VALUE,
    ) HResultError!ResourceHandle {
        const resource = blk: {
            var resource: *d3d12.IResource = undefined;
            try hrErrorOnFail(gr.device.CreateCommittedResource(
                &d3d12.HEAP_PROPERTIES.initType(heap_type),
                heap_flags,
                desc,
                initial_state,
                clear_value,
                &d3d12.IID_IResource,
                @ptrCast(*?*c_void, &resource),
            ));
            break :blk resource;
        };
        return gr.resource_pool.addResource(resource, initial_state);
    }

    pub fn releaseResource(gr: *GraphicsContext, handle: ResourceHandle) u32 {
        if (gr.resource_pool.isResourceValid(handle)) {
            var resource = gr.resource_pool.editResource(handle);
            const refcount = resource.raw.?.Release();
            if (refcount == 0) {
                resource.* = .{
                    .raw = null,
                    .state = d3d12.RESOURCE_STATE_COMMON,
                    .desc = d3d12.RESOURCE_DESC.initBuffer(0),
                };
            }
            return refcount;
        }
        return 0;
    }

    pub fn releaseResourceDeferred(gr: *GraphicsContext, handle: ResourceHandle) void {
        // TODO(mziulek): Does this make sense? Is there non-growing container?
        assert(gr.resources_to_release.items.len < gr.resources_to_release.capacity);

        gr.resources_to_release.appendAssumeCapacity(.{ .resource = handle, .counter = max_num_buffered_frames + 2 });
    }

    pub fn flushResourceBarriers(gr: *GraphicsContext) void {
        if (gr.num_transition_resource_barriers > 0) {
            var d3d12_barriers: [max_num_buffered_resource_barriers]d3d12.RESOURCE_BARRIER = undefined;

            var num_valid_barriers: u32 = 0;
            var barrier_index: u32 = 0;
            while (barrier_index < gr.num_transition_resource_barriers) : (barrier_index += 1) {
                const barrier = &gr.transition_resource_barriers[barrier_index];

                if (gr.resource_pool.isResourceValid(barrier.resource)) {
                    d3d12_barriers[num_valid_barriers] = .{
                        .Type = .TRANSITION,
                        .Flags = d3d12.RESOURCE_BARRIER_FLAG_NONE,
                        .u = .{
                            .Transition = .{
                                .pResource = gr.getResource(barrier.resource),
                                .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
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
        state_after: d3d12.RESOURCE_STATES,
    ) void {
        var resource = gr.resource_pool.editResource(handle);

        if (state_after != resource.state) {
            if (gr.num_transition_resource_barriers >= gr.transition_resource_barriers.len) {
                gr.flushResourceBarriers();
            }
            gr.transition_resource_barriers[gr.num_transition_resource_barriers] = .{
                .resource = handle,
                .state_before = resource.state,
                .state_after = state_after,
            };
            gr.num_transition_resource_barriers += 1;
            resource.*.state = state_after;
        }
    }

    pub fn createGraphicsShaderPipeline(
        gr: *GraphicsContext,
        arena: *std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        return createGraphicsShaderPipelineAllStages(gr, arena, pso_desc, vs_cso_path, null, ps_cso_path);
    }

    pub fn createGraphicsShaderPipelineAllStages(
        gr: *GraphicsContext,
        arena: *std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        gs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        const tracy_zone = tracy.zone(@src(), 1);
        defer tracy_zone.end();

        const vs_code = blk: {
            if (vs_cso_path) |path| {
                assert(pso_desc.VS.pShaderBytecode == null);
                const vs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
                defer vs_file.close();
                const vs_code = vs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
                pso_desc.VS = .{ .pShaderBytecode = vs_code.ptr, .BytecodeLength = vs_code.len };
                break :blk vs_code;
            } else {
                assert(pso_desc.VS.pShaderBytecode != null);
                break :blk null;
            }
        };
        const gs_code = blk: {
            if (gs_cso_path) |path| {
                assert(pso_desc.GS.pShaderBytecode == null);
                const gs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
                defer gs_file.close();
                const gs_code = gs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
                pso_desc.GS = .{ .pShaderBytecode = gs_code.ptr, .BytecodeLength = gs_code.len };
                break :blk gs_code;
            } else {
                break :blk null;
            }
        };
        const ps_code = blk: {
            if (ps_cso_path) |path| {
                assert(pso_desc.PS.pShaderBytecode == null);
                const ps_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
                defer ps_file.close();
                const ps_code = ps_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
                pso_desc.PS = .{ .pShaderBytecode = ps_code.ptr, .BytecodeLength = ps_code.len };
                break :blk ps_code;
            } else {
                assert(pso_desc.PS.pShaderBytecode != null);
                break :blk null;
            }
        };
        defer {
            if (vs_code != null) {
                pso_desc.VS = .{ .pShaderBytecode = null, .BytecodeLength = 0 };
            }
            if (gs_code != null) {
                pso_desc.GS = .{ .pShaderBytecode = null, .BytecodeLength = 0 };
            }
            if (ps_code != null) {
                pso_desc.PS = .{ .pShaderBytecode = null, .BytecodeLength = 0 };
            }
        }

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @ptrCast([*]const u8, pso_desc.VS.pShaderBytecode.?)[0..pso_desc.VS.BytecodeLength],
            );
            if (gs_code != null) {
                hasher.update(
                    @ptrCast([*]const u8, pso_desc.GS.pShaderBytecode.?)[0..pso_desc.GS.BytecodeLength],
                );
            }
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
        std.log.info("[graphics] Graphics pipeline hash: {d}", .{hash});

        if (gr.pipeline.map.contains(hash)) {
            std.log.info("[graphics] Graphics pipeline hit detected.", .{});
            const handle = gr.pipeline.map.getEntry(hash).?.value_ptr.*;
            _ = incrementPipelineRefcount(gr.*, handle);
            return handle;
        }

        const rs = blk: {
            var rs: *d3d12.IRootSignature = undefined;
            hrPanicOnFail(gr.device.CreateRootSignature(
                0,
                pso_desc.VS.pShaderBytecode.?,
                pso_desc.VS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @ptrCast(*?*c_void, &rs),
            ));
            break :blk rs;
        };

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gr.device.CreateGraphicsPipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*c_void, &pso),
            ));
            break :blk pso;
        };

        const handle = gr.pipeline.pool.addPipeline(pso, rs, .Graphics);
        gr.pipeline.map.putAssumeCapacity(hash, handle);
        return handle;
    }

    pub fn createComputeShaderPipeline(
        gr: *GraphicsContext,
        arena: *std.mem.Allocator,
        pso_desc: *d3d12.COMPUTE_PIPELINE_STATE_DESC,
        cs_cso_path: ?[]const u8,
    ) PipelineHandle {
        const tracy_zone = tracy.zone(@src(), 1);
        defer tracy_zone.end();

        const cs_code = blk: {
            if (cs_cso_path) |path| {
                assert(pso_desc.CS.pShaderBytecode == null);
                const cs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
                defer cs_file.close();
                const cs_code = cs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
                pso_desc.CS = .{ .pShaderBytecode = cs_code.ptr, .BytecodeLength = cs_code.len };
                break :blk cs_code;
            } else {
                assert(pso_desc.CS.pShaderBytecode != null);
                break :blk null;
            }
        };
        defer {
            if (cs_code != null) {
                pso_desc.CS = .{ .pShaderBytecode = null, .BytecodeLength = 0 };
            }
        }

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @ptrCast([*]const u8, pso_desc.CS.pShaderBytecode.?)[0..pso_desc.CS.BytecodeLength],
            );
            break :compute_hash hasher.final();
        };
        std.log.info("[graphics] Compute pipeline hash: {d}", .{hash});

        if (gr.pipeline.map.contains(hash)) {
            std.log.info("[graphics] Compute pipeline hit detected.", .{});
            const handle = gr.pipeline.map.getEntry(hash).?.value_ptr.*;
            _ = incrementPipelineRefcount(gr.*, handle);
            return handle;
        }

        const rs = blk: {
            var rs: *d3d12.IRootSignature = undefined;
            hrPanicOnFail(gr.device.CreateRootSignature(
                0,
                pso_desc.CS.pShaderBytecode.?,
                pso_desc.CS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @ptrCast(*?*c_void, &rs),
            ));
            break :blk rs;
        };

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gr.device.CreateComputePipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*c_void, &pso),
            ));
            break :blk pso;
        };

        const handle = gr.pipeline.pool.addPipeline(pso, rs, .Compute);
        gr.pipeline.map.putAssumeCapacity(hash, handle);
        return handle;
    }

    pub fn setCurrentPipeline(gr: *GraphicsContext, pipeline_handle: PipelineHandle) void {
        assert(gr.is_cmdlist_opened);
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
        comptime T: type,
        num_elements: u32,
    ) struct { cpu_slice: []T, gpu_base: d3d12.GPU_VIRTUAL_ADDRESS } {
        assert(num_elements > 0);
        const size = num_elements * @sizeOf(T);
        var memory = gr.upload_memory_heaps[gr.frame_index].allocate(size);
        if (memory.cpu_slice == null or memory.gpu_base == null) {
            std.log.info("[graphics] Upload memory exhausted - waiting for a GPU... (cmdlist state is lost).", .{});

            gr.finishGpuCommands();
            gr.beginFrame();

            memory = gr.upload_memory_heaps[gr.frame_index].allocate(size);
        }
        return .{
            .cpu_slice = std.mem.bytesAsSlice(T, @alignCast(@alignOf(T), memory.cpu_slice.?)),
            .gpu_base = memory.gpu_base.?,
        };
    }

    pub fn allocateUploadBufferRegion(
        gr: *GraphicsContext,
        comptime T: type,
        num_elements: u32,
    ) struct { cpu_slice: []T, buffer: *d3d12.IResource, buffer_offset: u64 } {
        assert(num_elements > 0);
        const size = num_elements * @sizeOf(T);
        const memory = gr.allocateUploadMemory(T, num_elements);
        const aligned_size = (size + (GpuMemoryHeap.alloc_alignment - 1)) & ~(GpuMemoryHeap.alloc_alignment - 1);
        return .{
            .cpu_slice = memory.cpu_slice,
            .buffer = gr.upload_memory_heaps[gr.frame_index].heap,
            .buffer_offset = gr.upload_memory_heaps[gr.frame_index].size - aligned_size,
        };
    }

    pub fn allocateCpuDescriptors(
        gr: *GraphicsContext,
        dtype: d3d12.DESCRIPTOR_HEAP_TYPE,
        num: u32,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
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

    pub fn allocateTempCpuDescriptors(
        gr: *GraphicsContext,
        dtype: d3d12.DESCRIPTOR_HEAP_TYPE,
        num: u32,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        assert(num > 0);
        var dheap = switch (dtype) {
            .CBV_SRV_UAV => &gr.cbv_srv_uav_cpu_heap,
            .RTV => &gr.rtv_heap,
            .DSV => &gr.dsv_heap,
            .SAMPLER => unreachable,
        };
        const handle = dheap.allocateDescriptors(num).cpu_handle;
        dheap.size_temp += num;
        return handle;
    }

    pub fn deallocateAllTempCpuDescriptors(gr: *GraphicsContext, dtype: d3d12.DESCRIPTOR_HEAP_TYPE) void {
        var dheap = switch (dtype) {
            .CBV_SRV_UAV => &gr.cbv_srv_uav_cpu_heap,
            .RTV => &gr.rtv_heap,
            .DSV => &gr.dsv_heap,
            .SAMPLER => unreachable,
        };
        assert(dheap.size_temp > 0);
        assert(dheap.size_temp <= dheap.size);
        dheap.size -= dheap.size_temp;
        dheap.size_temp = 0;
    }

    pub inline fn allocateGpuDescriptors(gr: *GraphicsContext, num_descriptors: u32) Descriptor {
        return gr.cbv_srv_uav_gpu_heaps[gr.frame_index].allocateDescriptors(num_descriptors);
    }

    pub fn copyDescriptorsToGpuHeap(
        gr: *GraphicsContext,
        num: u32,
        src_base_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    ) d3d12.GPU_DESCRIPTOR_HANDLE {
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

        var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
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

        gr.addTransitionBarrier(texture, d3d12.RESOURCE_STATE_COPY_DEST);
        gr.flushResourceBarriers();

        gr.cmdlist.CopyTextureRegion(&d3d12.TEXTURE_COPY_LOCATION{
            .pResource = gr.getResource(texture),
            .Type = .SUBRESOURCE_INDEX,
            .u = .{
                .SubresourceIndex = subresource,
            },
        }, 0, 0, 0, &d3d12.TEXTURE_COPY_LOCATION{
            .pResource = upload.buffer,
            .Type = .PLACED_FOOTPRINT,
            .u = .{
                .PlacedFootprint = layout[0],
            },
        }, null);
    }

    pub fn createAndUploadTex2dFromFile(
        gr: *GraphicsContext,
        path: []const u8,
        params: struct {
            num_mip_levels: u32 = 0,
            texture_flags: d3d12.RESOURCE_FLAGS = d3d12.RESOURCE_FLAG_NONE,
            //force_num_components = 0,
        },
    ) HResultError!ResourceHandle {
        assert(gr.is_cmdlist_opened);

        // TODO(mziulek): Hardcoded array size. Make it more robust.
        var path_u16: [300]u16 = undefined;
        assert(path.len < path_u16.len - 1);
        const path_len = std.unicode.utf8ToUtf16Le(path_u16[0..], path) catch unreachable;
        path_u16[path_len] = 0;

        const bmp_decoder = blk: {
            var maybe_bmp_decoder: ?*wic.IBitmapDecoder = undefined;
            hrPanicOnFail(gr.wic_factory.CreateDecoderFromFilename(
                @ptrCast(w.LPCWSTR, &path_u16),
                null,
                w.GENERIC_READ,
                .MetadataCacheOnDemand,
                &maybe_bmp_decoder,
            ));
            break :blk maybe_bmp_decoder.?;
        };
        defer _ = bmp_decoder.Release();

        const bmp_frame = blk: {
            var maybe_bmp_frame: ?*wic.IBitmapFrameDecode = null;
            hrPanicOnFail(bmp_decoder.GetFrame(0, &maybe_bmp_frame));
            break :blk maybe_bmp_frame.?;
        };
        defer _ = bmp_frame.Release();

        const pixel_format = blk: {
            var pixel_format: w.GUID = undefined;
            hrPanicOnFail(bmp_frame.GetPixelFormat(&pixel_format));
            break :blk pixel_format;
        };

        const eql = std.mem.eql;
        const asBytes = std.mem.asBytes;
        const num_components: u32 = blk: {
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat24bppRGB))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppRGB))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppRGBA))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppPRGBA))) break :blk 4;

            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat24bppBGR))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppBGR))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppBGRA))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppPBGRA))) break :blk 4;

            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat8bppGray))) break :blk 1;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat8bppAlpha))) break :blk 1;

            unreachable;
        };

        const wic_format = if (num_components == 1)
            &wic.GUID_PixelFormat8bppGray
        else
            &wic.GUID_PixelFormat32bppRGBA;

        const dxgi_format = if (num_components == 1) dxgi.FORMAT.R8_UNORM else dxgi.FORMAT.R8G8B8A8_UNORM;

        const image_conv = blk: {
            var maybe_image_conv: ?*wic.IFormatConverter = null;
            hrPanicOnFail(gr.wic_factory.CreateFormatConverter(&maybe_image_conv));
            break :blk maybe_image_conv.?;
        };
        defer _ = image_conv.Release();

        hrPanicOnFail(image_conv.Initialize(
            @ptrCast(*wic.IBitmapSource, bmp_frame),
            wic_format,
            .None,
            null,
            0.0,
            .Custom,
        ));
        const image_wh = blk: {
            var width: u32 = undefined;
            var height: u32 = undefined;
            hrPanicOnFail(image_conv.GetSize(&width, &height));
            break :blk .{ .w = width, .h = height };
        };
        const texture = try gr.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(dxgi_format, image_wh.w, image_wh.h, params.num_mip_levels);
                desc.Flags = params.texture_flags;
                break :blk desc;
            },
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        );

        const desc = gr.getResource(texture).GetDesc();

        var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
        var required_size: u64 = undefined;
        gr.device.GetCopyableFootprints(&desc, 0, 1, 0, &layout, null, null, &required_size);

        const upload = gr.allocateUploadBufferRegion(u8, @intCast(u32, required_size));
        layout[0].Offset = upload.buffer_offset;

        hrPanicOnFail(image_conv.CopyPixels(
            null,
            layout[0].Footprint.RowPitch,
            layout[0].Footprint.RowPitch * layout[0].Footprint.Height,
            upload.cpu_slice.ptr,
        ));

        gr.cmdlist.CopyTextureRegion(&d3d12.TEXTURE_COPY_LOCATION{
            .pResource = gr.getResource(texture),
            .Type = .SUBRESOURCE_INDEX,
            .u = .{ .SubresourceIndex = 0 },
        }, 0, 0, 0, &d3d12.TEXTURE_COPY_LOCATION{
            .pResource = upload.buffer,
            .Type = .PLACED_FOOTPRINT,
            .u = .{ .PlacedFootprint = layout[0] },
        }, null);

        return texture;
    }
};

pub const GuiContext = struct {
    font: ResourceHandle,
    font_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    pipeline: PipelineHandle,
    vb: [GraphicsContext.max_num_buffered_frames]ResourceHandle,
    ib: [GraphicsContext.max_num_buffered_frames]ResourceHandle,
    vb_cpu_addr: [GraphicsContext.max_num_buffered_frames][]align(8) u8,
    ib_cpu_addr: [GraphicsContext.max_num_buffered_frames][]align(8) u8,

    pub fn init(arena: *std.mem.Allocator, gr: *GraphicsContext, num_msaa_samples: u32) GuiContext {
        assert(gr.is_cmdlist_opened);
        assert(c.igGetCurrentContext() != null);

        const io = c.igGetIO().?;

        _ = c.ImFontAtlas_AddFontFromFileTTF(io.*.Fonts, "content/Roboto-Medium.ttf", 25.0, null, null);
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

        const font = gr.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, font_info.width, font_info.height, 1),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);

        gr.updateTex2dSubresource(font, 0, font_info.pixels, font_info.width * 4);
        gr.addTransitionBarrier(font, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        const font_srv = gr.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gr.device.CreateShaderResourceView(gr.getResource(font), null, font_srv);

        const pipeline = blk: {
            const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
                d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                d3d12.INPUT_ELEMENT_DESC.init("_Uv", 0, .R32G32_FLOAT, 0, 8, .PER_VERTEX_DATA, 0),
                d3d12.INPUT_ELEMENT_DESC.init("_Color", 0, .R8G8B8A8_UNORM, 0, 16, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
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
            pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
            pso_desc.NumRenderTargets = 1;
            pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
            pso_desc.PrimitiveTopologyType = .TRIANGLE;
            pso_desc.SampleDesc = .{ .Count = num_msaa_samples, .Quality = 0 };
            break :blk gr.createGraphicsShaderPipeline(
                arena,
                &pso_desc,
                "content/shaders/imgui.vs.cso",
                "content/shaders/imgui.ps.cso",
            );
        };
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
        gr.finishGpuCommands();
        _ = gr.releasePipeline(gui.pipeline);
        _ = gr.releaseResource(gui.font);
        for (gui.vb) |vb| _ = gr.releaseResource(vb);
        for (gui.ib) |ib| _ = gr.releaseResource(ib);
        gui.* = undefined;
    }

    pub fn draw(gui: *GuiContext, gr: *GraphicsContext) void {
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
            const new_size = (num_vertices + 5_000) * @sizeOf(c.ImDrawVert);
            vb = gr.createCommittedResource(
                .UPLOAD,
                d3d12.HEAP_FLAG_NONE,
                &d3d12.RESOURCE_DESC.initBuffer(new_size),
                d3d12.RESOURCE_STATE_GENERIC_READ,
                null,
            ) catch |err| hrPanic(err);
            gui.vb[gr.frame_index] = vb;
            gui.vb_cpu_addr[gr.frame_index] = blk: {
                var ptr: ?[*]align(8) u8 = null;
                hrPanicOnFail(gr.getResource(vb).Map(
                    0,
                    &.{ .Begin = 0, .End = 0 },
                    @ptrCast(*?*c_void, &ptr),
                ));
                break :blk ptr.?[0..new_size];
            };
        }
        if (gr.getResourceSize(ib) < num_indices * @sizeOf(c.ImDrawIdx)) {
            _ = gr.releaseResource(ib);
            const new_size = (num_indices + 10_000) * @sizeOf(c.ImDrawIdx);
            ib = gr.createCommittedResource(
                .UPLOAD,
                d3d12.HEAP_FLAG_NONE,
                &d3d12.RESOURCE_DESC.initBuffer(new_size),
                d3d12.RESOURCE_STATE_GENERIC_READ,
                null,
            ) catch |err| hrPanic(err);
            gui.ib[gr.frame_index] = ib;
            gui.ib_cpu_addr[gr.frame_index] = blk: {
                var ptr: ?[*]align(8) u8 = null;
                hrPanicOnFail(gr.getResource(ib).Map(
                    0,
                    &.{ .Begin = 0, .End = 0 },
                    @ptrCast(*?*c_void, &ptr),
                ));
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

        const display_x = draw_data.?.*.DisplayPos.x;
        const display_y = draw_data.?.*.DisplayPos.y;
        const display_w = draw_data.?.*.DisplaySize.x;
        const display_h = draw_data.?.*.DisplaySize.y;
        gr.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = display_w,
            .Height = display_h,
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        gr.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        gr.setCurrentPipeline(gui.pipeline);
        {
            const mem = gr.allocateUploadMemory(vm.Mat4, 1);
            mem.cpu_slice[0] = vm.Mat4.initOrthoOffCenterLh(
                display_x,
                display_x + display_w,
                display_y + display_h,
                display_y,
                0.0,
                1.0,
            ).transpose();

            gr.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }
        gr.cmdlist.SetGraphicsRootDescriptorTable(1, gr.copyDescriptorsToGpuHeap(1, gui.font_srv));
        gr.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
            .BufferLocation = gr.getResource(vb).GetGPUVirtualAddress(),
            .SizeInBytes = num_vertices * @sizeOf(c.ImDrawVert),
            .StrideInBytes = @sizeOf(c.ImDrawVert),
        }});
        gr.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = gr.getResource(ib).GetGPUVirtualAddress(),
            .SizeInBytes = num_indices * @sizeOf(c.ImDrawIdx),
            .Format = if (@sizeOf(c.ImDrawIdx) == 2) .R16_UINT else .R32_UINT,
        });

        var global_vtx_offset: i32 = 0;
        var global_idx_offset: u32 = 0;

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
                    const rect = [1]d3d12.RECT{.{
                        .left = @floatToInt(i32, cmd.*.ClipRect.x - display_x),
                        .top = @floatToInt(i32, cmd.*.ClipRect.y - display_y),
                        .right = @floatToInt(i32, cmd.*.ClipRect.z - display_x),
                        .bottom = @floatToInt(i32, cmd.*.ClipRect.w - display_y),
                    }};
                    if (rect[0].right > rect[0].left and rect[0].bottom > rect[0].top) {
                        gr.cmdlist.RSSetScissorRects(1, &rect);
                        gr.cmdlist.DrawIndexedInstanced(
                            cmd.*.ElemCount,
                            1,
                            cmd.*.IdxOffset + global_idx_offset,
                            @intCast(i32, cmd.*.VtxOffset) + global_vtx_offset,
                            0,
                        );
                    }
                }
            }
            global_idx_offset += @intCast(u32, cmdlist.*.IdxBuffer.Size);
            global_vtx_offset += cmdlist.*.VtxBuffer.Size;
        }
    }
};

pub const MipmapGenerator = struct {
    const num_scratch_textures = 4;

    pipeline: PipelineHandle,
    scratch_textures: [num_scratch_textures]ResourceHandle,
    base_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
    format: dxgi.FORMAT,

    pub fn init(arena: *std.mem.Allocator, gr: *GraphicsContext, format: dxgi.FORMAT) MipmapGenerator {
        var width: u32 = 2048 / 2;
        var height: u32 = 2048 / 2;

        var scratch_textures: [num_scratch_textures]ResourceHandle = undefined;
        for (scratch_textures) |_, texture_index| {
            scratch_textures[texture_index] = gr.createCommittedResource(
                .DEFAULT,
                d3d12.HEAP_FLAG_NONE,
                &blk: {
                    var desc = d3d12.RESOURCE_DESC.initTex2d(format, width, height, 1);
                    desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                    break :blk desc;
                },
                d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
                null,
            ) catch |err| hrPanic(err);
            width /= 2;
            height /= 2;
        }

        const base_uav = gr.allocateCpuDescriptors(.CBV_SRV_UAV, num_scratch_textures);
        var cpu_handle = base_uav;
        for (scratch_textures) |_, texture_index| {
            gr.device.CreateUnorderedAccessView(
                gr.getResource(scratch_textures[texture_index]),
                null,
                null,
                cpu_handle,
            );
            cpu_handle.ptr += gr.cbv_srv_uav_cpu_heap.descriptor_size;
        }

        var desc = d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault();
        const pipeline = gr.createComputeShaderPipeline(arena, &desc, "content/shaders/generate_mipmaps.cs.cso");

        return MipmapGenerator{
            .pipeline = pipeline,
            .scratch_textures = scratch_textures,
            .base_uav = base_uav,
            .format = format,
        };
    }

    pub fn deinit(mipgen: *MipmapGenerator, gr: *GraphicsContext) void {
        for (mipgen.scratch_textures) |_, texture_index| {
            _ = gr.releaseResource(mipgen.scratch_textures[texture_index]);
        }
        _ = gr.releasePipeline(mipgen.pipeline);
        mipgen.* = undefined;
    }

    pub fn generateMipmaps(mipgen: *MipmapGenerator, gr: *GraphicsContext, texture: ResourceHandle) void {
        const texture_desc = gr.getResourceDesc(texture);
        assert(mipgen.format == texture_desc.Format);
        assert(texture_desc.Width <= 2048 and texture_desc.Height <= 2048);
        assert(texture_desc.Width == texture_desc.Height);
        assert(texture_desc.MipLevels > 1);

        var array_slice: u32 = 0;
        while (array_slice < texture_desc.DepthOrArraySize) : (array_slice += 1) {
            const texture_srv = gr.allocateTempCpuDescriptors(.CBV_SRV_UAV, 1);
            gr.device.CreateShaderResourceView(
                gr.getResource(texture),
                &d3d12.SHADER_RESOURCE_VIEW_DESC{
                    .Format = .UNKNOWN,
                    .ViewDimension = .TEXTURE2DARRAY,
                    .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
                    .u = .{
                        .Texture2DArray = .{
                            .MipLevels = texture_desc.MipLevels,
                            .FirstArraySlice = array_slice,
                            .ArraySize = 1,
                            .MostDetailedMip = 0,
                            .PlaneSlice = 0,
                            .ResourceMinLODClamp = 0.0,
                        },
                    },
                },
                texture_srv,
            );
            const table_base = gr.copyDescriptorsToGpuHeap(1, texture_srv);
            _ = gr.copyDescriptorsToGpuHeap(num_scratch_textures, mipgen.base_uav);
            gr.deallocateAllTempCpuDescriptors(.CBV_SRV_UAV);

            gr.setCurrentPipeline(mipgen.pipeline);
            var total_num_mips: u32 = texture_desc.MipLevels - 1;
            var current_src_mip_level: u32 = 0;

            while (true) {
                for (mipgen.scratch_textures) |scratch_texture| {
                    gr.addTransitionBarrier(scratch_texture, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
                }
                gr.addTransitionBarrier(texture, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
                gr.flushResourceBarriers();

                const dispatch_num_mips = if (total_num_mips >= 4) 4 else total_num_mips;
                gr.cmdlist.SetComputeRoot32BitConstant(0, current_src_mip_level, 0);
                gr.cmdlist.SetComputeRoot32BitConstant(0, dispatch_num_mips, 1);
                gr.cmdlist.SetComputeRootDescriptorTable(1, table_base);
                const num_groups_x = std.math.max(
                    @intCast(u32, texture_desc.Width) >> @intCast(u5, 3 + current_src_mip_level),
                    1,
                );
                const num_groups_y = std.math.max(
                    texture_desc.Height >> @intCast(u5, 3 + current_src_mip_level),
                    1,
                );
                gr.cmdlist.Dispatch(num_groups_x, num_groups_y, 1);

                for (mipgen.scratch_textures) |scratch_texture| {
                    gr.addTransitionBarrier(scratch_texture, d3d12.RESOURCE_STATE_COPY_SOURCE);
                }
                gr.addTransitionBarrier(texture, d3d12.RESOURCE_STATE_COPY_DEST);
                gr.flushResourceBarriers();

                var mip_index: u32 = 0;
                while (mip_index < dispatch_num_mips) : (mip_index += 1) {
                    const dst = d3d12.TEXTURE_COPY_LOCATION{
                        .pResource = gr.getResource(texture),
                        .Type = .SUBRESOURCE_INDEX,
                        .u = .{ .SubresourceIndex = mip_index + 1 + current_src_mip_level +
                            array_slice * texture_desc.MipLevels },
                    };
                    const src = d3d12.TEXTURE_COPY_LOCATION{
                        .pResource = gr.getResource(mipgen.scratch_textures[mip_index]),
                        .Type = .SUBRESOURCE_INDEX,
                        .u = .{ .SubresourceIndex = 0 },
                    };
                    const box = d3d12.BOX{
                        .left = 0,
                        .top = 0,
                        .front = 0,
                        .right = @intCast(u32, texture_desc.Width) >> @intCast(u5, mip_index + 1 + current_src_mip_level),
                        .bottom = texture_desc.Height >> @intCast(u5, mip_index + 1 + current_src_mip_level),
                        .back = 1,
                    };
                    gr.cmdlist.CopyTextureRegion(&dst, 0, 0, 0, &src, &box);
                }

                assert(total_num_mips >= dispatch_num_mips);
                total_num_mips -= dispatch_num_mips;
                if (total_num_mips == 0) {
                    break;
                }
                current_src_mip_level += dispatch_num_mips;
            }
        }
    }
};

pub const ResourceHandle = struct {
    index: u16 align(4),
    generation: u16,
};

const Resource = struct {
    raw: ?*d3d12.IResource,
    state: d3d12.RESOURCE_STATES,
    desc: d3d12.RESOURCE_DESC,
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
                    res.* = .{
                        .raw = null,
                        .state = d3d12.RESOURCE_STATE_COMMON,
                        .desc = d3d12.RESOURCE_DESC.initBuffer(0),
                    };
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
        raw: *d3d12.IResource,
        state: d3d12.RESOURCE_STATES,
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
    pso: ?*d3d12.IPipelineState,
    rs: ?*d3d12.IRootSignature,
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
        pso: *d3d12.IPipelineState,
        rs: *d3d12.IRootSignature,
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
    cpu_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    gpu_handle: d3d12.GPU_DESCRIPTOR_HANDLE,
};

const DescriptorHeap = struct {
    heap: *d3d12.IDescriptorHeap,
    base: Descriptor,
    size: u32,
    size_temp: u32,
    capacity: u32,
    descriptor_size: u32,

    fn init(
        device: *d3d12.IDevice9,
        capacity: u32,
        heap_type: d3d12.DESCRIPTOR_HEAP_TYPE,
        flags: d3d12.DESCRIPTOR_HEAP_FLAGS,
    ) DescriptorHeap {
        assert(capacity > 0);
        const heap = blk: {
            var heap: *d3d12.IDescriptorHeap = undefined;
            hrPanicOnFail(device.CreateDescriptorHeap(&.{
                .Type = heap_type,
                .NumDescriptors = capacity,
                .Flags = flags,
                .NodeMask = 0,
            }, &d3d12.IID_IDescriptorHeap, @ptrCast(*?*c_void, &heap)));
            break :blk heap;
        };
        return DescriptorHeap{
            .heap = heap,
            .base = .{
                .cpu_handle = heap.GetCPUDescriptorHandleForHeapStart(),
                .gpu_handle = blk: {
                    if ((flags & d3d12.DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) != 0)
                        break :blk heap.GetGPUDescriptorHandleForHeapStart();
                    break :blk d3d12.GPU_DESCRIPTOR_HANDLE{ .ptr = 0 };
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

        const cpu_handle = d3d12.CPU_DESCRIPTOR_HANDLE{
            .ptr = dheap.base.cpu_handle.ptr + dheap.size * dheap.descriptor_size,
        };
        const gpu_handle = d3d12.GPU_DESCRIPTOR_HANDLE{
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

    heap: *d3d12.IResource,
    cpu_slice: []u8,
    gpu_base: d3d12.GPU_VIRTUAL_ADDRESS,
    size: u32,
    capacity: u32,

    fn init(device: *d3d12.IDevice9, capacity: u32, heap_type: d3d12.HEAP_TYPE) GpuMemoryHeap {
        assert(capacity > 0);
        const resource = blk: {
            var resource: *d3d12.IResource = undefined;
            hrPanicOnFail(device.CreateCommittedResource(
                &d3d12.HEAP_PROPERTIES.initType(heap_type),
                d3d12.HEAP_FLAG_NONE,
                &d3d12.RESOURCE_DESC.initBuffer(capacity),
                d3d12.RESOURCE_STATE_GENERIC_READ,
                null,
                &d3d12.IID_IResource,
                @ptrCast(*?*c_void, &resource),
            ));
            break :blk resource;
        };
        const cpu_base = blk: {
            var cpu_base: [*]u8 = undefined;
            hrPanicOnFail(resource.Map(
                0,
                &d3d12.RANGE{ .Begin = 0, .End = 0 },
                @ptrCast(*?*c_void, &cpu_base),
            ));
            break :blk cpu_base;
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
    ) struct { cpu_slice: ?[]u8, gpu_base: ?d3d12.GPU_VIRTUAL_ADDRESS } {
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
