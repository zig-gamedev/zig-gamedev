const std = @import("std");
const options = @import("zflecs_options");
const c = @cImport({
    if (!options.pipeline_support) @cDefine("FLECS_NO_PIPELINE", "");
    if (!options.http_support) @cDefine("FLECS_NO_HTTP", "");
    if (!options.http_support) @cDefine("FLECS_NO_REST", "");
    if (!options.http_support) @cDefine("FLECS_NO_JOURNAL", "");
    @cInclude("flecs.h");
});

test {
    const world = c.ecs_init();
    defer _ = c.ecs_fini(world);
}
