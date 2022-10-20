/*
 * Copyright (c) 2003-2019 Apple Inc. All rights reserved.
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

#ifndef _KERN_LOCKS_H_
#define _KERN_LOCKS_H_

#include <sys/cdefs.h>


__BEGIN_DECLS

#include <sys/appleapiopts.h>
#include <mach/boolean.h>
#include <kern/kern_types.h>
#include <kern/lock_group.h>
#include <machine/locks.h>
#include <kern/lock_types.h>
#include <kern/lock_attr.h>
#include <kern/lock_rw.h>

#define decl_lck_spin_data(class, name)     class lck_spin_t name

extern lck_spin_t      *lck_spin_alloc_init(
	lck_grp_t               *grp,
	lck_attr_t              *attr);

extern void             lck_spin_init(
	lck_spin_t              *lck,
	lck_grp_t               *grp,
	lck_attr_t              *attr);

extern void             lck_spin_lock(
	lck_spin_t              *lck);

extern void             lck_spin_lock_grp(
	lck_spin_t              *lck,
	lck_grp_t               *grp);

extern void             lck_spin_unlock(
	lck_spin_t              *lck);

extern void             lck_spin_destroy(
	lck_spin_t              *lck,
	lck_grp_t               *grp);

extern void             lck_spin_free(
	lck_spin_t              *lck,
	lck_grp_t               *grp);

extern wait_result_t    lck_spin_sleep(
	lck_spin_t              *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible);

extern wait_result_t    lck_spin_sleep_grp(
	lck_spin_t              *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible,
	lck_grp_t               *grp);

extern wait_result_t    lck_spin_sleep_deadline(
	lck_spin_t              *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible,
	uint64_t                deadline);


#define decl_lck_mtx_data(class, name)     class lck_mtx_t name

extern lck_mtx_t        *lck_mtx_alloc_init(
	lck_grp_t               *grp,
	lck_attr_t              *attr);

extern void             lck_mtx_init(
	lck_mtx_t               *lck,
	lck_grp_t               *grp,
	lck_attr_t              *attr);
extern void             lck_mtx_lock(
	lck_mtx_t               *lck);

extern void             lck_mtx_unlock(
	lck_mtx_t               *lck);

extern void             lck_mtx_destroy(
	lck_mtx_t               *lck,
	lck_grp_t               *grp);

extern void             lck_mtx_free(
	lck_mtx_t               *lck,
	lck_grp_t               *grp);

extern wait_result_t    lck_mtx_sleep(
	lck_mtx_t               *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible);

extern wait_result_t    lck_mtx_sleep_deadline(
	lck_mtx_t               *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible,
	uint64_t                deadline);


#if DEVELOPMENT || DEBUG
#define FULL_CONTENDED 0
#define HALF_CONTENDED 1
#define MAX_CONDENDED  2

extern void             erase_all_test_mtx_stats(void);
extern int              get_test_mtx_stats_string(char* buffer, int buffer_size);
extern void             lck_mtx_test_init(void);
extern void             lck_mtx_test_lock(void);
extern void             lck_mtx_test_unlock(void);
extern int              lck_mtx_test_mtx_uncontended(int iter, char* buffer, int buffer_size);
extern int              lck_mtx_test_mtx_contended(int iter, char* buffer, int buffer_size, int type);
extern int              lck_mtx_test_mtx_uncontended_loop_time(int iter, char* buffer, int buffer_size);
extern int              lck_mtx_test_mtx_contended_loop_time(int iter, char* buffer, int buffer_size, int type);
#endif

extern void             lck_mtx_assert(
	lck_mtx_t               *lck,
	unsigned                int    type);

#if MACH_ASSERT
#define LCK_MTX_ASSERT(lck, type) lck_mtx_assert((lck),(type))
#define LCK_SPIN_ASSERT(lck, type) lck_spin_assert((lck),(type))
#else /* MACH_ASSERT */
#define LCK_MTX_ASSERT(lck, type)
#define LCK_SPIN_ASSERT(lck, type)
#endif /* MACH_ASSERT */

#if DEBUG
#define LCK_MTX_ASSERT_DEBUG(lck, type) lck_mtx_assert((lck),(type))
#define LCK_SPIN_ASSERT_DEBUG(lck, type) lck_spin_assert((lck),(type))
#else /* DEBUG */
#define LCK_MTX_ASSERT_DEBUG(lck, type)
#define LCK_SPIN_ASSERT_DEBUG(lck, type)
#endif /* DEBUG */

#define LCK_ASSERT_OWNED                1
#define LCK_ASSERT_NOTOWNED             2

#define LCK_MTX_ASSERT_OWNED    LCK_ASSERT_OWNED
#define LCK_MTX_ASSERT_NOTOWNED LCK_ASSERT_NOTOWNED


__END_DECLS

#endif /* _KERN_LOCKS_H_ */
