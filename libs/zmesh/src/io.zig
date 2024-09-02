const std = @import("std");
const assert = std.debug.assert;

const mem = @import("memory.zig");

/// Deprecated. Use `zmesh.io.zcgltf.parseAndLoadFile` instead.
pub const parseAndLoadFile = zcgltf.parseAndLoadFile;
/// Deprecated. Use `zmesh.io.zcgltf.freeData` instead.
pub const freeData = zcgltf.freeData;
/// Deprecated. Use `zmesh.io.zcgltf.appendMeshPrimitive` instead.
pub const appendMeshPrimitive = zcgltf.appendMeshPrimitive;

pub const zcgltf = struct {
    const bindings = @import("zcgltf.zig");
    const Data = bindings.Data;

    pub usingnamespace bindings;

    pub fn parseAndLoadFile(pathname: [:0]const u8) bindings.Error!*Data {
        const options = bindings.Options{
            .memory = .{
                .alloc_func = mem.zmeshAllocUser,
                .free_func = mem.zmeshFreeUser,
            },
        };

        const data = try bindings.parseFile(options, pathname);
        errdefer bindings.free(data);

        try bindings.loadBuffers(options, data, pathname);

        return data;
    }

    pub fn freeData(data: *Data) void {
        bindings.free(data);
    }

    pub fn appendMeshPrimitive(
        data: *Data,
        mesh_index: u32,
        prim_index: u32,
        indices: *std.ArrayList(u32),
        positions: *std.ArrayList([3]f32),
        normals: ?*std.ArrayList([3]f32),
        texcoords0: ?*std.ArrayList([2]f32),
        tangents: ?*std.ArrayList([4]f32),
    ) !void {
        assert(mesh_index < data.meshes_count);
        assert(prim_index < data.meshes.?[mesh_index].primitives_count);

        const mesh = &data.meshes.?[mesh_index];
        const prim = &mesh.primitives[prim_index];

        const num_vertices: u32 = @as(u32, @intCast(prim.attributes[0].data.count));
        const num_indices: u32 = @as(u32, @intCast(prim.indices.?.count));

        // Indices.
        {
            try indices.ensureTotalCapacity(indices.items.len + num_indices);

            const accessor = prim.indices.?;
            const buffer_view = accessor.buffer_view.?;

            assert(accessor.stride == buffer_view.stride or buffer_view.stride == 0);
            assert(accessor.stride * accessor.count == buffer_view.size);
            assert(buffer_view.buffer.data != null);

            const data_addr = @as([*]const u8, @ptrCast(buffer_view.buffer.data)) +
                accessor.offset + buffer_view.offset;

            if (accessor.stride == 1) {
                if (accessor.component_type != .r_8u) {
                    return error.InvalidIndicesAccessorComponentType;
                }
                const src = @as([*]const u8, @ptrCast(data_addr));
                var i: u32 = 0;
                while (i < num_indices) : (i += 1) {
                    indices.appendAssumeCapacity(src[i]);
                }
            } else if (accessor.stride == 2) {
                if (accessor.component_type != .r_16u) {
                    return error.InvalidIndicesAccessorComponentType;
                }
                const src = @as([*]const u16, @ptrCast(@alignCast(data_addr)));
                var i: u32 = 0;
                while (i < num_indices) : (i += 1) {
                    indices.appendAssumeCapacity(src[i]);
                }
            } else if (accessor.stride == 4) {
                if (accessor.component_type != .r_32u) {
                    return error.InvalidIndicesAccessorComponentType;
                }
                const src = @as([*]const u32, @ptrCast(@alignCast(data_addr)));
                var i: u32 = 0;
                while (i < num_indices) : (i += 1) {
                    indices.appendAssumeCapacity(src[i]);
                }
            } else {
                return error.InvalidIndicesAccessorStride;
            }
        }

        // Attributes.
        {
            const attributes = prim.attributes[0..prim.attributes_count];
            for (attributes) |attrib| {
                const accessor = attrib.data;
                assert(accessor.component_type == .r_32f);

                const buffer_view = accessor.buffer_view.?;
                assert(buffer_view.buffer.data != null);

                assert(accessor.stride == buffer_view.stride or buffer_view.stride == 0);
                assert(accessor.stride * accessor.count == buffer_view.size);

                const data_addr = @as([*]const u8, @ptrCast(buffer_view.buffer.data)) +
                    accessor.offset + buffer_view.offset;

                if (attrib.type == .position) {
                    assert(accessor.type == .vec3);
                    const slice = @as([*]const [3]f32, @ptrCast(@alignCast(data_addr)))[0..num_vertices];
                    try positions.appendSlice(slice);
                } else if (attrib.type == .normal) {
                    if (normals) |n| {
                        assert(accessor.type == .vec3);
                        const slice = @as([*]const [3]f32, @ptrCast(@alignCast(data_addr)))[0..num_vertices];
                        try n.appendSlice(slice);
                    }
                } else if (attrib.type == .texcoord) {
                    if (texcoords0) |tc| {
                        assert(accessor.type == .vec2);
                        const slice = @as([*]const [2]f32, @ptrCast(@alignCast(data_addr)))[0..num_vertices];
                        try tc.appendSlice(slice);
                    }
                } else if (attrib.type == .tangent) {
                    if (tangents) |tan| {
                        assert(accessor.type == .vec4);
                        const slice = @as([*]const [4]f32, @ptrCast(@alignCast(data_addr)))[0..num_vertices];
                        try tan.appendSlice(slice);
                    }
                }
            }
        }
    }
};
