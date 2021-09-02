const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const wasapi = win32.wasapi;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
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

const window_name = "zig-gamedev: pbr";
const window_width = 1920;
const window_height = 1080;

const env_texture_resolution = 512;

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

fn loadMesh(
    gltf_path: []const u8,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList(Vec3),
    normals: ?*std.ArrayList(Vec3),
    texcoords0: ?*std.ArrayList(Vec2),
    tangents: ?*std.ArrayList(Vec4),
) void {
    const data = blk: {
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
        break :blk data;
    };
    defer c.cgltf_free(data);

    const num_vertices: u32 = @intCast(u32, data.meshes[0].primitives[0].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[0].primitives[0].indices.*.count);

    assert(indices.items.len == 0);
    assert(positions.items.len == 0);
    if (normals != null) assert(normals.?.items.len == 0);
    if (texcoords0 != null) assert(texcoords0.?.items.len == 0);
    if (tangents != null) assert(tangents.?.items.len == 0);

    indices.resize(num_indices) catch unreachable;
    positions.resize(num_vertices) catch unreachable;

    // Indices.
    {
        const accessor = data.meshes[0].primitives[0].indices;

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
                indices.items[i] = src[i];
            }
        } else if (accessor.*.stride == 2) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_16u);
            const src = @ptrCast([*]const u16, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.items[i] = src[i];
            }
        } else if (accessor.*.stride == 4) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_32u);
            const src = @ptrCast([*]const u32, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.items[i] = src[i];
            }
        } else {
            unreachable;
        }
    }
    // Attributes.
    {
        const num_attribs: u32 = @intCast(u32, data.meshes[0].primitives[0].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.*.meshes[0].primitives[0].attributes[attrib_index];
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
                @memcpy(@ptrCast([*]u8, positions.items.ptr), data_addr, accessor.*.count * accessor.*.stride);
            } else if (attrib.*.type == c.cgltf_attribute_type_normal and normals != null) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                normals.?.resize(num_vertices) catch unreachable;
                @memcpy(@ptrCast([*]u8, normals.?.items.ptr), data_addr, accessor.*.count * accessor.*.stride);
            } else if (attrib.*.type == c.cgltf_attribute_type_texcoord and texcoords0 != null) {
                assert(accessor.*.type == c.cgltf_type_vec2);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                texcoords0.?.resize(num_vertices) catch unreachable;
                @memcpy(@ptrCast([*]u8, texcoords0.?.items.ptr), data_addr, accessor.*.count * accessor.*.stride);
            } else if (attrib.*.type == c.cgltf_attribute_type_tangent and tangents != null) {
                assert(accessor.*.type == c.cgltf_type_vec4);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                tangents.?.resize(num_vertices) catch unreachable;
                @memcpy(@ptrCast([*]u8, tangents.?.items.ptr), data_addr, accessor.*.count * accessor.*.stride);
            }
        }
    }
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
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    mesh_pbr_pso: gr.PipelineHandle,
    sample_env_texture_pso: gr.PipelineHandle,

    depth_texture: gr.ResourceHandle,
    depth_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,

    brush: *d2d1.ISolidColorBrush,
    textformat: *dwrite.ITextFormat,

    meshes: std.ArrayList(Mesh),

    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,

    mesh_textures: [4]gr.ResourceHandle,
    mesh_textures_srv: [4]d3d12.CPU_DESCRIPTOR_HANDLE,

    env_texture: gr.ResourceHandle,
    env_texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
};

fn addMesh(
    temp_allocator: *std.mem.Allocator,
    gltf_path: []const u8,
    meshes: *std.ArrayList(Mesh),
    vertices: *std.ArrayList(Vertex),
    indices: *std.ArrayList(u32),
) void {
    var mesh_indices = std.ArrayList(u32).init(temp_allocator);
    var mesh_positions = std.ArrayList(Vec3).init(temp_allocator);
    var mesh_normals = std.ArrayList(Vec3).init(temp_allocator);
    var mesh_texcoords0 = std.ArrayList(Vec2).init(temp_allocator);
    var mesh_tangents = std.ArrayList(Vec4).init(temp_allocator);
    loadMesh(gltf_path, &mesh_indices, &mesh_positions, &mesh_normals, &mesh_texcoords0, &mesh_tangents);

    meshes.append(.{
        .index_offset = @intCast(u32, indices.items.len),
        .vertex_offset = @intCast(u32, vertices.items.len),
        .num_indices = @intCast(u32, mesh_indices.items.len),
    }) catch unreachable;

    indices.ensureTotalCapacity(indices.items.len + mesh_indices.items.len) catch unreachable;
    for (mesh_indices.items) |mesh_index| {
        indices.appendAssumeCapacity(mesh_index);
    }

    vertices.ensureTotalCapacity(vertices.items.len + mesh_positions.items.len) catch unreachable;
    for (mesh_positions.items) |_, index| {
        vertices.appendAssumeCapacity(.{
            .position = mesh_positions.items[index],
            .normal = mesh_normals.items[index],
            .texcoords0 = mesh_texcoords0.items[index],
            .tangent = mesh_tangents.items[index],
        });
    }
}

fn drawToCubeTexture(grfx: *gr.GraphicsContext, dest_texture: gr.ResourceHandle, dest_mip_level: u32) void {
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

        const mem = grfx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = object_to_view[cube_face_idx].mul(view_to_clip).transpose();

        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        grfx.cmdlist.DrawIndexedInstanced(36, 1, 0, 0, 0);
    }

    grfx.addTransitionBarrier(dest_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();
}

fn init(gpa: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

    var arena_allocator = std.heap.ArenaAllocator.init(gpa);
    defer arena_allocator.deinit();

    const brush = blk: {
        var maybe_brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &maybe_brush,
        ));
        break :blk maybe_brush.?;
    };
    const textformat = blk: {
        var maybe_textformat: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
    hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

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
        break :blk grfx.createGraphicsShaderPipeline(
            gpa,
            &pso_desc,
            "content/shaders/mesh_pbr.vs.cso",
            "content/shaders/mesh_pbr.ps.cso",
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
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.RasterizerState.CullMode = .FRONT;
        pso_desc.DepthStencilState.DepthFunc = .LESS_EQUAL;
        pso_desc.DepthStencilState.DepthWriteMask = .ZERO;
        break :blk grfx.createGraphicsShaderPipeline(
            gpa,
            &pso_desc,
            "content/shaders/sample_env_texture.vs.cso",
            "content/shaders/sample_env_texture.ps.cso",
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
        pso_desc.DepthStencilState.DepthEnable = w.FALSE;
        pso_desc.RasterizerState.CullMode = .FRONT;
        const generate_env_texture_pso = grfx.createGraphicsShaderPipeline(
            gpa,
            &pso_desc,
            "content/shaders/generate_env_texture.vs.cso",
            "content/shaders/generate_env_texture.ps.cso",
        );
        break :blk .{
            .generate_env_texture_pso = generate_env_texture_pso,
        };
    };

    var all_meshes = std.ArrayList(Mesh).init(gpa);
    var all_vertices = std.ArrayList(Vertex).init(&arena_allocator.allocator);
    var all_indices = std.ArrayList(u32).init(&arena_allocator.allocator);
    addMesh(
        &arena_allocator.allocator,
        "content/cube.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
    );
    addMesh(
        &arena_allocator.allocator,
        "content/SciFiHelmet/SciFiHelmet.gltf",
        &all_meshes,
        &all_vertices,
        &all_indices,
    );

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

    const depth_texture_srv = grfx.allocateCpuDescriptors(.DSV, 1);
    grfx.device.CreateDepthStencilView(grfx.getResource(depth_texture), null, depth_texture_srv);

    var mipgen_rgba8 = gr.MipmapGenerator.init(gpa, &grfx, .R8G8B8A8_UNORM);
    var mipgen_rgba16f = gr.MipmapGenerator.init(gpa, &grfx, .R16G16B16A16_FLOAT);

    grfx.beginFrame();

    var gui = gr.GuiContext.init(gpa, &grfx);

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

    const mesh_textures = .{
        grfx.createAndUploadTex2dFromFile(
            L("content/SciFiHelmet/SciFiHelmet_AmbientOcclusion.png"),
            0,
        ) catch |err| hrPanic(err),
        grfx.createAndUploadTex2dFromFile(
            L("content/SciFiHelmet/SciFiHelmet_BaseColor.png"),
            0,
        ) catch |err| hrPanic(err),
        grfx.createAndUploadTex2dFromFile(
            L("content/SciFiHelmet/SciFiHelmet_MetallicRoughness.png"),
            0,
        ) catch |err| hrPanic(err),
        grfx.createAndUploadTex2dFromFile(
            L("content/SciFiHelmet/SciFiHelmet_Normal.png"),
            0,
        ) catch |err| hrPanic(err),
    };
    mipgen_rgba8.generateMipmaps(&grfx, mesh_textures[0]);
    mipgen_rgba8.generateMipmaps(&grfx, mesh_textures[1]);
    mipgen_rgba8.generateMipmaps(&grfx, mesh_textures[2]);
    mipgen_rgba8.generateMipmaps(&grfx, mesh_textures[3]);

    const mesh_textures_srv = .{
        grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
        grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1),
    };
    grfx.device.CreateShaderResourceView(grfx.getResource(mesh_textures[0]), null, mesh_textures_srv[0]);
    grfx.device.CreateShaderResourceView(grfx.getResource(mesh_textures[1]), null, mesh_textures_srv[1]);
    grfx.device.CreateShaderResourceView(grfx.getResource(mesh_textures[2]), null, mesh_textures_srv[2]);
    grfx.device.CreateShaderResourceView(grfx.getResource(mesh_textures[3]), null, mesh_textures_srv[3]);

    grfx.addTransitionBarrier(mesh_textures[0], d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(mesh_textures[1], d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(mesh_textures[2], d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.addTransitionBarrier(mesh_textures[3], d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    grfx.flushResourceBarriers();

    const env = blk: {
        var width: u32 = 0;
        var height: u32 = 0;
        c.stbi_set_flip_vertically_on_load(1);
        const image_data = c.stbi_loadf(
            "content/Newport_Loft.hdr",
            @ptrCast(*i32, &width),
            @ptrCast(*i32, &height),
            null,
            3,
        );
        assert(image_data != null and width > 0 and height > 0);

        const equirect_texture = grfx.createCommittedResource(
            .DEFAULT,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initTex2d(.R32G32B32_FLOAT, width, height, 1),
            d3d12.RESOURCE_STATE_COPY_DEST,
            null,
        ) catch |err| hrPanic(err);

        const equirect_texture_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(grfx.getResource(equirect_texture), null, equirect_texture_srv);

        grfx.updateTex2dSubresource(
            equirect_texture,
            0,
            std.mem.sliceAsBytes(image_data[0 .. width * height * 3]),
            width * @sizeOf(f32) * 3,
        );
        c.stbi_image_free(image_data);

        grfx.addTransitionBarrier(equirect_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();

        const env_texture = grfx.createCommittedResource(
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
        ) catch |err| hrPanic(err);

        const env_texture_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(env_texture),
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
            env_texture_srv,
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
        grfx.setCurrentPipeline(temp_pipelines.generate_env_texture_pso);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, equirect_texture_srv));
        drawToCubeTexture(&grfx, env_texture, 0);

        mipgen_rgba16f.generateMipmaps(&grfx, env_texture);
        grfx.addTransitionBarrier(env_texture, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();

        break :blk .{
            .equirect_texture = equirect_texture,
            .env_texture = env_texture,
            .env_texture_srv = env_texture_srv,
        };
    };

    grfx.finishGpuCommands();

    mipgen_rgba8.deinit(&grfx);
    mipgen_rgba16f.deinit(&grfx);
    _ = grfx.releaseResource(env.equirect_texture);
    _ = grfx.releasePipeline(temp_pipelines.generate_env_texture_pso);
    grfx.deallocateAllTempCpuDescriptors(.RTV);

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .mesh_pbr_pso = mesh_pbr_pso,
        .sample_env_texture_pso = sample_env_texture_pso,
        .depth_texture = depth_texture,
        .depth_texture_srv = depth_texture_srv,
        .brush = brush,
        .textformat = textformat,
        .meshes = all_meshes,
        .vertex_buffer = vertex_buffer,
        .index_buffer = index_buffer,
        .mesh_textures = mesh_textures,
        .mesh_textures_srv = mesh_textures_srv,
        .env_texture = env.env_texture,
        .env_texture_srv = env.env_texture_srv,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    demo.meshes.deinit();
    _ = demo.grfx.releasePipeline(demo.mesh_pbr_pso);
    _ = demo.grfx.releasePipeline(demo.sample_env_texture_pso);
    _ = demo.grfx.releaseResource(demo.depth_texture);
    _ = demo.grfx.releaseResource(demo.env_texture);
    _ = demo.grfx.releaseResource(demo.vertex_buffer);
    _ = demo.grfx.releaseResource(demo.index_buffer);
    for (demo.mesh_textures) |texture| {
        _ = demo.grfx.releaseResource(texture);
    }
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(gpa);
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    lib.newImGuiFrame(demo.frame_stats.delta_time);

    c.igShowDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const world_to_view = vm.Mat4.initLookAtLh(
        vm.Vec3.init(2.2, 2.2, -2.2),
        vm.Vec3.init(0.0, 0.0, 0.0),
        vm.Vec3.init(0.0, 1.0, 0.0),
    );
    const view_to_clip = vm.Mat4.initPerspectiveFovLh(
        math.pi / 3.0,
        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
        0.1,
        100.0,
    );
    const world_to_clip = world_to_view.mul(view_to_clip);

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        &demo.depth_texture_srv,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );
    grfx.cmdlist.ClearDepthStencilView(demo.depth_texture_srv, d3d12.CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);
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
        const object_to_world = vm.Mat4.initRotationY(@floatCast(f32, 0.5 * demo.frame_stats.time));
        const object_to_clip = object_to_world.mul(world_to_clip);

        const mem = grfx.allocateUploadMemory(PsoMeshPbr_Const, 1);
        mem.cpu_slice[0] = .{
            .object_to_clip = object_to_clip.transpose(),
            .object_to_world = object_to_world.transpose(),
        };

        const mesh_index = 1;
        grfx.setCurrentPipeline(demo.mesh_pbr_pso);
        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, blk: {
            const table = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures_srv[0]);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures_srv[0]);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures_srv[1]);
            _ = grfx.copyDescriptorsToGpuHeap(1, demo.mesh_textures_srv[2]);
            break :blk table;
        });
        grfx.cmdlist.DrawIndexedInstanced(
            demo.meshes.items[mesh_index].num_indices,
            1,
            demo.meshes.items[mesh_index].index_offset,
            @intCast(i32, demo.meshes.items[mesh_index].vertex_offset),
            0,
        );
    }
    // Draw env. cube texture.
    {
        var world_to_view_origin = world_to_view;
        world_to_view_origin.m[3][0] = 0.0;
        world_to_view_origin.m[3][1] = 0.0;
        world_to_view_origin.m[3][2] = 0.0;
        world_to_view_origin.m[3][3] = 1.0;

        const mem = grfx.allocateUploadMemory(Mat4, 1);
        mem.cpu_slice[0] = world_to_view_origin.mul(view_to_clip).transpose();

        grfx.setCurrentPipeline(demo.sample_env_texture_pso);
        grfx.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, demo.env_texture_srv));
        grfx.cmdlist.DrawIndexedInstanced(36, 1, 0, 0, 0);
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
            demo.textformat,
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
