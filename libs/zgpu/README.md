# zgpu v0.1 - Cross-platform graphics layer

This library uses GLFW and WebGPU bindings + great build script from [mach/gpu](https://github.com/hexops/mach/tree/main/gpu) project.

`zgpu` is a cross-platform (Windows/Linux/MacOS) graphics layer built on top of native WebGPU API (Dawn).

## Features:

* Uniform buffer pool for fast CPU->GPU transfers
* Resource pools and resources identified by 32-bit integer handles
* Async shader compilation
* GPU mipmap generator
* Image loading via `stb_image` library (optional)
* GUI via `dear imgui` library (optional)

For more details please see below.

## Getting started

Copy `zgpu`, `mach-glfw` and `mach-gpu-dawn` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const glfw = @import("libs/mach-glfw/build.zig");
const zgpu = @import("libs/zgpu/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(glfw.pkg);
    exe.addPackage(zgpu.pkg);

    zgpu.link(exe, .{
        .glfw_options = .{},
        .gpu_dawn_options = .{ .from_source = false },
    });
}
```

For sample applications please see:
* [triangle (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/triangle_wgpu)
* [procedural mesh (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/procedural_mesh_wgpu)
* [textured quad (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/textured_quad_wgpu)

## Library overview

Below you can find an overview of main `zgpu` features.

### Init
```zig
const gctx = try zgpu.GraphicsContext.init(allocator, window);

// When you are done:
gctx.deinit(allocator);
```
### Uniforms

* Implemented as a uniform buffer pool
* Easy to use
* Efficient - only one copy operation per frame

```zig
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

### Resource pools

* Every GPU resource is identified by 32-bit integer handle
* All resources are stored in one system
* We keep basic info about each resource (size of the buffer, format of the texture, etc.)
* You can always check if resource is valid (very useful for async operations)
* System keeps basic info about resource dependencies, for example, `TextureViewHandle` knows about its
parent texture and becomes invalid when parent texture becomes invalid; `BindGroupHandle` knows
about all resources it binds so it becomes invalid if any of those resources become invalid

```zig
const buffer_handle = gctx.createBuffer(...);

if (gctx.isResourceValid(buffer_handle)) {
    const buffer = gctx.lookupResource(buffer_handle).?;  // Returns gpu.Buffer

    const buffer_info = gctx.lookupResourceInfo(buffer_handle).?; // Returns zgpu.BufferInfo
    std.debug.print("Buffer size is: {d}", .{buffer_info.size});
}

// If you want to destroy a resource before shutting down graphics context:
gctx.destroyResource(buffer_handle);

```
### Async shader compilation

* Thanks to resource pools and resources identified by handles we can easily async compile all our shaders

```zig
const DemoState = struct {
    pipeline_handle: zgpu.PipelineLayoutHandle = .{},
    ...
};
const demo = try allocator.create(DemoState);

// Below call schedules pipeline compilation and returns immediately. When compilation is complete
// valid pipeline handle will be stored in `demo.pipeline_handle`.
gctx.createRenderPipelineAsync(allocator, pipeline_layout, pipeline_descriptor, &demo.pipeline_handle);

// Pass using our pipeline will be skipped until compilation is ready
pass: {
    const pipeline = gctx.lookupResource(demo.pipeline_handle) orelse break :pass;
    ...

    pass.setPipeline(pipeline);
    pass.drawIndexed(...);
}
```

### Mipmap generation on the GPU

* WebGPU API does not provide mipmap generator
* zgpu provides decent mipmap generator implemented in a compute shader
* It supports 2D textures, array textures and cubemap textures of any format
(`rgba8_unorm`, `rg16_float`, `rgba32_float`, etc.)
* Currently it requires that: `texture_width == texture_height and isPowerOfTwo(texture_width)`
* It takes ~260 us to generate all mips for 1024x1024 `rgba8_unorm` texture on GTX 1660

```zig
// Usage:
gctx.generateMipmaps(arena, command_encoder, texture_handle);
```

### Image loading with `stb_image` library (optional)

```zig
// Defined in zgpu.stbi namespace
pub fn Image(comptime ChannelType: type) type {
    return struct {
        const Self = @This();

        data: []ChannelType, // ChannelType can be `u8`, `f16` or `f32`
        width: u32,
        height: u32,
        channels_in_memory: u32,
        channels_in_file: u32,
        ...

// Usage:
var image = try zgpu.stbi.Image(u8).init("path_to_image_file", num_desired_channels);
defer image.deinit();
```

If you don't want to use `stb_image` library you can disable it in `build.zig`.

### GUI based on `dear imgui` library (optional)

```zig
zgpu.gui.init(window, gpu_device, "path_to_content_dir", font_name, font_size);
defer zgpu.gui.deinit();

// Main loop
while (...) {
    zgpu.gui.newFrame(framebuffer_width, framebuffer_height);
    // Define your widgets here...
    // ...

    // Draw
    {
        // Begin render pass with only one color attachment and *without depth-stencil* attachment
        const pass = encoder.beginRenderPass(...);
        defer {
            pass.end();
            pass.release();
        }
        zgpu.gui.draw(pass);
    }
}
```

If you don't want to use `dear imgui` library you can disable it in `build.zig`.
