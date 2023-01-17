const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const dml = zwin32.directml;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;

const enable_dx_debug = @import("zd3d12_options").enable_debug_layer;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: DirectML convolution test";
const window_width = 1800;
const window_height = 900;
const image_size = 1024;

const filter_tensor = [_][3][3]f16{
    [3][3]f16{
        [3]f16{ -2.0, -2.0, 0.0 },
        [3]f16{ -2.0, 6.0, 0.0 },
        [3]f16{ 0.0, 0.0, 0.0 },
    },
    [3][3]f16{ // edge detection
        [3]f16{ -1.0, -1.0, -1.0 },
        [3]f16{ 0.0, 0.0, 0.0 },
        [3]f16{ 1.0, 1.0, 1.0 },
    },
    [3][3]f16{ // edge detection 2
        [3]f16{ -1.0, -1.0, -1.0 },
        [3]f16{ -1.0, 8.0, -1.0 },
        [3]f16{ -1.0, -1.0, -1.0 },
    },
};

const OperatorState = struct {
    cop: *dml.ICompiledOperator,
    dtbl: *dml.IBindingTable,
    info: dml.BINDING_PROPERTIES,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    dml_device: *dml.IDevice1,

    conv_op_state: OperatorState,

    temp_buffer: ?zd3d12.ResourceHandle,
    persistent_buffer: ?zd3d12.ResourceHandle,

    input_buffer: zd3d12.ResourceHandle,
    input_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    filter_buffer: zd3d12.ResourceHandle,

    output_buffer: zd3d12.ResourceHandle,
    output_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    dml_cmd_recorder: *dml.ICommandRecorder,

    texture_to_buffer_pso: zd3d12.PipelineHandle,
    buffer_to_texture_pso: zd3d12.PipelineHandle,
    draw_texture_pso: zd3d12.PipelineHandle,

    image_texture: zd3d12.ResourceHandle,
    image_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    image_texture_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
};

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    const draw_texture_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/draw_texture.vs.cso",
            content_dir ++ "shaders/draw_texture.ps.cso",
        );
    };

    const texture_to_buffer_pso = blk: {
        var desc = d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault();
        break :blk gctx.createComputeShaderPipeline(
            arena_allocator,
            &desc,
            content_dir ++ "shaders/texture_to_buffer.cs.cso",
        );
    };
    const buffer_to_texture_pso = blk: {
        var desc = d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault();
        break :blk gctx.createComputeShaderPipeline(
            arena_allocator,
            &desc,
            content_dir ++ "shaders/buffer_to_texture.cs.cso",
        );
    };

    var dml_device: *dml.IDevice1 = undefined;
    hrPanicOnFail(dml.createDevice(
        @ptrCast(*d3d12.IDevice, gctx.device),
        .{ .DEBUG = enable_dx_debug },
        .@"4_1",
        &dml.IID_IDevice1,
        @ptrCast(*?*anyopaque, &dml_device),
    ));

    const input_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = .{},
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, image_size, image_size },
            .Strides = &[_]u32{ image_size * image_size, image_size, image_size, 1 },
            .TotalTensorSizeInBytes = image_size * image_size * @sizeOf(f16),
            .GuaranteedBaseOffsetAlignment = 256,
        },
    };

    const output_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = .{},
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, image_size - 2, image_size - 2 },
            .Strides = &[_]u32{ image_size * image_size, image_size, image_size, 1 },
            .TotalTensorSizeInBytes = image_size * image_size * @sizeOf(f16),
            .GuaranteedBaseOffsetAlignment = 256,
        },
    };

    const filter_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = .{},
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, 3, 3 },
            .Strides = null,
            .TotalTensorSizeInBytes = std.mem.alignForward(filter_tensor.len * filter_tensor.len * @sizeOf(f16), 32),
            .GuaranteedBaseOffsetAlignment = 256,
        },
    };

    const conv_op = blk: {
        const desc = dml.OPERATOR_DESC{
            .Type = .CONVOLUTION,
            .Desc = &dml.CONVOLUTION_OPERATOR_DESC{
                .InputTensor = &input_tensor_desc,
                .FilterTensor = &filter_tensor_desc,
                .BiasTensor = null,
                .OutputTensor = &output_tensor_desc,
                .Mode = .CONVOLUTION,
                .Direction = .FORWARD,
                .DimensionCount = 2,
                .Strides = &[_]u32{ 1, 1 },
                .Dilations = &[_]u32{ 1, 1 },
                .StartPadding = &[_]u32{ 0, 0 },
                .EndPadding = &[_]u32{ 0, 0 },
                .OutputPadding = &[_]u32{ 0, 0 },
                .GroupCount = 1,
                .FusedActivation = null,
            },
        };

        var op: *dml.IOperator = undefined;
        hrPanicOnFail(dml_device.CreateOperator(&desc, &dml.IID_IOperator, @ptrCast(*?*anyopaque, &op)));
        break :blk op;
    };
    defer _ = conv_op.Release();

    const conv_cop = blk: {
        var cop: *dml.ICompiledOperator = undefined;
        hrPanicOnFail(dml_device.CompileOperator(
            conv_op,
            .{},
            &dml.IID_ICompiledOperator,
            @ptrCast(*?*anyopaque, &cop),
        ));
        break :blk cop;
    };

    const init_op = blk: {
        const operators = [_]*dml.ICompiledOperator{conv_cop};

        var iop: *dml.IOperatorInitializer = undefined;
        hrPanicOnFail(dml_device.CreateOperatorInitializer(
            operators.len,
            &operators,
            &dml.IID_IOperatorInitializer,
            @ptrCast(*?*anyopaque, &iop),
        ));
        break :blk iop;
    };
    defer _ = init_op.Release();

    const conv_info = conv_cop.GetBindingProperties();
    const init_info = init_op.GetBindingProperties();

    const temp_resource_size: u64 = math.max(init_info.TemporaryResourceSize, conv_info.TemporaryResourceSize);
    const persistent_resource_size: u64 = conv_info.PersistentResourceSize;

    const temp_buffer = if (temp_resource_size > 0) gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(temp_resource_size);
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        d3d12.RESOURCE_STATES.COMMON,
        null,
    ) catch |err| hrPanic(err) else null;

    const persistent_buffer = if (persistent_resource_size > 0) gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(persistent_resource_size);
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        d3d12.RESOURCE_STATES.COMMON,
        null,
    ) catch |err| hrPanic(err) else null;

    const input_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(image_size * image_size * @sizeOf(f16));
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        .{ .UNORDERED_ACCESS = true },
        null,
    ) catch |err| hrPanic(err);

    const input_buffer_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(input_buffer).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R16_FLOAT, 0, image_size * image_size),
        input_buffer_srv,
    );

    const input_buffer_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gctx.device.CreateUnorderedAccessView(
        gctx.lookupResource(input_buffer).?,
        null,
        &d3d12.UNORDERED_ACCESS_VIEW_DESC.initTypedBuffer(.R16_FLOAT, 0, image_size * image_size, 0),
        input_buffer_uav,
    );

    const filter_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(
                std.mem.alignForward(filter_tensor.len * filter_tensor.len * @sizeOf(f16), 32),
            );
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        .{ .UNORDERED_ACCESS = true },
        null,
    ) catch |err| hrPanic(err);

    const output_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(image_size * image_size * @sizeOf(f16));
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        .{ .UNORDERED_ACCESS = true },
        null,
    ) catch |err| hrPanic(err);

    const output_buffer_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(output_buffer).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R16_FLOAT, 0, image_size * image_size),
        output_buffer_srv,
    );

    const dml_cmd_recorder = blk: {
        var dml_cmd_recorder: *dml.ICommandRecorder = undefined;
        hrPanicOnFail(dml_device.CreateCommandRecorder(
            &dml.IID_ICommandRecorder,
            @ptrCast(*?*anyopaque, &dml_cmd_recorder),
        ));
        break :blk dml_cmd_recorder;
    };

    //
    // Begin frame to init/upload resources to the GPU.
    //
    gctx.beginFrame();
    gctx.endFrame();
    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    const image_texture = gctx.createAndUploadTex2dFromFile(
        content_dir ++ "genart_0025_5.png",
        .{
            .num_mip_levels = 1,
            .texture_flags = .{ .ALLOW_UNORDERED_ACCESS = true },
        },
    ) catch |err| hrPanic(err);

    const image_texture_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    const image_texture_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    gctx.device.CreateShaderResourceView(gctx.lookupResource(image_texture).?, null, image_texture_srv);
    gctx.device.CreateUnorderedAccessView(gctx.lookupResource(image_texture).?, null, null, image_texture_uav);

    gctx.addTransitionBarrier(image_texture, .{ .NON_PIXEL_SHADER_RESOURCE = true });
    gctx.addTransitionBarrier(input_buffer, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    gctx.setCurrentPipeline(texture_to_buffer_pso);
    gctx.cmdlist.SetComputeRootDescriptorTable(0, blk: {
        const table = gctx.copyDescriptorsToGpuHeap(1, image_texture_srv);
        _ = gctx.copyDescriptorsToGpuHeap(1, input_buffer_uav);
        break :blk table;
    });
    gctx.cmdlist.Dispatch((image_size + 7) / 8, (image_size + 7) / 8, 1);

    gctx.addTransitionBarrier(image_texture, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.addTransitionBarrier(input_buffer, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    const conv_op_state = blk: {
        const base_descriptor = gctx.allocateGpuDescriptors(conv_info.RequiredDescriptorCount);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, conv_cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = conv_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*anyopaque, &table)));

        break :blk .{
            .cop = conv_cop,
            .dtbl = table,
            .info = conv_info,
        };
    };

    const init_dtbl = blk: {
        const base_descriptor = gctx.allocateGpuDescriptors(init_info.RequiredDescriptorCount + 1);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, init_op),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = init_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*anyopaque, &table)));
        break :blk table;
    };
    defer _ = init_dtbl.Release();

    if (temp_buffer != null and init_info.TemporaryResourceSize > 0) {
        init_dtbl.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(temp_buffer.?).?,
                .Offset = 0,
                .SizeInBytes = init_info.TemporaryResourceSize,
            },
        });
    }

    if (persistent_buffer != null) {
        var offset: u64 = 0;
        const binding0 = if (conv_info.PersistentResourceSize == 0) dml.BINDING_DESC{
            .Type = .NONE,
            .Desc = null,
        } else dml.BINDING_DESC{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(persistent_buffer.?).?,
                .Offset = offset,
                .SizeInBytes = conv_info.PersistentResourceSize,
            },
        };
        offset += conv_info.PersistentResourceSize;

        init_dtbl.BindOutputs(1, &[_]dml.BINDING_DESC{binding0});
    }

    dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, gctx.cmdlist),
        @ptrCast(*dml.IDispatchable, init_op),
        init_dtbl,
    );

    gctx.endFrame();
    gctx.finishGpuCommands();

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .dml_device = dml_device,
        .conv_op_state = conv_op_state,
        .temp_buffer = temp_buffer,
        .persistent_buffer = persistent_buffer,
        .input_buffer = input_buffer,
        .input_buffer_srv = input_buffer_srv,
        .filter_buffer = filter_buffer,
        .output_buffer = output_buffer,
        .output_buffer_srv = output_buffer_srv,
        .dml_cmd_recorder = dml_cmd_recorder,
        .draw_texture_pso = draw_texture_pso,
        .texture_to_buffer_pso = texture_to_buffer_pso,
        .buffer_to_texture_pso = buffer_to_texture_pso,
        .image_texture = image_texture,
        .image_texture_srv = image_texture_srv,
        .image_texture_uav = image_texture_uav,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    _ = demo.dml_cmd_recorder.Release();
    _ = demo.conv_op_state.cop.Release();
    _ = demo.conv_op_state.dtbl.Release();
    _ = demo.dml_device.Release();
    demo.guir.deinit(&demo.gctx);
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.gctx.window, window_name);
    common.newImGuiFrame(demo.frame_stats.delta_time);
}

fn dispatchConvOperator(demo: *DemoState) void {
    var gctx = &demo.gctx;

    // Reset DML binding table.
    {
        const num_descriptors = demo.conv_op_state.info.RequiredDescriptorCount;
        const base_descriptor = gctx.allocateGpuDescriptors(num_descriptors);

        hrPanicOnFail(demo.conv_op_state.dtbl.Reset(&dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, demo.conv_op_state.cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = num_descriptors,
        }));
    }

    // If necessary, bind temporary buffer.
    if (demo.temp_buffer != null and demo.conv_op_state.info.TemporaryResourceSize > 0) {
        demo.conv_op_state.dtbl.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(demo.temp_buffer.?).?,
                .Offset = 0,
                .SizeInBytes = demo.conv_op_state.info.TemporaryResourceSize,
            },
        });
    }

    // If necessary, bind persistent buffer.
    if (demo.persistent_buffer != null and demo.conv_op_state.info.PersistentResourceSize > 0) {
        demo.conv_op_state.dtbl.BindPersistentResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(demo.persistent_buffer.?).?,
                .Offset = 0,
                .SizeInBytes = demo.conv_op_state.info.PersistentResourceSize,
            },
        });
    }

    // Bind input buffers.
    demo.conv_op_state.dtbl.BindInputs(3, &[_]dml.BINDING_DESC{
        .{ // InputTensor
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(demo.input_buffer).?,
                .Offset = 0,
                .SizeInBytes = gctx.getResourceSize(demo.input_buffer),
            },
        },
        .{ // FilterTensor
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = gctx.lookupResource(demo.filter_buffer).?,
                .Offset = 0,
                .SizeInBytes = gctx.getResourceSize(demo.filter_buffer),
            },
        },
        .{ // BiasTensor
            .Type = .NONE,
            .Desc = null,
        },
    });

    // Bind output buffer.
    demo.conv_op_state.dtbl.BindOutputs(1, &[_]dml.BINDING_DESC{.{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_BINDING{
            .Buffer = gctx.lookupResource(demo.output_buffer).?,
            .Offset = 0,
            .SizeInBytes = gctx.getResourceSize(demo.output_buffer),
        },
    }});

    demo.dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, gctx.cmdlist),
        @ptrCast(*dml.IDispatchable, demo.conv_op_state.cop),
        demo.conv_op_state.dtbl,
    );
}

fn dispatchBarriers(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.cmdlist.ResourceBarrier(
        2,
        &[_]d3d12.RESOURCE_BARRIER{
            d3d12.RESOURCE_BARRIER.initUav(gctx.lookupResource(demo.input_buffer).?),
            d3d12.RESOURCE_BARRIER.initUav(gctx.lookupResource(demo.output_buffer).?),
        },
    );
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const back_buffer = gctx.getBackBuffer();

    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.addTransitionBarrier(demo.filter_buffer, .{ .COPY_DEST = true });
    gctx.flushResourceBarriers();

    // Upload the input tensor to the GPU.
    {
        const upload = gctx.allocateUploadBufferRegion(f16, 9);
        const kernel_index = 0;
        upload.cpu_slice[0] = filter_tensor[kernel_index][0][0];
        upload.cpu_slice[1] = filter_tensor[kernel_index][0][1];
        upload.cpu_slice[2] = filter_tensor[kernel_index][0][2];
        upload.cpu_slice[3] = filter_tensor[kernel_index][1][0];
        upload.cpu_slice[4] = filter_tensor[kernel_index][1][1];
        upload.cpu_slice[5] = filter_tensor[kernel_index][1][2];
        upload.cpu_slice[6] = filter_tensor[kernel_index][2][0];
        upload.cpu_slice[7] = filter_tensor[kernel_index][2][1];
        upload.cpu_slice[8] = filter_tensor[kernel_index][2][2];

        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(demo.filter_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    gctx.addTransitionBarrier(demo.input_buffer, .{ .UNORDERED_ACCESS = true });
    gctx.addTransitionBarrier(demo.filter_buffer, .{ .UNORDERED_ACCESS = true });
    gctx.addTransitionBarrier(demo.output_buffer, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    dispatchConvOperator(demo);
    dispatchBarriers(demo);

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        null,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.1, 0.2, 0.4, 1.0 },
        0,
        null,
    );

    //
    // Draw input buffer.
    //
    gctx.addTransitionBarrier(demo.input_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
    gctx.addTransitionBarrier(demo.image_texture, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    gctx.setCurrentPipeline(demo.buffer_to_texture_pso);
    gctx.cmdlist.SetComputeRootDescriptorTable(0, blk: {
        const table = gctx.copyDescriptorsToGpuHeap(1, demo.input_buffer_srv);
        _ = gctx.copyDescriptorsToGpuHeap(1, demo.image_texture_uav);
        break :blk table;
    });
    gctx.cmdlist.Dispatch((image_size + 7) / 8, (image_size + 7) / 8, 1);

    gctx.addTransitionBarrier(demo.image_texture, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = 0.0,
        .TopLeftY = 0.0,
        .Width = @intToFloat(f32, gctx.viewport_width / 2),
        .Height = @intToFloat(f32, gctx.viewport_width / 2),
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});

    gctx.setCurrentPipeline(demo.draw_texture_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(0, gctx.copyDescriptorsToGpuHeap(1, demo.image_texture_srv));
    gctx.cmdlist.DrawInstanced(3, 1, 0, 0);

    //
    // Draw output buffer.
    //
    gctx.addTransitionBarrier(demo.output_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
    gctx.addTransitionBarrier(demo.image_texture, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    gctx.setCurrentPipeline(demo.buffer_to_texture_pso);
    gctx.cmdlist.SetComputeRootDescriptorTable(0, blk: {
        const table = gctx.copyDescriptorsToGpuHeap(1, demo.output_buffer_srv);
        _ = gctx.copyDescriptorsToGpuHeap(1, demo.image_texture_uav);
        break :blk table;
    });
    gctx.cmdlist.Dispatch((image_size + 7) / 8, (image_size + 7) / 8, 1);

    gctx.addTransitionBarrier(demo.image_texture, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = @intToFloat(f32, gctx.viewport_width / 2),
        .TopLeftY = 0.0,
        .Width = @intToFloat(f32, gctx.viewport_width / 2),
        .Height = @intToFloat(f32, gctx.viewport_width / 2),
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});

    gctx.setCurrentPipeline(demo.draw_texture_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(0, gctx.copyDescriptorsToGpuHeap(1, demo.image_texture_srv));
    gctx.cmdlist.DrawInstanced(3, 1, 0, 0);

    demo.guir.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
    gctx.flushResourceBarriers();

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
