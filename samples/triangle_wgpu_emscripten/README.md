## triangle (wgpu + emscripten)

Same sample as `triangle_wgpu` but with added emscripten sdk support to compile for web. Still can be compiled natively.

### Build
#### Native
```bash
zig build triangle_wgpu_emscripten-run
```
#### Wasm for web
#### Install emscripten sdk
* [Follow these instructions](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions-using-the-emsdk-recommended)
* emscripten needs this [patch](https://github.com/emscripten-core/emscripten/pull/19477/commits/f4bb4f578131578cd13abbbf78d7f4273788d76f) currently for this sample to run until it gets merged/released.

#### Compile
```bash
zig build triangle_wgpu_emscripten -Demscripten -Dtarget=wasm32-freestanding
```
See output under `zig-out\www\triangle_wgpu_emscripten`. It should contain:
* index.html
* index.js
* index.wasm
* (optional) `index.wasm.map` source mappings. If setup correctly with webserver it should allow debugging including previewing zig files, placing breakpoints and stepping through zig code from browser.

To run it in browser it is required that the files are served by web-server.  
Many IDEs have extensions for serving files, I used vscode extensions `liveServer` that worked.  
Open hosted site with browser that supports WebGPU, tested to work on:
* [chrome canary](https://www.google.com/chrome/canary/) ✔️
* Edge DEV ✔️

Didn't work on:

* Firefox DEV ❌ (could not run any webgpu sample to run, seems to be my system or browser issue)

![image](screenshot.png)

### Modifications and notable changes needed for emscripten

* Instead of compiling as executable, compile to static library and link it with emscripten to get final wasm and accompanying `html` and `js`. See root `build.zig` and `linkEmscripten()`.
* Add `zglfw.ClientApi.no_api` when creating window, otherwise emscripten will create webgl context that won't work with webgpu.
* Override default logger to emscripten console commands. Zig std does not have implementaion for freestanding target. Emscripten can provide posix dummy file system, stdin/stdout so it might work otherwise. But even then it might not be recomended due to generated file-size etc.
* Don't use GeneralPurposeAllocator - all allocations will fail. Currently easiest is to compile with emscripten emmaloc and use its interface to get memory from system.  
Otherwise it should be possible to export custom malloc/free interface to emscripten but not has been investigated.  
* WGSL changes/deprecated features: use `@vertex` instead of `@stage(vertex)` etc. Natively dawn doesn't seem to enforce these, but browser won't compile deprecated shader stuff.
* Using main loop is not recomended. Refactor your code to instead use `requestAnimationFrame` with callback.
* On each animation frame you MUST check for `gctx.canRender()` for robustness. And skip frame if it returns `false`. It is because `submit` in zgpu can run out of uniform buffers. On native platforms it spins waiting for buffer to free up. This is not possible on web because requestAnimationFrame should not block but return for other system callbacks to be fired. On my system mapAsync in browser runs much slower and can consume all 8(default) buffers. Usually by time next frame starts buffers are ready, but it requires calling canRender(). I have not cought situation where frame needs to be skipped with 8 buffers, but behaviour can be easly tested by reducing buffer count to 4.
* When using custom html shell you might need to resize frame buffer from js side. Glfw glue wont fire events on dom size changes just framebuffer resize. Required js sample code to keep native framebuffer size:

    ```js
    window.addEventListener("resize", function (e) { 
        Module.setCanvasSize(Module.canvas.clientWidth, Module.canvas.clientHeight, false); 
    });
    // resize event only fires when window is resized, add second check in case dom changed etc.
    setInterval(function () {
        if (Module.canvas.width != Module.canvas.clientWidth || Module.canvas.height != Module.canvas.clientHeight) {
            Module.setCanvasSize(Module.canvas.clientWidth, Module.canvas.clientHeight, false);
        }
    }, 100);
    ```
