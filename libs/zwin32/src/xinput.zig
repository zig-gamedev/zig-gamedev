const std = @import("std");
const w32 = @import("w32.zig");
const BYTE = w32.BYTE;
const WCHAR = w32.WCHAR;
const SHORT = w32.SHORT;
const WORD = w32.WORD;
const DWORD = w32.DWORD;
const UINT = w32.UINT;
const BOOL = w32.BOOL;
const LPWSTR = w32.LPWSTR;
const GUID = w32.GUID;
const WINAPI = w32.WINAPI;

pub const DEVTYPE_GAMEPAD = 0x01;

pub const DEVSUBTYPE_GAMEPAD = 0x01;
pub const DEVSUBTYPE_UNKNOWN = 0x00;
pub const DEVSUBTYPE_WHEEL = 0x02;
pub const DEVSUBTYPE_ARCADE_STICK = 0x03;
pub const DEVSUBTYPE_FLIGHT_STICK = 0x04;
pub const DEVSUBTYPE_DANCE_PAD = 0x05;
pub const DEVSUBTYPE_GUITAR = 0x06;
pub const DEVSUBTYPE_GUITAR_ALTERNATE = 0x07;
pub const DEVSUBTYPE_DRUM_KIT = 0x08;
pub const DEVSUBTYPE_GUITAR_BASS = 0x0B;
pub const DEVSUBTYPE_ARCADE_PAD = 0x13;

pub const CAPS_VOICE_SUPPORTED = 0x0004;
pub const CAPS_FFB_SUPPORTED = 0x0001;
pub const CAPS_WIRELESS = 0x0002;
pub const CAPS_PMD_SUPPORTED = 0x0008;
pub const CAPS_NO_NAVIGATION = 0x0010;

pub const GAMEPAD_DPAD_UP = 0x0001;
pub const GAMEPAD_DPAD_DOWN = 0x0002;
pub const GAMEPAD_DPAD_LEFT = 0x0004;
pub const GAMEPAD_DPAD_RIGHT = 0x0008;
pub const GAMEPAD_START = 0x0010;
pub const GAMEPAD_BACK = 0x0020;
pub const GAMEPAD_LEFT_THUMB = 0x0040;
pub const GAMEPAD_RIGHT_THUMB = 0x0080;
pub const GAMEPAD_LEFT_SHOULDER = 0x0100;
pub const GAMEPAD_RIGHT_SHOULDER = 0x0200;
pub const GAMEPAD_A = 0x1000;
pub const GAMEPAD_B = 0x2000;
pub const GAMEPAD_X = 0x4000;
pub const GAMEPAD_Y = 0x8000;

pub const GAMEPAD_LEFT_THUMB_DEADZONE = 7849;
pub const GAMEPAD_RIGHT_THUMB_DEADZONE = 8689;
pub const GAMEPAD_TRIGGER_THRESHOLD = 30;

pub const FLAG_GAMEPAD = 0x00000001;

pub const BATTERY_DEVTYPE_GAMEPAD = 0x00;
pub const BATTERY_DEVTYPE_HEADSET = 0x01;

pub const BATTERY_TYPE_DISCONNECTED = 0x00; // This device is not connected
pub const BATTERY_TYPE_WIRED = 0x01; // Wired device, no battery
pub const BATTERY_TYPE_ALKALINE = 0x02; // Alkaline battery source
pub const BATTERY_TYPE_NIMH = 0x03; // Nickel Metal Hydride battery source
pub const BATTERY_TYPE_UNKNOWN = 0xFF; // Cannot determine the battery type

pub const BATTERY_LEVEL_EMPTY = 0x00;
pub const BATTERY_LEVEL_LOW = 0x01;
pub const BATTERY_LEVEL_MEDIUM = 0x02;
pub const BATTERY_LEVEL_FULL = 0x03;

pub const XUSER_MAX_COUNT = 4;

pub const XUSER_INDEX_ANY = 0x000000FF;

pub const VK_PAD_A = 0x5800;
pub const VK_PAD_B = 0x5801;
pub const VK_PAD_X = 0x5802;
pub const VK_PAD_Y = 0x5803;
pub const VK_PAD_RSHOULDER = 0x5804;
pub const VK_PAD_LSHOULDER = 0x5805;
pub const VK_PAD_LTRIGGER = 0x5806;
pub const VK_PAD_RTRIGGER = 0x5807;

pub const VK_PAD_DPAD_UP = 0x5810;
pub const VK_PAD_DPAD_DOWN = 0x5811;
pub const VK_PAD_DPAD_LEFT = 0x5812;
pub const VK_PAD_DPAD_RIGHT = 0x5813;
pub const VK_PAD_START = 0x5814;
pub const VK_PAD_BACK = 0x5815;
pub const VK_PAD_LTHUMB_PRESS = 0x5816;
pub const VK_PAD_RTHUMB_PRESS = 0x5817;

pub const VK_PAD_LTHUMB_UP = 0x5820;
pub const VK_PAD_LTHUMB_DOWN = 0x5821;
pub const VK_PAD_LTHUMB_RIGHT = 0x5822;
pub const VK_PAD_LTHUMB_LEFT = 0x5823;
pub const VK_PAD_LTHUMB_UPLEFT = 0x5824;
pub const VK_PAD_LTHUMB_UPRIGHT = 0x5825;
pub const VK_PAD_LTHUMB_DOWNRIGHT = 0x5826;
pub const VK_PAD_LTHUMB_DOWNLEFT = 0x5827;

pub const VK_PAD_RTHUMB_UP = 0x5830;
pub const VK_PAD_RTHUMB_DOWN = 0x5831;
pub const VK_PAD_RTHUMB_RIGHT = 0x5832;
pub const VK_PAD_RTHUMB_LEFT = 0x5833;
pub const VK_PAD_RTHUMB_UPLEFT = 0x5834;
pub const VK_PAD_RTHUMB_UPRIGHT = 0x5835;
pub const VK_PAD_RTHUMB_DOWNRIGHT = 0x5836;
pub const VK_PAD_RTHUMB_DOWNLEFT = 0x5837;

pub const KEYSTROKE_KEYDOWN = 0x0001;
pub const KEYSTROKE_KEYUP = 0x0002;
pub const KEYSTROKE_REPEAT = 0x0004;

pub const BATTERY_INFORMATION = extern struct {
    BatteryType: BYTE,
    BatteryLevel: BYTE,
};

pub const CAPABILITIES = extern struct {
    Type: BYTE,
    SubType: BYTE,
    Flags: WORD,
    Gamepad: GAMEPAD,
    Vibration: VIBRATION,
};

pub const GAMEPAD = extern struct {
    wButtons: WORD,
    bLeftTrigger: BYTE,
    bRightTrigger: BYTE,
    sThumbLX: SHORT,
    sThumbLY: SHORT,
    sThumbRX: SHORT,
    sThumbRY: SHORT,
};

pub const KEYSTROKE = extern struct {
    VirtualKey: WORD,
    Unicode: WCHAR,
    Flags: WORD,
    UserIndex: BYTE,
    HidCode: BYTE,
};

pub const STATE = extern struct {
    dwPacketNumber: DWORD,
    Gamepad: GAMEPAD,
};

pub const VIBRATION = extern struct {
    wLeftMotorSpeed: WORD,
    wRightMotorSpeed: WORD,
};

pub extern "xinput1_4" fn XInputEnable(enable: BOOL) callconv(WINAPI) void;

pub extern "xinput1_4" fn XInputGetAudioDeviceIds(
    dwUserIndex: DWORD,
    pRenderDeviceId: LPWSTR,
    pRenderCount: *UINT,
    pCaptureDeviceId: LPWSTR,
    pCaptureCount: *UINT,
) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputGetBatteryInformation(
    dwUserIndex: DWORD,
    devType: BYTE,
    pBatteryInformation: *BATTERY_INFORMATION,
) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputGetCapabilities(
    dwUserIndex: DWORD,
    dwFlags: DWORD,
    pCapabilities: CAPABILITIES,
) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputGetDSoundAudioDeviceGuids(
    dwUserIndex: DWORD,
    pDSoundRenderGuid: GUID,
    pDSoundCaptureGuid: GUID,
) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputGetKeystroke(
    dwUserIndex: DWORD,
    dwReserved: DWORD,
    pKeystroke: *KEYSTROKE,
) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputGetState(dwUserIndex: DWORD, pState: *STATE) callconv(WINAPI) DWORD;

pub extern "xinput1_4" fn XInputSetState(dwUserIndex: DWORD, pVibration: *VIBRATION) callconv(WINAPI) DWORD;

pub const ERROR_SUCCESS = w32.SUCCESS;
pub const ERROR_EMPTY = w32.ERROR_EMPTY;
pub const ERROR_DEVICE_NOT_CONNECTED = w32.ERROR_DEVICE_NOT_CONNECTED;

// error set corresponding to the above return codes
pub const Error = error{
    EMPTY,
    DEVICE_NOT_CONNECTED,
};
