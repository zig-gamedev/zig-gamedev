/*
        NSInterfaceStyle.h
        Application Kit
        Copyright (c) 1995-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSResponder.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

enum {
    NSNoInterfaceStyle API_DEPRECATED("", macos(10.0,10.8)) = 0,    // Default value for a window's interfaceStyle
    NSNextStepInterfaceStyle API_DEPRECATED("", macos(10.0,10.8)) = 1,
    NSWindows95InterfaceStyle API_DEPRECATED("", macos(10.0,10.8)) = 2,
    NSMacintoshInterfaceStyle API_DEPRECATED("", macos(10.0,10.8)) = 3
};
typedef NSUInteger NSInterfaceStyle API_DEPRECATED("", macos(10.0,10.8));

APPKIT_EXTERN NSInterfaceStyle NSInterfaceStyleForKey(NSString *key, NSResponder *responder) API_DEPRECATED("", macos(10.0,10.8));
    // Responders can use this function to parameterize their drawing and behavior.  If the responder has specific defaults to control various aspects of its interface individually, the keys for those special settings can be passed in, otherwise pass nil to get the global setting.  The responder should always be passed, but in situations where a responder is not available, pass nil.

@interface NSResponder (NSInterfaceStyle)
- (NSInterfaceStyle)interfaceStyle API_DEPRECATED("", macos(10.0,10.8));
- (void)setInterfaceStyle:(NSInterfaceStyle)interfaceStyle API_DEPRECATED("", macos(10.0,10.8));
// A responder's style (if set) overrides all other settings.  Responders which need to redraw or do any other work when the interface style changes should override this to do it.  Always call super.
@end

// Default strings

APPKIT_EXTERN NSString *NSInterfaceStyleDefault API_DEPRECATED("", macos(10.0,10.8));

API_UNAVAILABLE_END
