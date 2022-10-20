/*
        NSPopUpButtonCell.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSMenuItemCell.h>
#import <AppKit/NSMenuItem.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMenu;

typedef NS_ENUM(NSUInteger, NSPopUpArrowPosition) {
    NSPopUpNoArrow = 0,
    NSPopUpArrowAtCenter = 1,
    NSPopUpArrowAtBottom = 2
};

@interface NSPopUpButtonCell : NSMenuItemCell <NSMenuItemValidation>

- (instancetype)initTextCell:(NSString *)stringValue pullsDown:(BOOL)pullDown NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

// Overrides behavior of NSCell.  This is the menu for the popup, not a context menu.  PopUpButtonCells do not have context menus.
@property (nullable, strong) NSMenu *menu;

// Behavior settings
// If usesItemFromMenu is true, then pull down popup buttons always show the first item in the menu.  That menu item is hidden via [menuItem setHidden:YES]; if you wish to make it visible you can unhide it with setHidden:NO
@property BOOL pullsDown;
    
@property BOOL autoenablesItems;

@property NSRectEdge preferredEdge;
    // The preferred edge is used for pull down menus and for popups under severe screen position restrictions.  It indicates what edge of the cell the menu should pop out from.

@property BOOL usesItemFromMenu;
    // If YES (the default) the popup button will display an item from the menu.  This will be the selected item for a popup or the first item for a pull-down.  If this is NO, then the menu item set with -setMenuItem: is always displayed.  This can be useful for a popup button that is an icon button that pops up a menu full of textual items, for example.

@property BOOL altersStateOfSelectedItem;
    // This only has an effect for popups (it is ignored for pulldowns).  If YES (the default) then the selected item gets its state set to NSOnState.  If NO the items in the menu are left alone.

// Adding and removing items
- (void)addItemWithTitle:(NSString *)title;
- (void)addItemsWithTitles:(NSArray<NSString *> *)itemTitles;
- (void)insertItemWithTitle:(NSString *)title atIndex:(NSInteger)index;

- (void)removeItemWithTitle:(NSString *)title;
- (void)removeItemAtIndex:(NSInteger)index;
- (void)removeAllItems;


// Accessing the items
@property (readonly, copy) NSArray<NSMenuItem *> *itemArray;
@property (readonly) NSInteger numberOfItems;

- (NSInteger)indexOfItem:(NSMenuItem *)item;
- (NSInteger)indexOfItemWithTitle:(NSString *)title;
- (NSInteger)indexOfItemWithTag:(NSInteger)tag;
- (NSInteger)indexOfItemWithRepresentedObject:(nullable id)obj;
- (NSInteger)indexOfItemWithTarget:(nullable id)target andAction:(nullable SEL)actionSelector;

- (nullable NSMenuItem *)itemAtIndex:(NSInteger)index;
- (nullable NSMenuItem *)itemWithTitle:(NSString *)title;
@property (nullable, readonly, strong) NSMenuItem *lastItem;


// Dealing with selection
- (void)selectItem:(nullable NSMenuItem *)item;
- (void)selectItemAtIndex:(NSInteger)index;
- (void)selectItemWithTitle:(NSString *)title;
- (BOOL)selectItemWithTag:(NSInteger)tag;
- (void)setTitle:(nullable NSString *)string;

@property (nullable, readonly, strong) NSMenuItem *selectedItem;
@property (readonly) NSInteger indexOfSelectedItem;
- (void)synchronizeTitleAndSelectedItem;


// Title conveniences
- (NSString *)itemTitleAtIndex:(NSInteger)index;
@property (readonly, copy) NSArray<NSString *> *itemTitles;
@property (nullable, readonly, copy) NSString *titleOfSelectedItem;

- (void)attachPopUpWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)dismissPopUp;
- (void)performClickWithFrame:(NSRect)frame inView:(NSView *)controlView;

// Arrow position for bezel style and borderless popups.
@property NSPopUpArrowPosition arrowPosition;

@end


/* Notifications */
APPKIT_EXTERN NSNotificationName NSPopUpButtonCellWillPopUpNotification;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
