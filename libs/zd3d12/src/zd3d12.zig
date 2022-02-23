const std = @import("std");
const assert = std.debug.assert;
const zwin32 = @import("zwin32");
const w = zwin32.base;
const dwrite = zwin32.dwrite;
const dxgi = zwin32.dxgi;
const d3d11 = zwin32.d3d11;
const d3d12 = zwin32.d3d12;
const d3d12d = zwin32.d3d12d;
const d2d1 = zwin32.d2d1;
const d3d11on12 = zwin32.d3d11on12;
const wic = zwin32.wic;
const HResultError = zwin32.HResultError;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const hrErrorOnFail = zwin32.hrErrorOnFail;
const ztracy = @import("ztracy");

const enable_dx_debug = @import("build_options").enable_dx_debug;
const enable_dx_gpu_debug = @import("build_options").enable_dx_gpu_debug;
const enable_d2d = @import("build_options").enable_d2d;

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

const num_swapbuffers = 4;

const D2dState = struct {
    factory: *d2d1.IFactory7,
    device: *d2d1.IDevice6,
    context: *d2d1.IDeviceContext6,
    device11on12: *d3d11on12.IDevice2,
    device11: *d3d11.IDevice,
    context11: *d3d11.IDeviceContext,
    swapbuffers11: [num_swapbuffers]*d3d11.IResource,
    targets: [num_swapbuffers]*d2d1.IBitmap1,
    dwrite_factory: *dwrite.IFactory,
};

pub const GraphicsContext = struct {
    pub const max_num_buffered_frames = 2;
    const num_rtv_descriptors = 128;
    const num_dsv_descriptors = 128;
    const num_cbv_srv_uav_cpu_descriptors = 16 * 1024;
    const num_cbv_srv_uav_gpu_descriptors = 8 * 1024;
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
    cbv_srv_uav_gpu_heaps: [max_num_buffered_frames + 1]DescriptorHeap,
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
    d2d: ?D2dState,
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
                @ptrCast(*?*anyopaque, &wic_factory),
            ));
            break :blk wic_factory;
        };

        const factory = blk: {
            var factory: *dxgi.IFactory6 = undefined;
            hrPanicOnFail(dxgi.CreateDXGIFactory2(
                if (enable_dx_debug) dxgi.CREATE_FACTORY_DEBUG else 0,
                &dxgi.IID_IFactory6,
                @ptrCast(*?*anyopaque, &factory),
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
            _ = d3d12.D3D12GetDebugInterface(&d3d12d.IID_IDebug1, @ptrCast(*?*anyopaque, &maybe_debug));
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
            var optional_adapter1: ?*dxgi.IAdapter1 = null;

            while (factory.EnumAdapterByGpuPreference(
                adapter_index,
                dxgi.GPU_PREFERENCE_HIGH_PERFORMANCE,
                &dxgi.IID_IAdapter1,
                &optional_adapter1,
            ) == w.S_OK) {
                if (optional_adapter1) |adapter1| {
                    var adapter1_desc: dxgi.ADAPTER_DESC1 = undefined;
                    if (adapter1.GetDesc1(&adapter1_desc) == w.S_OK) {
                        if ((adapter1_desc.Flags & dxgi.ADAPTER_FLAG_SOFTWARE) != 0) {
                            // Don't select the Basic Render Driver adapter.
                            continue;
                        }

                        const hr = d3d12.D3D12CreateDevice(
                            @ptrCast(*w.IUnknown, adapter1),
                            .FL_11_1,
                            &d3d12.IID_IDevice9,
                            null,
                        );
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
        defer {
            if (suitable_adapter) |adapter| _ = adapter.Release();
        }

        const device = blk: {
            var device: *d3d12.IDevice9 = undefined;
            const hr = d3d12.D3D12CreateDevice(
                if (suitable_adapter) |adapter| @ptrCast(*w.IUnknown, adapter) else null,
                .FL_11_1,
                &d3d12.IID_IDevice9,
                @ptrCast(*?*anyopaque, &device),
            );

            if (hr != w.S_OK) {
                _ = w.user32.messageBoxA(
                    window,
                    "Failed to create Direct3D 12 Device. This applications requires graphics card with DirectX 12" ++
                        "support.",
                    "Your graphics card driver may be old",
                    w.user32.MB_OK | w.user32.MB_ICONERROR,
                ) catch 0;
                w.kernel32.ExitProcess(0);
            }
            break :blk device;
        };

        // Check for Shader Model 6.6 support.
        {
            var data: d3d12.FEATURE_DATA_SHADER_MODEL = .{ .HighestShaderModel = .SM_6_7 };
            const hr = device.CheckFeatureSupport(.SHADER_MODEL, &data, @sizeOf(d3d12.FEATURE_DATA_SHADER_MODEL));
            if (hr != w.S_OK or @enumToInt(data.HighestShaderModel) < @enumToInt(d3d12.SHADER_MODEL.SM_6_6)) {
                _ = w.user32.messageBoxA(
                    window,
                    "This applications requires graphics card driver that supports Shader Model 6.6. " ++
                        "Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    w.user32.MB_OK | w.user32.MB_ICONERROR,
                ) catch 0;
                w.kernel32.ExitProcess(0);
            }
        }

        // Check for Resource Binding Tier 3 support.
        {
            var data: d3d12.FEATURE_DATA_D3D12_OPTIONS = std.mem.zeroes(d3d12.FEATURE_DATA_D3D12_OPTIONS);
            const hr = device.CheckFeatureSupport(.OPTIONS, &data, @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS));
            if (hr != w.S_OK or
                @enumToInt(data.ResourceBindingTier) < @enumToInt(d3d12.RESOURCE_BINDING_TIER.TIER_3))
            {
                _ = w.user32.messageBoxA(
                    window,
                    "This applications requires graphics card driver that supports Resource Binding Tier 3. " ++
                        "Please update your graphics driver and try again.",
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
            }, &d3d12.IID_ICommandQueue, @ptrCast(*?*anyopaque, &cmdqueue)));
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
                @ptrCast(*?*anyopaque, &swapchain3),
            ));
            break :blk swapchain3;
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

        var cbv_srv_uav_gpu_heaps: [max_num_buffered_frames + 1]DescriptorHeap = undefined;
        for (cbv_srv_uav_gpu_heaps) |_, heap_index| {
            // We create one large descriptor heap and then split it into ranges:
            //   - range 0: contains persistent descriptors (each descriptor lives until heap is destroyed)
            //   - range 1,2,..max_num_buffered_frames: contains non-persistent descriptors (1 frame lifetime)
            if (heap_index == 0) {
                cbv_srv_uav_gpu_heaps[0] = DescriptorHeap.init(
                    device,
                    num_cbv_srv_uav_gpu_descriptors * (max_num_buffered_frames + 1),
                    .CBV_SRV_UAV,
                    d3d12.DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                );
                cbv_srv_uav_gpu_heaps[0].capacity = @divExact(
                    cbv_srv_uav_gpu_heaps[0].capacity,
                    max_num_buffered_frames + 1,
                );
            } else {
                const range_capacity = cbv_srv_uav_gpu_heaps[0].capacity;
                const descriptor_size = cbv_srv_uav_gpu_heaps[0].descriptor_size;

                // Non-persistent heap does not own memory it is just a sub-range in a persistent heap
                cbv_srv_uav_gpu_heaps[heap_index] = cbv_srv_uav_gpu_heaps[0];
                cbv_srv_uav_gpu_heaps[heap_index].heap = null;
                cbv_srv_uav_gpu_heaps[heap_index].base.cpu_handle.ptr += heap_index * range_capacity * descriptor_size;
                cbv_srv_uav_gpu_heaps[heap_index].base.gpu_handle.ptr += heap_index * range_capacity * descriptor_size;
            }
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
                    @ptrCast(*?*anyopaque, &swapbuffers[buffer_index]),
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

        const d2d_state = if (enable_d2d) blk_d2d: {
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
                    @ptrCast(*?*anyopaque, &device11on12),
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
                    @ptrCast(*?*anyopaque, &d2d_factory),
                ));
                break :blk d2d_factory;
            };

            const dxgi_device = blk: {
                var dxgi_device: *dxgi.IDevice = undefined;
                hrPanicOnFail(device11on12.QueryInterface(
                    &dxgi.IID_IDevice,
                    @ptrCast(*?*anyopaque, &dxgi_device),
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
                    @ptrCast(*?*anyopaque, &dwrite_factory),
                ));
                break :blk dwrite_factory;
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
                        @ptrCast(*?*anyopaque, &swapbuffers11[buffer_index]),
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
                        @ptrCast(*?*anyopaque, &surface),
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

            break :blk_d2d .{
                .factory = d2d_factory,
                .device = d2d_device,
                .context = d2d_device_context,
                .device11on12 = device11on12,
                .device11 = dx11.device,
                .context11 = dx11.device_context,
                .swapbuffers11 = swapbuffers11,
                .targets = d2d_targets,
                .dwrite_factory = dwrite_factory,
            };
        } else null;

        const frame_fence = blk: {
            var frame_fence: *d3d12.IFence = undefined;
            hrPanicOnFail(device.CreateFence(
                0,
                d3d12.FENCE_FLAG_NONE,
                &d3d12.IID_IFence,
                @ptrCast(*?*anyopaque, &frame_fence),
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
                    @ptrCast(*?*anyopaque, &cmdallocs[cmdalloc_index]),
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
                @ptrCast(*?*anyopaque, &cmdlist),
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
                    hm.ensureTotalCapacity(
                        std.heap.page_allocator,
                        PipelinePool.max_num_pipelines,
                    ) catch unreachable;
                    break :blk hm;
                },
                .current = .{ .index = 0, .generation = 0 },
            },
            .transition_resource_barriers = std.heap.page_allocator.alloc(
                TransitionResourceBarrier,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
            .resources_to_release = std.ArrayList(ResourceWithCounter).initCapacity(
                std.heap.page_allocator,
                64,
            ) catch unreachable,
            .num_transition_resource_barriers = 0,
            .viewport_width = viewport_width,
            .viewport_height = viewport_height,
            .frame_index = 0,
            .back_buffer_index = swapchain.GetCurrentBackBufferIndex(),
            .window = window,
            .is_cmdlist_opened = is_cmdlist_opened,
            .d2d = d2d_state,
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
        if (enable_d2d) {
            _ = gr.d2d.?.factory.Release();
            _ = gr.d2d.?.device.Release();
            _ = gr.d2d.?.context.Release();
            _ = gr.d2d.?.device11on12.Release();
            _ = gr.d2d.?.device11.Release();
            _ = gr.d2d.?.context11.Release();
            _ = gr.d2d.?.dwrite_factory.Release();
            for (gr.d2d.?.targets) |target| _ = target.Release();
            for (gr.d2d.?.swapbuffers11) |swapbuffer11| _ = swapbuffer11.Release();
        }
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
            &[_]*d3d12.IDescriptorHeap{gr.cbv_srv_uav_gpu_heaps[0].heap.?},
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

        ztracy.frameMark();
        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));

        const gpu_frame_counter = gr.frame_fence.GetCompletedValue();
        if ((gr.frame_fence_counter - gpu_frame_counter) >= max_num_buffered_frames) {
            hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gr.frame_fence_event));
            w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;
        }

        gr.frame_index = (gr.frame_index + 1) % max_num_buffered_frames;
        gr.back_buffer_index = gr.swapchain.GetCurrentBackBufferIndex();

        // Reset current non-persistent heap (+1 because heap 0 is persistent)
        gr.cbv_srv_uav_gpu_heaps[gr.frame_index + 1].size = 0;
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

        gr.d2d.?.device11on12.AcquireWrappedResources(
            &[_]*d3d11.IResource{gr.d2d.?.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.?.context.SetTarget(@ptrCast(*d2d1.IImage, gr.d2d.?.targets[gr.back_buffer_index]));
        gr.d2d.?.context.BeginDraw();
    }

    pub fn endDraw2d(gr: *GraphicsContext) void {
        var info_queue: *d3d12d.IInfoQueue = undefined;
        const mute_d2d_completely = true;
        if (enable_dx_debug) {
            // NOTE(mziulek): D2D1 is slow. It creates and destroys resources every frame. To see create/destroy
            // messages in debug output set 'mute_d2d_completely' to 'false'.
            hrPanicOnFail(gr.device.QueryInterface(&d3d12d.IID_IInfoQueue, @ptrCast(*?*anyopaque, &info_queue)));

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
        hrPanicOnFail(gr.d2d.?.context.EndDraw(null, null));

        gr.d2d.?.device11on12.ReleaseWrappedResources(
            &[_]*d3d11.IResource{gr.d2d.?.swapbuffers11[gr.back_buffer_index]},
            1,
        );
        gr.d2d.?.context11.Flush();

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

    fn flushGpuCommands(gr: *GraphicsContext) void {
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
        const was_cmdlist_opened = gr.is_cmdlist_opened;
        gr.flushGpuCommands();

        gr.frame_fence_counter += 1;

        hrPanicOnFail(gr.cmdqueue.Signal(gr.frame_fence, gr.frame_fence_counter));
        hrPanicOnFail(gr.frame_fence.SetEventOnCompletion(gr.frame_fence_counter, gr.frame_fence_event));
        w.WaitForSingleObject(gr.frame_fence_event, w.INFINITE) catch unreachable;

        // Reset current non-persistent heap (+1 because heap 0 is persistent)
        gr.cbv_srv_uav_gpu_heaps[gr.frame_index + 1].size = 0;
        gr.upload_memory_heaps[gr.frame_index].size = 0;

        if (gr.resources_to_release.items.len > 0) {
            for (gr.resources_to_release.items) |res| {
                _ = gr.releaseResource(res.resource);
            }
            gr.resources_to_release.resize(0) catch unreachable;
        }

        if (was_cmdlist_opened) {
            beginFrame(gr);
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
                @ptrCast(*?*anyopaque, &resource),
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
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        return createGraphicsShaderPipelineVsGsPs(gr, arena, pso_desc, vs_cso_path, null, ps_cso_path);
    }

    pub fn createGraphicsShaderPipelineVsGsPs(
        gr: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        gs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        return createGraphicsShaderPipelineRsVsGsPs(gr, arena, pso_desc, null, vs_cso_path, gs_cso_path, ps_cso_path);
    }

    pub fn createGraphicsShaderPipelineRsVsGsPs(
        gr: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        root_signature: ?*d3d12.IRootSignature,
        vs_cso_path: ?[]const u8,
        gs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        const tracy_zone = ztracy.zone(@src(), 1);
        defer tracy_zone.end();

        if (vs_cso_path) |path| {
            const vs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer vs_file.close();
            const vs_code = vs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.VS = .{ .pShaderBytecode = vs_code.ptr, .BytecodeLength = vs_code.len };
        } else {
            assert(pso_desc.VS.pShaderBytecode != null);
        }

        if (gs_cso_path) |path| {
            const gs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer gs_file.close();
            const gs_code = gs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.GS = .{ .pShaderBytecode = gs_code.ptr, .BytecodeLength = gs_code.len };
        }

        if (ps_cso_path) |path| {
            const ps_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer ps_file.close();
            const ps_code = ps_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.PS = .{ .pShaderBytecode = ps_code.ptr, .BytecodeLength = ps_code.len };
        }

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @ptrCast([*]const u8, pso_desc.VS.pShaderBytecode.?)[0..pso_desc.VS.BytecodeLength],
            );
            if (pso_desc.GS.pShaderBytecode != null) {
                hasher.update(
                    @ptrCast([*]const u8, pso_desc.GS.pShaderBytecode.?)[0..pso_desc.GS.BytecodeLength],
                );
            }
            if (pso_desc.PS.pShaderBytecode != null) {
                hasher.update(
                    @ptrCast([*]const u8, pso_desc.PS.pShaderBytecode.?)[0..pso_desc.PS.BytecodeLength],
                );
            }
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
            std.log.info("[graphics] Graphics pipeline cache hit detected.", .{});
            const handle = gr.pipeline.map.getEntry(hash).?.value_ptr.*;
            _ = incrementPipelineRefcount(gr.*, handle);
            return handle;
        }

        const rs = blk: {
            if (root_signature) |rs| {
                break :blk rs;
            } else {
                var rs: *d3d12.IRootSignature = undefined;
                hrPanicOnFail(gr.device.CreateRootSignature(
                    0,
                    pso_desc.VS.pShaderBytecode.?,
                    pso_desc.VS.BytecodeLength,
                    &d3d12.IID_IRootSignature,
                    @ptrCast(*?*anyopaque, &rs),
                ));
                break :blk rs;
            }
        };

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gr.device.CreateGraphicsPipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
            ));
            break :blk pso;
        };

        const handle = gr.pipeline.pool.addPipeline(pso, rs, .Graphics);
        gr.pipeline.map.putAssumeCapacity(hash, handle);
        return handle;
    }

    pub fn createMeshShaderPipeline(
        gr: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.MESH_SHADER_PIPELINE_STATE_DESC,
        as_cso_path: ?[]const u8,
        ms_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        const tracy_zone = ztracy.zone(@src(), 1);
        defer tracy_zone.end();

        if (as_cso_path) |path| {
            const as_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer as_file.close();
            const as_code = as_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.AS = .{ .pShaderBytecode = as_code.ptr, .BytecodeLength = as_code.len };
        }

        if (ms_cso_path) |path| {
            const ms_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer ms_file.close();
            const ms_code = ms_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.MS = .{ .pShaderBytecode = ms_code.ptr, .BytecodeLength = ms_code.len };
        } else {
            assert(pso_desc.MS.pShaderBytecode != null);
        }

        if (ps_cso_path) |path| {
            const ps_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer ps_file.close();
            const ps_code = ps_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.PS = .{ .pShaderBytecode = ps_code.ptr, .BytecodeLength = ps_code.len };
        } else {
            assert(pso_desc.PS.pShaderBytecode != null);
        }

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @ptrCast([*]const u8, pso_desc.MS.pShaderBytecode.?)[0..pso_desc.MS.BytecodeLength],
            );
            if (pso_desc.AS.pShaderBytecode != null) {
                hasher.update(
                    @ptrCast([*]const u8, pso_desc.AS.pShaderBytecode.?)[0..pso_desc.AS.BytecodeLength],
                );
            }
            hasher.update(
                @ptrCast([*]const u8, pso_desc.PS.pShaderBytecode.?)[0..pso_desc.PS.BytecodeLength],
            );
            hasher.update(std.mem.asBytes(&pso_desc.BlendState));
            hasher.update(std.mem.asBytes(&pso_desc.SampleMask));
            hasher.update(std.mem.asBytes(&pso_desc.RasterizerState));
            hasher.update(std.mem.asBytes(&pso_desc.DepthStencilState));
            hasher.update(std.mem.asBytes(&pso_desc.PrimitiveTopologyType));
            hasher.update(std.mem.asBytes(&pso_desc.NumRenderTargets));
            hasher.update(std.mem.asBytes(&pso_desc.RTVFormats));
            hasher.update(std.mem.asBytes(&pso_desc.DSVFormat));
            hasher.update(std.mem.asBytes(&pso_desc.SampleDesc));
            break :compute_hash hasher.final();
        };
        std.log.info("[graphics] Mesh shader pipeline hash: {d}", .{hash});

        if (gr.pipeline.map.contains(hash)) {
            std.log.info("[graphics] Mesh shader pipeline cache hit detected.", .{});
            const handle = gr.pipeline.map.getEntry(hash).?.value_ptr.*;
            _ = incrementPipelineRefcount(gr.*, handle);
            return handle;
        }

        const rs = blk: {
            var rs: *d3d12.IRootSignature = undefined;
            hrPanicOnFail(gr.device.CreateRootSignature(
                0,
                pso_desc.MS.pShaderBytecode.?,
                pso_desc.MS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @ptrCast(*?*anyopaque, &rs),
            ));
            break :blk rs;
        };

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var stream = d3d12.PIPELINE_MESH_STATE_STREAM.init(pso_desc.*);
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gr.device.CreatePipelineState(
                &d3d12.PIPELINE_STATE_STREAM_DESC{
                    .SizeInBytes = @sizeOf(@TypeOf(stream)),
                    .pPipelineStateSubobjectStream = &stream,
                },
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
            ));
            break :blk pso;
        };

        const handle = gr.pipeline.pool.addPipeline(pso, rs, .Graphics);
        gr.pipeline.map.putAssumeCapacity(hash, handle);
        return handle;
    }

    pub fn createComputeShaderPipeline(
        gr: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.COMPUTE_PIPELINE_STATE_DESC,
        cs_cso_path: ?[]const u8,
    ) PipelineHandle {
        const tracy_zone = ztracy.zone(@src(), 1);
        defer tracy_zone.end();

        if (cs_cso_path) |path| {
            const cs_file = std.fs.cwd().openFile(path, .{}) catch unreachable;
            defer cs_file.close();
            const cs_code = cs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.CS = .{ .pShaderBytecode = cs_code.ptr, .BytecodeLength = cs_code.len };
        } else {
            assert(pso_desc.CS.pShaderBytecode != null);
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
                @ptrCast(*?*anyopaque, &rs),
            ));
            break :blk rs;
        };

        pso_desc.pRootSignature = rs;

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gr.device.CreateComputePipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
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
        // Allocate non-persistent descriptors
        return gr.cbv_srv_uav_gpu_heaps[gr.frame_index + 1].allocateDescriptors(num_descriptors);
    }

    pub fn allocatePersistentGpuDescriptors(gr: *GraphicsContext, num_descriptors: u32) PersistentDescriptor {
        // Allocate descriptors from persistent heap (heap 0)
        const index = gr.cbv_srv_uav_gpu_heaps[0].size;
        const base = gr.cbv_srv_uav_gpu_heaps[0].allocateDescriptors(num_descriptors);
        return .{
            .cpu_handle = base.cpu_handle,
            .gpu_handle = base.gpu_handle,
            .index = index,
        };
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
        gr.device.GetCopyableFootprints(
            &resource.desc,
            subresource,
            layout.len,
            0,
            &layout,
            null,
            null,
            &required_size,
        );

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

pub const MipmapGenerator = struct {
    const num_scratch_textures = 4;

    pipeline: PipelineHandle,
    scratch_textures: [num_scratch_textures]ResourceHandle,
    base_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
    format: dxgi.FORMAT,

    pub fn init(
        arena: std.mem.Allocator,
        gr: *GraphicsContext,
        format: dxgi.FORMAT,
        comptime content_dir: []const u8,
    ) MipmapGenerator {
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
        const pipeline = gr.createComputeShaderPipeline(
            arena,
            &desc,
            content_dir ++ "shaders/generate_mipmaps.cs.cso",
        );

        return .{
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
                        .right = @intCast(u32, texture_desc.Width) >> @intCast(
                            u5,
                            mip_index + 1 + current_src_mip_level,
                        ),
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
            if (i > 0 and i <= num_swapbuffers) {
                // Release internally created swapbuffers.
                if (resource.raw) |raw| {
                    _ = raw.Release();
                }
            } else if (i > num_swapbuffers) {
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

pub const PersistentDescriptor = struct {
    cpu_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    gpu_handle: d3d12.GPU_DESCRIPTOR_HANDLE,
    index: u32,
};

const DescriptorHeap = struct {
    heap: ?*d3d12.IDescriptorHeap,
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
            var heap: ?*d3d12.IDescriptorHeap = null;
            hrPanicOnFail(device.CreateDescriptorHeap(&.{
                .Type = heap_type,
                .NumDescriptors = capacity,
                .Flags = flags,
                .NodeMask = 0,
            }, &d3d12.IID_IDescriptorHeap, @ptrCast(*?*anyopaque, &heap)));
            break :blk heap.?;
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
        if (dheap.heap != null) {
            _ = dheap.heap.?.Release();
        }
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
                @ptrCast(*?*anyopaque, &resource),
            ));
            break :blk resource;
        };
        const cpu_base = blk: {
            var cpu_base: [*]u8 = undefined;
            hrPanicOnFail(resource.Map(
                0,
                &d3d12.RANGE{ .Begin = 0, .End = 0 },
                @ptrCast(*?*anyopaque, &cpu_base),
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
