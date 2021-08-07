const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");
usingnamespace @import("dcommon.zig");

pub const DWRITE_TEXT_ALIGNMENT = enum(UINT) {
    LEADING = 0,
    TRAILING = 1,
    CENTER = 2,
    JUSTIFIED = 3,
};

pub const DWRITE_PARAGRAPH_ALIGNMENT = enum(UINT) {
    NEAR = 0,
    FAR = 1,
    CENTER = 2,
};

pub const IDWriteTextFormat = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        textformat: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetTextAlignment(self: *T, alignment: D2D1_TEXT_ALIGNMENT) HRESULT {
                return self.v.textformat.SetTextAlignment(self, alignment);
            }
            pub inline fn SetParagraphAlignment(self: *T, alignment: D2D1_PARAGRAPH_ALIGNMENT) HRESULT {
                return self.v.textformat.SetParagraphAlignment(self, alignment);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            SetTextAlignment: fn (*T, DWRITE_TEXT_ALIGNMENT) callconv(WINAPI) HRESULT,
            SetParagraphAlignment: fn (*T, DWRITE_PARAGRAPH_ALIGNMENT) callconv(WINAPI) HRESULT,
            SetWordWrapping: *c_void,
            SetReadingDirection: *c_void,
            SetFlowDirection: *c_void,
            SetIncrementalTabStop: *c_void,
            SetTrimming: *c_void,
            SetLineSpacing: *c_void,
            GetTextAlignment: *c_void,
            GetParagraphAlignment: *c_void,
            GetWordWrapping: *c_void,
            GetReadingDirection: *c_void,
            GetFlowDirection: *c_void,
            GetIncrementalTabStop: *c_void,
            GetTrimming: *c_void,
            GetLineSpacing: *c_void,
            GetFontCollection: *c_void,
            GetFontFamilyNameLength: *c_void,
            GetFontFamilyName: *c_void,
            GetFontWeight: *c_void,
            GetFontStyle: *c_void,
            GetFontStretch: *c_void,
            GetFontSize: *c_void,
            GetLocaleNameLength: *c_void,
            GetLocaleName: *c_void,
        };
    }
};
