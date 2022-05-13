# zgpu - version 0.1

WebGPU bindings taken from: https://github.com/hexops/mach/tree/main/gpu

zgpu is a helper library for working with native WebGPU API.
Below you can find an overview of its main features.

1. Init
```
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    // When you are done:
    gctx.deinit(allocator);
```
2. Uniforms

    * Implemented as a uniform buffer pool
    * Easy to use
    * Efficient - only one copy operation per frame
```
    struct DrawUniforms = extern struct {
        object_to_world: zm.Mat,
    };
    const mem = gctx.uniformsAllocate(DrawUniforms, 1);
    mem.slice[0] = .{ .object_to_world = zm.transpose(zm.translation(...)) };

    pass.setBindGroup(0, bind_group, &.{mem.offset});
    pass.drawIndexed(...);

    // When you are done encoding all commands for a frame:
    gctx.submit(...); // Injects *one* copy operation to transfer *all* allocated uniforms
```
3. Resource pools

    * Every GPU resource is identified by 32-bit integer handle
    * All resources are stored in one system
    * We keep basic info about each resource (size of the buffer, format of the texture, etc.)
    * You can always check if resource is valid (very useful for async operations)
    * System keeps basic info about resource dependencies, for example, TextureViewHandle knows about its
      parent texture and becomes invalid when parent texture becomes invalid; BindGroupHandle knows
      about all resources it binds so it becomes invalid if any bounded resource become invalid
```
    const buffer_handle = gctx.createBuffer(...);
    if (gctx.isResourceValid(buffer_handle)) {
    const buffer = gctx.lookupResource(buffer_handle).?;  // Returns gpu.Buffer
    const buffer_info = gctx.lookupResourceInfo(buffer_handle).?; // Returns zgpu.BufferInfo
        std.debug.print("Buffer size is: {d}", .{buffer_info.size});
    }
    // If you want to destroy a resource before shutting down graphics context:
    gctx.destroyResource(buffer_handle);
```
4. Async shader compilation

    * Thanks to resource pools and resources identified by handles we can easily async compile
       all our shaders
```
    const DemoState = struct {
        pipeline: zgpu.PipelineLayoutHandle = .{},
        ...
    };
    const demo = try allocator.create(DemoState);
    // Below call schedules pipeline compilation and returns immediately. When compilation is complete
    // valid pipeline handle will be stored in `demo.pipeline`.
    gctx.createRenderPipelineAsync(allocator, pipeline_layout, pipeline_descriptor, &demo.pipeline);

    // Pass using our pipeline will be skipped until compilation is ready
    pass: {
        const pipeline = gctx.lookupResource(demo.pipeline) orelse break :pass;
        ...
    }
```
