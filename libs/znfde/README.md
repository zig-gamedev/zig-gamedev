# znfde v0.1.0 - NativeFileDialog extended binding

Easy to use zig binding for [Native file dialog - extended](https://github.com/btzy/nativefiledialog-extended).
On linux is using xdg-desktop-portal instead of GTK by default.

## Getting started

Copy `znfde` to a subdirectory in your project and add the following to your `build.zig.zon` .dependencies:

```zig
    .znfde = .{ .path = "libs/znfde" },
```

Then in your `build.zig` add:
```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const znfde = b.dependency("znfde", .{
        .target = target,
    });

    exe.root_module.addImport("znfde", znfde.module("root"));
    exe.linkLibrary(znfde.artifact("nfde"));
}
```

Now in your code you may import and use znfde:

```zig
const znfde = @import("znfde");

pub fn main() !void {
    try znfde.init();
    defer znfde.deinit();

    ...
    {
        const path = try znfde.openFileDialog(allocator, &.{.{ .name = "Text file", .spec = "txt" }}, null);
        defer allocator.free(path);
    }

    {
        const path = try znfde.saveFileDialog(allocator, &.{.{ .name = "Text file", .spec = "txt" }}, null);
        defer allocator.free(path);
    }

    {
        const path = try znfde.openFolderDialog(allocator, null);
        defer allocator.free(path);
    }
}
```
