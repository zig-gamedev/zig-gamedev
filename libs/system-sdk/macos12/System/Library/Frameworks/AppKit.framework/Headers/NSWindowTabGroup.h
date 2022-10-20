/*
    NSWindowTabGroup.h
    Application Kit
    Copyright (c) 2017-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/NSWindow.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/* NSWindowTabGroup represents a group of windows that are shown together in one tab group. See NSWindow's tabGroup property.
 */
API_AVAILABLE(macos(10.13))
@interface NSWindowTabGroup : NSObject

/* The identifier for this group; all windows in the group will have the same identifier.
 */
@property (readonly, copy) NSWindowTabbingIdentifier identifier;

/* Returns the entire group (stack) of windows that are all visually shown together in one virtual tabbed window and associated with this particular window tab group. Operations can then be done on each window, as necessary. For instance, iterating over each window in the group and calling performClose: will close the entire stack. The order of items in the array is the same order as the tabs visually shown (leading to trailing). This property is KVO compliant
 */
@property (readonly, copy) NSArray<NSWindow *> *windows;

/* Determines if the Tab Picker / Tab Overview UI is visible, and can be set to make it explicitly visible or hidden. KVO compliant and can be observed so that UI can be updated or disabled when the tab overview is visible.
 */
@property (getter=isOverviewVisible) BOOL overviewVisible;

/* Returns YES when the tab bar is visible.
 */
@property (readonly, getter=isTabBarVisible) BOOL tabBarVisible;

/* Returns the current window that is selected. This is KVO compliant. Assignments can only work for a window in the tab group, otherwise an exception will be thrown.
 */
@property (weak) NSWindow *selectedWindow;

/* Allows adding a new window to this tab group. Adding a window will first remove it from its existing tab window group.
 A window can implicitly be removed by ordering it out, or it can be explicitly be removed by calling removeWindow.
 */
- (void)addWindow:(NSWindow *)window;
- (void)insertWindow:(NSWindow *)window atIndex:(NSInteger)index;
- (void)removeWindow:(NSWindow *)window;

@end


API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
