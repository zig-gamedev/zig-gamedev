## procedural mesh (wgpu)

![image](screenshot.png)

* This sample shows how to efficiently draw several procedurally generated meshes
* All vertices and indices are stored in one large vertex/index buffer
* Simple physically-based shading is used
* Single-pass wireframe rendering
* Works on Windows/Linux/Mac

Main drawing loop is optimized and changes just one dynamic offset before each draw call:

```zig
pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
pass.setIndexBuffer(ib_info.gpuobj.?, .uint16, 0, ib_info.size);

pass.setPipeline(pipeline);

// Update "world to clip" (camera) xform.
{
    const mem = gctx.uniformsAllocate(FrameUniforms, 1);
    mem.slice[0].world_to_clip = zm.transpose(cam_world_to_clip);
    mem.slice[0].camera_position = demo.camera.position;

    pass.setBindGroup(0, bind_group, &.{mem.offset});
}

for (demo.drawables.items) |drawable| {
    // Update "object to world" xform.
    const object_to_world = zm.translationV(zm.load(drawable.position[0..], zm.Vec, 3));

    const mem = gctx.uniformsAllocate(DrawUniforms, 1);
    mem.slice[0].object_to_world = zm.transpose(object_to_world);
    mem.slice[0].basecolor_roughness = drawable.basecolor_roughness;

    pass.setBindGroup(1, bind_group, &.{mem.offset});

    // Draw.
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
