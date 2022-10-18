/*
    NSTouch.h
    Application Kit
    Copyright (c) 2009-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSDate.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSView;

typedef NS_OPTIONS(NSUInteger, NSTouchPhase) {
    NSTouchPhaseBegan           = 1U << 0,
    NSTouchPhaseMoved           = 1U << 1,
    NSTouchPhaseStationary      = 1U << 2,
    NSTouchPhaseEnded           = 1U << 3,
    NSTouchPhaseCancelled       = 1U << 4,

    NSTouchPhaseTouching        = NSTouchPhaseBegan | NSTouchPhaseMoved | NSTouchPhaseStationary,
    NSTouchPhaseAny             = NSUIntegerMax
} API_AVAILABLE(macos(10.7));

typedef NS_ENUM(NSInteger, NSTouchType) {
    NSTouchTypeDirect,      // A direct touch from a finger (on a screen)
    NSTouchTypeIndirect,      // An indirect touch (not a screen)
} API_AVAILABLE(macos(10.12.2));

typedef NS_OPTIONS(NSUInteger, NSTouchTypeMask) {
    NSTouchTypeMaskDirect      = (1 <<  NSTouchTypeDirect),      // A direct touch from a finger (on a screen)
    NSTouchTypeMaskIndirect    = (1 <<  NSTouchTypeIndirect),      // An indirect touch (not a screen)
} API_AVAILABLE(macos(10.12.2));

NS_INLINE NSTouchTypeMask NSTouchTypeMaskFromType(NSTouchType type) { return (1 << type); }

/* Unlike the iPhone, NSTouch objects do not persist for the life of the touch.
*/
API_AVAILABLE(macos(10.6))
@interface NSTouch : NSObject <NSCopying>
/* Properties of this touch */
/* Use the identity property to track changes to a particular touch during the touch's life. While touch identities may be re-used, they are unique during the life of the touch, even when multiple devices are present. Note: identity objects implement the NSCopying protocol so that they may be used as keys in an NSDictionary. Use isEqual: to compare two touch identities.
*/
@property (readonly, strong) id<NSObject, NSCopying> identity;
@property (readonly) NSTouchPhase phase;
@property (readonly) NSPoint normalizedPosition; // Scaled absolute position is in [0,1], where (0, 0) is the lower left of the surface.
@property (readonly, getter=isResting) BOOL resting;

/* Properties of the underlying touch device */ 
@property (nullable, readonly, strong) id device; // The digitizer that generated the touch. Useful to distinguish touches emanating from multiple-device scenario

/* The range of the touch device in points (72ppi). Note: 0,0 is the lower left of the surface.
*/
@property (readonly) NSSize deviceSize;

@end

@interface NSTouch (NSTouchBar)
/* A touch can only be one type at a time */
@property(readonly) NSTouchType type API_AVAILABLE(macos(10.12.2));

/* These two methods are only valid for Direct touches. A nil view means the touch location in the root container of touch. */
- (NSPoint)locationInView:(nullable NSView *)view API_AVAILABLE(macos(10.12.2));
- (NSPoint)previousLocationInView:(nullable NSView *)view API_AVAILABLE(macos(10.12.2));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

