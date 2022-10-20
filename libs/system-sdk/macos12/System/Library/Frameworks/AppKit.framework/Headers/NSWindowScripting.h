/*
        NSWindowScripting.h
        AppKit Framework
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSWindow.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSCloseCommand;
@class NSScriptCommand;

@interface NSWindow(NSScripting)

// Key value coding (KVC) compliant get-accessors for the same-named attributes declared in Cocoa's definition of the Standard Suite.  The -isMiniaturized, -isVisible, and -isZoomed methods, declared in NSWindow.h, also serve as KVC accessors.
@property (readonly) BOOL hasCloseBox;
@property (readonly) BOOL hasTitleBar;
@property (getter=isFloatingPanel, readonly) BOOL floatingPanel;
@property (getter=isMiniaturizable, readonly) BOOL miniaturizable;
@property (getter=isModalPanel, readonly) BOOL modalPanel;
@property (getter=isResizable, readonly) BOOL resizable;
@property (getter=isZoomable, readonly) BOOL zoomable;
@property NSInteger orderedIndex;

// Key value coding compliant set-accessors for the like-named attributes declared in Cocoa's definition of the Standard Suite.  Attributes that don't have corresponding methods here are read-only.
- (void)setIsMiniaturized:(BOOL)flag;
- (void)setIsVisible:(BOOL)flag;
- (void)setIsZoomed:(BOOL)flag;

// Handlers for script commands declared in Cocoa's definition of the Standard suite.
- (nullable id)handleCloseScriptCommand:(NSCloseCommand *)command;
- (nullable id)handlePrintScriptCommand:(NSScriptCommand *)command;
- (nullable id)handleSaveScriptCommand:(NSScriptCommand *)command;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
