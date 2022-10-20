/*
 * Copyright (c) 2019 Apple Inc. All rights reserved.
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

/* hv_support_kern.h: machine-independent hypervisor helpers for kernel use */

#pragma once

#include <kern/kern_types.h>

extern int hv_support_available;

/*!
 * @function hv_support_init
 * @abstract Initialize hypervisor facility during kernel bootstrap
 */
extern void hv_support_init(void);

/*!
 * @function hv_get_support
 * @abstract Indicates whether the hypervisor facility is supported on this platform
 * @return 0 if not supported, non-zero otherwise
 */
extern int hv_get_support(void);

/*!
 * @function hv_set_task_target
 * @abstract Associate an opaque hypervisor object with the current OS task
 * @param target The opaque object to be associated
 */
extern void hv_set_task_target(void *target);

/*!
 * @function hv_set_thread_target
 * @abstract Associate an opaque hypervisor object with the current OS thread
 * @param target The opaque object to be associated
 */
extern void hv_set_thread_target(void *target);

/*!
 * @function hv_get_task_target
 * @abstract Retrieve an opaque hypervisor object associated with the current OS task
 * @return The associated object, or NULL if no object is associated
 */
extern void * hv_get_task_target(void);

/*!
 * @function hv_get_thread_target
 * @abstract Retrieve an opaque hypervisor object associated with the current OS thread
 * @return The associated object, or NULL if no object is associated
 */
extern void * hv_get_thread_target(void);
