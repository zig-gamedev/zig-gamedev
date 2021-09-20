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
const Vec2 = vm.Vec2;
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: simple raytracer";
const window_width = 1920;
const window_height = 1080;

const Vertex = struct {
    position: Vec3,
    normal: Vec3,
    texcoords0: Vec2,
    tangent: Vec4,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    num_indices: u32,
    num_vertices: u32,
    material_index: u32,
};

const Material = struct {
    base_color: Vec3,
    roughness: f32,
    metallic: f32,
    base_color_tex_index: u16,
    metallic_roughness_tex_index: u16,
    normal_tex_index: u16,
};

const ResourceView = struct {
    resource: gr.ResourceHandle,
    view: d3d12.CPU_DESCRIPTOR_HANDLE,
};

const PsoStaticMesh_FrameConst = struct {
    object_to_clip: Mat4,
    object_to_world: Mat4,
    camera_position: Vec3,
    padding0: f32 = 0.0,
};
comptime {
    assert(@sizeOf(PsoStaticMesh_FrameConst) == 128 + 16);
}

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    static_mesh_pso: gr.PipelineHandle,

    depth_texture: ResourceView,
    vertex_buffer: ResourceView,
    index_buffer: ResourceView,

    brush: *d2d1.ISolidColorBrush,
    info_tfmt: *dwrite.ITextFormat,

    meshes: std.ArrayList(Mesh),
    materials: std.ArrayList(Material),
    textures: std.ArrayList(ResourceView),

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

fn parseAndLoadGltfFile(gltf_path: []const u8) *c.cgltf_data {
    var data: *c.cgltf_data = undefined;
    const options = std.mem.zeroes(c.cgltf_options);
    // Parse.
    {
        const result = c.cgltf_parse_file(&options, gltf_path.ptr, @ptrCast([*c][*c]c.cgltf_data, &data));
        assert(result == c.cgltf_result_success);
    }
    // Load.
    {
        const result = c.cgltf_load_buffers(&options, data, gltf_path.ptr);
        assert(result == c.cgltf_result_success);
    }
    return data;
}

fn appendMeshPrimitive(
    data: *c.cgltf_data,
    mesh_index: u32,
    prim_index: u32,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList(Vec3),
    normals: ?*std.ArrayList(Vec3),
    texcoords0: ?*std.ArrayList(Vec2),
    tangents: ?*std.ArrayList(Vec4),
) void {
    assert(mesh_index < data.meshes_count);
    assert(prim_index < data.meshes[mesh_index].primitives_count);
    const num_vertices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].indices.*.count);

    // Indices.
    {
        indices.ensureTotalCapacity(indices.items.len + num_indices) catch unreachable;

        const accessor = data.meshes[mesh_index].primitives[prim_index].indices;

        assert(accessor.*.buffer_view != null);
        assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
        assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
        assert(accessor.*.buffer_view.*.buffer.*.data != null);

        const data_addr = @alignCast(4, @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
            accessor.*.offset + accessor.*.buffer_view.*.offset);

        if (accessor.*.stride == 1) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_8u);
            const src = @ptrCast([*]const u8, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 2) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_16u);
            const src = @ptrCast([*]const u16, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 4) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_32u);
            const src = @ptrCast([*]const u32, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else {
            unreachable;
        }
    }

    // Attributes.
    {
        positions.resize(positions.items.len + num_vertices) catch unreachable;
        if (normals != null) normals.?.resize(normals.?.items.len + num_vertices) catch unreachable;
        if (texcoords0 != null) texcoords0.?.resize(texcoords0.?.items.len + num_vertices) catch unreachable;
        if (tangents != null) tangents.?.resize(tangents.?.items.len + num_vertices) catch unreachable;

        const num_attribs: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.meshes[mesh_index].primitives[prim_index].attributes[attrib_index];
            const accessor = attrib.data;

            assert(accessor.*.buffer_view != null);
            assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
            assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
            assert(accessor.*.buffer_view.*.buffer.*.data != null);

            const data_addr = @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
                accessor.*.offset + accessor.*.buffer_view.*.offset;

            if (attrib.*.type == c.cgltf_attribute_type_position) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &positions.items[positions.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_normal and normals != null) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &normals.?.items[normals.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_texcoord and texcoords0 != null) {
                assert(accessor.*.type == c.cgltf_type_vec2);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &texcoords0.?.items[texcoords0.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_tangent and tangents != null) {
                assert(accessor.*.type == c.cgltf_type_vec4);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &tangents.?.items[tangents.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            }
        }
    }
}

fn loadScene(
    arena: *std.mem.Allocator,
    grfx: *gr.GraphicsContext,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
    all_materials: *std.ArrayList(Material),
    all_textures: *std.ArrayList(ResourceView),
) void {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList(Vec3).init(arena);
    var normals = std.ArrayList(Vec3).init(arena);
    var texcoords0 = std.ArrayList(Vec2).init(arena);
    var tangents = std.ArrayList(Vec4).init(arena);

    const data = parseAndLoadGltfFile("content/Sponza/Sponza.gltf");
    defer c.cgltf_free(data);

    const num_meshes = @intCast(u32, data.meshes_count);
    var mesh_index: u32 = 0;

    while (mesh_index < num_meshes) : (mesh_index += 1) {
        const num_prims = @intCast(u32, data.meshes[mesh_index].primitives_count);
        var prim_index: u32 = 0;

        while (prim_index < num_prims) : (prim_index += 1) {
            const pre_indices_len = indices.items.len;
            const pre_positions_len = positions.items.len;

            appendMeshPrimitive(data, mesh_index, prim_index, &indices, &positions, &normals, &texcoords0, &tangents);

            const num_materials = @intCast(u32, data.materials_count);
            var material_index: u32 = 0;
            var assigned_material_index: u32 = 0xffff_ffff;

            while (material_index < num_materials) : (material_index += 1) {
                const prim = &data.meshes[mesh_index].primitives[prim_index];
                if (prim.material == &data.materials[material_index]) {
                    assigned_material_index = material_index;
                    break;
                }
            }
            assert(assigned_material_index != 0xffff_ffff);

            all_meshes.append(.{
                .index_offset = @intCast(u32, pre_indices_len),
                .vertex_offset = @intCast(u32, pre_positions_len),
                .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
                .num_vertices = @intCast(u32, positions.items.len - pre_positions_len),
                .material_index = assigned_material_index,
            }) catch unreachable;
        }
    }

    all_indices.ensureTotalCapacity(indices.items.len) catch unreachable;
    for (indices.items) |index| {
        all_indices.appendAssumeCapacity(index);
    }

    all_vertices.ensureTotalCapacity(positions.items.len) catch unreachable;
    for (positions.items) |_, index| {
        all_vertices.appendAssumeCapacity(.{
            .position = positions.items[index],
            .normal = normals.items[index],
            .texcoords0 = texcoords0.items[index],
            .tangent = tangents.items[index],
        });
    }

    const num_materials = @intCast(u32, data.materials_count);
    var material_index: u32 = 0;
    all_materials.ensureTotalCapacity(num_materials) catch unreachable;

    while (material_index < num_materials) : (material_index += 1) {
        const gltf_material = &data.materials[material_index];
        assert(gltf_material.has_pbr_metallic_roughness == 1);

        const mr = &gltf_material.pbr_metallic_roughness;

        const num_images = @intCast(u32, data.images_count);
        const invalid_image_index = num_images;

        var base_color_tex_index: u32 = invalid_image_index;
        var metallic_roughness_tex_index: u32 = invalid_image_index;
        var normal_tex_index: u32 = invalid_image_index;

        var image_index: u32 = 0;

        while (image_index < num_images) : (image_index += 1) {
            const image = &data.images[image_index];
            assert(image.uri != null);

            if (mr.base_color_texture.texture != null and
                mr.base_color_texture.texture.*.image.*.uri == image.uri)
            {
                assert(base_color_tex_index == invalid_image_index);
                base_color_tex_index = image_index;
            }

            if (mr.metallic_roughness_texture.texture != null and
                mr.metallic_roughness_texture.texture.*.image.*.uri == image.uri)
            {
                assert(metallic_roughness_tex_index == invalid_image_index);
                metallic_roughness_tex_index = image_index;
            }

            if (gltf_material.normal_texture.texture != null and
                gltf_material.normal_texture.texture.*.image.*.uri == image.uri)
            {
                assert(normal_tex_index == invalid_image_index);
                normal_tex_index = image_index;
            }
        }
        assert(base_color_tex_index != invalid_image_index);

        all_materials.appendAssumeCapacity(.{
            .base_color = Vec3.init(mr.base_color_factor[0], mr.base_color_factor[1], mr.base_color_factor[2]),
            .roughness = mr.roughness_factor,
            .metallic = mr.metallic_factor,
            .base_color_tex_index = @intCast(u16, base_color_tex_index),
            .metallic_roughness_tex_index = @intCast(u16, metallic_roughness_tex_index),
            .normal_tex_index = @intCast(u16, normal_tex_index),
        });
    }

    const num_images = @intCast(u32, data.images_count);
    var image_index: u32 = 0;
    all_textures.ensureTotalCapacity(num_images + 1) catch unreachable;

    while (image_index < num_images) : (image_index += 1) {
        const image = &data.images[image_index];

        var buffer: [64]u8 = undefined;
        const path = std.fmt.bufPrint(buffer[0..], "content/Sponza/{s}", .{image.uri}) catch unreachable;

        const texture = grfx.createAndUploadTex2dFromFile(path, 0) catch unreachable;
        const view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(grfx.getResource(texture), null, view);

        all_textures.appendAssumeCapacity(.{ .resource = texture, .view = view });
    }

    const texture_4x4 = ResourceView{
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, 4, 4, 0),
            d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(grfx.getResource(texture_4x4.resource), null, texture_4x4.view);

    all_textures.appendAssumeCapacity(texture_4x4);
}

fn init(gpa: *std.mem.Allocator) DemoState {
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

    const info_tfmt = blk: {
        var info_tfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            @ptrCast(*?*dwrite.ITextFormat, &info_tfmt),
        ));
        break :blk info_tfmt;
    };
    hrPanicOnFail(info_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(info_tfmt.SetParagraphAlignment(.NEAR));

    const static_mesh_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.DSVFormat = .D32_FLOAT;
        break :blk grfx.createGraphicsShaderPipeline(
            &arena_allocator.allocator,
            &pso_desc,
            "content/shaders/rast_static_mesh.vs.cso",
            "content/shaders/rast_static_mesh.ps.cso",
        );
    };

    const depth_texture = .{
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, grfx.viewport_width, grfx.viewport_height, 1);
                desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | d3d12.RESOURCE_FLAG_DENY_SHADER_RESOURCE;
                break :blk desc;
            },
            d3d12.RESOURCE_STATE_DEPTH_WRITE,
            &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.DSV, 1),
    };
    grfx.device.CreateDepthStencilView(grfx.getResource(depth_texture.resource), null, depth_texture.view);

    var mipgen_rgba8 = gr.MipmapGenerator.init(&arena_allocator.allocator, &grfx, .R8G8B8A8_UNORM);

    //
    // Begin frame to init/upload resources on the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx);

    var all_meshes = std.ArrayList(Mesh).init(gpa);
    var all_vertices = std.ArrayList(Vertex).init(&arena_allocator.allocator);
    var all_indices = std.ArrayList(u32).init(&arena_allocator.allocator);
    var all_materials = std.ArrayList(Material).init(gpa);
    var all_textures = std.ArrayList(ResourceView).init(gpa);
    loadScene(
        &arena_allocator.allocator,
        &grfx,
        &all_meshes,
        &all_vertices,
        &all_indices,
        &all_materials,
        &all_textures,
    );

    for (all_textures.items) |texture| {
        mipgen_rgba8.generateMipmaps(&grfx, texture.resource);
        grfx.addTransitionBarrier(texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
    grfx.flushResourceBarriers();

    const vertex_buffer = .{
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(vertex_buffer.resource),
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
            0,
            @intCast(u32, all_vertices.items.len),
            @sizeOf(Vertex),
        ),
        vertex_buffer.view,
    );

    const index_buffer = .{
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(index_buffer.resource),
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32_UINT, 0, @intCast(u32, all_indices.items.len)),
        index_buffer.view,
    );

    // Upload vertex buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| {
            upload.cpu_slice[i] = vertex;
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(vertex_buffer.resource),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(vertex_buffer.resource, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    }

    // Upload index buffer.
    {
        const upload = grfx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| {
            upload.cpu_slice[i] = index;
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(index_buffer.resource),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(index_buffer.resource, d3d12.RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    }

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    mipgen_rgba8.deinit(&grfx);

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .static_mesh_pso = static_mesh_pso,
        .brush = brush,
        .info_tfmt = info_tfmt,
        .meshes = all_meshes,
        .materials = all_materials,
        .textures = all_textures,
        .depth_texture = depth_texture,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .camera = .{
            .position = Vec3.init(0.0, 1.0, 0.0),
            .forward = Vec3.initZero(),
            .pitch = 0.0,
            .yaw = math.pi + 0.25 * math.pi,
        },
        .mouse = .{
            .cursor_prev_x = 0,
            .cursor_prev_y = 0,
        },
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.grfx.releaseResource(demo.depth_texture.resource);
    _ = demo.grfx.releaseResource(demo.vertex_buffer.resource);
    _ = demo.grfx.releaseResource(demo.index_buffer.resource);
    _ = demo.grfx.releasePipeline(demo.static_mesh_pso);
    for (demo.textures.items) |texture| {
        _ = demo.grfx.releaseResource(texture.resource);
    }
    demo.meshes.deinit();
    demo.materials.deinit();
    demo.textures.deinit();
    _ = demo.brush.Release();
    _ = demo.info_tfmt.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    lib.newImGuiFrame(demo.frame_stats.delta_time);

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

    const cam_world_to_view = vm.Mat4.initLookToLh(
        demo.camera.position,
        demo.camera.forward,
        vm.Vec3.init(0.0, 1.0, 0.0),
    );
    const cam_view_to_clip = vm.Mat4.initPerspectiveFovLh(
        math.pi / 3.0,
        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
        0.1,
        100.0,
    );
    const cam_world_to_clip = cam_world_to_view.mul(cam_view_to_clip);

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        &demo.depth_texture.view,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );
    grfx.cmdlist.ClearDepthStencilView(demo.depth_texture.view, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);
    grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

    // Draw Sponza.
    {
        const object_to_world = vm.Mat4.initIdentity();
        const object_to_clip = object_to_world.mul(cam_world_to_clip);

        const mem = grfx.allocateUploadMemory(PsoStaticMesh_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
            .object_to_world = object_to_world.transpose(),
            .camera_position = demo.camera.position,
        };

        grfx.setCurrentPipeline(demo.static_mesh_pso);
        grfx.cmdlist.SetGraphicsRootConstantBufferView(2, mem.gpu_base);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(3, blk: {
            const table = grfx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer.view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.index_buffer.view);
            break :blk table;
        });
        for (demo.meshes.items) |mesh| {
            grfx.cmdlist.SetGraphicsRoot32BitConstants(0, 2, &.{ mesh.vertex_offset, mesh.index_offset }, 0);
            grfx.cmdlist.SetGraphicsRootDescriptorTable(1, blk: {
                const color_index = demo.materials.items[mesh.material_index].base_color_tex_index;
                const mr_index = demo.materials.items[mesh.material_index].metallic_roughness_tex_index;
                const normal_index = demo.materials.items[mesh.material_index].normal_tex_index;

                const table = grfx.copyDescriptorsToGpuHeap(1, demo.textures.items[color_index].view);
                _ = grfx.copyDescriptorsToGpuHeap(1, demo.textures.items[mr_index].view);
                _ = grfx.copyDescriptorsToGpuHeap(1, demo.textures.items[normal_index].view);
                break :blk table;
            });
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
        lib.DrawText(
            grfx.d2d.context,
            text,
            demo.info_tfmt,
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
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

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
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch unreachable;
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
