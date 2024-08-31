comptime {
    std.testing.refAllDecls(@This());
}

const std = @import("std");
const assert = std.debug.assert;

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const dwrite = zwindows.dwrite;
const dxgi = zwindows.dxgi;
const d3d11 = zwindows.d3d11;
const d3d12 = zwindows.d3d12;
const d3d12d = zwindows.d3d12d;
const d3d = zwindows.d3d;
const d2d1 = zwindows.d2d1;
const d3d11on12 = zwindows.d3d11on12;
const dds_loader = zwindows.dds_loader;
const wic = zwindows.wic;
const HResultError = zwindows.HResultError;
const hrPanic = zwindows.hrPanic;
const hrPanicOnFail = zwindows.hrPanicOnFail;
const hrErrorOnFail = zwindows.hrErrorOnFail;

const options = @import("options");
const enable_debug_layer = options.zd3d12_debug_layer;
const enable_gbv = options.zd3d12_gbv;

// TODO(mziulek): For now, we always transition *all* subresources.
const TransitionResourceBarrier = struct {
    state_before: d3d12.RESOURCE_STATES,
    state_after: d3d12.RESOURCE_STATES,
    resource: ResourceHandle,
};

const default_upload_heap_capacity: u32 = 32 * 1024 * 1024;
const num_swapbuffers = 4;

const SwapchainBuffer = struct {
    resource: ResourceHandle,
    descriptor: d3d12.CPU_DESCRIPTOR_HANDLE,
};

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

pub fn ConstantBufferHandle(comptime T: type) type {
    return struct {
        resource: ResourceHandle,
        view_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
        ptr: *T,
    };
}
pub const VerticesHandle = struct {
    resource: ResourceHandle,
    view: d3d12.VERTEX_BUFFER_VIEW,

    fn init(comptime T: type, gctx: *GraphicsContext, vertices_length: usize) !VerticesHandle {
        switch (@typeInfo(T)) {
            .Struct => |s| {
                if (s.layout != .@"extern") {
                    @compileError(@typeName(T) ++ " must be extern");
                }
            },
            else => {},
        }

        const buffer_length: windows.UINT = @intCast(vertices_length * @sizeOf(T));
        const resource_handle = try gctx.createCommittedResource(
            .UPLOAD,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(buffer_length),
            d3d12.RESOURCE_STATES.GENERIC_READ,
            null,
        );

        return .{
            .resource = resource_handle,
            .view = .{
                .BufferLocation = gctx.lookupResource(resource_handle).?.GetGPUVirtualAddress(),
                .StrideInBytes = @sizeOf(T),
                .SizeInBytes = buffer_length,
            },
        };
    }
};
pub const VertexIndicesHandle = struct {
    resource: ResourceHandle,
    view: d3d12.INDEX_BUFFER_VIEW,
};

pub const GraphicsContext = struct {
    pub const max_num_buffered_frames = 2;
    const num_rtv_descriptors = 128;
    const num_dsv_descriptors = 128;
    const num_cbv_srv_uav_cpu_descriptors = 16 * 1024;
    const num_cbv_srv_uav_gpu_descriptors = 8 * 1024;
    const max_num_buffered_resource_barriers = 16;

    device: *d3d12.IDevice9,
    debug_device: ?*d3d12.IDebugDevice,
    adapter: *dxgi.IAdapter3,
    cmdqueue: *d3d12.ICommandQueue,
    cmdlist: *d3d12.IGraphicsCommandList6,
    cmdallocs: [max_num_buffered_frames]*d3d12.ICommandAllocator,
    swapchain: *dxgi.ISwapChain3,
    swapchain_buffers: [num_swapbuffers]SwapchainBuffer,
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
    frame_fence_event: windows.HANDLE,
    frame_fence_counter: u64,
    frame_index: u32,
    back_buffer_index: u32,
    window: windows.HWND,
    is_cmdlist_opened: bool,
    d2d: ?D2dState,
    wic_factory: *wic.IImagingFactory,
    present_flags: dxgi.PRESENT_FLAG,
    present_interval: windows.UINT,

    pub fn init(args: struct {
        allocator: std.mem.Allocator,
        window: windows.HWND,
        upload_heap_capacity: u32 = default_upload_heap_capacity,
    }) GraphicsContext {
        const wic_factory = blk: {
            var wic_factory: *wic.IImagingFactory = undefined;
            hrPanicOnFail(windows.CoCreateInstance(
                &wic.CLSID_ImagingFactory,
                null,
                windows.CLSCTX_INPROC_SERVER,
                &wic.IID_IImagingFactory,
                @as(*?*anyopaque, @ptrCast(&wic_factory)),
            ));
            break :blk wic_factory;
        };

        const factory = blk: {
            var factory: *dxgi.IFactory6 = undefined;
            hrPanicOnFail(dxgi.CreateDXGIFactory2(
                if (enable_debug_layer) dxgi.CREATE_FACTORY_DEBUG else 0,
                &dxgi.IID_IFactory6,
                @as(*?*anyopaque, @ptrCast(&factory)),
            ));
            break :blk factory;
        };
        defer _ = factory.Release();

        var present_flags: dxgi.PRESENT_FLAG = .{};
        const present_interval: windows.UINT = 0;
        {
            var allow_tearing: windows.BOOL = windows.FALSE;
            const hr = factory.CheckFeatureSupport(
                .PRESENT_ALLOW_TEARING,
                &allow_tearing,
                @sizeOf(@TypeOf(allow_tearing)),
            );

            if (hr == windows.S_OK and allow_tearing == windows.TRUE) {
                present_flags.ALLOW_TEARING = true;
            }
        }

        if (enable_debug_layer) {
            var maybe_debug: ?*d3d12d.IDebug1 = null;
            _ = d3d12.GetDebugInterface(&d3d12d.IID_IDebug1, @as(*?*anyopaque, @ptrCast(&maybe_debug)));
            if (maybe_debug) |debug| {
                debug.EnableDebugLayer();
                if (enable_gbv) {
                    debug.SetEnableGPUBasedValidation(windows.TRUE);
                }
                _ = debug.Release();
            }
        }

        const suitable_adapter = blk: {
            var adapter: ?*dxgi.IAdapter3 = null;
            var adapter_index: u32 = 0;
            var optional_adapter3: ?*dxgi.IAdapter3 = null;

            while (factory.EnumAdapterByGpuPreference(
                adapter_index,
                .HIGH_PERFORMANCE,
                &dxgi.IID_IAdapter3,
                &optional_adapter3,
            ) == windows.S_OK) {
                adapter_index += 1;
                if (optional_adapter3) |adapter3| {
                    var adapter2_desc: dxgi.ADAPTER_DESC2 = undefined;
                    if (adapter3.GetDesc2(&adapter2_desc) == windows.S_OK) {
                        if (adapter2_desc.Flags.SOFTWARE) {
                            // Don't select the Basic Render Driver adapter.
                            continue;
                        }

                        const hr = d3d12.CreateDevice(
                            @as(*windows.IUnknown, @ptrCast(adapter3)),
                            .@"11_1",
                            &d3d12.IID_IDevice9,
                            null,
                        );
                        if (hr == windows.S_OK or hr == windows.S_FALSE) {
                            adapter = adapter3;
                            break;
                        }
                    }
                }
            }
            break :blk adapter;
        };

        const device = blk: {
            var device: *d3d12.IDevice9 = undefined;
            const hr = d3d12.CreateDevice(
                if (suitable_adapter) |adapter| @as(*windows.IUnknown, @ptrCast(adapter)) else null,
                .@"11_1",
                &d3d12.IID_IDevice9,
                @as(*?*anyopaque, @ptrCast(&device)),
            );

            if (hr != windows.S_OK) {
                _ = windows.MessageBoxA(
                    args.window,
                    "Failed to create Direct3D 12 Device. This applications requires graphics card " ++
                        "with DirectX 12 Feature Level 11.1 support.",
                    "Your graphics card driver may be old",
                    windows.MB_OK | windows.MB_ICONERROR,
                );
                windows.ExitProcess(0);
            }
            break :blk device;
        };

        var debug_device: ?*d3d12.IDebugDevice = null;
        if (enable_debug_layer) {
            hrPanicOnFail(device.QueryInterface(
                &d3d12.IID_IDebugDevice,
                @as(*?*anyopaque, @ptrCast(&debug_device)),
            ));

            _ = debug_device.?.SetFeatureMask(.{ .CONSERVATIVE_RESOURCE_STATE_TRACKING = true });
        }

        // Check for Shader Model 6.6 support.
        {
            var data: d3d12.FEATURE_DATA_SHADER_MODEL = .{ .HighestShaderModel = .@"6_7" };
            const hr = device.CheckFeatureSupport(.SHADER_MODEL, &data, @sizeOf(d3d12.FEATURE_DATA_SHADER_MODEL));
            if (hr != windows.S_OK or @intFromEnum(data.HighestShaderModel) < @intFromEnum(d3d12.SHADER_MODEL.@"6_6")) {
                _ = windows.MessageBoxA(
                    args.window,
                    "This applications requires graphics card driver that supports Shader Model 6.6. " ++
                        "Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    windows.MB_OK | windows.MB_ICONERROR,
                );
                windows.ExitProcess(0);
            }
        }

        // Check for Resource Binding Tier 3 support.
        {
            var data: d3d12.FEATURE_DATA_D3D12_OPTIONS = std.mem.zeroes(d3d12.FEATURE_DATA_D3D12_OPTIONS);
            const hr = device.CheckFeatureSupport(.OPTIONS, &data, @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS));
            if (hr != windows.S_OK or
                @intFromEnum(data.ResourceBindingTier) < @intFromEnum(d3d12.RESOURCE_BINDING_TIER.TIER_3))
            {
                _ = windows.MessageBoxA(
                    args.window,
                    "This applications requires graphics card driver that supports Resource Binding Tier 3. " ++
                        "Please update your graphics driver and try again.",
                    "Your graphics card driver may be old",
                    windows.MB_OK | windows.MB_ICONERROR,
                );
                windows.ExitProcess(0);
            }
        }

        const cmdqueue = blk: {
            var cmdqueue: *d3d12.ICommandQueue = undefined;
            hrPanicOnFail(device.CreateCommandQueue(&.{
                .Type = .DIRECT,
                .Priority = @intFromEnum(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
                .Flags = .{},
                .NodeMask = 0,
            }, &d3d12.IID_ICommandQueue, @as(*?*anyopaque, @ptrCast(&cmdqueue))));
            break :blk cmdqueue;
        };

        var rect: windows.RECT = undefined;
        _ = windows.GetClientRect(args.window, &rect);
        const viewport_width = @as(u32, @intCast(rect.right - rect.left));
        const viewport_height = @as(u32, @intCast(rect.bottom - rect.top));

        const swapchain = blk: {
            var desc = dxgi.SWAP_CHAIN_DESC{
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
                .OutputWindow = args.window,
                .Windowed = windows.TRUE,
                .SwapEffect = .FLIP_DISCARD,
                .Flags = .{ .ALLOW_TEARING = present_flags.ALLOW_TEARING },
            };
            var swapchain: *dxgi.ISwapChain = undefined;
            hrPanicOnFail(factory.CreateSwapChain(
                @as(*windows.IUnknown, @ptrCast(cmdqueue)),
                &desc,
                @as(*?*dxgi.ISwapChain, @ptrCast(&swapchain)),
            ));
            defer _ = swapchain.Release();

            var swapchain3: *dxgi.ISwapChain3 = undefined;
            hrPanicOnFail(swapchain.QueryInterface(
                &dxgi.IID_ISwapChain3,
                @as(*?*anyopaque, @ptrCast(&swapchain3)),
            ));
            break :blk swapchain3;
        };

        const resource_pool = ResourcePool.init(args.allocator);
        const pipeline_pool = PipelinePool.init(args.allocator);

        const rtv_heap = DescriptorHeap.init(device, num_rtv_descriptors, .RTV, .{});
        const dsv_heap = DescriptorHeap.init(device, num_dsv_descriptors, .DSV, .{});

        const cbv_srv_uav_cpu_heap = DescriptorHeap.init(
            device,
            num_cbv_srv_uav_cpu_descriptors,
            .CBV_SRV_UAV,
            .{},
        );

        var cbv_srv_uav_gpu_heaps: [max_num_buffered_frames + 1]DescriptorHeap = undefined;
        for (cbv_srv_uav_gpu_heaps, 0..) |_, heap_index| {
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
        for (upload_heaps, 0..) |_, heap_index| {
            upload_heaps[heap_index] = GpuMemoryHeap.init(device, args.upload_heap_capacity, .UPLOAD);
        }

        // Disable ALT + ENTER
        hrPanicOnFail(factory.MakeWindowAssociation(args.window, .{ .NO_WINDOW_CHANGES = true }));

        const frame_fence = blk: {
            var frame_fence: *d3d12.IFence = undefined;
            hrPanicOnFail(device.CreateFence(0, .{}, &d3d12.IID_IFence, @as(*?*anyopaque, @ptrCast(&frame_fence))));
            break :blk frame_fence;
        };

        const frame_fence_event = windows.CreateEventExA(
            null,
            "frame_fence_event",
            0,
            windows.EVENT_ALL_ACCESS,
        ).?;

        const cmdallocs = blk: {
            var cmdallocs: [max_num_buffered_frames]*d3d12.ICommandAllocator = undefined;
            for (cmdallocs, 0..) |_, cmdalloc_index| {
                hrPanicOnFail(device.CreateCommandAllocator(
                    .DIRECT,
                    &d3d12.IID_ICommandAllocator,
                    @as(*?*anyopaque, @ptrCast(&cmdallocs[cmdalloc_index])),
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
                @as(*?*anyopaque, @ptrCast(&cmdlist)),
            ));
            break :blk cmdlist;
        };
        hrPanicOnFail(cmdlist.Close());
        const is_cmdlist_opened = false;

        var gctx = GraphicsContext{
            .device = device,
            .debug_device = debug_device,
            .adapter = suitable_adapter.?,
            .cmdqueue = cmdqueue,
            .cmdlist = cmdlist,
            .cmdallocs = cmdallocs,
            .swapchain = swapchain,
            .swapchain_buffers = undefined,
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
                args.allocator,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
            .viewport_width = viewport_width,
            .viewport_height = viewport_height,
            .frame_index = 0,
            .back_buffer_index = swapchain.GetCurrentBackBufferIndex(),
            .window = args.window,
            .is_cmdlist_opened = is_cmdlist_opened,
            .d2d = null,
            .wic_factory = wic_factory,
            .present_flags = present_flags,
            .present_interval = present_interval,
        };

        gctx.createSwapchainBuffers();
        gctx.createD2DResources();

        return gctx;
    }

    pub fn deinit(gctx: *GraphicsContext, allocator: std.mem.Allocator) void {
        _ = gctx.adapter.Release();
        gctx.finishGpuCommands();
        gctx.transition_resource_barriers.deinit(allocator);
        _ = windows.CloseHandle(gctx.frame_fence_event);
        gctx.resource_pool.deinit(allocator);
        gctx.pipeline_pool.deinit(allocator);
        gctx.rtv_heap.deinit();
        gctx.dsv_heap.deinit();
        gctx.cbv_srv_uav_cpu_heap.deinit();

        gctx.destroyD2DResources();

        for (&gctx.cbv_srv_uav_gpu_heaps) |*heap|
            heap.deinit();
        for (&gctx.upload_memory_heaps) |*heap|
            heap.deinit();

        _ = gctx.device.Release();
        _ = gctx.cmdqueue.Release();
        _ = gctx.swapchain.Release();
        _ = gctx.frame_fence.Release();
        _ = gctx.cmdlist.Release();
        for (gctx.cmdallocs) |cmdalloc|
            _ = cmdalloc.Release();
        _ = gctx.wic_factory.Release();

        if (gctx.debug_device) |debug_device| {
            hrPanicOnFail(debug_device.ReportLiveDeviceObjects(.{ .DETAIL = true, .IGNORE_INTERNAL = true }));
        }

        gctx.* = undefined;
    }

    pub fn resize(gctx: *GraphicsContext, width: u32, height: u32) void {
        if (gctx.viewport_width == width and gctx.viewport_height == height) {
            return;
        }

        if (gctx.viewport_width == 0 or gctx.viewport_height == 0) {
            return;
        }

        gctx.endFrame();

        gctx.destroyD2DResources();

        for (0..num_swapbuffers) |i| {
            gctx.resource_pool.destroyResource(gctx.swapchain_buffers[i].resource);
        }

        gctx.viewport_width = width;
        gctx.viewport_height = height;
        _ = gctx.swapchain.ResizeBuffers(
            num_swapbuffers,
            width,
            height,
            .R8G8B8A8_UNORM,
            .{ .ALLOW_TEARING = true },
        );

        gctx.back_buffer_index = gctx.swapchain.GetCurrentBackBufferIndex();

        gctx.createSwapchainBuffers();
        gctx.createD2DResources();
    }

    fn createSwapchainBuffers(gctx: *GraphicsContext) void {
        var swapbuffers: [num_swapbuffers]*d3d12.IResource = undefined;

        for (swapbuffers, 0..) |_, buffer_index| {
            hrPanicOnFail(gctx.swapchain.GetBuffer(
                @as(u32, @intCast(buffer_index)),
                &d3d12.IID_IResource,
                @as(*?*anyopaque, @ptrCast(&swapbuffers[buffer_index])),
            ));
            const descriptor = gctx.rtv_heap.allocateDescriptors(1).cpu_handle;
            gctx.device.CreateRenderTargetView(
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
                descriptor,
            );
            gctx.swapchain_buffers[buffer_index] = .{
                .resource = gctx.resource_pool.addResource(swapbuffers[buffer_index], d3d12.RESOURCE_STATES.PRESENT),
                .descriptor = descriptor,
            };
        }
    }

    fn createD2DResources(gctx: *GraphicsContext) void {
        const dx11 = blk: {
            var device11: *d3d11.IDevice = undefined;
            var device_context11: *d3d11.IDeviceContext = undefined;
            hrPanicOnFail(d3d11on12.D3D11On12CreateDevice(
                @as(*windows.IUnknown, @ptrCast(gctx.device)),
                .{ .DEBUG = enable_debug_layer, .BGRA_SUPPORT = true },
                null,
                0,
                &.{@as(*windows.IUnknown, @ptrCast(gctx.cmdqueue))},
                1,
                0,
                @as(*?*d3d11.IDevice, @ptrCast(&device11)),
                @as(*?*d3d11.IDeviceContext, @ptrCast(&device_context11)),
                null,
            ));
            break :blk .{ .device = device11, .device_context = device_context11 };
        };

        const device11on12 = blk: {
            var device11on12: *d3d11on12.IDevice2 = undefined;
            hrPanicOnFail(dx11.device.QueryInterface(
                &d3d11on12.IID_IDevice2,
                @as(*?*anyopaque, @ptrCast(&device11on12)),
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
                @as(*?*anyopaque, @ptrCast(&d2d_factory)),
            ));
            break :blk d2d_factory;
        };

        const dxgi_device = blk: {
            var dxgi_device: *dxgi.IDevice = undefined;
            hrPanicOnFail(device11on12.QueryInterface(
                &dxgi.IID_IDevice,
                @as(*?*anyopaque, @ptrCast(&dxgi_device)),
            ));
            break :blk dxgi_device;
        };
        defer _ = dxgi_device.Release();

        const d2d_device = blk: {
            var d2d_device: *d2d1.IDevice6 = undefined;
            hrPanicOnFail(d2d_factory.CreateDevice6(
                dxgi_device,
                @as(*?*d2d1.IDevice6, @ptrCast(&d2d_device)),
            ));
            break :blk d2d_device;
        };

        const d2d_device_context = blk: {
            var d2d_device_context: *d2d1.IDeviceContext6 = undefined;
            hrPanicOnFail(d2d_device.CreateDeviceContext6(
                d2d1.DEVICE_CONTEXT_OPTIONS_NONE,
                @as(*?*d2d1.IDeviceContext6, @ptrCast(&d2d_device_context)),
            ));
            break :blk d2d_device_context;
        };

        const dwrite_factory = blk: {
            var dwrite_factory: *dwrite.IFactory = undefined;
            hrPanicOnFail(dwrite.DWriteCreateFactory(
                .SHARED,
                &dwrite.IID_IFactory,
                @as(*?*anyopaque, @ptrCast(&dwrite_factory)),
            ));
            break :blk dwrite_factory;
        };

        gctx.d2d = .{
            .factory = d2d_factory,
            .device = d2d_device,
            .context = d2d_device_context,
            .device11on12 = device11on12,
            .device11 = dx11.device,
            .context11 = dx11.device_context,
            .swapbuffers11 = undefined,
            .targets = undefined,
            .dwrite_factory = dwrite_factory,
        };

        const swapbuffers11 = blk: {
            var swapbuffers11: [num_swapbuffers]*d3d11.IResource = undefined;
            for (swapbuffers11, 0..) |_, buffer_index| {
                hrPanicOnFail(gctx.d2d.?.device11on12.CreateWrappedResource(
                    @as(
                        *windows.IUnknown,
                        @ptrCast(gctx.resource_pool.lookupResource(gctx.swapchain_buffers[buffer_index].resource).?.raw.?),
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
                    @as(*?*anyopaque, @ptrCast(&swapbuffers11[buffer_index])),
                ));
            }
            break :blk swapbuffers11;
        };

        const d2d_targets = blk: {
            var d2d_targets: [num_swapbuffers]*d2d1.IBitmap1 = undefined;
            for (d2d_targets, 0..) |_, target_index| {
                const swapbuffer11 = swapbuffers11[target_index];

                var surface: *dxgi.ISurface = undefined;
                hrPanicOnFail(swapbuffer11.QueryInterface(
                    &dxgi.IID_ISurface,
                    @as(*?*anyopaque, @ptrCast(&surface)),
                ));
                defer _ = surface.Release();

                hrPanicOnFail(gctx.d2d.?.context.CreateBitmapFromDxgiSurface(
                    surface,
                    &d2d1.BITMAP_PROPERTIES1{
                        .pixelFormat = .{ .format = .R8G8B8A8_UNORM, .alphaMode = .PREMULTIPLIED },
                        .dpiX = 96.0,
                        .dpiY = 96.0,
                        .bitmapOptions = d2d1.BITMAP_OPTIONS_TARGET | d2d1.BITMAP_OPTIONS_CANNOT_DRAW,
                        .colorContext = null,
                    },
                    @as(*?*d2d1.IBitmap1, @ptrCast(&d2d_targets[target_index])),
                ));
            }
            break :blk d2d_targets;
        };

        gctx.d2d.?.swapbuffers11 = swapbuffers11;
        gctx.d2d.?.targets = d2d_targets;
    }

    pub fn destroyD2DResources(gctx: *GraphicsContext) void {
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

        gctx.d2d = null;
    }

    pub fn beginFrame(gctx: *GraphicsContext) void {
        assert(!gctx.is_cmdlist_opened);
        const cmdalloc = gctx.cmdallocs[gctx.frame_index];
        hrPanicOnFail(cmdalloc.Reset());
        hrPanicOnFail(gctx.cmdlist.Reset(cmdalloc, null));
        gctx.is_cmdlist_opened = true;
        gctx.cmdlist.SetDescriptorHeaps(
            1,
            &.{gctx.cbv_srv_uav_gpu_heaps[0].heap.?},
        );
        gctx.cmdlist.RSSetViewports(1, &.{
            .{
                .TopLeftX = 0.0,
                .TopLeftY = 0.0,
                .Width = @as(f32, @floatFromInt(gctx.viewport_width)),
                .Height = @as(f32, @floatFromInt(gctx.viewport_height)),
                .MinDepth = 0.0,
                .MaxDepth = 1.0,
            },
        });
        gctx.cmdlist.RSSetScissorRects(1, &.{
            .{
                .left = 0,
                .top = 0,
                .right = @as(c_long, @intCast(gctx.viewport_width)),
                .bottom = @as(c_long, @intCast(gctx.viewport_height)),
            },
        });
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
            windows.WaitForSingleObject(gctx.frame_fence_event, windows.INFINITE) catch {};
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
            &.{gctx.d2d.?.swapbuffers11[gctx.back_buffer_index]},
            1,
        );
        gctx.d2d.?.context.SetTarget(@as(*d2d1.IImage, @ptrCast(gctx.d2d.?.targets[gctx.back_buffer_index])));
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
                @as(*?*anyopaque, @ptrCast(&info_queue)),
            ));

            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(windows.TRUE);
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
            &.{gctx.d2d.?.swapbuffers11[gctx.back_buffer_index]},
            1,
        );
        gctx.d2d.?.context11.Flush();

        if (enable_debug_layer) {
            if (mute_d2d_completely) {
                info_queue.SetMuteDebugOutput(windows.FALSE);
            } else {
                info_queue.PopStorageFilter();
            }
            _ = info_queue.Release();
        }

        // Above calls will set back buffer state to PRESENT. We need to reflect this change
        // in 'resource_pool' by manually setting state.
        gctx.resource_pool.lookupResource(gctx.swapchain_buffers[gctx.back_buffer_index].resource).?.state =
            d3d12.RESOURCE_STATES.PRESENT;
    }

    fn flushGpuCommands(gctx: *GraphicsContext) void {
        if (gctx.is_cmdlist_opened) {
            gctx.flushResourceBarriers();
            hrPanicOnFail(gctx.cmdlist.Close());
            gctx.is_cmdlist_opened = false;
            gctx.cmdqueue.ExecuteCommandLists(
                1,
                &.{@as(*d3d12.ICommandList, @ptrCast(gctx.cmdlist))},
            );
        }
    }

    pub fn finishGpuCommands(gctx: *GraphicsContext) void {
        const was_cmdlist_opened = gctx.is_cmdlist_opened;
        gctx.flushGpuCommands();

        gctx.frame_fence_counter += 1;

        hrPanicOnFail(gctx.cmdqueue.Signal(gctx.frame_fence, gctx.frame_fence_counter));
        hrPanicOnFail(gctx.frame_fence.SetEventOnCompletion(gctx.frame_fence_counter, gctx.frame_fence_event));
        windows.WaitForSingleObject(gctx.frame_fence_event, windows.INFINITE) catch {};

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
            .resource_handle = gctx.swapchain_buffers[gctx.back_buffer_index].resource,
            .descriptor_handle = .{
                .ptr = gctx.swapchain_buffers[gctx.back_buffer_index].descriptor.ptr,
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

    pub fn checkFeatureSupport(gctx: *GraphicsContext, comptime feature: d3d12.FEATURE, data: anytype) HResultError!void {
        const Data = @TypeOf(data);
        const FeatureData = feature.Data();
        if (Data != FeatureData) {
            @compileError("expected " ++ @typeName(FeatureData) ++ " but was " ++ @typeName(Data));
        }
        try hrErrorOnFail(gctx.device.CheckFeatureSupport(feature, @constCast(@ptrCast(&data)), @sizeOf(FeatureData)));
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
                @as(*?*anyopaque, @ptrCast(&resource)),
            ));
            break :blk resource;
        };
        return gctx.resource_pool.addResource(resource, initial_state);
    }

    pub fn createConstantBuffer(
        gctx: *GraphicsContext,
        comptime T: type,
    ) HResultError!ConstantBufferHandle(T) {
        switch (@typeInfo(T)) {
            .Struct => |s| {
                if (s.layout != .@"extern") {
                    @compileError(@typeName(T) ++ " must be extern");
                }
            },
            else => {},
        }
        const buffer_length = buffer_length: {
            const buffer_length: windows.UINT = @sizeOf(T);
            break :buffer_length buffer_length + d3d12.CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - @mod(buffer_length, d3d12.CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
        };
        const resource_handle = try gctx.createCommittedResource(
            .UPLOAD,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(buffer_length),
            d3d12.RESOURCE_STATES.GENERIC_READ,
            null,
        );
        const resource = gctx.lookupResource(resource_handle).?;

        var buffer: [*]u8 = undefined;
        try hrErrorOnFail(resource.Map(
            0,
            &.{ .Begin = 0, .End = 0 },
            @ptrCast(&buffer),
        ));

        const view_handle = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateConstantBufferView(&.{
            .BufferLocation = resource.GetGPUVirtualAddress(),
            .SizeInBytes = buffer_length,
        }, view_handle);
        return .{
            .resource = resource_handle,
            .view_handle = view_handle,
            .ptr = @ptrCast(@alignCast(buffer)),
        };
    }

    pub fn uploadVertices(
        gctx: *GraphicsContext,
        comptime T: type,
        vertices: []const T,
    ) HResultError!VerticesHandle {
        const handle = try VerticesHandle.init(T, gctx, vertices.len);

        try gctx.writeVertices(T, handle, vertices);

        return handle;
    }

    pub fn uploadVertexIndices(
        gctx: *GraphicsContext,
        comptime T: type,
        vertex_indices: []const T,
    ) HResultError!VertexIndicesHandle {
        switch (T) {
            u16, u32 => {},
            else => @compileError(@typeName(T) ++ " is not a supported vertex index format"),
        }
        const buffer_length: windows.UINT = @intCast(vertex_indices.len * @sizeOf(T));
        const resource_handle = try gctx.createCommittedResource(
            .UPLOAD,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(buffer_length),
            d3d12.RESOURCE_STATES.GENERIC_READ,
            null,
        );

        try gctx.writeResource(T, resource_handle, vertex_indices);

        return .{
            .resource = resource_handle,
            .view = .{
                .BufferLocation = gctx.lookupResource(resource_handle).?.GetGPUVirtualAddress(),
                .Format = switch (T) {
                    u16 => .R16_UINT,
                    u32 => .R32_UINT,
                    else => unreachable,
                },
                .SizeInBytes = buffer_length,
            },
        };
    }

    pub fn createWritableVertices(
        gctx: *GraphicsContext,
        comptime T: type,
        vertices_length: usize,
    ) HResultError!VerticesHandle {
        switch (@typeInfo(T)) {
            .Struct => |s| {
                if (s.layout != .@"extern") {
                    @compileError(@typeName(T) ++ " must be extern");
                }
            },
            else => {},
        }

        return try VerticesHandle.init(T, gctx, vertices_length);
    }

    pub fn createDepthStencilView(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        desc: ?*const d3d12.DEPTH_STENCIL_VIEW_DESC,
        view: d3d12.CPU_DESCRIPTOR_HANDLE,
    ) void {
        gctx.device.CreateDepthStencilView(
            gctx.lookupResource(handle).?,
            desc,
            view,
        );
    }

    pub fn allocShaderResourceView(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        desc: ?*const d3d12.SHADER_RESOURCE_VIEW_DESC,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        const resource = gctx.lookupResource(handle).?;
        const cpu_handle = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

        gctx.device.CreateShaderResourceView(resource, desc, cpu_handle);

        return cpu_handle;
    }

    pub fn allocUnorderedAccessView(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        counter_handle: ?ResourceHandle,
        desc: ?*const d3d12.UNORDERED_ACCESS_VIEW_DESC,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        const resource = gctx.lookupResource(handle).?;
        const counter_resource = if (counter_handle) |ch| gctx.lookupResource(ch).? else null;
        const cpu_handle = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

        gctx.device.CreateUnorderedAccessView(resource, counter_resource, desc, cpu_handle);

        return cpu_handle;
    }

    pub fn allocRenderTargetView(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        desc: ?*const d3d12.RENDER_TARGET_VIEW_DESC,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        const resource = gctx.lookupResource(handle).?;
        const cpu_handle = gctx.allocateCpuDescriptors(.RTV, 1);

        gctx.device.CreateRenderTargetView(resource, desc, cpu_handle);

        return cpu_handle;
    }
    pub fn allocDepthStencilView(
        gctx: *GraphicsContext,
        handle: ResourceHandle,
        desc: ?*const d3d12.DEPTH_STENCIL_VIEW_DESC,
    ) d3d12.CPU_DESCRIPTOR_HANDLE {
        const resource = gctx.lookupResource(handle).?;
        const cpu_handle = gctx.allocateCpuDescriptors(.DSV, 1);

        gctx.device.CreateDepthStencilView(resource, desc, cpu_handle);

        return cpu_handle;
    }

    pub fn setGraphicsRootDescriptorTable(gctx: *GraphicsContext, root_index: windows.UINT, cpu_handles: []const d3d12.CPU_DESCRIPTOR_HANDLE) void {
        gctx.cmdlist.SetGraphicsRootDescriptorTable(root_index, gctx.copyDescriptorsToGpuHeap(1, cpu_handles[0]));
        for (cpu_handles[1..]) |cpu_handle| {
            _ = gctx.copyDescriptorsToGpuHeap(1, cpu_handle);
        }
    }

    pub fn setComputeRootDescriptorTable(gctx: *GraphicsContext, root_index: windows.UINT, cpu_handles: []const d3d12.CPU_DESCRIPTOR_HANDLE) void {
        gctx.cmdlist.SetComputeRootDescriptorTable(root_index, gctx.copyDescriptorsToGpuHeap(1, cpu_handles[0]));
        for (cpu_handles[1..]) |cpu_handle| {
            _ = gctx.copyDescriptorsToGpuHeap(1, cpu_handle);
        }
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

        if (@as(u32, @bitCast(state_after)) != @as(u32, @bitCast(resource.?.state))) {
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
        pso_desc: *d3d12.GRAPHICS_PIPELINE_STATE_DESC,
    ) PipelineHandle {
        assert(pso_desc.VS.pShaderBytecode != null);

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @as([*]const u8, @ptrCast(pso_desc.VS.pShaderBytecode.?))[0..pso_desc.VS.BytecodeLength],
            );
            if (pso_desc.GS.pShaderBytecode != null) {
                hasher.update(
                    @as([*]const u8, @ptrCast(pso_desc.GS.pShaderBytecode.?))[0..pso_desc.GS.BytecodeLength],
                );
            }
            if (pso_desc.PS.pShaderBytecode != null) {
                hasher.update(
                    @as([*]const u8, @ptrCast(pso_desc.PS.pShaderBytecode.?))[0..pso_desc.PS.BytecodeLength],
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

        if (pso_desc.pRootSignature == null) {
            pso_desc.pRootSignature = undefined;
            hrPanicOnFail(gctx.device.CreateRootSignature(
                0,
                pso_desc.VS.pShaderBytecode.?,
                pso_desc.VS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @as(*?*anyopaque, @ptrCast(&pso_desc.pRootSignature.?)),
            ));
        }

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gctx.device.CreateGraphicsPipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @as(*?*anyopaque, @ptrCast(&pso)),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, pso_desc.pRootSignature.?, .Graphics, hash);
    }

    pub fn createMeshShaderPipeline(
        gctx: *GraphicsContext,
        pso_desc: *d3d12.MESH_SHADER_PIPELINE_STATE_DESC,
    ) PipelineHandle {
        assert(pso_desc.MS.pShaderBytecode != null);
        assert(pso_desc.PS.pShaderBytecode != null);

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @as([*]const u8, @ptrCast(pso_desc.MS.pShaderBytecode.?))[0..pso_desc.MS.BytecodeLength],
            );
            if (pso_desc.AS.pShaderBytecode != null) {
                hasher.update(
                    @as([*]const u8, @ptrCast(pso_desc.AS.pShaderBytecode.?))[0..pso_desc.AS.BytecodeLength],
                );
            }
            hasher.update(
                @as([*]const u8, @ptrCast(pso_desc.PS.pShaderBytecode.?))[0..pso_desc.PS.BytecodeLength],
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

        if (pso_desc.pRootSignature == null) {
            pso_desc.pRootSignature = undefined;
            hrPanicOnFail(gctx.device.CreateRootSignature(
                0,
                pso_desc.MS.pShaderBytecode.?,
                pso_desc.MS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @as(*?*anyopaque, @ptrCast(&pso_desc.pRootSignature.?)),
            ));
        }

        const pso = blk: {
            var stream = d3d12.PIPELINE_MESH_STATE_STREAM.init(pso_desc.*);
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gctx.device.CreatePipelineState(
                &d3d12.PIPELINE_STATE_STREAM_DESC{
                    .SizeInBytes = @sizeOf(@TypeOf(stream)),
                    .pPipelineStateSubobjectStream = &stream,
                },
                &d3d12.IID_IPipelineState,
                @as(*?*anyopaque, @ptrCast(&pso)),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, pso_desc.pRootSignature.?, .Graphics, hash);
    }

    pub fn createComputeShaderPipeline(
        gctx: *GraphicsContext,
        pso_desc: *d3d12.COMPUTE_PIPELINE_STATE_DESC,
    ) PipelineHandle {
        assert(pso_desc.CS.pShaderBytecode != null);

        const hash = compute_hash: {
            var hasher = std.hash.Adler32.init();
            hasher.update(
                @as([*]const u8, @ptrCast(pso_desc.CS.pShaderBytecode.?))[0..pso_desc.CS.BytecodeLength],
            );
            break :compute_hash hasher.final();
        };
        std.log.info("[graphics] Compute pipeline hash: {d}", .{hash});

        if (gctx.pipeline_pool.map.contains(hash)) {
            std.log.info("[graphics] Compute pipeline hit detected.", .{});
            const handle = gctx.pipeline_pool.map.getEntry(hash).?.value_ptr.*;
            return handle;
        }

        if (pso_desc.pRootSignature == null) {
            pso_desc.pRootSignature = undefined;
            hrPanicOnFail(gctx.device.CreateRootSignature(
                0,
                pso_desc.CS.pShaderBytecode.?,
                pso_desc.CS.BytecodeLength,
                &d3d12.IID_IRootSignature,
                @as(*?*anyopaque, @ptrCast(&pso_desc.pRootSignature.?)),
            ));
        }

        const pso = blk: {
            var pso: *d3d12.IPipelineState = undefined;
            hrPanicOnFail(gctx.device.CreateComputePipelineState(
                pso_desc,
                &d3d12.IID_IPipelineState,
                @as(*?*anyopaque, @ptrCast(&pso)),
            ));
            break :blk pso;
        };

        return gctx.pipeline_pool.addPipeline(pso, pso_desc.pRootSignature.?, .Compute, hash);
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
            .cpu_slice = std.mem.bytesAsSlice(T, @as([]align(@alignOf(T)) u8, @alignCast(memory.cpu_slice.?))),
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

        const upload = gctx.allocateUploadBufferRegion(u8, @as(u32, @intCast(required_size)));
        layout[0].Offset = upload.buffer_offset;

        const pixel_size = resource.?.desc.Format.pixelSizeInBits() / 8;
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

    pub fn createAndUploadTex2d(
        gctx: *GraphicsContext,
        width: u32,
        height: u32,
        num_components: u8,
        data: []const u8,
    ) HResultError!ResourceHandle {
        const dxgi_format = if (num_components == 1) dxgi.FORMAT.R8_UNORM else dxgi.FORMAT.R8G8B8A8_UNORM;

        const desc = d3d12.RESOURCE_DESC.initTex2d(dxgi_format, width, height, 0);
        const texture = try gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &desc,
            .{ .COPY_DEST = true },
            null,
        );

        var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
        var required_size: u64 = undefined;
        gctx.device.GetCopyableFootprints(&desc, 0, 1, 0, &layout, null, null, &required_size);

        const upload = gctx.allocateUploadBufferRegion(u8, @as(u32, @intCast(required_size)));
        layout[0].Offset = upload.buffer_offset;

        @memcpy(upload.cpu_slice, data);

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
                windows.GENERIC_READ,
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
            var pixel_format: windows.GUID = undefined;
            hrPanicOnFail(bmp_frame.GetPixelFormat(&pixel_format));
            break :blk pixel_format;
        };

        const eql = std.mem.eql;
        const asBytes = std.mem.asBytes;
        const num_components: u32 = blk: {
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat2bppIndexed))) break :blk 4;

            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat24bppRGB))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppRGB))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppRGBA))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppPRGBA))) break :blk 4;

            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat24bppBGR))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppBGR))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppBGRA))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat32bppPBGRA))) break :blk 4;
            if (eql(u8, asBytes(&pixel_format), asBytes(&wic.GUID_PixelFormat64bppRGBA))) break :blk 4;

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
            @as(*wic.IBitmapSource, @ptrCast(bmp_frame)),
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

        const upload = gctx.allocateUploadBufferRegion(u8, @as(u32, @intCast(required_size)));
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

    pub fn createAndUploadTex2dFromDdsFile(
        gctx: *GraphicsContext,
        relpath: []const u8,
        arena: std.mem.Allocator,
        args: struct {
            is_cubemap: bool = false,
        },
    ) !ResourceHandle {
        assert(gctx.is_cmdlist_opened);

        var buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
        var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
        const allocator = fba.allocator();

        const abspath = std.fs.path.join(allocator, &.{
            std.fs.selfExeDirPathAlloc(allocator) catch unreachable,
            relpath,
        }) catch unreachable;

        // Load DDS data into D3D12_SUBRESOURCE_DATA
        var subresources = std.ArrayList(d3d12.SUBRESOURCE_DATA).init(arena);
        defer subresources.deinit();

        const dds_info = try dds_loader.loadTextureFromFile(abspath, arena, gctx.device, 0, &subresources);
        assert(dds_info.resource_dimension == .TEXTURE2D);
        assert(dds_info.cubemap == args.is_cubemap);

        var texture_desc = blk: {
            if (args.is_cubemap) {
                break :blk d3d12.RESOURCE_DESC.initTexCube(
                    dds_info.format,
                    dds_info.width,
                    dds_info.height,
                    dds_info.mip_map_count,
                );
            } else {
                break :blk d3d12.RESOURCE_DESC.initTex2d(
                    dds_info.format,
                    dds_info.width,
                    dds_info.height,
                    dds_info.mip_map_count,
                );
            }
        };

        const texture = try gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &texture_desc,
            .{ .COPY_DEST = true },
            null,
        );
        texture_desc = gctx.lookupResource(texture).?.GetDesc();

        for (0..subresources.items.len) |index| {
            const subresource_index = @as(u32, @intCast(index));

            var layout: [1]d3d12.PLACED_SUBRESOURCE_FOOTPRINT = undefined;
            var num_rows: [1]u32 = undefined;
            var row_size_in_bytes: [1]u64 = undefined;
            var required_size: u64 = undefined;
            gctx.device.GetCopyableFootprints(
                &texture_desc,
                subresource_index,
                layout.len,
                0,
                &layout,
                &num_rows,
                &row_size_in_bytes,
                &required_size,
            );

            const upload = gctx.allocateUploadBufferRegion(u8, @as(u32, @intCast(required_size)));
            layout[0].Offset = upload.buffer_offset;

            const subresource = &subresources.items[subresource_index];
            var row: u32 = 0;

            const row_size_in_bytes_fixed = row_size_in_bytes[0];
            var cpu_slice_as_bytes = std.mem.sliceAsBytes(upload.cpu_slice);
            const subresource_slice = subresource.pData.?;
            while (row < num_rows[0]) : (row += 1) {
                const cpu_slice_begin = layout[0].Footprint.RowPitch * row;
                const cpu_slice_end = cpu_slice_begin + row_size_in_bytes_fixed;
                const subresource_slice_begin = row_size_in_bytes[0] * row;
                const subresource_slice_end = subresource_slice_begin + row_size_in_bytes_fixed;
                @memcpy(
                    cpu_slice_as_bytes[cpu_slice_begin..cpu_slice_end],
                    subresource_slice[subresource_slice_begin..subresource_slice_end],
                );
            }

            gctx.cmdlist.CopyTextureRegion(&.{
                .pResource = gctx.lookupResource(texture).?,
                .Type = .SUBRESOURCE_INDEX,
                .u = .{ .SubresourceIndex = subresource_index },
            }, 0, 0, 0, &.{
                .pResource = upload.buffer,
                .Type = .PLACED_FOOTPRINT,
                .u = .{ .PlacedFootprint = layout[0] },
            }, null);
        }

        return texture;
    }

    pub fn createRootSignature(
        gctx: *GraphicsContext,
        node_mask: windows.UINT,
        signature: *d3d.IBlob,
    ) HResultError!*d3d12.IRootSignature {
        var root_signature: *d3d12.IRootSignature = undefined;
        try hrErrorOnFail(gctx.device.CreateRootSignature(
            node_mask,
            signature.GetBufferPointer(),
            signature.GetBufferSize(),
            &d3d12.IID_IRootSignature,
            @ptrCast(&root_signature),
        ));
        return root_signature;
    }

    pub fn writeResource(
        gctx: *GraphicsContext,
        comptime T: type,
        destination: ResourceHandle,
        source: []const T,
    ) HResultError!void {
        if (source.len == 0) {
            return;
        }
        const resource = gctx.lookupResource(destination).?;
        var mapped_buffer: [*]u8 = undefined;
        try hrErrorOnFail(resource.Map(
            0,
            &.{ .Begin = 0, .End = 0 },
            @ptrCast(&mapped_buffer),
        ));
        defer resource.Unmap(0, null);
        const byte_length = source.len * @sizeOf(T);
        const mapped_slice = std.mem.bytesAsSlice(T, mapped_buffer[0..byte_length]);
        @memcpy(mapped_slice, source);
    }

    pub fn writeVertices(
        gctx: *GraphicsContext,
        comptime T: type,
        destination: VerticesHandle,
        vertices: []const T,
    ) HResultError!void {
        try gctx.writeResource(T, destination.resource, vertices);
    }

    pub inline fn clearRenderTargetView(
        gctx: *GraphicsContext,
        rt_view: d3d12.CPU_DESCRIPTOR_HANDLE,
        rgba: *const [4]windows.FLOAT,
        rects: []const windows.RECT,
    ) void {
        gctx.cmdlist.ClearRenderTargetView(
            rt_view,
            rgba,
            rects.len,
            if (rects.len == 0) null else rects.ptr,
        );
    }

    pub inline fn clearDepthStencilView(
        gctx: *GraphicsContext,
        ds_view: d3d12.CPU_DESCRIPTOR_HANDLE,
        clear_flags: d3d12.CLEAR_FLAGS,
        depth: windows.FLOAT,
        stencil: windows.UINT8,
        rects: []const windows.RECT,
    ) void {
        gctx.cmdlist.ClearDepthStencilView(
            ds_view,
            clear_flags,
            depth,
            stencil,
            rects.len,
            if (rects.len == 0) null else rects.ptr,
        );
    }

    pub inline fn omSetRenderTargets(
        gctx: *GraphicsContext,
        render_target_descriptors: []const d3d12.CPU_DESCRIPTOR_HANDLE,
        single_handle: bool,
        ds_descriptors: ?*const d3d12.CPU_DESCRIPTOR_HANDLE,
    ) void {
        gctx.cmdlist.OMSetRenderTargets(
            @intCast(render_target_descriptors.len),
            render_target_descriptors.ptr,
            if (single_handle) windows.TRUE else windows.FALSE,
            ds_descriptors,
        );
    }
    pub inline fn rsSetViewports(gctx: *GraphicsContext, viewports: []const d3d12.VIEWPORT) void {
        gctx.cmdlist.RSSetViewports(@intCast(viewports.len), viewports.ptr);
    }
    pub inline fn rsSetScissorRects(gctx: *GraphicsContext, rects: []const d3d12.RECT) void {
        gctx.cmdlist.RSSetScissorRects(@intCast(rects.len), rects.ptr);
    }
    pub inline fn iaSetPrimitiveTopology(gctx: *GraphicsContext, topology: d3d12.PRIMITIVE_TOPOLOGY) void {
        gctx.cmdlist.IASetPrimitiveTopology(topology);
    }

    pub inline fn iaSetVertexBuffers(gctx: *GraphicsContext, start_slot: windows.UINT, views: []const d3d12.VERTEX_BUFFER_VIEW) void {
        gctx.cmdlist.IASetVertexBuffers(start_slot, @intCast(views.len), views.ptr);
    }
    pub inline fn iaSetIndexBuffer(gctx: *GraphicsContext, view: ?*const d3d12.INDEX_BUFFER_VIEW) void {
        gctx.cmdlist.IASetIndexBuffer(view);
    }
    pub inline fn setGraphicsRootConstantBufferView(gctx: *GraphicsContext, index: windows.UINT, handle: ResourceHandle) void {
        const resource = gctx.lookupResource(handle).?;
        gctx.cmdlist.SetGraphicsRootConstantBufferView(index, resource.GetGPUVirtualAddress());
    }

    pub inline fn drawInstanced(
        gctx: *GraphicsContext,
        vertex_count_per_instance: windows.UINT,
        instance_count: windows.UINT,
        start_vertex_location: windows.UINT,
        start_instance_location: windows.UINT,
    ) void {
        gctx.cmdlist.DrawInstanced(
            vertex_count_per_instance,
            instance_count,
            start_vertex_location,
            start_instance_location,
        );
    }
    pub inline fn drawIndexedInstanced(
        gctx: *GraphicsContext,
        index_count_per_instance: windows.UINT,
        instance_count: windows.UINT,
        start_index_location: windows.UINT,
        base_vertex_location: windows.INT,
        start_instance_location: windows.UINT,
    ) void {
        gctx.cmdlist.DrawIndexedInstanced(
            index_count_per_instance,
            instance_count,
            start_index_location,
            base_vertex_location,
            start_instance_location,
        );
    }
};

pub const MipmapGenerator = struct {
    const num_scratch_textures = 4;

    pipeline: PipelineHandle,
    scratch_textures: [num_scratch_textures]ResourceHandle,
    base_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
    format: dxgi.FORMAT,

    pub fn init(
        gctx: *GraphicsContext,
        format: dxgi.FORMAT,
        generate_mipmap_bytecode: d3d12.SHADER_BYTECODE,
    ) MipmapGenerator {
        var width: u32 = 2048 / 2;
        var height: u32 = 2048 / 2;

        var scratch_textures: [num_scratch_textures]ResourceHandle = undefined;
        for (scratch_textures, 0..) |_, texture_index| {
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
        for (scratch_textures, 0..) |_, texture_index| {
            gctx.device.CreateUnorderedAccessView(
                gctx.lookupResource(scratch_textures[texture_index]).?,
                null,
                null,
                cpu_handle,
            );
            cpu_handle.ptr += gctx.cbv_srv_uav_cpu_heap.descriptor_size;
        }

        var desc = d3d12.COMPUTE_PIPELINE_STATE_DESC{};
        desc.CS = generate_mipmap_bytecode;
        const pipeline = gctx.createComputeShaderPipeline(&desc);

        return .{
            .pipeline = pipeline,
            .scratch_textures = scratch_textures,
            .base_uav = base_uav,
            .format = format,
        };
    }

    pub fn deinit(mipgen: *MipmapGenerator, gctx: *GraphicsContext) void {
        for (mipgen.scratch_textures, 0..) |_, texture_index| {
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
                const num_groups_x = @max(
                    @as(u32, @intCast(texture_desc.Width)) >> @as(u5, @intCast(3 + current_src_mip_level)),
                    1,
                );
                const num_groups_y = @max(
                    texture_desc.Height >> @as(u5, @intCast(3 + current_src_mip_level)),
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
                        .right = @as(u32, @intCast(texture_desc.Width)) >> @as(
                            u5,
                            @intCast(mip_index + 1 + current_src_mip_level),
                        ),
                        .bottom = texture_desc.Height >> @as(u5, @intCast(mip_index + 1 + current_src_mip_level)),
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
                const resources = allocator.alloc(
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
                const generations = allocator.alloc(
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
            .index = @as(u16, @intCast(slot_idx)),
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
                const pipelines = allocator.alloc(
                    Pipeline,
                    max_num_pipelines + 1,
                ) catch unreachable;
                for (pipelines) |*pipeline| {
                    pipeline.* = .{ .pso = null, .rs = null, .ptype = null };
                }
                break :blk pipelines;
            },
            .generations = blk: {
                const generations = allocator.alloc(
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
            .index = @as(u16, @intCast(slot_idx)),
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
            }, &d3d12.IID_IDescriptorHeap, @as(*?*anyopaque, @ptrCast(&heap))));
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
                @as(*?*anyopaque, @ptrCast(&resource)),
            ));
            break :blk resource;
        };
        const cpu_base = blk: {
            var cpu_base: [*]u8 = undefined;
            hrPanicOnFail(resource.Map(
                0,
                &d3d12.RANGE{ .Begin = 0, .End = 0 },
                @as(*?*anyopaque, @ptrCast(&cpu_base)),
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

pub fn serializeVersionedRootSignature(root_signature_desc: *const d3d12.VERSIONED_ROOT_SIGNATURE_DESC) HResultError!*d3d.IBlob {
    var signature: ?*d3d.IBlob = undefined;
    try hrErrorOnFail(d3d12.SerializeVersionedRootSignature(root_signature_desc, &signature, null));
    return signature.?;
}
