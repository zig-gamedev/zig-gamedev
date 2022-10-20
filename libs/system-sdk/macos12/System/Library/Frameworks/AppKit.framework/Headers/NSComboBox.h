/*
	NSComboBox.h
	Application Kit
	Copyright (c) 1996-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSTextField.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

APPKIT_EXTERN NSNotificationName NSComboBoxWillPopUpNotification;
APPKIT_EXTERN NSNotificationName NSComboBoxWillDismissNotification;
APPKIT_EXTERN NSNotificationName NSComboBoxSelectionDidChangeNotification;
APPKIT_EXTERN NSNotificationName NSComboBoxSelectionIsChangingNotification;

@class NSComboBox;

@protocol NSComboBoxDataSource <NSObject>
@optional
/* These two methods are required when not using bindings */
- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)comboBox;
- (nullable id)comboBox:(NSComboBox *)comboBox objectValueForItemAtIndex:(NSInteger)index;

- (NSUInteger)comboBox:(NSComboBox *)comboBox indexOfItemWithStringValue:(NSString *)string;
- (nullable NSString *)comboBox:(NSComboBox *)comboBox completedString:(NSString *)string;
@end

@protocol NSComboBoxDelegate <NSTextFieldDelegate>
@optional

/* Notifications */
- (void)comboBoxWillPopUp:(NSNotification *)notification;
- (void)comboBoxWillDismiss:(NSNotification *)notification;
- (void)comboBoxSelectionDidChange:(NSNotification *)notification;
- (void)comboBoxSelectionIsChanging:(NSNotification *)notification;

@end

@interface NSComboBox : NSTextField

@property BOOL hasVerticalScroller;
@property NSSize intercellSpacing;
@property CGFloat itemHeight;
@property NSInteger numberOfVisibleItems;

@property (getter=isButtonBordered) BOOL buttonBordered;

- (void)reloadData;
- (void)noteNumberOfItemsChanged;

@property BOOL usesDataSource;

- (void)scrollItemAtIndexToTop:(NSInteger)index;
- (void)scrollItemAtIndexToVisible:(NSInteger)index;

- (void)selectItemAtIndex:(NSInteger)index;
- (void)deselectItemAtIndex:(NSInteger)index;
@property (readonly) NSInteger indexOfSelectedItem;
@property (readonly) NSInteger numberOfItems;

@property BOOL completes;

@property (nullable, weak) id<NSComboBoxDelegate> delegate;

/* The dataSource will only be used when usesDataSource is YES */
@property (nullable, assign) id<NSComboBoxDataSource> dataSource;

/* These methods can only be used when usesDataSource is NO */
- (void)addItemWithObjectValue:(id)object;
- (void)addItemsWithObjectValues:(NSArray *)objects;
- (void)insertItemWithObjectValue:(id)object atIndex:(NSInteger)index;
- (void)removeItemWithObjectValue:(id)object;
- (void)removeItemAtIndex:(NSInteger)index;
- (void)removeAllItems;
- (void)selectItemWithObjectValue:(nullable id)object;
- (id)itemObjectValueAtIndex:(NSInteger)index;
@property (nullable, readonly, strong) id objectValueOfSelectedItem;
- (NSInteger)indexOfItemWithObjectValue:(id)object;
@property (readonly, copy) NSArray *objectValues;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
