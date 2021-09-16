const builtin = @import("builtin");
const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d3d12 = win32.d3d12;
const d2d1 = win32.d2d1;
const dwrite = win32.dwrite;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const vm = common.vectormath;
const math = std.math;
const hrPanicOnFail = lib.hrPanicOnFail;
const hrPanic = lib.hrPanic;
const utf8ToUtf16LeStringLiteral = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const num_mipmaps = 5;

const Vertex = struct {
    position: vm.Vec3,
    uv: vm.Vec2,
};
comptime {
    std.debug.assert(@sizeOf([2]Vertex) == 40);
    std.debug.assert(@alignOf([2]Vertex) == 4);
}

const DemoState = struct {
    const window_name = "zig-gamedev: textured quad";
    const window_width = 1024;
    const window_height = 1024;

    window: w.HWND,
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,
    pipeline: gr.PipelineHandle,
    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,
    texture: gr.ResourceHandle,
    texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    brush: *d2d1.ISolidColorBrush,
    textformat: *dwrite.ITextFormat,
    mipmap_level: i32,

    fn init(allocator: *std.mem.Allocator) DemoState {
        const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
        var grfx = gr.GraphicsContext.init(window);

        const pipeline = blk: {
            const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
                d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                d3d12.INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
            pso_desc.RasterizerState.CullMode = .NONE;
            pso_desc.DepthStencilState.DepthEnable = w.FALSE;
            pso_desc.InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            };
            break :blk grfx.createGraphicsShaderPipeline(
                allocator,
                &pso_desc,
                "content/shaders/textured_quad.vs.cso",
                "content/shaders/textured_quad.ps.cso",
            );
        };

        const vertex_buffer = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(num_mipmaps * 4 * @sizeOf(Vertex)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);

        const index_buffer = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(4 * @sizeOf(u32)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);

        const brush = blk: {
            var maybe_brush: ?*d2d1.ISolidColorBrush = null;
            hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
                &d2d1.COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
                null,
                &maybe_brush,
            ));
            break :blk maybe_brush.?;
        };

        const textformat = blk: {
            var maybe_textformat: ?*dwrite.ITextFormat = null;
            hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
                utf8ToUtf16LeStringLiteral("Verdana"),
                null,
                dwrite.FONT_WEIGHT.NORMAL,
                dwrite.FONT_STYLE.NORMAL,
                dwrite.FONT_STRETCH.NORMAL,
                32.0,
                utf8ToUtf16LeStringLiteral("en-us"),
                &maybe_textformat,
            ));
            break :blk maybe_textformat.?;
        };
        hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
        hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

        var mipgen = gr.MipmapGenerator.init(allocator, &grfx, .R8G8B8A8_UNORM);

        grfx.beginFrame();

        const gui = gr.GuiContext.init(allocator, &grfx);

        const texture = grfx.createAndUploadTex2dFromFile(
            "content/genart_0025_5.png",
            0, // Create complete mipmap chain (up to 1x1).
        ) catch |err| hrPanic(err);

        mipgen.generateMipmaps(&grfx, texture);

        const texture_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(texture),
            &d3d12.SHADER_RESOURCE_VIEW_DESC{
                .Format = .UNKNOWN,
                .ViewDimension = .TEXTURE2D,
                .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
                .u = .{
                    .Texture2D = .{
                        .MostDetailedMip = 0,
                        .MipLevels = 0xffff_ffff,
                        .PlaneSlice = 0,
                        .ResourceMinLODClamp = 0.0,
                    },
                },
            },
            texture_srv,
        );

        // Fill vertex buffer.
        {
            const upload_verts = grfx.allocateUploadBufferRegion(Vertex, num_mipmaps * 4);
            var mipmap_index: u32 = 0;
            var r: f32 = 1.0;
            while (mipmap_index < num_mipmaps) : (mipmap_index += 1) {
                const index = mipmap_index * 4;
                upload_verts.cpu_slice[index] = .{
                    .position = vm.Vec3.init(-r, r, 0.0),
                    .uv = vm.Vec2.init(0.0, 0.0),
                };
                upload_verts.cpu_slice[index + 1] = .{
                    .position = vm.Vec3.init(r, r, 0.0),
                    .uv = vm.Vec2.init(1.0, 0.0),
                };
                upload_verts.cpu_slice[index + 2] = .{
                    .position = vm.Vec3.init(-r, -r, 0.0),
                    .uv = vm.Vec2.init(0.0, 1.0),
                };
                upload_verts.cpu_slice[index + 3] = .{
                    .position = vm.Vec3.init(r, -r, 0.0),
                    .uv = vm.Vec2.init(1.0, 1.0),
                };
                r *= 0.5;
            }
            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(vertex_buffer),
                0,
                upload_verts.buffer,
                upload_verts.buffer_offset,
                upload_verts.cpu_slice.len * @sizeOf(@TypeOf(upload_verts.cpu_slice[0])),
            );
        }
        // Fill index buffer.
        {
            const upload_indices = grfx.allocateUploadBufferRegion(u32, 4);
            upload_indices.cpu_slice[0] = 0;
            upload_indices.cpu_slice[1] = 1;
            upload_indices.cpu_slice[2] = 2;
            upload_indices.cpu_slice[3] = 3;

            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(index_buffer),
                0,
                upload_indices.buffer,
                upload_indices.buffer_offset,
                upload_indices.cpu_slice.len * @sizeOf(@TypeOf(upload_indices.cpu_slice[0])),
            );
        }

        grfx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        grfx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_INDEX_BUFFER);
        grfx.addTransitionBarrier(texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();

        grfx.finishGpuCommands();

        // NOTE(mziulek):
        // We need to call 'deinit' explicitly - we can't rely on 'defer' in this case because it runs *after*
        // 'grfx' is copied in 'return' statement below.
        mipgen.deinit(&grfx);

        return .{
            .grfx = grfx,
            .gui = gui,
            .window = window,
            .frame_stats = lib.FrameStats.init(),
            .pipeline = pipeline,
            .vertex_buffer = vertex_buffer,
            .index_buffer = index_buffer,
            .texture = texture,
            .texture_srv = texture_srv,
            .brush = brush,
            .textformat = textformat,
            .mipmap_level = 1,
        };
    }

    fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
        demo.grfx.finishGpuCommands();
        _ = demo.brush.Release();
        _ = demo.textformat.Release();
        _ = demo.grfx.releaseResource(demo.vertex_buffer);
        _ = demo.grfx.releaseResource(demo.index_buffer);
        _ = demo.grfx.releaseResource(demo.texture);
        _ = demo.grfx.releasePipeline(demo.pipeline);
        demo.gui.deinit(&demo.grfx);
        demo.grfx.deinit(allocator);
        lib.deinitWindow(allocator);
        demo.* = undefined;
    }

    fn update(demo: *DemoState) void {
        demo.frame_stats.update();

        lib.newImGuiFrame(demo.frame_stats.delta_time);

        c.igSetNextWindowPos(c.ImVec2{ .x = 10.0, .y = 100.0 }, c.ImGuiCond_FirstUseEver, c.ImVec2{ .x = 0.0, .y = 0.0 });
        c.igSetNextWindowSize(c.ImVec2{ .x = 600.0, .y = 0.0 }, c.ImGuiCond_FirstUseEver);
        _ = c.igBegin(
            "Demo Settings",
            null,
            c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
        );
        _ = c.igSliderInt("Mipmap Level", &demo.mipmap_level, 0, num_mipmaps - 1, null, c.ImGuiSliderFlags_None);
        c.igEnd();
    }

    fn draw(demo: *DemoState) void {
        var grfx = &demo.grfx;
        grfx.beginFrame();

        const back_buffer = grfx.getBackBuffer();

        grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
        grfx.flushResourceBarriers();

        grfx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w.TRUE,
            null,
        );
        grfx.cmdlist.ClearRenderTargetView(
            back_buffer.descriptor_handle,
            &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );
        grfx.setCurrentPipeline(demo.pipeline);
        grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLESTRIP);
        grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
            .BufferLocation = grfx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = num_mipmaps * 4 * @sizeOf(Vertex),
            .StrideInBytes = @sizeOf(Vertex),
        }});
        grfx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = grfx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = 4 * @sizeOf(u32),
            .Format = .R32_UINT,
        });
        grfx.cmdlist.SetGraphicsRootDescriptorTable(0, grfx.copyDescriptorsToGpuHeap(1, demo.texture_srv));
        grfx.cmdlist.DrawIndexedInstanced(4, 1, 0, demo.mipmap_level * 4, 0);

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

            demo.brush.SetColor(&d2d1.COLOR_F{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 });
            lib.DrawText(
                grfx.d2d.context,
                text,
                demo.textformat,
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
};

pub fn main() !void {
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = DemoState.init(allocator);
    defer demo.deinit(allocator);

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
            demo.update();
            demo.draw();
        }
    }
}
