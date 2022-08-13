#ifndef MACH_DAWNNATIVE_C_H_
#define MACH_DAWNNATIVE_C_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MACH_DAWNNATIVE_C_SHARED_LIBRARY)
#    if defined(_WIN32)
#        if defined(MACH_DAWNNATIVE_C_IMPLEMENTATION)
#            define MACH_EXPORT __declspec(dllexport)
#        else
#            define MACH_EXPORT __declspec(dllimport)
#        endif
#    else  // defined(_WIN32)
#        if defined(MACH_DAWNNATIVE_C_IMPLEMENTATION)
#            define MACH_EXPORT __attribute__((visibility("default")))
#        else
#            define MACH_EXPORT
#        endif
#    endif  // defined(_WIN32)
#else       // defined(MACH_DAWNNATIVE_C_SHARED_LIBRARY)
#    define MACH_EXPORT
#endif  // defined(MACH_DAWNNATIVE_C_SHARED_LIBRARY)

#include <dawn/webgpu.h>
#include <dawn/dawn_proc_table.h>

typedef struct MachDawnNativeInstanceImpl* MachDawnNativeInstance;

MACH_EXPORT MachDawnNativeInstance machDawnNativeInstance_init(void);
MACH_EXPORT void machDawnNativeInstance_deinit(MachDawnNativeInstance);
MACH_EXPORT WGPUInstance machDawnNativeInstance_get(MachDawnNativeInstance instance);
MACH_EXPORT void machDawnNativeInstance_discoverDefaultAdapters(MachDawnNativeInstance);

// Backend-agnostic API for dawn_native
MACH_EXPORT const DawnProcTable* machDawnNativeGetProcs();

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // MACH_DAWNNATIVE_C_H_
