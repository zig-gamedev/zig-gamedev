## virtual physics lab (Bullet physics test)

[Video showing demo in action](https://www.youtube.com/watch?v=GUaaXHSfDTE)

Demo consists of 4 scenes where user can interact and experiment with physics objects. In particular we show how to setup Newton's Cradle, simple machines with motors, hinges, sliders, etc. All basic properties like mass, friction, damping, gravity can be changed in real-time to observe how those affect object's behavior.

This demo shows how to use [Bullet](https://github.com/bulletphysics/bullet3) physics library from Zig. As part of the demo we have developed cbullet - a simple C API for Bullet physics library (which is written in C++):

* [cbullet.h](https://github.com/michal-z/zig-gamedev/blob/main/external/src/cbullet.h)
* [cbullet.cpp](https://github.com/michal-z/zig-gamedev/blob/main/external/src/cbullet.cpp)

![image](screenshot1.png)
![image](screenshot2.png)
![image](screenshot3.png)
