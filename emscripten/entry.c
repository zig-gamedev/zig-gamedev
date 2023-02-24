// "borrowed" from https://github.com/floooh/pacman.zig/blob/main/src/emscripten/entry.c

#include <stdint.h>
#include <stdlib.h>

// Zig compiles C code with -fstack-protector-strong which requires the following two symbols
// which don't seem to be provided by the emscripten toolchain(?)
uintptr_t __stack_chk_guard = 0xABBABABA;
_Noreturn void __stack_chk_fail(void) { abort(); };

// emsc_main() is the Zig entry function in pacman.zig
extern void emsc_main(void);
int main() {
    emsc_main();
    return 0;
}
