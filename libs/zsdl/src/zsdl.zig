const std = @import("std");
const assert = std.debug.assert;

pub const InitFlags = packed struct(u32) {
    timer: bool = false,
    __unused1: bool = false,
    __unused2: bool = false,
    __unused3: bool = false,
    audio: bool = false,
    video: bool = false,
    __unused6: bool = false,
    __unused7: bool = false,
    __unused8: bool = false,
    joystick: bool = false,
    __unused10: bool = false,
    __unused11: bool = false,
    haptic: bool = false,
    gamecontroller: bool = false,
    events: bool = false,
    sensor: bool = false,
    __unused16: bool = false,
    __unused17: bool = false,
    __unused18: bool = false,
    __unused19: bool = false,
    noparachute: bool = false,
    __unused: u11 = 0,

    pub const everything: InitFlags = .{
        .timer = true,
        .audio = true,
        .video = true,
        .events = true,
        .joystick = true,
        .haptic = true,
        .gamecontroller = true,
        .sensor = true,
    };
};

pub fn init(flags: InitFlags) Error!void {
    if (SDL_Init(flags) < 0) return makeError();
}
extern fn SDL_Init(flags: InitFlags) i32;

/// `pub fn quit() void`
pub const quit = SDL_Quit;
extern fn SDL_Quit() void;

pub fn getError() ?[:0]const u8 {
    if (SDL_GetError()) |cstr| {
        return std.mem.sliceTo(cstr, 0);
    } else {
        return null;
    }
}
extern fn SDL_GetError() ?[*:0]const u8;

pub const Error = error{SdlError};

pub fn makeError() error{SdlError} {
    if (getError()) |str| {
        std.log.debug("SDL2: {s}", .{str});
    }
    return error.SdlError;
}

pub const Window = opaque {
    pub const Flags = packed struct(u32) {
        fullscreen: bool = false,
        opengl: bool = false,
        shown: bool = false,
        hidden: bool = false,
        borderless: bool = false, // 0x10
        resizable: bool = false,
        minimized: bool = false,
        maximized: bool = false,
        mouse_grabbed: bool = false, // 0x100
        input_focus: bool = false,
        mouse_focus: bool = false,
        foreign: bool = false,
        _desktop: bool = false, // 0x1000
        allow_highdpi: bool = false,
        mouse_capture: bool = false,
        always_on_top: bool = false,
        skip_taskbar: bool = false, // 0x10000
        utility: bool = false,
        tooltip: bool = false,
        popup_menu: bool = false,
        keyboard_grabbed: bool = false,

        __unused21: bool = false,
        __unused22: bool = false,
        __unused23: bool = false,
        __unused24: bool = false,

        vulkan: bool = false,
        metal: bool = false,

        __unused: u5 = 0,

        pub const fullscreen_desktop: Flags = .{ .fullscreen = true, ._desktop = true };
        pub const input_grabbed: Flags = .{ .mouse_grabbed = true };
    };

    pub const pos_undefined = posUndefinedDisplay(0);
    pub const pos_centered = posCenteredDisplay(0);

    pub fn posUndefinedDisplay(x: i32) i32 {
        return pos_undefined_mask | x;
    }
    pub fn posCenteredDisplay(x: i32) i32 {
        return pos_centered_mask | x;
    }

    const pos_undefined_mask: i32 = 0x1fff_0000;
    const pos_centered_mask: i32 = 0x2fff_0000;

    pub fn create(title: [:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Flags) Error!*Window {
        return SDL_CreateWindow(title, x, y, w, h, flags) orelse return makeError();
    }
    extern fn SDL_CreateWindow(title: ?[*:0]const u8, x: i32, y: i32, w: i32, h: i32, flags: Flags) ?*Window;

    /// `pub fn destroy(window: *Window) void`
    pub const destroy = SDL_DestroyWindow;
    extern fn SDL_DestroyWindow(window: *Window) void;

    pub fn getDisplayMode(window: *Window) DisplayMode {
        var mode: DisplayMode = undefined;
        SDL_GetWindowDisplayMode(window, &mode);
        return mode;
    }
    extern fn SDL_GetWindowDisplayMode(*Window, *DisplayMode) void;
};

pub const EventType = enum(u32) {
    firstevent = 0,

    quit = 0x100,
    app_terminating,
    app_lowmemory,
    app_willenterbackground,
    app_didenterbackground,
    app_willenterforeground,
    app_didenterforeground,
    localechanged,

    displayevent = 0x150,

    windowevent = 0x200,
    syswmevent,

    keydown = 0x300,
    keyup,
    textediting,
    textinput,
    keymapchanged,
    textediting_ext,
    mousemotion = 0x400,
    mousebuttondown,
    mousebuttonup,
    mousewheel,

    joyaxismotion = 0x600,
    joyballmotion,
    joyhatmotion,
    joybuttondown,
    joybuttonup,
    joydeviceadded,
    joydeviceremoved,
    joybatteryupdated,

    controlleraxismotion = 0x650,
    controllerbuttondown,
    controllerbuttonup,
    controllerdeviceadded,
    controllerdeviceremoved,
    controllerdeviceremapped,
    controllertouchpaddown,
    controllertouchpadmotion,
    controllertouchpadup,
    controllersensorupdate,

    fingerdown = 0x700,
    fingerup,
    fingermotion,

    dollargesture = 0x800,
    dollarrecord,
    multigesture,

    clipboardupdate = 0x900,

    dropfile = 0x1000,
    droptext,
    dropbegin,
    dropcomplete,

    audiodeviceadded = 0x1100,
    audiodeviceremoved,

    sensorupdate = 0x1200,

    render_targets_reset = 0x2000,
    render_device_reset,

    pollsentinel = 0x7f00,

    userevent = 0x8000,

    lastevent = 0xffff,
};

pub const DisplayEventId = enum(u8) {
    none,
    orientation,
    connected,
    disconnected,
};

pub const WindowEventId = enum(u8) {
    none,
    shown,
    hidden,
    exposed,

    moved,

    resized,
    size_changed,

    minimized,
    maximized,
    restored,

    enter,
    leave,
    focus_gained,
    focus_lost,
    close,
    take_focus,
    hit_test,
    iccprof_changed,
    display_changed,
};

pub const ReleasedOrPressed = enum(u8) {
    released,
    pressed,
};

pub const MouseWheelDirection = enum(u32) {
    normal,
    flipped,
};

pub const Scancode = enum(u32) {
    unknown = 0,
    a = 4,
    b = 5,
    c = 6,
    d = 7,
    e = 8,
    f = 9,
    g = 10,
    h = 11,
    i = 12,
    j = 13,
    k = 14,
    l = 15,
    m = 16,
    n = 17,
    o = 18,
    p = 19,
    q = 20,
    r = 21,
    s = 22,
    t = 23,
    u = 24,
    v = 25,
    w = 26,
    x = 27,
    y = 28,
    z = 29,
    @"1" = 30,
    @"2" = 31,
    @"3" = 32,
    @"4" = 33,
    @"5" = 34,
    @"6" = 35,
    @"7" = 36,
    @"8" = 37,
    @"9" = 38,
    @"0" = 39,
    @"return" = 40,
    escape = 41,
    backspace = 42,
    tab = 43,
    space = 44,
    minus = 45,
    equals = 46,
    leftbracket = 47,
    rightbracket = 48,
    backslash = 49,
    nonushash = 50,
    semicolon = 51,
    apostrophe = 52,
    grave = 53,
    comma = 54,
    period = 55,
    slash = 56,
    capslock = 57,
    f1 = 58,
    f2 = 59,
    f3 = 60,
    f4 = 61,
    f5 = 62,
    f6 = 63,
    f7 = 64,
    f8 = 65,
    f9 = 66,
    f10 = 67,
    f11 = 68,
    f12 = 69,
    printscreen = 70,
    scrolllock = 71,
    pause = 72,
    insert = 73,
    home = 74,
    pageup = 75,
    delete = 76,
    end = 77,
    pagedown = 78,
    right = 79,
    left = 80,
    down = 81,
    up = 82,
    numlockclear = 83,
    kp_divide = 84,
    kp_multiply = 85,
    kp_minus = 86,
    kp_plus = 87,
    kp_enter = 88,
    kp_1 = 89,
    kp_2 = 90,
    kp_3 = 91,
    kp_4 = 92,
    kp_5 = 93,
    kp_6 = 94,
    kp_7 = 95,
    kp_8 = 96,
    kp_9 = 97,
    kp_0 = 98,
    kp_period = 99,
    nonusbackslash = 100,
    application = 101,
    power = 102,
    kp_equals = 103,
    f13 = 104,
    f14 = 105,
    f15 = 106,
    f16 = 107,
    f17 = 108,
    f18 = 109,
    f19 = 110,
    f20 = 111,
    f21 = 112,
    f22 = 113,
    f23 = 114,
    f24 = 115,
    execute = 116,
    help = 117,
    menu = 118,
    select = 119,
    stop = 120,
    again = 121,
    undo = 122,
    cut = 123,
    copy = 124,
    paste = 125,
    find = 126,
    mute = 127,
    volumeup = 128,
    volumedown = 129,
    kp_comma = 133,
    kp_equalsas400 = 134,
    international1 = 135,
    international2 = 136,
    international3 = 137,
    international4 = 138,
    international5 = 139,
    international6 = 140,
    international7 = 141,
    international8 = 142,
    international9 = 143,
    lang1 = 144,
    lang2 = 145,
    lang3 = 146,
    lang4 = 147,
    lang5 = 148,
    lang6 = 149,
    lang7 = 150,
    lang8 = 151,
    lang9 = 152,
    alterase = 153,
    sysreq = 154,
    cancel = 155,
    clear = 156,
    prior = 157,
    return2 = 158,
    separator = 159,
    out = 160,
    oper = 161,
    clearagain = 162,
    crsel = 163,
    exsel = 164,
    kp_00 = 176,
    kp_000 = 177,
    thousandsseparator = 178,
    decimalseparator = 179,
    currencyunit = 180,
    currencysubunit = 181,
    kp_leftparen = 182,
    kp_rightparen = 183,
    kp_leftbrace = 184,
    kp_rightbrace = 185,
    kp_tab = 186,
    kp_backspace = 187,
    kp_a = 188,
    kp_b = 189,
    kp_c = 190,
    kp_d = 191,
    kp_e = 192,
    kp_f = 193,
    kp_xor = 194,
    kp_power = 195,
    kp_percent = 196,
    kp_less = 197,
    kp_greater = 198,
    kp_ampersand = 199,
    kp_dblampersand = 200,
    kp_verticalbar = 201,
    kp_dblverticalbar = 202,
    kp_colon = 203,
    kp_hash = 204,
    kp_space = 205,
    kp_at = 206,
    kp_exclam = 207,
    kp_memstore = 208,
    kp_memrecall = 209,
    kp_memclear = 210,
    kp_memadd = 211,
    kp_memsubtract = 212,
    kp_memmultiply = 213,
    kp_memdivide = 214,
    kp_plusminus = 215,
    kp_clear = 216,
    kp_clearentry = 217,
    kp_binary = 218,
    kp_octal = 219,
    kp_decimal = 220,
    kp_hexadecimal = 221,
    lctrl = 224,
    lshift = 225,
    lalt = 226,
    lgui = 227,
    rctrl = 228,
    rshift = 229,
    ralt = 230,
    rgui = 231,
    mode = 257,
    audionext = 258,
    audioprev = 259,
    audiostop = 260,
    audioplay = 261,
    audiomute = 262,
    mediaselect = 263,
    www = 264,
    mail = 265,
    calculator = 266,
    computer = 267,
    ac_search = 268,
    ac_home = 269,
    ac_back = 270,
    ac_forward = 271,
    ac_stop = 272,
    ac_refresh = 273,
    ac_bookmarks = 274,
    brightnessdown = 275,
    brightnessup = 276,
    displayswitch = 277,
    kbdillumtoggle = 278,
    kbdillumdown = 279,
    kbdillumup = 280,
    eject = 281,
    sleep = 282,
    app1 = 283,
    app2 = 284,
    audiorewind = 285,
    audiofastforward = 286,
    softleft = 287,
    softright = 288,
    call = 289,
    endcall = 290,
    _,
};

pub const Keycode = enum(i32) {
    unknown = 0,
    @"return" = '\r',
    escape = '\x1b',
    backspace = '\x08',
    tab = '\t',
    space = ' ',
    exclaim = '!',
    quotedbl = '"',
    hash = '#',
    percent = '%',
    dollar = '$',
    ampersand = '&',
    quote = '\'',
    leftparen = '(',
    rightparen = ')',
    asterisk = '*',
    plus = '+',
    comma = ',',
    minus = '-',
    period = '.',
    slash = '/',
    @"0" = '0',
    @"1" = '1',
    @"2" = '2',
    @"3" = '3',
    @"4" = '4',
    @"5" = '5',
    @"6" = '6',
    @"7" = '7',
    @"8" = '8',
    @"9" = '9',
    colon = ':',
    semicolon = ';',
    less = '<',
    equals = '=',
    greater = '>',
    question = '?',
    at = '@',
    leftbracket = '[',
    backslash = '\\',
    rightbracket = ']',
    caret = '^',
    underscore = '_',
    backquote = '`',
    a = 'a',
    b = 'b',
    c = 'c',
    d = 'd',
    e = 'e',
    f = 'f',
    g = 'g',
    h = 'h',
    i = 'i',
    j = 'j',
    k = 'k',
    l = 'l',
    m = 'm',
    n = 'n',
    o = 'o',
    p = 'p',
    q = 'q',
    r = 'r',
    s = 's',
    t = 't',
    u = 'u',
    v = 'v',
    w = 'w',
    x = 'x',
    y = 'y',
    z = 'z',
    capslock = @enumToInt(Scancode.capslock) | mask,
    f1 = @enumToInt(Scancode.f1) | mask,
    f2 = @enumToInt(Scancode.f2) | mask,
    f3 = @enumToInt(Scancode.f3) | mask,
    f4 = @enumToInt(Scancode.f4) | mask,
    f5 = @enumToInt(Scancode.f5) | mask,
    f6 = @enumToInt(Scancode.f6) | mask,
    f7 = @enumToInt(Scancode.f7) | mask,
    f8 = @enumToInt(Scancode.f8) | mask,
    f9 = @enumToInt(Scancode.f9) | mask,
    f10 = @enumToInt(Scancode.f10) | mask,
    f11 = @enumToInt(Scancode.f11) | mask,
    f12 = @enumToInt(Scancode.f12) | mask,
    printscreen = @enumToInt(Scancode.printscreen) | mask,
    scrolllock = @enumToInt(Scancode.scrolllock) | mask,
    pause = @enumToInt(Scancode.pause) | mask,
    insert = @enumToInt(Scancode.insert) | mask,
    home = @enumToInt(Scancode.home) | mask,
    pageup = @enumToInt(Scancode.pageup) | mask,
    delete = '\x7f',
    end = @enumToInt(Scancode.end) | mask,
    pagedown = @enumToInt(Scancode.pagedown) | mask,
    right = @enumToInt(Scancode.right) | mask,
    left = @enumToInt(Scancode.left) | mask,
    down = @enumToInt(Scancode.down) | mask,
    up = @enumToInt(Scancode.up) | mask,
    numlockclear = @enumToInt(Scancode.numlockclear) | mask,
    kp_divide = @enumToInt(Scancode.kp_divide) | mask,
    kp_multiply = @enumToInt(Scancode.kp_multiply) | mask,
    kp_minus = @enumToInt(Scancode.kp_minus) | mask,
    kp_plus = @enumToInt(Scancode.kp_plus) | mask,
    kp_enter = @enumToInt(Scancode.kp_enter) | mask,
    kp_1 = @enumToInt(Scancode.kp_1) | mask,
    kp_2 = @enumToInt(Scancode.kp_2) | mask,
    kp_3 = @enumToInt(Scancode.kp_3) | mask,
    kp_4 = @enumToInt(Scancode.kp_4) | mask,
    kp_5 = @enumToInt(Scancode.kp_5) | mask,
    kp_6 = @enumToInt(Scancode.kp_6) | mask,
    kp_7 = @enumToInt(Scancode.kp_7) | mask,
    kp_8 = @enumToInt(Scancode.kp_8) | mask,
    kp_9 = @enumToInt(Scancode.kp_9) | mask,
    kp_0 = @enumToInt(Scancode.kp_0) | mask,
    kp_period = @enumToInt(Scancode.kp_period) | mask,
    application = @enumToInt(Scancode.application) | mask,
    power = @enumToInt(Scancode.power) | mask,
    kp_equals = @enumToInt(Scancode.kp_equals) | mask,
    f13 = @enumToInt(Scancode.f13) | mask,
    f14 = @enumToInt(Scancode.f14) | mask,
    f15 = @enumToInt(Scancode.f15) | mask,
    f16 = @enumToInt(Scancode.f16) | mask,
    f17 = @enumToInt(Scancode.f17) | mask,
    f18 = @enumToInt(Scancode.f18) | mask,
    f19 = @enumToInt(Scancode.f19) | mask,
    f20 = @enumToInt(Scancode.f20) | mask,
    f21 = @enumToInt(Scancode.f21) | mask,
    f22 = @enumToInt(Scancode.f22) | mask,
    f23 = @enumToInt(Scancode.f23) | mask,
    f24 = @enumToInt(Scancode.f24) | mask,
    execute = @enumToInt(Scancode.execute) | mask,
    help = @enumToInt(Scancode.help) | mask,
    menu = @enumToInt(Scancode.menu) | mask,
    select = @enumToInt(Scancode.select) | mask,
    stop = @enumToInt(Scancode.stop) | mask,
    again = @enumToInt(Scancode.again) | mask,
    undo = @enumToInt(Scancode.undo) | mask,
    cut = @enumToInt(Scancode.cut) | mask,
    copy = @enumToInt(Scancode.copy) | mask,
    paste = @enumToInt(Scancode.paste) | mask,
    find = @enumToInt(Scancode.find) | mask,
    mute = @enumToInt(Scancode.mute) | mask,
    volumeup = @enumToInt(Scancode.volumeup) | mask,
    volumedown = @enumToInt(Scancode.volumedown) | mask,
    kp_comma = @enumToInt(Scancode.kp_comma) | mask,
    kp_equalsas400 = @enumToInt(Scancode.kp_equalsas400) | mask,
    alterase = @enumToInt(Scancode.alterase) | mask,
    sysreq = @enumToInt(Scancode.sysreq) | mask,
    cancel = @enumToInt(Scancode.cancel) | mask,
    clear = @enumToInt(Scancode.clear) | mask,
    prior = @enumToInt(Scancode.prior) | mask,
    return2 = @enumToInt(Scancode.return2) | mask,
    separator = @enumToInt(Scancode.separator) | mask,
    out = @enumToInt(Scancode.out) | mask,
    oper = @enumToInt(Scancode.oper) | mask,
    clearagain = @enumToInt(Scancode.clearagain) | mask,
    crsel = @enumToInt(Scancode.crsel) | mask,
    exsel = @enumToInt(Scancode.exsel) | mask,
    kp_00 = @enumToInt(Scancode.kp_00) | mask,
    kp_000 = @enumToInt(Scancode.kp_000) | mask,
    thousandsseparator = @enumToInt(Scancode.thousandsseparator) | mask,
    decimalseparator = @enumToInt(Scancode.decimalseparator) | mask,
    currencyunit = @enumToInt(Scancode.currencyunit) | mask,
    currencysubunit = @enumToInt(Scancode.currencysubunit) | mask,
    kp_leftparen = @enumToInt(Scancode.kp_leftparen) | mask,
    kp_rightparen = @enumToInt(Scancode.kp_rightparen) | mask,
    kp_leftbrace = @enumToInt(Scancode.kp_leftbrace) | mask,
    kp_rightbrace = @enumToInt(Scancode.kp_rightbrace) | mask,
    kp_tab = @enumToInt(Scancode.kp_tab) | mask,
    kp_backspace = @enumToInt(Scancode.kp_backspace) | mask,
    kp_a = @enumToInt(Scancode.kp_a) | mask,
    kp_b = @enumToInt(Scancode.kp_b) | mask,
    kp_c = @enumToInt(Scancode.kp_c) | mask,
    kp_d = @enumToInt(Scancode.kp_d) | mask,
    kp_e = @enumToInt(Scancode.kp_e) | mask,
    kp_f = @enumToInt(Scancode.kp_f) | mask,
    kp_xor = @enumToInt(Scancode.kp_xor) | mask,
    kp_power = @enumToInt(Scancode.kp_power) | mask,
    kp_percent = @enumToInt(Scancode.kp_percent) | mask,
    kp_less = @enumToInt(Scancode.kp_less) | mask,
    kp_greater = @enumToInt(Scancode.kp_greater) | mask,
    kp_ampersand = @enumToInt(Scancode.kp_ampersand) | mask,
    kp_dblampersand = @enumToInt(Scancode.kp_dblampersand) | mask,
    kp_verticalbar = @enumToInt(Scancode.kp_verticalbar) | mask,
    kp_dblverticalbar = @enumToInt(Scancode.kp_dblverticalbar) | mask,
    kp_colon = @enumToInt(Scancode.kp_colon) | mask,
    kp_hash = @enumToInt(Scancode.kp_hash) | mask,
    kp_space = @enumToInt(Scancode.kp_space) | mask,
    kp_at = @enumToInt(Scancode.kp_at) | mask,
    kp_exclam = @enumToInt(Scancode.kp_exclam) | mask,
    kp_memstore = @enumToInt(Scancode.kp_memstore) | mask,
    kp_memrecall = @enumToInt(Scancode.kp_memrecall) | mask,
    kp_memclear = @enumToInt(Scancode.kp_memclear) | mask,
    kp_memadd = @enumToInt(Scancode.kp_memadd) | mask,
    kp_memsubtract = @enumToInt(Scancode.kp_memsubtract) | mask,
    kp_memmultiply = @enumToInt(Scancode.kp_memmultiply) | mask,
    kp_memdivide = @enumToInt(Scancode.kp_memdivide) | mask,
    kp_plusminus = @enumToInt(Scancode.kp_plusminus) | mask,
    kp_clear = @enumToInt(Scancode.kp_clear) | mask,
    kp_clearentry = @enumToInt(Scancode.kp_clearentry) | mask,
    kp_binary = @enumToInt(Scancode.kp_binary) | mask,
    kp_octal = @enumToInt(Scancode.kp_octal) | mask,
    kp_decimal = @enumToInt(Scancode.kp_decimal) | mask,
    kp_hexadecimal = @enumToInt(Scancode.kp_hexadecimal) | mask,
    lctrl = @enumToInt(Scancode.lctrl) | mask,
    lshift = @enumToInt(Scancode.lshift) | mask,
    lalt = @enumToInt(Scancode.lalt) | mask,
    lgui = @enumToInt(Scancode.lgui) | mask,
    rctrl = @enumToInt(Scancode.rctrl) | mask,
    rshift = @enumToInt(Scancode.rshift) | mask,
    ralt = @enumToInt(Scancode.ralt) | mask,
    rgui = @enumToInt(Scancode.rgui) | mask,
    mode = @enumToInt(Scancode.mode) | mask,
    audionext = @enumToInt(Scancode.audionext) | mask,
    audioprev = @enumToInt(Scancode.audioprev) | mask,
    audiostop = @enumToInt(Scancode.audiostop) | mask,
    audioplay = @enumToInt(Scancode.audioplay) | mask,
    audiomute = @enumToInt(Scancode.audiomute) | mask,
    mediaselect = @enumToInt(Scancode.mediaselect) | mask,
    www = @enumToInt(Scancode.www) | mask,
    mail = @enumToInt(Scancode.mail) | mask,
    calculator = @enumToInt(Scancode.calculator) | mask,
    computer = @enumToInt(Scancode.computer) | mask,
    ac_search = @enumToInt(Scancode.ac_search) | mask,
    ac_home = @enumToInt(Scancode.ac_home) | mask,
    ac_back = @enumToInt(Scancode.ac_back) | mask,
    ac_forward = @enumToInt(Scancode.ac_forward) | mask,
    ac_stop = @enumToInt(Scancode.ac_stop) | mask,
    ac_refresh = @enumToInt(Scancode.ac_refresh) | mask,
    ac_bookmarks = @enumToInt(Scancode.ac_bookmarks) | mask,
    brightnessdown = @enumToInt(Scancode.brightnessdown) | mask,
    brightnessup = @enumToInt(Scancode.brightnessup) | mask,
    displayswitch = @enumToInt(Scancode.displayswitch) | mask,
    kbdillumtoggle = @enumToInt(Scancode.kbdillumtoggle) | mask,
    kbdillumdown = @enumToInt(Scancode.kbdillumdown) | mask,
    kbdillumup = @enumToInt(Scancode.kbdillumup) | mask,
    eject = @enumToInt(Scancode.eject) | mask,
    sleep = @enumToInt(Scancode.sleep) | mask,
    app1 = @enumToInt(Scancode.app1) | mask,
    app2 = @enumToInt(Scancode.app2) | mask,
    audiorewind = @enumToInt(Scancode.audiorewind) | mask,
    audiofastforward = @enumToInt(Scancode.audiofastforward) | mask,
    softleft = @enumToInt(Scancode.softleft) | mask,
    softright = @enumToInt(Scancode.softright) | mask,
    call = @enumToInt(Scancode.call) | mask,
    endcall = @enumToInt(Scancode.endcall) | mask,
    _,

    const mask = 1 << 30;
};

pub const Keysym = extern struct {
    scancode: Scancode,
    sym: Keycode,
    mod: u16,
    unused: u32,
};

pub const CommonEvent = extern struct {
    type: EventType,
    timestamp: u32,
};

pub const DisplayEvent = extern struct {
    type: EventType,
    timestamp: u32,
    display: u32,
    event: DisplayEventId,
    padding1: u8,
    padding2: u8,
    padding3: u8,
    data1: i32,
};

pub const WindowEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    event: WindowEventId,
    padding1: u8,
    padding2: u8,
    padding3: u8,
    data1: i32,
    data2: i32,
};

pub const KeyboardEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    state: ReleasedOrPressed,
    repeat: u8,
    padding2: u8,
    padding3: u8,
    keysym: Keysym,
};

pub const TextEditingEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [text_size]u8,
    start: i32,
    length: i32,

    const text_size = 32;
};

pub const TextEditingExtEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [*:0]u8,
    start: i32,
    length: i32,
};

pub const TextInputEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    text: [text_size]u8,

    const text_size = 32;
};

pub const MouseMotionEvent = extern struct {
    type: EventType,
    timestamp: u32,
    windowID: u32,
    which: u32,
    state: u32,
    x: i32,
    y: i32,
    xrel: i32,
    yrel: i32,
};

pub const MouseButtonEvent = extern struct {
    type: EventType,
    timestamp: u32,
    windowID: u32,
    which: u32,
    button: u8,
    state: ReleasedOrPressed,
    clicks: u8,
    padding1: u8,
    x: i32,
    y: i32,
};

pub const MouseWheelEvent = extern struct {
    type: EventType,
    timestamp: u32,
    window_id: u32,
    which: u32,
    x: i32,
    y: i32,
    direction: MouseWheelDirection,
    preciseX: f32,
    preciseY: f32,
};

pub const QuitEvent = extern struct {
    type: EventType,
    timestamp: u32,
};

pub const Event = extern union {
    type: EventType,
    common: CommonEvent,
    display: DisplayEvent,
    window: WindowEvent,
    key: KeyboardEvent,
    edit: TextEditingEvent,
    editExt: TextEditingExtEvent,
    text: TextInputEvent,
    motion: MouseMotionEvent,
    button: MouseButtonEvent,
    wheel: MouseWheelEvent,
    quit: QuitEvent,

    padding: [size]u8,

    const size = if (@sizeOf(usize) <= 8) 56 else if (@sizeOf(usize) == 16) 64 else 3 * @sizeOf(usize);

    comptime {
        assert(@sizeOf(Event) == size);
    }
};

pub fn pollEvent(event: ?*Event) bool {
    return SDL_PollEvent(event) != 0;
}
extern fn SDL_PollEvent(event: ?*Event) i32;

pub const hint_windows_dpi_awareness = "SDL_WINDOWS_DPI_AWARENESS";

pub fn setHint(name: [:0]const u8, value: [:0]const u8) bool {
    return SDL_SetHint(name, value) != 0;
}
extern fn SDL_SetHint(name: [*:0]const u8, value: [*:0]const u8) i32;

pub const DisplayID = u32;

pub const DisplayMode = DisplayMode_SDL2;

const DisplayMode_SDL2 = extern struct {
    format: u32,
    w: c_int,
    h: c_int,
    refresh_rate: c_int,
    driverdata: *anyopaque,
};

const DisplayMode_SDL3 = extern struct {
    displayID: DisplayID,
    format: u32,
    pixel_w: c_int,
    pixel_h: c_int,
    screen_w: c_int,
    screen_h: c_int,
    display_scale: f32,
    refresh_rate: f32,
    driverdata: *anyopaque,
};

pub fn getPerformanceCounter() u64 {
    return SDL_GetPerformanceCounter();
}
extern fn SDL_GetPerformanceCounter() u64;

pub fn getPerformanceFrequency() u64 {
    return SDL_GetPerformanceFrequency();
}
extern fn SDL_GetPerformanceFrequency() u64;

pub fn delay(ms: u32) void {
    SDL_Delay(ms);
}
extern fn SDL_Delay(ms: u32) void;

pub const gl = struct {
    pub const Context = *anyopaque;

    pub const Attr = enum(i32) {
        red_size,
        green_size,
        blue_size,
        alpha_size,
        buffer_size,
        doublebuffer,
        depth_size,
        stencil_size,
        accum_red_size,
        accum_green_size,
        accum_blue_size,
        accum_alpha_size,
        stereo,
        multisamplebuffers,
        multisamplesamples,
        accelerated_visual,
        retained_backing,
        context_major_version,
        context_minor_version,
        context_egl,
        context_flags,
        context_profile_mask,
        share_with_current_context,
        framebuffer_srgb_capable,
        context_release_behavior,
        context_reset_notification,
        context_no_error,
        floatbuffers,
    };

    pub const Profile = enum(i32) {
        core = 0x0001,
        compatibility = 0x0002,
        es = 0x0004,
    };

    pub const ContextFlags = packed struct(i32) {
        debug: bool = false,
        forward_compatible: bool = false,
        robust_access: bool = false,
        reset_isolation: bool = false,
        __unused: i28 = 0,
    };

    pub const ContextReleaseFlags = packed struct(i32) {
        flush: bool = false,
        __unused: i31 = 0,
    };

    pub const ContextResetNotification = enum(i32) {
        no_notification = 0x0000,
        lose_context = 0x0001,
    };

    pub fn setAttribute(attr: Attr, value: i32) Error!void {
        if (SDL_GL_SetAttribute(attr, value) < 0) return makeError();
    }
    extern fn SDL_GL_SetAttribute(attr: Attr, value: i32) i32;

    pub fn getAttribute(attr: Attr) Error!i32 {
        var value: i32 = undefined;
        if (SDL_GL_GetAttribute(attr, &value) < 0) return makeError();
        return value;
    }
    extern fn SDL_GL_GetAttribute(attr: Attr, value: i32) i32;

    pub fn setSwapInterval(interval: i32) Error!void {
        if (SDL_GL_SetSwapInterval(interval) < 0) return makeError();
    }
    extern fn SDL_GL_SetSwapInterval(interval: i32) i32;

    /// `pub fn getSwapInterval() i32`
    pub const getSwapInterval = SDL_GL_GetSwapInterval;
    extern fn SDL_GL_GetSwapInterval() i32;

    /// `pub fn swapWindow(window: *Window) void`
    pub const swapWindow = SDL_GL_SwapWindow;
    extern fn SDL_GL_SwapWindow(window: *Window) void;

    pub fn getProcAddress(proc: [:0]const u8) ?*anyopaque {
        return SDL_GL_GetProcAddress(proc);
    }
    extern fn SDL_GL_GetProcAddress(proc: ?[*:0]const u8) ?*anyopaque;

    pub fn isExtensionSupported(extension: [:0]const u8) bool {
        return SDL_GL_ExtensionSupported(extension) != 0;
    }
    extern fn SDL_GL_ExtensionSupported(extension: ?[*:0]const u8) i32;

    pub fn createContext(window: *Window) Error!Context {
        return SDL_GL_CreateContext(window) orelse return makeError();
    }
    extern fn SDL_GL_CreateContext(window: *Window) ?Context;

    pub fn makeCurrent(window: *Window, context: Context) Error!void {
        if (SDL_GL_MakeCurrent(window, context) < 0) return makeError();
    }
    extern fn SDL_GL_MakeCurrent(window: *Window, context: Context) i32;

    /// `pub fn deleteContext(context: Context) void`
    pub const deleteContext = SDL_GL_DeleteContext;
    extern fn SDL_GL_DeleteContext(context: Context) void;
};
