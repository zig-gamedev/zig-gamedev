/*
    NSSplitViewItem.h
    Application Kit
    Copyright (c) 2014-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSLayoutConstraint.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSAnimation.h>
#import <AppKit/NSWindow.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSViewController;

typedef NS_ENUM(NSInteger, NSSplitViewItemBehavior) {
    NSSplitViewItemBehaviorDefault,
    NSSplitViewItemBehaviorSidebar,
    NSSplitViewItemBehaviorContentList
} API_AVAILABLE(macos(10.11));

typedef NS_ENUM(NSInteger, NSSplitViewItemCollapseBehavior) {
    /// The item uses the default collapsing behavior for its set `behavior`. The default may change over time.
    NSSplitViewItemCollapseBehaviorDefault,
    /// The item prefers to keep the other panes at their current size and position on screen, potentially growing or shrinking the window in the direction to best preserve that. But it will break that preference in order to keep the window fully on screen or when in full screen.
    NSSplitViewItemCollapseBehaviorPreferResizingSplitViewWithFixedSiblings,
    /// The item prefers to resize the other split panes. This will be broken when uncollapsing if the item can't fully uncollapse before hitting the minimum size of the other panes or the window.
    NSSplitViewItemCollapseBehaviorPreferResizingSiblingsWithFixedSplitView,
    /// The item will collapse/uncollapse purely from a constraint animation, with a constraint priority of the item’s `holdingPriority`. This could result in a partial internal content resize and window resize, and has no implications for keeping the window on screen. External constraints can be used to tweak exactly how the animation affects item, sibling, and window size and positions.
    NSSplitViewItemCollapseBehaviorUseConstraints
} API_AVAILABLE(macos(10.11));


/// This constant can be used with any sizing related \c NSSplitViewItem properties to unset their values.
APPKIT_EXTERN const CGFloat NSSplitViewItemUnspecifiedDimension API_AVAILABLE(macos(10.11));

/*!
 * NSSplitViewItem implements the items used in an NSSplitViewController.
 * The item describes a child ViewController's state in a SplitViewController, e.g. its collapsibility, holding priority and other metrics, and collapsed state.
 */
API_AVAILABLE(macos(10.10))
@interface NSSplitViewItem : NSObject <NSAnimatablePropertyContainer, NSCoding>

/*!
 * Creates an autoreleased SplitViewItem that represents the provided ViewController. All other properties are left at their default.
 * \param viewController The view controller used to set the viewController property
 * \return An autoreleased SplitViewItem.
 */
+ (instancetype)splitViewItemWithViewController:(NSViewController *)viewController;

/*!
 * Creates a split view item representing a sidebar for the provided ViewController.
 * Sidebars have standard system behavior, specifically:
 *  - Translucent material background
 *  - The ability to collapse/uncollapse on split view size changes
 *  - The ability to overlay at small split view sizes when in fullscreen
 *  - canCollapse is set to YES
 *  - minimumThickness and maximumThickness are set to the standard minimum and maximum sidebar size
 *  - preferredThicknessFraction is set to the standard fraction for sidebars (0.15)
 *  - springLoaded is set to YES
 * \param viewController The view controller used to set the viewController property
 * \return An autoreleased SplitViewItem that acts as a sidebar.
 */
+ (instancetype)sidebarWithViewController:(NSViewController *)viewController API_AVAILABLE(macos(10.11));

/*!
 * Creates a split view item representing a content list for the provided ViewController, akin to Mail's message list, Note's note list.
 * Content lists have system standard defaults, specifically:
 *  - minimumThickness and maximumThickness are set to the system standard for content lists
 *  - automaticMaximumThickness is set to the system standard for content lists
 *  - preferredThicknessFraction is set to the standard fraction for content lists (0.28 when a neighbor sidebar is visible, 0.33 if not)
 * \param viewController The view controller used to set the viewController property
 * \return An autoreleased SplitViewItem that acts as a content list.
 */
+ (instancetype)contentListWithViewController:(NSViewController *)viewController API_AVAILABLE(macos(10.11));

/// The standard behavior type of the receiver. See initializers for descriptions of each behavior.
@property (readonly) NSSplitViewItemBehavior behavior API_AVAILABLE(macos(10.11));

/// The view controller represented by the SplitViewItem. An exception will be thrown if a new viewController is set while the receiving SplitViewItem is added to a SplitViewController.
@property (strong) NSViewController *viewController;

/// Whether or not the child ViewController corresponding to the SplitViewItem is collapsed in the SplitViewController. The default is \c NO. This can be set with the animator proxy to animate the collapse or uncollapse. The exact animation used can be customized by setting it in the -animations dictionary with a key of "collapsed". If this is set to YES before it is added to the SplitViewController, it will be initially collapsed and the SplitViewController will not cause the view to be loaded until it is uncollapsed. This is KVC/KVO compliant and will be updated if the value changes from user interaction.
@property (getter=isCollapsed) BOOL collapsed;

/// Whether or not the child view controller is collapsible from user interaction - whether by dragging or double clicking a divider. The default is \c NO.
@property BOOL canCollapse;

/// The resize behavior when the receiver toggles its `collapsed` state programmatically, both animatedly and not. Defaults to `.Default`.
@property NSSplitViewItemCollapseBehavior collapseBehavior API_AVAILABLE(macos(10.11));

/// A convenience to set the minimum thickness of the split view item -- width for "vertical" split views, height otherwise. If NSSplitViewItemUnspecifiedDimension, no minimum size is enforced by the SplitViewItem, although constraints in the contained view hierarchy might have constraints specify some minimum size on their own. Defaults to NSSplitViewItemUnspecifiedDimension.
@property CGFloat minimumThickness API_AVAILABLE(macos(10.11));

/// A convenience to set the maximum thickness of the split view item -- width for "vertical" split views, height otherwise. If NSSplitViewItemUnspecifiedDimension, no maximum size is enforced by the SplitViewItem, although constraints in the contained view hierarchy might have constraints specify some maximum size on their own. Defaults to NSSplitViewItemUnspecifiedDimension.
@property CGFloat maximumThickness API_AVAILABLE(macos(10.11));

/// The percentage of the contained NSSplitView that the NSSplitViewItem prefers to encompass. This is used when double-clicking on a neighbor divider to return to that standard ratio. As well as after entering fullscreen to determine the initial size of the receiver. Defaults to NSSplitViewItemUnspecifiedDimension, which means no resize will occur on double-clicks, and the absolute size is preserved when entering fullscreen.
@property CGFloat preferredThicknessFraction API_AVAILABLE(macos(10.11));

#if !TARGET_OS_IPHONE
/// Sets the priority under which a SplitViewItem will hold its width (for a vertical split view) or height (for a horizontal split view). The view with the lowest priority will be the first to take on additional width if the split view grows or shrinks. The default is \c NSLayoutPriorityDefaultLow.
@property NSLayoutPriority holdingPriority;
#endif

/// The maximum thickness of the split view item when resizing due to automatic sizing, such as entering fullscreen with a set preferredThicknessFraction or proportional sizing. The user can still resize up to the absolute maximum size by dragging the divider or otherwise. If NSSplitViewItemUnspecifiedDimension, no automatic maximum is enforced. Defaults to NSSplitViewItemUnspecifiedDimension.
@property CGFloat automaticMaximumThickness API_AVAILABLE(macos(10.11));

/// If YES, the split view item can be temporarily uncollapsed during a drag by hovering or deep clicking on its neighboring divider. Defaults to NO.
@property (getter=isSpringLoaded) BOOL springLoaded API_AVAILABLE(macos(10.11));

/// Whether or not a sidebar is allowed to be full height in the window when the `NSFullSizeContentViewWindowMask` style mask is also set. Only applies to NSSplitViewItemBehaviorSidebar. Defaults to YES
@property BOOL allowsFullHeightLayout API_AVAILABLE(macos(11.0));

/// Specifies a preference for the style of separator displayed between the titlebar and the content of the split view item.
///
/// For this value to be applicable, the item's view must be associated with its own titlebar section (see `NSTrackingSeparatorToolbarItem` for more info).
/// The default value is NSTitlebarSeparatorStyleAutomatic. This value is subject to the containing window's preference and can be overridden.
@property NSTitlebarSeparatorStyle titlebarSeparatorStyle API_AVAILABLE(macos(11.0));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
