/*
        NSTrackingArea.h
        Application Kit
        Copyright (c) 2006-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_OPTIONS(NSUInteger, NSTrackingAreaOptions) {
    /* Type of tracking area.  You must specify one or more type from this list in the NSTrackingAreaOptions argument of -initWithRect:options:owner:userInfo: */
    NSTrackingMouseEnteredAndExited     = 0x01, // owner receives mouseEntered when mouse enters area, and mouseExited when mouse leaves area
    NSTrackingMouseMoved                = 0x02,	// owner receives mouseMoved while mouse is within area.  Note that mouseMoved events do not contain userInfo
    NSTrackingCursorUpdate 		= 0x04,	// owner receives cursorUpdate when mouse enters area.  Cursor is set appropriately when mouse exits area

    
    
    /* When tracking area is active.  You must specify one of the following in the NSTrackingAreaOptions argument of -initWithRect:options:owner:userInfo: */
    NSTrackingActiveWhenFirstResponder 	= 0x10,	// owner receives mouseEntered/Exited, mouseMoved, or cursorUpdate when view is first responder
    NSTrackingActiveInKeyWindow         = 0x20, // owner receives mouseEntered/Exited, mouseMoved, or cursorUpdate when view is in key window
    NSTrackingActiveInActiveApp 	= 0x40,	// owner receives mouseEntered/Exited, mouseMoved, or cursorUpdate when app is active
    NSTrackingActiveAlways 		= 0x80,	// owner receives mouseEntered/Exited or mouseMoved regardless of activation.  Not supported for NSTrackingCursorUpdate.
    
    

    /* Behavior of tracking area.  These values are used in NSTrackingAreaOptions.  You may specify any number of the following in the NSTrackingAreaOptions argument of -initWithRect:options:owner:userInfo: */
    NSTrackingAssumeInside              = 0x100,    // If set, generate mouseExited event when mouse leaves area (same as assumeInside argument in deprecated addTrackingRect:owner:userData:assumeInside:)
    NSTrackingInVisibleRect             = 0x200,    // If set, tracking occurs in visibleRect of view and rect is ignored
    NSTrackingEnabledDuringMouseDrag    = 0x400     // If set, mouseEntered events will be generated as mouse is dragged.  If not set, mouseEntered events will be generated as mouse is moved, and on mouseUp after a drag.  mouseExited events are paired with mouseEntered events so their delivery is affected indirectly.  That is, if a mouseEntered event is generated and the mouse subsequently moves out of the trackingArea, a mouseExited event will be generated whether the mouse is being moved or dragged, independent of this flag.
}; 


API_AVAILABLE(macos(10.5))
@interface NSTrackingArea : NSObject <NSCopying, NSCoding>
- (instancetype)initWithRect:(NSRect)rect options:(NSTrackingAreaOptions)options owner:(nullable id)owner userInfo:(nullable NSDictionary<id, id> *)userInfo;
@property (readonly) NSRect rect;
@property (readonly) NSTrackingAreaOptions options;
@property (nullable, readonly, weak) id owner;
@property (nullable, readonly, copy) NSDictionary<id, id> *userInfo;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
