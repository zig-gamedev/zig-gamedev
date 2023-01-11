# zstbi v0.9.2 - stb image bindings

## Features

* Supports Zig memory allocators
* Supports decoding most popular formats
* Supports HDR images
* Supports 8-bits and 16-bits per channel
* Supports image resizing
* Supports image writing (.png, .jpg)

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
Now in your code you may import and use `zstbi`.

Init the lib. `zstbi.init()` is cheap and you may call it whenever you need to change memory allocator. Must be called from the main thread.
```zig
const zstbi = @import("zstbi");

zstbi.init(allocator);
defer zstbi.deinit();
```

Load image:
```zig
var image = try zstbi.Image.init("data/image.png", num_desired_channels);
defer image.deinit();
_ = image.data; // stored as []u8
_ = image.width;
_ = image.height;
_ = image.num_components;
_ = image.bytes_per_component;
_ = image.bytes_per_row;
_ = image.is_hdr;

const new_resized_image = image.resize(1024, 1024);
```

Get image info without loading:
```zig
const image_info = zstbi.Image.info("data/image.jpg");
_ = image_info.is_supported; // Is image format supported?
_ = image_info.width;
_ = image_info.height;
_ = image_info.num_components;
```
Misc functions:
```zig
pub fn isHdr(filename: [:0]const u8) bool
pub fn is16bit(filename: [:0]const u8) bool

pub fn setFlipVerticallyOnLoad(should_flip: bool) void
```
