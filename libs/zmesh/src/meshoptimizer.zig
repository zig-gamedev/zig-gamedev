const std = @import("std");
const assert = std.debug.assert;

pub inline fn generateVertexRemap(
    destination: []u32,
    indices: []const u32,
    comptime T: type,
    vertices: []const T,
) usize {
    assert(destination.len >= indices.len);
    return meshopt_generateVertexRemap(
        destination.ptr,
        indices.ptr,
        indices.len,
        vertices.ptr,
        vertices.len,
        @sizeOf(T),
    );
}

pub inline fn remapVertexBuffer(
    comptime T: type,
    destination: []T,
    vertices: []const T,
    remap: []const u32,
) void {
    assert(destination.len >= vertices.len);
    meshopt_remapVertexBuffer(
        destination.ptr,
        vertices.ptr,
        vertices.len,
        @sizeOf(T),
        remap.ptr,
    );
}

pub inline fn remapIndexBuffer(
    destination: []u32,
    indices: []const u32,
    remap: []const u32,
) void {
    assert(destination.len >= indices.len);
    meshopt_remapIndexBuffer(destination.ptr, indices.ptr, indices.len, remap.ptr);
}

pub inline fn optimizeVertexCache(
    destination: []u32,
    indices: []const u32,
    vertex_count: usize,
) void {
    assert(destination.len >= indices.len);
    meshopt_optimizeVertexCache(
        destination.ptr,
        indices.ptr,
        indices.len,
        vertex_count,
    );
}

pub inline fn optimizeVertexFetch(
    comptime T: type,
    destination: []T,
    indices: []u32,
    vertices: []const T,
) usize {
    assert(destination.len >= vertices.len);
    return meshopt_optimizeVertexFetch(
        destination.ptr,
        indices.ptr,
        indices.len,
        vertices.ptr,
        vertices.len,
        @sizeOf(T),
    );
}

pub inline fn buildMeshletsBound(
    index_count: usize,
    max_vertices: usize,
    max_triangles: usize,
) usize {
    return meshopt_buildMeshletsBound(index_count, max_vertices, max_triangles);
}

pub const Meshlet = extern struct {
    vertex_offset: u32,
    triangle_offset: u32,
    vertex_count: u32,
    triangle_count: u32,
};

pub inline fn buildMeshlets(
    meshlets: []Meshlet,
    meshlet_vertices: []u32,
    meshlet_triangles: []u8,
    indices: []const u32,
    comptime T: type,
    vertices: []const T,
    max_vertices: usize,
    max_triangles: usize,
    cone_weight: f32,
) usize {
    return meshopt_buildMeshlets(
        meshlets.ptr,
        meshlet_vertices.ptr,
        meshlet_triangles.ptr,
        indices.ptr,
        indices.len,
        vertices.ptr,
        vertices.len,
        @sizeOf(T),
        max_vertices,
        max_triangles,
        cone_weight,
    );
}

extern fn meshopt_generateVertexRemap(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
) usize;

extern fn meshopt_remapVertexBuffer(
    destination: *anyopaque,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
    remap: [*]const u32,
) void;

extern fn meshopt_remapIndexBuffer(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    remap: [*]const u32,
) void;

extern fn meshopt_optimizeVertexCache(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertex_count: usize,
) void;

extern fn meshopt_optimizeVertexFetch(
    destination: *anyopaque,
    indices: [*]u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
) usize;

extern fn meshopt_buildMeshletsBound(
    index_count: usize,
    max_vertices: usize,
    max_triangles: usize,
) usize;

extern fn meshopt_buildMeshlets(
    meshlets: [*]Meshlet,
    meshlet_vertices: [*]u32,
    meshlet_triangles: [*]u8,
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
    max_vertices: usize,
    max_triangles: usize,
    cone_weight: f32,
) usize;
