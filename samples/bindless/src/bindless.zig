const std = @import("std");
const assert = std.debug.assert;
const math = std.math;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d = zwin32.d3d;
const d3d12 = zwin32.d3d12;
const wasapi = zwin32.wasapi;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;
const zmesh = @import("zmesh");
const zstbi = @import("zstbi");

const Vec2 = vm.Vec2;
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: bindless";
const window_width = 1920;
const window_height = 1080;

const env_texture_resolution = 512;
const irradiance_texture_resolution = 64;
const prefiltered_env_texture_resolution = 256;
const prefiltered_env_texture_num_mip_levels = 6;
const brdf_integration_texture_resolution = 512;

const mesh_cube = 0;
const mesh_helmet = 1;

const Vertex = struct {
    position: [3]f32,
    normal: [3]f32,
    texcoords0: [2]f32,
    tangent: [4]f32,
};
comptime {
    assert(@sizeOf([2]Vertex) == 2 * 48);
    assert(@alignOf([2]Vertex) == 4);
}

// In this demo program, Mesh is just a range of vertices/indices in a single global vertex/index buffer.
const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    num_indices: u32,
};

const Scene_Const = extern struct {
    world_to_clip: Mat4,
    camera_position: Vec3,
    draw_mode: i32,
};

const Draw_Const = extern struct {
    object_to_world: Mat4,
    base_color_index: u32,
    ao_index: u32,
    metallic_roughness_index: u32,
    normal_index: u32,
};

const ResourceView = struct {
    resource: zd3d12.ResourceHandle,
    view: d3d12.CPU_DESCRIPTOR_HANDLE,
};

const Texture = struct {
    resource: zd3d12.ResourceHandle,
    persistent_descriptor: zd3d12.PersistentDescriptor,
};

const texture_ao: u32 = 0;
const texture_base_color: u32 = 1;
const texture_metallic_roughness: u32 = 2;
const texture_normal: u32 = 3;

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    depth_texture: ResourceView,

    mesh_pbr_pso: zd3d12.PipelineHandle,
    sample_env_texture_pso: zd3d12.PipelineHandle,

    meshes: std.ArrayList(Mesh),
    vertex_buffer: zd3d12.ResourceHandle,

    index_buffer: zd3d12.ResourceHandle,

    mesh_textures: [4]Texture,

    env_texture: ResourceView,
    irradiance_texture: ResourceView,
    prefiltered_env_texture: ResourceView,
    brdf_integration_texture: ResourceView,

    draw_mode: i32,

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

fn loadMesh(
    arena: std.mem.Allocator,
    path: [:0]const u8,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
) !void {
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList([3]f32).init(arena);
    var normals = std.ArrayList([3]f32).init(arena);
    var texcoords0 = std.ArrayList([2]f32).init(arena);
    var tangents = std.ArrayList([4]f32).init(arena);

    const pre_indices_len = all_indices.items.len;
    const pre_positions_len = all_vertices.items.len;

    const pathname = std.fs.path.joinZ(arena, &.{
        std.fs.selfExeDirPathAlloc(arena) catch unreachable,
        path,
    }) catch unreachable;

    const data = try zmesh.io.parseAndLoadFile(pathname);
    defer zmesh.io.freeData(data);
    try zmesh.io.appendMeshPrimitive(data, 0, 0, &indices, &positions, &normals, &texcoords0, &tangents);

    all_meshes.append(.{
        .index_offset = @intCast(u32, pre_indices_len),
        .vertex_offset = @intCast(u32, pre_positions_len),
        .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
    }) catch unreachable;

    try all_indices.ensureTotalCapacity(indices.items.len);
    for (indices.items) |mesh_index| {
        all_indices.appendAssumeCapacity(mesh_index);
    }

    try all_vertices.ensureTotalCapacity(positions.items.len);
    for (positions.items) |_, index| {
        all_vertices.appendAssumeCapacity(.{
            .position = positions.items[index],
            .normal = normals.items[index],
            .texcoords0 = texcoords0.items[index],
            .tangent = tangents.items[index],
        });
    }
}

fn drawToCubeTexture(
    gctx: *zd3d12.GraphicsContext,
    dest_texture: zd3d12.ResourceHandle,
    dest_mip_level: u32,
) void {
    const desc = gctx.getResourceDesc(dest_texture);
    assert(dest_mip_level < desc.MipLevels);
    const texture_width = @intCast(u32, desc.Width) >> @intCast(u5, dest_mip_level);
    const texture_height = desc.Height >> @intCast(u5, dest_mip_level);
    assert(texture_width == texture_height);

    gctx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = 0.0,
        .TopLeftY = 0.0,
        .Width = @intToFloat(f32, texture_width),
        .Height = @intToFloat(f32, texture_height),
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});
    gctx.cmdlist.RSSetScissorRects(1, &[_]d3d12.RECT{.{
        .left = 0,
        .top = 0,
        .right = @intCast(c_long, texture_width),
        .bottom = @intCast(c_long, texture_height),
    }});
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

    const zero = Vec3.initZero();
    const object_to_view = [_]Mat4{
        Mat4.initLookToLh(zero, Vec3.init(1.0, 0.0, 0.0), Vec3.init(0.0, 1.0, 0.0)),
        Mat4.initLookToLh(zero, Vec3.init(-1.0, 0.0, 0.0), Vec3.init(0.0, 1.0, 0.0)),
        Mat4.initLookToLh(zero, Vec3.init(0.0, 1.0, 0.0), Vec3.init(0.0, 0.0, -1.0)),
        Mat4.initLookToLh(zero, Vec3.init(0.0, -1.0, 0.0), Vec3.init(0.0, 0.0, 1.0)),
        Mat4.initLookToLh(zero, Vec3.init(0.0, 0.0, 1.0), Vec3.init(0.0, 1.0, 0.0)),
        Mat4.initLookToLh(zero, Vec3.init(0.0, 0.0, -1.0), Vec3.init(0.0, 1.0, 0.0)),
    };
    const view_to_clip = Mat4.initPerspectiveFovLh(math.pi * 0.5, 1.0, 0.1, 10.0);

    var cube_face_idx: u32 = 0;
    while (cube_face_idx < 6) : (cube_face_idx += 1) {
        const cube_face_rtv = gctx.allocateTempCpuDescriptors(.RTV, 1);
        gctx.device.CreateRenderTargetView(
            gctx.lookupResource(dest_texture).?,
            &d3d12.RENDER_TARGET_VIEW_DESC{
                .Format = .UNKNOWN,
                .ViewDimension = .TEXTURE2DARRAY,
                .u = .{
                    .Texture2DArray = .{
                        .MipSlice = dest_mip_level,
                        .FirstArraySlice = cube_face_idx,
                        .ArraySize = 1,
                        .PlaneSlice = 0,
                    },
                },
            },
            cube_face_rtv,
        );

        gctx.addTransitionBarrier(dest_texture, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();
        gctx.cmdlist.OMSetRenderTargets(1, &[_]d3d12.CPU_DESCRIPTOR_HANDLE{cube_face_rtv}, w32.TRUE, null);
        gctx.deallocateAllTempCpuDescriptors(.RTV);

        const mem = gctx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = object_to_view[cube_face_idx].mul(view_to_clip).transpose();

        gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        // NOTE(mziulek): We assume that the first mesh in vertex/index buffer is a 'cube'.
        gctx.cmdlist.DrawIndexedInstanced(36, 1, 0, 0, 0);
    }

    gctx.addTransitionBarrier(dest_texture, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();
}

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);
    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    // V-Sync
    gctx.present_flags = .{};
    gctx.present_interval = 1;

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    const mesh_pbr_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Tangent", 0, .R32G32B32A32_FLOAT, 0, 32, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/mesh_pbr.vs.cso",
            content_dir ++ "shaders/mesh_pbr.ps.cso",
        );
    };

    const sample_env_texture_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Tangent", 0, .R32G32B32A32_FLOAT, 0, 32, .PER_VERTEX_DATA, 0),
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
        pso_desc.RasterizerState.CullMode = .FRONT;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.DepthStencilState.DepthWriteMask = .ZERO;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/sample_env_texture.vs.cso",
            content_dir ++ "shaders/sample_env_texture.ps.cso",
        );
    };

    const temp_pipelines = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Tangent", 0, .R32G32B32A32_FLOAT, 0, 32, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R16G16B16A16_FLOAT;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.RasterizerState.CullMode = .FRONT;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        const generate_env_texture_pso = gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_env_texture.vs.cso",
            content_dir ++ "shaders/generate_env_texture.ps.cso",
        );
        const generate_irradiance_texture_pso = gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_irradiance_texture.vs.cso",
            content_dir ++ "shaders/generate_irradiance_texture.ps.cso",
        );
        const generate_prefiltered_env_texture_pso = gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_prefiltered_env_texture.vs.cso",
            content_dir ++ "shaders/generate_prefiltered_env_texture.ps.cso",
        );
        var compute_desc = d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault();
        const generate_brdf_integration_texture_pso = gctx.createComputeShaderPipeline(
            arena_allocator,
            &compute_desc,
            content_dir ++ "shaders/generate_brdf_integration_texture.cs.cso",
        );
        break :blk .{
            .generate_env_texture_pso = generate_env_texture_pso,
            .generate_irradiance_texture_pso = generate_irradiance_texture_pso,
            .generate_prefiltered_env_texture_pso = generate_prefiltered_env_texture_pso,
            .generate_brdf_integration_texture_pso = generate_brdf_integration_texture_pso,
        };
    };

    zmesh.init(arena_allocator);
    defer zmesh.deinit();

    var all_meshes = std.ArrayList(Mesh).init(allocator);
    var all_vertices = std.ArrayList(Vertex).init(arena_allocator);
    var all_indices = std.ArrayList(u32).init(arena_allocator);

    try loadMesh(arena_allocator, content_dir ++ "cube.gltf", &all_meshes, &all_vertices, &all_indices);
    try loadMesh(
        arena_allocator,
        content_dir ++ "SciFiHelmet/SciFiHelmet.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
    );

    const depth_texture = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(.D32_FLOAT, gctx.viewport_width, gctx.viewport_height, 1);
                desc.Flags = .{ .ALLOW_DEPTH_STENCIL = true, .DENY_SHADER_RESOURCE = true };
                break :blk desc;
            },
            .{ .DEPTH_WRITE = true },
            &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.DSV, 1),
    };
    gctx.device.CreateDepthStencilView(
        gctx.lookupResource(depth_texture.resource).?,
        null,
        depth_texture.view,
    );

    var mipgen_rgba8 = zd3d12.MipmapGenerator.init(arena_allocator, &gctx, .R8G8B8A8_UNORM, content_dir);
    var mipgen_rgba16f = zd3d12.MipmapGenerator.init(arena_allocator, &gctx, .R16G16B16A16_FLOAT, content_dir);

    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    const vertex_buffer = blk: {
        var vertex_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err);
        const upload = gctx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| {
            upload.cpu_slice[i] = vertex;
        }
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(vertex_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(vertex_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
        break :blk vertex_buffer;
    };

    const index_buffer = blk: {
        var index_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err);
        const upload = gctx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| {
            upload.cpu_slice[i] = index;
        }
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(index_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        gctx.addTransitionBarrier(index_buffer, .{ .INDEX_BUFFER = true });
        break :blk index_buffer;
    };

    //
    // BEGIN: Upload texture data to the GPU.
    //
    const equirect_texture = blk: {
        zstbi.setFlipVerticallyOnLoad(true);

        const pathname = std.fs.path.joinZ(arena_allocator, &.{
            std.fs.selfExeDirPathAlloc(arena_allocator) catch unreachable,
            content_dir ++ "Newport_Loft.hdr",
        }) catch unreachable;

        var image = zstbi.Image.init(pathname, 4) catch unreachable;
        defer {
            image.deinit();
            zstbi.setFlipVerticallyOnLoad(false);
        }

        const equirect_texture = .{
            .resource = gctx.createCommittedResource(
                .DEFAULT,
                .{},
                &d3d12.RESOURCE_DESC.initTex2d(.R16G16B16A16_FLOAT, image.width, image.height, 1),
                .{ .COPY_DEST = true },
                null,
            ) catch |err| hrPanic(err),
            .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        };
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(equirect_texture.resource).?,
            null,
            equirect_texture.view,
        );

        gctx.updateTex2dSubresource(
            equirect_texture.resource,
            0,
            image.data,
            image.width * @sizeOf(f16) * 4,
        );

        gctx.addTransitionBarrier(equirect_texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();

        break :blk equirect_texture;
    };

    var mesh_textures: [4]Texture = undefined;

    {
        const resource = gctx.createAndUploadTex2dFromFile(
            content_dir ++ "SciFiHelmet/SciFiHelmet_AmbientOcclusion.png",
            .{},
        ) catch |err| hrPanic(err);
        _ = gctx.lookupResource(resource).?.SetName(L("SciFiHelmet/SciFiHelmet_AmbientOcclusion.png"));

        mesh_textures[texture_ao] = blk: {
            const srv_allocation = gctx.allocatePersistentGpuDescriptors(1);
            gctx.device.CreateShaderResourceView(
                gctx.lookupResource(resource).?,
                null,
                srv_allocation.cpu_handle,
            );

            mipgen_rgba8.generateMipmaps(&gctx, resource);
            gctx.addTransitionBarrier(resource, .{ .PIXEL_SHADER_RESOURCE = true });

            const t = Texture{
                .resource = resource,
                .persistent_descriptor = srv_allocation,
            };

            break :blk t;
        };
    }

    {
        const resource = gctx.createAndUploadTex2dFromFile(
            content_dir ++ "SciFiHelmet/SciFiHelmet_BaseColor.png",
            .{},
        ) catch |err| hrPanic(err);
        _ = gctx.lookupResource(resource).?.SetName(L("SciFiHelmet/SciFiHelmet_BaseColor.png"));

        mesh_textures[texture_base_color] = blk: {
            const srv_allocation = gctx.allocatePersistentGpuDescriptors(1);
            gctx.device.CreateShaderResourceView(
                gctx.lookupResource(resource).?,
                null,
                srv_allocation.cpu_handle,
            );

            mipgen_rgba8.generateMipmaps(&gctx, resource);
            gctx.addTransitionBarrier(resource, .{ .PIXEL_SHADER_RESOURCE = true });

            const t = Texture{
                .resource = resource,
                .persistent_descriptor = srv_allocation,
            };

            break :blk t;
        };
    }

    {
        const resource = gctx.createAndUploadTex2dFromFile(
            content_dir ++ "SciFiHelmet/SciFiHelmet_MetallicRoughness.png",
            .{},
        ) catch |err| hrPanic(err);
        _ = gctx.lookupResource(resource).?.SetName(L("SciFiHelmet/SciFiHelmet_MetallicRoughness.png"));

        mesh_textures[texture_metallic_roughness] = blk: {
            const srv_allocation = gctx.allocatePersistentGpuDescriptors(1);
            gctx.device.CreateShaderResourceView(
                gctx.lookupResource(resource).?,
                null,
                srv_allocation.cpu_handle,
            );

            mipgen_rgba8.generateMipmaps(&gctx, resource);
            gctx.addTransitionBarrier(resource, .{ .PIXEL_SHADER_RESOURCE = true });

            const t = Texture{
                .resource = resource,
                .persistent_descriptor = srv_allocation,
            };

            break :blk t;
        };
    }

    {
        const resource = gctx.createAndUploadTex2dFromFile(
            content_dir ++ "SciFiHelmet/SciFiHelmet_Normal.png",
            .{},
        ) catch |err| hrPanic(err);
        _ = gctx.lookupResource(resource).?.SetName(L("SciFiHelmet/SciFiHelmet_Normal.png"));

        mesh_textures[texture_normal] = blk: {
            const srv_allocation = gctx.allocatePersistentGpuDescriptors(1);
            gctx.device.CreateShaderResourceView(
                gctx.lookupResource(resource).?,
                null,
                srv_allocation.cpu_handle,
            );

            mipgen_rgba8.generateMipmaps(&gctx, resource);
            gctx.addTransitionBarrier(resource, .{ .PIXEL_SHADER_RESOURCE = true });

            const t = Texture{
                .resource = resource,
                .persistent_descriptor = srv_allocation,
            };

            break :blk t;
        };
    }

    const env_texture = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC{
                .Dimension = .TEXTURE2D,
                .Alignment = 0,
                .Width = env_texture_resolution,
                .Height = env_texture_resolution,
                .DepthOrArraySize = 6,
                .MipLevels = 0,
                .Format = .R16G16B16A16_FLOAT,
                .SampleDesc = .{ .Count = 1, .Quality = 0 },
                .Layout = .UNKNOWN,
                .Flags = .{ .ALLOW_RENDER_TARGET = true },
            },
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(env_texture.resource).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC{
            .Format = .UNKNOWN,
            .ViewDimension = .TEXTURECUBE,
            .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .TextureCube = .{
                    .MipLevels = 0xffff_ffff,
                    .MostDetailedMip = 0,
                    .ResourceMinLODClamp = 0.0,
                },
            },
        },
        env_texture.view,
    );

    const irradiance_texture = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC{
                .Dimension = .TEXTURE2D,
                .Alignment = 0,
                .Width = irradiance_texture_resolution,
                .Height = irradiance_texture_resolution,
                .DepthOrArraySize = 6,
                .MipLevels = 0,
                .Format = .R16G16B16A16_FLOAT,
                .SampleDesc = .{ .Count = 1, .Quality = 0 },
                .Layout = .UNKNOWN,
                .Flags = .{ .ALLOW_RENDER_TARGET = true },
            },
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(irradiance_texture.resource).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC{
            .Format = .UNKNOWN,
            .ViewDimension = .TEXTURECUBE,
            .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .TextureCube = .{
                    .MipLevels = 0xffff_ffff,
                    .MostDetailedMip = 0,
                    .ResourceMinLODClamp = 0.0,
                },
            },
        },
        irradiance_texture.view,
    );

    const prefiltered_env_texture = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC{
                .Dimension = .TEXTURE2D,
                .Alignment = 0,
                .Width = prefiltered_env_texture_resolution,
                .Height = prefiltered_env_texture_resolution,
                .DepthOrArraySize = 6,
                .MipLevels = prefiltered_env_texture_num_mip_levels,
                .Format = .R16G16B16A16_FLOAT,
                .SampleDesc = .{ .Count = 1, .Quality = 0 },
                .Layout = .UNKNOWN,
                .Flags = .{ .ALLOW_RENDER_TARGET = true },
            },
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(prefiltered_env_texture.resource).?,
        &d3d12.SHADER_RESOURCE_VIEW_DESC{
            .Format = .UNKNOWN,
            .ViewDimension = .TEXTURECUBE,
            .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
            .u = .{
                .TextureCube = .{
                    .MipLevels = prefiltered_env_texture_num_mip_levels,
                    .MostDetailedMip = 0,
                    .ResourceMinLODClamp = 0.0,
                },
            },
        },
        prefiltered_env_texture.view,
    );

    const brdf_integration_texture = .{
        .resource = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(
                    .R16G16_FLOAT,
                    brdf_integration_texture_resolution,
                    brdf_integration_texture_resolution,
                    1, // mip levels
                );
                desc.Flags = .{ .ALLOW_UNORDERED_ACCESS = true };
                break :blk desc;
            },
            .{ .UNORDERED_ACCESS = true },
            null,
        ) catch |err| hrPanic(err),
        .view = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    gctx.device.CreateShaderResourceView(
        gctx.lookupResource(brdf_integration_texture.resource).?,
        null,
        brdf_integration_texture.view,
    );

    gctx.flushResourceBarriers();

    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(vertex_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, gctx.getResourceSize(vertex_buffer)),
        .StrideInBytes = @sizeOf(Vertex),
    }});
    gctx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gctx.lookupResource(index_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, gctx.getResourceSize(index_buffer)),
        .Format = .R32_UINT,
    });

    //
    // Generate env. (cube) texture content.
    //
    gctx.setCurrentPipeline(temp_pipelines.generate_env_texture_pso);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(1, gctx.copyDescriptorsToGpuHeap(1, equirect_texture.view));
    drawToCubeTexture(&gctx, env_texture.resource, 0);
    mipgen_rgba16f.generateMipmaps(&gctx, env_texture.resource);
    gctx.addTransitionBarrier(env_texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    //
    // Generate irradiance (cube) texture content.
    //
    gctx.setCurrentPipeline(temp_pipelines.generate_irradiance_texture_pso);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(1, gctx.copyDescriptorsToGpuHeap(1, env_texture.view));
    drawToCubeTexture(&gctx, irradiance_texture.resource, 0);
    mipgen_rgba16f.generateMipmaps(&gctx, irradiance_texture.resource);
    gctx.addTransitionBarrier(irradiance_texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    //
    // Generate prefiltered env. (cube) texture content.
    //
    gctx.setCurrentPipeline(temp_pipelines.generate_prefiltered_env_texture_pso);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(2, gctx.copyDescriptorsToGpuHeap(1, env_texture.view));
    {
        var mip_level: u32 = 0;
        while (mip_level < prefiltered_env_texture_num_mip_levels) : (mip_level += 1) {
            const roughness = @intToFloat(f32, mip_level) /
                @intToFloat(f32, prefiltered_env_texture_num_mip_levels - 1);
            gctx.cmdlist.SetGraphicsRoot32BitConstant(1, @bitCast(u32, roughness), 0);
            drawToCubeTexture(&gctx, prefiltered_env_texture.resource, mip_level);
        }
    }
    gctx.addTransitionBarrier(prefiltered_env_texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
    gctx.flushResourceBarriers();

    //
    // Generate BRDF integration texture.
    //
    {
        const uav = gctx.allocateTempCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateUnorderedAccessView(
            gctx.lookupResource(brdf_integration_texture.resource).?,
            null,
            null,
            uav,
        );

        gctx.setCurrentPipeline(temp_pipelines.generate_brdf_integration_texture_pso);
        gctx.cmdlist.SetComputeRootDescriptorTable(0, gctx.copyDescriptorsToGpuHeap(1, uav));
        const num_groups = @divExact(brdf_integration_texture_resolution, 8);
        gctx.cmdlist.Dispatch(num_groups, num_groups, 1);

        gctx.addTransitionBarrier(brdf_integration_texture.resource, .{ .PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();
        gctx.deallocateAllTempCpuDescriptors(.CBV_SRV_UAV);
    }

    gctx.endFrame();
    gctx.finishGpuCommands();

    // Release temporary resources.
    mipgen_rgba8.deinit(&gctx);
    mipgen_rgba16f.deinit(&gctx);
    gctx.destroyResource(equirect_texture.resource);
    gctx.destroyPipeline(temp_pipelines.generate_env_texture_pso);
    gctx.destroyPipeline(temp_pipelines.generate_irradiance_texture_pso);
    gctx.destroyPipeline(temp_pipelines.generate_prefiltered_env_texture_pso);
    gctx.destroyPipeline(temp_pipelines.generate_brdf_integration_texture_pso);

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
        .depth_texture = depth_texture,
        .mesh_pbr_pso = mesh_pbr_pso,
        .sample_env_texture_pso = sample_env_texture_pso,
        .meshes = all_meshes,
        .mesh_textures = mesh_textures,
        .env_texture = env_texture,
        .irradiance_texture = irradiance_texture,
        .prefiltered_env_texture = prefiltered_env_texture,
        .brdf_integration_texture = brdf_integration_texture,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .draw_mode = 0,
        .camera = .{
            .position = Vec3.init(2.2, 0.0, 2.2),
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
    _ = c.igRadioButton_IntPtr("Draw PBR effect", &demo.draw_mode, 0);
    _ = c.igRadioButton_IntPtr("Draw Ambient Occlusion texture", &demo.draw_mode, 1);
    _ = c.igRadioButton_IntPtr("Draw Base Color texture", &demo.draw_mode, 2);
    _ = c.igRadioButton_IntPtr("Draw Metallic texture", &demo.draw_mode, 3);
    _ = c.igRadioButton_IntPtr("Draw Roughness texture", &demo.draw_mode, 4);
    _ = c.igRadioButton_IntPtr("Draw Normal texture", &demo.draw_mode, 5);
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
        100.0,
    );
    const cam_world_to_clip = cam_world_to_view.mul(cam_view_to_clip);

    const back_buffer = gctx.getBackBuffer();

    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        &demo.depth_texture.view,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 0.0 },
        0,
        null,
    );
    gctx.cmdlist.ClearDepthStencilView(demo.depth_texture.view, .{ .DEPTH = true }, 1.0, 0, 0, null);

    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
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

    gctx.setCurrentPipeline(demo.mesh_pbr_pso);
    // Set scene constants
    {
        const mem = gctx.allocateUploadMemory(Scene_Const, 1);
        mem.cpu_slice[0] = .{
            .world_to_clip = cam_world_to_clip.transpose(),
            .camera_position = demo.camera.position,
            .draw_mode = demo.draw_mode,
        };

        gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
    }

    // Draw SciFiHelmet.
    {
        const object_to_world = vm.Mat4.initRotationY(@floatCast(f32, 0.25 * demo.frame_stats.time));

        const mem = gctx.allocateUploadMemory(Draw_Const, 1);
        mem.cpu_slice[0] = .{
            .object_to_world = object_to_world.transpose(),
            .base_color_index = demo.mesh_textures[texture_base_color].persistent_descriptor.index,
            .ao_index = demo.mesh_textures[texture_ao].persistent_descriptor.index,
            .metallic_roughness_index = demo.mesh_textures[texture_metallic_roughness].persistent_descriptor.index,
            .normal_index = demo.mesh_textures[texture_normal].persistent_descriptor.index,
        };

        gctx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);

        gctx.cmdlist.SetGraphicsRootDescriptorTable(2, blk: {
            const table = gctx.copyDescriptorsToGpuHeap(1, demo.irradiance_texture.view);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.prefiltered_env_texture.view);
            _ = gctx.copyDescriptorsToGpuHeap(1, demo.brdf_integration_texture.view);
            break :blk table;
        });

        gctx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[mesh_helmet].num_indices,
            1,
            demo.meshes.items[mesh_helmet].index_offset,
            @intCast(i32, demo.meshes.items[mesh_helmet].vertex_offset),
            0,
        );
    }
    // Draw env. cube texture.
    {
        var world_to_view_origin = cam_world_to_view;
        world_to_view_origin.r[3] = Vec4.init(0.0, 0.0, 0.0, 1.0);

        const mem = gctx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = world_to_view_origin.mul(cam_view_to_clip).transpose();

        gctx.setCurrentPipeline(demo.sample_env_texture_pso);
        gctx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        gctx.cmdlist.SetGraphicsRootDescriptorTable(1, gctx.copyDescriptorsToGpuHeap(1, demo.env_texture.view));

        gctx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[mesh_cube].num_indices,
            1,
            demo.meshes.items[mesh_cube].index_offset,
            @intCast(i32, demo.meshes.items[mesh_cube].vertex_offset),
            0,
        );
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

    zstbi.init(allocator);
    defer zstbi.deinit();

    var demo = try init(allocator);
    defer deinit(&demo, allocator);

    while (common.handleWindowEvents()) {
        update(&demo);
        draw(&demo);
    }
}
