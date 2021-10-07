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

    rng_op_state: OperatorState,
    cast_op_state: OperatorState,

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

    const state_tensor_desc = dml.TENSOR_DESC{
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

    const u32_data_tensor_desc = dml.TENSOR_DESC{
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

    const f32_data_tensor_desc = dml.TENSOR_DESC{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_TENSOR_DESC{
            .DataType = .FLOAT32,
            .Flags = dml.TENSOR_FLAG_NONE,
            .DimensionCount = 1,
            .Sizes = &[_]u32{128},
            .Strides = null,
            .TotalTensorSizeInBytes = 128 * @sizeOf(f32),
            .GuaranteedBaseOffsetAlignment = 0,
        },
    };

    const rng_op = blk: {
        const desc = dml.OPERATOR_DESC{
            .Type = .RANDOM_GENERATOR,
            .Desc = &dml.RANDOM_GENERATOR_OPERATOR_DESC{
                .InputStateTensor = &state_tensor_desc,
                .OutputTensor = &u32_data_tensor_desc,
                .OutputStateTensor = null,
                .Type = .PHILOX_4X32_10,
            },
        };

        var op: *dml.IOperator = undefined;
        hrPanicOnFail(dml_device.CreateOperator(&desc, &dml.IID_IOperator, @ptrCast(*?*c_void, &op)));
        break :blk op;
    };
    defer _ = rng_op.Release();

    const cast_op = blk: {
        const desc = dml.OPERATOR_DESC{
            .Type = .CAST,
            .Desc = &dml.CAST_OPERATOR_DESC{
                .InputTensor = &u32_data_tensor_desc,
                .OutputTensor = &f32_data_tensor_desc,
            },
        };

        var op: *dml.IOperator = undefined;
        hrPanicOnFail(dml_device.CreateOperator(&desc, &dml.IID_IOperator, @ptrCast(*?*c_void, &op)));
        break :blk op;
    };
    defer _ = cast_op.Release();

    const rng_cop = blk: {
        var cop: *dml.ICompiledOperator = undefined;
        hrPanicOnFail(dml_device.CompileOperator(
            rng_op,
            dml.EXECUTION_FLAG_NONE,
            &dml.IID_ICompiledOperator,
            @ptrCast(*?*c_void, &cop),
        ));
        break :blk cop;
    };

    const cast_cop = blk: {
        var cop: *dml.ICompiledOperator = undefined;
        hrPanicOnFail(dml_device.CompileOperator(
            cast_op,
            dml.EXECUTION_FLAG_NONE,
            &dml.IID_ICompiledOperator,
            @ptrCast(*?*c_void, &cop),
        ));
        break :blk cop;
    };

    const init_op = blk: {
        const operators = [_]*dml.ICompiledOperator{ rng_cop, cast_cop };

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

    const rng_info = rng_cop.GetBindingProperties();
    const cast_info = cast_cop.GetBindingProperties();
    const init_info = init_op.GetBindingProperties();

    const temp_resource_size: u64 = math.max(
        init_info.TemporaryResourceSize,
        rng_info.TemporaryResourceSize + cast_info.TemporaryResourceSize,
    );
    const persistent_resource_size: u64 = rng_info.PersistentResourceSize + cast_info.PersistentResourceSize;

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
                @ptrCast(*const dml.BUFFER_TENSOR_DESC, u32_data_tensor_desc.Desc).TotalTensorSizeInBytes,
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
                @ptrCast(*const dml.BUFFER_TENSOR_DESC, u32_data_tensor_desc.Desc).TotalTensorSizeInBytes,
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

    const rng_op_state = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(rng_info.RequiredDescriptorCount);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, rng_cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = rng_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*c_void, &table)));

        break :blk .{
            .cop = rng_cop,
            .dtbl = table,
            .info = rng_info,
        };
    };

    const cast_op_state = blk: {
        const base_descriptor = grfx.allocateGpuDescriptors(cast_info.RequiredDescriptorCount);
        const desc = dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, cast_cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = cast_info.RequiredDescriptorCount,
        };

        var table: *dml.IBindingTable = undefined;
        hrPanicOnFail(dml_device.CreateBindingTable(&desc, &dml.IID_IBindingTable, @ptrCast(*?*c_void, &table)));

        break :blk .{
            .cop = cast_cop,
            .dtbl = table,
            .info = cast_info,
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
        const binding0 = if (rng_info.PersistentResourceSize == 0) dml.BINDING_DESC{
            .Type = .NONE,
            .Desc = null,
        } else dml.BINDING_DESC{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(persistent_buffer.?),
                .Offset = offset,
                .SizeInBytes = rng_info.PersistentResourceSize,
            },
        };
        offset += rng_info.PersistentResourceSize;

        const binding1 = if (cast_info.PersistentResourceSize == 0) dml.BINDING_DESC{
            .Type = .NONE,
            .Desc = null,
        } else dml.BINDING_DESC{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(persistent_buffer.?),
                .Offset = offset,
                .SizeInBytes = cast_info.PersistentResourceSize,
            },
        };
        offset += cast_info.PersistentResourceSize;

        init_dtbl.BindOutputs(2, &[_]dml.BINDING_DESC{ binding0, binding1 });
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
        .rng_op_state = rng_op_state,
        .cast_op_state = cast_op_state,
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
    _ = demo.rng_op_state.cop.Release();
    _ = demo.rng_op_state.dtbl.Release();
    _ = demo.cast_op_state.cop.Release();
    _ = demo.cast_op_state.dtbl.Release();
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
        const num_descriptors = demo.rng_op_state.info.RequiredDescriptorCount;
        const base_descriptor = grfx.allocateGpuDescriptors(num_descriptors);

        hrPanicOnFail(demo.rng_op_state.dtbl.Reset(&dml.BINDING_TABLE_DESC{
            .Dispatchable = @ptrCast(*dml.IDispatchable, demo.rng_op_state.cop),
            .CPUDescriptorHandle = base_descriptor.cpu_handle,
            .GPUDescriptorHandle = base_descriptor.gpu_handle,
            .SizeInDescriptors = num_descriptors,
        }));
    }

    // If necessary, bind temporary buffer.
    if (demo.temp_buffer != null and demo.rng_op_state.info.TemporaryResourceSize > 0) {
        demo.rng_op_state.dtbl.BindTemporaryResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.temp_buffer.?),
                .Offset = 0,
                .SizeInBytes = demo.rng_op_state.info.TemporaryResourceSize,
            },
        });
    }

    // If necessary, bind persistent buffer.
    if (demo.persistent_buffer != null and demo.rng_op_state.info.PersistentResourceSize > 0) {
        demo.rng_op_state.dtbl.BindPersistentResource(&.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.persistent_buffer.?),
                .Offset = 0,
                .SizeInBytes = demo.rng_op_state.info.PersistentResourceSize,
            },
        });
    }

    // Bind input buffer.
    demo.rng_op_state.dtbl.BindInputs(1, &[_]dml.BINDING_DESC{.{
        .Type = .BUFFER,
        .Desc = &dml.BUFFER_BINDING{
            .Buffer = grfx.getResource(demo.input_buffer),
            .Offset = 0,
            .SizeInBytes = 6 * @sizeOf(u32),
        },
    }});

    // Bind output buffers.
    demo.rng_op_state.dtbl.BindOutputs(2, &[_]dml.BINDING_DESC{
        .{ // Output tensor.
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.output_buffer),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.output_buffer),
            },
        },
        .{ .Type = .NONE, .Desc = null }, // We do not need output state.
    });

    demo.dml_cmd_recorder.RecordDispatch(
        @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
        @ptrCast(*dml.IDispatchable, demo.rng_op_state.cop),
        demo.rng_op_state.dtbl,
    );

    if (false) {
        // Reset DML binding table.
        {
            const num_descriptors = demo.cast_info.RequiredDescriptorCount;
            const base_descriptor = grfx.allocateGpuDescriptors(num_descriptors);

            hrPanicOnFail(demo.dml_binding_table.Reset(&dml.BINDING_TABLE_DESC{
                .Dispatchable = @ptrCast(*dml.IDispatchable, demo.cast_cop),
                .CPUDescriptorHandle = base_descriptor.cpu_handle,
                .GPUDescriptorHandle = base_descriptor.gpu_handle,
                .SizeInDescriptors = num_descriptors,
            }));
        }

        // If necessary, bind temporary buffer.
        if (demo.temp_buffer != null and demo.cast_info.TemporaryResourceSize > 0) {
            demo.dml_binding_table.BindTemporaryResource(&.{
                .Type = .BUFFER,
                .Desc = &dml.BUFFER_BINDING{
                    .Buffer = grfx.getResource(demo.temp_buffer.?),
                    .Offset = demo.random_generator_info.TemporaryResourceSize,
                    .SizeInBytes = demo.cast_info.TemporaryResourceSize,
                },
            });
        }

        // If necessary, bind persistent buffer.
        if (demo.persistent_buffer != null and demo.cast_info.PersistentResourceSize > 0) {
            demo.dml_binding_table.BindPersistentResource(&.{
                .Type = .BUFFER,
                .Desc = &dml.BUFFER_BINDING{
                    .Buffer = grfx.getResource(demo.persistent_buffer.?),
                    .Offset = demo.random_generator_info.TemporaryResourceSize,
                    .SizeInBytes = demo.cast_info.PersistentResourceSize,
                },
            });
        }

        // Bind input buffer.
        demo.dml_binding_table.BindInputs(1, &[_]dml.BINDING_DESC{.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.output_buffer),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.output_buffer),
            },
        }});

        // Bind output buffers.
        demo.dml_binding_table.BindOutputs(1, &[_]dml.BINDING_DESC{.{
            .Type = .BUFFER,
            .Desc = &dml.BUFFER_BINDING{
                .Buffer = grfx.getResource(demo.input_buffer),
                .Offset = 0,
                .SizeInBytes = grfx.getResourceSize(demo.input_buffer),
            },
        }});

        demo.dml_cmd_recorder.RecordDispatch(
            @ptrCast(*d3d12.ICommandList, grfx.cmdlist),
            @ptrCast(*dml.IDispatchable, demo.cast_cop),
            demo.dml_binding_table,
        );
    }

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
