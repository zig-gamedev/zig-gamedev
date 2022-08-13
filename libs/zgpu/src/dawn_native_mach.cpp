#include <dawn/native/DawnNative.h>
#include "dawn_native_mach.h"

#ifdef __cplusplus
extern "C" {
#endif

MACH_EXPORT MachDawnNativeInstance machDawnNativeInstance_init(void) {
    return reinterpret_cast<MachDawnNativeInstance>(new dawn_native::Instance());
}

MACH_EXPORT void machDawnNativeInstance_deinit(MachDawnNativeInstance instance) {
    delete reinterpret_cast<dawn_native::Instance*>(instance);
}

MACH_EXPORT WGPUInstance machDawnNativeInstance_get(MachDawnNativeInstance instance) {
    return reinterpret_cast<dawn_native::Instance*>(instance)->Get();
}

MACH_EXPORT void machDawnNativeInstance_discoverDefaultAdapters(MachDawnNativeInstance instance) {
    dawn_native::Instance* self = reinterpret_cast<dawn_native::Instance*>(instance);
    self->DiscoverDefaultAdapters();
}

MACH_EXPORT const DawnProcTable* machDawnNativeGetProcs() {
    return &dawn_native::GetProcs();
}

#ifdef __cplusplus
} // extern "C"
#endif
