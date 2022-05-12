## textured quad (wgpu)

This sample demonstrates how to display textured quad and how to generate a mipmap chain. Simple UI lets user change currently displayed mipmap level.


Mipmap generator takes ~260 micro sec. to generate all mips for 1024x1024 texture when running on GTX 1660.

It is very easy to use, just one call:

```
gfx_ctx.generateMipmaps(arena, cmd_encoder, texture_handle);
```

![image](screenshot.png)
