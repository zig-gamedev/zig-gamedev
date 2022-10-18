/*
    NSPanel.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSWindow.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSPanel : NSWindow

@property (getter=isFloatingPanel) BOOL floatingPanel;
@property BOOL becomesKeyOnlyIfNeeded;
@property BOOL worksWhenModal;

@end


/* Deprecated API */

APPKIT_EXTERN NSInteger NSRunAlertPanelRelativeToWindow(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, ...) API_DEPRECATED("Use NSAlert, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.0));
APPKIT_EXTERN NSInteger NSRunInformationalAlertPanelRelativeToWindow(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, ...) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleInformational, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.0));
APPKIT_EXTERN NSInteger NSRunCriticalAlertPanelRelativeToWindow(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, ...) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleCritical, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.0));

APPKIT_EXTERN NSInteger NSRunAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert instead", macos(10.0,10.10));
APPKIT_EXTERN NSInteger NSRunInformationalAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleInformational, and present it with -[NSAlert runModal] instead", macos(10.0,10.10));
APPKIT_EXTERN NSInteger NSRunCriticalAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleCritical, and present it with -[NSAlert runModal] instead", macos(10.0,10.10));

APPKIT_EXTERN void NSBeginAlertSheet(NSString *title, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, id modalDelegate, SEL didEndSelector, SEL didDismissSelector, void *contextInfo, NSString *msgFormat, ...) NS_FORMAT_FUNCTION(10,11) API_DEPRECATED("Use NSAlert, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.10));
APPKIT_EXTERN void NSBeginInformationalAlertSheet(NSString *title, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, id modalDelegate, SEL didEndSelector, SEL didDismissSelector, void *contextInfo, NSString *msgFormat, ...)NS_FORMAT_FUNCTION(10,11) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleInformational, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.10));
APPKIT_EXTERN void NSBeginCriticalAlertSheet(NSString *title, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, NSWindow *docWindow, id modalDelegate, SEL didEndSelector, SEL didDismissSelector, void *contextInfo, NSString *msgFormat, ...) NS_FORMAT_FUNCTION(10,11) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleCritical, and present it with -[NSAlert beginSheetModalForWindow:completionHandler:] instead", macos(10.0,10.10));
APPKIT_EXTERN id NSGetAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert", macos(10.0,10.10));
APPKIT_EXTERN id NSGetInformationalAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleInformational instead", macos(10.0,10.10));
APPKIT_EXTERN id NSGetCriticalAlertPanel(NSString *title, NSString *msgFormat, NSString *defaultButton, NSString *alternateButton, NSString *otherButton, ...) NS_FORMAT_FUNCTION(2,6) API_DEPRECATED("Use NSAlert with a style of NSAlertStyleCritical instead", macos(10.0,10.10));
APPKIT_EXTERN void NSReleaseAlertPanel(id panel) API_DEPRECATED("Use NSAlert instead", macos(10.0,10.10));

/*
 These constants are used by deprecated NSRunAlertPanel() and NSGetAlertPanel() functions.
 Modern NSAlert API uses NSAlertFirstButtonReturn, etc.
 */
enum {
    NSAlertDefaultReturn API_DEPRECATED("Use NSAlertFirstButtonReturn with an NSAlert presentation instead", macos(10.0,10.10)) = 1,
    NSAlertAlternateReturn API_DEPRECATED("Use NSAlertFirstButtonReturn and other NSModalResponses with an NSAlert presentation instead", macos(10.0,10.10)) = 0,
    NSAlertOtherReturn API_DEPRECATED("Use NSAlertFirstButtonReturn and other NSModalResponses with an NSAlert presentation instead", macos(10.0,10.10)) = -1,
    NSAlertErrorReturn API_DEPRECATED("Use NSAlertFirstButtonReturn and other NSModalResponses with an NSAlert presentation instead", macos(10.0,10.10)) = -2
};
enum {
    NSOKButton API_DEPRECATED_WITH_REPLACEMENT("NSModalResponseOK", macos(10.0,10.10)) = NSModalResponseOK,
    NSCancelButton API_DEPRECATED_WITH_REPLACEMENT("NSModalResponseCancel", macos(10.0,10.10)) = NSModalResponseCancel,
};

API_UNAVAILABLE_END
