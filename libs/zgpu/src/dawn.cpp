#include "dawn/native/DawnNative.h"
#include <assert.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DawnNativeInstanceImpl* DawnNativeInstance;

DawnNativeInstance dniCreate(void) {
    return reinterpret_cast<DawnNativeInstance>(new dawn::native::Instance());
}

void dniDestroy(DawnNativeInstance dni) {
    assert(dni);
    delete reinterpret_cast<dawn::native::Instance*>(dni);
}

WGPUInstance dniGetWgpuInstance(DawnNativeInstance dni) {
    assert(dni);
    return reinterpret_cast<dawn::native::Instance*>(dni)->Get();
}

void dniDiscoverDefaultAdapters(DawnNativeInstance dni) {
    assert(dni);
    dawn::native::Instance* instance = reinterpret_cast<dawn::native::Instance*>(dni);
    instance->DiscoverDefaultAdapters();
}

const DawnProcTable* dnGetProcs(void) {
    return &dawn::native::GetProcs();
}

#ifdef __cplusplus
}
#endif
