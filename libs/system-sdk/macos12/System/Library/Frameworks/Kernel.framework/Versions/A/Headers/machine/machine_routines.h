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
#ifndef _MACHINE_MACHINE_ROUTINES_H
#define _MACHINE_MACHINE_ROUTINES_H

#include <sys/cdefs.h>

#if defined (__i386__) || defined(__x86_64__)
#include "i386/machine_routines.h"
#elif defined (__arm__) || defined (__arm64__)
#include "arm/machine_routines.h"
#else
#error architecture not supported
#endif

__BEGIN_DECLS


/*!
 * @enum     cpu_event
 * @abstract Broadcast events allowing clients to hook CPU state transitions.
 * @constant CPU_BOOT_REQUESTED      Called from processor_start(); may block.
 * @constant CPU_BOOTED              Called from platform code on the newly-booted CPU; may not block.
 * @constant CPU_ACTIVE              Called from scheduler code; may block.
 * @constant CLUSTER_ACTIVE          Called from platform code; may not block.
 * @constant CPU_EXIT_REQUESTED      Called from processor_exit(); may block.
 * @constant CPU_DOWN                Called from platform code on the disabled CPU; may not block.
 * @constant CLUSTER_EXIT_REQUESTED  Called from platform code; may not block.
 * @constant CPU_EXITED              Called after CPU is stopped; may block.
 */
enum cpu_event {
	CPU_BOOT_REQUESTED = 0,
	CPU_BOOTED,
	CPU_ACTIVE,
	CLUSTER_ACTIVE,
	CPU_EXIT_REQUESTED,
	CPU_DOWN,
	CLUSTER_EXIT_REQUESTED,
	CPU_EXITED,
};

typedef bool (*cpu_callback_t)(void *param, enum cpu_event event, unsigned int cpu_or_cluster);

/*!
 * @function              cpu_event_register_callback
 * @abstract              Register a function to be called on CPU state changes.
 * @param fn              Function to call on state change events.
 * @param param           Optional argument to be passed to the callback (e.g. object pointer).
 */
void cpu_event_register_callback(cpu_callback_t fn, void *param);

/*!
 * @function              cpu_event_unregister_callback
 * @abstract              Unregister a previously-registered callback function.
 * @param fn              Function pointer previously passed to cpu_event_register_callback().
 */
void cpu_event_unregister_callback(cpu_callback_t fn);


/*!
 * @function      ml_io_read()
 * @brief         Perform an MMIO read access
 *
 * @return        The value resulting from the read.
 *
 */
unsigned long long ml_io_read(uintptr_t iovaddr, int iovsz);
unsigned int ml_io_read8(uintptr_t iovaddr);
unsigned int ml_io_read16(uintptr_t iovaddr);
unsigned int ml_io_read32(uintptr_t iovaddr);
unsigned long long ml_io_read64(uintptr_t iovaddr);

/*!
 * @function      ml_io_write()
 * @brief         Perform an MMIO write access
 *
 */
void ml_io_write(uintptr_t vaddr, uint64_t val, int size);
void ml_io_write8(uintptr_t vaddr, uint8_t val);
void ml_io_write16(uintptr_t vaddr, uint16_t val);
void ml_io_write32(uintptr_t vaddr, uint32_t val);
void ml_io_write64(uintptr_t vaddr, uint64_t val);


__END_DECLS

#endif /* _MACHINE_MACHINE_ROUTINES_H */
