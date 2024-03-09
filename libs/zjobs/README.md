# zjobs v0.1.0 - Generic job queue implementation

In order to take full advantage of modern multicore CPUs, it is necessary to
run much of your application logic on separate threads.  This `JobQueue`
provides a simple API to schedule "jobs" to run on a pool of threads, typically
as many threads as your CPU supports, less 1 (the main thread).

Each "job" is a user-defined struct that declares a `exec` function that will
be executed on a background thread by the `JobQueue`.

## Getting started

Copy `zjobs` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zjobs = .{ .path = "libs/zjobs" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zjobs = b.dependency("zjobs", .{});
    exe.root_module.addImport("zjobs", zjobs.module("root"));
}
```

## Example Usage

The following example is also a `test` case in `zjobs.zig`:

```zig
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
jobs.start();

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
        fn exec(_: *@This()) void {
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
        fn exec(self: *@This()) void {
            self.jobs.stop();
        }
    }{ // trailing `{}` initializes an instance of this anonymous struct
        .jobs = &jobs, // and initializes a pointer to `jobs` here
    },
);

// Now that we're done, we can call `join()` to wait for all of the
// background threads to finish processing scheduled jobs.
jobs.join();
```
