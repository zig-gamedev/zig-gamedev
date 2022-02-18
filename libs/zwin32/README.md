## Getting started

Copy `win32` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const win32 = std.build.Pkg{
        .name = "win32",
        .path = .{ .path = "libs/win32/win32.zig" },
    };
    exe.addPackage(win32);
}
```

Now in your code you may import and use win32:

```zig
const win32 = @import("win32");
const wbase = win32.base;
const dwrite = win32.dwrite;
const dxgi = win32.dxgi;
const d3d12 = win32.d3d12;
const d3d12d = win32.d3d12d;
const dml = win32.directml;

pub fn main() !void {
    ...
    const simple_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Normal", 0, .R32G32B32_FLOAT, 0, 12, .PER_VERTEX_DATA, 0),
        };

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            "content/shaders/intro3.vs.cso",
            "content/shaders/intro3.ps.cso",
        );
    };
}
```
