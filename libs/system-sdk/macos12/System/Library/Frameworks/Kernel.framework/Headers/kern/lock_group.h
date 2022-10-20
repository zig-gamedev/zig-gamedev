/*
 * Copyright (c) 2018 Apple Computer, Inc. All rights reserved.
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
#ifndef _KERN_LOCK_GROUP_H
#define _KERN_LOCK_GROUP_H

#include <kern/queue.h>
#include <kern/lock_types.h>

__BEGIN_DECLS

#define LCK_GRP_NULL    (lck_grp_t *)NULL

typedef enum lck_type {
	LCK_TYPE_SPIN,
	LCK_TYPE_MTX,
	LCK_TYPE_RW,
	LCK_TYPE_TICKET
} lck_type_t;

typedef struct _lck_grp_ lck_grp_t;

#define LCK_GRP_ATTR_STAT       0x1
#define LCK_GRP_ATTR_TIME_STAT  0x2

typedef struct __lck_grp_attr__ lck_grp_attr_t;

#define LCK_GRP_ATTR_NULL       (lck_grp_attr_t *)NULL

extern  lck_grp_attr_t  *lck_grp_attr_alloc_init(
	void);

extern  void            lck_grp_attr_setdefault(
	lck_grp_attr_t          *attr);

extern  void            lck_grp_attr_setstat(
	lck_grp_attr_t          *attr);

extern  void            lck_grp_attr_free(
	lck_grp_attr_t          *attr);

extern  lck_grp_t       *lck_grp_alloc_init(
	const char*             grp_name,
	lck_grp_attr_t  *attr);

extern void             lck_grp_free(
	lck_grp_t               *grp);


__END_DECLS

#endif /* _KERN_LOCK_GROUP_H */
