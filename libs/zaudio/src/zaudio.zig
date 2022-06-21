const c = @cImport(@cInclude("miniaudio.h"));

comptime {
    _ = c;
}
