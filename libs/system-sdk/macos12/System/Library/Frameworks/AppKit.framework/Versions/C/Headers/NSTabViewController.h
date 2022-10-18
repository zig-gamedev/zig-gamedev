/*
    NSTabViewController.h
    Application Kit
    Copyright (c) 2014-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSViewController.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSTabView.h>
#import <AppKit/NSToolbar.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSInteger, NSTabViewControllerTabStyle) {
    /// Uses an NSSegmentedControl to show the UI for the tabs. The control is on the top of the view.
    NSTabViewControllerTabStyleSegmentedControlOnTop = 0,
    
    /// Uses an NSSegmentedControl to show the UI for the tabs. The control is on the bottom of the view.
    NSTabViewControllerTabStyleSegmentedControlOnBottom,
    
    /// Automatically pushes the tabs into the window's toolbar as toolbar items, if non-nil. This style will cause the TabViewController to set its containing window's toolbar to its own and become that toolbar's delegate. The toolbar items can be customized or supplemented by overriding the relevant NSToolbarDelegate methods.
    NSTabViewControllerTabStyleToolbar,
    
    /// NSTabViewController will not provide any of its own tab control UI. Separate UI, such as a NSSegmentedControl or NSPopupButton, can be easily bound to the TabViewController. Or \c tabView.tabViewType can be changed for the TabView itself to draw the UI.
    NSTabViewControllerTabStyleUnspecified = -1,
} API_AVAILABLE(macos(10.10));

/*!
 * NSTabViewController is a container view controller that displays a single child view controller at a time from its \c childViewControllers.
 * It provides standard tab-style UI for user selection of tabs, or allows custom UI to be easily created by providing targets for bindings.
 * ChildViewControllers’ views are lazily loaded; they are only loaded once their tab is selected and visible.
 * The NSTabViewController is set as the delegate of its managed NSTabView. Any overrides of NSTabViewDelegate methods must call super. Properties of the TabView such as the tabStyle can be directly manipulated, but calling methods that add and remove tabViewItems or changing the delegate is not allowed.
 * NSViewController's methods \c -addChildViewController:, \c -insertViewController:atIndex:, and \c -removeChildViewControllerAtIndex: can all be used as convience methods to add children; default TabViewItems will be appropriately created or destroyed. The default NSTabViewItem created with with +[NSTabViewItem tabViewItemForViewController:].
 */
API_AVAILABLE(macos(10.10))
@interface NSTabViewController : NSViewController <NSTabViewDelegate, NSToolbarDelegate>

/// The style that this NSTabViewController displays its UI as. Defaults to \c NSTabViewControllerTabStyleSegmentedControlOnTop.
@property NSTabViewControllerTabStyle tabStyle;

/// Access to the tab view that the controller is controlling. To provide a custom NSTabView, assign the value anytime before \c self.viewLoaded is \c YES. Querying the value will create it on-demand, if needed. Check \c self.viewLoaded before querying the value to avoid prematurely creating the view. Note that the \c -tabView may not be equal to the \c viewController.view. Properties such as the tabStyle can be directly manipulated, but calling methods that add and remove tabViewItems or changing the delegate is not allowed. The NSTabViewController will be made the delegate of the NSTabView. Internally, the NSTabView is always used to switch between displayed childViewControllers, regardless of the style displayed.
@property (strong) NSTabView *tabView;

/// This defines how NSTabViewController transitions from one view to another. Transitions go through [self transitionFromViewController:toViewController:options:completionHandler:]. The default value is \c NSViewControllerTransitionCrossfade|NSViewControllerTransitionAllowUserInteraction.
@property NSViewControllerTransitionOptions transitionOptions;

/// If YES and the receiving NSTabViewController has a nil title, \c -title will return its selected child ViewController's title. If NO, it will continue to return nil. The default value is \c YES.
@property BOOL canPropagateSelectedChildViewControllerTitle;

/// The array of tab view items that correspond to the current child view controllers. After a child view controller is added to the receiving TabViewController, a NSTabViewItem with the default values will be created for it. Once the child is removed, its corresponding tabViewItem will be removed from the tabViewItems array.
@property (copy) NSArray<__kindof NSTabViewItem *> *tabViewItems;

/// Read and write the current selected TabViewItem that is being shown. This value is KVC compliant and can be the target of a binding. For instance, a NSSegmentedControl's selection can be bound to this value with: \code [segmentedControl bind:NSSelectedIndexBinding toObject:tabViewController withKeyPath:@“selectedTabViewItemIndex" options:nil];
@property NSInteger selectedTabViewItemIndex;

/*!
 * Adds a TabViewItem to the end of the TabViewController. The tabViewItem’s viewController’s view will only be loaded once its tab is selected.
 * \param tabViewItem The TabViewItem to add. It must have a \c viewController set by the time it is added or an exception will be thrown. An exception will also be thrown if tabViewItem is nil.
 */
- (void)addTabViewItem:(NSTabViewItem *)tabViewItem;

/*!
 * Adds a TabViewItem to a given index in the TabViewController. The tabViewItem’s viewController’s view will only be loaded once its tab is selected. \c -selectedTabViewItemIndex will be adjusted if the insertion index is before it. Subclasses must call through \c -insertTabViewItem:atIndex: to add a TabViewItem.
 * \param tabViewItem The TabViewItem to add. It must have a \c viewController set by the time it is added or an exception will be thrown. An exception will also be thrown if tabViewItem is nil.
 * \param index The index to add the TabViewItem at. Will throw an exception if \c index < 0 or \c index > \c tabViewItems.count
 */
- (void)insertTabViewItem:(NSTabViewItem *)tabViewItem atIndex:(NSInteger)index;

/*!
 * Removes a TabViewItem from the receiver. If the removed tabViewItem currently selected, the next (or previous, if there is no next) tabViewItem will become selected. If this is the only tabViewItem in the TabViewController, the selectedTabViewItemIndex will become \c -1. Subclasses must call through \c -removeTabViewItem: to remove a TabViewItem.
 * \param tabViewItem The TabViewItem to remove. An exception will be thrown if \c tabViewItem is not in the NSTabViewController or if it is nil.
 */
- (void)removeTabViewItem:(NSTabViewItem *)tabViewItem;

/*!
 * Convenience method for getting the associated tab view item for a particular childViewController.
 * \param viewController The ViewController to look up.
 * \return The corresponding TabViewItem. Returns nil if \c viewController is not a child of the TabViewController.
 */
- (nullable NSTabViewItem *)tabViewItemForViewController:(NSViewController *)viewController;


/* Overridden methods from NSViewController. These require a call to super if overriden by a subclass. Override this to set the tabView before calling super. */
- (void)viewDidLoad NS_REQUIRES_SUPER;

/* Implemented methods from NSTabViewDelegate. These require a call to super if overriden by a subclass. */
- (void)tabView:(NSTabView *)tabView willSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem NS_REQUIRES_SUPER;
- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem NS_REQUIRES_SUPER;
- (BOOL)tabView:(NSTabView *)tabView shouldSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem NS_REQUIRES_SUPER;

/* Implemented methods from NSToolbarDelegate. These require a call to super if overriden by a subclass. */
/* NSTabViewController will create a toolbar item for each of its tabViewItems. The toolbar item's identifier will be the same as the tab view item's; similarly, the toolbar item's label, image, and toolTip are bound to those of the corresponding tab view item. */
- (nullable NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag NS_REQUIRES_SUPER;

/* NSTabViewController returns the identifiers for the toolbar items corresponding to its tabViewItems. Overriding these allows additional items to be added to the toolbar. For example, adding NSToolbarFlexibleSpaceItemIdentifier to the beginning and end will center the TabViewController's toolbar items. If a subclass adds custom identifiers, -toolbar:itemForItemIdentifier:willBeInsertedIntoToolbar: must be overridden to return a toolbar item for that identifer. */
- (NSArray<NSToolbarItemIdentifier> *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar NS_REQUIRES_SUPER;
- (NSArray<NSToolbarItemIdentifier> *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar NS_REQUIRES_SUPER;

/* NSTabViewController returns all toolbar item identifiers corresponding to its tabViewItems. */
- (NSArray<NSToolbarItemIdentifier> *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar NS_REQUIRES_SUPER;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
