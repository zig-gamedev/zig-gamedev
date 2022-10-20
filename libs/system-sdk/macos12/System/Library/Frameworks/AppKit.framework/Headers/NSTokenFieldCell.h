/*
	NSTokenFieldCell.h
	Application Kit
	Copyright (c) 2004-2021, Apple Inc.
	All rights reserved.

*/

#import <AppKit/NSTextFieldCell.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTextContainer;
@protocol NSTokenFieldCellDelegate;

typedef NS_ENUM(NSUInteger, NSTokenStyle) {
    NSTokenStyleDefault, // This is equivalent to NSTokenStyleRounded. This may change in future releases.
    NSTokenStyleNone, // Style to use for data you want represented as plain-text and without any token background.
    NSTokenStyleRounded, // A token with rounded edges.
    NSTokenStyleSquared API_AVAILABLE(macos(10.10)), // A token with squared edges.
    NSTokenStylePlainSquared API_AVAILABLE(macos(10.10)), // A token with squared edges that has no background unless selected or highlighted.
};

@interface NSTokenFieldCell : NSTextFieldCell

/* Sets the default token style used for each new token.  However, if the delegate implements tokenField:styleForRepresentedObject:, that return value will be used instead.
*/
@property NSTokenStyle tokenStyle;

/* Sets the auto-completion delay before the list of possible completions automatically pops up.  Completions are only offered if the delegate implements the completion delegate API.  A negative delay will disable completion while a delay of 0.0 will make completion UI presentation immediate.
*/
@property NSTimeInterval completionDelay;
@property (class, readonly) NSTimeInterval defaultCompletionDelay;

@property (null_resettable, copy) NSCharacterSet *tokenizingCharacterSet;
@property (class, readonly, copy) NSCharacterSet *defaultTokenizingCharacterSet;

@property (nullable, weak) id<NSTokenFieldCellDelegate> delegate;

@end

@protocol NSTokenFieldCellDelegate <NSObject>

@optional

// Each element in the array should be an NSString or an array of NSStrings.
// substring is the partial string that is being completed.  tokenIndex is the index of the token being completed.
// selectedIndex allows you to return by reference an index specifying which of the completions should be selected initially. 
// The default behavior is not to have any completions.
- (NSArray *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell completionsForSubstring:(NSString *)substring indexOfToken:(NSInteger)tokenIndex indexOfSelectedItem:(NSInteger *)selectedIndex;

// return an array of represented objects you want to add.
// If you want to reject the add, return an empty array.
// returning nil will cause an error.
- (NSArray *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell shouldAddObjects:(NSArray *)tokens atIndex:(NSUInteger)index;

// If you return nil or don't implement these delegate methods, we will assume
// editing string = display string = represented object
- (nullable NSString *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell displayStringForRepresentedObject:(id)representedObject;
- (nullable NSString *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell editingStringForRepresentedObject:(id)representedObject;
- (nullable id)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell representedObjectForEditingString: (NSString *)editingString;

// We put the string on the pasteboard before calling this delegate method. 
// By default, we write the NSStringPboardType as well as an array of NSStrings.
- (BOOL)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell writeRepresentedObjects:(NSArray *)objects toPasteboard:(NSPasteboard *)pboard;

// Return an array of represented objects to add to the token field.
- (nullable NSArray *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell readFromPasteboard:(NSPasteboard *)pboard;

// By default the tokens have no menu.
- (nullable NSMenu *)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell menuForRepresentedObject:(id)representedObject;
- (BOOL)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell hasMenuForRepresentedObject:(id)representedObject; 

// This method allows you to change the style for individual tokens as well as have mixed text and tokens.
- (NSTokenStyle)tokenFieldCell:(NSTokenFieldCell *)tokenFieldCell styleForRepresentedObject:(id)representedObject;

@end

static const NSTokenStyle NSDefaultTokenStyle API_DEPRECATED_WITH_REPLACEMENT("NSTokenStyleDefault", macos(10.0,10.14)) = NSTokenStyleDefault;
static const NSTokenStyle NSPlainTextTokenStyle API_DEPRECATED_WITH_REPLACEMENT("NSTokenStyleNone", macos(10.0,10.14)) = NSTokenStyleNone;
static const NSTokenStyle NSRoundedTokenStyle API_DEPRECATED_WITH_REPLACEMENT("NSTokenStyleRounded", macos(10.0,10.14)) = NSTokenStyleRounded;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
