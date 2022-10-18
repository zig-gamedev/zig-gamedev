/*
        NSSwitch.h
        Application Kit
        Copyright (c) 2019-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.15))
@interface NSSwitch : NSControl <NSAccessibilitySwitch>

/* The current position of the switch. The values `NSControlStateValueOff` and `NSControlStateValueOn` indicate that the switch is in either the off or on position respectively. If the state property is set to any other value, it is interpreted as on.

 If set through the animator proxy, the switch animates to the new value.
 */
@property NSControlStateValue state;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
