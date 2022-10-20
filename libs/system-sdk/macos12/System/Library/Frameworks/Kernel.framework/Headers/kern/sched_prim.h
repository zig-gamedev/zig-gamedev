/*
 * Copyright (c) 2000-2012 Apple Inc. All rights reserved.
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
/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */
/*
 *	File:	sched_prim.h
 *	Author:	David Golub
 *
 *	Scheduling primitive definitions file
 *
 */

#ifndef _KERN_SCHED_PRIM_H_
#define _KERN_SCHED_PRIM_H_

#include <sys/cdefs.h>
#include <mach/boolean.h>
#include <mach/machine/vm_types.h>
#include <mach/kern_return.h>
#include <kern/clock.h>
#include <kern/kern_types.h>
#include <kern/percpu.h>
#include <kern/thread.h>
#include <kern/block_hint.h>

extern int              thread_get_current_cpuid(void);


__BEGIN_DECLS




/* Context switch */
extern wait_result_t    thread_block(
	thread_continue_t       continuation);

extern wait_result_t    thread_block_parameter(
	thread_continue_t       continuation,
	void                            *parameter);

/* Declare thread will wait on a particular event */
extern wait_result_t    assert_wait(
	event_t                         event,
	wait_interrupt_t        interruptible);

/* Assert that the thread intends to wait with a timeout */
extern wait_result_t    assert_wait_timeout(
	event_t                         event,
	wait_interrupt_t        interruptible,
	uint32_t                        interval,
	uint32_t                        scale_factor);

/* Assert that the thread intends to wait with an urgency, timeout and leeway */
extern wait_result_t    assert_wait_timeout_with_leeway(
	event_t                         event,
	wait_interrupt_t        interruptible,
	wait_timeout_urgency_t  urgency,
	uint32_t                        interval,
	uint32_t                        leeway,
	uint32_t                        scale_factor);

extern wait_result_t    assert_wait_deadline(
	event_t                         event,
	wait_interrupt_t        interruptible,
	uint64_t                        deadline);

/* Assert that the thread intends to wait with an urgency, deadline, and leeway */
extern wait_result_t    assert_wait_deadline_with_leeway(
	event_t                         event,
	wait_interrupt_t        interruptible,
	wait_timeout_urgency_t  urgency,
	uint64_t                        deadline,
	uint64_t                        leeway);

/* Wake up thread (or threads) waiting on a particular event */
extern kern_return_t    thread_wakeup_prim(
	event_t                         event,
	boolean_t                       one_thread,
	wait_result_t                   result);

#define thread_wakeup(x)                                        \
	                thread_wakeup_prim((x), FALSE, THREAD_AWAKENED)
#define thread_wakeup_with_result(x, z)         \
	                thread_wakeup_prim((x), FALSE, (z))
#define thread_wakeup_one(x)                            \
	                thread_wakeup_prim((x), TRUE, THREAD_AWAKENED)

/* Wakeup the specified thread if it is waiting on this event */
extern kern_return_t thread_wakeup_thread(event_t event, thread_t thread);

extern boolean_t preemption_enabled(void);


__END_DECLS

#endif  /* _KERN_SCHED_PRIM_H_ */
