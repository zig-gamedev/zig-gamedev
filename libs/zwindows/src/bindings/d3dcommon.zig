const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const WINAPI = windows.WINAPI;
const SIZE_T = windows.SIZE_T;
const LPCSTR = windows.LPCSTR;
const GUID = windows.GUID;

pub const PRIMITIVE_TOPOLOGY = enum(UINT) {
    UNDEFINED = 0,
    POINTLIST = 1,
    LINELIST = 2,
    LINESTRIP = 3,
    TRIANGLELIST = 4,
    TRIANGLESTRIP = 5,
    LINELIST_ADJ = 10,
    LINESTRIP_ADJ = 11,
    TRIANGLELIST_ADJ = 12,
    TRIANGLESTRIP_ADJ = 13,
    CONTROL_POINT_PATCHLIST = 33,
    @"2_CONTROL_POINT_PATCHLIST" = 34,
    @"3_CONTROL_POINT_PATCHLIST" = 35,
    @"4_CONTROL_POINT_PATCHLIST" = 36,
    @"5_CONTROL_POINT_PATCHLIST" = 37,
    @"6_CONTROL_POINT_PATCHLIST" = 38,
    @"7_CONTROL_POINT_PATCHLIST" = 39,
    @"8_CONTROL_POINT_PATCHLIST" = 40,
    @"9_CONTROL_POINT_PATCHLIST" = 41,
    @"10_CONTROL_POINT_PATCHLIST" = 42,
    @"11_CONTROL_POINT_PATCHLIST" = 43,
    @"12_CONTROL_POINT_PATCHLIST" = 44,
    @"13_CONTROL_POINT_PATCHLIST" = 45,
    @"14_CONTROL_POINT_PATCHLIST" = 46,
    @"15_CONTROL_POINT_PATCHLIST" = 47,
    @"16_CONTROL_POINT_PATCHLIST" = 48,
    @"17_CONTROL_POINT_PATCHLIST" = 49,
    @"18_CONTROL_POINT_PATCHLIST" = 50,
    @"19_CONTROL_POINT_PATCHLIST" = 51,
    @"20_CONTROL_POINT_PATCHLIST" = 52,
    @"21_CONTROL_POINT_PATCHLIST" = 53,
    @"22_CONTROL_POINT_PATCHLIST" = 54,
    @"23_CONTROL_POINT_PATCHLIST" = 55,
    @"24_CONTROL_POINT_PATCHLIST" = 56,
    @"25_CONTROL_POINT_PATCHLIST" = 57,
    @"26_CONTROL_POINT_PATCHLIST" = 58,
    @"27_CONTROL_POINT_PATCHLIST" = 59,
    @"28_CONTROL_POINT_PATCHLIST" = 60,
    @"29_CONTROL_POINT_PATCHLIST" = 61,
    @"30_CONTROL_POINT_PATCHLIST" = 62,
    @"31_CONTROL_POINT_PATCHLIST" = 63,
    @"32_CONTROL_POINT_PATCHLIST" = 64,
};

pub const FEATURE_LEVEL = enum(UINT) {
    @"1_0_CORE" = 0x1000,
    @"9_1" = 0x9100,
    @"9_2" = 0x9200,
    @"9_3" = 0x9300,
    @"10_0" = 0xa000,
    @"10_1" = 0xa100,
    @"11_0" = 0xb000,
    @"11_1" = 0xb100,
    @"12_0" = 0xc000,
    @"12_1" = 0xc100,
    @"12_2" = 0xc200,
};

pub const DRIVER_TYPE = enum(UINT) {
    UNKNOWN = 0,
    HARDWARE = 1,
    REFERENCE = 2,
    NULL = 3,
    SOFTWARE = 4,
    WARP = 5,
};

pub const SHADER_MACRO = extern struct {
    Name: LPCSTR,
    Definition: LPCSTR,
};

pub const INCLUDE_TYPE = enum(UINT) {
    INCLUDE_LOCAL = 0,
    INCLUDE_SYSTEM = 1,
};

pub const IID_IBlob = GUID.parse("{8BA5FB08-5195-40e2-AC58-0D989C3A0102}");
pub const IBlob = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn GetBufferPointer(self: *T) *anyopaque {
                return @as(*const IBlob.VTable, @ptrCast(self.__v)).GetBufferPointer(@as(*IBlob, @ptrCast(self)));
            }
            pub inline fn GetBufferSize(self: *T) SIZE_T {
                return @as(*const IBlob.VTable, @ptrCast(self.__v)).GetBufferSize(@as(*IBlob, @ptrCast(self)));
            }
        };
    }

    pub const VTable = extern struct {
        base: IUnknown.VTable,
        GetBufferPointer: *const fn (*IBlob) callconv(WINAPI) *anyopaque,
        GetBufferSize: *const fn (*IBlob) callconv(WINAPI) SIZE_T,
    };
};

pub const IInclude = extern struct {
    __v: *const VTable,

    pub const VTable = extern struct {
        Open: *const fn (*IInclude, INCLUDE_TYPE, LPCSTR, *anyopaque, **anyopaque, *UINT) callconv(WINAPI) void,
        Close: *const fn (*IInclude, *anyopaque) callconv(WINAPI) void,
    };
};
