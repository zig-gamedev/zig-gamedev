const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d3d = win32.d3d;
const d3d12 = win32.d3d12;

const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
 
const pix = common.pix;
const vm = common.vectormath;
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

const window_name = "zig-gamedev: bindless";
const window_width = 1920;
const window_height = 1080;

const mesh_helmet = 0;

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

const Scene_Const = extern struct {
    world_to_clip: Mat4,
    camera_position: Vec3,
};

const Draw_Const = extern struct {
    object_to_world: Mat4,
    base_color_index: u32,
    ao_index: u32,
    metallic_roughness_index: u32,
    normal_index: u32,
};

const ResourceView = struct {
    resource: gr.ResourceHandle,
    view: d3d12.CPU_DESCRIPTOR_HANDLE,
};

const Texture = struct {
    resource: gr.ResourceHandle,
    srv_index: u32,
};

const texture_ao: u32 = 0;
const texture_base_color: u32 = 1;
const texture_metallic_roughness: u32 = 2;
const texture_normal: u32 = 3;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    depth_texture: ResourceView,

    bindless_pso: gr.PipelineHandle,
    bindless_descriptor_heap: gr.PersistentDescriptorHeap,
    max_bindless_gpu_descriptors: u32 = 1024,

    meshes: std.ArrayList(Mesh),
    mesh_textures: [4]Texture,

    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,

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

    // fn loadTextures() {}
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

        const data = parseAndLoadGltfFile("content/SciFiHelmet/SciFiHelmet.gltf");
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

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const window = lib.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

    // V-Sync
    grfx.present_flags = 0;
    grfx.present_interval = 1;

    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    // Reserving the first N descriptors on the GPU SRV heaps for bindless resources
    const max_bindless_gpu_descriptors: u32 = 1024;
    var bindless_descriptor_heap = grfx.reserveGpuDescriptorHeaps(max_bindless_gpu_descriptors);

    const bindless_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("NORMAL", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("TEXCOORD", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("TANGENT", 0, .R32G32B32A32_FLOAT, 0, 32, .PER_VERTEX_DATA, 0),
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

        const rs = rs_blk: {
            const draw_const_root_desc: d3d12.ROOT_DESCRIPTOR1 = .{
                .Flags = d3d12.ROOT_DESCRIPTOR_FLAG_NONE,
                .ShaderRegister = 0,
                .RegisterSpace = 0,
            };

            const frame_const_root_desc: d3d12.ROOT_DESCRIPTOR1 = .{
                .Flags = d3d12.ROOT_DESCRIPTOR_FLAG_NONE,
                .ShaderRegister = 1,
                .RegisterSpace = 0,
            };

            // SRV (2 Textures: height and normal maps)
            var ranges = [_]d3d12.DESCRIPTOR_RANGE1{
                .{
                    .RangeType = .SRV,
                    .NumDescriptors = 0xffff_ffff,
                    .Flags = d3d12.DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE,
                    .OffsetInDescriptorsFromTableStart = 0xffff_ffff,
                    .BaseShaderRegister = 0,
                    .RegisterSpace = 0,
                }
            };

            const table: d3d12.ROOT_DESCRIPTOR_TABLE1 = .{
                .NumDescriptorRanges = ranges.len,
                .pDescriptorRanges = &ranges,
            };

            var params = [_]d3d12.ROOT_PARAMETER1{
                .{
                    .ParameterType = .CBV,
                    .u = .{ .Descriptor = draw_const_root_desc },
                    .ShaderVisibility = .ALL,
                },
                .{
                    .ParameterType = .CBV,
                    .u = .{ .Descriptor = frame_const_root_desc },
                    .ShaderVisibility = .ALL,
                },
                .{
                    .ParameterType = .DESCRIPTOR_TABLE,
                    .u = .{ .DescriptorTable = table },
                    .ShaderVisibility = .ALL,
                }
            };

            const sampler_descs = [_]d3d12.STATIC_SAMPLER_DESC{
                .{
                    .Filter = .MIN_MAG_MIP_LINEAR,
                    .AddressU = .CLAMP,
                    .AddressV = .CLAMP,
                    .AddressW = .CLAMP,
                    .MipLODBias = 0.0,
                    .MaxAnisotropy = 0,
                    .ComparisonFunc = .NEVER,
                    .BorderColor = .TRANSPARENT_BLACK,
                    .MinLOD = 0,
                    .MaxLOD = 12,
                    .ShaderRegister = 0,
                    .RegisterSpace = 0,
                    .ShaderVisibility = .ALL,
                }
            };

            const root_signature_desc: d3d12.ROOT_SIGNATURE_DESC1 = .{
                .Flags = d3d12.ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
                .NumParamenters = params.len,
                .pParameters = &params,
                .NumStaticSamplers = sampler_descs.len,
                .pStaticSamplers = &sampler_descs,
            };

            const versioned_root_signature_desc: d3d12.VERSIONED_ROOT_SIGNATURE_DESC = .{
                .Version = .VERSION_1_1,
                .u = .{ .Desc_1_1 = root_signature_desc },
            };

            var root_signature_blob: ?*d3d.IBlob = null;
            var error_blob: ?*d3d.IBlob = null;
            var hr = d3d12.D3D12SerializeVersionedRootSignature(&versioned_root_signature_desc, &root_signature_blob, &error_blob);
            if (hr != w.S_OK) {
                if (error_blob) |blob| {
                    std.debug.panic("Error while serializing versioned root signature: {}\n", .{blob.GetBufferPointer()});
                } else {
                    std.debug.panic("Error while serializing versioned root signature\n", .{});
                }
            }

            // NOTE(gmodarelli): This resource will be released when the associated PSO gets released
            var rs: *d3d12.IRootSignature = undefined;

            if (root_signature_blob) |blob| {
                hrPanicOnFail(grfx.device.CreateRootSignature(
                    0,
                    blob.GetBufferPointer(),
                    blob.GetBufferSize(),
                    &d3d12.IID_IRootSignature,
                    @ptrCast(*?*anyopaque, &rs),
                ));
            }

            if (root_signature_blob) |blob| {
                _ = blob.Release();
            }
            if (error_blob) |blob| {
                _ = blob.Release();
            }

            _ = rs.SetName(L("Bindless Root Signature"));

            break :rs_blk rs;
        };

        break :blk grfx.createGraphicsShaderPipelineRsVsGsPs(
            arena_allocator,
            &pso_desc,
            rs,
            "content/shaders/bindless.vs.cso",
            null,
            "content/shaders/bindless.ps.cso",
        );
    };

    // NOTE(gmodarelli): Maybe expose a setName on all resources from graphics.zig?
    // const pipeline = grfx.pipeline.pool.getPipeline(bindless_pso);
    // _ = pipeline.pso.?.SetName(L("Bindless PSO"));

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

    var mipgen_rgba8 = gr.MipmapGenerator.init(arena_allocator, &grfx, .R8G8B8A8_UNORM);

    grfx.beginFrame();

    var gui = gr.GuiContext.init(arena_allocator, &grfx, 1);

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

    var mesh_textures: [4]Texture = undefined;

    {
        const resource = grfx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_AmbientOcclusion.png",
            .{.num_mip_levels = 1},
        ) catch |err| hrPanic(err);
        _ = grfx.getResource(resource).SetName(L("content/SciFiHelmet/SciFiHelmet_AmbientOcclusion.png"));

        mesh_textures[texture_ao] = blk: {
            const srv_allocation = bindless_descriptor_heap.allocate();

            var i: u32 = 0;
            while (i < bindless_descriptor_heap.num_heaps) : (i += 1) {
                grfx.device.CreateShaderResourceView(grfx.getResource(resource), null, srv_allocation.handles[i]);
            }

            // mipgen_rgba8.generateMipmaps(&grfx, resource);
            // grfx.addTransitionBarrier(resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
            const t = Texture{
                .resource = resource,
                .srv_index = srv_allocation.index,
            };

            break :blk t;
        };
    }

    {
        const resource = grfx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_BaseColor.png",
            .{.num_mip_levels = 1},
        ) catch |err| hrPanic(err);
        _ = grfx.getResource(resource).SetName(L("content/SciFiHelmet/SciFiHelmet_BaseColor.png"));

        mesh_textures[texture_base_color] = blk: {
            const srv_allocation = bindless_descriptor_heap.allocate();

            var i: u32 = 0;
            while (i < bindless_descriptor_heap.num_heaps) : (i += 1) {
                grfx.device.CreateShaderResourceView(grfx.getResource(resource), null, srv_allocation.handles[i]);
            }

            // mipgen_rgba8.generateMipmaps(&grfx, resource);
            // grfx.addTransitionBarrier(resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

            const t = Texture{
                .resource = resource,
                .srv_index = srv_allocation.index,
            };

            break :blk t;
        };
    }

    {
        const resource = grfx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_MetallicRoughness.png",
            .{.num_mip_levels = 1},
        ) catch |err| hrPanic(err);
        _ = grfx.getResource(resource).SetName(L("content/SciFiHelmet/SciFiHelmet_MetallicRoughness.png"));
        mesh_textures[texture_metallic_roughness] = blk: {
            const srv_allocation = bindless_descriptor_heap.allocate();

            var i: u32 = 0;
            while (i < bindless_descriptor_heap.num_heaps) : (i += 1) {
                grfx.device.CreateShaderResourceView(grfx.getResource(resource), null, srv_allocation.handles[i]);
            }

            // mipgen_rgba8.generateMipmaps(&grfx, resource);
            // grfx.addTransitionBarrier(resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

            const t = Texture{
                .resource = resource,
                .srv_index = srv_allocation.index,
            };

            break :blk t;
        };
    }

    {
        const resource = grfx.createAndUploadTex2dFromFile(
            "content/SciFiHelmet/SciFiHelmet_Normal.png",
            .{.num_mip_levels = 1},
        ) catch |err| hrPanic(err);
        _ = grfx.getResource(resource).SetName(L("content/SciFiHelmet/SciFiHelmet_Normal.png"));

        mesh_textures[texture_normal] = blk: {
            const srv_allocation = bindless_descriptor_heap.allocate();

            var i: u32 = 0;
            while (i < bindless_descriptor_heap.num_heaps) : (i += 1) {
                grfx.device.CreateShaderResourceView(grfx.getResource(resource), null, srv_allocation.handles[i]);
            }

            // mipgen_rgba8.generateMipmaps(&grfx, resource);
            // grfx.addTransitionBarrier(resource, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

            const t = Texture{
                .resource = resource,
                .srv_index = srv_allocation.index,
            };

            break :blk t;
        };
    }

    grfx.flushResourceBarriers();
    grfx.endFrame();

    w.kernel32.Sleep(100);
    grfx.finishGpuCommands();

    // Release temporary resources.
    mipgen_rgba8.deinit(&grfx);

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .depth_texture = depth_texture,
        .bindless_pso = bindless_pso,
        .bindless_descriptor_heap = bindless_descriptor_heap,
        .max_bindless_gpu_descriptors = max_bindless_gpu_descriptors,
        .meshes = all_meshes,
        .mesh_textures = mesh_textures,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
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
    _ = demo.grfx.releasePipeline(demo.bindless_pso);
    _ = demo.grfx.releaseResource(demo.vertex_buffer);
    _ = demo.grfx.releaseResource(demo.index_buffer);
    for (demo.mesh_textures) |texture| {
        _ = demo.grfx.releaseResource(texture.resource);
    }
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa_allocator);
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
        &[4]f32{0.8, 0.8, 0.2, 1.0},
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

    pix.beginEventOnCommandList(
        @ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist),
        "Draw meshes",
    );

    grfx.setCurrentPipeline(demo.bindless_pso);
    // Set scene constants
    {
        const mem = grfx.allocateUploadMemory(Scene_Const, 1);
        mem.cpu_slice[0] = .{
            .world_to_clip = cam_world_to_clip.transpose(),
            .camera_position = demo.camera.position,
        };

        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
    }

    // Bind bindless texture descriptor table
    grfx.cmdlist.SetGraphicsRootDescriptorTable(2, demo.bindless_descriptor_heap.gpu_handles[grfx.frame_index]);

    // Draw SciFiHelmet.
    {
        const object_to_world = vm.Mat4.initRotationY(@floatCast(f32, 0.25 * demo.frame_stats.time));

        const mem = grfx.allocateUploadMemory(Draw_Const, 1);
        mem.cpu_slice[0] = .{
            .object_to_world = object_to_world.transpose(),
            .base_color_index = demo.mesh_textures[texture_base_color].srv_index,
            .ao_index = demo.mesh_textures[texture_ao].srv_index,
            .metallic_roughness_index = demo.mesh_textures[texture_metallic_roughness].srv_index,
            .normal_index = demo.mesh_textures[texture_normal].srv_index,
        };

        grfx.cmdlist.SetGraphicsRootConstantBufferView(1, mem.gpu_base);

        grfx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[mesh_helmet].num_indices,
            1,
            demo.meshes.items[mesh_helmet].index_offset,
            @intCast(i32, demo.meshes.items[mesh_helmet].vertex_offset),
            0,
        );
    }

    pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    demo.gui.draw(grfx);

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_PRESENT);
    grfx.flushResourceBarriers();

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