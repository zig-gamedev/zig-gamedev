# JoltPhysicsC v0.0.1 - C API for Jolt Physics C++ library

Jolt Physics is a fast and modern physics library written in C++: https://github.com/jrouwe/JoltPhysics

This project aims to create high-performance, consistent and roboust C API for Jolt.

JoltPhysicsC is not yet complete but already usable, to get started please take a look on [tests](https://github.com/michal-z/zig-gamedev/blob/main/libs/zphysics/libs/JoltC/JoltPhysicsC_Tests.c).

Folder structure:

* `Jolt/` - contains complete, up-to date source code of Jolt Physics
* `JoltC/`
    * `JoltPhysicsC.h/` - C API header file
    * `JoltPhysicsC.cpp/` - C API implementation
    * `JoltPhysicsC_Extensions.cpp/` - some additional, low-level functions implemented for performance reasons
    * `JoltPhysicsC_Tests.c` - tests and sample code for our C API

```c
// Safe, lock protected way of accessing all bodies (use when you interact with Jolt from multiple threads)

JPC_BodyID body_ids[16]; // You can use `JPC_PhysicsSystem_GetMaxBodies()` to pre-allocate storage
uint32_t num_body_ids = 0;
JPC_PhysicsSystem_GetBodyIDs(physics_system, 16, &num_body_ids, &body_ids[0]);

for (uint32_t i = 0; i < num_body_ids; ++i)
{
    JPC_BodyLockRead lock;
    JPC_BodyLockRead_Lock(&lock, JPC_PhysicsSystem_GetBodyLockInterface(physics_system), body_ids[i]);
    if (lock.body)
    {
        // Body has been locked, you can safely use `JPC_Body_*()` functions.
    }
    JPC_BodyLockRead_Unlock(&lock);
}
```
```c
// Low-level, advanced way of accessing body data. Not protected by a lock, no function calls overhead.
// Use when you interact with Jolt only from one thread or when you are sure that `JPC_PhysicsSystem_Update()`
// has already completed execution in a given frame.

JPC_Body **bodies = JPC_PhysicsSystem_GetBodiesUnsafe(physics_system);

// Access a single body (get the body pointer from a body id).
{
    JPC_Body *sphere = JPC_TRY_GET_BODY(bodies, sphere_id);
    if (sphere)
    {
        sphere->friction = 0.2f;
    }
}

// Access all body pointers.
for (uint32_t i = 0; i < JPC_PhysicsSystem_GetNumBodies(physics_system); ++i)
{
    JPC_Body *body = bodies[i];
    if (JPC_IS_VALID_BODY_POINTER(body))
    {
        // Body pointer is valid (not freed) you can access the data.
    }
}
```
