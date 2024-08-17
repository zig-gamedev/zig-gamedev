const std = @import("std");
const assert = std.debug.assert;
const builtin = @import("builtin");

pub const flecs_version = std.SemanticVersion{
    .major = 4,
    .minor = 0,
    .patch = 1,
};

// TODO: flecs_is_sanitize should come from flecs build flags.
const flecs_is_sanitize = builtin.mode == .Debug;
const flecs_is_debug = flecs_is_sanitize or builtin.mode == .Debug;

pub const ftime_t = f32;
pub const size_t = i32;
pub const flags8_t = u8;
pub const flags16_t = u16;
pub const flags32_t = u32;
pub const flags64_t = u64;

pub fn flagsn_t(comptime bits: u16) type {
    return std.meta.Int(.unsigned, bits);
}
pub const termset_t = flagsn_t(FLECS_TERM_COUNT_MAX);

pub const error_t = error{FlecsError};
fn make_error() error{FlecsError} {
    return error.FlecsError;
}

pub const world_t_magic = 0x65637377;
pub const stage_t_magic = 0x65637373;
pub const query_t_magic = 0x65637375;
pub const observer_t_magic = 0x65637362;

pub const ID_FLAGS_MASK: u64 = @as(u64, 0xFF) << 60;
pub const COMPONENT_MASK: u64 = ~ID_FLAGS_MASK;

// World flags
pub const WorldQuitWorkers = 1 << 0;
pub const WorldReadonly = 1 << 1;
pub const WorldInit = 1 << 2;
pub const WorldQuit = 1 << 3;
pub const WorldFini = 1 << 4;
pub const WorldMeasureFrameTime = 1 << 5;
pub const WorldMeasureSystemTime = 1 << 6;
pub const WorldMultiThreaded = 1 << 7;

// Iterator flags
pub const EcsIterIsValid = 1 << 0;
pub const EcsIterNoData = 1 << 1;
pub const EcsIterIsInstanced = 1 << 2;
pub const EcsIterNoResult = 1 << 3;
pub const EcsIterIgnoreThis = 1 << 4;
// 5 missing in flecs
pub const EcsIterHasCondSet = 1 << 6;
pub const EcsIterProfile = 1 << 7;
pub const EcsIterTrivialSearch = 1 << 8;
// 9, 10 missing in flecs
pub const EcsIterTrivialTest = 1 << 11;
pub const EcsIterTrivialCached = 1 << 14;
pub const EcsIterCacheSearch = 1 << 15;
pub const EcsIterFixedInChangeComputed = 1 << 16;
pub const EcsIterFixedInChanged = 1 << 17;
pub const EcsIterSkip = 1 << 18;
pub const EcsIterCppEach = 1 << 19;

pub const EcsIterTableOnly = 1 << 20; // same as event flag

// Event flags
pub const EcsEventTableOnly = 1 << 20;
pub const EcsEventNoOnSet = 1 << 16;

// Query flags
pub const EcsQueryMatchThis = 1 << 11;
pub const EcsQueryMatchOnlyThis = 1 << 12;
pub const EcsQueryMatchOnlySelf = 1 << 13;
pub const EcsQueryMatchWildcards = 1 << 14;
pub const EcsQueryHasCondSet = 1 << 15;
pub const EcsQueryHasPred = 1 << 16;
pub const EcsQueryHasScopes = 1 << 17;
pub const EcsQueryHasRefs = 1 << 18;
pub const EcsQueryHasOutTerms = 1 << 19;
pub const EcsQueryHasNonThisOutTerms = 1 << 20;
pub const EcsQueryHasMonitor = 1 << 21;
pub const EcsQueryIsTrivial = 1 << 22;
pub const EcsQueryHasCacheable = 1 << 23;
pub const EcsQueryIsCacheable = 1 << 24;
pub const EcsQueryHasTableThisVar = 1 << 25;
// 26 missing in flecs
pub const EcsQueryCacheYieldEmptyTables = 1 << 27;

// Term flags
pub const EcsTermMatchAny = 1 << 0;
pub const EcsTermMatchAnySrc = 1 << 1;
pub const EcsTermTransitive = 1 << 2;
pub const EcsTermReflexive = 1 << 3;
pub const EcsTermIdInherited = 1 << 4;
pub const EcsTermIsTrivial = 1 << 5;
// 6 missing in flecs
pub const EcsTermIsCacheable = 1 << 7;
pub const EcsTermIsScope = 1 << 8;
pub const EcsTermIsMember = 1 << 9;
pub const EcsTermIsToggle = 1 << 10;
pub const EcsTermKeepAlive = 1 << 11;
pub const EcsTermIsSparse = 1 << 12;
pub const EcsTermIsUnion = 1 << 13;
pub const EcsTermIsOr = 1 << 14;

// Observer flags

pub const EcsObserverIsMulti = 1 << 1;
pub const EcsObserverIsMonitor = 1 << 2;
pub const EcsObserverIsDisabled = 1 << 3;
pub const EcsObserverIsParentDisabled = 1 << 4;
pub const EcsObserverBypassQuery = 1 << 5;

// Table flags (used by ecs_table_t::flags)

pub const EcsTableHasBuiltins = 1 << 1;
pub const EcsTableIsPrefab = 1 << 2;
pub const EcsTableHasIsA = 1 << 3;
pub const EcsTableHasChildOf = 1 << 4;
pub const EcsTableHasName = 1 << 5;
pub const EcsTableHasPairs = 1 << 6;
pub const EcsTableHasModule = 1 << 7;
pub const EcsTableIsDisabled = 1 << 8;
pub const EcsTableNotQueryable = 1 << 9;
pub const EcsTableHasCtors = 1 << 10;
pub const EcsTableHasDtors = 1 << 11;
pub const EcsTableHasCopy = 1 << 12;
pub const EcsTableHasMove = 1 << 13;
pub const EcsTableHasToggle = 1 << 14;
pub const EcsTableHasOverrides = 1 << 15;

pub const EcsTableHasOnAdd = 1 << 16;
pub const EcsTableHasOnRemove = 1 << 17;
pub const EcsTableHasOnSet = 1 << 18;
// 19 missing in flecs
pub const EcsTableHasOnTableFill = 1 << 20;
pub const EcsTableHasOnTableEmpty = 1 << 21;
pub const EcsTableHasOnTableCreate = 1 << 22;
pub const EcsTableHasOnTableDelete = 1 << 23;
pub const EcsTableHasSparse = 1 << 24;
pub const EcsTableHasUnion = 1 << 25;

pub const EcsTableHasTraversable = 1 << 26;
pub const EcsTableMarkedForDelete = 1 << 30;

// Composite table flags
pub const EcsTableHasLifecycle = EcsTableHasCtors | EcsTableHasDtors;
pub const EcsTableIsComplex = EcsTableHasLifecycle | EcsTableHasToggle | EcsTableHasSparse;
pub const EcsTableHasAddActions = EcsTableHasIsA | EcsTableHasCtors | EcsTableHasOnAdd | EcsTableHasOnSet;
pub const EcsTableHasRemoveActions = EcsTableHasIsA | EcsTableHasDtors | EcsTableHasOnRemove;

// Aperiodic action flags (used by ecs_run_aperiodic)

pub const EcsAperiodicEmptyTables = 1 << 1;
pub const EcsAperiodicComponentMonitors = 1 << 2;
pub const EcsAperiodicEmptyQueries = 1 << 4;

// Extern declarations
extern const EcsWildcard: entity_t;
extern const EcsAny: entity_t;
extern const EcsTransitive: entity_t;
extern const EcsReflexive: entity_t;
extern const EcsFinal: entity_t;
extern const EcsDontInherit: entity_t;
extern const EcsAlwaysOverride: entity_t;
extern const EcsSymmetric: entity_t;
extern const EcsExclusive: entity_t;
extern const EcsAcyclic: entity_t;
extern const EcsTraversable: entity_t;
extern const EcsWith: entity_t;
extern const EcsOneOf: entity_t;
extern const EcsPairIsTag: entity_t;
extern const EcsUnion: entity_t;
extern const EcsAlias: entity_t;
extern const EcsChildOf: entity_t;
extern const EcsSlotOf: entity_t;
extern const EcsPrefab: entity_t;
extern const EcsDisabled: entity_t;

extern const EcsOnStart: entity_t;
extern const EcsPreFrame: entity_t;
extern const EcsOnLoad: entity_t;
extern const EcsPostLoad: entity_t;
extern const EcsPreUpdate: entity_t;
extern const EcsOnUpdate: entity_t;
extern const EcsOnValidate: entity_t;
extern const EcsPostUpdate: entity_t;
extern const EcsPreStore: entity_t;
extern const EcsOnStore: entity_t;
extern const EcsPostFrame: entity_t;
extern const EcsPhase: entity_t;

extern const EcsOnAdd: entity_t;
extern const EcsOnRemove: entity_t;
extern const EcsOnSet: entity_t;
extern const EcsMonitor: entity_t;
extern const EcsOnTableCreate: entity_t;
extern const EcsOnTableDelete: entity_t;
extern const EcsOnTableEmpty: entity_t;
extern const EcsOnTableFill: entity_t;

extern const EcsOnDelete: entity_t;
extern const EcsOnDeleteTarget: entity_t;
extern const EcsRemove: entity_t;
extern const EcsDelete: entity_t;
extern const EcsPanic: entity_t;

extern const EcsFlatten: entity_t;

pub const EcsDefaultChildComponent = extern struct {
    component: id_t,
};

extern const EcsPredEq: entity_t;
extern const EcsPredMatch: entity_t;
extern const EcsPredLookup: entity_t;

extern const EcsIsA: entity_t;
extern const EcsDependsOn: entity_t;

pub var Wildcard: entity_t = undefined;
pub var Any: entity_t = undefined;
pub var Transitive: entity_t = undefined;
pub var Reflexive: entity_t = undefined;
pub var Final: entity_t = undefined;
pub var DontInherit: entity_t = undefined;
pub var PairIsTag: entity_t = undefined;
pub var Union: entity_t = undefined;
pub var Exclusive: entity_t = undefined;
pub var Acyclic: entity_t = undefined;
pub var Traversable: entity_t = undefined;
pub var Symmetric: entity_t = undefined;
pub var With: entity_t = undefined;
pub var OneOf: entity_t = undefined;

pub var IsA: entity_t = undefined;
pub var ChildOf: entity_t = undefined;
pub var DependsOn: entity_t = undefined;
pub var SlotOf: entity_t = undefined;

pub var AlwaysOverride: entity_t = undefined;
pub var Alias: entity_t = undefined;
pub var Prefab: entity_t = undefined;
pub var Disabled: entity_t = undefined;

pub var OnStart: entity_t = undefined;
pub var PreFrame: entity_t = undefined;
pub var OnLoad: entity_t = undefined;
pub var PostLoad: entity_t = undefined;
pub var PreUpdate: entity_t = undefined;
pub var OnUpdate: entity_t = undefined;
pub var OnValidate: entity_t = undefined;
pub var PostUpdate: entity_t = undefined;
pub var PreStore: entity_t = undefined;
pub var OnStore: entity_t = undefined;
pub var PostFrame: entity_t = undefined;
pub var Phase: entity_t = undefined;

pub var OnAdd: entity_t = undefined;
pub var OnRemove: entity_t = undefined;
pub var OnSet: entity_t = undefined;
pub var UnSet: entity_t = undefined;
pub var Monitor: entity_t = undefined;
pub var OnTableCreate: entity_t = undefined;
pub var OnTableDelete: entity_t = undefined;
pub var OnTableEmpty: entity_t = undefined;
pub var OnTableFill: entity_t = undefined;

pub var OnDelete: entity_t = undefined;
pub var OnDeleteTarget: entity_t = undefined;
pub var Remove: entity_t = undefined;
pub var Delete: entity_t = undefined;
pub var Panic: entity_t = undefined;

pub var DefaultChildComponent: EcsDefaultChildComponent = undefined;

pub var PredEq: entity_t = undefined;
pub var PredMatch: entity_t = undefined;
pub var PredLookup: entity_t = undefined;

//--------------------------------------------------------------------------------------------------
//
// Types for core API objects.
//
//--------------------------------------------------------------------------------------------------
pub const id_t = u64;
pub const entity_t = id_t;

pub const type_t = extern struct {
    array: [*]id_t,
    count: i32,
};

pub const world_t = opaque {};
pub const stage_t = opaque {};
pub const table_t = opaque {};

pub const query_cache_table_match_t = opaque {};
pub const data_t = opaque {};

pub const table_cache_t = opaque {};
pub const id_record_t = opaque {};

pub const poly_t = anyopaque;

pub const mixins_t = opaque {};

pub const header_t = extern struct {
    magic: i32,
    type: i32 = 0,
    refcount: i32 = 0,
    mixins: ?*mixins_t = null,
};

pub const record_t = extern struct {
    idr: *id_record_t,
    table: *table_t,
    row: u32,
    dense: i32,
};

pub const table_cache_hdr_t = extern struct {
    cache: *table_cache_t,
    table: *table_t,
    prev: *table_cache_hdr_t,
    empty: bool,
};

pub const table_record_t = extern struct {
    hdr: table_cache_hdr_t,
    index: i16,
    count: i16,
    column: i16,
};

//--------------------------------------------------------------------------------------------------
//
// Function types.
//
//--------------------------------------------------------------------------------------------------
pub const run_action_t = *const fn (it: *iter_t) callconv(.C) void;

pub const iter_action_t = *const fn (it: *iter_t) callconv(.C) void;

pub const iter_next_action_t = *const fn (it: *iter_t) callconv(.C) bool;

pub const iter_fini_action_t = *const fn (it: *iter_t) callconv(.C) void;

pub const order_by_action_t = *const fn (
    e1: entity_t,
    ptr1: *const anyopaque,
    e2: entity_t,
    ptr2: *const anyopaque,
) callconv(.C) i32;

pub const sort_table_action_t = *const fn (
    world: *world_t,
    table: *table_t,
    entities: ?[*]entity_t,
    ptr: ?*anyopaque,
    size: i32,
    lo: i32,
    hi: i32,
    order_by: ?order_by_action_t,
) callconv(.C) void;

pub const group_by_action_t = *const fn (
    world: *world_t,
    table: *table_t,
    group_id: id_t,
    ctx: ?*anyopaque,
) callconv(.C) u64;

pub const group_create_action_t = *const fn (
    world: *world_t,
    group_id: u64,
    group_by_ctx: ?*anyopaque,
) callconv(.C) ?*anyopaque;

pub const group_delete_action_t = *const fn (
    world: *world_t,
    group_id: u64,
    group_ctx: ?*anyopaque,
    group_by_ctx: ?*anyopaque,
) callconv(.C) void;

pub const module_action_t = *const fn (world: *world_t) callconv(.C) void;

pub const fini_action_t = *const fn (world: *world_t, ctx: ?*anyopaque) callconv(.C) void;

pub const ctx_free_t = *const fn (ctx: ?*anyopaque) callconv(.C) void;

pub const compare_action_t = *const fn (
    ptr1: *const anyopaque,
    ptr2: *const anyopaque,
) callconv(.C) i32;

pub const hash_value_action_t = *const fn (ptr: ?*const anyopaque) callconv(.C) u64;

pub const xtor_t = *const fn (
    ptr: *anyopaque,
    count: i32,
    type_info: *const type_info_t,
) callconv(.C) void;

pub const copy_t = *const fn (
    dst_ptr: *anyopaque,
    src_ptr: *const anyopaque,
    count: i32,
    type_info: *const type_info_t,
) callconv(.C) void;

pub const move_t = *const fn (
    dst_ptr: *anyopaque,
    src_ptr: *anyopaque,
    count: i32,
    type_info: *const type_info_t,
) callconv(.C) void;

pub const poly_dtor_t = *const fn (poly: *poly_t) callconv(.C) void;

pub const system_desc_t = extern struct {
    _canary: i32 = 0,
    entity: entity_t = 0,
    query: query_desc_t = .{},
    callback: ?iter_action_t = null,
    run: ?run_action_t = null,
    ctx: ?*anyopaque = null,
    ctx_free: ?ctx_free_t = null,
    callback_ctx: ?*anyopaque = null,
    callback_ctx_free: ?ctx_free_t = null,
    run_ctx: ?*anyopaque = null,
    run_ctx_free: ?ctx_free_t = null,
    interval: ftime_t = 0.0,
    rate: i32 = 0,
    tick_source: entity_t = 0,
    multi_threaded: bool = false,
    immediate: bool = false,
};

/// `pub fn system_init(world: *world_t, desc: *const system_desc_t) entity_t`
pub const system_init = ecs_system_init;
extern fn ecs_system_init(world: *world_t, desc: *const system_desc_t) entity_t;

//--------------------------------------------------------------------------------------------------
//
// Query descriptor types.
//
//--------------------------------------------------------------------------------------------------
pub const inout_kind_t = enum(i16) {
    InOutDefault,
    InOutNone,
    EcsInOutFilter,
    InOut,
    In,
    Out,
};

pub const oper_kind_t = enum(i16) {
    And,
    Or,
    Not,
    Optional,
    AndFrom,
    OrFrom,
    NotFrom,
};

pub const query_cache_kind_t = enum(i32) {
    QueryCacheDefault,
    QueryCacheAuto,
    QueryCacheAll,
    QueryCacheNone,
};

pub const Self = 1 << 63;
pub const Up = 1 << 62;
pub const Trav = 1 << 61;
pub const Cascade = 1 << 60;
pub const Desc = 1 << 59;
pub const IsVariable = 1 << 58;
pub const IsEntity = 1 << 57;
pub const IsName = 1 << 56;
pub const TraverseFlags = Self | Up | Trav | Cascade | Desc;
pub const TermRefFlags = TraverseFlags | IsVariable | IsEntity | IsName;

pub const term_ref_t = extern struct {
    id: entity_t = 0,
    name: ?[*:0]const u8 = null,
};

pub const term_t = extern struct {
    id: id_t = 0,

    src: term_ref_t = .{},
    first: term_ref_t = .{},
    second: term_ref_t = .{},

    trav: entity_t = 0,

    inout: inout_kind_t = .InOutDefault,
    oper: oper_kind_t = .And,

    field_index: i8 = 0,
    flags_: flags16_t = 0,
};

pub const query_t = extern struct {
    hdr: header_t = .{},

    terms: [FLECS_TERM_COUNT_MAX]term_t = .{},
    sizes: [FLECS_TERM_COUNT_MAX]size_t = .{},
    ids: [FLECS_TERM_COUNT_MAX]id_t = .{},

    flags: flags32_t = 0,
    var_count: i8 = 0,
    term_count: i8 = 0,
    field_count: i8 = 0,

    // /* Bitmasks for quick field information lookups */
    fixed_fields: termset_t = 0,
    static_id_fields: termset_t = 0,
    data_fields: termset_t = 0,
    write_fields: termset_t = 0,
    read_fields: termset_t = 0,
    row_fields: termset_t = 0,
    set_fields: termset_t = 0,

    cache_kind: query_cache_kind_t = .QueryCacheDefault,

    vars: ?[*][*:0]u8 = null,
    ctx: ?*anyopaque = null,
    binding_ctx: ?*anyopaque = null,

    entity: entity_t = 0,
    real_world: ?*world_t = null,
    world: ?*world_t = null,

    eval_count: i32 = 0,
};

pub fn array(comptime T: type, comptime len: comptime_int) [len]T {
    return [_]T{.{}} ** len;
}

pub const observer_t = extern struct {
    hdr: header_t,

    query: ?*query_t,

    events: [FLECS_EVENT_DESC_MAX]entity_t,
    event_count: i32,

    callback: iter_action_t,
    run: run_action_t,

    ctx: ?*anyopaque,
    callback_ctx: ?*anyopaque,
    run_ctx: ?*anyopaque,

    ctx_free: ?ctx_free_t,
    callback_ctx_free: ?ctx_free_t,
    run_ctx_free: ?ctx_free_t,

    observable: [*c]observable_t,

    world: ?*world_t,
    entity: entity_t = 0,
};
//--------------------------------------------------------------------------------------------------
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

pub const event_id_record_t = opaque {};

pub const event_record_t = extern struct {
    any: ?*event_id_record_t,
    wildcard: ?*event_id_record_t,
    wildcard_pair: ?*event_id_record_t,
    event_ids: map_t,
    event: entity_t,
};

pub const observable_t = extern struct {
    on_add: event_record_t,
    on_remove: event_record_t,
    on_set: event_record_t,
    on_wildcard: event_record_t,
    events: sparse_t,
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

pub const ref_t = extern struct {
    entity: entity_t,
    id: entity_t,
    table_id: u64,
    tr: *table_record_t,
    record: *record_t,
};

pub const stack_page_t = opaque {};
pub const stack_t = opaque {};

pub const stack_cursor_t = extern struct {
    prev: ?*stack_cursor_t,
    page: ?*stack_page_t,
    sp: i16,
    is_free: bool,
    owner: if (flecs_is_debug) ?*stack_t else void,
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

pub const table_cache_iter_t = extern struct {
    cur: ?*table_cache_hdr_t,
    next: ?*table_cache_hdr_t,
    next_list: ?*table_cache_hdr_t,
};

pub const each_iter_t = extern struct {
    it: table_cache_iter_t,

    ids: id_t,
    sources: entity_t,
    sizes: size_t,
    columns: i32,
    trs: ?[*]table_record_t,
};

pub const query_var_t = opaque {};
pub const query_op_t = opaque {};
pub const query_op_ctx_t = opaque {};
pub const query_op_profile_t = extern struct {
    count: [2]i32,
};

pub const query_iter_t = extern struct {
    query: ?*const query_t = null,
    vars: ?*var_t = null,
    query_vars: ?*const query_var_t = null,
    ops: ?*const query_op_t = null,
    op_ctx: ?*query_op_ctx_t = null,
    node: ?*query_cache_table_match_t = null,
    prev: ?*query_cache_table_match_t = null,
    last: ?*query_cache_table_match_t = null,
    written: ?*i64 = null,
    skip_count: i32,

    profile: ?*query_op_profile_t = null,

    op: i16,
    sp: i16,
};

pub const iter_cache_t = extern struct {
    stack_cursor: ?*stack_cursor_t,
    used: flags8_t,
    allocated: flags8_t,
};

pub const iter_private_t = extern struct {
    iter: extern union {
        query: query_iter_t,
        page: page_iter_t,
        worker: worker_iter_t,
        each: each_iter_t,
    },
    entity_iter: ?*anyopaque,
    cache: iter_cache_t,
};

//--------------------------------------------------------------------------------------------------
//
// allocator_t, vec_t, map_t, switch_node
//
//--------------------------------------------------------------------------------------------------
pub const vec_t = extern struct {
    array: ?*anyopaque,
    count: i32,
    size: i32,
    elem_size: if (flecs_is_sanitize) size_t else void,
};

pub const sparse_t = extern struct {
    dense: vec_t,
    pages: vec_t,
    size: size_t,
    count: i32,
    max_id: u64,
    allocator: [*c]allocator_t,
    page_allocator: [*c]block_allocator_t,
};

pub const block_allocator_chunk_header_t = extern struct {
    next: ?*block_allocator_chunk_header_t,
};

pub const block_allocator_block_t = extern struct {
    memory: ?*anyopaque,
    next: ?*block_allocator_block_t,
};

pub const block_allocator_t = extern struct {
    head: ?*block_allocator_chunk_header_t,
    block_head: ?*block_allocator_block_t,
    block_tail: ?*block_allocator_block_t,
    chunk_size: i32,
    data_size: i32,
    chunks_per_block: i32,
    block_size: i32,
    alloc_count: i32,
};

pub const allocator_t = extern struct {
    chunks: block_allocator_t,
    sizes: sparse_t,
};

pub const map_data_t = u64;
pub const map_key_t = map_data_t;
pub const map_val_t = map_data_t;

pub const bucket_entry_t = extern struct {
    key: map_key_t,
    value: map_val_t,
    next: [*c]bucket_entry_t,
};

pub const bucket_t = extern struct {
    first: [*c]bucket_entry_t,
};

pub const map_t = extern struct {
    bucket_shift: u8,
    shared_allocator: bool,
    buckets: [*c]bucket_t,
    bucket_count: i32,
    count: i32,
    entry_allocator: [*c]block_allocator_t,
    allocator: [*c]allocator_t,
};

pub const map_iter_t = extern struct {
    map: [*c]const map_t,
    bucket: [*c]bucket_t,
    entry: [*c]bucket_entry_t,
    res: [*c]map_data_t,
};

pub const map_params_t = extern struct {
    allocator: [*c]allocator_t,
    entry_allocator: block_allocator_t,
};

pub const switch_node_t = extern struct {
    next: u32,
    prev: u32,
};

pub const switch_page_t = extern struct {
    nodes: vec_t,
    values: vec_t,
};

pub const switch_t = extern struct {
    hdrs: map_t,
    pages: vec_t,
};

//--------------------------------------------------------------------------------------------------

pub const value_t = extern struct {
    type: entity_t = 0,
    ptr: ?*anyopaque = null,
};

pub const FLECS_HI_COMPONENT_ID = 256;
pub const FLECS_HI_ID_RECORD_ID = 1024;
pub const FLECS_ID_DESC_MAX = 32;
pub const FLECS_EVENT_DESC_MAX = 8;
pub const FLECS_VARIABLE_COUNT_MAX = 64;
pub const FLECS_TERM_COUNT_MAX = 32;
pub const FLECS_TERM_ARG_COUNT_MAX = 16;
pub const FLECS_QUERY_VARIABLE_COUNT_MAX = 64;
pub const FLECS_QUERY_SCOPE_NESTING_MAX = 8;

pub const entity_desc_t = extern struct {
    _canary: i32 = 0,
    id: entity_t = 0,
    parent: entity_t = 0,
    name: ?[*:0]const u8 = null,
    sep: ?[*:0]const u8 = null,
    root_sep: ?[*:0]const u8 = null,
    symbol: ?[*:0]const u8 = null,
    use_low_id: bool = false,
    add: ?[*:0]const id_t = null,
    set: ?[*:.{}]const value_t = null,
    add_expr: ?[*:0]const u8 = null,
};

pub const bulk_desc_t = extern struct {
    _canary: i32 = 0,
    entities: ?[*]entity_t,
    count: i32,
    ids: [FLECS_ID_DESC_MAX]id_t,
    data: [*]?*anyopaque,
    table: ?*table_t,
};

pub const component_desc_t = extern struct {
    _canary: i32 = 0,
    entity: entity_t,
    type: type_info_t,
};

pub const iter_t = extern struct {
    world: *world_t,
    real_world: *world_t,

    entities_: [*]const entity_t,
    sizes: ?[*]size_t,
    table: *table_t,
    other_table: ?*table_t,
    ids: ?[*]id_t,
    variables: ?[*]var_t,
    trs: ?[*]*table_record_t,
    sources: ?[*]entity_t,
    constrained_vars: flags64_t,
    group_id: u64,
    set_fields: termset_t,
    ref_fields: termset_t,
    row_fields: termset_t,
    up_fields: termset_t,

    system: entity_t,
    event: entity_t,
    event_id: id_t,
    event_cur: i32,

    field_count: i8,
    term_index: i8,

    variable_count: i8,
    query: *const query_t,
    variable_names: ?[*][*:0]u8,

    param: ?*anyopaque,
    ctx: ?*anyopaque,
    binding_ctx: ?*anyopaque,
    callback_ctx: ?*anyopaque,
    run_ctx: ?*anyopaque,

    delta_time: f32,
    delta_system_time: f32,

    frame_offset: i32,
    offset: i32,
    count_: i32,

    flags: flags32_t,
    interrupted_by: entity_t,
    priv_: iter_private_t,

    next: iter_next_action_t,
    callback: *const fn (it: *iter_t) callconv(.C) void, // TODO: Compiler bug. Should be `iter_action_t`.
    fini: iter_fini_action_t,
    chain_it: ?*iter_t,

    pub fn entities(iter: iter_t) []const entity_t {
        return iter.entities_[0..@as(usize, @intCast(iter.count_))];
    }
    pub fn count(iter: iter_t) usize {
        return @as(usize, @intCast(iter.count_));
    }
};

pub const query_desc_t = extern struct {
    _canary: i32 = 0,

    terms: [FLECS_TERM_COUNT_MAX]term_t = [_]term_t{.{}} ** FLECS_TERM_COUNT_MAX,
    expr: ?[*:0]const u8 = null,

    cache_kind: query_cache_kind_t = .QueryCacheNone,

    flags: flags32_t = 0,

    order_by_callback: ?order_by_action_t = null,
    order_by_table_callback: ?sort_table_action_t = null,
    order_by: entity_t = 0,

    group_by: id_t = 0,
    group_by_callback: ?group_by_action_t = null,
    on_group_create: ?group_create_action_t = null,
    on_group_delete: ?group_delete_action_t = null,
    group_by_ctx: ?*anyopaque = null,
    group_by_ctx_free: ?ctx_free_t = null,
    ctx: ?*anyopaque = null,
    binding_ctx: ?*anyopaque = null,
    ctx_free: ?ctx_free_t = null,
    binding_ctx_free: ?ctx_free_t = null,

    entity: entity_t = 0,
};

pub const observer_desc_t = extern struct {
    _canary: i32 = 0,
    entity: entity_t = 0,

    query: query_desc_t = .{},

    events: [FLECS_EVENT_DESC_MAX]entity_t = [_]entity_t{0} ** FLECS_EVENT_DESC_MAX,

    yield_existing: bool = false,
    callback: iter_action_t,
    run: ?run_action_t = null,
    ctx: ?*anyopaque = null,
    ctx_free: ?ctx_free_t = null,
    callback_ctx: ?*anyopaque = null,
    callback_ctx_free: ?ctx_free_t = null,

    run_ctx: ?*anyopaque = null,
    run_ctx_free: ?ctx_free_t = null,
    observable: ?*poly_t = null,
    last_event_id: ?*i32 = null,

    term_index_: i8 = 0,
    flags_: flags32_t = 0,
};

pub const event_desc_t = extern struct {
    event: entity_t = 0,
    ids: ?[*]const type_t = null,
    table: ?*table_t = null,
    other_table: ?*table_t = null,
    offset: i32 = 0,
    count: i32 = 0,
    entity: entity_t = 0,
    param: ?*anyopaque = null,
    const_param: ?*const anyopaque = null,
    observable: ?*poly_t = null,
    flags: flags32_t = 0,
};

pub const build_info_t = extern struct {
    compiler: ?[*:0]const u8 = null,
    addons: ?*const [*:0]const u8 = null,
    version: ?[*:0]const u8 = null,
    version_major: i16 = 0,
    version_minor: i16 = 0,
    version_patch: i16 = 0,
    debug: bool,
    sanitize: bool,
    perf_trace: bool,
};

pub const world_info_t = extern struct {
    last_component_id: entity_t,
    min_id: entity_t,
    max_id: entity_t,

    delta_time_raw: ftime_t,
    delta_time: ftime_t,
    time_scale: ftime_t,
    target_fps: ftime_t,
    frame_time_total: ftime_t,
    system_time_total: ftime_t,
    emit_time_total: ftime_t,
    merge_time_total: ftime_t,
    rematch_time_total: ftime_t,
    world_time_total: f64,
    world_time_total_raw: f64,

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

    tag_id_count: i32,
    component_id_count: i32,
    pair_id_count: i32,

    table_count: i32,
    empty_table_count: i32,

    cmd: extern struct {
        add_count: i64,
        remove_count: i64,
        delete_count: i64,
        clear_count: i64,
        set_count: i64,
        ensure_count: i64,
        modified_count: i64,
        discard_count: i64,
        event_count: i64,
        other_count: i64,
        batched_entity_count: i64,
        batched_command_count: i64,
    },

    name_prefix: [*:0]const u8,
};

pub const query_group_info = extern struct {
    match_count: i32,
    table_count: i32,
    ctx: ?*anyopaque,
};

const EcsAllocator = struct {
    const AllocationHeader = struct {
        size: usize,
    };

    const Alignment = 16;

    var gpa: ?std.heap.GeneralPurposeAllocator(.{}) = null;
    var allocator: ?std.mem.Allocator = null;

    fn alloc(size: i32) callconv(.C) ?*anyopaque {
        if (size < 0) {
            return null;
        }

        const allocation_size = Alignment + @as(usize, @intCast(size));

        const data = allocator.?.alignedAlloc(u8, Alignment, allocation_size) catch {
            return null;
        };

        var allocation_header = @as(
            *align(Alignment) AllocationHeader,
            @ptrCast(@alignCast(data.ptr)),
        );

        allocation_header.size = allocation_size;

        return data.ptr + Alignment;
    }

    fn free(ptr: ?*anyopaque) callconv(.C) void {
        if (ptr == null) {
            return;
        }
        var ptr_unwrapped = @as([*]u8, @ptrCast(ptr.?)) - Alignment;
        const allocation_header = @as(
            *align(Alignment) AllocationHeader,
            @ptrCast(@alignCast(ptr_unwrapped)),
        );

        allocator.?.free(
            @as([]align(Alignment) u8, @alignCast(ptr_unwrapped[0..allocation_header.size])),
        );
    }

    fn realloc(old: ?*anyopaque, size: i32) callconv(.C) ?*anyopaque {
        if (old == null) {
            return alloc(size);
        }

        const ptr_unwrapped = @as([*]u8, @ptrCast(old.?)) - Alignment;

        const allocation_header = @as(
            *align(Alignment) AllocationHeader,
            @ptrCast(@alignCast(ptr_unwrapped)),
        );

        const old_allocation_size = allocation_header.size;
        const old_slice = @as([*]u8, @ptrCast(ptr_unwrapped))[0..old_allocation_size];
        const old_slice_aligned = @as([]align(Alignment) u8, @alignCast(old_slice));

        const new_allocation_size = Alignment + @as(usize, @intCast(size));
        const new_data = allocator.?.realloc(old_slice_aligned, new_allocation_size) catch {
            return null;
        };

        var new_allocation_header = @as(*align(Alignment) AllocationHeader, @ptrCast(@alignCast(new_data.ptr)));
        new_allocation_header.size = new_allocation_size;

        return new_data.ptr + Alignment;
    }

    fn calloc(size: i32) callconv(.C) ?*anyopaque {
        const data_maybe = alloc(size);
        if (data_maybe) |data| {
            @memset(@as([*]u8, @ptrCast(data))[0..@as(usize, @intCast(size))], 0);
        }

        return data_maybe;
    }
};

fn flecs_abort() callconv(.C) noreturn {
    std.debug.dumpCurrentStackTrace(@returnAddress());
    @breakpoint();
    std.posix.exit(1);
}

//--------------------------------------------------------------------------------------------------
//
// Creation & Deletion
//
//--------------------------------------------------------------------------------------------------
pub fn init() *world_t {
    if (builtin.os.tag == .windows) {
        os.ecs_os_api.abort_ = flecs_abort;
    }

    assert(num_worlds == 0);

    if (num_worlds == 0) {
        EcsAllocator.gpa = .{};
        EcsAllocator.allocator = EcsAllocator.gpa.?.allocator();

        os.ecs_os_api.malloc_ = &EcsAllocator.alloc;
        os.ecs_os_api.free_ = &EcsAllocator.free;
        os.ecs_os_api.realloc_ = &EcsAllocator.realloc;
        os.ecs_os_api.calloc_ = &EcsAllocator.calloc;
    }

    num_worlds += 1;
    component_ids_hm.ensureTotalCapacity(32) catch @panic("OOM");
    const world = ecs_init();

    Wildcard = EcsWildcard;
    Any = EcsAny;
    Transitive = EcsTransitive;
    Reflexive = EcsReflexive;
    Final = EcsFinal;
    DontInherit = EcsDontInherit;
    Exclusive = EcsExclusive;
    Acyclic = EcsAcyclic;
    Traversable = EcsTraversable;
    Symmetric = EcsSymmetric;
    With = EcsWith;
    OneOf = EcsOneOf;

    IsA = EcsIsA;
    ChildOf = EcsChildOf;
    DependsOn = EcsDependsOn;
    SlotOf = EcsSlotOf;

    OnDelete = EcsOnDelete;
    OnDeleteTarget = EcsOnDeleteTarget;
    Remove = EcsRemove;
    Delete = EcsDelete;
    Panic = EcsPanic;

    // TODO DefaultChildComponent = EcsDefaultChildComponent;

    PredEq = EcsPredEq;
    PredMatch = EcsPredMatch;
    PredLookup = EcsPredLookup;

    PairIsTag = EcsPairIsTag;
    Union = EcsUnion;
    Alias = EcsAlias;
    Prefab = EcsPrefab;
    Disabled = EcsDisabled;
    OnStart = EcsOnStart;
    PreFrame = EcsPreFrame;
    OnLoad = EcsOnLoad;
    PostLoad = EcsPostLoad;
    PreUpdate = EcsPreUpdate;
    OnUpdate = EcsOnUpdate;
    OnValidate = EcsOnValidate;
    PostUpdate = EcsPostUpdate;
    PreStore = EcsPreStore;
    OnStore = EcsOnStore;
    PostFrame = EcsPostFrame;
    Phase = EcsPhase;
    OnAdd = EcsOnAdd;
    OnRemove = EcsOnRemove;
    OnSet = EcsOnSet;
    Monitor = EcsMonitor;
    OnTableCreate = EcsOnTableCreate;
    OnTableDelete = EcsOnTableDelete;
    OnTableEmpty = EcsOnTableEmpty;
    OnTableFill = EcsOnTableFill;

    return world;
}
extern fn ecs_init() *world_t;

pub fn fini(world: *world_t) i32 {
    assert(num_worlds == 1);
    num_worlds -= 1;

    const fini_result = ecs_fini(world);

    var it = component_ids_hm.iterator();
    while (it.next()) |kv| {
        const ptr = kv.key_ptr.*;
        ptr.* = 0;
    }
    component_ids_hm.clearRetainingCapacity();

    if (num_worlds == 0) {
        _ = EcsAllocator.gpa.?.deinit();
        EcsAllocator.gpa = null;
        EcsAllocator.allocator = null;
    }

    return fini_result;
}
extern fn ecs_fini(world: *world_t) i32;

/// `pub fn is_fini(world: *const world_t) bool`
pub const is_fini = ecs_is_fini;
extern fn ecs_is_fini(world: *const world_t) bool;

/// `pub fn atfini(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) bool`
pub const atfini = ecs_atfini;
extern fn ecs_atfini(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) bool;
//--------------------------------------------------------------------------------------------------
//
// Frame functions
//
//--------------------------------------------------------------------------------------------------
/// `pub fn frame_begin(world: *world_t, delta_time: ftime_t) ftime_t`
pub const frame_begin = ecs_frame_begin;
extern fn ecs_frame_begin(world: *world_t, delta_time: ftime_t) ftime_t;

/// `pub fn frame_end(world: *world_t) void`
pub const frame_end = ecs_frame_end;
extern fn ecs_frame_end(world: *world_t) void;

/// `pub fn run_post_frame(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) void`
pub const run_post_frame = ecs_run_post_frame;
extern fn ecs_run_post_frame(world: *world_t, action: fini_action_t, ctx: ?*anyopaque) void;

/// `pub fn quit(world: *world_t) void`
pub const quit = ecs_quit;
extern fn ecs_quit(world: *world_t) void;

/// `pub fn should_quit(world: *const world_t) bool`
pub const should_quit = ecs_should_quit;
extern fn ecs_should_quit(world: *const world_t) bool;

/// `pub fn measure_frame_time(world: *world_t, enable: bool) void`
pub const measure_frame_time = ecs_measure_frame_time;
extern fn ecs_measure_frame_time(world: *world_t, enable: bool) void;

/// `pub fn measure_system_time(world: *world_t, enable: bool) void`
pub const measure_system_time = ecs_measure_system_time;
extern fn ecs_measure_system_time(world: *world_t, enable: bool) void;

/// `pub fn set_target_fps(world: *world_t, fps: ftime_t) void`
pub const set_target_fps = ecs_set_target_fps;
extern fn ecs_set_target_fps(world: *world_t, fps: ftime_t) void;
//--------------------------------------------------------------------------------------------------
//
// Commands
//
//--------------------------------------------------------------------------------------------------
/// `pub fn readonly_begin(world: *world_t) bool`
pub const readonly_begin = ecs_readonly_begin;
extern fn ecs_readonly_begin(world: *world_t) bool;

/// `pub fn readonly_end(world: *world_t) void`
pub const readonly_end = ecs_readonly_end;
extern fn ecs_readonly_end(world: *world_t) void;

/// `pub fn merge(world: *world_t) void`
pub const merge = ecs_merge;
extern fn ecs_merge(world: *world_t) void;

/// `pub fn defer_begin(world: *world_t) bool`
pub const defer_begin = ecs_defer_begin;
extern fn ecs_defer_begin(world: *world_t) bool;

/// `pub fn is_deferred(world: *const world_t) bool`
pub const is_deferred = ecs_is_deferred;
extern fn ecs_is_deferred(world: *const world_t) bool;

/// `pub fn defer_end(world: *world_t) bool`
pub const defer_end = ecs_defer_end;
extern fn ecs_defer_end(world: *world_t) bool;

/// `pub fn defer_suspend(world: *world_t) void`
pub const defer_suspend = ecs_defer_suspend;
extern fn ecs_defer_suspend(world: *world_t) void;

/// `pub fn defer_resume(world: *world_t) void`
pub const defer_resume = ecs_defer_resume;
extern fn ecs_defer_resume(world: *world_t) void;

/// `pub fn set_stage_count(world: *world_t, stages: i32) void`
pub const set_stage_count = ecs_set_stage_count;
extern fn ecs_set_stage_count(world: *world_t, stages: i32) void;

/// `pub fn get_stage_count(world: *const world_t) i32`
pub const get_stage_count = ecs_get_stage_count;
extern fn ecs_get_stage_count(world: *const world_t) i32;

/// `pub fn get_stage_id(world: *const world_t) i32`
pub const get_stage_id = ecs_get_stage_id;
extern fn ecs_get_stage_id(world: *const world_t) i32;

/// `pub fn get_stage(world: *const world_t, stage_id: i32) *world_t`
pub const get_stage = ecs_get_stage;
extern fn ecs_get_stage(world: *const world_t, stage_id: i32) *world_t;

/// `pub fn stage_is_readonly(world: *const world_t) bool`
pub const stage_is_readonly = ecs_stage_is_readonly;
extern fn ecs_stage_is_readonly(world: *const world_t) bool;

/// `pub fn async_stage_new(world: *world_t) *world_t`
pub const async_stage_new = ecs_stage_new;
extern fn ecs_stage_new(world: *world_t) *world_t;

/// `pub fn async_stage_free(world: *world_t) *world_t`
pub const async_stage_free = ecs_async_stage_free;
extern fn ecs_async_stage_free(world: *world_t) *world_t;

/// `pub fn stage_is_async(world: *const world_t) bool`
pub const stage_is_async = ecs_stage_is_async;
extern fn ecs_stage_is_async(world: *const world_t) bool;
//--------------------------------------------------------------------------------------------------
//
// Misc
//
//--------------------------------------------------------------------------------------------------
/// `pub fn set_ctx(world: *world_t, ctx: ?*anyopaque, ctx_free: ctx_free_t) void`
pub const set_ctx = ecs_set_ctx;
extern fn ecs_set_ctx(world: *world_t, ctx: ?*anyopaque, ctx_free: ctx_free_t) void;

/// `pub fn set_binding_ctx(world: *world_t, ctx: ?*anyopaque, ctx_free: ctx_free_t) void`
pub const set_binding_ctx = ecs_set_binding_ctx;
extern fn ecs_set_binding_ctx(world: *world_t, ctx: ?*anyopaque, ctx_free: ctx_free_t) void;

/// `pub fn get_ctx(world: *const world_t) ?*anyopaque`
pub const get_ctx = ecs_get_ctx;
extern fn ecs_get_ctx(world: *const world_t) ?*anyopaque;

/// `pub fn get_binding_ctx(world: *const world_t) ?*anyopaque`
pub const get_binding_ctx = ecs_get_binding_ctx;
extern fn ecs_get_binding_ctx(world: *const world_t) ?*anyopaque;

/// `pub fn ecs_get_world_info(world: *const world_t) *const world_info_t`
pub const get_world_info = ecs_get_world_info;
extern fn ecs_get_world_info(world: *const world_t) *const world_info_t;

/// `pub fn dim(world: *world_t, entity_count: i32) void`
pub const dim = ecs_dim;
extern fn ecs_dim(world: *world_t, entity_count: i32) void;

/// `pub fn set_entity_range(world: *world_t, id_start: entity_t, id_end: entity_t) void`
pub const set_entity_range = ecs_set_entity_range;
extern fn ecs_set_entity_range(world: *world_t, id_start: entity_t, id_end: entity_t) void;

/// `pub fn enable_range_check(world: *world_t, enable: bool) bool`
pub const enable_range_check = ecs_enable_range_check;
extern fn ecs_enable_range_check(world: *world_t, enable: bool) bool;

/// `pub fn get_max_id(world: *const world_t) entity_t`
pub const get_max_id = ecs_get_max_id;
extern fn ecs_get_max_id(world: *const world_t) entity_t;

/// `pub fn run_aperiodic(world: *world_t, flags: flags32_t) void`
pub const run_aperiodic = ecs_run_aperiodic;
extern fn ecs_run_aperiodic(world: *world_t, flags: flags32_t) void;

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
extern fn ecs_delete_empty_tables(
    world: *world_t,
    id: id_t,
    clear_generation: u16,
    delete_generation: u16,
    min_id_count: i32,
    time_budget_seconds: f64,
) i32;

/// `pub fn make_pair(first: entity_t, second: entity_t) id_t`
pub const make_pair = ecs_make_pair;
extern fn ecs_make_pair(first: entity_t, second: entity_t) id_t;

pub fn pair_first(pair_id: entity_t) entity_t {
    return @as(entity_t, @intCast(@as(u32, @truncate((pair_id & COMPONENT_MASK) >> 32))));
}

pub fn pair_second(pair_id: entity_t) entity_t {
    return @as(entity_t, @intCast(@as(u32, @truncate(pair_id))));
}
//--------------------------------------------------------------------------------------------------
//
// Functions for creating and deleting entities.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn new_id(world: *world_t) entity_t`
pub const new_id = ecs_new;
extern fn ecs_new(world: *world_t) entity_t;

/// `pub fn new_low_id(world: *world_t) entity_t`
pub const new_low_id = ecs_new_low_id;
extern fn ecs_new_low_id(world: *world_t) entity_t;

/// `pub fn new_w_id(world: *world_t, id: id_t) entity_t`
pub const new_w_id = ecs_new_w_id;
extern fn ecs_new_w_id(world: *world_t, id: id_t) entity_t;

/// `pub fn new_w_table(world: *world_t, table: *table_t) entity_t`
pub const new_w_table = ecs_new_w_table;
extern fn ecs_new_w_table(world: *world_t, table: *table_t) entity_t;

/// `pub fn entity_init(world: *world_t, desc: *const entity_desc_t) entity_t`
pub const entity_init = ecs_entity_init;
extern fn ecs_entity_init(world: *world_t, desc: *const entity_desc_t) entity_t;

/// `pub fn bulk_new_w_id(world: *world_t, id: id_t, count: i32) [*]const entity_t`
pub const bulk_new_w_id = ecs_bulk_new_w_id;
extern fn ecs_bulk_new_w_id(world: *world_t, id: id_t, count: i32) [*]const entity_t;

/// `pub fn clone(world: *world_t, dst: entity_t, src: entity_t, copy_value: bool) entity_t`
pub const clone = ecs_clone;
extern fn ecs_clone(world: *world_t, dst: entity_t, src: entity_t, copy_value: bool) entity_t;

/// `pub fn delete(world: *world_t, entity: entity_t) void`
pub const delete = ecs_delete;
extern fn ecs_delete(world: *world_t, entity: entity_t) void;

/// `pub fn delete_with(world: *world_t, id: id_t) void`
pub const delete_with = ecs_delete_with;
extern fn ecs_delete_with(world: *world_t, id: id_t) void;
//--------------------------------------------------------------------------------------------------
//
// Functions for adding and removing components.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn add_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const add_id = ecs_add_id;
extern fn ecs_add_id(world: *world_t, entity: entity_t, id: id_t) void;

/// `pub fn remove_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const remove_id = ecs_remove_id;
extern fn ecs_remove_id(world: *world_t, entity: entity_t, id: id_t) void;

/// `pub fn override_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const override_id = ecs_override_id;
extern fn ecs_override_id(world: *world_t, entity: entity_t, id: id_t) void;

/// `pub fn clear(world: *world_t, entity: entity_t) void`
pub const clear = ecs_clear;
extern fn ecs_clear(world: *world_t, entity: entity_t) void;

/// `pub fn remove_all(world: *world_t, id: id_t) void`
pub const remove_all = ecs_remove_all;
extern fn ecs_remove_all(world: *world_t, id: id_t) void;

/// `pub fn set_with(world: *world_t, id: id_t) void`
pub const set_with = ecs_set_with;
extern fn ecs_set_with(world: *world_t, id: id_t) id_t;

/// `pub fn get_with(world: *const world_t) id_t`
pub const get_with = ecs_get_with;
extern fn ecs_get_with(world: *const world_t) id_t;
//--------------------------------------------------------------------------------------------------
//
// Functions for enabling/disabling entities and components.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn enable(world: *const world_t, entity: entity_t, enable: bool) void`
pub const enable = ecs_enable;
extern fn ecs_enable(world: *world_t, entity: entity_t, enable: bool) void;

/// `pub fn enable_id(world: *const world_t, entity: entity_t, id: id_t, enable: bool) void`
pub const enable_id = ecs_enable_id;
extern fn ecs_enable_id(world: *world_t, entity: entity_t, id: id_t, enable: bool) void;

/// `pub fn is_enabled_id(world: *const world_t, entity: entity_t, id: id_t) bool`
pub const is_enabled_id = ecs_is_enabled_id;
extern fn ecs_is_enabled_id(world: *const world_t, entity: entity_t, id: id_t) bool;
//--------------------------------------------------------------------------------------------------
//
// Functions for getting/setting components.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn get_id(world: *const world_t, entity: entity_t, id: id_t) ?*const anyopaque`
pub const get_id = ecs_get_id;
extern fn ecs_get_id(world: *const world_t, entity: entity_t, id: id_t) ?*const anyopaque;

/// `pub fn ref_init_id(world: *const world_t, entity: entity_t, id: id_t) ref_t`
pub const ref_init_id = ecs_ref_init_id;
extern fn ecs_ref_init_id(world: *const world_t, entity: entity_t, id: id_t) ref_t;

/// `pub fn ref_get_id(world: *const world_t, ref: *ref_t, id: id_t) ?*anyopaque`
pub const ref_get_id = ecs_ref_get_id;
extern fn ecs_ref_get_id(world: *const world_t, ref: *ref_t, id: id_t) ?*anyopaque;

/// `pub fn ref_get_id(world: *const world_t, ref: *ref_t) void`
pub const ref_update = ecs_ref_update;
extern fn ecs_ref_update(world: *const world_t, ref: *ref_t) void;

/// `pub fn get_mut_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque`
pub const get_mut_id = ecs_get_mut_id;
extern fn ecs_get_mut_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque;

/// `pub fn get_mut_modified_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque`
pub const get_mut_modified_id = ecs_get_mut_modified_id;
extern fn ecs_get_mut_modified_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque;

/// `pub fn write_begin(world: *world_t, entity: entity_t) ?*record_t`
pub const write_begin = ecs_write_begin;
extern fn ecs_write_begin(world: *world_t, entity: entity_t) ?*record_t;

/// `pub fn write_end(record: *record_t) void`
pub const write_end = ecs_write_end;
extern fn ecs_write_end(record: *record_t) void;

/// `pub fn read_begin(world: *world_t, entity: entity_t) ?*const record_t`
pub const read_begin = ecs_read_begin;
extern fn ecs_read_begin(world: *world_t, entity: entity_t) ?*const record_t;

/// `pub fn read_end(record: *const record_t) void`
pub const read_end = ecs_read_end;
extern fn ecs_read_end(record: *const record_t) void;

/// `pub fn record_get_entity(record: *const record_t) entity_t`
pub const record_get_entity = ecs_record_get_entity;
extern fn ecs_record_get_entity(record: *const record_t) entity_t;

/// `pub fn record_get_id(world: *world_t, record: *const record_t, id: id_t) ?*const anyopaque`
pub const record_get_id = ecs_record_get_id;
extern fn ecs_record_get_id(world: *const world_t, record: *const record_t, id: id_t) ?*const anyopaque;

/// `pub fn record_get_mut_id(world: *world_t, record: *record_t, id: id_t) ?*anyopaque`
pub const record_get_mut_id = ecs_record_get_mut_id;
extern fn ecs_record_get_mut_id(world: *world_t, record: *record_t, id: id_t) ?*anyopaque;

/// `pub fn record_has_id(world: *world_t, record: *const record_t, id: id_t) bool`
pub const record_has_id = ecs_record_has_id;
extern fn ecs_record_has_id(world: *world_t, record: *const record_t, id: id_t) bool;

/// `pub fn emplace_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque`
pub const emplace_id = ecs_emplace_id;
extern fn ecs_emplace_id(world: *world_t, entity: entity_t, id: id_t) ?*anyopaque;

/// `pub fn modified_id(world: *world_t, entity: entity_t, id: id_t) void`
pub const modified_id = ecs_modified_id;
extern fn ecs_modified_id(world: *world_t, entity: entity_t, id: id_t) void;

/// `pub fn set_id(world: *world_t, entity: entity_t, id: id_t, size: usize, ptr: ?*const anyopaque) entity_t`
pub const set_id = ecs_set_id;
extern fn ecs_set_id(world: *world_t, entity: entity_t, id: id_t, size: usize, ptr: ?*const anyopaque) entity_t;
//--------------------------------------------------------------------------------------------------
//
// Functions for testing and modifying entity liveliness.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn is_valid(world: *const world_t, entity: entity_t) bool`
pub const is_valid = ecs_is_valid;
extern fn ecs_is_valid(world: *const world_t, entity: entity_t) bool;

/// `pub fn is_alive(world: *const world_t, entity: entity_t) bool`
pub const is_alive = ecs_is_alive;
extern fn ecs_is_alive(world: *const world_t, entity: entity_t) bool;

/// `pub fn strip_generation(entity: entity_t) id_t`
pub const strip_generation = ecs_strip_generation;
extern fn ecs_strip_generation(entity: entity_t) id_t;

/// `pub fn set_entity_generation(world: *world_t, entity: entity_t) void`
pub const set_entity_generation = ecs_set_version;
extern fn ecs_set_version(world: *world_t, entity: entity_t) void;

/// `pub fn get_alive(world: *const world_t, entity: entity_t) entity_t`
pub const get_alive = ecs_get_alive;
extern fn ecs_get_alive(world: *const world_t, entity: entity_t) entity_t;

/// `pub fn ensure(world: *world_t, entity: entity_t) void`
pub const ensure = ecs_ensure;
extern fn ecs_ensure(world: *world_t, entity: entity_t) void;

/// `pub fn ensure_id(world: *world_t, id: id_t) void`
pub const ensure_id = ecs_ensure_id;
extern fn ecs_ensure_id(world: *world_t, id: id_t) void;

/// `pub fn exists(world: *const world_t, entity: entity_t) bool`
pub const exists = ecs_exists;
extern fn ecs_exists(world: *const world_t, entity: entity_t) bool;
//--------------------------------------------------------------------------------------------------
//
// Get information from entity.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn get_type(world: *const world_t, entity: entity_t) ?*const type_t`
pub const get_type = ecs_get_type;
extern fn ecs_get_type(world: *const world_t, entity: entity_t) ?*const type_t;

/// `pub fn get_table(world: *const world_t, entity: entity_t) ?*const table_t`
pub const get_table = ecs_get_table;
extern fn ecs_get_table(world: *const world_t, entity: entity_t) ?*const table_t;

/// `pub fn type_str(world: *const world_t, type: ?*const type_t) ?[*:0]u8`
pub const type_str = ecs_type_str;
extern fn ecs_type_str(world: *const world_t, type: ?*const type_t) ?[*:0]u8;

/// `pub fn table_str(world: *const world_t, table: ?*const table_t) ?[*:0]u8`
pub const table_str = ecs_table_str;
extern fn ecs_table_str(world: *const world_t, table: ?*const table_t) ?[*:0]u8;

/// `pub fn entity_str(world: *const world_t, entity: entity_t) ?[*:0]u8`
pub const entity_str = ecs_entity_str;
extern fn ecs_entity_str(world: *const world_t, entity: entity_t) ?[*:0]u8;

/// `pub fn has_id(world: *const world_t, entity: entity_t, id: id_t) bool`
pub const has_id = ecs_has_id;
extern fn ecs_has_id(world: *const world_t, entity: entity_t, id: id_t) bool;

/// `pub fn owns_id(world: *const world_t, entity: entity_t, id: id_t) bool`
pub const owns_id = ecs_owns_id;
extern fn ecs_owns_id(world: *const world_t, entity: entity_t, id: id_t) bool;

/// `pub fn get_target(world: *const world_t, entity: entity_t, rel: entity_t, index: i32) entity_t`
pub const get_target = ecs_get_target;
extern fn ecs_get_target(world: *const world_t, entity: entity_t, rel: entity_t, index: i32) entity_t;

/// `pub fn get_target(world: *const world_t, entity: entity_t, rel: entity_t, index: i32) entity_t`
pub const get_parent = ecs_get_parent;
extern fn ecs_get_parent(world: *const world_t, entity: entity_t) entity_t;

/// `pub fn get_target_for_id(world: *const world_t, entity: entity_t, rel: entity_t, id: id_t) entity_t`
pub const get_target_for_id = ecs_get_target_for_id;
extern fn ecs_get_target_for_id(world: *const world_t, entity: entity_t, rel: entity_t, id: id_t) entity_t;

/// `pub fn get_depth(world: *const world_t, entity: entity_t, rel: entity_t) i32`
pub const get_depth = ecs_get_depth;
extern fn ecs_get_depth(world: *const world_t, entity: entity_t, rel: entity_t) i32;

/// `pub fn count_id(world: *const world_t, entity: entity_t) i32`
pub const count_id = ecs_count_id;
extern fn ecs_count_id(world: *const world_t, entity: entity_t) i32;
//--------------------------------------------------------------------------------------------------
//
// Functions for working with entity names and paths.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn get_name(world: *const world_t, entity: entity_t) ?[*:0]const u8`
pub const get_name = ecs_get_name;
extern fn ecs_get_name(world: *const world_t, entity: entity_t) ?[*:0]const u8;

/// `pub fn get_symbol(world: *const world_t, entity: entity_t) ?[*:0]const u8`
pub const get_symbol = ecs_get_symbol;
extern fn ecs_get_symbol(world: *const world_t, entity: entity_t) ?[*:0]const u8;

/// `pub fn set_name(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t`
pub const set_name = ecs_set_name;
extern fn ecs_set_name(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t;

/// `pub fn set_symbol(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t`
pub const set_symbol = ecs_set_symbol;
extern fn ecs_set_symbol(world: *world_t, entity: entity_t, name: ?[*:0]const u8) entity_t;

/// `pub fn set_alias(world: *world_t, entity: entity_t, name: ?[*:0]const u8) void`
pub const set_alias = ecs_set_alias;
extern fn ecs_set_alias(world: *world_t, entity: entity_t, name: ?[*:0]const u8) void;

/// `pub fn lookup(world: *const world_t, name: ?[*:0]const u8) entity_t`
pub const lookup = ecs_lookup;
extern fn ecs_lookup(world: *const world_t, name: ?[*:0]const u8) entity_t;

/// `pub fn lookup_child(world: *const world_t, parent: entity_t, name: ?[*:0]const u8) entity_t`
pub const lookup_child = ecs_lookup_child;
extern fn ecs_lookup_child(world: *const world_t, parent: entity_t, name: ?[*:0]const u8) entity_t;

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
extern fn ecs_lookup_path_w_sep(
    world: *const world_t,
    parent: entity_t,
    path: ?[*:0]const u8,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
    recursive: bool,
) entity_t;

/// `pub fn lookup_symbol(world: *const world_t, symbol: ?[*:0]const u8, lookup_as_path: bool, recursive: bool) entity_t`
pub const lookup_symbol = ecs_lookup_symbol;
extern fn ecs_lookup_symbol(world: *const world_t, symbol: ?[*:0]const u8, lookup_as_path: bool, recursive: bool) entity_t;

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
extern fn ecs_get_path_w_sep(
    world: *const world_t,
    parent: entity_t,
    child: entity_t,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
) ?[*]u8;

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
extern fn ecs_new_from_path_w_sep(
    world: *world_t,
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
extern fn ecs_add_path_w_sep(
    world: *world_t,
    entity: entity_t,
    parent: entity_t,
    path: ?[*:0]const u8,
    sep: ?[*:0]const u8,
    prefix: ?[*:0]const u8,
) entity_t;

/// `pub fn set_scope(world: *world_t, scope: entity_t) entity_t`
pub const set_scope = ecs_set_scope;
extern fn ecs_set_scope(world: *world_t, scope: entity_t) entity_t;

/// `pub fn get_scope(world: *const world_t) entity_t`
pub const get_scope = ecs_get_scope;
extern fn ecs_get_scope(world: *const world_t) entity_t;

/// `pub fn set_name_prefix(world: *world_t, prefix: ?[*:0]const u8) ?[*:0]const u8`
pub const set_name_prefix = ecs_set_name_prefix;
extern fn ecs_set_name_prefix(world: *world_t, prefix: ?[*:0]const u8) ?[*:0]const u8;

/// `pub fn set_lookup_path(world: *world_t, lookup_path: ?[*]const entity_t) ?[*]entity_t`
pub const set_lookup_path = ecs_set_lookup_path;
extern fn ecs_set_lookup_path(world: *world_t, lookup_path: ?[*:0]const entity_t) ?[*]entity_t;

/// `pub fn get_lookup_path(world: *const world_t) ?[*]entity_t`
pub const get_lookup_path = ecs_get_lookup_path;
extern fn ecs_get_lookup_path(world: *const world_t) ?[*]entity_t;
//--------------------------------------------------------------------------------------------------
//
// Functions for registering and working with components.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn component_init(world: *world_t, desc: *const component_desc_t) entity_t`
pub const component_init = ecs_component_init;
extern fn ecs_component_init(world: *world_t, desc: *const component_desc_t) entity_t;

/// `pub fn get_type_info(world: *const world_t, id: id_t) *const type_info_t`
pub const get_type_info = ecs_get_type_info;
extern fn ecs_get_type_info(world: *const world_t, id: id_t) *const type_info_t;

/// `pub fn set_hooks_id(world: *world_t, id: entity_t, hooks: *const type_hooks_t) void`
pub const set_hooks_id = ecs_set_hooks_id;
extern fn ecs_set_hooks_id(world: *world_t, id: entity_t, hooks: *const type_hooks_t) void;

/// `pub fn get_hooks_id(world: *const world_t, id: entity_t) *const type_hooks_t`
pub const get_hooks_id = ecs_get_hooks_id;
extern fn ecs_get_hooks_id(world: *const world_t, id: entity_t) *const type_hooks_t;

/// `pub fn id_is_tag(world: *const world_t, id: id_t) bool;
pub const id_is_tag = ecs_id_is_tag;
extern fn ecs_id_is_tag(world: *const world_t, id: id_t) bool;

/// `pub fn id_is_union(world: *const world_t, id: id_t) bool;
pub const id_is_union = ecs_id_is_union;
extern fn ecs_id_is_union(world: *const world_t, id: id_t) bool;

/// `pub fn id_in_use(world: *const world_t, id: id_t) bool`
pub const id_in_use = ecs_id_in_use;
extern fn ecs_id_in_use(world: *const world_t, id: id_t) bool;

/// `pub fn get_typeid(world: *const world_t, id: id_t) entity_t`
pub const get_typeid = ecs_get_typeid;
extern fn ecs_get_typeid(world: *const world_t, id: id_t) entity_t;

/// `pub fn id_match(id: id_t, pattern: id_t) bool`
pub const id_match = ecs_id_match;
extern fn ecs_id_match(id: id_t, pattern: id_t) bool;

/// `pub fn id_is_pair(id: id_t) bool`
pub const id_is_pair = ecs_id_is_pair;
extern fn ecs_id_is_pair(id: id_t) bool;

/// `pub fn id_is_wildcard(id: id_t) bool`
pub const id_is_wildcard = ecs_id_is_wildcard;
extern fn ecs_id_is_wildcard(id: id_t) bool;

/// `pub fn id_is_valid(world: *const world_t, id: id_t) bool`
pub const id_is_valid = ecs_id_is_valid;
extern fn ecs_id_is_valid(world: *const world_t, id: id_t) bool;

/// `pub fn id_get_flags(world: *const world_t, id: id_t) flags32_t`
pub const id_get_flags = ecs_id_get_flags;
extern fn ecs_id_get_flags(world: *const world_t, id: id_t) flags32_t;

/// `pub fn id_flag_str(id_flags: id_t) ?[*]const u8`
pub const id_flag_str = ecs_id_flag_str;
extern fn ecs_id_flag_str(id_flags: id_t) ?[*:0]const u8;

/// `pub fn id_str(world: *const world_t, id: id_t) ?[*]u8`
pub const id_str = ecs_id_str;
extern fn ecs_id_str(world: *const world_t, id: id_t) ?[*:0]u8;
//--------------------------------------------------------------------------------------------------
//
// Functions for working with `term_t` and `query_t`.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn term_iter(world: *const world_t, term: *term_t) iter_t`
pub const each = ecs_each_id;
extern fn ecs_each_id(world: *const world_t, term: *term_t) iter_t;

/// `pub fn term_chain_iter(world: *const world_t, term: *term_t) iter_t`
pub const term_chain_iter = ecs_term_chain_iter;
extern fn ecs_term_chain_iter(world: *const world_t, term: *term_t) iter_t;

/// `pub fn term_next(it: *iter_t) bool`
pub const each_next = ecs_each_next;
extern fn ecs_each_next(it: *iter_t) bool;

/// `pub fn children(world: *const world_t, parent: entity_t) iter_t`
pub const children = ecs_children;
extern fn ecs_children(world: *const world_t, parent: entity_t) iter_t;

/// `pub fn children_next(it: *iter_t) bool`
pub const children_next = ecs_children_next;
extern fn ecs_children_next(it: *iter_t) bool;

/// `pub fn term_id_is_set(id: *term_id_t) bool`
pub const term_id_is_set = ecs_term_id_is_set;
extern fn ecs_term_id_is_set(id: *term_ref_t) bool;

/// `pub fn term_is_initialized(term: *const term_t) bool`
pub const term_is_initialized = ecs_term_is_initialized;
extern fn ecs_term_is_initialized(term: *const term_t) bool;

/// `pub fn term_match_this(term: *const term_t) bool`
pub const term_match_this = ecs_term_match_this;
extern fn ecs_term_match_this(term: *const term_t) bool;

/// `pub fn term_match_0(term: *const term_t) bool`
pub const term_match_0 = ecs_term_match_0;
extern fn ecs_term_match_0(term: *const term_t) bool;

/// `pub fn term_finalize(world: *const world_t, term: *term_t) i32`
pub const term_finalize = ecs_term_finalize;
extern fn ecs_term_finalize(world: *const world_t, term: *term_t) i32;

/// `pub fn term_str(world: *const world_t, term: *const term_t) ?[*:0]u8`
pub const term_str = ecs_term_str;
extern fn ecs_term_str(world: *const world_t, term: *const term_t) ?[*:0]u8;

//--------------------------------------------------------------------------------------------------
//
// Functions for working with `query_t`.
//
//--------------------------------------------------------------------------------------------------
pub fn query_init(world: *world_t, desc: *const query_desc_t) error_t!*query_t {
    return ecs_query_init(world, desc) orelse return make_error();
}
extern fn ecs_query_init(world: *world_t, desc: *const query_desc_t) ?*query_t;

/// `pub fn query_fini(query: *query_t) void`
pub const query_fini = ecs_query_fini;
extern fn ecs_query_fini(query: *query_t) void;

/// `pub fn query_iter(world: *const world_t: query: *const query_t) iter_t`
pub const query_iter = ecs_query_iter;
extern fn ecs_query_iter(world: *const world_t, query: *const query_t) iter_t;

/// `pub fn query_next(iter: *iter_t) bool`
pub const query_next = ecs_query_next;
extern fn ecs_query_next(iter: *iter_t) bool;

/// `pub fn query_next_table(iter: *iter_t) bool`
pub const query_next_table = ecs_query_next_table;
extern fn ecs_query_next_table(iter: *iter_t) bool;

/// `pub fn query_populate(iter: *iter_t, when_changed: bool) c_int`
pub const query_populate = ecs_query_populate;
extern fn ecs_query_populate(iter: *iter_t, when_changed: bool) c_int;

/// `pub fn query_changed(query: *query_t) bool`
pub const query_changed = ecs_query_changed;
extern fn ecs_query_changed(query: *query_t) bool;

/// `pub fn iter_skip(iter: *iter_t) void`
pub const iter_skip = ecs_iter_skip;
extern fn ecs_iter_skip(iter: *iter_t) void;

/// `pub fn query_set_group(iter: *iter_t, group_id: u64) void`
pub const iter_set_group = ecs_iter_set_group;
extern fn ecs_iter_set_group(iter: *iter_t, group_id: u64) void;

/// `pub fn query_get_group_ctx(query: *const query_t, group_id: u64) ?*anyopaque`
pub const query_get_group_ctx = ecs_query_get_group_ctx;
extern fn ecs_query_get_group_ctx(query: *const query_t, group_id: u64) ?*anyopaque;

pub const query_group_info_t = extern struct {
    match_count: i32,
    table_count: i32,
    ctx: ?*anyopaque,
};

/// `pub fn query_get_group_info(query: *const query_t, group_id: u64) ?*const query_group_info_t`
pub const query_get_group_info = ecs_query_get_group_info;
extern fn ecs_query_get_group_info(query: *const query_t, group_id: u64) ?*const query_group_info_t;

/// `pub fn query_orphaned(query: *const query_t) bool`
pub const query_orphaned = ecs_query_orphaned;
extern fn ecs_query_orphaned(query: *const query_t) bool;

/// `pub fn query_str(query: *const query_t) [*:0]u8`
pub const query_str = ecs_query_str;
extern fn ecs_query_str(query: *const query_t) [*:0]u8;

/// `pub fn query_table_count(query: *const query_t) i32`
pub const query_table_count = ecs_query_table_count;
extern fn ecs_query_table_count(query: *const query_t) i32;

/// `pub fn query_empty_table_count(query: *const query_t) i32`
pub const query_empty_table_count = ecs_query_empty_table_count;
extern fn ecs_query_empty_table_count(query: *const query_t) i32;

/// `pub fn query_entity_count(query: *const query_t) i32`
pub const query_entity_count = ecs_query_entity_count;
extern fn ecs_query_entity_count(query: *const query_t) i32;

/// `pub fn query_get_ctx(query: *const query_t) i32`
pub const query_get_ctx = ecs_query_get_ctx;
extern fn ecs_query_get_ctx(query: *const query_t) ?*anyopaque;

/// `pub fn query_get_binding_ctx(query: *const query_t) i32`
pub const query_get_binding_ctx = ecs_query_get_binding_ctx;
extern fn ecs_query_get_binding_ctx(query: *const query_t) ?*anyopaque;

//--------------------------------------------------------------------------------------------------
//
// Functions for working with events and observers.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn emit(world: *world_t, desc: *event_desc_t) void`
pub const emit = ecs_emit;
extern fn ecs_emit(world: *world_t, desc: *event_desc_t) void;

/// `pub fn enqueue(world: *world_t, desc: *event_desc_t) void`
pub const enqueue = ecs_enqueue;
extern fn ecs_enqueue(world: *world_t, desc: *event_desc_t) void;

/// `pub fn observer_init(world: *world_t, desc: *const observer_desc_t) entity_t`
pub const observer_init = ecs_observer_init;
extern fn ecs_observer_init(world: *world_t, desc: *const observer_desc_t) entity_t;

/// `pub fn observer_default_run_action(it: *iter_t) bool`
pub const observer_default_run_action = ecs_observer_default_run_action;
extern fn ecs_observer_default_run_action(it: *iter_t) bool;

/// `pub fn observer_get_ctx(world: *const world_t, observer: entity_t) ?*anyopaque`
pub const observer_get_ctx = ecs_observer_get_ctx;
extern fn ecs_observer_get_ctx(world: *const world_t, observer: entity_t) ?*anyopaque;

/// `pub fn observer_get_binding_ctx(world: *const world_t, observer: entity_t) ?*anyopaque`
pub const observer_get_binding_ctx = ecs_observer_get_binding_ctx;
extern fn ecs_observer_get_binding_ctx(world: *const world_t, observer: entity_t) ?*anyopaque;
//--------------------------------------------------------------------------------------------------
//
// Functions for working with `iter_t`.
//
//--------------------------------------------------------------------------------------------------
pub const entities_t = extern struct {
    entities: ?[*]const entity_t,
    count: i32,
    alive_count: i32,
};
/// `pub fn iter_poly(world: *const world_t, poly: *const poly_t, iter: [*]iter_t, filter: ?*term_t) void`
pub const get_entities = ecs_get_entities;
extern fn ecs_get_entities(world: *const world_t) entities_t;

/// `pub fn iter_next(it: *iter_t) bool`
pub const iter_next = ecs_iter_next;
extern fn ecs_iter_next(it: *iter_t) bool;

/// `pub fn iter_fini(it: *iter_t) void`
pub const iter_fini = ecs_iter_fini;
extern fn ecs_iter_fini(it: *iter_t) void;

/// `pub fn iter_count(it: *iter_t) i32`
pub const iter_count = ecs_iter_count;
extern fn ecs_iter_count(it: *iter_t) i32;

/// `pub fn iter_is_true(it: *iter_t) bool`
pub const iter_is_true = ecs_iter_is_true;
extern fn ecs_iter_is_true(it: *iter_t) bool;

/// `pub fn iter_first(it: *iter_t) entity_t`
pub const iter_first = ecs_iter_first;
extern fn ecs_iter_first(it: *iter_t) entity_t;

/// `pub fn iter_set_var(it: *iter_t, var_id: i32, entity: entity_t) void`
pub const iter_set_var = ecs_iter_set_var;
extern fn ecs_iter_set_var(it: *iter_t, var_id: i32, entity: entity_t) void;

/// `pub fn iter_set_var_as_table(it: *iter_t, var_id: i32, table: *const table_t) void`
pub const iter_set_var_as_table = ecs_iter_set_var_as_table;
extern fn ecs_iter_set_var_as_table(it: *iter_t, var_id: i32, table: *const table_t) void;

/// `pub fn iter_set_var_as_range(it: *iter_t, var_id: i32, range: *const table_range_t) void`
pub const iter_set_var_as_range = ecs_iter_set_var_as_range;
extern fn ecs_iter_set_var_as_range(it: *iter_t, var_id: i32, range: *const table_range_t) void;

/// `pub fn iter_get_var(it: *iter_t, var_id: i32) entity_t`
pub const iter_get_var = ecs_iter_get_var;
extern fn ecs_iter_get_var(it: *iter_t, var_id: i32) entity_t;

/// `pub fn iter_get_var_as_table(it: *iter_t, var_id: i32) ?*table_t`
pub const iter_get_var_as_table = ecs_iter_get_var_as_table;
extern fn ecs_iter_get_var_as_table(it: *iter_t, var_id: i32) ?*table_t;

/// `pub fn iter_get_var_as_range(it: *iter_t, var_id: i32) table_range_t`
pub const iter_get_var_as_range = ecs_iter_get_var_as_range;
extern fn ecs_iter_get_var_as_range(it: *iter_t, var_id: i32) table_range_t;

/// `pub fn iter_var_is_constrained(it: *iter_t, var_id: i32) bool`
pub const iter_var_is_constrained = ecs_iter_var_is_constrained;
extern fn ecs_iter_var_is_constrained(it: *iter_t, var_id: i32) bool;

/// `pub fn iter_str(it: *const iter_t) ?[*:0]u8`
pub const iter_str = ecs_iter_str;
extern fn ecs_iter_str(it: *const iter_t) ?[*:0]u8;

/// `pub fn page_iter(it: *const iter_t, offset: i32, limit: i32) iter_t`
pub const page_iter = ecs_page_iter;
extern fn ecs_page_iter(it: *const iter_t, offset: i32, limit: i32) iter_t;

/// `pub fn page_next(it: *iter_t) bool`
pub const page_next = ecs_page_next;
extern fn ecs_page_next(it: *iter_t) bool;

/// `pub fn worker_iter(it: *const iter_t, index: i32, count: i32) iter_t`
pub const worker_iter = ecs_worker_iter;
extern fn ecs_worker_iter(it: *const iter_t, index: i32, count: i32) iter_t;

/// `pub fn field_w_size(it: *const iter_t, size: usize, index: i8) ?*anyopaque`
pub const field_w_size = ecs_field_w_size;
extern fn ecs_field_w_size(it: *const iter_t, size: usize, index: i8) ?*anyopaque;

/// `pub fn field_w_size(it: *const iter_t, size: usize, index: i8) ?*anyopaque`
pub const ecs_field_at_w_size = ecs_ecs_field_at_w_size;
extern fn ecs_ecs_field_at_w_size(it: *const iter_t, size: usize, index: i8, row: i32) ?*anyopaque;

/// `pub fn field_is_readonly(it: *const iter_t, index: i8) bool`
pub const field_is_readonly = ecs_field_is_readonly;
extern fn ecs_field_is_readonly(it: *const iter_t, index: i8) bool;

/// `pub fn field_is_writeonly(it: *const iter_t, index: i8) bool`
pub const field_is_writeonly = ecs_field_is_writeonly;
extern fn ecs_field_is_writeonly(it: *const iter_t, index: i8) bool;

/// `pub fn field_is_set(it: *const iter_t, index: i8) bool`
pub const field_is_set = ecs_field_is_set;
extern fn ecs_field_is_set(it: *const iter_t, index: i8) bool;

/// `pub fn field_id(it: *const iter_t, index: i8) id_t`
pub const field_id = ecs_field_id;
extern fn ecs_field_id(it: *const iter_t, index: i8) id_t;

/// `pub fn field_column(it: *const iter_t, index: i8) i32`
pub const field_column = ecs_field_column;
extern fn ecs_field_column(it: *const iter_t, index: i8) i32;

/// `pub fn field_src(it: *const iter_t, index: i8) entity_t`
pub const field_src = ecs_field_src;
extern fn ecs_field_src(it: *const iter_t, index: i8) entity_t;

/// `pub fn field_size(it: *const iter_t, index: i8) usize`
pub const field_size = ecs_field_size;
extern fn ecs_field_size(it: *const iter_t, index: i8) usize;

/// `pub fn field_is_self(it: *const iter_t, index: i8) bool`
pub const field_is_self = ecs_field_is_self;
extern fn ecs_field_is_self(it: *const iter_t, index: i8) bool;
//--------------------------------------------------------------------------------------------------
//
// Functions for working with `table_t`.
//
//--------------------------------------------------------------------------------------------------
/// `pub fn table_get_type(table: *const table_t) *const type_t`
pub const table_get_type = ecs_table_get_type;
extern fn ecs_table_get_type(table: *const table_t) *const type_t;

/// `pub fn table_get_type_index(world: *const world_t, table: *const table_t, id: id_t) i32`
pub const table_get_type_index = ecs_table_get_type_index;
extern fn ecs_table_get_type_index(world: *const world_t, table: *const table_t, id: id_t) i32;

/// `pub fn table_get_column_index(world: *const world_t, table: *const table_t, id: id_t) i32`
pub const table_get_column_index = ecs_table_get_column_index;
extern fn ecs_table_get_column_index(world: *const world_t, table: *const table_t, id: id_t) i32;

/// `pub fn table_column_count(table: *const table_t) i32`
pub const table_column_count = ecs_table_column_count;
extern fn ecs_table_column_count(table: *const table_t) i32;

/// `pub fn table_type_to_column_index(table: *const table_t, index:i32) i32`
pub const table_type_to_column_index = ecs_table_type_to_column_index;
extern fn ecs_table_type_to_column_index(table: *const table_t, index: i32) i32;

/// `pub fn table_column_to_type_index(table: *const table_t, index:i32) i32`
pub const table_column_to_type_index = ecs_table_column_to_type_index;
extern fn ecs_table_column_to_type_index(table: *const table_t, index: i32) i32;

/// `pub fn table_get_column(table: *const table_t, index: i32, offset: i32) ?*anyopaque`
pub const table_get_column = ecs_table_get_column;
extern fn ecs_table_get_column(table: *const table_t, index: i32, offset: i32) ?*anyopaque;

/// `pub fn table_get_id(world: *const world_t, table: *const table_t, id:id_t, offset: i32) ?*anyopaque`
pub const table_get_id = ecs_table_get_id;
extern fn ecs_table_get_id(world: *const world_t, table: *const table_t, id: id_t, offset: i32) ?*anyopaque;

/// `pub fn table_get_column_size(table: *const table_t, index: i32, offset: i32) ?*anyopaque`
pub const table_get_column_size = ecs_table_get_column_size;
extern fn ecs_table_get_column_size(table: *const table_t, index: i32) usize;

/// `pub fn table_count(table: *const table_t) i32`
pub const table_count = ecs_table_count;
extern fn ecs_table_count(table: *const table_t) i32;

/// `pub fn table_count(table: *const table_t) i32`
pub const table_size = ecs_table_size;
extern fn ecs_table_size(table: *const table_t) i32;

/// `pub fn table_count(table: *const table_t) i32`
pub const table_entities = ecs_table_entities;
extern fn ecs_table_entities(table: *const table_t) [*]entity_t;

/// `pub fn table_has_id(world: *const world_t, table: *const table_t, id: id_t) bool`
pub const table_has_id = ecs_table_has_id;
extern fn ecs_table_has_id(world: *const world_t, table: *const table_t, id: id_t) bool;

/// `pub fn table_get_depth(world: *const world_t, table: *const table_t, rel: entity_t) i32`
pub const table_get_depth = ecs_table_get_depth;
extern fn ecs_table_get_depth(world: *const world_t, table: *const table_t, rel: entity_t) i32;

/// `pub fn table_add_id(world: *world_t, table: *table_t, id: id_t) *table_t`
pub const table_add_id = ecs_table_add_id;
extern fn ecs_table_add_id(world: *world_t, table: *table_t, id: id_t) *table_t;

/// `pub fn table_find(world: *world_t, ids: [*]const id_t, id_count: i32) *table_t`
pub const table_find = ecs_table_find;
extern fn ecs_table_find(world: *world_t, ids: [*]const id_t, id_count: i32) *table_t;

/// `pub fn table_remove_id(world: *world_t, table: *table_t, id: id_t) *table_t`
pub const table_remove_id = ecs_table_remove_id;
extern fn ecs_table_remove_id(world: *world_t, table: *table_t, id: id_t) *table_t;

/// `pub fn table_lock(world: *world_t, table: *table_t) void`
pub const table_lock = ecs_table_lock;
extern fn ecs_table_lock(world: *world_t, table: *table_t) void;

/// `pub fn table_unlock(world: *world_t, table: *table_t) void`
pub const table_unlock = ecs_table_unlock;
extern fn ecs_table_unlock(world: *world_t, table: *table_t) void;

/// `pub fn table_has_flags(table: *table_t, flags: flags32_t) bool`
pub const table_has_flags = ecs_table_has_flags;
extern fn ecs_table_has_flags(table: *table_t, flags: flags32_t) bool;

/// `pub fn table_has_module(table: *const table_t) bool`
pub const table_has_module = ecs_table_has_module;
extern fn ecs_table_has_module(table: *const table_t) bool;

/// `pub fn table_swap_rows(world: *world_t, table: *table_t, row_1: i32, row_2: i32) void`
pub const table_swap_rows = ecs_table_swap_rows;
extern fn ecs_table_swap_rows(world: *world_t, table: *table_t, row_1: i32, row_2: i32) void;

/// ```
/// extern fn commit(
///     world: *world_t,
///     entity: entity_t,
///     record: ?*record_t,
///     table: *table_t,
///     added: ?*const type_t,
///     removed: ?*const type_t,
/// ) void;
/// ```
pub const commit = ecs_commit;
extern fn ecs_commit(
    world: *world_t,
    entity: entity_t,
    record: ?*record_t,
    table: *table_t,
    added: ?*const type_t,
    removed: ?*const type_t,
) void;

/// `pub fn record_find(world: *const world_t, entity: entity_t) ?*record_t`
pub const record_find = ecs_record_find;
extern fn ecs_record_find(world: *const world_t, entity: entity_t) ?*record_t;

/// `pub fn record_get_column(r: *const record_t, column: i32, c_size: usize) ?*anyopaque`
pub const record_get_column = ecs_record_get_column;
extern fn ecs_record_get_column(r: *const record_t, column: i32, c_size: usize) ?*anyopaque;

/// `pub fn search(world: *const world_t, table: *const table_t, id: id_t, id_out: ?*id_t) i32`
pub const search = ecs_search;
extern fn ecs_search(world: *const world_t, table: *const table_t, id: id_t, id_out: ?*id_t) i32;

/// ```
/// extern fn search_offset(
///     world: *const world_t,
///     table: *const table_t,
///     offset: i32,
///     id: id_t,
///     id_out: ?*id_t,
/// ) i32;
/// ```
pub const search_offset = ecs_search_offset;
extern fn ecs_search_offset(
    world: *const world_t,
    table: *const table_t,
    offset: i32,
    id: id_t,
    id_out: ?*id_t,
) i32;

/// ```
/// extern fn search_relation(
///     world: *const world_t,
///     table: *const table_t,
///     offset: i32,
///     id: id_t,
///     rel: entity_t,
///     flags: flags32_t,
///     subject_out: ?*entity_t,
///     id_out: ?*id_t,
///     tr_out: ?**table_record_t,
/// ) i32;
/// ```
pub const search_relation = ecs_search_relation;
extern fn ecs_search_relation(
    world: *const world_t,
    table: *const table_t,
    offset: i32,
    id: id_t,
    rel: entity_t,
    flags: flags32_t,
    subject_out: ?*entity_t,
    id_out: ?*id_t,
    tr_out: ?**table_record_t,
) i32;
//--------------------------------------------------------------------------------------------------
//
// Log api
//
//--------------------------------------------------------------------------------------------------

pub const log_set_level = ecs_log_set_level;
extern fn ecs_log_set_level(level: c_int) c_int;

pub const log_get_level = ecs_log_get_level;
extern fn ecs_log_get_level() c_int;

pub const log_enable_colors = ecs_log_enable_colors;
extern fn ecs_log_enable_colors(enabled: bool) bool;

pub const log_enable_timestamp = ecs_log_enable_timestamp;
extern fn ecs_log_enable_timestamp(enabled: bool) bool;

pub const log_enable_timedelta = ecs_log_enable_timedelta;
extern fn ecs_log_enable_timedelta(enabled: bool) bool;

pub const log_last_error = ecs_log_last_error;
extern fn ecs_log_last_error() c_int;

//--------------------------------------------------------------------------------------------------
//
// Construct, destruct, copy and move dynamically created values.
//
//--------------------------------------------------------------------------------------------------
pub fn value_init(world: *const world_t, value_type: entity_t, ptr: *anyopaque) error_t!void {
    if (ecs_value_init(world, value_type, ptr) != 0) return make_error();
}
extern fn ecs_value_init(world: *const world_t, value_type: entity_t, ptr: *anyopaque) i32;

pub fn value_init_w_type_info(world: *const world_t, ti: *const type_info_t, ptr: *anyopaque) error_t!void {
    if (ecs_value_init_w_type_info(world, ti, ptr) != 0) return make_error();
}
extern fn ecs_value_init_w_type_info(world: *const world_t, ti: *const type_info_t, ptr: *anyopaque) i32;

// TODO: Add missing functions
//--------------------------------------------------------------------------------------------------
/// `pub fn progress(world: *world_t, delta_time: ftime_t) bool`
pub const progress = ecs_progress;
extern fn ecs_progress(world: *world_t, delta_time: ftime_t) bool;

/// `pub fn set_time_scale(world: *world_t, scale: ftime_t) void`
pub const set_time_scale = ecs_set_time_scale;
extern fn ecs_set_time_scale(world: *world_t, scale: ftime_t) void;

/// `pub fn reset_clock(world: *world_t) void`
pub const reset_clock = ecs_reset_clock;
extern fn ecs_reset_clock(world: *world_t) void;

/// `pub fn ecs_run_pipeline(world: *world_t, pipeline: entity_t, delta_time: ftime_t) void`
pub const run_pipeline = ecs_run_pipeline;
extern fn ecs_run_pipeline(world: *world_t, pipeline: entity_t, delta_time: ftime_t) void;

/// `pub fn ecs_set_threads(world: *world_t, threads: i32) void`
pub const set_threads = ecs_set_threads;
extern fn ecs_set_threads(world: *world_t, threads: i32) void;

/// `pub fn ecs_set_task_threads(world: *world_t, task_threads: i32) void`
pub const set_task_threads = ecs_set_task_threads;
extern fn ecs_set_task_threads(world: *world_t, task_threads: i32) void;

/// `pub fn ecs_using_task_threads(world: *world_t) bool`
pub const using_task_threads = ecs_using_task_threads;
extern fn ecs_using_task_threads(world: *world_t) bool;

//--------------------------------------------------------------------------------------------------
//
// Declarative functions (ECS_* macros in flecs)
//
//--------------------------------------------------------------------------------------------------
// TODO: We support only one `world_t` at the time because type ids are stored in a global static memory.
// We need to reset those ids to zero when the world is destroyed
// (we do this in `pub fn fini(world: *world_t) i32`).
var num_worlds: u32 = 0;
var component_ids_hm = std.AutoHashMap(*id_t, u0).init(std.heap.page_allocator);

pub fn COMPONENT(world: *world_t, comptime T: type) void {
    if (@sizeOf(T) == 0)
        @compileError("Size of the type must be greater than zero");

    const type_id_ptr = perTypeGlobalVarPtr(T);
    if (type_id_ptr.* != 0)
        return;

    component_ids_hm.put(type_id_ptr, 0) catch @panic("OOM");

    type_id_ptr.* = ecs_component_init(world, &.{
        .entity = ecs_entity_init(world, &.{
            .use_low_id = true,
            .name = typeName(T),
            .symbol = typeName(T),
        }),
        .type = .{
            .alignment = @alignOf(T),
            .size = @sizeOf(T),
            .hooks = .{
                .dtor = switch (@typeInfo(T)) {
                    .Struct => if (@hasDecl(T, "dtor")) struct {
                        pub fn dtor(ptr: *anyopaque, _: i32, _: *const type_info_t) callconv(.C) void {
                            T.dtor(@as(*T, @alignCast(@ptrCast(ptr))).*);
                        }
                    }.dtor else null,
                    else => null,
                },
            },
        },
    });
}

pub fn TAG(world: *world_t, comptime T: type) void {
    if (@sizeOf(T) != 0)
        @compileError("Size of the type must be zero");

    const type_id_ptr = perTypeGlobalVarPtr(T);
    if (type_id_ptr.* != 0)
        return;

    component_ids_hm.put(type_id_ptr, 0) catch @panic("OOM");

    type_id_ptr.* = ecs_entity_init(world, &.{ .name = typeName(T) });
}

pub fn SYSTEM(
    world: *world_t,
    name: [*:0]const u8,
    phase: entity_t,
    system_desc: *system_desc_t,
) void {
    var entity_desc = entity_desc_t{};
    entity_desc.id = new_id(world);
    entity_desc.name = name;
    const first = if (phase != 0) pair(EcsDependsOn, phase) else 0;
    const second = phase;
    entity_desc.add = &.{ first, second, 0 };

    system_desc.entity = entity_init(world, &entity_desc);
    _ = system_init(world, system_desc);
}

pub fn OBSERVER(
    world: *world_t,
    name: [*:0]const u8,
    observer_desc: *observer_desc_t,
) void {
    var entity_desc = entity_desc_t{};
    entity_desc.id = new_id(world);
    entity_desc.name = name;

    observer_desc.entity = entity_init(world, &entity_desc);
    _ = observer_init(world, observer_desc);
}

/// Implements a flecs system from function parameters.
/// For instance, the function below
/// fn move_system(positions: []Position, velocities: []const Velocity) void {
///     for (positions, velocities) |*p, *v| {
///         p.x += v.x;
///         p.y += v.y;
///     }
/// }
/// Would return the following implementation
/// fn exec(it: *ecs.iter_t) callconv(.C) void {
///     const c1 = ecs.field(it, Position, 0).?;
///     const c2 = ecs.field(it, Velocity, 1).?;
///     move_system(c1, c2);//probably inlined
// }
fn SystemImpl(comptime fn_system: anytype) type {
    const fn_type = @typeInfo(@TypeOf(fn_system));
    if (fn_type.Fn.params.len == 0) {
        @compileError("System need at least one parameter");
    }

    return struct {
        fn exec(it: *iter_t) callconv(.C) void {
            const ArgsTupleType = std.meta.ArgsTuple(@TypeOf(fn_system));
            var args_tuple: ArgsTupleType = undefined;

            const has_it_param = fn_type.Fn.params[0].type == *iter_t;
            if (has_it_param) {
                args_tuple[0] = it;
            }

            const start_index = if (has_it_param) 1 else 0;

            inline for (start_index..fn_type.Fn.params.len) |i| {
                const p = fn_type.Fn.params[i];
                args_tuple[i] = field(it, @typeInfo(p.type.?).Pointer.child, i - start_index).?;
            }

            //NOTE: .always_inline seems ok, but unsure. Replace to .auto if it breaks
            _ = @call(.always_inline, fn_system, args_tuple);
        }
    };
}

/// Creates system_desc_t from function parameters
pub fn SYSTEM_DESC(comptime fn_system: anytype) system_desc_t {
    const system_struct = SystemImpl(fn_system);

    var system_desc = system_desc_t{};
    system_desc.callback = system_struct.exec;

    const fn_type = @typeInfo(@TypeOf(fn_system)).Fn;
    const has_it_param = fn_type.params[0].type == *iter_t;
    const start_index = if (has_it_param) 1 else 0;
    inline for (start_index..fn_type.params.len) |i| {
        const p = fn_type.params[i];
        const param_type_info = @typeInfo(p.type.?).Pointer;
        const inout = if (param_type_info.is_const) .In else .InOut;
        system_desc.query.terms[i - start_index] = .{ .id = id(param_type_info.child), .inout = inout };
    }

    return system_desc;
}

/// Creates system_desc_t from function parameters.
/// Accepts additional query terms
pub fn SYSTEM_DESC_WITH_FILTERS(comptime fn_system: anytype, filters: []const term_t) system_desc_t {
    const fn_type = @typeInfo(@TypeOf(fn_system)).Fn;
    var system_desc = SYSTEM_DESC(fn_system);

    const has_it_param = fn_type.params[0].type == *iter_t;
    const start_index = if (has_it_param) 1 else 0;
    for (filters, 0..) |t, i| {
        system_desc.query.terms[i + fn_type.params.len - start_index] = t;
    }

    return system_desc;
}

/// Creates a system description and adds it to the world, from function parameters
pub fn ADD_SYSTEM(
    world: *world_t,
    name: [*:0]const u8,
    phase: entity_t,
    comptime fn_system: anytype,
) void {
    var desc = SYSTEM_DESC(fn_system);
    SYSTEM(world, name, phase, &desc);
}

/// Creates a system description and adds it to the world, from function parameters
/// Accepts aditional filter terms
pub fn ADD_SYSTEM_WITH_FILTERS(
    world: *world_t,
    name: [*:0]const u8,
    phase: entity_t,
    comptime fn_system: anytype,
    filters: []const term_t,
) void {
    var desc = SYSTEM_DESC_WITH_FILTERS(fn_system, filters);
    SYSTEM(world, name, phase, &desc);
}

pub fn new_entity(world: *world_t, name: [*:0]const u8) entity_t {
    return entity_init(world, &.{ .name = name });
}

pub fn new_prefab(world: *world_t, name: [*:0]const u8) entity_t {
    return entity_init(world, &.{
        .name = name,
        .add = [_]id_t{EcsPrefab} ++ [_]id_t{0} ** (FLECS_ID_DESC_MAX - 1),
    });
}

pub fn add_pair(world: *world_t, subject: entity_t, first: entity_t, second: entity_t) void {
    add_id(world, subject, pair(first, second));
}

pub fn set_pair(
    world: *world_t,
    subject: entity_t,
    first: entity_t,
    second: entity_t,
    comptime T: type,
    val: T,
) entity_t {
    return ecs_set_id(world, subject, pair(first, second), @sizeOf(T), @as(*const anyopaque, @ptrCast(@alignCast(&val))));
}

pub fn get_pair(
    world: *world_t,
    subject: entity_t,
    first: entity_t,
    second: entity_t,
    comptime T: type,
) ?*const T {
    if (get_id(world, subject, pair(first, second))) |ptr| {
        return cast(T, ptr);
    }
    return null;
}

pub fn has_pair(
    world: *world_t,
    subject: entity_t,
    first: entity_t,
    second: entity_t,
) bool {
    return ecs_has_id(world, subject, pair(first, second));
}

pub fn remove_pair(world: *world_t, subject: entity_t, first: entity_t, second: entity_t) void {
    remove_id(world, subject, pair(first, second));
}

// flecs internally reserves names like u16, u32, f32, etc. so we re-map them to uppercase to avoid collisions
pub fn typeName(comptime T: type) @TypeOf(@typeName(T)) {
    return switch (T) {
        u8 => return "U8",
        u16 => return "U16",
        u32 => return "U32",
        u64 => return "U64",
        i8 => return "I8",
        i16 => return "I16",
        i32 => return "I32",
        i64 => return "I64",
        f32 => return "F32",
        f64 => return "F64",
        else => return @typeName(T),
    };
}
//--------------------------------------------------------------------------------------------------
//
// Function wrappers (ecs_* macros in flecs)
//
//--------------------------------------------------------------------------------------------------
pub fn set(world: *world_t, entity: entity_t, comptime T: type, val: T) entity_t {
    return ecs_set_id(world, entity, id(T), @sizeOf(T), @as(*const anyopaque, @ptrCast(&val)));
}

pub fn get(world: *const world_t, entity: entity_t, comptime T: type) ?*const T {
    if (get_id(world, entity, id(T))) |ptr| {
        return cast(T, ptr);
    }
    return null;
}

pub fn get_mut(world: *world_t, entity: entity_t, comptime T: type) ?*T {
    if (get_mut_id(world, entity, id(T))) |ptr| {
        return cast_mut(T, ptr);
    }
    return null;
}

pub fn add(world: *world_t, entity: entity_t, comptime T: type) void {
    ecs_add_id(world, entity, id(T));
}

pub fn remove(world: *world_t, entity: entity_t, comptime T: type) void {
    ecs_remove_id(world, entity, id(T));
}

pub fn override(world: *world_t, entity: entity_t, comptime T: type) void {
    ecs_override_id(world, entity, id(T));
}

pub fn modified(world: *world_t, entity: entity_t, comptime T: type) void {
    ecs_modified_id(world, entity, id(T));
}

pub fn field(it: *iter_t, comptime T: type, index: i8) ?[]T {
    if (ecs_field_w_size(it, @sizeOf(T), index)) |anyptr| {
        const ptr = @as([*]T, @ptrCast(@alignCast(anyptr)));
        return ptr[0..it.count()];
    }
    return null;
}

pub inline fn id(comptime T: type) id_t {
    return perTypeGlobalVarPtr(T).*;
}

pub const pair = make_pair;

pub fn cast(comptime T: type, val: ?*const anyopaque) *const T {
    return @as(*const T, @ptrCast(@alignCast(val)));
}

pub fn cast_mut(comptime T: type, val: ?*anyopaque) *T {
    return @as(*T, @ptrCast(@alignCast(val)));
}

pub fn singleton_set(world: *world_t, comptime T: type, val: T) entity_t {
    return set(world, id(T), T, val);
}

pub fn singleton_get(world: *world_t, comptime T: type) ?*const T {
    return get(world, id(T), T);
}

pub fn singleton_get_mut(world: *world_t, comptime T: type) ?*T {
    return get_mut(world, id(T), T);
}

pub fn singleton_add(world: *world_t, comptime T: type) void {
    add(world, id(T), T);
}

pub fn singleton_remove(world: *world_t, comptime T: type) void {
    remove(world, id(T), T);
}

pub fn singleton_modified(world: *world_t, comptime T: type) void {
    modified(world, id(T), T);
}

// Entity Names

pub fn lookup_path(world: *const world_t, parent: entity_t, path: [*:0]const u8) entity_t {
    return ecs_lookup_path_w_sep(world, parent, path, ".", null, true);
}

pub fn lookup_fullpath(world: *const world_t, path: [*:0]const u8) entity_t {
    return ecs_lookup_path_w_sep(world, 0, path, ".", null, true);
}

pub fn get_path(world: *const world_t, parent: entity_t, child: entity_t) [*:0]u8 {
    return ecs_get_path_w_sep(world, parent, child, ".", null);
}

pub fn get_fullpath(world: *const world_t, child: entity_t) [*:0]u8 {
    return ecs_get_path_w_sep(world, 0, child, ".", null);
}

//--------------------------------------------------------------------------------------------------
fn PerTypeGlobalVar(comptime in_type: type) type {
    if (@alignOf(in_type) > EcsAllocator.Alignment) {
        const message = std.fmt.comptimePrint(
            "Type [{s}] requires an alignment of [{}] but the EcsAllocator only provides an alignment of [{}].",
            .{
                @typeName(in_type),
                @alignOf(in_type),
                EcsAllocator.Alignment,
            },
        );
        @compileError(message);
    }

    return struct {
        var id: id_t = 0;

        // Ensure that a unique struct type is generated for each unique `in_type`. See
        // https://github.com/ziglang/zig/issues/18816
        comptime {
            // We cannot just do `_ = in_type`
            // https://github.com/ziglang/zig/issues/19274
            _ = @alignOf(in_type);
        }
    };
}
inline fn perTypeGlobalVarPtr(comptime T: type) *id_t {
    return comptime &PerTypeGlobalVar(T).id;
}
//--------------------------------------------------------------------------------------------------
//
// OS API
//
//--------------------------------------------------------------------------------------------------

pub const os_init = ecs_os_init;
extern fn ecs_os_init() void;

pub const os_fini = ecs_os_fini;
extern fn ecs_os_fini() void;

pub const os_get_api = ecs_os_get_api;
extern fn ecs_os_get_api() os.api_t;

pub const os_set_api = ecs_os_set_api;
extern fn ecs_os_set_api(api: *os.api_t) void;

pub const time_t = extern struct {
    sec: u32,
    nanosec: u32,
};

pub const os = struct {
    pub const thread_t = usize;
    pub const cond_t = usize;
    pub const mutex_t = usize;
    pub const dl_t = usize;
    pub const sock_t = usize;
    pub const thread_id_t = u64;
    pub const proc_t = *const fn () callconv(.C) void;
    pub const api_init_t = *const fn () callconv(.C) void;
    pub const api_fini_t = *const fn () callconv(.C) void;
    pub const api_malloc_t = *const fn (size_t) callconv(.C) ?*anyopaque;
    pub const api_free_t = *const fn (?*anyopaque) callconv(.C) void;
    pub const api_realloc_t = *const fn (?*anyopaque, size_t) callconv(.C) ?*anyopaque;
    pub const api_calloc_t = *const fn (size_t) callconv(.C) ?*anyopaque;
    pub const api_strdup_t = *const fn ([*:0]const u8) callconv(.C) [*c]u8;
    pub const thread_callback_t = *const fn (?*anyopaque) callconv(.C) ?*anyopaque;
    pub const api_thread_new_t = *const fn (thread_callback_t, ?*anyopaque) callconv(.C) thread_t;
    pub const api_thread_join_t = *const fn (thread_t) callconv(.C) ?*anyopaque;
    pub const api_thread_self_t = *const fn () callconv(.C) thread_id_t;
    pub const api_task_new_t = *const fn (thread_callback_t, ?*anyopaque) callconv(.C) thread_t;
    pub const api_task_join_t = *const fn (thread_t) callconv(.C) ?*anyopaque;
    pub const api_ainc_t = *const fn (*i32) callconv(.C) i32;
    pub const api_lainc_t = *const fn (*i64) callconv(.C) i64;
    pub const api_mutex_new_t = *const fn () callconv(.C) mutex_t;
    pub const api_mutex_lock_t = *const fn (mutex_t) callconv(.C) void;
    pub const api_mutex_unlock_t = *const fn (mutex_t) callconv(.C) void;
    pub const api_mutex_free_t = *const fn (mutex_t) callconv(.C) void;
    pub const api_cond_new_t = *const fn () callconv(.C) cond_t;
    pub const api_cond_free_t = *const fn (cond_t) callconv(.C) void;
    pub const api_cond_signal_t = *const fn (cond_t) callconv(.C) void;
    pub const api_cond_broadcast_t = *const fn (cond_t) callconv(.C) void;
    pub const api_cond_wait_t = *const fn (cond_t, mutex_t) callconv(.C) void;
    pub const api_sleep_t = *const fn (i32, i32) callconv(.C) void;
    pub const api_enable_high_timer_resolution_t = *const fn (bool) callconv(.C) void;
    pub const api_get_time_t = *const fn (*time_t) callconv(.C) void;
    pub const api_now_t = *const fn () callconv(.C) u64;
    pub const api_log_t = *const fn (i32, [*:0]const u8, i32, [*:0]const u8) callconv(.C) void;
    pub const api_abort_t = *const fn () callconv(.C) void;
    pub const api_dlopen_t = *const fn ([*:0]const u8) callconv(.C) dl_t;
    pub const api_dlproc_t = *const fn (dl_t, [*:0]const u8) callconv(.C) proc_t;
    pub const api_dlclose_t = *const fn (dl_t) callconv(.C) void;
    pub const api_module_to_path_t = *const fn ([*:0]const u8) callconv(.C) [*:0]u8;

    const api_t = extern struct {
        init_: api_init_t,
        fini_: api_fini_t,
        malloc_: api_malloc_t,
        realloc_: api_realloc_t,
        calloc_: api_calloc_t,
        free_: api_free_t,
        strdup_: api_strdup_t,
        thread_new_: api_thread_new_t,
        thread_join_: api_thread_join_t,
        thread_self_: api_thread_self_t,
        task_new_: api_task_new_t,
        task_join_: api_task_join_t,
        ainc_: api_ainc_t,
        adec_: api_ainc_t,
        lainc_: api_lainc_t,
        ladec_: api_lainc_t,
        mutex_new_: api_mutex_new_t,
        mutex_free_: api_mutex_free_t,
        mutex_lock_: api_mutex_lock_t,
        mutex_unlock_: api_mutex_lock_t,
        cond_new_: api_cond_new_t,
        cond_free_: api_cond_free_t,
        cond_signal_: api_cond_signal_t,
        cond_broadcast_: api_cond_broadcast_t,
        cond_wait_: api_cond_wait_t,
        sleep_: api_sleep_t,
        now_: api_now_t,
        get_time_: api_get_time_t,
        log_: api_log_t,
        abort_: api_abort_t,
        dlopen_: api_dlopen_t,
        dlproc_: api_dlproc_t,
        dlclose_: api_dlclose_t,
        module_to_dl_: api_module_to_path_t,
        module_to_etc_: api_module_to_path_t,
        log_level_: i32,
        log_indent_: i32,
        log_last_error_: i32,
        log_last_timestamp_: i64,
        flags_: flags32_t,
        log_out_: *anyopaque, // *FILE
    };

    extern var ecs_os_api: api_t;

    pub fn free(ptr: ?*anyopaque) void {
        ecs_os_api.free_(ptr);
    }
};

//--------------------------------------------------------------------------------------------------
//
// ADDONS
//
//--------------------------------------------------------------------------------------------------

// ecs_new_w_pair
pub fn new_w_pair(world: *world_t, first: entity_t, second: entity_t) entity_t {
    const pair_id = make_pair(first, second);
    return new_w_id(world, pair_id);
}

// ecs_delete_children
pub fn delete_children(world: *world_t, parent: entity_t) void {
    delete_with(world, make_pair(ChildOf, parent));
}

//--------------------------------------------------------------------------------------------------
//
// FLECS_MODULE
//
//--------------------------------------------------------------------------------------------------

/// `pub fn import_c(world: *world_t, comptime module: type) entity_t`
pub const import_c = ecs_import_c;
extern fn ecs_import_c(world: *world_t, module: module_action_t, module_name_c: [*:0]const u8) entity_t;

//--------------------------------------------------------------------------------------------------
//
// FLECS_MONITOR
//
//--------------------------------------------------------------------------------------------------

pub extern fn FlecsMonitorImport(world: *world_t) void;
//--------------------------------------------------------------------------------------------------
//
// FLECS_REST
//
//--------------------------------------------------------------------------------------------------

pub extern fn FlecsRestImport(world: *world_t) void;

pub const EcsRest = extern struct {
    port: u16 = 0,
    ipaddr: ?[*:0]u8 = null,
    impl: ?*anyopaque = null,
};
