# zstbi - stbi bindings

## Getting started

Copy `zstbi` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:
```zig
const zstbi = @import("libs/zstbi/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zstbi.pkg);

    zstbi.link(exe);
}
```
Now in your code you may import and use `zstbi`:
```zig
var image = try zgpu.stbi.Image(u8).init("path_to_image_file", num_desired_channels);
defer image.deinit();
```
