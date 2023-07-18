const options = @import("zsdl_options");

pub usingnamespace @import("sdl2.zig");

pub const ttf = if (options.enable_ttf) @import("ttf.zig") else @compileError("zsdl: SDL_ttf not enabled; check your build options.");
