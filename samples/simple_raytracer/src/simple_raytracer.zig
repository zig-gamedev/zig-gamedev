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
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;
const zpix = @import("zpix");

const Vec2 = vm.Vec2;
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

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
    resource: zd3d12.ResourceHandle,
    view: d3d12.CPU_DESCRIPTOR_HANDLE,
};

const PsoStaticMesh_FrameConst = struct {
    object_to_clip: Mat4,
    object_to_world: Mat4,
    camera_position: Vec3,
    padding0: f32 = 0.0,
    light_position: Vec3,
    draw_mode: i32, // 0 - no shadows, 1 - shadows, 2 - shadow mask
};
comptime {
    assert(@sizeOf(PsoStaticMesh_FrameConst) == 128 + 32);
}

const PsoZPrePass_FrameConst = struct {
    object_to_clip: Mat4,
};

const PsoGenShadowRays_FrameConst = struct {
    object_to_clip: Mat4,
    object_to_world: Mat4,
};

const PsoTraceShadowRays_FrameConst = struct {
    light_position: Vec3,
    padding0: f32 = 0.0,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    static_mesh_pso: zd3d12.PipelineHandle,
    z_pre_pass_pso: zd3d12.PipelineHandle,
    gen_shadow_rays_pso: zd3d12.PipelineHandle,

    trace_shadow_rays_stateobj: ?*d3d12.IStateObject,
    trace_shadow_rays_rs: ?*d3d12.IRootSignature,
    trace_shadow_rays_table: zd3d12.ResourceHandle,

    depth_texture: zd3d12.ResourceHandle,
    depth_texture_dsv: d3d12.CPU_DESCRIPTOR_HANDLE,
    depth_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    shadow_rays_texture: zd3d12.ResourceHandle,
    shadow_rays_texture_rtv: d3d12.CPU_DESCRIPTOR_HANDLE,
    shadow_rays_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    shadow_mask_texture: zd3d12.ResourceHandle,
    shadow_mask_texture_uav: d3d12.CPU_DESCRIPTOR_HANDLE,
    shadow_mask_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    vertex_buffer: ResourceView,
    index_buffer: ResourceView,

    blas_buffer: zd3d12.ResourceHandle,
    tlas_buffer: zd3d12.ResourceHandle,

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
    light_position: Vec3,

    dxr_is_supported: bool,
    dxr_draw_mode: i32, // 0 - no shadows, 1 - shadows, 2 - shadow mask
};

fn parseAndLoadGltfFile(path: []const u8) *c.cgltf_data {
    var data: *c.cgltf_data = undefined;
    const options = std.mem.zeroes(c.cgltf_options);

    var buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
    var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
    const allocator = fba.allocator();

    const full_path = std.fs.path.joinZ(allocator, &.{
        std.fs.selfExeDirPathAlloc(allocator) catch unreachable,
        path,
    }) catch unreachable;

    // Parse.
    {
        const result = c.cgltf_parse_file(
            &options,
            full_path.ptr,
            @ptrCast([*c][*c]c.cgltf_data, &data),
        );
        assert(result == c.cgltf_result_success);
    }
    // Load.
    {
        const result = c.cgltf_load_buffers(&options, data, full_path.ptr);
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
    arena: std.mem.Allocator,
    gctx: *zd3d12.GraphicsContext,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
    all_materials: *std.ArrayList(Material),
    all_textures: *std.ArrayList(ResourceView),
) void {
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList(Vec3).init(arena);
    var normals = std.ArrayList(Vec3).init(arena);
    var texcoords0 = std.ArrayList(Vec2).init(arena);
    var tangents = std.ArrayList(Vec4).init(arena);

    const data = parseAndLoadGltfFile(content_dir ++ "Sponza/Sponza.gltf");
    defer c.cgltf_free(data);

    const num_meshes = @intCast(u32, data.meshes_count);
    var mesh_index: u32 = 0;

    while (mesh_index < num_meshes) : (mesh_index += 1) {
        const num_prims = @intCast(u32, data.meshes[mesh_index].primitives_count);
        var prim_index: u32 = 0;

        while (prim_index < num_prims) : (prim_index += 1) {
            const pre_indices_len = indices.items.len;
            const pre_positions_len = positions.items.len;

            appendMeshPrimitive(
                data,
                mesh_index,
                prim_index,
                &indices,
                &positions,
                &normals,
                &texcoords0,
                &tangents,
            );

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
            .position = positions.items[index].scale(0.008), // NOTE(mziulek): Sponza requires scaling.
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

        const relpath = std.fmt.allocPrint(
            arena,
            content_dir ++ "Sponza/{s}",
            .{image.uri},
        ) catch unreachable;

        const texture = gctx.createAndUploadTex2dFromFile(relpath, .{}) catch unreachable;
        const view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(gctx.lookupResource(texture).?, null, view);

        all_textures.appendAssumeCapacity(.{ .resource = texture, .view = view });
    }

    const texture_4x4 = ResourceView{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, 4, 4, 0),
            .{ .PIXEL_SHADER_RESOURCE = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(gctx.lookupResource(texture_4x4.resource).?, null, texture_4x4.view);

    all_textures.appendAssumeCapacity(texture_4x4);
}

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    _ = zpix.loadGpuCapturerLibrary();
    _ = zpix.setTargetWindow(window);
    _ = zpix.beginCapture(
        .{ .GPU = true },
        &.{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    // Check for DirectX Raytracing (DXR) support.
    const dxr_is_supported = blk: {
        var options5: d3d12.FEATURE_DATA_D3D12_OPTIONS5 = undefined;
        const res = gctx.device.CheckFeatureSupport(
            .OPTIONS5,
            &options5,
            @sizeOf(d3d12.FEATURE_DATA_D3D12_OPTIONS5),
        );
        break :blk options5.RaytracingTier != .NOT_SUPPORTED and res == w32.S_OK;
    };
    const dxr_draw_mode = @boolToInt(dxr_is_supported);

    const static_mesh_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/rast_static_mesh.vs.cso",
            content_dir ++ "shaders/rast_static_mesh.ps.cso",
        );
    };

    const z_pre_pass_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .UNKNOWN;
        pso_desc.NumRenderTargets = 0;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/z_pre_pass.vs.cso",
            content_dir ++ "shaders/z_pre_pass.ps.cso",
        );
    };

    const gen_shadow_rays_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R32G32B32A32_FLOAT;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.DepthStencilState.DepthWriteMask = .ZERO;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/gen_shadow_rays.vs.cso",
            content_dir ++ "shaders/gen_shadow_rays.ps.cso",
        );
    };

    // Create 'trace shadow rays' RT state object.
    var trace_shadow_rays_stateobj: ?*d3d12.IStateObject = null;
    var trace_shadow_rays_rs: ?*d3d12.IRootSignature = null;
    if (dxr_is_supported) {
        const full_path = std.fs.path.join(arena_allocator, &.{
            std.fs.selfExeDirPathAlloc(arena_allocator) catch unreachable,
            content_dir ++ "shaders/trace_shadow_rays.lib.cso",
        }) catch unreachable;

        const cso_file = std.fs.openFileAbsolute(full_path, .{}) catch unreachable;
        defer cso_file.close();

        const cso_code = cso_file.reader().readAllAlloc(arena_allocator, 256 * 1024) catch unreachable;

        const lib_desc = d3d12.DXIL_LIBRARY_DESC{
            .DXILLibrary = .{ .pShaderBytecode = cso_code.ptr, .BytecodeLength = cso_code.len },
            .NumExports = 0,
            .pExports = null,
        };

        const subobject = d3d12.STATE_SUBOBJECT{
            .Type = .DXIL_LIBRARY,
            .pDesc = &lib_desc,
        };

        const state_object_desc = d3d12.STATE_OBJECT_DESC{
            .Type = .RAYTRACING_PIPELINE,
            .NumSubobjects = 1,
            .pSubobjects = @ptrCast([*]const d3d12.STATE_SUBOBJECT, &subobject),
        };

        hrPanicOnFail(gctx.device.CreateStateObject(
            &state_object_desc,
            &d3d12.IID_IStateObject,
            @ptrCast(*?*anyopaque, &trace_shadow_rays_stateobj),
        ));
        hrPanicOnFail(gctx.device.CreateRootSignature(
            0,
            cso_code.ptr,
            cso_code.len,
            &d3d12.IID_IRootSignature,
            @ptrCast(*?*anyopaque, &trace_shadow_rays_rs),
        ));
    }

    const trace_shadow_rays_table = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(64 * 1024),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const depth_texture = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, gctx.viewport_width, gctx.viewport_height, 1);
            desc.Flags = .{ .ALLOW_DEPTH_STENCIL = true };
            break :blk desc;
        },
        .{ .DEPTH_WRITE = true },
        &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
    ) catch |err| hrPanic(err);

    const depth_texture_dsv = gctx.allocateCpuDescriptors(.DSV, 1);
    const depth_texture_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    gctx.device.CreateDepthStencilView(gctx.lookupResource(depth_texture).?, null, depth_texture_dsv);
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(depth_texture).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC{
            .Format = .R32_FLOAT,
            .ViewDimension = .TEXTURE2D,
            .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .Texture2D = .{
                    .MostDetailedMip = 0,
                    .MipLevels = 1,
                    .PlaneSlice = 0,
                    .ResourceMinLODClamp = 0.0,
                },
            },
        },
        depth_texture_srv,
    );

    const shadow_rays_texture = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(
                .R32G32B32A32_FLOAT,
                gctx.viewport_width,
                gctx.viewport_height,
                1,
            );
            desc.Flags = .{ .ALLOW_RENDER_TARGET = true };
            break :blk desc;
        },
        .{ .RENDER_TARGET = true },
        &d3d12.CLEAR_VALUE.initColor(.R32G32B32A32_FLOAT, &.{ 0.0, 0.0, 0.0, 0.0 }),
    ) catch |err| hrPanic(err);

    const shadow_rays_texture_rtv = gctx.allocateCpuDescriptors(.RTV, 1);
    const shadow_rays_texture_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    gctx.device.CreateRenderTargetView(
        gctx.lookupResource(shadow_rays_texture).?,
        null,
        shadow_rays_texture_rtv,
    );
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(shadow_rays_texture).?,
        null,
        shadow_rays_texture_srv,
    );

    const shadow_mask_texture = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &blk: {
            var desc = d3d12.RESOURCE_DESC.initTex2d(.R32_FLOAT, gctx.viewport_width, gctx.viewport_height, 1);
            desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
            break :blk desc;
        },
        .{ .UNORDERED_ACCESS = true },
        null,
    ) catch |err| hrPanic(err);

    const shadow_mask_texture_uav = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    const shadow_mask_texture_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);

    gctx.device.CreateUnorderedAccessView(
        gctx.lookupResource(shadow_mask_texture).?,
        null,
        null,
        shadow_mask_texture_uav,
    );
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(shadow_mask_texture).?,
        null,
        shadow_mask_texture_srv,
    );

    var mipgen_rgba8 = zd3d12.MipmapGenerator.init(arena_allocator, &gctx, .R8G8B8A8_UNORM, content_dir);

    //
    // Begin frame to init/upload resources on the GPU.
    //
    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    var all_meshes = std.ArrayList(Mesh).init(allocator);
    var all_vertices = std.ArrayList(Vertex).init(arena_allocator);
    var all_indices = std.ArrayList(u32).init(arena_allocator);
    var all_materials = std.ArrayList(Material).init(allocator);
    var all_textures = std.ArrayList(ResourceView).init(allocator);
    loadScene(
        arena_allocator,
        &gctx,
        &all_meshes,
        &all_vertices,
        &all_indices,
        &all_materials,
        &all_textures,
    );

    for (all_textures.items) |texture| {
        mipgen_rgba8.generateMipmaps(&gctx, texture.resource);
        gctx.addTransitionBarrier(texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
    }
    gctx.flushResourceBarriers();

    const vertex_buffer = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(vertex_buffer.resource).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(
            0,
            @intCast(u32, all_vertices.items.len),
            @sizeOf(Vertex),
        ),
        vertex_buffer.view,
    );

    const index_buffer = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(index_buffer.resource).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32_UINT, 0, @intCast(u32, all_indices.items.len)),
        index_buffer.view,
    );

    // Upload vertex buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| {
            upload.cpu_slice[i] = vertex;
        }
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(vertex_buffer.resource).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(vertex_buffer.resource, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    // Upload index buffer.
    {
        const upload = gctx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| {
            upload.cpu_slice[i] = index;
        }
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(index_buffer.resource).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(index_buffer.resource, .{ .NON_PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
    }

    var temp_resources = std.ArrayList(zd3d12.ResourceHandle).init(arena_allocator);

    // Create "Bottom Level Acceleration Structure" (blas).
    const blas_buffer = if (dxr_is_supported) blk_blas: {
        var geometry_descs = std.ArrayList(d3d12.RAYTRACING_GEOMETRY_DESC).initCapacity(
            arena_allocator,
            all_meshes.items.len,
        ) catch unreachable;

        const vertex_buffer_addr = gctx.lookupResource(vertex_buffer.resource).?.GetGPUVirtualAddress();
        const index_buffer_addr = gctx.lookupResource(index_buffer.resource).?.GetGPUVirtualAddress();

        for (all_meshes.items) |mesh| {
            const desc = d3d12.RAYTRACING_GEOMETRY_DESC{
                .Flags = .{ .OPAQUE = true },
                .Type = .TRIANGLES,
                .u = .{
                    .Triangles = .{
                        .Transform3x4 = 0,
                        .IndexFormat = .R32_UINT,
                        .VertexFormat = .R32G32B32_FLOAT,
                        .IndexCount = mesh.num_indices,
                        .VertexCount = mesh.num_vertices,
                        .IndexBuffer = index_buffer_addr + mesh.index_offset * @sizeOf(u32),
                        .VertexBuffer = .{
                            .StrideInBytes = @sizeOf(Vertex),
                            .StartAddress = vertex_buffer_addr + mesh.vertex_offset * @sizeOf(Vertex),
                        },
                    },
                },
            };
            geometry_descs.appendAssumeCapacity(desc);
        }

        const blas_inputs = d3d12.BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS{
            .Type = .BOTTOM_LEVEL,
            .Flags = .{ .PREFER_FAST_TRACE = true },
            .NumDescs = @intCast(u32, geometry_descs.items.len),
            .DescsLayout = .ARRAY,
            .u = .{
                .pGeometryDescs = geometry_descs.items.ptr,
            },
        };

        var blas_build_info: d3d12.RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO = undefined;
        gctx.device.GetRaytracingAccelerationStructurePrebuildInfo(&blas_inputs, &blas_build_info);
        std.log.info("BLAS: {}", .{blas_build_info});

        const blas_scratch_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initBuffer(blas_build_info.ScratchDataSizeInBytes);
                desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                break :blk desc;
            },
            .{ .UNORDERED_ACCESS = true },
            null,
        ) catch |err| hrPanic(err);
        temp_resources.append(blas_scratch_buffer) catch unreachable;

        const blas_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initBuffer(blas_build_info.ResultDataMaxSizeInBytes);
                desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                break :blk desc;
            },
            .{ .RAYTRACING_ACCELERATION_STRUCTURE = true },
            null,
        ) catch |err| hrPanic(err);

        const blas_desc = d3d12.BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC{
            .DestAccelerationStructureData = gctx.lookupResource(blas_buffer).?.GetGPUVirtualAddress(),
            .Inputs = blas_inputs,
            .SourceAccelerationStructureData = 0,
            .ScratchAccelerationStructureData = gctx.lookupResource(blas_scratch_buffer).?.GetGPUVirtualAddress(),
        };
        gctx.cmdlist.BuildRaytracingAccelerationStructure(&blas_desc, 0, null);
        gctx.cmdlist.ResourceBarrier(
            1,
            &[_]d3d12.RESOURCE_BARRIER{
                d3d12.RESOURCE_BARRIER.initUav(gctx.lookupResource(blas_buffer).?),
            },
        );

        break :blk_blas blas_buffer;
    } else blk_blas: {
        // DXR is not supported. Create a dummy BLAS buffer to simplify code.
        break :blk_blas gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(1),
            d3d12.RESOURCE_STATES.COMMON,
            null,
        ) catch |err| hrPanic(err);
    };

    // Create "Top Level Acceleration Structure" (tlas).
    const tlas_buffer = if (dxr_is_supported) blk_tlas: {
        const instance_desc = d3d12.RAYTRACING_INSTANCE_DESC{
            .Transform = [3][4]f32{
                [4]f32{ 1.0, 0.0, 0.0, 0.0 },
                [4]f32{ 0.0, 1.0, 0.0, 0.0 },
                [4]f32{ 0.0, 0.0, 1.0, 0.0 },
            },
            .p = .{
                .InstanceID = 0,
                .InstanceMask = 1,
                .InstanceContributionToHitGroupIndex = 0,
                .Flags = 0,
            },
            .AccelerationStructure = gctx.lookupResource(blas_buffer).?.GetGPUVirtualAddress(),
        };

        const instance_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(@sizeOf(d3d12.RAYTRACING_INSTANCE_DESC)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err);
        temp_resources.append(instance_buffer) catch unreachable;

        // Upload instance desc to instance buffer.
        {
            const upload = gctx.allocateUploadBufferRegion(d3d12.RAYTRACING_INSTANCE_DESC, 1);
            upload.cpu_slice[0] = instance_desc;

            gctx.cmdlist.CopyBufferRegion(
                gctx.lookupResource(instance_buffer).?,
                0,
                upload.buffer,
                upload.buffer_offset,
                upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
            );
            gctx.addTransitionBarrier(instance_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
            gctx.flushResourceBarriers();
        }

        const tlas_inputs = d3d12.BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS{
            .Type = .TOP_LEVEL,
            .Flags = .{ .PREFER_FAST_TRACE = true },
            .NumDescs = 1,
            .DescsLayout = .ARRAY,
            .u = .{
                .InstanceDescs = gctx.lookupResource(instance_buffer).?.GetGPUVirtualAddress(),
            },
        };

        var tlas_build_info: d3d12.RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO = undefined;
        gctx.device.GetRaytracingAccelerationStructurePrebuildInfo(&tlas_inputs, &tlas_build_info);
        std.log.info("TLAS: {}", .{tlas_build_info});

        const tlas_scratch_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initBuffer(tlas_build_info.ScratchDataSizeInBytes);
                desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                break :blk desc;
            },
            .{ .UNORDERED_ACCESS = true },
            null,
        ) catch |err| hrPanic(err);
        temp_resources.append(tlas_scratch_buffer) catch unreachable;

        const tlas_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initBuffer(tlas_build_info.ResultDataMaxSizeInBytes);
                desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                break :blk desc;
            },
            .{ .RAYTRACING_ACCELERATION_STRUCTURE = true },
            null,
        ) catch |err| hrPanic(err);

        const tlas_desc = d3d12.BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC{
            .DestAccelerationStructureData = gctx.lookupResource(tlas_buffer).?.GetGPUVirtualAddress(),
            .Inputs = tlas_inputs,
            .SourceAccelerationStructureData = 0,
            .ScratchAccelerationStructureData = gctx.lookupResource(tlas_scratch_buffer).?.GetGPUVirtualAddress(),
        };
        gctx.cmdlist.BuildRaytracingAccelerationStructure(&tlas_desc, 0, null);
        gctx.cmdlist.ResourceBarrier(
            1,
            &[_]d3d12.RESOURCE_BARRIER{
                d3d12.RESOURCE_BARRIER.initUav(gctx.lookupResource(tlas_buffer).?),
            },
        );

        break :blk_tlas tlas_buffer;
    } else blk_tlas: {
        // DXR is not supported. Create a dummy TLAS buffer to simplify code.
        break :blk_tlas gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(1),
            d3d12.RESOURCE_STATES.COMMON,
            null,
        ) catch |err| hrPanic(err);
    };

    gctx.endFrame();
    gctx.finishGpuCommands();

    _ = zpix.endCapture();

    mipgen_rgba8.deinit(&gctx);
    for (temp_resources.items) |resource| {
        gctx.destroyResource(resource);
    }

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .static_mesh_pso = static_mesh_pso,
        .z_pre_pass_pso = z_pre_pass_pso,
        .gen_shadow_rays_pso = gen_shadow_rays_pso,
        .trace_shadow_rays_stateobj = trace_shadow_rays_stateobj,
        .trace_shadow_rays_rs = trace_shadow_rays_rs,
        .trace_shadow_rays_table = trace_shadow_rays_table,
        .shadow_rays_texture = shadow_rays_texture,
        .shadow_rays_texture_rtv = shadow_rays_texture_rtv,
        .shadow_rays_texture_srv = shadow_rays_texture_srv,
        .shadow_mask_texture = shadow_mask_texture,
        .shadow_mask_texture_uav = shadow_mask_texture_uav,
        .shadow_mask_texture_srv = shadow_mask_texture_srv,
        .meshes = all_meshes,
        .materials = all_materials,
        .textures = all_textures,
        .depth_texture = depth_texture,
        .depth_texture_dsv = depth_texture_dsv,
        .depth_texture_srv = depth_texture_srv,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .blas_buffer = blas_buffer,
        .tlas_buffer = tlas_buffer,
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
        .light_position = Vec3.init(0.0, 5.0, 0.0),
        .dxr_is_supported = dxr_is_supported,
        .dxr_draw_mode = dxr_draw_mode,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    if (demo.dxr_is_supported) {
        _ = demo.trace_shadow_rays_stateobj.?.Release();
        _ = demo.trace_shadow_rays_rs.?.Release();
    }
    demo.meshes.deinit();
    demo.materials.deinit();
    demo.textures.deinit();
    demo.guir.deinit(&demo.gctx);
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.gctx.window, window_name);

    common.newImGuiFrame(demo.frame_stats.delta_time);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.gctx.viewport_width) - 600.0 - 20, .y = 20.0 },
        c.ImGuiCond_FirstUseEver,
        c.ImVec2{ .x = 0.0, .y = 0.0 },
    );
    c.igSetNextWindowSize(c.ImVec2{ .x = 600.0, .y = 0.0 }, c.ImGuiCond_FirstUseEver);
    _ = c.igBegin(
        "Demo Settings",
        null,
        c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
    );
    if (demo.dxr_is_supported) {
        c.igTextColored(
            c.ImVec4{ .x = 0.0, .y = 0.75, .z = 0.0, .w = 1.0 },
            "DirectX Raytracing (DXR) is supported.",
            "",
        );
    } else {
        c.igTextColored(
            c.ImVec4{ .x = 0.75, .y = 0.0, .z = 0.0, .w = 1.0 },
            "DirectX Raytracing (DXR) is NOT supported.",
            "",
        );
    }
    c.igBeginDisabled(!demo.dxr_is_supported);
    _ = c.igRadioButton_IntPtr("No Shadows", &demo.dxr_draw_mode, 0);
    _ = c.igRadioButton_IntPtr("Shadows", &demo.dxr_draw_mode, 1);
    _ = c.igRadioButton_IntPtr("Shadow Mask", &demo.dxr_draw_mode, 2);
    c.igEndDisabled();
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
        const speed: f32 = 5.0;
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

    demo.light_position.c[0] = @floatCast(f32, 0.5 * @sin(0.25 * demo.frame_stats.time));
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const cam_world_to_view = vm.Mat4.initLookToLh(
        demo.camera.position,
        demo.camera.forward,
        vm.Vec3.init(0.0, 1.0, 0.0),
    );
    const cam_view_to_clip = vm.Mat4.initPerspectiveFovLh(
        math.pi / 3.0,
        @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
        0.1,
        50.0,
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
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture_dsv, .{ .DEPTH = true }, 1.0, 0, 0, null);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

    // Z Pre Pass.
    {
        zpix.beginEvent(gctx.cmdlist, "Z Pre Pass");
        defer zpix.endEvent(gctx.cmdlist);

        const object_to_clip = cam_world_to_clip;

        const mem = gctx.allocateUploadMemory(PsoZPrePass_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
        };

        gctx.setCurrentPipeline(demo.z_pre_pass_pso);
        gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        gctx.cmdlist.SetGraphicsRootDescriptorTable(2, blk: {
            const table = gctx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer.view);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.index_buffer.view);
            break :blk table;
        });
        for (demo.meshes.items) |mesh| {
            gctx.cmdlist.SetGraphicsRoot32BitConstants(0, 2, &.{ mesh.vertex_offset, mesh.index_offset }, 0);
            gctx.cmdlist.DrawInstanced(mesh.num_indices, 1, 0, 0);
        }
    }

    gctx.addTransitionBarrier(demo.shadow_rays_texture, .{ .RENDER_TARGET = true });
    gctx.flushResourceBarriers();

    // Generate shadow rays.
    if (demo.dxr_is_supported and demo.dxr_draw_mode > 0) {
        zpix.beginEvent(gctx.cmdlist, "Generate shadow rays");
        defer zpix.endEvent(gctx.cmdlist);

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{demo.shadow_rays_texture_rtv},
            w32.TRUE,
            &demo.depth_texture_dsv,
        );
        gctx.cmdlist.ClearRenderTargetView(
            demo.shadow_rays_texture_rtv,
            &[4]f32{ 0.0, 0.0, 0.0, 0.0 },
            0,
            null,
        );

        const object_to_clip = cam_world_to_clip;

        const mem = gctx.allocateUploadMemory(PsoGenShadowRays_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
            .object_to_world = Mat4.initIdentity(),
        };

        gctx.setCurrentPipeline(demo.gen_shadow_rays_pso);
        gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);
        gctx.cmdlist.SetGraphicsRootDescriptorTable(2, blk: {
            const table = gctx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer.view);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.index_buffer.view);
            break :blk table;
        });
        for (demo.meshes.items) |mesh| {
            gctx.cmdlist.SetGraphicsRoot32BitConstants(0, 2, &.{ mesh.vertex_offset, mesh.index_offset }, 0);
            gctx.cmdlist.DrawInstanced(mesh.num_indices, 1, 0, 0);
        }

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w32.TRUE,
            &demo.depth_texture_dsv,
        );
    }

    gctx.addTransitionBarrier(demo.shadow_rays_texture, .{ .NON_PIXEL_SHADER_RESOURCE = true });
    gctx.addTransitionBarrier(demo.shadow_mask_texture, .{ .UNORDERED_ACCESS = true });
    gctx.flushResourceBarriers();

    // Trace shadow rays.
    if (demo.dxr_is_supported and demo.dxr_draw_mode > 0) {
        zpix.beginEvent(gctx.cmdlist, "Trace Shadow Rays");
        defer zpix.endEvent(gctx.cmdlist);

        // Upload 'shader table' content (in this demo it could be done only once at init time).
        {
            gctx.addTransitionBarrier(demo.trace_shadow_rays_table, .{ .COPY_DEST = true });
            gctx.flushResourceBarriers();

            const total_table_size = 192;
            const upload = gctx.allocateUploadBufferRegion(u8, total_table_size);

            var properties: *d3d12.IStateObjectProperties = undefined;
            hrPanicOnFail(demo.trace_shadow_rays_stateobj.?.QueryInterface(
                &d3d12.IID_IStateObjectProperties,
                @ptrCast(*?*anyopaque, &properties),
            ));
            defer _ = properties.Release();

            // ----------------------------------------------------------------------------------
            // | raygen (32 B) | 0 (32 B) | miss (32 B) | 0 (32 B) | hitgroup (32 B) | 0 (32 B) |
            // ----------------------------------------------------------------------------------
            @memcpy(
                upload.cpu_slice.ptr,
                @ptrCast([*]const u8, properties.GetShaderIdentifier(L("generateShadowRay"))),
                32,
            );
            @memset(upload.cpu_slice.ptr + 32, 0, 32);
            @memcpy(
                upload.cpu_slice.ptr + 64,
                @ptrCast([*]const u8, properties.GetShaderIdentifier(L("shadowMiss"))),
                32,
            );
            @memset(upload.cpu_slice.ptr + 64 + 32, 0, 32);
            @memcpy(
                upload.cpu_slice.ptr + 2 * 64,
                @ptrCast([*]const u8, properties.GetShaderIdentifier(L("g_shadow_hit_group"))),
                32,
            );
            @memset(upload.cpu_slice.ptr + 2 * 64 + 32, 0, 32);

            gctx.cmdlist.CopyBufferRegion(
                gctx.lookupResource(demo.trace_shadow_rays_table).?,
                0,
                upload.buffer,
                upload.buffer_offset,
                total_table_size,
            );
            gctx.addTransitionBarrier(demo.trace_shadow_rays_table, .{ .NON_PIXEL_SHADER_RESOURCE = true });
            gctx.flushResourceBarriers();
        }

        const mem = gctx.allocateUploadMemory(PsoTraceShadowRays_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .light_position = demo.light_position,
        };

        gctx.cmdlist.SetPipelineState1(demo.trace_shadow_rays_stateobj.?);
        gctx.cmdlist.SetComputeRootSignature(demo.trace_shadow_rays_rs.?);
        gctx.cmdlist.SetComputeRootShaderResourceView(
            0,
            gctx.lookupResource(demo.tlas_buffer).?.GetGPUVirtualAddress(),
        );
        gctx.cmdlist.SetComputeRootDescriptorTable(1, blk: {
            const table = gctx.copyDescriptorsToGpuHeap(1, demo.shadow_rays_texture_srv);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.shadow_mask_texture_uav);
            break :blk table;
        });
        gctx.cmdlist.SetComputeRootConstantBufferView(2, mem.gpu_base);

        const base_addr = gctx.lookupResource(demo.trace_shadow_rays_table).?.GetGPUVirtualAddress();
        const dispatch_desc = d3d12.DISPATCH_RAYS_DESC{
            .RayGenerationShaderRecord = .{ .StartAddress = base_addr, .SizeInBytes = 32 },
            .MissShaderTable = .{ .StartAddress = base_addr + 64, .SizeInBytes = 32, .StrideInBytes = 32 },
            .HitGroupTable = .{ .StartAddress = base_addr + 128, .SizeInBytes = 32, .StrideInBytes = 32 },
            .CallableShaderTable = .{ .StartAddress = 0, .SizeInBytes = 0, .StrideInBytes = 0 },
            .Width = gctx.viewport_width,
            .Height = gctx.viewport_height,
            .Depth = 1,
        };
        gctx.cmdlist.DispatchRays(&dispatch_desc);
    } else {
        const gpu_view = gctx.copyDescriptorsToGpuHeap(1, demo.shadow_mask_texture_uav);
        gctx.cmdlist.ClearUnorderedAccessViewFloat(
            gpu_view,
            demo.shadow_mask_texture_uav,
            gctx.lookupResource(demo.shadow_mask_texture).?,
            &.{ 1000.0, 0.0, 0.0, 0.0 },
            0,
            null,
        );
    }

    gctx.addTransitionBarrier(demo.shadow_mask_texture, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    // Draw Sponza.
    {
        zpix.beginEvent(gctx.cmdlist, "Main Pass");
        defer zpix.endEvent(gctx.cmdlist);

        const object_to_world = vm.Mat4.initIdentity();
        const object_to_clip = object_to_world.mul(cam_world_to_clip);

        const mem = gctx.allocateUploadMemory(PsoStaticMesh_FrameConst, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
            .object_to_world = object_to_world.transpose(),
            .camera_position = demo.camera.position,
            .light_position = demo.light_position,
            .draw_mode = demo.dxr_draw_mode,
        };

        gctx.setCurrentPipeline(demo.static_mesh_pso);
        gctx.cmdlist.SetGraphicsRootConstantBufferView(2, mem.gpu_base);
        gctx.cmdlist.SetGraphicsRootDescriptorTable(3, blk: {
            const table = gctx.copyDescriptorsToGpuHeap(1, demo.vertex_buffer.view);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.index_buffer.view);
            break :blk table;
        });
        gctx.cmdlist.SetGraphicsRootDescriptorTable(
            4,
            gctx.copyDescriptorsToGpuHeap(1, demo.shadow_mask_texture_srv),
        );
        for (demo.meshes.items) |mesh| {
            gctx.cmdlist.SetGraphicsRoot32BitConstants(0, 2, &.{ mesh.vertex_offset, mesh.index_offset }, 0);
            gctx.cmdlist.SetGraphicsRootDescriptorTable(1, blk: {
                const color_index = demo.materials.items[mesh.material_index].base_color_tex_index;
                const mr_index = demo.materials.items[mesh.material_index].metallic_roughness_tex_index;
                const normal_index = demo.materials.items[mesh.material_index].normal_tex_index;

                const table = gctx.copyDescriptorsToGpuHeap(1, demo.textures.items[color_index].view);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.textures.items[mr_index].view);
                _ = gctx.copyDescriptorsToGpuHeap(1, demo.textures.items[normal_index].view);
                break :blk table;
            });
            gctx.cmdlist.DrawInstanced(mesh.num_indices, 1, 0, 0);
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
