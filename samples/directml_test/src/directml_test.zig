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
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: DirectML test";
const window_width = 1920;
const window_height = 1080;

const tensor_sizes = [_]u32{ 10, 20, 30, 40 };
const tensor_buffer_size: u64 = dml.calcBufferTensorSize(.FLOAT32, tensor_sizes[0..], null);

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    info_tfmt: *dwrite.ITextFormat,

    dml_device: *dml.IDevice1,
    dml_compiled_operator: *dml.ICompiledOperator,
    dml_binding_table: *dml.IBindingTable,
    dml_num_descriptors: u32,

    dml_temp_buffer: gr.ResourceHandle,
    dml_persistent_buffer: gr.ResourceHandle,
    dml_input_buffer: gr.ResourceHandle,
    dml_output_buffer: gr.ResourceHandle,

    dml_cmd_recorder: *dml.ICommandRecorder,
};

fn init(gpa: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;

    _ = pix.loadLatestWinPixGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);

    var dml_device: *dml.IDevice1 = undefined;
    hrPanicOnFail(dml.createDevice(
        @ptrCast(*d3d12.IDevice, grfx.device),
        if (comptime builtin.mode == .Debug) dml.CREATE_DEVICE_FLAG_DEBUG else dml.CREATE_DEVICE_FLAG_NONE,
        .FL_4_1,
        &dml.IID_IDevice1,
        @ptrCast(*?*c_void, &dml_device),
    ));

    const dml_operator = blk: {
        const buffer_tensor_desc = dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT32,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = @intCast(u32, tensor_sizes.len),
            .Sizes = &tensor_sizes,
            .Strides = null,
            .TotalTensorSizeInBytes = tensor_buffer_size,
            .GuaranteedBaseOffsetAlignment = 0,
        };
        const tensor_desc = dml.TENSOR_DESC{
            .Type = .BUFFER,
            .Desc = &buffer_tensor_desc,
        };
        const identity_operator_desc = dml.ELEMENT_WISE_IDENTITY_OPERATOR_DESC{
            .InputTensor = &tensor_desc,
            .OutputTensor = &tensor_desc,
            .ScaleBias = null,
        };
        const operator_desc = dml.OPERATOR_DESC{
            .Type = .ELEMENT_WISE_IDENTITY,
            .Desc = &identity_operator_desc,
        };

        var dml_operator: *dml.IOperator = undefined;
        hrPanicOnFail(dml_device.CreateOperator(&operator_desc, &dml.IID_IOperator, @ptrCast(*?*c_void, &dml_operator)));
        break :blk dml_operator;
    };
    defer _ = dml_operator.Release();

    const dml_compiled_operator = blk: {
        var dml_compiled_operator: *dml.ICompiledOperator = undefined;
        hrPanicOnFail(dml_device.CompileOperator(
            dml_operator,
            dml.EXECUTION_FLAG_NONE,
            &dml.IID_ICompiledOperator,
            @ptrCast(*?*c_void, &dml_compiled_operator),
        ));
        break :blk dml_compiled_operator;
    };

    const dml_init_operator = blk: {
        const operators = [_]*dml.ICompiledOperator{dml_compiled_operator};
        var dml_init_operator: *dml.IOperatorInitializer = undefined;
        hrPanicOnFail(dml_device.CreateOperatorInitializer(
            operators.len,
            &operators,
            &dml.IID_IOperatorInitializer,
            @ptrCast(*?*c_void, &dml_init_operator),
        ));
        break :blk dml_init_operator;
    };
    defer _ = dml_init_operator.Release();

    const exec_binding_info = dml_compiled_operator.GetBindingProperties();
    const init_binding_info = dml_init_operator.GetBindingProperties();
    const dml_num_descriptors = math.max(
        exec_binding_info.RequiredDescriptorCount,
        init_binding_info.RequiredDescriptorCount,
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

    const info_tfmt = blk: {
        var info_tfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            @ptrCast(*?*dwrite.ITextFormat, &info_tfmt),
        ));
        break :blk info_tfmt;
    };
    hrPanicOnFail(info_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(info_tfmt.SetParagraphAlignment(.NEAR));

    const temp_resource_size: u64 = 1 + math.max(
        init_binding_info.TemporaryResourceSize,
        exec_binding_info.TemporaryResourceSize,
    );
    const persistent_resource_size: u64 = 1 + math.max(
        init_binding_info.PersistentResourceSize,
        exec_binding_info.PersistentResourceSize,
    );

    const dml_temp_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(temp_resource_size);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_COMMON,
        null,
    ) catch |err| hrPanic(err);

    const dml_persistent_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(persistent_resource_size),
        d3d12.RESOURCE_STATE_COMMON,
        null,
    ) catch |err| hrPanic(err);

    const dml_input_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(tensor_buffer_size);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const dml_output_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(tensor_buffer_size);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const dml_cmd_recorder = blk: {
        var dml_cmd_recorder: *dml.ICommandRecorder = undefined;
        hrPanicOnFail(dml_device.CreateCommandRecorder(
            &dml.IID_ICommandRecorder,
            @ptrCast(*?*c_void, &dml_cmd_recorder),
        ));
        break :blk dml_cmd_recorder;
    };

    //
    // Begin frame to init/upload resources on the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(gpa, &grfx);

    const dml_binding_table = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(dml_num_descriptors);
        const dml_binding_table_desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, dml_init_operator),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = dml_num_descriptors,
        };
        var dml_binding_table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(
            &dml_binding_table_desc,
            &dml.IID_IBindingTable,
            @ptrCast(*?*c_void, &dml_binding_table),
        ));
        break :blk dml_binding_table;
    };

    if (grfx.getResourceSize(dml_temp_buffer) > 1) {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(dml_temp_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(dml_temp_buffer),
        };
        dml_binding_table.BindTemporaryResource(&.{ .Type = .BUFFER, .Desc = &binding });
    }

    if (grfx.getResourceSize(dml_persistent_buffer) > 1) {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(dml_persistent_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(dml_persistent_buffer),
        };
        dml_binding_table.BindOutputs(1, &[_]dml.BINDING_DESC{.{ .Type = .BUFFER, .Desc = &binding }});
    }

    dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
        @ptrCast(*dml.IDispatchable, dml_init_operator),
        dml_binding_table,
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
        .info_tfmt = info_tfmt,
        .dml_device = dml_device,
        .dml_compiled_operator = dml_compiled_operator,
        .dml_num_descriptors = dml_num_descriptors,
        .dml_binding_table = dml_binding_table,
        .dml_temp_buffer = dml_temp_buffer,
        .dml_persistent_buffer = dml_persistent_buffer,
        .dml_input_buffer = dml_input_buffer,
        .dml_output_buffer = dml_output_buffer,
        .dml_cmd_recorder = dml_cmd_recorder,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.dml_cmd_recorder.Release();
    _ = demo.grfx.releaseResource(demo.dml_input_buffer);
    _ = demo.grfx.releaseResource(demo.dml_output_buffer);
    _ = demo.grfx.releaseResource(demo.dml_temp_buffer);
    _ = demo.grfx.releaseResource(demo.dml_persistent_buffer);
    _ = demo.dml_binding_table.Release();
    _ = demo.dml_compiled_operator.Release();
    _ = demo.dml_device.Release();
    _ = demo.brush.Release();
    _ = demo.info_tfmt.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(gpa);
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    lib.newImGuiFrame(demo.frame_stats.delta_time);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.addTransitionBarrier(demo.dml_input_buffer, d3d12.RESOURCE_STATE_COPY_DEST);
    grfx.flushResourceBarriers();

    // Upload the input tensor to the GPU.
    {
        const num_elements = @intCast(u32, @divExact(grfx.getResourceSize(demo.dml_input_buffer), 4));
        const upload = grfx.allocateUploadBufferRegion(f32, num_elements);
        var i: u32 = 0;
        while (i < num_elements) : (i += 1) {
            upload.cpu_slice[i] = 1.618;
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(demo.dml_input_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    grfx.addTransitionBarrier(demo.dml_input_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    // Set DML binding table.
    {
        const base_descriptor = grfx.allocateGpuDescriptors(demo.dml_num_descriptors);
        hrPanicOnFail(demo.dml_binding_table.Reset(&dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, demo.dml_compiled_operator),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = demo.dml_num_descriptors,
        }));
    }

    // If necessary, bind temporary buffer.
    if (grfx.getResourceSize(demo.dml_temp_buffer) > 1) {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.dml_temp_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.dml_temp_buffer),
        };
        demo.dml_binding_table.BindTemporaryResource(&.{ .Type = .BUFFER, .Desc = &binding });
    }

    // If necessary, bind persistent buffer.
    if (grfx.getResourceSize(demo.dml_persistent_buffer) > 1) {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.dml_persistent_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.dml_persistent_buffer),
        };
        demo.dml_binding_table.BindPersistentResource(&.{ .Type = .BUFFER, .Desc = &binding });
    }

    // Bind input buffer.
    {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.dml_input_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.dml_input_buffer),
        };
        demo.dml_binding_table.BindInputs(1, &[_]dml.BINDING_DESC{.{ .Type = .BUFFER, .Desc = &binding }});
    }

    // Bind output buffer.
    {
        const binding = dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.dml_output_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.dml_output_buffer),
        };
        demo.dml_binding_table.BindOutputs(1, &[_]dml.BINDING_DESC{.{ .Type = .BUFFER, .Desc = &binding }});
    }

    demo.dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
        @ptrCast(*dml.IDispatchable, demo.dml_compiled_operator),
        demo.dml_binding_table,
    );

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
        lib.DrawText(
            grfx.d2d.context,
            text,
            demo.info_tfmt,
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
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

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
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch unreachable;
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
