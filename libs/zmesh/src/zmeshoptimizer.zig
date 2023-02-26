// For the documentation see: ../libs/meshoptimizer/meshoptimizer.h

const std = @import("std");
const assert = std.debug.assert;

// Indexing
pub inline fn generateVertexRemap(
    destination: []u32,
    indices: ?[]const u32,
    comptime T: type,
    vertices: []const T,
) usize {
    return meshopt_generateVertexRemap(
        destination.ptr,
        if (indices) |ind| ind.ptr else null,
        if (indices) |ind| ind.len else vertices.len,
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
    indices: ?[]const u32,
    remap: []const u32,
) void {
    meshopt_remapIndexBuffer(
        destination.ptr,
        if (indices) |ind| ind.ptr else null,
        if (indices) |ind| ind.len else remap.len,
        remap.ptr,
    );
}

// Vertex cache optimization
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

pub const VertexCacheStatistics = extern struct {
    vertices_transformed: u32,
    warps_executed: u32,
    acmr: f32,
    atvr: f32,
};

pub inline fn analyzeVertexCache(
    indices: []const u32,
    vertex_count: usize,
    cache_size: u32,
    warp_size: u32,
    primgroup_size: u32,
) VertexCacheStatistics {
    return meshopt_analyzeVertexCache(
        indices.ptr,
        indices.len,
        vertex_count,
        cache_size,
        warp_size,
        primgroup_size,
    );
}

// Overdraw optimization
pub inline fn optimizeOverdraw(
    destination: []u32,
    indices: []const u32,
    comptime T: type,
    vertices: []const T,
    threshold: f32,
) void {
    assert(destination.len >= indices.len);
    meshopt_optimizeOverdraw(
        destination.ptr,
        indices.ptr,
        indices.len,
        vertices.ptr,
        vertices.len,
        @sizeOf(T),
        threshold,
    );
}

pub const OverdrawStatistics = extern struct {
    pixels_covered: u32,
    pixels_shaded: u32,
    overdraw: f32,
};

pub inline fn analyzeOverdraw(
    indices: []const u32,
    comptime T: type,
    vertices: []const T,
) OverdrawStatistics {
    return meshopt_analyzeOverdraw(indices.ptr, indices.len, vertices.ptr, vertices.len, @sizeOf(T));
}

// Vertex fetch optimization
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

pub const VertexFetchStatistics = extern struct {
    bytes_fetched: u32,
    overfetch: f32,
};

pub inline fn analyzeVertexFetch(
    indices: []const u32,
    vertex_count: usize,
    vertex_size: usize,
) VertexFetchStatistics {
    return meshopt_analyzeVertexFetch(indices.ptr, indices.len, vertex_count, vertex_size);
}

// Simplifier
pub inline fn simplify(
    comptime T: type,
    destination: []u32,
    indices: []const u32,
    index_count: usize,
    vertices: []const T,
    vertex_count: usize,
    target_index_count: usize,
    target_error: f32,
    options: u32,
    out_result_error: *f32,
) usize {
    return meshopt_simplify(
        destination.ptr,
        indices.ptr,
        index_count,
        vertices.ptr,
        vertex_count,
        @sizeOf(T),
        target_index_count,
        target_error,
        options,
        out_result_error,
    );
}

pub inline fn simplifySloppy(
    comptime T: type,
    destination: []u32,
    indices: []const u32,
    index_count: usize,
    vertices: []const T,
    vertex_count: usize,
    target_index_count: usize,
    target_error: f32,
    out_result_error: *f32,
) usize {
    return meshopt_simplifySloppy(
        destination.ptr,
        indices.ptr,
        index_count,
        vertices.ptr,
        vertex_count,
        @sizeOf(T),
        target_index_count,
        target_error,
        out_result_error,
    );
}

// Mesh shading
pub inline fn buildMeshletsBound(index_count: usize, max_vertices: usize, max_triangles: usize) usize {
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
    indices: ?[*]const u32,
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
    indices: ?[*]const u32,
    index_count: usize,
    remap: [*]const u32,
) void;
extern fn meshopt_optimizeVertexCache(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertex_count: usize,
) void;
extern fn meshopt_analyzeVertexCache(
    indices: [*]const u32,
    index_count: usize,
    vertex_count: usize,
    cache_size: u32,
    warp_size: u32,
    primgroup_size: u32,
) VertexCacheStatistics;
extern fn meshopt_optimizeOverdraw(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
    threshold: f32,
) void;
extern fn meshopt_analyzeOverdraw(
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
) OverdrawStatistics;
extern fn meshopt_optimizeVertexFetch(
    destination: *anyopaque,
    indices: [*]u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_size: usize,
) usize;
extern fn meshopt_analyzeVertexFetch(
    indices: [*]const u32,
    index_count: usize,
    vertex_count: usize,
    vertex_size: usize,
) VertexFetchStatistics;
extern fn meshopt_simplify(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_stride: usize,
    target_index_count: usize,
    target_error: f32,
    options: u32,
    out_result_error: *f32,
) usize;
extern fn meshopt_simplifySloppy(
    destination: [*]u32,
    indices: [*]const u32,
    index_count: usize,
    vertices: *const anyopaque,
    vertex_count: usize,
    vertex_stride: usize,
    target_index_count: usize,
    target_error: f32,
    out_result_error: *f32,
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
