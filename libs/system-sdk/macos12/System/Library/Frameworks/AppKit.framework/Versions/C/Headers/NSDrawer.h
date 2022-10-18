/*
    NSDrawer.h
    Application Kit
    Copyright (c) 1999-2021, Apple Inc.
    All rights reserved.
*/

#import <CoreFoundation/CFDate.h>
#import <CoreFoundation/CFRunLoop.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSResponder.h>
#import <AppKit/NSWindow.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSLock;
@class NSView;
@class NSWindow;
@class NSNotification;
@protocol NSDrawerDelegate;

typedef NS_ENUM(NSUInteger, NSDrawerState) {
    NSDrawerClosedState API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13)) = 0,
    NSDrawerOpeningState API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13)) = 1,
    NSDrawerOpenState API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13)) = 2,
    NSDrawerClosingState API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13)) = 3
};

API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13))
@interface NSDrawer : NSResponder <NSAccessibilityElement, NSAccessibility>

- (instancetype)initWithContentSize:(NSSize)contentSize preferredEdge:(NSRectEdge)edge;

@property (nullable, assign) NSWindow *parentWindow;
@property (nullable, strong) NSView *contentView;
@property NSRectEdge preferredEdge;
@property (nullable, assign) id<NSDrawerDelegate> delegate;

- (void)open;
- (void)openOnEdge:(NSRectEdge)edge;
- (void)close;

- (void)open:(nullable id)sender;
- (void)close:(nullable id)sender;
- (void)toggle:(nullable id)sender;

@property (readonly) NSInteger state;
@property (readonly) NSRectEdge edge;

@property NSSize contentSize;
@property NSSize minContentSize;
@property NSSize maxContentSize;

@property CGFloat leadingOffset;
@property CGFloat trailingOffset;

@end

@interface NSWindow(NSDrawers)

@property (nullable, readonly, copy) NSArray<NSDrawer *> *drawers API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));

@end

@protocol NSDrawerDelegate <NSObject>
@optional
- (BOOL)drawerShouldOpen:(NSDrawer *)sender API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
- (BOOL)drawerShouldClose:(NSDrawer *)sender API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
- (NSSize)drawerWillResizeContents:(NSDrawer *)sender toSize:(NSSize)contentSize API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));

- (void)drawerWillOpen:(NSNotification *)notification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
- (void)drawerDidOpen:(NSNotification *)notification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
- (void)drawerWillClose:(NSNotification *)notification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
- (void)drawerDidClose:(NSNotification *)notification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));

@end

APPKIT_EXTERN NSNotificationName NSDrawerWillOpenNotification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
APPKIT_EXTERN NSNotificationName NSDrawerDidOpenNotification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
APPKIT_EXTERN NSNotificationName NSDrawerWillCloseNotification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));
APPKIT_EXTERN NSNotificationName NSDrawerDidCloseNotification API_DEPRECATED("Drawers are deprecated; consider using NSSplitViewController", macos(10.0,10.13));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

