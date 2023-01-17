const std = @import("std");
const math = std.math;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const hrPanic = zwin32.hrPanic;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const num_mipmaps = 5;

const Vertex = struct {
    position: vm.Vec3,
    uv: vm.Vec2,
};
comptime {
    std.debug.assert(@sizeOf([2]Vertex) == 40);
    std.debug.assert(@alignOf([2]Vertex) == 4);
}

const DemoState = struct {
    const window_name = "zig-gamedev: textured quad";
    const window_width = 1024;
    const window_height = 1024;

    window: w32.HWND,
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,
    pipeline: zd3d12.PipelineHandle,
    vertex_buffer: zd3d12.ResourceHandle,
    index_buffer: zd3d12.ResourceHandle,
    texture: zd3d12.ResourceHandle,
    texture_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
    mipmap_level: i32,

    fn init(allocator: std.mem.Allocator) !DemoState {
        const window = try common.initWindow(allocator, window_name, window_width, window_height);
        var gctx = zd3d12.GraphicsContext.init(allocator, window);

        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        const pipeline = blk: {
            const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
                d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                d3d12.INPUT_ELEMENT_DESC.init("_Texcoords", 0, .R32G32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
            pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
            pso_desc.NumRenderTargets = 1;
            pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
            pso_desc.PrimitiveTopologyType = .TRIANGLE;
            pso_desc.RasterizerState.CullMode = .NONE;
            pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
            pso_desc.InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            };

            break :blk gctx.createGraphicsShaderPipeline(
                arena_allocator,
                &pso_desc,
                content_dir ++ "shaders/textured_quad.vs.cso",
                content_dir ++ "shaders/textured_quad.ps.cso",
            );
        };

        const vertex_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(num_mipmaps * 4 * @sizeOf(Vertex)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err);

        const index_buffer = gctx.createCommittedResource(
            .DEFAULT,
            .{},
            &d3d12.RESOURCE_DESC.initBuffer(4 * @sizeOf(u32)),
            .{ .COPY_DEST = true },
            null,
        ) catch |err| hrPanic(err);

        var mipgen = zd3d12.MipmapGenerator.init(arena_allocator, &gctx, .R8G8B8A8_UNORM, content_dir);

        gctx.beginFrame();

        const guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

        const texture = gctx.createAndUploadTex2dFromFile(
            content_dir ++ "genart_0025_5.png",
            .{}, // Create complete mipmap chain (up to 1x1).
        ) catch |err| hrPanic(err);

        mipgen.generateMipmaps(&gctx, texture);

        const texture_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        gctx.device.CreateShaderResourceView(
            gctx.lookupResource(texture).?,
            &d3d12.SHADER_RESOURCE_VIEW_DESC{
                .Format = .UNKNOWN,
                .ViewDimension = .TEXTURE2D,
                .Shader4ComponentMapping = d3d12.DEFAULT_SHADER_4_COMPONENT_MAPPING,
                .u = .{
                    .Texture2D = .{
                        .MostDetailedMip = 0,
                        .MipLevels = 0xffff_ffff,
                        .PlaneSlice = 0,
                        .ResourceMinLODClamp = 0.0,
                    },
                },
            },
            texture_srv,
        );

        // Fill vertex buffer.
        {
            const upload_verts = gctx.allocateUploadBufferRegion(Vertex, num_mipmaps * 4);
            var mipmap_index: u32 = 0;
            var r: f32 = 1.0;
            while (mipmap_index < num_mipmaps) : (mipmap_index += 1) {
                const index = mipmap_index * 4;
                upload_verts.cpu_slice[index] = .{
                    .position = vm.Vec3.init(-r, r, 0.0),
                    .uv = vm.Vec2.init(0.0, 0.0),
                };
                upload_verts.cpu_slice[index + 1] = .{
                    .position = vm.Vec3.init(r, r, 0.0),
                    .uv = vm.Vec2.init(1.0, 0.0),
                };
                upload_verts.cpu_slice[index + 2] = .{
                    .position = vm.Vec3.init(-r, -r, 0.0),
                    .uv = vm.Vec2.init(0.0, 1.0),
                };
                upload_verts.cpu_slice[index + 3] = .{
                    .position = vm.Vec3.init(r, -r, 0.0),
                    .uv = vm.Vec2.init(1.0, 1.0),
                };
                r *= 0.5;
            }
            gctx.cmdlist.CopyBufferRegion(
                gctx.lookupResource(vertex_buffer).?,
                0,
                upload_verts.buffer,
                upload_verts.buffer_offset,
                upload_verts.cpu_slice.len * @sizeOf(@TypeOf(upload_verts.cpu_slice[0])),
            );
        }
        // Fill index buffer.
        {
            const upload_indices = gctx.allocateUploadBufferRegion(u32, 4);
            upload_indices.cpu_slice[0] = 0;
            upload_indices.cpu_slice[1] = 1;
            upload_indices.cpu_slice[2] = 2;
            upload_indices.cpu_slice[3] = 3;

            gctx.cmdlist.CopyBufferRegion(
                gctx.lookupResource(index_buffer).?,
                0,
                upload_indices.buffer,
                upload_indices.buffer_offset,
                upload_indices.cpu_slice.len * @sizeOf(@TypeOf(upload_indices.cpu_slice[0])),
            );
        }

        gctx.addTransitionBarrier(vertex_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
        gctx.addTransitionBarrier(index_buffer, .{ .INDEX_BUFFER = true });
        gctx.addTransitionBarrier(texture, .{ .PIXEL_SHADER_RESOURCE = true });
        gctx.flushResourceBarriers();

        gctx.endFrame();
        gctx.finishGpuCommands();

        // NOTE(mziulek):
        // We need to call 'deinit' explicitly - we can't rely on 'defer' in this case because it runs *after*
        // 'gctx' is copied in 'return' statement below.
        mipgen.deinit(&gctx);

        return DemoState{
            .gctx = gctx,
            .guir = guir,
            .window = window,
            .frame_stats = common.FrameStats.init(),
            .pipeline = pipeline,
            .vertex_buffer = vertex_buffer,
            .index_buffer = index_buffer,
            .texture = texture,
            .texture_srv = texture_srv,
            .mipmap_level = 1,
        };
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        demo.gctx.finishGpuCommands();
        demo.guir.deinit(&demo.gctx);
        demo.gctx.deinit(allocator);
        common.deinitWindow(allocator);
        demo.* = undefined;
    }

    fn update(demo: *DemoState) void {
        demo.frame_stats.update(demo.gctx.window, window_name);

        common.newImGuiFrame(demo.frame_stats.delta_time);

        c.igSetNextWindowPos(.{ .x = 10.0, .y = 10.0 }, c.ImGuiCond_FirstUseEver, .{ .x = 0.0, .y = 0.0 });
        c.igSetNextWindowSize(.{ .x = 600.0, .y = 0.0 }, c.ImGuiCond_FirstUseEver);
        _ = c.igBegin(
            "Demo Settings",
            null,
            c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
        );
        _ = c.igSliderInt("Mipmap Level", &demo.mipmap_level, 0, num_mipmaps - 1, null, c.ImGuiSliderFlags_None);
        c.igEnd();
    }

    fn draw(demo: *DemoState) void {
        var gctx = &demo.gctx;
        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();

        gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w32.TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(
            back_buffer.descriptor_handle,
            &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );
        gctx.setCurrentPipeline(demo.pipeline);
        gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLESTRIP);
        gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
            .BufferLocation = gctx.lookupResource(demo.vertex_buffer).?.GetGPUVirtualAddress(),
            .SizeInBytes = num_mipmaps * 4 * @sizeOf(Vertex),
            .StrideInBytes = @sizeOf(Vertex),
        }});
        gctx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = gctx.lookupResource(demo.index_buffer).?.GetGPUVirtualAddress(),
            .SizeInBytes = 4 * @sizeOf(u32),
            .Format = .R32_UINT,
        });
        gctx.cmdlist.SetGraphicsRootDescriptorTable(0, gctx.copyDescriptorsToGpuHeap(1, demo.texture_srv));
        gctx.cmdlist.DrawIndexedInstanced(4, 1, 0, demo.mipmap_level * 4, 0);

        demo.guir.draw(gctx);

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }
};

pub fn main() !void {
    common.init();
    defer common.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try DemoState.init(allocator);
    defer demo.deinit(allocator);

    while (common.handleWindowEvents()) {
        demo.update();
        demo.draw();
    }
}
