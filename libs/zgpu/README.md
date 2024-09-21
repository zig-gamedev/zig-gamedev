# zgpu v0.11.0 - Cross-platform graphics library

Small helper library built on top of [Dawn](https://github.com/zig-gamedev/dawn) native WebGPU implementation

Supports Windows 10+ (DirectX 12), macOS 12+ (Metal) and Linux (Vulkan).

## Features:

- Zero-overhead wgpu API bindings ([source code](https://github.com/michal-z/zig-gamedev/blob/main/libs/zgpu/src/wgpu.zig))
- Uniform buffer pool for fast CPU->GPU transfers
- Resource pools and handle-based GPU resources
- Async shader compilation
- GPU mipmap generator

For more details please see below.

## Getting started

Copy `zgpu`, `zpool` and `system-sdk` to a subdirectory of your project and add the following to your `build.zig.zon` dependencies:

```zig
.{
    .zgpu = .{ .path = "libs/zgpu" },
    .zpool = .{ .path = "libs/zpool" },
    .dawn_x86_64_windows_gnu = .{
        .url = "https://github.com/michal-z/webgpu_dawn-x86_64-windows-gnu/archive/d3a68014e6b6b53fd330a0ccba99e4dcfffddae5.tar.gz",
        .hash = "1220f9448cde02ef3cd51bde2e0850d4489daa0541571d748154e89c6eb46c76a267",
        .lazy = true,
    },
    .dawn_x86_64_linux_gnu = .{
        .url = "https://github.com/michal-z/webgpu_dawn-x86_64-linux-gnu/archive/7d70db023bf254546024629cbec5ee6113e12a42.tar.gz",
        .hash = "12204a3519efd49ea2d7cf63b544492a3a771d37eda320f86380813376801e4cfa73",
        .lazy = true,
    },
    .dawn_aarch64_linux_gnu = .{
        .url = "https://github.com/michal-z/webgpu_dawn-aarch64-linux-gnu/archive/c1f55e740a62f6942ff046e709ecd509a005dbeb.tar.gz",
        .hash = "12205cd13f6849f94ef7688ee88c6b74c7918a5dfb514f8a403fcc2929a0aa342627",
        .lazy = true,
    },
    .dawn_aarch64_macos = .{
        .url = "https://github.com/michal-z/webgpu_dawn-aarch64-macos/archive/d2360cdfff0cf4a780cb77aa47c57aca03cc6dfe.tar.gz",
        .hash = "12201fe677e9c7cfb8984a36446b329d5af23d03dc1e4f79a853399529e523a007fa",
        .lazy = true,
    },
    .dawn_x86_64_macos = .{
        .url = "https://github.com/michal-z/webgpu_dawn-x86_64-macos/archive/901716b10b31ce3e0d3fe479326b41e91d59c661.tar.gz",
        .hash = "1220b1f02f2f7edd98a078c64e3100907d90311d94880a3cc5927e1ac009d002667a",
        .lazy = true,
    },
}
```

then in your `build.zig`:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    @import("zgpu").addLibraryPathsTo(exe);

    const zgpu = b.dependency("zgpu", .{});
    exe.root_module.addImport("zgpu", zgpu.module("root"));
    exe.linkLibrary(zgpu.artifact("zdawn"));
}
```

---

## Sample applications

- [gui test (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/gui_test_wgpu)
- [physically based rendering (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu)
- [bullet physics test (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/bullet_physics_test_wgpu)
- [procedural mesh (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/procedural_mesh_wgpu)
- [textured quad (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/textured_quad_wgpu)
- [triangle (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/triangle_wgpu)

## Library overview

Below you can find an overview of main `zgpu` features.

### Compile-time options

You can override default options in your `build.zig`:

```zig
pub fn build(b: *std.Build) void {
    ...

    const zgpu = @import("zgpu").package(b, target, optimize, .{
        .options = .{
            .uniforms_buffer_size = 4 * 1024 * 1024,
            .dawn_skip_validation = false,
            .buffer_pool_size = 256,
            .texture_pool_size = 256,
            .texture_view_pool_size = 256,
            .sampler_pool_size = 16,
            .render_pipeline_pool_size = 128,
            .compute_pipeline_pool_size = 128,
            .bind_group_pool_size = 32,
            .bind_group_layout_pool_size = 32,
            .pipeline_layout_pool_size = 32,
        },
    });

    zgpu.link(exe);

    ...
}
```

### Graphics Context

Create a `GraphicsContext` using a `WindowProvider`. For example, using [zglfw](https://github.com/zig-gamedev):

```zig
const gctx = try zgpu.GraphicsContext.create(
    allocator,
    .{
        .window = window,
        .fn_getTime = @ptrCast(&zglfw.getTime),
        .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),

        // optional fields
        .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
        .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
        .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
        .fn_getWaylandDisplay = @ptrCast(&zglfw.getWaylandDisplay),
        .fn_getWaylandSurface = @ptrCast(&zglfw.getWaylandWindow),
        .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
    },
    .{}, // default context creation options
);
```

### Uniforms

- Implemented as a uniform buffer pool
- Easy to use
- Efficient - only one copy operation per frame

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

- Every GPU resource is identified by 32-bit integer handle
- All resources are stored in one system
- We keep basic info about each resource (size of the buffer, format of the texture, etc.)
- You can always check if resource is valid (very useful for async operations)
- System keeps basic info about resource dependencies, for example, `TextureViewHandle` knows about its
  parent texture and becomes invalid when parent texture becomes invalid; `BindGroupHandle` knows
  about all resources it binds so it becomes invalid if any of those resources become invalid

```zig
const buffer_handle = gctx.createBuffer(...);

if (gctx.isResourceValid(buffer_handle)) {
    const buffer = gctx.lookupResource(buffer_handle).?;  // Returns `wgpu.Buffer`

    const buffer_info = gctx.lookupResourceInfo(buffer_handle).?; // Returns `zgpu.BufferInfo`
    std.debug.print("Buffer size is: {d}", .{buffer_info.size});
}

// If you want to destroy a resource before shutting down graphics context:
gctx.destroyResource(buffer_handle);

```

### Async shader compilation

- Thanks to resource pools and resources identified by handles we can easily async compile all our shaders

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

- wgpu API does not provide mipmap generator
- zgpu provides decent mipmap generator implemented in a compute shader
- It supports 2D textures, array textures and cubemap textures of any format
  (`rgba8_unorm`, `rg16_float`, `rgba32_float`, etc.)
- Currently it requires that: `texture_width == texture_height and isPowerOfTwo(texture_width)`
- It takes ~260 microsec to generate all mips for 1024x1024 `rgba8_unorm` texture on GTX 1660

```zig
// Usage:
gctx.generateMipmaps(arena, command_encoder, texture_handle);
```
