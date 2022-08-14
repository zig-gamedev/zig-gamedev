#pragma once

#include <dawn/webgpu.h>
#include <dawn/dawn_proc_table.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DawnNativeInstanceImpl* DawnNativeInstance;

DawnNativeInstance dawnNativeCreateInstance(void);
void dawnNativeDestroyInstance(DawnNativeInstance native_instance);
WGPUInstance dawnNativeGetWgpuInstance(DawnNativeInstance native_instance);
void dawnNativeDiscoverDefaultAdapters(DawnNativeInstance native_instance);

const DawnProcTable* dawnNativeGetProcs(void);

#ifdef __cplusplus
}
#endif
