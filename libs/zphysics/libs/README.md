# JoltPhysicsC v0.0.1 - C API for Jolt Physics C++ library

Jolt Physics is a fast and modern physics library written in C++: https://github.com/jrouwe/JoltPhysics

This project aims to create high-performance, consistent and roboust C API for Jolt.

Folder structure:

* `Jolt/` - contains complete, up-to date source code of Jolt Physics
* `JoltC/`
    * `JoltPhysicsC.h/` - C API header file
    * `JoltPhysicsC.cpp/` - C API implementation
    * `JoltPhysicsC_Extensions.cpp/` - Some additional, low-level functions implemented for performance reasons
    * `JoltPhysicsC_Tests.c` - tests and sample code for our C API
