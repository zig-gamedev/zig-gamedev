/* CoreAnimation - CABase.h

   Copyright (c) 2006-2021, Apple Inc.
   All rights reserved. */

#ifndef CABASE_H
#define CABASE_H

/* Adapted from <CoreGraphics/CGBase.h> */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>

#include <CoreFoundation/CoreFoundation.h>

#if TARGET_OS_MAC
#include <CoreGraphics/CoreGraphics.h>
#endif

#include <os/availability.h>
#include <TargetConditionals.h>

#if TARGET_OS_OSX
# define CA_OSX_VERSION(v) ((__MAC_##v) > 0 && __MAC_OS_X_VERSION_MAX_ALLOWED >= (__MAC_##v))
#else
# define CA_OSX_VERSION(v) (0)
#endif

#if TARGET_OS_IOS
# define CA_IOS_VERSION(v) ((__IPHONE_##v) > 0 && __IPHONE_OS_VERSION_MIN_REQUIRED >= (__IPHONE_##v))
#else
# define CA_IOS_VERSION(v) (0)
#endif

#if TARGET_OS_TV
# define CA_TV_VERSION(v) ((__TVOS_##v) > 0 && __TV_OS_VERSION_MIN_REQUIRED >= (__TVOS_##v))
#else
# define CA_TV_VERSION(v) (0)
#endif

#if TARGET_OS_WATCH
# define CA_WATCH_VERSION(v) ((__WATCHOS_##v) > 0 && __WATCH_OS_VERSION_MIN_REQUIRED >= (__WATCHOS_##v))
#else
# define CA_WATCH_VERSION(v) (0)
#endif

#ifdef __cplusplus
# define CA_EXTERN_C_BEGIN extern "C" {
# define CA_EXTERN_C_END   }
#else
# define CA_EXTERN_C_BEGIN
# define CA_EXTERN_C_END
#endif

#ifdef __GNUC__
# define CA_GNUC(major, minor) \
  (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
#else
# define CA_GNUC(major, minor) 0
#endif

#ifndef CA_EXTERN
# define CA_EXTERN extern __attribute__((visibility("default")))
#endif

#ifndef CA_INLINE
# if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define CA_INLINE static inline
# elif defined (__cplusplus)
#  define CA_INLINE static inline
# elif CA_GNUC (3, 0)
#  define CA_INLINE static __inline__ __attribute__ ((always_inline))
# else
#  define CA_INLINE static
# endif
#endif

#ifndef CA_HIDDEN
# if CA_GNUC (4,0)
#  define CA_HIDDEN __attribute__ ((visibility ("hidden")))
# else
#  define CA_HIDDEN /* no hidden */
# endif
#endif

#ifndef CA_PURE
# if CA_GNUC (3, 0)
#  define CA_PURE __attribute__ ((pure))
# else
#  define CA_PURE /* no pure */
# endif
#endif

#ifndef CA_CONST
# if CA_GNUC (3, 0)
#  define CA_CONST __attribute__ ((const))
# else
#  define CA_CONST /* no const */
# endif
#endif

#ifndef CA_NORETURN
# if CA_GNUC (3, 0)
#  define CA_NORETURN __attribute__ ((noreturn))
# else
#  define CA_NORETURN /* no noreturn */
# endif
#endif

#ifndef CA_MALLOC
# if CA_GNUC (3, 0)
#  define CA_MALLOC __attribute__ ((malloc))
# else
#  define CA_MALLOC /* no malloc */
# endif
#endif

#ifndef CA_WARN_UNUSED
# if CA_GNUC (3, 4)
#  define CA_WARN_UNUSED __attribute__ ((warn_unused_result))
# else
#  define CA_WARN_UNUSED /* no warn_unused */
# endif
#endif

#ifndef CA_WARN_DEPRECATED
# define CA_WARN_DEPRECATED 1
#endif

#ifndef CA_DEPRECATED
# if CA_GNUC (3, 0) && CA_WARN_DEPRECATED
#  define CA_DEPRECATED __attribute__ ((deprecated))
# else
#  define CA_DEPRECATED
# endif
#endif

#if defined(__has_attribute) && __has_attribute(objc_boxable)
# define CA_BOXABLE __attribute__((objc_boxable))
#else
# define CA_BOXABLE
#endif

CA_EXTERN_C_BEGIN

/* Returns the current CoreAnimation absolute time. This is the result of
 * calling mach_absolute_time () and converting the units to seconds. */

CA_EXTERN CFTimeInterval CACurrentMediaTime (void)
    API_AVAILABLE (macos(10.5), ios(2.0));

CA_EXTERN_C_END

#endif /* CABASE_H */
