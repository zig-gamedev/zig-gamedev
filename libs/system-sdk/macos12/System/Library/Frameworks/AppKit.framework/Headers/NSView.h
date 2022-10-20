/*
    NSView.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSRange.h>

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSAnimation.h>
#import <AppKit/NSAppearance.h>
#import <AppKit/NSDragging.h>
#import <AppKit/NSGraphics.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/NSResponder.h>
#import <AppKit/NSTouch.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/NSUserInterfaceLayout.h>

@protocol NSDraggingSource;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSBitmapImageRep, NSCursor, NSDraggingSession, NSGestureRecognizer, NSGraphicsContext, NSImage, NSScrollView, NSTextInputContext, NSWindow, NSAttributedString;
@class CIFilter, CALayer, NSScreen, NSShadow, NSTrackingArea, NSLayoutGuide;

// Bitset options for the autoresizingMask
typedef NS_OPTIONS(NSUInteger, NSAutoresizingMaskOptions) {
    NSViewNotSizable			=  0,
    NSViewMinXMargin			=  1,
    NSViewWidthSizable			=  2,
    NSViewMaxXMargin			=  4,
    NSViewMinYMargin			=  8,
    NSViewHeightSizable			= 16,
    NSViewMaxYMargin			= 32
};

typedef NS_ENUM(NSUInteger, NSBorderType) {
    NSNoBorder				= 0,
    NSLineBorder			= 1,
    NSBezelBorder			= 2,
    NSGrooveBorder			= 3
};

typedef NS_ENUM(NSInteger, NSViewLayerContentsRedrawPolicy) {
    // Leave the layer's contents alone. Never mark the layer as needing display, or draw the view's contents to the layer
    NSViewLayerContentsRedrawNever = 0,
    // Map view -setNeedsDisplay...: activity to the layer, and redraw affected layer parts by invoking the view's -drawRect:, but don't mark the view or layer as needing display when the view's size changes.
    NSViewLayerContentsRedrawOnSetNeedsDisplay = 1,
    // Resize the layer and redraw the view to the layer when the view's size changes. If the resize is animated, AppKit will drive the resize animation itself and will do this resize+redraw at each step of the animation. Affected parts of the layer will also be redrawn when the view is marked as needing display. (This mode is a superset of NSViewLayerContentsRedrawOnSetNeedsDisplay.) 
    NSViewLayerContentsRedrawDuringViewResize = 2,
    // Resize the layer and redraw the view to the layer when the view's size changes. This will be done just once at the beginning of a resize animation, not at each frame of the animation. Affected parts of the layer will also be redrawn when the view is marked as needing display. (This mode is a superset of NSViewLayerContentsRedrawOnSetNeedsDisplay.)
    NSViewLayerContentsRedrawBeforeViewResize = 3,
    // When a view is resized, the layer contents will be redrawn once and the contents will crossfade from the old value to the new value. Use this in conjunction with the layerContentsPlacement to get a nice crossfade animation for complex layer-backed views that can't correctly update on each step of the animation
    NSViewLayerContentsRedrawCrossfade API_AVAILABLE(macos(10.9)) = 4
} API_AVAILABLE(macos(10.6));

typedef NS_ENUM(NSInteger, NSViewLayerContentsPlacement) {
    NSViewLayerContentsPlacementScaleAxesIndependently      =  0,
    NSViewLayerContentsPlacementScaleProportionallyToFit    =  1,
    NSViewLayerContentsPlacementScaleProportionallyToFill   =  2,
    NSViewLayerContentsPlacementCenter                      =  3,
    NSViewLayerContentsPlacementTop                         =  4,
    NSViewLayerContentsPlacementTopRight                    =  5,
    NSViewLayerContentsPlacementRight                       =  6,
    NSViewLayerContentsPlacementBottomRight                 =  7,
    NSViewLayerContentsPlacementBottom                      =  8,
    NSViewLayerContentsPlacementBottomLeft                  =  9,
    NSViewLayerContentsPlacementLeft                        = 10,
    NSViewLayerContentsPlacementTopLeft                     = 11
} API_AVAILABLE(macos(10.6));

typedef NSInteger NSTrackingRectTag;
typedef NSInteger NSToolTipTag;

@interface NSView : NSResponder <NSAnimatablePropertyContainer, NSUserInterfaceItemIdentification, NSDraggingDestination, NSAppearanceCustomization, NSAccessibilityElement, NSAccessibility>

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (nullable, readonly, assign) NSWindow *window;
@property (nullable, readonly, assign) NSView *superview;
@property (copy) NSArray<__kindof NSView *> *subviews;
- (BOOL)isDescendantOf:(NSView *)view;
- (nullable NSView *)ancestorSharedWithView:(NSView *)view;
@property (nullable, readonly, assign) NSView *opaqueAncestor;
@property (getter=isHidden) BOOL hidden;
@property (getter=isHiddenOrHasHiddenAncestor, readonly) BOOL hiddenOrHasHiddenAncestor;

- (void)getRectsBeingDrawn:(const NSRect * _Nullable * _Nullable)rects count:(nullable NSInteger *)count;
- (BOOL)needsToDrawRect:(NSRect)rect;
@property (readonly) BOOL wantsDefaultClipping;
- (void)viewDidHide API_AVAILABLE(macos(10.5));
- (void)viewDidUnhide API_AVAILABLE(macos(10.5));

- (void)addSubview:(NSView *)view;
- (void)addSubview:(NSView *)view positioned:(NSWindowOrderingMode)place relativeTo:(nullable NSView *)otherView;
- (void)sortSubviewsUsingFunction:(NSComparisonResult (NS_NOESCAPE *)(__kindof NSView *, __kindof NSView *,  void * _Nullable))compare context:(nullable void *)context;

/* NOTE: In general, it is good practice to call 'super' for the viewWill* and viewDid* methods. Some AppKit subclasses, such as NSTableView, depend on this behavior, and calling super is required for things to work properly.
 */
- (void)viewWillMoveToWindow:(nullable NSWindow *)newWindow;
- (void)viewDidMoveToWindow;
- (void)viewWillMoveToSuperview:(nullable NSView *)newSuperview;
- (void)viewDidMoveToSuperview;

- (void)didAddSubview:(NSView *)subview;
- (void)willRemoveSubview:(NSView *)subview;

- (void)removeFromSuperview;
- (void)replaceSubview:(NSView *)oldView with:(NSView *)newView;
- (void)removeFromSuperviewWithoutNeedingDisplay;
- (void)viewDidChangeBackingProperties API_AVAILABLE(macos(10.7)); // available in 10.7.4

@property BOOL postsFrameChangedNotifications;

- (void)resizeSubviewsWithOldSize:(NSSize)oldSize;
- (void)resizeWithOldSuperviewSize:(NSSize)oldSize;
@property BOOL autoresizesSubviews;
@property NSAutoresizingMaskOptions autoresizingMask;

- (void)setFrameOrigin:(NSPoint)newOrigin;
- (void)setFrameSize:(NSSize)newSize;
@property NSRect frame;
@property CGFloat frameRotation;
@property CGFloat frameCenterRotation API_AVAILABLE(macos(10.5));

- (void)setBoundsOrigin:(NSPoint)newOrigin;
- (void)setBoundsSize:(NSSize)newSize;
@property CGFloat boundsRotation;
- (void)translateOriginToPoint:(NSPoint)translation;
- (void)scaleUnitSquareToSize:(NSSize)newUnitSize;
- (void)rotateByAngle:(CGFloat)angle;
@property NSRect bounds;

@property (getter=isFlipped, readonly) BOOL flipped;
@property (getter=isRotatedFromBase, readonly) BOOL rotatedFromBase;
@property (getter=isRotatedOrScaledFromBase, readonly) BOOL rotatedOrScaledFromBase;

/* A hint as to whether or not this view draws its contents completely opaque or not. Opaque content drawing can allow some optimizations to happen. The default value is NO.
 */
@property (getter=isOpaque, readonly) BOOL opaque;

- (NSPoint)convertPoint:(NSPoint)point fromView:(nullable NSView *)view;
- (NSPoint)convertPoint:(NSPoint)point toView:(nullable NSView *)view;
- (NSSize)convertSize:(NSSize)size fromView:(nullable NSView *)view;
- (NSSize)convertSize:(NSSize)size toView:(nullable NSView *)view;
- (NSRect)convertRect:(NSRect)rect fromView:(nullable NSView *)view;
- (NSRect)convertRect:(NSRect)rect toView:(nullable NSView *)view;

/* Uses NSIntegralRectWithOptions() to produce a backing store pixel aligned rectangle from the given input rectangle in local view coordinates.
 */
- (NSRect)backingAlignedRect:(NSRect)rect options:(NSAlignmentOptions)options API_AVAILABLE(macos(10.7));
- (NSRect)centerScanRect:(NSRect)rect;

/* New methods for converting to and from backing store pixels */

- (NSPoint)convertPointToBacking:(NSPoint)point API_AVAILABLE(macos(10.7));
- (NSPoint)convertPointFromBacking:(NSPoint)point API_AVAILABLE(macos(10.7));
- (NSSize)convertSizeToBacking:(NSSize)size API_AVAILABLE(macos(10.7));
- (NSSize)convertSizeFromBacking:(NSSize)size API_AVAILABLE(macos(10.7));
- (NSRect)convertRectToBacking:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSRect)convertRectFromBacking:(NSRect)rect API_AVAILABLE(macos(10.7));

/* Use these methods to transform geometry between a view's interior (bounds) coordinate space and its layer's interior coordinate space.  The layer's space is virtual, and doesn't take into account the layer's contentsScale setting.  For conversion between view space and layer pixels, use -convert(Point/Size/Rect)(To/From)Backing: instead.
*/
- (NSPoint)convertPointToLayer:(NSPoint)point API_AVAILABLE(macos(10.7));
- (NSPoint)convertPointFromLayer:(NSPoint)point API_AVAILABLE(macos(10.7));
- (NSSize)convertSizeToLayer:(NSSize)size API_AVAILABLE(macos(10.7));
- (NSSize)convertSizeFromLayer:(NSSize)size API_AVAILABLE(macos(10.7));
- (NSRect)convertRectToLayer:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSRect)convertRectFromLayer:(NSRect)rect API_AVAILABLE(macos(10.7));

/* Reports whether AppKit may invoke the view's -drawRect: method on a background thread, where it would otherwise be invoked on the main thread.  Defaults to NO.
*/
@property BOOL canDrawConcurrently API_AVAILABLE(macos(10.6));

/* Sets whether AppKit may invoke the view's -drawRect: method on a background thread, where it would otherwise be invoked on the main thread.  Defaults to NO for most kinds of views.  May be set to YES to enable threaded drawing for a particular view instance.  The view's window must also have its "allowsConcurrentViewDrawing" property set to YES (the default) for threading of view drawing to actually take place. */

@property (readonly) BOOL canDraw API_DEPRECATED("If a view needs display, -drawRect: or -updateLayer will be called automatically when the view is able to draw.  To check whether a view is in a window, call -window.  To check whether a view is hidden, call -isHiddenOrHasHiddenAncestor.", macos(10.0,10.14));
- (void)setNeedsDisplayInRect:(NSRect)invalidRect;
@property BOOL needsDisplay;
- (void)lockFocus API_DEPRECATED("To draw, subclass NSView and implement -drawRect:; AppKit's automatic deferred display mechanism will call -drawRect: as necessary to display the view.", macos(10.0,10.14));
- (void)unlockFocus API_DEPRECATED("To draw, subclass NSView and implement -drawRect:; AppKit's automatic deferred display mechanism will call -drawRect: as necessary to display the view.", macos(10.0,10.14));
- (BOOL)lockFocusIfCanDraw API_DEPRECATED("To draw, subclass NSView and implement -drawRect:; AppKit's automatic deferred display mechanism will call -drawRect: as necessary to display the view.", macos(10.0,10.14));
- (BOOL)lockFocusIfCanDrawInContext:(NSGraphicsContext *)context API_DEPRECATED("Use -[NSView displayRectIgnoringOpacity:inContext:] to draw a view subtree into a graphics context.", macos(10.4,10.13));
@property (class, readonly, nullable, strong) NSView *focusView;
@property (readonly) NSRect visibleRect;

- (void)display;
- (void)displayIfNeeded;
- (void)displayIfNeededIgnoringOpacity;
- (void)displayRect:(NSRect)rect;
- (void)displayIfNeededInRect:(NSRect)rect;
- (void)displayRectIgnoringOpacity:(NSRect)rect;
- (void)displayIfNeededInRectIgnoringOpacity:(NSRect)rect;
- (void)drawRect:(NSRect)dirtyRect;
- (void)displayRectIgnoringOpacity:(NSRect)rect inContext:(NSGraphicsContext *)context;

- (nullable NSBitmapImageRep *)bitmapImageRepForCachingDisplayInRect:(NSRect)rect;
- (void)cacheDisplayInRect:(NSRect)rect toBitmapImageRep:(NSBitmapImageRep *)bitmapImageRep;
- (void)viewWillDraw API_AVAILABLE(macos(10.5));

- (void)scrollPoint:(NSPoint)point;
- (BOOL)scrollRectToVisible:(NSRect)rect;
- (BOOL)autoscroll:(NSEvent *)event;
- (NSRect)adjustScroll:(NSRect)newVisible;
- (void)scrollRect:(NSRect)rect by:(NSSize)delta API_DEPRECATED("Use NSScrollView to achieve scrolling views.", macos(10.0,10.14));
- (void)translateRectsNeedingDisplayInRect:(NSRect)clipRect by:(NSSize)delta API_AVAILABLE(macos(10.5));

- (nullable NSView *)hitTest:(NSPoint)point;
- (BOOL)mouse:(NSPoint)point inRect:(NSRect)rect;
- (nullable __kindof NSView *)viewWithTag:(NSInteger)tag;
@property (readonly) NSInteger tag;
- (BOOL)performKeyEquivalent:(NSEvent *)event;
- (BOOL)acceptsFirstMouse:(nullable NSEvent *)event;
- (BOOL)shouldDelayWindowOrderingForEvent:(NSEvent *)event;
@property (readonly) BOOL needsPanelToBecomeKey;
@property (readonly) BOOL mouseDownCanMoveWindow;

/* Deprecated in favor of allowedTouchTypes. Return YES if allowedTouchTypes includes NSTouchTypeMaskIndirect */
@property BOOL acceptsTouchEvents API_DEPRECATED("Use allowedTouchTypes instead", macos(10.6,10.12.2));

/* In some cases, the user may rest a thumb or other touch on the device. By default, these touches are not delivered and are not included in the event's set of touches. Touches may transition in and out of resting at any time. Unless the view wants restingTouches, began / ended events are simlulated as touches transition from resting to active and vice versa.
*/
@property BOOL wantsRestingTouches API_AVAILABLE(macos(10.6));

- (void)addCursorRect:(NSRect)rect cursor:(NSCursor *)object;
- (void)removeCursorRect:(NSRect)rect cursor:(NSCursor *)object;
- (void)discardCursorRects;
- (void)resetCursorRects;

- (NSTrackingRectTag)addTrackingRect:(NSRect)rect owner:(id)owner userData:(nullable void *)data assumeInside:(BOOL)flag;
- (void)removeTrackingRect:(NSTrackingRectTag)tag;

- (CALayer *)makeBackingLayer API_AVAILABLE(macos(10.6));

/* Get and set how the layer should redraw when resizing and redisplaying. Prior to 10.8, the default value was always set to NSViewLayerContentsRedrawDuringViewResize when an AppKit managed layer was created. In 10.8 and higher, the value is initialized to the appropriate thing for each individual AppKit view. Generally, the default value is NSViewLayerContentsRedrawOnSetNeedsDisplay if the view responds YES to -wantsUpdateLayer. Otherwise, the value is usually NSViewLayerContentsRedrawDuringViewResize, indicating that the view needs to redraw on each step of an animation via a setFrame: change. On 10.8, the value is not encoded by the view.
*/
@property NSViewLayerContentsRedrawPolicy layerContentsRedrawPolicy API_AVAILABLE(macos(10.6));

@property NSViewLayerContentsPlacement layerContentsPlacement API_AVAILABLE(macos(10.6));

/* Indicates if this view should be a "Layer Backed View". When layer backed, all subviews will subsequently also have a layer set on them (however, wantsLayer will only be YES on views that have had it explicitly set). Contents for a layer are specified in one of two ways: if -wantsUpdateLayer returns YES, then one can directly update the layer's contents (or other properties) in -updateLayer. If -wantsUpdateLayer returns NO, then the layer's contents is filled with whatever is drawn by -drawRect:
 */
@property BOOL wantsLayer API_AVAILABLE(macos(10.5));

/* Get and set the CALayer for this view. The layer is not encoded by the view.
 */
@property (nullable, strong) CALayer *layer API_AVAILABLE(macos(10.5));

/* Layer Backed Views: Return YES if this view supports directly setting the layer properties (such as the contents and backgroundColor) as opposed to filling in the contents with a drawRect: implementation. Most AppKit controls return YES if there is no subclassing involved that would alter the drawing appearance. It will return NO for views that do have subclassing that AppKit does not know about (such as, overriding drawRect:, or other drawing methods).
 */
@property (readonly) BOOL wantsUpdateLayer API_AVAILABLE(macos(10.8));

/* Layer Backed Views: If the view responds YES to wantsUpdateLayer, then updateLayer will be called as opposed to drawRect:. This method should be used for better performance; it is faster to directly set the layer.contents with a shared image and inform it how to stretch with the layer.contentsCenter property instead of drawing into a context with drawRect:. In general, one should also set the layerContentsRedrawPolicy to an appropriate value in the init method (frequently NSViewLayerContentsRedrawOnSetNeedsDisplay is desired). To signal a refresh of the layer contents, one will then call [view setNeedsDisplay:YES], and -updateLayer will be lazily called when the layer needs its contents. One should not alter geometry or add/remove subviews (or layers) during this method. To add subviews (or layers) use -layout. -layout will still be called even if autolayout is not enabled, and wantsUpdateLayer returns YES.
 */
- (void)updateLayer API_AVAILABLE(macos(10.8));

/* When canDrawSubviewsIntoLayer is set to YES, and the view is layer-backed (either explicitly with -wantsLayer=YES, or by having a parent view that is layer-backed), the layer will draw all subviews into this view's layer, and each subview will not get an individual layer (the exception to this is a subview which has wantsLayer explicitly set to YES). This is useful for layer-backing a complex set of views that can not be refactored to take advantage of proper resizing and -wantsUpdateLayer==YES. If canDrawSubviewsIntoLayer is YES, the value returned from wantsUpdateLayer will be ignored, and the layer will always have drawRect: invoked to get the layer's contents. The default value is NO. NOTE: These methods should NOT be overridden, and the setter should always be used.
 */
@property BOOL canDrawSubviewsIntoLayer API_AVAILABLE(macos(10.9));

- (void)layoutSubtreeIfNeeded API_AVAILABLE(macos(10.7));
- (void)layout API_AVAILABLE(macos(10.7));
@property BOOL needsLayout API_AVAILABLE(macos(10.7));

@property CGFloat alphaValue API_AVAILABLE(macos(10.5));

/* If you have set a custom layer on your view, and it (or one of its sublayers) uses CIFilters, you should set this. You do not need to set this if you are using the backgroundFilters, compositingFilter, or contentFilters properties below. See the release notes for more information.
 */
@property BOOL layerUsesCoreImageFilters API_AVAILABLE(macos(10.9));

@property (copy) NSArray<__kindof CIFilter *> *backgroundFilters API_AVAILABLE(macos(10.5));

@property (nullable, strong) CIFilter *compositingFilter API_AVAILABLE(macos(10.5));

@property (copy) NSArray<__kindof CIFilter *> *contentFilters API_AVAILABLE(macos(10.5));

@property (nullable, copy) NSShadow *shadow API_AVAILABLE(macos(10.5));

/* The following methods are meant to be invoked, and probably don't need to be overridden
*/
- (void)addTrackingArea:(NSTrackingArea *)trackingArea API_AVAILABLE(macos(10.5));
- (void)removeTrackingArea:(NSTrackingArea *)trackingArea API_AVAILABLE(macos(10.5));
@property (readonly, copy) NSArray<NSTrackingArea *> *trackingAreas API_AVAILABLE(macos(10.5));

/* updateTrackingAreas should be overridden to remove out of date tracking areas and add recomputed tracking areas, and should call super.
*/
- (void)updateTrackingAreas API_AVAILABLE(macos(10.5));

@property BOOL postsBoundsChangedNotifications;

@property (nullable, readonly, strong) NSScrollView *enclosingScrollView;

- (nullable NSMenu *)menuForEvent:(NSEvent *)event;
@property (class, readonly, nullable, strong) NSMenu *defaultMenu;

/*!
 * A contextual menu is being opened from the receiving view.
 * The view should update any visual state in response — such as making a selection.
 * \param menu The contextual menu that is being opened on the view
 * \param event The event that caused the menu to open.
 */
- (void)willOpenMenu:(NSMenu *)menu withEvent:(NSEvent *)event API_AVAILABLE(macos(10.11));

/*!
 * A contextual menu shown from the receiving view has been closed. This is only called if the menu had been opened and the view previously received \c -willOpenMenu:withEvent:.
 * The view should update any visual state in response — such as removing a temporary selection.
 * \param menu The contextual menu that was open on the view
 * \param event The event that caused the menu to close. This may be nil if there is no specific event that triggered the closing.
 */
- (void)didCloseMenu:(NSMenu *)menu withEvent:(nullable NSEvent *)event API_AVAILABLE(macos(10.11));

@property (nullable, copy) NSString *toolTip;
- (NSToolTipTag)addToolTipRect:(NSRect)rect owner:(id)owner userData:(nullable void *)data;
- (void)removeToolTip:(NSToolTipTag)tag;
- (void)removeAllToolTips;

/* Live resize support */
/* A view receives viewWillStartLiveResize before the frame is first changed for a live resize. It is important to call [super viewWillStartLiveResize] to allow AppKit to do any internal work it needs. */
- (void)viewWillStartLiveResize;

/* A view receives viewWillEndLiveResize after the frame is last changed for a live resize. It is important to call [super viewDidEndLiveResize].
 */
- (void)viewDidEndLiveResize;

/* inLiveResize can be called at any time to determine if the window is performing a live resize or not. Drawing optimizations can be done when the view is being live-resized.
 */
@property (readonly) BOOL inLiveResize;

/* A view that returns YES for -preservesContentDuringLiveResize is responsible for invalidating its own dirty rects during live resize 
 */
@property (readonly) BOOL preservesContentDuringLiveResize;

/* -rectPreservedDuringLiveResize indicates the rect the view previously occupied, in the current coordinate system of the view 
 */
@property (readonly) NSRect rectPreservedDuringLiveResize;

/* On return from -getRectsExposedDuringLiveResize, exposedRects indicates the parts of the view that are newly exposed (at most 4 rects).  *count indicates how many rects are in the exposedRects list */
- (void)getRectsExposedDuringLiveResize:(NSRect[_Nonnull 4])exposedRects count:(NSInteger *)count;

/* Text Input */
/* Returns NSTextInputContext object for the receiver. Returns nil if the receiver doesn't conform to NSTextInputClient protocol.
 */
@property (nullable, readonly, strong) NSTextInputContext *inputContext API_AVAILABLE(macos(10.6));

/* Return the complete rect of the most appropriate content grouping at the specified location. For example, if your content is divided into three columns, return the entire rect of the column that contains the location. NSScrollView will attempt to magnify such that the width fits inside the scroll view while remaining within the minMagnification, maxMagnification range.
 
 If your content layout is sub-divided further than one level deep (for example, two boxes that each contain multiple text boxes), then use the visibleRect parameter to determine when to provide the rect of a sub-grouping. Always return a rect for the appropriate grouping. If there is no deeper content grouping, return the rect for the deepest grouping. NSScrollView will determine when to pan, magnify in, and magnify out.
 
 Return NSZeroRect for the default behavior.
 */
- (NSRect)rectForSmartMagnificationAtPoint:(NSPoint)location inRect:(NSRect)visibleRect API_AVAILABLE(macos(10.8));

/* Get and set the user interface layout direction. By default, a basic NSView may not respect the User Interface Layout Direction that is uniquely set on it, and it is up to the developer and supporting subclasses to correctly implement a Right To Left layout implementation. The default value is set to [NSApp userInterfaceLayoutDirection]. 
 */
@property NSUserInterfaceLayoutDirection userInterfaceLayoutDirection API_AVAILABLE(macos(10.8));

/* The View Based NSTableView allows views to be reused. Sometimes it is necessary to prepare a view with some initial state before it is to be reused. This method can be overridden to allow a view to be prepared back to the default state. Override this method to do the preparation. By default, NSView will do some setup, such as setting the view to not be hidden and have a 1.0 alpha. It is important to call super to get this work done. This method was made public in 10.9, but exists back to 10.7.
 */
- (void)prepareForReuse API_AVAILABLE(macos(10.7));

/* A subclass of any NSScrollView, NSClipView or the NSScrollView's documentView can override this method to verify that its customizations are compatible with 10.9's responsive scrolling behavior. By default, AppKit assumes that a ScrollView is responsive scrolling compatible if it and its clipview and document view do not override certain methods such as -scrollWheel: (See documentation for the expanded list). However, these views may still opt into responsive scrolling if they work with the new scrolling behavior by overriding and returning YES for this method. Likewise, the same set of views may return NO to explicitly opt out of responsive scrolling.
*/
@property (class, readonly, getter=isCompatibleWithResponsiveScrolling) BOOL compatibleWithResponsiveScrolling API_AVAILABLE(macos(10.9));

/* Called by NSView with a 'rect' for a recommended area that should be fully rendered for overdraw. Override this method and bring in additional subviews and pre-cached content for the 'rect' in order to perform responsive scrolling. Calling super may be required for some subclasses (such as NSTableView and NSOutlineView), so in general, super should always be called. To suppress overdraw for a particular view (such as NSTableView), override this method and call [super prepareContentInRect:[self visibleRect]].
 */
- (void)prepareContentInRect:(NSRect)rect API_AVAILABLE(macos(10.9));

/* The preparedContentRect is the area of the NSView that has full content coverage. In general, this should be called with the area that is filled in fully with views.  Set this with a value equal to the visibleRect to have overdraw start from the visibleRect and automatically grow larger on idle, as is needed for optimal system performance. The result of preparedContentRect may include the visibleRect, and may not include it if the previously prepared area has been scrolled away.
 */
@property NSRect preparedContentRect API_AVAILABLE(macos(10.9));

/* allowsVibrancy is queried when a vibrant appearance is used on a view hierarchy. When allowsVibrancy returns YES, the view will have an appropriate measure taken to ensure it is vibrant on top of its given material.
 
 Specific subclasses, such as NSControl, will answer this question based on the artwork they draw for a given appearance.
 */
@property (readonly) BOOL allowsVibrancy API_AVAILABLE(macos(10.10));

/// Override point for reacting to the effective appearance of the receiver changing. At this point `effectiveAppearance` property reflects the new appearance.
- (void)viewDidChangeEffectiveAppearance API_AVAILABLE(macos(10.14));

@end

@protocol NSViewLayerContentScaleDelegate <NSObject>
@optional

/* This method can be implemented as an optional CALayer delegate method, for handling resolution changes.  When a window changes its backing resolution, AppKit attempts to automatically update the contentsScale and contents of all CALayers in the window to match the new resolution.  View backing layers are updated automatically.  Any layer whose "contents" property is set to an NSImage will also be updated automatically.  (Based on the NSImage's available representations, AppKit will select an appropriate bitmapped representation, or rasterize a resolution-independent representation at the appropriate scale factor.)  For all other layers, AppKit will check whether the layer has a delegate that implements this method.  If so, AppKit will send this message to the layer's delegate to ask whether it should automatically update the contentsScale for that layer to match the backingScaleFactor of the window.  If you return YES for a given layer, AppKit will set the layer's contentsScale as proposed, and you must ensure that the layer's contents and other properties are configured appropriately for that new contentsScale.  (If you expressed the layer's "contents" as a CGImage, you may need to provide a different CGImage that's appropriate for the new contentsScale.)  Note that this method is only invoked when a window's backingScaleFactor changes.  You are responsible for setting the initial contentsScale of your layers.
*/
- (BOOL)layer:(CALayer *)layer shouldInheritContentsScale:(CGFloat)newScale fromWindow:(NSWindow *)window API_AVAILABLE(macos(10.7)); // added in 10.7.3

@end

#if __swift__ < 40200
@interface NSObject (NSLayerDelegateContentsScaleUpdating)
- (BOOL)layer:(CALayer *)layer shouldInheritContentsScale:(CGFloat)newScale fromWindow:(NSWindow *)window API_DEPRECATED("This is now a method of the NSViewLayerContentScaleDelegate protocol.", macos(10.7, 11.0)); // added in 10.7.3
@end
#endif

@protocol NSViewToolTipOwner <NSObject>
- (NSString *)view:(NSView *)view stringForToolTip:(NSToolTipTag)tag point:(NSPoint)point userData:(nullable void *)data;
@end

#if __swift__ < 40200
@interface NSObject(NSToolTipOwner)
- (NSString *)view:(NSView *)view stringForToolTip:(NSToolTipTag)tag point:(NSPoint)point userData:(nullable void *)data API_DEPRECATED("This is now a method of the NSViewToolTipOwner protocol.", macos(10.0, 11.0));
@end
#endif

@interface NSView(NSKeyboardUI)
@property (nullable, assign) NSView *nextKeyView;
@property (nullable, readonly, assign) NSView *previousKeyView;
@property (nullable, readonly, assign) NSView *nextValidKeyView;
@property (nullable, readonly, assign) NSView *previousValidKeyView;

@property (readonly) BOOL canBecomeKeyView;

- (void)setKeyboardFocusRingNeedsDisplayInRect:(NSRect)rect;

@property NSFocusRingType focusRingType;
@property (class, readonly) NSFocusRingType defaultFocusRingType;

- (void)drawFocusRingMask API_AVAILABLE(macos(10.7));
@property (readonly) NSRect focusRingMaskBounds API_AVAILABLE(macos(10.7));
- (void)noteFocusRingMaskChanged API_AVAILABLE(macos(10.7));

@end

@interface NSView(NSPrinting)

/* EPS/PDF generation */
- (void)writeEPSInsideRect:(NSRect)rect toPasteboard:(NSPasteboard *)pasteboard;
- (NSData *)dataWithEPSInsideRect:(NSRect)rect;
- (void)writePDFInsideRect:(NSRect)rect toPasteboard:(NSPasteboard *)pasteboard;
- (NSData *)dataWithPDFInsideRect:(NSRect)rect;

- (void)print:(nullable id)sender;

/* Pagination */
- (BOOL)knowsPageRange:(NSRangePointer)range;
@property (readonly) CGFloat heightAdjustLimit;
@property (readonly) CGFloat widthAdjustLimit;
- (void)adjustPageWidthNew:(CGFloat *)newRight left:(CGFloat)oldLeft right:(CGFloat)oldRight limit:(CGFloat)rightLimit;
- (void)adjustPageHeightNew:(CGFloat *)newBottom top:(CGFloat)oldTop bottom:(CGFloat)oldBottom limit:(CGFloat)bottomLimit;
- (NSRect)rectForPage:(NSInteger)page;
- (NSPoint)locationOfPrintRect:(NSRect)rect;
- (void)drawPageBorderWithSize:(NSSize)borderSize;
@property (readonly, copy) NSAttributedString *pageHeader;
@property (readonly, copy) NSAttributedString *pageFooter;

/*** This method is obsolete.  It will never be invoked from within AppKit, and NSView's implementation of it does nothing. ***/
- (void)drawSheetBorderWithSize:(NSSize)borderSize API_DEPRECATED("This is never invoked and the NSView implementation does nothing", macos(10.0,10.14));

/* Printing */
/* Returns print job title. Default implementation first tries its window's NSDocument (displayName), then window's title */
@property (readonly, copy) NSString *printJobTitle;
- (void)beginDocument;
- (void)endDocument;

- (void)beginPageInRect:(NSRect)rect atPlacement:(NSPoint)location;
- (void)endPage;
@end


@interface NSView(NSDrag)
/* Starts a dragging session with a group of NSDraggingItems. The frame property of each NSDraggingItem must be in the view's coordinate system. The images may animate (flock) from thier initial layout into a system defined formation. Flocking or may not be done immediately. (see NSDraggingSession's flockOnBeginDrag. The visible area of the view is used as the clip rect.
*/
- (NSDraggingSession *)beginDraggingSessionWithItems:(NSArray<NSDraggingItem *> *)items event:(NSEvent *)event source:(id<NSDraggingSource>)source API_AVAILABLE(macos(10.7));

@property (readonly, copy) NSArray<NSPasteboardType> *registeredDraggedTypes;
- (void)registerForDraggedTypes:(NSArray<NSPasteboardType> *)newTypes;
- (void)unregisterDraggedTypes;
@end

/* Constants for options when entering and exiting full screen mode */
typedef NSString * NSViewFullScreenModeOptionKey NS_TYPED_ENUM;
APPKIT_EXTERN NSViewFullScreenModeOptionKey const NSFullScreenModeAllScreens API_AVAILABLE(macos(10.5));                      // NSNumber numberWithBool:YES/NO
APPKIT_EXTERN NSViewFullScreenModeOptionKey const NSFullScreenModeSetting API_AVAILABLE(macos(10.5));                         // NSDictionary (obtained from CGSDisplay based functions)
APPKIT_EXTERN NSViewFullScreenModeOptionKey const NSFullScreenModeWindowLevel API_AVAILABLE(macos(10.5));                     // NSNumber numberWithInt:windowLevel
APPKIT_EXTERN NSViewFullScreenModeOptionKey const NSFullScreenModeApplicationPresentationOptions API_AVAILABLE(macos(10.5));  // NSNumber numberWithUnsignedInteger:(NSApplicationPresentationOptions flags)

@interface NSView (NSFullScreenMode) 
- (BOOL)enterFullScreenMode:(NSScreen *)screen withOptions:(nullable NSDictionary<NSViewFullScreenModeOptionKey, id> *)options API_AVAILABLE(macos(10.5));
- (void)exitFullScreenModeWithOptions:(nullable NSDictionary<NSViewFullScreenModeOptionKey, id> *)options API_AVAILABLE(macos(10.5));
@property (getter=isInFullScreenMode, readonly) BOOL inFullScreenMode API_AVAILABLE(macos(10.5)); 
@end


/* NSDefinitionPresentationTypeKey is an optional key in 'options' that specifies the presentation type of the definition display.  The possible values are NSDefinitionPresentationTypeOverlay that produces a small overlay window at the string location, or NSDefinitionPresentationTypeDictionaryApplication that invokes 'Dictionary' application to display the definition.  Without this option, the definition will be shown in either of those presentation forms depending on the 'Contextual Menu:' setting in Dictionary application preferences.
 */
typedef NSString * NSDefinitionOptionKey NS_TYPED_ENUM;
APPKIT_EXTERN NSDefinitionOptionKey const NSDefinitionPresentationTypeKey API_AVAILABLE(macos(10.6));

typedef NSString * NSDefinitionPresentationType NS_TYPED_ENUM;
APPKIT_EXTERN NSDefinitionPresentationType const NSDefinitionPresentationTypeOverlay API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSDefinitionPresentationType const NSDefinitionPresentationTypeDictionaryApplication API_AVAILABLE(macos(10.6));

@interface NSView(NSDefinition)
/* Shows a window that displays the definition (or other subject depending on available dictionaries) of the specified attributed string.  This method can be used for implementing the same functionality as NSTextView's 'Look Up in Dictionary' contextual menu on a custom view.
 
 textBaselineOrigin specifies the baseline origin of attrString in the receiver view coordinate system.  If a small overlay window is selected as default presentation (see NSDefinitionPresentationTypeKey option for details), the overlay text would be rendered starting from the location.  Otherwise, 'Dictionary' application will be invoked to show the definition of the specified string.
 
 This method is equivalent to using showDefinitionForAttributedString:range:options:baselineOriginProvider: and passing attrString with the whole range, nil options, and an originProvider which returns textBaselineOrigin.
 */
- (void)showDefinitionForAttributedString:(nullable NSAttributedString *)attrString atPoint:(NSPoint)textBaselineOrigin API_AVAILABLE(macos(10.6));

/* Takes a whole attributed string with the target range (normally, this is the selected range), and shows a window displaying the definition of the specified range.  The caller can pass a zero-length range and the appropriate range will be auto-detected around the range's offset.  That's the recommended approach when there is no selection.
 
 This method also an 'options' dictionary containing options described below as key-value pairs (can be nil).
 
 Except when NSDefinitionPresentationTypeKey with NSDefinitionPresentationTypeDictionaryApplication is specified in options, the caller must supply an originProvider Block which returns the baseline origin of the first character at proposed adjustedRange in the receiver view coordinate system.
 
 If the receiver is an NSTextView, both attrString and originProvider may be nil, in which case the text view will automatically supply values suitable for displaying definitions for the specified range within its text content.  This method does not cause scrolling, so clients should perform any necessary scrolling before calling this method.
 */
- (void)showDefinitionForAttributedString:(nullable NSAttributedString *)attrString range:(NSRange)targetRange options:(nullable NSDictionary<NSDefinitionOptionKey, id> *)options baselineOriginProvider:(NSPoint (^ _Nullable)(NSRange adjustedRange))originProvider API_AVAILABLE(macos(10.6));

@end


@interface NSView(NSFindIndicator)

/* When this method returns YES, the receiver or one of its ancestors is being drawn for a find indicator, meaning the receiver should draw so its contents will be easily readable.
 */
@property (getter=isDrawingFindIndicator, readonly) BOOL drawingFindIndicator API_AVAILABLE(macos(10.7));

@end



@interface NSView (NSGestureRecognizer)
@property (copy) NSArray<__kindof NSGestureRecognizer *> *gestureRecognizers API_AVAILABLE(macos(10.10));
- (void)addGestureRecognizer:(NSGestureRecognizer *)gestureRecognizer API_AVAILABLE(macos(10.10));
- (void)removeGestureRecognizer:(NSGestureRecognizer *)gestureRecognizer API_AVAILABLE(macos(10.10));
@end

@interface NSView (NSTouchBar)
/* Defaults to NSTouchTypeDirect if linked on or after 10_12, 0 otherwise */
@property NSTouchTypeMask allowedTouchTypes API_AVAILABLE(macos(10.12.2));
@end

@interface NSView (NSSafeAreas)

/* Indicates the potentially-obscured distance from each edge of the view (e.g, not behind the window title bar, etc., if present). */
@property (readonly) NSEdgeInsets safeAreaInsets API_AVAILABLE(macos(11.0));

/* Configurable insets added to insets inherited from the superview (if any), and contributing to self.safeAreaInsets. */
@property NSEdgeInsets additionalSafeAreaInsets API_AVAILABLE(macos(11.0));

/* This layout guide reflects the safe area for this view (i.e., the frame minus safeAreaInsets), and provides a convenient way to arrange content relative to the safe area using auto layout. */
@property(readonly, strong) NSLayoutGuide *safeAreaLayoutGuide API_AVAILABLE(macos(11.0));

/* The safe area for this view expressed as a rectangle in the view's coordinate space. */
@property (readonly) NSRect safeAreaRect API_AVAILABLE(macos(11.0));

/* This layout guide provides the recommended amount of padding for content inside a view. */
@property (readonly, strong) NSLayoutGuide *layoutMarginsGuide API_AVAILABLE(macos(11.0));

@end

@interface NSView(NSDeprecated)

- (void)dragImage:(NSImage *)image at:(NSPoint)viewLocation offset:(NSSize)initialOffset event:(NSEvent *)event pasteboard:(NSPasteboard *)pboard source:(id)sourceObj slideBack:(BOOL)slideFlag API_DEPRECATED("Use -beginDraggingSessionWithItems:event:source: instead", macos(10.0,10.7));

- (BOOL)dragFile:(NSString *)filename fromRect:(NSRect)rect slideBack:(BOOL)flag event:(NSEvent *)event API_DEPRECATED("Use -beginDraggingSessionWithItems:event:source: instead", macos(10.0,10.13));
- (BOOL)dragPromisedFilesOfTypes:(NSArray<NSString *> *)typeArray fromRect:(NSRect)rect source:(id)sourceObject slideBack:(BOOL)flag event:(NSEvent *)event API_DEPRECATED("Use -beginDraggingSessionWithItems:event:source: with an NSFilePromiseProvider instead", macos(10.0,10.13));

- (NSPoint)convertPointToBase:(NSPoint)point API_DEPRECATED("", macos(10.5,10.7));
- (NSPoint)convertPointFromBase:(NSPoint)point API_DEPRECATED("", macos(10.5,10.7));
- (NSSize)convertSizeToBase:(NSSize)size API_DEPRECATED("", macos(10.5,10.7));
- (NSSize)convertSizeFromBase:(NSSize)size API_DEPRECATED("", macos(10.5,10.7));
- (NSRect)convertRectToBase:(NSRect)rect API_DEPRECATED("", macos(10.5,10.7));
- (NSRect)convertRectFromBase:(NSRect)rect API_DEPRECATED("", macos(10.5,10.7));

- (BOOL)performMnemonic:(NSString *)string API_DEPRECATED("This has always returned NO and had no effect on macOS", macos(10.0,10.8));

/* shouldDrawColor is no longer used by AppKit.
 */
- (BOOL)shouldDrawColor API_DEPRECATED("This method no longer does anything", macos(10.0,10.10));

/* The gState class of methods are deprecated and in many cases did not do anything, or not what one would expect.
 */
- (NSInteger)gState API_DEPRECATED("", macos(10.0,10.10));
- (void)allocateGState API_DEPRECATED("", macos(10.0,10.10));
- (oneway void)releaseGState API_DEPRECATED("", macos(10.0,10.10));
- (void)setUpGState API_DEPRECATED("", macos(10.0,10.10));
- (void)renewGState API_DEPRECATED("", macos(10.0,10.10));
                                      
@end


/* Notifications */

/* Sent when the frame changes for a view. This is only sent if postsFrameChangedNotifications is set to YES.
 */
APPKIT_EXTERN NSNotificationName NSViewFrameDidChangeNotification;
APPKIT_EXTERN NSNotificationName NSViewFocusDidChangeNotification API_DEPRECATED("", macos(10.0,10.4));

/* This notification is sent whenever the views bounds change and the frame does not.  That is, it is sent whenever the view's bounds are translated, scaled or rotated, but NOT when the bounds change as a result of, for example, setFrameSize:.
 */
APPKIT_EXTERN NSNotificationName NSViewBoundsDidChangeNotification;


/* This notification is sent whenever an NSView that has an attached NSOpenGLContext changes size or changes screens (thus potentially changing graphics hardware drivers).
 */
APPKIT_EXTERN NSNotificationName NSViewGlobalFrameDidChangeNotification API_DEPRECATED("Use NSOpenGLView instead.", macos(10.0,10.14));

/* This notification is sent whenever tracking areas should be recalculated for the view.  It is sent after the view receives -updateTrackingAreas.
 */
APPKIT_EXTERN NSNotificationName NSViewDidUpdateTrackingAreasNotification API_AVAILABLE(macos(10.5));



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

