// This intro application shows how to draw 2D triangle.

const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w = zwin32.base;
const d3d12 = zwin32.d3d12;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const GuiRenderer = common.GuiRenderer;

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 4;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: intro 1";
const window_width = 1920;
const window_height = 1080;

const Vertex = struct {
    position: [3]f32,
    color: u32,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guictx: GuiRenderer,
    frame_stats: common.FrameStats,

    intro1_pso: zd3d12.PipelineHandle,

    vertex_buffer: zd3d12.ResourceHandle,
    index_buffer: zd3d12.ResourceHandle,
};

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    // Create application window and initialize dear imgui library.
    const window = common.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;

    // Create temporary memory allocator for use during initialization. We pass this allocator to all
    // subsystems that need memory and then free everyting with a single deallocation.
    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    // Create DirectX 12 context.
    var gctx = zd3d12.GraphicsContext.init(window, gpa_allocator);

    // Enable vsync.
    // gctx.present_flags = 0;
    // gctx.present_interval = 1;

    // Create pipeline state object (pso) which is needed to draw geometry - it contains vertex shader,
    // pixel shader and draw state data all linked together.
    const intro1_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Color", 0, .R32_UINT, 0, 12, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DepthStencilState.DepthEnable = w.FALSE;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/intro1.vs.cso",
            content_dir ++ "shaders/intro1.ps.cso",
        );
    };

    // Create vertex buffer and return a *handle* to the underlying Direct3D12 resource.
    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(3 * @sizeOf(Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    // Create index buffer and return a *handle* to the underlying Direct3D12 resource.
    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(3 * @sizeOf(u16)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    // Open D3D12 command list, setup descriptor heap, etc. After this call we can upload resources to the GPU,
    // draw 3D graphics etc.
    gctx.beginFrame();

    // Create and upload graphics resources for dear imgui renderer.
    var guictx = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    // Fill vertex buffer with vertex data.
    {
        // Allocate memory from upload heap and fill it with vertex data.
        const verts = gctx.allocateUploadBufferRegion(Vertex, 3);
        verts.cpu_slice[0].position = [3]f32{ -0.7, -0.7, 0.0 };
        verts.cpu_slice[1].position = [3]f32{ 0.0, 0.7, 0.0 };
        verts.cpu_slice[2].position = [3]f32{ 0.7, -0.7, 0.0 };
        verts.cpu_slice[0].color = 0x00_00_00_ff;
        verts.cpu_slice[1].color = 0x00_00_ff_00;
        verts.cpu_slice[2].color = 0x00_ff_00_00;

        // Copy vertex data from upload heap to vertex buffer resource that resides in high-performance memory
        // on the GPU.
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(vertex_buffer).?,
            0,
            verts.buffer,
            verts.buffer_offset,
            verts.cpu_slice.len * @sizeOf(@TypeOf(verts.cpu_slice[0])),
        );
    }

    // Fill index buffer with index data.
    {
        // Allocate memory from upload heap and fill it with index data.
        const indices = gctx.allocateUploadBufferRegion(u16, 3);
        indices.cpu_slice[0] = 0;
        indices.cpu_slice[1] = 1;
        indices.cpu_slice[2] = 2;

        // Copy index data from upload heap to index buffer resource that resides in high-performance memory
        // on the GPU.
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(index_buffer).?,
            0,
            indices.buffer,
            indices.buffer_offset,
            indices.cpu_slice.len * @sizeOf(@TypeOf(indices.cpu_slice[0])),
        );
    }

    // Transition vertex and index buffers from 'copy dest' state to the state appropriate for rendering.
    gctx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    gctx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_INDEX_BUFFER);
    gctx.flushResourceBarriers();

    // This will send command list to the GPU, call 'Present' and do some other bookkeeping.
    gctx.endFrame();

    // Wait for the GPU to finish all commands.
    gctx.finishGpuCommands();

    return .{
        .gctx = gctx,
        .guictx = guictx,
        .frame_stats = common.FrameStats.init(),
        .intro1_pso = intro1_pso,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
    };
}

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    demo.guictx.deinit(&demo.gctx);
    demo.gctx.deinit(gpa_allocator);
    common.deinitWindow(gpa_allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    // Update frame counter and fps stats.
    demo.frame_stats.update(demo.gctx.window, window_name);
    const dt = demo.frame_stats.delta_time;

    // Update dear imgui common. After this call we can define our widgets.
    common.newImGuiFrame(dt);
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;

    // Begin DirectX 12 rendering.
    gctx.beginFrame();

    // Get current back buffer resource and transition it to 'render target' state.
    const back_buffer = gctx.getBackBuffer();
    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    // Set graphics state and draw.
    gctx.setCurrentPipeline(demo.intro1_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = 3 * @sizeOf(Vertex),
        .StrideInBytes = @sizeOf(Vertex),
    }});
    gctx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gctx.lookupResource(demo.index_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = 3 * @sizeOf(u16),
        .Format = .R16_UINT,
    });
    gctx.cmdlist.DrawIndexedInstanced(3, 1, 0, 0, 0);

    // Draw dear imgui (not used in this demo).
    demo.guictx.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_PRESENT);
    gctx.flushResourceBarriers();

    // Call 'Present' and prepare for the next frame.
    gctx.endFrame();
}

pub fn main() !void {
    // Initialize some low-level Windows stuff (DPI awarness, COM), check Windows version and also check
    // if DirectX 12 Agility SDK is supported.
    common.init();
    defer common.deinit();

    // Create main memory allocator for our application.
    var gpa_allocator_state = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa_allocator_state.deinit();
        std.debug.assert(leaked == false);
    }
    const gpa_allocator = gpa_allocator_state.allocator();

    var demo = init(gpa_allocator);
    defer deinit(&demo, gpa_allocator);

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
