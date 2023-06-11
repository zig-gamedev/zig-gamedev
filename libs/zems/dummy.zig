// dummy interface for non emscripten builds
pub const is_emscripten = false;

pub const EmBool = enum(c_int) {
    true = 1,
    false = 0,
};
