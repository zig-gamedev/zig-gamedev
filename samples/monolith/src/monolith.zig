const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zphy = @import("zphysics");
const zmesh = @import("zmesh");
const wgsl = @import("monolith_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: monolith sample (wgpu)";

const floor_material = zm.f32x4(-0.7, -0.7, -0.7, 0.8);
const monolith_scale = zm.scaling(10, 50, 5);
const monolith_default_rotate = zm.mul(zm.rotationZ(0.24), zm.rotationY(0.3));
const monolith_default_center = zm.f32x4(-15, 0, 30, 1);
const monolith_radius = zm.mul(monolith_scale, zm.f32x4(0.5, 0.5, 0.5, 0));
const monolith_ray_radius = zm.mul(monolith_scale, zm.f32x4(0.4999, 0.4999, 0.4999, 0)); // .4999 is anti-artifact bias
const monolith_default_translate = zm.transpose(zm.translationV(monolith_default_center));
const monolith_default_transform = zm.mul(monolith_default_translate, zm.mul(monolith_default_rotate, monolith_scale));
const monolith_default_rotate_t = zm.transpose(monolith_default_rotate);

const FrameUniforms = extern struct {
    world_to_clip: zm.Mat align(16),
    floor_material: [4]f32 align(16),
    monolith_rotation: [3][4]f32 align(16), // GPU will expect each element to be 16-aligned too, so extra f32 padding.
    monolith_center: [4]f32 align(16), // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    monolith_ray_radius: [4]f32 align(16), // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    monolith_inv_radius: [4]f32 align(16), // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    camera_position: [3]f32 align(16), // Only [3]f32 logically, but [4]f32 in memory, so 3 or 4 works the same.
    lights: [9][4]f32 align(16) = .{.{ 0, 0, 0, 0 }} ** 9, // padding again - only using vec3s.
};

const DrawUniforms = extern struct {
    object_to_world: zm.Mat align(16),
    basecolor_roughness: [4]f32 align(16),
};

const Vertex = extern struct {
    position: [3]f32,
    normal: [3]f32,
};

const Mesh = struct {
    index_offset: u32,
    vertex_offset: i32,
    num_indices: u32,
    num_vertices: u32,

    const IndexType = zmesh.Shape.IndexType;
};

const object_layers = struct {
    const non_moving: zphy.ObjectLayer = 0;
    const moving: zphy.ObjectLayer = 1;
    const sensors: zphy.ObjectLayer = 2;
    const player: zphy.ObjectLayer = 3;
    const len: u32 = 4;
};

const broad_phase_layers = struct {
    const non_moving: zphy.BroadPhaseLayer = 0;
    const moving: zphy.BroadPhaseLayer = 1;
    const sensors: zphy.ObjectLayer = 2;
    const player: zphy.ObjectLayer = 3;
    const len: u32 = 4;
};

const BroadPhaseLayerInterface = extern struct {
    usingnamespace zphy.BroadPhaseLayerInterface.Methods(@This());
    __v: *const zphy.BroadPhaseLayerInterface.VTable = &vtable,

    object_to_broad_phase: [object_layers.len]zphy.BroadPhaseLayer = undefined,

    const vtable = zphy.BroadPhaseLayerInterface.VTable{
        .getNumBroadPhaseLayers = _getNumBroadPhaseLayers,
        .getBroadPhaseLayer = _getBroadPhaseLayer,
    };

    fn init() BroadPhaseLayerInterface {
        var layer_interface: BroadPhaseLayerInterface = .{};
        layer_interface.object_to_broad_phase[object_layers.non_moving] = broad_phase_layers.non_moving;
        layer_interface.object_to_broad_phase[object_layers.moving] = broad_phase_layers.moving;
        layer_interface.object_to_broad_phase[object_layers.sensors] = broad_phase_layers.sensors;
        layer_interface.object_to_broad_phase[object_layers.player] = broad_phase_layers.player;
        return layer_interface;
    }

    fn _getNumBroadPhaseLayers(_: *const zphy.BroadPhaseLayerInterface) callconv(.C) u32 {
        return broad_phase_layers.len;
    }

    fn _getBroadPhaseLayer(
        iself: *const zphy.BroadPhaseLayerInterface,
        layer: zphy.ObjectLayer,
    ) callconv(.C) zphy.BroadPhaseLayer {
        const self = @as(*const BroadPhaseLayerInterface, @ptrCast(iself));
        return self.object_to_broad_phase[layer];
    }
};

const ObjectVsBroadPhaseLayerFilter = extern struct {
    usingnamespace zphy.ObjectVsBroadPhaseLayerFilter.Methods(@This());
    __v: *const zphy.ObjectVsBroadPhaseLayerFilter.VTable = &vtable,

    const vtable = zphy.ObjectVsBroadPhaseLayerFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphy.ObjectVsBroadPhaseLayerFilter,
        layer1: zphy.ObjectLayer,
        layer2: zphy.BroadPhaseLayer,
    ) callconv(.C) bool {
        return switch (layer1) {
            object_layers.non_moving => layer2 == broad_phase_layers.moving or layer2 == broad_phase_layers.player,
            object_layers.moving => layer2 == broad_phase_layers.non_moving or layer2 == broad_phase_layers.sensors,
            object_layers.sensors => layer2 == broad_phase_layers.moving,
            object_layers.player => layer2 == broad_phase_layers.non_moving,
            else => unreachable,
        };
    }
};

const ObjectLayerPairFilter = extern struct {
    usingnamespace zphy.ObjectLayerPairFilter.Methods(@This());
    __v: *const zphy.ObjectLayerPairFilter.VTable = &vtable,

    const vtable = zphy.ObjectLayerPairFilter.VTable{ .shouldCollide = _shouldCollide };

    fn _shouldCollide(
        _: *const zphy.ObjectLayerPairFilter,
        object1: zphy.ObjectLayer,
        object2: zphy.ObjectLayer,
    ) callconv(.C) bool {
        return switch (object1) {
            object_layers.non_moving => object2 == object_layers.moving or object2 == object_layers.player,
            object_layers.moving => object2 == object_layers.non_moving or object2 == object_layers.sensors,
            object_layers.sensors => object2 == object_layers.moving,
            object_layers.player => object2 == object_layers.non_moving,
            else => unreachable,
        };
    }
};

const ContactListener = extern struct {
    usingnamespace zphy.ContactListener.Methods(@This());

    pub const SensorContacts = extern struct {
        dynamic: zphy.BodyId = std.math.maxInt(zphy.BodyId),
        kinematic: zphy.BodyId = std.math.maxInt(zphy.BodyId),
        static: zphy.BodyId = std.math.maxInt(zphy.BodyId),
    };

    __v: *const zphy.ContactListener.VTable = &vtable,
    bodies_touching_sensors: [9]SensorContacts = .{.{}} ** 9,

    const vtable = zphy.ContactListener.VTable{
        .onContactValidate = _onContactValidate,
        .onContactPersisted = _onContactPersisted,
    };

    fn _onContactValidate(
        _: *zphy.ContactListener,
        _: *const zphy.Body,
        _: *const zphy.Body,
        _: *const [3]zphy.Real,
        _: *const zphy.CollideShapeResult,
    ) callconv(.C) zphy.ValidateResult {
        return .accept_all_contacts;
    }

    fn _onContactPersisted(
        self_base: *zphy.ContactListener,
        body1: *const zphy.Body,
        body2: *const zphy.Body,
        _: *const zphy.ContactManifold,
        _: *zphy.ContactSettings,
    ) callconv(.C) void {
        const self = @as(*ContactListener, @ptrCast(self_base));
        if (body1.isSensor()) self.appendSensorContact(body2, body1);
        if (body2.isSensor()) self.appendSensorContact(body1, body2);
    }

    fn appendSensorContact(self: *ContactListener, dynamic_body: *const zphy.Body, sensor: *const zphy.Body) void {
        const index = index: {
            const cached_index_plus_one = dynamic_body.getUserData();
            if (cached_index_plus_one > 0) break :index cached_index_plus_one - 1;
            for (self.bodies_touching_sensors, 0..) |sensor_contacts, i| {
                if (sensor_contacts.dynamic == dynamic_body.getId()) break :index i;
                if (sensor_contacts.dynamic == std.math.maxInt(zphy.BodyId)) {
                    self.bodies_touching_sensors[i].dynamic = dynamic_body.getId();
                    break :index i;
                }
            }
            unreachable; // bodies_touching_sensors array may need to be larger
        };
        switch (sensor.motion_type) {
            .kinematic => self.bodies_touching_sensors[index].kinematic = sensor.getId(),
            .static => self.bodies_touching_sensors[index].static = sensor.getId(),
            else => unreachable,
        }
    }

    pub fn getSensorContacts(self: *ContactListener, dynamic_body: *zphy.Body) SensorContacts {
        const cached_index_plus_one = dynamic_body.getUserData();
        if (cached_index_plus_one > 0) return self.bodies_touching_sensors[cached_index_plus_one - 1];
        for (self.bodies_touching_sensors, 0..) |sensor_contacts, i| {
            if (sensor_contacts.dynamic == dynamic_body.getId()) {
                dynamic_body.setUserData(i + 1);
                return sensor_contacts;
            }
            if (sensor_contacts.dynamic == std.math.maxInt(zphy.BodyId)) break;
        }
        return SensorContacts{};
    }

    pub fn clearSensorContacts(self: *ContactListener) void {
        for (0..self.bodies_touching_sensors.len) |i| {
            if (self.bodies_touching_sensors[i].dynamic == std.math.maxInt(zphy.BodyId)) break;
            self.bodies_touching_sensors[i].kinematic = std.math.maxInt(zphy.BodyId);
            self.bodies_touching_sensors[i].static = std.math.maxInt(zphy.BodyId);
        }
    }

    pub const touching_sensor_value: u64 = 1;
};

const DebugRenderer = struct {
    // array sizes may not be large enough for other scenes - they are the next 2^n past needed for this sample scene.
    const max_prims = std.math.pow(usize, 2, 5);
    const max_verts = std.math.pow(usize, 2, 14);
    const max_indcs = std.math.pow(usize, 2, 15);
    const DebugVertex = struct {
        position: [3]f32 = .{ 0, 0, 0 },
        normal: [3]f32 = .{ 0, 1, 0 },
    };
    const Primitive = struct {
        index_start: usize = std.math.maxInt(usize),
        index_count: usize = std.math.maxInt(usize),
        vert_offset: usize = std.math.maxInt(usize),
    };
    const DrawInstance = struct {
        prim: *const Primitive,
        mat: [16]zphy.Real,
        color: [3]f32 = .{ 0, 1, 1 },
    };
    usingnamespace zphy.DebugRenderer.Methods(@This());
    __v: *const zphy.DebugRenderer.VTable(@This()) = &vtable,

    primitives: [max_prims]Primitive = .{.{}} ** max_prims,
    vertices: [max_verts]DebugVertex = .{.{}} ** max_verts,
    indices: [max_indcs]u16 = .{std.math.maxInt(u16)} ** max_indcs,
    heads: struct {
        prim: usize = 0,
        vert: usize = 0,
        indx: usize = 0,
    } = .{},

    demo: *DemoState,
    vertex_buf: zgpu.BufferHandle = .{},
    index_buf: zgpu.BufferHandle = .{},
    debug_render_pipe: zgpu.RenderPipelineHandle = .{},
    body_draw_settings: zphy.DebugRenderer.BodyDrawSettings = .{ .shape_color = .instance_color },
    body_draw_filter: *zphy.DebugRenderer.BodyDrawFilter,
    body_draw_list: std.ArrayList(DrawInstance),

    const vtable = zphy.DebugRenderer.VTable(@This()){
        .drawLine = drawLine,
        .drawTriangle = drawTriangle,
        .createTriangleBatch = createTriangleBatch,
        .createTriangleBatchIndexed = createTriangleBatchIndexed,
        .drawGeometry = drawGeometry,
        .drawText3D = drawText3D,
    };

    pub fn init(alloc: std.mem.Allocator, demo: *DemoState) DebugRenderer {
        return DebugRenderer{
            .demo = demo,
            .body_draw_filter = zphy.DebugRenderer.createBodyDrawFilter(DebugRenderer.shouldBodyDraw),
            .body_draw_list = std.ArrayList(DrawInstance).init(alloc),
        };
    }

    pub fn deinit(self: *DebugRenderer) void {
        self.body_draw_list.clearAndFree();
        self.body_draw_list.deinit();
    }

    pub fn initGraphics(
        self: *DebugRenderer,
        alloc: std.mem.Allocator,
        unif: zgpu.BindGroupLayoutHandle,
    ) !void {
        // Tell Jolt to draw the bodies to make sure it actually loads all the primitives before the vertex buffers
        // are uploaded. Otherwise it will wait until the first time you call drawBodies. This won't actually draw here.
        self.demo.physics_system.drawBodies(
            &self.demo.physics_debug_renderer.body_draw_settings,
            self.demo.physics_debug_renderer.body_draw_filter,
        );

        const gctx = self.demo.gctx;

        self.vertex_buf = gctx.createBuffer(.{
            .usage = .{ .copy_dst = true, .vertex = true },
            .size = self.vertices.len * @sizeOf(DebugVertex),
        });
        gctx.queue.writeBuffer(gctx.lookupResource(self.vertex_buf).?, 0, DebugVertex, &self.vertices);

        self.index_buf = gctx.createBuffer(.{
            .usage = .{ .copy_dst = true, .index = true },
            .size = self.indices.len * @sizeOf(u16),
        });
        gctx.queue.writeBuffer(gctx.lookupResource(self.index_buf).?, 0, u16, &self.indices);

        { // pipeline
            const pl = gctx.createPipelineLayout(&.{ unif, unif });
            defer gctx.releaseResource(pl);

            const vs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl.debug.vs, null);
            defer vs_mod.release();

            const fs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl.debug.fs, null);
            defer fs_mod.release();

            const color_targets = [_]wgpu.ColorTargetState{.{
                .format = zgpu.GraphicsContext.swapchain_format,
                .blend = &wgpu.BlendState{ .color = wgpu.BlendComponent{
                    .operation = .add,
                    .src_factor = .src_alpha,
                    .dst_factor = .one_minus_src_alpha,
                }, .alpha = wgpu.BlendComponent{} },
            }};

            const vertex_attributes = [_]wgpu.VertexAttribute{
                .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
                .{ .format = .float32x3, .offset = @offsetOf(DebugVertex, "normal"), .shader_location = 1 },
            };

            const vertex_buffers = [_]wgpu.VertexBufferLayout{.{
                .array_stride = @sizeOf(Vertex),
                .attribute_count = vertex_attributes.len,
                .attributes = &vertex_attributes,
            }};

            const pipe_desc = wgpu.RenderPipelineDescriptor{
                .vertex = wgpu.VertexState{
                    .module = vs_mod,
                    .entry_point = "main",
                    .buffer_count = vertex_buffers.len,
                    .buffers = &vertex_buffers,
                },
                .fragment = &wgpu.FragmentState{
                    .module = fs_mod,
                    .entry_point = "main",
                    .target_count = color_targets.len,
                    .targets = &color_targets,
                },
                .depth_stencil = &wgpu.DepthStencilState{
                    .format = .depth32_float,
                    .depth_write_enabled = false,
                    .depth_compare = .greater,
                    .depth_bias = 0,
                },
                .primitive = .{
                    .topology = .triangle_list,
                    .front_face = .cw,
                    .cull_mode = .back,
                },
            };
            gctx.createRenderPipelineAsync(alloc, pl, pipe_desc, &self.debug_render_pipe);
        }
    }

    pub fn draw(self: *DebugRenderer, pass: wgpu.RenderPassEncoder) void {
        const gctx = self.demo.gctx;
        const uniform_bg = gctx.lookupResource(self.demo.uniform_bg) orelse return;
        const vertex_buf_info = gctx.lookupResourceInfo(self.vertex_buf) orelse return;
        const index_buf_info = gctx.lookupResourceInfo(self.index_buf) orelse return;
        const debug_render_pipe = gctx.lookupResource(self.debug_render_pipe) orelse return;

        pass.setVertexBuffer(0, vertex_buf_info.gpuobj.?, 0, vertex_buf_info.size);
        pass.setIndexBuffer(index_buf_info.gpuobj.?, .uint16, 0, index_buf_info.size);
        pass.setPipeline(debug_render_pipe);

        self.demo.physics_system.drawBodies(
            &self.demo.physics_debug_renderer.body_draw_settings,
            self.demo.physics_debug_renderer.body_draw_filter,
        );
        for (self.body_draw_list.items) |instance| {
            const mem = gctx.uniformsAllocate(DrawUniforms, 1);
            mem.slice[0] = .{
                .object_to_world = zm.loadMat(&instance.mat),
                .basecolor_roughness = .{ instance.color[0], instance.color[1], instance.color[2], 0.5 },
            };
            pass.setBindGroup(1, uniform_bg, &.{mem.offset});
            pass.drawIndexed(@as(u32, @intCast(instance.prim.index_count)), 1, @as(u32, @intCast(instance.prim.index_start)), @as(i32, @intCast(instance.prim.vert_offset)), 0);
        }
        self.body_draw_list.clearRetainingCapacity();
    }

    pub fn drawGui(self: *DebugRenderer) void {
        zgui.setNextWindowPos(.{ .x = 1.0, .y = 1.0, .cond = .once });
        zgui.setNextWindowSize(.{ .w = 400, .h = -1, .cond = .once });
        zgui.setNextWindowCollapsed(.{ .collapsed = false, .cond = .once });

        if (zgui.begin("Monolith", .{ .flags = .{
            .no_move = true,
            .no_resize = true,
            .no_scrollbar = true,
            .no_scroll_with_mouse = true,
            .no_saved_settings = true,
            .no_nav_inputs = true,
            .no_nav_focus = true,
        } })) {
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });
            zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average: ");
            zgui.sameLine(.{});
            zgui.text(
                "{d:.3} ms/frame ({d:.1} fps)",
                .{ self.demo.gctx.stats.average_cpu_time, self.demo.gctx.stats.fps },
            );
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });

            zgui.separator();
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });
            zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Controls: ");
            zgui.sameLine(.{});
            zgui.textWrapped("WASD. Left ALT moves down. SPACE moves up. Hold shift for speed. Right click to capture mouse " ++
                "cursor and enable mouse look. Left click to disable mouse look and free mouse cursor.", .{});
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });

            zgui.separator();
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });
            zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Options:");
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });
            _ = zgui.checkbox("Physics Debug Renderer - Draw Bodies", .{ .v = &self.demo.physics_debug_enabled });
            _ = zgui.checkbox("Camera - Detach From Physics", .{ .v = &self.demo.camera.out_of_body });
            _ = zgui.checkbox("ImGuizmo - Move Monolith Around", .{ .v = &self.demo.gizmo_enabled });
            if (self.demo.gizmo_enabled) {
                zgui.sameLine(.{});
                _ = zgui.checkbox("World Space", .{ .v = &self.demo.gizmo_world_space });
            }
            zgui.dummy(.{ .w = -1.0, .h = 5.0 });
        }
        zgui.end();

        if (!self.demo.gizmo_enabled) return;

        const eye = zm.loadArr3(self.demo.camera.position);
        const up = zm.f32x4(0.0, 1.0, 0.0, 0.0);
        const az = zm.normalize3(zm.loadArr3(self.demo.camera.forward));
        const view = zm.matToArr(zm.lookToLh(eye, az, up));

        const near = 1.0;
        const fov_y = 0.33 * math.pi;
        const fb_width = self.demo.gctx.swapchain_descriptor.width;
        const fb_height = self.demo.gctx.swapchain_descriptor.height;
        const a: f32 = @as(f32, @floatFromInt(fb_width)) / @as(f32, @floatFromInt(fb_height));
        const proj = zm.matToArr(zm.perspectiveFovLh(fov_y, a, near, 1000.0));

        zgui.gizmo.beginFrame();
        zgui.gizmo.setDrawList(zgui.getForegroundDrawList());
        zgui.gizmo.allowAxisFlip(true);
        zgui.gizmo.setOrthographic(false);
        zgui.gizmo.setRect(0, 0, @floatFromInt(fb_width), @floatFromInt(fb_height));

        const gizmo_op = zgui.gizmo.Operation{
            .translate_x = true,
            .translate_y = true,
            .translate_z = true,
            .rotate_x = true,
            .rotate_y = true,
            .rotate_z = true,
        };
        var gizmo_mode: u32 = @intFromEnum(zgui.gizmo.Mode.local);
        if (self.demo.gizmo_world_space) gizmo_mode = @intFromEnum(zgui.gizmo.Mode.world);

        var matrix = zm.matToArr(zm.transpose(self.demo.monolith.transform));
        const changed = zgui.gizmo.manipulate(&view, &proj, gizmo_op, @enumFromInt(gizmo_mode), &matrix, .{});
        if (changed) {
            self.demo.monolith.transform = zm.transpose(zm.matFromArr(matrix));
            self.demo.monolith.recalculateFromTransform();
            const body_interface = self.demo.physics_system.getBodyInterfaceMut();
            body_interface.setPosition(self.demo.monolith.body, zm.vecToArr3(self.demo.monolith.center), .activate);
            body_interface.setRotation(self.demo.monolith.body, zm.quatFromMat(self.demo.monolith.rotate_t), .activate);
        }
    }

    pub fn shouldBodyDraw(body: *const zphy.Body) callconv(.C) bool {
        if (body.object_layer == object_layers.non_moving) return false;
        return true;
    }

    fn drawLine(
        _: *DebugRenderer,
        _: *const [3]zphy.Real,
        _: *const [3]zphy.Real,
        _: *const zphy.DebugRenderer.Color,
    ) callconv(.C) void {}

    fn drawTriangle(
        _: *DebugRenderer,
        _: *const [3]zphy.Real,
        _: *const [3]zphy.Real,
        _: *const [3]zphy.Real,
        _: *const zphy.DebugRenderer.Color,
    ) callconv(.C) void {}

    fn createTriangleBatch(_: *DebugRenderer, _: [*]zphy.DebugRenderer.Triangle, _: u32) callconv(.C) *anyopaque {
        unreachable; // Jolt's debug renderer seems to only use the indexed one below, so not implementing this.
    }

    fn createTriangleBatchIndexed(
        self: *DebugRenderer,
        vertices: [*]zphy.DebugRenderer.Vertex,
        vertex_count: u32,
        indices: [*]u32,
        index_count: u32,
    ) callconv(.C) *anyopaque {
        self.primitives[self.heads.prim] = .{
            .index_start = self.heads.indx,
            .index_count = index_count,
            .vert_offset = self.heads.vert,
        };
        const prim_ptr = &self.primitives[self.heads.prim];
        self.heads.prim += 1;
        for (0..index_count) |i| {
            self.indices[self.heads.indx] = @as(u16, @intCast(indices[i]));
            self.heads.indx += 1;
        }
        for (0..vertex_count) |i| {
            self.vertices[self.heads.vert] = .{
                .position = vertices[i].position,
                .normal = vertices[i].normal,
            };
            self.heads.vert += 1;
        }
        return zphy.DebugRenderer.createTriangleBatch(prim_ptr);
    }

    fn drawGeometry(
        self: *DebugRenderer,
        mat: *const zphy.RMatrix,
        _: *const zphy.AABox,
        _: f32,
        color: zphy.DebugRenderer.Color,
        geometry: *const zphy.DebugRenderer.Geometry,
        _: zphy.DebugRenderer.CullMode,
        _: zphy.DebugRenderer.CastShadow,
        _: zphy.DebugRenderer.DrawMode,
    ) callconv(.C) void {
        const batch = geometry.LODs[0].batch;
        const prim = @as(*const Primitive, @alignCast(@ptrCast(zphy.DebugRenderer.getPrimitiveFromBatch(batch))));
        const lowp_model_matrix: [16]f32 = .{
            mat.column_0[0], mat.column_1[0], mat.column_2[0], lowP(mat.column_3[0]),
            mat.column_0[1], mat.column_1[1], mat.column_2[1], lowP(mat.column_3[1]),
            mat.column_0[2], mat.column_1[2], mat.column_2[2], lowP(mat.column_3[2]),
            0,               0,               0,               1,
        };
        self.body_draw_list.append(.{
            .prim = prim,
            .mat = lowp_model_matrix,
            .color = .{
                @as(f32, @floatFromInt(color.comp.r)) / 255.0,
                @as(f32, @floatFromInt(color.comp.g)) / 255.0,
                @as(f32, @floatFromInt(color.comp.b)) / 255.0,
            },
        }) catch unreachable;
    }

    fn drawText3D(
        _: *DebugRenderer,
        _: *const [3]zphy.Real,
        _: [*:0]const u8,
        _: zphy.DebugRenderer.Color,
        _: f32,
    ) callconv(.C) void {}
};

inline fn lowP(r: zphy.Real) f32 {
    return @as(f32, @floatCast(r));
}

const DemoState = struct {
    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,

    mesh_render_pipe: zgpu.RenderPipelineHandle = .{},
    uniform_bg: zgpu.BindGroupHandle,

    vertex_buf: zgpu.BufferHandle,
    index_buf: zgpu.BufferHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    meshes: std.ArrayList(Mesh),

    broad_phase_layer_interface: *BroadPhaseLayerInterface,
    object_vs_broad_phase_layer_filter: *ObjectVsBroadPhaseLayerFilter,
    object_layer_pair_filter: *ObjectLayerPairFilter,
    contact_listener: *ContactListener,
    physics_system: *zphy.PhysicsSystem,

    wisp_bodies: [9]zphy.BodyId = .{0} ** 9,
    physics_debug_renderer: DebugRenderer,
    physics_debug_enabled: bool = false,
    gizmo_enabled: bool = false,
    gizmo_world_space: bool = false,

    camera: struct {
        position: [3]f32 = .{ 0.0, 12.0, -64.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.04 * math.pi,
        yaw: f32 = -0.10 * math.pi,
        rotation: zm.Mat = zm.identity(),
        rigid_body: zphy.BodyId = std.math.maxInt(zphy.BodyId),
        collector_body: zphy.BodyId = std.math.maxInt(zphy.BodyId),
        collector_attractor: [3]f32 = .{ 0, 0, 0 },
        out_of_body: bool = false,
    } = .{},
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
        captured: bool = false,
    } = .{},
    monolith: struct {
        body: zphy.BodyId = 0,
        center: zm.Vec = monolith_default_center,
        translate: zm.Mat = monolith_default_translate,
        transform: zm.Mat = monolith_default_transform,
        rotate_t: zm.Mat = monolith_default_rotate_t,

        pub fn recalculateFromTransform(self: *@This()) void {
            const gizmo_xform = zm.matToArr(zm.transpose(self.transform));
            var gizmo_center = zgui.gizmo.Vector{ 0, 0, 0 };
            var gizmo_rot = zgui.gizmo.Vector{ 0, 0, 0 };
            var gizmo_scale = zgui.gizmo.Vector{ 0, 0, 0 };
            zgui.gizmo.decomposeMatrixToComponents(&gizmo_xform, &gizmo_center, &gizmo_rot, &gizmo_scale);
            self.center = zm.f32x4(gizmo_center[0], gizmo_center[1], gizmo_center[2], 1);
            self.translate = zm.transpose(zm.translationV(self.center));

            var arr_rot = zm.matToArr(zm.identity());
            zgui.gizmo.recomposeMatrixFromComponents(&.{ 0, 0, 0 }, &gizmo_rot, &.{ 1, 1, 1 }, &arr_rot);
            self.rotate_t = zm.matFromArr(arr_rot);
        }
    } = .{},
};

fn appendMesh(
    mesh: zmesh.Shape,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(Mesh.IndexType),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals_or_colors: *std.ArrayList([3]f32),
) void {
    meshes.append(.{
        .index_offset = @as(u32, @intCast(meshes_indices.items.len)),
        .vertex_offset = @as(i32, @intCast(meshes_positions.items.len)),
        .num_indices = @as(u32, @intCast(mesh.indices.len)),
        .num_vertices = @as(u32, @intCast(mesh.positions.len)),
    }) catch unreachable;

    meshes_indices.appendSlice(mesh.indices) catch unreachable;
    meshes_positions.appendSlice(mesh.positions) catch unreachable;
    meshes_normals_or_colors.appendSlice(mesh.normals.?) catch unreachable;
}

const mesh_floor = 0;
const mesh_cube = 1;

fn generateMeshes(
    allocator: std.mem.Allocator,
    meshes: *std.ArrayList(Mesh),
    meshes_indices: *std.ArrayList(Mesh.IndexType),
    meshes_positions: *std.ArrayList([3]f32),
    meshes_normals: *std.ArrayList([3]f32),
) void {
    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    zmesh.init(arena);
    defer zmesh.deinit();

    {
        const terrain = (struct {
            fn impl(uv: *const [2]f32, position: *[3]f32, userdata: ?*anyopaque) callconv(.C) void {
                _ = userdata;
                position[0] = uv[0];
                position[1] = 0.0;
                position[2] = uv[1];
            }
        }).impl;
        var ground = zmesh.Shape.initParametric(terrain, 32, 32, null);
        defer ground.deinit();
        ground.translate(-0.5, 0.0, -0.5);
        ground.invert(0, 0);
        ground.scale(2000.0, 1.0, 2000.0);
        ground.unweld();
        ground.computeNormals();

        appendMesh(ground, meshes, meshes_indices, meshes_positions, meshes_normals);
    }

    {
        var cube = zmesh.Shape.initCube();
        defer cube.deinit();
        cube.translate(-0.5, -0.5, -0.5);
        cube.unweld();
        cube.computeNormals();

        appendMesh(cube, meshes, meshes_indices, meshes_positions, meshes_normals);
    }
}

fn create(allocator: std.mem.Allocator, window: *zglfw.Window) !*DemoState {
    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    //
    // Procedural meshes
    //
    var meshes = std.ArrayList(Mesh).init(allocator);
    var meshes_indices = std.ArrayList(Mesh.IndexType).init(arena);
    var meshes_positions = std.ArrayList([3]f32).init(arena);
    var meshes_normals = std.ArrayList([3]f32).init(arena);
    generateMeshes(allocator, &meshes, &meshes_indices, &meshes_positions, &meshes_normals);

    const total_num_vertices = @as(u32, @intCast(meshes_positions.items.len));
    const total_num_indices = @as(u32, @intCast(meshes_indices.items.len));

    //
    // Graphics
    //
    const gctx = try zgpu.GraphicsContext.create(
        allocator,
        .{
            .window = window,
            .fn_getTime = @ptrCast(&zglfw.getTime),
            .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
            .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
            .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
            .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
            .fn_getWaylandDisplay = @ptrCast(&zglfw.getWaylandDisplay),
            .fn_getWaylandSurface = @ptrCast(&zglfw.getWaylandWindow),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

    // Uniform buffer and layout
    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    const uniform_bg = gctx.createBindGroup(uniform_bgl, &.{
        .{
            .binding = 0,
            .buffer_handle = gctx.uniforms.buffer,
            .offset = 0,
            .size = @max(@sizeOf(FrameUniforms), @sizeOf(DrawUniforms)),
        },
    });

    // Vertex buffer
    const vertex_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .vertex = true },
        .size = total_num_vertices * @sizeOf(Vertex),
    });
    {
        var vertex_data = std.ArrayList(Vertex).init(arena);
        defer vertex_data.deinit();
        vertex_data.resize(total_num_vertices) catch unreachable;

        for (meshes_positions.items, 0..) |_, i| {
            vertex_data.items[i].position = meshes_positions.items[i];
            vertex_data.items[i].normal = meshes_normals.items[i];
        }
        gctx.queue.writeBuffer(gctx.lookupResource(vertex_buf).?, 0, Vertex, vertex_data.items);
    }

    // Index buffer
    const index_buf = gctx.createBuffer(.{
        .usage = .{ .copy_dst = true, .index = true },
        .size = total_num_indices * @sizeOf(Mesh.IndexType),
    });
    gctx.queue.writeBuffer(gctx.lookupResource(index_buf).?, 0, Mesh.IndexType, meshes_indices.items);

    // Depth texture
    const depth = createDepthTexture(gctx);

    //
    // Physics
    //
    try zphy.init(allocator, .{});

    const broad_phase_layer_interface = try allocator.create(BroadPhaseLayerInterface);
    broad_phase_layer_interface.* = BroadPhaseLayerInterface.init();

    const object_vs_broad_phase_layer_filter = try allocator.create(ObjectVsBroadPhaseLayerFilter);
    object_vs_broad_phase_layer_filter.* = .{};

    const object_layer_pair_filter = try allocator.create(ObjectLayerPairFilter);
    object_layer_pair_filter.* = .{};

    const contact_listener = try allocator.create(ContactListener);
    contact_listener.* = .{};

    const physics_system = try zphy.PhysicsSystem.create(
        @as(*const zphy.BroadPhaseLayerInterface, @ptrCast(broad_phase_layer_interface)),
        @as(*const zphy.ObjectVsBroadPhaseLayerFilter, @ptrCast(object_vs_broad_phase_layer_filter)),
        @as(*const zphy.ObjectLayerPairFilter, @ptrCast(object_layer_pair_filter)),
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );
    physics_system.setContactListener(contact_listener);
    physics_system.setGravity(.{ 0, 0, 0 });

    //
    // Demo
    //
    const demo = try allocator.create(DemoState);
    demo.* = .{
        .window = window,
        .gctx = gctx,
        .uniform_bg = uniform_bg,
        .vertex_buf = vertex_buf,
        .index_buf = index_buf,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .meshes = meshes,
        .broad_phase_layer_interface = broad_phase_layer_interface,
        .object_vs_broad_phase_layer_filter = object_vs_broad_phase_layer_filter,
        .object_layer_pair_filter = object_layer_pair_filter,
        .contact_listener = contact_listener,
        .physics_system = physics_system,
        .physics_debug_renderer = DebugRenderer.init(allocator, demo),
    };

    //
    // Physics Objects
    //
    {
        const body_interface = physics_system.getBodyInterfaceMut();

        const monolith_shape_settings = try zphy.BoxShapeSettings.create(.{
            monolith_radius[0],
            monolith_radius[1],
            monolith_radius[2],
        });
        defer monolith_shape_settings.release();
        const monolith_shape = try monolith_shape_settings.createShape();
        defer monolith_shape.release();
        demo.monolith.body = try body_interface.createAndAddBody(.{
            .position = demo.monolith.center,
            .rotation = zm.quatFromMat(demo.monolith.rotate_t),
            .shape = monolith_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);

        const floor_shape_settings = try zphy.BoxShapeSettings.create(.{ 800.0, 10.0, 800.0 });
        defer floor_shape_settings.release();
        const floor_shape = try floor_shape_settings.createShape();
        defer floor_shape.release();
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0.0, -10.0, 0.0, 1.0 },
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0.0, 1610.0, 0.0, 1.0 },
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0.0, 800.0, -790.0, 1.0 },
            .rotation = zm.quatFromNormAxisAngle(.{ 1, 0, 0, 0 }, 0.5 * math.pi),
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0.0, 800.0, 790.0, 1.0 },
            .rotation = zm.quatFromNormAxisAngle(.{ 1, 0, 0, 0 }, 0.5 * math.pi),
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);
        _ = try body_interface.createAndAddBody(.{
            .position = .{ -790.0, 800.0, 0.0, 1.0 },
            .rotation = zm.quatFromNormAxisAngle(.{ 0, 0, 1, 0 }, 0.5 * math.pi),
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 790.0, 800.0, 0.0, 1.0 },
            .rotation = zm.quatFromNormAxisAngle(.{ 0, 0, 1, 0 }, 0.5 * math.pi),
            .shape = floor_shape,
            .motion_type = .static,
            .object_layer = object_layers.non_moving,
        }, .activate);

        const sphere_shape_settings = try zphy.SphereShapeSettings.create(1.5);
        defer sphere_shape_settings.release();
        const sphere_shape = try sphere_shape_settings.createShape();
        defer sphere_shape.release();
        var i: u32 = 0;
        while (i < 9) : (i += 1) {
            const fi = @as(f32, @floatFromInt(i));
            const angle: f32 = std.math.degreesToRadians(fi * 40.0);
            demo.wisp_bodies[i] = try body_interface.createAndAddBody(.{
                .position = .{ 24.0 * std.math.cos(angle), 8.0 + std.math.sin(angle), 16.0 * std.math.sin(angle), 1 },
                .shape = sphere_shape,
                .motion_type = .dynamic,
                .object_layer = object_layers.moving,
                .allow_sleeping = false,
                .restitution = 0.8,
            }, .activate);
        }
        demo.camera.rigid_body = try body_interface.createAndAddBody(.{
            .position = zm.loadArr3(demo.camera.position),
            .shape = sphere_shape,
            .motion_type = .dynamic,
            .motion_quality = .linear_cast,
            .object_layer = object_layers.player,
            .allow_sleeping = false,
            .restitution = 0.0,
            .friction = 0.0,
        }, .activate);

        const flask_shape_settings = try zphy.TaperedCapsuleShapeSettings.create(4.0, 1.0, 5.0);
        defer flask_shape_settings.release();
        const flask_shape = try flask_shape_settings.createShape();
        defer flask_shape.release();
        demo.camera.collector_body = try body_interface.createAndAddBody(.{
            .position = zm.loadArr3(demo.camera.position),
            .shape = flask_shape,
            .motion_type = .static,
            .object_layer = object_layers.sensors,
            .is_sensor = true,
        }, .activate);

        const stir_bar_shape_settings = try zphy.BoxShapeSettings.create(.{ 60.0, 7.0, 4.0 });
        defer stir_bar_shape_settings.release();
        const stir_bar_shape = try stir_bar_shape_settings.createShape();
        defer stir_bar_shape.release();
        _ = try body_interface.createAndAddBody(.{
            .position = .{ 0, 8.0, 0, 1 },
            .shape = stir_bar_shape,
            .motion_type = .kinematic,
            .object_layer = object_layers.sensors,
            .angular_velocity = zm.quatFromNormAxisAngle(.{ 0, 1, 0, 0 }, -0.7),
            .angular_damping = 0.0,
            .is_sensor = true,
        }, .activate);

        physics_system.optimizeBroadPhase();
    }

    //
    // GPU pipelines (async compiled; need to be created *after* the `demo` instance is constructed)
    //
    {
        const pl = gctx.createPipelineLayout(&.{ uniform_bgl, uniform_bgl });
        defer gctx.releaseResource(pl);

        const vs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl.mesh.vs, null);
        defer vs_mod.release();

        const fs_mod = zgpu.createWgslShaderModule(gctx.device, wgsl.mesh.fs, null);
        defer fs_mod.release();

        const color_targets = [_]wgpu.ColorTargetState{.{
            .format = zgpu.GraphicsContext.swapchain_format,
            .blend = &wgpu.BlendState{ .color = wgpu.BlendComponent{
                .operation = .add,
                .src_factor = .one,
                .dst_factor = .zero,
            }, .alpha = wgpu.BlendComponent{
                .operation = .add,
                .src_factor = .src_alpha,
                .dst_factor = .zero,
            } },
        }};

        const vertex_attributes = [_]wgpu.VertexAttribute{
            .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
            .{ .format = .float32x3, .offset = @offsetOf(Vertex, "normal"), .shader_location = 1 },
        };

        const vertex_buffers = [_]wgpu.VertexBufferLayout{.{
            .array_stride = @sizeOf(Vertex),
            .attribute_count = vertex_attributes.len,
            .attributes = &vertex_attributes,
        }};

        const pipe_desc = wgpu.RenderPipelineDescriptor{
            .vertex = wgpu.VertexState{
                .module = vs_mod,
                .entry_point = "main",
                .buffer_count = vertex_buffers.len,
                .buffers = &vertex_buffers,
            },
            .fragment = &wgpu.FragmentState{
                .module = fs_mod,
                .entry_point = "main",
                .target_count = color_targets.len,
                .targets = &color_targets,
            },
            .depth_stencil = &wgpu.DepthStencilState{
                .format = .depth32_float,
                .depth_write_enabled = true,
                .depth_compare = .greater,
            },
            .primitive = .{
                .topology = .triangle_list,
                .front_face = .cw,
                .cull_mode = .back,
            },
        };
        gctx.createRenderPipelineAsync(allocator, pl, pipe_desc, &demo.mesh_render_pipe);
    }

    //
    // Physics Debug Renderer (includes own pipeline and buffers)
    //
    try zphy.DebugRenderer.createSingleton(&demo.physics_debug_renderer);
    try demo.physics_debug_renderer.initGraphics(allocator, uniform_bgl);

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    zphy.DebugRenderer.destroyBodyDrawFilter(demo.physics_debug_renderer.body_draw_filter);
    zphy.DebugRenderer.destroySingleton();
    demo.physics_debug_renderer.deinit();
    demo.meshes.deinit();
    demo.physics_system.destroy();
    allocator.destroy(demo.contact_listener);
    allocator.destroy(demo.object_vs_broad_phase_layer_filter);
    allocator.destroy(demo.object_layer_pair_filter);
    allocator.destroy(demo.broad_phase_layer_interface);
    zphy.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) void {
    const window = demo.window;

    { // Handle camera rotation with mouse.
        const cursor_pos = window.getCursorPos();
        const delta_x = @as(f32, @floatCast(cursor_pos[0] - demo.mouse.cursor_pos[0]));
        const delta_y = @as(f32, @floatCast(cursor_pos[1] - demo.mouse.cursor_pos[1]));
        demo.mouse.cursor_pos = cursor_pos;

        if (window.getMouseButton(.left) == .press) {
            if (demo.mouse.captured) {
                window.setInputMode(.cursor, zglfw.Cursor.Mode.normal);
                window.setInputMode(.raw_mouse_motion, false);
            }
            demo.mouse.captured = false;
        }
        if (window.getMouseButton(.right) == .press) {
            if (!demo.mouse.captured) {
                window.setInputMode(.cursor, zglfw.Cursor.Mode.disabled);
                window.setInputMode(.raw_mouse_motion, true);
            }
            demo.mouse.captured = true;
        }
        if (demo.mouse.captured) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = @min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = @max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = zm.modAngle(demo.camera.yaw);
        }
    }

    { // Apply contextual forces to the floating lights
        const bodies = demo.physics_system.getBodiesMutUnsafe();
        for (bodies) |body| {
            if (!zphy.isValidBodyPointer(body) or body.object_layer != object_layers.moving) continue;
            const sensor_contacts = demo.contact_listener.getSensorContacts(body);
            if (sensor_contacts.kinematic != std.math.maxInt(zphy.BodyId)) { // stirring force around gathering point
                const upward = zm.f32x4(0.0, 1.0, 0.0, 0.0);
                const tangent = zm.cross3(upward, zm.loadArr3(body.getPosition())) * zm.f32x4s(0.05);
                const force = zm.normalize3(upward + tangent) * zm.f32x4s(300000);
                body.addForce(.{ force[0], force[1], force[2] });
            } else if (sensor_contacts.static != std.math.maxInt(zphy.BodyId)) { // collection force
                const to_center = zm.loadArr3(demo.camera.collector_attractor) - zm.loadArr3(body.getPosition());
                const force = zm.normalize3(to_center) * zm.f32x4s(60000);
                body.addForce(.{ force[0], force[1], force[2] });
                const velocity = zm.loadArr3(body.getLinearVelocity());
                const speed = zm.length3(velocity);
                if ((speed > zm.f32x4s(6.0))[0]) {
                    const braking = velocity + velocity * speed * zm.f32x4s(-0.001);
                    body.setLinearVelocity(.{ braking[0], braking[1], braking[2] });
                }
            } else { // pulling force toward gathering point
                const to_center = zm.f32x4(0, 5, 0, 1) - zm.loadArr3(body.getPosition());
                const force = zm.normalize3(to_center) * zm.f32x4s(100000);
                body.addForce(.{ force[0], force[1], force[2] });
            }
        }
        demo.contact_listener.clearSensorContacts();
    }

    { // Handle camera movement with 'WASD' keys.
        demo.camera.rotation = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        const up = zm.f32x4(0.0, 1.0, 0.0, 0.0);
        const forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), demo.camera.rotation));
        const right = zm.normalize3(zm.cross3(up, forward));
        zm.storeArr3(&demo.camera.forward, forward);

        const speed = if (window.getKey(.left_shift) == .press) zm.f32x4s(150.0) else zm.f32x4s(25.0);
        var velocity = zm.f32x4s(0);
        if (window.getKey(.w) == .press) {
            velocity += forward;
        } else if (window.getKey(.s) == .press) {
            velocity -= forward;
        }
        if (window.getKey(.d) == .press) {
            velocity += right;
        } else if (window.getKey(.a) == .press) {
            velocity -= right;
        }
        if (window.getKey(.space) == .press) {
            velocity += up;
        } else if (window.getKey(.left_alt) == .press) {
            velocity -= up;
        }

        if ((zm.length3(velocity) > zm.f32x4s(0))[0]) velocity = zm.normalize3(velocity) * speed;
        if (demo.camera.out_of_body) {
            var cam_pos = zm.loadArr3(demo.camera.position);
            const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
            cam_pos += velocity * delta_time;
            zm.storeArr3(&demo.camera.position, cam_pos);
        } else {
            const body_interface = demo.physics_system.getBodyInterfaceMut();
            body_interface.setLinearVelocity(demo.camera.rigid_body, .{ velocity[0], velocity[1], velocity[2] });
        }
    }

    zgui.backend.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);
    demo.physics_debug_renderer.drawGui();

    const physics_step = @min(demo.gctx.stats.delta_time, 1.0 / 10.0);
    demo.physics_system.update(physics_step, .{}) catch unreachable;

    if (!demo.camera.out_of_body) {
        const body_const_interface = demo.physics_system.getBodyInterface();
        demo.camera.position = body_const_interface.getPosition(demo.camera.rigid_body);

        const center = zm.loadArr3(demo.camera.position) + zm.loadArr3(demo.camera.forward) * zm.f32x4s(4.0);
        zm.storeArr3(&demo.camera.collector_attractor, center + zm.loadArr3(demo.camera.forward) * zm.f32x4s(4.0));
        const body_interface = demo.physics_system.getBodyInterfaceMut();
        body_interface.setPosition(demo.camera.collector_body, .{ center[0], center[1], center[2] }, .activate);
        body_interface.setRotation(demo.camera.collector_body, zm.qmul(
            zm.quatFromNormAxisAngle(.{ 1, 0, 0, 0 }, -0.5 * math.pi),
            zm.quatFromMat(demo.camera.rotation),
        ), .activate);
    }
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const eye = zm.loadArr3(demo.camera.position);
    const up = zm.f32x4(0.0, 1.0, 0.0, 0.0);
    const az = zm.normalize3(zm.loadArr3(demo.camera.forward));
    const ax = zm.normalize3(zm.cross3(up, az));
    const ay = zm.normalize3(zm.cross3(az, ax));
    const cam_world_to_view = zm.Mat{
        zm.f32x4(ax[0], ax[1], ax[2], -zm.dot3(ax, eye)[0]),
        zm.f32x4(ay[0], ay[1], ay[2], -zm.dot3(ay, eye)[0]),
        zm.f32x4(az[0], az[1], az[2], -zm.dot3(az, eye)[0]),
        zm.f32x4(0.0, 0.0, 0.0, 1.0),
    };

    const near = 1.0;
    const fov_y = 0.33 * math.pi;
    const f = 1.0 / std.math.tan(fov_y / 2.0);
    const a: f32 = @as(f32, @floatFromInt(fb_width)) / @as(f32, @floatFromInt(fb_height));
    const cam_view_to_clip = zm.Mat{
        zm.f32x4(f / a, 0.0, 0.0, 0.0),
        zm.f32x4(0.0, f, 0.0, 0.0),
        zm.f32x4(0.0, 0.0, 0.0, near),
        zm.f32x4(0.0, 0.0, 1.0, 0.0),
    };

    // Lookup common resources which may be needed for all the passes.
    const depth_texv = gctx.lookupResource(demo.depth_texv) orelse return;
    const uniform_bg = gctx.lookupResource(demo.uniform_bg) orelse return;
    const vertex_buf_info = gctx.lookupResourceInfo(demo.vertex_buf) orelse return;
    const index_buf_info = gctx.lookupResourceInfo(demo.index_buf) orelse return;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        const frame_unif_mem = frame_unif_mem: {
            const mem = gctx.uniformsAllocate(FrameUniforms, 1);
            mem.slice[0] = .{
                .world_to_clip = zm.mul(cam_view_to_clip, cam_world_to_view),
                .floor_material = floor_material,
                .monolith_rotation = .{
                    demo.monolith.rotate_t[0],
                    demo.monolith.rotate_t[1],
                    demo.monolith.rotate_t[2],
                },
                .monolith_center = demo.monolith.center,
                .monolith_ray_radius = monolith_ray_radius,
                .monolith_inv_radius = .{
                    1.0 / monolith_ray_radius[0],
                    1.0 / monolith_ray_radius[1],
                    1.0 / monolith_ray_radius[2],
                    0,
                },
                .camera_position = demo.camera.position,
            };
            const bodies = demo.physics_system.getBodiesUnsafe();
            var body_count: usize = 0;
            for (bodies) |body| {
                if (!zphy.isValidBodyPointer(body) or body.object_layer != object_layers.moving) continue;
                mem.slice[0].lights[body_count] = if (zphy.Real == f32)
                    zm.loadArr4(body.position)
                else
                    zm.loadArr4(.{
                        @as(f32, @floatCast(body.position[0])),
                        @as(f32, @floatCast(body.position[1])),
                        @as(f32, @floatCast(body.position[2])),
                        @as(f32, @floatCast(body.position[3])),
                    });
                body_count += 1;
            }
            break :frame_unif_mem mem;
        };

        pass: {
            const mesh_render_pipe = gctx.lookupResource(demo.mesh_render_pipe) orelse break :pass;

            const pass = zgpu.beginRenderPassSimple(
                encoder,
                .clear,
                swapchain_texv,
                .{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 },
                depth_texv,
                0.0,
            );
            defer zgpu.endReleasePass(pass);

            pass.setVertexBuffer(0, vertex_buf_info.gpuobj.?, 0, vertex_buf_info.size);
            pass.setIndexBuffer(
                index_buf_info.gpuobj.?,
                if (Mesh.IndexType == u16) .uint16 else .uint32,
                0,
                index_buf_info.size,
            );
            pass.setPipeline(mesh_render_pipe);
            pass.setBindGroup(0, uniform_bg, &.{frame_unif_mem.offset});

            { // Draw monolith
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = demo.monolith.transform,
                    .basecolor_roughness = .{ 0.24, 0.24, 0.24, -0.04 },
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_cube].num_indices,
                    1,
                    demo.meshes.items[mesh_cube].index_offset,
                    demo.meshes.items[mesh_cube].vertex_offset,
                    0,
                );
            }

            { // Draw floor
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.identity(),
                    .basecolor_roughness = floor_material,
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_floor].num_indices,
                    1,
                    demo.meshes.items[mesh_floor].index_offset,
                    demo.meshes.items[mesh_floor].vertex_offset,
                    0,
                );
            }

            { // Draw skybox-analogue (lazy way to ensure whole frame runs fragment shader)
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{
                    .object_to_world = zm.scaling(-2000, -2000, -2000),
                    .basecolor_roughness = .{ 0.0, 0.0, 0.0, 1.0 },
                };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
                pass.drawIndexed(
                    demo.meshes.items[mesh_cube].num_indices,
                    1,
                    demo.meshes.items[mesh_cube].index_offset,
                    demo.meshes.items[mesh_cube].vertex_offset,
                    0,
                );
            }

            if (demo.physics_debug_enabled) demo.physics_debug_renderer.draw(pass);
        }

        {
            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.endReleasePass(pass);
            zgui.backend.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    tex: zgpu.TextureHandle,
    texv: zgpu.TextureViewHandle,
} {
    const tex = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .tdim_2d,
        .size = .{
            .width = gctx.swapchain_descriptor.width,
            .height = gctx.swapchain_descriptor.height,
            .depth_or_array_layers = 1,
        },
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const texv = gctx.createTextureView(tex, .{});
    return .{ .tex = tex, .texv = texv };
}

pub fn main() !void {
    try zglfw.init();
    defer zglfw.terminate();

    { // Change current working directory to where the executable is located.
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(1600, 1000, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try create(allocator, window);
    defer destroy(allocator, demo);

    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };

    zgui.init(allocator);
    defer zgui.deinit();

    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(16.0 * scale_factor));

    zgui.backend.init(
        window,
        demo.gctx.device,
        @intFromEnum(zgpu.GraphicsContext.swapchain_format),
        @intFromEnum(wgpu.TextureFormat.undef),
    );
    defer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();

        update(demo);
        draw(demo);

        if (demo.gctx.present() == .swap_chain_resized) {
            // Release old depth texture.
            demo.gctx.releaseResource(demo.depth_texv);
            demo.gctx.destroyResource(demo.depth_tex);

            // Create a new depth texture to match the new window size.
            const depth = createDepthTexture(demo.gctx);
            demo.depth_tex = depth.tex;
            demo.depth_texv = depth.texv;
        }
    }
}
