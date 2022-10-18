/*
 NSMenu.h
 Application Kit
 Copyright (c) 1996-2021, Apple Inc.
 All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSMenuItem.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSEvent, NSView, NSFont;
@class NSMenu;
@protocol NSMenuDelegate;

@interface NSMenu : NSObject <NSCopying, NSCoding, NSUserInterfaceItemIdentification, NSAppearanceCustomization, NSAccessibilityElement, NSAccessibility>

/* Designated initializer.  If this menu is used as a submenu of an item in the application's main menu, then the title is what appears in the menu bar.  Otherwise, the title is ignored.  Do not pass nil (an exception will result), but you may pass an empty string.
 */
- (instancetype)initWithTitle:(NSString *)title NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Set and get the menu's title.  The titles of the submenus of the application's main menu items appear in the menu bar. */
@property (copy) NSString *title;

/* Pops up the receiver as a context menu.  The menu is positioned such that the top left corner lies at the given event's location, interpreted in the coordinate system of the given view.  If this would cause the menu to scroll offscreen, it will be moved up to be onscreen.  The event must not be nil.
 
 If a font is specified, then it will be applied to the menu.  In 10.6, the setFont: API below should be preferred.
*/
 
+ (void)popUpContextMenu:(NSMenu*)menu withEvent:(NSEvent*)event forView:(NSView*)view;
+ (void)popUpContextMenu:(NSMenu*)menu withEvent:(NSEvent*)event forView:(NSView*)view withFont:(nullable NSFont*)font;

/* Pops up the receiver as a popup menu. The top left or right corner of the given item (which must be present in the receiver) is positioned at the given location in the given view, interpreted in the view's own coordinate system.  If item is nil, the menu is positioned such that the top left or right of the menu content frame is at the given location.  If view is nil, the location is interpreted in the screen coordinate system. This allows you to pop up a menu disconnected from any window.  The method returns YES if menu tracking ended because an item was selected, and NO if menu tracking was cancelled for any reason.
 
   This method determines whether to use the top left or right corner by calling userInterfaceLayoutDirection on the view or its cell (if implemented).  If not implemented, or if the view is nil, it inspects the userInterfaceLayoutDirection of the NSApplication.
*/
- (BOOL)popUpMenuPositioningItem:(nullable NSMenuItem *)item atLocation:(NSPoint)location inView:(nullable NSView *)view API_AVAILABLE(macos(10.6));

/* Determines whether the menu bar is visible for this application.  Users cannot access the menu bar unless it is visible.  It is visible by default. */
+ (void)setMenuBarVisible:(BOOL)visible;
+ (BOOL)menuBarVisible;

/* Getter: Returns the menu containing the item that has the receiver as a submenu, or nil if this menu is not the submenu of an item in a menu. 
   Setter: If a menu item has the receiver as a submenu, then this method will be called when the menu containing that item changes.  You should never call this, but you may override it to take some action when the supermenu changes. */
@property (nullable, assign) NSMenu *supermenu;

/* Inserts a menu item at the given index, which must be at least zero and no more than the receiver's item count.  If newItem is nil, this raises an exception. */
- (void)insertItem:(NSMenuItem *)newItem atIndex:(NSInteger)index;

/* Appends an item to the end of the menu.  A nil item will raise an exception. */
- (void)addItem:(NSMenuItem *)newItem;

/* Inserts a new menu item with the given title, action, and key equivalent, at the given index.  The index must be at least zero and no more than the receiver's item count.  The title and key equivalent must not be nil (pass the empty string to indicate no key equivalent). This returns the new item.
 */
- (NSMenuItem *)insertItemWithTitle:(NSString *)string action:(nullable SEL)selector keyEquivalent:(NSString *)charCode atIndex:(NSInteger)index;

/* Appends a new menu item with the given properties to the end of the menu. */
- (NSMenuItem *)addItemWithTitle:(NSString *)string action:(nullable SEL)selector keyEquivalent:(NSString *)charCode;

/* Removes the item at the given index, which must be at least zero and less than the number of items.  All subsequent items will shift down one index. */
- (void)removeItemAtIndex:(NSInteger)index;

/* Removes the item from the menu.  If the item is nil, or is not present in the receiver, an exception will be raised. */
- (void)removeItem:(NSMenuItem *)item;

/* Same as [item setSubmenu:menu].  item may not be nil. */
- (void)setSubmenu:(nullable NSMenu *)menu forItem:(NSMenuItem *)item;

/* Removes all items.  This is more efficient than removing items one by one.  This does not post NSMenuDidRemoveItemNotification, for efficiency.
*/
- (void)removeAllItems API_AVAILABLE(macos(10.6));

/* Returns an array containing the receiver's menu items. This property is settable in macOS 10.14 and later. */
@property (copy) NSArray<NSMenuItem *> *itemArray;

/* Returns the number of menu items in the menu. */
@property (readonly) NSInteger numberOfItems;

/* Returns the item at the given index, which must be at least zero and less than the number of items. */
 - (nullable NSMenuItem *)itemAtIndex:(NSInteger)index;

/* Returns the index of the item in the menu, or -1 if the item is not present in the menu */
- (NSInteger)indexOfItem:(NSMenuItem *)item;

/* Returns the first item in the menu that matches the given property, or -1 if no item in the menu matches. */
- (NSInteger)indexOfItemWithTitle:(NSString *)title;
- (NSInteger)indexOfItemWithTag:(NSInteger)tag;
- (NSInteger)indexOfItemWithRepresentedObject:(nullable id)object;
- (NSInteger)indexOfItemWithSubmenu:(nullable NSMenu *)submenu;
- (NSInteger)indexOfItemWithTarget:(nullable id)target andAction:(nullable SEL)actionSelector;

/* Returns the first item in the menu with the given property, or nil if no item in the menu matches. */
- (nullable NSMenuItem *)itemWithTitle:(NSString *)title;
- (nullable NSMenuItem *)itemWithTag:(NSInteger)tag;

/* Set and get whether the menu autoenables items.  If a menu autoenables items, then calls to -[NSMenuItem setEnabled:] are ignored, and the enabled state is computed via the NSMenuValidation informal protocol below.  Autoenabling is on by default. */
@property BOOL autoenablesItems;
 
 /* If the receiver is set to autoenable items, then this triggers autovalidation of all menu items according to the NSMenuValidation informal protocol; otherwise this does nothing.  It is normally not necessary to call this; it will be called for you at the right time. */
- (void)update;
  
/* Attempts to perform the given key equivalent.  If the event is a key down event that matches the key equivalent of a menu item in the receiver or, recursively, any menu item in a submenu of the receiver, then this triggers that menu item's action (if the item is enabled) and returns YES.  Otherwise, this returns NO.
*/
- (BOOL)performKeyEquivalent:(NSEvent *)event;

/* This method is called when a menu item's enabled state, submenu, title, attributed title, image, key equivalent, key equivalent modifier mask, alternate status, indent, tooltip, view, or visibility (via isHidden) changes.  This method posts NSMenuDidChangeItemNotification.  Future properties will likely not call this method when they change, because posting a notification when a property changes is rather expensive.
 */
- (void)itemChanged:(NSMenuItem *)item;

/* Triggers the action for the item at the given index in the receiver.  This is useful as both an override point and something you may call directly.  As an override point, this is called when the user chooses the item during normal menu tracking; subclassers can override it to take some different action.  If you call it directly, it triggers the action for that item.  In SnowLeopard and later, calling it directly will also highlight the menu (if present in the main menu) and trigger accessibility notifications, including VoiceOver speaking the title.
*/
- (void)performActionForItemAtIndex:(NSInteger)index;

/* Set and get the delegate for the menu. The delegate is weakly referenced for zeroing-weak compatible objects on 10.9 and later. Otherwise the behavior of this property is 'assign'. See the NSMenuDelegate protocol for methods that the delegate may implement. */
@property (nullable, weak) id<NSMenuDelegate> delegate;

/* If called on the main menu, returns the height of the menu bar in pixels.  If called on any other menu, returns 0.
 */
@property (readonly) CGFloat menuBarHeight;

/* Dismisses the menu and ends all menu tracking */
- (void)cancelTracking API_AVAILABLE(macos(10.5));

/* Dismisses the menu immediately, without any fade or other effect, and ends all menu tracking */
- (void)cancelTrackingWithoutAnimation API_AVAILABLE(macos(10.6));

/* Returns the highlighted item in the menu, or nil if no item in the menu is highlighted */
@property (nullable, readonly, strong) NSMenuItem *highlightedItem API_AVAILABLE(macos(10.5));

/* Set the minimum width of the menu, in screen coordinates. The menu will prefer to not draw smaller than its minimum width, but may draw larger if it needs more space. The default value is 0.
*/
@property CGFloat minimumWidth API_AVAILABLE(macos(10.6));

/* Returns the size of the menu, in screen coordinates.  The menu may draw at a smaller size when shown, depending on its positioning and display configuration.
*/
@property (readonly) NSSize size API_AVAILABLE(macos(10.6));

/* Sets the font for the menu.  This also affects the font of all submenus that do not have their own font.
*/
@property (null_resettable, strong) NSFont *font API_AVAILABLE(macos(10.6));

/* Determines whether contextual menu plugins may be appended to the menu, if used as a context menu. The default is YES.
*/
@property BOOL allowsContextMenuPlugIns API_AVAILABLE(macos(10.6));

/* Determines whether the menu contains a column for the state image.  The default is YES. */
@property BOOL showsStateColumn API_AVAILABLE(macos(10.5));

/* Determines the layout direction for menu items in the menu. If no layout direction is explicitly set, the menu will default to the value of [NSApp userInterfaceLayoutDirection]. */
@property NSUserInterfaceLayoutDirection userInterfaceLayoutDirection API_AVAILABLE(macos(10.11));

@end

@interface NSMenu (NSSubmenuAction)
- (void)submenuAction:(nullable id)sender;
@end

@protocol NSMenuItemValidation <NSObject>
- (BOOL)validateMenuItem:(NSMenuItem *)menuItem;
@end

#if __swift__ < 40200
@interface NSObject (NSMenuValidation)
- (BOOL)validateMenuItem:(NSMenuItem *)menuItem API_DEPRECATED("This is now a method of the NSMenuItemValidation protocol.", macos(10.0, 11.0));
@end
#endif

@protocol NSMenuDelegate <NSObject>
@optional
- (void)menuNeedsUpdate:(NSMenu*)menu;

- (NSInteger)numberOfItemsInMenu:(NSMenu*)menu;
- (BOOL)menu:(NSMenu*)menu updateItem:(NSMenuItem*)item atIndex:(NSInteger)index shouldCancel:(BOOL)shouldCancel;
    // implement either the first one or the next two to populate the menu
- (BOOL)menuHasKeyEquivalent:(NSMenu*)menu forEvent:(NSEvent*)event target:(_Nullable id* _Nonnull)target action:(_Nullable SEL* _Nonnull)action;
    // bypasses populating the menu for checking for key equivalents. set target and action on return

/* indicates that the menu is being opened (displayed) or closed (hidden).  Do not modify the structure of the menu or the menu items from within these callbacks. */
- (void)menuWillOpen:(NSMenu *)menu API_AVAILABLE(macos(10.5));
- (void)menuDidClose:(NSMenu *)menu API_AVAILABLE(macos(10.5));

/* Indicates that menu is about to highlight item.  Only one item per menu can be highlighted at a time.  If item is nil, it means all items in the menu are about to be unhighlighted. */
- (void)menu:(NSMenu *)menu willHighlightItem:(nullable NSMenuItem *)item API_AVAILABLE(macos(10.5));


/* Given a menu that is about to be opened on the given screen, return a rect, in screen coordinates, within which the menu will be positioned.  If you return NSZeroRect, or if the delegate does not implement this method, the menu will be confined to the bounds appropriate for the given screen.  The returned rect may not be honored in all cases, such as if it would force the menu to be too small.
*/
- (NSRect)confinementRectForMenu:(NSMenu *)menu onScreen:(nullable NSScreen *)screen API_AVAILABLE(macos(10.6));

@end

/* The NSMenuProperties type is a bitmask used for specifying a set of menu or menu item properties, used in the following method.
 */
typedef NS_OPTIONS(NSUInteger, NSMenuProperties) {
    NSMenuPropertyItemTitle = 1 << 0,            // the menu item's title
    NSMenuPropertyItemAttributedTitle = 1 << 1,  // the menu item's attributed title
    NSMenuPropertyItemKeyEquivalent = 1 << 2,    // the menu item's key equivalent
    NSMenuPropertyItemImage = 1 << 3,            // the menu item's image
    NSMenuPropertyItemEnabled = 1 << 4,          // whether the menu item is enabled or disabled
    NSMenuPropertyItemAccessibilityDescription = 1 << 5 // the menu item's accessibility description
};

/* The following method may be called from delegate callbacks to determine which properties need to be updated and which may be skipped.  It is intended to allow more efficient updating of the menu in certain circumstances.  For example, if the NSMenuPropertyItemImage bit is zero, your delegate does not need to update the images of the menu items, because the images are not needed (for example, during key equivalent matching).  Calling this is optional: it is always acceptable to fully update the menu.
 
 This may be called from the menu delegate method -menuNeedsUpdate:.  Calling this at other times will raise an exception.
 */
@interface NSMenu (NSMenuPropertiesToUpdate)
@property (readonly) NSMenuProperties propertiesToUpdate API_AVAILABLE(macos(10.6));
@end

APPKIT_EXTERN NSNotificationName NSMenuWillSendActionNotification;
APPKIT_EXTERN NSNotificationName NSMenuDidSendActionNotification;

APPKIT_EXTERN NSNotificationName NSMenuDidAddItemNotification;
APPKIT_EXTERN NSNotificationName NSMenuDidRemoveItemNotification;
APPKIT_EXTERN NSNotificationName NSMenuDidChangeItemNotification;
// All three of these have a user info key NSMenuItemIndex with a NSNumber value.

APPKIT_EXTERN NSNotificationName NSMenuDidBeginTrackingNotification;
APPKIT_EXTERN NSNotificationName NSMenuDidEndTrackingNotification;

// The remainder of this file contains deprecated methods
@interface NSMenu (NSDeprecated)

- (void)setMenuRepresentation:(null_unspecified id)menuRep API_DEPRECATED("", macos(10.0,10.2));
- (null_unspecified id)menuRepresentation API_DEPRECATED("", macos(10.0,10.2));

- (void)setContextMenuRepresentation:(null_unspecified id)menuRep API_DEPRECATED("", macos(10.0,10.2));
- (null_unspecified id)contextMenuRepresentation API_DEPRECATED("", macos(10.0,10.2));

- (void)setTearOffMenuRepresentation:(null_unspecified id)menuRep API_DEPRECATED("", macos(10.0,10.2));
- (null_unspecified id)tearOffMenuRepresentation API_DEPRECATED("", macos(10.0,10.2));

/* Returns the zone used to allocate NSMenu objects.  This is left in for compatibility and has returned NSDefaultMallocZone() since OS X 10.2.  It is not necessary to use this - menus can be allocated the usual way. */
+ (null_unspecified NSZone *)menuZone API_DEPRECATED("", macos(10.0,10.11));
+ (void)setMenuZone:(null_unspecified NSZone *)zone API_DEPRECATED("", macos(10.0,10.2));

- (null_unspecified NSMenu *)attachedMenu API_DEPRECATED("", macos(10.0,10.2));
- (BOOL)isAttached API_DEPRECATED("", macos(10.0,10.2));
- (void)sizeToFit API_DEPRECATED("", macos(10.0,10.2));
- (NSPoint)locationForSubmenu:(null_unspecified NSMenu *)submenu API_DEPRECATED("", macos(10.0,10.2));

/* In OS X 10.6 and later, the following methods no longer do anything useful. */
@property BOOL menuChangedMessagesEnabled API_DEPRECATED("", macos(10.0,10.11));
- (void)helpRequested:(NSEvent *)eventPtr API_DEPRECATED("", macos(10.0,10.11));
@property (getter=isTornOff, readonly) BOOL tornOff API_DEPRECATED("", macos(10.0,10.11));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
