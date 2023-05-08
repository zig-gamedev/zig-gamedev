# JoltPhysicsC v0.0.6 - C API for Jolt Physics C++ library

[Jolt Physics](https://github.com/jrouwe/JoltPhysics) is a fast and modern physics library written in C++.

This project aims to provide high-performance, consistent and roboust C API for Jolt.

JoltPhysicsC is not yet complete but already usable, to get started please take a look at our [tests](https://github.com/michal-z/zig-gamedev/blob/main/libs/zphysics/libs/JoltC/JoltPhysicsC_Tests.c).

Folder structure:

* `Jolt/` - contains complete, up-to date source code of Jolt Physics
* `JoltC/`
    * `JoltPhysicsC.h` - C API header file
    * `JoltPhysicsC.cpp` - C API implementation
    * `JoltPhysicsC_Extensions.cpp` - some additional, low-level functions implemented for performance reasons
    * `JoltPhysicsC_Tests.c` - tests for our C API
