/*
	NSTokenField.h
	Application Kit
	Copyright (c) 2004-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSTextContainer.h>
#import <AppKit/NSTextField.h>
#import <AppKit/NSTokenFieldCell.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTokenField;


@protocol NSTokenFieldDelegate <NSTextFieldDelegate>

@optional

// Each element in the array should be an NSString or an array of NSStrings.
// substring is the partial string that is being completed.  tokenIndex is the index of the token being completed.
// selectedIndex allows you to return by reference an index specifying which of the completions should be selected initially.
// The default behavior is not to have any completions.
- (nullable NSArray *)tokenField:(NSTokenField *)tokenField completionsForSubstring:(NSString *)substring indexOfToken:(NSInteger)tokenIndex indexOfSelectedItem:(nullable NSInteger *)selectedIndex;

// return an array of represented objects you want to add.
// If you want to reject the add, return an empty array.
// returning nil will cause an error.
- (NSArray *)tokenField:(NSTokenField *)tokenField shouldAddObjects:(NSArray *)tokens atIndex:(NSUInteger)index;

// If you return nil or don't implement these delegate methods, we will assume
// editing string = display string = represented object
- (nullable NSString *)tokenField:(NSTokenField *)tokenField displayStringForRepresentedObject:(id)representedObject;
- (nullable NSString *)tokenField:(NSTokenField *)tokenField editingStringForRepresentedObject:(id)representedObject;
- (nullable id)tokenField:(NSTokenField *)tokenField representedObjectForEditingString:(NSString *)editingString;

// We put the string on the pasteboard before calling this delegate method.
// By default, we write the NSStringPboardType as well as an array of NSStrings.
- (BOOL)tokenField:(NSTokenField *)tokenField writeRepresentedObjects:(NSArray *)objects toPasteboard:(NSPasteboard *)pboard;

// Return an array of represented objects to add to the token field.
- (nullable NSArray *)tokenField:(NSTokenField *)tokenField readFromPasteboard:(NSPasteboard *)pboard;

// By default the tokens have no menu.
- (nullable NSMenu *)tokenField:(NSTokenField *)tokenField menuForRepresentedObject:(id)representedObject;
- (BOOL)tokenField:(NSTokenField *)tokenField hasMenuForRepresentedObject:(id)representedObject;

// This method allows you to change the style for individual tokens as well as have mixed text and tokens.
- (NSTokenStyle)tokenField:(NSTokenField *)tokenField styleForRepresentedObject:(id)representedObject;


@end


@interface NSTokenField : NSTextField

/* For apps linked against 10.12, this property has zeroing weak memory semantics. When linked against an older SDK, or with objects that do not support zeroing weak references this falls back to having `assign` semantics. */
@property (nullable, weak) id<NSTokenFieldDelegate> delegate;

/* Sets the default token style used for each new token.  However, if the delegate implements tokenField:styleForRepresentedObject:, that return value will be used instead.
*/
@property NSTokenStyle tokenStyle;

@property NSTimeInterval completionDelay;
@property (class, readonly) NSTimeInterval defaultCompletionDelay;

@property (null_resettable, copy) NSCharacterSet *tokenizingCharacterSet;
@property (class, readonly, copy) NSCharacterSet *defaultTokenizingCharacterSet;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
