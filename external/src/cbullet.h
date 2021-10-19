#pragma once

#define PL_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

typedef float plVector3[3];
typedef float plQuaternion[4];

#ifdef __cplusplus
extern "C" {
#endif

PL_DECLARE_HANDLE(plWorldHandle);
PL_DECLARE_HANDLE(plShapeHandle);
PL_DECLARE_HANDLE(plBodyHandle);

typedef void (*plDrawLineCallback)(const plVector3 from, const plVector3 to, const plVector3 color, void* user);
typedef void (*plErrorWarningCallback)(const char* str);

plWorldHandle plWorldCreate(void);
void plWorldDestroy(plWorldHandle handle);
void plWorldDebugSetDrawLineCallback(plWorldHandle handle, plDrawLineCallback callback, void* user);
void plWorldDebugSetErrorWarningCallback(plWorldHandle handle, plErrorWarningCallback callback);
void plWorldDebugDraw(plWorldHandle handle);

plShapeHandle plShapeCreateBox(float half_x, float half_y, float half_z);
plShapeHandle plShapeCreateSphere(float radius);
plShapeHandle plShapeCreatePlane(float nx, float ny, float nz, float d);
plShapeHandle plShapeCreateCapsule(float radius, float height, int up_axis);
void plShapeDestroy(plShapeHandle handle);
int plShapeGetType(plShapeHandle handle);

plBodyHandle plBodyCreate(
    plWorldHandle world_handle,
    float mass,
    const plVector3 transform[4],
    plShapeHandle shape_handle
);
void plBodyDestroy(plWorldHandle world_handle, plBodyHandle body_handle);
void plBodyGetGraphicsTransform(plBodyHandle handle, plVector3 transform[4]);

#ifdef __cplusplus
}
#endif
