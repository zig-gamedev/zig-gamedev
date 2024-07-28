const std = @import("std");
const sdl = @import("zsdl2");

comptime {
    _ = std.testing.refAllDecls(@This());
}

pub fn init() !void {
    if (TTF_Init() < 0) return sdl.makeError();
}
extern fn TTF_Init() c_int;

pub fn wasInit() bool {
    return TTF_WasInit() == 1;
}
extern fn TTF_WasInit() c_int;

pub fn quit() void {
    TTF_Quit();
}
extern fn TTF_Quit() void;

pub const Font = opaque {
    pub fn open(file: [:0]const u8, ptsize: i32) !*Font {
        return TTF_OpenFont(file, ptsize) orelse sdl.makeError();
    }
    extern fn TTF_OpenFont(file: [*c]const u8, ptsize: c_int) ?*Font;

    pub fn close(font: *Font) void {
        TTF_CloseFont(font);
    }
    extern fn TTF_CloseFont(font: *Font) void;

    pub fn renderTextSolid(font: *Font, text: [:0]const u8, fg: sdl.Color) !*sdl.Surface {
        return TTF_RenderText_Solid(font, text, fg) orelse sdl.makeError();
    }
    extern fn TTF_RenderText_Solid(font: *Font, text: [*c]const u8, fg: sdl.Color) ?*sdl.Surface;

    pub fn renderTextShaded(font: *Font, text: [:0]const u8, fg: sdl.Color, bg: sdl.Color) !*sdl.Surface {
        return TTF_RenderText_Shaded(font, text, fg, bg) orelse sdl.makeError();
    }
    extern fn TTF_RenderText_Shaded(font: *Font, text: [*c]const u8, fg: sdl.Color, bg: sdl.Color) ?*sdl.Surface;

    pub fn renderTextBlended(font: *Font, text: [:0]const u8, fg: sdl.Color) !*sdl.Surface {
        return TTF_RenderText_Blended(font, text, fg) orelse sdl.makeError();
    }
    extern fn TTF_RenderText_Blended(font: *Font, text: [*c]const u8, fg: sdl.Color) ?*sdl.Surface;
};
