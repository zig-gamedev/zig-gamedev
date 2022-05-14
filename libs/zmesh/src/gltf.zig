const std = @import("std");
const assert = std.debug.assert;
const cgltf = @import("zcgltf.zig");
const mem = @import("memory.zig");
pub usingnamespace cgltf;

pub fn parseAndLoadFile(gltf_path: [:0]const u8) cgltf.Error!*cgltf.Data {
    const options = cgltf.Options{
        .memory = .{
            .alloc = mem.zmeshAllocUser,
            .free = mem.zmeshFreeUser,
        },
    };

    const data = try cgltf.parseFile(options, gltf_path);
    errdefer cgltf.free(data);

    try cgltf.loadBuffers(options, data, gltf_path);

    return data;
}

pub fn appendMeshPrimitive(
    data: *cgltf.Data,
    mesh_index: u32,
    prim_index: u32,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList([3]f32),
    normals: ?*std.ArrayList([3]f32),
    texcoords0: ?*std.ArrayList([2]f32),
    tangents: ?*std.ArrayList([4]f32),
) void {
    assert(mesh_index < data.meshes_count);
    assert(prim_index < data.meshes.?[mesh_index].primitives_count);

    const num_vertices: u32 = @intCast(
        u32,
        data.meshes.?[mesh_index].primitives[prim_index].attributes[0].data.count,
    );
    const num_indices: u32 = @intCast(u32, data.meshes.?[mesh_index].primitives[prim_index].indices.?.count);

    // Indices.
    {
        indices.ensureTotalCapacity(indices.items.len + num_indices) catch unreachable;

        const accessor = data.meshes.?[mesh_index].primitives[prim_index].indices.?;

        assert(accessor.stride == accessor.buffer_view.?.stride or accessor.buffer_view.?.stride == 0);
        assert((accessor.stride * accessor.count) == accessor.buffer_view.?.size);
        assert(accessor.buffer_view.?.buffer.data != null);

        const data_addr = @alignCast(4, @ptrCast([*]const u8, accessor.buffer_view.?.buffer.data) +
            accessor.offset + accessor.buffer_view.?.offset);

        if (accessor.stride == 1) {
            assert(accessor.component_type == .r_8u);
            const src = @ptrCast([*]const u8, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.stride == 2) {
            assert(accessor.component_type == .r_16u);
            const src = @ptrCast([*]const u16, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.stride == 4) {
            assert(accessor.component_type == .r_32u);
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
        positions.resize(positions.items.len + num_vertices) catch unreachable;
        if (normals != null) normals.?.resize(normals.?.items.len + num_vertices) catch unreachable;
        if (texcoords0 != null) texcoords0.?.resize(texcoords0.?.items.len + num_vertices) catch unreachable;
        if (tangents != null) tangents.?.resize(tangents.?.items.len + num_vertices) catch unreachable;

        const num_attribs: u32 = @intCast(u32, data.meshes.?[mesh_index].primitives[prim_index].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.meshes.?[mesh_index].primitives[prim_index].attributes[attrib_index];
            const accessor = attrib.data;

            assert(accessor.stride == accessor.buffer_view.?.stride or accessor.buffer_view.?.stride == 0);
            assert((accessor.stride * accessor.count) == accessor.buffer_view.?.size);
            assert(accessor.buffer_view.?.buffer.data != null);

            const data_addr = @ptrCast([*]const u8, accessor.buffer_view.?.buffer.data) +
                accessor.offset + accessor.buffer_view.?.offset;

            if (attrib.type == .position) {
                assert(accessor.type == .vec3);
                assert(accessor.component_type == .r_32f);
                @memcpy(
                    @ptrCast([*]u8, &positions.items[positions.items.len - num_vertices]),
                    data_addr,
                    accessor.count * accessor.stride,
                );
            } else if (attrib.type == .normal and normals != null) {
                assert(accessor.type == .vec3);
                assert(accessor.component_type == .r_32f);
                @memcpy(
                    @ptrCast([*]u8, &normals.?.items[normals.?.items.len - num_vertices]),
                    data_addr,
                    accessor.count * accessor.stride,
                );
            } else if (attrib.type == .texcoord and texcoords0 != null) {
                assert(accessor.type == .vec2);
                assert(accessor.component_type == .r_32f);
                @memcpy(
                    @ptrCast([*]u8, &texcoords0.?.items[texcoords0.?.items.len - num_vertices]),
                    data_addr,
                    accessor.count * accessor.stride,
                );
            } else if (attrib.type == .tangent and tangents != null) {
                assert(accessor.type == .vec4);
                assert(accessor.component_type == .r_32f);
                @memcpy(
                    @ptrCast([*]u8, &tangents.?.items[tangents.?.items.len - num_vertices]),
                    data_addr,
                    accessor.count * accessor.stride,
                );
            }
        }
    }
}
