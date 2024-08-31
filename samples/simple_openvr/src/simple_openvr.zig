const std = @import("std");
const OpenVR = @import("zopenvr");

const zmath = @import("zmath");
const zglfw = @import("zglfw");

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const d3d12 = zwindows.d3d12;
const d3d = zwindows.d3d;
const dxgi = zwindows.dxgi;

const zd3d12 = @import("zd3d12");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

pub const std_options = .{
    .log_level = .debug,
};

const TrackedDeviceByIndex = std.AutoHashMap(OpenVR.TrackedDeviceIndex, TrackedDevice);
const DevicePoseByIndex = std.AutoHashMap(OpenVR.TrackedDeviceIndex, zmath.Mat);
const ConstantBufferByEye = std.EnumArray(OpenVR.Eye, zd3d12.ConstantBufferHandle(ConstantBuffer));

const App = struct {
    openvr: OpenVR,
    system: OpenVR.System,
    compositor: OpenVR.Compositor,
    render_models: OpenVR.RenderModels,
    tracked_device_by_index: TrackedDeviceByIndex,
    mipmapgen_bytecode: d3d12.SHADER_BYTECODE,

    pub fn init(allocator: std.mem.Allocator, mipmapgen_bytecode: d3d12.SHADER_BYTECODE) !App {
        const openvr = try OpenVR.init(.scene);

        return .{
            .openvr = openvr,
            .system = try openvr.system(),
            .compositor = try openvr.compositor(),
            .render_models = try openvr.renderModels(),
            .tracked_device_by_index = TrackedDeviceByIndex.init(allocator),
            .mipmapgen_bytecode = mipmapgen_bytecode,
        };
    }

    fn deinit(self: *App) void {
        self.tracked_device_by_index.deinit();
        self.openvr.deinit();
        self.* = undefined;
    }

    fn allocTrackedDevice(self: *App, allocator: std.mem.Allocator, gctx: *zd3d12.GraphicsContext, index: OpenVR.TrackedDeviceIndex) !void {
        // try to find a device we've already set up
        if (self.tracked_device_by_index.contains(index)) {
            return;
        }

        const render_model_name = try self.system.allocTrackedDevicePropertyString(allocator, index, .render_model_name);
        defer allocator.free(render_model_name);
        if (std.mem.eql(u8, render_model_name, "generic_hmd")) {
            return;
        }

        const render_model = try self.render_models.loadRenderModel(render_model_name);
        defer self.render_models.freeRenderModel(render_model);

        const render_model_texture = try self.render_models.loadTexture(render_model.diffuse_texture_id);
        defer self.render_models.freeTexture(render_model_texture);
        std.debug.assert(render_model_texture.format == .rgba8_srgb);

        const vertices = try gctx.uploadVertices(OpenVR.RenderModel.Vertex, render_model.vertex_data);
        const vertex_indices = try gctx.uploadVertexIndices(u16, render_model.index_data);

        var mipmap_genenerator = zd3d12.MipmapGenerator.init(gctx, .R8G8B8A8_UNORM, self.mipmapgen_bytecode);
        defer mipmap_genenerator.deinit(gctx);

        gctx.beginFrame();
        defer gctx.endFrame();
        defer gctx.finishGpuCommands();

        const number_of_components = 4;
        const texture_map_data_length = @as(usize, render_model_texture.width) * @as(usize, render_model_texture.height) * number_of_components;
        const texture = try gctx.createAndUploadTex2d(render_model_texture.width, render_model_texture.height, number_of_components, render_model_texture.texture_map_data[0..texture_map_data_length]);
        mipmap_genenerator.generateMipmaps(gctx, texture);

        const texture_shader_resource_view = gctx.allocShaderResourceView(texture, null);

        gctx.addTransitionBarrier(texture, .{ .PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();

        const tracked_device = TrackedDevice{
            .index = index,
            .vertices = vertices,
            .vertex_indices = vertex_indices,
            .texture = texture,
            .texture_shader_resource_view = texture_shader_resource_view,
            .constants = ConstantBufferByEye.init(.{
                .left = try gctx.createConstantBuffer(ConstantBuffer),
                .right = try gctx.createConstantBuffer(ConstantBuffer),
            }),
            .vertex_count = render_model.triangle_count * 3,
        };
        try self.tracked_device_by_index.put(index, tracked_device);
    }
};

const SceneVertex = extern struct {
    position: [3]f32,
    tex_coord: [2]f32,
};
fn addCubeToScene(mat: zmath.Mat, vert_data: *std.ArrayList(SceneVertex)) !void {
    const A = zmath.mul(zmath.Vec{ 0, 0, 0, 1 }, mat);
    const B = zmath.mul(zmath.Vec{ 1, 0, 0, 1 }, mat);
    const C = zmath.mul(zmath.Vec{ 1, 1, 0, 1 }, mat);
    const D = zmath.mul(zmath.Vec{ 0, 1, 0, 1 }, mat);
    const E = zmath.mul(zmath.Vec{ 0, 0, 1, 1 }, mat);
    const F = zmath.mul(zmath.Vec{ 1, 0, 1, 1 }, mat);
    const G = zmath.mul(zmath.Vec{ 1, 1, 1, 1 }, mat);
    const H = zmath.mul(zmath.Vec{ 0, 1, 1, 1 }, mat);

    const vertices = [_]SceneVertex{
        // Front
        .{ .position = .{ E[0], E[1], E[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ F[0], F[1], F[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ G[0], G[1], G[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ G[0], G[1], G[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ H[0], H[1], H[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ E[0], E[1], E[2] }, .tex_coord = .{ 0, 1 } },

        // Back
        .{ .position = .{ B[0], B[1], B[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ A[0], A[1], A[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ D[0], D[1], D[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ D[0], D[1], D[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ C[0], C[1], C[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ B[0], B[1], B[2] }, .tex_coord = .{ 0, 1 } },

        // Top
        .{ .position = .{ H[0], H[1], H[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ G[0], G[1], G[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ C[0], C[1], C[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ C[0], C[1], C[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ D[0], D[1], D[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ H[0], H[1], H[2] }, .tex_coord = .{ 0, 1 } },

        // Bottom
        .{ .position = .{ A[0], A[1], A[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ B[0], B[1], B[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ F[0], F[1], F[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ F[0], F[1], F[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ E[0], E[1], E[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ A[0], A[1], A[2] }, .tex_coord = .{ 0, 1 } },

        // Left
        .{ .position = .{ A[0], A[1], A[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ E[0], E[1], E[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ H[0], H[1], H[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ H[0], H[1], H[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ D[0], D[1], D[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ A[0], A[1], A[2] }, .tex_coord = .{ 0, 1 } },

        // Right
        .{ .position = .{ F[0], F[1], F[2] }, .tex_coord = .{ 0, 1 } },
        .{ .position = .{ B[0], B[1], B[2] }, .tex_coord = .{ 1, 1 } },
        .{ .position = .{ C[0], C[1], C[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ C[0], C[1], C[2] }, .tex_coord = .{ 1, 0 } },
        .{ .position = .{ G[0], G[1], G[2] }, .tex_coord = .{ 0, 0 } },
        .{ .position = .{ F[0], F[1], F[2] }, .tex_coord = .{ 0, 1 } },
    };
    // triangles instead of quads
    try vert_data.appendSlice(&vertices);
}

const AxesVertex = extern struct {
    position: [3]f32,
    color: [3]f32,
};

fn matFromMatrix34(mat: OpenVR.Matrix34) zmath.Mat {
    return zmath.matFromArr(.{
        mat.m[0][0], mat.m[1][0], mat.m[2][0], 0,
        mat.m[0][1], mat.m[1][1], mat.m[2][1], 0,
        mat.m[0][2], mat.m[1][2], mat.m[2][2], 0,
        mat.m[0][3], mat.m[1][3], mat.m[2][3], 1,
    });
}
fn matFromMatrix44(mat: OpenVR.Matrix44) zmath.Mat {
    return zmath.matFromArr(.{
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3],
    });
}
const RTVIndex = enum(i32) {
    left_eye = 0,
    right_eye,
    swapchain0,
    swapchain1,

    const NUM_RTVS = 4;
};

// Slots in the ConstantBufferView/ShaderResourceView descriptor heap
const CBVSRVIndex = enum(i32) {
    cbv_left_eye = 0,
    cbv_right_eye,
    srv_left_eye,
    srv_right_eye,
    srv_texture_map,

    // Slot for texture in each possible render model
    srv_texture_render_model0,
    srv_texture_render_model_max = @intFromEnum(CBVSRVIndex.srv_texture_render_model0) + OpenVR.max_tracked_device_count,

    // Slot for transform in each possible rendermodel
    cbv_left_eye_render_model0,
    cbv_left_eye_render_model_max = @intFromEnum(CBVSRVIndex.cbv_left_eye_render_model0) + OpenVR.max_tracked_device_count,

    cbv_right_eye_render_model0,
    cbv_right_eye_render_model_max = @intFromEnum(CBVSRVIndex.cbv_right_eye_render_model0) + OpenVR.max_tracked_device_count,

    NUM_SRV_CBVS,
};

const Float4 = [4]f32;
const Float4x4 = [4]Float4;
const ConstantBuffer = extern struct {
    model_view_projection: Float4x4,
};
const EyeFramebuffer = struct {
    eye: OpenVR.Eye,
    texture: zd3d12.ResourceHandle,
    render_target_view_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    shader_resource_view_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    depth_stencil: zd3d12.ResourceHandle,
    depth_stencil_view_handle: d3d12.CPU_DESCRIPTOR_HANDLE,
    constant: zd3d12.ConstantBufferHandle(ConstantBuffer),
};

fn createEye(gctx: *zd3d12.GraphicsContext, eye: OpenVR.Eye, msaa_sample_count: u32, render_target_size: OpenVR.RenderTargetSize) !EyeFramebuffer {
    var eye_framebuffer: EyeFramebuffer = undefined;
    eye_framebuffer.eye = eye;
    const texture_desc = d3d12.RESOURCE_DESC.initFrameBuffer(
        .R8G8B8A8_UNORM_SRGB,
        render_target_size.width,
        render_target_size.height,
        msaa_sample_count,
    );

    // Create color target
    eye_framebuffer.texture = try gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &texture_desc,
        .{ .PIXEL_SHADER_RESOURCE = true },
        &d3d12.CLEAR_VALUE.initColor(.R8G8B8A8_UNORM_SRGB, &.{ 0, 0, 0, 1 }),
    );

    eye_framebuffer.render_target_view_handle = gctx.allocRenderTargetView(eye_framebuffer.texture, null);
    eye_framebuffer.shader_resource_view_handle = gctx.allocShaderResourceView(eye_framebuffer.texture, null);

    // Create depth
    {
        var depth_desc: d3d12.RESOURCE_DESC = texture_desc;
        depth_desc.Format = .D32_FLOAT;
        depth_desc.Flags = .{ .ALLOW_DEPTH_STENCIL = true };
        eye_framebuffer.depth_stencil = try gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &depth_desc,
            .{ .DEPTH_WRITE = true },
            &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
        );
    }

    eye_framebuffer.depth_stencil_view_handle = gctx.allocDepthStencilView(eye_framebuffer.depth_stencil, null);

    eye_framebuffer.constant = try gctx.createConstantBuffer(ConstantBuffer);

    return eye_framebuffer;
}

const TrackedDevice = struct {
    index: OpenVR.TrackedDeviceIndex,
    show: bool = true,
    vertices: zd3d12.VerticesHandle,
    vertex_indices: zd3d12.VertexIndicesHandle,
    texture: zd3d12.ResourceHandle,
    texture_shader_resource_view: d3d12.CPU_DESCRIPTOR_HANDLE,
    constants: ConstantBufferByEye,
    vertex_count: usize,
};

var show_cubes = true;
fn toggle_show_cubes(_: *zglfw.Window, key: zglfw.Key, _: i32, action: zglfw.Action, _: zglfw.Mods) callconv(.C) void {
    if (key == .c and action == .press) {
        show_cubes = !show_cubes;
    }
}
pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try zglfw.init();
    defer zglfw.terminate();

    const framebuffer_size = [_]i32{ 640, 320 };

    zglfw.windowHintTyped(.position_x, 700);
    zglfw.windowHintTyped(.position_y, 100);
    zglfw.windowHintTyped(.resizable, false);
    zglfw.windowHintTyped(.client_api, .no_api);
    const window = try zglfw.Window.create(framebuffer_size[0], framebuffer_size[1], "", null);
    defer window.destroy();

    const win32_window = zglfw.getWin32Window(window) orelse @panic("failed to get win32 handle to window");
    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator,
        .window = win32_window,
    });
    defer gctx.deinit(allocator);

    const msaa_sample_count = 4;
    const mipmapgen_bytecode_slice = try std.fs.cwd().readFileAlloc(allocator, content_dir ++ "/shaders/generate_mipmaps.cs.cso", 256 * 1024);
    defer allocator.free(mipmapgen_bytecode_slice);
    const mipmapgen_bytecode = d3d12.SHADER_BYTECODE.init(mipmapgen_bytecode_slice);

    var app = try App.init(allocator, mipmapgen_bytecode);
    defer app.deinit();

    {
        const driver = try app.system.allocTrackedDevicePropertyString(allocator, OpenVR.hmd, .tracking_system_name);
        defer allocator.free(driver);
        const display = try app.system.allocTrackedDevicePropertyString(allocator, OpenVR.hmd, .serial_number);
        defer allocator.free(display);

        const title = try std.fmt.allocPrintZ(allocator, "zig-gamedev: simple openvr - {s} {s}", .{ driver, display });
        defer allocator.free(title);
        window.setTitle(title);
    }

    // create all shaders
    const root_signature = root_signature: {
        try gctx.checkFeatureSupport(.ROOT_SIGNATURE, d3d12.FEATURE_DATA_ROOT_SIGNATURE{
            .HighestVersion = .VERSION_1_1,
        });

        const root_signature_desc = d3d12.VERSIONED_ROOT_SIGNATURE_DESC.initVersion1_1(
            d3d12.ROOT_SIGNATURE_DESC1.init(
                &.{
                    .{
                        .ParameterType = .CBV,
                        .u = .{
                            .Descriptor = .{
                                .ShaderRegister = 0,
                            },
                        },
                        .ShaderVisibility = .VERTEX,
                    },
                    .{
                        .ParameterType = .DESCRIPTOR_TABLE,
                        .u = .{
                            .DescriptorTable = d3d12.ROOT_DESCRIPTOR_TABLE1{
                                .NumDescriptorRanges = 1,
                                .pDescriptorRanges = &.{
                                    .{
                                        .RangeType = .SRV,
                                        .NumDescriptors = 1,
                                        .BaseShaderRegister = 0,
                                        .RegisterSpace = 0,
                                        .Flags = .{},
                                        .OffsetInDescriptorsFromTableStart = d3d12.DESCRIPTOR_RANGE_OFFSET_APPEND,
                                    },
                                },
                            },
                        },
                        .ShaderVisibility = .PIXEL,
                    },
                },
                &.{
                    .{
                        .Filter = .MIN_MAG_MIP_POINT,
                        .AddressU = .CLAMP,
                        .AddressV = .CLAMP,
                        .AddressW = .CLAMP,
                        .MipLODBias = 0.0,
                        .MaxAnisotropy = 0,
                        .ComparisonFunc = .NEVER,
                        .BorderColor = .TRANSPARENT_BLACK,
                        .MinLOD = 0.0,
                        .MaxLOD = 340282350000000000000000000000000000000.0,
                        .ShaderRegister = 0,
                        .RegisterSpace = 0,
                        .ShaderVisibility = .ALL,
                    },
                },
                .{
                    .ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = true,
                    .DENY_HULL_SHADER_ROOT_ACCESS = true,
                    .DENY_DOMAIN_SHADER_ROOT_ACCESS = true,
                    .DENY_GEOMETRY_SHADER_ROOT_ACCESS = true,
                },
            ),
        );
        const signature: *d3d.IBlob = try zd3d12.serializeVersionedRootSignature(&root_signature_desc);

        break :root_signature try gctx.createRootSignature(0, signature);
    };

    const scene = scene: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        // Describe and create the graphics pipeline state object (PSO).
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        // Define the vertex input layout.
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            .{
                .SemanticName = "POSITION",
                .SemanticIndex = 0,
                .Format = .R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 0,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
            .{
                .SemanticName = "TEXCOORD",
                .SemanticIndex = 0,
                .Format = .R32G32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 12,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
        });
        pso_desc.pRootSignature = root_signature;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/scene.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/scene.ps.cso", 256 * 1024));
        pso_desc.RasterizerState = rasterizer_state: {
            var rasterizer_state = d3d12.RASTERIZER_DESC.initDefault();
            rasterizer_state.FrontCounterClockwise = windows.TRUE;
            rasterizer_state.MultisampleEnable = windows.TRUE;
            break :rasterizer_state rasterizer_state;
        };
        pso_desc.SampleMask = windows.UINT_MAX;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats = .{
            .R8G8B8A8_UNORM_SRGB,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
        };
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.SampleDesc = .{
            .Count = msaa_sample_count,
            .Quality = 0,
        };

        const pipeline = gctx.createGraphicsShaderPipeline(&pso_desc);

        // setup scene
        const vertices, const vertex_count = vertices: {
            const scale = 0.3;
            const scale_spacing = 4.0;

            const scene_volume_init = 20;

            const scene_volume_width = scene_volume_init;
            const scene_volume_height = scene_volume_init;
            const scene_volume_depth = scene_volume_init;

            var vert_data_array = std.ArrayList(SceneVertex).init(arena_allocator);
            const mat_scale = zmath.scaling(scale, scale, scale);
            const mat_transform = zmath.translation(
                -(scene_volume_width * scale_spacing) / 2,
                -(scene_volume_height * scale_spacing) / 2,
                -(scene_volume_depth * scale_spacing) / 2,
            );
            var mat = zmath.mul(mat_transform, mat_scale);
            for (0..scene_volume_depth) |_| {
                for (0..scene_volume_height) |_| {
                    for (0..scene_volume_width) |_| {
                        try addCubeToScene(mat, &vert_data_array);
                        mat = zmath.mul(zmath.translation(scale_spacing, 0, 0), mat);
                    }
                    mat = zmath.mul(zmath.translation(-scene_volume_width * scale_spacing, scale_spacing, 0), mat);
                }
                mat = zmath.mul(zmath.translation(0, -scene_volume_height * scale_spacing, scale_spacing), mat);
            }

            break :vertices .{
                try gctx.uploadVertices(SceneVertex, vert_data_array.items),
                vert_data_array.items.len,
            };
        };

        // setup texture maps
        const texture = texture: {
            var mipmap_genenerator = zd3d12.MipmapGenerator.init(&gctx, .R8G8B8A8_UNORM, mipmapgen_bytecode);
            defer mipmap_genenerator.deinit(&gctx);

            gctx.beginFrame();
            defer gctx.endFrame();
            defer gctx.finishGpuCommands();

            const texture = try gctx.createAndUploadTex2dFromFile(content_dir ++ "cube_texture.png", .{});
            mipmap_genenerator.generateMipmaps(&gctx, texture);

            const shader_resource_view_handle = gctx.allocShaderResourceView(texture, null);

            gctx.addTransitionBarrier(texture, .{ .PIXEL_SHADER_RESOURCE = true });
            gctx.flushResourceBarriers();

            break :texture .{
                .resource = texture,
                .shader_resource_view_handle = shader_resource_view_handle,
            };
        };

        break :scene .{
            .pipeline = pipeline,
            .vertices = vertices,
            .vertex_count = vertex_count,
            .texture = texture,
        };
    };

    const companion = companion: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        // Describe and create the graphics pipeline state object (PSO).
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        // Define the vertex input layout.
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            .{
                .SemanticName = "POSITION",
                .SemanticIndex = 0,
                .Format = .R32G32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 0,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
            .{
                .SemanticName = "TEXCOORD",
                .SemanticIndex = 0,
                .Format = .R32G32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 8,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
        });
        _ = root_signature.AddRef();
        pso_desc.pRootSignature = root_signature;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/companion.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/companion.ps.cso", 256 * 1024));
        pso_desc.RasterizerState = rasterizer_state: {
            var rasterizer_state = d3d12.RASTERIZER_DESC.initDefault();
            rasterizer_state.FrontCounterClockwise = windows.TRUE;
            break :rasterizer_state rasterizer_state;
        };
        pso_desc.DepthStencilState = depth_stencil_state: {
            var depth_stencil_state = d3d12.DEPTH_STENCIL_DESC.initDefault();
            depth_stencil_state.DepthEnable = windows.FALSE;
            depth_stencil_state.StencilEnable = windows.FALSE;
            break :depth_stencil_state depth_stencil_state;
        };
        pso_desc.SampleMask = windows.UINT_MAX;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats = .{
            .R8G8B8A8_UNORM,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
        };
        pso_desc.DSVFormat = .UNKNOWN;
        pso_desc.SampleDesc = .{
            .Count = 1,
            .Quality = 0,
        };

        const pipeline = gctx.createGraphicsShaderPipeline(&pso_desc);

        const Vertex = extern struct {
            position: [2]f32,
            tex_coord: [2]f32,
        };
        const vertices = try gctx.uploadVertices(Vertex, &.{
            // left eye verts
            .{ .position = [2]f32{ -1, -1 }, .tex_coord = [2]f32{ 0, 1 } },
            .{ .position = [2]f32{ 0, -1 }, .tex_coord = [2]f32{ 1, 1 } },
            .{ .position = [2]f32{ -1, 1 }, .tex_coord = [2]f32{ 0, 0 } },
            .{ .position = [2]f32{ 0, 1 }, .tex_coord = [2]f32{ 1, 0 } },

            // right eye verts
            .{ .position = [2]f32{ 0, -1 }, .tex_coord = [2]f32{ 0, 1 } },
            .{ .position = [2]f32{ 1, -1 }, .tex_coord = [2]f32{ 1, 1 } },
            .{ .position = [2]f32{ 0, 1 }, .tex_coord = [2]f32{ 0, 0 } },
            .{ .position = [2]f32{ 1, 1 }, .tex_coord = [2]f32{ 1, 0 } },
        });

        const vertex_indices, const vertex_index_count = vertex_indices: {
            const indices: []const u16 = &.{
                0, 1, 3,
                0, 3, 2,
                4, 5, 7,
                4, 7, 6,
            };
            break :vertex_indices .{
                try gctx.uploadVertexIndices(u16, indices),
                indices.len,
            };
        };

        break :companion .{
            .pipeline = pipeline,
            .vertices = vertices,
            .vertex_indices = vertex_indices,
            .vertex_index_count = vertex_index_count,
        };
    };

    var axes = axes: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        // Describe and create the graphics pipeline state object (PSO).
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        // Define the vertex input layout.
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            .{
                .SemanticName = "POSITION",
                .SemanticIndex = 0,
                .Format = .R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 0,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
            .{
                .SemanticName = "COLOR",
                .SemanticIndex = 0,
                .Format = .R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 12,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
        });
        _ = root_signature.AddRef();
        pso_desc.pRootSignature = root_signature;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/axes.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/axes.ps.cso", 256 * 1024));
        pso_desc.RasterizerState = rasterizer_state: {
            var rasterizer_state = d3d12.RASTERIZER_DESC.initDefault();
            rasterizer_state.FrontCounterClockwise = windows.TRUE;
            rasterizer_state.MultisampleEnable = windows.TRUE;
            break :rasterizer_state rasterizer_state;
        };
        pso_desc.SampleMask = windows.UINT_MAX;
        pso_desc.PrimitiveTopologyType = .LINE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats = .{
            .R8G8B8A8_UNORM_SRGB,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
        };
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.SampleDesc = .{
            .Count = msaa_sample_count,
            .Quality = 0,
        };

        const pipeline = gctx.createGraphicsShaderPipeline(&pso_desc);

        const vertices_per_device = 8;
        const vertices_array = std.mem.zeroes([OpenVR.max_tracked_device_count * vertices_per_device]AxesVertex);
        const vertices = try gctx.uploadVertices(AxesVertex, &vertices_array);

        // workaround https://github.com/ziglang/zig/issues/17101
        const AxesComponent = struct {
            pipeline: zd3d12.PipelineHandle,
            vertices: zd3d12.VerticesHandle,
            vertices_array: [OpenVR.max_tracked_device_count * vertices_per_device]AxesVertex,
            vertex_count: usize,
        };
        break :axes AxesComponent{
            .pipeline = pipeline,
            .vertices = vertices,
            .vertices_array = vertices_array,
            .vertex_count = 0,
        };
    };

    const render_model = render_model: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        // Describe and create the graphics pipeline state object (PSO).
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();

        // Define the vertex input layout.
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            .{
                .SemanticName = "POSITION",
                .SemanticIndex = 0,
                .Format = .R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 0,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
            .{
                .SemanticName = "TEXCOORD",
                .SemanticIndex = 0,
                .Format = .R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 12,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
            .{
                .SemanticName = "TEXCOORD",
                .SemanticIndex = 1,
                .Format = .R32G32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 24,
                .InputSlotClass = .PER_VERTEX_DATA,
                .InstanceDataStepRate = 0,
            },
        });
        _ = root_signature.AddRef();
        pso_desc.pRootSignature = root_signature;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/render_model.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "/shaders/render_model.ps.cso", 256 * 1024));
        pso_desc.RasterizerState = rasterizer_state: {
            var rasterizer_state = d3d12.RASTERIZER_DESC.initDefault();
            rasterizer_state.FrontCounterClockwise = windows.TRUE;
            rasterizer_state.MultisampleEnable = windows.TRUE;
            break :rasterizer_state rasterizer_state;
        };
        pso_desc.SampleMask = windows.UINT_MAX;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats = .{
            .R8G8B8A8_UNORM_SRGB,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
            .UNKNOWN,
        };
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.SampleDesc = .{
            .Count = msaa_sample_count,
            .Quality = 0,
        };

        const pipeline = gctx.createGraphicsShaderPipeline(&pso_desc);

        break :render_model .{
            .pipeline = pipeline,
        };
    };

    // setup cameras

    const eye_matrix = eye_matrix: {
        const near_clip = 0.1;
        const far_clip = 30.0;
        break :eye_matrix std.EnumArray(OpenVR.Eye, zmath.Mat).init(.{
            .left = zmath.mul(
                zmath.inverse(matFromMatrix34(app.system.getEyeToHeadTransform(.left))),
                matFromMatrix44(app.system.getProjectionMatrix(.left, near_clip, far_clip)),
            ),
            .right = zmath.mul(
                zmath.inverse(matFromMatrix34(app.system.getEyeToHeadTransform(.right))),
                matFromMatrix44(app.system.getProjectionMatrix(.right, near_clip, far_clip)),
            ),
        });
    };

    var hmd_pose = zmath.identity();
    var device_pose_by_index = DevicePoseByIndex.init(allocator);
    defer device_pose_by_index.deinit();

    // setup stereo render targets
    const render_target_size = render_target_size: {
        const super_sample_scale = 1.0;
        var render_target_size = app.system.getRecommendedRenderTargetSize();
        render_target_size.width = @intFromFloat(super_sample_scale * @as(f32, @floatFromInt(render_target_size.width)));
        render_target_size.height = @intFromFloat(super_sample_scale * @as(f32, @floatFromInt(render_target_size.height)));
        break :render_target_size render_target_size;
    };

    const eye_descs = [_]EyeFramebuffer{
        try createEye(&gctx, .left, msaa_sample_count, render_target_size),
        try createEye(&gctx, .right, msaa_sample_count, render_target_size),
    };

    for (OpenVR.hmd + 1..OpenVR.max_tracked_device_count) |tracked_device_index| {
        if (!app.system.isTrackedDeviceConnected(@intCast(tracked_device_index))) {
            continue;
        }
        try app.allocTrackedDevice(allocator, &gctx, @intCast(tracked_device_index));
    }

    var frame_timer = try std.time.Timer.start();
    _ = window.setKeyCallback(toggle_show_cubes);

    main: while (!window.shouldClose() and window.getKey(.escape) != .press) {
        {
            // spin loop for frame limiter
            const frame_rate_target: u64 = 100;
            const target_ns = @divTrunc(std.time.ns_per_s, frame_rate_target);
            while (frame_timer.read() < target_ns) {
                std.atomic.spinLoopHint();
            }
            frame_timer.reset();
        }

        // poll for input immediately after vsync or frame limiter to reduce input latency
        zglfw.pollEvents();

        while (app.system.pollNextEvent()) |event| switch (event.event_type) {
            .tracked_device_activated => {
                std.log.debug("Device {} attached. Setting up render model.", .{event.tracked_device_index});
                try app.allocTrackedDevice(allocator, &gctx, event.tracked_device_index);
                // frame consumed by mipmap generator
                continue :main;
            },
            .tracked_device_deactivated => {
                std.log.debug("Device {} detached.", .{event.tracked_device_index});
            },
            .tracked_device_updated => {
                std.log.debug("Device {} updated.", .{event.tracked_device_index});
            },
            else => {},
        };

        // Process SteamVR controller state
        {
            var it = app.tracked_device_by_index.valueIterator();
            while (it.next()) |tracked_device| {
                if (app.system.getControllerState(tracked_device.index)) |state| {
                    tracked_device.show = state.button_pressed == 0;
                }
            }
        }

        //  update poses
        {
            const poses = try app.compositor.allocWaitPoses(allocator, OpenVR.max_tracked_device_count, 0);
            defer poses.deinit(allocator);

            for (poses.render_poses, 0..) |render_pose, device| {
                if (render_pose.pose_is_valid) {
                    const device_to_absolute_tracking = matFromMatrix34(render_pose.device_to_absolute_tracking);
                    try device_pose_by_index.put(@intCast(device), device_to_absolute_tracking);
                    if (device == OpenVR.hmd) {
                        hmd_pose = zmath.inverse(device_to_absolute_tracking);
                    }
                } else {
                    _ = device_pose_by_index.remove(@intCast(device));
                }
            }
        }

        // update controller axes
        if (app.system.isInputAvailable()) {
            axes.vertex_count = 0;

            var it = app.tracked_device_by_index.valueIterator();
            while (it.next()) |tracked_device| {
                if (app.system.getTrackedDeviceClass(tracked_device.index) != .controller) {
                    continue;
                }
                if (device_pose_by_index.get(tracked_device.index)) |device_to_tracking| {
                    const center = zmath.mul(zmath.Vec{ 0, 0, 0, 1 }, device_to_tracking);
                    for (0..3) |i| {
                        const color = color: {
                            var color = [_]f32{ 0, 0, 0 };
                            color[i] += 1;
                            break :color color;
                        };
                        const point = point: {
                            var point = [_]f32{ 0, 0, 0, 1 };
                            point[i] += 0.05;
                            break :point zmath.mul(@as(zmath.Vec, point), device_to_tracking);
                        };

                        axes.vertices_array[axes.vertex_count] = .{
                            .position = @as([4]f32, center)[0..3].*,
                            .color = color,
                        };
                        axes.vertex_count += 1;

                        axes.vertices_array[axes.vertex_count] = .{
                            .position = @as([4]f32, point)[0..3].*,
                            .color = color,
                        };
                        axes.vertex_count += 1;
                    }

                    {
                        const start = zmath.mul(zmath.Vec{ 0, 0, -0.02, 1 }, device_to_tracking);
                        const end = zmath.mul(zmath.Vec{ 0, 0, -39, 1 }, device_to_tracking);
                        const color = [_]f32{ 0.92, 0.92, 0.71 };

                        axes.vertices_array[axes.vertex_count] = .{
                            .position = @as([4]f32, start)[0..3].*,
                            .color = color,
                        };
                        axes.vertex_count += 1;

                        axes.vertices_array[axes.vertex_count] = .{
                            .position = @as([4]f32, end)[0..3].*,
                            .color = color,
                        };
                        axes.vertex_count += 1;
                    }
                }
            }

            try gctx.writeResource(
                AxesVertex,
                axes.vertices.resource,
                axes.vertices_array[0..axes.vertex_count],
            );
        }

        {
            gctx.beginFrame();
            defer gctx.endFrame();

            app.compositor.setExplicitTimingMode(.explicit_application_performs_post_present_handoff);

            // render stereo targets
            gctx.rsSetViewports(&.{
                .{
                    .TopLeftX = 0.0,
                    .TopLeftY = 0.0,
                    .Width = @floatFromInt(render_target_size.width),
                    .Height = @floatFromInt(render_target_size.height),
                    .MinDepth = 0.0,
                    .MaxDepth = 1.0,
                },
            });
            gctx.rsSetScissorRects(&.{
                .{
                    .left = 0,
                    .top = 0,
                    .right = @intCast(render_target_size.width),
                    .bottom = @intCast(render_target_size.height),
                },
            });
            for (eye_descs) |eye_desc| {
                gctx.addTransitionBarrier(eye_desc.texture, .{ .RENDER_TARGET = true });
                gctx.flushResourceBarriers();

                gctx.omSetRenderTargets(
                    &.{eye_desc.render_target_view_handle},
                    false,
                    &eye_desc.depth_stencil_view_handle,
                );
                gctx.clearRenderTargetView(eye_desc.render_target_view_handle, &.{ 0.0, 0.0, 0.0, 1.0 }, &.{});
                gctx.clearDepthStencilView(eye_desc.depth_stencil_view_handle, .{ .DEPTH = true }, 1.0, 0, &.{});

                const current_view_projection = zmath.mul(hmd_pose, eye_matrix.get(eye_desc.eye));
                eye_desc.constant.ptr.model_view_projection = current_view_projection;

                if (show_cubes) {
                    // draw scene
                    gctx.setCurrentPipeline(scene.pipeline);

                    gctx.setGraphicsRootConstantBufferView(0, eye_desc.constant.resource);

                    gctx.setGraphicsRootDescriptorTable(1, &.{
                        scene.texture.shader_resource_view_handle,
                    });

                    gctx.iaSetPrimitiveTopology(.TRIANGLELIST);
                    gctx.iaSetVertexBuffers(0, &.{scene.vertices.view});
                    gctx.drawInstanced(@intCast(scene.vertex_count), 1, 0, 0);
                }

                // draw the controller axis lines
                {
                    gctx.setCurrentPipeline(axes.pipeline);

                    gctx.setGraphicsRootConstantBufferView(0, eye_desc.constant.resource);

                    gctx.iaSetPrimitiveTopology(.LINELIST);
                    gctx.iaSetVertexBuffers(0, &.{axes.vertices.view});
                    gctx.drawInstanced(@intCast(axes.vertex_count), 1, 0, 0);
                }

                // render model rendering
                {
                    gctx.setCurrentPipeline(render_model.pipeline);
                    var it = app.tracked_device_by_index.valueIterator();
                    while (it.next()) |tracked_device| {
                        if (!tracked_device.show) {
                            continue;
                        }
                        if (device_pose_by_index.get(tracked_device.index)) |device_to_tracking| {
                            {
                                const model_view_projection = zmath.mul(device_to_tracking, current_view_projection);
                                const constant = tracked_device.constants.get(eye_desc.eye);
                                // Update the CB with the transform
                                constant.ptr.model_view_projection = model_view_projection;

                                // Bind the CB
                                gctx.setGraphicsRootConstantBufferView(0, constant.resource);
                            }

                            // Bind the texture
                            gctx.setGraphicsRootDescriptorTable(1, &.{
                                tracked_device.texture_shader_resource_view,
                            });

                            gctx.iaSetPrimitiveTopology(.TRIANGLELIST);
                            gctx.iaSetVertexBuffers(0, &.{tracked_device.vertices.view});
                            gctx.iaSetIndexBuffer(&tracked_device.vertex_indices.view);
                            gctx.drawIndexedInstanced(@intCast(tracked_device.vertex_count), 1, 0, 0, 0);
                        }
                    }
                }

                gctx.addTransitionBarrier(eye_desc.texture, .{ .PIXEL_SHADER_RESOURCE = true });
                gctx.flushResourceBarriers();
            }

            // render companion window
            gctx.setCurrentPipeline(companion.pipeline);

            const back_buffer = gctx.getBackBuffer();
            gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            gctx.flushResourceBarriers();

            gctx.omSetRenderTargets(
                &.{back_buffer.descriptor_handle},
                true,
                null,
            );

            gctx.rsSetViewports(&.{
                .{
                    .TopLeftX = 0.0,
                    .TopLeftY = 0.0,
                    .Width = @floatFromInt(framebuffer_size[0]),
                    .Height = @floatFromInt(framebuffer_size[1]),
                    .MinDepth = 0.0,
                    .MaxDepth = 1.0,
                },
            });
            gctx.rsSetScissorRects(&.{
                .{
                    .left = 0,
                    .top = 0,
                    .right = framebuffer_size[0],
                    .bottom = framebuffer_size[1],
                },
            });

            gctx.iaSetPrimitiveTopology(.TRIANGLELIST);
            gctx.iaSetVertexBuffers(0, &.{
                companion.vertices.view,
            });
            gctx.iaSetIndexBuffer(&companion.vertex_indices.view);

            gctx.setGraphicsRootDescriptorTable(1, &.{
                eye_descs[0].shader_resource_view_handle,
            });
            gctx.drawIndexedInstanced(companion.vertex_index_count / 2, 1, 0, 0, 0);

            gctx.setGraphicsRootDescriptorTable(1, &.{
                eye_descs[1].shader_resource_view_handle,
            });
            gctx.drawIndexedInstanced(companion.vertex_index_count / 2, 1, companion.vertex_index_count / 2, 0, 0);

            for (eye_descs) |eye_desc| {
                const dx12_texture = OpenVR.D3D12TextureData{
                    .resource = gctx.lookupResource(eye_desc.texture).?,
                    .command_queue = gctx.cmdqueue,
                    .node_mask = 0,
                };
                app.compositor.submit(eye_desc.eye, &.{
                    .handle = @ptrCast(&dx12_texture),
                    .texture_type = .directx12,
                    .color_space = .gamma,
                }, .{
                    .u_min = 0,
                    .u_max = 1,
                    .v_min = 0,
                    .v_max = 1,
                }, .{}) catch |err| switch (err) {
                    error.DoNotHaveFocus => {},
                    else => return err,
                };
            }
            gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
            gctx.flushResourceBarriers();

            try app.compositor.submitExplicitTimingData();
        }
        app.compositor.postPresentHandoff();
    }

    gctx.finishGpuCommands();
}
