/*
    NSPressGestureRecognizer.h
    Application Kit
    Copyright (c) 2013-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/NSGestureRecognizer.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.10))
@interface NSPressGestureRecognizer : NSGestureRecognizer <NSCoding>

/* bitfield of the button(s) required to recognize this click where bit 0 is the primary button, 1 is the secondary button, etc...
   NSPressGestureRecognizer dynamically returns YES to delay primary, secondary and other mouse events depending on this value.
*/
@property NSUInteger buttonMask; // Defaults to 0x1

/* Time in seconds the mouse button must be held down for the gesture to be recognized */
@property NSTimeInterval minimumPressDuration; // Defaults to the user double-click interval

/* Maximum movement in screen points allowed before the gesture fails. Once recognized (after minimumPressDuration) there is no limit on cursor movement for the remainder of the gesture tracking
*/
@property CGFloat allowableMovement; // in screen points. Defaults to double-click distance

@property NSInteger numberOfTouchesRequired API_AVAILABLE(macos(10.12.2));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
