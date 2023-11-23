const options = @import("zsdl_options");

pub usingnamespace switch (options.api_version) {
    .sdl2 => @import("sdl2.zig"),
    .sdl3 => @import("sdl3.zig"),
};

pub const ttf = if (options.enable_ttf) @import("ttf.zig") else @compileError("zsdl: SDL_ttf not enabled; check your build options.");

test {
    const testing = @import("std").testing;
    testing.refAllDeclsRecursive(switch (options.api_version) {
        .sdl2 => @import("sdl2.zig"),
        .sdl3 => @import("sdl3.zig"),
    });
    if (options.enable_ttf) {
        testing.refAllDeclsRecursive(ttf);
    }
}
