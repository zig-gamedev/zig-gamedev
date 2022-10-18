/*
 * Copyright (c) 2020 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _STRING_X86_H
#define _STRING_X86_H

#include <Availability.h>

#if defined(__x86_64__)

__BEGIN_DECLS
/* These SSE variants have the same behavior as their original functions.
 * SSE instructions are used in these variants instead of best possible
 * implementation.
 */
__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void	*memmove_sse_np(void *__dst, const void *__src, size_t __len);

__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void	*memset_sse_np(void *__b, int __c, size_t __len);

__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void	 bzero_sse_np(void *, size_t);

__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void     memset_pattern4_sse_np(void *__b, const void *__pattern4, size_t __len);

__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void     memset_pattern8_sse_np(void *__b, const void *__pattern8, size_t __len);

__OSX_AVAILABLE(11.0) __IOS_UNAVAILABLE __TVOS_UNAVAILABLE __WATCHOS_UNAVAILABLE
void     memset_pattern16_sse_np(void *__b, const void *__pattern16, size_t __len);
__END_DECLS

#endif /* __x86_64__ */

#endif /* _STRING_X86_H */
