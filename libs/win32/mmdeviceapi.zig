const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("misc.zig");

pub const EDataFlow = enum(UINT) {
    eRender = 0,
    eCapture = 1,
    eAll = 2,
};

pub const ERole = enum(UINT) {
    eConsole = 0,
    eMultimedia = 1,
    eCommunications = 2,
};

pub const IMMDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            Activate: *c_void,
            OpenPropertyStore: *c_void,
            GetId: *c_void,
            GetState: *c_void,
        };
    }
};

pub const IMMDeviceEnumerator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devenum: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            EnumAudioEndpoints: *c_void,
            GetDefaultAudioEndpoint: *c_void,
            GetDevice: *c_void,
            RegisterEndpointNotificationCallback: *c_void,
            UnregisterEndpointNotificationCallback: *c_void,
        };
    }
};

pub const CLSID_MMDeviceEnumerator = GUID{
    .Data1 = 0xBCDE0395,
    .Data2 = 0xE52F,
    .Data3 = 0x467C,
    .Data4 = .{ 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E },
};
pub const IID_IMMDeviceEnumerator = GUID{
    .Data1 = 0xA95664D2,
    .Data2 = 0x9614,
    .Data3 = 0x4F35,
    .Data4 = .{ 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 },
};
