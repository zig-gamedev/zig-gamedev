const std = @import("std");
const assert = std.debug.assert;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
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

const enable_debug_layer = @import("zd3d12_options").enable_debug_layer;
const enable_gbv = @import("zd3d12_options").enable_gbv;
const enable_d2d = @import("zd3d12_options").enable_d2d;
const upload_heap_capacity = @import("zd3d12_options").upload_heap_capacity;

// TODO(mziulek): For now, we always transition *all* subresources.
const TransitionResourceBarrier = struct {
    state_before: d3d12.RESOURCE_STATES,
    state_after: d3d12.RESOURCE_STATES,
    resource: ResourceHandle,
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
    pipeline_pool: PipelinePool,
    current_pipeline: PipelineHandle,
    transition_resource_barriers: std.ArrayListUnmanaged(TransitionResourceBarrier),
    viewport_width: u32,
    viewport_height: u32,
    frame_fence: *d3d12.IFence,
    frame_fence_event: w32.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,
    window: w32.HWND,
    is_cmdlist_opened: bool,
    d2d: ?D2dState,
    wic_factory: *wic.IImagingFactory,
    present_flags: dxgi.PRESENT_FLAG,
    present_interval: w32.UINT,

    pub fn init(allocator: std.mem.Allocator, window: w32.HWND) GraphicsContext {
        const wic_factory = blk: {
            var wic_factory: *wic.IImagingFactory = undefined;
            hrPanicOnFail(w32.CoCreateInstance(
                &wic.CLSID_ImagingFactory,
                null,
                w32.CLSCTX_INPROC_SERVER,
                &wic.IID_IImagingFactory,
                @ptrCast(*?*anyopaque, &wic_factory),
            ));
            break :blk wic_factory;
        };

        const factory = blk: {
            var factory: *dxgi.IFactory6 = undefined;
            hrPanicOnFail(dxgi.CreateDXGIFactory2(
                if (enable_debug_layer) dxgi.CREATE_FACTORY_DEBUG else 0,
                &dxgi.IID_IFactory6,
                @ptrCast(*?*anyopaque, &factory),
            ));
            break :blk factory;
        };
        defer _ = factory.Release();

        var present_flags: dxgi.PRESENT_FLAG = .{};
        var present_interval: w32.UINT = 0;
        {
            var allow_tearing: w32.BOOL = w32.FALSE;
            var hr = factory.CheckFeatureSupport(
                .PRESENT_ALLOW_TEARING,
                &allow_tearing,
                @sizeOf(@TypeOf(allow_tearing)),
            );

            if (hr == w32.S_OK and allow_tearing == w32.TRUE) {
                present_flags.ALLOW_TEARING = true;
            }
        }

        if (enable_debug_layer) {
            var maybe_debug: ?*d3d12d.IDebug1 = null;
            _ = d3d12.GetDebugInterface(&d3d12d.IID_IDebug1, @ptrCast(*?*anyopaque, &maybe_debug));
            if (maybe_debug) |debug| {
                debug.EnableDebugLayer();
                if (enable_gbv) {
                    debug.SetEnableGPUBasedValidation(w32.TRUE);
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
                .HIGH_PERFORMANCE,
                &dxgi.IID_IAdapter1,
                &optional_adapter1,
            ) == w32.S_OK) {
                adapter_index += 1;
                if (optional_adapter1) |adapter1| {
                    var adapter1_desc: dxgi.ADAPTER_DESC1 = undefined;
                    if (adapter1.GetDesc1(&adapter1_desc) == w32.S_OK) {
                        if (adapter1_desc.Flags.SOFTWARE) {
                            // Don't select the Basic Render Driver adapter.
                            continue;
                        }

                        const hr = d3d12.CreateDevice(
                            @ptrCast(*w32.IUnknown, adapter1),
                            .@"11_1",
                            &d3d12.IID_IDevice9,
                            null,
                        );
                        if (hr == w32.S_OK or hr == w32.S_FALSE) {
                            adapter = adapter1;
                            break;
                        }
                    }
                }
            }
            break :blk adapter;
        };
        defer {
            if (suitable_adapter) |adapter| _ = adapter.Release();
        }

        const device = blk: {
            var device: *d3d12.IDevice9 = undefined;
            const hr = d3d12.CreateDevice(
                if (suitable_adapter) |adapter| @ptrCast(*w32.IUnknown, adapter) else null,
                .@"11_1",
                &d3d12.IID_IDevice9,
                @ptrCast(*?*anyopaque, &device),
            );

            if (hr != w32.S_OK) {
                _ = w32.MessageBoxA(
                    window,
                    "Failed to create Direct3D 12 Device. This applications requires graphics card " ++
                        "with DirectX 12 Feature Level 11.1 support.",
                    "Your graphics card driver may be old",
                    w32.MB_OK | w32.MB_ICONERROR,
                );
                w32.ExitProcess(0);
            }
            break :blk device;
        };

        // Check for Shader Model 6.6 support.
        {
            var data: d3d12.FEATURE_DATA_SHADER_MODEL = .{ .HighestShaderModel = .@"6_7" };
            const hr = device.CheckFeatureSupport(.SHADER_MODEL, &data, @sizeOf(d3d12.FEATURE_DATA_SHADER_MODEL));
            if (hr != w32.S_OK or @enumToInt(data.HighestShaderModel) < @enumToInt(d3d12.SHADER_MODEL.@"6_6")) {
                _ = w32.MessageBoxA(
                    window,
                    "This applications requires graphics card driver that supports Shader Model 6.6. " ++
                        "Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    w32.MB_OK | w32.MB_ICONERROR,
                );
                w32.ExitProcess(0);
            }
        }

        // Check for Resource Binding Tier 3 support.
        {
            var data: d3d12.FEATURE_DATA_D3D12_OPTIONS = std.mem.zeroes(d3d12.FEATURE_DATA_D3D12_OPTIONS);
            const hr = device.CheckFeatureSupport(.OPTIONS, &data, @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS));
            if (hr != w32.S_OK or
                @enumToInt(data.ResourceBindingTier) < @enumToInt(d3d12.RESOURCE_BINDING_TIER.TIER_3))
            {
                _ = w32.MessageBoxA(
                    window,
                    "This applications requires graphics card driver that supports Resource Binding Tier 3. " ++
                        "Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    w32.MB_OK | w32.MB_ICONERROR,
                );
                w32.ExitProcess(0);
            }
        }

        const cmdqueue = blk: {
            var cmdqueue: *d3d12.ICommandQueue = undefined;
            hrPanicOnFail(device.CreateCommandQueue(&.{
                .Type = .DIRECT,
                .Priority = @enumToInt(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
                .Flags = .{},
                .NodeMask = 0,
            }, &d3d12.IID_ICommandQueue, @ptrCast(*?*anyopaque, &cmdqueue)));
            break :blk cmdqueue;
        };

        var rect: w32.RECT = undefined;
        _ = w32.GetClientRect(window, &rect);
        const viewport_width = @intCast(u32, rect.right - rect.left);
        const viewport_height = @intCast(u32, rect.bottom - rect.top);

        const swapchain = blk: {
            var swapchain: *dxgi.ISwapChain = undefined;
            hrPanicOnFail(factory.CreateSwapChain(
                @ptrCast(*w32.IUnknown, cmdqueue),
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
                    .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                    .BufferCount = num_swapbuffers,
                    .OutputWindow = window,
                    .Windowed = w32.TRUE,
                    .SwapEffect = .FLIP_DISCARD,
                    .Flags = .{ .ALLOW_TEARING = present_flags.ALLOW_TEARING },
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

        var resource_pool = ResourcePool.init(allocator);
        var pipeline_pool = PipelinePool.init(allocator);

        var rtv_heap = DescriptorHeap.init(device, num_rtv_descriptors, .RTV, .{});
        var dsv_heap = DescriptorHeap.init(device, num_dsv_descriptors, .DSV, .{});

        var cbv_srv_uav_cpu_heap = DescriptorHeap.init(
            device,
            num_cbv_srv_uav_cpu_descriptors,
            .CBV_SRV_UAV,
            .{},
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
                    .{ .SHADER_VISIBLE = true },
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
                cbv_srv_uav_gpu_heaps[heap_index].base.cpu_handle.ptr +=
                    heap_index * range_capacity * descriptor_size;
                cbv_srv_uav_gpu_heaps[heap_index].base.gpu_handle.ptr +=
                    heap_index * range_capacity * descriptor_size;
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
                    d3d12.RESOURCE_STATES.PRESENT,
                );
            }
            break :blk swapchain_buffers;
        };

        const d2d_state = if (enable_d2d) blk_d2d: {
            const dx11 = blk: {
                var device11: *d3d11.IDevice = undefined;
                var device_context11: *d3d11.IDeviceContext = undefined;
                hrPanicOnFail(d3d11on12.D3D11On12CreateDevice(
                    @ptrCast(*w32.IUnknown, device),
                    .{ .DEBUG = enable_debug_layer, .BGRA_SUPPORT = true },
                    null,
                    0,
                    &[_]*w32.IUnknown{@ptrCast(*w32.IUnknown, cmdqueue)},
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
                    if (enable_debug_layer)
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
                        @ptrCast(
                            *w32.IUnknown,
                            resource_pool.lookupResource(swapchain_buffers[buffer_index]).?.raw.?,
                        ),
                        &d3d11on12.RESOURCE_FLAGS{
                            .BindFlags = .{ .RENDER_TARGET = true },
                            .MiscFlags = .{},
                            .CPUAccessFlags = .{},
                            .StructureByteStride = 0,
                        },
                        .{ .RENDER_TARGET = true },
                        d3d12.RESOURCE_STATES.PRESENT,
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
            hrPanicOnFail(device.CreateFence(0, .{}, &d3d12.IID_IFence, @ptrCast(*?*anyopaque, &frame_fence)));
            break :blk frame_fence;
        };

        const frame_fence_event = w32.CreateEventExA(
            null,
            "frame_fence_event",
            0,
            w32.EVENT_ALL_ACCESS,
        ).?;

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
            .pipeline_pool = pipeline_pool,
            .current_pipeline = .{},
            .transition_resource_barriers = std.ArrayListUnmanaged(TransitionResourceBarrier).initCapacity(
                allocator,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
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

    pub fn deinit(gctx: *GraphicsContext, allocator: std.mem.Allocator) void {
        gctx.finishGpuCommands();
        gctx.transition_resource_barriers.deinit(allocator);
        _ = w32.CloseHandle(gctx.frame_fence_event);
        gctx.resource_pool.deinit(allocator);
        gctx.pipeline_pool.deinit(allocator);
        gctx.rtv_heap.deinit();
        gctx.dsv_heap.deinit();
        gctx.cbv_srv_uav_cpu_heap.deinit();
        if (enable_d2d) {
            _ = gctx.d2d.?.factory.Release();
            _ = gctx.d2d.?.device.Release();
            _ = gctx.d2d.?.context.Release();
            _ = gctx.d2d.?.device11on12.Release();
            _ = gctx.d2d.?.device11.Release();
            _ = gctx.d2d.?.context11.Release();
            _ = gctx.d2d.?.dwrite_factory.Release();
            for (gctx.d2d.?.targets) |target|
                _ = target.Release();
            for (gctx.d2d.?.swapbuffers11) |swapbuffer11|
                _ = swapbuffer11.Release();
        }
        for (gctx.cbv_srv_uav_gpu_heaps) |*heap|
            heap.deinit();
        for (gctx.upload_memory_heaps) |*heap|
            heap.deinit();
        _ = gctx.device.Release();
        _ = gctx.cmdqueue.Release();
        _ = gctx.swapchain.Release();
        _ = gctx.frame_fence.Release();
        _ = gctx.cmdlist.Release();
        for (gctx.cmdallocs) |cmdalloc|
            _ = cmdalloc.Release();
        _ = gctx.wic_factory.Release();
        gctx.* = undefined;
    }

    pub fn beginFrame(gctx: *GraphicsContext) void {
        assert(!gctx.is_cmdlist_opened);
        const cmdalloc = gctx.cmdallocs[gctx.frame_index];
        hrPanicOnFail(cmdalloc.Reset());
        hrPanicOnFail(gctx.cmdlist.Reset(cmdalloc, null));
        gctx.is_cmdlist_opened = true;
        gctx.cmdlist.SetDescriptorHeaps(
            1,
            &[_]*d3d12.IDescriptorHeap{gctx.cbv_srv_uav_gpu_heaps[0].heap.?},
        );
        gctx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = @intToFloat(f32, gctx.viewport_width),
            .Height = @intToFloat(f32, gctx.viewport_height),
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        gctx.cmdlist.RSSetScissorRects(1, &[_]d3d12.RECT{.{
            .left = 0,
            .top = 0,
            .right = @intCast(c_long, gctx.viewport_width),
            .bottom = @intCast(c_long, gctx.viewport_height),
        }});
        gctx.current_pipeline = .{};
    }

    pub fn endFrame(gctx: *GraphicsContext) void {
        gctx.flushGpuCommands();

        gctx.frame_fence_counter += 1;
        hrPanicOnFail(gctx.swapchain.Present(gctx.present_interval, gctx.present_flags));
        // TODO(mziulek):
        // Handle DXGI_ERROR_DEVICE_REMOVED and DXGI_ERROR_DEVICE_RESET codes here - we need to re-create
        // all resources in that case.
        // Take a look at:
        // https://github.com/microsoft/DirectML/blob/master/Samples/DirectMLSuperResolution/DeviceResources.cpp

        hrPanicOnFail(gctx.cmdqueue.Signal(gctx.frame_fence, gctx.frame_fence_counter));

        const gpu_frame_counter = gctx.frame_fence.GetCompletedValue();
        if ((gctx.frame_fence_counter - gpu_frame_counter) >= max_num_buffered_frames) {
            hrPanicOnFail(gctx.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, gctx.frame_fence_event));
            _ = w32.WaitForSingleObject(gctx.frame_fence_event, w32.INFINITE);
        }

        gctx.frame_index = (gctx.frame_index + 1) % max_num_buffered_frames;
        gctx.back_buffer_index = gctx.swapchain.GetCurrentBackBufferIndex();

        // Reset current non-persistent heap (+1 because heap 0 is persistent)
        gctx.cbv_srv_uav_gpu_heaps[gctx.frame_index + 1].size = 0;
        gctx.upload_memory_heaps[gctx.frame_index].size = 0;
    }

    pub fn beginDraw2d(gctx: *GraphicsContext) void {
        gctx.flushGpuCommands();

        gctx.d2d.?.device11on12.AcquireWrappedResources(
            &[_]*d3d11.IResource{gctx.d2d.?.swapbuffers11[gctx.back_buffer_index]},
            1,
        );
        gctx.d2d.?.context.SetTarget(@ptrCast(*d2d1.IImage, gctx.d2d.?.targets[gctx.back_buffer_index]));
        gctx.d2d.?.context.BeginDraw();
    }

    pub fn endDraw2d(gctx: *GraphicsContext) void {
        var info_queue: *d3d12d.IInfoQueue = undefined;
        const mute_d2d_completely = true;
        if (enable_debug_layer) {
            // NOTE(mziulek): D2D1 is slow. It creates and destroys resources every frame. To see create/destroy
            // messages in debug output set 'mute_d2d_completely' to 'false'.
            hrPanicOnFail(gctx.device.QueryInterface(
                &d3d12d.IID_IInfoQueue,
                @ptrCast(*?*anyopaque, &info_queue),
            ));

            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(w32.TRUE);
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
        hrPanicOnFail(gctx.d2d.?.context.EndDraw(null, null));

        gctx.d2d.?.device11on12.ReleaseWrappedResources(
            &[_]*d3d11.IResource{gctx.d2d.?.swapbuffers11[gctx.back_buffer_index]},
            1,
        );
        gctx.d2d.?.context11.Flush();

        if (enable_debug_layer) {
            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(w32.FALSE);
            } else {
                info_queue.PopStorageFilter();
            }
            _ = info_queue.Release();
        }

        // Above calls will set back buffer state to PRESENT. We need to reflect this change
        // in 'resource_pool' by manually setting state.
        gctx.resource_pool.lookupResource(gctx.swapchain_buffers[gctx.back_buffer_index]).?.state =
            d3d12.RESOURCE_STATES.PRESENT;
    }

    fn flushGpuCommands(gctx: *GraphicsContext) void {
        if (gctx.is_cmdlist_opened) {
            gctx.flushResourceBarriers();
            hrPanicOnFail(gctx.cmdlist.Close());
            gctx.is_cmdlist_opened = false;
            gctx.cmdqueue.ExecuteCommandLists(
                1,
                &[_]*d3d12.ICommandList{@ptrCast(*d3d12.ICommandList, gctx.cmdlist)},
            );
        }
    }

    pub fn finishGpuCommands(gctx: *GraphicsContext) void {
        const was_cmdlist_opened = gctx.is_cmdlist_opened;
        gctx.flushGpuCommands();

        gctx.frame_fence_counter += 1;

        hrPanicOnFail(gctx.cmdqueue.Signal(gctx.frame_fence, gctx.frame_fence_counter));
        hrPanicOnFail(gctx.frame_fence.SetEventOnCompletion(gctx.frame_fence_counter, gctx.frame_fence_event));
        _ = w32.WaitForSingleObject(gctx.frame_fence_event, w32.INFINITE);

        // Reset current non-persistent heap (+1 because heap 0 is persistent)
        gctx.cbv_srv_uav_gpu_heaps[gctx.frame_index + 1].size = 0;
        gctx.upload_memory_heaps[gctx.frame_index].size = 0;

        if (was_cmdlist_opened) {
            beginFrame(gctx);
        }
    }

    pub fn getBackBuffer(gctx: GraphicsContext) struct {
        resource_handle: ResourceHandle,
        descriptor_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    } {
        return .{
            .resource_handle = gctx.swapchain_buffers[gctx.back_buffer_index],
            .descriptor_handle = .{
                .ptr = gctx.rtv_heap.base.cpu_handle.ptr + gctx.back_buffer_index * gctx.rtv_heap.descriptor_size,
            },
        };
    }

    pub inline fn lookupResource(gctx: GraphicsContext, handle: ResourceHandle) ?*d3d12.IResource {
        const resource = gctx.resource_pool.lookupResource(handle);
        if (resource == null)
            return null;

        return resource.?.raw.?;
    }

    pub fn isResourceValid(gctx: GraphicsContext, handle: ResourceHandle) bool {
        return gctx.resource_pool.isResourceValid(handle);
    }

    pub fn getResourceSize(gctx: GraphicsContext, handle: ResourceHandle) u64 {
        const resource = gctx.resource_pool.lookupResource(handle);
        if (resource == null)
            return 0;

        assert(resource.?.desc.Dimension == .BUFFER);
        return resource.?.desc.Width;
    }

    pub fn getResourceDesc(gctx: GraphicsContext, handle: ResourceHandle) d3d12.RESOURCE_DESC {
        const resource = gctx.resource_pool.lookupResource(handle);
        if (resource == null)
            return d3d12.RESOURCE_DESC.initBuffer(0);

        return resource.?.desc;
    }

    pub fn createCommittedResource(
        gctx: *GraphicsContext,
        heap_type: d3d12.HEAP_TYPE,
        heap_flags: d3d12.HEAP_FLAGS,
        desc: *const d3d12.RESOURCE_DESC,
        initial_state: d3d12.RESOURCE_STATES,
        clear_value: ?*const d3d12.CLEAR_VALUE,
    ) HResultError!ResourceHandle {
        const resource = blk: {
            var resource: *d3d12.IResource = undefined;
            try hrErrorOnFail(gctx.device.CreateCommittedResource(
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
        return gctx.resource_pool.addResource(resource, initial_state);
    }

    pub fn destroyResource(gctx: GraphicsContext, handle: ResourceHandle) void {
        gctx.resource_pool.destroyResource(handle);
    }

    pub fn flushResourceBarriers(gctx: *GraphicsContext) void {
        if (gctx.transition_resource_barriers.items.len > 0) {
            var d3d12_barriers: [max_num_buffered_resource_barriers]d3d12.RESOURCE_BARRIER = undefined;

            var num_valid_barriers: u32 = 0;
            for (gctx.transition_resource_barriers.items) |barrier| {
                if (gctx.resource_pool.isResourceValid(barrier.resource)) {
                    d3d12_barriers[num_valid_barriers] = .{
                        .Type = .TRANSITION,
                        .Flags = .{},
                        .u = .{
                            .Transition = .{
                                .pResource = gctx.lookupResource(barrier.resource).?,
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
                gctx.cmdlist.ResourceBarrier(num_valid_barriers, &d3d12_barriers);
            }
            gctx.transition_resource_barriers.clearRetainingCapacity();
        }
    }

    pub fn addTransitionBarrier(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        state_after: d3d12.RESOURCE_STATES,
    ) void {
        var resource = gctx.resource_pool.lookupResource(handle);
        if (resource == null)
            return;

        if (@bitCast(u32, state_after) != @bitCast(u32, resource.?.state)) {
            if (gctx.transition_resource_barriers.items.len == max_num_buffered_resource_barriers)
                gctx.flushResourceBarriers();

            gctx.transition_resource_barriers.appendAssumeCapacity(.{
                .resource = handle,
                .state_before = resource.?.state,
                .state_after = state_after,
            });
            resource.?.state = state_after;
        }
    }

    pub fn createGraphicsShaderPipeline(
        gctx: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        return createGraphicsShaderPipelineVsGsPs(gctx, arena, pso_desc, vs_cso_path, null, ps_cso_path);
    }

    pub fn createGraphicsShaderPipelineVsGsPs(
        gctx: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        vs_cso_path: ?[]const u8,
        gs_cso_path: ?[]const u8,
        ps_cso_path: ?[]const u8,
    ) PipelineHandle {
        return createGraphicsShaderPipelineRsVsGsPs(
            gctx,
            arena,
            pso_desc,
            null,
            vs_cso_path,
            gs_cso_path,
            ps_cso_path,
        );
    }

    pub fn createGraphicsShaderPipelineRsVsGsPs(
        gctx: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
        root_signature: ?*d3d12.IRootSignature,
        vs_cso_relpath: ?[]const u8,
        gs_cso_relpath: ?[]const u8,
        ps_cso_relpath: ?[]const u8,
    ) PipelineHandle {
        const self_exe_dir_path = std.fs.selfExeDirPathAlloc(arena) catch unreachable;

        if (vs_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const vs_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
            defer vs_file.close();
            const vs_code = vs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.VS = .{ .pShaderBytecode = vs_code.ptr, .BytecodeLength = vs_code.len };
        } else {
            assert(pso_desc.VS.pShaderBytecode != null);
        }

        if (gs_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const gs_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
            defer gs_file.close();
            const gs_code = gs_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.GS = .{ .pShaderBytecode = gs_code.ptr, .BytecodeLength = gs_code.len };
        }

        if (ps_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const ps_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
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

        if (gctx.pipeline_pool.map.contains(hash)) {
            std.log.info("[graphics] Graphics pipeline cache hit detected.", .{});
            const handle = gctx.pipeline_pool.map.getEntry(hash).?.value_ptr.*;
            return handle;
        }

        const rs = blk: {
            if (root_signature) |rs| {
                break :blk rs;
            } else {
                var rs: *d3d12.IRootSignature = undefined;
                hrPanicOnFail(gctx.device.CreateRootSignature(
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
            hrPanicOnFail(gctx.device.CreateGraphicsPipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, rs, .Graphics, hash);
    }

    pub fn createMeshShaderPipeline(
        gctx: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.MESH_SHADER_PIPELINE_STATE_DESC,
        as_cso_relpath: ?[]const u8,
        ms_cso_relpath: ?[]const u8,
        ps_cso_relpath: ?[]const u8,
    ) PipelineHandle {
        const self_exe_dir_path = std.fs.selfExeDirPathAlloc(arena) catch unreachable;

        if (as_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const as_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
            defer as_file.close();
            const as_code = as_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.AS = .{ .pShaderBytecode = as_code.ptr, .BytecodeLength = as_code.len };
        }

        if (ms_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const ms_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
            defer ms_file.close();
            const ms_code = ms_file.reader().readAllAlloc(arena, 256 * 1024) catch unreachable;
            pso_desc.MS = .{ .pShaderBytecode = ms_code.ptr, .BytecodeLength = ms_code.len };
        } else {
            assert(pso_desc.MS.pShaderBytecode != null);
        }

        if (ps_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{ self_exe_dir_path, relpath }) catch unreachable;
            const ps_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
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

        if (gctx.pipeline_pool.map.contains(hash)) {
            std.log.info("[graphics] Mesh shader pipeline cache hit detected.", .{});
            const handle = gctx.pipeline_pool.map.getEntry(hash).?.value_ptr.*;
            return handle;
        }

        const rs = blk: {
            var rs: *d3d12.IRootSignature = undefined;
            hrPanicOnFail(gctx.device.CreateRootSignature(
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
            hrPanicOnFail(gctx.device.CreatePipelineState(
                &d3d12.PIPELINE_STATE_STREAM_DESC{
                    .SizeInBytes = @sizeOf(@TypeOf(stream)),
                    .pPipelineStateSubobjectStream = &stream,
                },
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, rs, .Graphics, hash);
    }

    pub fn createComputeShaderPipeline(
        gctx: *GraphicsContext,
        arena: std.mem.Allocator,
        pso_desc: *d3d12.COMPUTE_PIPELINE_STATE_DESC,
        cs_cso_relpath: ?[]const u8,
    ) PipelineHandle {
        if (cs_cso_relpath) |relpath| {
            const abspath = std.fs.path.join(arena, &.{
                std.fs.selfExeDirPathAlloc(arena) catch unreachable,
                relpath,
            }) catch unreachable;
            const cs_file = std.fs.openFileAbsolute(abspath, .{}) catch unreachable;
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

        if (gctx.pipeline_pool.map.contains(hash)) {
            std.log.info("[graphics] Compute pipeline hit detected.", .{});
            const handle = gctx.pipeline_pool.map.getEntry(hash).?.value_ptr.*;
            return handle;
        }

        const rs = blk: {
            var rs: *d3d12.IRootSignature = undefined;
            hrPanicOnFail(gctx.device.CreateRootSignature(
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
            hrPanicOnFail(gctx.device.CreateComputePipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @ptrCast(*?*anyopaque, &pso),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, rs, .Compute, hash);
    }

    pub fn setCurrentPipeline(gctx: *GraphicsContext, pipeline_handle: PipelineHandle) void {
        assert(gctx.is_cmdlist_opened);

        const pipeline = gctx.pipeline_pool.lookupPipeline(pipeline_handle);
        if (pipeline == null)
            return;

        if (pipeline_handle.index == gctx.current_pipeline.index and
            pipeline_handle.generation == gctx.current_pipeline.generation)
        {
            return;
        }

        gctx.cmdlist.SetPipelineState(pipeline.?.pso.?);
        switch (pipeline.?.ptype.?) {
            .Graphics => gctx.cmdlist.SetGraphicsRootSignature(pipeline.?.rs.?),
            .Compute => gctx.cmdlist.SetComputeRootSignature(pipeline.?.rs.?),
        }

        gctx.current_pipeline = pipeline_handle;
    }

    pub fn destroyPipeline(gctx: *GraphicsContext, handle: PipelineHandle) void {
        gctx.pipeline_pool.destroyPipeline(handle);
    }

    pub fn allocateUploadMemory(
        gctx: *GraphicsContext,
        comptime T: type,
        num_elements: u32,
    ) struct { cpu_slice: []T, gpu_base: d3d12.GPU_VIRTUAL_ADDRESS } {
        assert(num_elements > 0);
        const size = num_elements * @sizeOf(T);
        var memory = gctx.upload_memory_heaps[gctx.frame_index].allocate(size);
        if (memory.cpu_slice == null or memory.gpu_base == null) {
            std.log.info(
                "[graphics] Upload memory exhausted - waiting for a GPU... (cmdlist state is lost).",
                .{},
            );

            gctx.finishGpuCommands();

            memory = gctx.upload_memory_heaps[gctx.frame_index].allocate(size);
        }
        return .{
            .cpu_slice = std.mem.bytesAsSlice(T, @alignCast(@alignOf(T), memory.cpu_slice.?)),
            .gpu_base = memory.gpu_base.?,
        };
    }

    pub fn allocateUploadBufferRegion(
        gctx: *GraphicsContext,
        comptime T: type,
        num_elements: u32,
    ) struct { cpu_slice: []T, buffer: *d3d12.IResource, buffer_offset: u64 } {
        assert(num_elements > 0);
        const size = num_elements * @sizeOf(T);
        const memory = gctx.allocateUploadMemory(T, num_elements);
        const aligned_size = (size + (GpuMemoryHeap.alloc_alignment - 1)) & ~(GpuMemoryHeap.alloc_alignment - 1);
        return .{
            .cpu_slice = memory.cpu_slice,
            .buffer = gctx.upload_memory_heaps[gctx.frame_index].heap,
            .buffer_offset = gctx.upload_memory_heaps[gctx.frame_index].size - aligned_size,
        };
    }

    pub fn allocateCpuDescriptors(
        gctx: *GraphicsContext,
        dtype: d3d12.DESCRIPTOR_HEAP_TYPE,
        num: u32,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        assert(num > 0);
        switch (dtype) {
            .CBV_SRV_UAV => {
                assert(gctx.cbv_srv_uav_cpu_heap.size_temp == 0);
                return gctx.cbv_srv_uav_cpu_heap.allocateDescriptors(num).cpu_handle;
            },
            .RTV => {
                assert(gctx.rtv_heap.size_temp == 0);
                return gctx.rtv_heap.allocateDescriptors(num).cpu_handle;
            },
            .DSV => {
                assert(gctx.dsv_heap.size_temp == 0);
                return gctx.dsv_heap.allocateDescriptors(num).cpu_handle;
            },
            .SAMPLER => unreachable,
        }
    }

    pub fn allocateTempCpuDescriptors(
        gctx: *GraphicsContext,
        dtype: d3d12.DESCRIPTOR_HEAP_TYPE,
        num: u32,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        assert(num > 0);
        var dheap = switch (dtype) {
            .CBV_SRV_UAV => &gctx.cbv_srv_uav_cpu_heap,
            .RTV => &gctx.rtv_heap,
            .DSV => &gctx.dsv_heap,
            .SAMPLER => unreachable,
        };
        const handle = dheap.allocateDescriptors(num).cpu_handle;
        dheap.size_temp += num;
        return handle;
    }

    pub fn deallocateAllTempCpuDescriptors(
        gctx: *GraphicsContext,
        dtype: d3d12.DESCRIPTOR_HEAP_TYPE,
    ) void {
        var dheap = switch (dtype) {
            .CBV_SRV_UAV => &gctx.cbv_srv_uav_cpu_heap,
            .RTV => &gctx.rtv_heap,
            .DSV => &gctx.dsv_heap,
            .SAMPLER => unreachable,
        };
        assert(dheap.size_temp > 0);
        assert(dheap.size_temp <= dheap.size);
        dheap.size -= dheap.size_temp;
        dheap.size_temp = 0;
    }

    pub inline fn allocateGpuDescriptors(gctx: *GraphicsContext, num_descriptors: u32) Descriptor {
        // Allocate non-persistent descriptors
        return gctx.cbv_srv_uav_gpu_heaps[gctx.frame_index + 1].allocateDescriptors(num_descriptors);
    }

    pub fn allocatePersistentGpuDescriptors(
        gctx: *GraphicsContext,
        num_descriptors: u32,
    ) PersistentDescriptor {
        // Allocate descriptors from persistent heap (heap 0)
        const index = gctx.cbv_srv_uav_gpu_heaps[0].size;
        const base = gctx.cbv_srv_uav_gpu_heaps[0].allocateDescriptors(num_descriptors);
        return .{
            .cpu_handle = base.cpu_handle,
            .gpu_handle = base.gpu_handle,
            .index = index,
        };
    }

    pub fn copyDescriptorsToGpuHeap(
        gctx: *GraphicsContext,
        num: u32,
        src_base_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    ) d3d12.GPU_DESCRIPTOR_HANDLE {
        const base = gctx.allocateGpuDescriptors(num);
        gctx.device.CopyDescriptorsSimple(num, base.cpu_handle, src_base_handle, .CBV_SRV_UAV);
        return base.gpu_handle;
    }

    pub fn updateTex2dSubresource(
        gctx: *GraphicsContext,
        texture: ResourceHandle,
        subresource: u32,
        data: []const u8,
        row_pitch: u32,
    ) void {
        assert(gctx.is_cmdlist_opened);
        const resource = gctx.resource_pool.lookupResource(texture);
        if (resource == null)
            return;

        assert(resource.?.desc.Dimension == .TEXTURE2D);

        var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
        var required_size: u64 = undefined;
        gctx.device.GetCopyableFootprints(
            &resource.?.desc,
            subresource,
            layout.len,
            0,
            &layout,
            null,
            null,
            &required_size,
        );

        const upload = gctx.allocateUploadBufferRegion(u8, @intCast(u32, required_size));
        layout[0].Offset = upload.buffer_offset;

        const pixel_size = resource.?.desc.Format.pixelSizeInBytes();
        var y: u32 = 0;
        while (y < layout[0].Footprint.Height) : (y += 1) {
            var x: u32 = 0;
            while (x < layout[0].Footprint.Width * pixel_size) : (x += 1) {
                upload.cpu_slice[y * layout[0].Footprint.RowPitch + x] = data[y * row_pitch + x];
            }
        }

        gctx.addTransitionBarrier(texture, .{ .COPY_DEST = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.CopyTextureRegion(&.{
            .pResource = gctx.lookupResource(texture).?,
            .Type = .SUBRESOURCE_INDEX,
            .u = .{ .SubresourceIndex = subresource },
        }, 0, 0, 0, &.{
            .pResource = upload.buffer,
            .Type = .PLACED_FOOTPRINT,
            .u = .{ .PlacedFootprint = layout[0] },
        }, null);
    }

    pub fn createAndUploadTex2dFromFile(
        gctx: *GraphicsContext,
        relpath: []const u8,
        args: struct {
            num_mip_levels: u32 = 0,
            texture_flags: d3d12.RESOURCE_FLAGS = .{},
        },
    ) HResultError!ResourceHandle {
        assert(gctx.is_cmdlist_opened);

        var buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
        var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
        const allocator = fba.allocator();

        const abspath = std.fs.path.join(allocator, &.{
            std.fs.selfExeDirPathAlloc(allocator) catch unreachable,
            relpath,
        }) catch unreachable;

        var abspath_w: [std.os.windows.PATH_MAX_WIDE:0]u16 = undefined;
        abspath_w[std.unicode.utf8ToUtf16Le(abspath_w[0..], abspath) catch unreachable] = 0;

        const bmp_decoder = blk: {
            var maybe_bmp_decoder: ?*wic.IBitmapDecoder = undefined;
            hrPanicOnFail(gctx.wic_factory.CreateDecoderFromFilename(
                &abspath_w,
                null,
                w32.GENERIC_READ,
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
            var pixel_format: w32.GUID = undefined;
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
            hrPanicOnFail(gctx.wic_factory.CreateFormatConverter(&maybe_image_conv));
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
        const texture = try gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(dxgi_format, image_wh.w, image_wh.h, args.num_mip_levels);
                desc.Flags = args.texture_flags;
                break :blk desc;
            },
            .{ .COPY_DEST = true },
            null,
        );

        const desc = gctx.lookupResource(texture).?.GetDesc();

        var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
        var required_size: u64 = undefined;
        gctx.device.GetCopyableFootprints(&desc, 0, 1, 0, &layout, null, null, &required_size);

        const upload = gctx.allocateUploadBufferRegion(u8, @intCast(u32, required_size));
        layout[0].Offset = upload.buffer_offset;

        hrPanicOnFail(image_conv.CopyPixels(
            null,
            layout[0].Footprint.RowPitch,
            layout[0].Footprint.RowPitch * layout[0].Footprint.Height,
            upload.cpu_slice.ptr,
        ));

        gctx.cmdlist.CopyTextureRegion(&d3d12.TEXTURE_COPY_LOCATION{
            .pResource = gctx.lookupResource(texture).?,
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
        gctx: *GraphicsContext,
        format: dxgi.FORMAT,
        comptime content_dir: []const u8,
    ) MipmapGenerator {
        var width: u32 = 2048 / 2;
        var height: u32 = 2048 / 2;

        var scratch_textures: [num_scratch_textures]ResourceHandle = undefined;
        for (scratch_textures) |_, texture_index| {
            scratch_textures[texture_index] = gctx.createCommittedResource(
                .DEFAULT,
                .{},
                &blk: {
                    var desc = d3d12.RESOURCE_DESC.initTex2d(format, width, height, 1);
                    desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                    break :blk desc;
                },
                .{ .UNORDERED_ACCESS = true },
                null,
            ) catch |err| hrPanic(err);
            width /= 2;
            height /= 2;
        }

        const base_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, num_scratch_textures);
        var cpu_handle = base_uav;
        for (scratch_textures) |_, texture_index| {
            gctx.device.CreateUnorderedAccessView(
                gctx.lookupResource(scratch_textures[texture_index]).?,
                null,
                null,
                cpu_handle,
            );
            cpu_handle.ptr += gctx.cbv_srv_uav_cpu_heap.descriptor_size;
        }

        var desc = d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault();
        const pipeline = gctx.createComputeShaderPipeline(
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

    pub fn deinit(mipgen: *MipmapGenerator, gctx: *GraphicsContext) void {
        for (mipgen.scratch_textures) |_, texture_index| {
            gctx.destroyResource(mipgen.scratch_textures[texture_index]);
        }
        mipgen.* = undefined;
    }

    pub fn generateMipmaps(
        mipgen: *MipmapGenerator,
        gctx: *GraphicsContext,
        texture_handle: ResourceHandle,
    ) void {
        if (!gctx.resource_pool.isResourceValid(texture_handle))
            return;

        const texture_desc = gctx.getResourceDesc(texture_handle);
        assert(mipgen.format == texture_desc.Format);
        assert(texture_desc.Width <= 2048 and texture_desc.Height <= 2048);
        assert(texture_desc.Width == texture_desc.Height);
        assert(texture_desc.MipLevels > 1);

        var array_slice: u32 = 0;
        while (array_slice < texture_desc.DepthOrArraySize) : (array_slice += 1) {
            const texture_srv = gctx.allocateTempCpuDescriptors(.CBV_SRV_UAV, 1);
            gctx.device.CreateShaderResourceView(
                gctx.lookupResource(texture_handle).?,
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
            const table_base = gctx.copyDescriptorsToGpuHeap(1, texture_srv);
            _ = gctx.copyDescriptorsToGpuHeap(num_scratch_textures, mipgen.base_uav);
            gctx.deallocateAllTempCpuDescriptors(.CBV_SRV_UAV);

            gctx.setCurrentPipeline(mipgen.pipeline);
            var total_num_mips: u32 = texture_desc.MipLevels - 1;
            var current_src_mip_level: u32 = 0;

            while (true) {
                for (mipgen.scratch_textures) |scratch_texture| {
                    gctx.addTransitionBarrier(scratch_texture, .{ .UNORDERED_ACCESS = true });
                }
                gctx.addTransitionBarrier(texture_handle, .{ .NON_PIXEL_SHADER_RESOURCE = true });
                gctx.flushResourceBarriers();

                const dispatch_num_mips = if (total_num_mips >= 4) 4 else total_num_mips;
                gctx.cmdlist.SetComputeRoot32BitConstant(0, current_src_mip_level, 0);
                gctx.cmdlist.SetComputeRoot32BitConstant(0, dispatch_num_mips, 1);
                gctx.cmdlist.SetComputeRootDescriptorTable(1, table_base);
                const num_groups_x = std.math.max(
                    @intCast(u32, texture_desc.Width) >> @intCast(u5, 3 + current_src_mip_level),
                    1,
                );
                const num_groups_y = std.math.max(
                    texture_desc.Height >> @intCast(u5, 3 + current_src_mip_level),
                    1,
                );
                gctx.cmdlist.Dispatch(num_groups_x, num_groups_y, 1);

                for (mipgen.scratch_textures) |scratch_texture| {
                    gctx.addTransitionBarrier(scratch_texture, .{ .COPY_SOURCE = true });
                }
                gctx.addTransitionBarrier(texture_handle, .{ .COPY_DEST = true });
                gctx.flushResourceBarriers();

                var mip_index: u32 = 0;
                while (mip_index < dispatch_num_mips) : (mip_index += 1) {
                    const dst = d3d12.TEXTURE_COPY_LOCATION{
                        .pResource = gctx.lookupResource(texture_handle).?,
                        .Type = .SUBRESOURCE_INDEX,
                        .u = .{ .SubresourceIndex = mip_index + 1 + current_src_mip_level +
                            array_slice * texture_desc.MipLevels },
                    };
                    const src = d3d12.TEXTURE_COPY_LOCATION{
                        .pResource = gctx.lookupResource(mipgen.scratch_textures[mip_index]).?,
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
                    gctx.cmdlist.CopyTextureRegion(&dst, 0, 0, 0, &src, &box);
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
    index: u16 align(4) = 0,
    generation: u16 = 0,
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

    fn init(allocator: std.mem.Allocator) ResourcePool {
        return .{
            .resources = blk: {
                var resources = allocator.alloc(
                    Resource,
                    max_num_resources + 1,
                ) catch unreachable;
                for (resources) |*res| {
                    res.* = .{
                        .raw = null,
                        .state = d3d12.RESOURCE_STATES.COMMON,
                        .desc = d3d12.RESOURCE_DESC.initBuffer(0),
                    };
                }
                break :blk resources;
            },
            .generations = blk: {
                var generations = allocator.alloc(
                    u16,
                    max_num_resources + 1,
                ) catch unreachable;
                for (generations) |*gen| gen.* = 0;
                break :blk generations;
            },
        };
    }

    fn deinit(pool: *ResourcePool, allocator: std.mem.Allocator) void {
        for (pool.resources) |resource| {
            if (resource.raw != null)
                _ = resource.raw.?.Release();
        }
        allocator.free(pool.resources);
        allocator.free(pool.generations);
        pool.* = undefined;
    }

    fn addResource(
        pool: ResourcePool,
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

    fn destroyResource(pool: ResourcePool, handle: ResourceHandle) void {
        var resource = pool.lookupResource(handle);
        if (resource == null)
            return;

        _ = resource.?.raw.?.Release();
        resource.?.* = .{
            .raw = null,
            .state = d3d12.RESOURCE_STATES.COMMON,
            .desc = d3d12.RESOURCE_DESC.initBuffer(0),
        };
    }

    fn isResourceValid(pool: ResourcePool, handle: ResourceHandle) bool {
        return handle.index > 0 and
            handle.index <= max_num_resources and
            handle.generation > 0 and
            handle.generation == pool.generations[handle.index] and
            pool.resources[handle.index].raw != null;
    }

    fn lookupResource(pool: ResourcePool, handle: ResourceHandle) ?*Resource {
        if (pool.isResourceValid(handle)) {
            return &pool.resources[handle.index];
        }
        return null;
    }
};

pub const PipelineHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
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
    map: std.AutoHashMapUnmanaged(u32, PipelineHandle),

    fn init(allocator: std.mem.Allocator) PipelinePool {
        return .{
            .pipelines = blk: {
                var pipelines = allocator.alloc(
                    Pipeline,
                    max_num_pipelines + 1,
                ) catch unreachable;
                for (pipelines) |*pipeline| {
                    pipeline.* = .{ .pso = null, .rs = null, .ptype = null };
                }
                break :blk pipelines;
            },
            .generations = blk: {
                var generations = allocator.alloc(
                    u16,
                    max_num_pipelines + 1,
                ) catch unreachable;
                for (generations) |*gen| gen.* = 0;
                break :blk generations;
            },
            .map = blk: {
                var hm: std.AutoHashMapUnmanaged(u32, PipelineHandle) = .{};
                hm.ensureTotalCapacity(
                    allocator,
                    max_num_pipelines,
                ) catch unreachable;
                break :blk hm;
            },
        };
    }

    fn deinit(pool: *PipelinePool, allocator: std.mem.Allocator) void {
        for (pool.pipelines) |pipeline| {
            if (pipeline.pso != null)
                _ = pipeline.pso.?.Release();
            if (pipeline.rs != null)
                _ = pipeline.rs.?.Release();
        }
        pool.map.deinit(allocator);
        allocator.free(pool.pipelines);
        allocator.free(pool.generations);
        pool.* = undefined;
    }

    fn addPipeline(
        pool: *PipelinePool,
        pso: *d3d12.IPipelineState,
        rs: *d3d12.IRootSignature,
        ptype: PipelineType,
        hash: u32,
    ) PipelineHandle {
        var slot_idx: u32 = 1;
        while (slot_idx <= max_num_pipelines) : (slot_idx += 1) {
            if (pool.pipelines[slot_idx].pso == null)
                break;
        }
        assert(slot_idx <= max_num_pipelines);

        pool.pipelines[slot_idx] = .{ .pso = pso, .rs = rs, .ptype = ptype };
        const handle = PipelineHandle{
            .index = @intCast(u16, slot_idx),
            .generation = blk: {
                pool.generations[slot_idx] += 1;
                break :blk pool.generations[slot_idx];
            },
        };
        pool.map.putAssumeCapacity(hash, handle);
        return handle;
    }

    pub fn destroyPipeline(pool: *PipelinePool, handle: PipelineHandle) void {
        var pipeline = pool.lookupPipeline(handle);
        if (pipeline == null)
            return;

        _ = pipeline.?.pso.?.Release();
        _ = pipeline.?.rs.?.Release();

        const hash_to_delete = blk: {
            var it = pool.map.iterator();
            while (it.next()) |kv| {
                if (kv.value_ptr.*.index == handle.index and
                    kv.value_ptr.*.generation == handle.generation)
                {
                    break :blk kv.key_ptr.*;
                }
            }
            unreachable;
        };
        _ = pool.map.remove(hash_to_delete);

        pipeline.?.* = .{
            .pso = null,
            .rs = null,
            .ptype = null,
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

    fn lookupPipeline(pool: PipelinePool, handle: PipelineHandle) ?*Pipeline {
        if (pool.isPipelineValid(handle)) {
            return &pool.pipelines[handle.index];
        }
        return null;
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
                    if (flags.SHADER_VISIBLE)
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
                .{},
                &d3d12.RESOURCE_DESC.initBuffer(capacity),
                d3d12.RESOURCE_STATES.GENERIC_READ,
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
