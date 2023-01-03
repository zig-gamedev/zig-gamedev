# zstbtt v0.1.0

stb_truetype bindings with Zig allocator support

## Getting started

Copy `zstbtt` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:
```zig
const zstbtt = @import("libs/zstbtt/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zstbtt.pkg);

    zstbtt.link(exe);
}
```

Now in your code you may import and use `zstbtt`, first initilize the library with an allocator:
```zig
const zstbtt = @import("zstbtt");

zstbtt.init(allocator);
defer zstbtt.deinit();
```

TODO: basic usage example

See [stb_truetype.h](./src/stb_truetype.h) for the original API documentation.
