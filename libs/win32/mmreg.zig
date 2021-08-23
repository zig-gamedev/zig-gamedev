const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");

pub const WAVEFORMATEX = extern struct {
    wFormatTag: WORD,
    nChannels: WORD,
    nSamplesPerSec: DWORD,
    nAvgBytesPerSec: DWORD,
    nBlockAlign: WORD,
    wBitsPerSample: WORD,
    cbSize: WORD,
};

pub const WAVE_FORMAT_PCM = @as(u32, 1);
pub const WAVE_FORMAT_IEEE_FLOAT = @as(u32, 0x0003);
