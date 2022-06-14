## bullet physics test (wgpu)

Some info:
* Works on Windows, Linux and Mac. Uses Bullet for physics and native wgpu for rendering.
* `cbullet` and `zbullet` libraries have been developed to make it work nicely with Zig. Please see [here](https://github.com/michal-z/zig-gamedev/tree/main/libs/zbullet) for the details.
* Comes with 4 example 'scenes'. User can add more scenes by implementing single `setup()` function. This allows testing various physics effects/setups.
* Multi-threading is enabled by default.

See demo in action: [video](https://www.youtube.com/watch?v=XUuPGigPKSI).

![image](screenshot.jpg)
