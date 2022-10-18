/*
	NSToolbarItemGroup.h
	Application Kit
	Copyright (c) 2000-2021, Apple Inc.
	All rights reserved.
*/

#import <TargetConditionals.h>
#import <AppKit/NSToolbarItem.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
@class UIImage;
#endif

/* NSToolbarItemGroup is a subclass of NSToolbarItem which can be used to create sets of NSToolbarItems that are always attached to one another and that are added, removed, or reordered as a single unit.  Properties that get set on the parent toolbar item, such as label or view, apply to the entire item.  Otherwise, the individual properties are displayed adjacent to one another.  */

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSToolbarItemGroupSelectionMode) {
    NSToolbarItemGroupSelectionModeSelectOne = 0, // Only one option in the control can be selected at a time.
    NSToolbarItemGroupSelectionModeSelectAny = 1, // One or more options in the control can be selected at a time.
    NSToolbarItemGroupSelectionModeMomentary = 2  // An option is only highlighted when the user is engaging within the bounds of that option.
} API_AVAILABLE(macos(10.15), ios(13.0));

typedef NS_ENUM(NSInteger, NSToolbarItemGroupControlRepresentation) {
    NSToolbarItemGroupControlRepresentationAutomatic,   // The representation will be chosen based on available space
    NSToolbarItemGroupControlRepresentationExpanded,    // All functions of the control will be visible in a horizontal representation
    NSToolbarItemGroupControlRepresentationCollapsed   // All functions of the control will be presented through a menu
} API_AVAILABLE(macos(10.15), ios(13.0));

API_AVAILABLE(macos(10.5), ios(13.0))
@interface NSToolbarItemGroup : NSToolbarItem

/*
 Convenience constructors for creating segmented control based toolbar items with images or text.
 The item returned will have a custom view for representing the control and automatically create subitems for the group.
 The labels array, if not nil, will be used to provide individual labels under the item for each segment of the control.
 When space in the toolbar is tight, the control may switch to a smaller alternate representation as necessary to remain in the toolbar.
 */
+ (instancetype)groupWithItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier titles:(NSArray<NSString *> *)titles selectionMode:(NSToolbarItemGroupSelectionMode)selectionMode labels:(nullable NSArray<NSString *> *)labels target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.15));

#if !TARGET_OS_IPHONE
+ (instancetype)groupWithItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier images:(NSArray<NSImage *> *)images selectionMode:(NSToolbarItemGroupSelectionMode)selectionMode labels:(nullable NSArray<NSString *> *)labels target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.15));
#else
+ (instancetype)groupWithItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier images:(NSArray<UIImage *> *)images selectionMode:(NSToolbarItemGroupSelectionMode)selectionMode labels:(nullable NSArray<NSString *> *)labels target:(nullable id)target action:(nullable SEL)action;
#endif

/* Set or get the array of subitems for the toolbar item.  By default, a NSToolbarItemGroup has an empty array of subitems.  You should call this to set the subitems before returning the item to the toolbar.  NSToolbarItemGroups may not contain other NSToolbarItemGroups as subitems.
*/
@property (copy) NSArray<__kindof NSToolbarItem *> *subitems;

/*
 The style in which this item will be represented to the user.
 Defaults to NSToolbarItemGroupControlRepresentationAutomatic.
 */
@property NSToolbarItemGroupControlRepresentation controlRepresentation API_AVAILABLE(macos(10.15));

/*
 Get and set how selection is handled by the control.
 */
@property NSToolbarItemGroupSelectionMode selectionMode API_AVAILABLE(macos(10.15));

/*
 The most recently selected item of the group, or -1 if nothing is selected.
 */
@property NSInteger selectedIndex API_AVAILABLE(macos(10.15));

/*
 Get and set selection of individual subitems of the group item.
 */
- (void)setSelected:(BOOL)selected atIndex:(NSInteger)index API_AVAILABLE(macos(10.15));
- (BOOL)isSelectedAtIndex:(NSInteger)index API_AVAILABLE(macos(10.15));

@end

NS_ASSUME_NONNULL_END

