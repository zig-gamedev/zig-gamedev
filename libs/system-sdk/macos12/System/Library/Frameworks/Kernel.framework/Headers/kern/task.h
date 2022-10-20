/*
 * Copyright (c) 2000-2019 Apple Inc. All rights reserved.
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
 * @OSF_FREE_COPYRIGHT@
 */
/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988 Carnegie Mellon University
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
 *	File:	task.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for tasks.
 *
 */
/*
 * Copyright (c) 1993 The University of Utah and
 * the Computer Systems Laboratory (CSL).  All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 *
 */
/*
 * NOTICE: This file was modified by McAfee Research in 2004 to introduce
 * support for mandatory and extensible security protections.  This notice
 * is included in support of clause 2.2 (b) of the Apple Public License,
 * Version 2.0.
 * Copyright (c) 2005 SPARTA, Inc.
 */

#ifndef _KERN_TASK_H_
#define _KERN_TASK_H_

#include <kern/btlog.h>
#include <kern/kern_types.h>
#include <kern/task_ref.h>
#include <mach/mach_types.h>
#include <sys/cdefs.h>



__BEGIN_DECLS

extern task_t   current_task(void);

extern bool task_is_driver(task_t task);

#define TF_NONE                 0

#define TWF_NONE                 0
#define TRW_LRETURNWAIT          0x01           /* task is waiting for fork/posix_spawn/exec to complete */
#define TRW_LRETURNWAITER        0x02           /* task is waiting for TRW_LRETURNWAIT to get cleared */

/* task clear return wait flags */
#define TCRW_CLEAR_INITIAL_WAIT   0x1
#define TCRW_CLEAR_FINAL_WAIT     0x2
#define TCRW_CLEAR_ALL_WAIT       (TCRW_CLEAR_INITIAL_WAIT | TCRW_CLEAR_FINAL_WAIT)


#define TPF_NONE                0
#define TPF_EXEC_COPY           0x00000002                              /* task is the new copy of an exec */


__END_DECLS


__BEGIN_DECLS



extern task_t   kernel_task;

extern void             task_name_deallocate_mig(
	task_name_t             task_name);

extern void             task_policy_set_deallocate_mig(
	task_policy_set_t       task_policy_set);

extern void             task_policy_get_deallocate_mig(
	task_policy_get_t       task_policy_get);

extern void             task_inspect_deallocate_mig(
	task_inspect_t          task_inspect);

extern void             task_read_deallocate_mig(
	task_read_t          task_read);

extern void             task_suspension_token_deallocate(
	task_suspension_token_t token);

extern boolean_t task_self_region_footprint(void);
extern void task_self_region_footprint_set(boolean_t newval);
extern void task_ledgers_footprint(ledger_t ledger,
    ledger_amount_t *ledger_resident,
    ledger_amount_t *ledger_compressed);
extern void task_set_memory_ownership_transfer(
	task_t task,
	boolean_t value);

#if DEVELOPMENT || DEBUG
extern void task_set_no_footprint_for_debug(
	task_t task,
	boolean_t value);
extern int task_get_no_footprint_for_debug(
	task_t task);
#endif /* DEVELOPMENT || DEBUG */

#if CONFIG_X86_64_COMPAT
extern boolean_t task_is_translated(task_t task);
#endif

__END_DECLS

#endif  /* _KERN_TASK_H_ */
