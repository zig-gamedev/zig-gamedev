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
var image_u8 = try zstbi.Image(u8).init("path_to_image_file", num_desired_channels);
defer image_u8.deinit();

var image_f16 = try zstbi.Image(f16).init("path_to_image_file", num_desired_channels);
defer image_f16.deinit();

var image_f32 = try zstbi.Image(f32).init("path_to_image_file", num_desired_channels);
defer image_f32.deinit();
```
