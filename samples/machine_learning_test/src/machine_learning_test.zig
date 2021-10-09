const builtin = @import("builtin");
const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const dml = win32.directml;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const pix = common.pix;
const vm = common.vectormath;
const tracy = common.tracy;
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const enable_dx_debug = @import("build_options").enable_dx_debug;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: machine learning test";
const window_width = 1920;
const window_height = 1080;

const filter_tensor = [3][3]f16{
    [3]f16{ -2.0, -2.0, 0.0 },
    [3]f16{ -2.0, 6.0, 0.0 },
    [3]f16{ 0.0, 0.0, 0.0 },
};

const OperatorState = struct {
    cop: *dml.ICompiledOperator,
    dtbl: *dml.IBindingTable,
    info: dml.BINDING_PROPERTIES,
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    info_txtfmt: *dwrite.ITextFormat,

    dml_device: *dml.IDevice1,

    conv_op_state: OperatorState,

    temp_buffer: ?gr.ResourceHandle,
    persistent_buffer: ?gr.ResourceHandle,

    input_buffer: gr.ResourceHandle,
    input_buffer_uav: d3d12.CPU_DESCRIPTOR_HANDLE,

    filter_buffer: gr.ResourceHandle,

    output_buffer: gr.ResourceHandle,
    output_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    dml_cmd_recorder: *dml.ICommandRecorder,

    texture_to_buffer_pso: gr.PipelineHandle,
    buffer_to_texture_pso: gr.PipelineHandle,
    draw_texture_pso: gr.PipelineHandle,

    image_texture: gr.ResourceHandle,
    image_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    image_texture_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
};

fn init(gpa: *std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;

    var arena_allocator = std.heap.ArenaAllocator.init(gpa);
    defer arena_allocator.deinit();

    _ = pix.loadGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);

    const draw_texture_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.DepthStencilState.DepthEnable = w.FALSE;

        break :blk grfx.createGraphicsShaderPipeline(
            &arena_allocator.allocator,
            &pso_desc,
            "content/shaders/draw_texture.vs.cso",
            "content/shaders/draw_texture.ps.cso",
        );
    };

    const texture_to_buffer_pso = grfx.createComputeShaderPipeline(
        &arena_allocator.allocator,
        &d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault(),
        "content/shaders/texture_to_buffer.cs.cso",
    );
    const buffer_to_texture_pso = grfx.createComputeShaderPipeline(
        &arena_allocator.allocator,
        &d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault(),
        "content/shaders/buffer_to_texture.cs.cso",
    );

    const brush = blk: {
        var brush: *d2d1.ISolidColorBrush = undefined;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            @ptrCast(*?*d2d1.ISolidColorBrush, &brush),
        ));
        break :blk brush;
    };

    const info_txtfmt = blk: {
        var info_txtfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            @ptrCast(*?*dwrite.ITextFormat, &info_txtfmt),
        ));
        break :blk info_txtfmt;
    };
    hrPanicOnFail(info_txtfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(info_txtfmt.SetParagraphAlignment(.NEAR));

    var dml_device: *dml.IDevice1 = undefined;
    hrPanicOnFail(dml.createDevice(
        @ptrCast(*d3d12.IDevice, grfx.device),
        if (enable_dx_debug) dml.CREATE_DEVICE_FLAG_DEBUG else dml.CREATE_DEVICE_FLAG_NONE,
        .FL_4_1,
        &dml.IID_IDevice1,
        @ptrCast(*?*c_void, &dml_device),
    ));

    const input_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, 1024, 1024 },
            .Strides = &[_]u32{ 1024 * 1024, 1024, 1024, 1 },
            .TotalTensorSizeInBytes = 1024 * 1024 * @sizeOf(f16),
            .GuaranteedBaseOffsetAlignment = 256,
        },
    };

    const output_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, 1022, 1022 },
            .Strides = &[_]u32{ 1024 * 1024, 1024, 1024, 1 },
            .TotalTensorSizeInBytes = 1024 * 1024 * @sizeOf(f16),
            .GuaranteedBaseOffsetAlignment = 256,
        },
    };

    const filter_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT16,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, 3, 3 },
            .Strides = null,
            .TotalTensorSizeInBytes = 3 * 3 * @sizeOf(f16) + 2,
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
        hrPanicOnFail(dml_device.CreateOperator(&desc, &dml.IID_IOperator, @ptrCast(*?*c_void, &op)));
        break :blk op;
    };
    defer _ = conv_op.Release();

    const conv_cop = blk: {
        var cop: *dml.ICompiledOperator = undefined;
        hrPanicOnFail(dml_device.CompileOperator(
            conv_op,
            dml.EXECUTION_FLAG_NONE,
            &dml.IID_ICompiledOperator,
            @ptrCast(*?*c_void, &cop),
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
            @ptrCast(*?*c_void, &iop),
        ));
        break :blk iop;
    };
    defer _ = init_op.Release();

    const conv_info = conv_cop.GetBindingProperties();
    const init_info = init_op.GetBindingProperties();

    const temp_resource_size: u64 = math.max(init_info.TemporaryResourceSize, conv_info.TemporaryResourceSize);
    const persistent_resource_size: u64 = conv_info.PersistentResourceSize;

    const temp_buffer = if (temp_resource_size > 0) grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(temp_resource_size);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_COMMON,
        null,
    ) catch |err| hrPanic(err) else null;

    const persistent_buffer = if (persistent_resource_size > 0) grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(persistent_resource_size);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_COMMON,
        null,
    ) catch |err| hrPanic(err) else null;

    const input_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(1024 * 1024 * @sizeOf(f16));
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const input_buffer_uav = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    grfx.device.CreateUnorderedAccessView(
        grfx.getResource(input_buffer),
        null,
        &d3d12.UNORDERED_ACCESS_VIEW_DESC.initTypedBuffer(.R16_FLOAT, 0, 1024 * 1024, 0),
        input_buffer_uav,
    );

    const filter_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(3 * 3 * @sizeOf(f16) + 2);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const output_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(1024 * 1024 * @sizeOf(f16));
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const output_buffer_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    grfx.device.CreateShaderResourceView(
        grfx.getResource(output_buffer),
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R16_FLOAT, 0, 1024 * 1024),
        output_buffer_srv,
    );

    const dml_cmd_recorder = blk: {
        var dml_cmd_recorder: *dml.ICommandRecorder = undefined;
        hrPanicOnFail(dml_device.CreateCommandRecorder(
            &dml.IID_ICommandRecorder,
            @ptrCast(*?*c_void, &dml_cmd_recorder),
        ));
        break :blk dml_cmd_recorder;
    };

    //
    // Begin frame to init/upload resources to the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx);

    const image_texture = grfx.createAndUploadTex2dFromFile(
        "content/genart_0025_5.png",
        .{
            .num_mip_levels = 1,
            .texture_flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
        },
    ) catch |err| hrPanic(err);

    const image_texture_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    const image_texture_uav = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    grfx.device.CreateShaderResourceView(grfx.getResource(image_texture), null, image_texture_srv);
    grfx.device.CreateUnorderedAccessView(grfx.getResource(image_texture), null, null, image_texture_uav);

    grfx.addTransitionBarrier(image_texture, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(input_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    grfx.setCurrentPipeline(texture_to_buffer_pso);
    grfx.cmdlist.SetComputeRootDescriptorTable(0, blk: {
        const table = grfx.copyDescriptorsToGpuHeap(1, image_texture_srv);
        _ = grfx.copyDescriptorsToGpuHeap(1, input_buffer_uav);
        break :blk table;
    });
    grfx.cmdlist.Dispatch(128, 128, 1);

    grfx.addTransitionBarrier(image_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(input_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    const conv_op_state = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(conv_info.RequiredDescriptorCount);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, conv_cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = conv_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*c_void, &table)));

        break :blk .{
            .cop = conv_cop,
            .dtbl = table,
            .info = conv_info,
        };
    };

    const init_dtbl = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(init_info.RequiredDescriptorCount + 1);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, init_op),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = init_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*c_void, &table)));
        break :blk table;
    };
    defer _ = init_dtbl.Release();

    if (temp_buffer != null and init_info.TemporaryResourceSize > 0) {
        init_dtbl.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(temp_buffer.?),
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
                .Buffer = grfx.getResource(persistent_buffer.?),
                .Offset = offset,
                .SizeInBytes = conv_info.PersistentResourceSize,
            },
        };
        offset += conv_info.PersistentResourceSize;

        init_dtbl.BindOutputs(1, &[_]dml.BINDING_DESC{binding0});
    }

    dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
        @ptrCast(*dml.IDispatchable, init_op),
        init_dtbl,
    );

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .info_txtfmt = info_txtfmt,
        .dml_device = dml_device,
        .conv_op_state = conv_op_state,
        .temp_buffer = temp_buffer,
        .persistent_buffer = persistent_buffer,
        .input_buffer = input_buffer,
        .input_buffer_uav = input_buffer_uav,
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

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.grfx.releasePipeline(demo.draw_texture_pso);
    _ = demo.grfx.releasePipeline(demo.texture_to_buffer_pso);
    _ = demo.grfx.releasePipeline(demo.buffer_to_texture_pso);
    _ = demo.grfx.releaseResource(demo.image_texture);
    _ = demo.dml_cmd_recorder.Release();
    _ = demo.grfx.releaseResource(demo.input_buffer);
    _ = demo.grfx.releaseResource(demo.filter_buffer);
    _ = demo.grfx.releaseResource(demo.output_buffer);
    if (demo.temp_buffer != null) _ = demo.grfx.releaseResource(demo.temp_buffer.?);
    if (demo.persistent_buffer != null) _ = demo.grfx.releaseResource(demo.persistent_buffer.?);
    _ = demo.conv_op_state.cop.Release();
    _ = demo.conv_op_state.dtbl.Release();
    _ = demo.dml_device.Release();
    _ = demo.brush.Release();
    _ = demo.info_txtfmt.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    lib.newImGuiFrame(demo.frame_stats.delta_time);
}

fn dispatchConvOperator(demo: *DemoState) void {
    var grfx = &demo.grfx;

    // Reset DML binding table.
    {
        const num_descriptors = demo.conv_op_state.info.RequiredDescriptorCount;
        const base_descriptor = grfx.allocateGpuDescriptors(num_descriptors);

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
                .Buffer = grfx.getResource(demo.temp_buffer.?),
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
                .Buffer = grfx.getResource(demo.persistent_buffer.?),
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
                .Buffer = grfx.getResource(demo.input_buffer),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.input_buffer),
            },
        },
        .{ // FilterTensor
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.filter_buffer),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.filter_buffer),
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
            .Buffer = grfx.getResource(demo.output_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.output_buffer),
        },
    }});

    demo.dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
        @ptrCast(*dml.IDispatchable, demo.conv_op_state.cop),
        demo.conv_op_state.dtbl,
    );
}

fn dispatchBarriers(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.cmdlist.ResourceBarrier(
        2,
        &[_]d3d12.RESOURCE_BARRIER{
            .{ .Type = .UAV, .Flags = 0, .u = .{ .UAV = .{ .pResource = grfx.getResource(demo.input_buffer) } } },
            .{ .Type = .UAV, .Flags = 0, .u = .{ .UAV = .{ .pResource = grfx.getResource(demo.output_buffer) } } },
        },
    );
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.addTransitionBarrier(demo.filter_buffer, d3d12.RESOURCE_STATE_COPY_DEST);
    grfx.flushResourceBarriers();

    // Upload the input tensor to the GPU.
    {
        const upload = grfx.allocateUploadBufferRegion(f16, 9);
        upload.cpu_slice[0] = filter_tensor[0][0];
        upload.cpu_slice[1] = filter_tensor[0][1];
        upload.cpu_slice[2] = filter_tensor[0][2];
        upload.cpu_slice[3] = filter_tensor[1][0];
        upload.cpu_slice[4] = filter_tensor[1][1];
        upload.cpu_slice[5] = filter_tensor[1][2];
        upload.cpu_slice[6] = filter_tensor[2][0];
        upload.cpu_slice[7] = filter_tensor[2][1];
        upload.cpu_slice[8] = filter_tensor[2][2];

        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(demo.filter_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    grfx.addTransitionBarrier(demo.filter_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    dispatchConvOperator(demo);
    dispatchBarriers(demo);

    grfx.addTransitionBarrier(demo.output_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(demo.image_texture, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    grfx.setCurrentPipeline(demo.buffer_to_texture_pso);
    grfx.cmdlist.SetComputeRootDescriptorTable(0, blk: {
        const table = grfx.copyDescriptorsToGpuHeap(1, demo.output_buffer_srv);
        _ = grfx.copyDescriptorsToGpuHeap(1, demo.image_texture_uav);
        break :blk table;
    });
    grfx.cmdlist.Dispatch(128, 128, 1);

    grfx.addTransitionBarrier(demo.output_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.addTransitionBarrier(demo.image_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    grfx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = 0.0,
        .TopLeftY = 0.0,
        .Width = @intToFloat(f32, 1024),
        .Height = @intToFloat(f32, 1024),
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});
    grfx.cmdlist.RSSetScissorRects(1, &[_]d3d12.RECT{.{
        .left = 0,
        .top = 0,
        .right = 1024,
        .bottom = 1024,
    }});

    grfx.setCurrentPipeline(demo.draw_texture_pso);
    grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    grfx.cmdlist.SetGraphicsRootDescriptorTable(0, grfx.copyDescriptorsToGpuHeap(1, demo.image_texture_srv));
    grfx.cmdlist.DrawInstanced(3, 1, 0, 0);

    demo.gui.draw(grfx);

    grfx.beginDraw2d();
    {
        const stats = &demo.frame_stats;
        var buffer = [_]u8{0} ** 64;
        const text = std.fmt.bufPrint(
            buffer[0..],
            "FPS: {d:.1}\nCPU time: {d:.3} ms",
            .{ stats.fps, stats.average_cpu_time },
        ) catch unreachable;

        demo.brush.SetColor(&.{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        lib.drawText(
            grfx.d2d.context,
            text,
            demo.info_txtfmt,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, grfx.viewport_width),
                .bottom = @intToFloat(f32, grfx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }
    grfx.endDraw2d();

    grfx.endFrame();
}

pub fn main() !void {
    lib.init();
    defer lib.deinit();

    var gpa_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa_allocator.deinit();
        std.debug.assert(leaked == false);
    }
    const gpa = &gpa_allocator.allocator;

    var demo = init(gpa);
    defer deinit(&demo, gpa);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch false;
        if (has_message) {
            _ = w.user32.translateMessage(&message);
            _ = w.user32.dispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT) {
                break;
            }
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
