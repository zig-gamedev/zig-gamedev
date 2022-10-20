/*
 * Copyright (c) 2021 Apple Computer, Inc. All rights reserved.
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
#ifndef _KERN_LOCK_TYPES_H
#define _KERN_LOCK_TYPES_H

#include <kern/kern_types.h>

__BEGIN_DECLS


/*!
 * @enum lck_sleep_action_t
 *
 * @abstract
 * An action to pass to the @c lck_*_sleep* family of functions.
 */
__options_decl(lck_sleep_action_t, unsigned int, {
	LCK_SLEEP_DEFAULT      = 0x00,    /**< Release the lock while waiting for the event, then reclaim */
	LCK_SLEEP_UNLOCK       = 0x01,    /**< Release the lock and return unheld */
	LCK_SLEEP_SHARED       = 0x02,    /**< Reclaim the lock in shared mode (RW only) */
	LCK_SLEEP_EXCLUSIVE    = 0x04,    /**< Reclaim the lock in exclusive mode (RW only) */
	LCK_SLEEP_SPIN         = 0x08,    /**< Reclaim the lock in spin mode (mutex only) */
	LCK_SLEEP_PROMOTED_PRI = 0x10,    /**< Sleep at a promoted priority */
	LCK_SLEEP_SPIN_ALWAYS  = 0x20,    /**< Reclaim the lock in spin-always mode (mutex only) */
});

#define LCK_SLEEP_MASK           0x3f     /* Valid actions */

__options_decl(lck_wake_action_t, unsigned int, {
	LCK_WAKE_DEFAULT                = 0x00,  /* If waiters are present, transfer their push to the wokenup thread */
	LCK_WAKE_DO_NOT_TRANSFER_PUSH   = 0x01,  /* Do not transfer waiters push when waking up */
});


__END_DECLS

#endif /* _KERN_LOCK_TYPES_H */
