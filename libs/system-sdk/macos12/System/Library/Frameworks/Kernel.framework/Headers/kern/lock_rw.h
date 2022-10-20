/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
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

#ifndef _KERN_RW_LOCK_H_
#define _KERN_RW_LOCK_H_

#include <kern/lock_types.h>
#include <kern/lock_group.h>
#include <kern/lock_attr.h>


__BEGIN_DECLS


typedef struct __lck_rw_t__     lck_rw_t;

#if DEVELOPMENT || DEBUG
#endif /* DEVELOPMENT || DEBUG */

typedef unsigned int     lck_rw_type_t;

#define LCK_RW_TYPE_SHARED              0x01
#define LCK_RW_TYPE_EXCLUSIVE           0x02

#define decl_lck_rw_data(class, name)   class lck_rw_t name


#if MACH_ASSERT
#define LCK_RW_ASSERT(lck, type) lck_rw_assert((lck),(type))
#else /* MACH_ASSERT */
#define LCK_RW_ASSERT(lck, type)
#endif /* MACH_ASSERT */

#if DEBUG
#define LCK_RW_ASSERT_DEBUG(lck, type) lck_rw_assert((lck),(type))
#else /* DEBUG */
#define LCK_RW_ASSERT_DEBUG(lck, type)
#endif /* DEBUG */

/*!
 * @function lck_rw_alloc_init
 *
 * @abstract
 * Allocates and initializes a rw_lock_t.
 *
 * @discussion
 * The function can block. See lck_rw_init() for initialization details.
 *
 * @param grp           lock group to associate with the lock.
 * @param attr          lock attribute to initialize the lock.
 *
 * @returns             NULL or the allocated lock
 */
extern lck_rw_t         *lck_rw_alloc_init(
	lck_grp_t               *grp,
	lck_attr_t              *attr);

/*!
 * @function lck_rw_init
 *
 * @abstract
 * Initializes a rw_lock_t.
 *
 * @discussion
 * Usage statistics for the lock are going to be added to the lock group provided.
 *
 * The lock attribute can be LCK_ATTR_NULL or an attribute can be allocated with
 * lck_attr_alloc_init. So far however none of the attribute settings are supported.
 *
 * @param lck           lock to initialize.
 * @param grp           lock group to associate with the lock.
 * @param attr          lock attribute to initialize the lock.
 */
extern void             lck_rw_init(
	lck_rw_t                *lck,
	lck_grp_t               *grp,
	lck_attr_t              *attr);

/*!
 * @function lck_rw_free
 *
 * @abstract
 * Frees a rw_lock previously allocated with lck_rw_alloc_init().
 *
 * @discussion
 * The lock must be not held by any thread.
 *
 * @param lck           rw_lock to free.
 */
extern void             lck_rw_free(
	lck_rw_t                *lck,
	lck_grp_t               *grp);

/*!
 * @function lck_rw_destroy
 *
 * @abstract
 * Destroys a rw_lock previously initialized with lck_rw_init().
 *
 * @discussion
 * The lock must be not held by any thread.
 *
 * @param lck           rw_lock to destroy.
 */
extern void             lck_rw_destroy(
	lck_rw_t                *lck,
	lck_grp_t               *grp);

/*!
 * @function lck_rw_lock
 *
 * @abstract
 * Locks a rw_lock with the specified type.
 *
 * @discussion
 * See lck_rw_lock_shared() or lck_rw_lock_exclusive() for more details.
 *
 * @param lck           rw_lock to lock.
 * @param lck_rw_type   LCK_RW_TYPE_SHARED or LCK_RW_TYPE_EXCLUSIVE
 */
extern void             lck_rw_lock(
	lck_rw_t                *lck,
	lck_rw_type_t           lck_rw_type);

/*!
 * @function lck_rw_try_lock
 *
 * @abstract
 * Tries to locks a rw_lock with the specified type.
 *
 * @discussion
 * This function will return and not wait/block in case the lock is already held.
 * See lck_rw_try_lock_shared() or lck_rw_try_lock_exclusive() for more details.
 *
 * @param lck           rw_lock to lock.
 * @param lck_rw_type   LCK_RW_TYPE_SHARED or LCK_RW_TYPE_EXCLUSIVE
 *
 * @returns TRUE if the lock is successfully acquired, FALSE in case it was already held.
 */
extern boolean_t        lck_rw_try_lock(
	lck_rw_t                *lck,
	lck_rw_type_t           lck_rw_type);

/*!
 * @function lck_rw_unlock
 *
 * @abstract
 * Unlocks a rw_lock previously locked with lck_rw_type.
 *
 * @discussion
 * The lock must be unlocked by the same thread it was locked from.
 * The type of the lock/unlock have to match, unless an upgrade/downgrade was performed while
 * holding the lock.
 *
 * @param lck           rw_lock to unlock.
 * @param lck_rw_type   LCK_RW_TYPE_SHARED or LCK_RW_TYPE_EXCLUSIVE
 */
extern void             lck_rw_unlock(
	lck_rw_t                *lck,
	lck_rw_type_t           lck_rw_type);

/*!
 * @function lck_rw_lock_shared
 *
 * @abstract
 * Locks a rw_lock in shared mode.
 *
 * @discussion
 * This function can block.
 * Multiple threads can acquire the lock in shared mode at the same time, but only one thread at a time
 * can acquire it in exclusive mode.
 * If the lock is held in shared mode and there are no writers waiting, a reader will be able to acquire
 * the lock without waiting.
 * If the lock is held in shared mode and there is at least a writer waiting, a reader will wait
 * for all the writers to make progress.
 * NOTE: the thread cannot return to userspace while the lock is held. Recursive locking is not supported.
 *
 * @param lck           rw_lock to lock.
 */
extern void             lck_rw_lock_shared(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_lock_shared_to_exclusive
 *
 * @abstract
 * Upgrades a rw_lock held in shared mode to exclusive.
 *
 * @discussion
 * This function can block.
 * Only one reader at a time can upgrade to exclusive mode. If the upgrades fails the function will
 * return with the lock not held.
 * The caller needs to hold the lock in shared mode to upgrade it.
 *
 * @param lck           rw_lock already held in shared mode to upgrade.
 *
 * @returns TRUE if the lock was upgraded, FALSE if it was not possible.
 *          If the function was not able to upgrade the lock, the lock will be dropped
 *          by the function.
 */
extern boolean_t        lck_rw_lock_shared_to_exclusive(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_unlock_shared
 *
 * @abstract
 * Unlocks a rw_lock previously locked in shared mode.
 *
 * @discussion
 * The same thread that locked the lock needs to unlock it.
 *
 * @param lck           rw_lock held in shared mode to unlock.
 */
extern void             lck_rw_unlock_shared(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_lock_exclusive
 *
 * @abstract
 * Locks a rw_lock in exclusive mode.
 *
 * @discussion
 * This function can block.
 * Multiple threads can acquire the lock in shared mode at the same time, but only one thread at a time
 * can acquire it in exclusive mode.
 * NOTE: the thread cannot return to userspace while the lock is held. Recursive locking is not supported.
 *
 * @param lck           rw_lock to lock.
 */
extern void             lck_rw_lock_exclusive(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_lock_exclusive_to_shared
 *
 * @abstract
 * Downgrades a rw_lock held in exclusive mode to shared.
 *
 * @discussion
 * The caller needs to hold the lock in exclusive mode to be able to downgrade it.
 *
 * @param lck           rw_lock already held in exclusive mode to downgrade.
 */
extern void             lck_rw_lock_exclusive_to_shared(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_unlock_exclusive
 *
 * @abstract
 * Unlocks a rw_lock previously locked in exclusive mode.
 *
 * @discussion
 * The same thread that locked the lock needs to unlock it.
 *
 * @param lck           rw_lock held in exclusive mode to unlock.
 */
extern void             lck_rw_unlock_exclusive(
	lck_rw_t                *lck);

/*!
 * @function lck_rw_sleep
 *
 * @abstract
 * Assert_wait on an event while holding the rw_lock.
 *
 * @discussion
 * the flags can decide how to re-acquire the lock upon wake up
 * (LCK_SLEEP_SHARED, or LCK_SLEEP_EXCLUSIVE, or LCK_SLEEP_UNLOCK)
 * and if the priority needs to be kept boosted until the lock is
 * re-acquired (LCK_SLEEP_PROMOTED_PRI).
 *
 * @param lck                   rw_lock to use to synch the assert_wait.
 * @param lck_sleep_action      flags.
 * @param event                 event to assert_wait on.
 * @param interruptible         wait type.
 */
extern wait_result_t    lck_rw_sleep(
	lck_rw_t                *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible);

/*!
 * @function lck_rw_sleep_deadline
 *
 * @abstract
 * Assert_wait_deadline on an event while holding the rw_lock.
 *
 * @discussion
 * the flags can decide how to re-acquire the lock upon wake up
 * (LCK_SLEEP_SHARED, or LCK_SLEEP_EXCLUSIVE, or LCK_SLEEP_UNLOCK)
 * and if the priority needs to be kept boosted until the lock is
 * re-acquired (LCK_SLEEP_PROMOTED_PRI).
 *
 * @param lck                   rw_lock to use to synch the assert_wait.
 * @param lck_sleep_action      flags.
 * @param event                 event to assert_wait on.
 * @param interruptible         wait type.
 * @param deadline              maximum time after which being woken up
 */
extern wait_result_t    lck_rw_sleep_deadline(
	lck_rw_t                *lck,
	lck_sleep_action_t      lck_sleep_action,
	event_t                 event,
	wait_interrupt_t        interruptible,
	uint64_t                deadline);




__END_DECLS

#endif /* _KERN_RW_LOCK_H_ */
