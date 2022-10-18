/*
	NSScrollView.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSDate.h>
#import <AppKit/NSView.h>
#import <AppKit/NSScroller.h>
#import <AppKit/NSTextFinder.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSClipView, NSRulerView, NSScroller;

typedef NS_ENUM(NSInteger, NSScrollElasticity) {
    NSScrollElasticityAutomatic = 0, // automatically determine whether to allow elasticity on this axis
    NSScrollElasticityNone      = 1, // disallow scrolling beyond document bounds on this axis
    NSScrollElasticityAllowed   = 2, // allow content to be scrolled past its bounds on this axis in an elastic fashion
} API_AVAILABLE(macos(10.7));

@interface NSScrollView : NSView <NSTextFinderBarContainer>

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Returns the NSScrollView frame size that yields the specified contentView frame size.  This method should be used in preference to the deprecated +frameSizeForContentSize:hasHorizontalScroller:hasVerticalScroller:borderType:, which makes assumptions about the scrollers' classes, control size, and style.  The "horizontalScrollerClass" parameter should specify the class of horizontal scroller to use if the NSScrollView will have a horizontal scroller, or Nil if it will not.  Likewise for the "verticalScrollerClass" parameter.
*/
+ (NSSize)frameSizeForContentSize:(NSSize)cSize horizontalScrollerClass:(nullable Class)horizontalScrollerClass verticalScrollerClass:(nullable Class)verticalScrollerClass borderType:(NSBorderType)type controlSize:(NSControlSize)controlSize scrollerStyle:(NSScrollerStyle)scrollerStyle API_AVAILABLE(macos(10.7));

/* Returns the contentView frame size that yields the specified NSScrollView frame size.  This method should be used in preference to the deprecated +contentSizeForFrameSize:hasHorizontalScroller:hasVerticalScroller:borderType: method, which makes assumptions about the scrollers' classes, control size, and style.  The "horizontalScrollerClass" parameter should specify the class of horizontal scroller to use if the NSScrollView will have a horizontal scroller, or Nil if it will not.  Likewise for the "verticalScrollerClass" parameter.
*/
+ (NSSize)contentSizeForFrameSize:(NSSize)fSize horizontalScrollerClass:(nullable Class)horizontalScrollerClass verticalScrollerClass:(nullable Class)verticalScrollerClass borderType:(NSBorderType)type controlSize:(NSControlSize)controlSize scrollerStyle:(NSScrollerStyle)scrollerStyle API_AVAILABLE(macos(10.7));

/* Returns the NSScrollView frame size that yields the specified contentView frame size.  This method assumes scrollers of NSRegularControlSize, that are not subclassed in a way that affects their metrics (scrollerWidth), and also assumes that scrollers of the current [NSScroller preferredScrollerStyle] will be used, which may not be the case if conditions such as legacy scroller subclassing or presence of accessory views force fallback to NSScrollerStyleLegacy for a particular NSScrollView instance.  Since those assumptions will produce incorrect results for some cases, this method should be considered deprecated; use +frameSizeForContentSize:horizontalScrollerClass:verticalScrollerClass:borderType:controlSize:scrollerStyle:, which provides for full specification of the relevant parameters, instead.
*/
+ (NSSize)frameSizeForContentSize:(NSSize)cSize hasHorizontalScroller:(BOOL)hFlag hasVerticalScroller:(BOOL)vFlag borderType:(NSBorderType)type API_DEPRECATED("Use +frameSizeForContentSize:horizontalScrollerClass:verticalScrollerClass:borderType:controlSize:scrollerStyle: instead", macos(10.0,10.7));

/*Returns the contentView frame size that yields the specified NSScrollView frame size.  This method assumes scrollers of NSRegularControlSize, that are not subclassed in a way that affects their metrics (scrollerWidth), and also assumes that scrollers of the current [NSScroller preferredScrollerStyle] will be used, which may not be the case if conditions such as legacy scroller subclassing or presence of accessory views force fallback to NSScrollerStyleLegacy for a particular NSScrollView instance.  Since those assumptions will produce incorrect results for some cases, this method should be considered deprecated; use +contentSizeForFrameSize:horizontalScrollerClass:verticalScrollerClass:borderType:controlSize:scrollerStyle:, which provides for full specification of the relevant parameters, instead.
*/
+ (NSSize)contentSizeForFrameSize:(NSSize)fSize hasHorizontalScroller:(BOOL)hFlag hasVerticalScroller:(BOOL)vFlag borderType:(NSBorderType)type API_DEPRECATED("+contentSizeForFrameSize:horizontalScrollerClass:verticalScrollerClass:borderType:controlSize:scrollerStyle: instead", macos(10.0,10.7));

@property (readonly) NSRect documentVisibleRect;
@property (readonly) NSSize contentSize;
@property (nullable, strong) __kindof NSView *documentView;
@property (strong) NSClipView *contentView;
@property (nullable, strong) NSCursor *documentCursor;
@property NSBorderType borderType;
@property (copy) NSColor *backgroundColor;
@property BOOL drawsBackground;
@property BOOL hasVerticalScroller;
@property BOOL hasHorizontalScroller;
@property (nullable, strong) NSScroller *verticalScroller;
@property (nullable, strong) NSScroller *horizontalScroller;
@property BOOL autohidesScrollers;
@property CGFloat horizontalLineScroll;
@property CGFloat verticalLineScroll;
@property CGFloat lineScroll;
@property CGFloat horizontalPageScroll;
@property CGFloat verticalPageScroll;
@property CGFloat pageScroll;
@property BOOL scrollsDynamically;
- (void)tile;
- (void)reflectScrolledClipView:(NSClipView *)cView;
- (void)scrollWheel:(NSEvent *)event;

/* An NSScrollView's scrollerStyle determines the style of scrollers that it will use.  AppKit sets this property automatically at runtime, based on the user's "Show scroll bars" setting and (if relevant) the set of connected pointing devices and their configured scroll capabilities, as determined by [NSScroller preferredScrollerStyle].  Setting an NSScrollView's scrollerStyle sets the scrollerStyle of its horizontalScroller and verticalScroller to match the new value.  If the NSScrollView subsequently creates or is assigned a new horizontalScroller or verticalScroller, they will at that time be assigned the same scrollerStyle that was given to the NSScrollView.
*/
@property NSScrollerStyle scrollerStyle API_AVAILABLE(macos(10.7));

/* Applicable only to ScrolViews that use Overlay scrollers.  Setting an NSScrollView's scrollerKnobStyle sets the knobStyle of its horizontalScroller and verticalScroller to match the new value.  If the NSScrollView subsequently creates or is assigned a new horizontalScroller or verticalScroller, they will at that time be assigned the same knobStyle that was given to the NSScrollView.
*/
@property NSScrollerKnobStyle scrollerKnobStyle API_AVAILABLE(macos(10.7));

/* Applicable only to NSScrollViews that use Overlay scrollers.  This method can be invoked to cause the Overlay scroller knobs to be momentarily shown.
*/
- (void)flashScrollers API_AVAILABLE(macos(10.7));

/* NSScrollView can scroll its contents past its bounds to achieve an elastic effect. When set to NSScrollElasticityAutomatic, scrolling the horizontal axis beyond its document bounds only occurs if the document width is greater than the view width or, the vertical scroller is hidden and the horizontal scroller is visible. The default value is NSScrollElasticityAutomatic.
*/
@property NSScrollElasticity horizontalScrollElasticity API_AVAILABLE(macos(10.7));

/* NSScrollView can scroll its contents past its bounds to achieve an elastic effect. When set to NSScrollElasticityAuto, scrolling the vertical axis beyond its docment bounds occurs if any of the following are true: visible vertical scroller, content height greater than view height, horizontal scroller hidden
*/
@property NSScrollElasticity verticalScrollElasticity API_AVAILABLE(macos(10.7));

/* Some content is scrollable in both the horizontal and vertical axes, but is predominantly scrolled one axis at a time. Other content (such as a drawing canvas) should scroll freely in both axes. Traditionally this is not an issue with scroll wheels since they can only scroll in one direction at a time. With scroll balls and touch surfaces, it becomes more difficult to determine the user's intention. This property helps NSScrollView determine the user's intention by specifying the type of scrolling content. When set to YES, NSScrollView will only scroll in the predominate direction the user is scrolling. The default value is YES for apps build on 10.7; NO otherwise.
*/
@property BOOL usesPredominantAxisScrolling API_AVAILABLE(macos(10.7));

/* Allow the user to magnify the scrollview. Note: this does not prevent the developer from manually adjusting the magnification value. If magnification exceeds either the maximum or minimum limits for magnification, and allowsMagnification is YES, the scroll view temporarily animates the content magnification just past those limits before returning to them. The default value is NO.
*/
@property BOOL allowsMagnification API_AVAILABLE(macos(10.8));

/* This value determines how much the content is currently scaled. To animate the magnification, use the object's animator. The default value is 1.0 */
@property CGFloat magnification API_AVAILABLE(macos(10.8));

/* This value determines how large the content can be magnified. It must be greater than or equal to the minimum magnification. The default value is 4.0.
*/
@property CGFloat maxMagnification API_AVAILABLE(macos(10.8));

/* This value determines how small the content can be magnified. The default value is 0.25.
*/
@property CGFloat minMagnification API_AVAILABLE(macos(10.8));

/* Magnify content view proportionally such that the entire rect (in content view space) fits centered in the scroll view. The resulting magnification value is clipped to the minMagnification and maxMagnification values. To animate the magnification, use the object's animator.
*/
- (void)magnifyToFitRect:(NSRect)rect API_AVAILABLE(macos(10.8));

/* Scale the content view such that the passed in point (in content view space) remains at the same screen location once the scaling is completed. The resulting magnification value is clipped to the minMagnification and maxMagnification values. To animate the magnification, use the object's animator.
*/
- (void)setMagnification:(CGFloat)magnification centeredAtPoint:(NSPoint)point API_AVAILABLE(macos(10.8));

/* Some subviews of the document view do not scroll like the rest of the document. Instead these views appear to float over the document (see NSTableView floating group rows). The following API makes it easier to work with these types of floating views. Note: You are responsible for keeping track of the floating views and removing them via -removeFromSuperview when they should no longer float. A view may only float on one axis at a time.
*/
- (void)addFloatingSubview:(NSView *)view forAxis:(NSEventGestureAxis)axis API_AVAILABLE(macos(10.9));


/*  Automatically sets the scroll view's contentInsets property to account for any overlapping title bar. Defaults to YES.
*/
@property BOOL automaticallyAdjustsContentInsets API_AVAILABLE(macos(10.10));

/* The distance that the sub views are inset from the enclosing scroll view during tiling. When contentInset equal to NSEdgeInsetsZero, traditional tiling is performed. That is, the rulers, headers, etc... are tiled with the contentView frame filling the remaining space. When contentInset is not equal to NSEdgeInsetsZero, the rulers, header, etc... are inset as specified. The contentView is is placed underneath these sibling views and is only inset by scroll view border and non-overlay scrollers.
*/
@property NSEdgeInsets contentInsets API_AVAILABLE(macos(10.10));

/* The distance the scrollers are inset from the edge of the scroll view. */
@property NSEdgeInsets scrollerInsets API_AVAILABLE(macos(10.10));


@end

/* This notification is sent at the beginning of a magnify gesture. The notification object is the scroll view performing the magnification.
*/
APPKIT_EXTERN NSNotificationName const NSScrollViewWillStartLiveMagnifyNotification API_AVAILABLE(macos(10.8));

/* This notification is sent at the end of magnify gesture. The notification object is the scroll view view performing the magnification.
*/
APPKIT_EXTERN NSNotificationName const NSScrollViewDidEndLiveMagnifyNotification API_AVAILABLE(macos(10.8));

/* This notification is sent on the main thread at the beginning of user initiated live scroll tracking (gesture scroll or scroller tracking, e.g. thumb dragging).
 The notification object is the scroll view performing the scroll.
*/
APPKIT_EXTERN NSNotificationName const NSScrollViewWillStartLiveScrollNotification API_AVAILABLE(macos(10.9));

/* This notification is sent on the main thread after changing the clipview bounds origin due to a user initiated event.
 Not all user initiated scrolls are bracketed by a willStart/didEnd notification pair (legacy mice).
 The notification object is the scroll view performing the scroll.
*/
APPKIT_EXTERN NSNotificationName const NSScrollViewDidLiveScrollNotification API_AVAILABLE(macos(10.9));

/* This notification is sent on the main thread at the end of live scroll tracking.
 The notification object is the scroll view performing the scroll.
*/
APPKIT_EXTERN NSNotificationName const NSScrollViewDidEndLiveScrollNotification API_AVAILABLE(macos(10.9));


@interface NSScrollView (NSRulerSupport)

@property (class, null_resettable) Class rulerViewClass;

@property BOOL rulersVisible;

@property BOOL hasHorizontalRuler;
@property BOOL hasVerticalRuler;

@property (nullable, strong) NSRulerView *horizontalRulerView;
@property (nullable, strong) NSRulerView *verticalRulerView;

@end

typedef NS_ENUM(NSInteger, NSScrollViewFindBarPosition) {
    NSScrollViewFindBarPositionAboveHorizontalRuler = 0,
    NSScrollViewFindBarPositionAboveContent = 1,
    NSScrollViewFindBarPositionBelowContent = 2
} API_AVAILABLE(macos(10.7));

@interface NSScrollView (NSFindBarSupport)

@property NSScrollViewFindBarPosition findBarPosition API_AVAILABLE(macos(10.7));

@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
