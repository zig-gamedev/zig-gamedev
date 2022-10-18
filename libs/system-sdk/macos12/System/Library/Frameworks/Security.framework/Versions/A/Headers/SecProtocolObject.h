/*
 * Copyright (c) 2018 Apple Inc. All Rights Reserved.
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

#ifndef SecProtocolObject_h
#define SecProtocolObject_h

#include <sys/cdefs.h>
#include <os/object.h>

#if OS_OBJECT_USE_OBJC
#  define SEC_OBJECT_DECL(type) OS_OBJECT_DECL(type)
#else // OS_OBJECT_USE_OBJC
#  define SEC_OBJECT_DECL(type)                \
struct type;                        \
typedef    struct type *type##_t
#endif // OS_OBJECT_USE_OBJC

#if __has_feature(assume_nonnull)
#  define SEC_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#  define SEC_ASSUME_NONNULL_END   _Pragma("clang assume_nonnull end")
#else // !__has_feature(assume_nonnull)
#  define SEC_ASSUME_NONNULL_BEGIN
#  define SEC_ASSUME_NONNULL_END
#endif // !__has_feature(assume_nonnull)

#if defined(__OBJC__) && __has_attribute(ns_returns_retained)
#  define SEC_RETURNS_RETAINED __attribute__((__ns_returns_retained__))
#else // __OBJC__ && ns_returns_retained
#  define SEC_RETURNS_RETAINED
#endif // __OBJC__ && ns_returns_retained

#if !OS_OBJECT_USE_OBJC_RETAIN_RELEASE
__BEGIN_DECLS
__attribute__((visibility("default"))) void *sec_retain(void *obj);
__attribute__((visibility("default"))) void sec_release(void *obj);
__END_DECLS
#else // !OS_OBJECT_USE_OBJC_RETAIN_RELEASE
#undef sec_retain
#undef sec_release
#define sec_retain(object) [(object) retain]
#define sec_release(object) [(object) release]
#endif // !OS_OBJECT_USE_OBJC_RETAIN_RELEASE

#ifndef SEC_OBJECT_IMPL
/*!
 * A `sec_object` is a generic, ARC-able type wrapper for common CoreFoundation Security types.
 */
SEC_OBJECT_DECL(sec_object);
#endif // !SEC_OBJECT_IMPL

#endif // SecProtocolObject_h
