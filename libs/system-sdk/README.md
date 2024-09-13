# [zig-gamedev system-sdk](https://github.com/zig-gamedev/zig-gamedev/tree/main/libs/system-sdk)

System libraries and headers for cross-compiling [zig-gamedev libs](https://github.com/zig-gamedev/zig-gamedev#libraries)

## Usage
build.zig
```zig
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                if (target.abi.isGnu() or target.abi.isMusl()) {
                    const system_sdk = b.lazyDependency("system_sdk", .{}).?;
                    compile_step.addLibraryPath(system_sdk.path("windows/lib/x86_64-windows-gnu"));
                }
            }
        },
        .macos => {
            const system_sdk = b.lazyDependency("system_sdk", .{}).?;
            compile_step.addLibraryPath(system_sdk.path("macos12/usr/lib"));
            compile_step.addFrameworkPath(system_sdk.path("macos12/System/Library/Frameworks"));
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                const system_sdk = b.lazyDependency("system_sdk", .{}).?;
                compile_step.addLibraryPath(system_sdk.path("linux/lib/x86_64-linux-gnu"));
            } else if (target.cpu.arch == .aarch64) {
                const system_sdk = b.lazyDependency("system_sdk", .{}).?;
                compile_step.addLibraryPath(system_sdk.path("linux/lib/aarch64-linux-gnu"));
            }
        },
        else => {},
    }
```
