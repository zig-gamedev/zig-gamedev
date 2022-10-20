/*
 * Copyright (c) 1993-1995, 1999-2008 Apple Inc. All rights reserved.
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
 * The timer_call system is responsible for manipulating timers that call
 * callbacks at a given deadline (with or without some leeway for coalescing).
 *
 * Call timer_call_setup once on a timer_call structure to register the callback
 * function and a context parameter that's passed to it (param0).
 *
 * To arm the timer to fire at a deadline, call any of the timer_call_enter
 * functions.  If the function used accepts a parameter, it will be passed to
 * the callback function when it fires.
 *
 * If the timer needs to be cancelled (like if the timer_call has been armed but
 * now needs to be deallocated), call timer_call_cancel.
 */

#ifndef _KERN_TIMER_CALL_H_
#define _KERN_TIMER_CALL_H_

#include <mach/mach_types.h>
#include <kern/kern_types.h>


#endif /* _KERN_TIMER_CALL_H_ */
