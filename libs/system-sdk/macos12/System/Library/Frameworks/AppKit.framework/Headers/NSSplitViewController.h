/*
    NSSplitViewController.h
    Application Kit
    Copyright (c) 2014-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSViewController.h>
#import <AppKit/NSSplitViewItem.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSSplitView.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/// This constant can be used with any sizing related \c NSSplitViewController properties to get the default system behavior.
APPKIT_EXTERN const CGFloat NSSplitViewControllerAutomaticDimension API_AVAILABLE(macos(10.11));

/*!
 * NSSplitViewController is a container view controller that manages side-by-side (horizontal or vertical) children view controllers.
 * Views are lazily loaded. For instance, adding a collapsed SplitViewItem will not load the associated ViewController's view until it is uncollapsed.
 * The NSSplitViewController is set as the delegate of its managed NSSplitView. Any overrides of NSSplitViewDelegate methods must call super.
 * Only the \c -vertical, \c -autosaveName, and divider properties should be manipulated on the managed NSSplitView. Changing other properties (such as delegate, manipulating subviews, holding priorities) will cause an exception to be thrown.
 * Autolayout must be used with NSSplitViewController to properly control the layout of the child views and the animations of collapses and reveals. e.g., Constraints can be used to setup whether a window should grow/shrink or stay the same size when showing and hiding a sidebar.
 * NSViewController's methods \c -addChildViewController:, \c -insertViewController:atIndex:, and \c -removeChildViewControllerAtIndex: can all be used as convience methods to add children; default SplitViewItems will be appropriately created or destroyed.
 */
API_AVAILABLE(macos(10.10))
@interface NSSplitViewController : NSViewController <NSSplitViewDelegate, NSUserInterfaceValidations>

/// The split view managed by the SplitViewController. This can be used to customize view properties such as the dividerStyle, vertical, and autosaveName. It is not guaranteed to be the same view as the receivers 'view' property. The default created splitView is vertical with a dividerStyle of \c NSSplitViewDividerStyleThin. To provide a custom NSSplitView, set the splitView property anytime before self.viewLoaded is YES.
@property (strong) NSSplitView *splitView;

/// The array of SplitViewItems that correspond to the current child view controllers. After a child view controller is added to the receiving splitViewController, a NSSplitViewItem with the default values will be created for it. Once the child is removed, its corresponding splitViewItem will be removed from the splitViewItems array. Setting this will call through to \c -insertSplitViewItem:atIndex and \c -removeSplitViewItem: for items that are new or need removal.
@property (copy) NSArray<__kindof NSSplitViewItem *> *splitViewItems;

/*!
 * Adds a SplitViewItem to the end of the SplitViewController. If the receiver's view is loaded and the SplitViewItem is not collapsed, the SplitViewItem's viewController's view will be loaded and added to the splitView. This calls through to -insertSplitViewItem:atIndex:.
 * \param splitViewItem The SplitViewItem to add. It must have a viewController set by the time it is added or an exception will be thrown. An exception will also be thrown if splitViewItem is nil.
 */
- (void)addSplitViewItem:(NSSplitViewItem *)splitViewItem;

/*!
 * Adds a SplitViewItem to a given index in the SplitViewController. If the receiver's view is loaded and the SplitViewItem is not collapsed, the SplitViewItem's viewController's view will be loaded and added to the \c splitView. Subclasses must call through \c -insertSplitViewItem:atIndex: to add a SplitViewItem.
 * \param splitViewItem The SplitViewItem to add. It must have a \c viewController set by the time it is added or an exception will be thrown. An exception will also be thrown if splitViewItem is nil.
 * \param index The index to add the SplitViewItem at. Will throw an exception if \c index < 0 or \c index > \c splitViewItems.count
 */
- (void)insertSplitViewItem:(NSSplitViewItem *)splitViewItem atIndex:(NSInteger)index;

/*!
 * Removes a SplitViewItem from the receiver. The layout of the \c splitView will be adjusted for its removal. Subclasses must call through \c -removeSplitViewItem: to remove a SplitViewItem.
 * \param splitViewItem The SplitViewItem to remove. An exception will be thrown if \c splitViewItem is not in the SplitViewController or if it is nil.
 */
- (void)removeSplitViewItem:(NSSplitViewItem *)splitViewItem;

/*!
 * Returns the corresponding SplitViewItem for a given child ViewController.
 * \param viewController The ViewController to look up.
 * \return The corresponding SplitViewItem. Returns nil if \c viewController is not a child of the SplitViewController.
 */
- (nullable NSSplitViewItem *)splitViewItemForViewController:(NSViewController *)viewController;

/*!
 * The minimum thickness in the primary axis of split view (width for "vertical", height otherwise) before sidebar items will automatically collapse. If reshown in fullscreen, they will overlay over the other split items.
 * Auto-collapsed sidebars will automatically uncollapse if the thickness is increased back to or past the minimum thickness.
 * Defaults to \c NSSplitViewControllerAutomaticDimension, which will use the effective minimum sizes of the split view item views as described by constraints in the window to determine the minimum size for inline sidebars. Once constraints establishing the minimum size can't be satisfied for all non-collapsed split panes, all sidebars will auto-collapse. When fullscreen, if a sidebar tries to uncollapse in this state, it will overlay.
 */
@property CGFloat minimumThicknessForInlineSidebars API_AVAILABLE(macos(10.11));

/// Validates items with an action of `toggleSidebar:` to reflect the status of the sidebar item contained within the receiver.
- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item API_AVAILABLE(macos(10.11));

/* Overridden methods from NSViewController. These require a call to super if overriden by a subclass */
- (void)viewDidLoad NS_REQUIRES_SUPER;

/* Implemented methods from NSSplitViewDelegate. These require a call to super if overriden by a subclass */
- (BOOL)splitView:(NSSplitView *)splitView canCollapseSubview:(NSView *)subview NS_REQUIRES_SUPER;
- (BOOL)splitView:(NSSplitView *)splitView shouldCollapseSubview:(NSView *)subview forDoubleClickOnDividerAtIndex:(NSInteger)dividerIndex NS_REQUIRES_SUPER API_DEPRECATED("NSSplitView no longer supports collapsing sections via double-click. This delegate method is never called, and NSSplitViewController's implementation always returns NO.", macos(10.5,10.15));
- (BOOL)splitView:(NSSplitView *)splitView shouldHideDividerAtIndex:(NSInteger)dividerIndex NS_REQUIRES_SUPER; // By default, NSSplitViewController hides the first and last dividers if their outer neighbor is collapsed
- (NSRect)splitView:(NSSplitView *)splitView effectiveRect:(NSRect)proposedEffectiveRect forDrawnRect:(NSRect)drawnRect ofDividerAtIndex:(NSInteger)dividerIndex NS_REQUIRES_SUPER;
- (NSRect)splitView:(NSSplitView *)splitView additionalEffectiveRectOfDividerAtIndex:(NSInteger)dividerIndex NS_REQUIRES_SUPER;
@end

@interface NSSplitViewController (NSSplitViewControllerToggleSidebarAction)

/// Animatedly collapses or uncollapses the first sidebar split view item in the receiver. Does nothing if the receiver does not contain any sidebars.
- (IBAction)toggleSidebar:(nullable id)sender API_AVAILABLE(macos(10.11));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
