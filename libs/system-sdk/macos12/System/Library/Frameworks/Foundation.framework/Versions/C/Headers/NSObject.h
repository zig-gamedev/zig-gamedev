/*	NSObject.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#include <TargetConditionals.h>
#import <objc/NSObject.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSZone.h>

@class NSInvocation, NSMethodSignature, NSCoder, NSString, NSEnumerator;
@class Protocol;

NS_ASSUME_NONNULL_BEGIN

/***************	Basic protocols		***************/

@protocol NSCopying

- (id)copyWithZone:(nullable NSZone *)zone;

@end

@protocol NSMutableCopying

- (id)mutableCopyWithZone:(nullable NSZone *)zone;

@end

@protocol NSCoding

- (void)encodeWithCoder:(NSCoder *)coder;
- (nullable instancetype)initWithCoder:(NSCoder *)coder; // NS_DESIGNATED_INITIALIZER

@end

// Objects which are safe to be encoded and decoded across privilege boundaries should adopt NSSecureCoding instead of NSCoding. Secure coders (those that respond YES to requiresSecureCoding) will only encode objects that adopt the NSSecureCoding protocol.
// NOTE: NSSecureCoding guarantees only that an archive contains the classes it claims. It makes no guarantees about the suitability for consumption by the receiver of the decoded content of the archive. Archived objects which  may trigger code evaluation should be validated independently by the consumer of the objects to verify that no malicious code is executed (i.e. by checking key paths, selectors etc. specified in the archive).

@protocol NSSecureCoding <NSCoding>
@required
// This property must return YES on all classes that allow secure coding. Subclasses of classes that adopt NSSecureCoding and override initWithCoder: must also override this method and return YES.
// The Secure Coding Guide should be consulted when writing methods that decode data.
@property (class, readonly) BOOL supportsSecureCoding;
@end

/***********	Base class		***********/

@interface NSObject (NSCoderMethods)

+ (NSInteger)version;
+ (void)setVersion:(NSInteger)aVersion;
@property (readonly) Class classForCoder;
- (nullable id)replacementObjectForCoder:(NSCoder *)coder;
- (nullable id)awakeAfterUsingCoder:(NSCoder *)coder NS_REPLACES_RECEIVER;

@end

#if TARGET_OS_OSX
@interface NSObject (NSDeprecatedMethods)

+ (void)poseAsClass:(Class)aClass API_DEPRECATED("Posing no longer supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0))
#if __OBJC2__
UNAVAILABLE_ATTRIBUTE
#endif
;

@end
#endif


/***********	Discardable Content		***********/

@protocol NSDiscardableContent
@required
- (BOOL)beginContentAccess;
- (void)endContentAccess;
- (void)discardContentIfPossible;
- (BOOL)isContentDiscarded;
@end

@interface NSObject (NSDiscardableContentProxy)
@property (readonly, retain) id autoContentAccessingProxy API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@end

/***********	Object Allocation / Deallocation		*******/

// For ARC code that needs this functionality, consider using class_createInstance directly.
FOUNDATION_EXPORT id NSAllocateObject(Class aClass, NSUInteger extraBytes, NSZone * _Nullable zone) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

FOUNDATION_EXPORT void NSDeallocateObject(id object) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

FOUNDATION_EXPORT id NSCopyObject(id object, NSUInteger extraBytes, NSZone * _Nullable zone) NS_AUTOMATED_REFCOUNT_UNAVAILABLE API_DEPRECATED("Not supported", macos(10.0,10.8), ios(2.0,6.0), watchos(2.0,2.0), tvos(9.0,9.0));

FOUNDATION_EXPORT BOOL NSShouldRetainWithZone(id anObject, NSZone * _Nullable requestedZone) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

FOUNDATION_EXPORT void NSIncrementExtraRefCount(id object) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

FOUNDATION_EXPORT BOOL NSDecrementExtraRefCountWasZero(id object) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

FOUNDATION_EXPORT NSUInteger NSExtraRefCount(id object) NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

#if __has_feature(objc_arc)

// After using a CFBridgingRetain on an NSObject, the caller must take responsibility for calling CFRelease at an appropriate time.
NS_INLINE CF_RETURNS_RETAINED CFTypeRef _Nullable CFBridgingRetain(id _Nullable X) {
    return (__bridge_retained CFTypeRef)X;
}

NS_INLINE id _Nullable CFBridgingRelease(CFTypeRef CF_CONSUMED _Nullable X) {
    return (__bridge_transfer id)X;
}

#else

// This function is intended for use while converting to ARC mode only.
NS_INLINE CF_RETURNS_RETAINED CFTypeRef _Nullable CFBridgingRetain(id _Nullable X) {
    return X ? CFRetain((CFTypeRef)X) : NULL;
}

// Casts a CoreFoundation object to an Objective-C object, transferring ownership to ARC (ie. no need to CFRelease to balance a prior +1 CFRetain count). NS_RETURNS_RETAINED is used to indicate that the Objective-C object returned has +1 retain count.  So the object is 'released' as far as CoreFoundation reference counting semantics are concerned, but retained (and in need of releasing) in the view of ARC. This function is intended for use while converting to ARC mode only.
NS_INLINE id _Nullable CFBridgingRelease(CFTypeRef CF_CONSUMED _Nullable X) NS_RETURNS_RETAINED {
    return [(id)CFMakeCollectable(X) autorelease];
}

#endif

NS_ASSUME_NONNULL_END
