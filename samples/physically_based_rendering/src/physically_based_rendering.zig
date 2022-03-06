const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w = zwin32.base;
const d3d12 = zwin32.d3d12;
const wasapi = zwin32.wasapi;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;

const Vec2 = vm.Vec2;
const Vec3 = vm.Vec3;
const Vec4 = vm.Vec4;
const Mat4 = vm.Mat4;

pub export const D3D12SDKVersion: u32 = 4;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: physically based rendering";
const window_width = 1920;
const window_height = 1080;

const env_texture_resolution = 512;
const irradiance_texture_resolution = 64;
const prefiltered_env_texture_resolution = 256;
const prefiltered_env_texture_num_mip_levels = 6;
const brdf_integration_texture_resolution = 512;

const mesh_cube = 0;
const mesh_helmet = 1;

const ResourceView = struct {
    resource: zd3d12.ResourceHandle,
    view: d3d12.CPU_DESCRIPTOR_HANDLE,
};

const Vertex = struct {
    position: Vec3,
    normal: Vec3,
    texcoords0: Vec2,
    tangent: Vec4,
};
comptime {
    assert(@sizeOf([2]Vertex) == 2 * 48);
    assert(@alignOf([2]Vertex) == 4);
}

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

fn appendMesh(
    data: *c.cgltf_data,
    mesh_index: u32,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList(Vec3),
    normals: ?*std.ArrayList(Vec3),
    texcoords0: ?*std.ArrayList(Vec2),
    tangents: ?*std.ArrayList(Vec4),
) void {
    assert(mesh_index < data.meshes_count);
    const num_vertices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[0].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[0].indices.*.count);

    // Indices.
    {
        indices.ensureTotalCapacity(indices.items.len + num_indices) catch unreachable;

        const accessor = data.meshes[mesh_index].primitives[0].indices;

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
        const num_attribs: u32 = @intCast(u32, data.meshes[mesh_index].primitives[0].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.*.meshes[mesh_index].primitives[0].attributes[attrib_index];
            const accessor = attrib.*.data;

            assert(accessor.*.buffer_view != null);
            assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
            assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
            assert(accessor.*.buffer_view.*.buffer.*.data != null);

            const data_addr = @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
                accessor.*.offset + accessor.*.buffer_view.*.offset;

            if (attrib.*.type == c.cgltf_attribute_type_position) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);

                positions.resize(positions.items.len + num_vertices) catch unreachable;
                @memcpy(
                    @ptrCast([*]u8, &positions.items[positions.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_normal and normals != null) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);

                normals.?.resize(normals.?.items.len + num_vertices) catch unreachable;
                @memcpy(
                    @ptrCast([*]u8, &normals.?.items[normals.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_texcoord and texcoords0 != null) {
                assert(accessor.*.type == c.cgltf_type_vec2);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);

                texcoords0.?.resize(texcoords0.?.items.len + num_vertices) catch unreachable;
                @memcpy(
                    @ptrCast([*]u8, &texcoords0.?.items[texcoords0.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_tangent and tangents != null) {
                assert(accessor.*.type == c.cgltf_type_vec4);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);

                tangents.?.resize(tangents.?.items.len + num_vertices) catch unreachable;
                @memcpy(
                    @ptrCast([*]u8, &tangents.?.items[tangents.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            }
        }
    }

    if (normals != null) assert(normals.?.items.len == positions.items.len);
    if (texcoords0 != null) assert(texcoords0.?.items.len == positions.items.len);
    if (tangents != null) assert(tangents.?.items.len == positions.items.len);
}

// In this demo program, Mesh is just a range of vertices/indices in a single global vertex/index buffer.
const Mesh = struct {
    index_offset: u32,
    vertex_offset: u32,
    num_indices: u32,
};

const PsoMeshPbr_Const = extern struct {
    object_to_clip: Mat4,
    object_to_world: Mat4,
    camera_position: Vec3,
    draw_mode: i32,
};

const DemoState = struct {
    grfx: zd3d12.GraphicsContext,
    gui: GuiRenderer,
    frame_stats: common.FrameStats,

    mesh_pbr_pso: zd3d12.PipelineHandle,
    sample_env_texture_pso: zd3d12.PipelineHandle,

    depth_texture: ResourceView,

    meshes: std.ArrayList(Mesh),

    vertex_buffer: zd3d12.ResourceHandle,
    index_buffer: zd3d12.ResourceHandle,

    mesh_textures: [4]ResourceView,

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

fn loadAllMeshes(
    arena: std.mem.Allocator,
    all_meshes: *std.ArrayList(Mesh),
    all_vertices: *std.ArrayList(Vertex),
    all_indices: *std.ArrayList(u32),
) void {
    var indices = std.ArrayList(u32).init(arena);
    var positions = std.ArrayList(Vec3).init(arena);
    var normals = std.ArrayList(Vec3).init(arena);
    var texcoords0 = std.ArrayList(Vec2).init(arena);
    var tangents = std.ArrayList(Vec4).init(arena);

    {
        const pre_indices_len = indices.items.len;
        const pre_positions_len = positions.items.len;

        const data = parseAndLoadGltfFile(content_dir ++ "cube.gltf");
        defer c.cgltf_free(data);
        appendMesh(data, 0, &indices, &positions, &normals, &texcoords0, &tangents);

        all_meshes.append(.{
            .index_offset = @intCast(u32, pre_indices_len),
            .vertex_offset = @intCast(u32, pre_positions_len),
            .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
        }) catch unreachable;
    }
    {
        const pre_indices_len = indices.items.len;
        const pre_positions_len = positions.items.len;

        const data = parseAndLoadGltfFile(content_dir ++ "SciFiHelmet/SciFiHelmet.gltf");
        defer c.cgltf_free(data);
        appendMesh(data, 0, &indices, &positions, &normals, &texcoords0, &tangents);

        all_meshes.append(.{
            .index_offset = @intCast(u32, pre_indices_len),
            .vertex_offset = @intCast(u32, pre_positions_len),
            .num_indices = @intCast(u32, indices.items.len - pre_indices_len),
        }) catch unreachable;
    }

    all_indices.ensureTotalCapacity(indices.items.len) catch unreachable;
    for (indices.items) |mesh_index| {
        all_indices.appendAssumeCapacity(mesh_index);
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
}

fn drawToCubeTexture(grfx: *zd3d12.GraphicsContext, dest_texture: zd3d12.ResourceHandle, dest_mip_level: u32) void {
    const desc = grfx.getResourceDesc(dest_texture);
    assert(dest_mip_level < desc.MipLevels);
    const texture_width = @intCast(u32, desc.Width) >> @intCast(u5, dest_mip_level);
    const texture_height = desc.Height >> @intCast(u5, dest_mip_level);
    assert(texture_width == texture_height);

    grfx.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = 0.0,
        .TopLeftY = 0.0,
        .Width = @intToFloat(f32, texture_width),
        .Height = @intToFloat(f32, texture_height),
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});
    grfx.cmdlist.RSSetScissorRects(1, &[_]d3d12.RECT{.{
        .left = 0,
        .top = 0,
        .right = @intCast(c_long, texture_width),
        .bottom = @intCast(c_long, texture_height),
    }});
    grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);

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
        const cube_face_rtv = grfx.allocateTempCpuDescriptors(.RTV, 1);
        grfx.device.CreateRenderTargetView(
            grfx.getResource(dest_texture),
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

        grfx.addTransitionBarrier(dest_texture, d3d12.RESOURCE_STATE_RENDER_TARGET);
        grfx.flushResourceBarriers();
        grfx.cmdlist.OMSetRenderTargets(1, &[_]d3d12.CPU_DESCRIPTOR_HANDLE{cube_face_rtv}, w.TRUE, null);
        grfx.deallocateAllTempCpuDescriptors(.RTV);

        const mem = grfx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = object_to_view[cube_face_idx].mul(view_to_clip).transpose();

        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        // NOTE(mziulek): We assume that the first mesh in vertex/index buffer is a 'cube'.
        grfx.cmdlist.DrawIndexedInstanced(36, 1, 0, 0, 0);
    }

    grfx.addTransitionBarrier(dest_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();
}

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const window = common.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = zd3d12.GraphicsContext.init(window);

    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
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

        break :blk grfx.createGraphicsShaderPipeline(
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

        break :blk grfx.createGraphicsShaderPipeline(
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
        pso_desc.DepthStencilState.DepthEnable = w.FALSE;
        pso_desc.RasterizerState.CullMode = .FRONT;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        const generate_env_texture_pso = grfx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_env_texture.vs.cso",
            content_dir ++ "shaders/generate_env_texture.ps.cso",
        );
        const generate_irradiance_texture_pso = grfx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_irradiance_texture.vs.cso",
            content_dir ++ "shaders/generate_irradiance_texture.ps.cso",
        );
        const generate_prefiltered_env_texture_pso = grfx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/generate_prefiltered_env_texture.vs.cso",
            content_dir ++ "shaders/generate_prefiltered_env_texture.ps.cso",
        );
        const generate_brdf_integration_texture_pso = grfx.createComputeShaderPipeline(
            arena_allocator,
            &d3d12.COMPUTE_PIPELINE_STATE_DESC.initDefault(),
            content_dir ++ "shaders/generate_brdf_integration_texture.cs.cso",
        );
        break :blk .{
            .generate_env_texture_pso = generate_env_texture_pso,
            .generate_irradiance_texture_pso = generate_irradiance_texture_pso,
            .generate_prefiltered_env_texture_pso = generate_prefiltered_env_texture_pso,
            .generate_brdf_integration_texture_pso = generate_brdf_integration_texture_pso,
        };
    };

    var all_meshes = std.ArrayList(Mesh).init(gpa_allocator);
    var all_vertices = std.ArrayList(Vertex).init(arena_allocator);
    var all_indices = std.ArrayList(u32).init(arena_allocator);
    loadAllMeshes(arena_allocator, &all_meshes, &all_vertices, &all_indices);

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

    var mipgen_rgba8 = zd3d12.MipmapGenerator.init(arena_allocator, &grfx, .R8G8B8A8_UNORM, content_dir);
    var mipgen_rgba16f = zd3d12.MipmapGenerator.init(arena_allocator, &grfx, .R16G16B16A16_FLOAT, content_dir);

    grfx.beginFrame();

    var gui = GuiRenderer.init(arena_allocator, &grfx, 1, content_dir);

    const vertex_buffer = blk: {
        var vertex_buffer = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(all_vertices.items.len * @sizeOf(Vertex)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);
        const upload = grfx.allocateUploadBufferRegion(Vertex, @intCast(u32, all_vertices.items.len));
        for (all_vertices.items) |vertex, i| {
            upload.cpu_slice[i] = vertex;
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(vertex_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(vertex_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        break :blk vertex_buffer;
    };

    const index_buffer = blk: {
        var index_buffer = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(all_indices.items.len * @sizeOf(u32)),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);
        const upload = grfx.allocateUploadBufferRegion(u32, @intCast(u32, all_indices.items.len));
        for (all_indices.items) |index, i| {
            upload.cpu_slice[i] = index;
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(index_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
        grfx.addTransitionBarrier(index_buffer, d3d12.RESOURCE_STATE_INDEX_BUFFER);
        break :blk index_buffer;
    };

    //
    // BEGIN: Upload texture data to the GPU.
    //
    const equirect_texture = blk: {
        var width: u32 = 0;
        var height: u32 = 0;
        c.stbi_set_flip_vertically_on_load(1);
        const image_data = c.stbi_loadf(
            content_dir ++ "Newport_Loft.hdr",
            @ptrCast(*i32, &width),
            @ptrCast(*i32, &height),
            null,
            3,
        );
        assert(image_data != null and width > 0 and height > 0);

        const equirect_texture = .{
            .resource = grfx.createCommittedResource(
                .DEFAULT,
                d3d12.HEAP_FLAG_NONE,
                &d3d12.RESOURCE_DESC.initTex2d(.R32G32B32_FLOAT, width, height, 1),
                d3d12.RESOURCE_STATE_COPY_DEST,
                null,
            ) catch |err| hrPanic(err),
            .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        };
        grfx.device.CreateShaderResourceView(grfx.getResource(equirect_texture.resource), null, equirect_texture.view);

        grfx.updateTex2dSubresource(
            equirect_texture.resource,
            0,
            std.mem.sliceAsBytes(image_data[0 .. width * height * 3]),
            width * @sizeOf(f32) * 3,
        );
        c.stbi_image_free(image_data);

        grfx.addTransitionBarrier(equirect_texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();

        break :blk equirect_texture;
    };

    const mesh_textures = [_]ResourceView{
        .{
            .resource = grfx.createAndUploadTex2dFromFile(
                content_dir ++ "SciFiHelmet/SciFiHelmet_AmbientOcclusion.png",
                .{},
            ) catch |err| hrPanic(err),
            .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        },
        .{
            .resource = grfx.createAndUploadTex2dFromFile(
                content_dir ++ "SciFiHelmet/SciFiHelmet_BaseColor.png",
                .{},
            ) catch |err| hrPanic(err),
            .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        },
        .{
            .resource = grfx.createAndUploadTex2dFromFile(
                content_dir ++ "SciFiHelmet/SciFiHelmet_MetallicRoughness.png",
                .{},
            ) catch |err| hrPanic(err),
            .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        },
        .{
            .resource = grfx.createAndUploadTex2dFromFile(
                content_dir ++ "SciFiHelmet/SciFiHelmet_Normal.png",
                .{},
            ) catch |err| hrPanic(err),
            .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        },
    };
    //
    // END: Upload texture data to the GPU.
    //

    for (mesh_textures) |texture| {
        mipgen_rgba8.generateMipmaps(&grfx, texture.resource);
        grfx.device.CreateShaderResourceView(grfx.getResource(texture.resource), null, texture.view);
        grfx.addTransitionBarrier(texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
    grfx.flushResourceBarriers();

    const env_texture = .{
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
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
                .Flags = d3d12.RESOURCE_FLAG_ALLOW_RENDER_TARGET,
            },
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(env_texture.resource),
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
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
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
                .Flags = d3d12.RESOURCE_FLAG_ALLOW_RENDER_TARGET,
            },
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(irradiance_texture.resource),
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
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
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
                .Flags = d3d12.RESOURCE_FLAG_ALLOW_RENDER_TARGET,
            },
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(prefiltered_env_texture.resource),
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
        .resource = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &blk: {
                var desc = d3d12.RESOURCE_DESC.initTex2d(
                    .R16G16_FLOAT,
                    brdf_integration_texture_resolution,
                    brdf_integration_texture_resolution,
                    1, // mip levels
                );
                desc.Flags = d3d12.RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                break :blk desc;
            },
            d3d12.RESOURCE_STATE_UNORDERED_ACCESS,
            null,
        ) catch |err| hrPanic(err),
        .view = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(
        grfx.getResource(brdf_integration_texture.resource),
        null,
        brdf_integration_texture.view,
    );

    grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = grfx.getResource(vertex_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, grfx.getResourceSize(vertex_buffer)),
        .StrideInBytes = @sizeOf(Vertex),
    }});
    grfx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = grfx.getResource(index_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, grfx.getResourceSize(index_buffer)),
        .Format = .R32_UINT,
    });

    //
    // Generate env. (cube) texture content.
    //
    grfx.setCurrentPipeline(temp_pipelines.generate_env_texture_pso);
    grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, equirect_texture.view));
    drawToCubeTexture(&grfx, env_texture.resource, 0);
    mipgen_rgba16f.generateMipmaps(&grfx, env_texture.resource);
    grfx.addTransitionBarrier(env_texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();

    //
    // Generate irradiance (cube) texture content.
    //
    grfx.setCurrentPipeline(temp_pipelines.generate_irradiance_texture_pso);
    grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, env_texture.view));
    drawToCubeTexture(&grfx, irradiance_texture.resource, 0);
    mipgen_rgba16f.generateMipmaps(&grfx, irradiance_texture.resource);
    grfx.addTransitionBarrier(irradiance_texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();

    //
    // Generate prefiltered env. (cube) texture content.
    //
    grfx.setCurrentPipeline(temp_pipelines.generate_prefiltered_env_texture_pso);
    grfx.cmdlist.SetGraphicsRootDescriptorTable(2, grfx.copyDescriptorsToGpuHeap(1, env_texture.view));
    {
        var mip_level: u32 = 0;
        while (mip_level < prefiltered_env_texture_num_mip_levels) : (mip_level += 1) {
            const roughness = @intToFloat(f32, mip_level) /
                @intToFloat(f32, prefiltered_env_texture_num_mip_levels - 1);
            grfx.cmdlist.SetGraphicsRoot32BitConstant(1, @bitCast(u32, roughness), 0);
            drawToCubeTexture(&grfx, prefiltered_env_texture.resource, mip_level);
        }
    }
    grfx.addTransitionBarrier(prefiltered_env_texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();

    //
    // Generate BRDF integration texture.
    //
    {
        const uav = grfx.allocateTempCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateUnorderedAccessView(grfx.getResource(brdf_integration_texture.resource), null, null, uav);

        grfx.setCurrentPipeline(temp_pipelines.generate_brdf_integration_texture_pso);
        grfx.cmdlist.SetComputeRootDescriptorTable(0, grfx.copyDescriptorsToGpuHeap(1, uav));
        const num_groups = @divExact(brdf_integration_texture_resolution, 8);
        grfx.cmdlist.Dispatch(num_groups, num_groups, 1);

        grfx.addTransitionBarrier(brdf_integration_texture.resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();
        grfx.deallocateAllTempCpuDescriptors(.CBV_SRV_UAV);
    }

    grfx.endFrame();
    grfx.finishGpuCommands();

    // Release temporary resources.
    mipgen_rgba8.deinit(&grfx);
    mipgen_rgba16f.deinit(&grfx);
    _ = grfx.releaseResource(equirect_texture.resource);
    grfx.destroyPipeline(temp_pipelines.generate_env_texture_pso);
    grfx.destroyPipeline(temp_pipelines.generate_irradiance_texture_pso);
    grfx.destroyPipeline(temp_pipelines.generate_prefiltered_env_texture_pso);
    grfx.destroyPipeline(temp_pipelines.generate_brdf_integration_texture_pso);

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = common.FrameStats.init(),
        .mesh_pbr_pso = mesh_pbr_pso,
        .sample_env_texture_pso = sample_env_texture_pso,
        .depth_texture = depth_texture,
        .meshes = all_meshes,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .mesh_textures = mesh_textures,
        .env_texture = env_texture,
        .irradiance_texture = irradiance_texture,
        .prefiltered_env_texture = prefiltered_env_texture,
        .brdf_integration_texture = brdf_integration_texture,
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

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    demo.meshes.deinit();
    _ = demo.grfx.releaseResource(demo.depth_texture.resource);
    _ = demo.grfx.releaseResource(demo.env_texture.resource);
    _ = demo.grfx.releaseResource(demo.irradiance_texture.resource);
    _ = demo.grfx.releaseResource(demo.prefiltered_env_texture.resource);
    _ = demo.grfx.releaseResource(demo.brdf_integration_texture.resource);
    _ = demo.grfx.releaseResource(demo.vertex_buffer);
    _ = demo.grfx.releaseResource(demo.index_buffer);
    for (demo.mesh_textures) |texture| {
        _ = demo.grfx.releaseResource(texture.resource);
    }
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    common.deinitWindow(gpa_allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.grfx.window, window_name);

    common.newImGuiFrame(demo.frame_stats.delta_time);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.grfx.viewport_width) - 600.0 - 20, .y = 20.0 },
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
    grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = grfx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, grfx.getResourceSize(demo.vertex_buffer)),
        .StrideInBytes = @sizeOf(Vertex),
    }});
    grfx.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = grfx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = @intCast(u32, grfx.getResourceSize(demo.index_buffer)),
        .Format = .R32_UINT,
    });

    // Draw SciFiHelmet.
    {
        const object_to_world = vm.Mat4.initRotationY(@floatCast(f32, 0.25 * demo.frame_stats.time));
        const object_to_clip = object_to_world.mul(cam_world_to_clip);

        const mem = grfx.allocateUploadMemory(PsoMeshPbr_Const, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
            .object_to_world = object_to_world.transpose(),
            .camera_position = demo.camera.position,
            .draw_mode = demo.draw_mode,
        };

        grfx.setCurrentPipeline(demo.mesh_pbr_pso);
        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, blk: {
            const table = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures[0].view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures[1].view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures[2].view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures[3].view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.irradiance_texture.view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.prefiltered_env_texture.view);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.brdf_integration_texture.view);
            break :blk table;
        });
        grfx.cmdlist.DrawIndexedInstanced(
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

        const mem = grfx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = world_to_view_origin.mul(cam_view_to_clip).transpose();

        grfx.setCurrentPipeline(demo.sample_env_texture_pso);
        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, demo.env_texture.view));
        grfx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[mesh_cube].num_indices,
            1,
            demo.meshes.items[mesh_cube].index_offset,
            @intCast(i32, demo.meshes.items[mesh_cube].vertex_offset),
            0,
        );
    }

    demo.gui.draw(grfx);

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_PRESENT);
    grfx.flushResourceBarriers();

    grfx.endFrame();
}

pub fn main() !void {
    common.init();
    defer common.deinit();

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
