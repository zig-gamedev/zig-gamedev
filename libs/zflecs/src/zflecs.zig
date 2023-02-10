const std = @import("std");
const options = @import("zflecs_options");
const c = @cImport({
    if (options.no_pipeline_support) @cDefine("FLECS_NO_PIPELINE", "");
    if (options.no_http_support) @cDefine("FLECS_NO_HTTP", "");
    @cInclude("flecs.h");
});

test "zflecs.basic" {
    const world = c.ecs_init();
    defer _ = c.ecs_fini(world);
}
