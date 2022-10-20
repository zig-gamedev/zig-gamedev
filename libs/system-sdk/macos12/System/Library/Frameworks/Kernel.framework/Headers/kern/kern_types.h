/*
 * Copyright (c) 2000-2007 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */

#ifndef _KERN_KERN_TYPES_H_
#define _KERN_KERN_TYPES_H_

#include <stdint.h>
#include <mach/mach_types.h>
#include <mach/machine/vm_types.h>


typedef void *event_t;          /* wait event */
#define         NO_EVENT                        ((event_t) 0)

typedef uint64_t event64_t;             /* 64 bit wait event */
#define         NO_EVENT64              ((event64_t) 0)
#define         CAST_EVENT64_T(a_ptr)   ((event64_t)((uintptr_t)(a_ptr)))

/*
 *	Possible wait_result_t values.
 */
typedef int wait_result_t;
#define THREAD_WAITING          -1              /* thread is waiting */
#define THREAD_AWAKENED         0               /* normal wakeup */
#define THREAD_TIMED_OUT        1               /* timeout expired */
#define THREAD_INTERRUPTED      2               /* aborted/interrupted */
#define THREAD_RESTART          3               /* restart operation entirely */
#define THREAD_NOT_WAITING      10              /* thread didn't need to wait */

typedef void (*thread_continue_t)(void *, wait_result_t);
#define THREAD_CONTINUE_NULL    ((thread_continue_t) NULL)

/*
 * Interruptible flag for waits.
 *
 * THREAD_UNINT: Uninterruptible wait
 *   Wait will only end when someone explicitly wakes up the thread, or if the
 *   wait timeout expires.
 *
 *   Use this state if the system as a whole cannot recover from a thread being
 *   interrupted out of the wait.
 *
 * THREAD_INTERRUPTIBLE:
 *    Wait will end if someone explicitly wakes up the thread, the wait timeout
 *    expires,  or the current thread is being terminated.
 *
 *    This value can be used when your operation may not be cleanly restartable
 *    for the current process or thread (i.e. the loss of state would be only visible
 *    to the current client).  Since the thread is exiting anyways, you're willing
 *    to cut the operation short.  The system as a whole must be able to cleanly
 *    deal with the interruption (i.e. remain in a consistent and recoverable state).
 *
 * THREAD_ABORTSAFE:
 *    Wait will end if someone explicitly wakes up the thread, the wait timeout
 *    expires, the current thread is being terminated, if any signal arrives for
 *    the task, or thread_abort_safely() is called on the thread.
 *
 *    Using this value means that you are willing to be interrupted in the face
 *    of any user signal, and safely rewind the thread back to the user/kernel
 *    boundary.  Many syscalls will try to restart the operation they were performing
 *    after the signal has been handled.
 *
 *    You must provide this value for any unbounded wait - otherwise you will
 *    pend user signals forever.
 *
 * THREAD_WAIT_NOREPORT:
 *    The scheduler has a callback (sched_call) that some subsystems use to
 *    decide whether more threads should be thrown at a given problem by trying
 *    to maintain a good level of concurrency.
 *
 *    When the wait will not be helped by adding more threads (e.g. lock
 *    contention), using this flag as an argument to assert_wait* (or any of its
 *    wrappers) will prevent the next wait/block to cause thread creation.
 *
 *    This comes in two flavors: THREAD_WAIT_NOREPORT_KERNEL, and
 *    THREAD_WAIT_NOREPORT_USER to prevent reporting about the wait for kernel
 *    and user threads respectively.
 *
 * Thread interrupt mask:
 *
 *    The current maximum interruptible state for the thread, as set by
 *    thread_interrupt_level(), will limit the conditions that will cause a wake.
 *    This is useful for code that can't be interrupted to set before calling code
 *    that doesn't know that.
 *
 * Thread termination vs safe abort:
 *
 *    Termination abort: thread_abort(), thread_terminate()
 *
 *    A termination abort is sticky.  Once a thread is marked for termination, every
 *    THREAD_INTERRUPTIBLE wait will return immediately with THREAD_INTERRUPTED
 *    until the thread successfully exits.
 *
 *    Safe abort: thread_abort_safely()
 *
 *    A safe abort is not sticky.  The current wait, (or the next wait if the thread
 *    is not currently waiting) will be interrupted, but then the abort condition is cleared.
 *    The next wait will sleep as normal. Safe aborts only have a single effect.
 *
 *    The path back to the user/kernel boundary must not make any further unbounded
 *    wait calls. The waiter should detect the THREAD_INTERRUPTED return code
 *    from an ABORTSAFE wait and return an error code that causes its caller
 *    to understand that the current operation has been interrupted, and its
 *    caller should return a similar error code, and so on until the
 *    user/kernel boundary is reached.  For Mach, the error code is usually KERN_ABORTED,
 *    for BSD it is EINTR.
 *
 *    Debuggers rely on the safe abort mechanism - a signaled thread must return to
 *    the AST at the user/kernel boundary for the debugger to finish attaching.
 *
 *    No wait/block will ever disappear a thread out from under the waiter. The block
 *    call will always either return or call the passed in continuation.
 */
typedef int wait_interrupt_t;
#define THREAD_UNINT                    0x00000000  /* not interruptible      */
#define THREAD_INTERRUPTIBLE            0x00000001  /* may not be restartable */
#define THREAD_ABORTSAFE                0x00000002  /* abortable safely       */
#define THREAD_WAIT_NOREPORT_KERNEL     0x80000000
#define THREAD_WAIT_NOREPORT_USER       0x40000000
#define THREAD_WAIT_NOREPORT            (THREAD_WAIT_NOREPORT_KERNEL | THREAD_WAIT_NOREPORT_USER)

typedef int wait_timeout_urgency_t;
#define TIMEOUT_URGENCY_SYS_NORMAL      0x00            /* use default leeway thresholds for system */
#define TIMEOUT_URGENCY_SYS_CRITICAL    0x01            /* use critical leeway thresholds for system */
#define TIMEOUT_URGENCY_SYS_BACKGROUND  0x02            /* use background leeway thresholds for system */

#define TIMEOUT_URGENCY_USER_MASK       0x10            /* mask to identify user timeout urgency classes */
#define TIMEOUT_URGENCY_USER_NORMAL     0x10            /* use default leeway thresholds for user */
#define TIMEOUT_URGENCY_USER_CRITICAL   0x11            /* use critical leeway thresholds for user */
#define TIMEOUT_URGENCY_USER_BACKGROUND 0x12            /* use background leeway thresholds for user */

#define TIMEOUT_URGENCY_MASK            0x13            /* mask to identify timeout urgency */

#define TIMEOUT_URGENCY_LEEWAY          0x20            /* don't ignore provided leeway value */

#define TIMEOUT_URGENCY_FIRST_AVAIL     0x40            /* first available bit outside of urgency mask/leeway */
#define TIMEOUT_URGENCY_RATELIMITED     0x80

/*
 * Timeout and deadline tokens for waits.
 * The following tokens define common values for leeway and deadline parameters.
 */
#define TIMEOUT_NO_LEEWAY               (0ULL)
#define TIMEOUT_WAIT_FOREVER            (0ULL)


#endif  /* _KERN_KERN_TYPES_H_ */
