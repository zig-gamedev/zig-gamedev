/* CoreGraphics - CGBase.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGBASE_H_
#define CGBASE_H_

#include <stdbool.h>
#include <stddef.h>
#include <float.h>
#include <TargetConditionals.h>
#include <CoreFoundation/CFBase.h>
#include <os/availability.h>

/* Definition of `__CG_HAS_COMPILER_ATTRIBUTE'. */

#if !defined(__CG_HAS_COMPILER_ATTRIBUTE)
# if defined(__has_attribute)
#  define __CG_HAS_COMPILER_ATTRIBUTE(attribute) __has_attribute(attribute)
# elif defined(__GNUC__) && __GNUC__ >= 4
#  define __CG_HAS_COMPILER_ATTRIBUTE(attribute) (1)
# else
#  define __CG_HAS_COMPILER_ATTRIBUTE(attribute) (0)
# endif
#endif

/* Definition of `__CG_HAS_COMPILER_EXTENSION'. */

#if !defined(__CG_HAS_COMPILER_EXTENSION)
# if defined(__has_extension)
#  define __CG_HAS_COMPILER_EXTENSION(extension) __has_extension(extension)
# elif defined(__has_feature)
#  define __CG_HAS_COMPILER_EXTENSION(extension) __has_feature(extension)
# else
#  define __CG_HAS_COMPILER_EXTENSION(extension) (0)
# endif
#endif

/* Define `__WIN32__' where appropriate if it's not already defined. */

#if !defined(__WIN32__)
# if defined(_WIN32) || defined(__CYGWIN32__)
#  define __WIN32__ 1
# endif
#endif

/* Definition of `CG_EXTERN'. */

#if !defined(CG_EXTERN)
# if defined(__WIN32__)
#  if defined(CG_BUILDING_CG)
#   if defined(__cplusplus)
#    define CG_EXTERN extern "C" __declspec(dllexport)
#   else
#    define CG_EXTERN extern __declspec(dllexport)
#   endif
#  else /* !defined(CG_BUILDING_CG) */
#   if defined(__cplusplus)
#    define CG_EXTERN extern "C" __declspec(dllimport) 
#   else
#    define CG_EXTERN extern __declspec(dllimport)
#   endif
#  endif /* !defined(CG_BUILDING_CG) */
# else /* !defined(__WIN32__) */
#  if defined(__cplusplus)
#   define CG_EXTERN extern "C" __attribute__((visibility("default")))
#  else
#   define CG_EXTERN extern __attribute__((visibility("default")))
#  endif
# endif /* !defined(__WIN32__) */
#endif /* !defined(CG_EXTERN) */

/* Definition of `CG_LOCAL'. */

#if !defined(CG_LOCAL)
# if __CG_HAS_COMPILER_ATTRIBUTE(visibility)
#  if defined(__cplusplus)
#   define CG_LOCAL extern "C" __attribute__((visibility("hidden")))
#  else
#   define CG_LOCAL extern __attribute__((visibility("hidden")))
#  endif
# else
#  define CG_LOCAL CG_EXTERN
# endif
#endif /* !defined(CG_LOCAL) */

/* Definition of `CG_EXTERN_64` */

#if !defined(CG_EXTERN_64)
# if defined(__LP64__)
#  define CG_EXTERN_64 CG_EXTERN
# else /* !defined(__LP64__) */
#  define CG_EXTERN_64 CG_LOCAL
# endif /* defined(__LP64__) */
#endif /* !defined(CG_EXTERN_64) */

/* Definition of `CG_SKYLIGHT_EXTERN_64` */

#if !defined(CG_SKYLIGHT_EXTERN_64)
# if defined(__LP64__)
#  define CG_SKYLIGHT_EXTERN_64 CG_EXTERN
# else /* !defined(__LP64__) */
#  if defined(CG_BUILDING_CG)
#    define CG_SKYLIGHT_EXTERN_64 CG_EXTERN
#  else /* !defined(CG_BUILDING_CG) */
#    define CG_SKYLIGHT_EXTERN_64 CG_LOCAL
#  endif /* !defined(CG_BUILDING_CG) */
# endif /* defined(__LP64__) */
#endif /* !defined(CG_SKYLIGHT_EXTERN_64) */

/* Definition of `CG_EXTERN_32` */

#if !defined(CG_EXTERN_32)
# if defined(__LP64__)
#  define CG_EXTERN_32 CG_LOCAL __attribute__((unused))
# else /* !defined(__LP64__) */
#  define CG_EXTERN_32 CG_EXTERN
# endif /* defined(__LP64__) */
#endif /* !defined(CG_EXTERN_32) */

/* Definition of `CG_LOCAL_64` */

#if !defined(CG_LOCAL_64)
# if defined(__LP64__)
#  define CG_LOCAL_64 CG_LOCAL
# else /* !defined(__LP64__) */
#  define CG_LOCAL_64 CG_LOCAL __attribute__((unused))
# endif /* defined(__LP64__) */
#endif /* !defined(CG_LOCAL_64) */

/* Definition of `CG_LOCAL_32` */

#if !defined(CG_LOCAL_32)
# if defined(__LP64__)
#  define CG_LOCAL_32 CG_LOCAL __attribute__((unused))
# else /* !defined(__LP64__) */
#  define CG_LOCAL_32 CG_LOCAL
# endif /* defined(__LP64__) */
#endif /* !defined(CG_LOCAL_32) */

/* Definition of `__CG_DEPRECATED'. */

#if !defined(__CG_DEPRECATED)
# if __CG_HAS_COMPILER_ATTRIBUTE(deprecated) && !defined(CG_BUILDING_CG)
#  define __CG_DEPRECATED __attribute__((deprecated))
# else
#  define __CG_DEPRECATED
# endif
#endif

/* Definition of `__CG_DEPRECATED_WITH_MSG'. */

#if !defined(__CG_DEPRECATED_WITH_MSG)
# if __CG_HAS_COMPILER_ATTRIBUTE(deprecated)                            \
    && __CG_HAS_COMPILER_EXTENSION(attribute_deprecated_with_message)   \
    && !defined(CG_BUILDING_CG)
#  define __CG_DEPRECATED_WITH_MSG(msg) __attribute__((deprecated(msg)))
# else
#  define __CG_DEPRECATED_WITH_MSG(msg) __CG_DEPRECATED
# endif
#endif

/* Definition of `__CG_DEPRECATED_ENUMERATOR'. */

#if !defined(__CG_DEPRECATED_ENUMERATOR)
# if __CG_HAS_COMPILER_ATTRIBUTE(deprecated)                        \
   && __CG_HAS_COMPILER_EXTENSION(enumerator_attributes)            \
   && !defined(CG_BUILDING_CG)
#  define __CG_DEPRECATED_ENUMERATOR __attribute__((deprecated))
# else
#  define __CG_DEPRECATED_ENUMERATOR
# endif
#endif

/* Definition of `__CG_DEPRECATED_ENUMERATOR_WITH_MSG'. */

#if !defined(__CG_DEPRECATED_ENUMERATOR_WITH_MSG)
# if __CG_HAS_COMPILER_ATTRIBUTE(deprecated)                      \
&& __CG_HAS_COMPILER_EXTENSION(attribute_deprecated_with_message) \
&& __CG_HAS_COMPILER_EXTENSION(enumerator_attributes)             \
&& !defined(CG_BUILDING_CG)
#  define __CG_DEPRECATED_ENUMERATOR_WITH_MSG(msg) __attribute__((deprecated(msg)))
# else
#  define __CG_DEPRECATED_ENUMERATOR_WITH_MSG(msg) __CG_DEPRECATED_ENUMERATOR
# endif
#endif

/* Define `CG_AVAILABLE_STARTING' and `CG_AVAILABLE_BUT_DEPRECATED'. */

#define GET_CG_AVAIL_MACRO(_1,_2,NAME,...) NAME
#define CG_AVAILABLE_STARTING(...) GET_CG_AVAIL_MACRO(__VA_ARGS__, CG_AVAILABLE_STARTING2, CG_AVAILABLE_STARTING1)(__VA_ARGS__)
#define CG_AVAILABLE_STARTING_IOS(i) API_AVAILABLE(ios(i))
#define CG_AVAILABLE_STARTING1(m) API_AVAILABLE(macos(m))
#define CG_AVAILABLE_STARTING2(m,i) API_AVAILABLE(macos(m), ios(i))

#define GET_CG_AVAIL_BUT_DEPR_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
#define CG_AVAILABLE_BUT_DEPRECATED(...) GET_CG_AVAIL_BUT_DEPR_MACRO(__VA_ARGS__, CG_AVAILABLE_BUT_DEPRECATED5, CG_AVAILABLE_BUT_DEPRECATED4, CG_AVAILABLE_BUT_DEPRECATED3, CG_AVAILABLE_BUT_DEPRECATED2, CG_AVAILABLE_BUT_DEPRECATED1)(__VA_ARGS__)
#define CG_AVAILABLE_BUT_DEPRECATED1(m0)
#define CG_AVAILABLE_BUT_DEPRECATED2(m0,m1) API_DEPRECATED("No longer supported", macos(m0,m1))
#define CG_AVAILABLE_BUT_DEPRECATED3(m0,m1,w) API_DEPRECATED(w, macos(m0,m1))
#define CG_AVAILABLE_BUT_DEPRECATED4(m0,m1,i0,i1) API_DEPRECATED("No longer supported", macos(m0,m1), ios(i0,i1))
#define CG_AVAILABLE_BUT_DEPRECATED5(m0,m1,i0,i1,w) API_DEPRECATED(w, macos(m0,m1), ios(i0,i1))
#define CG_UNAVAILABLE_DESKTOP API_UNAVAILABLE(macos)
#define CG_UNAVAILABLE_EMBEDDED API_UNAVAILABLE(ios, tvos, watchos)

#if (defined(TARGET_OS_LINUX) && TARGET_OS_LINUX) || defined(CG_LINUX)
# undef __OSX_AVAILABLE_STARTING
# undef __OSX_AVAILABLE_BUT_DEPRECATED
# undef CG_AVAILABLE_STARTING
# undef CG_AVAILABLE_STARTING_IOS
# undef CG_AVAILABLE_BUT_DEPRECATED
# undef CG_UNAVAILABLE_DESKTOP
# undef CG_UNAVAILABLE_EMBEDDED

# if defined(__x86_64__)
    typedef unsigned int    boolean_t;
# else
    typedef int        boolean_t;
# endif

# define CG_OS_VERSION_2020 1
#else
# if TARGET_OS_OSX
#  define CG_OSX_VERSION(v) ((v) > 0 && __MAC_OS_X_VERSION_MAX_ALLOWED >= (v))
# else
#  define CG_OSX_VERSION(v) (0)
#endif

# if TARGET_OS_IPHONE
#  define CG_IOS_VERSION(v) ((v) > 0 && __IPHONE_OS_VERSION_MIN_REQUIRED >= (v))
# else
#  define CG_IOS_VERSION(v) (0)
# endif

# if TARGET_OS_TV
#  define CG_TV_VERSION(v) ((v) > 0 && __TV_OS_VERSION_MIN_REQUIRED >= (v))
# else
#  define CG_TV_VERSION(v) (0)
# endif

# if TARGET_OS_WATCH
#  define CG_WATCH_VERSION(v) ((v) > 0 && __WATCH_OS_VERSION_MIN_REQUIRED >= (v))
# else
#  define CG_WATCH_VERSION(v) (0)
# endif


#  define CG_BRIDGE_VERSION(v) (0)


# define CG_OS_VERSION_2020 (CG_OSX_VERSION(__MAC_10_16)     || \
                             CG_IOS_VERSION(__IPHONE_14_0)   || \
                             CG_TV_VERSION(__TVOS_14_0)      || \
                             CG_WATCH_VERSION(__WATCHOS_7_0) || \
                             CG_BRIDGE_VERSION(50000))
#endif

#ifndef __OSX_AVAILABLE_STARTING
# define __OSX_AVAILABLE_STARTING(m0,i)
# define __OSX_AVAILABLE_BUT_DEPRECATED(m0,m1,i0,i1)
# define CG_AVAILABLE_STARTING(...)
# define CG_AVAILABLE_STARTING_IOS(...)
# define CG_AVAILABLE_BUT_DEPRECATED(...)
# define CG_UNAVAILABLE_DESKTOP
# define CG_UNAVAILABLE_EMBEDDED
#endif

/* Definition of `__CG_STATIC_ASSERT'. */

#if !defined(__CG_STATIC_ASSERT)
# if defined(__cplusplus) && __CG_HAS_COMPILER_EXTENSION(cxx_static_assert)
#  define __CG_STATIC_ASSERT(constant_expression)                 \
     static_assert(constant_expression, #constant_expression)
# elif !defined(__cplusplus) && __CG_HAS_COMPILER_EXTENSION(c_static_assert)
#  define __CG_STATIC_ASSERT(constant_expression)                 \
     _Static_assert(constant_expression, #constant_expression)
# else
#  define __CG_STATIC_ASSERT(constant_expression)
# endif
#endif

/* Definition of `CG_INLINE'. */

#if !defined(CG_INLINE)
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define CG_INLINE static inline
# elif defined(__cplusplus)
#  define CG_INLINE static inline
# elif defined(__GNUC__)
#  define CG_INLINE static __inline__
# elif defined(__WIN32__)
#  define CG_INLINE static __inline
# else
#  define CG_INLINE static    
# endif
#endif

/* Definition of `__CG_NO_INLINE'. */

#if !defined(__CG_NO_INLINE)
# if __CG_HAS_COMPILER_ATTRIBUTE(noinline)
#  define __CG_NO_INLINE static __attribute__((noinline))
# else
#  define __CG_NO_INLINE static
# endif
#endif

/* Definition of `__CG_FORMAT_PRINTF'. */

#if !defined(__CG_FORMAT_PRINTF)
# if __CG_HAS_COMPILER_ATTRIBUTE(format)
#  define __CG_FORMAT_PRINTF(FORMAT_STRING, STARTING_ARG)           \
     __attribute__((format(printf, FORMAT_STRING, STARTING_ARG)))   \
     __attribute__((nonnull(FORMAT_STRING)))
# else
#  define __CG_FORMAT_PRINTF(FORMAT_STRING, STARTING_ARG)
# endif
#endif

/* Definition of `__CG_FORMAT_VPRINTF'. */

#if !defined(__CG_FORMAT_VPRINTF)
# if __CG_HAS_COMPILER_ATTRIBUTE(format)
#  define __CG_FORMAT_VPRINTF(FORMAT_STRING)                        \
     __attribute__((format(printf, FORMAT_STRING, 0)))
# else
#  define __CG_FORMAT_VPRINTF(FORMAT_STRING)
# endif
#endif

/* Definition of `CGFLOAT_TYPE', `CGFLOAT_IS_DOUBLE', `CGFLOAT_MIN', and
   `CGFLOAT_MAX'. */

#if defined(__LP64__) && __LP64__
# define CGFLOAT_TYPE double
# define CGFLOAT_IS_DOUBLE 1
# define CGFLOAT_MIN DBL_MIN
# define CGFLOAT_MAX DBL_MAX
# define CGFLOAT_EPSILON DBL_EPSILON
#else
# define CGFLOAT_TYPE float
# define CGFLOAT_IS_DOUBLE 0
# define CGFLOAT_MIN FLT_MIN
# define CGFLOAT_MAX FLT_MAX
# define CGFLOAT_EPSILON FLT_EPSILON
#endif

/* Definition of the `CGFloat' type and `CGFLOAT_DEFINED'. */

typedef CGFLOAT_TYPE CGFloat;
#define CGFLOAT_DEFINED 1

/* Definition of `CG_OBSOLETE'. Deprecated; to be removed. */

#if !defined(CG_OBSOLETE)
# define CG_OBSOLETE __CG_DEPRECATED
#endif

/* Definition of `CG_PRIVATE_EXTERN'. Deprecated; to be removed. */

#if !defined(CG_PRIVATE_EXTERN) && !defined(CG_BUILDING_CG)
# define CG_PRIVATE_EXTERN CG_LOCAL
#endif

typedef struct  CF_BRIDGED_TYPE(id) CF_BRIDGED_MUTABLE_TYPE(IOSurface) __IOSurface *IOSurfaceRef 
__attribute__((swift_name("IOSurfaceRef")))
;

/* 'cg_nullable' will be dropped for new Swift clients. All others get currently the old behavior */

#if defined(__swift__)
#   define cg_nullable
#else
#   define cg_nullable __nullable
#endif

#if __has_feature(nullability_on_arrays)
# define CG_NONNULL_ARRAY __nonnull
# define CG_NULLABLE_ARRAY __nullable
#else
# define CG_NONNULL_ARRAY
# define CG_NULLABLE_ARRAY
#endif

/* Definition of the `CG_BOXABLE'. */

#if defined(__has_attribute) && __has_attribute(objc_boxable)
# define CG_BOXABLE __attribute__((objc_boxable))
#else
# define CG_BOXABLE
#endif


#endif /* CGBASE_H_ */
