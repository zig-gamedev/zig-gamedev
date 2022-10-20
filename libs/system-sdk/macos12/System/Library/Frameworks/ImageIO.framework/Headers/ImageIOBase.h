/*
 * ImageIO - ImageIOBase.h
 * Copyright (c) 2009-2016 Apple Inc. 
 * All rights reserved.
 *
 */

#ifndef __IMAGEIOBASE__
#define __IMAGEIOBASE__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>

#include <CoreFoundation/CoreFoundation.h>
#include <os/availability.h>
#include <TargetConditionals.h>


#ifdef IMAGEIO_BUILDING_IMAGEIO

# undef __OSX_AVAILABLE_STARTING
# define __OSX_AVAILABLE_STARTING(m0,i)
# undef __OSX_AVAILABLE_BUT_DEPRECATED
# define __OSX_AVAILABLE_BUT_DEPRECATED(m0,m1,i0,i1)
# define IMAGEIO_AVAILABLE_STARTING(...)
# define IMAGEIO_AVAILABLE_BUT_DEPRECATED(...)
# define IMAGEIO_UNAVAILABLE_DESKTOP
# define IMAGEIO_UNAVAILABLE_EMBEDDED

# define _iio_Nullable _Nullable
# define _iio_Nonnull  _Nonnull

#else

#define GET_IMAGEIO_AVAIL_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define IMAGEIO_AVAILABLE_STARTING(...) GET_IMAGEIO_AVAIL_MACRO(__VA_ARGS__, IMAGEIO_AVAILABLE_STARTING4, IMAGEIO_AVAILABLE_STARTING3, IMAGEIO_AVAILABLE_STARTING2, IMAGEIO_AVAILABLE_STARTING1)(__VA_ARGS__)
#define IMAGEIO_AVAILABLE_STARTING1(m) API_AVAILABLE(macos(m))
#define IMAGEIO_AVAILABLE_STARTING2(m,i) API_AVAILABLE(macos(m), ios(i))
#define IMAGEIO_AVAILABLE_STARTING3(m,i,t) API_AVAILABLE(macos(m), ios(i), tvos(t))
#define IMAGEIO_AVAILABLE_STARTING4(m,i,t,w) API_AVAILABLE(macos(m), ios(i), tvos(t), watchos(w))

#define GET_IMAGEIO_AVAIL_BUT_DEPR_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED(...) GET_IMAGEIO_AVAIL_BUT_DEPR_MACRO(__VA_ARGS__, IMAGEIO_AVAILABLE_BUT_DEPRECATED5, IMAGEIO_AVAILABLE_BUT_DEPRECATED4, IMAGEIO_AVAILABLE_BUT_DEPRECATED3, IMAGEIO_AVAILABLE_BUT_DEPRECATED2, IMAGEIO_AVAILABLE_BUT_DEPRECATED1)(__VA_ARGS__)
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED1(m0)
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED2(m0,m1) API_DEPRECATED("No longer supported", macos(m0,m1))
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED3(m0,m1,w) API_DEPRECATED(w, macos(m0,m1))
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED4(m0,m1,i0,i1) API_DEPRECATED("No longer supported", macos(m0,m1), ios(i0,i1))
#define IMAGEIO_AVAILABLE_BUT_DEPRECATED5(m0,m1,i0,i1,w) API_DEPRECATED(w, macos(m0,m1), ios(i0,i1))

#define IMAGEIO_UNAVAILABLE_DESKTOP API_UNAVAILABLE(macos)
#define IMAGEIO_UNAVAILABLE_EMBEDDED API_UNAVAILABLE(ios, tvos, watchos)

# define _iio_Nullable _Nullable
# define _iio_Nonnull _Nonnull

#endif

#if !defined(IMAGEIO_EXTERN)
#   if defined(__WIN32__)
#       if defined(IMAGEIO_BUILDING_IMAGEIO)
#           if defined(__cplusplus)
#               define IMAGEIO_EXTERN extern "C" __declspec(dllexport)
#           else /* !defined(__cplusplus) */
#               define IMAGEIO_EXTERN extern __declspec(dllexport)
#           endif /* !defined(__cplusplus) */
#       else /* !defined(IMAGEIO_BUILDING_IMAGEIO) */
#           if defined(__cplusplus)
#               define IMAGEIO_EXTERN extern "C" __declspec(dllimport) 
#           else /* !defined(__cplusplus) */
#               define IMAGEIO_EXTERN extern __declspec(dllimport)
#           endif /* !defined(__cplusplus) */
#       endif /* !defined(IMAGEIO_BUILDING_IMAGEIO) */
#   else /* !defined(__WIN32__) */
#       if defined(__cplusplus)
#           define IMAGEIO_EXTERN extern "C" __attribute__((visibility("default")))
#       else /* !defined(__cplusplus) */
#           define IMAGEIO_EXTERN extern __attribute__((visibility("default")))
#       endif /* !defined(__cplusplus) */
#   endif /* !defined(__WIN32__) */
#endif /* !defined(IMAGEIO_EXTERN) */


#if !defined(IMAGEIO_EXTERN_C_BEGIN)
#   ifdef __cplusplus
#       define IMAGEIO_EXTERN_C_BEGIN extern "C" {
#       define IMAGEIO_EXTERN_C_END   }
#   else
#       define IMAGEIO_EXTERN_C_BEGIN
#       define IMAGEIO_EXTERN_C_END
#   endif
#endif

#if (!IMAGEIO_bridgeOS) 
#    define IIO_HAS_IOSURFACE 1
#else
#    define IIO_HAS_IOSURFACE 0
#endif

#if defined(__has_feature) && __has_feature(objc_bridge_id)
#define IIO_BRIDGED_TYPE(type)       CF_BRIDGED_TYPE(type)
#else
#define IIO_BRIDGED_TYPE(type)
#endif


#endif  /* __IMAGEIOBASE__ */
