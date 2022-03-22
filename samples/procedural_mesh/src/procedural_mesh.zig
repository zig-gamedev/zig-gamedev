const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.base;
const d3d12 = zwin32.d3d12;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const GuiRenderer = common.GuiRenderer;
const c = common.c;
const zm = @import("zmath");
const zmesh = @import("zmesh");

pub export const D3D12SDKVersion: u32 = 4;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: procedural mesh";
const window_width = 1920;
const window_height = 1080;

const Pso_DrawConst = struct {
    object_to_world: [16]f32,
    basecolor_roughness: [4]f32,
};

const Pso_FrameConst = struct {
    world_to_clip: [16]f32,
    camera_position: [3]f32,
};

const Pso_Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,
};

const Drawable = struct {
    mesh_index: u32,
    position: [3]f32,
    basecolor_roughness: [4]f32,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    simple_entity_pso: zd3d12.PipelineHandle,

    vertex_buffer: zd3d12.ResourceHandle,
    index_buffer: zd3d12.ResourceHandle,

    depth_texture: zd3d12.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    meshes: std.ArrayList(Mesh),
    drawables: std.ArrayList(Drawable),

    camera: struct {
        position: [3]f32 = .{ 0.0, 0.0, -3.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.0,
        yaw: f32 = 0.0,
    } = .{},
    mouse: struct {
        cursor_prev_x: i32 = 0,
        cursor_prev_y: i32 = 0,
    } = .{},
};

fn appendMesh(
    mesh: zmesh.Mesh,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(u16),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    meshes.append(.{
        .index_offset = @intCast(u32, meshes_indices.items.len),
        .vertex_offset = @intCast(i32, meshes_positions.items.len),
        .num_indices = @intCast(u32, mesh.indices.len),
        .num_vertices = @intCast(u32, mesh.positions.len),
    }) catch unreachable;

    meshes_indices.appendSlice(mesh.indices) catch unreachable;
    meshes_positions.appendSlice(mesh.positions) catch unreachable;
    meshes_normals.appendSlice(mesh.normals.?) catch unreachable;
}

fn initScene(
    drawables: *std.ArrayList(Drawable),
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(u16),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    // Trefoil Knot.
    {
        var mesh = zmesh.initTrefoilKnot(10, 128, 0.8);
        defer mesh.deinit();
        mesh.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 0, 0 },
            .basecolor_roughness = .{ 0.0, 0.7, 0.0, 0.6 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Parametric Sphere.
    {
        var mesh = zmesh.initParametricSphere(20, 20);
        defer mesh.deinit();
        mesh.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 3, 0, 0 },
            .basecolor_roughness = .{ 0.7, 0.0, 0.0, 0.4 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Icosahedron.
    {
        var mesh = zmesh.initIcosahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -3, 0, 0 },
            .basecolor_roughness = .{ 0.7, 0.6, 0.0, 0.4 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Dodecahedron.
    {
        var mesh = zmesh.initDodecahedron();
        defer mesh.deinit();
        mesh.unweld();
        mesh.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ 0, 0, 3 },
            .basecolor_roughness = .{ 0.0, 0.2, 1.0, 0.4 },
        }) catch unreachable;

        appendMesh(mesh, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
    // Cylinder with top and bottom caps.
    {
        var disk = zmesh.initParametricDisk(10, 2);
        defer disk.deinit();
        disk.invert(0, 0);

        var cylinder = zmesh.initCylinder(10, 4);
        defer cylinder.deinit();

        cylinder.merge(disk);
        cylinder.translate(0, 0, -1);
        disk.invert(0, 0);
        cylinder.merge(disk);

        cylinder.scale(0.5, 0.5, 2);
        cylinder.rotate(math.pi * 0.5, 1.0, 0.0, 0.0);

        cylinder.unweld();
        cylinder.computeNormals();

        drawables.append(.{
            .mesh_index = @intCast(u32, meshes.items.len),
            .position = .{ -3, 0, 3 },
            .basecolor_roughness = .{ 1.0, 0.0, 0.0, 0.4 },
        }) catch unreachable;

        appendMesh(cylinder, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
}

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    const barycentrics_supported = blk: {
        var options3: d3d12.FEATURE_DATA_D3D12_OPTIONS3 = undefined;
        const res = gctx.device.CheckFeatureSupport(
            .OPTIONS3,
            &options3,
            @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS3),
        );
        break :blk options3.BarycentricsSupported == w32.TRUE and res == w32.S_OK;
    };

    const simple_entity_pso = blk: {
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
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.DSVFormat = .D32_FLOAT;

        if (!barycentrics_supported) {
            break :blk gctx.createGraphicsShaderPipelineVsGsPs(
                arena_allocator,
                &pso_desc,
                content_dir ++ "shaders/simple_entity.vs.cso",
                content_dir ++ "shaders/simple_entity.gs.cso",
                content_dir ++ "shaders/simple_entity_with_gs.ps.cso",
            );
        } else {
            break :blk gctx.createGraphicsShaderPipeline(
                arena_allocator,
                &pso_desc,
                content_dir ++ "shaders/simple_entity.vs.cso",
                content_dir ++ "shaders/simple_entity.ps.cso",
            );
        }
    };

    var drawables = std.ArrayList(Drawable).init(allocator);
    var meshes = std.ArrayList(Mesh).init(allocator);
    var meshes_indices = std.ArrayList(u16).init(arena_allocator);
    var meshes_positions = std.ArrayList([3]f32).init(arena_allocator);
    var meshes_normals = std.ArrayList([3]f32).init(arena_allocator);
    initScene(&drawables, &meshes, &meshes_indices, &meshes_positions, &meshes_normals);

    const num_vertices = @intCast(u32, meshes_positions.items.len);
    const num_indices = @intCast(u32, meshes_indices.items.len);

    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(num_vertices * @sizeOf(Pso_Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(num_indices * @sizeOf(u16)),
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

    // Create depth texture view.
    const depth_texture_dsv = gctx.allocateCpuDescriptors(.DSV, 1);
    gctx.device.CreateDepthStencilView(
        gctx.lookupResource(depth_texture).?,
        null,
        depth_texture_dsv,
    );

    //
    // Begin frame to init/upload resources to the GPU.
    //
    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    // Fill vertex buffer with vertex data.
    {
        const verts = gctx.allocateUploadBufferRegion(Pso_Vertex, num_vertices);
        for (meshes_positions.items) |_, i| {
            verts.cpu_slice[i].position = meshes_positions.items[i];
            verts.cpu_slice[i].normal = meshes_normals.items[i];
        }

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
        const indices = gctx.allocateUploadBufferRegion(u16, num_indices);
        for (meshes_indices.items) |_, i| {
            indices.cpu_slice[i] = meshes_indices.items[i];
        }

        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(index_buffer).?,
            0,
            indices.buffer,
            indices.buffer_offset,
            indices.cpu_slice.len * @sizeOf(@TypeOf(indices.cpu_slice[0])),
        );
    }

    gctx.endFrame();
    gctx.finishGpuCommands();

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .simple_entity_pso = simple_entity_pso,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .meshes = meshes,
        .drawables = drawables,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    demo.meshes.deinit();
    demo.drawables.deinit();
    demo.guir.deinit(&demo.gctx);
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.gctx.window, window_name);
    const dt = demo.frame_stats.delta_time;
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
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "Right Mouse Button + drag", "");
    c.igSameLine(0, -1);
    c.igText(" :  rotate camera", "");

    c.igBulletText("", "");
    c.igSameLine(0, -1);
    c.igTextColored(.{ .x = 0, .y = 0.8, .z = 0, .w = 1 }, "W, A, S, D", "");
    c.igSameLine(0, -1);
    c.igText(" :  move camera", "");

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
        const speed = zm.f32x4s(2.0);
        const delta_time = zm.f32x4s(demo.frame_stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.store(demo.camera.forward[0..], forward, 3);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

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

        zm.store(demo.camera.position[0..], cpos, 3);
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

    const back_buffer = gctx.getBackBuffer();
    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
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
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);

    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, gctx.getResourceSize(demo.vertex_buffer)),
        .StrideInBytes = @sizeOf(Pso_Vertex),
    }});
    gctx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gctx.lookupResource(demo.index_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, gctx.getResourceSize(demo.index_buffer)),
        .Format = .R16_UINT,
    });
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

    gctx.setCurrentPipeline(demo.simple_entity_pso);

    // Upload per-frame constant data (camera xform).
    {
        const mem = gctx.allocateUploadMemory(Pso_FrameConst, 1);

        zm.storeMat(mem.cpu_slice[0].world_to_clip[0..], zm.transpose(cam_world_to_clip));
        mem.cpu_slice[0].camera_position = demo.camera.position;

        gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
    }

    for (demo.drawables.items) |drawable| {
        const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);

        const object_to_world = zm.translationV(zm.load(drawable.position[0..], zm.Vec, 3));
        zm.storeMat(mem.cpu_slice[0].object_to_world[0..], zm.transpose(object_to_world));
        mem.cpu_slice[0].basecolor_roughness = drawable.basecolor_roughness;

        gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        gctx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[drawable.mesh_index].num_indices,
            1,
            demo.meshes.items[drawable.mesh_index].index_offset,
            demo.meshes.items[drawable.mesh_index].vertex_offset,
            0,
        );
    }

    demo.guir.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_PRESENT);
    gctx.flushResourceBarriers();

    gctx.endFrame();
}

pub fn main() !void {
    common.init();
    defer common.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    zmesh.init(allocator);
    defer zmesh.deinit();

    var demo = try init(allocator);
    defer deinit(&demo, allocator);

    while (common.handleWindowEvents()) {
        update(&demo);
        draw(&demo);
    }
}
