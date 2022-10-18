/*
 * Copyright (c) 2007 Apple Inc. All rights reserved.
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
 * FILE_ID: vm_param.h
 */

/*
 *	ARM machine dependent virtual memory parameters.
 */

#ifndef _MACH_ARM_VM_PARAM_H_
#define _MACH_ARM_VM_PARAM_H_

#if defined (__arm__) || defined (__arm64__)



#define BYTE_SIZE       8       /* byte size in bits */


#ifndef __ASSEMBLER__

#ifdef  __arm__
#define PAGE_SHIFT_CONST        12
#elif defined(__arm64__)
extern int PAGE_SHIFT_CONST;
#else
#error Unsupported arch
#endif

#define PAGE_SHIFT              PAGE_SHIFT_CONST
#define PAGE_SIZE               (1 << PAGE_SHIFT)
#define PAGE_MASK               (PAGE_SIZE-1)

#define VM_PAGE_SIZE            PAGE_SIZE

#define machine_ptob(x)         ((x) << PAGE_SHIFT)

/*
 * Defined for the purpose of testing the pmap advertised page
 * size; this does not necessarily match the hardware page size.
 */
#define TEST_PAGE_SIZE_16K      ((PAGE_SHIFT_CONST == 14))
#define TEST_PAGE_SIZE_4K       ((PAGE_SHIFT_CONST == 12))

#endif  /* !__ASSEMBLER__ */


#define PAGE_MAX_SHIFT          14
#define PAGE_MAX_SIZE           (1 << PAGE_MAX_SHIFT)
#define PAGE_MAX_MASK           (PAGE_MAX_SIZE-1)

#define PAGE_MIN_SHIFT          12
#define PAGE_MIN_SIZE           (1 << PAGE_MIN_SHIFT)
#define PAGE_MIN_MASK           (PAGE_MIN_SIZE-1)

#define VM_MAX_PAGE_ADDRESS     MACH_VM_MAX_ADDRESS

#ifndef __ASSEMBLER__


#if defined (__arm__)

#define VM_MIN_ADDRESS          ((vm_address_t) 0x00000000)
#define VM_MAX_ADDRESS          ((vm_address_t) 0x80000000)

/* system-wide values */
#define MACH_VM_MIN_ADDRESS     ((mach_vm_offset_t) 0)
#define MACH_VM_MAX_ADDRESS     ((mach_vm_offset_t) VM_MAX_ADDRESS)

#elif defined (__arm64__)

#define VM_MIN_ADDRESS          ((vm_address_t) 0x0000000000000000ULL)
#define VM_MAX_ADDRESS          ((vm_address_t) 0x0000000080000000ULL)

/* system-wide values */
#define MACH_VM_MIN_ADDRESS_RAW 0x0ULL
#define MACH_VM_MAX_ADDRESS_RAW 0x00007FFFFE000000ULL

#define MACH_VM_MIN_ADDRESS     ((mach_vm_offset_t) MACH_VM_MIN_ADDRESS_RAW)
#define MACH_VM_MAX_ADDRESS     ((mach_vm_offset_t) MACH_VM_MAX_ADDRESS_RAW)

#define MACH_VM_MIN_GPU_CARVEOUT_ADDRESS_RAW 0x0000001000000000ULL
#define MACH_VM_MAX_GPU_CARVEOUT_ADDRESS_RAW 0x0000007000000000ULL
#define MACH_VM_MIN_GPU_CARVEOUT_ADDRESS     ((mach_vm_offset_t) MACH_VM_MIN_GPU_CARVEOUT_ADDRESS_RAW)
#define MACH_VM_MAX_GPU_CARVEOUT_ADDRESS     ((mach_vm_offset_t) MACH_VM_MAX_GPU_CARVEOUT_ADDRESS_RAW)

#else /* defined(__arm64__) */
#error architecture not supported
#endif

#define VM_MAP_MIN_ADDRESS      VM_MIN_ADDRESS
#define VM_MAP_MAX_ADDRESS      VM_MAX_ADDRESS


#if defined (__arm__)
#define VM_KERNEL_POINTER_SIGNIFICANT_BITS  31
#define VM_MIN_KERNEL_ADDRESS   ((vm_address_t) 0x80000000)
#define VM_MAX_KERNEL_ADDRESS   ((vm_address_t) 0xFFFEFFFF)
#define VM_HIGH_KERNEL_WINDOW   ((vm_address_t) 0xFFFE0000)
#elif defined (__arm64__)
/*
 * The minimum and maximum kernel address; some configurations may
 * constrain the address space further.
 */
#define TiB(x) ((0ULL + (x)) << 40)
#define GiB(x) ((0ULL + (x)) << 30)

// Inform kexts about largest possible kernel address space
#define VM_MIN_KERNEL_ADDRESS   ((vm_address_t) (0ULL - TiB(2)))
#define VM_MAX_KERNEL_ADDRESS   ((vm_address_t) 0xfffffffbffffffffULL)
#else
#error architecture not supported
#endif

#define VM_MIN_KERNEL_AND_KEXT_ADDRESS  VM_MIN_KERNEL_ADDRESS

#if defined (__arm64__)
/* Top-Byte-Ignore */
#define TBI_MASK           0xff00000000000000ULL
#define tbi_clear(addr)    ((typeof (addr))(((uintptr_t)(addr)) &~ (TBI_MASK)))
#define tbi_fill(addr)     ((typeof (addr))(((uintptr_t)(addr)) | (TBI_MASK)))
#endif /* __arm64__ */

#if CONFIG_KERNEL_TBI
/*
 * 'strip' in PAC sense, therefore replacing the stripped bits sign extending
 * the sign bit.
 */
#define VM_KERNEL_TBI_FILL(_v)  (tbi_fill(_v))
#define VM_KERNEL_TBI_CLEAR(_v) (tbi_clear(_v))
#define VM_KERNEL_STRIP_TBI(_v) (VM_KERNEL_TBI_FILL(_v))
#else /* CONFIG_KERNEL_TBI */
#define VM_KERNEL_TBI_FILL(_v)  (_v)
#define VM_KERNEL_TBI_CLEAR(_v) (_v)
#define VM_KERNEL_STRIP_TBI(_v) (_v)
#endif /* CONFIG_KERNE_TBI */

#if __has_feature(ptrauth_calls)
#include <ptrauth.h>
#define VM_KERNEL_STRIP_PAC(_v) (ptrauth_strip((void *)(uintptr_t)(_v), ptrauth_key_asia))
#else /* !ptrauth_calls */
#define VM_KERNEL_STRIP_PAC(_v) (_v)
#endif /* ptrauth_calls */

#define VM_KERNEL_STRIP_PTR(_va)        ((VM_KERNEL_STRIP_TBI(VM_KERNEL_STRIP_PAC(_va))))
#define VM_KERNEL_STRIP_UPTR(_va)       ((vm_address_t)VM_KERNEL_STRIP_PTR((uintptr_t)(_va)))
#define VM_KERNEL_ADDRESS(_va)  \
	((VM_KERNEL_STRIP_UPTR(_va) >= VM_MIN_KERNEL_ADDRESS) && \
	 (VM_KERNEL_STRIP_UPTR(_va) <= VM_MAX_KERNEL_ADDRESS))



#endif  /* !__ASSEMBLER__ */

#define SWI_SYSCALL     0x80

#endif /* defined (__arm__) || defined (__arm64__) */

#endif  /* _MACH_ARM_VM_PARAM_H_ */
