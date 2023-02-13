const std = @import("std");
//--------------------------------------------------------------------------------------------------
//
// Types
//
//--------------------------------------------------------------------------------------------------
pub const world_t = opaque {};
pub const poly_t = anyopaque;
pub const id_t = u64;
pub const entity_t = id_t;
pub const ftime_t = f32;
pub const size_t = i32;
pub const flags8_t = u8;
pub const flags16_t = u16;
pub const flags32_t = u32;
pub const flags64_t = u64;

const ID_CACHE_SIZE = 32;

pub const world_info_t = extern struct {
    last_component_id: entity_t,
    last_id: entity_t,
    min_id: entity_t,
    max_id: entity_t,
    delta_time_raw: f32,
    delta_time: f32,
    time_scale: f32,
    target_fps: f32,
    frame_time_total: f32,
    system_time_total: f32,
    emit_time_total: f32,
    merge_time_total: f32,
    world_time_total: f32,
    world_time_total_raw: f32,
    rematch_time_total: f32,
    frame_count_total: i64,
    merge_count_total: i64,
    rematch_count_total: i64,
    id_create_total: i64,
    id_delete_total: i64,
    table_create_total: i64,
    table_delete_total: i64,
    pipeline_build_count_total: i64,
    systems_ran_frame: i64,
    observers_ran_frame: i64,
    id_count: i32,
    tag_id_count: i32,
    component_id_count: i32,
    pair_id_count: i32,
    wildcard_id_count: i32,
    table_count: i32,
    tag_table_count: i32,
    trivial_table_count: i32,
    empty_table_count: i32,
    table_record_count: i32,
    table_storage_count: i32,
    cmd: extern struct {
        add_count: i64,
        remove_count: i64,
        delete_count: i64,
        clear_count: i64,
        set_count: i64,
        get_mut_count: i64,
        modified_count: i64,
        other_count: i64,
        discard_count: i64,
        batched_entity_count: i64,
        batched_command_count: i64,
    },
    name_prefix: [*:0]const u8,
};

pub const entity_desc_t = extern struct {
    _canary: i32 = 0,
    id: entity_t = 0,
    name: ?[*:0]const u8 = null,
    sep: ?[*:0]const u8 = null,
    root_sep: ?[*:0]const u8 = null,
    symbol: ?[*:0]const u8 = null,
    use_low_id: bool = false,
    add: [ID_CACHE_SIZE]id_t = [_]id_t{0} ** ID_CACHE_SIZE,
    add_expr: ?[*:0]const u8 = null,
};

pub const fini_action_t = *const fn (*world_t, ?*anyopaque) callconv(.C) void;
//--------------------------------------------------------------------------------------------------
//
// Creation & Deletion
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_init() *world_t;
/// `pub fn init() *world_t`
pub const init = ecs_init;

extern fn ecs_mini() *world_t;
/// `pub fn mini() *world_t`
pub const mini = ecs_mini;

extern fn ecs_fini(world: *world_t) i32;
/// `pub fn fini(world: *world_t) i32`
pub const fini = ecs_fini;

extern fn ecs_is_fini(world: *const world_t) bool;
/// `pub fn is_fini(world: *const world_t) bool`
pub const is_fini = ecs_is_fini;

extern fn ecs_atfini(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) bool;
/// `pub fn atfini(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) bool`
pub const atfini = ecs_atfini;
//--------------------------------------------------------------------------------------------------
//
// Frame functions
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_frame_begin(world: *world_t, delta_time: ftime_t) ftime_t;
/// `pub fn frame_begin(world: *world_t, delta_time: ftime_t) ftime_t`
pub const frame_begin = ecs_frame_begin;

extern fn ecs_frame_end(world: *world_t) void;
/// `pub fn frame_end(world: *world_t) void`
pub const frame_end = ecs_frame_end;

extern fn ecs_run_post_frame(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) void;
/// `pub fn run_post_frame(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) void`
pub const run_post_frame = ecs_run_post_frame;

extern fn ecs_quit(world: *world_t) void;
/// `pub fn quit(world: *world_t) void`
pub const quit = ecs_quit;

extern fn ecs_should_quit(world: *const world_t) bool;
/// `pub fn should_quit(world: *const world_t) bool`
pub const should_quit = ecs_should_quit;

extern fn ecs_measure_frame_time(world: *world_t, enable: bool) void;
/// `pub fn measure_frame_time(world: *world_t, enable: bool) void`
pub const measure_frame_time = ecs_measure_frame_time;

extern fn ecs_measure_system_time(world: *world_t, enable: bool) void;
/// `pub fn measure_system_time(world: *world_t, enable: bool) void`
pub const measure_system_time = ecs_measure_system_time;

extern fn ecs_set_target_fps(world: *world_t, fps: ftime_t) void;
/// `pub fn set_target_fps(world: *world_t, fps: ftime_t) void`
pub const set_target_fps = ecs_set_target_fps;
//--------------------------------------------------------------------------------------------------
//
// Commands
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_readonly_begin(world: *world_t) bool;
/// `pub fn readonly_begin(world: *world_t) bool`
pub const readonly_begin = ecs_readonly_begin;

extern fn ecs_readonly_end(world: *world_t) void;
/// `pub fn readonly_end(world: *world_t) void`
pub const readonly_end = ecs_readonly_end;

extern fn ecs_merge(world: *world_t) void;
/// `pub fn merge(world: *world_t) void`
pub const merge = ecs_merge;

extern fn ecs_defer_begin(world: *world_t) bool;
/// `pub fn defer_begin(world: *world_t) bool`
pub const defer_begin = ecs_defer_begin;

extern fn ecs_is_deferred(world: *const world_t) bool;
/// `pub fn is_deferred(world: *const world_t) bool`
pub const is_deferred = ecs_is_deferred;

extern fn ecs_defer_end(world: *world_t) bool;
/// `pub fn defer_end(world: *world_t) bool`
pub const defer_end = ecs_defer_end;

extern fn ecs_defer_suspend(world: *world_t) void;
/// `pub fn defer_suspend(world: *world_t) void`
pub const defer_suspend = ecs_defer_suspend;

extern fn ecs_defer_resume(world: *world_t) void;
/// `pub fn defer_resume(world: *world_t) void`
pub const defer_resume = ecs_defer_resume;

extern fn ecs_set_automerge(world: *world_t, automerge: bool) void;
/// `pub fn set_automerge(world: *world_t, automerge: bool) void`
pub const set_automerge = ecs_set_automerge;

extern fn ecs_set_stage_count(world: *world_t, stages: i32) void;
/// `pub fn set_stage_count(world: *world_t, stages: i32) void`
pub const set_stage_count = ecs_set_stage_count;

extern fn ecs_get_stage_count(world: *const world_t) i32;
/// `pub fn get_stage_count(world: *const world_t) i32`
pub const get_stage_count = ecs_get_stage_count;

extern fn ecs_get_stage_id(world: *const world_t) i32;
/// `pub fn get_stage_id(world: *const world_t) i32`
pub const get_stage_id = ecs_get_stage_id;

extern fn ecs_get_stage(world: *const world_t, stage_id: i32) *world_t;
/// `pub fn get_stage(world: *const world_t, stage_id: i32) *world_t`
pub const get_stage = ecs_get_stage;

extern fn ecs_stage_is_readonly(world: *const world_t) bool;
/// `pub fn stage_is_readonly(world: *const world_t) bool`
pub const stage_is_readonly = ecs_stage_is_readonly;

extern fn ecs_async_stage_new(world: *world_t) *world_t;
/// `pub fn async_stage_new(world: *world_t) *world_t`
pub const async_stage_new = ecs_async_stage_new;

extern fn ecs_async_stage_free(world: *world_t) *world_t;
/// `pub fn async_stage_free(world: *world_t) *world_t`
pub const async_stage_free = ecs_async_stage_free;

extern fn ecs_stage_is_async(world: *const world_t) bool;
/// `pub fn stage_is_async(world: *const world_t) bool`
pub const stage_is_async = ecs_stage_is_async;
//--------------------------------------------------------------------------------------------------
//
// Misc
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_set_context(world: *world_t, ctx: ?*anyopaque) void;
/// `pub fn set_context(world: *world_t, ctx: ?*anyopaque) void`
pub const set_context = ecs_set_context;

extern fn ecs_get_context(world: *const world_t) ?*anyopaque;
/// `pub fn get_context(world: *const world_t) ?*anyopaque`
pub const get_context = ecs_get_context;

extern fn ecs_get_world_info(world: *const world_t) *const world_info_t;
/// `pub fn ecs_get_world_info(world: *const world_t) *const world_info_t`
pub const get_world_info = ecs_get_world_info;

extern fn ecs_dim(world: *world_t, entity_count: i32) void;
/// `pub fn dim(world: *world_t, entity_count: i32) void`
pub const dim = ecs_dim;

extern fn ecs_set_entity_range(world: *world_t, id_start: entity_t, id_end: entity_t) void;
/// `pub fn set_entity_range(world: *world_t, id_start: entity_t, id_end: entity_t) void`
pub const set_entity_range = ecs_set_entity_range;

extern fn ecs_enable_range_check(world: *world_t, enable: bool) bool;
/// `pub fn enable_range_check(world: *world_t, enable: bool) bool`
pub const enable_range_check = ecs_enable_range_check;

extern fn ecs_run_aperiodic(world: *world_t, flags: flags32_t) void;
/// `pub fn run_aperiodic(world: *world_t, flags: flags32_t) void`
pub const run_aperiodic = ecs_run_aperiodic;

extern fn ecs_delete_empty_tables(
    world: *world_t,
    id: id_t,
    clear_generation: u16,
    delete_generation: u16,
    min_id_count: i32,
    time_budget_seconds: f64,
) i32;
/// ```
/// pub fn delete_empty_tables(
///     world: *world_t,
///     id: id_t,
///     clear_generation: u16,
///     delete_generation: u16,
///     min_id_count: i32,
///     time_budget_seconds: f64,
/// ) i32;
/// ```
pub const delete_empty_tables = ecs_delete_empty_tables;

extern fn ecs_make_pair(first: entity_t, second: entity_t) id_t;
/// `pub fn make_pair(first: entity_t, second: entity_t) id_t`
pub const make_pair = ecs_make_pair;
//--------------------------------------------------------------------------------------------------
//
// Functions for creating and deleting entities.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_new_id(world: *world_t) entity_t;
/// `pub fn new_id(world: *world_t) entity_t`
pub const new_id = ecs_new_id;

extern fn ecs_new_low_id(world: *world_t) entity_t;
/// `pub fn new_low_id(world: *world_t) entity_t`
pub const new_low_id = ecs_new_low_id;

extern fn ecs_new_w_id(world: *world_t, id: id_t) entity_t;
/// `pub fn new_w_id(world: *world_t, id: id_t) entity_t`
pub const new_w_id = ecs_new_w_id;

extern fn ecs_entity_init(world: *world_t, desc: *const entity_desc_t) entity_t;
/// `pub fn entity_init(world: *world_t, desc: *const entity_desc_t) entity_t`
pub const entity_init = ecs_entity_init;

extern fn ecs_bulk_new_w_id(world: *world_t, id: id_t, count: i32) [*]const entity_t;
/// `pub fn bulk_new_w_id(world: *world_t, id: id_t, count: i32) [*]const entity_t`
pub const bulk_new_w_id = ecs_bulk_new_w_id;

extern fn ecs_clone(world: *world_t, dst: entity_t, src: entity_t, copy_value: bool) entity_t;
/// `pub fn clone(world: *world_t, dst: entity_t, src: entity_t, copy_value: bool) entity_t`
pub const clone = ecs_clone;

extern fn ecs_delete(world: *world_t, entity: entity_t) void;
/// `pub fn delete(world: *world_t, entity: entity_t) void`
pub const delete = ecs_delete;

extern fn ecs_delete_with(world: *world_t, id: id_t) void;
/// `pub fn delete_with(world: *world_t, id: id_t) void`
pub const delete_with = ecs_delete_with;
//--------------------------------------------------------------------------------------------------
fn IdHandle(comptime _: type) type {
    return struct {
        var handle: id_t = 0;
    };
}
pub inline fn idof(comptime T: type) id_t {
    return id_handle_ptr(T).*;
}
inline fn id_handle_ptr(comptime T: type) *id_t {
    return comptime &IdHandle(T).handle;
}
//--------------------------------------------------------------------------------------------------
comptime {
    _ = @import("tests.zig");
}
//--------------------------------------------------------------------------------------------------
