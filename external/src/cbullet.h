#pragma once

#define CBT_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

#ifdef __cplusplus
extern "C" {
#endif

CBT_DECLARE_HANDLE(cbtDynamicsWorldHandle);

cbtDynamicsWorldHandle cbtCreateDynamicsWorld();
void cbtDeleteDynamicsWorld(cbtDynamicsWorldHandle world);

#ifdef __cplusplus
}
#endif
