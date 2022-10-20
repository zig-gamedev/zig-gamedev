/*
	NSComboBoxCell.h
	Application Kit
	Copyright (c) 1996-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSTextFieldCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSButtonCell, NSTableView;
@protocol NSComboBoxCellDataSource;

@interface NSComboBoxCell : NSTextFieldCell

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
- (nullable NSString *)completedString:(NSString *)string;

/* These two methods can only be used when usesDataSource is YES */
@property (nullable, assign) id<NSComboBoxCellDataSource> dataSource;

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

@protocol NSComboBoxCellDataSource <NSObject>
@optional
/* These two methods are required when not using bindings */
- (NSInteger)numberOfItemsInComboBoxCell:(NSComboBoxCell *)comboBoxCell;
- (id)comboBoxCell:(NSComboBoxCell *)comboBoxCell objectValueForItemAtIndex:(NSInteger)index;

- (NSUInteger)comboBoxCell:(NSComboBoxCell *)comboBoxCell indexOfItemWithStringValue:(NSString *)string;
- (nullable NSString *)comboBoxCell:(NSComboBoxCell *)comboBoxCell completedString:(NSString *)uncompletedString; 
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
