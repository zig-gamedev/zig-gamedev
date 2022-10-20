/*	NSZone.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#include <CoreFoundation/CFBase.h>
#include <stddef.h>

@class NSString;

NS_ASSUME_NONNULL_BEGIN

typedef struct _NSZone NSZone;

FOUNDATION_EXPORT NSZone *NSDefaultMallocZone(void) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT NSZone *NSCreateZone(NSUInteger startSize, NSUInteger granularity, BOOL canFree) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT void NSRecycleZone(NSZone *zone)NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");

FOUNDATION_EXPORT void NSSetZoneName(NSZone * _Nullable zone, NSString *name)NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT NSString *NSZoneName(NSZone * _Nullable zone) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT NSZone * _Nullable NSZoneFromPointer(void *ptr) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");

FOUNDATION_EXPORT void *NSZoneMalloc(NSZone * _Nullable zone, NSUInteger size) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT void *NSZoneCalloc(NSZone * _Nullable zone, NSUInteger numElems, NSUInteger byteSize) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT void *NSZoneRealloc(NSZone * _Nullable zone, void * _Nullable ptr, NSUInteger size) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");
FOUNDATION_EXPORT void NSZoneFree(NSZone * _Nullable zone, void *ptr) NS_SWIFT_UNAVAILABLE("Zone-based memory management is unavailable");

#if TARGET_OS_OSX

/* Garbage Collected memory allocation. Garbage Collection is deprecated. */

NS_ENUM(NSUInteger) {
    NSScannedOption = (1UL << 0),
    NSCollectorDisabledOption = (1UL << 1),
};

FOUNDATION_EXPORT void *NSAllocateCollectable(NSUInteger size, NSUInteger options) NS_SWIFT_UNAVAILABLE("Garbage Collection is not supported");
FOUNDATION_EXPORT void *NSReallocateCollectable(void * _Nullable ptr, NSUInteger size, NSUInteger options) NS_SWIFT_UNAVAILABLE("Garbage Collection is not supported");

#endif

#ifndef CF_CONSUMED
#if __has_feature(attribute_cf_consumed)
#define CF_CONSUMED __attribute__((cf_consumed))
#else
#define CF_CONSUMED
#endif
#endif

/*
 NSMakeCollectable
 CFTypeRef style objects are garbage collected, yet only sometime after the last CFRelease() is performed.  Particulary for fully-bridged CFTypeRef objects such as CFStrings and collections (CFDictionaryRef et alia) it is imperative that either CFMakeCollectable or the more type safe NSMakeCollectable be performed, preferably right upon allocation.  Conceptually, this moves them from a "C" style opaque pointer into an "id" style object.
 This function is unavailable in ARC mode. Use CFBridgingRelease instead.
*/
NS_INLINE NS_RETURNS_RETAINED id _Nullable NSMakeCollectable(CFTypeRef _Nullable CF_CONSUMED cf) NS_AUTOMATED_REFCOUNT_UNAVAILABLE NS_SWIFT_UNAVAILABLE("Garbage Collection is not supported");
NS_INLINE NS_RETURNS_RETAINED id _Nullable NSMakeCollectable(CFTypeRef _Nullable CF_CONSUMED cf) {
#if __has_feature(objc_arc)
    return nil;
#else
    return (id)cf;
#endif
}

FOUNDATION_EXPORT NSUInteger NSPageSize(void);
FOUNDATION_EXPORT NSUInteger NSLogPageSize(void);
FOUNDATION_EXPORT NSUInteger NSRoundUpToMultipleOfPageSize(NSUInteger bytes);
FOUNDATION_EXPORT NSUInteger NSRoundDownToMultipleOfPageSize(NSUInteger bytes);
FOUNDATION_EXPORT void *NSAllocateMemoryPages(NSUInteger bytes);
FOUNDATION_EXPORT void NSDeallocateMemoryPages(void *ptr, NSUInteger bytes);
FOUNDATION_EXPORT void NSCopyMemoryPages(const void *source, void *dest, NSUInteger bytes);
FOUNDATION_EXPORT NSUInteger NSRealMemoryAvailable(void) API_DEPRECATED("Use NSProcessInfo instead", macos(10.0,10.8), ios(2.0,6.0), watchos(2.0,2.0), tvos(9.0,9.0)); // see NSProcessInfo.h instead

NS_ASSUME_NONNULL_END
