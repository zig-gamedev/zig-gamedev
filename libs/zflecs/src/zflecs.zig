const std = @import("std");
const c = @cImport(@cInclude("flecs.h"));

test "zflecs.basic" {
    const world = c.ecs_init();
    defer _ = c.ecs_fini(world);
}
