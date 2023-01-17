// This intro application shows how to use zmath lib and std.MultiArrayList to efficiently generate dynamic wave
// consisting of more than 1M points on the CPU.

const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const GuiRenderer = common.GuiRenderer;
const c = common.c;
const zm = @import("zmath");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: intro 5";
const window_width = 1920;
const window_height = 1080;

const grid_size = 1024;
const max_num_vertices = grid_size * grid_size;

// By convention, we use 'Pso_' prefix for structures that are also defined in HLSL code
// (see 'DrawConst' and 'FrameConst' in intro5.hlsl).
const Pso_DrawConst = struct {
    object_to_world: [16]f32,
};

const Pso_FrameConst = struct {
    world_to_clip: [16]f32,
};

const Pso_Vertex = struct {
    position: [3]f32,
};

const Vertex = struct {
    x: f32,
    y: f32,
    z: f32,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    main_pso: zd3d12.PipelineHandle,

    vertex_buffer: zd3d12.ResourceHandle,
    vertex_data: std.MultiArrayList(Vertex),

    depth_texture: zd3d12.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    simd_width: i32,

    camera: struct {
        position: [3]f32,
        forward: [3]f32,
        pitch: f32,
        yaw: f32,
    },
    mouse: struct {
        cursor_prev_x: i32,
        cursor_prev_y: i32,
    },
};

fn init(allocator: std.mem.Allocator) !DemoState {
    // Create application window and initialize dear imgui library.
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    // Create temporary memory allocator for use during initialization. We pass this allocator to all
    // subsystems that need memory and then free everyting with a single deallocation.
    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    // Create DirectX 12 context.
    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    const main_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
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
        pso_desc.PrimitiveTopologyType = .POINT;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/intro5.vs.cso",
            content_dir ++ "shaders/intro5.ps.cso",
        );
    };

    // Create vertex buffer and return a *handle* to the underlying Direct3D12 resource.
    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(max_num_vertices * @sizeOf(Vertex)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const vertex_data = blk: {
        var vertex_data = std.MultiArrayList(Vertex){};
        vertex_data.ensureTotalCapacity(allocator, max_num_vertices) catch unreachable;

        var i: u32 = 0;
        while (i < max_num_vertices) : (i += 1) {
            vertex_data.appendAssumeCapacity(.{ .x = 0, .y = 0, .z = 0 });
        }
        break :blk vertex_data;
    };

    // Create depth texture resource.
    const depth_texture = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, gctx.viewport_width, gctx.viewport_height, 1);
            desc.Flags = .{ .ALLOW_DEPTH_STENCIL = true, .DENY_SHADER_RESOURCE = true };
            break :blk desc;
        },
        .{ .DEPTH_WRITE = true },
        &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
    ) catch |err| hrPanic(err);

    // Create depth texture 'view' - a descriptor which can be send to Direct3D 12 API.
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
    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    // This will send command list to the GPU, call 'Present' and do some other bookkeeping.
    gctx.endFrame();

    // Wait for the GPU to finish all commands.
    gctx.finishGpuCommands();

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .main_pso = main_pso,
        .vertex_buffer = vertex_buffer,
        .vertex_data = vertex_data,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .camera = .{
            .position = [3]f32{ 0.0, 5.0, -5.0 },
            .forward = [3]f32{ 0.0, 0.0, 1.0 },
            .pitch = 0.175 * math.pi,
            .yaw = 0.0,
        },
        .mouse = .{
            .cursor_prev_x = 0,
            .cursor_prev_y = 0,
        },
        .simd_width = 4,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    demo.vertex_data.deinit(allocator);
    demo.guir.deinit(&demo.gctx);
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    // Update frame counter and fps stats.
    demo.frame_stats.update(demo.gctx.window, window_name);
    const dt = demo.frame_stats.delta_time;

    // Update dear imgui common. After this call we can define our widgets.
    common.newImGuiFrame(dt);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.gctx.viewport_width) - 600.0 - 20, .y = 20.0 },
        c.ImGuiCond_FirstUseEver,
        c.ImVec2{ .x = 0.0, .y = 0.0 },
    );
    c.igSetNextWindowSize(.{ .x = 600.0, .y = -1 }, c.ImGuiCond_Always);

    _ = c.igBegin(
        "Demo Settings",
        null,
        c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
    );
    c.igBulletText("", "");
    c.igSameLine(0, -1);
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "Right Mouse Button + Drag", "");
    c.igSameLine(0, -1);
    c.igText(" :  rotate camera", "");

    c.igBulletText("", "");
    c.igSameLine(0, -1);
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "W, A, S, D", "");
    c.igSameLine(0, -1);
    c.igText(" :  move camera", "");

    _ = c.igRadioButton_IntPtr("SIMD vector width 4", &demo.simd_width, 4);
    _ = c.igRadioButton_IntPtr("SIMD vector width 8", &demo.simd_width, 8);
    _ = c.igRadioButton_IntPtr("SIMD vector width 16", &demo.simd_width, 16);

    c.igEnd();

    // Handle camera rotation with mouse.
    {
        var pos: w32.POINT = undefined;
        _ = w32.GetCursorPos(&pos);
        const delta_x = @intToFloat(f32, pos.x) - @intToFloat(f32, demo.mouse.cursor_prev_x);
        const delta_y = @intToFloat(f32, pos.y) - @intToFloat(f32, demo.mouse.cursor_prev_y);
        demo.mouse.cursor_prev_x = pos.x;
        demo.mouse.cursor_prev_y = pos.y;

        if (w32.GetAsyncKeyState(w32.VK_RBUTTON) < 0) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = math.min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = math.max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = zm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed = zm.f32x4s(10.0);
        const delta_time = zm.f32x4s(demo.frame_stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.store(demo.camera.forward[0..], forward, 3);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        // Load camera position from memory to SIMD register ('3' means that we want to load three components).
        var cpos = zm.load(demo.camera.position[0..], zm.Vec, 3);

        if (w32.GetAsyncKeyState('W') < 0) {
            cpos += forward;
        } else if (w32.GetAsyncKeyState('S') < 0) {
            cpos -= forward;
        }
        if (w32.GetAsyncKeyState('D') < 0) {
            cpos += right;
        } else if (w32.GetAsyncKeyState('A') < 0) {
            cpos -= right;
        }

        // Copy updated position from SIMD register to memory.
        zm.store(demo.camera.position[0..], cpos, 3);
    }
}

fn computeWaves(comptime T: type, vertex_data: std.MultiArrayList(Vertex), time: f32) void {
    const static = struct {
        const offsets = [16]f32{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    };
    const voffset = zm.load(static.offsets[0..], T, 0);
    const vtime = zm.splat(T, time);

    const xslice = vertex_data.items(.x);
    const yslice = vertex_data.items(.y);
    const zslice = vertex_data.items(.z);

    const scale: f32 = 0.05;

    var z_index: i32 = 0;
    while (z_index < grid_size) : (z_index += 1) {
        const z: f32 = scale * @intToFloat(f32, z_index - grid_size / 2);
        const vz = zm.splat(T, z);

        var x_index: i32 = 0;
        while (x_index < grid_size) : (x_index += zm.veclen(T)) {
            const x: f32 = scale * @intToFloat(f32, x_index - grid_size / 2);
            const vx = zm.splat(T, x) + voffset * zm.splat(T, scale);

            const d = zm.sqrt(vx * vx + vz * vz);

            const vy = zm.sin(d - vtime);

            const index = @intCast(usize, x_index + z_index * grid_size);
            zm.store(xslice[index..], vx, 0);
            zm.store(yslice[index..], vy, 0);
            zm.store(zslice[index..], vz, 0);
        }
    }
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;

    const cam_world_to_view = zm.lookToLh(
        zm.load(demo.camera.position[0..], zm.Vec, 3),
        zm.load(demo.camera.forward[0..], zm.Vec, 3),
        zm.f32x4(0.0, 1.0, 0.0, 0.0),
    );
    const cam_view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
        0.01,
        200.0,
    );
    const cam_world_to_clip = zm.mul(cam_world_to_view, cam_view_to_clip);

    // Begin DirectX 12 rendering.
    gctx.beginFrame();

    // Get current back buffer resource and transition it to 'render target' state.
    const back_buffer = gctx.getBackBuffer();
    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        &demo.depth_texture_dsv,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.2, 0.2, 0.2, 1.0 },
        0,
        null,
    );
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, .{ .DEPTH = true }, 1.0, 0, 0, null);

    // Update dynamic mesh.
    {
        switch (demo.simd_width) {
            4 => computeWaves(zm.F32x4, demo.vertex_data, @floatCast(f32, demo.frame_stats.time)),
            8 => computeWaves(zm.F32x8, demo.vertex_data, @floatCast(f32, demo.frame_stats.time)),
            16 => computeWaves(zm.F32x16, demo.vertex_data, @floatCast(f32, demo.frame_stats.time)),
            else => unreachable,
        }

        const xslice = demo.vertex_data.items(.x);
        const yslice = demo.vertex_data.items(.y);
        const zslice = demo.vertex_data.items(.z);

        const verts = gctx.allocateUploadBufferRegion(Pso_Vertex, @intCast(u32, demo.vertex_data.len));
        var i: usize = 0;
        while (i < demo.vertex_data.len) : (i += 1) {
            verts.cpu_slice[i].position = [3]f32{ xslice[i], yslice[i], zslice[i] };
        }

        gctx.addTransitionBarrier(demo.vertex_buffer, .{ .COPY_DEST = true });
        gctx.flushResourceBarriers();
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(demo.vertex_buffer).?,
            0,
            verts.buffer,
            verts.buffer_offset,
            verts.cpu_slice.len * @sizeOf(@TypeOf(verts.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(demo.vertex_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
        gctx.flushResourceBarriers();
    }

    gctx.setCurrentPipeline(demo.main_pso);

    // Set input assembler (IA) state.
    gctx.cmdlist.IASetPrimitiveTopology(.POINTLIST);
    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = max_num_vertices * @sizeOf(Pso_Vertex),
        .StrideInBytes = @sizeOf(Pso_Vertex),
    }});

    // Upload per-frame constant data (camera xform).
    {
        // Allocate memory for one instance of Pso_FrameConst structure.
        const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);

        // Copy 'cam_world_to_clip' matrix to upload memory. We need to transpose it because
        // HLSL uses column-major matrices by default (zmath uses row-major matrices).
        zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));

        // Set GPU handle of our allocated memory region so that it is visible to the shader.
        gctx.cmdlist.SetGraphicsRootConstantBufferView(
            0, // Slot index in Root Signature (CBV(b0), see intro5.hlsl).
            mem.gpu_base,
        );
    }

    gctx.cmdlist.DrawInstanced(max_num_vertices, 1, 0, 0);

    // Draw dear imgui widgets.
    demo.guir.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
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
