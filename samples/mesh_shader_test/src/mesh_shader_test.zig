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
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: mesh shader test";
const window_width = 1920;
const window_height = 1080;

const Vertex = struct {
    position: Vec3,
    normal: Vec3,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    meshlet_offset: u32,
    num_indices: u32,
    num_vertices: u32,
    num_meshlets: u32,
};

const Meshlet = packed struct {
    data_offset: u32 align(8),
    num_vertices: u16,
    num_triangles: u16,
};
comptime {
    assert(@sizeOf(Meshlet) == 8);
    assert(@alignOf(Meshlet) == 8);
}

const Pso_DrawConst = extern struct {
    object_to_world: Mat4,
    base_color_roughness: Vec4,
};

const Pso_FrameConst = extern struct {
    world_to_clip: Mat4,
    camera_position: Vec3,
};

const Entity = struct {
    position: Vec3,
    base_color_roughness: Vec4,
    mesh_index: u32,
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    mesh_shader_pso: gr.PipelineHandle,
    vertex_shader_pso: gr.PipelineHandle,

    vertex_buffer: gr.ResourceHandle,
    vertex_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    index_buffer: gr.ResourceHandle,
    index_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    meshlet_buffer: gr.ResourceHandle,
    meshlet_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    meshlet_data_buffer: gr.ResourceHandle,
    meshlet_data_buffer_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    depth_texture: gr.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,

    meshes: std.ArrayList(Mesh),
    entities: std.ArrayList(Entity),

    brush: *d2d1.ISolidColorBrush,
    normal_tfmt: *dwrite.ITextFormat,

    use_mesh_shader: bool,

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
    file_path: []const u8,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
    all_meshlets: *std.ArrayList(Meshlet),
    all_meshlets_data: *std.ArrayList(u32),
) void {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    var src_positions = std.ArrayList(Vec3).init(arena_allocator);
    var src_normals = std.ArrayList(Vec3).init(arena_allocator);
    var src_indices = std.ArrayList(u32).init(arena_allocator);

    const data = lib.parseAndLoadGltfFile(file_path);
    defer c.cgltf_free(data);
    lib.appendMeshPrimitive(data, 0, 0, &src_indices, &src_positions, &src_normals, null, null);

    var src_vertices = std.ArrayList(Vertex).initCapacity(arena_allocator, src_positions.items.len) catch unreachable;

    for (src_positions.items) |_, index| {
        src_vertices.appendAssumeCapacity(.{
            .position = src_positions.items[index],
            .normal = src_normals.items[index],
        });
    }

    var remap = std.ArrayList(u32).init(arena_allocator);
    remap.resize(src_indices.items.len) catch unreachable;
    const num_unique_vertices = c.meshopt_generateVertexRemap(
        remap.items.ptr,
        src_indices.items.ptr,
        src_indices.items.len,
        src_vertices.items.ptr,
        src_vertices.items.len,
        @sizeOf(Vertex),
    );

    var opt_vertices = std.ArrayList(Vertex).init(arena_allocator);
    opt_vertices.resize(num_unique_vertices) catch unreachable;
    c.meshopt_remapVertexBuffer(
        opt_vertices.items.ptr,
        src_vertices.items.ptr,
        src_vertices.items.len,
        @sizeOf(Vertex),
        remap.items.ptr,
    );

    var opt_indices = std.ArrayList(u32).init(arena_allocator);
    opt_indices.resize(src_indices.items.len) catch unreachable;
    c.meshopt_remapIndexBuffer(opt_indices.items.ptr, src_indices.items.ptr, src_indices.items.len, remap.items.ptr);

    c.meshopt_optimizeVertexCache(
        opt_indices.items.ptr,
        opt_indices.items.ptr,
        opt_indices.items.len,
        opt_vertices.items.len,
    );
    const num_opt_vertices = c.meshopt_optimizeVertexFetch(
        opt_vertices.items.ptr,
        opt_indices.items.ptr,
        opt_indices.items.len,
        opt_vertices.items.ptr,
        opt_vertices.items.len,
        @sizeOf(Vertex),
    );
    assert(num_opt_vertices == opt_vertices.items.len);

    const max_num_meshlet_vertices = 64;
    const max_num_meshlet_triangles = 128;
    const max_num_meshlets = c.meshopt_buildMeshletsBound(
        opt_indices.items.len,
        max_num_meshlet_vertices,
        max_num_meshlet_triangles,
    );

    var meshlets = std.ArrayList(c.meshopt_Meshlet).init(arena_allocator);
    var meshlet_vertices = std.ArrayList(u32).init(arena_allocator);
    var meshlet_triangles = std.ArrayList(u8).init(arena_allocator);
    meshlets.resize(max_num_meshlets) catch unreachable;
    meshlet_vertices.resize(max_num_meshlets * max_num_meshlet_vertices) catch unreachable;
    meshlet_triangles.resize(max_num_meshlets * max_num_meshlet_triangles * 3) catch unreachable;

    const num_meshlets = c.meshopt_buildMeshlets(
        meshlets.items.ptr,
        meshlet_vertices.items.ptr,
        meshlet_triangles.items.ptr,
        opt_indices.items.ptr,
        opt_indices.items.len,
        @ptrCast([*c]const f32, opt_vertices.items.ptr),
        opt_vertices.items.len,
        @sizeOf(Vertex),
        max_num_meshlet_vertices,
        max_num_meshlet_triangles,
        0.0,
    );
    assert(num_meshlets <= max_num_meshlets);
    meshlets.resize(num_meshlets) catch unreachable;

    all_meshes.append(.{
        .index_offset = @intCast(u32, all_indices.items.len),
        .vertex_offset = @intCast(u32, all_vertices.items.len),
        .meshlet_offset = @intCast(u32, all_meshlets.items.len),
        .num_indices = @intCast(u32, opt_indices.items.len),
        .num_vertices = @intCast(u32, opt_vertices.items.len),
        .num_meshlets = @intCast(u32, meshlets.items.len),
    }) catch unreachable;

    for (meshlets.items) |src_meshlet| {
        const meshlet = Meshlet{
            .data_offset = @intCast(u32, all_meshlets_data.items.len),
            .num_vertices = @intCast(u16, src_meshlet.vertex_count),
            .num_triangles = @intCast(u16, src_meshlet.triangle_count),
        };
        all_meshlets.append(meshlet) catch unreachable;

        var i: u32 = 0;
        while (i < src_meshlet.vertex_count) : (i += 1) {
            all_meshlets_data.append(meshlet_vertices.items[src_meshlet.vertex_offset + i]) catch unreachable;
        }

        i = 0;
        while (i < src_meshlet.triangle_count) : (i += 1) {
            const index0 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 0]);
            const index1 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 1]);
            const index2 = @intCast(u10, meshlet_triangles.items[src_meshlet.triangle_offset + i * 3 + 2]);
            const prim = @intCast(u32, index0) | (@intCast(u32, index1) << 10) | (@intCast(u32, index2) << 20);
            all_meshlets_data.append(prim) catch unreachable;
        }
    }

    all_indices.appendSlice(opt_indices.items) catch unreachable;
    all_vertices.appendSlice(opt_vertices.items) catch unreachable;
}

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const window = lib.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;

    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    _ = pix.loadGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);

    // Check for Mesh Shader support.
    {
        var options7: d3d12.FEATURE_DATA_D3D12_OPTIONS7 = undefined;
        const res = grfx.device.CheckFeatureSupport(.OPTIONS7, &options7, @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS7));
        if (options7.MeshShaderTier == .NOT_SUPPORTED or res != w.S_OK) {
            _ = w.user32.messageBoxA(
                window,
                "This applications requires graphics card that supports Mesh Shader " ++
                    "(NVIDIA GeForce Turing or newer, AMD Radeon RX 6000 or newer).",
                "No DirectX 12 Mesh Shader support",
                w.user32.MB_OK | w.user32.MB_ICONERROR,
            ) catch 0;
            w.kernel32.ExitProcess(0);
        }
    }

    const brush = blk: {
        var brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &brush,
        ));
        break :blk brush.?;
    };

    const normal_tfmt = blk: {
        var txtfmt: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.BOLD,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            &txtfmt,
        ));
        break :blk txtfmt.?;
    };
    hrPanicOnFail(normal_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(normal_tfmt.SetParagraphAlignment(.NEAR));

    const mesh_shader_pso = blk: {
        var pso_desc = d3d12.MESH_SHADER_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.SampleDesc = .{ .Count = 1, .Quality = 0 };

        break :blk grfx.createMeshShaderPipeline(
            arena_allocator,
            &pso_desc,
            null,
            "content/shaders/mesh_shader.ms.cso",
            "content/shaders/mesh_shader.ps.cso",
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

        break :blk grfx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            "content/shaders/vertex_shader.vs.cso",
            "content/shaders/vertex_shader.ps.cso",
        );
    };

    var all_meshes = std.ArrayList(Mesh).init(gpa_allocator);
    var all_vertices = std.ArrayList(Vertex).init(arena_allocator);
    var all_indices = std.ArrayList(u32).init(arena_allocator);
    var all_meshlets = std.ArrayList(Meshlet).init(arena_allocator);
    var all_meshlets_data = std.ArrayList(u32).init(arena_allocator);
    loadMeshAndGenerateMeshlets(
        arena_allocator,
        "content/engine.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
        &all_meshlets,
        &all_meshlets_data,
    );

    var entities = std.ArrayList(Entity).init(gpa_allocator);

    const world_half_extent: f32 = 5.01;
    {
        const spread: f32 = 2.5;

        var y: f32 = -world_half_extent;
        while (y < world_half_extent) : (y += spread) {
            var x: f32 = -world_half_extent;
            while (x < world_half_extent) : (x += spread) {
                entities.append(.{
                    .position = Vec3.init(x, y, 0),
                    .base_color_roughness = Vec4.init(1.0, 1.0, 1.0, -0.5),
                    .mesh_index = 0,
                }) catch unreachable;
            }
        }
    }

    const vertex_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const vertex_buffer_srv = blk: {
        const srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(vertex_buffer),
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
                0,
                @intCast(u32, all_vertices.items.len),
                @sizeOf(Vertex),
            ),
            srv,
        );
        break :blk srv;
    };

    const index_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const index_buffer_srv = blk: {
        const srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(index_buffer),
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32_UINT, 0, @intCast(u32, all_indices.items.len)),
            srv,
        );
        break :blk srv;
    };

    const meshlet_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_meshlets.items.len * @sizeOf(Meshlet)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const meshlet_buffer_srv = blk: {
        const srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(meshlet_buffer),
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
                0,
                @intCast(u32, all_meshlets.items.len),
                @sizeOf(Meshlet),
            ),
            srv,
        );
        break :blk srv;
    };

    const meshlet_data_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(all_meshlets_data.items.len * @sizeOf(u32)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    const meshlet_data_buffer_srv = blk: {
        const srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(meshlet_data_buffer),
            &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32_UINT, 0, @intCast(u32, all_meshlets_data.items.len)),
            srv,
        );
        break :blk srv;
    };

    const depth_texture = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, grfx.viewport_width, grfx.viewport_height, 1);
            desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | d3d12.RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            break :blk desc;
        },
        d3d12.RESOURCE_STATE_DEPTH_WRITE,
        &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
    ) catch |err| hrPanic(err);

    const depth_texture_dsv = blk: {
        const dsv = grfx.allocateCpuDescriptors(.DSV, 1);
        grfx.device.CreateDepthStencilView(grfx.getResource(depth_texture), null, dsv);
        break :blk dsv;
    };

    //
    // Begin data upload to the GPU.
    //
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(arena_allocator, &grfx, 1);

    // Upload vertex buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| upload.cpu_slice[i] = vertex;
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(vertex_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();
    }

    // Upload index buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| upload.cpu_slice[i] = index;
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(index_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();
    }

    // Upload meshlet buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(Meshlet, @intCast(u32, all_meshlets.items.len));
        for (all_meshlets.items) |meshlet, i| upload.cpu_slice[i] = meshlet;
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(meshlet_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(meshlet_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();
    }

    // Upload meshlet data buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(u32, @intCast(u32, all_meshlets_data.items.len));
        for (all_meshlets_data.items) |meshlet_data, i| upload.cpu_slice[i] = meshlet_data;
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(meshlet_data_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(meshlet_data_buffer, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();
    }

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .mesh_shader_pso = mesh_shader_pso,
        .vertex_shader_pso = vertex_shader_pso,
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
        .entities = entities,
        .brush = brush,
        .normal_tfmt = normal_tfmt,
        .use_mesh_shader = true,
        .camera = .{
            .position = Vec3.init(0.0, 0.0, -world_half_extent),
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

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    demo.entities.deinit();
    demo.meshes.deinit();
    _ = demo.brush.Release();
    _ = demo.normal_tfmt.Release();
    _ = demo.grfx.releaseResource(demo.vertex_buffer);
    _ = demo.grfx.releaseResource(demo.index_buffer);
    _ = demo.grfx.releaseResource(demo.meshlet_buffer);
    _ = demo.grfx.releaseResource(demo.meshlet_data_buffer);
    _ = demo.grfx.releaseResource(demo.depth_texture);
    _ = demo.grfx.releasePipeline(demo.mesh_shader_pso);
    _ = demo.grfx.releasePipeline(demo.vertex_shader_pso);
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa_allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    const dt = demo.frame_stats.delta_time;
    lib.newImGuiFrame(dt);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.grfx.viewport_width) - 600.0 - 20, .y = 20.0 },
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

    var draw_mode: i32 = if (demo.use_mesh_shader) 0 else 1;
    _ = c.igRadioButton_IntPtr("Use Mesh Shader", &draw_mode, 0);
    _ = c.igRadioButton_IntPtr("Use Vertex Shader with programmable vertex fetch", &draw_mode, 1);
    demo.use_mesh_shader = if (draw_mode == 0) true else false;

    c.igEnd();

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
            demo.camera.yaw = vm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed: f32 = 5.0;
        const delta_time = demo.frame_stats.delta_time;
        const transform = Mat4.initRotationX(demo.camera.pitch).mul(Mat4.initRotationY(demo.camera.yaw));
        var forward = Vec3.init(0.0, 0.0, 1.0).transform(transform).normalize();

        demo.camera.forward = forward;
        const right = Vec3.init(0.0, 1.0, 0.0).cross(forward).normalize().scale(speed * delta_time);
        forward = forward.scale(speed * delta_time);

        if (w.GetAsyncKeyState('W') < 0) {
            demo.camera.position = demo.camera.position.add(forward);
        } else if (w.GetAsyncKeyState('S') < 0) {
            demo.camera.position = demo.camera.position.sub(forward);
        }
        if (w.GetAsyncKeyState('D') < 0) {
            demo.camera.position = demo.camera.position.add(right);
        } else if (w.GetAsyncKeyState('A') < 0) {
            demo.camera.position = demo.camera.position.sub(right);
        }
    }
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const cam_world_to_view = Mat4.initLookToLh(
        demo.camera.position,
        demo.camera.forward,
        Vec3.init(0.0, 1.0, 0.0),
    );
    const cam_view_to_clip = Mat4.initPerspectiveFovLh(
        math.pi / 3.0,
        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
        0.01,
        200.0,
    );
    const cam_world_to_clip = cam_world_to_view.mul(cam_view_to_clip);

    const back_buffer = grfx.getBackBuffer();
    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        &demo.depth_texture_dsv,
    );
    grfx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.1, 0.2, 0.4, 1.0 },
        0,
        null,
    );

    //
    // Draw all entities.
    //
    const use_mesh_shader = demo.use_mesh_shader;
    grfx.setCurrentPipeline(if (use_mesh_shader) demo.mesh_shader_pso else demo.vertex_shader_pso);
    grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

    // Bind global buffers that contain data for *all meshes* and *all meshlets*.
    grfx.cmdlist.SetGraphicsRootDescriptorTable(3, blk: {
        const table = grfx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer_srv);
        _ = grfx.copyDescriptorsToGpuHeap(1, demo.index_buffer_srv);
        if (use_mesh_shader) {
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.meshlet_buffer_srv);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.meshlet_data_buffer_srv);
        }
        break :blk table;
    });

    // Upload per-frame constant data.
    {
        const mem = grfx.allocateUploadMemory(Pso_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .world_to_clip = cam_world_to_clip.transpose(),
            .camera_position = demo.camera.position,
        };
        grfx.cmdlist.SetGraphicsRootConstantBufferView(2, mem.gpu_base);
    }

    for (demo.entities.items) |entity| {
        // Upload per-draw constant data.
        {
            const mem = grfx.allocateUploadMemory(Pso_DrawConst, 1);
            mem.cpu_slice[0] = .{
                .object_to_world = Mat4.initTranslation(entity.position).transpose(),
                .base_color_roughness = entity.base_color_roughness,
            };
            grfx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        }

        const mesh = &demo.meshes.items[entity.mesh_index];

        // Select a mesh to draw by specifying offsets in global buffers.
        grfx.cmdlist.SetGraphicsRoot32BitConstants(0, 2, &[_]u32{
            mesh.vertex_offset,
            if (use_mesh_shader) mesh.meshlet_offset else mesh.index_offset,
        }, 0);

        if (use_mesh_shader) {
            grfx.cmdlist.DispatchMesh(mesh.num_meshlets, 1, 1);
        } else {
            grfx.cmdlist.DrawInstanced(mesh.num_indices, 1, 0, 0);
        }
    }

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
            demo.normal_tfmt,
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
