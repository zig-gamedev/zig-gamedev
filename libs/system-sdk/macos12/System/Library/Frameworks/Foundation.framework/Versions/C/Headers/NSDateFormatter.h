/*	NSDateFormatter.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSFormatter.h>
#include <CoreFoundation/CFDateFormatter.h>

@class NSLocale, NSDate, NSCalendar, NSTimeZone, NSError, NSArray<ObjectType>, NSMutableDictionary, NSString;

NS_ASSUME_NONNULL_BEGIN

#if !defined(NS_CALENDAR_ENUM_DEPRECATED)
#if 1 || NS_ENABLE_CALENDAR_DEPRECATIONS
#define NS_CALENDAR_ENUM_DEPRECATED(A, B, C, D, ...) NS_ENUM_DEPRECATED(A, B, C, D, __VA_ARGS__)
#define NS_CALENDAR_DEPRECATED(A, B, C, D, ...) NS_DEPRECATED(A, B, C, D, __VA_ARGS__)
#define NS_CALENDAR_DEPRECATED_MAC(A, B, ...) NS_DEPRECATED_MAC(A, B, __VA_ARGS__)
#else
#define NS_CALENDAR_ENUM_DEPRECATED(A, B, C, D, ...) NS_ENUM_AVAILABLE(A, C)
#define NS_CALENDAR_DEPRECATED(A, B, C, D, ...) NS_AVAILABLE(A, C)
#define NS_CALENDAR_DEPRECATED_MAC(A, B, ...) NS_AVAILABLE_MAC(A)
#endif
#endif


@interface NSDateFormatter : NSFormatter {
@private
    NSMutableDictionary *_attributes;
    CFDateFormatterRef _formatter;
    NSUInteger _counter;
}

@property NSFormattingContext formattingContext API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // default is NSFormattingContextUnknown

// - (id)init; // designated initializer

// Report the used range of the string and an NSError, in addition to the usual stuff from NSFormatter

- (BOOL)getObjectValue:(out id _Nullable * _Nullable)obj forString:(NSString *)string range:(inout nullable NSRange *)rangep error:(out NSError **)error;

// Even though NSDateFormatter responds to the usual NSFormatter methods,
//   here are some convenience methods which are a little more obvious.

- (NSString *)stringFromDate:(NSDate *)date;
- (nullable NSDate *)dateFromString:(NSString *)string;

typedef NS_ENUM(NSUInteger, NSDateFormatterStyle) {    // date and time format styles
    NSDateFormatterNoStyle = kCFDateFormatterNoStyle,
    NSDateFormatterShortStyle = kCFDateFormatterShortStyle,
    NSDateFormatterMediumStyle = kCFDateFormatterMediumStyle,
    NSDateFormatterLongStyle = kCFDateFormatterLongStyle,
    NSDateFormatterFullStyle = kCFDateFormatterFullStyle
};

typedef NS_ENUM(NSUInteger, NSDateFormatterBehavior) {
    NSDateFormatterBehaviorDefault = 0,
#if TARGET_OS_OSX
    NSDateFormatterBehavior10_0 = 1000,
#endif
    NSDateFormatterBehavior10_4 = 1040,
};


+ (NSString *)localizedStringFromDate:(NSDate *)date dateStyle:(NSDateFormatterStyle)dstyle timeStyle:(NSDateFormatterStyle)tstyle API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (nullable NSString *)dateFormatFromTemplate:(NSString *)tmplate options:(NSUInteger)opts locale:(nullable NSLocale *)locale API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
	// no options defined, pass 0 for now

// Attributes of an NSDateFormatter
@property (class) NSDateFormatterBehavior defaultFormatterBehavior;

/*
 A convenient way to generate an appropriate localized date format, and set it, in a single operation.

 Equivalent to, though not necessarily implemented as:
 formatter.dateFormat = [NSDateFormatter dateFormatFromTemplate:dateFormatTemplate options:0 locale:formatter.locale];

 Note that the template string is used only to specify which date format components should be included. Ordering and other text will not be preserved.
 The parameter is also not stored, or updated when the locale or other options change, just as with the ‘dateFormat’ property.
 */
- (void) setLocalizedDateFormatFromTemplate:(NSString *)dateFormatTemplate API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@property (null_resettable, copy) NSString *dateFormat;
@property NSDateFormatterStyle dateStyle;
@property NSDateFormatterStyle timeStyle;
@property (null_resettable, copy) NSLocale *locale;
@property BOOL generatesCalendarDates;
@property NSDateFormatterBehavior formatterBehavior;
@property (null_resettable, copy) NSTimeZone *timeZone;
@property (null_resettable, copy) NSCalendar *calendar;
@property (getter=isLenient) BOOL lenient;
@property (nullable, copy) NSDate *twoDigitStartDate;
@property (nullable, copy) NSDate *defaultDate;
@property (null_resettable, copy) NSArray<NSString *> *eraSymbols;
@property (null_resettable, copy) NSArray<NSString *> *monthSymbols;
@property (null_resettable, copy) NSArray<NSString *> *shortMonthSymbols;
@property (null_resettable, copy) NSArray<NSString *> *weekdaySymbols;
@property (null_resettable, copy) NSArray<NSString *> *shortWeekdaySymbols;
@property (null_resettable, copy) NSString *AMSymbol;
@property (null_resettable, copy) NSString *PMSymbol;
@property (null_resettable, copy) NSArray<NSString *> *longEraSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *veryShortMonthSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *standaloneMonthSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *shortStandaloneMonthSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *veryShortStandaloneMonthSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *veryShortWeekdaySymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *standaloneWeekdaySymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *shortStandaloneWeekdaySymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *veryShortStandaloneWeekdaySymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *quarterSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *shortQuarterSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *standaloneQuarterSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (null_resettable, copy) NSArray<NSString *> *shortStandaloneQuarterSymbols API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (nullable, copy) NSDate *gregorianStartDate API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property BOOL doesRelativeDateFormatting API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

#if TARGET_OS_OSX
@interface NSDateFormatter (NSDateFormatterCompatibility)

- (id)initWithDateFormat:(NSString *)format allowNaturalLanguage:(BOOL)flag API_DEPRECATED("Create an NSDateFormatter with `init` and set the dateFormat property instead.", macos(10.4, 10.9));
- (BOOL)allowsNaturalLanguage API_DEPRECATED("There is no replacement", macos(10.4, 10.9));

@end
#endif

NS_ASSUME_NONNULL_END
