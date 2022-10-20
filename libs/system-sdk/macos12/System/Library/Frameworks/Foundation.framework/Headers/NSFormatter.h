/*	NSFormatter.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSAttributedString.h>

@class NSString;
@class NSAttributedString;
@class NSDictionary<KeyType, ObjectType>;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSFormattingContext) {
    
    // The capitalization context to be used is unknown (this is the default value).
    NSFormattingContextUnknown = 0,
    
    // The capitalization context is determined dynamically from the set {NSFormattingContextStandalone, NSFormattingContextBeginningOfSentence, NSFormattingContextMiddleOfSentence}. For example, if a date is placed at the beginning of a sentence, NSFormattingContextBeginningOfSentence is used to format the string automatically. When this context is used, the formatter will return a string proxy that works like a normal string in most cases. After returning from the formatter, the string in the string proxy is formatted by using NSFormattingContextUnknown. When the string proxy is used in stringWithFormat:, we can determine where the %@ is and then set the context accordingly. With the new context, the string in the string proxy will be formatted again and be put into the final string returned from stringWithFormat:.
    NSFormattingContextDynamic = 1,
    
    // The capitalization context if a date or date symbol is to be formatted with capitalization appropriate for stand-alone usage such as an isolated name on a calendar page.
    NSFormattingContextStandalone = 2,
    
    // The capitalization context if a date or date symbol is to be formatted with capitalization appropriate for a list or menu item.
    NSFormattingContextListItem = 3,
    
    // The capitalization context if a date or date symbol is to be formatted with capitalization appropriate for the beginning of a sentence.
    NSFormattingContextBeginningOfSentence = 4,
    
    // The capitalization context if a date or date symbol is to be formatted with capitalization appropriate for the middle of a sentence.
    NSFormattingContextMiddleOfSentence = 5,
    
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*
 * There are 3 widths: long, medium, and short.
 * For example, for English, when formatting "3 pounds"
 * Long is "3 pounds"; medium is "3 lb"; short is "3#";
 */
typedef NS_ENUM(NSInteger, NSFormattingUnitStyle) {
    NSFormattingUnitStyleShort = 1,
    NSFormattingUnitStyleMedium,
    NSFormattingUnitStyleLong,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@interface NSFormatter : NSObject <NSCopying, NSCoding>

- (nullable NSString *)stringForObjectValue:(nullable id)obj;

- (nullable NSAttributedString *)attributedStringForObjectValue:(id)obj withDefaultAttributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs;

- (nullable NSString *)editingStringForObjectValue:(id)obj;

- (BOOL)getObjectValue:(out id _Nullable * _Nullable)obj forString:(NSString *)string errorDescription:(out NSString * _Nullable * _Nullable)error;

- (BOOL)isPartialStringValid:(NSString *)partialString newEditingString:(NSString * _Nullable * _Nullable)newString errorDescription:(NSString * _Nullable * _Nullable)error;
    // Compatibility method.  If a subclass overrides this and does not override the new method below, this will be called as before (the new method just calls this one by default).  The selection range will always be set to the end of the text with this method if replacement occurs.

- (BOOL)isPartialStringValid:(NSString * _Nonnull * _Nonnull)partialStringPtr proposedSelectedRange:(nullable NSRangePointer)proposedSelRangePtr originalString:(NSString *)origString originalSelectedRange:(NSRange)origSelRange errorDescription:(NSString * _Nullable * _Nullable)error;

@end

NS_ASSUME_NONNULL_END
