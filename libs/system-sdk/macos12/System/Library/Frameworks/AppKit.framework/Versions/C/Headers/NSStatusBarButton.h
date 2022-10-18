/*
    NSStatusBarButton.h
    Application Kit
    Copyright (c) 1997-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSButton.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/*!
 * StatusBarButtons are the visual representation of NSStatusItems, and are primarily displayed on the right side of the menu bar.
 * When a template image is set as the \c image property of the status bar button, it is rendered with the correct menu bar style. This guarantees that the button will look correct in various button states and appearances (such as dark menu bar).
 */
API_AVAILABLE(macos(10.10))
@interface NSStatusBarButton : NSButton

/// When YES the status bar icon has a disabled/off appearance while still being functional, such as allowing selection and actions. Defaults to NO.
@property BOOL appearsDisabled;

@end

API_UNAVAILABLE_END
