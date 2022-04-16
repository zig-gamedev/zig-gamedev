## procedural mesh wgpu

* This WebGPU sample shows how to efficiently draw several procedurally generated meshes
* All vertices and indices are stored in one large vertex/index buffer
* Two bind groups are used - frame bind group and draw bind group
* Simple physically-based shading is used
* Single-pass wireframe rendering (hacky)

Main drawing loop is optimized and changes just one dynamic offset before each draw call:

```zig
pass.setVertexBuffer(0, demo.vertex_buffer, 0, demo.total_num_vertices * @sizeOf(Vertex));
pass.setIndexBuffer(demo.index_buffer, .uint16, 0, demo.total_num_indices * @sizeOf(u16));

pass.setPipeline(demo.pipeline);
pass.setBindGroup(1, demo.frame_bind_group, &.{});

for (demo.drawables.items) |drawable, drawable_index| {
    pass.setBindGroup(0, demo.draw_bind_group, &.{@intCast(u32, drawable_index * 256)});
    pass.drawIndexed(
        demo.meshes.items[drawable.mesh_index].num_indices,
        1,
        demo.meshes.items[drawable.mesh_index].index_offset,
        demo.meshes.items[drawable.mesh_index].vertex_offset,
        0,
    );
}
```

Used libraries:
* zgpu
* zmath
* zmesh
* znoise

![image](screenshot.png)
