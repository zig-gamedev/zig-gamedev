const std = @import("std");
const zgui = @import("zgui");
const glfw = @import("zglfw");
const zd3d12 = @import("zd3d12");
const zmath = @import("zmath");
const zpix = @import("zpix");
const zphysics = @import("zphysics");

const zwindows = @import("zwindows");
const d3d12 = zwindows.d3d12;
const dxgi = zwindows.dxgi;

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;
const demo_name = @import("build_options").demo_name;

const window_name = "zig-gamedev: " ++ demo_name;

const ObjectLayers = struct {
    const non_moving: zphysics.ObjectLayer = 0;
    const moving: zphysics.ObjectLayer = 1;
    const len: u32 = 2;
};

const BroadPhaseLayers = struct {
    const non_moving: zphysics.BroadPhaseLayer = 0;
    const moving: zphysics.BroadPhaseLayer = 1;
    const len: u32 = 2;
};

const BroadPhaseLayerInterface = extern struct {
    usingnamespace zphysics.BroadPhaseLayerInterface.Methods(@This());
    __v: *const zphysics.BroadPhaseLayerInterface.VTable = &vtable,

    object_to_broad_phase: [ObjectLayers.len]zphysics.BroadPhaseLayer = undefined,

    const vtable = zphysics.BroadPhaseLayerInterface.VTable{
        .getNumBroadPhaseLayers = _getNumBroadPhaseLayers,
        .getBroadPhaseLayer = _getBroadPhaseLayer,
    };

    fn init() BroadPhaseLayerInterface {
        var layer_interface: BroadPhaseLayerInterface = .{};
        layer_interface.object_to_broad_phase[ObjectLayers.non_moving] = BroadPhaseLayers.non_moving;
        layer_interface.object_to_broad_phase[ObjectLayers.moving] = BroadPhaseLayers.moving;
        return layer_interface;
    }

    fn _getNumBroadPhaseLayers(_: *const zphysics.BroadPhaseLayerInterface) callconv(.C) u32 {
        return BroadPhaseLayers.len;
    }

    fn _getBroadPhaseLayer(
        interface_self: *const zphysics.BroadPhaseLayerInterface,
        object_layer: zphysics.ObjectLayer,
    ) callconv(.C) zphysics.BroadPhaseLayer {
        const self: *const BroadPhaseLayerInterface = @ptrCast(interface_self);
        return self.object_to_broad_phase[object_layer];
    }
};

const ObjectVsBroadPhaseLayerFilter = extern struct {
    usingnamespace zphysics.ObjectVsBroadPhaseLayerFilter.Methods(@This());
    __v: *const zphysics.ObjectVsBroadPhaseLayerFilter.VTable = &vtable,

    const vtable = zphysics.ObjectVsBroadPhaseLayerFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphysics.ObjectVsBroadPhaseLayerFilter,
        object_layer: zphysics.ObjectLayer,
        broad_phase_layer: zphysics.BroadPhaseLayer,
    ) callconv(.C) bool {
        return switch (object_layer) {
            ObjectLayers.non_moving => broad_phase_layer == BroadPhaseLayers.moving,
            ObjectLayers.moving => true,
            else => unreachable,
        };
    }
};

const ObjectLayerPairFilter = extern struct {
    usingnamespace zphysics.ObjectLayerPairFilter.Methods(@This());
    __v: *const zphysics.ObjectLayerPairFilter.VTable = &vtable,

    const vtable = zphysics.ObjectLayerPairFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphysics.ObjectLayerPairFilter,
        a: zphysics.ObjectLayer,
        b: zphysics.ObjectLayer,
    ) callconv(.C) bool {
        return switch (a) {
            ObjectLayers.non_moving => b == ObjectLayers.moving,
            ObjectLayers.moving => true,
            else => unreachable,
        };
    }
};

pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    const pix_library = try zpix.loadGpuCapturerLibrary();
    defer pix_library.deinit();

    var rng = std.rand.DefaultPrng.init(42);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try glfw.init();
    defer glfw.terminate();

    glfw.windowHintTyped(.client_api, .no_api);
    const glfw_window = try glfw.Window.create(800, 600, window_name, null);
    defer glfw_window.destroy();

    zgui.init(allocator);
    defer zgui.deinit();

    const window = glfw.getWin32Window(glfw_window) orelse return error.FailedToGetWin32Window;
    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator,
        .window = window,
    });
    defer gctx.deinit(allocator);

    const cube_pipeline = cube_pipeline: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_MESH_POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_MESH_NORMAL", 0, .R32G32B32_FLOAT, 0, 16, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_TRANSFORM", 0, .R32G32B32A32_FLOAT, 1, 0, .PER_INSTANCE_DATA, 1),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_TRANSFORM", 1, .R32G32B32A32_FLOAT, 1, d3d12.APPEND_ALIGNED_ELEMENT, .PER_INSTANCE_DATA, 1),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_TRANSFORM", 2, .R32G32B32A32_FLOAT, 1, d3d12.APPEND_ALIGNED_ELEMENT, .PER_INSTANCE_DATA, 1),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_TRANSFORM", 3, .R32G32B32A32_FLOAT, 1, d3d12.APPEND_ALIGNED_ELEMENT, .PER_INSTANCE_DATA, 1),
            d3d12.INPUT_ELEMENT_DESC.init("CUBE_COLOR", 0, .R32G32B32_FLOAT, 1, d3d12.APPEND_ALIGNED_ELEMENT, .PER_INSTANCE_DATA, 1),
        });
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ demo_name ++ "-cube.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ demo_name ++ ".ps.cso", 256 * 1024));

        break :cube_pipeline gctx.createGraphicsShaderPipeline(&pso_desc);
    };

    const depth_texture_view = gctx.allocateCpuDescriptors(.DSV, 1);
    var depth_texture_resource: ?zd3d12.ResourceHandle = null;

    try zphysics.init(allocator, .{});
    defer zphysics.deinit();

    const broad_phase_layer_interface = try allocator.create(BroadPhaseLayerInterface);
    defer allocator.destroy(broad_phase_layer_interface);
    broad_phase_layer_interface.* = BroadPhaseLayerInterface.init();

    const object_vs_broad_phase_layer_filter = try allocator.create(ObjectVsBroadPhaseLayerFilter);
    defer allocator.destroy(object_vs_broad_phase_layer_filter);
    object_vs_broad_phase_layer_filter.* = .{};

    const object_layer_pair_filter = try allocator.create(ObjectLayerPairFilter);
    defer allocator.destroy(object_layer_pair_filter);
    object_layer_pair_filter.* = .{};

    const MAX_CUBES = 128;

    const physics_system = try zphysics.PhysicsSystem.create(
        @as(*const zphysics.BroadPhaseLayerInterface, @ptrCast(broad_phase_layer_interface)),
        @as(*const zphysics.ObjectVsBroadPhaseLayerFilter, @ptrCast(object_vs_broad_phase_layer_filter)),
        @as(*const zphysics.ObjectLayerPairFilter, @ptrCast(object_layer_pair_filter)),
        .{
            .max_bodies = MAX_CUBES,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );
    defer physics_system.destroy();

    const body_interface = physics_system.getBodyInterfaceMut();

    const cube_shape_settings = try zphysics.BoxShapeSettings.create(.{ 0.5, 0.5, 0.5 });
    defer cube_shape_settings.release();

    const cube_shape = try cube_shape_settings.createShape();
    defer cube_shape.release();
    {
        const floor_shape_settings = try zphysics.BoxShapeSettings.create(.{ 3, 1, 3 });
        defer floor_shape_settings.release();

        const floor_shape = try floor_shape_settings.createShape();
        defer floor_shape.release();

        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0, -1, 0, 1 },
            .rotation = .{ 0, 0, 0, 1 },
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = ObjectLayers.non_moving,
        }, .activate);

        physics_system.optimizeBroadPhase();
    }

    const F32x3 = @Vector(3, f32);

    const CubeVertex = extern struct {
        position: F32x3,
        normal: F32x3,
    };
    const cube_mesh = [_]CubeVertex{
        .{ .position = .{ -0.5, -0.5, -0.5 }, .normal = .{ 0, -1, 0 } },
        .{ .position = .{ 0.5, -0.5, -0.5 }, .normal = .{ 0, -1, 0 } },
        .{ .position = .{ -0.5, -0.5, 0.5 }, .normal = .{ 0, -1, 0 } },

        .{ .position = .{ 0.5, -0.5, -0.5 }, .normal = .{ 0, -1, 0 } },
        .{ .position = .{ 0.5, -0.5, 0.5 }, .normal = .{ 0, -1, 0 } },
        .{ .position = .{ -0.5, -0.5, 0.5 }, .normal = .{ 0, -1, 0 } },

        .{ .position = .{ -0.5, -0.5, -0.5 }, .normal = .{ 0, 0, -1 } },
        .{ .position = .{ -0.5, 0.5, -0.5 }, .normal = .{ 0, 0, -1 } },
        .{ .position = .{ 0.5, -0.5, -0.5 }, .normal = .{ 0, 0, -1 } },

        .{ .position = .{ -0.5, 0.5, -0.5 }, .normal = .{ 0, 0, -1 } },
        .{ .position = .{ 0.5, 0.5, -0.5 }, .normal = .{ 0, 0, -1 } },
        .{ .position = .{ 0.5, -0.5, -0.5 }, .normal = .{ 0, 0, -1 } },

        .{ .position = .{ -0.5, -0.5, -0.5 }, .normal = .{ -1, 0, 0 } },
        .{ .position = .{ -0.5, -0.5, 0.5 }, .normal = .{ -1, 0, 0 } },
        .{ .position = .{ -0.5, 0.5, -0.5 }, .normal = .{ -1, 0, 0 } },

        .{ .position = .{ -0.5, -0.5, 0.5 }, .normal = .{ -1, 0, 0 } },
        .{ .position = .{ -0.5, 0.5, 0.5 }, .normal = .{ -1, 0, 0 } },
        .{ .position = .{ -0.5, 0.5, -0.5 }, .normal = .{ -1, 0, 0 } },

        .{ .position = .{ -0.5, 0.5, -0.5 }, .normal = .{ 0, 1, 0 } },
        .{ .position = .{ -0.5, 0.5, 0.5 }, .normal = .{ 0, 1, 0 } },
        .{ .position = .{ 0.5, 0.5, -0.5 }, .normal = .{ 0, 1, 0 } },

        .{ .position = .{ 0.5, 0.5, -0.5 }, .normal = .{ 0, 1, 0 } },
        .{ .position = .{ -0.5, 0.5, 0.5 }, .normal = .{ 0, 1, 0 } },
        .{ .position = .{ 0.5, 0.5, 0.5 }, .normal = .{ 0, 1, 0 } },

        .{ .position = .{ -0.5, -0.5, 0.5 }, .normal = .{ 0, 0, 1 } },
        .{ .position = .{ 0.5, -0.5, 0.5 }, .normal = .{ 0, 0, 1 } },
        .{ .position = .{ -0.5, 0.5, 0.5 }, .normal = .{ 0, 0, 1 } },

        .{ .position = .{ -0.5, 0.5, 0.5 }, .normal = .{ 0, 0, 1 } },
        .{ .position = .{ 0.5, -0.5, 0.5 }, .normal = .{ 0, 0, 1 } },
        .{ .position = .{ 0.5, 0.5, 0.5 }, .normal = .{ 0, 0, 1 } },

        .{ .position = .{ 0.5, -0.5, -0.5 }, .normal = .{ 1, 0, 0 } },
        .{ .position = .{ 0.5, 0.5, -0.5 }, .normal = .{ 1, 0, 0 } },
        .{ .position = .{ 0.5, -0.5, 0.5 }, .normal = .{ 1, 0, 0 } },

        .{ .position = .{ 0.5, -0.5, 0.5 }, .normal = .{ 1, 0, 0 } },
        .{ .position = .{ 0.5, 0.5, -0.5 }, .normal = .{ 1, 0, 0 } },
        .{ .position = .{ 0.5, 0.5, 0.5 }, .normal = .{ 1, 0, 0 } },
    };

    const cube_vertices = try gctx.uploadVertices(CubeVertex, &cube_mesh);
    const Cube = extern struct {
        transform: zmath.Mat,
        color: F32x3,
    };
    const cubes = try gctx.createWritableVertices(Cube, MAX_CUBES);
    const ConstantBuffer = extern struct {
        model_view_projection: zmath.Mat,
    };
    const constant = try gctx.createConstantBuffer(ConstantBuffer);

    const scale_factor = scale_factor: {
        const scale = glfw_window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };
    _ = zgui.io.addFontFromFile(
        content_dir ++ "Roboto-Medium.ttf",
        std.math.floor(16.0 * scale_factor),
    );

    zgui.getStyle().scaleAllSizes(scale_factor);

    {
        const cbv_srv = gctx.cbv_srv_uav_gpu_heaps[0];
        zgui.backend.init(
            glfw_window,
            gctx.device,
            zd3d12.GraphicsContext.max_num_buffered_frames,
            @intFromEnum(dxgi.FORMAT.R8G8B8A8_UNORM),
            cbv_srv.heap.?,
            @bitCast(cbv_srv.base.cpu_handle),
            @bitCast(cbv_srv.base.gpu_handle),
        );
    }
    defer zgui.backend.deinit();

    var framebuffer_size: [2]i32 = .{ 0, 0 };

    var frame_timer = try std.time.Timer.start();
    const frame_rate_target: u64 = 60;

    var cube_spawn_timer = try std.time.Timer.start();
    const cube_spawn_rate_ns = @divTrunc(std.time.ns_per_s, 2);

    var cube_instances = std.ArrayList(Cube).init(allocator);
    defer cube_instances.deinit();

    var out_of_bounds_body_id_indices = std.ArrayList(usize).init(allocator);
    defer out_of_bounds_body_id_indices.deinit();

    var cube_body_ids = std.ArrayList(zphysics.BodyId).init(allocator);
    defer cube_body_ids.deinit();

    const lock_interface = physics_system.getBodyLockInterface();

    while (!glfw_window.shouldClose() and glfw_window.getKey(.escape) != .press) {
        {
            // spin loop for frame limiter
            const target_ns = @divTrunc(std.time.ns_per_s, frame_rate_target);
            while (frame_timer.read() < target_ns) {
                std.atomic.spinLoopHint();
            }
            frame_timer.reset();
        }

        if (cube_spawn_timer.read() > cube_spawn_rate_ns) {
            _ = try body_interface.createAndAddBody(.{
                .position = .{ 0, 10, 0, 1 },
                .rotation = zmath.quatFromRollPitchYaw(
                    2 * std.math.pi * rng.random().float(f32),
                    2 * std.math.pi * rng.random().float(f32),
                    2 * std.math.pi * rng.random().float(f32),
                ),
                .shape = cube_shape,
                .motion_type = .dynamic,
                .object_layer = ObjectLayers.moving,
                .angular_velocity = .{ 0, 0, 0, 0 },
                //.allow_sleeping = false,
            }, .activate);
            cube_spawn_timer.reset();
        }

        physics_system.update(1.0 / @as(f32, @floatFromInt(frame_rate_target)), .{}) catch unreachable;

        glfw.pollEvents();

        if (glfw_window.getAttribute(.iconified)) {
            // Window is minimized
            const ns_in_ms: u64 = 1_000_000;
            std.time.sleep(10 * ns_in_ms);
            continue;
        }

        {
            const next_framebuffer_size = glfw_window.getFramebufferSize();
            if (!std.meta.eql(framebuffer_size, next_framebuffer_size)) {
                gctx.resize(@intCast(next_framebuffer_size[0]), @intCast(next_framebuffer_size[1]));
                if (depth_texture_resource) |resource| {
                    gctx.destroyResource(resource);
                }
                depth_texture_resource = try gctx.createCommittedResource(
                    .DEFAULT,
                    .{},
                    &d3d12.RESOURCE_DESC.initDepthBuffer(.D32_FLOAT, gctx.viewport_width, gctx.viewport_height),
                    .{ .DEPTH_WRITE = true },
                    &d3d12.CLEAR_VALUE.initDepthStencil(.D32_FLOAT, 1.0, 0),
                );
                gctx.createDepthStencilView(
                    depth_texture_resource.?,
                    null,
                    depth_texture_view,
                );
                const camera_transform = zmath.lookAtLh(
                    .{ 2, 5, -10, 0 },
                    .{ 0, 2, 0, 0 },
                    .{ 0, 1, 0, 0 },
                );
                const perspective_transform = zmath.perspectiveFovLh(
                    0.25 * std.math.pi,
                    @as(f32, @floatFromInt(gctx.viewport_width)) / @as(f32, @floatFromInt(gctx.viewport_height)),
                    0.01,
                    100.0,
                );
                constant.ptr.model_view_projection = zmath.mul(camera_transform, perspective_transform);
            }
            framebuffer_size = next_framebuffer_size;
        }

        {
            gctx.beginFrame();
            defer gctx.endFrame();

            {
                cube_instances.clearRetainingCapacity();
                out_of_bounds_body_id_indices.clearRetainingCapacity();

                try physics_system.getBodyIds(&cube_body_ids);
                for (cube_body_ids.items, 0..) |body_id, i| {
                    var read_lock: zphysics.BodyLockRead = .{};
                    read_lock.lock(lock_interface, body_id);
                    defer read_lock.unlock();

                    if (read_lock.body) |body| {
                        if (body.position[1] < -6) {
                            try out_of_bounds_body_id_indices.append(i);
                            continue;
                        }
                        const position = if (zphysics.Real == f32)
                            zmath.loadArr4(body.position)
                        else
                            zmath.loadArr4(.{
                                @as(f32, @floatCast(body.position[0])),
                                @as(f32, @floatCast(body.position[1])),
                                @as(f32, @floatCast(body.position[2])),
                                @as(f32, @floatCast(body.position[3])),
                            });

                        const cube_size = zphysics.BoxShape.asBoxShape(body.shape).getHalfExtent();

                        const scaling = zmath.mul(zmath.scaling(2.0, 2.0, 2.0), zmath.scalingV(if (zphysics.Real == f32)
                            zmath.loadArr3w(cube_size, 1)
                        else
                            zmath.loadArr3w(.{
                                @as(f32, @floatCast(cube_size[0])),
                                @as(f32, @floatCast(cube_size[1])),
                                @as(f32, @floatCast(cube_size[2])),
                            }, 1)));
                        const rotation = zmath.matFromQuat(zmath.loadArr4(body.rotation));
                        const translate = zmath.translationV(position);
                        const transform = zmath.mul(scaling, zmath.mul(rotation, translate));

                        try cube_instances.append(.{
                            .transform = transform,
                            .color = if (body.motion_properties == null) .{ 0.8, 0.8, 0.8 } else .{ 1, 0, 0 },
                        });
                    }
                }
                try gctx.writeVertices(Cube, cubes, cube_instances.items);

                {
                    var it = std.mem.reverseIterator(out_of_bounds_body_id_indices.items);
                    while (it.next()) |i| {
                        const body_id = cube_body_ids.swapRemove(i);
                        body_interface.removeAndDestroyBody(body_id);
                    }
                }
            }

            const back_buffer = gctx.getBackBuffer();
            gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            gctx.flushResourceBarriers();

            gctx.omSetRenderTargets(
                &.{back_buffer.descriptor_handle},
                true,
                &depth_texture_view,
            );
            gctx.clearRenderTargetView(
                back_buffer.descriptor_handle,
                &.{ 0.0, 0.0, 0.0, 1.0 },
                &.{},
            );
            gctx.clearDepthStencilView(depth_texture_view, .{ .DEPTH = true }, 1.0, 0, &.{});

            {
                gctx.setCurrentPipeline(cube_pipeline);

                gctx.setGraphicsRootConstantBufferView(0, constant.resource);

                gctx.iaSetPrimitiveTopology(.TRIANGLELIST);
                gctx.iaSetVertexBuffers(0, &.{ cube_vertices.view, cubes.view });
                gctx.drawInstanced(36, @intCast(cube_instances.items.len), 0, 0);
            }

            zgui.backend.newFrame(@intCast(framebuffer_size[0]), @intCast(framebuffer_size[1]));

            // Set the starting window position and size to custom values
            zgui.setNextWindowPos(.{ .x = 20, .y = 20, .cond = .first_use_ever });
            zgui.setNextWindowSize(.{ .w = 300, .h = 80, .cond = .first_use_ever });

            if (zgui.begin("Status", .{})) {
                _ = zgui.inputScalar("cube count", usize, .{ .v = &cube_instances.items.len, .flags = .{
                    .read_only = true,
                } });
            }
            zgui.end();

            zgui.backend.draw(gctx.cmdlist);

            gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
            gctx.flushResourceBarriers();
        }
    }

    gctx.finishGpuCommands();
}
