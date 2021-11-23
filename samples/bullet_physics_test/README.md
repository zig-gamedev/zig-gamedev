## virtual physics lab (Bullet physics test)

[Video showing demo in action](https://youtu.be/9Ri6xS2-9k8)

Demo consists of 4 scenes where user can interact and experiment with physics objects. In particular we show how to setup Newton's Cradle, simple machines with motors, hinges, sliders, etc. All basic properties like mass, friction, damping, gravity can be changed in real-time to observe how those affect object's behavior.

This demo shows how to use [Bullet](https://github.com/bulletphysics/bullet3) physics library in Zig language. As part of the effort we have developed cbullet - a simple C API for Bullet physics library:

* [cbullet.h](https://github.com/michal-z/zig-gamedev/blob/main/external/src/cbullet.h)
* [cbullet.cpp](https://github.com/michal-z/zig-gamedev/blob/main/external/src/cbullet.cpp)

Some features:
* Most collision shapes
* Rigid bodies
* Most constraint types
* Tries to minimize number of memory allocations
  * Multiple rigid bodies and motion states can be created with only one memory allocation
  * New physics objects can re-use existing memory
* Lots of error checks in debug builds

![image](screenshot1.png)
![image](screenshot2.png)
![image](screenshot3.png)
