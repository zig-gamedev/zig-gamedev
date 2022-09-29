#include "dawn/native/DawnNative.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DawnNativeInstanceImpl* DawnNativeInstance;

DawnNativeInstance dniCreate(void) {
    return reinterpret_cast<DawnNativeInstance>(new dawn::native::Instance());
}

void dniDestroy(DawnNativeInstance instance) {
    assert(instance);
    delete reinterpret_cast<dawn::native::Instance*>(instance);
}

WGPUInstance dniGetWgpuInstance(DawnNativeInstance instance) {
    assert(instance);
    return reinterpret_cast<dawn::native::Instance*>(instance)->Get();
}

void dniDiscoverDefaultAdapters(DawnNativeInstance instance) {
    assert(instance);
    dawn::native::Instance* self = reinterpret_cast<dawn::native::Instance*>(instance);
    self->DiscoverDefaultAdapters();
}

/*
void dniEnableBackendValidation(DawnNativeInstance instance, bool enable) {
    assert(instance);
    dawn::native::Instance* self = reinterpret_cast<dawn::native::Instance*>(instance);
    self->EnableBackendValidation(enable);
    self->SetBackendValidationLevel(enable ? dawn::native::Full : dawn::native::Disabled);
}
*/

const DawnProcTable* dnGetProcs(void) {
    return &dawn::native::GetProcs();
}

#ifdef __cplusplus
}
#endif
