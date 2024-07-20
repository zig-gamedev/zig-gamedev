# zemscripten
Zig build package and shims for [Emscripten](https://emscripten.org) emsdk

## How to use it

Add `zemscripten` and `emsdk` to your build.zig.zon dependencies:
```
        .zemscripten = .{ .path = "libs/zemscripten" },
        .emsdk = .{
            .url = "https://github.com/emscripten-core/emsdk/archive/refs/tags/3.1.52.tar.gz",
            .hash = "12202192726bf983ec243c7eea956d6107baf6f49d50b62f6a91f5d7471bc6daf53b",
        },
```

zemscripten uses sysroot to locate emsdk. Either specify it when calling `zig build --sysroot /path/to/local/emsdk` or use zemscripten's emsdk. Example build.zig code:
```zig
    const zemscripten = @import("zemscripten");

    // If user did not set --sysroot then default to zemscripten's emsdk path
    if (b.sysroot == null) {
        b.sysroot = zemscripten.getEmsdkSysroot(b);
        std.log.info("sysroot set to \"{s}\"", .{b.sysroot.?});

        const activateEmsdkStep = zemscripten.activateEmsdkStep(b);
        b.default_step.dependOn(activateEmsdkStep);
    }
```

Note that Emsdk must be activated before it can be used. If using the builtin emask, `activateEmsdkStep` can be used to create a build step that everything else should be dependent on.

TODO...