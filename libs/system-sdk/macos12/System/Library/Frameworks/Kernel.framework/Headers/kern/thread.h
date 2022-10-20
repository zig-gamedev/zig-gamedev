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
 *	File:	thread.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for threads.
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

#ifndef _KERN_THREAD_H_
#define _KERN_THREAD_H_

#include <mach/kern_return.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/boolean.h>
#include <mach/vm_param.h>
#include <mach/thread_info.h>
#include <mach/thread_status.h>
#include <mach/exception_types.h>

#include <kern/kern_types.h>
#include <vm/vm_kern.h>
#include <sys/cdefs.h>



__BEGIN_DECLS

extern void thread_mtx_lock(thread_t thread);

extern void thread_mtx_unlock(thread_t thread);

extern thread_t         current_thread(void) __attribute__((const));

extern void                     thread_reference(
	thread_t        thread);

extern void                     thread_deallocate(
	thread_t        thread);


__END_DECLS



__BEGIN_DECLS

extern uint64_t                 thread_tid(thread_t thread);

__END_DECLS


__BEGIN_DECLS



/*! @function thread_has_thread_name
 *   @abstract Checks if a thread has a name.
 *   @discussion This function takes one input, a thread, and returns a boolean value indicating if that thread already has a name associated with it.
 *   @param th The thread to inspect.
 *   @result TRUE if the thread has a name, FALSE otherwise.
 */
extern boolean_t thread_has_thread_name(thread_t th);

/*! @function thread_set_thread_name
 *   @abstract Set a thread's name.
 *   @discussion This function takes two input parameters: a thread to name, and the name to apply to the thread.  The name will be copied over to the thread in order to better identify the thread.  If the name is longer than MAXTHREADNAMESIZE - 1, it will be truncated.
 *   @param th The thread to be named.
 *   @param name The name to apply to the thread.
 */
extern void thread_set_thread_name(thread_t th, const char* name);


/*! @function kernel_thread_start
 *   @abstract Create a kernel thread.
 *   @discussion This function takes three input parameters, namely reference to the function that the thread should execute, caller specified data and a reference which is used to return the newly created kernel thread. The function returns KERN_SUCCESS on success or an appropriate kernel code type indicating the error. It may be noted that the caller is responsible for explicitly releasing the reference to the created thread when no longer needed. This should be done by calling thread_deallocate(new_thread).
 *   @param continuation A C-function pointer where the thread will begin execution.
 *   @param parameter Caller specified data to be passed to the new thread.
 *   @param new_thread Reference to the new thread is returned in this parameter.
 *   @result Returns KERN_SUCCESS on success or an appropriate kernel code type.
 */

extern kern_return_t    kernel_thread_start(
	thread_continue_t       continuation,
	void                            *parameter,
	thread_t                        *new_thread);


__END_DECLS

#endif  /* _KERN_THREAD_H_ */
