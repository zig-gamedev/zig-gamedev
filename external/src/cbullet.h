#pragma once

#define PL_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

typedef float plVector3[3];
typedef float plQuaternion[4];

#ifdef __cplusplus
extern "C" {
#endif

PL_DECLARE_HANDLE(plWorldHandle);

typedef void (*plDrawLineCallback)(plVector3 from, plVector3 to, plVector3 color);
typedef void (*plErrorWarningCallback)(const char* str);

plWorldHandle plWorldCreate(void);
void plWorldDestroy(plWorldHandle world_handle);

void plWorldDebugSetDrawLineCallback(plWorldHandle world_handle, plDrawLineCallback callback);
void plWorldDebugSetErrorWarningCallback(plWorldHandle world_handle, plErrorWarningCallback callback);

#ifdef __cplusplus
}
#endif
