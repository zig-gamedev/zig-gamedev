const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const utf8ToUtf16LeStringLiteral = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const Vertex = struct {
    position: Vec3,
    normal: Vec3,
    texcoords0: Vec2,
};
comptime {
    assert(@sizeOf([2]Vertex) == 64);
    assert(@alignOf([2]Vertex) == 4);
}

fn loadMesh(
    filename: []const u8,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList(Vec3),
    normals: ?*std.ArrayList(Vec3),
    texcoords0: ?*std.ArrayList(Vec2),
) void {
    const data = blk: {
        var data: *c.cgltf_data = undefined;
        const options = std.mem.zeroes(c.cgltf_options);
        // Parse.
        {
            const result = c.cgltf_parse_file(&options, filename.ptr, @ptrCast([*c][*c]c.cgltf_data, &data));
            assert(result == c.cgltf_result_success);
        }
        // Load.
        {
            const result = c.cgltf_load_buffers(&options, data, filename.ptr);
            assert(result == c.cgltf_result_success);
        }
        break :blk data;
    };
    defer c.cgltf_free(data);

    const num_vertices: u32 = @intCast(u32, data.meshes[0].primitives[0].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[0].primitives[0].indices.*.count);

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
            }
        }
    }
}

const DemoState = struct {
    const window_name = "zig-gamedev: simple3d";
    const window_width = 1920;
    const window_height = 1080;

    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,
    pipeline: gr.PipelineHandle,

    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,
    entity_buffer: gr.ResourceHandle,

    base_color_texture: gr.ResourceHandle,
    depth_texture: gr.ResourceHandle,

    entity_buffer_srv: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    base_color_texture_srv: w.D3D12_CPU_DESCRIPTOR_HANDLE,
    depth_texture_srv: w.D3D12_CPU_DESCRIPTOR_HANDLE,

    brush: *w.ID2D1SolidColorBrush,
    textformat: *w.IDWriteTextFormat,
    num_mesh_vertices: u32,
    num_mesh_indices: u32,

    fn init(allocator: *std.mem.Allocator) DemoState {
        const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
        var grfx = gr.GraphicsContext.init(window);

        const pipeline = blk: {
            const input_layout_desc = [_]w.D3D12_INPUT_ELEMENT_DESC{
                w.D3D12_INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                w.D3D12_INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
                w.D3D12_INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 24, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = w.D3D12_GRAPHICS_PIPELINE_STATE_DESC.initDefault();
            pso_desc.RasterizerState.CullMode = .NONE;
            pso_desc.DSVFormat = .D32_FLOAT;
            pso_desc.InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            };
            pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
            break :blk grfx.createGraphicsShaderPipeline(
                allocator,
                &pso_desc,
                "content/shaders/simple3d.vs.cso",
                "content/shaders/simple3d.ps.cso",
            );
        };

        const entity_buffer = grfx.createCommittedResource(
            .DEFAULT,
            w.D3D12_HEAP_FLAG_NONE,
            &w.D3D12_RESOURCE_DESC.initBuffer(1 * @sizeOf(Mat4)),
            w.D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
            null,
        ) catch |err| hrPanic(err);

        const entity_buffer_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(entity_buffer),
            &w.D3D12_SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(0, 1, @sizeOf(Mat4)),
            entity_buffer_srv,
        );

        const brush = blk: {
            var maybe_brush: ?*w.ID2D1SolidColorBrush = null;
            hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
                &w.D2D1_COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
                null,
                &maybe_brush,
            ));
            break :blk maybe_brush.?;
        };

        const textformat = blk: {
            var maybe_textformat: ?*w.IDWriteTextFormat = null;
            hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
                utf8ToUtf16LeStringLiteral("Verdana"),
                null,
                w.DWRITE_FONT_WEIGHT.NORMAL,
                w.DWRITE_FONT_STYLE.NORMAL,
                w.DWRITE_FONT_STRETCH.NORMAL,
                32.0,
                utf8ToUtf16LeStringLiteral("en-us"),
                &maybe_textformat,
            ));
            break :blk maybe_textformat.?;
        };
        hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
        hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

        var mipgen = gr.MipmapGenerator.init(allocator, &grfx, .R8G8B8A8_UNORM);

        grfx.beginFrame();

        var gui = gr.GuiContext.init(allocator, &grfx);

        const base_color_texture = grfx.createAndUploadTex2dFromFile(
            utf8ToUtf16LeStringLiteral("content/SciFiHelmet/SciFiHelmet_BaseColor.png"),
            0, // Create complete mipmap chain (up to 1x1).
        ) catch |err| hrPanic(err);

        mipgen.generateMipmaps(&grfx, base_color_texture);

        const base_color_texture_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(grfx.getResource(base_color_texture), null, base_color_texture_srv);

        const depth_texture = grfx.createCommittedResource(
            .DEFAULT,
            w.D3D12_HEAP_FLAG_NONE,
            &blk: {
                var desc = w.D3D12_RESOURCE_DESC.initTex2d(.D32_FLOAT, grfx.viewport_width, grfx.viewport_height, 1);
                desc.Flags = w.D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | w.D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
                break :blk desc;
            },
            w.D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &w.D3D12_CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
        ) catch |err| hrPanic(err);

        const depth_texture_srv = grfx.allocateCpuDescriptors(.DSV, 1);
        grfx.device.CreateDepthStencilView(grfx.getResource(depth_texture), null, depth_texture_srv);

        const buffers = blk: {
            var indices = std.ArrayList(u32).init(allocator);
            defer indices.deinit();
            var positions = std.ArrayList(Vec3).init(allocator);
            defer positions.deinit();
            var normals = std.ArrayList(Vec3).init(allocator);
            defer normals.deinit();
            var texcoords0 = std.ArrayList(Vec2).init(allocator);
            defer texcoords0.deinit();
            loadMesh("content/SciFiHelmet/SciFiHelmet.gltf", &indices, &positions, &normals, &texcoords0);

            const num_vertices = @intCast(u32, positions.items.len);
            const num_indices = @intCast(u32, indices.items.len);

            const vertex_buffer = grfx.createCommittedResource(
                .DEFAULT,
                w.D3D12_HEAP_FLAG_NONE,
                &w.D3D12_RESOURCE_DESC.initBuffer(num_vertices * @sizeOf(Vertex)),
                w.D3D12_RESOURCE_STATE_COPY_DEST,
                null,
            ) catch |err| hrPanic(err);

            const index_buffer = grfx.createCommittedResource(
                .DEFAULT,
                w.D3D12_HEAP_FLAG_NONE,
                &w.D3D12_RESOURCE_DESC.initBuffer(num_indices * @sizeOf(u32)),
                w.D3D12_RESOURCE_STATE_COPY_DEST,
                null,
            ) catch |err| hrPanic(err);

            const upload_verts = grfx.allocateUploadBufferRegion(Vertex, num_vertices);
            for (positions.items) |_, i| {
                upload_verts.cpu_slice[i] = .{
                    .position = positions.items[i],
                    .normal = normals.items[i],
                    .texcoords0 = texcoords0.items[i],
                };
            }
            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(vertex_buffer),
                0,
                upload_verts.buffer,
                upload_verts.buffer_offset,
                upload_verts.cpu_slice.len * @sizeOf(@TypeOf(upload_verts.cpu_slice[0])),
            );

            const upload_indices = grfx.allocateUploadBufferRegion(u32, num_indices);
            for (indices.items) |index, i| {
                upload_indices.cpu_slice[i] = index;
            }
            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(index_buffer),
                0,
                upload_indices.buffer,
                upload_indices.buffer_offset,
                upload_indices.cpu_slice.len * @sizeOf(@TypeOf(upload_indices.cpu_slice[0])),
            );

            break :blk .{
                .num_vertices = num_vertices,
                .num_indices = num_indices,
                .vertex = vertex_buffer,
                .index = index_buffer,
            };
        };

        grfx.addTransitionBarrier(buffers.vertex, w.D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        grfx.addTransitionBarrier(buffers.index, w.D3D12_RESOURCE_STATE_INDEX_BUFFER);
        grfx.addTransitionBarrier(base_color_texture, w.D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        grfx.flushResourceBarriers();

        grfx.finishGpuCommands();

        mipgen.deinit(&grfx);

        return .{
            .grfx = grfx,
            .gui = gui,
            .frame_stats = lib.FrameStats.init(),
            .pipeline = pipeline,
            .vertex_buffer = buffers.vertex,
            .index_buffer = buffers.index,
            .entity_buffer = entity_buffer,
            .base_color_texture = base_color_texture,
            .depth_texture = depth_texture,
            .entity_buffer_srv = entity_buffer_srv,
            .base_color_texture_srv = base_color_texture_srv,
            .depth_texture_srv = depth_texture_srv,
            .brush = brush,
            .textformat = textformat,
            .num_mesh_vertices = buffers.num_vertices,
            .num_mesh_indices = buffers.num_indices,
        };
    }

    fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
        demo.grfx.finishGpuCommands();
        _ = demo.brush.Release();
        _ = demo.textformat.Release();
        _ = demo.grfx.releaseResource(demo.vertex_buffer);
        _ = demo.grfx.releaseResource(demo.index_buffer);
        _ = demo.grfx.releaseResource(demo.entity_buffer);
        _ = demo.grfx.releaseResource(demo.base_color_texture);
        _ = demo.grfx.releaseResource(demo.depth_texture);
        _ = demo.grfx.releasePipeline(demo.pipeline);
        demo.gui.deinit(&demo.grfx);
        demo.grfx.deinit(allocator);
        lib.deinitWindow(allocator);
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

        grfx.addTransitionBarrier(demo.entity_buffer, w.D3D12_RESOURCE_STATE_COPY_DEST);
        grfx.flushResourceBarriers();

        {
            const object_to_camera = mat4.mul(
                mat4.initRotationY(@floatCast(f32, 0.5 * demo.frame_stats.time)),
                mat4.initLookAtLh(
                    vec3.init(2.2, 2.2, -2.2),
                    vec3.init(0.0, 0.0, 0.0),
                    vec3.init(0.0, 1.0, 0.0),
                ),
            );
            const upload_entity = grfx.allocateUploadBufferRegion(Mat4, 1);
            upload_entity.cpu_slice[0] = mat4.transpose(
                mat4.mul(
                    object_to_camera,
                    mat4.initPerspectiveFovLh(
                        math.pi / 3.0,
                        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
                        0.1,
                        100.0,
                    ),
                ),
            );
            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(demo.entity_buffer),
                0,
                upload_entity.buffer,
                upload_entity.buffer_offset,
                upload_entity.cpu_slice.len * @sizeOf(Mat4),
            );
        }

        const back_buffer = grfx.getBackBuffer();

        grfx.addTransitionBarrier(demo.entity_buffer, w.D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_RENDER_TARGET);
        grfx.flushResourceBarriers();

        grfx.cmdlist.OMSetRenderTargets(
            1,
            &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w.TRUE,
            &demo.depth_texture_srv,
        );
        grfx.cmdlist.ClearRenderTargetView(
            back_buffer.descriptor_handle,
            &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );
        grfx.cmdlist.ClearDepthStencilView(demo.depth_texture_srv, w.D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, null);
        grfx.setCurrentPipeline(demo.pipeline);
        grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]w.D3D12_VERTEX_BUFFER_VIEW{.{
            .BufferLocation = grfx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = demo.num_mesh_vertices * @sizeOf(Vertex),
            .StrideInBytes = @sizeOf(Vertex),
        }});
        grfx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = grfx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = demo.num_mesh_indices * @sizeOf(u32),
            .Format = .R32_UINT,
        });
        grfx.cmdlist.SetGraphicsRootDescriptorTable(2, grfx.copyDescriptorsToGpuHeap(1, demo.base_color_texture_srv));
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, demo.entity_buffer_srv));
        grfx.cmdlist.SetGraphicsRoot32BitConstant(0, 0, 0);
        grfx.cmdlist.DrawIndexedInstanced(demo.num_mesh_indices, 1, 0, 0, 0);

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

            demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 });
            lib.DrawText(
                grfx.d2d.context,
                text,
                demo.textformat,
                &w.D2D1_RECT_F{
                    .left = 10.0,
                    .top = 10.0,
                    .right = @intToFloat(f32, grfx.viewport_width),
                    .bottom = @intToFloat(f32, grfx.viewport_height),
                },
                @ptrCast(*w.ID2D1Brush, demo.brush),
            );
        }
        grfx.endDraw2d();

        grfx.endFrame();
    }
};

pub fn main() !void {
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = DemoState.init(allocator);
    defer demo.deinit(allocator);

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
            demo.update();
            demo.draw();
        }
    }
}
