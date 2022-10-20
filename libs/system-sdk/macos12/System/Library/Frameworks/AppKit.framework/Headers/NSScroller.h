/*
	NSScroller.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSUInteger, NSUsableScrollerParts) {
    NSNoScrollerParts			= 0,
    NSOnlyScrollerArrows API_DEPRECATED("Scroller arrows are not used anymore.", macos(10.0,10.14)) = 1,
    NSAllScrollerParts			= 2,
};

typedef NS_ENUM(NSUInteger, NSScrollerPart) {
    NSScrollerNoPart			= 0,
    NSScrollerDecrementPage		= 1,
    NSScrollerKnob			= 2,
    NSScrollerIncrementPage		= 3,
    NSScrollerDecrementLine API_DEPRECATED("Scroller arrows are not used anymore.", macos(10.0,10.14)) = 4,
    NSScrollerIncrementLine API_DEPRECATED("Scroller arrows are not used anymore.", macos(10.0,10.14)) = 5,
    NSScrollerKnobSlot			= 6,
};

typedef NS_ENUM(NSInteger, NSScrollerStyle) {
    NSScrollerStyleLegacy       = 0,
    NSScrollerStyleOverlay      = 1,
} API_AVAILABLE(macos(10.7));

typedef NS_ENUM(NSInteger, NSScrollerKnobStyle) {
    NSScrollerKnobStyleDefault  = 0,    // uses the default look for the appearance
    NSScrollerKnobStyleDark     = 1,    // dark; good against a light background
    NSScrollerKnobStyleLight    = 2,    // light; good against a dark background
} API_AVAILABLE(macos(10.7));

@interface NSScroller : NSControl

/* A subclass of NSScroller can override this method to certify that its customizations are compatible with 10.7's new iOS-like "Overlay" scroller style and behaviors.  By default, AppKit assumes that instances of NSScroller subclasses may not be compatible with the way that Overlay scrollers are presented, and falls back to the more compatible 10.6-like scroller metrics and behavior for subclassed scrollers.  The recommended override technique for a subclass "MyCustomScroller" that wants to delcare itself compatible with Overlay scroller presentation is:

    @implementation MyCustomScroller
    ...
    + (BOOL)isCompatibleWithOverlayScrollers {
        return self == [MyCustomScroller class];
    }

In this way, compatibility will be properly assessed for both "MyCustomScroller", and for potentially unknown subclasses thereof.

When it opts in in this manner, an NSScroller subclass certifies that:
- It performs any appearance customization by overriding the parts-drawing methods -drawKnob and -drawKnobSlotInRect:highlight:, and not by overriding -drawRect: wholesale.  (This is necessary to allow for separate knob and track fade in/out.  AppKit automatically applies the necessary fade alpha to whatever is drawn by -drawKnob and -drawKnobSlotInRect:highlight:.)
- It likewise performs any event-handling customization by overriding the parts-based methods -testPart: and -trackKnob:, and not by overriding -mouseDown: wholesale.
- It can deal with the fact that scroller arrows no longer exist, and -rectForPart: returns empty rects for them.
- It can accommodate the potentially different size and layout metrics used by Overlay scrollers.
*/
@property (class, readonly, getter=isCompatibleWithOverlayScrollers) BOOL compatibleWithOverlayScrollers API_AVAILABLE(macos(10.7));

/* Returns the with for scrollers of the receiving class, given the specified controlSize and scrollerStyle.  This method should be used in preference to +scrollerWidthForControlSize:, which assumes a scrollerStyle of NSScrollerStyleLegacy, and +scrollerWidth:, which assumes that and a controlSize of NSRegularControlSize.
*/
+ (CGFloat)scrollerWidthForControlSize:(NSControlSize)controlSize scrollerStyle:(NSScrollerStyle)scrollerStyle API_AVAILABLE(macos(10.7));

/* Returns the style of scrollers that applications should use wherever possible.  This value is determined by the Appearance preference panel's "Show scroll bars" setting for the current user, and -- when the user's prefernce is set to "Automatically based on input device" -- by the set of built-in and connected pointing devices and the user's scroll capability preference settings for them.  The preferredScrollerStyle will therefore change over time, and applications should be prepared to adapt their user interfaces to the new scroller style if needed.  In most cases, the updating is automatic: When the preferredScrollerStyle changes, AppKit notifies all NSScrollView instances, sending -setScrollerStyle: to each with the new style, which causees each NSScrollView to automatically re-tile (update its layout) to adapt to the new scroller style.  Some NSScrollView instances may refuse the new scroller style setting if they cannot accommodate it for compatibility reasons (presence of accessory views or legacy scroller subclasses prevent use of Overlay scrollers), but most instances will switch to the specified new preferredScrollerStyle.  Clients that wish to be notified of changes to +preferredScrollerStyle's return value can subscribe to NSPreferredScrollerStyleDidChangeNotification (declared below).
*/
@property (class, readonly) NSScrollerStyle preferredScrollerStyle API_AVAILABLE(macos(10.7));

/* Accessors for scroller's style.  For a scroller that's managed by an NSScrollView, the setter is automatically invoked by the ScrollView with the appropriate setting, according to the user's Appearance preference settings and possibly what pointing device(s) are present.
*/
@property NSScrollerStyle scrollerStyle API_AVAILABLE(macos(10.7));

/* Accessors for the scroller's knob style.  The value of this property does not affect Legacy scrollers.  NSScrollerKnobStyleDefault is appropriate for a wide range of content, but in some cases choosing an alternative knob style may enhance visibility of the scroller knob atop some kinds of content.
*/
@property NSScrollerKnobStyle knobStyle API_AVAILABLE(macos(10.7));

- (NSRect)rectForPart:(NSScrollerPart)partCode;
- (void)checkSpaceForParts;
@property (readonly) NSUsableScrollerParts usableParts;
@property NSControlSize controlSize;
- (void)drawKnob;
- (void)drawKnobSlotInRect:(NSRect)slotRect highlight:(BOOL)flag;
- (NSScrollerPart)testPart:(NSPoint)point;
- (void)trackKnob:(NSEvent *)event;
@property (readonly) NSScrollerPart hitPart;
@property CGFloat knobProportion;
- (void)setKnobProportion:(CGFloat)proportion API_AVAILABLE(macos(10.5));

@end

/* Posted when the preferred scroller style changes.  The notification object is private; disregard it.  Consult NSScroller's +preferredScrollerStyle method when this notification is received, or thereafter, to determine the new scroller style to use.
 */
APPKIT_EXTERN NSNotificationName const NSPreferredScrollerStyleDidChangeNotification API_AVAILABLE(macos(10.7));


/* Deprecated API */

typedef NS_ENUM(NSUInteger, NSScrollArrowPosition) {
    NSScrollerArrowsMaxEnd        = 0,    /* previously deprecated */
    NSScrollerArrowsMinEnd        = 1,    /* previously deprecated */
    NSScrollerArrowsDefaultSetting    = 0,
    NSScrollerArrowsNone               = 2
} API_DEPRECATED("Scroller arrows are not used anymore.", macos(10.0,10.14));

typedef NS_ENUM(NSUInteger, NSScrollerArrow) {
    NSScrollerIncrementArrow        = 0,
    NSScrollerDecrementArrow        = 1
} API_DEPRECATED("Scroller arrows are not used anymore.", macos(10.0,10.14));

@interface NSScroller(NSDeprecated)

/* Returns the with for scrollers of the receiving class, given the specified controlSize and assuming a scrollerStyle of NSScrollerStyleLegacy.
 */
+ (CGFloat)scrollerWidthForControlSize:(NSControlSize)controlSize API_DEPRECATED("Use +scrollerWidthForControlSize:scrollerStyle: instead", macos(10.0,10.7));

/* Returns the width for scrollers of the receiving class, assuming a controlSize NSRegularControlSize, and a scrollerStyle of NSScrollerStyleLegacy.
 */
+ (CGFloat)scrollerWidth API_DEPRECATED("Use +scrollerWidthForControlSize:scrollerStyle: instead", macos(10.0,10.7));

/* To maintain binary compatibility, AppKit will continue to invoke overrides of this method. Code that targets Mac OS 10.5 and later should use -setDoubleValue: and -setKnobProportion: instead, and eliminate any overrides of -setFloatValue:knobProportion:. Code that needs to remain compatible with Mac OS 10.4 and earlier should continue to use -setFloatValue:knobProportion:.
*/
- (void)setFloatValue:(float)value knobProportion:(CGFloat)proportion API_DEPRECATED("", macos(10.0,10.5));

@property NSScrollArrowPosition arrowsPosition API_DEPRECATED("Has had no effect since 10.7", macos(10.0,10.14));
@property NSControlTint controlTint API_DEPRECATED("Has had no effect since 10.7", macos(10.0,10.14));
- (void)highlight:(BOOL)flag API_DEPRECATED("Has had no effect since 10.7", macos(10.0,10.14));
- (void)trackScrollButtons:(NSEvent *)event API_DEPRECATED("Not invoked since 10.7", macos(10.0,10.14));
- (void)drawParts API_DEPRECATED("Not invoked on any macOS version", macos(10.0,10.7));
- (void)drawArrow:(NSScrollerArrow)whichArrow highlight:(BOOL)flag API_DEPRECATED("Scrollers don't have arrows as of 10.7", macos(10.0,10.14));

@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
