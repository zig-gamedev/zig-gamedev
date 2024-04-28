const std = @import("std");

const assert = std.debug.assert;

const panic = std.debug.panic;

const print = std.log.info;

const RingQueue = @import("ring_queue.zig").RingQueue;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ensure transitive closure of test coverage
comptime {
    _ = RingQueue;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pub const cache_line_size = 64;

pub const min_jobs = 16;

pub const Error = error{ Uninitialized, Stopped };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pub const JobId = enum(u32) {
    none,
    _, // non-exhaustive enum

    pub fn format(
        id: JobId,
        comptime _: []const u8,
        _: std.fmt.FormatOptions,
        writer: anytype,
    ) !void {
        const f = id.fields();
        return writer.print("{}:{}", .{ f.index, f.cycle });
    }

    pub inline fn cycle(id: JobId) u16 {
        return id.fields().cycle;
    }

    pub inline fn index(id: JobId) u16 {
        return id.fields().index;
    }

    inline fn fields(id: *const JobId) Fields {
        return @as(*const Fields, @ptrCast(id)).*;
    }

    const Fields = packed struct {
        cycle: u16, // lo bits
        index: u16, // hi bits

        inline fn init(_index: u16, _cycle: u16) Fields {
            return .{ .index = _index, .cycle = _cycle };
        }

        inline fn id(_fields: *const Fields) JobId {
            comptime assert(@sizeOf(Fields) == @sizeOf(JobId));
            return @as(*const JobId, @ptrCast(_fields)).*;
        }
    };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pub const QueueConfig = struct {
    max_jobs: u16 = 256,
    max_job_size: u16 = 64,
    max_threads: u8 = 32,
    idle_sleep_ns: u32 = 50,
};
/// Returns a struct that executes jobs on a pool of threads, which may be
/// configured as follows:
/// * `max_jobs` - the maximum number of jobs that can be waiting in the queue.
/// * `max_job_size` - the maximum size of a job struct that can be stored in
///    the queue.
/// * `max_threads` - the maximum number of threads that can be spawned by the
///   `JobQueue`. Even when `max_threads` is greater, the `JobQueue` will never
///   spawn more than `std.Thread.getCpuCount() - 1` threads.
/// * `idle_sleep_ns` - the maximum number of nanoseconds to sleep when a
///   thread is waiting for a job to become available.  When `idle_sleep_ns`
///   is `0`, idle threads will not sleep at all.
///
/// Open issues:
/// * `JobQueue` is not designed to support single threaded environments, and
///   has not been tested for correctness in case background threads cannot be
///   spawned.
pub fn JobQueue(comptime queue_config: QueueConfig) type {
    compileAssert(
        queue_config.max_jobs >= min_jobs,
        "config.max_jobs ({}) must be at least min_jobs ({})",
        .{ queue_config.max_jobs, min_jobs },
    );

    compileAssert(
        queue_config.max_job_size >= cache_line_size,
        "config.max_job_size ({}) must be at least cache_line_size ({})",
        .{ queue_config.max_job_size, cache_line_size },
    );

    compileAssert(
        queue_config.max_job_size % cache_line_size == 0,
        "config.max_job_size ({}) must be a multiple of cache_line_size ({})",
        .{ queue_config.max_job_size, cache_line_size },
    );

    const Atomic = std.atomic.Value;

    const Slot = struct {
        const Self = @This();

        pub const max_job_size = queue_config.max_job_size;

        const Data = [max_job_size]u8;
        const Main = *const fn (*Data) void;

        // zig fmt: off
        data           : Data align(cache_line_size) = undefined,
        exec           : Main align(cache_line_size) = undefined,
        id             : JobId                       = JobId.none,
        prereq         : JobId                       = JobId.none,
        cycle          : Atomic(u16)                 = .{ .raw = 0 },
        idle_mutex     : std.Thread.Mutex            = .{},
        idle_condition : std.Thread.Condition        = .{},
        // zig fmg: on

        fn storeJob(
            self: *Self,
            comptime Job: type,
            job: *const Job,
            index: usize,
            prereq: JobId,
        ) JobId {
            const old_cycle: u16 = self.cycle.load(.acquire);
            assert(isFreeCycle(old_cycle));

            const new_cycle: u16 = old_cycle +% 1;
            assert(isLiveCycle(new_cycle));

            {
                self.idle_mutex.lock();
                defer self.idle_mutex.unlock();

                const acquired: bool = null == self.cycle.cmpxchgStrong(
                    old_cycle,
                    new_cycle,
                    .monotonic,
                    .monotonic,
                );
                assert(acquired);
            }

            @memset(&self.data, 0);

            const job_bytes = std.mem.asBytes(job);
            @memcpy(self.data[0..job_bytes.len], job_bytes);

            const exec: *const fn (*Job) void = &@field(Job, "exec");
            const id = jobId(@as(u16, @truncate(index)), new_cycle);

            self.exec = @as(Main, @ptrCast(exec));
            self.id = id;
            self.prereq = if (prereq != id) prereq else JobId.none;
            return id;
        }

        fn executeJob(self: *Self, id: JobId) void {
            const old_id = @atomicLoad(JobId, &self.id, .monotonic);
            assert(old_id == id);

            const old_cycle: u16 = old_id.cycle();
            assert(isLiveCycle(old_cycle));

            const new_cycle: u16 = old_cycle +% 1;
            assert(isFreeCycle(new_cycle));

            self.exec(&self.data);

            {
                self.idle_mutex.lock();
                defer self.idle_mutex.unlock();
                const released: bool = null == self.cycle.cmpxchgStrong(
                    old_cycle,
                    new_cycle,
                    .monotonic,
                    .monotonic,
                );
                assert(released);

                self.idle_condition.broadcast();
            }
        }

        fn jobId(index: u16, cycle: u16) JobId {
            return JobId.Fields.init(index, cycle).id();
        }
    };

    compileAssert(
        @alignOf(Slot) == cache_line_size,
        "@alignOf({s}) ({}) not equal to cache_line_size ({})",
        .{ @typeName(Slot), @alignOf(Slot), cache_line_size },
    );

    compileAssert(
        @sizeOf(Slot) % cache_line_size == 0,
        "@sizeOf({s}) ({}) not a multiple of cache_line_size ({})",
        .{ @typeName(Slot), @sizeOf(Slot), cache_line_size },
    );

    return struct {

        pub const StartConfig = struct {
            num_threads: ?u8 = null,
        };

        pub const max_jobs: u16 = queue_config.max_jobs;

        pub const max_threads: u8 = queue_config.max_threads;

        pub const max_job_size: u16 = queue_config.max_job_size;

        pub const idle_sleep_ns: u64 = queue_config.idle_sleep_ns;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const Self = @This();
        const Instant = std.time.Instant;
        const Thread = std.Thread;
        const Mutex = Thread.Mutex;
        const ResetEvent = Thread.ResetEvent;

        const Slots = [max_jobs]Slot;
        const Threads = [max_threads]Thread;

        const FreeQueue = RingQueue(usize, max_jobs);
        const LiveQueue = RingQueue(JobId, max_jobs);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // zig fmt: off
        // slots first because they are cache-aligned
        _slots          : Slots        = [_]Slot{.{}} ** max_jobs,
        _threads        : Threads      = [_]Thread{undefined} ** max_threads,
        _mutex          : Mutex        = .{},
        _live_queue     : LiveQueue    = .{},
        _free_queue     : FreeQueue    = .{},
        _idle_event     : ResetEvent   = .{},
        _num_threads    : u64          = 0,
        _main_thread    : Atomic(u64)  = .{ .raw = 0 },
        _lock_thread    : Atomic(u64)  = .{ .raw = 0 },
        _initialized    : Atomic(bool) = .{ .raw = false },
        _started        : Atomic(bool) = .{ .raw = false },
        _running        : Atomic(bool) = .{ .raw = false },
        _stopping       : Atomic(bool) = .{ .raw = false },
        // zig fmt: on

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        /// Initializes the `JobQueue`, required before calling `start()`.
        pub fn init() Self {
            comptime compileAssert(
                @alignOf(Self) == cache_line_size,
                "@alignOf({s}) ({}) not equal to cache_line_size ({})",
                .{ @typeName(Self), @alignOf(Self), cache_line_size },
            );

            var self = Self{};

            // initialize free queue
            for (0..FreeQueue.capacity) |free_index| {
                self._free_queue.enqueueAssumeNotFull(free_index);
            }

            self._initialized.store(true, .monotonic);

            return self;
        }

        /// Calls `stop()` and `join()` as needed.
        pub fn deinit(self: *Self) void {
            if (self.isInitialized()) self.stop();
            if (self.isStarted()) self.join();
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        /// Spawn threads and begin executing jobs.
        /// `JobQueue` must be initialized, and not yet started or stopped.
        pub fn start(self: *Self, start_config: StartConfig) void {
            self.lock("start");
            defer self.unlock("start");

            const this_thread = Thread.getCurrentId();
            const prev_thread = self._main_thread.swap(this_thread, .monotonic);
            assert(prev_thread == 0);

            const was_initialized = self._initialized.load(.monotonic);
            assert(was_initialized == true);

            const was_started = self._started.swap(true, .monotonic);
            assert(was_started == false);

            const was_running = self._running.swap(true, .monotonic);
            assert(was_running == false);

            const was_stopping = self._stopping.load(.monotonic);
            assert(was_stopping == false);

            // spawn up to (num_cpus - 1) threads
            const num_cpus = num_cpus_blk: {
                if (start_config.num_threads) |num_threads| {
                    std.debug.assert(num_threads <= max_threads);
                    break :num_cpus_blk num_threads;
                }

                break :num_cpus_blk (Thread.getCpuCount() catch 2) - 1;
            };

            // zjobs does not currently support less than 2 threads
            std.debug.assert(num_cpus > 1);

            self._num_threads = @min(num_cpus, max_threads);
            for (self._threads[0..self._num_threads], 0..) |*thread, thread_index| {
                if (Thread.spawn(.{}, threadMain, .{self})) |spawned_thread| {
                    thread.* = spawned_thread;
                    nameThread(thread.*, "JobQueue[{}]", .{thread_index});
                } else |err| {
                    print("thread[{}]: {}\n", .{ thread_index, err });
                    self._num_threads = thread_index;
                    break;
                }
            }
        }

        /// Signals threads to stop running, and prevents scheduling more jobs.
        /// Call `stop()` from any thread.
        pub fn stop(self: *Self) void {
            if (!self.isRunning()) return;

            // signal threads to stop running
            const was_running = self._running.swap(false, .monotonic);
            assert(was_running == true);

            // prevent scheduling more jobs
            const was_stopping = self._stopping.swap(true, .monotonic);
            assert(was_stopping == false);

            self._idle_event.set();
        }

        /// Waits for all threads to finish, then executes any remaining jobs
        /// before returning.  After `join()` returns, the `JobQueue` has been
        /// reset to its default, uninitialized state.
        /// Call `join()` from the same thread that called `start()`.
        /// `JobQueue` must be initialized and started before calling `join()`.
        /// You may call `join()` before calling `stop()`, but since `join()`
        /// will not return until after `stop()` is called, you must then call
        /// `stop()` from another thread, e.g. from a job.
        pub fn join(self: *Self) void {
            assert(self.isMainThread());

            if (!self.isStarted()) return;

            for (self._threads[0..self._num_threads]) |thread| {
                thread.join();
            }

            // drain job queue
            assert(self.isUnlockedThread());
            self.executeJobs(.unlocked, .dequeue_jobid_after_join);

            // reset to default state
            self.* = Self{};
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        /// Returns `true` if `init()` has been called, and `join()` has not
        /// yet run to completion.
        pub fn isInitialized(self: *const Self) bool {
            return self._initialized.load(.monotonic);
        }

        /// Returns `true` if `start()` has been called, and `join()` has not
        /// yet run to completion.
        pub fn isStarted(self: *const Self) bool {
            return self._started.load(.monotonic);
        }

        /// Returns `true` if `start()` has been called, and `stop()` has not
        /// yet been called.
        pub fn isRunning(self: *const Self) bool {
            return self._running.load(.monotonic);
        }

        /// Returns `true` if `stop()` has been called, and `join()` has not
        /// yet run to completion.
        pub fn isStopping(self: *const Self) bool {
            return self._stopping.load(.monotonic);
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        /// Returns `true` if the provided `JobId` identifies a job that has
        /// been completed.
        /// Returns `false` for a `JobId` that is scheduled and has not yet
        /// completed execution.
        /// Always returns `true` for `JobId.none`, which is always considered
        /// trivially complete.
        pub fn isComplete(self: *const Self, id: JobId) bool {
            if (id == JobId.none) return true;

            const _id = id.fields();
            assert(isLiveCycle(_id.cycle));

            const slot: *const Slot = &self._slots[_id.index];
            const slot_cycle = slot.cycle.load(.monotonic);
            return slot_cycle != _id.cycle;
        }

        /// Returns `true` if the provided `JobId` identifies a job that has
        /// been scheduled and has not yet completed execution.  The job may or
        /// may not already be executing on a background thread.
        /// Returns `false` for a JobId that has not been scheduled, or has
        /// already completed.
        /// Always returns false for `JobId.none`, which is considered
        /// trivially complete.
        pub fn isPending(self: *const Self, id: JobId) bool {
            if (id == JobId.none) return false;

            const _id = id.fields();
            assert(isLiveCycle(_id.cycle));

            const slot: *const Slot = &self._slots[_id.index];
            const slot_cycle = slot.cycle.load(.monotonic);
            return slot_cycle == _id.cycle;
        }

        /// Returns the number of jobs waiting in the queue.
        /// Only includes jobs that have not yet begun execution.
        pub fn numWaiting(self: *const Self) usize {
            return self._live_queue.len();
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        /// Inserts a job into the queue, and returns a `JobId` that can be
        /// used to specify dependencies between jobs.
        ///
        /// `prereq` - specifies a job that must run to completion before the
        /// job being scheduled can begin.  To schedule a job that must wait
        /// for more than one job to complete, call `combine()` to consolidate
        /// a set of `JobIds` into a single `JobId` that can be provided as the
        /// `prereq` argument to `schedule()`.
        ///
        /// `job` - provides an instance of a struct that captures the context
        /// and provides a function that will be executed on a separate thread.
        /// The provided `job` must satisfy the following requirements:
        /// * The total size of the job must not exceed `config.max_job_size`
        /// * The job must be an instance of a struct
        /// * The job must declare a public function named `exec` with one of the
        ///   following supported signatures:
        /// ```
        ///   pub fn exec(*@This())
        ///   pub fn exec(*const @This())
        /// ```
        pub fn schedule(self: *Self, prereq: JobId, job: anytype) Error!JobId {
            const Job = @TypeOf(job);
            validateJob(Job);

            self.lock("schedule");
            defer self.unlock("schedule");

            if (!self.isInitialized()) return Error.Uninitialized;
            if (self.isStopping()) return Error.Stopped;

            const index = self.dequeueFreeIndex();
            const slot: *Slot = &self._slots[index];
            const id = slot.storeJob(Job, &job, index, prereq);
            self.enqueueJobId(id);
            return id;
        }

        /// Combines zero or more `JobIds` into a single `JobId` that can be
        /// provided to the `prereq` argument when calling `schedule()`.
        /// This enables scheduling jobs that must wait on the completion of
        /// an arbitrary number of other jobs.
        /// Returns `JobId.none` when `prereqs` is empty.
        /// Returns `prereqs[0]` when `prereqs` contains only one element.
        pub fn combine(self: *Self, prereqs: []const JobId) Error!JobId {
            if (prereqs.len == 0) return JobId.none;
            if (prereqs.len == 1) return prereqs[0];

            var id = JobId.none;
            var in: []const JobId = prereqs;
            while (in.len > 0) {
                var job = CombinePrereqsJob{ .jobs = self };

                // copy prereqs to job
                const out: []JobId = &job.prereqs;

                const copy_len = @min(in.len, out.len);
                std.mem.copyForwards(JobId, out, in[0..copy_len]);
                in = in[copy_len..];

                id = try self.schedule(id, job);
            }
            return id;
        }

        /// Waits until the specified `prereq` is completed.
        pub fn wait(self: *Self, prereq: JobId) void {
            if (prereq == JobId.none) return;

            const _id = prereq.fields();
            assert(isLiveCycle(_id.cycle));

            const slot: *Slot = &self._slots[_id.index];

            {
                slot.idle_mutex.lock();
                defer slot.idle_mutex.unlock();
                const slot_cycle = slot.cycle.load(.monotonic);

                if (slot_cycle == _id.cycle) {
                    slot.idle_condition.wait(&slot.idle_mutex);
                }
            }
        }

        //----------------------------------------------------------------------

        fn dequeueFreeIndex(self: *Self) usize {
            assert(self.isLockedThread());

            if (self._free_queue.dequeueIfNotEmpty()) |index| {
                return index;
            }

            while (true) {
                // must process jobs to acquire free index
                const id = self._live_queue.dequeueAssumeNotEmpty();
                if (self.executeJob(id, .locked, .acquire_free_index)) |index| {
                    return index;
                }
            }
        }

        fn enqueueJobId(self: *Self, new_id: JobId) void {
            assert(self.isLockedThread());

            while (self._live_queue.isFull()) {
                // must process jobs to unblock live queue
                const old_id = self._live_queue.dequeueAssumeNotEmpty();
                self.executeJob(old_id, .locked, .enqueue_free_index);
            }

            const was_empty = self._live_queue.isEmpty();

            self._live_queue.enqueueAssumeNotFull(new_id);

            if (was_empty) {
                self._idle_event.set();
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn threadMain(self: *Self) void {
            assert(self.notMainThread());
            assert(self.isUnlockedThread());

            while (self.isRunning()) {
                if (self._live_queue.isEmpty() and self.isStopping() == false) {
                    self._idle_event.wait();
                }

                self.executeJobs(.unlocked, .dequeue_jobid_if_running);
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn nameThread(t: Thread, comptime fmt: []const u8, args: anytype) void {
            var buf: [Thread.max_name_len]u8 = undefined;
            if (std.fmt.bufPrint(&buf, fmt, args)) |name| {
                t.setName(name) catch |err| ignore(err);
            } else |err| {
                ignore(err);
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const CombinePrereqsJob = struct {
            const jobs_size = @sizeOf(*Self);
            const prereq_size = @sizeOf(JobId);
            const max_prereqs = (max_job_size - jobs_size) / prereq_size;

            jobs: *Self,
            prereqs: [max_prereqs]JobId = [_]JobId{JobId.none} ** max_prereqs,

            pub fn exec(job: *@This()) void {
                for (job.prereqs) |prereq| {
                    job.jobs.wait(prereq);
                }
            }
        };

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const ExecuteJobResult = enum {
            acquire_free_index,
            enqueue_free_index,
            dequeue_jobid_after_join,
            dequeue_jobid_if_running,
        };

        fn ExecuteJobReturnType(comptime result: ExecuteJobResult) type {
            return switch (result) {
                // zig fmt: off
                .acquire_free_index       => ?usize,
                .enqueue_free_index       => void,
                .dequeue_jobid_after_join => ?JobId,
                .dequeue_jobid_if_running => ?JobId,
                // zig fmt: on
            };
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        fn executeJobs(
            self: *Self,
            comptime scope: LockScope,
            comptime result: ExecuteJobResult,
        ) void {
            var id = JobId.none;
            if (self.acquireJobId(scope, result)) |a| {
                id = a;
                while (self.executeJob(id, scope, result)) |b| {
                    id = b;
                }
            }
        }

        inline fn acquireJobId(
            self: *Self,
            comptime scope: LockScope,
            comptime result: ExecuteJobResult,
        ) ?JobId {
            if (self._live_queue.isEmpty()) return null;

            switch (result) {
                .acquire_free_index => unreachable,
                .enqueue_free_index => unreachable,
                .dequeue_jobid_after_join => {
                    assert(self.isMainThread());
                    return self._live_queue.dequeueIfNotEmpty();
                },
                .dequeue_jobid_if_running => {
                    assert(self.notMainThread());

                    self.lockIfScopeUnlocked("dequeueJobId", scope);
                    defer self.unlockIfScopeUnlocked("dequeueJobId", scope);

                    assert(self.isLockedThread());
                    return self._live_queue.dequeueIfNotEmpty();
                },
            }
        }

        fn executeJob(
            self: *Self,
            id: JobId,
            comptime scope: LockScope,
            comptime result: ExecuteJobResult,
        ) ExecuteJobReturnType(result) {
            const _id = id.fields();
            assert(isLiveCycle(_id.cycle));

            // this index was assigned to us,
            // no other threads should be reading or writing this slot,
            // so we don't need to be locked to read/write here
            const slot: *Slot = &self._slots[_id.index];
            assert(slot.id == id);
            assert(slot.cycle.load(.monotonic) == _id.cycle);
            assert(slot.prereq != id);

            {
                self.unlockIfScopeLocked("executeJob(a)", scope);
                defer self.lockIfScopeLocked("executeJob(a)", scope);

                // we cannot be locked when executing a job,
                // because the job may call schedule() or stop()
                assert(self.isUnlockedThread());
                self.wait(slot.prereq);
                slot.executeJob(id);
            }

            const free_index = _id.index;

            switch (result) {
                .acquire_free_index => {
                    return free_index;
                },
                .enqueue_free_index => {
                    self.lockIfScopeUnlocked("executeJob(b)", scope);
                    defer self.unlockIfScopeUnlocked("executeJob(b)", scope);

                    assert(self.isLockedThread());
                    self._free_queue.enqueueAssumeNotFull(free_index);
                    return;
                },
                .dequeue_jobid_after_join => {
                    assert(self.isMainThread());
                    return self._live_queue.dequeueIfNotEmpty();
                },
                .dequeue_jobid_if_running => {
                    assert(self.notMainThread());

                    self.lockIfScopeUnlocked("executeJob(d)", scope);
                    defer self.unlockIfScopeUnlocked("executeJob(d)", scope);

                    assert(self.isLockedThread());
                    self._free_queue.enqueueAssumeNotFull(free_index);
                    if (self.isRunning()) {
                        return self._live_queue.dequeueIfNotEmpty();
                    } else {
                        return null;
                    }
                },
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        const LockScope = enum { unlocked, locked };

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn lockIfScopeLocked(
            self: *Self,
            comptime site: []const u8,
            comptime scope: LockScope,
        ) void {
            switch (scope) {
                .unlocked => assert(self.isUnlockedThread()),
                .locked => self.lock(site),
            }
        }

        inline fn unlockIfScopeLocked(
            self: *Self,
            comptime site: []const u8,
            comptime scope: LockScope,
        ) void {
            switch (scope) {
                .unlocked => assert(self.isUnlockedThread()),
                .locked => self.unlock(site),
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn lockIfScopeUnlocked(
            self: *Self,
            comptime site: []const u8,
            comptime scope: LockScope,
        ) void {
            switch (scope) {
                .unlocked => self.lock(site),
                .locked => assert(self.isLockedThread()),
            }
        }

        inline fn unlockIfScopeUnlocked(
            self: *Self,
            comptime site: []const u8,
            comptime scope: LockScope,
        ) void {
            switch (scope) {
                .unlocked => self.unlock(site),
                .locked => assert(self.isLockedThread()),
            }
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn lock(self: *Self, comptime site: []const u8) void {
            const this_thread = Thread.getCurrentId();
            const lock_thread = self._lock_thread.load(.acquire);
            assert(this_thread != lock_thread);
            assert(site.len > 0);

            self._mutex.lock();
            self._lock_thread.store(this_thread, .release);
        }

        inline fn unlock(self: *Self, comptime site: []const u8) void {
            const this_thread = Thread.getCurrentId();
            const lock_thread = self._lock_thread.swap(0, .monotonic);
            assert(this_thread == lock_thread);
            assert(site.len > 0);

            self._mutex.unlock();
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn isLockedThread(self: *const Self) bool {
            const this_thread = Thread.getCurrentId();
            const lock_thread = self._lock_thread.load(.acquire);
            return lock_thread == this_thread;
        }

        inline fn isUnlockedThread(self: *const Self) bool {
            const this_thread = Thread.getCurrentId();
            const lock_thread = self._lock_thread.load(.acquire);
            return lock_thread != this_thread;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn isMainThread(self: *const Self) bool {
            const this_thread = Thread.getCurrentId();
            const main_thread = self._main_thread.load(.monotonic);
            assert(main_thread != 0);
            return main_thread == this_thread;
        }

        inline fn notMainThread(self: *const Self) bool {
            const this_thread = Thread.getCurrentId();
            const main_thread = self._main_thread.load(.monotonic);
            assert(main_thread != 0);
            return main_thread != this_thread;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub fn validateJob(comptime Job: type) void {
            comptime {
                const struct_info = switch (@typeInfo(Job)) {
                    .Struct => |info| info,
                    else => {
                        compileError("{s} must be a struct", .{@typeName(Job)});
                        unreachable;
                    },
                };

                compileAssert(
                    @sizeOf(Job) <= max_job_size,
                    "@sizeOf({s}) ({}) exceeds max_job_size ({})",
                    .{ @typeName(Job), @sizeOf(Job), max_job_size },
                );

                for (struct_info.decls) |decl| {
                    if (std.mem.eql(u8, decl.name, "exec")) {
                        break;
                    }
                } else {
                    compileError(
                        "{s}.exec(*{s}) is either not declared or not public",
                        .{ @typeName(Job), @typeName(Job) },
                    );
                }

                const Main = @TypeOf(@field(Job, "exec"));
                const fn_info = switch (@typeInfo(Main)) {
                    .Fn => |info| info,
                    else => {
                        compileError(
                            "{s}.exec must be a function",
                            .{@typeName(Job)},
                        );
                        unreachable;
                    },
                };

                compileAssert(
                    fn_info.is_generic == false,
                    "{s}.exec() must not be generic",
                    .{@typeName(Job)},
                );

                compileAssert(
                    fn_info.is_var_args == false,
                    "{s}.exec() must not have variadic arguments",
                    .{@typeName(Job)},
                );

                compileAssert(
                    fn_info.return_type != null,
                    "{s}.exec() must return void",
                    .{@typeName(Job)},
                );

                compileAssert(
                    fn_info.return_type == void,
                    "{s}.exec() must return void, not {s}",
                    .{ @typeName(Job), @typeName(fn_info.return_type.?) },
                );

                compileAssert(
                    fn_info.params.len > 0,
                    "{s}.exec() must have at least one parameter",
                    .{@typeName(Job)},
                );

                const arg_type_0 = fn_info.params[0].type;

                compileAssert(
                    arg_type_0 == *Job or arg_type_0 == *const Job,
                    "{s}.exec() must accept *@This() or *const @This() as first parameter",
                    .{@typeName(Job)},
                );
            }
        }
    };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline fn isFreeCycle(cycle: u64) bool {
    return (cycle & 1) == 0;
}

inline fn isLiveCycle(cycle: u64) bool {
    return (cycle & 1) == 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

fn compileError(
    comptime format: []const u8,
    comptime args: anytype,
) void {
    @compileError(std.fmt.comptimePrint(format, args));
}

fn compileAssert(
    comptime ok: bool,
    comptime format: []const u8,
    comptime args: anytype,
) void {
    if (!ok) compileError(format, args);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline fn ignore(_: anytype) void {}

inline fn now() std.time.Instant {
    return std.time.Instant.now() catch unreachable;
}

////////////////////////////////// T E S T S ///////////////////////////////////

test "JobQueue example" {
    print("\n", .{});

    const Jobs = JobQueue(.{}); // a default-configured `JobQueue` type

    var jobs = Jobs.init(); // initialize an instance of `Jobs`
    defer jobs.deinit(); // ensure that `jobs` is cleaned up when we're done

    // First we will define a job that will print "hello " when it runs.
    // The job must declare a `exec` function, which defines the code that
    // will be executed on a background thread by the `JobQueue`.
    // Our `HelloJob` doesn't contain any member variables, but it could.
    // We will see an example of a job with some member variables below.
    const HelloJob = struct {
        pub fn exec(_: *@This()) void {
            print("hello ", .{});
        }
    };

    // Now we will schedule an instance of `HelloJob` to run on a separate
    // thread.
    // The `schedule()` function returns a `JobId`, which is how we can refer
    // to this job to determine when it is done.
    // The first argument to `schedule()` is the `prereq`, which
    // specifies a job that must be completed before this job can run.
    // Here, we specify the `prereq` of `JobId.none`, which means that
    // this job does not need to wait for any other jobs to complete.
    // The second argument to `schedule()` is the `job`, which is a user-
    // defined struct that declares a `exec` function that will be executed
    // on a background thread.
    // Here we are providing an instance of our `HelloJob` defined above.
    const hello_job_id: JobId = try jobs.schedule(
        JobId.none, // does not wait for any other job
        HelloJob{}, // runs `HelloJob.exec()` on another thread
    );

    // Scheduled jobs will not execute until `start()` is called.
    // The `start()` function spawns the threads that will run the jobs.
    // Note that we can schedule jobs before and after calling `start()`.
    jobs.start(.{});

    // Now we will schedule a second job that will print "world!" when it runs.
    // We want this job to run after the `HelloJob` completes, so we provide
    // `hello_job_id` as the `prereq` when scheduling this job.
    // This ensures that the string "hello " will be printed before we print
    // the string "world!\n"
    // This time, we will use an anonymous struct to declare the job directly
    // within the call to `schedule()`.
    // Note the trailing empty braces, `{}`, which initialize an instance of
    // this anonymous struct.
    const world_job_id: JobId = try jobs.schedule(
        hello_job_id, // waits for `hello_job_id` to be completed
        struct {
            pub fn exec(_: *@This()) void {
                print("world!\n", .{});
            }
        }{}, // trailing `{}` initializes an instance of this anonymous struct
    );

    // When we want to shut down all of the background threads, we can call
    // the `stop()` function.
    // Here we will schedule a job to call `stop()` after our "world!" job
    // completes.
    // This ensures that the string "hello world!\n" will be printed before
    // we stop running our jobs.
    // Note that our anonymous "stop job" captures a pointer to `jobs` so that
    // it can call `stop()`.
    _ = try jobs.schedule(
        world_job_id, // waits for `world_job_id` to be completed
        struct {
            jobs: *Jobs, // stores a pointer to `jobs`
            pub fn exec(self: *@This()) void {
                self.jobs.stop();
            }
        }{ // trailing `{}` initializes an instance of this anonymous struct
            .jobs = &jobs, // and initializes a pointer to `jobs` here
        },
    );

    // Now that we're done, we can call `join()` to wait for all of the
    // background threads to finish processing scheduled jobs.
    jobs.join();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

test "JobQueue throughput" {
    const Jobs = JobQueue(.{
        .max_threads = 8,
    });

    print("\n@sizeOf(Jobs):{}\n", .{@sizeOf(Jobs)});

    const main_thread = std.Thread.getCurrentId();
    print("main_thread: {}\n", .{main_thread});

    const job_workload_size = cache_line_size * 1024 * 1024;
    const job_count = blk: {
        const total_memory = std.process.totalSystemMemory() catch break :blk min_jobs;
        const upper_bound = @as(usize, @intFromFloat(@as(f64, @floatFromInt(total_memory)) * 0.667));
        var count: usize = min_jobs * 4;
        while (job_workload_size * count > upper_bound and count > min_jobs) : (count -= min_jobs) {}
        break :blk count;
    };

    const JobWorkload = struct {
        const Unit = u64;
        const unit_size = @sizeOf(Unit);
        const unit_count = job_workload_size / unit_size;
        units: [unit_count]Unit align(cache_line_size) = [_]Unit{undefined} ** unit_count,
    };

    var allocator = std.testing.allocator;
    var job_workloads: []JobWorkload = try allocator.alignedAlloc(JobWorkload, @alignOf(JobWorkload), job_count);
    defer print("allocator.free(job_workloads) DONE\n", .{});
    defer allocator.free(job_workloads);
    defer print("allocator.free(job_workloads)...\n", .{});

    const JobStat = struct {
        main: std.Thread.Id = 0,
        thread: std.Thread.Id = 0,
        started: std.time.Instant = undefined,
        stopped: std.time.Instant = undefined,

        fn start(self: *@This()) void {
            self.thread = std.Thread.getCurrentId();
            self.started = now();
        }

        fn stop(self: *@This()) void {
            assert(self.thread == std.Thread.getCurrentId());
            self.stopped = now();
        }

        fn ms(self: @This()) u64 {
            return self.stopped.since(self.started) / std.time.ns_per_ms;
        }

        pub fn format(
            self: @This(),
            comptime _: []const u8,
            _: std.fmt.FormatOptions,
            writer: anytype,
        ) !void {
            if (self.thread == self.main) {
                return writer.print(
                    "ran on the main thread and took {}ms",
                    .{self.ms()},
                );
            } else {
                return writer.print(
                    "ran on thread id {} and took {}ms",
                    .{ self.thread, self.ms() },
                );
            }
        }
    };

    const job_stats = try allocator.alloc(JobStat, job_count);
    defer allocator.free(job_stats);
    for (job_stats) |*job_stat| {
        job_stat.* = .{ .main = main_thread };
    }

    const FillJob = struct {
        stat: *JobStat,
        workload: *JobWorkload,

        pub fn exec(self: *@This()) void {
            self.stat.start();
            defer self.stat.stop();

            assert(@intFromPtr(self.workload) % 64 == 0);
            const thread: u64 = self.stat.thread;
            for (&self.workload.units, 0..) |*unit, index| {
                unit.* = thread +% index;
            }
        }
    };

    var jobs = Jobs.init();
    defer jobs.deinit();

    // schedule job_count jobs to fill some arrays
    for (job_stats, 0..) |*job_stat, i| {
        _ = try jobs.schedule(.none, FillJob{
            .stat = job_stat,
            .workload = &job_workloads[i % job_count],
        });
    }

    // schedule a job to stop the job queue
    _ = try jobs.schedule(.none, struct {
        jobs: *Jobs,
        pub fn exec(self: *@This()) void {
            self.jobs.stop();
        }
    }{ .jobs = &jobs });

    jobs.start(.{});
    const started = now();

    jobs.join();
    const stopped = now();
    const main_ms = stopped.since(started) / std.time.ns_per_ms;
    var job_ms: u64 = 0;

    for (job_stats, 0..) |job_stat, i| {
        print("    job {} {}\n", .{ i, job_stat });
        job_ms += job_stat.ms();
    }

    const throughput = @as(f64, @floatFromInt(job_ms)) / @as(f64, @floatFromInt(main_ms));
    print("completed {} jobs ({}ms) in {}ms ({d:.1}x)\n", .{ job_count, job_ms, main_ms, throughput });
}

test "combine jobs respect prereq" {
    const Jobs = JobQueue(.{});
    var jobs = Jobs.init();
    defer jobs.deinit();

    const Counter = std.atomic.Value(u32);

    const PrereqJob = struct {
        counter: *Counter,

        pub fn exec(self: *@This()) void {
            _ = self.counter.fetchAdd(1, .monotonic);
        }
    };

    const FinalJob = struct {
        counter: *Counter,

        pub fn exec(self: *@This()) void {
            const count = self.counter.load(.monotonic);
            self.counter.store(count * 100, .monotonic);
        }
    };

    var counter = try std.testing.allocator.create(Counter);
    defer std.testing.allocator.destroy(counter);
    counter.* = Counter.init(0);

    jobs.start(.{});
    var stressers: usize = 0;
    while (stressers < 1000) : (stressers += 1) {
        // Generate more prereqs than fit in a single CombinePrereqsJob
        const prereq_count = Jobs.CombinePrereqsJob.max_prereqs + 2;
        var chain_data: [prereq_count]PrereqJob = undefined;
        for (&chain_data) |*step| {
            step.* = PrereqJob{ .counter = counter };
        }

        var prereqs: [prereq_count]JobId = undefined;
        for (chain_data, 0..) |step, i| {
            prereqs[i] = try jobs.schedule(JobId.none, step);
        }

        const final = FinalJob{ .counter = counter };
        const combined_prereq = try jobs.combine(&prereqs);
        const final_job = try jobs.schedule(combined_prereq, final);

        jobs.wait(final_job);
        try std.testing.expectEqual(@as(u32, prereq_count * 100), counter.load(.monotonic));
        counter.store(0, .monotonic);
    }

    jobs.deinit();
}
