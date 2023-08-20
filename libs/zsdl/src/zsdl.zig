const options = @import("zsdl_options");

const sdl2 = @import("sdl2.zig");
pub usingnamespace sdl2;

pub const ttf = if (options.enable_ttf) @import("ttf.zig") else @compileError("zsdl: SDL_ttf not enabled; check your build options.");

test {
    const testing = @import("std").testing;

    testing.refAllDeclsRecursive(sdl2);

    // TEMPORARY: uncomment to test SDL3 bindings are compilable
    //testing.refAllDeclsRecursive(@import("sdl3.zig"));

    if (options.enable_ttf) {
        testing.refAllDeclsRecursive(ttf);
    }
}
