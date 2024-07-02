const std = @import("std");

const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;

const content_dir = @import("build_options").content_dir;

pub const Input = extern struct {
    mouse_position: [2]f32,
};

gctx: *zd3d12.GraphicsContext,
pipeline: zd3d12.PipelineHandle,
vertices: zd3d12.VerticesHandle,
vertex_indices: zd3d12.VertexIndicesHandle,
input: zd3d12.ConstantBufferHandle(Input),
frac: f32 = 0.0,
frac_delta: f32 = 0.0001,

const Self = @This();

pub fn init(allocator: std.mem.Allocator, gctx: *zd3d12.GraphicsContext) (zwin32.HResultError || std.fs.File.OpenError || std.fs.SelfExePathError || std.posix.SeekError || std.posix.ReadError || error{OutOfMemory})!Self {
    const Vertex = extern struct {
        position: [2]f32,
    };

    return .{
        .gctx = gctx,
        .pipeline = pipeline: {
            var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
            defer arena_allocator_state.deinit();
            const arena_allocator = arena_allocator_state.allocator();

            const exe_path = try std.fs.selfExeDirPathAlloc(arena_allocator);
            var exe_dir = try std.fs.openDirAbsolute(exe_path, .{});
            defer exe_dir.close();

            var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC{
                .InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&[_]d3d12.INPUT_ELEMENT_DESC{
                    d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
                }),
                .DepthStencilState = .{
                    .DepthEnable = w32.FALSE,
                },
                .NumRenderTargets = 1,
                .PrimitiveTopologyType = .TRIANGLE,
                .VS = d3d12.SHADER_BYTECODE.init(try exe_dir.readFileAlloc(arena_allocator, content_dir ++ "live_editing.vs.cso", 256 * 1024)),
                .PS = d3d12.SHADER_BYTECODE.init(try exe_dir.readFileAlloc(arena_allocator, content_dir ++ "live_editing.ps.cso", 256 * 1024)),
            };
            pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;

            break :pipeline gctx.createGraphicsShaderPipeline(&pso_desc);
        },
        .vertices = try gctx.uploadVertices(Vertex, &[_]Vertex{
            .{ .position = [_]f32{ -0.9, -0.9 } },
            .{ .position = [_]f32{ 0.0, 0.9 } },
            .{ .position = [_]f32{ 0.9, -0.9 } },
        }),
        .vertex_indices = try gctx.uploadVertexIndices(u16, &[_]u16{ 0, 1, 2 }),
        .input = try gctx.createConstantBuffer(Input),
    };
}

pub fn deinit(self: Self) void {
    self.input.deinit(self.gctx.*);
    self.vertex_indices.deinit(self.gctx.*);
    self.vertices.deinit(self.gctx.*);
}

pub fn inputUpdated(self: *Self, input: Input) void {
    self.input.ptr.* = input;
}

pub fn renderFrameD3d12(self: Self) zwin32.HResultError!void {
    const back_buffer = self.gctx.getBackBuffer();
    self.gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    self.gctx.flushResourceBarriers();

    self.gctx.omSetRenderTargets(
        &.{back_buffer.descriptor_handle},
        true,
        null,
    );
    self.gctx.clearRenderTargetView(
        back_buffer.descriptor_handle,
        &.{ 0.2, self.frac, 0.8, 1.0 },
        &.{},
    );

    self.gctx.setCurrentPipeline(self.pipeline);
    self.gctx.setGraphicsRootConstantBufferView(0, self.input.resource);

    self.gctx.iaSetPrimitiveTopology(.TRIANGLELIST);
    self.gctx.iaSetVertexBuffers(0, &.{self.vertices.view});
    self.gctx.iaSetIndexBuffer(&self.vertex_indices.view);
    self.gctx.drawIndexedInstanced(3, 1, 0, 0, 0);

    self.gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
    self.gctx.flushResourceBarriers();
}

pub fn postRenderFrame(self: *Self) void {
    self.frac += self.frac_delta;
    if (self.frac > 1.0 or self.frac < 0.0) {
        self.frac_delta = -self.frac_delta;
    }
}
