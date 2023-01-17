const std = @import("std");
const assert = std.debug.assert;
const math = std.math;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const dml = zwin32.directml;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;
const zmesh = @import("zmesh");

const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: mesh shader test";
const window_width = 1920;
const window_height = 1080;

// Also need to change MAX_NUM_VERTICES and MAX_NUM_TRIANGLES in mesh_shader_test.hlsl
const max_num_meshlet_vertices: usize = 64;
const max_num_meshlet_triangles: usize = 64;

const mesh_engine = 1;

const Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    meshlet_offset: u32,
    num_indices: u32,
    num_vertices: u32,
    num_meshlets: u32,
};

const Meshlet = struct {
    data_offset: u32 align(8),
    num_vertices: u16,
    num_triangles: u16,
};
comptime {
    assert(@sizeOf(Meshlet) == 8);
    assert(@alignOf(Meshlet) == 8);
}

const Pso_DrawConst = extern struct {
    object_to_clip: Mat4,
};

const DrawMode = enum {
    mesh_shader,
    vertex_shader,
    vertex_shader_fixed,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    mesh_shader_pso: zd3d12.PipelineHandle,
    vertex_shader_pso: zd3d12.PipelineHandle,
    vertex_shader_fixed_pso: zd3d12.PipelineHandle,

    vertex_buffer: zd3d12.ResourceHandle,
    vertex_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    index_buffer: zd3d12.ResourceHandle,
    index_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    meshlet_buffer: zd3d12.ResourceHandle,
    meshlet_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    meshlet_data_buffer: zd3d12.ResourceHandle,
    meshlet_data_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    depth_texture: zd3d12.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    meshes: std.ArrayList(Mesh),

    draw_mode: DrawMode,
    num_objects_to_draw: i32,

    camera: struct {
        position: Vec3,
        forward: Vec3,
        pitch: f32,
        yaw: f32,
    },
    mouse: struct {
        cursor_prev_x: i32,
        cursor_prev_y: i32,
    },
};

fn loadMeshAndGenerateMeshlets(
    arena_allocator: std.mem.Allocator,
    file_path: [:0]const u8,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
    all_meshlets: *std.ArrayList(Meshlet),
    all_meshlets_data: *std.ArrayList(u32),
) !void {
    var src_positions = std.ArrayList([3]f32).init(arena_allocator);
    var src_normals = std.ArrayList([3]f32).init(arena_allocator);
    var src_indices = std.ArrayList(u32).init(arena_allocator);

    const data = try zmesh.io.parseAndLoadFile(file_path);
    defer zmesh.io.freeData(data);
    try zmesh.io.appendMeshPrimitive(data, 0, 0, &src_indices, &src_positions, &src_normals, null, null);

    var src_vertices = try std.ArrayList(Vertex).initCapacity(
        arena_allocator,
        src_positions.items.len,
    );

    for (src_positions.items) |_, index| {
        src_vertices.appendAssumeCapacity(.{
            .position = src_positions.items[index],
            .normal = src_normals.items[index],
        });
    }

    var remap = std.ArrayList(u32).init(arena_allocator);
    try remap.resize(src_indices.items.len);
    const num_unique_vertices = zmesh.opt.generateVertexRemap(
        remap.items,
        src_indices.items,
        Vertex,
        src_vertices.items,
    );

    var opt_vertices = std.ArrayList(Vertex).init(arena_allocator);
    try opt_vertices.resize(num_unique_vertices);
    zmesh.opt.remapVertexBuffer(
        Vertex,
        opt_vertices.items,
        src_vertices.items,
        remap.items,
    );

    var opt_indices = std.ArrayList(u32).init(arena_allocator);
    try opt_indices.resize(src_indices.items.len);
    zmesh.opt.remapIndexBuffer(
        opt_indices.items,
        src_indices.items,
        remap.items,
    );

    zmesh.opt.optimizeVertexCache(
        opt_indices.items,
        opt_indices.items,
        opt_vertices.items.len,
    );
    const num_opt_vertices = zmesh.opt.optimizeVertexFetch(
        Vertex,
        opt_vertices.items,
        opt_indices.items,
        opt_vertices.items,
    );
    assert(num_opt_vertices == opt_vertices.items.len);

    const max_num_meshlets = zmesh.opt.buildMeshletsBound(
        opt_indices.items.len,
        max_num_meshlet_vertices,
        max_num_meshlet_triangles,
    );

    var meshlets = std.ArrayList(zmesh.opt.Meshlet).init(arena_allocator);
    var meshlet_vertices = std.ArrayList(u32).init(arena_allocator);
    var meshlet_triangles = std.ArrayList(u8).init(arena_allocator);
    try meshlets.resize(max_num_meshlets);
    try meshlet_vertices.resize(max_num_meshlets * max_num_meshlet_vertices);
    try meshlet_triangles.resize(max_num_meshlets * max_num_meshlet_triangles * 3);

    const num_meshlets = zmesh.opt.buildMeshlets(
        meshlets.items,
        meshlet_vertices.items,
        meshlet_triangles.items,
        opt_indices.items,
        Vertex,
        opt_vertices.items,
        max_num_meshlet_vertices,
        max_num_meshlet_triangles,
        0.0,
    );
    assert(num_meshlets <= max_num_meshlets);
    try meshlets.resize(num_meshlets);

    try all_meshes.append(.{
        .index_offset = @intCast(u32, all_indices.items.len),
        .vertex_offset = @intCast(u32, all_vertices.items.len),
        .meshlet_offset = @intCast(u32, all_meshlets.items.len),
        .num_indices = @intCast(u32, opt_indices.items.len),
        .num_vertices = @intCast(u32, opt_vertices.items.len),
        .num_meshlets = @intCast(u32, meshlets.items.len),
    });

    for (meshlets.items) |src_meshlet| {
        const meshlet = Meshlet{
            .data_offset = @intCast(u32, all_meshlets_data.items.len),
            .num_vertices = @intCast(u16, src_meshlet.vertex_count),
            .num_triangles = @intCast(u16, src_meshlet.triangle_count),
        };
        try all_meshlets.append(meshlet);

        var i: u32 = 0;
        while (i < src_meshlet.vertex_count) : (i += 1) {
            try all_meshlets_data.append(meshlet_vertices.items[src_meshlet.vertex_offset + i]);
        }

        i = 0;
        while (i < src_meshlet.triangle_count) : (i += 1) {
            const index0 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 0]);
            const index1 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 1]);
            const index2 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 2]);
            const prim = @intCast(u32, index0) | (@intCast(u32, index1) << 10) | (@intCast(u32, index2) << 20);
            try all_meshlets_data.append(prim);
        }
    }

    try all_indices.appendSlice(opt_indices.items);
    try all_vertices.appendSlice(opt_vertices.items);
}

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    // Check for Mesh Shader support.
    {
        var options7: d3d12.FEATURE_DATA_D3D12_OPTIONS7 = undefined;
        const res = gctx.device.CheckFeatureSupport(
            .OPTIONS7,
            &options7,
            @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS7),
        );
        if (options7.MeshShaderTier == .NOT_SUPPORTED or res != w32.S_OK) {
            _ = w32.MessageBoxA(
                window,
                "This applications requires graphics card that supports Mesh Shader " ++
                    "(NVIDIA GeForce Turing or newer, AMD Radeon RX 6000 or newer).",
                "No DirectX 12 Mesh Shader support",
                w32.MB_OK | w32.MB_ICONERROR,
            );
            w32.ExitProcess(0);
        }
    }

    const mesh_shader_pso = blk: {
        var pso_desc = d3d12.MESH_SHADER_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.SampleDesc = .{ .Count = 1, .Quality = 0 };

        break :blk gctx.createMeshShaderPipeline(
            arena_allocator,
            &pso_desc,
            null,
            content_dir ++ "shaders/mesh_shader.ms.cso",
            content_dir ++ "shaders/mesh_shader.ps.cso",
        );
    };

    const vertex_shader_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.SampleDesc = .{ .Count = 1, .Quality = 0 };

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/vertex_shader.vs.cso",
            content_dir ++ "shaders/vertex_shader.ps.cso",
        );
    };

    const vertex_shader_fixed_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("Position", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
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
        pso_desc.SampleDesc = .{ .Count = 1, .Quality = 0 };

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/vertex_shader_fixed.vs.cso",
            content_dir ++ "shaders/vertex_shader_fixed.ps.cso",
        );
    };

    zmesh.init(arena_allocator);
    defer zmesh.deinit();

    var all_meshes = std.ArrayList(Mesh).init(allocator);
    var all_vertices = std.ArrayList(Vertex).init(arena_allocator);
    var all_indices = std.ArrayList(u32).init(arena_allocator);
    var all_meshlets = std.ArrayList(Meshlet).init(arena_allocator);
    var all_meshlets_data = std.ArrayList(u32).init(arena_allocator);
    try loadMeshAndGenerateMeshlets(
        arena_allocator,
        content_dir ++ "cube.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
        &all_meshlets,
        &all_meshlets_data,
    );
    try loadMeshAndGenerateMeshlets(
        arena_allocator,
        content_dir ++ "engine.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
        &all_meshlets,
        &all_meshlets_data,
    );

    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const vertex_buffer_srv = blk: {
        const srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(vertex_buffer).?,
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
                0,
                @intCast(u32, all_vertices.items.len),
                @sizeOf(Vertex),
            ),
            srv,
        );
        break :blk srv;
    };

    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const index_buffer_srv = blk: {
        const srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(index_buffer).?,
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32_UINT, 0, @intCast(u32, all_indices.items.len)),
            srv,
        );
        break :blk srv;
    };

    const meshlet_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(all_meshlets.items.len * @sizeOf(Meshlet)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const meshlet_buffer_srv = blk: {
        const srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(meshlet_buffer).?,
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
                0,
                @intCast(u32, all_meshlets.items.len),
                @sizeOf(Meshlet),
            ),
            srv,
        );
        break :blk srv;
    };

    const meshlet_data_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(all_meshlets_data.items.len * @sizeOf(u32)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const meshlet_data_buffer_srv = blk: {
        const srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(meshlet_data_buffer).?,
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(
                .R32_UINT,
                0,
                @intCast(u32, all_meshlets_data.items.len),
            ),
            srv,
        );
        break :blk srv;
    };

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

    const depth_texture_dsv = blk: {
        const dsv = gctx.allocateCpuDescriptors(.DSV, 1);
        gctx.device.CreateDepthStencilView(gctx.lookupResource(depth_texture).?, null, dsv);
        break :blk dsv;
    };

    //
    // Begin data upload to the GPU.
    //
    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    // Upload vertex buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| upload.cpu_slice[i] = vertex;
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(vertex_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(vertex_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    // Upload index buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| upload.cpu_slice[i] = index;
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(index_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(index_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    // Upload meshlet buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(Meshlet, @intCast(u32, all_meshlets.items.len));
        for (all_meshlets.items) |meshlet, i| upload.cpu_slice[i] = meshlet;
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(meshlet_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(meshlet_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    // Upload meshlet data buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(u32, @intCast(u32, all_meshlets_data.items.len));
        for (all_meshlets_data.items) |meshlet_data, i| upload.cpu_slice[i] = meshlet_data;
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(meshlet_data_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(meshlet_data_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    gctx.endFrame();
    gctx.finishGpuCommands();

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .mesh_shader_pso = mesh_shader_pso,
        .vertex_shader_pso = vertex_shader_pso,
        .vertex_shader_fixed_pso = vertex_shader_fixed_pso,
        .vertex_buffer = vertex_buffer,
        .vertex_buffer_srv = vertex_buffer_srv,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .index_buffer = index_buffer,
        .index_buffer_srv = index_buffer_srv,
        .meshlet_buffer = meshlet_buffer,
        .meshlet_buffer_srv = meshlet_buffer_srv,
        .meshlet_data_buffer = meshlet_data_buffer,
        .meshlet_data_buffer_srv = meshlet_data_buffer_srv,
        .meshes = all_meshes,
        .draw_mode = .mesh_shader,
        .num_objects_to_draw = 16,
        .camera = .{
            .position = Vec3.init(0.0, 0.0, -2.0),
            .forward = Vec3.init(0.0, 0.0, 1.0),
            .pitch = 0.0,
            .yaw = 0.0,
        },
        .mouse = .{
            .cursor_prev_x = 0,
            .cursor_prev_y = 0,
        },
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    demo.meshes.deinit();
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

    c.igSpacing();
    c.igSpacing();

    c.igText("Draw mode:", "");
    var draw_mode: i32 = @enumToInt(demo.draw_mode);
    _ = c.igRadioButton_IntPtr("Mesh Shader emulating VS (no culling)", &draw_mode, 0);
    _ = c.igRadioButton_IntPtr("VS with manual vertex fetching (no HW index buffer)", &draw_mode, 1);
    _ = c.igRadioButton_IntPtr("VS with fixed function vertex fetching", &draw_mode, 2);
    demo.draw_mode = @intToEnum(DrawMode, draw_mode);

    _ = c.igSliderInt("Num. objects", &demo.num_objects_to_draw, 1, 1000, null, c.ImGuiSliderFlags_None);

    c.igSpacing();
    c.igSpacing();
    c.igSpacing();

    c.igText("Triangles: ");
    c.igSameLine(0, -1);
    c.igTextColored(
        .{ .x = 0, .y = 0.8, .z = 0, .w = 1 },
        "%.3f M",
        @intToFloat(f64, demo.num_objects_to_draw) *
            @intToFloat(f64, demo.meshes.items[mesh_engine].num_indices / 3) / 1_000_000.0,
    );

    c.igText("Vertices: ");
    c.igSameLine(0, -1);
    c.igTextColored(
        .{ .x = 0, .y = 0.8, .z = 0, .w = 1 },
        "%.3f M",
        @intToFloat(f64, demo.num_objects_to_draw) *
            @intToFloat(f64, demo.meshes.items[mesh_engine].num_vertices) / 1_000_000.0,
    );

    if (demo.draw_mode == .mesh_shader) {
        c.igText("Meshlets: ");
        c.igSameLine(0, -1);
        c.igTextColored(
            .{ .x = 0, .y = 0.8, .z = 0, .w = 1 },
            "%.3f K",
            @intToFloat(f64, demo.num_objects_to_draw) *
                @intToFloat(f64, demo.meshes.items[mesh_engine].num_meshlets) / 1_000.0,
        );

        c.igSpacing();
        c.igSpacing();
        c.igText("Max. vertices / meshlet: %d", max_num_meshlet_vertices);
        c.igText("Max. triangles / meshlet: %d", max_num_meshlet_triangles);
    }

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
            demo.camera.yaw = vm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed: f32 = 0.25;
        const delta_time = demo.frame_stats.delta_time;
        const transform = Mat4.initRotationX(demo.camera.pitch).mul(Mat4.initRotationY(demo.camera.yaw));
        var forward = Vec3.init(0.0, 0.0, 1.0).transform(transform).normalize();

        demo.camera.forward = forward;
        const right = Vec3.init(0.0, 1.0, 0.0).cross(forward).normalize().scale(speed * delta_time);
        forward = forward.scale(speed * delta_time);

        if (w32.GetAsyncKeyState('W') < 0) {
            demo.camera.position = demo.camera.position.add(forward);
        } else if (w32.GetAsyncKeyState('S') < 0) {
            demo.camera.position = demo.camera.position.sub(forward);
        }
        if (w32.GetAsyncKeyState('D') < 0) {
            demo.camera.position = demo.camera.position.add(right);
        } else if (w32.GetAsyncKeyState('A') < 0) {
            demo.camera.position = demo.camera.position.sub(right);
        }
    }
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const cam_world_to_view = Mat4.initLookToLh(
        demo.camera.position,
        demo.camera.forward,
        Vec3.init(0.0, 1.0, 0.0),
    );
    const cam_view_to_clip = Mat4.initPerspectiveFovLh(
        math.pi / 3.0,
        @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
        0.01,
        200.0,
    );
    const cam_world_to_clip = cam_world_to_view.mul(cam_view_to_clip);

    const back_buffer = gctx.getBackBuffer();
    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        &demo.depth_texture_dsv,
    );
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, .{ .DEPTH = true }, 1.0, 0, 0, null);
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.1, 0.2, 0.4, 1.0 },
        0,
        null,
    );

    //
    // Draw all objects.
    //
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    switch (demo.draw_mode) {
        .mesh_shader => {
            gctx.setCurrentPipeline(demo.mesh_shader_pso);

            // Bind global buffers that contain data for *all meshes* and *all meshlets*.
            gctx.cmdlist.SetGraphicsRootDescriptorTable(2, blk: {
                const table = gctx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer_srv);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.index_buffer_srv);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.meshlet_buffer_srv);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.meshlet_data_buffer_srv);
                break :blk table;
            });
        },
        .vertex_shader => {
            gctx.setCurrentPipeline(demo.vertex_shader_pso);

            // Bind global buffers that contain data for *all meshes*.
            gctx.cmdlist.SetGraphicsRootDescriptorTable(2, blk: {
                const table = gctx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer_srv);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.index_buffer_srv);
                break :blk table;
            });
        },
        .vertex_shader_fixed => {
            gctx.setCurrentPipeline(demo.vertex_shader_fixed_pso);

            gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
                .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
                .SizeInBytes = @intCast(u32, gctx.getResourceSize(demo.vertex_buffer)),
                .StrideInBytes = @sizeOf(Vertex),
            }});
            gctx.cmdlist.IASetIndexBuffer(&.{
                .BufferLocation = gctx.lookupResource(demo.index_buffer).?.GetGPUVirtualAddress(),
                .SizeInBytes = @intCast(u32, gctx.getResourceSize(demo.index_buffer)),
                .Format = .R32_UINT,
            });
        },
    }

    var entity_index: i32 = 0;
    while (entity_index < demo.num_objects_to_draw) : (entity_index += 1) {
        // Upload per-draw constant data.
        {
            const position = Vec3.init(0.0, 0.0, @intToFloat(f32, entity_index) * 2.5);
            const mem = gctx.allocateUploadMemory(Pso_DrawConst, 1);
            mem.cpu_slice[0] = .{
                .object_to_clip = Mat4.initTranslation(position).mul(cam_world_to_clip).transpose(),
            };
            gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        }

        const mesh = &demo.meshes.items[mesh_engine];

        switch (demo.draw_mode) {
            .mesh_shader => {
                // Select a mesh to draw by specifying offsets in global buffers.
                gctx.cmdlist.SetGraphicsRoot32BitConstants(1, 2, &[_]u32{
                    mesh.vertex_offset,
                    mesh.meshlet_offset,
                }, 0);
                gctx.cmdlist.DispatchMesh(mesh.num_meshlets, 1, 1);
            },
            .vertex_shader => {
                // Select a mesh to draw by specifying offsets in global buffers.
                gctx.cmdlist.SetGraphicsRoot32BitConstants(1, 2, &[_]u32{
                    mesh.vertex_offset,
                    mesh.index_offset,
                }, 0);
                gctx.cmdlist.DrawInstanced(mesh.num_indices, 1, 0, 0);
            },
            .vertex_shader_fixed => {
                gctx.cmdlist.DrawIndexedInstanced(
                    mesh.num_indices,
                    1,
                    mesh.index_offset,
                    @intCast(i32, mesh.vertex_offset),
                    0,
                );
            },
        }
    }

    demo.guir.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
    gctx.flushResourceBarriers();

    gctx.endFrame();
}

pub fn main() !void {
    common.init();
    defer common.deinit();

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
