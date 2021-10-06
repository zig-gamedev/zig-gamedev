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

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    info_txtfmt: *dwrite.ITextFormat,

    dml_device: *dml.IDevice1,
    dml_compiled_operator: *dml.ICompiledOperator,
    dml_binding_table: *dml.IBindingTable,

    num_descriptors: u32,

    temp_buffer: ?gr.ResourceHandle,
    persistent_buffer: ?gr.ResourceHandle,
    input_buffer: gr.ResourceHandle,
    output_buffer: gr.ResourceHandle,

    dml_cmd_recorder: *dml.ICommandRecorder,
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

    var dml_device: *dml.IDevice1 = undefined;
    hrPanicOnFail(dml.createDevice(
        @ptrCast(*d3d12.IDevice, grfx.device),
        if (enable_dx_debug) dml.CREATE_DEVICE_FLAG_DEBUG else dml.CREATE_DEVICE_FLAG_NONE,
        .FL_4_1,
        &dml.IID_IDevice1,
        @ptrCast(*?*c_void, &dml_device),
    ));

    const input_state_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .UINT32,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 4,
            .Sizes = &[_]u32{ 1, 1, 1, 6 },
            .Strides = null,
            .TotalTensorSizeInBytes = 6 * @sizeOf(u32),
            .GuaranteedBaseOffsetAlignment = 0,
        },
    };

    const output_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .UINT32,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 1,
            .Sizes = &[_]u32{128},
            .Strides = null,
            .TotalTensorSizeInBytes = 128 * @sizeOf(u32),
            .GuaranteedBaseOffsetAlignment = 0,
        },
    };

    const dml_operator = blk: {
        const operator_desc = dml.OPERATOR_DESC{
            .Type = .RANDOM_GENERATOR,
            .Desc = &dml.RANDOM_GENERATOR_OPERATOR_DESC{
                .InputStateTensor = &input_state_tensor_desc,
                .OutputTensor = &output_tensor_desc,
                .OutputStateTensor = &input_state_tensor_desc,
                .Type = .PHILOX_4X32_10,
            },
        };

        var dml_operator: *dml.IOperator = undefined;
        hrPanicOnFail(dml_device.CreateOperator(
            &operator_desc,
            &dml.IID_IOperator,
            @ptrCast(*?*c_void, &dml_operator),
        ));
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
    const num_descriptors = math.max(
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

    const temp_resource_size: u64 = math.max(
        init_binding_info.TemporaryResourceSize,
        exec_binding_info.TemporaryResourceSize,
    );
    const persistent_resource_size: u64 = math.max(
        init_binding_info.PersistentResourceSize,
        exec_binding_info.PersistentResourceSize,
    );

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
        &d3d12.RESOURCE_DESC.initBuffer(persistent_resource_size),
        d3d12.RESOURCE_STATE_COMMON,
        null,
    ) catch |err| hrPanic(err) else null;

    const input_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(
                @ptrCast(*const dml.BUFFER_TENSOR_DESC, input_state_tensor_desc.Desc).TotalTensorSizeInBytes,
            );
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
            var desc = d3d12.RESOURCE_DESC.initBuffer(
                @ptrCast(*const dml.BUFFER_TENSOR_DESC, output_tensor_desc.Desc).TotalTensorSizeInBytes + 32,
            );
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
    // Begin frame to init/upload resources to the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx);

    const dml_binding_table = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(num_descriptors);

        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, dml_init_operator),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = num_descriptors,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*c_void, &table)));
        break :blk table;
    };

    if (temp_buffer != null) {
        dml_binding_table.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(temp_buffer.?),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(temp_buffer.?),
            },
        });
    }

    if (persistent_buffer != null) {
        dml_binding_table.BindOutputs(1, &[_]dml.BINDING_DESC{.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(persistent_buffer.?),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(persistent_buffer.?),
            },
        }});
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
        .info_txtfmt = info_txtfmt,
        .dml_device = dml_device,
        .dml_compiled_operator = dml_compiled_operator,
        .num_descriptors = num_descriptors,
        .dml_binding_table = dml_binding_table,
        .temp_buffer = temp_buffer,
        .persistent_buffer = persistent_buffer,
        .input_buffer = input_buffer,
        .output_buffer = output_buffer,
        .dml_cmd_recorder = dml_cmd_recorder,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.dml_cmd_recorder.Release();
    _ = demo.grfx.releaseResource(demo.input_buffer);
    _ = demo.grfx.releaseResource(demo.output_buffer);
    if (demo.temp_buffer != null) _ = demo.grfx.releaseResource(demo.temp_buffer.?);
    if (demo.persistent_buffer != null) _ = demo.grfx.releaseResource(demo.persistent_buffer.?);
    _ = demo.dml_binding_table.Release();
    _ = demo.dml_compiled_operator.Release();
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

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.addTransitionBarrier(demo.input_buffer, d3d12.RESOURCE_STATE_COPY_DEST);
    grfx.flushResourceBarriers();

    // Upload the input tensor to the GPU.
    {
        const upload = grfx.allocateUploadBufferRegion(u32, 6);
        upload.cpu_slice[0] = 0;
        upload.cpu_slice[1] = 0;
        upload.cpu_slice[2] = 0;
        upload.cpu_slice[3] = 0;
        upload.cpu_slice[4] = 123;
        upload.cpu_slice[5] = 987;

        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(demo.input_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    grfx.addTransitionBarrier(demo.input_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    grfx.flushResourceBarriers();

    // Reset DML binding table.
    {
        const base_descriptor = grfx.allocateGpuDescriptors(demo.num_descriptors);

        hrPanicOnFail(demo.dml_binding_table.Reset(&dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, demo.dml_compiled_operator),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = demo.num_descriptors,
        }));
    }

    // If necessary, bind temporary buffer.
    if (demo.temp_buffer != null) {
        demo.dml_binding_table.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.temp_buffer.?),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.temp_buffer.?),
            },
        });
    }

    // If necessary, bind persistent buffer.
    if (demo.persistent_buffer != null) {
        demo.dml_binding_table.BindPersistentResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.persistent_buffer.?),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.persistent_buffer.?),
            },
        });
    }

    // Bind input buffer.
    demo.dml_binding_table.BindInputs(1, &[_]dml.BINDING_DESC{.{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.input_buffer),
            .Offset = 0,
            .SizeInBytes = grfx.getResourceSize(demo.input_buffer),
        },
    }});

    // Bind output buffers.
    demo.dml_binding_table.BindOutputs(2, &[_]dml.BINDING_DESC{
        .{ // Output tensor.
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.output_buffer),
                .Offset = 32, // 24 bytes aligned to 32
                .SizeInBytes = grfx.getResourceSize(demo.output_buffer) - 32,
            },
        },
        .{ // Output state tensor (first 24 bytes of the buffer).
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.output_buffer),
                .Offset = 0,
                .SizeInBytes = 6 * @sizeOf(u32),
            },
        },
    });

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
