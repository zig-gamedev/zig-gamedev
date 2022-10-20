/*
    NSTabViewItem.h
    Application Kit
    Copyright (c) 2000-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSTabView, NSView, NSViewController, NSImage;

typedef NS_ENUM(NSUInteger, NSTabState) {
    NSSelectedTab = 0,
    NSBackgroundTab = 1,
    NSPressedTab = 2
};

@interface NSTabViewItem : NSObject <NSCoding>

/*!
 * Creates an autoreleased TabViewItem that wraps the provided ViewController. The viewController is set as the tab view item’s \c -viewController property, which sets several of the tab view item’s other properties.
 * \param viewController The view controller to wrap, used to set the viewController property
 */
+ (instancetype)tabViewItemWithViewController:(NSViewController *)viewController API_AVAILABLE(macos(10.10));

/* By default, NSTabViewItem creates a basic NSView for you automatically.
 */
- (instancetype)initWithIdentifier:(nullable id)identifier; // identifier is retained

@property (strong, nullable) id identifier;
@property (copy) NSColor *color;
@property (copy) NSString *label;
/// Get and set the image for this tab view item. The image may only be used in certain tab view styles and options.  The default value is nil.
@property (nullable, strong) NSImage *image API_AVAILABLE(macos(10.10));
@property (nullable, strong) NSView *view;

/*! The view controller wrapped by the tab view item. This property must be set if the tab view item will be added to an NSTabViewController, but can also be used if the tab view item is added to an NSTabView. 
 * If this is set, the tab view item will forward \c -view calls onto the viewController. Setting a viewController will also set the following properties on the tab view item: \c -identifier from the address of the viewController, \c -label from the viewController's title, and \c -image based on the classname as the view controller.
 * An image named "ViewControllerClassName-TabViewItem" will be searched for first, followed by "ViewControllerClassName". It will search first using +[NSImage imageNamed:], then in \c viewController.nibBundle, and lastly in the bundle containing the view controller's class.
 * As defined by: -[NSImage imageNamed:imageName], -[viewController.nibBundle imageForResource:imageName], -[[NSBundle bundleForClass:[viewController class]] imageForResource:imageName]. One pass with imageName as [NSStringFromClass([viewController class]) stringByAppendingString:@"-TabViewItem"], followed by imageName as NSStringFromClass([viewController class]).
 */
@property (nullable, strong) NSViewController *viewController API_AVAILABLE(macos(10.10));

@property (readonly) NSTabState tabState;
@property (nullable, readonly) NSTabView *tabView;

@property (nullable, weak) NSView *initialFirstResponder;

/* The following methods provide support for tooltips on the tabs a tab view.  The default behavior is to use the label as a tooltip if the label is truncated due to insufficient space; this automatic tooltip is never returned by the toolTip method.  If you explicitly set a non-nil tooltip, this default behavior is suppressed, and the given tooltip will be used whether or not the label is truncated.
*/
@property (nullable, copy) NSString *toolTip API_AVAILABLE(macos(10.6));

/* Tab Drawing/Measuring */

/* This method draws the tab label assuming a local coordinate system whose label axis runs along the x axis.  Assume the label axis runs along the length of the label (perpendicular to the direction of the tab).  'labelRect' is the area in between the curved end caps.  'shouldTruncateLabel' is a hint that the label should be truncated.  ie. if shouldTruncateLabel is YES, then labelRect.size < ceil([sizeOfLabel:NO])
*/
- (void)drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)labelRect;

/* This method returns the minimum or nominal size of the tab label.  The width of the label is measured along the label axis, and height is measured in the direction parallel to the tab direction.  'computeMin' indicates whether you should return the minimum or nominal label size.  The returned value is used to compute the range of legal sizes for the tab label.
*/
- (NSSize)sizeOfLabel:(BOOL)computeMin;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

