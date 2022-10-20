/*
 NSMenuToolbarItem.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/NSToolbarItem.h>

@class NSMenu;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.15), ios(13.0))
@interface NSMenuToolbarItem : NSToolbarItem

/*
 The menu presented to the user from the item.
 If the item has an action, an arrow will be shown to the user next to the button to access the menu.
 This will also be shown as the menuFormRepresentation unless otherwise set.
 */
@property (strong) NSMenu *menu API_UNAVAILABLE(ios);

/*
 When YES, an indicator will be visible to the user as part of the control indicating that additional functionality is available.
 Defaults to YES.
 */
@property BOOL showsIndicator;

@end

NS_ASSUME_NONNULL_END
