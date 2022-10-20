/*
	NSClipView.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>
#import <Availability.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor;

@interface NSClipView : NSView

@property (copy) NSColor *backgroundColor;
@property BOOL drawsBackground;
@property (nullable, strong) __kindof NSView *documentView;
@property (readonly) NSRect documentRect;
@property (nullable, strong) NSCursor *documentCursor;
@property (readonly) NSRect documentVisibleRect;
- (void)viewFrameChanged:(NSNotification *)notification;
- (void)viewBoundsChanged:(NSNotification *)notification;
- (BOOL)autoscroll:(NSEvent *)event;
- (void)scrollToPoint:(NSPoint)newOrigin;

/* This is used to constrain the bounds of the clip view under magnification and scrolling. This also comes with the deprecation of -constrainScrollPoint:. The logic of an existing -constrainScrollPoint: can be moved to -constrainBoundsRect: by adjusting the proposedBounds origin (as opposed to 'newOrigin').
 
 To preserve compatibility, if a subclass overrides -constrainScrollPoint:, the default behavior of -constrainBoundsRect: will be to use that -constrainScrollPoint: to adjust the proposedBounds origin, and to not change the size.
 */
- (NSRect)constrainBoundsRect:(NSRect)proposedBounds API_AVAILABLE(macos(10.9));

/* The distance that the content view is inset from the enclosing scroll view.
   Note: animate with [self animator]
*/
@property NSEdgeInsets contentInsets API_AVAILABLE(macos(10.10));

/* When YES, and used as the contentView of an NSScrollView, the clip view will automatically account for other scroll view subviews such as rulers and headers. Defaults to YES.
*/
@property BOOL automaticallyAdjustsContentInsets API_AVAILABLE(macos(10.10));

@end

@interface NSView (NSClipViewSuperview)
- (void)reflectScrolledClipView:(NSClipView *)clipView;
- (void)scrollClipView:(NSClipView *)clipView toPoint:(NSPoint)point;
@end

#pragma mark Deprecated declarations

@interface NSClipView ()
- (NSPoint)constrainScrollPoint:(NSPoint)newOrigin API_DEPRECATED("Use -constrainBoundsRect: instead.", macos(10.0,10.10));
@property BOOL copiesOnScroll API_DEPRECATED("Setting this property has no effect.  NSClipView will always minimize the area of the document view that is invalidated.  To force invalidation of the document view, use -[NSView setNeedsDisplayInRect:].", macos(10.0, 11.0));
@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
