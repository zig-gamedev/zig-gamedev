/*
        NSPopUpButton.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSButton.h>
#import <AppKit/NSMenuItemCell.h>
#import <AppKit/NSMenuItem.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMenu;

@interface NSPopUpButton : NSButton

- (instancetype)initWithFrame:(NSRect)buttonFrame pullsDown:(BOOL)flag;

// Overrides behavior of NSView.  This is the menu for the popup, not a context menu.  PopUpButtons do not have context menus.
@property (nullable, strong) NSMenu *menu;

// Behavior settings
@property BOOL pullsDown;
    
@property BOOL autoenablesItems;

@property NSRectEdge preferredEdge;
    // The preferred edge is used for pull down menus and for popups under severe screen position restrictions.  It indicates what edge of the cell the menu should pop out from.

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
- (void)setTitle:(NSString *)string;

@property (nullable, readonly, strong) NSMenuItem *selectedItem;
@property (readonly) NSInteger indexOfSelectedItem;
@property (readonly) NSInteger selectedTag;
- (void)synchronizeTitleAndSelectedItem;

// Title conveniences
- (NSString *)itemTitleAtIndex:(NSInteger)index;
@property (readonly, copy) NSArray<NSString *> *itemTitles;
@property (nullable, readonly, copy) NSString *titleOfSelectedItem;

@end

/* Notifications */
APPKIT_EXTERN NSNotificationName NSPopUpButtonWillPopUpNotification;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
