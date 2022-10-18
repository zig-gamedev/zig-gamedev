/*
 * ColorSync - ColorSyncBase.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
 */

#ifndef __COLORSYNCBASE__
#define __COLORSYNCBASE__

#if !defined(__swift__)

#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <os/availability.h>
#include <stdint.h>
#include <TargetConditionals.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__CS_HAS_COMPILER_ATTRIBUTE)
    #if defined(__has_attribute)
        #define __CS_HAS_COMPILER_ATTRIBUTE(attribute) __has_attribute(attribute)
    #elif defined(__GNUC__) && __GNUC__ >= 4
        #define __CS_HAS_COMPILER_ATTRIBUTE(attribute) (1)
    #else
        #define __CS_HAS_COMPILER_ATTRIBUTE(attribute) (0)
    #endif
#endif
    
#if !defined(CSEXTERN)
    #if defined(_MSC_VER)
        #if defined(CG_BUILDING_CG)
            #define CSEXTERN __declspec(dllexport) extern
        #else
            #define CSEXTERN __declspec(dllimport) extern
        #endif
    #else
        #define CSEXTERN    __attribute__((visibility("default"))) extern
    #endif
#endif

#if !defined(CSEXPORT)
    #if defined(_MSC_VER)
        #if defined(CG_BUILDING_CG)
            #define CSEXPORT __declspec(dllexport)
        #else
            #define CSEXPORT __declspec(dllimport)
        #endif
    #else
        #define CSEXPORT    __attribute__((visibility("default")))
    #endif
#endif

#if !defined(CSLOCAL)
    #if __CS_HAS_COMPILER_ATTRIBUTE(visibility)
        #if defined(__cplusplus)
            #define CSLOCAL extern "C" __attribute__((visibility("hidden")))
        #else
            #define CSLOCAL extern __attribute__((visibility("hidden")))
        #endif
    #else
        #define CSLOCAL CSEXTERN
    #endif
#endif /* !defined(CSLOCAL) */

#if !defined(CSEXTERN_DESKTOP)
    #if TARGET_OS_OSX
        #define CSEXTERN_DESKTOP CSEXTERN
    #else
        #define CSEXTERN_DESKTOP CSLOCAL
    #endif
#endif

#ifdef __cplusplus
}
#endif


#define GET_CS_AVAIL_MACRO(_1,_2,NAME,...) NAME
#define CS_AVAILABLE_STARTING(...) GET_CS_AVAIL_MACRO(__VA_ARGS__, CS_AVAILABLE_STARTING2, CS_AVAILABLE_STARTING1)(__VA_ARGS__)
#define CS_AVAILABLE_STARTING_IOS(i) API_AVAILABLE(ios(i))
#define CS_AVAILABLE_STARTING1(m) API_AVAILABLE(macos(m))
#define CS_AVAILABLE_STARTING2(m,i) API_AVAILABLE(macos(m), ios(i))

#define GET_CS_AVAIL_BUT_DEPR_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
#define CS_AVAILABLE_BUT_DEPRECATED(...) GET_CS_AVAIL_BUT_DEPR_MACRO(__VA_ARGS__, CS_AVAILABLE_BUT_DEPRECATED5, CS_AVAILABLE_BUT_DEPRECATED4, CS_AVAILABLE_BUT_DEPRECATED3, CS_AVAILABLE_BUT_DEPRECATED2, CS_AVAILABLE_BUT_DEPRECATED1)(__VA_ARGS__)
#define CS_AVAILABLE_BUT_DEPRECATED1(m0)
#define CS_AVAILABLE_BUT_DEPRECATED2(m0,m1) API_DEPRECATED("No longer supported", macos(m0,m1))
#define CS_AVAILABLE_BUT_DEPRECATED3(m0,m1,w) API_DEPRECATED(w, macos(m0,m1))
#define CS_AVAILABLE_BUT_DEPRECATED4(m0,m1,i0,i1) API_DEPRECATED("No longer supported", macos(m0,m1), ios(i0,i1))
#define CS_AVAILABLE_BUT_DEPRECATED5(m0,m1,i0,i1,w) API_DEPRECATED(w, macos(m0,m1), ios(i0,i1))
#define CS_UNAVAILABLE_DESKTOP API_UNAVAILABLE(macos)
#define CS_UNAVAILABLE_EMBEDDED API_UNAVAILABLE(ios, tvos, watchos, macCatalyst)

#define CS_UNAVAILABLE_PUBLIC_EMBEDDED CS_UNAVAILABLE_EMBEDDED
#define CS_AVAILABLE_PUBLIC_STARTING(_1,_2) CS_AVAILABLE_STARTING(_1) CS_UNAVAILABLE_EMBEDDED
#define CS_AVAILABLE_PUBLIC_DESKTOP(_1) CS_AVAILABLE_STARTING(_1) CS_UNAVAILABLE_EMBEDDED


#else /* #if !defined(__swift__) */

#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#if !defined(__CS_HAS_COMPILER_ATTRIBUTE)
#if defined(__has_attribute)
#define __CS_HAS_COMPILER_ATTRIBUTE(attribute) __has_attribute(attribute)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define __CS_HAS_COMPILER_ATTRIBUTE(attribute) (1)
#else
#define __CS_HAS_COMPILER_ATTRIBUTE(attribute) (0)
#endif
#endif
    
#if !defined(CSEXTERN)
#if defined(_MSC_VER)
#if defined(CG_BUILDING_CG)
#define CSEXTERN __declspec(dllexport) extern
#else
#define CSEXTERN __declspec(dllimport) extern
#endif
#else
#define CSEXTERN    __attribute__((visibility("default"))) extern
#endif
#endif
    
#if !defined(CSEXPORT)
#if defined(_MSC_VER)
#if defined(CG_BUILDING_CG)
#define CSEXPORT __declspec(dllexport)
#else
#define CSEXPORT __declspec(dllimport)
#endif
#else
#define CSEXPORT    __attribute__((visibility("default")))
#endif
#endif
    
#if !defined(CSLOCAL)
#if __CS_HAS_COMPILER_ATTRIBUTE(visibility)
#if defined(__cplusplus)
#define CSLOCAL extern "C" __attribute__((visibility("hidden")))
#else
#define CSLOCAL extern __attribute__((visibility("hidden")))
#endif
#else
#define CSLOCAL CSEXTERN
#endif
#endif /* !defined(CSLOCAL) */
    
#ifdef __cplusplus
}
#endif

#endif /* #if !defined(__swift__) */

#endif /* __COLORSYNCBASE__ */
