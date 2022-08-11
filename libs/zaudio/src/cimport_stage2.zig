pub const __builtin_bswap16 = @import("std").zig.c_builtins.__builtin_bswap16;
pub const __builtin_bswap32 = @import("std").zig.c_builtins.__builtin_bswap32;
pub const __builtin_bswap64 = @import("std").zig.c_builtins.__builtin_bswap64;
pub const __builtin_signbit = @import("std").zig.c_builtins.__builtin_signbit;
pub const __builtin_signbitf = @import("std").zig.c_builtins.__builtin_signbitf;
pub const __builtin_popcount = @import("std").zig.c_builtins.__builtin_popcount;
pub const __builtin_ctz = @import("std").zig.c_builtins.__builtin_ctz;
pub const __builtin_clz = @import("std").zig.c_builtins.__builtin_clz;
pub const __builtin_sqrt = @import("std").zig.c_builtins.__builtin_sqrt;
pub const __builtin_sqrtf = @import("std").zig.c_builtins.__builtin_sqrtf;
pub const __builtin_sin = @import("std").zig.c_builtins.__builtin_sin;
pub const __builtin_sinf = @import("std").zig.c_builtins.__builtin_sinf;
pub const __builtin_cos = @import("std").zig.c_builtins.__builtin_cos;
pub const __builtin_cosf = @import("std").zig.c_builtins.__builtin_cosf;
pub const __builtin_exp = @import("std").zig.c_builtins.__builtin_exp;
pub const __builtin_expf = @import("std").zig.c_builtins.__builtin_expf;
pub const __builtin_exp2 = @import("std").zig.c_builtins.__builtin_exp2;
pub const __builtin_exp2f = @import("std").zig.c_builtins.__builtin_exp2f;
pub const __builtin_log = @import("std").zig.c_builtins.__builtin_log;
pub const __builtin_logf = @import("std").zig.c_builtins.__builtin_logf;
pub const __builtin_log2 = @import("std").zig.c_builtins.__builtin_log2;
pub const __builtin_log2f = @import("std").zig.c_builtins.__builtin_log2f;
pub const __builtin_log10 = @import("std").zig.c_builtins.__builtin_log10;
pub const __builtin_log10f = @import("std").zig.c_builtins.__builtin_log10f;
pub const __builtin_abs = @import("std").zig.c_builtins.__builtin_abs;
pub const __builtin_fabs = @import("std").zig.c_builtins.__builtin_fabs;
pub const __builtin_fabsf = @import("std").zig.c_builtins.__builtin_fabsf;
pub const __builtin_floor = @import("std").zig.c_builtins.__builtin_floor;
pub const __builtin_floorf = @import("std").zig.c_builtins.__builtin_floorf;
pub const __builtin_ceil = @import("std").zig.c_builtins.__builtin_ceil;
pub const __builtin_ceilf = @import("std").zig.c_builtins.__builtin_ceilf;
pub const __builtin_trunc = @import("std").zig.c_builtins.__builtin_trunc;
pub const __builtin_truncf = @import("std").zig.c_builtins.__builtin_truncf;
pub const __builtin_round = @import("std").zig.c_builtins.__builtin_round;
pub const __builtin_roundf = @import("std").zig.c_builtins.__builtin_roundf;
pub const __builtin_strlen = @import("std").zig.c_builtins.__builtin_strlen;
pub const __builtin_strcmp = @import("std").zig.c_builtins.__builtin_strcmp;
pub const __builtin_object_size = @import("std").zig.c_builtins.__builtin_object_size;
pub const __builtin___memset_chk = @import("std").zig.c_builtins.__builtin___memset_chk;
pub const __builtin_memset = @import("std").zig.c_builtins.__builtin_memset;
pub const __builtin___memcpy_chk = @import("std").zig.c_builtins.__builtin___memcpy_chk;
pub const __builtin_memcpy = @import("std").zig.c_builtins.__builtin_memcpy;
pub const __builtin_expect = @import("std").zig.c_builtins.__builtin_expect;
pub const __builtin_nanf = @import("std").zig.c_builtins.__builtin_nanf;
pub const __builtin_huge_valf = @import("std").zig.c_builtins.__builtin_huge_valf;
pub const __builtin_inff = @import("std").zig.c_builtins.__builtin_inff;
pub const __builtin_isnan = @import("std").zig.c_builtins.__builtin_isnan;
pub const __builtin_isinf = @import("std").zig.c_builtins.__builtin_isinf;
pub const __builtin_isinf_sign = @import("std").zig.c_builtins.__builtin_isinf_sign;
pub const __has_builtin = @import("std").zig.c_builtins.__has_builtin;
pub const __builtin_assume = @import("std").zig.c_builtins.__builtin_assume;
pub const __builtin_unreachable = @import("std").zig.c_builtins.__builtin_unreachable;
pub const __builtin_constant_p = @import("std").zig.c_builtins.__builtin_constant_p;
pub const __builtin_mul_overflow = @import("std").zig.c_builtins.__builtin_mul_overflow;
pub const ptrdiff_t = c_long;
pub const wchar_t = c_int;
pub const max_align_t = extern struct {
    __clang_max_align_nonce1: c_longlong align(8),
    __clang_max_align_nonce2: c_longdouble align(16),
};
pub const ma_int8 = i8;
pub const ma_uint8 = u8;
pub const ma_int16 = c_short;
pub const ma_uint16 = c_ushort;
pub const ma_int32 = c_int;
pub const ma_uint32 = c_uint;
pub const ma_int64 = c_longlong;
pub const ma_uint64 = c_ulonglong;
pub const ma_uintptr = ma_uint64;
pub const ma_bool8 = ma_uint8;
pub const ma_bool32 = ma_uint32;
pub const ma_handle = ?*anyopaque;
pub const ma_ptr = ?*anyopaque;
pub const ma_proc = ?*const fn () callconv(.C) void;
pub const __u_char = u8;
pub const __u_short = c_ushort;
pub const __u_int = c_uint;
pub const __u_long = c_ulong;
pub const __int8_t = i8;
pub const __uint8_t = u8;
pub const __int16_t = c_short;
pub const __uint16_t = c_ushort;
pub const __int32_t = c_int;
pub const __uint32_t = c_uint;
pub const __int64_t = c_long;
pub const __uint64_t = c_ulong;
pub const __int_least8_t = __int8_t;
pub const __uint_least8_t = __uint8_t;
pub const __int_least16_t = __int16_t;
pub const __uint_least16_t = __uint16_t;
pub const __int_least32_t = __int32_t;
pub const __uint_least32_t = __uint32_t;
pub const __int_least64_t = __int64_t;
pub const __uint_least64_t = __uint64_t;
pub const __quad_t = c_long;
pub const __u_quad_t = c_ulong;
pub const __intmax_t = c_long;
pub const __uintmax_t = c_ulong;
pub const __dev_t = c_ulong;
pub const __uid_t = c_uint;
pub const __gid_t = c_uint;
pub const __ino_t = c_ulong;
pub const __ino64_t = c_ulong;
pub const __mode_t = c_uint;
pub const __nlink_t = c_ulong;
pub const __off_t = c_long;
pub const __off64_t = c_long;
pub const __pid_t = c_int;
pub const __fsid_t = extern struct {
    __val: [2]c_int,
};
pub const __clock_t = c_long;
pub const __rlim_t = c_ulong;
pub const __rlim64_t = c_ulong;
pub const __id_t = c_uint;
pub const __time_t = c_long;
pub const __useconds_t = c_uint;
pub const __suseconds_t = c_long;
pub const __suseconds64_t = c_long;
pub const __daddr_t = c_int;
pub const __key_t = c_int;
pub const __clockid_t = c_int;
pub const __timer_t = ?*anyopaque;
pub const __blksize_t = c_long;
pub const __blkcnt_t = c_long;
pub const __blkcnt64_t = c_long;
pub const __fsblkcnt_t = c_ulong;
pub const __fsblkcnt64_t = c_ulong;
pub const __fsfilcnt_t = c_ulong;
pub const __fsfilcnt64_t = c_ulong;
pub const __fsword_t = c_long;
pub const __ssize_t = c_long;
pub const __syscall_slong_t = c_long;
pub const __syscall_ulong_t = c_ulong;
pub const __loff_t = __off64_t;
pub const __caddr_t = [*c]u8;
pub const __intptr_t = c_long;
pub const __socklen_t = c_uint;
pub const __sig_atomic_t = c_int;
pub const time_t = __time_t;
pub const struct_timespec = extern struct {
    tv_sec: __time_t,
    tv_nsec: __syscall_slong_t,
};
pub const pid_t = __pid_t;
pub const struct_sched_param = extern struct {
    sched_priority: c_int,
};
pub const __cpu_mask = c_ulong;
pub const cpu_set_t = extern struct {
    __bits: [16]__cpu_mask,
};
pub extern fn __sched_cpucount(__setsize: usize, __setp: [*c]const cpu_set_t) c_int;
pub extern fn __sched_cpualloc(__count: usize) [*c]cpu_set_t;
pub extern fn __sched_cpufree(__set: [*c]cpu_set_t) void;
pub extern fn sched_setparam(__pid: __pid_t, __param: [*c]const struct_sched_param) c_int;
pub extern fn sched_getparam(__pid: __pid_t, __param: [*c]struct_sched_param) c_int;
pub extern fn sched_setscheduler(__pid: __pid_t, __policy: c_int, __param: [*c]const struct_sched_param) c_int;
pub extern fn sched_getscheduler(__pid: __pid_t) c_int;
pub extern fn sched_yield() c_int;
pub extern fn sched_get_priority_max(__algorithm: c_int) c_int;
pub extern fn sched_get_priority_min(__algorithm: c_int) c_int;
pub extern fn sched_rr_get_interval(__pid: __pid_t, __t: [*c]struct_timespec) c_int;
pub const clock_t = __clock_t;
pub const struct_tm = extern struct {
    tm_sec: c_int,
    tm_min: c_int,
    tm_hour: c_int,
    tm_mday: c_int,
    tm_mon: c_int,
    tm_year: c_int,
    tm_wday: c_int,
    tm_yday: c_int,
    tm_isdst: c_int,
    tm_gmtoff: c_long,
    tm_zone: [*c]const u8,
};
pub const clockid_t = __clockid_t;
pub const timer_t = __timer_t;
pub const struct_itimerspec = extern struct {
    it_interval: struct_timespec,
    it_value: struct_timespec,
};
pub const struct_sigevent = opaque {};
pub const struct___locale_data = opaque {};
pub const struct___locale_struct = extern struct {
    __locales: [13]?*struct___locale_data,
    __ctype_b: [*c]const c_ushort,
    __ctype_tolower: [*c]const c_int,
    __ctype_toupper: [*c]const c_int,
    __names: [13][*c]const u8,
};
pub const __locale_t = [*c]struct___locale_struct;
pub const locale_t = __locale_t;
pub extern fn clock() clock_t;
pub extern fn time(__timer: [*c]time_t) time_t;
pub extern fn difftime(__time1: time_t, __time0: time_t) f64;
pub extern fn mktime(__tp: [*c]struct_tm) time_t;
pub extern fn strftime(noalias __s: [*c]u8, __maxsize: usize, noalias __format: [*c]const u8, noalias __tp: [*c]const struct_tm) usize;
pub extern fn strftime_l(noalias __s: [*c]u8, __maxsize: usize, noalias __format: [*c]const u8, noalias __tp: [*c]const struct_tm, __loc: locale_t) usize;
pub extern fn gmtime(__timer: [*c]const time_t) [*c]struct_tm;
pub extern fn localtime(__timer: [*c]const time_t) [*c]struct_tm;
pub extern fn gmtime_r(noalias __timer: [*c]const time_t, noalias __tp: [*c]struct_tm) [*c]struct_tm;
pub extern fn localtime_r(noalias __timer: [*c]const time_t, noalias __tp: [*c]struct_tm) [*c]struct_tm;
pub extern fn asctime(__tp: [*c]const struct_tm) [*c]u8;
pub extern fn ctime(__timer: [*c]const time_t) [*c]u8;
pub extern fn asctime_r(noalias __tp: [*c]const struct_tm, noalias __buf: [*c]u8) [*c]u8;
pub extern fn ctime_r(noalias __timer: [*c]const time_t, noalias __buf: [*c]u8) [*c]u8;
pub extern var __tzname: [2][*c]u8;
pub extern var __daylight: c_int;
pub extern var __timezone: c_long;
pub extern var tzname: [2][*c]u8;
pub extern fn tzset() void;
pub extern var daylight: c_int;
pub extern var timezone: c_long;
pub extern fn timegm(__tp: [*c]struct_tm) time_t;
pub extern fn timelocal(__tp: [*c]struct_tm) time_t;
pub extern fn dysize(__year: c_int) c_int;
pub extern fn nanosleep(__requested_time: [*c]const struct_timespec, __remaining: [*c]struct_timespec) c_int;
pub extern fn clock_getres(__clock_id: clockid_t, __res: [*c]struct_timespec) c_int;
pub extern fn clock_gettime(__clock_id: clockid_t, __tp: [*c]struct_timespec) c_int;
pub extern fn clock_settime(__clock_id: clockid_t, __tp: [*c]const struct_timespec) c_int;
pub extern fn clock_nanosleep(__clock_id: clockid_t, __flags: c_int, __req: [*c]const struct_timespec, __rem: [*c]struct_timespec) c_int;
pub extern fn clock_getcpuclockid(__pid: pid_t, __clock_id: [*c]clockid_t) c_int;
pub extern fn timer_create(__clock_id: clockid_t, noalias __evp: ?*struct_sigevent, noalias __timerid: [*c]timer_t) c_int;
pub extern fn timer_delete(__timerid: timer_t) c_int;
pub extern fn timer_settime(__timerid: timer_t, __flags: c_int, noalias __value: [*c]const struct_itimerspec, noalias __ovalue: [*c]struct_itimerspec) c_int;
pub extern fn timer_gettime(__timerid: timer_t, __value: [*c]struct_itimerspec) c_int;
pub extern fn timer_getoverrun(__timerid: timer_t) c_int;
pub extern fn timespec_get(__ts: [*c]struct_timespec, __base: c_int) c_int;
pub const struct___pthread_internal_list = extern struct {
    __prev: [*c]struct___pthread_internal_list,
    __next: [*c]struct___pthread_internal_list,
};
pub const __pthread_list_t = struct___pthread_internal_list;
pub const struct___pthread_internal_slist = extern struct {
    __next: [*c]struct___pthread_internal_slist,
};
pub const __pthread_slist_t = struct___pthread_internal_slist;
pub const struct___pthread_mutex_s = extern struct {
    __lock: c_int,
    __count: c_uint,
    __owner: c_int,
    __nusers: c_uint,
    __kind: c_int,
    __spins: c_short,
    __elision: c_short,
    __list: __pthread_list_t,
};
pub const struct___pthread_rwlock_arch_t = extern struct {
    __readers: c_uint,
    __writers: c_uint,
    __wrphase_futex: c_uint,
    __writers_futex: c_uint,
    __pad3: c_uint,
    __pad4: c_uint,
    __cur_writer: c_int,
    __shared: c_int,
    __rwelision: i8,
    __pad1: [7]u8,
    __pad2: c_ulong,
    __flags: c_uint,
};
const struct_unnamed_2 = extern struct {
    __low: c_uint,
    __high: c_uint,
};
const union_unnamed_1 = extern union {
    __wseq: c_ulonglong,
    __wseq32: struct_unnamed_2,
};
const struct_unnamed_4 = extern struct {
    __low: c_uint,
    __high: c_uint,
};
const union_unnamed_3 = extern union {
    __g1_start: c_ulonglong,
    __g1_start32: struct_unnamed_4,
};
pub const struct___pthread_cond_s = extern struct {
    unnamed_0: union_unnamed_1,
    unnamed_1: union_unnamed_3,
    __g_refs: [2]c_uint,
    __g_size: [2]c_uint,
    __g1_orig_size: c_uint,
    __wrefs: c_uint,
    __g_signals: [2]c_uint,
};
pub const __tss_t = c_uint;
pub const __thrd_t = c_ulong;
pub const __once_flag = extern struct {
    __data: c_int,
};
pub const pthread_t = c_ulong;
pub const pthread_mutexattr_t = extern union {
    __size: [4]u8,
    __align: c_int,
};
pub const pthread_condattr_t = extern union {
    __size: [4]u8,
    __align: c_int,
};
pub const pthread_key_t = c_uint;
pub const pthread_once_t = c_int;
pub const union_pthread_attr_t = extern union {
    __size: [56]u8,
    __align: c_long,
};
pub const pthread_attr_t = union_pthread_attr_t;
pub const pthread_mutex_t = extern union {
    __data: struct___pthread_mutex_s,
    __size: [40]u8,
    __align: c_long,
};
pub const pthread_cond_t = extern union {
    __data: struct___pthread_cond_s,
    __size: [48]u8,
    __align: c_longlong,
};
pub const pthread_rwlock_t = extern union {
    __data: struct___pthread_rwlock_arch_t,
    __size: [56]u8,
    __align: c_long,
};
pub const pthread_rwlockattr_t = extern union {
    __size: [8]u8,
    __align: c_long,
};
pub const pthread_spinlock_t = c_int;
pub const pthread_barrier_t = extern union {
    __size: [32]u8,
    __align: c_long,
};
pub const pthread_barrierattr_t = extern union {
    __size: [4]u8,
    __align: c_int,
};
pub const __jmp_buf = [8]c_long;
pub const __sigset_t = extern struct {
    __val: [16]c_ulong,
};
pub const struct___jmp_buf_tag = extern struct {
    __jmpbuf: __jmp_buf,
    __mask_was_saved: c_int,
    __saved_mask: __sigset_t,
};
pub const PTHREAD_CREATE_JOINABLE: c_int = 0;
pub const PTHREAD_CREATE_DETACHED: c_int = 1;
const enum_unnamed_5 = c_uint;
pub const PTHREAD_MUTEX_TIMED_NP: c_int = 0;
pub const PTHREAD_MUTEX_RECURSIVE_NP: c_int = 1;
pub const PTHREAD_MUTEX_ERRORCHECK_NP: c_int = 2;
pub const PTHREAD_MUTEX_ADAPTIVE_NP: c_int = 3;
pub const PTHREAD_MUTEX_NORMAL: c_int = 0;
pub const PTHREAD_MUTEX_RECURSIVE: c_int = 1;
pub const PTHREAD_MUTEX_ERRORCHECK: c_int = 2;
pub const PTHREAD_MUTEX_DEFAULT: c_int = 0;
const enum_unnamed_6 = c_uint;
pub const PTHREAD_MUTEX_STALLED: c_int = 0;
pub const PTHREAD_MUTEX_STALLED_NP: c_int = 0;
pub const PTHREAD_MUTEX_ROBUST: c_int = 1;
pub const PTHREAD_MUTEX_ROBUST_NP: c_int = 1;
const enum_unnamed_7 = c_uint;
pub const PTHREAD_PRIO_NONE: c_int = 0;
pub const PTHREAD_PRIO_INHERIT: c_int = 1;
pub const PTHREAD_PRIO_PROTECT: c_int = 2;
const enum_unnamed_8 = c_uint;
pub const PTHREAD_RWLOCK_PREFER_READER_NP: c_int = 0;
pub const PTHREAD_RWLOCK_PREFER_WRITER_NP: c_int = 1;
pub const PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP: c_int = 2;
pub const PTHREAD_RWLOCK_DEFAULT_NP: c_int = 0;
const enum_unnamed_9 = c_uint;
pub const PTHREAD_INHERIT_SCHED: c_int = 0;
pub const PTHREAD_EXPLICIT_SCHED: c_int = 1;
const enum_unnamed_10 = c_uint;
pub const PTHREAD_SCOPE_SYSTEM: c_int = 0;
pub const PTHREAD_SCOPE_PROCESS: c_int = 1;
const enum_unnamed_11 = c_uint;
pub const PTHREAD_PROCESS_PRIVATE: c_int = 0;
pub const PTHREAD_PROCESS_SHARED: c_int = 1;
const enum_unnamed_12 = c_uint;
pub const struct__pthread_cleanup_buffer = extern struct {
    __routine: ?*const fn (?*anyopaque) callconv(.C) void,
    __arg: ?*anyopaque,
    __canceltype: c_int,
    __prev: [*c]struct__pthread_cleanup_buffer,
};
pub const PTHREAD_CANCEL_ENABLE: c_int = 0;
pub const PTHREAD_CANCEL_DISABLE: c_int = 1;
const enum_unnamed_13 = c_uint;
pub const PTHREAD_CANCEL_DEFERRED: c_int = 0;
pub const PTHREAD_CANCEL_ASYNCHRONOUS: c_int = 1;
const enum_unnamed_14 = c_uint;
pub extern fn pthread_create(noalias __newthread: [*c]pthread_t, noalias __attr: [*c]const pthread_attr_t, __start_routine: ?*const fn (?*anyopaque) callconv(.C) ?*anyopaque, noalias __arg: ?*anyopaque) c_int;
pub extern fn pthread_exit(__retval: ?*anyopaque) noreturn;
pub extern fn pthread_join(__th: pthread_t, __thread_return: [*c]?*anyopaque) c_int;
pub extern fn pthread_detach(__th: pthread_t) c_int;
pub extern fn pthread_self() pthread_t;
pub extern fn pthread_equal(__thread1: pthread_t, __thread2: pthread_t) c_int;
pub extern fn pthread_attr_init(__attr: [*c]pthread_attr_t) c_int;
pub extern fn pthread_attr_destroy(__attr: [*c]pthread_attr_t) c_int;
pub extern fn pthread_attr_getdetachstate(__attr: [*c]const pthread_attr_t, __detachstate: [*c]c_int) c_int;
pub extern fn pthread_attr_setdetachstate(__attr: [*c]pthread_attr_t, __detachstate: c_int) c_int;
pub extern fn pthread_attr_getguardsize(__attr: [*c]const pthread_attr_t, __guardsize: [*c]usize) c_int;
pub extern fn pthread_attr_setguardsize(__attr: [*c]pthread_attr_t, __guardsize: usize) c_int;
pub extern fn pthread_attr_getschedparam(noalias __attr: [*c]const pthread_attr_t, noalias __param: [*c]struct_sched_param) c_int;
pub extern fn pthread_attr_setschedparam(noalias __attr: [*c]pthread_attr_t, noalias __param: [*c]const struct_sched_param) c_int;
pub extern fn pthread_attr_getschedpolicy(noalias __attr: [*c]const pthread_attr_t, noalias __policy: [*c]c_int) c_int;
pub extern fn pthread_attr_setschedpolicy(__attr: [*c]pthread_attr_t, __policy: c_int) c_int;
pub extern fn pthread_attr_getinheritsched(noalias __attr: [*c]const pthread_attr_t, noalias __inherit: [*c]c_int) c_int;
pub extern fn pthread_attr_setinheritsched(__attr: [*c]pthread_attr_t, __inherit: c_int) c_int;
pub extern fn pthread_attr_getscope(noalias __attr: [*c]const pthread_attr_t, noalias __scope: [*c]c_int) c_int;
pub extern fn pthread_attr_setscope(__attr: [*c]pthread_attr_t, __scope: c_int) c_int;
pub extern fn pthread_attr_getstackaddr(noalias __attr: [*c]const pthread_attr_t, noalias __stackaddr: [*c]?*anyopaque) c_int;
pub extern fn pthread_attr_setstackaddr(__attr: [*c]pthread_attr_t, __stackaddr: ?*anyopaque) c_int;
pub extern fn pthread_attr_getstacksize(noalias __attr: [*c]const pthread_attr_t, noalias __stacksize: [*c]usize) c_int;
pub extern fn pthread_attr_setstacksize(__attr: [*c]pthread_attr_t, __stacksize: usize) c_int;
pub extern fn pthread_attr_getstack(noalias __attr: [*c]const pthread_attr_t, noalias __stackaddr: [*c]?*anyopaque, noalias __stacksize: [*c]usize) c_int;
pub extern fn pthread_attr_setstack(__attr: [*c]pthread_attr_t, __stackaddr: ?*anyopaque, __stacksize: usize) c_int;
pub extern fn pthread_setschedparam(__target_thread: pthread_t, __policy: c_int, __param: [*c]const struct_sched_param) c_int;
pub extern fn pthread_getschedparam(__target_thread: pthread_t, noalias __policy: [*c]c_int, noalias __param: [*c]struct_sched_param) c_int;
pub extern fn pthread_setschedprio(__target_thread: pthread_t, __prio: c_int) c_int;
pub extern fn pthread_once(__once_control: [*c]pthread_once_t, __init_routine: ?*const fn () callconv(.C) void) c_int;
pub extern fn pthread_setcancelstate(__state: c_int, __oldstate: [*c]c_int) c_int;
pub extern fn pthread_setcanceltype(__type: c_int, __oldtype: [*c]c_int) c_int;
pub extern fn pthread_cancel(__th: pthread_t) c_int;
pub extern fn pthread_testcancel() void;
pub const struct___cancel_jmp_buf_tag = extern struct {
    __cancel_jmp_buf: __jmp_buf,
    __mask_was_saved: c_int,
};
pub const __pthread_unwind_buf_t = extern struct {
    __cancel_jmp_buf: [1]struct___cancel_jmp_buf_tag,
    __pad: [4]?*anyopaque,
};
pub const struct___pthread_cleanup_frame = extern struct {
    __cancel_routine: ?*const fn (?*anyopaque) callconv(.C) void,
    __cancel_arg: ?*anyopaque,
    __do_it: c_int,
    __cancel_type: c_int,
};
pub extern fn __pthread_register_cancel(__buf: [*c]__pthread_unwind_buf_t) void;
pub extern fn __pthread_unregister_cancel(__buf: [*c]__pthread_unwind_buf_t) void;
pub extern fn __pthread_unwind_next(__buf: [*c]__pthread_unwind_buf_t) noreturn;
pub extern fn __sigsetjmp(__env: [*c]struct___jmp_buf_tag, __savemask: c_int) c_int;
pub extern fn pthread_mutex_init(__mutex: [*c]pthread_mutex_t, __mutexattr: [*c]const pthread_mutexattr_t) c_int;
pub extern fn pthread_mutex_destroy(__mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_mutex_trylock(__mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_mutex_lock(__mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_mutex_timedlock(noalias __mutex: [*c]pthread_mutex_t, noalias __abstime: [*c]const struct_timespec) c_int;
pub extern fn pthread_mutex_unlock(__mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_mutex_getprioceiling(noalias __mutex: [*c]const pthread_mutex_t, noalias __prioceiling: [*c]c_int) c_int;
pub extern fn pthread_mutex_setprioceiling(noalias __mutex: [*c]pthread_mutex_t, __prioceiling: c_int, noalias __old_ceiling: [*c]c_int) c_int;
pub extern fn pthread_mutex_consistent(__mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_mutexattr_init(__attr: [*c]pthread_mutexattr_t) c_int;
pub extern fn pthread_mutexattr_destroy(__attr: [*c]pthread_mutexattr_t) c_int;
pub extern fn pthread_mutexattr_getpshared(noalias __attr: [*c]const pthread_mutexattr_t, noalias __pshared: [*c]c_int) c_int;
pub extern fn pthread_mutexattr_setpshared(__attr: [*c]pthread_mutexattr_t, __pshared: c_int) c_int;
pub extern fn pthread_mutexattr_gettype(noalias __attr: [*c]const pthread_mutexattr_t, noalias __kind: [*c]c_int) c_int;
pub extern fn pthread_mutexattr_settype(__attr: [*c]pthread_mutexattr_t, __kind: c_int) c_int;
pub extern fn pthread_mutexattr_getprotocol(noalias __attr: [*c]const pthread_mutexattr_t, noalias __protocol: [*c]c_int) c_int;
pub extern fn pthread_mutexattr_setprotocol(__attr: [*c]pthread_mutexattr_t, __protocol: c_int) c_int;
pub extern fn pthread_mutexattr_getprioceiling(noalias __attr: [*c]const pthread_mutexattr_t, noalias __prioceiling: [*c]c_int) c_int;
pub extern fn pthread_mutexattr_setprioceiling(__attr: [*c]pthread_mutexattr_t, __prioceiling: c_int) c_int;
pub extern fn pthread_mutexattr_getrobust(__attr: [*c]const pthread_mutexattr_t, __robustness: [*c]c_int) c_int;
pub extern fn pthread_mutexattr_setrobust(__attr: [*c]pthread_mutexattr_t, __robustness: c_int) c_int;
pub extern fn pthread_rwlock_init(noalias __rwlock: [*c]pthread_rwlock_t, noalias __attr: [*c]const pthread_rwlockattr_t) c_int;
pub extern fn pthread_rwlock_destroy(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlock_rdlock(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlock_tryrdlock(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlock_timedrdlock(noalias __rwlock: [*c]pthread_rwlock_t, noalias __abstime: [*c]const struct_timespec) c_int;
pub extern fn pthread_rwlock_wrlock(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlock_trywrlock(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlock_timedwrlock(noalias __rwlock: [*c]pthread_rwlock_t, noalias __abstime: [*c]const struct_timespec) c_int;
pub extern fn pthread_rwlock_unlock(__rwlock: [*c]pthread_rwlock_t) c_int;
pub extern fn pthread_rwlockattr_init(__attr: [*c]pthread_rwlockattr_t) c_int;
pub extern fn pthread_rwlockattr_destroy(__attr: [*c]pthread_rwlockattr_t) c_int;
pub extern fn pthread_rwlockattr_getpshared(noalias __attr: [*c]const pthread_rwlockattr_t, noalias __pshared: [*c]c_int) c_int;
pub extern fn pthread_rwlockattr_setpshared(__attr: [*c]pthread_rwlockattr_t, __pshared: c_int) c_int;
pub extern fn pthread_rwlockattr_getkind_np(noalias __attr: [*c]const pthread_rwlockattr_t, noalias __pref: [*c]c_int) c_int;
pub extern fn pthread_rwlockattr_setkind_np(__attr: [*c]pthread_rwlockattr_t, __pref: c_int) c_int;
pub extern fn pthread_cond_init(noalias __cond: [*c]pthread_cond_t, noalias __cond_attr: [*c]const pthread_condattr_t) c_int;
pub extern fn pthread_cond_destroy(__cond: [*c]pthread_cond_t) c_int;
pub extern fn pthread_cond_signal(__cond: [*c]pthread_cond_t) c_int;
pub extern fn pthread_cond_broadcast(__cond: [*c]pthread_cond_t) c_int;
pub extern fn pthread_cond_wait(noalias __cond: [*c]pthread_cond_t, noalias __mutex: [*c]pthread_mutex_t) c_int;
pub extern fn pthread_cond_timedwait(noalias __cond: [*c]pthread_cond_t, noalias __mutex: [*c]pthread_mutex_t, noalias __abstime: [*c]const struct_timespec) c_int;
pub extern fn pthread_condattr_init(__attr: [*c]pthread_condattr_t) c_int;
pub extern fn pthread_condattr_destroy(__attr: [*c]pthread_condattr_t) c_int;
pub extern fn pthread_condattr_getpshared(noalias __attr: [*c]const pthread_condattr_t, noalias __pshared: [*c]c_int) c_int;
pub extern fn pthread_condattr_setpshared(__attr: [*c]pthread_condattr_t, __pshared: c_int) c_int;
pub extern fn pthread_condattr_getclock(noalias __attr: [*c]const pthread_condattr_t, noalias __clock_id: [*c]__clockid_t) c_int;
pub extern fn pthread_condattr_setclock(__attr: [*c]pthread_condattr_t, __clock_id: __clockid_t) c_int;
pub extern fn pthread_spin_init(__lock: [*c]volatile pthread_spinlock_t, __pshared: c_int) c_int;
pub extern fn pthread_spin_destroy(__lock: [*c]volatile pthread_spinlock_t) c_int;
pub extern fn pthread_spin_lock(__lock: [*c]volatile pthread_spinlock_t) c_int;
pub extern fn pthread_spin_trylock(__lock: [*c]volatile pthread_spinlock_t) c_int;
pub extern fn pthread_spin_unlock(__lock: [*c]volatile pthread_spinlock_t) c_int;
pub extern fn pthread_barrier_init(noalias __barrier: [*c]pthread_barrier_t, noalias __attr: [*c]const pthread_barrierattr_t, __count: c_uint) c_int;
pub extern fn pthread_barrier_destroy(__barrier: [*c]pthread_barrier_t) c_int;
pub extern fn pthread_barrier_wait(__barrier: [*c]pthread_barrier_t) c_int;
pub extern fn pthread_barrierattr_init(__attr: [*c]pthread_barrierattr_t) c_int;
pub extern fn pthread_barrierattr_destroy(__attr: [*c]pthread_barrierattr_t) c_int;
pub extern fn pthread_barrierattr_getpshared(noalias __attr: [*c]const pthread_barrierattr_t, noalias __pshared: [*c]c_int) c_int;
pub extern fn pthread_barrierattr_setpshared(__attr: [*c]pthread_barrierattr_t, __pshared: c_int) c_int;
pub extern fn pthread_key_create(__key: [*c]pthread_key_t, __destr_function: ?*const fn (?*anyopaque) callconv(.C) void) c_int;
pub extern fn pthread_key_delete(__key: pthread_key_t) c_int;
pub extern fn pthread_getspecific(__key: pthread_key_t) ?*anyopaque;
pub extern fn pthread_setspecific(__key: pthread_key_t, __pointer: ?*const anyopaque) c_int;
pub extern fn pthread_getcpuclockid(__thread_id: pthread_t, __clock_id: [*c]__clockid_t) c_int;
pub extern fn pthread_atfork(__prepare: ?*const fn () callconv(.C) void, __parent: ?*const fn () callconv(.C) void, __child: ?*const fn () callconv(.C) void) c_int;
pub const ma_pthread_t = pthread_t;
pub const ma_pthread_mutex_t = pthread_mutex_t;
pub const ma_pthread_cond_t = pthread_cond_t;
pub const MA_LOG_LEVEL_DEBUG: c_int = 4;
pub const MA_LOG_LEVEL_INFO: c_int = 3;
pub const MA_LOG_LEVEL_WARNING: c_int = 2;
pub const MA_LOG_LEVEL_ERROR: c_int = 1;
pub const ma_log_level = c_uint;
pub const ma_context = struct_ma_context;
const struct_unnamed_15 = extern struct {
    useVerboseDeviceEnumeration: ma_bool32,
};
const struct_unnamed_16 = extern struct {
    pApplicationName: [*c]const u8,
    pServerName: [*c]const u8,
    tryAutoSpawn: ma_bool32,
};
const struct_unnamed_17 = extern struct {
    sessionCategory: ma_ios_session_category,
    sessionCategoryOptions: ma_uint32,
    noAudioSessionActivate: ma_bool32,
    noAudioSessionDeactivate: ma_bool32,
};
const struct_unnamed_18 = extern struct {
    pClientName: [*c]const u8,
    tryStartServer: ma_bool32,
};
pub const struct_ma_context_config = extern struct {
    pLog: [*c]ma_log,
    threadPriority: ma_thread_priority,
    threadStackSize: usize,
    pUserData: ?*anyopaque,
    allocationCallbacks: ma_allocation_callbacks,
    alsa: struct_unnamed_15,
    pulse: struct_unnamed_16,
    coreaudio: struct_unnamed_17,
    jack: struct_unnamed_18,
    custom: ma_backend_callbacks,
};
pub const ma_context_config = struct_ma_context_config;
pub const ma_enum_devices_callback_proc = ?*const fn ([*c]ma_context, ma_device_type, [*c]const ma_device_info, ?*anyopaque) callconv(.C) ma_bool32;
pub const ma_device_data_proc = ?*const fn (*anyopaque, ?*anyopaque, ?*const anyopaque, ma_uint32) callconv(.C) void;
pub const ma_device_notification_proc = ?*const fn (*const anyopaque) callconv(.C) void;
pub const ma_stop_proc = ?*const fn (*anyopaque) callconv(.C) void;
pub const ma_mutex = ma_pthread_mutex_t;
pub const ma_thread = ma_pthread_t;
const struct_unnamed_20 = extern struct {
    lpfOrder: ma_uint32,
};
const struct_unnamed_19 = extern struct {
    algorithm: ma_resample_algorithm,
    pBackendVTable: [*c]ma_resampling_backend_vtable,
    pBackendUserData: ?*anyopaque,
    linear: struct_unnamed_20,
};
pub const ma_channel = ma_uint8;
const struct_unnamed_21 = extern struct {
    pID: [*c]ma_device_id,
    id: ma_device_id,
    name: [256]u8,
    shareMode: ma_share_mode,
    format: ma_format,
    channels: ma_uint32,
    channelMap: [254]ma_channel,
    internalFormat: ma_format,
    internalChannels: ma_uint32,
    internalSampleRate: ma_uint32,
    internalChannelMap: [254]ma_channel,
    internalPeriodSizeInFrames: ma_uint32,
    internalPeriods: ma_uint32,
    channelMixMode: ma_channel_mix_mode,
    converter: ma_data_converter,
    pIntermediaryBuffer: ?*anyopaque,
    intermediaryBufferCap: ma_uint32,
    intermediaryBufferLen: ma_uint32,
    pInputCache: ?*anyopaque,
    inputCacheCap: ma_uint64,
    inputCacheConsumed: ma_uint64,
    inputCacheRemaining: ma_uint64,
};
const struct_unnamed_22 = extern struct {
    pID: [*c]ma_device_id,
    id: ma_device_id,
    name: [256]u8,
    shareMode: ma_share_mode,
    format: ma_format,
    channels: ma_uint32,
    channelMap: [254]ma_channel,
    internalFormat: ma_format,
    internalChannels: ma_uint32,
    internalSampleRate: ma_uint32,
    internalChannelMap: [254]ma_channel,
    internalPeriodSizeInFrames: ma_uint32,
    internalPeriods: ma_uint32,
    channelMixMode: ma_channel_mix_mode,
    converter: ma_data_converter,
    pIntermediaryBuffer: ?*anyopaque,
    intermediaryBufferCap: ma_uint32,
    intermediaryBufferLen: ma_uint32,
};
const struct_unnamed_24 = extern struct {
    pPCMPlayback: ma_ptr,
    pPCMCapture: ma_ptr,
    pPollDescriptorsPlayback: ?*anyopaque,
    pPollDescriptorsCapture: ?*anyopaque,
    pollDescriptorCountPlayback: c_int,
    pollDescriptorCountCapture: c_int,
    wakeupfdPlayback: c_int,
    wakeupfdCapture: c_int,
    isUsingMMapPlayback: ma_bool8,
    isUsingMMapCapture: ma_bool8,
};
const struct_unnamed_25 = extern struct {
    pMainLoop: ma_ptr,
    pPulseContext: ma_ptr,
    pStreamPlayback: ma_ptr,
    pStreamCapture: ma_ptr,
};
const struct_unnamed_26 = extern struct {
    pClient: ma_ptr,
    ppPortsPlayback: [*c]ma_ptr,
    ppPortsCapture: [*c]ma_ptr,
    pIntermediaryBufferPlayback: [*c]f32,
    pIntermediaryBufferCapture: [*c]f32,
};
const struct_unnamed_27 = extern struct {
    deviceThread: ma_thread,
    operationEvent: ma_event,
    operationCompletionEvent: ma_event,
    operationSemaphore: ma_semaphore,
    operation: ma_uint32,
    operationResult: ma_result,
    timer: ma_timer,
    priorRunTime: f64,
    currentPeriodFramesRemainingPlayback: ma_uint32,
    currentPeriodFramesRemainingCapture: ma_uint32,
    lastProcessedFramePlayback: ma_uint64,
    lastProcessedFrameCapture: ma_uint64,
    isStarted: ma_bool32 align(4),
};
const union_unnamed_23 = extern union {
    alsa: struct_unnamed_24,
    pulse: struct_unnamed_25,
    jack: struct_unnamed_26,
    null_device: struct_unnamed_27,
};
pub const struct_ma_device = extern struct {
    pContext: [*c]ma_context,
    type: ma_device_type,
    sampleRate: ma_uint32,
    state: ma_device_state align(4),
    onData: ma_device_data_proc,
    onNotification: ma_device_notification_proc,
    onStop: ma_stop_proc,
    pUserData: ?*anyopaque,
    startStopLock: ma_mutex,
    wakeupEvent: ma_event,
    startEvent: ma_event,
    stopEvent: ma_event,
    thread: ma_thread,
    workResult: ma_result,
    isOwnerOfContext: ma_bool8,
    noPreSilencedOutputBuffer: ma_bool8,
    noClip: ma_bool8,
    noDisableDenormals: ma_bool8,
    noFixedSizedCallback: ma_bool8,
    masterVolumeFactor: f32 align(4),
    duplexRB: ma_duplex_rb,
    resampling: struct_unnamed_19,
    playback: struct_unnamed_21,
    capture: struct_unnamed_22,
    unnamed_0: union_unnamed_23,
};
pub const ma_device = struct_ma_device;
const struct_unnamed_28 = extern struct {
    lpfOrder: ma_uint32,
};
pub const struct_ma_resampler_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRateIn: ma_uint32,
    sampleRateOut: ma_uint32,
    algorithm: ma_resample_algorithm,
    pBackendVTable: [*c]ma_resampling_backend_vtable,
    pBackendUserData: ?*anyopaque,
    linear: struct_unnamed_28,
};
pub const ma_resampler_config = struct_ma_resampler_config;
const struct_unnamed_29 = extern struct {
    pDeviceID: [*c]const ma_device_id,
    format: ma_format,
    channels: ma_uint32,
    pChannelMap: [*c]ma_channel,
    channelMixMode: ma_channel_mix_mode,
    shareMode: ma_share_mode,
};
const struct_unnamed_30 = extern struct {
    pDeviceID: [*c]const ma_device_id,
    format: ma_format,
    channels: ma_uint32,
    pChannelMap: [*c]ma_channel,
    channelMixMode: ma_channel_mix_mode,
    shareMode: ma_share_mode,
};
const struct_unnamed_31 = extern struct {
    noAutoConvertSRC: ma_bool8,
    noDefaultQualitySRC: ma_bool8,
    noAutoStreamRouting: ma_bool8,
    noHardwareOffloading: ma_bool8,
};
const struct_unnamed_32 = extern struct {
    noMMap: ma_bool32,
    noAutoFormat: ma_bool32,
    noAutoChannels: ma_bool32,
    noAutoResample: ma_bool32,
};
const struct_unnamed_33 = extern struct {
    pStreamNamePlayback: [*c]const u8,
    pStreamNameCapture: [*c]const u8,
};
const struct_unnamed_34 = extern struct {
    allowNominalSampleRateChange: ma_bool32,
};
const struct_unnamed_35 = extern struct {
    streamType: ma_opensl_stream_type,
    recordingPreset: ma_opensl_recording_preset,
};
const struct_unnamed_36 = extern struct {
    usage: ma_aaudio_usage,
    contentType: ma_aaudio_content_type,
    inputPreset: ma_aaudio_input_preset,
    noAutoStartAfterReroute: ma_bool32,
};
pub const struct_ma_device_config = extern struct {
    deviceType: ma_device_type,
    sampleRate: ma_uint32,
    periodSizeInFrames: ma_uint32,
    periodSizeInMilliseconds: ma_uint32,
    periods: ma_uint32,
    performanceProfile: ma_performance_profile,
    noPreSilencedOutputBuffer: ma_bool8,
    noClip: ma_bool8,
    noDisableDenormals: ma_bool8,
    noFixedSizedCallback: ma_bool8,
    dataCallback: ma_device_data_proc,
    notificationCallback: ma_device_notification_proc,
    stopCallback: ma_stop_proc,
    pUserData: ?*anyopaque,
    resampling: ma_resampler_config,
    playback: struct_unnamed_29,
    capture: struct_unnamed_30,
    wasapi: struct_unnamed_31,
    alsa: struct_unnamed_32,
    pulse: struct_unnamed_33,
    coreaudio: struct_unnamed_34,
    opensl: struct_unnamed_35,
    aaudio: struct_unnamed_36,
};
pub const ma_device_config = struct_ma_device_config;
pub const struct_ma_backend_callbacks = extern struct {
    onContextInit: ?*const fn ([*c]ma_context, [*c]const ma_context_config, [*c]ma_backend_callbacks) callconv(.C) ma_result,
    onContextUninit: ?*const fn ([*c]ma_context) callconv(.C) ma_result,
    onContextEnumerateDevices: ?*const fn ([*c]ma_context, ma_enum_devices_callback_proc, ?*anyopaque) callconv(.C) ma_result,
    onContextGetDeviceInfo: ?*const fn ([*c]ma_context, ma_device_type, [*c]const ma_device_id, [*c]ma_device_info) callconv(.C) ma_result,
    onDeviceInit: ?*const fn ([*c]ma_device, [*c]const ma_device_config, [*c]ma_device_descriptor, [*c]ma_device_descriptor) callconv(.C) ma_result,
    onDeviceUninit: ?*const fn ([*c]ma_device) callconv(.C) ma_result,
    onDeviceStart: ?*const fn ([*c]ma_device) callconv(.C) ma_result,
    onDeviceStop: ?*const fn ([*c]ma_device) callconv(.C) ma_result,
    onDeviceRead: ?*const fn ([*c]ma_device, ?*anyopaque, ma_uint32, [*c]ma_uint32) callconv(.C) ma_result,
    onDeviceWrite: ?*const fn ([*c]ma_device, ?*const anyopaque, ma_uint32, [*c]ma_uint32) callconv(.C) ma_result,
    onDeviceDataLoop: ?*const fn ([*c]ma_device) callconv(.C) ma_result,
    onDeviceDataLoopWakeup: ?*const fn ([*c]ma_device) callconv(.C) ma_result,
    onDeviceGetInfo: ?*const fn ([*c]ma_device, ma_device_type, [*c]ma_device_info) callconv(.C) ma_result,
};
pub const ma_backend_callbacks = struct_ma_backend_callbacks;
const struct_unnamed_38 = extern struct {
    asoundSO: ma_handle,
    snd_pcm_open: ma_proc,
    snd_pcm_close: ma_proc,
    snd_pcm_hw_params_sizeof: ma_proc,
    snd_pcm_hw_params_any: ma_proc,
    snd_pcm_hw_params_set_format: ma_proc,
    snd_pcm_hw_params_set_format_first: ma_proc,
    snd_pcm_hw_params_get_format_mask: ma_proc,
    snd_pcm_hw_params_set_channels: ma_proc,
    snd_pcm_hw_params_set_channels_near: ma_proc,
    snd_pcm_hw_params_set_channels_minmax: ma_proc,
    snd_pcm_hw_params_set_rate_resample: ma_proc,
    snd_pcm_hw_params_set_rate: ma_proc,
    snd_pcm_hw_params_set_rate_near: ma_proc,
    snd_pcm_hw_params_set_buffer_size_near: ma_proc,
    snd_pcm_hw_params_set_periods_near: ma_proc,
    snd_pcm_hw_params_set_access: ma_proc,
    snd_pcm_hw_params_get_format: ma_proc,
    snd_pcm_hw_params_get_channels: ma_proc,
    snd_pcm_hw_params_get_channels_min: ma_proc,
    snd_pcm_hw_params_get_channels_max: ma_proc,
    snd_pcm_hw_params_get_rate: ma_proc,
    snd_pcm_hw_params_get_rate_min: ma_proc,
    snd_pcm_hw_params_get_rate_max: ma_proc,
    snd_pcm_hw_params_get_buffer_size: ma_proc,
    snd_pcm_hw_params_get_periods: ma_proc,
    snd_pcm_hw_params_get_access: ma_proc,
    snd_pcm_hw_params_test_format: ma_proc,
    snd_pcm_hw_params_test_channels: ma_proc,
    snd_pcm_hw_params_test_rate: ma_proc,
    snd_pcm_hw_params: ma_proc,
    snd_pcm_sw_params_sizeof: ma_proc,
    snd_pcm_sw_params_current: ma_proc,
    snd_pcm_sw_params_get_boundary: ma_proc,
    snd_pcm_sw_params_set_avail_min: ma_proc,
    snd_pcm_sw_params_set_start_threshold: ma_proc,
    snd_pcm_sw_params_set_stop_threshold: ma_proc,
    snd_pcm_sw_params: ma_proc,
    snd_pcm_format_mask_sizeof: ma_proc,
    snd_pcm_format_mask_test: ma_proc,
    snd_pcm_get_chmap: ma_proc,
    snd_pcm_state: ma_proc,
    snd_pcm_prepare: ma_proc,
    snd_pcm_start: ma_proc,
    snd_pcm_drop: ma_proc,
    snd_pcm_drain: ma_proc,
    snd_pcm_reset: ma_proc,
    snd_device_name_hint: ma_proc,
    snd_device_name_get_hint: ma_proc,
    snd_card_get_index: ma_proc,
    snd_device_name_free_hint: ma_proc,
    snd_pcm_mmap_begin: ma_proc,
    snd_pcm_mmap_commit: ma_proc,
    snd_pcm_recover: ma_proc,
    snd_pcm_readi: ma_proc,
    snd_pcm_writei: ma_proc,
    snd_pcm_avail: ma_proc,
    snd_pcm_avail_update: ma_proc,
    snd_pcm_wait: ma_proc,
    snd_pcm_nonblock: ma_proc,
    snd_pcm_info: ma_proc,
    snd_pcm_info_sizeof: ma_proc,
    snd_pcm_info_get_name: ma_proc,
    snd_pcm_poll_descriptors: ma_proc,
    snd_pcm_poll_descriptors_count: ma_proc,
    snd_pcm_poll_descriptors_revents: ma_proc,
    snd_config_update_free_global: ma_proc,
    internalDeviceEnumLock: ma_mutex,
    useVerboseDeviceEnumeration: ma_bool32,
};
const struct_unnamed_39 = extern struct {
    pulseSO: ma_handle,
    pa_mainloop_new: ma_proc,
    pa_mainloop_free: ma_proc,
    pa_mainloop_quit: ma_proc,
    pa_mainloop_get_api: ma_proc,
    pa_mainloop_iterate: ma_proc,
    pa_mainloop_wakeup: ma_proc,
    pa_threaded_mainloop_new: ma_proc,
    pa_threaded_mainloop_free: ma_proc,
    pa_threaded_mainloop_start: ma_proc,
    pa_threaded_mainloop_stop: ma_proc,
    pa_threaded_mainloop_lock: ma_proc,
    pa_threaded_mainloop_unlock: ma_proc,
    pa_threaded_mainloop_wait: ma_proc,
    pa_threaded_mainloop_signal: ma_proc,
    pa_threaded_mainloop_accept: ma_proc,
    pa_threaded_mainloop_get_retval: ma_proc,
    pa_threaded_mainloop_get_api: ma_proc,
    pa_threaded_mainloop_in_thread: ma_proc,
    pa_threaded_mainloop_set_name: ma_proc,
    pa_context_new: ma_proc,
    pa_context_unref: ma_proc,
    pa_context_connect: ma_proc,
    pa_context_disconnect: ma_proc,
    pa_context_set_state_callback: ma_proc,
    pa_context_get_state: ma_proc,
    pa_context_get_sink_info_list: ma_proc,
    pa_context_get_source_info_list: ma_proc,
    pa_context_get_sink_info_by_name: ma_proc,
    pa_context_get_source_info_by_name: ma_proc,
    pa_operation_unref: ma_proc,
    pa_operation_get_state: ma_proc,
    pa_channel_map_init_extend: ma_proc,
    pa_channel_map_valid: ma_proc,
    pa_channel_map_compatible: ma_proc,
    pa_stream_new: ma_proc,
    pa_stream_unref: ma_proc,
    pa_stream_connect_playback: ma_proc,
    pa_stream_connect_record: ma_proc,
    pa_stream_disconnect: ma_proc,
    pa_stream_get_state: ma_proc,
    pa_stream_get_sample_spec: ma_proc,
    pa_stream_get_channel_map: ma_proc,
    pa_stream_get_buffer_attr: ma_proc,
    pa_stream_set_buffer_attr: ma_proc,
    pa_stream_get_device_name: ma_proc,
    pa_stream_set_write_callback: ma_proc,
    pa_stream_set_read_callback: ma_proc,
    pa_stream_set_suspended_callback: ma_proc,
    pa_stream_set_moved_callback: ma_proc,
    pa_stream_is_suspended: ma_proc,
    pa_stream_flush: ma_proc,
    pa_stream_drain: ma_proc,
    pa_stream_is_corked: ma_proc,
    pa_stream_cork: ma_proc,
    pa_stream_trigger: ma_proc,
    pa_stream_begin_write: ma_proc,
    pa_stream_write: ma_proc,
    pa_stream_peek: ma_proc,
    pa_stream_drop: ma_proc,
    pa_stream_writable_size: ma_proc,
    pa_stream_readable_size: ma_proc,
    pMainLoop: ma_ptr,
    pPulseContext: ma_ptr,
    pApplicationName: [*c]u8,
    pServerName: [*c]u8,
};
const struct_unnamed_40 = extern struct {
    jackSO: ma_handle,
    jack_client_open: ma_proc,
    jack_client_close: ma_proc,
    jack_client_name_size: ma_proc,
    jack_set_process_callback: ma_proc,
    jack_set_buffer_size_callback: ma_proc,
    jack_on_shutdown: ma_proc,
    jack_get_sample_rate: ma_proc,
    jack_get_buffer_size: ma_proc,
    jack_get_ports: ma_proc,
    jack_activate: ma_proc,
    jack_deactivate: ma_proc,
    jack_connect: ma_proc,
    jack_port_register: ma_proc,
    jack_port_name: ma_proc,
    jack_port_get_buffer: ma_proc,
    jack_free: ma_proc,
    pClientName: [*c]u8,
    tryStartServer: ma_bool32,
};
const struct_unnamed_41 = extern struct {
    _unused: c_int,
};
const union_unnamed_37 = extern union {
    alsa: struct_unnamed_38,
    pulse: struct_unnamed_39,
    jack: struct_unnamed_40,
    null_backend: struct_unnamed_41,
};
const struct_unnamed_43 = extern struct {
    pthreadSO: ma_handle,
    pthread_create: ma_proc,
    pthread_join: ma_proc,
    pthread_mutex_init: ma_proc,
    pthread_mutex_destroy: ma_proc,
    pthread_mutex_lock: ma_proc,
    pthread_mutex_unlock: ma_proc,
    pthread_cond_init: ma_proc,
    pthread_cond_destroy: ma_proc,
    pthread_cond_wait: ma_proc,
    pthread_cond_signal: ma_proc,
    pthread_attr_init: ma_proc,
    pthread_attr_destroy: ma_proc,
    pthread_attr_setschedpolicy: ma_proc,
    pthread_attr_getschedparam: ma_proc,
    pthread_attr_setschedparam: ma_proc,
};
const union_unnamed_42 = extern union {
    posix: struct_unnamed_43,
    _unused: c_int,
};
pub const struct_ma_context = extern struct {
    callbacks: ma_backend_callbacks,
    backend: ma_backend,
    pLog: [*c]ma_log,
    log: ma_log,
    threadPriority: ma_thread_priority,
    threadStackSize: usize,
    pUserData: ?*anyopaque,
    allocationCallbacks: ma_allocation_callbacks,
    deviceEnumLock: ma_mutex,
    deviceInfoLock: ma_mutex,
    deviceInfoCapacity: ma_uint32,
    playbackDeviceInfoCount: ma_uint32,
    captureDeviceInfoCount: ma_uint32,
    pDeviceInfos: [*c]ma_device_info,
    unnamed_0: union_unnamed_37,
    unnamed_1: union_unnamed_42,
};
pub const MA_CHANNEL_NONE: c_int = 0;
pub const MA_CHANNEL_MONO: c_int = 1;
pub const MA_CHANNEL_FRONT_LEFT: c_int = 2;
pub const MA_CHANNEL_FRONT_RIGHT: c_int = 3;
pub const MA_CHANNEL_FRONT_CENTER: c_int = 4;
pub const MA_CHANNEL_LFE: c_int = 5;
pub const MA_CHANNEL_BACK_LEFT: c_int = 6;
pub const MA_CHANNEL_BACK_RIGHT: c_int = 7;
pub const MA_CHANNEL_FRONT_LEFT_CENTER: c_int = 8;
pub const MA_CHANNEL_FRONT_RIGHT_CENTER: c_int = 9;
pub const MA_CHANNEL_BACK_CENTER: c_int = 10;
pub const MA_CHANNEL_SIDE_LEFT: c_int = 11;
pub const MA_CHANNEL_SIDE_RIGHT: c_int = 12;
pub const MA_CHANNEL_TOP_CENTER: c_int = 13;
pub const MA_CHANNEL_TOP_FRONT_LEFT: c_int = 14;
pub const MA_CHANNEL_TOP_FRONT_CENTER: c_int = 15;
pub const MA_CHANNEL_TOP_FRONT_RIGHT: c_int = 16;
pub const MA_CHANNEL_TOP_BACK_LEFT: c_int = 17;
pub const MA_CHANNEL_TOP_BACK_CENTER: c_int = 18;
pub const MA_CHANNEL_TOP_BACK_RIGHT: c_int = 19;
pub const MA_CHANNEL_AUX_0: c_int = 20;
pub const MA_CHANNEL_AUX_1: c_int = 21;
pub const MA_CHANNEL_AUX_2: c_int = 22;
pub const MA_CHANNEL_AUX_3: c_int = 23;
pub const MA_CHANNEL_AUX_4: c_int = 24;
pub const MA_CHANNEL_AUX_5: c_int = 25;
pub const MA_CHANNEL_AUX_6: c_int = 26;
pub const MA_CHANNEL_AUX_7: c_int = 27;
pub const MA_CHANNEL_AUX_8: c_int = 28;
pub const MA_CHANNEL_AUX_9: c_int = 29;
pub const MA_CHANNEL_AUX_10: c_int = 30;
pub const MA_CHANNEL_AUX_11: c_int = 31;
pub const MA_CHANNEL_AUX_12: c_int = 32;
pub const MA_CHANNEL_AUX_13: c_int = 33;
pub const MA_CHANNEL_AUX_14: c_int = 34;
pub const MA_CHANNEL_AUX_15: c_int = 35;
pub const MA_CHANNEL_AUX_16: c_int = 36;
pub const MA_CHANNEL_AUX_17: c_int = 37;
pub const MA_CHANNEL_AUX_18: c_int = 38;
pub const MA_CHANNEL_AUX_19: c_int = 39;
pub const MA_CHANNEL_AUX_20: c_int = 40;
pub const MA_CHANNEL_AUX_21: c_int = 41;
pub const MA_CHANNEL_AUX_22: c_int = 42;
pub const MA_CHANNEL_AUX_23: c_int = 43;
pub const MA_CHANNEL_AUX_24: c_int = 44;
pub const MA_CHANNEL_AUX_25: c_int = 45;
pub const MA_CHANNEL_AUX_26: c_int = 46;
pub const MA_CHANNEL_AUX_27: c_int = 47;
pub const MA_CHANNEL_AUX_28: c_int = 48;
pub const MA_CHANNEL_AUX_29: c_int = 49;
pub const MA_CHANNEL_AUX_30: c_int = 50;
pub const MA_CHANNEL_AUX_31: c_int = 51;
pub const MA_CHANNEL_LEFT: c_int = 2;
pub const MA_CHANNEL_RIGHT: c_int = 3;
pub const MA_CHANNEL_POSITION_COUNT: c_int = 52;
pub const _ma_channel_position = c_uint;
pub const MA_SUCCESS: c_int = 0;
pub const MA_ERROR: c_int = -1;
pub const MA_INVALID_ARGS: c_int = -2;
pub const MA_INVALID_OPERATION: c_int = -3;
pub const MA_OUT_OF_MEMORY: c_int = -4;
pub const MA_OUT_OF_RANGE: c_int = -5;
pub const MA_ACCESS_DENIED: c_int = -6;
pub const MA_DOES_NOT_EXIST: c_int = -7;
pub const MA_ALREADY_EXISTS: c_int = -8;
pub const MA_TOO_MANY_OPEN_FILES: c_int = -9;
pub const MA_INVALID_FILE: c_int = -10;
pub const MA_TOO_BIG: c_int = -11;
pub const MA_PATH_TOO_LONG: c_int = -12;
pub const MA_NAME_TOO_LONG: c_int = -13;
pub const MA_NOT_DIRECTORY: c_int = -14;
pub const MA_IS_DIRECTORY: c_int = -15;
pub const MA_DIRECTORY_NOT_EMPTY: c_int = -16;
pub const MA_AT_END: c_int = -17;
pub const MA_NO_SPACE: c_int = -18;
pub const MA_BUSY: c_int = -19;
pub const MA_IO_ERROR: c_int = -20;
pub const MA_INTERRUPT: c_int = -21;
pub const MA_UNAVAILABLE: c_int = -22;
pub const MA_ALREADY_IN_USE: c_int = -23;
pub const MA_BAD_ADDRESS: c_int = -24;
pub const MA_BAD_SEEK: c_int = -25;
pub const MA_BAD_PIPE: c_int = -26;
pub const MA_DEADLOCK: c_int = -27;
pub const MA_TOO_MANY_LINKS: c_int = -28;
pub const MA_NOT_IMPLEMENTED: c_int = -29;
pub const MA_NO_MESSAGE: c_int = -30;
pub const MA_BAD_MESSAGE: c_int = -31;
pub const MA_NO_DATA_AVAILABLE: c_int = -32;
pub const MA_INVALID_DATA: c_int = -33;
pub const MA_TIMEOUT: c_int = -34;
pub const MA_NO_NETWORK: c_int = -35;
pub const MA_NOT_UNIQUE: c_int = -36;
pub const MA_NOT_SOCKET: c_int = -37;
pub const MA_NO_ADDRESS: c_int = -38;
pub const MA_BAD_PROTOCOL: c_int = -39;
pub const MA_PROTOCOL_UNAVAILABLE: c_int = -40;
pub const MA_PROTOCOL_NOT_SUPPORTED: c_int = -41;
pub const MA_PROTOCOL_FAMILY_NOT_SUPPORTED: c_int = -42;
pub const MA_ADDRESS_FAMILY_NOT_SUPPORTED: c_int = -43;
pub const MA_SOCKET_NOT_SUPPORTED: c_int = -44;
pub const MA_CONNECTION_RESET: c_int = -45;
pub const MA_ALREADY_CONNECTED: c_int = -46;
pub const MA_NOT_CONNECTED: c_int = -47;
pub const MA_CONNECTION_REFUSED: c_int = -48;
pub const MA_NO_HOST: c_int = -49;
pub const MA_IN_PROGRESS: c_int = -50;
pub const MA_CANCELLED: c_int = -51;
pub const MA_MEMORY_ALREADY_MAPPED: c_int = -52;
pub const MA_FORMAT_NOT_SUPPORTED: c_int = -100;
pub const MA_DEVICE_TYPE_NOT_SUPPORTED: c_int = -101;
pub const MA_SHARE_MODE_NOT_SUPPORTED: c_int = -102;
pub const MA_NO_BACKEND: c_int = -103;
pub const MA_NO_DEVICE: c_int = -104;
pub const MA_API_NOT_FOUND: c_int = -105;
pub const MA_INVALID_DEVICE_CONFIG: c_int = -106;
pub const MA_LOOP: c_int = -107;
pub const MA_DEVICE_NOT_INITIALIZED: c_int = -200;
pub const MA_DEVICE_ALREADY_INITIALIZED: c_int = -201;
pub const MA_DEVICE_NOT_STARTED: c_int = -202;
pub const MA_DEVICE_NOT_STOPPED: c_int = -203;
pub const MA_FAILED_TO_INIT_BACKEND: c_int = -300;
pub const MA_FAILED_TO_OPEN_BACKEND_DEVICE: c_int = -301;
pub const MA_FAILED_TO_START_BACKEND_DEVICE: c_int = -302;
pub const MA_FAILED_TO_STOP_BACKEND_DEVICE: c_int = -303;
pub const ma_result = c_int;
pub const ma_stream_format_pcm: c_int = 0;
pub const ma_stream_format = c_uint;
pub const ma_stream_layout_interleaved: c_int = 0;
pub const ma_stream_layout_deinterleaved: c_int = 1;
pub const ma_stream_layout = c_uint;
pub const ma_dither_mode_none: c_int = 0;
pub const ma_dither_mode_rectangle: c_int = 1;
pub const ma_dither_mode_triangle: c_int = 2;
pub const ma_dither_mode = c_uint;
pub const ma_format_unknown: c_int = 0;
pub const ma_format_u8: c_int = 1;
pub const ma_format_s16: c_int = 2;
pub const ma_format_s24: c_int = 3;
pub const ma_format_s32: c_int = 4;
pub const ma_format_f32: c_int = 5;
pub const ma_format_count: c_int = 6;
pub const ma_format = c_uint;
pub const ma_standard_sample_rate_48000: c_int = 48000;
pub const ma_standard_sample_rate_44100: c_int = 44100;
pub const ma_standard_sample_rate_32000: c_int = 32000;
pub const ma_standard_sample_rate_24000: c_int = 24000;
pub const ma_standard_sample_rate_22050: c_int = 22050;
pub const ma_standard_sample_rate_88200: c_int = 88200;
pub const ma_standard_sample_rate_96000: c_int = 96000;
pub const ma_standard_sample_rate_176400: c_int = 176400;
pub const ma_standard_sample_rate_192000: c_int = 192000;
pub const ma_standard_sample_rate_16000: c_int = 16000;
pub const ma_standard_sample_rate_11025: c_int = 11250;
pub const ma_standard_sample_rate_8000: c_int = 8000;
pub const ma_standard_sample_rate_352800: c_int = 352800;
pub const ma_standard_sample_rate_384000: c_int = 384000;
pub const ma_standard_sample_rate_min: c_int = 8000;
pub const ma_standard_sample_rate_max: c_int = 384000;
pub const ma_standard_sample_rate_count: c_int = 14;
pub const ma_standard_sample_rate = c_uint;
pub const ma_channel_mix_mode_rectangular: c_int = 0;
pub const ma_channel_mix_mode_simple: c_int = 1;
pub const ma_channel_mix_mode_custom_weights: c_int = 2;
pub const ma_channel_mix_mode_default: c_int = 0;
pub const ma_channel_mix_mode = c_uint;
pub const ma_standard_channel_map_microsoft: c_int = 0;
pub const ma_standard_channel_map_alsa: c_int = 1;
pub const ma_standard_channel_map_rfc3551: c_int = 2;
pub const ma_standard_channel_map_flac: c_int = 3;
pub const ma_standard_channel_map_vorbis: c_int = 4;
pub const ma_standard_channel_map_sound4: c_int = 5;
pub const ma_standard_channel_map_sndio: c_int = 6;
pub const ma_standard_channel_map_webaudio: c_int = 3;
pub const ma_standard_channel_map_default: c_int = 0;
pub const ma_standard_channel_map = c_uint;
pub const ma_performance_profile_low_latency: c_int = 0;
pub const ma_performance_profile_conservative: c_int = 1;
pub const ma_performance_profile = c_uint;
pub const ma_allocation_callbacks = extern struct {
    pUserData: ?*anyopaque,
    onMalloc: ?*const fn (usize, ?*anyopaque) callconv(.C) ?*anyopaque,
    onRealloc: ?*const fn (?*anyopaque, usize, ?*anyopaque) callconv(.C) ?*anyopaque,
    onFree: ?*const fn (?*anyopaque, ?*anyopaque) callconv(.C) void,
};
pub const ma_lcg = extern struct {
    state: ma_int32,
};
pub const ma_spinlock = ma_uint32;
pub const ma_thread_priority_idle: c_int = -5;
pub const ma_thread_priority_lowest: c_int = -4;
pub const ma_thread_priority_low: c_int = -3;
pub const ma_thread_priority_normal: c_int = -2;
pub const ma_thread_priority_high: c_int = -1;
pub const ma_thread_priority_highest: c_int = 0;
pub const ma_thread_priority_realtime: c_int = 1;
pub const ma_thread_priority_default: c_int = 0;
pub const ma_thread_priority = c_int;
pub const ma_event = extern struct {
    value: ma_uint32,
    lock: ma_pthread_mutex_t,
    cond: ma_pthread_cond_t,
};
pub const ma_semaphore = extern struct {
    value: c_int,
    lock: ma_pthread_mutex_t,
    cond: ma_pthread_cond_t,
};
pub extern fn ma_version(pMajor: [*c]ma_uint32, pMinor: [*c]ma_uint32, pRevision: [*c]ma_uint32) void;
pub extern fn ma_version_string() [*c]const u8;
pub const struct___va_list_tag = extern struct {
    gp_offset: c_uint,
    fp_offset: c_uint,
    overflow_arg_area: ?*anyopaque,
    reg_save_area: ?*anyopaque,
};
pub const __builtin_va_list = [1]struct___va_list_tag;
pub const va_list = __builtin_va_list;
pub const __gnuc_va_list = __builtin_va_list;
pub const ma_log_callback_proc = ?*const fn (?*anyopaque, ma_uint32, [*c]const u8) callconv(.C) void;
pub const ma_log_callback = extern struct {
    onLog: ma_log_callback_proc,
    pUserData: ?*anyopaque,
};
pub extern fn ma_log_callback_init(onLog: ma_log_callback_proc, pUserData: ?*anyopaque) ma_log_callback;
pub const ma_log = extern struct {
    callbacks: [4]ma_log_callback,
    callbackCount: ma_uint32,
    allocationCallbacks: ma_allocation_callbacks,
    lock: ma_mutex,
};
pub extern fn ma_log_init(pAllocationCallbacks: [*c]const ma_allocation_callbacks, pLog: [*c]ma_log) ma_result;
pub extern fn ma_log_uninit(pLog: [*c]ma_log) void;
pub extern fn ma_log_register_callback(pLog: [*c]ma_log, callback: ma_log_callback) ma_result;
pub extern fn ma_log_unregister_callback(pLog: [*c]ma_log, callback: ma_log_callback) ma_result;
pub extern fn ma_log_post(pLog: [*c]ma_log, level: ma_uint32, pMessage: [*c]const u8) ma_result;
pub extern fn ma_log_postv(pLog: [*c]ma_log, level: ma_uint32, pFormat: [*c]const u8, args: [*c]struct___va_list_tag) ma_result;
pub extern fn ma_log_postf(pLog: [*c]ma_log, level: ma_uint32, pFormat: [*c]const u8, ...) ma_result;
pub const ma_biquad_coefficient = extern union {
    f32: f32,
    s32: ma_int32,
};
pub const ma_biquad_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    b0: f64,
    b1: f64,
    b2: f64,
    a0: f64,
    a1: f64,
    a2: f64,
};
pub extern fn ma_biquad_config_init(format: ma_format, channels: ma_uint32, b0: f64, b1: f64, b2: f64, a0: f64, a1: f64, a2: f64) ma_biquad_config;
pub const ma_biquad = extern struct {
    format: ma_format,
    channels: ma_uint32,
    b0: ma_biquad_coefficient,
    b1: ma_biquad_coefficient,
    b2: ma_biquad_coefficient,
    a1: ma_biquad_coefficient,
    a2: ma_biquad_coefficient,
    pR1: [*c]ma_biquad_coefficient,
    pR2: [*c]ma_biquad_coefficient,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_biquad_get_heap_size(pConfig: [*c]const ma_biquad_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_biquad_init_preallocated(pConfig: [*c]const ma_biquad_config, pHeap: ?*anyopaque, pBQ: [*c]ma_biquad) ma_result;
pub extern fn ma_biquad_init(pConfig: [*c]const ma_biquad_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pBQ: [*c]ma_biquad) ma_result;
pub extern fn ma_biquad_uninit(pBQ: [*c]ma_biquad, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_biquad_reinit(pConfig: [*c]const ma_biquad_config, pBQ: [*c]ma_biquad) ma_result;
pub extern fn ma_biquad_clear_cache(pBQ: [*c]ma_biquad) ma_result;
pub extern fn ma_biquad_process_pcm_frames(pBQ: [*c]ma_biquad, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_biquad_get_latency(pBQ: [*c]const ma_biquad) ma_uint32;
pub const ma_lpf1_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    q: f64,
};
pub const ma_lpf2_config = ma_lpf1_config;
pub extern fn ma_lpf1_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64) ma_lpf1_config;
pub extern fn ma_lpf2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, q: f64) ma_lpf2_config;
pub const ma_lpf1 = extern struct {
    format: ma_format,
    channels: ma_uint32,
    a: ma_biquad_coefficient,
    pR1: [*c]ma_biquad_coefficient,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_lpf1_get_heap_size(pConfig: [*c]const ma_lpf1_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_lpf1_init_preallocated(pConfig: [*c]const ma_lpf1_config, pHeap: ?*anyopaque, pLPF: [*c]ma_lpf1) ma_result;
pub extern fn ma_lpf1_init(pConfig: [*c]const ma_lpf1_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pLPF: [*c]ma_lpf1) ma_result;
pub extern fn ma_lpf1_uninit(pLPF: [*c]ma_lpf1, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_lpf1_reinit(pConfig: [*c]const ma_lpf1_config, pLPF: [*c]ma_lpf1) ma_result;
pub extern fn ma_lpf1_clear_cache(pLPF: [*c]ma_lpf1) ma_result;
pub extern fn ma_lpf1_process_pcm_frames(pLPF: [*c]ma_lpf1, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_lpf1_get_latency(pLPF: [*c]const ma_lpf1) ma_uint32;
pub const ma_lpf2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_lpf2_get_heap_size(pConfig: [*c]const ma_lpf2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_lpf2_init_preallocated(pConfig: [*c]const ma_lpf2_config, pHeap: ?*anyopaque, pHPF: [*c]ma_lpf2) ma_result;
pub extern fn ma_lpf2_init(pConfig: [*c]const ma_lpf2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pLPF: [*c]ma_lpf2) ma_result;
pub extern fn ma_lpf2_uninit(pLPF: [*c]ma_lpf2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_lpf2_reinit(pConfig: [*c]const ma_lpf2_config, pLPF: [*c]ma_lpf2) ma_result;
pub extern fn ma_lpf2_clear_cache(pLPF: [*c]ma_lpf2) ma_result;
pub extern fn ma_lpf2_process_pcm_frames(pLPF: [*c]ma_lpf2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_lpf2_get_latency(pLPF: [*c]const ma_lpf2) ma_uint32;
pub const ma_lpf_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    order: ma_uint32,
};
pub extern fn ma_lpf_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_lpf_config;
pub const ma_lpf = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    lpf1Count: ma_uint32,
    lpf2Count: ma_uint32,
    pLPF1: [*c]ma_lpf1,
    pLPF2: [*c]ma_lpf2,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_lpf_get_heap_size(pConfig: [*c]const ma_lpf_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_lpf_init_preallocated(pConfig: [*c]const ma_lpf_config, pHeap: ?*anyopaque, pLPF: [*c]ma_lpf) ma_result;
pub extern fn ma_lpf_init(pConfig: [*c]const ma_lpf_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pLPF: [*c]ma_lpf) ma_result;
pub extern fn ma_lpf_uninit(pLPF: [*c]ma_lpf, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_lpf_reinit(pConfig: [*c]const ma_lpf_config, pLPF: [*c]ma_lpf) ma_result;
pub extern fn ma_lpf_clear_cache(pLPF: [*c]ma_lpf) ma_result;
pub extern fn ma_lpf_process_pcm_frames(pLPF: [*c]ma_lpf, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_lpf_get_latency(pLPF: [*c]const ma_lpf) ma_uint32;
pub const ma_hpf1_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    q: f64,
};
pub const ma_hpf2_config = ma_hpf1_config;
pub extern fn ma_hpf1_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64) ma_hpf1_config;
pub extern fn ma_hpf2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, q: f64) ma_hpf2_config;
pub const ma_hpf1 = extern struct {
    format: ma_format,
    channels: ma_uint32,
    a: ma_biquad_coefficient,
    pR1: [*c]ma_biquad_coefficient,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_hpf1_get_heap_size(pConfig: [*c]const ma_hpf1_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_hpf1_init_preallocated(pConfig: [*c]const ma_hpf1_config, pHeap: ?*anyopaque, pLPF: [*c]ma_hpf1) ma_result;
pub extern fn ma_hpf1_init(pConfig: [*c]const ma_hpf1_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pHPF: [*c]ma_hpf1) ma_result;
pub extern fn ma_hpf1_uninit(pHPF: [*c]ma_hpf1, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_hpf1_reinit(pConfig: [*c]const ma_hpf1_config, pHPF: [*c]ma_hpf1) ma_result;
pub extern fn ma_hpf1_process_pcm_frames(pHPF: [*c]ma_hpf1, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_hpf1_get_latency(pHPF: [*c]const ma_hpf1) ma_uint32;
pub const ma_hpf2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_hpf2_get_heap_size(pConfig: [*c]const ma_hpf2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_hpf2_init_preallocated(pConfig: [*c]const ma_hpf2_config, pHeap: ?*anyopaque, pHPF: [*c]ma_hpf2) ma_result;
pub extern fn ma_hpf2_init(pConfig: [*c]const ma_hpf2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pHPF: [*c]ma_hpf2) ma_result;
pub extern fn ma_hpf2_uninit(pHPF: [*c]ma_hpf2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_hpf2_reinit(pConfig: [*c]const ma_hpf2_config, pHPF: [*c]ma_hpf2) ma_result;
pub extern fn ma_hpf2_process_pcm_frames(pHPF: [*c]ma_hpf2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_hpf2_get_latency(pHPF: [*c]const ma_hpf2) ma_uint32;
pub const ma_hpf_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    order: ma_uint32,
};
pub extern fn ma_hpf_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_hpf_config;
pub const ma_hpf = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    hpf1Count: ma_uint32,
    hpf2Count: ma_uint32,
    pHPF1: [*c]ma_hpf1,
    pHPF2: [*c]ma_hpf2,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_hpf_get_heap_size(pConfig: [*c]const ma_hpf_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_hpf_init_preallocated(pConfig: [*c]const ma_hpf_config, pHeap: ?*anyopaque, pLPF: [*c]ma_hpf) ma_result;
pub extern fn ma_hpf_init(pConfig: [*c]const ma_hpf_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pHPF: [*c]ma_hpf) ma_result;
pub extern fn ma_hpf_uninit(pHPF: [*c]ma_hpf, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_hpf_reinit(pConfig: [*c]const ma_hpf_config, pHPF: [*c]ma_hpf) ma_result;
pub extern fn ma_hpf_process_pcm_frames(pHPF: [*c]ma_hpf, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_hpf_get_latency(pHPF: [*c]const ma_hpf) ma_uint32;
pub const ma_bpf2_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    q: f64,
};
pub extern fn ma_bpf2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, q: f64) ma_bpf2_config;
pub const ma_bpf2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_bpf2_get_heap_size(pConfig: [*c]const ma_bpf2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_bpf2_init_preallocated(pConfig: [*c]const ma_bpf2_config, pHeap: ?*anyopaque, pBPF: [*c]ma_bpf2) ma_result;
pub extern fn ma_bpf2_init(pConfig: [*c]const ma_bpf2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pBPF: [*c]ma_bpf2) ma_result;
pub extern fn ma_bpf2_uninit(pBPF: [*c]ma_bpf2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_bpf2_reinit(pConfig: [*c]const ma_bpf2_config, pBPF: [*c]ma_bpf2) ma_result;
pub extern fn ma_bpf2_process_pcm_frames(pBPF: [*c]ma_bpf2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_bpf2_get_latency(pBPF: [*c]const ma_bpf2) ma_uint32;
pub const ma_bpf_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cutoffFrequency: f64,
    order: ma_uint32,
};
pub extern fn ma_bpf_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_bpf_config;
pub const ma_bpf = extern struct {
    format: ma_format,
    channels: ma_uint32,
    bpf2Count: ma_uint32,
    pBPF2: [*c]ma_bpf2,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_bpf_get_heap_size(pConfig: [*c]const ma_bpf_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_bpf_init_preallocated(pConfig: [*c]const ma_bpf_config, pHeap: ?*anyopaque, pBPF: [*c]ma_bpf) ma_result;
pub extern fn ma_bpf_init(pConfig: [*c]const ma_bpf_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pBPF: [*c]ma_bpf) ma_result;
pub extern fn ma_bpf_uninit(pBPF: [*c]ma_bpf, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_bpf_reinit(pConfig: [*c]const ma_bpf_config, pBPF: [*c]ma_bpf) ma_result;
pub extern fn ma_bpf_process_pcm_frames(pBPF: [*c]ma_bpf, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_bpf_get_latency(pBPF: [*c]const ma_bpf) ma_uint32;
pub const ma_notch2_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    q: f64,
    frequency: f64,
};
pub const ma_notch_config = ma_notch2_config;
pub extern fn ma_notch2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, q: f64, frequency: f64) ma_notch2_config;
pub const ma_notch2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_notch2_get_heap_size(pConfig: [*c]const ma_notch2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_notch2_init_preallocated(pConfig: [*c]const ma_notch2_config, pHeap: ?*anyopaque, pFilter: [*c]ma_notch2) ma_result;
pub extern fn ma_notch2_init(pConfig: [*c]const ma_notch2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pFilter: [*c]ma_notch2) ma_result;
pub extern fn ma_notch2_uninit(pFilter: [*c]ma_notch2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_notch2_reinit(pConfig: [*c]const ma_notch2_config, pFilter: [*c]ma_notch2) ma_result;
pub extern fn ma_notch2_process_pcm_frames(pFilter: [*c]ma_notch2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_notch2_get_latency(pFilter: [*c]const ma_notch2) ma_uint32;
pub const ma_peak2_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    gainDB: f64,
    q: f64,
    frequency: f64,
};
pub const ma_peak_config = ma_peak2_config;
pub extern fn ma_peak2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, q: f64, frequency: f64) ma_peak2_config;
pub const ma_peak2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_peak2_get_heap_size(pConfig: [*c]const ma_peak2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_peak2_init_preallocated(pConfig: [*c]const ma_peak2_config, pHeap: ?*anyopaque, pFilter: [*c]ma_peak2) ma_result;
pub extern fn ma_peak2_init(pConfig: [*c]const ma_peak2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pFilter: [*c]ma_peak2) ma_result;
pub extern fn ma_peak2_uninit(pFilter: [*c]ma_peak2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_peak2_reinit(pConfig: [*c]const ma_peak2_config, pFilter: [*c]ma_peak2) ma_result;
pub extern fn ma_peak2_process_pcm_frames(pFilter: [*c]ma_peak2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_peak2_get_latency(pFilter: [*c]const ma_peak2) ma_uint32;
pub const ma_loshelf2_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    gainDB: f64,
    shelfSlope: f64,
    frequency: f64,
};
pub const ma_loshelf_config = ma_loshelf2_config;
pub extern fn ma_loshelf2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, shelfSlope: f64, frequency: f64) ma_loshelf2_config;
pub const ma_loshelf2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_loshelf2_get_heap_size(pConfig: [*c]const ma_loshelf2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_loshelf2_init_preallocated(pConfig: [*c]const ma_loshelf2_config, pHeap: ?*anyopaque, pFilter: [*c]ma_loshelf2) ma_result;
pub extern fn ma_loshelf2_init(pConfig: [*c]const ma_loshelf2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pFilter: [*c]ma_loshelf2) ma_result;
pub extern fn ma_loshelf2_uninit(pFilter: [*c]ma_loshelf2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_loshelf2_reinit(pConfig: [*c]const ma_loshelf2_config, pFilter: [*c]ma_loshelf2) ma_result;
pub extern fn ma_loshelf2_process_pcm_frames(pFilter: [*c]ma_loshelf2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_loshelf2_get_latency(pFilter: [*c]const ma_loshelf2) ma_uint32;
pub const ma_hishelf2_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    gainDB: f64,
    shelfSlope: f64,
    frequency: f64,
};
pub const ma_hishelf_config = ma_hishelf2_config;
pub extern fn ma_hishelf2_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, shelfSlope: f64, frequency: f64) ma_hishelf2_config;
pub const ma_hishelf2 = extern struct {
    bq: ma_biquad,
};
pub extern fn ma_hishelf2_get_heap_size(pConfig: [*c]const ma_hishelf2_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_hishelf2_init_preallocated(pConfig: [*c]const ma_hishelf2_config, pHeap: ?*anyopaque, pFilter: [*c]ma_hishelf2) ma_result;
pub extern fn ma_hishelf2_init(pConfig: [*c]const ma_hishelf2_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pFilter: [*c]ma_hishelf2) ma_result;
pub extern fn ma_hishelf2_uninit(pFilter: [*c]ma_hishelf2, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_hishelf2_reinit(pConfig: [*c]const ma_hishelf2_config, pFilter: [*c]ma_hishelf2) ma_result;
pub extern fn ma_hishelf2_process_pcm_frames(pFilter: [*c]ma_hishelf2, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_hishelf2_get_latency(pFilter: [*c]const ma_hishelf2) ma_uint32;
pub const ma_delay_config = extern struct {
    channels: ma_uint32,
    sampleRate: ma_uint32,
    delayInFrames: ma_uint32,
    delayStart: ma_bool32,
    wet: f32,
    dry: f32,
    decay: f32,
};
pub extern fn ma_delay_config_init(channels: ma_uint32, sampleRate: ma_uint32, delayInFrames: ma_uint32, decay: f32) ma_delay_config;
pub const ma_delay = extern struct {
    config: ma_delay_config,
    cursor: ma_uint32,
    bufferSizeInFrames: ma_uint32,
    pBuffer: [*c]f32,
};
pub extern fn ma_delay_init(pConfig: [*c]const ma_delay_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pDelay: [*c]ma_delay) ma_result;
pub extern fn ma_delay_uninit(pDelay: [*c]ma_delay, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_delay_process_pcm_frames(pDelay: [*c]ma_delay, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint32) ma_result;
pub extern fn ma_delay_set_wet(pDelay: [*c]ma_delay, value: f32) void;
pub extern fn ma_delay_get_wet(pDelay: [*c]const ma_delay) f32;
pub extern fn ma_delay_set_dry(pDelay: [*c]ma_delay, value: f32) void;
pub extern fn ma_delay_get_dry(pDelay: [*c]const ma_delay) f32;
pub extern fn ma_delay_set_decay(pDelay: [*c]ma_delay, value: f32) void;
pub extern fn ma_delay_get_decay(pDelay: [*c]const ma_delay) f32;
pub const ma_gainer_config = extern struct {
    channels: ma_uint32,
    smoothTimeInFrames: ma_uint32,
};
pub extern fn ma_gainer_config_init(channels: ma_uint32, smoothTimeInFrames: ma_uint32) ma_gainer_config;
pub const ma_gainer = extern struct {
    config: ma_gainer_config,
    t: ma_uint32,
    pOldGains: [*c]f32,
    pNewGains: [*c]f32,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_gainer_get_heap_size(pConfig: [*c]const ma_gainer_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_gainer_init_preallocated(pConfig: [*c]const ma_gainer_config, pHeap: ?*anyopaque, pGainer: [*c]ma_gainer) ma_result;
pub extern fn ma_gainer_init(pConfig: [*c]const ma_gainer_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pGainer: [*c]ma_gainer) ma_result;
pub extern fn ma_gainer_uninit(pGainer: [*c]ma_gainer, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_gainer_process_pcm_frames(pGainer: [*c]ma_gainer, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_gainer_set_gain(pGainer: [*c]ma_gainer, newGain: f32) ma_result;
pub extern fn ma_gainer_set_gains(pGainer: [*c]ma_gainer, pNewGains: [*c]f32) ma_result;
pub const ma_pan_mode_balance: c_int = 0;
pub const ma_pan_mode_pan: c_int = 1;
pub const ma_pan_mode = c_uint;
pub const ma_panner_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    mode: ma_pan_mode,
    pan: f32,
};
pub extern fn ma_panner_config_init(format: ma_format, channels: ma_uint32) ma_panner_config;
pub const ma_panner = extern struct {
    format: ma_format,
    channels: ma_uint32,
    mode: ma_pan_mode,
    pan: f32,
};
pub extern fn ma_panner_init(pConfig: [*c]const ma_panner_config, pPanner: [*c]ma_panner) ma_result;
pub extern fn ma_panner_process_pcm_frames(pPanner: [*c]ma_panner, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_panner_set_mode(pPanner: [*c]ma_panner, mode: ma_pan_mode) void;
pub extern fn ma_panner_get_mode(pPanner: [*c]const ma_panner) ma_pan_mode;
pub extern fn ma_panner_set_pan(pPanner: [*c]ma_panner, pan: f32) void;
pub extern fn ma_panner_get_pan(pPanner: [*c]const ma_panner) f32;
pub const ma_fader_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
};
pub extern fn ma_fader_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32) ma_fader_config;
pub const ma_fader = extern struct {
    config: ma_fader_config,
    volumeBeg: f32,
    volumeEnd: f32,
    lengthInFrames: ma_uint64,
    cursorInFrames: ma_uint64,
};
pub extern fn ma_fader_init(pConfig: [*c]const ma_fader_config, pFader: [*c]ma_fader) ma_result;
pub extern fn ma_fader_process_pcm_frames(pFader: [*c]ma_fader, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_fader_get_data_format(pFader: [*c]const ma_fader, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32) void;
pub extern fn ma_fader_set_fade(pFader: [*c]ma_fader, volumeBeg: f32, volumeEnd: f32, lengthInFrames: ma_uint64) void;
pub extern fn ma_fader_get_current_volume(pFader: [*c]ma_fader) f32;
pub const ma_vec3f = extern struct {
    x: f32,
    y: f32,
    z: f32,
};
pub const ma_attenuation_model_none: c_int = 0;
pub const ma_attenuation_model_inverse: c_int = 1;
pub const ma_attenuation_model_linear: c_int = 2;
pub const ma_attenuation_model_exponential: c_int = 3;
pub const ma_attenuation_model = c_uint;
pub const ma_positioning_absolute: c_int = 0;
pub const ma_positioning_relative: c_int = 1;
pub const ma_positioning = c_uint;
pub const ma_handedness_right: c_int = 0;
pub const ma_handedness_left: c_int = 1;
pub const ma_handedness = c_uint;
pub const ma_spatializer_listener_config = extern struct {
    channelsOut: ma_uint32,
    pChannelMapOut: [*c]ma_channel,
    handedness: ma_handedness,
    coneInnerAngleInRadians: f32,
    coneOuterAngleInRadians: f32,
    coneOuterGain: f32,
    speedOfSound: f32,
    worldUp: ma_vec3f,
};
pub extern fn ma_spatializer_listener_config_init(channelsOut: ma_uint32) ma_spatializer_listener_config;
pub const ma_spatializer_listener = extern struct {
    config: ma_spatializer_listener_config,
    position: ma_vec3f,
    direction: ma_vec3f,
    velocity: ma_vec3f,
    isEnabled: ma_bool32,
    _ownsHeap: ma_bool32,
    _pHeap: ?*anyopaque,
};
pub extern fn ma_spatializer_listener_get_heap_size(pConfig: [*c]const ma_spatializer_listener_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_spatializer_listener_init_preallocated(pConfig: [*c]const ma_spatializer_listener_config, pHeap: ?*anyopaque, pListener: [*c]ma_spatializer_listener) ma_result;
pub extern fn ma_spatializer_listener_init(pConfig: [*c]const ma_spatializer_listener_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pListener: [*c]ma_spatializer_listener) ma_result;
pub extern fn ma_spatializer_listener_uninit(pListener: [*c]ma_spatializer_listener, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_spatializer_listener_get_channel_map(pListener: [*c]ma_spatializer_listener) [*c]ma_channel;
pub extern fn ma_spatializer_listener_set_cone(pListener: [*c]ma_spatializer_listener, innerAngleInRadians: f32, outerAngleInRadians: f32, outerGain: f32) void;
pub extern fn ma_spatializer_listener_get_cone(pListener: [*c]const ma_spatializer_listener, pInnerAngleInRadians: [*c]f32, pOuterAngleInRadians: [*c]f32, pOuterGain: [*c]f32) void;
pub extern fn ma_spatializer_listener_set_position(pListener: [*c]ma_spatializer_listener, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_listener_get_position(pListener: [*c]const ma_spatializer_listener) ma_vec3f;
pub extern fn ma_spatializer_listener_set_direction(pListener: [*c]ma_spatializer_listener, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_listener_get_direction(pListener: [*c]const ma_spatializer_listener) ma_vec3f;
pub extern fn ma_spatializer_listener_set_velocity(pListener: [*c]ma_spatializer_listener, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_listener_get_velocity(pListener: [*c]const ma_spatializer_listener) ma_vec3f;
pub extern fn ma_spatializer_listener_set_speed_of_sound(pListener: [*c]ma_spatializer_listener, speedOfSound: f32) void;
pub extern fn ma_spatializer_listener_get_speed_of_sound(pListener: [*c]const ma_spatializer_listener) f32;
pub extern fn ma_spatializer_listener_set_world_up(pListener: [*c]ma_spatializer_listener, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_listener_get_world_up(pListener: [*c]const ma_spatializer_listener) ma_vec3f;
pub extern fn ma_spatializer_listener_set_enabled(pListener: [*c]ma_spatializer_listener, isEnabled: ma_bool32) void;
pub extern fn ma_spatializer_listener_is_enabled(pListener: [*c]const ma_spatializer_listener) ma_bool32;
pub const ma_spatializer_config = extern struct {
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    pChannelMapIn: [*c]ma_channel,
    attenuationModel: ma_attenuation_model,
    positioning: ma_positioning,
    handedness: ma_handedness,
    minGain: f32,
    maxGain: f32,
    minDistance: f32,
    maxDistance: f32,
    rolloff: f32,
    coneInnerAngleInRadians: f32,
    coneOuterAngleInRadians: f32,
    coneOuterGain: f32,
    dopplerFactor: f32,
    directionalAttenuationFactor: f32,
    gainSmoothTimeInFrames: ma_uint32,
};
pub extern fn ma_spatializer_config_init(channelsIn: ma_uint32, channelsOut: ma_uint32) ma_spatializer_config;
pub const ma_spatializer = extern struct {
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    pChannelMapIn: [*c]ma_channel,
    attenuationModel: ma_attenuation_model,
    positioning: ma_positioning,
    handedness: ma_handedness,
    minGain: f32,
    maxGain: f32,
    minDistance: f32,
    maxDistance: f32,
    rolloff: f32,
    coneInnerAngleInRadians: f32,
    coneOuterAngleInRadians: f32,
    coneOuterGain: f32,
    dopplerFactor: f32,
    directionalAttenuationFactor: f32,
    gainSmoothTimeInFrames: ma_uint32,
    position: ma_vec3f,
    direction: ma_vec3f,
    velocity: ma_vec3f,
    dopplerPitch: f32,
    gainer: ma_gainer,
    pNewChannelGainsOut: [*c]f32,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_spatializer_get_heap_size(pConfig: [*c]const ma_spatializer_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_spatializer_init_preallocated(pConfig: [*c]const ma_spatializer_config, pHeap: ?*anyopaque, pSpatializer: [*c]ma_spatializer) ma_result;
pub extern fn ma_spatializer_init(pConfig: [*c]const ma_spatializer_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pSpatializer: [*c]ma_spatializer) ma_result;
pub extern fn ma_spatializer_uninit(pSpatializer: [*c]ma_spatializer, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_spatializer_process_pcm_frames(pSpatializer: [*c]ma_spatializer, pListener: [*c]ma_spatializer_listener, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_spatializer_get_input_channels(pSpatializer: [*c]const ma_spatializer) ma_uint32;
pub extern fn ma_spatializer_get_output_channels(pSpatializer: [*c]const ma_spatializer) ma_uint32;
pub extern fn ma_spatializer_set_attenuation_model(pSpatializer: [*c]ma_spatializer, attenuationModel: ma_attenuation_model) void;
pub extern fn ma_spatializer_get_attenuation_model(pSpatializer: [*c]const ma_spatializer) ma_attenuation_model;
pub extern fn ma_spatializer_set_positioning(pSpatializer: [*c]ma_spatializer, positioning: ma_positioning) void;
pub extern fn ma_spatializer_get_positioning(pSpatializer: [*c]const ma_spatializer) ma_positioning;
pub extern fn ma_spatializer_set_rolloff(pSpatializer: [*c]ma_spatializer, rolloff: f32) void;
pub extern fn ma_spatializer_get_rolloff(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_min_gain(pSpatializer: [*c]ma_spatializer, minGain: f32) void;
pub extern fn ma_spatializer_get_min_gain(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_max_gain(pSpatializer: [*c]ma_spatializer, maxGain: f32) void;
pub extern fn ma_spatializer_get_max_gain(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_min_distance(pSpatializer: [*c]ma_spatializer, minDistance: f32) void;
pub extern fn ma_spatializer_get_min_distance(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_max_distance(pSpatializer: [*c]ma_spatializer, maxDistance: f32) void;
pub extern fn ma_spatializer_get_max_distance(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_cone(pSpatializer: [*c]ma_spatializer, innerAngleInRadians: f32, outerAngleInRadians: f32, outerGain: f32) void;
pub extern fn ma_spatializer_get_cone(pSpatializer: [*c]const ma_spatializer, pInnerAngleInRadians: [*c]f32, pOuterAngleInRadians: [*c]f32, pOuterGain: [*c]f32) void;
pub extern fn ma_spatializer_set_doppler_factor(pSpatializer: [*c]ma_spatializer, dopplerFactor: f32) void;
pub extern fn ma_spatializer_get_doppler_factor(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_directional_attenuation_factor(pSpatializer: [*c]ma_spatializer, directionalAttenuationFactor: f32) void;
pub extern fn ma_spatializer_get_directional_attenuation_factor(pSpatializer: [*c]const ma_spatializer) f32;
pub extern fn ma_spatializer_set_position(pSpatializer: [*c]ma_spatializer, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_get_position(pSpatializer: [*c]const ma_spatializer) ma_vec3f;
pub extern fn ma_spatializer_set_direction(pSpatializer: [*c]ma_spatializer, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_get_direction(pSpatializer: [*c]const ma_spatializer) ma_vec3f;
pub extern fn ma_spatializer_set_velocity(pSpatializer: [*c]ma_spatializer, x: f32, y: f32, z: f32) void;
pub extern fn ma_spatializer_get_velocity(pSpatializer: [*c]const ma_spatializer) ma_vec3f;
pub extern fn ma_spatializer_get_relative_position_and_direction(pSpatializer: [*c]const ma_spatializer, pListener: [*c]const ma_spatializer_listener, pRelativePos: [*c]ma_vec3f, pRelativeDir: [*c]ma_vec3f) void;
pub const ma_linear_resampler_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRateIn: ma_uint32,
    sampleRateOut: ma_uint32,
    lpfOrder: ma_uint32,
    lpfNyquistFactor: f64,
};
pub extern fn ma_linear_resampler_config_init(format: ma_format, channels: ma_uint32, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32) ma_linear_resampler_config;
const union_unnamed_44 = extern union {
    f32: [*c]f32,
    s16: [*c]ma_int16,
};
const union_unnamed_45 = extern union {
    f32: [*c]f32,
    s16: [*c]ma_int16,
};
pub const ma_linear_resampler = extern struct {
    config: ma_linear_resampler_config,
    inAdvanceInt: ma_uint32,
    inAdvanceFrac: ma_uint32,
    inTimeInt: ma_uint32,
    inTimeFrac: ma_uint32,
    x0: union_unnamed_44,
    x1: union_unnamed_45,
    lpf: ma_lpf,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_linear_resampler_get_heap_size(pConfig: [*c]const ma_linear_resampler_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_linear_resampler_init_preallocated(pConfig: [*c]const ma_linear_resampler_config, pHeap: ?*anyopaque, pResampler: [*c]ma_linear_resampler) ma_result;
pub extern fn ma_linear_resampler_init(pConfig: [*c]const ma_linear_resampler_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pResampler: [*c]ma_linear_resampler) ma_result;
pub extern fn ma_linear_resampler_uninit(pResampler: [*c]ma_linear_resampler, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_linear_resampler_process_pcm_frames(pResampler: [*c]ma_linear_resampler, pFramesIn: ?*const anyopaque, pFrameCountIn: [*c]ma_uint64, pFramesOut: ?*anyopaque, pFrameCountOut: [*c]ma_uint64) ma_result;
pub extern fn ma_linear_resampler_set_rate(pResampler: [*c]ma_linear_resampler, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32) ma_result;
pub extern fn ma_linear_resampler_set_rate_ratio(pResampler: [*c]ma_linear_resampler, ratioInOut: f32) ma_result;
pub extern fn ma_linear_resampler_get_input_latency(pResampler: [*c]const ma_linear_resampler) ma_uint64;
pub extern fn ma_linear_resampler_get_output_latency(pResampler: [*c]const ma_linear_resampler) ma_uint64;
pub extern fn ma_linear_resampler_get_required_input_frame_count(pResampler: [*c]const ma_linear_resampler, outputFrameCount: ma_uint64, pInputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_linear_resampler_get_expected_output_frame_count(pResampler: [*c]const ma_linear_resampler, inputFrameCount: ma_uint64, pOutputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_linear_resampler_reset(pResampler: [*c]ma_linear_resampler) ma_result;
pub const ma_resampling_backend = anyopaque;
pub const ma_resampling_backend_vtable = extern struct {
    onGetHeapSize: ?*const fn (?*anyopaque, [*c]const ma_resampler_config, [*c]usize) callconv(.C) ma_result,
    onInit: ?*const fn (?*anyopaque, [*c]const ma_resampler_config, ?*anyopaque, [*c]?*ma_resampling_backend) callconv(.C) ma_result,
    onUninit: ?*const fn (?*anyopaque, ?*ma_resampling_backend, [*c]const ma_allocation_callbacks) callconv(.C) void,
    onProcess: ?*const fn (?*anyopaque, ?*ma_resampling_backend, ?*const anyopaque, [*c]ma_uint64, ?*anyopaque, [*c]ma_uint64) callconv(.C) ma_result,
    onSetRate: ?*const fn (?*anyopaque, ?*ma_resampling_backend, ma_uint32, ma_uint32) callconv(.C) ma_result,
    onGetInputLatency: ?*const fn (?*anyopaque, ?*const ma_resampling_backend) callconv(.C) ma_uint64,
    onGetOutputLatency: ?*const fn (?*anyopaque, ?*const ma_resampling_backend) callconv(.C) ma_uint64,
    onGetRequiredInputFrameCount: ?*const fn (?*anyopaque, ?*const ma_resampling_backend, ma_uint64, [*c]ma_uint64) callconv(.C) ma_result,
    onGetExpectedOutputFrameCount: ?*const fn (?*anyopaque, ?*const ma_resampling_backend, ma_uint64, [*c]ma_uint64) callconv(.C) ma_result,
    onReset: ?*const fn (?*anyopaque, ?*ma_resampling_backend) callconv(.C) ma_result,
};
pub const ma_resample_algorithm_linear: c_int = 0;
pub const ma_resample_algorithm_custom: c_int = 1;
pub const ma_resample_algorithm = c_uint;
pub extern fn ma_resampler_config_init(format: ma_format, channels: ma_uint32, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32, algorithm: ma_resample_algorithm) ma_resampler_config;
const union_unnamed_46 = extern union {
    linear: ma_linear_resampler,
};
pub const ma_resampler = extern struct {
    pBackend: ?*ma_resampling_backend,
    pBackendVTable: [*c]ma_resampling_backend_vtable,
    pBackendUserData: ?*anyopaque,
    format: ma_format,
    channels: ma_uint32,
    sampleRateIn: ma_uint32,
    sampleRateOut: ma_uint32,
    state: union_unnamed_46,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_resampler_get_heap_size(pConfig: [*c]const ma_resampler_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_resampler_init_preallocated(pConfig: [*c]const ma_resampler_config, pHeap: ?*anyopaque, pResampler: [*c]ma_resampler) ma_result;
pub extern fn ma_resampler_init(pConfig: [*c]const ma_resampler_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pResampler: [*c]ma_resampler) ma_result;
pub extern fn ma_resampler_uninit(pResampler: [*c]ma_resampler, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_resampler_process_pcm_frames(pResampler: [*c]ma_resampler, pFramesIn: ?*const anyopaque, pFrameCountIn: [*c]ma_uint64, pFramesOut: ?*anyopaque, pFrameCountOut: [*c]ma_uint64) ma_result;
pub extern fn ma_resampler_set_rate(pResampler: [*c]ma_resampler, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32) ma_result;
pub extern fn ma_resampler_set_rate_ratio(pResampler: [*c]ma_resampler, ratio: f32) ma_result;
pub extern fn ma_resampler_get_input_latency(pResampler: [*c]const ma_resampler) ma_uint64;
pub extern fn ma_resampler_get_output_latency(pResampler: [*c]const ma_resampler) ma_uint64;
pub extern fn ma_resampler_get_required_input_frame_count(pResampler: [*c]const ma_resampler, outputFrameCount: ma_uint64, pInputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_resampler_get_expected_output_frame_count(pResampler: [*c]const ma_resampler, inputFrameCount: ma_uint64, pOutputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_resampler_reset(pResampler: [*c]ma_resampler) ma_result;
pub const ma_channel_conversion_path_unknown: c_int = 0;
pub const ma_channel_conversion_path_passthrough: c_int = 1;
pub const ma_channel_conversion_path_mono_out: c_int = 2;
pub const ma_channel_conversion_path_mono_in: c_int = 3;
pub const ma_channel_conversion_path_shuffle: c_int = 4;
pub const ma_channel_conversion_path_weights: c_int = 5;
pub const ma_channel_conversion_path = c_uint;
pub const ma_mono_expansion_mode_duplicate: c_int = 0;
pub const ma_mono_expansion_mode_average: c_int = 1;
pub const ma_mono_expansion_mode_stereo_only: c_int = 2;
pub const ma_mono_expansion_mode_default: c_int = 0;
pub const ma_mono_expansion_mode = c_uint;
pub const ma_channel_converter_config = extern struct {
    format: ma_format,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    pChannelMapIn: [*c]const ma_channel,
    pChannelMapOut: [*c]const ma_channel,
    mixingMode: ma_channel_mix_mode,
    ppWeights: [*c][*c]f32,
};
pub extern fn ma_channel_converter_config_init(format: ma_format, channelsIn: ma_uint32, pChannelMapIn: [*c]const ma_channel, channelsOut: ma_uint32, pChannelMapOut: [*c]const ma_channel, mixingMode: ma_channel_mix_mode) ma_channel_converter_config;
const union_unnamed_47 = extern union {
    f32: [*c][*c]f32,
    s16: [*c][*c]ma_int32,
};
pub const ma_channel_converter = extern struct {
    format: ma_format,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    mixingMode: ma_channel_mix_mode,
    conversionPath: ma_channel_conversion_path,
    pChannelMapIn: [*c]ma_channel,
    pChannelMapOut: [*c]ma_channel,
    pShuffleTable: [*c]ma_uint8,
    weights: union_unnamed_47,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_channel_converter_get_heap_size(pConfig: [*c]const ma_channel_converter_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_channel_converter_init_preallocated(pConfig: [*c]const ma_channel_converter_config, pHeap: ?*anyopaque, pConverter: [*c]ma_channel_converter) ma_result;
pub extern fn ma_channel_converter_init(pConfig: [*c]const ma_channel_converter_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pConverter: [*c]ma_channel_converter) ma_result;
pub extern fn ma_channel_converter_uninit(pConverter: [*c]ma_channel_converter, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_channel_converter_process_pcm_frames(pConverter: [*c]ma_channel_converter, pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64) ma_result;
pub extern fn ma_channel_converter_get_input_channel_map(pConverter: [*c]const ma_channel_converter, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_channel_converter_get_output_channel_map(pConverter: [*c]const ma_channel_converter, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub const ma_data_converter_config = extern struct {
    formatIn: ma_format,
    formatOut: ma_format,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    sampleRateIn: ma_uint32,
    sampleRateOut: ma_uint32,
    pChannelMapIn: [*c]ma_channel,
    pChannelMapOut: [*c]ma_channel,
    ditherMode: ma_dither_mode,
    channelMixMode: ma_channel_mix_mode,
    ppChannelWeights: [*c][*c]f32,
    allowDynamicSampleRate: ma_bool32,
    resampling: ma_resampler_config,
};
pub extern fn ma_data_converter_config_init_default() ma_data_converter_config;
pub extern fn ma_data_converter_config_init(formatIn: ma_format, formatOut: ma_format, channelsIn: ma_uint32, channelsOut: ma_uint32, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32) ma_data_converter_config;
pub const ma_data_converter_execution_path_passthrough: c_int = 0;
pub const ma_data_converter_execution_path_format_only: c_int = 1;
pub const ma_data_converter_execution_path_channels_only: c_int = 2;
pub const ma_data_converter_execution_path_resample_only: c_int = 3;
pub const ma_data_converter_execution_path_resample_first: c_int = 4;
pub const ma_data_converter_execution_path_channels_first: c_int = 5;
pub const ma_data_converter_execution_path = c_uint;
pub const ma_data_converter = extern struct {
    formatIn: ma_format,
    formatOut: ma_format,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    sampleRateIn: ma_uint32,
    sampleRateOut: ma_uint32,
    ditherMode: ma_dither_mode,
    executionPath: ma_data_converter_execution_path,
    channelConverter: ma_channel_converter,
    resampler: ma_resampler,
    hasPreFormatConversion: ma_bool8,
    hasPostFormatConversion: ma_bool8,
    hasChannelConverter: ma_bool8,
    hasResampler: ma_bool8,
    isPassthrough: ma_bool8,
    _ownsHeap: ma_bool8,
    _pHeap: ?*anyopaque,
};
pub extern fn ma_data_converter_get_heap_size(pConfig: [*c]const ma_data_converter_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_data_converter_init_preallocated(pConfig: [*c]const ma_data_converter_config, pHeap: ?*anyopaque, pConverter: [*c]ma_data_converter) ma_result;
pub extern fn ma_data_converter_init(pConfig: [*c]const ma_data_converter_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pConverter: [*c]ma_data_converter) ma_result;
pub extern fn ma_data_converter_uninit(pConverter: [*c]ma_data_converter, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_data_converter_process_pcm_frames(pConverter: [*c]ma_data_converter, pFramesIn: ?*const anyopaque, pFrameCountIn: [*c]ma_uint64, pFramesOut: ?*anyopaque, pFrameCountOut: [*c]ma_uint64) ma_result;
pub extern fn ma_data_converter_set_rate(pConverter: [*c]ma_data_converter, sampleRateIn: ma_uint32, sampleRateOut: ma_uint32) ma_result;
pub extern fn ma_data_converter_set_rate_ratio(pConverter: [*c]ma_data_converter, ratioInOut: f32) ma_result;
pub extern fn ma_data_converter_get_input_latency(pConverter: [*c]const ma_data_converter) ma_uint64;
pub extern fn ma_data_converter_get_output_latency(pConverter: [*c]const ma_data_converter) ma_uint64;
pub extern fn ma_data_converter_get_required_input_frame_count(pConverter: [*c]const ma_data_converter, outputFrameCount: ma_uint64, pInputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_data_converter_get_expected_output_frame_count(pConverter: [*c]const ma_data_converter, inputFrameCount: ma_uint64, pOutputFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_data_converter_get_input_channel_map(pConverter: [*c]const ma_data_converter, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_data_converter_get_output_channel_map(pConverter: [*c]const ma_data_converter, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_data_converter_reset(pConverter: [*c]ma_data_converter) ma_result;
pub extern fn ma_pcm_u8_to_s16(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_u8_to_s24(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_u8_to_s32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_u8_to_f32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s16_to_u8(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s16_to_s24(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s16_to_s32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s16_to_f32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s24_to_u8(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s24_to_s16(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s24_to_s32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s24_to_f32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s32_to_u8(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s32_to_s16(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s32_to_s24(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_s32_to_f32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_f32_to_u8(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_f32_to_s16(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_f32_to_s24(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_f32_to_s32(pOut: ?*anyopaque, pIn: ?*const anyopaque, count: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_pcm_convert(pOut: ?*anyopaque, formatOut: ma_format, pIn: ?*const anyopaque, formatIn: ma_format, sampleCount: ma_uint64, ditherMode: ma_dither_mode) void;
pub extern fn ma_convert_pcm_frames_format(pOut: ?*anyopaque, formatOut: ma_format, pIn: ?*const anyopaque, formatIn: ma_format, frameCount: ma_uint64, channels: ma_uint32, ditherMode: ma_dither_mode) void;
pub extern fn ma_deinterleave_pcm_frames(format: ma_format, channels: ma_uint32, frameCount: ma_uint64, pInterleavedPCMFrames: ?*const anyopaque, ppDeinterleavedPCMFrames: [*c]?*anyopaque) void;
pub extern fn ma_interleave_pcm_frames(format: ma_format, channels: ma_uint32, frameCount: ma_uint64, ppDeinterleavedPCMFrames: [*c]?*const anyopaque, pInterleavedPCMFrames: ?*anyopaque) void;
pub extern fn ma_channel_map_get_channel(pChannelMap: [*c]const ma_channel, channelCount: ma_uint32, channelIndex: ma_uint32) ma_channel;
pub extern fn ma_channel_map_init_blank(pChannelMap: [*c]ma_channel, channels: ma_uint32) void;
pub extern fn ma_channel_map_init_standard(standardChannelMap: ma_standard_channel_map, pChannelMap: [*c]ma_channel, channelMapCap: usize, channels: ma_uint32) void;
pub extern fn ma_channel_map_copy(pOut: [*c]ma_channel, pIn: [*c]const ma_channel, channels: ma_uint32) void;
pub extern fn ma_channel_map_copy_or_default(pOut: [*c]ma_channel, channelMapCapOut: usize, pIn: [*c]const ma_channel, channels: ma_uint32) void;
pub extern fn ma_channel_map_is_valid(pChannelMap: [*c]const ma_channel, channels: ma_uint32) ma_bool32;
pub extern fn ma_channel_map_is_equal(pChannelMapA: [*c]const ma_channel, pChannelMapB: [*c]const ma_channel, channels: ma_uint32) ma_bool32;
pub extern fn ma_channel_map_is_blank(pChannelMap: [*c]const ma_channel, channels: ma_uint32) ma_bool32;
pub extern fn ma_channel_map_contains_channel_position(channels: ma_uint32, pChannelMap: [*c]const ma_channel, channelPosition: ma_channel) ma_bool32;
pub extern fn ma_convert_frames(pOut: ?*anyopaque, frameCountOut: ma_uint64, formatOut: ma_format, channelsOut: ma_uint32, sampleRateOut: ma_uint32, pIn: ?*const anyopaque, frameCountIn: ma_uint64, formatIn: ma_format, channelsIn: ma_uint32, sampleRateIn: ma_uint32) ma_uint64;
pub extern fn ma_convert_frames_ex(pOut: ?*anyopaque, frameCountOut: ma_uint64, pIn: ?*const anyopaque, frameCountIn: ma_uint64, pConfig: [*c]const ma_data_converter_config) ma_uint64;
pub const ma_rb = extern struct {
    pBuffer: ?*anyopaque,
    subbufferSizeInBytes: ma_uint32,
    subbufferCount: ma_uint32,
    subbufferStrideInBytes: ma_uint32,
    encodedReadOffset: ma_uint32 align(4),
    encodedWriteOffset: ma_uint32 align(4),
    ownsBuffer: ma_bool8,
    clearOnWriteAcquire: ma_bool8,
    allocationCallbacks: ma_allocation_callbacks,
};
pub extern fn ma_rb_init_ex(subbufferSizeInBytes: usize, subbufferCount: usize, subbufferStrideInBytes: usize, pOptionalPreallocatedBuffer: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pRB: [*c]ma_rb) ma_result;
pub extern fn ma_rb_init(bufferSizeInBytes: usize, pOptionalPreallocatedBuffer: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pRB: [*c]ma_rb) ma_result;
pub extern fn ma_rb_uninit(pRB: [*c]ma_rb) void;
pub extern fn ma_rb_reset(pRB: [*c]ma_rb) void;
pub extern fn ma_rb_acquire_read(pRB: [*c]ma_rb, pSizeInBytes: [*c]usize, ppBufferOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_rb_commit_read(pRB: [*c]ma_rb, sizeInBytes: usize) ma_result;
pub extern fn ma_rb_acquire_write(pRB: [*c]ma_rb, pSizeInBytes: [*c]usize, ppBufferOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_rb_commit_write(pRB: [*c]ma_rb, sizeInBytes: usize) ma_result;
pub extern fn ma_rb_seek_read(pRB: [*c]ma_rb, offsetInBytes: usize) ma_result;
pub extern fn ma_rb_seek_write(pRB: [*c]ma_rb, offsetInBytes: usize) ma_result;
pub extern fn ma_rb_pointer_distance(pRB: [*c]ma_rb) ma_int32;
pub extern fn ma_rb_available_read(pRB: [*c]ma_rb) ma_uint32;
pub extern fn ma_rb_available_write(pRB: [*c]ma_rb) ma_uint32;
pub extern fn ma_rb_get_subbuffer_size(pRB: [*c]ma_rb) usize;
pub extern fn ma_rb_get_subbuffer_stride(pRB: [*c]ma_rb) usize;
pub extern fn ma_rb_get_subbuffer_offset(pRB: [*c]ma_rb, subbufferIndex: usize) usize;
pub extern fn ma_rb_get_subbuffer_ptr(pRB: [*c]ma_rb, subbufferIndex: usize, pBuffer: ?*anyopaque) ?*anyopaque;
pub const ma_pcm_rb = extern struct {
    rb: ma_rb,
    format: ma_format,
    channels: ma_uint32,
};
pub extern fn ma_pcm_rb_init_ex(format: ma_format, channels: ma_uint32, subbufferSizeInFrames: ma_uint32, subbufferCount: ma_uint32, subbufferStrideInFrames: ma_uint32, pOptionalPreallocatedBuffer: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pRB: [*c]ma_pcm_rb) ma_result;
pub extern fn ma_pcm_rb_init(format: ma_format, channels: ma_uint32, bufferSizeInFrames: ma_uint32, pOptionalPreallocatedBuffer: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pRB: [*c]ma_pcm_rb) ma_result;
pub extern fn ma_pcm_rb_uninit(pRB: [*c]ma_pcm_rb) void;
pub extern fn ma_pcm_rb_reset(pRB: [*c]ma_pcm_rb) void;
pub extern fn ma_pcm_rb_acquire_read(pRB: [*c]ma_pcm_rb, pSizeInFrames: [*c]ma_uint32, ppBufferOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_pcm_rb_commit_read(pRB: [*c]ma_pcm_rb, sizeInFrames: ma_uint32) ma_result;
pub extern fn ma_pcm_rb_acquire_write(pRB: [*c]ma_pcm_rb, pSizeInFrames: [*c]ma_uint32, ppBufferOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_pcm_rb_commit_write(pRB: [*c]ma_pcm_rb, sizeInFrames: ma_uint32) ma_result;
pub extern fn ma_pcm_rb_seek_read(pRB: [*c]ma_pcm_rb, offsetInFrames: ma_uint32) ma_result;
pub extern fn ma_pcm_rb_seek_write(pRB: [*c]ma_pcm_rb, offsetInFrames: ma_uint32) ma_result;
pub extern fn ma_pcm_rb_pointer_distance(pRB: [*c]ma_pcm_rb) ma_int32;
pub extern fn ma_pcm_rb_available_read(pRB: [*c]ma_pcm_rb) ma_uint32;
pub extern fn ma_pcm_rb_available_write(pRB: [*c]ma_pcm_rb) ma_uint32;
pub extern fn ma_pcm_rb_get_subbuffer_size(pRB: [*c]ma_pcm_rb) ma_uint32;
pub extern fn ma_pcm_rb_get_subbuffer_stride(pRB: [*c]ma_pcm_rb) ma_uint32;
pub extern fn ma_pcm_rb_get_subbuffer_offset(pRB: [*c]ma_pcm_rb, subbufferIndex: ma_uint32) ma_uint32;
pub extern fn ma_pcm_rb_get_subbuffer_ptr(pRB: [*c]ma_pcm_rb, subbufferIndex: ma_uint32, pBuffer: ?*anyopaque) ?*anyopaque;
pub const ma_duplex_rb = extern struct {
    rb: ma_pcm_rb,
};
pub extern fn ma_duplex_rb_init(captureFormat: ma_format, captureChannels: ma_uint32, sampleRate: ma_uint32, captureInternalSampleRate: ma_uint32, captureInternalPeriodSizeInFrames: ma_uint32, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pRB: [*c]ma_duplex_rb) ma_result;
pub extern fn ma_duplex_rb_uninit(pRB: [*c]ma_duplex_rb) ma_result;
pub extern fn ma_result_description(result: ma_result) [*c]const u8;
pub extern fn ma_malloc(sz: usize, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ?*anyopaque;
pub extern fn ma_calloc(sz: usize, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ?*anyopaque;
pub extern fn ma_realloc(p: ?*anyopaque, sz: usize, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ?*anyopaque;
pub extern fn ma_free(p: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_aligned_malloc(sz: usize, alignment: usize, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ?*anyopaque;
pub extern fn ma_aligned_free(p: ?*anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_get_format_name(format: ma_format) [*c]const u8;
pub extern fn ma_blend_f32(pOut: [*c]f32, pInA: [*c]f32, pInB: [*c]f32, factor: f32, channels: ma_uint32) void;
pub extern fn ma_get_bytes_per_sample(format: ma_format) ma_uint32;
pub inline fn ma_get_bytes_per_frame(arg_format: ma_format, arg_channels: ma_uint32) ma_uint32 {
    var format = arg_format;
    var channels = arg_channels;
    return ma_get_bytes_per_sample(format) *% channels;
}
pub extern fn ma_log_level_to_string(logLevel: ma_uint32) [*c]const u8;
pub extern fn ma_spinlock_lock(pSpinlock: [*c]volatile ma_spinlock) ma_result;
pub extern fn ma_spinlock_lock_noyield(pSpinlock: [*c]volatile ma_spinlock) ma_result;
pub extern fn ma_spinlock_unlock(pSpinlock: [*c]volatile ma_spinlock) ma_result;
pub extern fn ma_mutex_init(pMutex: [*c]ma_mutex) ma_result;
pub extern fn ma_mutex_uninit(pMutex: [*c]ma_mutex) void;
pub extern fn ma_mutex_lock(pMutex: [*c]ma_mutex) void;
pub extern fn ma_mutex_unlock(pMutex: [*c]ma_mutex) void;
pub extern fn ma_event_init(pEvent: [*c]ma_event) ma_result;
pub extern fn ma_event_uninit(pEvent: [*c]ma_event) void;
pub extern fn ma_event_wait(pEvent: [*c]ma_event) ma_result;
pub extern fn ma_event_signal(pEvent: [*c]ma_event) ma_result;
pub const ma_fence = extern struct {
    e: ma_event,
    counter: ma_uint32,
};
pub extern fn ma_fence_init(pFence: [*c]ma_fence) ma_result;
pub extern fn ma_fence_uninit(pFence: [*c]ma_fence) void;
pub extern fn ma_fence_acquire(pFence: [*c]ma_fence) ma_result;
pub extern fn ma_fence_release(pFence: [*c]ma_fence) ma_result;
pub extern fn ma_fence_wait(pFence: [*c]ma_fence) ma_result;
pub const ma_async_notification = anyopaque;
pub const ma_async_notification_callbacks = extern struct {
    onSignal: ?*const fn (?*ma_async_notification) callconv(.C) void,
};
pub extern fn ma_async_notification_signal(pNotification: ?*ma_async_notification) ma_result;
pub const ma_async_notification_poll = extern struct {
    cb: ma_async_notification_callbacks,
    signalled: ma_bool32,
};
pub extern fn ma_async_notification_poll_init(pNotificationPoll: [*c]ma_async_notification_poll) ma_result;
pub extern fn ma_async_notification_poll_is_signalled(pNotificationPoll: [*c]const ma_async_notification_poll) ma_bool32;
pub const ma_async_notification_event = extern struct {
    cb: ma_async_notification_callbacks,
    e: ma_event,
};
pub extern fn ma_async_notification_event_init(pNotificationEvent: [*c]ma_async_notification_event) ma_result;
pub extern fn ma_async_notification_event_uninit(pNotificationEvent: [*c]ma_async_notification_event) ma_result;
pub extern fn ma_async_notification_event_wait(pNotificationEvent: [*c]ma_async_notification_event) ma_result;
pub extern fn ma_async_notification_event_signal(pNotificationEvent: [*c]ma_async_notification_event) ma_result;
pub const ma_slot_allocator_config = extern struct {
    capacity: ma_uint32,
};
pub extern fn ma_slot_allocator_config_init(capacity: ma_uint32) ma_slot_allocator_config;
pub const ma_slot_allocator_group = extern struct {
    bitfield: ma_uint32 align(4),
};
pub const ma_slot_allocator = extern struct {
    pGroups: [*c]ma_slot_allocator_group,
    pSlots: [*c]ma_uint32,
    count: ma_uint32,
    capacity: ma_uint32,
    _ownsHeap: ma_bool32,
    _pHeap: ?*anyopaque,
};
pub extern fn ma_slot_allocator_get_heap_size(pConfig: [*c]const ma_slot_allocator_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_slot_allocator_init_preallocated(pConfig: [*c]const ma_slot_allocator_config, pHeap: ?*anyopaque, pAllocator: [*c]ma_slot_allocator) ma_result;
pub extern fn ma_slot_allocator_init(pConfig: [*c]const ma_slot_allocator_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pAllocator: [*c]ma_slot_allocator) ma_result;
pub extern fn ma_slot_allocator_uninit(pAllocator: [*c]ma_slot_allocator, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_slot_allocator_alloc(pAllocator: [*c]ma_slot_allocator, pSlot: [*c]ma_uint64) ma_result;
pub extern fn ma_slot_allocator_free(pAllocator: [*c]ma_slot_allocator, slot: ma_uint64) ma_result;
const struct_unnamed_49 = extern struct {
    code: ma_uint16,
    slot: ma_uint16,
    refcount: ma_uint32,
};
const union_unnamed_48 = extern union {
    breakup: struct_unnamed_49,
    allocation: ma_uint64,
};
pub const ma_job = struct_ma_job;
pub const ma_job_proc = ?*const fn ([*c]ma_job) callconv(.C) ma_result;
const struct_unnamed_51 = extern struct {
    proc: ma_job_proc,
    data0: ma_uintptr,
    data1: ma_uintptr,
};
const struct_unnamed_53 = extern struct {
    pResourceManager: ?*anyopaque,
    pDataBufferNode: ?*anyopaque,
    pFilePath: [*c]u8,
    pFilePathW: [*c]wchar_t,
    flags: ma_uint32,
    pInitNotification: ?*ma_async_notification,
    pDoneNotification: ?*ma_async_notification,
    pInitFence: [*c]ma_fence,
    pDoneFence: [*c]ma_fence,
};
const struct_unnamed_54 = extern struct {
    pResourceManager: ?*anyopaque,
    pDataBufferNode: ?*anyopaque,
    pDoneNotification: ?*ma_async_notification,
    pDoneFence: [*c]ma_fence,
};
const struct_unnamed_55 = extern struct {
    pResourceManager: ?*anyopaque,
    pDataBufferNode: ?*anyopaque,
    pDecoder: ?*anyopaque,
    pDoneNotification: ?*ma_async_notification,
    pDoneFence: [*c]ma_fence,
};
const struct_unnamed_56 = extern struct {
    pDataBuffer: ?*anyopaque,
    pInitNotification: ?*ma_async_notification,
    pDoneNotification: ?*ma_async_notification,
    pInitFence: [*c]ma_fence,
    pDoneFence: [*c]ma_fence,
    rangeBegInPCMFrames: ma_uint64,
    rangeEndInPCMFrames: ma_uint64,
    loopPointBegInPCMFrames: ma_uint64,
    loopPointEndInPCMFrames: ma_uint64,
    isLooping: ma_uint32,
};
const struct_unnamed_57 = extern struct {
    pDataBuffer: ?*anyopaque,
    pDoneNotification: ?*ma_async_notification,
    pDoneFence: [*c]ma_fence,
};
const struct_unnamed_58 = extern struct {
    pDataStream: ?*anyopaque,
    pFilePath: [*c]u8,
    pFilePathW: [*c]wchar_t,
    initialSeekPoint: ma_uint64,
    pInitNotification: ?*ma_async_notification,
    pInitFence: [*c]ma_fence,
};
const struct_unnamed_59 = extern struct {
    pDataStream: ?*anyopaque,
    pDoneNotification: ?*ma_async_notification,
    pDoneFence: [*c]ma_fence,
};
const struct_unnamed_60 = extern struct {
    pDataStream: ?*anyopaque,
    pageIndex: ma_uint32,
};
const struct_unnamed_61 = extern struct {
    pDataStream: ?*anyopaque,
    frameIndex: ma_uint64,
};
const union_unnamed_52 = extern union {
    loadDataBufferNode: struct_unnamed_53,
    freeDataBufferNode: struct_unnamed_54,
    pageDataBufferNode: struct_unnamed_55,
    loadDataBuffer: struct_unnamed_56,
    freeDataBuffer: struct_unnamed_57,
    loadDataStream: struct_unnamed_58,
    freeDataStream: struct_unnamed_59,
    pageDataStream: struct_unnamed_60,
    seekDataStream: struct_unnamed_61,
};
const struct_unnamed_64 = extern struct {
    pDevice: ?*anyopaque,
    deviceType: ma_uint32,
};
const union_unnamed_63 = extern union {
    reroute: struct_unnamed_64,
};
const union_unnamed_62 = extern union {
    aaudio: union_unnamed_63,
};
const union_unnamed_50 = extern union {
    custom: struct_unnamed_51,
    resourceManager: union_unnamed_52,
    device: union_unnamed_62,
};
pub const struct_ma_job = extern struct {
    toc: union_unnamed_48,
    next: ma_uint64 align(8),
    order: ma_uint32,
    data: union_unnamed_50,
};
pub const MA_JOB_TYPE_QUIT: c_int = 0;
pub const MA_JOB_TYPE_CUSTOM: c_int = 1;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_LOAD_DATA_BUFFER_NODE: c_int = 2;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_FREE_DATA_BUFFER_NODE: c_int = 3;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_PAGE_DATA_BUFFER_NODE: c_int = 4;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_LOAD_DATA_BUFFER: c_int = 5;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_FREE_DATA_BUFFER: c_int = 6;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_LOAD_DATA_STREAM: c_int = 7;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_FREE_DATA_STREAM: c_int = 8;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_PAGE_DATA_STREAM: c_int = 9;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_SEEK_DATA_STREAM: c_int = 10;
pub const MA_JOB_TYPE_DEVICE_AAUDIO_REROUTE: c_int = 11;
pub const MA_JOB_TYPE_COUNT: c_int = 12;
pub const ma_job_type = c_uint;
pub extern fn ma_job_init(code: ma_uint16) ma_job;
pub extern fn ma_job_process(pJob: [*c]ma_job) ma_result;
pub const MA_JOB_QUEUE_FLAG_NON_BLOCKING: c_int = 1;
pub const ma_job_queue_flags = c_uint;
pub const ma_job_queue_config = extern struct {
    flags: ma_uint32,
    capacity: ma_uint32,
};
pub extern fn ma_job_queue_config_init(flags: ma_uint32, capacity: ma_uint32) ma_job_queue_config;
pub const ma_job_queue = extern struct {
    flags: ma_uint32,
    capacity: ma_uint32,
    head: ma_uint64 align(8),
    tail: ma_uint64 align(8),
    sem: ma_semaphore,
    allocator: ma_slot_allocator,
    pJobs: [*c]ma_job,
    lock: ma_spinlock,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_job_queue_get_heap_size(pConfig: [*c]const ma_job_queue_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_job_queue_init_preallocated(pConfig: [*c]const ma_job_queue_config, pHeap: ?*anyopaque, pQueue: [*c]ma_job_queue) ma_result;
pub extern fn ma_job_queue_init(pConfig: [*c]const ma_job_queue_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pQueue: [*c]ma_job_queue) ma_result;
pub extern fn ma_job_queue_uninit(pQueue: [*c]ma_job_queue, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_job_queue_post(pQueue: [*c]ma_job_queue, pJob: [*c]const ma_job) ma_result;
pub extern fn ma_job_queue_next(pQueue: [*c]ma_job_queue, pJob: [*c]ma_job) ma_result;
pub const ma_device_state_uninitialized: c_int = 0;
pub const ma_device_state_stopped: c_int = 1;
pub const ma_device_state_started: c_int = 2;
pub const ma_device_state_starting: c_int = 3;
pub const ma_device_state_stopping: c_int = 4;
pub const ma_device_state = c_uint;
pub const ma_backend_wasapi: c_int = 0;
pub const ma_backend_dsound: c_int = 1;
pub const ma_backend_winmm: c_int = 2;
pub const ma_backend_coreaudio: c_int = 3;
pub const ma_backend_sndio: c_int = 4;
pub const ma_backend_audio4: c_int = 5;
pub const ma_backend_oss: c_int = 6;
pub const ma_backend_pulseaudio: c_int = 7;
pub const ma_backend_alsa: c_int = 8;
pub const ma_backend_jack: c_int = 9;
pub const ma_backend_aaudio: c_int = 10;
pub const ma_backend_opensl: c_int = 11;
pub const ma_backend_webaudio: c_int = 12;
pub const ma_backend_custom: c_int = 13;
pub const ma_backend_null: c_int = 14;
pub const ma_backend = c_uint;
pub const ma_device_job_thread_config = extern struct {
    noThread: ma_bool32,
    jobQueueCapacity: ma_uint32,
    jobQueueFlags: ma_uint32,
};
pub extern fn ma_device_job_thread_config_init() ma_device_job_thread_config;
pub const ma_device_job_thread = extern struct {
    thread: ma_thread,
    jobQueue: ma_job_queue,
    _hasThread: ma_bool32,
};
pub extern fn ma_device_job_thread_init(pConfig: [*c]const ma_device_job_thread_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pJobThread: [*c]ma_device_job_thread) ma_result;
pub extern fn ma_device_job_thread_uninit(pJobThread: [*c]ma_device_job_thread, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_device_job_thread_post(pJobThread: [*c]ma_device_job_thread, pJob: [*c]const ma_job) ma_result;
pub extern fn ma_device_job_thread_next(pJobThread: [*c]ma_device_job_thread, pJob: [*c]ma_job) ma_result;
pub const ma_device_notification_type_started: c_int = 0;
pub const ma_device_notification_type_stopped: c_int = 1;
pub const ma_device_notification_type_rerouted: c_int = 2;
pub const ma_device_notification_type_interruption_began: c_int = 3;
pub const ma_device_notification_type_interruption_ended: c_int = 4;
pub const ma_device_notification_type = c_uint;
const struct_unnamed_66 = extern struct {
    _unused: c_int,
};
const struct_unnamed_67 = extern struct {
    _unused: c_int,
};
const struct_unnamed_68 = extern struct {
    _unused: c_int,
};
const struct_unnamed_69 = extern struct {
    _unused: c_int,
};
const union_unnamed_65 = extern union {
    started: struct_unnamed_66,
    stopped: struct_unnamed_67,
    rerouted: struct_unnamed_68,
    interruption: struct_unnamed_69,
};
pub const ma_device_notification = extern struct {
    pDevice: [*c]ma_device,
    type: ma_device_notification_type,
    data: union_unnamed_65,
};
pub const ma_device_type_playback: c_int = 1;
pub const ma_device_type_capture: c_int = 2;
pub const ma_device_type_duplex: c_int = 3;
pub const ma_device_type_loopback: c_int = 4;
pub const ma_device_type = c_uint;
pub const ma_share_mode_shared: c_int = 0;
pub const ma_share_mode_exclusive: c_int = 1;
pub const ma_share_mode = c_uint;
pub const ma_ios_session_category_default: c_int = 0;
pub const ma_ios_session_category_none: c_int = 1;
pub const ma_ios_session_category_ambient: c_int = 2;
pub const ma_ios_session_category_solo_ambient: c_int = 3;
pub const ma_ios_session_category_playback: c_int = 4;
pub const ma_ios_session_category_record: c_int = 5;
pub const ma_ios_session_category_play_and_record: c_int = 6;
pub const ma_ios_session_category_multi_route: c_int = 7;
pub const ma_ios_session_category = c_uint;
pub const ma_ios_session_category_option_mix_with_others: c_int = 1;
pub const ma_ios_session_category_option_duck_others: c_int = 2;
pub const ma_ios_session_category_option_allow_bluetooth: c_int = 4;
pub const ma_ios_session_category_option_default_to_speaker: c_int = 8;
pub const ma_ios_session_category_option_interrupt_spoken_audio_and_mix_with_others: c_int = 17;
pub const ma_ios_session_category_option_allow_bluetooth_a2dp: c_int = 32;
pub const ma_ios_session_category_option_allow_air_play: c_int = 64;
pub const ma_ios_session_category_option = c_uint;
pub const ma_opensl_stream_type_default: c_int = 0;
pub const ma_opensl_stream_type_voice: c_int = 1;
pub const ma_opensl_stream_type_system: c_int = 2;
pub const ma_opensl_stream_type_ring: c_int = 3;
pub const ma_opensl_stream_type_media: c_int = 4;
pub const ma_opensl_stream_type_alarm: c_int = 5;
pub const ma_opensl_stream_type_notification: c_int = 6;
pub const ma_opensl_stream_type = c_uint;
pub const ma_opensl_recording_preset_default: c_int = 0;
pub const ma_opensl_recording_preset_generic: c_int = 1;
pub const ma_opensl_recording_preset_camcorder: c_int = 2;
pub const ma_opensl_recording_preset_voice_recognition: c_int = 3;
pub const ma_opensl_recording_preset_voice_communication: c_int = 4;
pub const ma_opensl_recording_preset_voice_unprocessed: c_int = 5;
pub const ma_opensl_recording_preset = c_uint;
pub const ma_aaudio_usage_default: c_int = 0;
pub const ma_aaudio_usage_announcement: c_int = 1;
pub const ma_aaudio_usage_emergency: c_int = 2;
pub const ma_aaudio_usage_safety: c_int = 3;
pub const ma_aaudio_usage_vehicle_status: c_int = 4;
pub const ma_aaudio_usage_alarm: c_int = 5;
pub const ma_aaudio_usage_assistance_accessibility: c_int = 6;
pub const ma_aaudio_usage_assistance_navigation_guidance: c_int = 7;
pub const ma_aaudio_usage_assistance_sonification: c_int = 8;
pub const ma_aaudio_usage_assitant: c_int = 9;
pub const ma_aaudio_usage_game: c_int = 10;
pub const ma_aaudio_usage_media: c_int = 11;
pub const ma_aaudio_usage_notification: c_int = 12;
pub const ma_aaudio_usage_notification_event: c_int = 13;
pub const ma_aaudio_usage_notification_ringtone: c_int = 14;
pub const ma_aaudio_usage_voice_communication: c_int = 15;
pub const ma_aaudio_usage_voice_communication_signalling: c_int = 16;
pub const ma_aaudio_usage = c_uint;
pub const ma_aaudio_content_type_default: c_int = 0;
pub const ma_aaudio_content_type_movie: c_int = 1;
pub const ma_aaudio_content_type_music: c_int = 2;
pub const ma_aaudio_content_type_sonification: c_int = 3;
pub const ma_aaudio_content_type_speech: c_int = 4;
pub const ma_aaudio_content_type = c_uint;
pub const ma_aaudio_input_preset_default: c_int = 0;
pub const ma_aaudio_input_preset_generic: c_int = 1;
pub const ma_aaudio_input_preset_camcorder: c_int = 2;
pub const ma_aaudio_input_preset_unprocessed: c_int = 3;
pub const ma_aaudio_input_preset_voice_recognition: c_int = 4;
pub const ma_aaudio_input_preset_voice_communication: c_int = 5;
pub const ma_aaudio_input_preset_voice_performance: c_int = 6;
pub const ma_aaudio_input_preset = c_uint;
pub const ma_timer = extern union {
    counter: ma_int64,
    counterD: f64,
};
const union_unnamed_70 = extern union {
    i: c_int,
    s: [256]u8,
    p: ?*anyopaque,
};
pub const ma_device_id = extern union {
    wasapi: [64]wchar_t,
    dsound: [16]ma_uint8,
    winmm: ma_uint32,
    alsa: [256]u8,
    pulse: [256]u8,
    jack: c_int,
    coreaudio: [256]u8,
    sndio: [256]u8,
    audio4: [256]u8,
    oss: [64]u8,
    aaudio: ma_int32,
    opensl: ma_uint32,
    webaudio: [32]u8,
    custom: union_unnamed_70,
    nullbackend: c_int,
};
const struct_unnamed_71 = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    flags: ma_uint32,
};
pub const ma_device_info = extern struct {
    id: ma_device_id,
    name: [256]u8,
    isDefault: ma_bool32,
    nativeDataFormatCount: ma_uint32,
    nativeDataFormats: [64]struct_unnamed_71,
};
pub const ma_device_descriptor = extern struct {
    pDeviceID: [*c]const ma_device_id,
    shareMode: ma_share_mode,
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    channelMap: [254]ma_channel,
    periodSizeInFrames: ma_uint32,
    periodSizeInMilliseconds: ma_uint32,
    periodCount: ma_uint32,
};
const struct_unnamed_73 = extern struct {
    _unused: c_int,
};
const struct_unnamed_74 = extern struct {
    deviceType: ma_device_type,
    pAudioClient: ?*anyopaque,
    ppAudioClientService: [*c]?*anyopaque,
    pResult: [*c]ma_result,
};
const struct_unnamed_75 = extern struct {
    pDevice: [*c]ma_device,
    deviceType: ma_device_type,
};
const union_unnamed_72 = extern union {
    quit: struct_unnamed_73,
    createAudioClient: struct_unnamed_74,
    releaseAudioClient: struct_unnamed_75,
};
pub const ma_context_command__wasapi = extern struct {
    code: c_int,
    pEvent: [*c]ma_event,
    data: union_unnamed_72,
};
pub extern fn ma_context_config_init() ma_context_config;
pub extern fn ma_context_init(backends: [*c]const ma_backend, backendCount: ma_uint32, pConfig: [*c]const ma_context_config, pContext: [*c]ma_context) ma_result;
pub extern fn ma_context_uninit(pContext: [*c]ma_context) ma_result;
pub extern fn ma_context_sizeof() usize;
pub extern fn ma_context_get_log(pContext: [*c]ma_context) [*c]ma_log;
pub extern fn ma_context_enumerate_devices(pContext: [*c]ma_context, callback: ma_enum_devices_callback_proc, pUserData: ?*anyopaque) ma_result;
pub extern fn ma_context_get_devices(pContext: [*c]ma_context, ppPlaybackDeviceInfos: [*c][*c]ma_device_info, pPlaybackDeviceCount: [*c]ma_uint32, ppCaptureDeviceInfos: [*c][*c]ma_device_info, pCaptureDeviceCount: [*c]ma_uint32) ma_result;
pub extern fn ma_context_get_device_info(pContext: [*c]ma_context, deviceType: ma_device_type, pDeviceID: [*c]const ma_device_id, pDeviceInfo: [*c]ma_device_info) ma_result;
pub extern fn ma_context_is_loopback_supported(pContext: [*c]ma_context) ma_bool32;
pub extern fn ma_device_config_init(deviceType: ma_device_type) ma_device_config;
pub extern fn ma_device_init(pContext: [*c]ma_context, pConfig: [*c]const ma_device_config, pDevice: [*c]ma_device) ma_result;
pub extern fn ma_device_init_ex(backends: [*c]const ma_backend, backendCount: ma_uint32, pContextConfig: [*c]const ma_context_config, pConfig: [*c]const ma_device_config, pDevice: [*c]ma_device) ma_result;
pub extern fn ma_device_uninit(pDevice: [*c]ma_device) void;
pub extern fn ma_device_get_context(pDevice: [*c]ma_device) [*c]ma_context;
pub extern fn ma_device_get_log(pDevice: [*c]ma_device) [*c]ma_log;
pub extern fn ma_device_get_info(pDevice: [*c]ma_device, @"type": ma_device_type, pDeviceInfo: [*c]ma_device_info) ma_result;
pub extern fn ma_device_get_name(pDevice: [*c]ma_device, @"type": ma_device_type, pName: [*c]u8, nameCap: usize, pLengthNotIncludingNullTerminator: [*c]usize) ma_result;
pub extern fn ma_device_start(pDevice: [*c]ma_device) ma_result;
pub extern fn ma_device_stop(pDevice: [*c]ma_device) ma_result;
pub extern fn ma_device_is_started(pDevice: [*c]const ma_device) ma_bool32;
pub extern fn ma_device_get_state(pDevice: [*c]const ma_device) ma_device_state;
pub extern fn ma_device_post_init(pDevice: [*c]ma_device, deviceType: ma_device_type, pPlaybackDescriptor: [*c]const ma_device_descriptor, pCaptureDescriptor: [*c]const ma_device_descriptor) ma_result;
pub extern fn ma_device_set_master_volume(pDevice: [*c]ma_device, volume: f32) ma_result;
pub extern fn ma_device_get_master_volume(pDevice: [*c]ma_device, pVolume: [*c]f32) ma_result;
pub extern fn ma_device_set_master_volume_db(pDevice: [*c]ma_device, gainDB: f32) ma_result;
pub extern fn ma_device_get_master_volume_db(pDevice: [*c]ma_device, pGainDB: [*c]f32) ma_result;
pub extern fn ma_device_handle_backend_data_callback(pDevice: [*c]ma_device, pOutput: ?*anyopaque, pInput: ?*const anyopaque, frameCount: ma_uint32) ma_result;
pub extern fn ma_calculate_buffer_size_in_frames_from_descriptor(pDescriptor: [*c]const ma_device_descriptor, nativeSampleRate: ma_uint32, performanceProfile: ma_performance_profile) ma_uint32;
pub extern fn ma_get_backend_name(backend: ma_backend) [*c]const u8;
pub extern fn ma_is_backend_enabled(backend: ma_backend) ma_bool32;
pub extern fn ma_get_enabled_backends(pBackends: [*c]ma_backend, backendCap: usize, pBackendCount: [*c]usize) ma_result;
pub extern fn ma_is_loopback_supported(backend: ma_backend) ma_bool32;
pub extern fn ma_calculate_buffer_size_in_milliseconds_from_frames(bufferSizeInFrames: ma_uint32, sampleRate: ma_uint32) ma_uint32;
pub extern fn ma_calculate_buffer_size_in_frames_from_milliseconds(bufferSizeInMilliseconds: ma_uint32, sampleRate: ma_uint32) ma_uint32;
pub extern fn ma_copy_pcm_frames(dst: ?*anyopaque, src: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32) void;
pub extern fn ma_silence_pcm_frames(p: ?*anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32) void;
pub extern fn ma_offset_pcm_frames_ptr(p: ?*anyopaque, offsetInFrames: ma_uint64, format: ma_format, channels: ma_uint32) ?*anyopaque;
pub extern fn ma_offset_pcm_frames_const_ptr(p: ?*const anyopaque, offsetInFrames: ma_uint64, format: ma_format, channels: ma_uint32) ?*const anyopaque;
pub inline fn ma_offset_pcm_frames_ptr_f32(arg_p: [*c]f32, arg_offsetInFrames: ma_uint64, arg_channels: ma_uint32) [*c]f32 {
    var p = arg_p;
    var offsetInFrames = arg_offsetInFrames;
    var channels = arg_channels;
    return @ptrCast([*c]f32, @alignCast(@import("std").meta.alignment([*c]f32), ma_offset_pcm_frames_ptr(@ptrCast(?*anyopaque, p), offsetInFrames, @bitCast(c_uint, ma_format_f32), channels)));
}
pub inline fn ma_offset_pcm_frames_const_ptr_f32(arg_p: [*c]const f32, arg_offsetInFrames: ma_uint64, arg_channels: ma_uint32) [*c]const f32 {
    var p = arg_p;
    var offsetInFrames = arg_offsetInFrames;
    var channels = arg_channels;
    return @ptrCast([*c]const f32, @alignCast(@import("std").meta.alignment([*c]const f32), ma_offset_pcm_frames_const_ptr(@ptrCast(?*const anyopaque, p), offsetInFrames, @bitCast(c_uint, ma_format_f32), channels)));
}
pub extern fn ma_clip_samples_u8(pDst: [*c]ma_uint8, pSrc: [*c]const ma_int16, count: ma_uint64) void;
pub extern fn ma_clip_samples_s16(pDst: [*c]ma_int16, pSrc: [*c]const ma_int32, count: ma_uint64) void;
pub extern fn ma_clip_samples_s24(pDst: [*c]ma_uint8, pSrc: [*c]const ma_int64, count: ma_uint64) void;
pub extern fn ma_clip_samples_s32(pDst: [*c]ma_int32, pSrc: [*c]const ma_int64, count: ma_uint64) void;
pub extern fn ma_clip_samples_f32(pDst: [*c]f32, pSrc: [*c]const f32, count: ma_uint64) void;
pub extern fn ma_clip_pcm_frames(pDst: ?*anyopaque, pSrc: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32) void;
pub extern fn ma_copy_and_apply_volume_factor_u8(pSamplesOut: [*c]ma_uint8, pSamplesIn: [*c]const ma_uint8, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_s16(pSamplesOut: [*c]ma_int16, pSamplesIn: [*c]const ma_int16, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_s24(pSamplesOut: ?*anyopaque, pSamplesIn: ?*const anyopaque, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_s32(pSamplesOut: [*c]ma_int32, pSamplesIn: [*c]const ma_int32, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_f32(pSamplesOut: [*c]f32, pSamplesIn: [*c]const f32, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_apply_volume_factor_u8(pSamples: [*c]ma_uint8, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_apply_volume_factor_s16(pSamples: [*c]ma_int16, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_apply_volume_factor_s24(pSamples: ?*anyopaque, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_apply_volume_factor_s32(pSamples: [*c]ma_int32, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_apply_volume_factor_f32(pSamples: [*c]f32, sampleCount: ma_uint64, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames_u8(pFramesOut: [*c]ma_uint8, pFramesIn: [*c]const ma_uint8, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames_s16(pFramesOut: [*c]ma_int16, pFramesIn: [*c]const ma_int16, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames_s24(pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames_s32(pFramesOut: [*c]ma_int32, pFramesIn: [*c]const ma_int32, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames_f32(pFramesOut: [*c]f32, pFramesIn: [*c]const f32, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_pcm_frames(pFramesOut: ?*anyopaque, pFramesIn: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames_u8(pFrames: [*c]ma_uint8, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames_s16(pFrames: [*c]ma_int16, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames_s24(pFrames: ?*anyopaque, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames_s32(pFrames: [*c]ma_int32, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames_f32(pFrames: [*c]f32, frameCount: ma_uint64, channels: ma_uint32, factor: f32) void;
pub extern fn ma_apply_volume_factor_pcm_frames(pFrames: ?*anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32, factor: f32) void;
pub extern fn ma_copy_and_apply_volume_factor_per_channel_f32(pFramesOut: [*c]f32, pFramesIn: [*c]const f32, frameCount: ma_uint64, channels: ma_uint32, pChannelGains: [*c]f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_samples_u8(pDst: [*c]ma_uint8, pSrc: [*c]const ma_int16, count: ma_uint64, volume: f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_samples_s16(pDst: [*c]ma_int16, pSrc: [*c]const ma_int32, count: ma_uint64, volume: f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_samples_s24(pDst: [*c]ma_uint8, pSrc: [*c]const ma_int64, count: ma_uint64, volume: f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_samples_s32(pDst: [*c]ma_int32, pSrc: [*c]const ma_int64, count: ma_uint64, volume: f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_samples_f32(pDst: [*c]f32, pSrc: [*c]const f32, count: ma_uint64, volume: f32) void;
pub extern fn ma_copy_and_apply_volume_and_clip_pcm_frames(pDst: ?*anyopaque, pSrc: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32, volume: f32) void;
pub extern fn ma_volume_linear_to_db(factor: f32) f32;
pub extern fn ma_volume_db_to_linear(gain: f32) f32;
pub const ma_data_source = anyopaque;
pub const ma_data_source_vtable = extern struct {
    onRead: ?*const fn (?*ma_data_source, ?*anyopaque, ma_uint64, [*c]ma_uint64) callconv(.C) ma_result,
    onSeek: ?*const fn (?*ma_data_source, ma_uint64) callconv(.C) ma_result,
    onGetDataFormat: ?*const fn (?*ma_data_source, [*c]ma_format, [*c]ma_uint32, [*c]ma_uint32, [*c]ma_channel, usize) callconv(.C) ma_result,
    onGetCursor: ?*const fn (?*ma_data_source, [*c]ma_uint64) callconv(.C) ma_result,
    onGetLength: ?*const fn (?*ma_data_source, [*c]ma_uint64) callconv(.C) ma_result,
    onSetLooping: ?*const fn (?*ma_data_source, ma_bool32) callconv(.C) ma_result,
    flags: ma_uint32,
};
pub const ma_data_source_get_next_proc = ?*const fn (?*ma_data_source) callconv(.C) ?*ma_data_source;
pub const ma_data_source_config = extern struct {
    vtable: [*c]const ma_data_source_vtable,
};
pub extern fn ma_data_source_config_init() ma_data_source_config;
pub const ma_data_source_base = extern struct {
    vtable: [*c]const ma_data_source_vtable,
    rangeBegInFrames: ma_uint64,
    rangeEndInFrames: ma_uint64,
    loopBegInFrames: ma_uint64,
    loopEndInFrames: ma_uint64,
    pCurrent: ?*ma_data_source,
    pNext: ?*ma_data_source,
    onGetNext: ma_data_source_get_next_proc,
    isLooping: ma_bool32 align(4),
};
pub extern fn ma_data_source_init(pConfig: [*c]const ma_data_source_config, pDataSource: ?*ma_data_source) ma_result;
pub extern fn ma_data_source_uninit(pDataSource: ?*ma_data_source) void;
pub extern fn ma_data_source_read_pcm_frames(pDataSource: ?*ma_data_source, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_data_source_seek_pcm_frames(pDataSource: ?*ma_data_source, frameCount: ma_uint64, pFramesSeeked: [*c]ma_uint64) ma_result;
pub extern fn ma_data_source_seek_to_pcm_frame(pDataSource: ?*ma_data_source, frameIndex: ma_uint64) ma_result;
pub extern fn ma_data_source_get_data_format(pDataSource: ?*ma_data_source, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_data_source_get_cursor_in_pcm_frames(pDataSource: ?*ma_data_source, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_data_source_get_length_in_pcm_frames(pDataSource: ?*ma_data_source, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_data_source_get_cursor_in_seconds(pDataSource: ?*ma_data_source, pCursor: [*c]f32) ma_result;
pub extern fn ma_data_source_get_length_in_seconds(pDataSource: ?*ma_data_source, pLength: [*c]f32) ma_result;
pub extern fn ma_data_source_set_looping(pDataSource: ?*ma_data_source, isLooping: ma_bool32) ma_result;
pub extern fn ma_data_source_is_looping(pDataSource: ?*const ma_data_source) ma_bool32;
pub extern fn ma_data_source_set_range_in_pcm_frames(pDataSource: ?*ma_data_source, rangeBegInFrames: ma_uint64, rangeEndInFrames: ma_uint64) ma_result;
pub extern fn ma_data_source_get_range_in_pcm_frames(pDataSource: ?*const ma_data_source, pRangeBegInFrames: [*c]ma_uint64, pRangeEndInFrames: [*c]ma_uint64) void;
pub extern fn ma_data_source_set_loop_point_in_pcm_frames(pDataSource: ?*ma_data_source, loopBegInFrames: ma_uint64, loopEndInFrames: ma_uint64) ma_result;
pub extern fn ma_data_source_get_loop_point_in_pcm_frames(pDataSource: ?*const ma_data_source, pLoopBegInFrames: [*c]ma_uint64, pLoopEndInFrames: [*c]ma_uint64) void;
pub extern fn ma_data_source_set_current(pDataSource: ?*ma_data_source, pCurrentDataSource: ?*ma_data_source) ma_result;
pub extern fn ma_data_source_get_current(pDataSource: ?*const ma_data_source) ?*ma_data_source;
pub extern fn ma_data_source_set_next(pDataSource: ?*ma_data_source, pNextDataSource: ?*ma_data_source) ma_result;
pub extern fn ma_data_source_get_next(pDataSource: ?*const ma_data_source) ?*ma_data_source;
pub extern fn ma_data_source_set_next_callback(pDataSource: ?*ma_data_source, onGetNext: ma_data_source_get_next_proc) ma_result;
pub extern fn ma_data_source_get_next_callback(pDataSource: ?*const ma_data_source) ma_data_source_get_next_proc;
pub const ma_audio_buffer_ref = extern struct {
    ds: ma_data_source_base,
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    cursor: ma_uint64,
    sizeInFrames: ma_uint64,
    pData: ?*const anyopaque,
};
pub extern fn ma_audio_buffer_ref_init(format: ma_format, channels: ma_uint32, pData: ?*const anyopaque, sizeInFrames: ma_uint64, pAudioBufferRef: [*c]ma_audio_buffer_ref) ma_result;
pub extern fn ma_audio_buffer_ref_uninit(pAudioBufferRef: [*c]ma_audio_buffer_ref) void;
pub extern fn ma_audio_buffer_ref_set_data(pAudioBufferRef: [*c]ma_audio_buffer_ref, pData: ?*const anyopaque, sizeInFrames: ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_read_pcm_frames(pAudioBufferRef: [*c]ma_audio_buffer_ref, pFramesOut: ?*anyopaque, frameCount: ma_uint64, loop: ma_bool32) ma_uint64;
pub extern fn ma_audio_buffer_ref_seek_to_pcm_frame(pAudioBufferRef: [*c]ma_audio_buffer_ref, frameIndex: ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_map(pAudioBufferRef: [*c]ma_audio_buffer_ref, ppFramesOut: [*c]?*anyopaque, pFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_unmap(pAudioBufferRef: [*c]ma_audio_buffer_ref, frameCount: ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_at_end(pAudioBufferRef: [*c]const ma_audio_buffer_ref) ma_bool32;
pub extern fn ma_audio_buffer_ref_get_cursor_in_pcm_frames(pAudioBufferRef: [*c]const ma_audio_buffer_ref, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_get_length_in_pcm_frames(pAudioBufferRef: [*c]const ma_audio_buffer_ref, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_ref_get_available_frames(pAudioBufferRef: [*c]const ma_audio_buffer_ref, pAvailableFrames: [*c]ma_uint64) ma_result;
pub const ma_audio_buffer_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    sizeInFrames: ma_uint64,
    pData: ?*const anyopaque,
    allocationCallbacks: ma_allocation_callbacks,
};
pub extern fn ma_audio_buffer_config_init(format: ma_format, channels: ma_uint32, sizeInFrames: ma_uint64, pData: ?*const anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ma_audio_buffer_config;
pub const ma_audio_buffer = extern struct {
    ref: ma_audio_buffer_ref,
    allocationCallbacks: ma_allocation_callbacks,
    ownsData: ma_bool32,
    _pExtraData: [1]ma_uint8,
};
pub extern fn ma_audio_buffer_init(pConfig: [*c]const ma_audio_buffer_config, pAudioBuffer: [*c]ma_audio_buffer) ma_result;
pub extern fn ma_audio_buffer_init_copy(pConfig: [*c]const ma_audio_buffer_config, pAudioBuffer: [*c]ma_audio_buffer) ma_result;
pub extern fn ma_audio_buffer_alloc_and_init(pConfig: [*c]const ma_audio_buffer_config, ppAudioBuffer: [*c][*c]ma_audio_buffer) ma_result;
pub extern fn ma_audio_buffer_uninit(pAudioBuffer: [*c]ma_audio_buffer) void;
pub extern fn ma_audio_buffer_uninit_and_free(pAudioBuffer: [*c]ma_audio_buffer) void;
pub extern fn ma_audio_buffer_read_pcm_frames(pAudioBuffer: [*c]ma_audio_buffer, pFramesOut: ?*anyopaque, frameCount: ma_uint64, loop: ma_bool32) ma_uint64;
pub extern fn ma_audio_buffer_seek_to_pcm_frame(pAudioBuffer: [*c]ma_audio_buffer, frameIndex: ma_uint64) ma_result;
pub extern fn ma_audio_buffer_map(pAudioBuffer: [*c]ma_audio_buffer, ppFramesOut: [*c]?*anyopaque, pFrameCount: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_unmap(pAudioBuffer: [*c]ma_audio_buffer, frameCount: ma_uint64) ma_result;
pub extern fn ma_audio_buffer_at_end(pAudioBuffer: [*c]const ma_audio_buffer) ma_bool32;
pub extern fn ma_audio_buffer_get_cursor_in_pcm_frames(pAudioBuffer: [*c]const ma_audio_buffer, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_get_length_in_pcm_frames(pAudioBuffer: [*c]const ma_audio_buffer, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_audio_buffer_get_available_frames(pAudioBuffer: [*c]const ma_audio_buffer, pAvailableFrames: [*c]ma_uint64) ma_result;
pub const ma_paged_audio_buffer_page = struct_ma_paged_audio_buffer_page;
pub const struct_ma_paged_audio_buffer_page = extern struct {
    pNext: [*c]ma_paged_audio_buffer_page align(8),
    sizeInFrames: ma_uint64,
    pAudioData: [1]ma_uint8,
};
pub const ma_paged_audio_buffer_data = extern struct {
    format: ma_format,
    channels: ma_uint32,
    head: ma_paged_audio_buffer_page,
    pTail: [*c]ma_paged_audio_buffer_page align(8),
};
pub extern fn ma_paged_audio_buffer_data_init(format: ma_format, channels: ma_uint32, pData: [*c]ma_paged_audio_buffer_data) ma_result;
pub extern fn ma_paged_audio_buffer_data_uninit(pData: [*c]ma_paged_audio_buffer_data, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_paged_audio_buffer_data_get_head(pData: [*c]ma_paged_audio_buffer_data) [*c]ma_paged_audio_buffer_page;
pub extern fn ma_paged_audio_buffer_data_get_tail(pData: [*c]ma_paged_audio_buffer_data) [*c]ma_paged_audio_buffer_page;
pub extern fn ma_paged_audio_buffer_data_get_length_in_pcm_frames(pData: [*c]ma_paged_audio_buffer_data, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_paged_audio_buffer_data_allocate_page(pData: [*c]ma_paged_audio_buffer_data, pageSizeInFrames: ma_uint64, pInitialData: ?*const anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks, ppPage: [*c][*c]ma_paged_audio_buffer_page) ma_result;
pub extern fn ma_paged_audio_buffer_data_free_page(pData: [*c]ma_paged_audio_buffer_data, pPage: [*c]ma_paged_audio_buffer_page, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ma_result;
pub extern fn ma_paged_audio_buffer_data_append_page(pData: [*c]ma_paged_audio_buffer_data, pPage: [*c]ma_paged_audio_buffer_page) ma_result;
pub extern fn ma_paged_audio_buffer_data_allocate_and_append_page(pData: [*c]ma_paged_audio_buffer_data, pageSizeInFrames: ma_uint32, pInitialData: ?*const anyopaque, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ma_result;
pub const ma_paged_audio_buffer_config = extern struct {
    pData: [*c]ma_paged_audio_buffer_data,
};
pub extern fn ma_paged_audio_buffer_config_init(pData: [*c]ma_paged_audio_buffer_data) ma_paged_audio_buffer_config;
pub const ma_paged_audio_buffer = extern struct {
    ds: ma_data_source_base,
    pData: [*c]ma_paged_audio_buffer_data,
    pCurrent: [*c]ma_paged_audio_buffer_page,
    relativeCursor: ma_uint64,
    absoluteCursor: ma_uint64,
};
pub extern fn ma_paged_audio_buffer_init(pConfig: [*c]const ma_paged_audio_buffer_config, pPagedAudioBuffer: [*c]ma_paged_audio_buffer) ma_result;
pub extern fn ma_paged_audio_buffer_uninit(pPagedAudioBuffer: [*c]ma_paged_audio_buffer) void;
pub extern fn ma_paged_audio_buffer_read_pcm_frames(pPagedAudioBuffer: [*c]ma_paged_audio_buffer, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_paged_audio_buffer_seek_to_pcm_frame(pPagedAudioBuffer: [*c]ma_paged_audio_buffer, frameIndex: ma_uint64) ma_result;
pub extern fn ma_paged_audio_buffer_get_cursor_in_pcm_frames(pPagedAudioBuffer: [*c]ma_paged_audio_buffer, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_paged_audio_buffer_get_length_in_pcm_frames(pPagedAudioBuffer: [*c]ma_paged_audio_buffer, pLength: [*c]ma_uint64) ma_result;
pub const ma_vfs = anyopaque;
pub const ma_vfs_file = ma_handle;
pub const MA_OPEN_MODE_READ: c_int = 1;
pub const MA_OPEN_MODE_WRITE: c_int = 2;
pub const ma_open_mode_flags = c_uint;
pub const ma_seek_origin_start: c_int = 0;
pub const ma_seek_origin_current: c_int = 1;
pub const ma_seek_origin_end: c_int = 2;
pub const ma_seek_origin = c_uint;
pub const ma_file_info = extern struct {
    sizeInBytes: ma_uint64,
};
pub const ma_vfs_callbacks = extern struct {
    onOpen: ?*const fn (?*ma_vfs, [*c]const u8, ma_uint32, [*c]ma_vfs_file) callconv(.C) ma_result,
    onOpenW: ?*const fn (?*ma_vfs, [*c]const wchar_t, ma_uint32, [*c]ma_vfs_file) callconv(.C) ma_result,
    onClose: ?*const fn (?*ma_vfs, ma_vfs_file) callconv(.C) ma_result,
    onRead: ?*const fn (?*ma_vfs, ma_vfs_file, ?*anyopaque, usize, [*c]usize) callconv(.C) ma_result,
    onWrite: ?*const fn (?*ma_vfs, ma_vfs_file, ?*const anyopaque, usize, [*c]usize) callconv(.C) ma_result,
    onSeek: ?*const fn (?*ma_vfs, ma_vfs_file, ma_int64, ma_seek_origin) callconv(.C) ma_result,
    onTell: ?*const fn (?*ma_vfs, ma_vfs_file, [*c]ma_int64) callconv(.C) ma_result,
    onInfo: ?*const fn (?*ma_vfs, ma_vfs_file, [*c]ma_file_info) callconv(.C) ma_result,
};
pub extern fn ma_vfs_open(pVFS: ?*ma_vfs, pFilePath: [*c]const u8, openMode: ma_uint32, pFile: [*c]ma_vfs_file) ma_result;
pub extern fn ma_vfs_open_w(pVFS: ?*ma_vfs, pFilePath: [*c]const wchar_t, openMode: ma_uint32, pFile: [*c]ma_vfs_file) ma_result;
pub extern fn ma_vfs_close(pVFS: ?*ma_vfs, file: ma_vfs_file) ma_result;
pub extern fn ma_vfs_read(pVFS: ?*ma_vfs, file: ma_vfs_file, pDst: ?*anyopaque, sizeInBytes: usize, pBytesRead: [*c]usize) ma_result;
pub extern fn ma_vfs_write(pVFS: ?*ma_vfs, file: ma_vfs_file, pSrc: ?*const anyopaque, sizeInBytes: usize, pBytesWritten: [*c]usize) ma_result;
pub extern fn ma_vfs_seek(pVFS: ?*ma_vfs, file: ma_vfs_file, offset: ma_int64, origin: ma_seek_origin) ma_result;
pub extern fn ma_vfs_tell(pVFS: ?*ma_vfs, file: ma_vfs_file, pCursor: [*c]ma_int64) ma_result;
pub extern fn ma_vfs_info(pVFS: ?*ma_vfs, file: ma_vfs_file, pInfo: [*c]ma_file_info) ma_result;
pub extern fn ma_vfs_open_and_read_file(pVFS: ?*ma_vfs, pFilePath: [*c]const u8, ppData: [*c]?*anyopaque, pSize: [*c]usize, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ma_result;
pub const ma_default_vfs = extern struct {
    cb: ma_vfs_callbacks,
    allocationCallbacks: ma_allocation_callbacks,
};
pub extern fn ma_default_vfs_init(pVFS: [*c]ma_default_vfs, pAllocationCallbacks: [*c]const ma_allocation_callbacks) ma_result;
pub const ma_read_proc = ?*const fn (?*anyopaque, ?*anyopaque, usize, [*c]usize) callconv(.C) ma_result;
pub const ma_seek_proc = ?*const fn (?*anyopaque, ma_int64, ma_seek_origin) callconv(.C) ma_result;
pub const ma_tell_proc = ?*const fn (?*anyopaque, [*c]ma_int64) callconv(.C) ma_result;
pub const ma_encoding_format_unknown: c_int = 0;
pub const ma_encoding_format_wav: c_int = 1;
pub const ma_encoding_format_flac: c_int = 2;
pub const ma_encoding_format_mp3: c_int = 3;
pub const ma_encoding_format_vorbis: c_int = 4;
pub const ma_encoding_format = c_uint;
pub const ma_decoder = struct_ma_decoder;
pub const ma_decoder_read_proc = ?*const fn ([*c]ma_decoder, ?*anyopaque, usize, [*c]usize) callconv(.C) ma_result;
pub const ma_decoder_seek_proc = ?*const fn ([*c]ma_decoder, ma_int64, ma_seek_origin) callconv(.C) ma_result;
pub const ma_decoder_tell_proc = ?*const fn ([*c]ma_decoder, [*c]ma_int64) callconv(.C) ma_result;
const struct_unnamed_77 = extern struct {
    pVFS: ?*ma_vfs,
    file: ma_vfs_file,
};
const struct_unnamed_78 = extern struct {
    pData: [*c]const ma_uint8,
    dataSize: usize,
    currentReadPos: usize,
};
const union_unnamed_76 = extern union {
    vfs: struct_unnamed_77,
    memory: struct_unnamed_78,
};
pub const struct_ma_decoder = extern struct {
    ds: ma_data_source_base,
    pBackend: ?*ma_data_source,
    pBackendVTable: [*c]const ma_decoding_backend_vtable,
    pBackendUserData: ?*anyopaque,
    onRead: ma_decoder_read_proc,
    onSeek: ma_decoder_seek_proc,
    onTell: ma_decoder_tell_proc,
    pUserData: ?*anyopaque,
    readPointerInPCMFrames: ma_uint64,
    outputFormat: ma_format,
    outputChannels: ma_uint32,
    outputSampleRate: ma_uint32,
    converter: ma_data_converter,
    pInputCache: ?*anyopaque,
    inputCacheCap: ma_uint64,
    inputCacheConsumed: ma_uint64,
    inputCacheRemaining: ma_uint64,
    allocationCallbacks: ma_allocation_callbacks,
    data: union_unnamed_76,
};
pub const ma_decoding_backend_config = extern struct {
    preferredFormat: ma_format,
    seekPointCount: ma_uint32,
};
pub extern fn ma_decoding_backend_config_init(preferredFormat: ma_format, seekPointCount: ma_uint32) ma_decoding_backend_config;
pub const ma_decoding_backend_vtable = extern struct {
    onInit: ?*const fn (?*anyopaque, ma_read_proc, ma_seek_proc, ma_tell_proc, ?*anyopaque, [*c]const ma_decoding_backend_config, [*c]const ma_allocation_callbacks, [*c]?*ma_data_source) callconv(.C) ma_result,
    onInitFile: ?*const fn (?*anyopaque, [*c]const u8, [*c]const ma_decoding_backend_config, [*c]const ma_allocation_callbacks, [*c]?*ma_data_source) callconv(.C) ma_result,
    onInitFileW: ?*const fn (?*anyopaque, [*c]const wchar_t, [*c]const ma_decoding_backend_config, [*c]const ma_allocation_callbacks, [*c]?*ma_data_source) callconv(.C) ma_result,
    onInitMemory: ?*const fn (?*anyopaque, ?*const anyopaque, usize, [*c]const ma_decoding_backend_config, [*c]const ma_allocation_callbacks, [*c]?*ma_data_source) callconv(.C) ma_result,
    onUninit: ?*const fn (?*anyopaque, ?*ma_data_source, [*c]const ma_allocation_callbacks) callconv(.C) void,
};
pub const ma_decoder_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    pChannelMap: [*c]ma_channel,
    channelMixMode: ma_channel_mix_mode,
    ditherMode: ma_dither_mode,
    resampling: ma_resampler_config,
    allocationCallbacks: ma_allocation_callbacks,
    encodingFormat: ma_encoding_format,
    seekPointCount: ma_uint32,
    ppCustomBackendVTables: [*c][*c]ma_decoding_backend_vtable,
    customBackendCount: ma_uint32,
    pCustomBackendUserData: ?*anyopaque,
};
pub extern fn ma_decoder_config_init(outputFormat: ma_format, outputChannels: ma_uint32, outputSampleRate: ma_uint32) ma_decoder_config;
pub extern fn ma_decoder_config_init_default() ma_decoder_config;
pub extern fn ma_decoder_init(onRead: ma_decoder_read_proc, onSeek: ma_decoder_seek_proc, pUserData: ?*anyopaque, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_init_memory(pData: ?*const anyopaque, dataSize: usize, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_init_vfs(pVFS: ?*ma_vfs, pFilePath: [*c]const u8, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_init_vfs_w(pVFS: ?*ma_vfs, pFilePath: [*c]const wchar_t, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_init_file(pFilePath: [*c]const u8, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_init_file_w(pFilePath: [*c]const wchar_t, pConfig: [*c]const ma_decoder_config, pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_uninit(pDecoder: [*c]ma_decoder) ma_result;
pub extern fn ma_decoder_read_pcm_frames(pDecoder: [*c]ma_decoder, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_decoder_seek_to_pcm_frame(pDecoder: [*c]ma_decoder, frameIndex: ma_uint64) ma_result;
pub extern fn ma_decoder_get_data_format(pDecoder: [*c]ma_decoder, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_decoder_get_cursor_in_pcm_frames(pDecoder: [*c]ma_decoder, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_decoder_get_length_in_pcm_frames(pDecoder: [*c]ma_decoder, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_decoder_get_available_frames(pDecoder: [*c]ma_decoder, pAvailableFrames: [*c]ma_uint64) ma_result;
pub extern fn ma_decode_from_vfs(pVFS: ?*ma_vfs, pFilePath: [*c]const u8, pConfig: [*c]ma_decoder_config, pFrameCountOut: [*c]ma_uint64, ppPCMFramesOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_decode_file(pFilePath: [*c]const u8, pConfig: [*c]ma_decoder_config, pFrameCountOut: [*c]ma_uint64, ppPCMFramesOut: [*c]?*anyopaque) ma_result;
pub extern fn ma_decode_memory(pData: ?*const anyopaque, dataSize: usize, pConfig: [*c]ma_decoder_config, pFrameCountOut: [*c]ma_uint64, ppPCMFramesOut: [*c]?*anyopaque) ma_result;
pub const ma_encoder = struct_ma_encoder;
pub const ma_encoder_write_proc = ?*const fn ([*c]ma_encoder, ?*const anyopaque, usize, [*c]usize) callconv(.C) ma_result;
pub const ma_encoder_seek_proc = ?*const fn ([*c]ma_encoder, ma_int64, ma_seek_origin) callconv(.C) ma_result;
pub const ma_encoder_init_proc = ?*const fn ([*c]ma_encoder) callconv(.C) ma_result;
pub const ma_encoder_uninit_proc = ?*const fn ([*c]ma_encoder) callconv(.C) void;
pub const ma_encoder_write_pcm_frames_proc = ?*const fn ([*c]ma_encoder, ?*const anyopaque, ma_uint64, [*c]ma_uint64) callconv(.C) ma_result;
const struct_unnamed_80 = extern struct {
    pVFS: ?*ma_vfs,
    file: ma_vfs_file,
};
const union_unnamed_79 = extern union {
    vfs: struct_unnamed_80,
};
pub const struct_ma_encoder = extern struct {
    config: ma_encoder_config,
    onWrite: ma_encoder_write_proc,
    onSeek: ma_encoder_seek_proc,
    onInit: ma_encoder_init_proc,
    onUninit: ma_encoder_uninit_proc,
    onWritePCMFrames: ma_encoder_write_pcm_frames_proc,
    pUserData: ?*anyopaque,
    pInternalEncoder: ?*anyopaque,
    data: union_unnamed_79,
};
pub const ma_encoder_config = extern struct {
    encodingFormat: ma_encoding_format,
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    allocationCallbacks: ma_allocation_callbacks,
};
pub extern fn ma_encoder_config_init(encodingFormat: ma_encoding_format, format: ma_format, channels: ma_uint32, sampleRate: ma_uint32) ma_encoder_config;
pub extern fn ma_encoder_init(onWrite: ma_encoder_write_proc, onSeek: ma_encoder_seek_proc, pUserData: ?*anyopaque, pConfig: [*c]const ma_encoder_config, pEncoder: [*c]ma_encoder) ma_result;
pub extern fn ma_encoder_init_vfs(pVFS: ?*ma_vfs, pFilePath: [*c]const u8, pConfig: [*c]const ma_encoder_config, pEncoder: [*c]ma_encoder) ma_result;
pub extern fn ma_encoder_init_vfs_w(pVFS: ?*ma_vfs, pFilePath: [*c]const wchar_t, pConfig: [*c]const ma_encoder_config, pEncoder: [*c]ma_encoder) ma_result;
pub extern fn ma_encoder_init_file(pFilePath: [*c]const u8, pConfig: [*c]const ma_encoder_config, pEncoder: [*c]ma_encoder) ma_result;
pub extern fn ma_encoder_init_file_w(pFilePath: [*c]const wchar_t, pConfig: [*c]const ma_encoder_config, pEncoder: [*c]ma_encoder) ma_result;
pub extern fn ma_encoder_uninit(pEncoder: [*c]ma_encoder) void;
pub extern fn ma_encoder_write_pcm_frames(pEncoder: [*c]ma_encoder, pFramesIn: ?*const anyopaque, frameCount: ma_uint64, pFramesWritten: [*c]ma_uint64) ma_result;
pub const ma_waveform_type_sine: c_int = 0;
pub const ma_waveform_type_square: c_int = 1;
pub const ma_waveform_type_triangle: c_int = 2;
pub const ma_waveform_type_sawtooth: c_int = 3;
pub const ma_waveform_type = c_uint;
pub const ma_waveform_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    type: ma_waveform_type,
    amplitude: f64,
    frequency: f64,
};
pub extern fn ma_waveform_config_init(format: ma_format, channels: ma_uint32, sampleRate: ma_uint32, @"type": ma_waveform_type, amplitude: f64, frequency: f64) ma_waveform_config;
pub const ma_waveform = extern struct {
    ds: ma_data_source_base,
    config: ma_waveform_config,
    advance: f64,
    time: f64,
};
pub extern fn ma_waveform_init(pConfig: [*c]const ma_waveform_config, pWaveform: [*c]ma_waveform) ma_result;
pub extern fn ma_waveform_uninit(pWaveform: [*c]ma_waveform) void;
pub extern fn ma_waveform_read_pcm_frames(pWaveform: [*c]ma_waveform, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_waveform_seek_to_pcm_frame(pWaveform: [*c]ma_waveform, frameIndex: ma_uint64) ma_result;
pub extern fn ma_waveform_set_amplitude(pWaveform: [*c]ma_waveform, amplitude: f64) ma_result;
pub extern fn ma_waveform_set_frequency(pWaveform: [*c]ma_waveform, frequency: f64) ma_result;
pub extern fn ma_waveform_set_type(pWaveform: [*c]ma_waveform, @"type": ma_waveform_type) ma_result;
pub extern fn ma_waveform_set_sample_rate(pWaveform: [*c]ma_waveform, sampleRate: ma_uint32) ma_result;
pub const ma_noise_type_white: c_int = 0;
pub const ma_noise_type_pink: c_int = 1;
pub const ma_noise_type_brownian: c_int = 2;
pub const ma_noise_type = c_uint;
pub const ma_noise_config = extern struct {
    format: ma_format,
    channels: ma_uint32,
    type: ma_noise_type,
    seed: ma_int32,
    amplitude: f64,
    duplicateChannels: ma_bool32,
};
pub extern fn ma_noise_config_init(format: ma_format, channels: ma_uint32, @"type": ma_noise_type, seed: ma_int32, amplitude: f64) ma_noise_config;
const struct_unnamed_82 = extern struct {
    bin: [*c][*c]f64,
    accumulation: [*c]f64,
    counter: [*c]ma_uint32,
};
const struct_unnamed_83 = extern struct {
    accumulation: [*c]f64,
};
const union_unnamed_81 = extern union {
    pink: struct_unnamed_82,
    brownian: struct_unnamed_83,
};
pub const ma_noise = extern struct {
    ds: ma_data_source_vtable,
    config: ma_noise_config,
    lcg: ma_lcg,
    state: union_unnamed_81,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub extern fn ma_noise_get_heap_size(pConfig: [*c]const ma_noise_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_noise_init_preallocated(pConfig: [*c]const ma_noise_config, pHeap: ?*anyopaque, pNoise: [*c]ma_noise) ma_result;
pub extern fn ma_noise_init(pConfig: [*c]const ma_noise_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNoise: [*c]ma_noise) ma_result;
pub extern fn ma_noise_uninit(pNoise: [*c]ma_noise, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_noise_read_pcm_frames(pNoise: [*c]ma_noise, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_noise_set_amplitude(pNoise: [*c]ma_noise, amplitude: f64) ma_result;
pub extern fn ma_noise_set_seed(pNoise: [*c]ma_noise, seed: ma_int32) ma_result;
pub extern fn ma_noise_set_type(pNoise: [*c]ma_noise, @"type": ma_noise_type) ma_result;
pub const struct_ma_resource_manager_data_buffer_node = extern struct {
    hashedName32: ma_uint32,
    refCount: ma_uint32,
    result: ma_result align(4),
    executionCounter: ma_uint32 align(4),
    executionPointer: ma_uint32 align(4),
    isDataOwnedByResourceManager: ma_bool32,
    data: ma_resource_manager_data_supply,
    pParent: [*c]ma_resource_manager_data_buffer_node,
    pChildLo: [*c]ma_resource_manager_data_buffer_node,
    pChildHi: [*c]ma_resource_manager_data_buffer_node,
};
pub const ma_resource_manager_data_buffer_node = struct_ma_resource_manager_data_buffer_node;
pub const struct_ma_resource_manager = extern struct {
    config: ma_resource_manager_config,
    pRootDataBufferNode: [*c]ma_resource_manager_data_buffer_node,
    dataBufferBSTLock: ma_mutex,
    jobThreads: [64]ma_thread,
    jobQueue: ma_job_queue,
    defaultVFS: ma_default_vfs,
    log: ma_log,
};
pub const ma_resource_manager = struct_ma_resource_manager;
const union_unnamed_84 = extern union {
    decoder: ma_decoder,
    buffer: ma_audio_buffer,
    pagedBuffer: ma_paged_audio_buffer,
};
pub const struct_ma_resource_manager_data_buffer = extern struct {
    ds: ma_data_source_base,
    pResourceManager: [*c]ma_resource_manager,
    pNode: [*c]ma_resource_manager_data_buffer_node,
    flags: ma_uint32,
    executionCounter: ma_uint32 align(4),
    executionPointer: ma_uint32 align(4),
    seekTargetInPCMFrames: ma_uint64,
    seekToCursorOnNextRead: ma_bool32,
    result: ma_result align(4),
    isLooping: ma_bool32 align(4),
    isConnectorInitialized: ma_bool32,
    connector: union_unnamed_84,
};
pub const ma_resource_manager_data_buffer = struct_ma_resource_manager_data_buffer;
pub const struct_ma_resource_manager_data_stream = extern struct {
    ds: ma_data_source_base,
    pResourceManager: [*c]ma_resource_manager,
    flags: ma_uint32,
    decoder: ma_decoder,
    isDecoderInitialized: ma_bool32,
    totalLengthInPCMFrames: ma_uint64,
    relativeCursor: ma_uint32,
    absoluteCursor: ma_uint64 align(8),
    currentPageIndex: ma_uint32,
    executionCounter: ma_uint32 align(4),
    executionPointer: ma_uint32 align(4),
    isLooping: ma_bool32 align(4),
    pPageData: ?*anyopaque,
    pageFrameCount: [2]ma_uint32 align(4),
    result: ma_result align(4),
    isDecoderAtEnd: ma_bool32 align(4),
    isPageValid: [2]ma_bool32 align(4),
    seekCounter: ma_bool32 align(4),
};
pub const ma_resource_manager_data_stream = struct_ma_resource_manager_data_stream;
const union_unnamed_85 = extern union {
    buffer: ma_resource_manager_data_buffer,
    stream: ma_resource_manager_data_stream,
};
pub const struct_ma_resource_manager_data_source = extern struct {
    backend: union_unnamed_85,
    flags: ma_uint32,
    executionCounter: ma_uint32 align(4),
    executionPointer: ma_uint32 align(4),
};
pub const ma_resource_manager_data_source = struct_ma_resource_manager_data_source;
pub const MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM: c_int = 1;
pub const MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE: c_int = 2;
pub const MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC: c_int = 4;
pub const MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_WAIT_INIT: c_int = 8;
pub const MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_UNKNOWN_LENGTH: c_int = 16;
pub const ma_resource_manager_data_source_flags = c_uint;
pub const ma_resource_manager_pipeline_stage_notification = extern struct {
    pNotification: ?*ma_async_notification,
    pFence: [*c]ma_fence,
};
pub const ma_resource_manager_pipeline_notifications = extern struct {
    init: ma_resource_manager_pipeline_stage_notification,
    done: ma_resource_manager_pipeline_stage_notification,
};
pub extern fn ma_resource_manager_pipeline_notifications_init() ma_resource_manager_pipeline_notifications;
pub const MA_RESOURCE_MANAGER_FLAG_NON_BLOCKING: c_int = 1;
pub const MA_RESOURCE_MANAGER_FLAG_NO_THREADING: c_int = 2;
pub const ma_resource_manager_flags = c_uint;
pub const ma_resource_manager_data_source_config = extern struct {
    pFilePath: [*c]const u8,
    pFilePathW: [*c]const wchar_t,
    pNotifications: [*c]const ma_resource_manager_pipeline_notifications,
    initialSeekPointInPCMFrames: ma_uint64,
    rangeBegInPCMFrames: ma_uint64,
    rangeEndInPCMFrames: ma_uint64,
    loopPointBegInPCMFrames: ma_uint64,
    loopPointEndInPCMFrames: ma_uint64,
    isLooping: ma_bool32,
    flags: ma_uint32,
};
pub extern fn ma_resource_manager_data_source_config_init() ma_resource_manager_data_source_config;
pub const ma_resource_manager_data_supply_type_unknown: c_int = 0;
pub const ma_resource_manager_data_supply_type_encoded: c_int = 1;
pub const ma_resource_manager_data_supply_type_decoded: c_int = 2;
pub const ma_resource_manager_data_supply_type_decoded_paged: c_int = 3;
pub const ma_resource_manager_data_supply_type = c_uint;
const struct_unnamed_87 = extern struct {
    pData: ?*const anyopaque,
    sizeInBytes: usize,
};
const struct_unnamed_88 = extern struct {
    pData: ?*const anyopaque,
    totalFrameCount: ma_uint64,
    decodedFrameCount: ma_uint64,
    format: ma_format,
    channels: ma_uint32,
    sampleRate: ma_uint32,
};
const struct_unnamed_89 = extern struct {
    data: ma_paged_audio_buffer_data,
    decodedFrameCount: ma_uint64,
    sampleRate: ma_uint32,
};
const union_unnamed_86 = extern union {
    encoded: struct_unnamed_87,
    decoded: struct_unnamed_88,
    decodedPaged: struct_unnamed_89,
};
pub const ma_resource_manager_data_supply = extern struct {
    type: ma_resource_manager_data_supply_type align(4),
    backend: union_unnamed_86,
};
pub const ma_resource_manager_config = extern struct {
    allocationCallbacks: ma_allocation_callbacks,
    pLog: [*c]ma_log,
    decodedFormat: ma_format,
    decodedChannels: ma_uint32,
    decodedSampleRate: ma_uint32,
    jobThreadCount: ma_uint32,
    jobQueueCapacity: ma_uint32,
    flags: ma_uint32,
    pVFS: ?*ma_vfs,
    ppCustomDecodingBackendVTables: [*c][*c]ma_decoding_backend_vtable,
    customDecodingBackendCount: ma_uint32,
    pCustomDecodingBackendUserData: ?*anyopaque,
};
pub extern fn ma_resource_manager_config_init() ma_resource_manager_config;
pub extern fn ma_resource_manager_init(pConfig: [*c]const ma_resource_manager_config, pResourceManager: [*c]ma_resource_manager) ma_result;
pub extern fn ma_resource_manager_uninit(pResourceManager: [*c]ma_resource_manager) void;
pub extern fn ma_resource_manager_get_log(pResourceManager: [*c]ma_resource_manager) [*c]ma_log;
pub extern fn ma_resource_manager_register_file(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const u8, flags: ma_uint32) ma_result;
pub extern fn ma_resource_manager_register_file_w(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const wchar_t, flags: ma_uint32) ma_result;
pub extern fn ma_resource_manager_register_decoded_data(pResourceManager: [*c]ma_resource_manager, pName: [*c]const u8, pData: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32, sampleRate: ma_uint32) ma_result;
pub extern fn ma_resource_manager_register_decoded_data_w(pResourceManager: [*c]ma_resource_manager, pName: [*c]const wchar_t, pData: ?*const anyopaque, frameCount: ma_uint64, format: ma_format, channels: ma_uint32, sampleRate: ma_uint32) ma_result;
pub extern fn ma_resource_manager_register_encoded_data(pResourceManager: [*c]ma_resource_manager, pName: [*c]const u8, pData: ?*const anyopaque, sizeInBytes: usize) ma_result;
pub extern fn ma_resource_manager_register_encoded_data_w(pResourceManager: [*c]ma_resource_manager, pName: [*c]const wchar_t, pData: ?*const anyopaque, sizeInBytes: usize) ma_result;
pub extern fn ma_resource_manager_unregister_file(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const u8) ma_result;
pub extern fn ma_resource_manager_unregister_file_w(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const wchar_t) ma_result;
pub extern fn ma_resource_manager_unregister_data(pResourceManager: [*c]ma_resource_manager, pName: [*c]const u8) ma_result;
pub extern fn ma_resource_manager_unregister_data_w(pResourceManager: [*c]ma_resource_manager, pName: [*c]const wchar_t) ma_result;
pub extern fn ma_resource_manager_data_buffer_init_ex(pResourceManager: [*c]ma_resource_manager, pConfig: [*c]const ma_resource_manager_data_source_config, pDataBuffer: [*c]ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_init(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const u8, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataBuffer: [*c]ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_init_w(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const wchar_t, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataBuffer: [*c]ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_init_copy(pResourceManager: [*c]ma_resource_manager, pExistingDataBuffer: [*c]const ma_resource_manager_data_buffer, pDataBuffer: [*c]ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_uninit(pDataBuffer: [*c]ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_read_pcm_frames(pDataBuffer: [*c]ma_resource_manager_data_buffer, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_buffer_seek_to_pcm_frame(pDataBuffer: [*c]ma_resource_manager_data_buffer, frameIndex: ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_buffer_get_data_format(pDataBuffer: [*c]ma_resource_manager_data_buffer, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_resource_manager_data_buffer_get_cursor_in_pcm_frames(pDataBuffer: [*c]ma_resource_manager_data_buffer, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_buffer_get_length_in_pcm_frames(pDataBuffer: [*c]ma_resource_manager_data_buffer, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_buffer_result(pDataBuffer: [*c]const ma_resource_manager_data_buffer) ma_result;
pub extern fn ma_resource_manager_data_buffer_set_looping(pDataBuffer: [*c]ma_resource_manager_data_buffer, isLooping: ma_bool32) ma_result;
pub extern fn ma_resource_manager_data_buffer_is_looping(pDataBuffer: [*c]const ma_resource_manager_data_buffer) ma_bool32;
pub extern fn ma_resource_manager_data_buffer_get_available_frames(pDataBuffer: [*c]ma_resource_manager_data_buffer, pAvailableFrames: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_stream_init_ex(pResourceManager: [*c]ma_resource_manager, pConfig: [*c]const ma_resource_manager_data_source_config, pDataStream: [*c]ma_resource_manager_data_stream) ma_result;
pub extern fn ma_resource_manager_data_stream_init(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const u8, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataStream: [*c]ma_resource_manager_data_stream) ma_result;
pub extern fn ma_resource_manager_data_stream_init_w(pResourceManager: [*c]ma_resource_manager, pFilePath: [*c]const wchar_t, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataStream: [*c]ma_resource_manager_data_stream) ma_result;
pub extern fn ma_resource_manager_data_stream_uninit(pDataStream: [*c]ma_resource_manager_data_stream) ma_result;
pub extern fn ma_resource_manager_data_stream_read_pcm_frames(pDataStream: [*c]ma_resource_manager_data_stream, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_stream_seek_to_pcm_frame(pDataStream: [*c]ma_resource_manager_data_stream, frameIndex: ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_stream_get_data_format(pDataStream: [*c]ma_resource_manager_data_stream, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_resource_manager_data_stream_get_cursor_in_pcm_frames(pDataStream: [*c]ma_resource_manager_data_stream, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_stream_get_length_in_pcm_frames(pDataStream: [*c]ma_resource_manager_data_stream, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_stream_result(pDataStream: [*c]const ma_resource_manager_data_stream) ma_result;
pub extern fn ma_resource_manager_data_stream_set_looping(pDataStream: [*c]ma_resource_manager_data_stream, isLooping: ma_bool32) ma_result;
pub extern fn ma_resource_manager_data_stream_is_looping(pDataStream: [*c]const ma_resource_manager_data_stream) ma_bool32;
pub extern fn ma_resource_manager_data_stream_get_available_frames(pDataStream: [*c]ma_resource_manager_data_stream, pAvailableFrames: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_source_init_ex(pResourceManager: [*c]ma_resource_manager, pConfig: [*c]const ma_resource_manager_data_source_config, pDataSource: [*c]ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_init(pResourceManager: [*c]ma_resource_manager, pName: [*c]const u8, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataSource: [*c]ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_init_w(pResourceManager: [*c]ma_resource_manager, pName: [*c]const wchar_t, flags: ma_uint32, pNotifications: [*c]const ma_resource_manager_pipeline_notifications, pDataSource: [*c]ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_init_copy(pResourceManager: [*c]ma_resource_manager, pExistingDataSource: [*c]const ma_resource_manager_data_source, pDataSource: [*c]ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_uninit(pDataSource: [*c]ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_read_pcm_frames(pDataSource: [*c]ma_resource_manager_data_source, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_source_seek_to_pcm_frame(pDataSource: [*c]ma_resource_manager_data_source, frameIndex: ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_source_get_data_format(pDataSource: [*c]ma_resource_manager_data_source, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_resource_manager_data_source_get_cursor_in_pcm_frames(pDataSource: [*c]ma_resource_manager_data_source, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_source_get_length_in_pcm_frames(pDataSource: [*c]ma_resource_manager_data_source, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_data_source_result(pDataSource: [*c]const ma_resource_manager_data_source) ma_result;
pub extern fn ma_resource_manager_data_source_set_looping(pDataSource: [*c]ma_resource_manager_data_source, isLooping: ma_bool32) ma_result;
pub extern fn ma_resource_manager_data_source_is_looping(pDataSource: [*c]const ma_resource_manager_data_source) ma_bool32;
pub extern fn ma_resource_manager_data_source_get_available_frames(pDataSource: [*c]ma_resource_manager_data_source, pAvailableFrames: [*c]ma_uint64) ma_result;
pub extern fn ma_resource_manager_post_job(pResourceManager: [*c]ma_resource_manager, pJob: [*c]const ma_job) ma_result;
pub extern fn ma_resource_manager_post_job_quit(pResourceManager: [*c]ma_resource_manager) ma_result;
pub extern fn ma_resource_manager_next_job(pResourceManager: [*c]ma_resource_manager, pJob: [*c]ma_job) ma_result;
pub extern fn ma_resource_manager_process_job(pResourceManager: [*c]ma_resource_manager, pJob: [*c]ma_job) ma_result;
pub extern fn ma_resource_manager_process_next_job(pResourceManager: [*c]ma_resource_manager) ma_result;
pub const ma_node_graph = struct_ma_node_graph;
pub const ma_node = anyopaque;
pub const struct_ma_node_output_bus = extern struct {
    pNode: ?*ma_node,
    outputBusIndex: ma_uint8,
    channels: ma_uint8,
    inputNodeInputBusIndex: ma_uint8 align(1),
    flags: ma_uint32 align(4),
    refCount: ma_uint32 align(4),
    isAttached: ma_bool32 align(4),
    lock: ma_spinlock align(4),
    volume: f32 align(4),
    pNext: [*c]ma_node_output_bus align(8),
    pPrev: [*c]ma_node_output_bus align(8),
    pInputNode: ?*ma_node align(8),
};
pub const ma_node_output_bus = struct_ma_node_output_bus;
pub const struct_ma_node_input_bus = extern struct {
    head: ma_node_output_bus,
    nextCounter: ma_uint32 align(4),
    lock: ma_spinlock align(4),
    channels: ma_uint8,
};
pub const ma_node_input_bus = struct_ma_node_input_bus;
pub const struct_ma_node_base = extern struct {
    pNodeGraph: [*c]ma_node_graph,
    vtable: [*c]const ma_node_vtable,
    pCachedData: [*c]f32,
    cachedDataCapInFramesPerBus: ma_uint16,
    cachedFrameCountOut: ma_uint16,
    cachedFrameCountIn: ma_uint16,
    consumedFrameCountIn: ma_uint16,
    state: ma_node_state align(4),
    stateTimes: [2]ma_uint64 align(8),
    localTime: ma_uint64 align(8),
    inputBusCount: ma_uint32,
    outputBusCount: ma_uint32,
    pInputBuses: [*c]ma_node_input_bus,
    pOutputBuses: [*c]ma_node_output_bus,
    _inputBuses: [2]ma_node_input_bus,
    _outputBuses: [2]ma_node_output_bus,
    _pHeap: ?*anyopaque,
    _ownsHeap: ma_bool32,
};
pub const ma_node_base = struct_ma_node_base;
pub const struct_ma_node_graph = extern struct {
    base: ma_node_base,
    endpoint: ma_node_base,
    nodeCacheCapInFrames: ma_uint16,
    isReading: ma_bool32 align(4),
};
pub const MA_NODE_FLAG_PASSTHROUGH: c_int = 1;
pub const MA_NODE_FLAG_CONTINUOUS_PROCESSING: c_int = 2;
pub const MA_NODE_FLAG_ALLOW_NULL_INPUT: c_int = 4;
pub const MA_NODE_FLAG_DIFFERENT_PROCESSING_RATES: c_int = 8;
pub const MA_NODE_FLAG_SILENT_OUTPUT: c_int = 16;
pub const ma_node_flags = c_uint;
pub const ma_node_state_started: c_int = 0;
pub const ma_node_state_stopped: c_int = 1;
pub const ma_node_state = c_uint;
pub const ma_node_vtable = extern struct {
    onProcess: ?*const fn (?*ma_node, [*c][*c]const f32, [*c]ma_uint32, [*c][*c]f32, [*c]ma_uint32) callconv(.C) void,
    onGetRequiredInputFrameCount: ?*const fn (?*ma_node, ma_uint32, [*c]ma_uint32) callconv(.C) ma_result,
    inputBusCount: ma_uint8,
    outputBusCount: ma_uint8,
    flags: ma_uint32,
};
pub const ma_node_config = extern struct {
    vtable: [*c]const ma_node_vtable,
    initialState: ma_node_state,
    inputBusCount: ma_uint32,
    outputBusCount: ma_uint32,
    pInputChannels: [*c]const ma_uint32,
    pOutputChannels: [*c]const ma_uint32,
};
pub extern fn ma_node_config_init() ma_node_config;
pub extern fn ma_node_get_heap_size(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_node_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_node_init_preallocated(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_node_config, pHeap: ?*anyopaque, pNode: ?*ma_node) ma_result;
pub extern fn ma_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: ?*ma_node) ma_result;
pub extern fn ma_node_uninit(pNode: ?*ma_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_node_get_node_graph(pNode: ?*const ma_node) [*c]ma_node_graph;
pub extern fn ma_node_get_input_bus_count(pNode: ?*const ma_node) ma_uint32;
pub extern fn ma_node_get_output_bus_count(pNode: ?*const ma_node) ma_uint32;
pub extern fn ma_node_get_input_channels(pNode: ?*const ma_node, inputBusIndex: ma_uint32) ma_uint32;
pub extern fn ma_node_get_output_channels(pNode: ?*const ma_node, outputBusIndex: ma_uint32) ma_uint32;
pub extern fn ma_node_attach_output_bus(pNode: ?*ma_node, outputBusIndex: ma_uint32, pOtherNode: ?*ma_node, otherNodeInputBusIndex: ma_uint32) ma_result;
pub extern fn ma_node_detach_output_bus(pNode: ?*ma_node, outputBusIndex: ma_uint32) ma_result;
pub extern fn ma_node_detach_all_output_buses(pNode: ?*ma_node) ma_result;
pub extern fn ma_node_set_output_bus_volume(pNode: ?*ma_node, outputBusIndex: ma_uint32, volume: f32) ma_result;
pub extern fn ma_node_get_output_bus_volume(pNode: ?*const ma_node, outputBusIndex: ma_uint32) f32;
pub extern fn ma_node_set_state(pNode: ?*ma_node, state: ma_node_state) ma_result;
pub extern fn ma_node_get_state(pNode: ?*const ma_node) ma_node_state;
pub extern fn ma_node_set_state_time(pNode: ?*ma_node, state: ma_node_state, globalTime: ma_uint64) ma_result;
pub extern fn ma_node_get_state_time(pNode: ?*const ma_node, state: ma_node_state) ma_uint64;
pub extern fn ma_node_get_state_by_time(pNode: ?*const ma_node, globalTime: ma_uint64) ma_node_state;
pub extern fn ma_node_get_state_by_time_range(pNode: ?*const ma_node, globalTimeBeg: ma_uint64, globalTimeEnd: ma_uint64) ma_node_state;
pub extern fn ma_node_get_time(pNode: ?*const ma_node) ma_uint64;
pub extern fn ma_node_set_time(pNode: ?*ma_node, localTime: ma_uint64) ma_result;
pub const ma_node_graph_config = extern struct {
    channels: ma_uint32,
    nodeCacheCapInFrames: ma_uint16,
};
pub extern fn ma_node_graph_config_init(channels: ma_uint32) ma_node_graph_config;
pub extern fn ma_node_graph_init(pConfig: [*c]const ma_node_graph_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNodeGraph: [*c]ma_node_graph) ma_result;
pub extern fn ma_node_graph_uninit(pNodeGraph: [*c]ma_node_graph, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_node_graph_get_endpoint(pNodeGraph: [*c]ma_node_graph) ?*ma_node;
pub extern fn ma_node_graph_read_pcm_frames(pNodeGraph: [*c]ma_node_graph, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_node_graph_get_channels(pNodeGraph: [*c]const ma_node_graph) ma_uint32;
pub extern fn ma_node_graph_get_time(pNodeGraph: [*c]const ma_node_graph) ma_uint64;
pub extern fn ma_node_graph_set_time(pNodeGraph: [*c]ma_node_graph, globalTime: ma_uint64) ma_result;
pub const ma_data_source_node_config = extern struct {
    nodeConfig: ma_node_config,
    pDataSource: ?*ma_data_source,
};
pub extern fn ma_data_source_node_config_init(pDataSource: ?*ma_data_source) ma_data_source_node_config;
pub const ma_data_source_node = extern struct {
    base: ma_node_base,
    pDataSource: ?*ma_data_source,
};
pub extern fn ma_data_source_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_data_source_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pDataSourceNode: [*c]ma_data_source_node) ma_result;
pub extern fn ma_data_source_node_uninit(pDataSourceNode: [*c]ma_data_source_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_data_source_node_set_looping(pDataSourceNode: [*c]ma_data_source_node, isLooping: ma_bool32) ma_result;
pub extern fn ma_data_source_node_is_looping(pDataSourceNode: [*c]ma_data_source_node) ma_bool32;
pub const ma_splitter_node_config = extern struct {
    nodeConfig: ma_node_config,
    channels: ma_uint32,
};
pub extern fn ma_splitter_node_config_init(channels: ma_uint32) ma_splitter_node_config;
pub const ma_splitter_node = extern struct {
    base: ma_node_base,
};
pub extern fn ma_splitter_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_splitter_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pSplitterNode: [*c]ma_splitter_node) ma_result;
pub extern fn ma_splitter_node_uninit(pSplitterNode: [*c]ma_splitter_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_biquad_node_config = extern struct {
    nodeConfig: ma_node_config,
    biquad: ma_biquad_config,
};
pub extern fn ma_biquad_node_config_init(channels: ma_uint32, b0: f32, b1: f32, b2: f32, a0: f32, a1: f32, a2: f32) ma_biquad_node_config;
pub const ma_biquad_node = extern struct {
    baseNode: ma_node_base,
    biquad: ma_biquad,
};
pub extern fn ma_biquad_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_biquad_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_biquad_node) ma_result;
pub extern fn ma_biquad_node_reinit(pConfig: [*c]const ma_biquad_config, pNode: [*c]ma_biquad_node) ma_result;
pub extern fn ma_biquad_node_uninit(pNode: [*c]ma_biquad_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_lpf_node_config = extern struct {
    nodeConfig: ma_node_config,
    lpf: ma_lpf_config,
};
pub extern fn ma_lpf_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_lpf_node_config;
pub const ma_lpf_node = extern struct {
    baseNode: ma_node_base,
    lpf: ma_lpf,
};
pub extern fn ma_lpf_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_lpf_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_lpf_node) ma_result;
pub extern fn ma_lpf_node_reinit(pConfig: [*c]const ma_lpf_config, pNode: [*c]ma_lpf_node) ma_result;
pub extern fn ma_lpf_node_uninit(pNode: [*c]ma_lpf_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_hpf_node_config = extern struct {
    nodeConfig: ma_node_config,
    hpf: ma_hpf_config,
};
pub extern fn ma_hpf_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_hpf_node_config;
pub const ma_hpf_node = extern struct {
    baseNode: ma_node_base,
    hpf: ma_hpf,
};
pub extern fn ma_hpf_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_hpf_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_hpf_node) ma_result;
pub extern fn ma_hpf_node_reinit(pConfig: [*c]const ma_hpf_config, pNode: [*c]ma_hpf_node) ma_result;
pub extern fn ma_hpf_node_uninit(pNode: [*c]ma_hpf_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_bpf_node_config = extern struct {
    nodeConfig: ma_node_config,
    bpf: ma_bpf_config,
};
pub extern fn ma_bpf_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, cutoffFrequency: f64, order: ma_uint32) ma_bpf_node_config;
pub const ma_bpf_node = extern struct {
    baseNode: ma_node_base,
    bpf: ma_bpf,
};
pub extern fn ma_bpf_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_bpf_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_bpf_node) ma_result;
pub extern fn ma_bpf_node_reinit(pConfig: [*c]const ma_bpf_config, pNode: [*c]ma_bpf_node) ma_result;
pub extern fn ma_bpf_node_uninit(pNode: [*c]ma_bpf_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_notch_node_config = extern struct {
    nodeConfig: ma_node_config,
    notch: ma_notch_config,
};
pub extern fn ma_notch_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, q: f64, frequency: f64) ma_notch_node_config;
pub const ma_notch_node = extern struct {
    baseNode: ma_node_base,
    notch: ma_notch2,
};
pub extern fn ma_notch_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_notch_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_notch_node) ma_result;
pub extern fn ma_notch_node_reinit(pConfig: [*c]const ma_notch_config, pNode: [*c]ma_notch_node) ma_result;
pub extern fn ma_notch_node_uninit(pNode: [*c]ma_notch_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_peak_node_config = extern struct {
    nodeConfig: ma_node_config,
    peak: ma_peak_config,
};
pub extern fn ma_peak_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, q: f64, frequency: f64) ma_peak_node_config;
pub const ma_peak_node = extern struct {
    baseNode: ma_node_base,
    peak: ma_peak2,
};
pub extern fn ma_peak_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_peak_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_peak_node) ma_result;
pub extern fn ma_peak_node_reinit(pConfig: [*c]const ma_peak_config, pNode: [*c]ma_peak_node) ma_result;
pub extern fn ma_peak_node_uninit(pNode: [*c]ma_peak_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_loshelf_node_config = extern struct {
    nodeConfig: ma_node_config,
    loshelf: ma_loshelf_config,
};
pub extern fn ma_loshelf_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, q: f64, frequency: f64) ma_loshelf_node_config;
pub const ma_loshelf_node = extern struct {
    baseNode: ma_node_base,
    loshelf: ma_loshelf2,
};
pub extern fn ma_loshelf_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_loshelf_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_loshelf_node) ma_result;
pub extern fn ma_loshelf_node_reinit(pConfig: [*c]const ma_loshelf_config, pNode: [*c]ma_loshelf_node) ma_result;
pub extern fn ma_loshelf_node_uninit(pNode: [*c]ma_loshelf_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_hishelf_node_config = extern struct {
    nodeConfig: ma_node_config,
    hishelf: ma_hishelf_config,
};
pub extern fn ma_hishelf_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, gainDB: f64, q: f64, frequency: f64) ma_hishelf_node_config;
pub const ma_hishelf_node = extern struct {
    baseNode: ma_node_base,
    hishelf: ma_hishelf2,
};
pub extern fn ma_hishelf_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_hishelf_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pNode: [*c]ma_hishelf_node) ma_result;
pub extern fn ma_hishelf_node_reinit(pConfig: [*c]const ma_hishelf_config, pNode: [*c]ma_hishelf_node) ma_result;
pub extern fn ma_hishelf_node_uninit(pNode: [*c]ma_hishelf_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_delay_node_config = extern struct {
    nodeConfig: ma_node_config,
    delay: ma_delay_config,
};
pub extern fn ma_delay_node_config_init(channels: ma_uint32, sampleRate: ma_uint32, delayInFrames: ma_uint32, decay: f32) ma_delay_node_config;
pub const ma_delay_node = extern struct {
    baseNode: ma_node_base,
    delay: ma_delay,
};
pub extern fn ma_delay_node_init(pNodeGraph: [*c]ma_node_graph, pConfig: [*c]const ma_delay_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pDelayNode: [*c]ma_delay_node) ma_result;
pub extern fn ma_delay_node_uninit(pDelayNode: [*c]ma_delay_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub extern fn ma_delay_node_set_wet(pDelayNode: [*c]ma_delay_node, value: f32) void;
pub extern fn ma_delay_node_get_wet(pDelayNode: [*c]const ma_delay_node) f32;
pub extern fn ma_delay_node_set_dry(pDelayNode: [*c]ma_delay_node, value: f32) void;
pub extern fn ma_delay_node_get_dry(pDelayNode: [*c]const ma_delay_node) f32;
pub extern fn ma_delay_node_set_decay(pDelayNode: [*c]ma_delay_node, value: f32) void;
pub extern fn ma_delay_node_get_decay(pDelayNode: [*c]const ma_delay_node) f32;
pub const struct_ma_sound = extern struct {
    engineNode: ma_engine_node,
    pDataSource: ?*ma_data_source,
    seekTarget: ma_uint64 align(8),
    atEnd: ma_bool32 align(4),
    ownsDataSource: ma_bool8,
    pResourceManagerDataSource: [*c]ma_resource_manager_data_source,
};
pub const ma_sound = struct_ma_sound;
pub const struct_ma_sound_inlined = extern struct {
    sound: ma_sound,
    pNext: [*c]ma_sound_inlined,
    pPrev: [*c]ma_sound_inlined,
};
pub const ma_sound_inlined = struct_ma_sound_inlined;
pub const struct_ma_engine = extern struct {
    nodeGraph: ma_node_graph,
    pResourceManager: [*c]ma_resource_manager,
    pDevice: [*c]ma_device,
    pLog: [*c]ma_log,
    sampleRate: ma_uint32,
    listenerCount: ma_uint32,
    listeners: [4]ma_spatializer_listener,
    allocationCallbacks: ma_allocation_callbacks,
    ownsResourceManager: ma_bool8,
    ownsDevice: ma_bool8,
    inlinedSoundLock: ma_spinlock,
    pInlinedSoundHead: [*c]ma_sound_inlined,
    inlinedSoundCount: ma_uint32 align(4),
    gainSmoothTimeInFrames: ma_uint32,
    monoExpansionMode: ma_mono_expansion_mode,
};
pub const ma_engine = struct_ma_engine;
pub const MA_SOUND_FLAG_STREAM: c_int = 1;
pub const MA_SOUND_FLAG_DECODE: c_int = 2;
pub const MA_SOUND_FLAG_ASYNC: c_int = 4;
pub const MA_SOUND_FLAG_WAIT_INIT: c_int = 8;
pub const MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT: c_int = 16;
pub const MA_SOUND_FLAG_NO_PITCH: c_int = 32;
pub const MA_SOUND_FLAG_NO_SPATIALIZATION: c_int = 64;
pub const ma_sound_flags = c_uint;
pub const ma_engine_node_type_sound: c_int = 0;
pub const ma_engine_node_type_group: c_int = 1;
pub const ma_engine_node_type = c_uint;
pub const ma_engine_node_config = extern struct {
    pEngine: [*c]ma_engine,
    type: ma_engine_node_type,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    sampleRate: ma_uint32,
    isPitchDisabled: ma_bool8,
    isSpatializationDisabled: ma_bool8,
    pinnedListenerIndex: ma_uint8,
};
pub extern fn ma_engine_node_config_init(pEngine: [*c]ma_engine, @"type": ma_engine_node_type, flags: ma_uint32) ma_engine_node_config;
pub const ma_engine_node = extern struct {
    baseNode: ma_node_base,
    pEngine: [*c]ma_engine,
    sampleRate: ma_uint32,
    fader: ma_fader,
    resampler: ma_linear_resampler,
    spatializer: ma_spatializer,
    panner: ma_panner,
    pitch: f32 align(4),
    oldPitch: f32,
    oldDopplerPitch: f32,
    isPitchDisabled: ma_bool32 align(4),
    isSpatializationDisabled: ma_bool32 align(4),
    pinnedListenerIndex: ma_uint32 align(4),
    _ownsHeap: ma_bool8,
    _pHeap: ?*anyopaque,
};
pub extern fn ma_engine_node_get_heap_size(pConfig: [*c]const ma_engine_node_config, pHeapSizeInBytes: [*c]usize) ma_result;
pub extern fn ma_engine_node_init_preallocated(pConfig: [*c]const ma_engine_node_config, pHeap: ?*anyopaque, pEngineNode: [*c]ma_engine_node) ma_result;
pub extern fn ma_engine_node_init(pConfig: [*c]const ma_engine_node_config, pAllocationCallbacks: [*c]const ma_allocation_callbacks, pEngineNode: [*c]ma_engine_node) ma_result;
pub extern fn ma_engine_node_uninit(pEngineNode: [*c]ma_engine_node, pAllocationCallbacks: [*c]const ma_allocation_callbacks) void;
pub const ma_sound_config = extern struct {
    pFilePath: [*c]const u8,
    pFilePathW: [*c]const wchar_t,
    pDataSource: ?*ma_data_source,
    pInitialAttachment: ?*ma_node,
    initialAttachmentInputBusIndex: ma_uint32,
    channelsIn: ma_uint32,
    channelsOut: ma_uint32,
    flags: ma_uint32,
    initialSeekPointInPCMFrames: ma_uint64,
    rangeBegInPCMFrames: ma_uint64,
    rangeEndInPCMFrames: ma_uint64,
    loopPointBegInPCMFrames: ma_uint64,
    loopPointEndInPCMFrames: ma_uint64,
    isLooping: ma_bool32,
    pDoneFence: [*c]ma_fence,
};
pub extern fn ma_sound_config_init() ma_sound_config;
pub const ma_sound_group_config = ma_sound_config;
pub const ma_sound_group = ma_sound;
pub extern fn ma_sound_group_config_init() ma_sound_group_config;
pub const ma_engine_config = extern struct {
    pResourceManager: [*c]ma_resource_manager,
    pContext: [*c]ma_context,
    pDevice: [*c]ma_device,
    pPlaybackDeviceID: [*c]ma_device_id,
    pLog: [*c]ma_log,
    listenerCount: ma_uint32,
    channels: ma_uint32,
    sampleRate: ma_uint32,
    periodSizeInFrames: ma_uint32,
    periodSizeInMilliseconds: ma_uint32,
    gainSmoothTimeInFrames: ma_uint32,
    gainSmoothTimeInMilliseconds: ma_uint32,
    allocationCallbacks: ma_allocation_callbacks,
    noAutoStart: ma_bool32,
    noDevice: ma_bool32,
    monoExpansionMode: ma_mono_expansion_mode,
    pResourceManagerVFS: ?*ma_vfs,
};
pub extern fn ma_engine_config_init() ma_engine_config;
pub extern fn ma_engine_init(pConfig: [*c]const ma_engine_config, pEngine: [*c]ma_engine) ma_result;
pub extern fn ma_engine_uninit(pEngine: [*c]ma_engine) void;
pub extern fn ma_engine_read_pcm_frames(pEngine: [*c]ma_engine, pFramesOut: ?*anyopaque, frameCount: ma_uint64, pFramesRead: [*c]ma_uint64) ma_result;
pub extern fn ma_engine_get_node_graph(pEngine: [*c]ma_engine) [*c]ma_node_graph;
pub extern fn ma_engine_get_resource_manager(pEngine: [*c]ma_engine) [*c]ma_resource_manager;
pub extern fn ma_engine_get_device(pEngine: [*c]ma_engine) [*c]ma_device;
pub extern fn ma_engine_get_log(pEngine: [*c]ma_engine) [*c]ma_log;
pub extern fn ma_engine_get_endpoint(pEngine: [*c]ma_engine) ?*ma_node;
pub extern fn ma_engine_get_time(pEngine: [*c]const ma_engine) ma_uint64;
pub extern fn ma_engine_set_time(pEngine: [*c]ma_engine, globalTime: ma_uint64) ma_result;
pub extern fn ma_engine_get_channels(pEngine: [*c]const ma_engine) ma_uint32;
pub extern fn ma_engine_get_sample_rate(pEngine: [*c]const ma_engine) ma_uint32;
pub extern fn ma_engine_start(pEngine: [*c]ma_engine) ma_result;
pub extern fn ma_engine_stop(pEngine: [*c]ma_engine) ma_result;
pub extern fn ma_engine_set_volume(pEngine: [*c]ma_engine, volume: f32) ma_result;
pub extern fn ma_engine_set_gain_db(pEngine: [*c]ma_engine, gainDB: f32) ma_result;
pub extern fn ma_engine_get_listener_count(pEngine: [*c]const ma_engine) ma_uint32;
pub extern fn ma_engine_find_closest_listener(pEngine: [*c]const ma_engine, absolutePosX: f32, absolutePosY: f32, absolutePosZ: f32) ma_uint32;
pub extern fn ma_engine_listener_set_position(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, x: f32, y: f32, z: f32) void;
pub extern fn ma_engine_listener_get_position(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32) ma_vec3f;
pub extern fn ma_engine_listener_set_direction(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, x: f32, y: f32, z: f32) void;
pub extern fn ma_engine_listener_get_direction(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32) ma_vec3f;
pub extern fn ma_engine_listener_set_velocity(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, x: f32, y: f32, z: f32) void;
pub extern fn ma_engine_listener_get_velocity(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32) ma_vec3f;
pub extern fn ma_engine_listener_set_cone(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, innerAngleInRadians: f32, outerAngleInRadians: f32, outerGain: f32) void;
pub extern fn ma_engine_listener_get_cone(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32, pInnerAngleInRadians: [*c]f32, pOuterAngleInRadians: [*c]f32, pOuterGain: [*c]f32) void;
pub extern fn ma_engine_listener_set_world_up(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, x: f32, y: f32, z: f32) void;
pub extern fn ma_engine_listener_get_world_up(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32) ma_vec3f;
pub extern fn ma_engine_listener_set_enabled(pEngine: [*c]ma_engine, listenerIndex: ma_uint32, isEnabled: ma_bool32) void;
pub extern fn ma_engine_listener_is_enabled(pEngine: [*c]const ma_engine, listenerIndex: ma_uint32) ma_bool32;
pub extern fn ma_engine_play_sound_ex(pEngine: [*c]ma_engine, pFilePath: [*c]const u8, pNode: ?*ma_node, nodeInputBusIndex: ma_uint32) ma_result;
pub extern fn ma_engine_play_sound(pEngine: [*c]ma_engine, pFilePath: [*c]const u8, pGroup: [*c]ma_sound_group) ma_result;
pub extern fn ma_sound_init_from_file(pEngine: [*c]ma_engine, pFilePath: [*c]const u8, flags: ma_uint32, pGroup: [*c]ma_sound_group, pDoneFence: [*c]ma_fence, pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_init_from_file_w(pEngine: [*c]ma_engine, pFilePath: [*c]const wchar_t, flags: ma_uint32, pGroup: [*c]ma_sound_group, pDoneFence: [*c]ma_fence, pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_init_copy(pEngine: [*c]ma_engine, pExistingSound: [*c]const ma_sound, flags: ma_uint32, pGroup: [*c]ma_sound_group, pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_init_from_data_source(pEngine: [*c]ma_engine, pDataSource: ?*ma_data_source, flags: ma_uint32, pGroup: [*c]ma_sound_group, pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_init_ex(pEngine: [*c]ma_engine, pConfig: [*c]const ma_sound_config, pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_uninit(pSound: [*c]ma_sound) void;
pub extern fn ma_sound_get_engine(pSound: [*c]const ma_sound) [*c]ma_engine;
pub extern fn ma_sound_get_data_source(pSound: [*c]const ma_sound) ?*ma_data_source;
pub extern fn ma_sound_start(pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_stop(pSound: [*c]ma_sound) ma_result;
pub extern fn ma_sound_set_volume(pSound: [*c]ma_sound, volume: f32) void;
pub extern fn ma_sound_get_volume(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_pan(pSound: [*c]ma_sound, pan: f32) void;
pub extern fn ma_sound_get_pan(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_pan_mode(pSound: [*c]ma_sound, panMode: ma_pan_mode) void;
pub extern fn ma_sound_get_pan_mode(pSound: [*c]const ma_sound) ma_pan_mode;
pub extern fn ma_sound_set_pitch(pSound: [*c]ma_sound, pitch: f32) void;
pub extern fn ma_sound_get_pitch(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_spatialization_enabled(pSound: [*c]ma_sound, enabled: ma_bool32) void;
pub extern fn ma_sound_is_spatialization_enabled(pSound: [*c]const ma_sound) ma_bool32;
pub extern fn ma_sound_set_pinned_listener_index(pSound: [*c]ma_sound, listenerIndex: ma_uint32) void;
pub extern fn ma_sound_get_pinned_listener_index(pSound: [*c]const ma_sound) ma_uint32;
pub extern fn ma_sound_get_listener_index(pSound: [*c]const ma_sound) ma_uint32;
pub extern fn ma_sound_get_direction_to_listener(pSound: [*c]const ma_sound) ma_vec3f;
pub extern fn ma_sound_set_position(pSound: [*c]ma_sound, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_get_position(pSound: [*c]const ma_sound) ma_vec3f;
pub extern fn ma_sound_set_direction(pSound: [*c]ma_sound, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_get_direction(pSound: [*c]const ma_sound) ma_vec3f;
pub extern fn ma_sound_set_velocity(pSound: [*c]ma_sound, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_get_velocity(pSound: [*c]const ma_sound) ma_vec3f;
pub extern fn ma_sound_set_attenuation_model(pSound: [*c]ma_sound, attenuationModel: ma_attenuation_model) void;
pub extern fn ma_sound_get_attenuation_model(pSound: [*c]const ma_sound) ma_attenuation_model;
pub extern fn ma_sound_set_positioning(pSound: [*c]ma_sound, positioning: ma_positioning) void;
pub extern fn ma_sound_get_positioning(pSound: [*c]const ma_sound) ma_positioning;
pub extern fn ma_sound_set_rolloff(pSound: [*c]ma_sound, rolloff: f32) void;
pub extern fn ma_sound_get_rolloff(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_min_gain(pSound: [*c]ma_sound, minGain: f32) void;
pub extern fn ma_sound_get_min_gain(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_max_gain(pSound: [*c]ma_sound, maxGain: f32) void;
pub extern fn ma_sound_get_max_gain(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_min_distance(pSound: [*c]ma_sound, minDistance: f32) void;
pub extern fn ma_sound_get_min_distance(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_max_distance(pSound: [*c]ma_sound, maxDistance: f32) void;
pub extern fn ma_sound_get_max_distance(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_cone(pSound: [*c]ma_sound, innerAngleInRadians: f32, outerAngleInRadians: f32, outerGain: f32) void;
pub extern fn ma_sound_get_cone(pSound: [*c]const ma_sound, pInnerAngleInRadians: [*c]f32, pOuterAngleInRadians: [*c]f32, pOuterGain: [*c]f32) void;
pub extern fn ma_sound_set_doppler_factor(pSound: [*c]ma_sound, dopplerFactor: f32) void;
pub extern fn ma_sound_get_doppler_factor(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_directional_attenuation_factor(pSound: [*c]ma_sound, directionalAttenuationFactor: f32) void;
pub extern fn ma_sound_get_directional_attenuation_factor(pSound: [*c]const ma_sound) f32;
pub extern fn ma_sound_set_fade_in_pcm_frames(pSound: [*c]ma_sound, volumeBeg: f32, volumeEnd: f32, fadeLengthInFrames: ma_uint64) void;
pub extern fn ma_sound_set_fade_in_milliseconds(pSound: [*c]ma_sound, volumeBeg: f32, volumeEnd: f32, fadeLengthInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_get_current_fade_volume(pSound: [*c]ma_sound) f32;
pub extern fn ma_sound_set_start_time_in_pcm_frames(pSound: [*c]ma_sound, absoluteGlobalTimeInFrames: ma_uint64) void;
pub extern fn ma_sound_set_start_time_in_milliseconds(pSound: [*c]ma_sound, absoluteGlobalTimeInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_set_stop_time_in_pcm_frames(pSound: [*c]ma_sound, absoluteGlobalTimeInFrames: ma_uint64) void;
pub extern fn ma_sound_set_stop_time_in_milliseconds(pSound: [*c]ma_sound, absoluteGlobalTimeInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_is_playing(pSound: [*c]const ma_sound) ma_bool32;
pub extern fn ma_sound_get_time_in_pcm_frames(pSound: [*c]const ma_sound) ma_uint64;
pub extern fn ma_sound_set_looping(pSound: [*c]ma_sound, isLooping: ma_bool32) void;
pub extern fn ma_sound_is_looping(pSound: [*c]const ma_sound) ma_bool32;
pub extern fn ma_sound_at_end(pSound: [*c]const ma_sound) ma_bool32;
pub extern fn ma_sound_seek_to_pcm_frame(pSound: [*c]ma_sound, frameIndex: ma_uint64) ma_result;
pub extern fn ma_sound_get_data_format(pSound: [*c]ma_sound, pFormat: [*c]ma_format, pChannels: [*c]ma_uint32, pSampleRate: [*c]ma_uint32, pChannelMap: [*c]ma_channel, channelMapCap: usize) ma_result;
pub extern fn ma_sound_get_cursor_in_pcm_frames(pSound: [*c]ma_sound, pCursor: [*c]ma_uint64) ma_result;
pub extern fn ma_sound_get_length_in_pcm_frames(pSound: [*c]ma_sound, pLength: [*c]ma_uint64) ma_result;
pub extern fn ma_sound_get_cursor_in_seconds(pSound: [*c]ma_sound, pCursor: [*c]f32) ma_result;
pub extern fn ma_sound_get_length_in_seconds(pSound: [*c]ma_sound, pLength: [*c]f32) ma_result;
pub extern fn ma_sound_group_init(pEngine: [*c]ma_engine, flags: ma_uint32, pParentGroup: [*c]ma_sound_group, pGroup: [*c]ma_sound_group) ma_result;
pub extern fn ma_sound_group_init_ex(pEngine: [*c]ma_engine, pConfig: [*c]const ma_sound_group_config, pGroup: [*c]ma_sound_group) ma_result;
pub extern fn ma_sound_group_uninit(pGroup: [*c]ma_sound_group) void;
pub extern fn ma_sound_group_get_engine(pGroup: [*c]const ma_sound_group) [*c]ma_engine;
pub extern fn ma_sound_group_start(pGroup: [*c]ma_sound_group) ma_result;
pub extern fn ma_sound_group_stop(pGroup: [*c]ma_sound_group) ma_result;
pub extern fn ma_sound_group_set_volume(pGroup: [*c]ma_sound_group, volume: f32) void;
pub extern fn ma_sound_group_get_volume(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_pan(pGroup: [*c]ma_sound_group, pan: f32) void;
pub extern fn ma_sound_group_get_pan(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_pan_mode(pGroup: [*c]ma_sound_group, panMode: ma_pan_mode) void;
pub extern fn ma_sound_group_get_pan_mode(pGroup: [*c]const ma_sound_group) ma_pan_mode;
pub extern fn ma_sound_group_set_pitch(pGroup: [*c]ma_sound_group, pitch: f32) void;
pub extern fn ma_sound_group_get_pitch(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_spatialization_enabled(pGroup: [*c]ma_sound_group, enabled: ma_bool32) void;
pub extern fn ma_sound_group_is_spatialization_enabled(pGroup: [*c]const ma_sound_group) ma_bool32;
pub extern fn ma_sound_group_set_pinned_listener_index(pGroup: [*c]ma_sound_group, listenerIndex: ma_uint32) void;
pub extern fn ma_sound_group_get_pinned_listener_index(pGroup: [*c]const ma_sound_group) ma_uint32;
pub extern fn ma_sound_group_get_listener_index(pGroup: [*c]const ma_sound_group) ma_uint32;
pub extern fn ma_sound_group_get_direction_to_listener(pGroup: [*c]const ma_sound_group) ma_vec3f;
pub extern fn ma_sound_group_set_position(pGroup: [*c]ma_sound_group, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_group_get_position(pGroup: [*c]const ma_sound_group) ma_vec3f;
pub extern fn ma_sound_group_set_direction(pGroup: [*c]ma_sound_group, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_group_get_direction(pGroup: [*c]const ma_sound_group) ma_vec3f;
pub extern fn ma_sound_group_set_velocity(pGroup: [*c]ma_sound_group, x: f32, y: f32, z: f32) void;
pub extern fn ma_sound_group_get_velocity(pGroup: [*c]const ma_sound_group) ma_vec3f;
pub extern fn ma_sound_group_set_attenuation_model(pGroup: [*c]ma_sound_group, attenuationModel: ma_attenuation_model) void;
pub extern fn ma_sound_group_get_attenuation_model(pGroup: [*c]const ma_sound_group) ma_attenuation_model;
pub extern fn ma_sound_group_set_positioning(pGroup: [*c]ma_sound_group, positioning: ma_positioning) void;
pub extern fn ma_sound_group_get_positioning(pGroup: [*c]const ma_sound_group) ma_positioning;
pub extern fn ma_sound_group_set_rolloff(pGroup: [*c]ma_sound_group, rolloff: f32) void;
pub extern fn ma_sound_group_get_rolloff(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_min_gain(pGroup: [*c]ma_sound_group, minGain: f32) void;
pub extern fn ma_sound_group_get_min_gain(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_max_gain(pGroup: [*c]ma_sound_group, maxGain: f32) void;
pub extern fn ma_sound_group_get_max_gain(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_min_distance(pGroup: [*c]ma_sound_group, minDistance: f32) void;
pub extern fn ma_sound_group_get_min_distance(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_max_distance(pGroup: [*c]ma_sound_group, maxDistance: f32) void;
pub extern fn ma_sound_group_get_max_distance(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_cone(pGroup: [*c]ma_sound_group, innerAngleInRadians: f32, outerAngleInRadians: f32, outerGain: f32) void;
pub extern fn ma_sound_group_get_cone(pGroup: [*c]const ma_sound_group, pInnerAngleInRadians: [*c]f32, pOuterAngleInRadians: [*c]f32, pOuterGain: [*c]f32) void;
pub extern fn ma_sound_group_set_doppler_factor(pGroup: [*c]ma_sound_group, dopplerFactor: f32) void;
pub extern fn ma_sound_group_get_doppler_factor(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_directional_attenuation_factor(pGroup: [*c]ma_sound_group, directionalAttenuationFactor: f32) void;
pub extern fn ma_sound_group_get_directional_attenuation_factor(pGroup: [*c]const ma_sound_group) f32;
pub extern fn ma_sound_group_set_fade_in_pcm_frames(pGroup: [*c]ma_sound_group, volumeBeg: f32, volumeEnd: f32, fadeLengthInFrames: ma_uint64) void;
pub extern fn ma_sound_group_set_fade_in_milliseconds(pGroup: [*c]ma_sound_group, volumeBeg: f32, volumeEnd: f32, fadeLengthInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_group_get_current_fade_volume(pGroup: [*c]ma_sound_group) f32;
pub extern fn ma_sound_group_set_start_time_in_pcm_frames(pGroup: [*c]ma_sound_group, absoluteGlobalTimeInFrames: ma_uint64) void;
pub extern fn ma_sound_group_set_start_time_in_milliseconds(pGroup: [*c]ma_sound_group, absoluteGlobalTimeInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_group_set_stop_time_in_pcm_frames(pGroup: [*c]ma_sound_group, absoluteGlobalTimeInFrames: ma_uint64) void;
pub extern fn ma_sound_group_set_stop_time_in_milliseconds(pGroup: [*c]ma_sound_group, absoluteGlobalTimeInMilliseconds: ma_uint64) void;
pub extern fn ma_sound_group_is_playing(pGroup: [*c]const ma_sound_group) ma_bool32;
pub extern fn ma_sound_group_get_time_in_pcm_frames(pGroup: [*c]const ma_sound_group) ma_uint64;
pub const __INTMAX_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `L`"); // (no file):80:9
pub const __UINTMAX_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `UL`"); // (no file):86:9
pub const __INT64_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `L`"); // (no file):169:9
pub const __UINT32_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `U`"); // (no file):191:9
pub const __UINT64_C_SUFFIX__ = @compileError("unable to translate macro: undefined identifier `UL`"); // (no file):199:9
pub const __seg_gs = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // (no file):329:9
pub const __seg_fs = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // (no file):330:9
pub const MA_STRINGIFY = @compileError("unable to translate C expr: unexpected token '#'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3638:9
pub const MA_VERSION_STRING = @compileError("unable to translate C expr: unexpected token 'StringLiteral'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3644:9
pub const offsetof = @compileError("unable to translate macro: undefined identifier `__builtin_offsetof`"); // /home/michal/zig/lib/include/stddef.h:104:9
pub const __GLIBC_USE = @compileError("unable to translate macro: undefined identifier `__GLIBC_USE_`"); // /home/michal/zig/lib/libc/include/generic-glibc/features.h:186:9
pub const __glibc_has_attribute = @compileError("unable to translate macro: undefined identifier `__has_attribute`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:44:10
pub const __glibc_has_extension = @compileError("unable to translate macro: undefined identifier `__has_extension`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:54:10
pub const __THROW = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:78:11
pub const __THROWNL = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:79:11
pub const __NTH = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:80:11
pub const __NTHNL = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:81:11
pub const __CONCAT = @compileError("unable to translate C expr: unexpected token '##'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:123:9
pub const __STRING = @compileError("unable to translate C expr: unexpected token '#'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:124:9
pub const __warnattr = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:158:10
pub const __errordecl = @compileError("unable to translate C expr: unexpected token 'extern'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:159:10
pub const __flexarr = @compileError("unable to translate C expr: unexpected token '['"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:167:10
pub const __REDIRECT = @compileError("unable to translate macro: undefined identifier `__asm__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:198:10
pub const __REDIRECT_NTH = @compileError("unable to translate macro: undefined identifier `__asm__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:205:11
pub const __REDIRECT_NTHNL = @compileError("unable to translate macro: undefined identifier `__asm__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:207:11
pub const __ASMNAME2 = @compileError("unable to translate C expr: unexpected token 'Identifier'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:211:10
pub const __attribute_malloc__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:232:10
pub const __attribute_alloc_size__ = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:243:10
pub const __attribute_pure__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:250:10
pub const __attribute_const__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:257:10
pub const __attribute_maybe_unused__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:263:10
pub const __attribute_used__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:272:10
pub const __attribute_noinline__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:273:10
pub const __attribute_deprecated__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:281:10
pub const __attribute_deprecated_msg__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:291:10
pub const __attribute_format_arg__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:304:10
pub const __attribute_format_strfmon__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:314:10
pub const __nonnull = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:324:11
pub const __returns_nonnull = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:337:10
pub const __attribute_warn_unused_result__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:346:10
pub const __always_inline = @compileError("unable to translate macro: undefined identifier `__inline`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:364:10
pub const __attribute_artificial__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:373:10
pub const __extern_inline = @compileError("unable to translate macro: undefined identifier `__inline`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:391:11
pub const __extern_always_inline = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:392:11
pub const __restrict_arr = @compileError("unable to translate macro: undefined identifier `__restrict`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:435:10
pub const __attribute_copy__ = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:484:10
pub const __LDBL_REDIR2_DECL = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:560:10
pub const __LDBL_REDIR_DECL = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:561:10
pub const __glibc_macro_warning1 = @compileError("unable to translate macro: undefined identifier `_Pragma`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:575:10
pub const __glibc_macro_warning = @compileError("unable to translate macro: undefined identifier `GCC`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:576:10
pub const __attr_access = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:612:11
pub const __attr_access_none = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:613:11
pub const __attr_dealloc = @compileError("unable to translate C expr: unexpected token 'Eof'"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:623:10
pub const __attribute_returns_twice__ = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/zig/lib/libc/include/generic-glibc/sys/cdefs.h:630:10
pub const __STD_TYPE = @compileError("unable to translate C expr: unexpected token 'typedef'"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/types.h:137:10
pub const __FSID_T_TYPE = @compileError("unable to translate macro: undefined identifier `__val`"); // /home/michal/zig/lib/libc/include/x86_64-linux-gnu/bits/typesizes.h:73:9
pub const __CPU_ZERO_S = @compileError("unable to translate C expr: unexpected token 'do'"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:46:10
pub const __CPU_SET_S = @compileError("unable to translate macro: undefined identifier `__extension__`"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:58:9
pub const __CPU_CLR_S = @compileError("unable to translate macro: undefined identifier `__extension__`"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:65:9
pub const __CPU_ISSET_S = @compileError("unable to translate macro: undefined identifier `__extension__`"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:72:9
pub const __CPU_EQUAL_S = @compileError("unable to translate macro: undefined identifier `__builtin_memcmp`"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:84:10
pub const __CPU_OP_S = @compileError("unable to translate macro: undefined identifier `__extension__`"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/cpu-set.h:99:9
pub const __PTHREAD_MUTEX_INITIALIZER = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/x86_64-linux-gnu/bits/struct_mutex.h:56:10
pub const __PTHREAD_RWLOCK_ELISION_EXTRA = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/x86_64-linux-gnu/bits/struct_rwlock.h:40:11
pub const __ONCE_FLAG_INIT = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/generic-glibc/bits/thread-shared-types.h:127:9
pub const PTHREAD_MUTEX_INITIALIZER = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/generic-glibc/pthread.h:90:9
pub const PTHREAD_RWLOCK_INITIALIZER = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/generic-glibc/pthread.h:114:10
pub const PTHREAD_COND_INITIALIZER = @compileError("unable to translate C expr: unexpected token '{'"); // /home/michal/zig/lib/libc/include/generic-glibc/pthread.h:155:9
pub const pthread_cleanup_push = @compileError("unable to translate macro: undefined identifier `__cancel_buf`"); // /home/michal/zig/lib/libc/include/generic-glibc/pthread.h:681:10
pub const pthread_cleanup_pop = @compileError("unable to translate macro: undefined identifier `__cancel_buf`"); // /home/michal/zig/lib/libc/include/generic-glibc/pthread.h:702:10
pub const MA_GNUC_INLINE_HINT = @compileError("unable to translate C expr: unexpected token 'inline'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3802:17
pub const MA_INLINE = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3806:17
pub const MA_API = @compileError("unable to translate C expr: unexpected token 'extern'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3841:17
pub const MA_PRIVATE = @compileError("unable to translate C expr: unexpected token 'static'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3842:17
pub const alignas = @compileError("unable to translate C expr: unexpected token '_Alignas'"); // /home/michal/zig/lib/include/stdalign.h:14:9
pub const alignof = @compileError("unable to translate C expr: expected '(' instead got 'Eof'"); // /home/michal/zig/lib/include/stdalign.h:15:9
pub const MA_ATOMIC = @compileError("unable to translate C expr: unexpected token 'Identifier'"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:3895:13
pub const va_start = @compileError("unable to translate macro: undefined identifier `__builtin_va_start`"); // /home/michal/zig/lib/include/stdarg.h:17:9
pub const va_end = @compileError("unable to translate macro: undefined identifier `__builtin_va_end`"); // /home/michal/zig/lib/include/stdarg.h:18:9
pub const va_arg = @compileError("unable to translate macro: undefined identifier `__builtin_va_arg`"); // /home/michal/zig/lib/include/stdarg.h:19:9
pub const __va_copy = @compileError("unable to translate macro: undefined identifier `__builtin_va_copy`"); // /home/michal/zig/lib/include/stdarg.h:24:9
pub const va_copy = @compileError("unable to translate macro: undefined identifier `__builtin_va_copy`"); // /home/michal/zig/lib/include/stdarg.h:27:9
pub const MA_ATTRIBUTE_FORMAT = @compileError("unable to translate macro: undefined identifier `__attribute__`"); // /home/michal/projects/zig-gamedev/libs/zaudio/libs/miniaudio/miniaudio.h:4246:17
pub const __llvm__ = @as(c_int, 1);
pub const __clang__ = @as(c_int, 1);
pub const __clang_major__ = @as(c_int, 14);
pub const __clang_minor__ = @as(c_int, 0);
pub const __clang_patchlevel__ = @as(c_int, 6);
pub const __clang_version__ = "14.0.6 (git@github.com:ziglang/zig-bootstrap.git dbc902054739800b8c1656dc1fb29571bba074b9)";
pub const __GNUC__ = @as(c_int, 4);
pub const __GNUC_MINOR__ = @as(c_int, 2);
pub const __GNUC_PATCHLEVEL__ = @as(c_int, 1);
pub const __GXX_ABI_VERSION = @as(c_int, 1002);
pub const __ATOMIC_RELAXED = @as(c_int, 0);
pub const __ATOMIC_CONSUME = @as(c_int, 1);
pub const __ATOMIC_ACQUIRE = @as(c_int, 2);
pub const __ATOMIC_RELEASE = @as(c_int, 3);
pub const __ATOMIC_ACQ_REL = @as(c_int, 4);
pub const __ATOMIC_SEQ_CST = @as(c_int, 5);
pub const __OPENCL_MEMORY_SCOPE_WORK_ITEM = @as(c_int, 0);
pub const __OPENCL_MEMORY_SCOPE_WORK_GROUP = @as(c_int, 1);
pub const __OPENCL_MEMORY_SCOPE_DEVICE = @as(c_int, 2);
pub const __OPENCL_MEMORY_SCOPE_ALL_SVM_DEVICES = @as(c_int, 3);
pub const __OPENCL_MEMORY_SCOPE_SUB_GROUP = @as(c_int, 4);
pub const __PRAGMA_REDEFINE_EXTNAME = @as(c_int, 1);
pub const __VERSION__ = "Clang 14.0.6 (git@github.com:ziglang/zig-bootstrap.git dbc902054739800b8c1656dc1fb29571bba074b9)";
pub const __OBJC_BOOL_IS_BOOL = @as(c_int, 0);
pub const __CONSTANT_CFSTRINGS__ = @as(c_int, 1);
pub const __clang_literal_encoding__ = "UTF-8";
pub const __clang_wide_literal_encoding__ = "UTF-32";
pub const __ORDER_LITTLE_ENDIAN__ = @as(c_int, 1234);
pub const __ORDER_BIG_ENDIAN__ = @as(c_int, 4321);
pub const __ORDER_PDP_ENDIAN__ = @as(c_int, 3412);
pub const __BYTE_ORDER__ = __ORDER_LITTLE_ENDIAN__;
pub const __LITTLE_ENDIAN__ = @as(c_int, 1);
pub const _LP64 = @as(c_int, 1);
pub const __LP64__ = @as(c_int, 1);
pub const __CHAR_BIT__ = @as(c_int, 8);
pub const __BOOL_WIDTH__ = @as(c_int, 8);
pub const __SHRT_WIDTH__ = @as(c_int, 16);
pub const __INT_WIDTH__ = @as(c_int, 32);
pub const __LONG_WIDTH__ = @as(c_int, 64);
pub const __LLONG_WIDTH__ = @as(c_int, 64);
pub const __BITINT_MAXWIDTH__ = @as(c_int, 128);
pub const __SCHAR_MAX__ = @as(c_int, 127);
pub const __SHRT_MAX__ = @as(c_int, 32767);
pub const __INT_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __LONG_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __LONG_LONG_MAX__ = @as(c_longlong, 9223372036854775807);
pub const __WCHAR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __WCHAR_WIDTH__ = @as(c_int, 32);
pub const __WINT_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __WINT_WIDTH__ = @as(c_int, 32);
pub const __INTMAX_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INTMAX_WIDTH__ = @as(c_int, 64);
pub const __SIZE_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __SIZE_WIDTH__ = @as(c_int, 64);
pub const __UINTMAX_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINTMAX_WIDTH__ = @as(c_int, 64);
pub const __PTRDIFF_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __PTRDIFF_WIDTH__ = @as(c_int, 64);
pub const __INTPTR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INTPTR_WIDTH__ = @as(c_int, 64);
pub const __UINTPTR_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINTPTR_WIDTH__ = @as(c_int, 64);
pub const __SIZEOF_DOUBLE__ = @as(c_int, 8);
pub const __SIZEOF_FLOAT__ = @as(c_int, 4);
pub const __SIZEOF_INT__ = @as(c_int, 4);
pub const __SIZEOF_LONG__ = @as(c_int, 8);
pub const __SIZEOF_LONG_DOUBLE__ = @as(c_int, 16);
pub const __SIZEOF_LONG_LONG__ = @as(c_int, 8);
pub const __SIZEOF_POINTER__ = @as(c_int, 8);
pub const __SIZEOF_SHORT__ = @as(c_int, 2);
pub const __SIZEOF_PTRDIFF_T__ = @as(c_int, 8);
pub const __SIZEOF_SIZE_T__ = @as(c_int, 8);
pub const __SIZEOF_WCHAR_T__ = @as(c_int, 4);
pub const __SIZEOF_WINT_T__ = @as(c_int, 4);
pub const __SIZEOF_INT128__ = @as(c_int, 16);
pub const __INTMAX_TYPE__ = c_long;
pub const __INTMAX_FMTd__ = "ld";
pub const __INTMAX_FMTi__ = "li";
pub const __UINTMAX_TYPE__ = c_ulong;
pub const __UINTMAX_FMTo__ = "lo";
pub const __UINTMAX_FMTu__ = "lu";
pub const __UINTMAX_FMTx__ = "lx";
pub const __UINTMAX_FMTX__ = "lX";
pub const __PTRDIFF_TYPE__ = c_long;
pub const __PTRDIFF_FMTd__ = "ld";
pub const __PTRDIFF_FMTi__ = "li";
pub const __INTPTR_TYPE__ = c_long;
pub const __INTPTR_FMTd__ = "ld";
pub const __INTPTR_FMTi__ = "li";
pub const __SIZE_TYPE__ = c_ulong;
pub const __SIZE_FMTo__ = "lo";
pub const __SIZE_FMTu__ = "lu";
pub const __SIZE_FMTx__ = "lx";
pub const __SIZE_FMTX__ = "lX";
pub const __WCHAR_TYPE__ = c_int;
pub const __WINT_TYPE__ = c_uint;
pub const __SIG_ATOMIC_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __SIG_ATOMIC_WIDTH__ = @as(c_int, 32);
pub const __CHAR16_TYPE__ = c_ushort;
pub const __CHAR32_TYPE__ = c_uint;
pub const __UINTPTR_TYPE__ = c_ulong;
pub const __UINTPTR_FMTo__ = "lo";
pub const __UINTPTR_FMTu__ = "lu";
pub const __UINTPTR_FMTx__ = "lx";
pub const __UINTPTR_FMTX__ = "lX";
pub const __FLT_DENORM_MIN__ = @as(f32, 1.40129846e-45);
pub const __FLT_HAS_DENORM__ = @as(c_int, 1);
pub const __FLT_DIG__ = @as(c_int, 6);
pub const __FLT_DECIMAL_DIG__ = @as(c_int, 9);
pub const __FLT_EPSILON__ = @as(f32, 1.19209290e-7);
pub const __FLT_HAS_INFINITY__ = @as(c_int, 1);
pub const __FLT_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __FLT_MANT_DIG__ = @as(c_int, 24);
pub const __FLT_MAX_10_EXP__ = @as(c_int, 38);
pub const __FLT_MAX_EXP__ = @as(c_int, 128);
pub const __FLT_MAX__ = @as(f32, 3.40282347e+38);
pub const __FLT_MIN_10_EXP__ = -@as(c_int, 37);
pub const __FLT_MIN_EXP__ = -@as(c_int, 125);
pub const __FLT_MIN__ = @as(f32, 1.17549435e-38);
pub const __DBL_DENORM_MIN__ = 4.9406564584124654e-324;
pub const __DBL_HAS_DENORM__ = @as(c_int, 1);
pub const __DBL_DIG__ = @as(c_int, 15);
pub const __DBL_DECIMAL_DIG__ = @as(c_int, 17);
pub const __DBL_EPSILON__ = 2.2204460492503131e-16;
pub const __DBL_HAS_INFINITY__ = @as(c_int, 1);
pub const __DBL_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __DBL_MANT_DIG__ = @as(c_int, 53);
pub const __DBL_MAX_10_EXP__ = @as(c_int, 308);
pub const __DBL_MAX_EXP__ = @as(c_int, 1024);
pub const __DBL_MAX__ = 1.7976931348623157e+308;
pub const __DBL_MIN_10_EXP__ = -@as(c_int, 307);
pub const __DBL_MIN_EXP__ = -@as(c_int, 1021);
pub const __DBL_MIN__ = 2.2250738585072014e-308;
pub const __LDBL_DENORM_MIN__ = @as(c_longdouble, 3.64519953188247460253e-4951);
pub const __LDBL_HAS_DENORM__ = @as(c_int, 1);
pub const __LDBL_DIG__ = @as(c_int, 18);
pub const __LDBL_DECIMAL_DIG__ = @as(c_int, 21);
pub const __LDBL_EPSILON__ = @as(c_longdouble, 1.08420217248550443401e-19);
pub const __LDBL_HAS_INFINITY__ = @as(c_int, 1);
pub const __LDBL_HAS_QUIET_NAN__ = @as(c_int, 1);
pub const __LDBL_MANT_DIG__ = @as(c_int, 64);
pub const __LDBL_MAX_10_EXP__ = @as(c_int, 4932);
pub const __LDBL_MAX_EXP__ = @as(c_int, 16384);
pub const __LDBL_MAX__ = @as(c_longdouble, 1.18973149535723176502e+4932);
pub const __LDBL_MIN_10_EXP__ = -@as(c_int, 4931);
pub const __LDBL_MIN_EXP__ = -@as(c_int, 16381);
pub const __LDBL_MIN__ = @as(c_longdouble, 3.36210314311209350626e-4932);
pub const __POINTER_WIDTH__ = @as(c_int, 64);
pub const __BIGGEST_ALIGNMENT__ = @as(c_int, 16);
pub const __WINT_UNSIGNED__ = @as(c_int, 1);
pub const __INT8_TYPE__ = i8;
pub const __INT8_FMTd__ = "hhd";
pub const __INT8_FMTi__ = "hhi";
pub const __INT8_C_SUFFIX__ = "";
pub const __INT16_TYPE__ = c_short;
pub const __INT16_FMTd__ = "hd";
pub const __INT16_FMTi__ = "hi";
pub const __INT16_C_SUFFIX__ = "";
pub const __INT32_TYPE__ = c_int;
pub const __INT32_FMTd__ = "d";
pub const __INT32_FMTi__ = "i";
pub const __INT32_C_SUFFIX__ = "";
pub const __INT64_TYPE__ = c_long;
pub const __INT64_FMTd__ = "ld";
pub const __INT64_FMTi__ = "li";
pub const __UINT8_TYPE__ = u8;
pub const __UINT8_FMTo__ = "hho";
pub const __UINT8_FMTu__ = "hhu";
pub const __UINT8_FMTx__ = "hhx";
pub const __UINT8_FMTX__ = "hhX";
pub const __UINT8_C_SUFFIX__ = "";
pub const __UINT8_MAX__ = @as(c_int, 255);
pub const __INT8_MAX__ = @as(c_int, 127);
pub const __UINT16_TYPE__ = c_ushort;
pub const __UINT16_FMTo__ = "ho";
pub const __UINT16_FMTu__ = "hu";
pub const __UINT16_FMTx__ = "hx";
pub const __UINT16_FMTX__ = "hX";
pub const __UINT16_C_SUFFIX__ = "";
pub const __UINT16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __INT16_MAX__ = @as(c_int, 32767);
pub const __UINT32_TYPE__ = c_uint;
pub const __UINT32_FMTo__ = "o";
pub const __UINT32_FMTu__ = "u";
pub const __UINT32_FMTx__ = "x";
pub const __UINT32_FMTX__ = "X";
pub const __UINT32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __INT32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __UINT64_TYPE__ = c_ulong;
pub const __UINT64_FMTo__ = "lo";
pub const __UINT64_FMTu__ = "lu";
pub const __UINT64_FMTx__ = "lx";
pub const __UINT64_FMTX__ = "lX";
pub const __UINT64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __INT64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_LEAST8_TYPE__ = i8;
pub const __INT_LEAST8_MAX__ = @as(c_int, 127);
pub const __INT_LEAST8_WIDTH__ = @as(c_int, 8);
pub const __INT_LEAST8_FMTd__ = "hhd";
pub const __INT_LEAST8_FMTi__ = "hhi";
pub const __UINT_LEAST8_TYPE__ = u8;
pub const __UINT_LEAST8_MAX__ = @as(c_int, 255);
pub const __UINT_LEAST8_FMTo__ = "hho";
pub const __UINT_LEAST8_FMTu__ = "hhu";
pub const __UINT_LEAST8_FMTx__ = "hhx";
pub const __UINT_LEAST8_FMTX__ = "hhX";
pub const __INT_LEAST16_TYPE__ = c_short;
pub const __INT_LEAST16_MAX__ = @as(c_int, 32767);
pub const __INT_LEAST16_WIDTH__ = @as(c_int, 16);
pub const __INT_LEAST16_FMTd__ = "hd";
pub const __INT_LEAST16_FMTi__ = "hi";
pub const __UINT_LEAST16_TYPE__ = c_ushort;
pub const __UINT_LEAST16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __UINT_LEAST16_FMTo__ = "ho";
pub const __UINT_LEAST16_FMTu__ = "hu";
pub const __UINT_LEAST16_FMTx__ = "hx";
pub const __UINT_LEAST16_FMTX__ = "hX";
pub const __INT_LEAST32_TYPE__ = c_int;
pub const __INT_LEAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __INT_LEAST32_WIDTH__ = @as(c_int, 32);
pub const __INT_LEAST32_FMTd__ = "d";
pub const __INT_LEAST32_FMTi__ = "i";
pub const __UINT_LEAST32_TYPE__ = c_uint;
pub const __UINT_LEAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __UINT_LEAST32_FMTo__ = "o";
pub const __UINT_LEAST32_FMTu__ = "u";
pub const __UINT_LEAST32_FMTx__ = "x";
pub const __UINT_LEAST32_FMTX__ = "X";
pub const __INT_LEAST64_TYPE__ = c_long;
pub const __INT_LEAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_LEAST64_WIDTH__ = @as(c_int, 64);
pub const __INT_LEAST64_FMTd__ = "ld";
pub const __INT_LEAST64_FMTi__ = "li";
pub const __UINT_LEAST64_TYPE__ = c_ulong;
pub const __UINT_LEAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINT_LEAST64_FMTo__ = "lo";
pub const __UINT_LEAST64_FMTu__ = "lu";
pub const __UINT_LEAST64_FMTx__ = "lx";
pub const __UINT_LEAST64_FMTX__ = "lX";
pub const __INT_FAST8_TYPE__ = i8;
pub const __INT_FAST8_MAX__ = @as(c_int, 127);
pub const __INT_FAST8_WIDTH__ = @as(c_int, 8);
pub const __INT_FAST8_FMTd__ = "hhd";
pub const __INT_FAST8_FMTi__ = "hhi";
pub const __UINT_FAST8_TYPE__ = u8;
pub const __UINT_FAST8_MAX__ = @as(c_int, 255);
pub const __UINT_FAST8_FMTo__ = "hho";
pub const __UINT_FAST8_FMTu__ = "hhu";
pub const __UINT_FAST8_FMTx__ = "hhx";
pub const __UINT_FAST8_FMTX__ = "hhX";
pub const __INT_FAST16_TYPE__ = c_short;
pub const __INT_FAST16_MAX__ = @as(c_int, 32767);
pub const __INT_FAST16_WIDTH__ = @as(c_int, 16);
pub const __INT_FAST16_FMTd__ = "hd";
pub const __INT_FAST16_FMTi__ = "hi";
pub const __UINT_FAST16_TYPE__ = c_ushort;
pub const __UINT_FAST16_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 65535, .decimal);
pub const __UINT_FAST16_FMTo__ = "ho";
pub const __UINT_FAST16_FMTu__ = "hu";
pub const __UINT_FAST16_FMTx__ = "hx";
pub const __UINT_FAST16_FMTX__ = "hX";
pub const __INT_FAST32_TYPE__ = c_int;
pub const __INT_FAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_int, 2147483647, .decimal);
pub const __INT_FAST32_WIDTH__ = @as(c_int, 32);
pub const __INT_FAST32_FMTd__ = "d";
pub const __INT_FAST32_FMTi__ = "i";
pub const __UINT_FAST32_TYPE__ = c_uint;
pub const __UINT_FAST32_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_uint, 4294967295, .decimal);
pub const __UINT_FAST32_FMTo__ = "o";
pub const __UINT_FAST32_FMTu__ = "u";
pub const __UINT_FAST32_FMTx__ = "x";
pub const __UINT_FAST32_FMTX__ = "X";
pub const __INT_FAST64_TYPE__ = c_long;
pub const __INT_FAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_long, 9223372036854775807, .decimal);
pub const __INT_FAST64_WIDTH__ = @as(c_int, 64);
pub const __INT_FAST64_FMTd__ = "ld";
pub const __INT_FAST64_FMTi__ = "li";
pub const __UINT_FAST64_TYPE__ = c_ulong;
pub const __UINT_FAST64_MAX__ = @import("std").zig.c_translation.promoteIntLiteral(c_ulong, 18446744073709551615, .decimal);
pub const __UINT_FAST64_FMTo__ = "lo";
pub const __UINT_FAST64_FMTu__ = "lu";
pub const __UINT_FAST64_FMTx__ = "lx";
pub const __UINT_FAST64_FMTX__ = "lX";
pub const __USER_LABEL_PREFIX__ = "";
pub const __FINITE_MATH_ONLY__ = @as(c_int, 0);
pub const __GNUC_STDC_INLINE__ = @as(c_int, 1);
pub const __GCC_ATOMIC_TEST_AND_SET_TRUEVAL = @as(c_int, 1);
pub const __CLANG_ATOMIC_BOOL_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR16_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_CHAR32_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_WCHAR_T_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_SHORT_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_INT_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_LONG_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_LLONG_LOCK_FREE = @as(c_int, 2);
pub const __CLANG_ATOMIC_POINTER_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_BOOL_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR16_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_CHAR32_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_WCHAR_T_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_SHORT_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_INT_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_LONG_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_LLONG_LOCK_FREE = @as(c_int, 2);
pub const __GCC_ATOMIC_POINTER_LOCK_FREE = @as(c_int, 2);
pub const __NO_INLINE__ = @as(c_int, 1);
pub const __PIC__ = @as(c_int, 2);
pub const __pic__ = @as(c_int, 2);
pub const __FLT_EVAL_METHOD__ = @as(c_int, 0);
pub const __FLT_RADIX__ = @as(c_int, 2);
pub const __DECIMAL_DIG__ = __LDBL_DECIMAL_DIG__;
pub const __SSP_STRONG__ = @as(c_int, 2);
pub const __GCC_ASM_FLAG_OUTPUTS__ = @as(c_int, 1);
pub const __code_model_small__ = @as(c_int, 1);
pub const __amd64__ = @as(c_int, 1);
pub const __amd64 = @as(c_int, 1);
pub const __x86_64 = @as(c_int, 1);
pub const __x86_64__ = @as(c_int, 1);
pub const __SEG_GS = @as(c_int, 1);
pub const __SEG_FS = @as(c_int, 1);
pub const __k8 = @as(c_int, 1);
pub const __k8__ = @as(c_int, 1);
pub const __tune_k8__ = @as(c_int, 1);
pub const __REGISTER_PREFIX__ = "";
pub const __NO_MATH_INLINES = @as(c_int, 1);
pub const __AES__ = @as(c_int, 1);
pub const __VAES__ = @as(c_int, 1);
pub const __PCLMUL__ = @as(c_int, 1);
pub const __VPCLMULQDQ__ = @as(c_int, 1);
pub const __LAHF_SAHF__ = @as(c_int, 1);
pub const __LZCNT__ = @as(c_int, 1);
pub const __RDRND__ = @as(c_int, 1);
pub const __FSGSBASE__ = @as(c_int, 1);
pub const __BMI__ = @as(c_int, 1);
pub const __BMI2__ = @as(c_int, 1);
pub const __POPCNT__ = @as(c_int, 1);
pub const __PRFCHW__ = @as(c_int, 1);
pub const __RDSEED__ = @as(c_int, 1);
pub const __ADX__ = @as(c_int, 1);
pub const __MOVBE__ = @as(c_int, 1);
pub const __FMA__ = @as(c_int, 1);
pub const __F16C__ = @as(c_int, 1);
pub const __GFNI__ = @as(c_int, 1);
pub const __AVX512CD__ = @as(c_int, 1);
pub const __AVX512VPOPCNTDQ__ = @as(c_int, 1);
pub const __AVX512VNNI__ = @as(c_int, 1);
pub const __AVX512DQ__ = @as(c_int, 1);
pub const __AVX512BITALG__ = @as(c_int, 1);
pub const __AVX512BW__ = @as(c_int, 1);
pub const __AVX512VL__ = @as(c_int, 1);
pub const __AVX512VBMI__ = @as(c_int, 1);
pub const __AVX512VBMI2__ = @as(c_int, 1);
pub const __AVX512IFMA__ = @as(c_int, 1);
pub const __AVX512VP2INTERSECT__ = @as(c_int, 1);
pub const __SHA__ = @as(c_int, 1);
pub const __FXSR__ = @as(c_int, 1);
pub const __XSAVE__ = @as(c_int, 1);
pub const __XSAVEOPT__ = @as(c_int, 1);
pub const __XSAVEC__ = @as(c_int, 1);
pub const __XSAVES__ = @as(c_int, 1);
pub const __CLFLUSHOPT__ = @as(c_int, 1);
pub const __CLWB__ = @as(c_int, 1);
pub const __SHSTK__ = @as(c_int, 1);
pub const __RDPID__ = @as(c_int, 1);
pub const __MOVDIRI__ = @as(c_int, 1);
pub const __MOVDIR64B__ = @as(c_int, 1);
pub const __INVPCID__ = @as(c_int, 1);
pub const __AVX512F__ = @as(c_int, 1);
pub const __AVX2__ = @as(c_int, 1);
pub const __AVX__ = @as(c_int, 1);
pub const __SSE4_2__ = @as(c_int, 1);
pub const __SSE4_1__ = @as(c_int, 1);
pub const __SSSE3__ = @as(c_int, 1);
pub const __SSE3__ = @as(c_int, 1);
pub const __SSE2__ = @as(c_int, 1);
pub const __SSE2_MATH__ = @as(c_int, 1);
pub const __SSE__ = @as(c_int, 1);
pub const __SSE_MATH__ = @as(c_int, 1);
pub const __MMX__ = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 = @as(c_int, 1);
pub const __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 = @as(c_int, 1);
pub const __SIZEOF_FLOAT128__ = @as(c_int, 16);
pub const unix = @as(c_int, 1);
pub const __unix = @as(c_int, 1);
pub const __unix__ = @as(c_int, 1);
pub const linux = @as(c_int, 1);
pub const __linux = @as(c_int, 1);
pub const __linux__ = @as(c_int, 1);
pub const __ELF__ = @as(c_int, 1);
pub const __gnu_linux__ = @as(c_int, 1);
pub const __FLOAT128__ = @as(c_int, 1);
pub const __STDC__ = @as(c_int, 1);
pub const __STDC_HOSTED__ = @as(c_int, 1);
pub const __STDC_VERSION__ = @as(c_long, 201710);
pub const __STDC_UTF_16__ = @as(c_int, 1);
pub const __STDC_UTF_32__ = @as(c_int, 1);
pub const __GLIBC_MINOR__ = @as(c_int, 31);
pub const _DEBUG = @as(c_int, 1);
pub const __GCC_HAVE_DWARF2_CFI_ASM = @as(c_int, 1);
pub const miniaudio_h = "";
pub inline fn MA_XSTRINGIFY(x: anytype) @TypeOf(MA_STRINGIFY(x)) {
    return MA_STRINGIFY(x);
}
pub const MA_VERSION_MAJOR = @as(c_int, 0);
pub const MA_VERSION_MINOR = @as(c_int, 11);
pub const MA_VERSION_REVISION = @as(c_int, 9);
pub const MA_SIZEOF_PTR = @as(c_int, 8);
pub const __STDDEF_H = "";
pub const __need_ptrdiff_t = "";
pub const __need_size_t = "";
pub const __need_wchar_t = "";
pub const __need_NULL = "";
pub const __need_STDDEF_H_misc = "";
pub const _PTRDIFF_T = "";
pub const _SIZE_T = "";
pub const _WCHAR_T = "";
pub const NULL = @import("std").zig.c_translation.cast(?*anyopaque, @as(c_int, 0));
pub const __CLANG_MAX_ALIGN_T_DEFINED = "";
pub const MA_TRUE = @as(c_int, 1);
pub const MA_FALSE = @as(c_int, 0);
pub const MA_SIZE_MAX = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0xFFFFFFFF, .hexadecimal);
pub const MA_POSIX = "";
pub const _PTHREAD_H = @as(c_int, 1);
pub const _FEATURES_H = @as(c_int, 1);
pub const __KERNEL_STRICT_NAMES = "";
pub inline fn __GNUC_PREREQ(maj: anytype, min: anytype) @TypeOf(((__GNUC__ << @as(c_int, 16)) + __GNUC_MINOR__) >= ((maj << @as(c_int, 16)) + min)) {
    return ((__GNUC__ << @as(c_int, 16)) + __GNUC_MINOR__) >= ((maj << @as(c_int, 16)) + min);
}
pub inline fn __glibc_clang_prereq(maj: anytype, min: anytype) @TypeOf(((__clang_major__ << @as(c_int, 16)) + __clang_minor__) >= ((maj << @as(c_int, 16)) + min)) {
    return ((__clang_major__ << @as(c_int, 16)) + __clang_minor__) >= ((maj << @as(c_int, 16)) + min);
}
pub const _DEFAULT_SOURCE = @as(c_int, 1);
pub const __GLIBC_USE_ISOC2X = @as(c_int, 0);
pub const __USE_ISOC11 = @as(c_int, 1);
pub const __USE_ISOC99 = @as(c_int, 1);
pub const __USE_ISOC95 = @as(c_int, 1);
pub const __USE_POSIX_IMPLICITLY = @as(c_int, 1);
pub const _POSIX_SOURCE = @as(c_int, 1);
pub const _POSIX_C_SOURCE = @as(c_long, 200809);
pub const __USE_POSIX = @as(c_int, 1);
pub const __USE_POSIX2 = @as(c_int, 1);
pub const __USE_POSIX199309 = @as(c_int, 1);
pub const __USE_POSIX199506 = @as(c_int, 1);
pub const __USE_XOPEN2K = @as(c_int, 1);
pub const __USE_XOPEN2K8 = @as(c_int, 1);
pub const _ATFILE_SOURCE = @as(c_int, 1);
pub const __WORDSIZE = @as(c_int, 64);
pub const __WORDSIZE_TIME64_COMPAT32 = @as(c_int, 1);
pub const __SYSCALL_WORDSIZE = @as(c_int, 64);
pub const __TIMESIZE = __WORDSIZE;
pub const __USE_MISC = @as(c_int, 1);
pub const __USE_ATFILE = @as(c_int, 1);
pub const __USE_FORTIFY_LEVEL = @as(c_int, 0);
pub const __GLIBC_USE_DEPRECATED_GETS = @as(c_int, 0);
pub const __GLIBC_USE_DEPRECATED_SCANF = @as(c_int, 0);
pub const _STDC_PREDEF_H = @as(c_int, 1);
pub const __STDC_IEC_559__ = @as(c_int, 1);
pub const __STDC_IEC_559_COMPLEX__ = @as(c_int, 1);
pub const __STDC_ISO_10646__ = @as(c_long, 201706);
pub const __GNU_LIBRARY__ = @as(c_int, 6);
pub const __GLIBC__ = @as(c_int, 2);
pub inline fn __GLIBC_PREREQ(maj: anytype, min: anytype) @TypeOf(((__GLIBC__ << @as(c_int, 16)) + __GLIBC_MINOR__) >= ((maj << @as(c_int, 16)) + min)) {
    return ((__GLIBC__ << @as(c_int, 16)) + __GLIBC_MINOR__) >= ((maj << @as(c_int, 16)) + min);
}
pub const _SYS_CDEFS_H = @as(c_int, 1);
pub inline fn __glibc_has_builtin(name: anytype) @TypeOf(__has_builtin(name)) {
    return __has_builtin(name);
}
pub const __LEAF = "";
pub const __LEAF_ATTR = "";
pub inline fn __P(args: anytype) @TypeOf(args) {
    return args;
}
pub inline fn __PMT(args: anytype) @TypeOf(args) {
    return args;
}
pub const __ptr_t = ?*anyopaque;
pub const __BEGIN_DECLS = "";
pub const __END_DECLS = "";
pub inline fn __bos(ptr: anytype) @TypeOf(__builtin_object_size(ptr, __USE_FORTIFY_LEVEL > @as(c_int, 1))) {
    return __builtin_object_size(ptr, __USE_FORTIFY_LEVEL > @as(c_int, 1));
}
pub inline fn __bos0(ptr: anytype) @TypeOf(__builtin_object_size(ptr, @as(c_int, 0))) {
    return __builtin_object_size(ptr, @as(c_int, 0));
}
pub inline fn __glibc_objsize0(__o: anytype) @TypeOf(__bos0(__o)) {
    return __bos0(__o);
}
pub inline fn __glibc_objsize(__o: anytype) @TypeOf(__bos(__o)) {
    return __bos(__o);
}
pub const __glibc_c99_flexarr_available = @as(c_int, 1);
pub inline fn __ASMNAME(cname: anytype) @TypeOf(__ASMNAME2(__USER_LABEL_PREFIX__, cname)) {
    return __ASMNAME2(__USER_LABEL_PREFIX__, cname);
}
pub const __wur = "";
pub const __fortify_function = __extern_always_inline ++ __attribute_artificial__;
pub inline fn __glibc_unlikely(cond: anytype) @TypeOf(__builtin_expect(cond, @as(c_int, 0))) {
    return __builtin_expect(cond, @as(c_int, 0));
}
pub inline fn __glibc_likely(cond: anytype) @TypeOf(__builtin_expect(cond, @as(c_int, 1))) {
    return __builtin_expect(cond, @as(c_int, 1));
}
pub const __attribute_nonstring__ = "";
pub const __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI = @as(c_int, 0);
pub inline fn __LDBL_REDIR1(name: anytype, proto: anytype, alias: anytype) @TypeOf(name ++ proto) {
    _ = alias;
    return name ++ proto;
}
pub inline fn __LDBL_REDIR(name: anytype, proto: anytype) @TypeOf(name ++ proto) {
    return name ++ proto;
}
pub inline fn __LDBL_REDIR1_NTH(name: anytype, proto: anytype, alias: anytype) @TypeOf(name ++ proto ++ __THROW) {
    _ = alias;
    return name ++ proto ++ __THROW;
}
pub inline fn __LDBL_REDIR_NTH(name: anytype, proto: anytype) @TypeOf(name ++ proto ++ __THROW) {
    return name ++ proto ++ __THROW;
}
pub inline fn __REDIRECT_LDBL(name: anytype, proto: anytype, alias: anytype) @TypeOf(__REDIRECT(name, proto, alias)) {
    return __REDIRECT(name, proto, alias);
}
pub inline fn __REDIRECT_NTH_LDBL(name: anytype, proto: anytype, alias: anytype) @TypeOf(__REDIRECT_NTH(name, proto, alias)) {
    return __REDIRECT_NTH(name, proto, alias);
}
pub const __HAVE_GENERIC_SELECTION = @as(c_int, 1);
pub const __attr_dealloc_free = "";
pub const __stub___compat_bdflush = "";
pub const __stub_chflags = "";
pub const __stub_fchflags = "";
pub const __stub_gtty = "";
pub const __stub_revoke = "";
pub const __stub_setlogin = "";
pub const __stub_sigreturn = "";
pub const __stub_stty = "";
pub const _SCHED_H = @as(c_int, 1);
pub const _BITS_TYPES_H = @as(c_int, 1);
pub const __S16_TYPE = c_short;
pub const __U16_TYPE = c_ushort;
pub const __S32_TYPE = c_int;
pub const __U32_TYPE = c_uint;
pub const __SLONGWORD_TYPE = c_long;
pub const __ULONGWORD_TYPE = c_ulong;
pub const __SQUAD_TYPE = c_long;
pub const __UQUAD_TYPE = c_ulong;
pub const __SWORD_TYPE = c_long;
pub const __UWORD_TYPE = c_ulong;
pub const __SLONG32_TYPE = c_int;
pub const __ULONG32_TYPE = c_uint;
pub const __S64_TYPE = c_long;
pub const __U64_TYPE = c_ulong;
pub const _BITS_TYPESIZES_H = @as(c_int, 1);
pub const __SYSCALL_SLONG_TYPE = __SLONGWORD_TYPE;
pub const __SYSCALL_ULONG_TYPE = __ULONGWORD_TYPE;
pub const __DEV_T_TYPE = __UQUAD_TYPE;
pub const __UID_T_TYPE = __U32_TYPE;
pub const __GID_T_TYPE = __U32_TYPE;
pub const __INO_T_TYPE = __SYSCALL_ULONG_TYPE;
pub const __INO64_T_TYPE = __UQUAD_TYPE;
pub const __MODE_T_TYPE = __U32_TYPE;
pub const __NLINK_T_TYPE = __SYSCALL_ULONG_TYPE;
pub const __FSWORD_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __OFF_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __OFF64_T_TYPE = __SQUAD_TYPE;
pub const __PID_T_TYPE = __S32_TYPE;
pub const __RLIM_T_TYPE = __SYSCALL_ULONG_TYPE;
pub const __RLIM64_T_TYPE = __UQUAD_TYPE;
pub const __BLKCNT_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __BLKCNT64_T_TYPE = __SQUAD_TYPE;
pub const __FSBLKCNT_T_TYPE = __SYSCALL_ULONG_TYPE;
pub const __FSBLKCNT64_T_TYPE = __UQUAD_TYPE;
pub const __FSFILCNT_T_TYPE = __SYSCALL_ULONG_TYPE;
pub const __FSFILCNT64_T_TYPE = __UQUAD_TYPE;
pub const __ID_T_TYPE = __U32_TYPE;
pub const __CLOCK_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __TIME_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __USECONDS_T_TYPE = __U32_TYPE;
pub const __SUSECONDS_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __SUSECONDS64_T_TYPE = __SQUAD_TYPE;
pub const __DADDR_T_TYPE = __S32_TYPE;
pub const __KEY_T_TYPE = __S32_TYPE;
pub const __CLOCKID_T_TYPE = __S32_TYPE;
pub const __TIMER_T_TYPE = ?*anyopaque;
pub const __BLKSIZE_T_TYPE = __SYSCALL_SLONG_TYPE;
pub const __SSIZE_T_TYPE = __SWORD_TYPE;
pub const __CPU_MASK_TYPE = __SYSCALL_ULONG_TYPE;
pub const __OFF_T_MATCHES_OFF64_T = @as(c_int, 1);
pub const __INO_T_MATCHES_INO64_T = @as(c_int, 1);
pub const __RLIM_T_MATCHES_RLIM64_T = @as(c_int, 1);
pub const __STATFS_MATCHES_STATFS64 = @as(c_int, 1);
pub const __KERNEL_OLD_TIMEVAL_MATCHES_TIMEVAL64 = @as(c_int, 1);
pub const __FD_SETSIZE = @as(c_int, 1024);
pub const _BITS_TIME64_H = @as(c_int, 1);
pub const __TIME64_T_TYPE = __TIME_T_TYPE;
pub const __time_t_defined = @as(c_int, 1);
pub const _STRUCT_TIMESPEC = @as(c_int, 1);
pub const _BITS_ENDIAN_H = @as(c_int, 1);
pub const __LITTLE_ENDIAN = @as(c_int, 1234);
pub const __BIG_ENDIAN = @as(c_int, 4321);
pub const __PDP_ENDIAN = @as(c_int, 3412);
pub const _BITS_ENDIANNESS_H = @as(c_int, 1);
pub const __BYTE_ORDER = __LITTLE_ENDIAN;
pub const __FLOAT_WORD_ORDER = __BYTE_ORDER;
pub inline fn __LONG_LONG_PAIR(HI: anytype, LO: anytype) @TypeOf(HI) {
    return blk: {
        _ = LO;
        break :blk HI;
    };
}
pub const __pid_t_defined = "";
pub const _BITS_SCHED_H = @as(c_int, 1);
pub const SCHED_OTHER = @as(c_int, 0);
pub const SCHED_FIFO = @as(c_int, 1);
pub const SCHED_RR = @as(c_int, 2);
pub const _BITS_TYPES_STRUCT_SCHED_PARAM = @as(c_int, 1);
pub const _BITS_CPU_SET_H = @as(c_int, 1);
pub const __CPU_SETSIZE = @as(c_int, 1024);
pub const __NCPUBITS = @as(c_int, 8) * @import("std").zig.c_translation.sizeof(__cpu_mask);
pub inline fn __CPUELT(cpu: anytype) @TypeOf(cpu / __NCPUBITS) {
    return cpu / __NCPUBITS;
}
pub inline fn __CPUMASK(cpu: anytype) @TypeOf(@import("std").zig.c_translation.cast(__cpu_mask, @as(c_int, 1)) << (cpu % __NCPUBITS)) {
    return @import("std").zig.c_translation.cast(__cpu_mask, @as(c_int, 1)) << (cpu % __NCPUBITS);
}
pub inline fn __CPU_COUNT_S(setsize: anytype, cpusetp: anytype) @TypeOf(__sched_cpucount(setsize, cpusetp)) {
    return __sched_cpucount(setsize, cpusetp);
}
pub inline fn __CPU_ALLOC_SIZE(count: anytype) @TypeOf((((count + __NCPUBITS) - @as(c_int, 1)) / __NCPUBITS) * @import("std").zig.c_translation.sizeof(__cpu_mask)) {
    return (((count + __NCPUBITS) - @as(c_int, 1)) / __NCPUBITS) * @import("std").zig.c_translation.sizeof(__cpu_mask);
}
pub inline fn __CPU_ALLOC(count: anytype) @TypeOf(__sched_cpualloc(count)) {
    return __sched_cpualloc(count);
}
pub inline fn __CPU_FREE(cpuset: anytype) @TypeOf(__sched_cpufree(cpuset)) {
    return __sched_cpufree(cpuset);
}
//pub const __sched_priority = sched_priority;
pub const _TIME_H = @as(c_int, 1);
pub const _BITS_TIME_H = @as(c_int, 1);
pub const CLOCKS_PER_SEC = @import("std").zig.c_translation.cast(__clock_t, @import("std").zig.c_translation.promoteIntLiteral(c_int, 1000000, .decimal));
pub const CLOCK_REALTIME = @as(c_int, 0);
pub const CLOCK_MONOTONIC = @as(c_int, 1);
pub const CLOCK_PROCESS_CPUTIME_ID = @as(c_int, 2);
pub const CLOCK_THREAD_CPUTIME_ID = @as(c_int, 3);
pub const CLOCK_MONOTONIC_RAW = @as(c_int, 4);
pub const CLOCK_REALTIME_COARSE = @as(c_int, 5);
pub const CLOCK_MONOTONIC_COARSE = @as(c_int, 6);
pub const CLOCK_BOOTTIME = @as(c_int, 7);
pub const CLOCK_REALTIME_ALARM = @as(c_int, 8);
pub const CLOCK_BOOTTIME_ALARM = @as(c_int, 9);
pub const CLOCK_TAI = @as(c_int, 11);
pub const TIMER_ABSTIME = @as(c_int, 1);
pub const __clock_t_defined = @as(c_int, 1);
pub const __struct_tm_defined = @as(c_int, 1);
pub const __clockid_t_defined = @as(c_int, 1);
pub const __timer_t_defined = @as(c_int, 1);
pub const __itimerspec_defined = @as(c_int, 1);
pub const _BITS_TYPES_LOCALE_T_H = @as(c_int, 1);
pub const _BITS_TYPES___LOCALE_T_H = @as(c_int, 1);
pub const TIME_UTC = @as(c_int, 1);
pub inline fn __isleap(year: anytype) @TypeOf(((year % @as(c_int, 4)) == @as(c_int, 0)) and (((year % @as(c_int, 100)) != @as(c_int, 0)) or ((year % @as(c_int, 400)) == @as(c_int, 0)))) {
    return ((year % @as(c_int, 4)) == @as(c_int, 0)) and (((year % @as(c_int, 100)) != @as(c_int, 0)) or ((year % @as(c_int, 400)) == @as(c_int, 0)));
}
pub const _BITS_PTHREADTYPES_COMMON_H = @as(c_int, 1);
pub const _THREAD_SHARED_TYPES_H = @as(c_int, 1);
pub const _BITS_PTHREADTYPES_ARCH_H = @as(c_int, 1);
pub const __SIZEOF_PTHREAD_MUTEX_T = @as(c_int, 40);
pub const __SIZEOF_PTHREAD_ATTR_T = @as(c_int, 56);
pub const __SIZEOF_PTHREAD_RWLOCK_T = @as(c_int, 56);
pub const __SIZEOF_PTHREAD_BARRIER_T = @as(c_int, 32);
pub const __SIZEOF_PTHREAD_MUTEXATTR_T = @as(c_int, 4);
pub const __SIZEOF_PTHREAD_COND_T = @as(c_int, 48);
pub const __SIZEOF_PTHREAD_CONDATTR_T = @as(c_int, 4);
pub const __SIZEOF_PTHREAD_RWLOCKATTR_T = @as(c_int, 8);
pub const __SIZEOF_PTHREAD_BARRIERATTR_T = @as(c_int, 4);
pub const __LOCK_ALIGNMENT = "";
pub const __ONCE_ALIGNMENT = "";
pub const _THREAD_MUTEX_INTERNAL_H = @as(c_int, 1);
pub const __PTHREAD_MUTEX_HAVE_PREV = @as(c_int, 1);
pub const _RWLOCK_INTERNAL_H = "";
pub inline fn __PTHREAD_RWLOCK_INITIALIZER(__flags: anytype) @TypeOf(__flags) {
    return blk: {
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = @as(c_int, 0);
        _ = __PTHREAD_RWLOCK_ELISION_EXTRA;
        _ = @as(c_int, 0);
        break :blk __flags;
    };
}
pub const __have_pthread_attr_t = @as(c_int, 1);
pub const _BITS_SETJMP_H = @as(c_int, 1);
pub const ____sigset_t_defined = "";
pub const _SIGSET_NWORDS = @as(c_int, 1024) / (@as(c_int, 8) * @import("std").zig.c_translation.sizeof(c_ulong));
pub const __jmp_buf_tag_defined = @as(c_int, 1);
pub const PTHREAD_STACK_MIN = @as(c_int, 16384);
pub const PTHREAD_CANCELED = @import("std").zig.c_translation.cast(?*anyopaque, -@as(c_int, 1));
pub const PTHREAD_ONCE_INIT = @as(c_int, 0);
pub const PTHREAD_BARRIER_SERIAL_THREAD = -@as(c_int, 1);
pub const __cleanup_fct_attribute = "";
pub inline fn __sigsetjmp_cancel(env: anytype, savemask: anytype) @TypeOf(__sigsetjmp(@import("std").zig.c_translation.cast([*c]struct___jmp_buf_tag, @import("std").zig.c_translation.cast(?*anyopaque, env)), savemask)) {
    return __sigsetjmp(@import("std").zig.c_translation.cast([*c]struct___jmp_buf_tag, @import("std").zig.c_translation.cast(?*anyopaque, env)), savemask);
}
pub const MA_UNIX = "";
pub const MA_LINUX = "";
pub const MA_SIMD_ALIGNMENT = @as(c_int, 32);
pub const __STDALIGN_H = "";
pub const __alignas_is_defined = @as(c_int, 1);
pub const __alignof_is_defined = @as(c_int, 1);
pub const MA_MIN_CHANNELS = @as(c_int, 1);
pub const MA_MAX_CHANNELS = @as(c_int, 254);
pub const MA_MAX_FILTER_ORDER = @as(c_int, 8);
pub const __STDARG_H = "";
pub const _VA_LIST = "";
pub const __GNUC_VA_LIST = @as(c_int, 1);
pub const MA_MAX_LOG_CALLBACKS = @as(c_int, 4);
pub const MA_CHANNEL_INDEX_NULL = @as(c_int, 255);
pub const MA_SUPPORT_ALSA = "";
pub const MA_SUPPORT_PULSEAUDIO = "";
pub const MA_SUPPORT_JACK = "";
pub const MA_SUPPORT_CUSTOM = "";
pub const MA_SUPPORT_NULL = "";
pub const MA_HAS_ALSA = "";
pub const MA_HAS_PULSEAUDIO = "";
pub const MA_HAS_JACK = "";
pub const MA_HAS_CUSTOM = "";
pub const MA_HAS_NULL = "";
pub const MA_BACKEND_COUNT = ma_backend_null + @as(c_int, 1);
pub const MA_DATA_FORMAT_FLAG_EXCLUSIVE_MODE = @as(c_uint, 1) << @as(c_int, 1);
pub const MA_MAX_DEVICE_NAME_LENGTH = @as(c_int, 255);
pub const MA_DATA_SOURCE_SELF_MANAGED_RANGE_AND_LOOP_POINT = @as(c_int, 0x00000001);
pub const ma_resource_manager_job = ma_job;
pub const ma_resource_manager_job_init = ma_job_init;
pub const MA_JOB_TYPE_RESOURCE_MANAGER_QUEUE_FLAG_NON_BLOCKING = MA_JOB_QUEUE_FLAG_NON_BLOCKING;
pub const ma_resource_manager_job_queue_config = ma_job_queue_config;
pub const ma_resource_manager_job_queue_config_init = ma_job_queue_config_init;
pub const ma_resource_manager_job_queue = ma_job_queue;
pub const ma_resource_manager_job_queue_get_heap_size = ma_job_queue_get_heap_size;
pub const ma_resource_manager_job_queue_init_preallocated = ma_job_queue_init_preallocated;
pub const ma_resource_manager_job_queue_init = ma_job_queue_init;
pub const ma_resource_manager_job_queue_uninit = ma_job_queue_uninit;
pub const ma_resource_manager_job_queue_post = ma_job_queue_post;
pub const ma_resource_manager_job_queue_next = ma_job_queue_next;
pub const MA_RESOURCE_MANAGER_MAX_JOB_THREAD_COUNT = @as(c_int, 64);
pub const MA_MAX_NODE_BUS_COUNT = @as(c_int, 254);
pub const MA_MAX_NODE_LOCAL_BUS_COUNT = @as(c_int, 2);
pub const MA_NODE_BUS_COUNT_UNKNOWN = @as(c_int, 255);
pub const MA_ENGINE_MAX_LISTENERS = @as(c_int, 4);
pub const MA_LISTENER_INDEX_CLOSEST = @import("std").zig.c_translation.cast(ma_uint8, -@as(c_int, 1));
pub const MA_SOUND_SOURCE_CHANNEL_COUNT = @import("std").zig.c_translation.promoteIntLiteral(c_int, 0xFFFFFFFF, .hexadecimal);
pub const timespec = struct_timespec;
pub const sched_param = struct_sched_param;
pub const tm = struct_tm;
pub const itimerspec = struct_itimerspec;
pub const sigevent = struct_sigevent;
pub const __locale_data = struct___locale_data;
pub const __locale_struct = struct___locale_struct;
pub const __pthread_internal_list = struct___pthread_internal_list;
pub const __pthread_internal_slist = struct___pthread_internal_slist;
pub const __pthread_mutex_s = struct___pthread_mutex_s;
pub const __pthread_rwlock_arch_t = struct___pthread_rwlock_arch_t;
pub const __pthread_cond_s = struct___pthread_cond_s;
pub const __jmp_buf_tag = struct___jmp_buf_tag;
pub const _pthread_cleanup_buffer = struct__pthread_cleanup_buffer;
pub const __cancel_jmp_buf_tag = struct___cancel_jmp_buf_tag;
pub const __pthread_cleanup_frame = struct___pthread_cleanup_frame;
pub const __va_list_tag = struct___va_list_tag;
