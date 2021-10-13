#pragma once

#define PL_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

#ifdef PL_USE_DOUBLE_PRECISION
typedef double plReal;
#else
typedef float plReal;
#endif

typedef plReal plVector3[3];
typedef plReal plQuaternion[4];

#ifdef __cplusplus
extern "C" {
#endif

PL_DECLARE_HANDLE(plDynamicsWorldHandle);

plDynamicsWorldHandle plCreateDynamicsWorld();
void plDeleteDynamicsWorld(plDynamicsWorldHandle world);

#ifdef __cplusplus
}
#endif
