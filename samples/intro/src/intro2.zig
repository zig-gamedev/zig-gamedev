// This intro application shows how to setup a zbuffer, how to load a mesh from file and how to draw it in 3D space.

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
const c = common.c;
const zm = @import("zmath");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 4;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: intro 2";
const window_width = 1920;
const window_height = 1080;

// By convention, we use 'Pso_' prefix for structures that are also defined in HLSL code
// (see 'DrawConst' in intro2.hlsl).
const Pso_DrawConst = struct {
    object_to_clip: [16]f32,
};

// In this intro application vertex consists of position and normal vector.
const Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guictx: GuiRenderer,
    frame_stats: common.FrameStats,

    intro2_pso: zd3d12.PipelineHandle,

    vertex_buffer: zd3d12.ResourceHandle,
    index_buffer: zd3d12.ResourceHandle,

    depth_texture: zd3d12.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    mesh_num_vertices: u32,
    mesh_num_indices: u32,
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
    const intro2_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/intro2.vs.cso",
            content_dir ++ "shaders/intro2.ps.cso",
        );
    };

    // Load a mesh from file and store the data in temporary arrays.
    var mesh_indices = std.ArrayList(u32).init(arena_allocator);
    var mesh_positions = std.ArrayList([3]f32).init(arena_allocator);
    var mesh_normals = std.ArrayList([3]f32).init(arena_allocator);
    {
        const data = common.parseAndLoadGltfFile(content_dir ++ "cube.gltf");
        defer c.cgltf_free(data);
        common.appendMeshPrimitive(data, 0, 0, &mesh_indices, &mesh_positions, &mesh_normals, null, null);
    }
    const mesh_num_indices = @intCast(u32, mesh_indices.items.len);
    const mesh_num_vertices = @intCast(u32, mesh_positions.items.len);

    // Create vertex buffer and return a *handle* to the underlying Direct3D12 resource.
    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(mesh_num_vertices * @sizeOf(Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    // Create index buffer and return a *handle* to the underlying Direct3D12 resource.
    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(mesh_num_indices * @sizeOf(u32)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    // Create depth texture resource.
    const depth_texture = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, gctx.viewport_width, gctx.viewport_height, 1);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | d3d12.RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_DEPTH_WRITE,
        &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
    ) catch |err| hrPanic(err);

    // Create depth texture 'view' - a descriptor which can be send to Direct3D12 API.
    const depth_texture_dsv = gctx.allocateCpuDescriptors(.DSV, 1);
    gctx.device.CreateDepthStencilView(
        gctx.lookupResource(depth_texture).?, // Get the D3D12 resource from a handle.
        null,
        depth_texture_dsv,
    );

    // Open D3D12 command list, setup descriptor heap, etc. After this call we can upload resources to the GPU,
    // draw 3D graphics etc.
    gctx.beginFrame();

    // Create and upload graphics resources for dear imgui renderer.
    var guictx = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    // Fill vertex buffer with vertex data.
    {
        // Allocate memory from upload heap and fill it with vertex data.
        const verts = gctx.allocateUploadBufferRegion(Vertex, mesh_num_vertices);
        for (mesh_positions.items) |_, i| {
            verts.cpu_slice[i].position = mesh_positions.items[i];
            verts.cpu_slice[i].normal = mesh_normals.items[i];
        }

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
        const indices = gctx.allocateUploadBufferRegion(u32, mesh_num_indices);
        for (mesh_indices.items) |_, i| {
            indices.cpu_slice[i] = mesh_indices.items[i];
        }

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
        .intro2_pso = intro2_pso,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .mesh_num_vertices = mesh_num_vertices,
        .mesh_num_indices = mesh_num_indices,
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
        &demo.depth_texture_dsv,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);

    // Set graphics state and draw.
    gctx.setCurrentPipeline(demo.intro2_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = demo.mesh_num_vertices * @sizeOf(Vertex),
        .StrideInBytes = @sizeOf(Vertex),
    }});
    gctx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gctx.lookupResource(demo.index_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = demo.mesh_num_indices * @sizeOf(u32),
        .Format = .R32_UINT,
    });

    // Upload per-draw constant data.
    {
        // Compute transformation matrices.
        const object_to_world = zm.rotationY(@floatCast(f32, demo.frame_stats.time));
        const world_to_view = zm.lookAtLh(
            zm.f32x4(3.0, 3.0, -3.0, 1.0), // eye position
            zm.f32x4(0.0, 0.0, 0.0, 1.0), // focus point
            zm.f32x4(0.0, 1.0, 0.0, 0.0), // up direction ('w' coord is zero because this is a vector not a point)
        );
        const view_to_clip = zm.perspectiveFovLh(
            0.25 * math.pi,
            @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
            0.1,
            20.0,
        );

        const object_to_view = zm.mul(object_to_world, world_to_view);
        const object_to_clip = zm.mul(object_to_view, view_to_clip);

        // Allocate memory for one instance of Pso_DrawConst structure.
        const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);

        // Copy 'object_to_clip' matrix to upload memory. We need to transpose it because
        // HLSL uses column-major matrices by default (zmath uses row-major matrices).
        zm.storeMat(mem.cpu_slice[0].object_to_clip[0..], zm.transpose(object_to_clip));

        // Set GPU handle of our allocated memory region so that it is visible to the shader.
        gctx.cmdlist.SetGraphicsRootConstantBufferView(
            0, // Slot index 0 in Root Signature (CBV(b0), see intro2.hlsl).
            mem.gpu_base,
        );
    }

    gctx.cmdlist.DrawIndexedInstanced(demo.mesh_num_indices, 1, 0, 0, 0);

    // Draw dear imgui widgets (not used in this demo).
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
