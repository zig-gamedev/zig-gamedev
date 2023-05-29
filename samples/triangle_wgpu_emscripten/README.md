## triangle (wgpu + emscripten)

Same sample as `triangle_wgpu` but with added emscripten sdk support to compile for web.

### Build
#### Native
```bash
zig build triangle_wgpu_emscripten-run
```
#### Wasm for web
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
Open hosted site with [chrome canary](https://www.google.com/chrome/canary/) other browsers might still not support features such as spir-v shaders used by imgui (see [known-issues](#todo-and-known-issues)).

![image](screenshot.png)

### Modifications for emscripten

* Instead of compiling as executable, compile to static library and link it with emscripten to get final wasm and accompanying `html` and `js`. See root `build.zig` and `linkEmscripten()`.
* Add `zglfw.ClientApi.no_api` when creating window, otherwise emscripten will create webgl context that won't work with webgpu.
* Override default logger to emscripten console commands. Zig std does not have implementaion for freestanding target. Emscripten tries providing posix dummy file system, stdin/stdou so it might work otherwise. But even then it might not be recomended due to generated file-size etc.
* Don't use GeneralPurposeAllocator - all allocations will fail. Currently easiest is to compile with emscripten emmaloc and use its interface to get memory from system.  
Otherwise it should be possible to export custom malloc/free interface to emscripten but not has been investigated.  

* WGSL changes/deprecated features: use `@vertex` instead of `@stage(vertex)` etc. Natively dawn doesn't seem to enforce these, but browser won't compile deprecated shader stuff.
* When using custom html shell you might need to resize frame buffer from js side. Glfw glue wont fire events on dom size changes just framebuffer. Required js sample code to keep native framebuffer size:

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
### TODO and Known Issues

* Flickering: I suspect it happens due to custom loop relaying on emscripten `asyncify` to give control back to JS (as you are blocking main thread). Refactoring out of main loop and using `requestAnimationFrame` should be correct way to implement this. 
* `imgui_wgpu` outdated implementation uses spirv instead of `wgsl`. Its out of spec and won't work in most browsers. Imgui or imgui_wgpu has to be updated. It still works in `chrome`.

