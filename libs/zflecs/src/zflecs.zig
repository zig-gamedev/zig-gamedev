const std = @import("std");
//--------------------------------------------------------------------------------------------------
//
// Types
//
//--------------------------------------------------------------------------------------------------
pub const world_t = opaque {};
pub const table_t = opaque {};
pub const table_record_t = opaque {};
pub const id_record_t = opaque {};
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

pub const record_t = extern struct {
    idr: *id_record_t,
    table: *table_t,
    row: u32,
};

pub const ref_t = extern struct {
    entity: entity_t,
    id: entity_t,
    tr: *table_record_t,
    record: *record_t,
};

pub const type_t = extern struct {
    array: [*]id_t,
    count: i32,
};

pub const fini_action_t = *const fn (*world_t, ?*anyopaque) callconv(.C) void;

pub const time_t = extern struct {
    sec: u32,
    nanosec: u32,
};
pub const os_thread_t = usize;
pub const os_cond_t = usize;
pub const os_mutex_t = usize;
pub const os_dl_t = usize;
pub const os_sock_t = usize;
pub const os_thread_id_t = u64;
pub const os_proc_t = *const fn () callconv(.C) void;
pub const os_api_init_t = *const fn () callconv(.C) void;
pub const os_api_fini_t = *const fn () callconv(.C) void;
pub const os_api_malloc_t = *const fn (size_t) callconv(.C) ?*anyopaque;
pub const os_api_free_t = *const fn (?*anyopaque) callconv(.C) void;
pub const os_api_realloc_t = *const fn (?*anyopaque, size_t) callconv(.C) ?*anyopaque;
pub const os_api_calloc_t = *const fn (size_t) callconv(.C) ?*anyopaque;
pub const os_api_strdup_t = *const fn ([*:0]const u8) callconv(.C) [*c]u8;
pub const os_thread_callback_t = *const fn (?*anyopaque) callconv(.C) ?*anyopaque;
pub const os_api_thread_new_t = *const fn (os_thread_callback_t, ?*anyopaque) callconv(.C) os_thread_t;
pub const os_api_thread_join_t = *const fn (os_thread_t) callconv(.C) ?*anyopaque;
pub const os_api_thread_self_t = *const fn () callconv(.C) os_thread_id_t;
pub const os_api_ainc_t = *const fn (*i32) callconv(.C) i32;
pub const os_api_lainc_t = *const fn (*i64) callconv(.C) i64;
pub const os_api_mutex_new_t = *const fn () callconv(.C) os_mutex_t;
pub const os_api_mutex_lock_t = *const fn (os_mutex_t) callconv(.C) void;
pub const os_api_mutex_unlock_t = *const fn (os_mutex_t) callconv(.C) void;
pub const os_api_mutex_free_t = *const fn (os_mutex_t) callconv(.C) void;
pub const os_api_cond_new_t = *const fn () callconv(.C) os_cond_t;
pub const os_api_cond_free_t = *const fn (os_cond_t) callconv(.C) void;
pub const os_api_cond_signal_t = *const fn (os_cond_t) callconv(.C) void;
pub const os_api_cond_broadcast_t = *const fn (os_cond_t) callconv(.C) void;
pub const os_api_cond_wait_t = *const fn (os_cond_t, os_mutex_t) callconv(.C) void;
pub const os_api_sleep_t = *const fn (i32, i32) callconv(.C) void;
pub const os_api_enable_high_timer_resolution_t = *const fn (bool) callconv(.C) void;
pub const os_api_get_time_t = *const fn (*time_t) callconv(.C) void;
pub const os_api_now_t = *const fn () callconv(.C) u64;
pub const os_api_log_t = *const fn (i32, [*c]const u8, i32, [*:0]const u8) callconv(.C) void;
pub const os_api_abort_t = *const fn () callconv(.C) void;
pub const os_api_dlopen_t = *const fn ([*:0]const u8) callconv(.C) os_dl_t;
pub const os_api_dlproc_t = *const fn (os_dl_t, [*:0]const u8) callconv(.C) os_proc_t;
pub const os_api_dlclose_t = *const fn (os_dl_t) callconv(.C) void;
pub const os_api_module_to_path_t = *const fn ([*:0]const u8) callconv(.C) [*:0]u8;

const os_api_t = extern struct {
    init_: os_api_init_t,
    fini_: os_api_fini_t,
    malloc_: os_api_malloc_t,
    realloc_: os_api_realloc_t,
    calloc_: os_api_calloc_t,
    free_: os_api_free_t,
    strdup_: os_api_strdup_t,
    thread_new_: os_api_thread_new_t,
    thread_join_: os_api_thread_join_t,
    thread_self_: os_api_thread_self_t,
    ainc_: os_api_ainc_t,
    adec_: os_api_ainc_t,
    lainc_: os_api_lainc_t,
    ladec_: os_api_lainc_t,
    mutex_new_: os_api_mutex_new_t,
    mutex_free_: os_api_mutex_free_t,
    mutex_lock_: os_api_mutex_lock_t,
    mutex_unlock_: os_api_mutex_lock_t,
    cond_new_: os_api_cond_new_t,
    cond_free_: os_api_cond_free_t,
    cond_signal_: os_api_cond_signal_t,
    cond_broadcast_: os_api_cond_broadcast_t,
    cond_wait_: os_api_cond_wait_t,
    sleep_: os_api_sleep_t,
    now_: os_api_now_t,
    get_time_: os_api_get_time_t,
    log_: os_api_log_t,
    abort_: os_api_abort_t,
    dlopen_: os_api_dlopen_t,
    dlproc_: os_api_dlproc_t,
    dlclose_: os_api_dlclose_t,
    module_to_dl_: os_api_module_to_path_t,
    module_to_etc_: os_api_module_to_path_t,
    log_level_: i32,
    log_indent_: i32,
    log_last_error_: i32,
    log_last_timestamp_: i64,
    flags_: flags32_t,
};

extern var ecs_os_api: os_api_t;

pub fn os_free(ptr: ?*anyopaque) void {
    ecs_os_api.free_(ptr);
}

pub const stack_page_t = opaque {};

pub const stack_cursor_t = extern struct {
    cur: ?*stack_page_t,
    sp: i16,
};

pub const iter_cache_t = extern struct {
    stack_cursor: stack_cursor_t,
    used: flags8_t,
    allocated: flags8_t,
};

pub const term_iter_t = extern struct {
    term: term_t,
    self_index: ?*id_record_t,
    set_index: ?*id_record_t,
    cur: ?*id_record_t,
    it: table_cache_iter_t,
    index: i32,
    observed_table_count: i32,
    table: ?*table_t,
    cur_match: i32,
    match_count: i32,
    last_column: i32,
    empty_tables: bool,
    id: id_t,
    column: i32,
    subject: entity_t,
    size: size_t,
    ptr: ?*anyopaque,
};

pub const mixins_t = opaque {};

pub const header_t = extern struct {
    magic: i32,
    type: i32,
    mixins: ?*mixins_t,
};

pub const iter_init_action_t = *const fn (?*const world_t, ?*const poly_t, [*c]iter_t, [*c]term_t) callconv(.C) void;

pub const poly_dtor_t = *const fn (?*poly_t) callconv(.C) void;

pub const iterable_t = extern struct {
    init: iter_init_action_t,
};

pub const filter_t = extern struct {
    hdr: header_t,
    terms: [*c]term_t,
    term_count: i32,
    field_count: i32,
    owned: bool,
    terms_owned: bool,
    flags: flags32_t,
    variable_names: [1][*c]u8,
    entity: entity_t,
    world: ?*world_t,
    iterable: iterable_t,
    dtor: poly_dtor_t,
};

pub const table_cache_hdr_t = opaque {};

pub const table_cache_iter_t = extern struct {
    cur: ?*table_cache_hdr_t,
    next: ?*table_cache_hdr_t,
    next_list: ?*table_cache_hdr_t,
};

pub const iter_kind_t = enum(i32) {
    EvalCondition,
    EvalTables,
    EvalChain,
    EvalNone,
};

pub const filter_iter_t = extern struct {
    filter: [*c]const filter_t,
    kind: iter_kind_t,
    term_iter: term_iter_t,
    matches_left: i32,
    pivot_term: i32,
};

pub const query_t = opaque {};
pub const query_table_node_t = opaque {};
pub const rule_t = opaque {};
pub const vector_t = opaque {};

pub const query_iter_t = extern struct {
    query: ?*query_t,
    node: ?*query_table_node_t,
    prev: ?*query_table_node_t,
    last: ?*query_table_node_t,
    sparse_smallest: i32,
    sparse_first: i32,
    bitset_first: i32,
    skip_count: i32,
};
pub const rule_op_ctx_t = opaque {};

pub const rule_iter_t = extern struct {
    rule: ?*const rule_t,
    registers: [*c]var_t,
    op_ctx: ?*rule_op_ctx_t,
    columns: [*c]i32,
    entity: entity_t,
    redo: bool,
    op: i32,
    sp: i32,
};

pub const snapshot_iter_t = extern struct {
    filter: filter_t,
    tables: ?*vector_t,
    index: i32,
};

pub const page_iter_t = extern struct {
    offset: i32,
    limit: i32,
    remaining: i32,
};

pub const worker_iter_t = extern struct {
    index: i32,
    count: i32,
};

pub const iter_private_t = extern struct {
    iter: extern union {
        term: term_iter_t,
        filter: filter_iter_t,
        query: query_iter_t,
        rule: rule_iter_t,
        snapshot: snapshot_iter_t,
        page: page_iter_t,
        worker: worker_iter_t,
    },
    entity_iter: ?*anyopaque,
    cache: iter_cache_t,
};

pub const inout_kind_t = enum(i32) {
    InOutDefault,
    InOutNone,
    InOut,
    In,
    Out,
};

pub const oper_kind_t = enum(i32) {
    And,
    Or,
    Not,
    Optional,
    AndFrom,
    OrFrom,
    NotFrom,
};

pub const term_id_t = extern struct {
    id: entity_t,
    name: [*c]u8,
    trav: entity_t,
    flags: flags32_t,
};

pub const term_t = extern struct {
    id: id_t,
    src: term_id_t,
    first: term_id_t,
    second: term_id_t,
    inout: inout_kind_t,
    oper: oper_kind_t,
    id_flags: id_t,
    name: [*c]u8,
    field_index: i32,
    idr: ?*id_record_t,
    move: bool,
};

pub const table_range_t = extern struct {
    table: ?*table_t,
    offset: i32,
    count: i32,
};
pub const var_t = extern struct {
    range: table_range_t,
    entity: entity_t,
};

pub const xtor_t = *const fn (*anyopaque, i32, *const type_info_t) callconv(.C) void;
pub const copy_t = *const fn (*anyopaque, *const anyopaque, i32, *const type_info_t) callconv(.C) void;
pub const move_t = *const fn (*anyopaque, *anyopaque, i32, *const type_info_t) callconv(.C) void;

// TODO: Compiler bug (dependency loop detected)
//pub const iter_next_action_t = *const fn (*iter_t) callconv(.C) bool;
pub const iter_next_action_t = *const fn (*anyopaque) callconv(.C) bool;

// TODO: Compiler bug (dependency loop detected)
//pub const iter_action_t = *const fn (*iter_t) callconv(.C) void;
pub const iter_action_t = *const fn (*anyopaque) callconv(.C) void;

// TODO: Compiler bug (dependency loop detected)
//pub const iter_fini_action_t = *const fn (*iter_t) callconv(.C) void;
pub const iter_fini_action_t = *const fn (*anyopaque) callconv(.C) void;

pub const ctx_free_t = *const fn (?*anyopaque) callconv(.C) void;

pub const iter_t = extern struct {
    world: ?*world_t,
    real_world: ?*world_t,
    entities: [*c]entity_t,
    ptrs: [*c]?*anyopaque,
    sizes: [*c]size_t,
    table: ?*table_t,
    other_table: ?*table_t,
    ids: [*c]id_t,
    variables: [*c]var_t,
    columns: [*c]i32,
    sources: [*c]entity_t,
    match_indices: [*c]i32,
    references: [*c]ref_t,
    constrained_vars: flags64_t,
    group_id: u64,
    field_count: i32,
    system: entity_t,
    event: entity_t,
    event_id: id_t,
    terms: [*c]term_t,
    table_count: i32,
    term_index: i32,
    variable_count: i32,
    variable_names: [*c][*c]u8,
    param: ?*anyopaque,
    ctx: ?*anyopaque,
    binding_ctx: ?*anyopaque,
    delta_time: f32,
    delta_system_time: f32,
    frame_offset: i32,
    offset: i32,
    count: i32,
    instance_count: i32,
    flags: flags32_t,
    interrupted_by: entity_t,
    priv: iter_private_t,
    next: iter_next_action_t,
    callback: iter_action_t,
    fini: iter_fini_action_t,
    chain_it: [*c]iter_t,
};

pub const type_hooks_t = extern struct {
    ctor: ?xtor_t = null,
    dtor: ?xtor_t = null,
    copy: ?copy_t = null,
    move: ?move_t = null,
    copy_ctor: ?copy_t = null,
    move_ctor: ?move_t = null,
    ctor_move_dtor: ?move_t = null,
    move_dtor: ?move_t = null,
    on_add: ?iter_action_t = null,
    on_set: ?iter_action_t = null,
    on_remove: ?iter_action_t = null,
    ctx: ?*anyopaque = null,
    binding_ctx: ?*anyopaque = null,
    ctx_free: ?ctx_free_t = null,
    binding_ctx_free: ?ctx_free_t = null,
};

pub const type_info_t = extern struct {
    size: size_t,
    alignment: size_t,
    hooks: type_hooks_t = .{},
    component: entity_t = 0,
    name: ?[*:0]const u8 = null,
};

pub const component_desc_t = extern struct {
    _canary: i32 = 0,
    entity: entity_t,
    type: type_info_t,
};
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
//
// Functions for adding and removing components.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_add_id(world: *world_t, entity: entity_t, id: id_t) void;
/// `pub fn add_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const add_id = ecs_add_id;

extern fn ecs_remove_id(world: *world_t, entity: entity_t, id: id_t) void;
/// `pub fn remove_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const remove_id = ecs_remove_id;

extern fn ecs_override_id(world: *world_t, entity: entity_t, id: id_t) void;
/// `pub fn override_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const override_id = ecs_remove_id;

extern fn ecs_clear(world: *world_t, entity: entity_t) void;
/// `pub fn clear(world: *world_t, entity: entity_t) void`
pub const clear = ecs_clear;

extern fn ecs_remove_all(world: *world_t, id: id_t) void;
/// `pub fn remove_all(world: *world_t, id: id_t) void`
pub const remove_all = ecs_remove_all;

extern fn ecs_set_with(world: *world_t, id: id_t) id_t;
/// `pub fn set_with(world: *world_t, id: id_t) void`
pub const set_with = ecs_set_with;

extern fn ecs_get_with(world: *const world_t) id_t;
/// `pub fn get_with(world: *const world_t) id_t`
pub const get_with = ecs_get_with;
//--------------------------------------------------------------------------------------------------
//
// Functions for enabling/disabling entities and components.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_enable(world: *world_t, entity: entity_t, enable: bool) void;
/// `pub fn enable(world: *const world_t, entity: entity_t, enable: bool) void`
pub const enable = ecs_enable;

extern fn ecs_enable_id(world: *world_t, entity: entity_t, id: id_t, enable: bool) void;
/// `pub fn enable_id(world: *const world_t, entity: entity_t, id: id_t, enable: bool) void`
pub const enable_id = ecs_enable_id;

extern fn ecs_is_enabled_id(world: *const world_t, entity: entity_t, id: id_t) bool;
/// `pub fn is_enabled_id(world: *const world_t, entity: entity_t, id: id_t) bool`
pub const is_enabled_id = ecs_is_enabled_id;
//--------------------------------------------------------------------------------------------------
//
// Functions for getting/setting components.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_get_id(world: *const world_t, entity: entity_t, id: id_t) ?*const anyopaque;
/// `pub fn get_id(world: *const world_t, entity: entity_t, id: id_t) ?*const anyopaque`
pub const get_id = ecs_get_id;

extern fn ecs_ref_init_id(world: *const world_t, entity: entity_t, id: id_t) ref_t;
/// `pub fn ref_init_id(world: *const world_t, entity: entity_t, id: id_t) ref_t`
pub const ref_init_id = ecs_ref_init_id;

extern fn ecs_ref_get_id(world: *const world_t, ref: *ref_t, id: id_t) ?*anyopaque;
/// `pub fn ref_get_id(world: *const world_t, ref: *ref_t, id: id_t) ?*anyopaque`
pub const ref_get_id = ecs_ref_get_id;

extern fn ecs_ref_update(world: *const world_t, ref: *ref_t) void;
/// `pub fn ref_get_id(world: *const world_t, ref: *ref_t) void`
pub const ref_update = ecs_ref_update;

extern fn ecs_get_mut_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque;
/// `pub fn get_mut_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque`
pub const get_mut_id = ecs_get_mut_id;

extern fn ecs_write_begin(world: *world_t, entity: entity_t) ?*record_t;
/// `pub fn write_begin(world: *world_t, entity: entity_t) ?*record_t`
pub const write_begin = ecs_write_begin;

extern fn ecs_write_end(record: *record_t) void;
/// `pub fn write_end(record: *record_t) void`
pub const write_end = ecs_write_end;

extern fn ecs_read_begin(world: *world_t, entity: entity_t) ?*const record_t;
/// `pub fn read_begin(world: *world_t, entity: entity_t) ?*const record_t`
pub const read_begin = ecs_read_begin;

extern fn ecs_read_end(record: *const record_t) void;
/// `pub fn read_end(record: *const record_t) void`
pub const read_end = ecs_read_end;

extern fn ecs_record_get_id(world: *world_t, record: *const record_t, id: id_t) ?*const anyopaque;
/// `pub fn record_get_id(world: *world_t, record: *const record_t, id: id_t) ?*const anyopaque`
pub const record_get_id = ecs_record_get_id;

extern fn ecs_record_get_mut_id(world: *world_t, record: *record_t, id: id_t) ?*anyopaque;
/// `pub fn record_get_mut_id(world: *world_t, record: *record_t, id: id_t) ?*anyopaque`
pub const record_get_mut_id = ecs_record_get_mut_id;

extern fn ecs_emplace_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque;
/// `pub fn emplace_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque`
pub const emplace_id = ecs_emplace_id;

extern fn ecs_modified_id(world: *world_t, entity: entity_t, id: id_t) void;
/// `pub fn modified_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const modified_id = ecs_modified_id;

extern fn ecs_set_id(world: *world_t, entity: entity_t, id: id_t, size: usize, ptr: ?*const anyopaque) entity_t;
/// `pub fn set_id(world: *world_t, entity: entity_t, id: id_t, size: usize, ptr: ?*const anyopaque) entity_t`
pub const set_id = ecs_set_id;
//--------------------------------------------------------------------------------------------------
//
// Functions for testing and modifying entity liveliness.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_is_valid(world: *const world_t, entity: entity_t) bool;
/// `pub fn is_valid(world: *const world_t, entity: entity_t) bool`
pub const is_valid = ecs_is_valid;

extern fn ecs_is_alive(world: *const world_t, entity: entity_t) bool;
/// `pub fn is_alive(world: *const world_t, entity: entity_t) bool`
pub const is_alive = ecs_is_alive;

extern fn ecs_strip_generation(entity: entity_t) id_t;
/// `pub fn strip_generation(entity: entity_t) id_t`
pub const strip_generation = ecs_strip_generation;

extern fn ecs_set_entity_generation(world: *world_t, entity: entity_t) void;
/// `pub fn set_entity_generation(world: *world_t, entity: entity_t) void`
pub const set_entity_generation = ecs_set_entity_generation;

extern fn ecs_get_alive(world: *const world_t, entity: entity_t) entity_t;
/// `pub fn get_alive(world: *const world_t, entity: entity_t) entity_t`
pub const get_alive = ecs_get_alive;

extern fn ecs_ensure(world: *world_t, entity: entity_t) void;
/// `pub fn ensure(world: *world_t, entity: entity_t) void`
pub const ensure = ecs_ensure;

extern fn ecs_ensure_id(world: *world_t, id: id_t) void;
/// `pub fn ensure_id(world: *world_t, id: id_t) void`
pub const ensure_id = ecs_ensure_id;

extern fn ecs_exists(world: *const world_t, entity: entity_t) bool;
/// `pub fn exists(world: *const world_t, entity: entity_t) bool`
pub const exists = ecs_exists;
//--------------------------------------------------------------------------------------------------
//
// Get information from entity.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_get_type(world: *const world_t, entity: entity_t) ?*const type_t;
/// `pub fn get_type(world: *const world_t, entity: entity_t) ?*const type_t`
pub const get_type = ecs_get_type;

extern fn ecs_get_table(world: *const world_t, entity: entity_t) ?*const table_t;
/// `pub fn get_table(world: *const world_t, entity: entity_t) ?*const table_t`
pub const get_table = ecs_get_table;

extern fn ecs_type_str(world: *const world_t, type: ?*const type_t) [*:0]u8;
/// `pub fn type_str(world: *const world_t, type: ?*const type_t) [*:0]u8`
pub const type_str = ecs_type_str;

extern fn ecs_table_str(world: *const world_t, table: ?*const table_t) ?[*:0]u8;
/// `pub fn table_str(world: *const world_t, table: ?*const table_t) ?[*:0]u8`
pub const table_str = ecs_table_str;

extern fn ecs_entity_str(world: *const world_t, entity: entity_t) ?[*:0]u8;
/// `pub fn entity_str(world: *const world_t, entity: entity_t) ?[*:0]u8`
pub const entity_str = ecs_entity_str;

extern fn ecs_has_id(world: *const world_t, entity: entity_t, id: id_t) bool;
/// `pub fn has_id(world: *const world_t, entity: entity_t, id: id_t) bool`
pub const has_id = ecs_has_id;

extern fn ecs_get_target(world: *const world_t, entity: entity_t, rel: entity_t, index: i32) entity_t;
/// `pub fn get_target(world: *const world_t, entity: entity_t, rel: entity_t, index: i32) entity_t`
pub const get_target = ecs_get_target;

extern fn ecs_get_target_for_id(world: *const world_t, entity: entity_t, rel: entity_t, id: id_t) entity_t;
/// `pub fn get_target_for_id(world: *const world_t, entity: entity_t, rel: entity_t, id: id_t) entity_t`
pub const get_target_for_id = ecs_get_target_for_id;

extern fn ecs_get_depth(world: *const world_t, entity: entity_t, rel: entity_t) i32;
/// `pub fn get_depth(world: *const world_t, entity: entity_t, rel: entity_t) i32`
pub const get_depth = ecs_get_depth;

extern fn ecs_count_id(world: *const world_t, entity: entity_t) i32;
/// `pub fn count_id(world: *const world_t, entity: entity_t) i32`
pub const count_id = ecs_count_id;
//--------------------------------------------------------------------------------------------------
//
// Functions for working with entity names and paths.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_get_name(world: *const world_t, entity: entity_t) ?[*:0]const u8;
/// `pub fn get_name(world: *const world_t, entity: entity_t) ?[*:0]const u8`
pub const get_name = ecs_get_name;

extern fn ecs_get_symbol(world: *const world_t, entity: entity_t) ?[*:0]const u8;
/// `pub fn get_symbol(world: *const world_t, entity: entity_t) ?[*:0]const u8`
pub const get_symbol = ecs_get_symbol;

extern fn ecs_set_name(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t;
/// `pub fn set_name(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t`
pub const set_name = ecs_set_name;

extern fn ecs_set_symbol(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t;
/// `pub fn set_symbol(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t`
pub const set_symbol = ecs_set_symbol;

extern fn ecs_set_alias(world: *world_t, entity: entity_t, name: ?[*:0]const u8) void;
/// `pub fn set_alias(world: *world_t, entity: entity_t, name: ?[*:0]const u8) void`
pub const set_alias = ecs_set_alias;

extern fn ecs_lookup(world: *const world_t, name: ?[*:0]const u8) entity_t;
/// `pub fn lookup(world: *const world_t, name: ?[*:0]const u8) entity_t`
pub const lookup = ecs_lookup;

extern fn ecs_lookup_child(world: *const world_t, parent: entity_t, name: ?[*:0]const u8) entity_t;
/// `pub fn lookup_child(world: *const world_t, parent: entity_t, name: ?[*:0]const u8) entity_t`
pub const lookup_child = ecs_lookup_child;

extern fn ecs_lookup_path_w_sep(
    world: *const world_t,
    parent: entity_t,
    path: ?[*:0]const u8,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
    recursive: bool,
) entity_t;
/// ```
/// pub fn lookup_path_w_sep(
///     world: *const world_t,
///     parent: entity_t,
///     path: ?[*:0]const u8,
///     sep: ?[*:0]const u8,
///     prefix: ?[*:0]const u8,
///    recursive: bool,
/// ) entity_t;
/// ```
pub const lookup_path_w_sep = ecs_lookup_path_w_sep;

extern fn ecs_lookup_symbol(world: *const world_t, symbol: ?[*:0]const u8, lookup_as_path: bool) entity_t;
/// `pub fn lookup_symbol(world: *const world_t, symbol: ?[*:0]const u8, lookup_as_path: bool) entity_t`
pub const lookup_symbol = ecs_lookup_symbol;

extern fn ecs_get_path_w_sep(
    world: *const world_t,
    parent: entity_t,
    child: entity_t,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
) ?[*]u8;
/// ```
/// pub fn ecs_get_path_w_sep(
///     world: *const world_t,
///     parent: entity_t,
///     child: entity_t,
///     sep: ?[*:0]const u8,
///     prefix: ?[*:0]const u8,
/// ) ?[*]u8;
/// ```
pub const get_path_w_sep = ecs_get_path_w_sep;

extern fn ecs_new_from_path_w_sep(
    world: *world_t,
    parent: entity_t,
    path: ?[*:0]const u8,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
) entity_t;
/// ```
/// pub fn ecs_new_from_path_w_sep(
///     world: *world_t,
///     parent: entity_t,
///     path: ?[*:0]const u8,
///     sep: ?[*:0]const u8,
///     prefix: ?[*:0]const u8,
/// ) entity_t;
/// ```
pub const new_from_path_w_sep = ecs_new_from_path_w_sep;

extern fn ecs_add_path_w_sep(
    world: *world_t,
    entity: entity_t,
    parent: entity_t,
    path: ?[*:0]const u8,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
) entity_t;
/// ```
/// pub fn ecs_add_path_w_sep(
///     world: *world_t,
///     entity: entity_t,
///     parent: entity_t,
///     path: ?[*:0]const u8,
///     sep: ?[*:0]const u8,
///     prefix: ?[*:0]const u8,
/// ) entity_t;
/// ```
pub const add_path_w_sep = ecs_add_path_w_sep;

extern fn ecs_set_scope(world: *world_t, scope: entity_t) entity_t;
/// `pub fn set_scope(world: *world_t, scope: entity_t) entity_t`
pub const set_scope = ecs_set_scope;

extern fn ecs_get_scope(world: *const world_t) entity_t;
/// `pub fn get_scope(world: *const world_t) entity_t`
pub const get_scope = ecs_get_scope;

extern fn ecs_set_name_prefix(world: *world_t, prefix: ?[*:0]const u8) ?[*:0]const u8;
/// `pub fn set_name_prefix(world: *world_t, prefix: ?[*:0]const u8) ?[*:0]const u8`
pub const set_name_prefix = ecs_set_name_prefix;

extern fn ecs_set_lookup_path(world: *world_t, lookup_path: ?[*]const entity_t) ?[*]entity_t;
/// `pub fn set_lookup_path(world: *world_t, lookup_path: ?[*]const entity_t) ?[*]entity_t`
pub const set_lookup_path = ecs_set_lookup_path;

extern fn ecs_get_lookup_path(world: *const world_t) ?[*]entity_t;
/// `pub fn get_lookup_path(world: *const world_t) ?[*]entity_t`
pub const get_lookup_path = ecs_get_lookup_path;
//--------------------------------------------------------------------------------------------------
//
// Functions for registering and working with components.
//
//--------------------------------------------------------------------------------------------------
extern fn ecs_component_init(world: *world_t, desc: *const component_desc_t) entity_t;
/// `pub fn component_init(world: *world_t, desc: *const component_desc_t) entity_t`
pub const component_init = ecs_component_init;

extern fn ecs_set_hooks_id(world: *world_t, id: entity_t, hooks: *const type_hooks_t) void;
/// `pub fn set_hooks_id(world: *world_t, id: entity_t, hooks: *const type_hooks_t) void`
pub const set_hooks_id = ecs_set_hooks_id;

extern fn ecs_get_hooks_id(world: *const world_t, id: entity_t) *const type_hooks_t;
/// `pub fn get_hooks_id(world: *const world_t, id: entity_t) *const type_hooks_t`
pub const get_hooks_id = ecs_get_hooks_id;

// TODO: flecs upstream: missing const
extern fn ecs_id_is_tag(world: *const world_t, id: id_t) bool;
/// `pub fn id_is_tag(world: *const world_t, id: id_t) bool;
pub const id_is_tag = ecs_id_is_tag;

// TODO: flecs upstream: missing const
extern fn ecs_id_in_use(world: *const world_t, id: id_t) bool;
/// `pub fn id_in_use(world: *const world_t, id: id_t) bool`
pub const id_in_use = ecs_id_in_use;

extern fn ecs_get_type_info(world: *const world_t, id: id_t) *const type_info_t;
/// `pub fn get_type_info(world: *const world_t, id: id_t) *const type_info_t`
pub const get_type_info = ecs_get_type_info;

extern fn ecs_get_typeid(world: *const world_t, id: id_t) entity_t;
/// `pub fn get_typeid(world: *const world_t, id: id_t) entity_t`
pub const get_typeid = ecs_get_typeid;

extern fn ecs_id_match(id: id_t, pattern: id_t) bool;
/// `pub fn id_match(id: id_t, pattern: id_t) bool`
pub const id_match = ecs_id_match;

extern fn ecs_id_is_pair(id: id_t) bool;
/// `pub fn id_is_pair(id: id_t) bool`
pub const id_is_pair = ecs_id_is_pair;

extern fn ecs_id_is_wildcard(id: id_t) bool;
/// `pub fn id_is_wildcard(id: id_t) bool`
pub const id_is_wildcard = ecs_id_is_wildcard;

extern fn ecs_id_is_valid(world: *const world_t, id: id_t) bool;
/// `pub fn id_is_valid(world: *const world_t, id: id_t) bool`
pub const id_is_valid = ecs_id_is_valid;

extern fn ecs_id_get_flags(world: *const world_t, id: id_t) flags32_t;
/// `pub fn id_get_flags(world: *const world_t, id: id_t) flags32_t`
pub const id_get_flags = ecs_id_get_flags;

extern fn ecs_id_flag_str(id_flags: id_t) ?[*:0]const u8;
/// `pub fn id_flag_str(id_flags: id_t) ?[*]const u8`
pub const id_flag_str = ecs_id_flag_str;

extern fn ecs_id_str(world: *const world_t, id: id_t) ?[*:0]u8;
/// `pub fn id_str(world: *const world_t, id: id_t) ?[*]u8`
pub const id_str = ecs_id_str;
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
pub fn component(world: *world_t, comptime T: type) void {
    if (@typeInfo(T) != .Struct and @typeInfo(T) != .Type and @typeInfo(T) != .Enum)
        @compileError("T must be .Struct, .Type or .Enum");

    const type_id_ptr = typeIdPtr(T);

    const edesc: entity_desc_t = .{
        .id = type_id_ptr.*,
        .use_low_id = @sizeOf(T) > 0,
        .name = @typeName(T),
        .symbol = @typeName(T),
    };

    if (@sizeOf(T) == 0) {
        type_id_ptr.* = ecs_entity_init(world, &edesc);
    } else {
        const desc: component_desc_t = .{
            .entity = ecs_entity_init(world, &edesc),
            .type = .{
                .alignment = @alignOf(T),
                .size = @sizeOf(T),
            },
        };
        type_id_ptr.* = ecs_component_init(world, &desc);
    }
}

pub inline fn id(comptime T: type) id_t {
    return typeIdPtr(T).*;
}
//--------------------------------------------------------------------------------------------------
fn TypeId(comptime _: type) type {
    return struct {
        var id: id_t = 0;
    };
}
inline fn typeIdPtr(comptime T: type) *id_t {
    return comptime &TypeId(T).id;
}
//--------------------------------------------------------------------------------------------------
comptime {
    _ = @import("tests.zig");
}
//--------------------------------------------------------------------------------------------------
