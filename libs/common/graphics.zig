const builtin = @import("builtin");
const std = @import("std");
const w = @import("../win32/win32.zig");
const assert = std.debug.assert;

pub inline fn vhr(hr: w.HRESULT) !void {
    if (hr != 0) {
        return error.HResult;
        //std.debug.panic("HRESULT function failed ({}).", .{hr});
    }
}

pub const GraphicsContext = struct {
    const max_num_buffered_frames = 2;
    const num_swapbuffers = 4;
    const num_rtv_descriptors = 128;
    const num_dsv_descriptors = 128;
    const num_cbv_srv_uav_cpu_descriptors = 16 * 1024;
    const num_cbv_srv_uav_gpu_descriptors = 4 * 1024;
    const max_num_buffered_resource_barriers = 32;
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
    buffered_resource_barriers: []w.D3D12_RESOURCE_BARRIER,
    num_buffered_resource_barriers: u32,
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
            var maybe_device: ?*w.ID3D12Device9 = null;
            try vhr(w.D3D12CreateDevice(null, ._11_1, &w.IID_ID3D12Device9, @ptrCast(*?*c_void, &maybe_device)));
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
            try vhr(maybe_swapchain.?.QueryInterface(
                &w.IID_IDXGISwapChain3,
                @ptrCast(*?*c_void, &maybe_swapchain3),
            ));
            break :blk maybe_swapchain3.?;
        };
        errdefer _ = swapchain.Release();

        var resource_pool = ResourcePool.init();
        var pipeline_pool = PipelinePool.init();

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
        for (cbv_srv_uav_gpu_heaps) |*heap, heap_index| {
            heap.* = DescriptorHeap.init(
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
        for (upload_heaps) |*heap, heap_index| {
            heap.* = GpuMemoryHeap.init(device, upload_heap_capacity, .UPLOAD) catch |err| {
                var i: u32 = 0;
                while (i < heap_index) : (i += 1) {
                    upload_heaps[i].deinit();
                }
                return err;
            };
        }
        errdefer for (upload_heaps) |*heap| heap.*.deinit();

        const swapbuffers = blk: {
            var maybe_swapbuffers = [_]?*w.ID3D12Resource{null} ** num_swapbuffers;
            errdefer {
                for (maybe_swapbuffers) |swapbuffer| {
                    if (swapbuffer) |sb| _ = sb.Release();
                }
            }
            for (maybe_swapbuffers) |*swapbuffer, buffer_idx| {
                try vhr(swapchain.GetBuffer(
                    @intCast(u32, buffer_idx),
                    &w.IID_ID3D12Resource,
                    @ptrCast(*?*c_void, &swapbuffer.*),
                ));
                device.CreateRenderTargetView(swapbuffer.*, null, rtv_heap.allocateDescriptors(1).cpu_handle);
            }
            var swapbuffers: [num_swapbuffers]*w.ID3D12Resource = undefined;
            for (maybe_swapbuffers) |swapbuffer, i| swapbuffers[i] = swapbuffer.?;
            break :blk swapbuffers;
        };
        errdefer {
            for (swapbuffers) |swapbuffer| _ = swapbuffer.Release();
        }

        const swapchain_buffers = blk: {
            var swapchain_buffers: [num_swapbuffers]ResourceHandle = undefined;
            for (swapbuffers) |swapbuffer, i| {
                swapchain_buffers[i] = resource_pool.addResource(swapbuffer, .{});
            }
            break :blk swapchain_buffers;
        };

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
            var maybe_cmdlist: ?*w.ID3D12GraphicsCommandList6 = null;
            try vhr(device.CreateCommandList(
                0,
                .DIRECT,
                cmdallocs[0],
                null,
                &w.IID_ID3D12GraphicsCommandList6,
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
            .buffered_resource_barriers = std.heap.page_allocator.alloc(
                w.D3D12_RESOURCE_BARRIER,
                max_num_buffered_resource_barriers,
            ) catch unreachable,
            .num_buffered_resource_barriers = 0,
            .viewport_width = viewport_width,
            .viewport_height = viewport_height,
            .frame_index = 0,
            .back_buffer_index = swapchain.GetCurrentBackBufferIndex(),
        };
    }

    pub fn deinit(gr: *GraphicsContext, allocator: *std.mem.Allocator) void {
        std.heap.page_allocator.free(gr.buffered_resource_barriers);
        assert(gr.pipeline.map.count() == 0);
        gr.pipeline.map.deinit(allocator);
        gr.resource_pool.deinit();
        gr.rtv_heap.deinit();
        gr.dsv_heap.deinit();
        gr.cbv_srv_uav_cpu_heap.deinit();
        for (gr.cbv_srv_uav_gpu_heaps) |*heap| heap.*.deinit();
        for (gr.upload_memory_heaps) |*heap| heap.*.deinit();
        _ = gr.device.Release();
        _ = gr.cmdqueue.Release();
        _ = gr.swapchain.Release();
        _ = gr.frame_fence.Release();
        _ = gr.cmdlist.Release();
        for (gr.cmdallocs) |cmdalloc| _ = cmdalloc.Release();
        gr.* = undefined;
    }

    pub fn beginFrame(gr: *GraphicsContext) !void {
        const cmdalloc = gr.cmdallocs[gr.frame_index];
        try vhr(cmdalloc.Reset());
        try vhr(gr.cmdlist.Reset(cmdalloc, null));
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

    pub fn flushGpuCommands(gr: *GraphicsContext) !void {
        gr.flushResourceBarriers();
        try vhr(gr.cmdlist.Close());
        gr.cmdqueue.ExecuteCommandLists(
            1,
            &[_]*w.ID3D12CommandList{@ptrCast(*w.ID3D12CommandList, gr.cmdlist)},
        );
    }

    pub fn finishGpuCommands(gr: *GraphicsContext) !void {
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

    pub fn releaseResource(gr: GraphicsContext, handle: ResourceHandle) u32 {
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
        if (gr.num_buffered_resource_barriers > 0) {
            gr.cmdlist.ResourceBarrier(gr.num_buffered_resource_barriers, gr.buffered_resource_barriers.ptr);
            gr.num_buffered_resource_barriers = 0;
        }
    }

    pub fn addTransitionBarrier(
        gr: *GraphicsContext,
        handle: ResourceHandle,
        state_after: w.D3D12_RESOURCE_STATES,
    ) void {
        var resource = gr.resource_pool.editResource(handle);
        if (@bitCast(u32, state_after) != @bitCast(u32, resource.state)) {
            if (gr.num_buffered_resource_barriers >= gr.buffered_resource_barriers.len) {
                gr.flushResourceBarriers();
            }
            gr.buffered_resource_barriers[gr.num_buffered_resource_barriers] = .{
                .Type = .TRANSITION,
                .Flags = .{},
                .u = .{
                    .Transition = .{
                        .pResource = resource.raw.?,
                        .Subresource = w.D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = resource.state,
                        .StateAfter = state_after,
                    },
                },
            };
            gr.num_buffered_resource_barriers += 1;
            resource.state = state_after;
        }
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

            gr.flushGpuCommands() catch unreachable;
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
                _ = resource.raw.?.Release();
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

    fn editResource(pool: ResourcePool, handle: ResourceHandle) *Resource {
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
        std.heap.page_allocator.free(poll.generations);
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
