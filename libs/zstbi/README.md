# zstbi v0.9.3 - stb image bindings

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

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zstbi_pkg = zstbi.package(b, target, optimize, .{});

    zstbi_pkg.link(exe);
}
```
Now in your code you may import and use `zstbi`.

Init the lib. `zstbi.init()` is cheap and you may call it whenever you need to change memory allocator. Must be called from the main thread.
```zig
const zstbi = @import("zstbi");

zstbi.init(allocator);
defer zstbi.deinit();
```
```zig
pub const Image = struct {
    data: []u8,
    width: u32,
    height: u32,
    num_components: u32,
    bytes_per_component: u32,
    bytes_per_row: u32,
    is_hdr: bool,
    ...
```
```zig
pub fn loadFromFile(pathname: [:0]const u8, forced_num_components: u32) !Image

pub fn loadFromMemory(data: []const u8, forced_num_components: u32) !Image

pub fn createEmpty(width: u32, height: u32, num_components: u32, args: struct {
    bytes_per_component: u32 = 0,
    bytes_per_row: u32 = 0,
}) !Image

pub fn info(pathname: [:0]const u8) struct {
    is_supported: bool,
    width: u32,
    height: u32,
    num_components: u32,
}

pub fn resize(image: *const Image, new_width: u32, new_height: u32) Image

pub fn writeToFile(
    image: *const Image,
    filename: [:0]const u8,
    image_format: ImageWriteFormat,
) ImageWriteError!void

pub fn writeToFn(
    image: *const Image,
    write_fn: *const fn (ctx: ?*anyopaque, data: ?*anyopaque, size: c_int) callconv(.C) void,
    context: ?*anyopaque,
    image_format: ImageWriteFormat,
) ImageWriteError!void
```
```zig
var image = try zstbi.Image.loadFromFile("data/image.png", forced_num_components);
defer image.deinit();

const new_resized_image = image.resize(1024, 1024);
```
Misc functions:
```zig
pub fn isHdr(filename: [:0]const u8) bool
pub fn is16bit(filename: [:0]const u8) bool

pub fn setFlipVerticallyOnLoad(should_flip: bool) void
```
