const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const common = @import("common");
const gfx = common.graphics;
const lib = common.library;
const c = common.c;
const zm = @import("zmath");

const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: rasterization";
const window_width = 1920;
const window_height = 1080;

// Compute shader group size in 'csClearPixels' and 'csDrawPixels' (rasterization.hlsl).
const compute_group_size = 32;

const Pso_DrawConst = struct {
    object_to_world: [16]f32,
};

const Pso_FrameConst = struct {
    world_to_clip: [16]f32,
    camera_position: [3]f32,
};

const Pso_Pixel = struct {
    position: [2]f32,
    color: [3]f32,
};

const Pso_Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
    texcoord: [2]f32,
    tangent: [4]f32,
};

const DemoState = struct {
    gctx: gfx.GraphicsContext,
    guictx: gfx.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    normal_tfmt: *dwrite.ITextFormat,

    z_pre_pass_pso: gfx.PipelineHandle,
    record_pixels_pso: gfx.PipelineHandle,
    draw_pixels_pso: gfx.PipelineHandle,
    clear_pixels_pso: gfx.PipelineHandle,
    draw_mesh_pso: gfx.PipelineHandle,

    vertex_buffer: gfx.ResourceHandle,
    index_buffer: gfx.ResourceHandle,

    depth_texture: gfx.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    pixel_buffer: gfx.ResourceHandle,
    pixel_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    pixel_buffer_uav: d3d12.CPU_DESCRIPTOR_HANDLE,

    pixel_texture: gfx.ResourceHandle,
    pixel_texture_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
    pixel_texture_rtv: d3d12.CPU_DESCRIPTOR_HANDLE,

    mesh_num_vertices: u32,
    mesh_num_indices: u32,

    draw_wireframe: bool,
    num_pixel_groups: u32,
    raster_speed: i32,

    mesh_textures: [4]gfx.ResourceHandle,

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

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const window = lib.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;

    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    var gctx = gfx.GraphicsContext.init(window);

    // Enable vsync.
    gctx.present_flags = 0;
    gctx.present_interval = 1;

    const brush = blk: {
        var brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(gctx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &brush,
        ));
        break :blk brush.?;
    };

    const normal_tfmt = blk: {
        var info_txtfmt: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(gctx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            .BOLD,
            .NORMAL,
            .NORMAL,
            32.0,
            L("en-us"),
            &info_txtfmt,
        ));
        break :blk info_txtfmt.?;
    };
    hrPanicOnFail(normal_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(normal_tfmt.SetParagraphAlignment(.NEAR));

    const z_pre_pass_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.RasterizerState.CullMode = .NONE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            "content/shaders/draw_mesh.vs.cso",
            null,
        );
    };

    const record_pixels_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Texcoord", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Tangent", 0, .R32G32B32A32_FLOAT, 0, 32, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.DepthStencilState.DepthWriteMask = .ZERO;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.RasterizerState.CullMode = .NONE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            "content/shaders/record_pixels.vs.cso",
            "content/shaders/record_pixels.ps.cso",
        );
    };

    const draw_mesh_pso = blk: {
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
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.RasterizerState.FillMode = .WIREFRAME;
        pso_desc.RasterizerState.CullMode = .NONE;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.DepthStencilState.DepthWriteMask = .ZERO;
        pso_desc.RasterizerState.AntialiasedLineEnable = w.TRUE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            "content/shaders/draw_mesh.vs.cso",
            "content/shaders/draw_mesh.ps.cso",
        );
    };

    const draw_pixels_pso = gctx.createComputeShaderPipeline(
        arena_allocator,
        &d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault(),
        "content/shaders/draw_pixels.cs.cso",
    );

    const clear_pixels_pso = gctx.createComputeShaderPipeline(
        arena_allocator,
        &d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault(),
        "content/shaders/clear_pixels.cs.cso",
    );

    var mesh_indices = std.ArrayList(u32).init(arena_allocator);
    var mesh_positions = std.ArrayList([3]f32).init(arena_allocator);
    var mesh_normals = std.ArrayList([3]f32).init(arena_allocator);
    var mesh_texcoords = std.ArrayList([2]f32).init(arena_allocator);
    var mesh_tangents = std.ArrayList([4]f32).init(arena_allocator);
    {
        const data = lib.parseAndLoadGltfFile("content/SciFiHelmet/SciFiHelmet.gltf");
        defer c.cgltf_free(data);
        lib.appendMeshPrimitive(
            data,
            0,
            0,
            &mesh_indices,
            &mesh_positions,
            &mesh_normals,
            &mesh_texcoords,
            &mesh_tangents,
        );
    }
    const mesh_num_indices = @intCast(u32, mesh_indices.items.len);
    const mesh_num_vertices = @intCast(u32, mesh_positions.items.len);

    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(mesh_num_vertices * @sizeOf(Pso_Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(mesh_num_indices * @sizeOf(u32)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

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

    const depth_texture_dsv = gctx.allocateCpuDescriptors(.DSV, 1);
    gctx.device.CreateDepthStencilView(
        gctx.getResource(depth_texture), // Get the D3D12 resource from a handle.
        null,
        depth_texture_dsv,
    );

    const pixel_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_ALLOW_SHADER_ATOMICS,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initBuffer(
                (gctx.viewport_width * gctx.viewport_height + 1) * @sizeOf(Pso_Pixel),
            );
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        null,
    ) catch |err| hrPanic(err);

    const pixel_buffer_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    const pixel_buffer_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    gctx.device.CreateShaderResourceView(
        gctx.getResource(pixel_buffer),
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
            1, // FirstElement
            gctx.viewport_width * gctx.viewport_height, // NumElements
            @sizeOf(Pso_Pixel), // StructureByteStride
        ),
        pixel_buffer_srv,
    );

    gctx.device.CreateUnorderedAccessView(
        gctx.getResource(pixel_buffer),
        gctx.getResource(pixel_buffer),
        &d3d12.UNORDERED_ACCESS_VIEW_DESC.initStructuredBuffer(
            1,
            gctx.viewport_width * gctx.viewport_height,
            @sizeOf(Pso_Pixel),
            0, // CounterOffsetInBytes
        ),
        pixel_buffer_uav,
    );

    const pixel_texture = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, gctx.viewport_width, gctx.viewport_height, 1);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | d3d12.RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
        &d3d12.CLEAR_VALUE.initColor(.R8G8B8A8_UNORM, &.{ 0.2, 0.4, 0.8, 1.0 }),
    ) catch |err| hrPanic(err);

    const pixel_texture_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gctx.device.CreateUnorderedAccessView(
        gctx.getResource(pixel_texture),
        null,
        null,
        pixel_texture_uav,
    );

    const pixel_texture_rtv = gctx.allocateCpuDescriptors(.RTV, 1);
    gctx.device.CreateRenderTargetView(
        gctx.getResource(pixel_texture),
        null,
        pixel_texture_rtv,
    );

    gctx.beginFrame();

    var guictx = gfx.GuiContext.init(arena_allocator, &gctx, 1);

    const mesh_textures = [_]gfx.ResourceHandle{
        gctx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_AmbientOcclusion.png",
            .{},
        ) catch |err| hrPanic(err),
        gctx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_BaseColor.png",
            .{},
        ) catch |err| hrPanic(err),
        gctx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_MetallicRoughness.png",
            .{},
        ) catch |err| hrPanic(err),
        gctx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_Normal.png",
            .{},
        ) catch |err| hrPanic(err),
    };

    for (mesh_textures) |texture| {
        const descriptor = gctx.allocatePersistentGpuDescriptors(1);
        gctx.device.CreateShaderResourceView(gctx.getResource(texture), null, descriptor.cpu_handle);
    }

    // Generate mipmaps.
    {
        var mipgen = gfx.MipmapGenerator.init(arena_allocator, &gctx, .R8G8B8A8_UNORM);
        defer mipgen.deinit(&gctx);
        for (mesh_textures) |texture| {
            mipgen.generateMipmaps(&gctx, texture);
            gctx.addTransitionBarrier(texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        }
        gctx.finishGpuCommands(); // Wait for the GPU so that we can release the generator.
    }

    // Fill vertex buffer with vertex data.
    {
        const verts = gctx.allocateUploadBufferRegion(Pso_Vertex, mesh_num_vertices);
        for (mesh_positions.items) |_, i| {
            verts.cpu_slice[i].position = mesh_positions.items[i];
            verts.cpu_slice[i].normal = mesh_normals.items[i];
            verts.cpu_slice[i].texcoord = mesh_texcoords.items[i];
            verts.cpu_slice[i].tangent = mesh_tangents.items[i];
        }

        gctx.cmdlist.CopyBufferRegion(
            gctx.getResource(vertex_buffer),
            0,
            verts.buffer,
            verts.buffer_offset,
            verts.cpu_slice.len * @sizeOf(@TypeOf(verts.cpu_slice[0])),
        );
    }

    // Fill index buffer with index data.
    {
        const indices = gctx.allocateUploadBufferRegion(u32, mesh_num_indices);
        for (mesh_indices.items) |_, i| {
            indices.cpu_slice[i] = mesh_indices.items[i];
        }

        gctx.cmdlist.CopyBufferRegion(
            gctx.getResource(index_buffer),
            0,
            indices.buffer,
            indices.buffer_offset,
            indices.cpu_slice.len * @sizeOf(@TypeOf(indices.cpu_slice[0])),
        );
    }

    gctx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    gctx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_INDEX_BUFFER);
    gctx.flushResourceBarriers();

    gctx.endFrame();

    gctx.finishGpuCommands();

    return .{
        .gctx = gctx,
        .guictx = guictx,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .normal_tfmt = normal_tfmt,
        .z_pre_pass_pso = z_pre_pass_pso,
        .record_pixels_pso = record_pixels_pso,
        .draw_pixels_pso = draw_pixels_pso,
        .clear_pixels_pso = clear_pixels_pso,
        .draw_mesh_pso = draw_mesh_pso,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .pixel_buffer = pixel_buffer,
        .pixel_buffer_srv = pixel_buffer_srv,
        .pixel_buffer_uav = pixel_buffer_uav,
        .pixel_texture = pixel_texture,
        .pixel_texture_uav = pixel_texture_uav,
        .pixel_texture_rtv = pixel_texture_rtv,
        .mesh_textures = mesh_textures,
        .mesh_num_vertices = mesh_num_vertices,
        .mesh_num_indices = mesh_num_indices,
        .draw_wireframe = true,
        .num_pixel_groups = 0,
        .raster_speed = 4,
        .camera = .{
            .position = [3]f32{ 3.0, 0.0, -3.0 },
            .forward = [3]f32{ 0.0, 0.0, 0.0 },
            .pitch = 0.0 * math.pi,
            .yaw = -0.25 * math.pi,
        },
        .mouse = .{
            .cursor_prev_x = 0,
            .cursor_prev_y = 0,
        },
    };
}

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    for (demo.mesh_textures) |texture| {
        _ = demo.gctx.releaseResource(texture);
    }
    _ = demo.gctx.releaseResource(demo.pixel_texture);
    _ = demo.gctx.releaseResource(demo.pixel_buffer);
    _ = demo.gctx.releaseResource(demo.depth_texture);
    _ = demo.gctx.releaseResource(demo.vertex_buffer);
    _ = demo.gctx.releaseResource(demo.index_buffer);
    _ = demo.gctx.releasePipeline(demo.draw_mesh_pso);
    _ = demo.gctx.releasePipeline(demo.clear_pixels_pso);
    _ = demo.gctx.releasePipeline(demo.z_pre_pass_pso);
    _ = demo.gctx.releasePipeline(demo.draw_pixels_pso);
    _ = demo.gctx.releasePipeline(demo.record_pixels_pso);
    _ = demo.brush.Release();
    _ = demo.normal_tfmt.Release();
    demo.guictx.deinit(&demo.gctx);
    demo.gctx.deinit();
    lib.deinitWindow(gpa_allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    const dt = demo.frame_stats.delta_time;

    lib.newImGuiFrame(dt);

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

    if (demo.draw_wireframe) {
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
    }

    if (c.igButton(
        if (demo.draw_wireframe) "  Freeze & Rasterize  " else "  Clear & Unfreeze  ",
        .{ .x = 0, .y = 0 },
    )) {
        demo.draw_wireframe = !demo.draw_wireframe;
        demo.num_pixel_groups = 0;
    }

    if (!demo.draw_wireframe) {
        _ = c.igSliderInt("Rasterization speed", &demo.raster_speed, 4, 32, null, c.ImGuiSliderFlags_None);
    }

    c.igEnd();

    if (demo.draw_wireframe) {
        // Handle camera rotation with mouse.
        {
            var pos: w.POINT = undefined;
            _ = w.GetCursorPos(&pos);
            const delta_x = @intToFloat(f32, pos.x) - @intToFloat(f32, demo.mouse.cursor_prev_x);
            const delta_y = @intToFloat(f32, pos.y) - @intToFloat(f32, demo.mouse.cursor_prev_y);
            demo.mouse.cursor_prev_x = pos.x;
            demo.mouse.cursor_prev_y = pos.y;

            if (w.GetAsyncKeyState(w.VK_RBUTTON) < 0) {
                demo.camera.pitch += 0.0025 * delta_y;
                demo.camera.yaw += 0.0025 * delta_x;
                demo.camera.pitch = math.min(demo.camera.pitch, 0.48 * math.pi);
                demo.camera.pitch = math.max(demo.camera.pitch, -0.48 * math.pi);
                demo.camera.yaw = zm.modAngle(demo.camera.yaw);
            }
        }

        // Handle camera movement with 'WASD' keys.
        {
            const speed = zm.f32x4s(1.0);
            const delta_time = zm.f32x4s(demo.frame_stats.delta_time);
            const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
            var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

            zm.store(demo.camera.forward[0..], forward, 3);

            const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
            forward = speed * delta_time * forward;

            var cpos = zm.load(demo.camera.position[0..], zm.Vec, 3);

            if (w.GetAsyncKeyState('W') < 0) {
                cpos += forward;
            } else if (w.GetAsyncKeyState('S') < 0) {
                cpos -= forward;
            }
            if (w.GetAsyncKeyState('D') < 0) {
                cpos += right;
            } else if (w.GetAsyncKeyState('A') < 0) {
                cpos -= right;
            }

            zm.store(demo.camera.position[0..], cpos, 3);
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

    gctx.beginFrame();

    // Set input assembler (IA) state.
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = demo.mesh_num_vertices * @sizeOf(Pso_Vertex),
        .StrideInBytes = @sizeOf(Pso_Vertex),
    }});
    gctx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gctx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = demo.mesh_num_indices * @sizeOf(u32),
        .Format = .R32_UINT,
    });

    if (demo.draw_wireframe) {
        gctx.cmdlist.OMSetRenderTargets(
            0,
            null,
            w.TRUE,
            &demo.depth_texture_dsv,
        );
        gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);

        //
        // Z pre pass for wireframe mesh.
        //
        gctx.setCurrentPipeline(demo.z_pre_pass_pso);
        // Upload per-frame constant data (camera xform).
        {
            const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);
            zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));
            gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        }
        // Upload per-draw constant data (object to world xform) and draw.
        {
            const object_to_world = zm.translation(0.0, 0.0, 0.0);
            const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);
            zm.storeMat(mem.cpu_slice[0].object_to_world[0..], zm.transpose(object_to_world));
            gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }
        gctx.cmdlist.DrawIndexedInstanced(demo.mesh_num_indices, 1, 0, 0, 0);

        gctx.addTransitionBarrier(demo.pixel_texture, d3d12.RESOURCE_STATE_RENDER_TARGET);
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{demo.pixel_texture_rtv},
            w.TRUE,
            &demo.depth_texture_dsv,
        );
        gctx.cmdlist.ClearRenderTargetView(
            demo.pixel_texture_rtv,
            &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );

        //
        // Draw wireframe mesh.
        //
        gctx.setCurrentPipeline(demo.draw_mesh_pso);
        // Upload per-frame constant data (camera xform).
        {
            const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);
            zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));
            gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        }
        // Upload per-draw constant data (object to world xform) and draw.
        {
            const object_to_world = zm.translation(0.0, 0.0, 0.0);
            const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);
            zm.storeMat(mem.cpu_slice[0].object_to_world[0..], zm.transpose(object_to_world));
            gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }
        gctx.cmdlist.DrawIndexedInstanced(demo.mesh_num_indices, 1, 0, 0, 0);
    } else {
        // Check if we need to regenerate our pixel buffer.
        if (demo.num_pixel_groups == 0) {
            // Reset pixel buffer atomic counter.
            {
                const param = [_]d3d12.WRITEBUFFERIMMEDIATE_PARAMETER{.{
                    .Dest = gctx.getResource(demo.pixel_buffer).GetGPUVirtualAddress(),
                    .Value = 0,
                }};
                gctx.addTransitionBarrier(demo.pixel_buffer, d3d12.RESOURCE_STATE_COPY_DEST);
                gctx.flushResourceBarriers();
                gctx.cmdlist.WriteBufferImmediate(param.len, &param, null);
                gctx.addTransitionBarrier(demo.pixel_buffer, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
                gctx.flushResourceBarriers();
            }

            // Clear pixel buffer.
            gctx.setCurrentPipeline(demo.clear_pixels_pso);
            gctx.cmdlist.SetComputeRootDescriptorTable(
                0,
                gctx.copyDescriptorsToGpuHeap(1, demo.pixel_buffer_uav),
            );
            gctx.cmdlist.Dispatch(gctx.viewport_width * gctx.viewport_height / compute_group_size, 1, 1);
            gctx.cmdlist.ResourceBarrier(
                1,
                &[_]d3d12.RESOURCE_BARRIER{
                    d3d12.RESOURCE_BARRIER.initUav(gctx.getResource(demo.pixel_buffer)),
                },
            );

            gctx.cmdlist.OMSetRenderTargets(0, null, w.TRUE, &demo.depth_texture_dsv);
            gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);

            //
            // Z pre pass.
            //
            gctx.setCurrentPipeline(demo.z_pre_pass_pso);
            // Upload per-frame constant data (camera xform).
            {
                const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);
                zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));
                gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
            }
            // Upload per-draw constant data (object to world xform) and draw.
            {
                const object_to_world = zm.translation(0.0, 0.0, 0.0);
                const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);
                zm.storeMat(mem.cpu_slice[0].object_to_world[0..], zm.transpose(object_to_world));
                gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
            }
            gctx.cmdlist.DrawIndexedInstanced(demo.mesh_num_indices, 1, 0, 0, 0);

            //
            // Record pixels to linear pixel buffer
            //
            gctx.setCurrentPipeline(demo.record_pixels_pso);
            // Bind pixel buffer UAV.
            gctx.cmdlist.SetGraphicsRootDescriptorTable(
                2,
                gctx.copyDescriptorsToGpuHeap(1, demo.pixel_buffer_uav),
            );
            // Upload per-frame constant data (camera xform).
            {
                const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);

                zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));
                mem.cpu_slice[0].camera_position = demo.camera.position;

                gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
            }
            // Upload per-draw constant data (object to world xform) and draw.
            {
                const object_to_world = zm.translation(0.0, 0.0, 0.0);
                const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);
                zm.storeMat(mem.cpu_slice[0].object_to_world[0..], zm.transpose(object_to_world));
                gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
            }
            gctx.cmdlist.DrawIndexedInstanced(demo.mesh_num_indices, 1, 0, 0, 0);
        }

        // Increase number of drawn pixels to achieve animation effect.
        demo.num_pixel_groups += @intCast(u32, demo.raster_speed);
        if (demo.num_pixel_groups > gctx.viewport_width * gctx.viewport_height / compute_group_size) {
            demo.num_pixel_groups = gctx.viewport_width * gctx.viewport_height / compute_group_size;
        }

        gctx.addTransitionBarrier(demo.pixel_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        gctx.addTransitionBarrier(demo.pixel_texture, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
        gctx.flushResourceBarriers();

        //
        // Draw pixels to the pixel texture.
        //
        gctx.setCurrentPipeline(demo.draw_pixels_pso);
        gctx.cmdlist.SetComputeRootDescriptorTable(
            0,
            blk: {
                const table = gctx.copyDescriptorsToGpuHeap(1, demo.pixel_buffer_srv);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.pixel_texture_uav);
                break :blk table;
            },
        );
        gctx.cmdlist.Dispatch(demo.num_pixel_groups, 1, 1);
    }

    const back_buffer = gctx.getBackBuffer();

    // Copy pixel texture to the back buffer.
    gctx.addTransitionBarrier(demo.pixel_texture, d3d12.RESOURCE_STATE_COPY_SOURCE);
    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_COPY_DEST);
    gctx.flushResourceBarriers();

    gctx.cmdlist.CopyResource(gctx.getResource(back_buffer.resource_handle), gctx.getResource(demo.pixel_texture));

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    gctx.addTransitionBarrier(demo.pixel_texture, d3d12.RESOURCE_STATE_UNORDERED_ACCESS);
    gctx.flushResourceBarriers();

    // Set back buffer as a render target (for UI and Direct2D).
    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );

    demo.guictx.draw(gctx);

    gctx.beginDraw2d();
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
            gctx.d2d.context,
            text,
            demo.normal_tfmt,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, gctx.viewport_width),
                .bottom = @intToFloat(f32, gctx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }
    gctx.endDraw2d();

    gctx.endFrame();
}

pub fn main() !void {
    lib.init();
    defer lib.deinit();

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
