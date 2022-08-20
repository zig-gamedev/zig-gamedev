#include <dawn/native/DawnNative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DawnNativeInstanceImpl* DawnNativeInstance;

DawnNativeInstance dawnNativeCreateInstance(void) {
    return reinterpret_cast<DawnNativeInstance>(new dawn_native::Instance());
}

void dawnNativeDestroyInstance(DawnNativeInstance instance) {
    delete reinterpret_cast<dawn_native::Instance*>(instance);
}

WGPUInstance dawnNativeGetWgpuInstance(DawnNativeInstance instance) {
    return reinterpret_cast<dawn_native::Instance*>(instance)->Get();
}

void dawnNativeDiscoverDefaultAdapters(DawnNativeInstance instance) {
    dawn_native::Instance* self = reinterpret_cast<dawn_native::Instance*>(instance);
    self->DiscoverDefaultAdapters();
}

const DawnProcTable* dawnNativeGetProcs(void) {
    return &dawn_native::GetProcs();
}

#ifdef __cplusplus
}
#endif
