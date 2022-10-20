/*	NSDateComponentsFormatter.h
	Copyright (c) 2014-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSFormatter.h>
#import <Foundation/NSCalendar.h>
#import <Foundation/NSNumberFormatter.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
typedef NS_ENUM(NSInteger, NSDateComponentsFormatterUnitsStyle) {
    NSDateComponentsFormatterUnitsStylePositional = 0, // "1:10; may fall back to abbreviated units in some cases, e.g. 3d"
    NSDateComponentsFormatterUnitsStyleAbbreviated, // "1h 10m"
    NSDateComponentsFormatterUnitsStyleShort, // "1 hr, 10 min"
    NSDateComponentsFormatterUnitsStyleFull, // "1 hour, 10 minutes"
    NSDateComponentsFormatterUnitsStyleSpellOut, // "One hour, ten minutes"
    NSDateComponentsFormatterUnitsStyleBrief API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) // "1hr 10min" - Brief is shorter than Short (e.g. in English, it removes the comma)
};

API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
typedef NS_OPTIONS(NSUInteger, NSDateComponentsFormatterZeroFormattingBehavior) {
    NSDateComponentsFormatterZeroFormattingBehaviorNone = (0), //drop none, pad none
    NSDateComponentsFormatterZeroFormattingBehaviorDefault = (1 << 0), //Positional units: drop leading zeros, pad other zeros. All others: drop all zeros.

    NSDateComponentsFormatterZeroFormattingBehaviorDropLeading = (1 << 1), // Off: "0h 10m", On: "10m"
    NSDateComponentsFormatterZeroFormattingBehaviorDropMiddle = (1 << 2), // Off: "1h 0m 10s", On: "1h 10s"
    NSDateComponentsFormatterZeroFormattingBehaviorDropTrailing = (1 << 3), // Off: "1h 0m", On: "1h"
    NSDateComponentsFormatterZeroFormattingBehaviorDropAll = (NSDateComponentsFormatterZeroFormattingBehaviorDropLeading | NSDateComponentsFormatterZeroFormattingBehaviorDropMiddle | NSDateComponentsFormatterZeroFormattingBehaviorDropTrailing),

    NSDateComponentsFormatterZeroFormattingBehaviorPad = (1 << 16), // Off: "1:0:10", On: "01:00:10" 
};

/* NSDateComponentsFormatter provides locale-correct and flexible string formatting of quantities of time, such as "1 day" or "1h 10m", as specified by NSDateComponents. For formatting intervals of time (such as "2PM to 5PM"), see NSDateIntervalFormatter. NSDateComponentsFormatter is thread-safe, in that calling methods on it from multiple threads will not cause crashes or incorrect results, but it makes no attempt to prevent confusion when one thread sets something and another thread isn't expecting it to change.
 */
API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSDateComponentsFormatter : NSFormatter {
    @private
    pthread_mutex_t _lock;
    void *_fmt;
    void *_unused;
    NSString *_fmtLocaleIdent;
    NSCalendar *_calendar;
    NSDate *_referenceDate;
    NSNumberFormatter *_unitFormatter;
    NSCalendarUnit _allowedUnits;
    NSFormattingContext _formattingContext;
    NSDateComponentsFormatterUnitsStyle _unitsStyle;
    NSDateComponentsFormatterZeroFormattingBehavior _zeroFormattingBehavior;
    NSInteger _maximumUnitCount;
    BOOL _allowsFractionalUnits;
    BOOL _collapsesLargestUnit;
    BOOL _includesApproximationPhrase;
    BOOL _includesTimeRemainingPhrase;
    void *_reserved;
}

/* 'obj' must be an instance of NSDateComponents.
 */
- (nullable NSString *)stringForObjectValue:(nullable id)obj;

/* Convenience method for formatting an NSDateComponents object. This calculates the count of the unit(s) starting from referenceDate.
 */
- (nullable NSString *)stringFromDateComponents:(NSDateComponents *)components;

/* Normally, NSDateComponentsFormatter will calculate as though counting from referenceDate (e.g. in February, 1 month formatted as a number of days will be 28). -stringFromDate:toDate: calculates from the passed-in startDate instead.
 
   See 'allowedUnits' for how the default set of allowed units differs from -stringFromDateComponents:.
 
   Note that this is still formatting the quantity of time between the dates, not the pair of dates itself. For strings like "Feb 22nd - Feb 28th", use NSDateIntervalFormatter.
 */
- (nullable NSString *)stringFromDate:(NSDate *)startDate toDate:(NSDate *)endDate;

/* Convenience method for formatting a number of seconds. See 'allowedUnits' for how the default set of allowed units differs from -stringFromDateComponents:. This calculates the count of the unit(s) from referenceDate to the passed-in time interval.
 */
- (nullable NSString *)stringFromTimeInterval:(NSTimeInterval)ti;


+ (nullable NSString *)localizedStringFromDateComponents:(NSDateComponents *)components unitsStyle:(NSDateComponentsFormatterUnitsStyle) unitsStyle;

/* Choose how to indicate units. For example, 1h 10m vs 1:10. Default is NSDateComponentsFormatterUnitsStylePositional.
 */
@property NSDateComponentsFormatterUnitsStyle unitsStyle;

/* Bitmask of units to include. Set to 0 to get the default behavior. Note that, especially if the maximum number of units is low, unit collapsing is on, or zero dropping is on, not all allowed units may actually be used for a given NSDateComponents. Default value is the components of the passed-in NSDateComponents object, or years | months | weeks | days | hours | minutes | seconds if passed an NSTimeInterval or pair of NSDates.
 
   Allowed units are:
 
    NSCalendarUnitYear
    NSCalendarUnitMonth
    NSCalendarUnitWeekOfMonth (used to mean "quantity of weeks")
    NSCalendarUnitDay
    NSCalendarUnitHour
    NSCalendarUnitMinute
    NSCalendarUnitSecond
 
   Specifying any other NSCalendarUnits will result in an exception.
 */
@property NSCalendarUnit allowedUnits;

/* Bitmask specifying how to handle zeros in units. This includes both padding and dropping zeros so that a consistent number digits are displayed, causing updating displays to remain more stable. Default is NSDateComponentsFormatterZeroFormattingBehaviorDefault.
 
   If the combination of zero formatting behavior and style would lead to ambiguous date formats (for example, 1:10 meaning 1 hour, 10 seconds), NSDateComponentsFormatter will throw an exception.
 */
@property NSDateComponentsFormatterZeroFormattingBehavior zeroFormattingBehavior;

/* Specifies the locale and calendar to use for formatting date components that do not themselves have calendars. Defaults to NSAutoupdatingCurrentCalendar. If set to nil, uses the gregorian calendar with the en_US_POSIX locale.
 */
@property (nullable, copy) NSCalendar *calendar;

/*  Where units have variable length (number of days in a month, number of hours in a day, etc...), NSDateComponentsFormatter will calculate as though counting from the date specified by the referenceDate in the appropriate calendar. Defaults to [NSDate dateWithTimeIntervalSinceReferenceDate:0] at the time of the -stringForObjectValue: call if not set. Set to nil to get the default behavior.
 */
@property (nullable, copy) NSDate *referenceDate API_AVAILABLE(macosx(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/* Choose whether non-integer units should be used to handle display of values that can't be exactly represented with the allowed units. For example, if minutes aren't allowed, then "1h 30m" could be formatted as "1.5h". Default is NO.
 */
@property BOOL allowsFractionalUnits;

/* Choose whether or not, and at which point, to round small units in large values to zero.
   Examples:
    1h 10m 30s, maximumUnitCount set to 0: "1h 10m 30s"
    1h 10m 30s, maximumUnitCount set to 2: "1h 10m"
    10m 30s, maximumUnitCount set to 0: "10m 30s"
    10m 30s, maximumUnitCount set to 2: "10m 30s"

   Default is 0, which is interpreted as unlimited.
 */
@property NSInteger maximumUnitCount;

/* Choose whether to express largest units just above the threshold for the next lowest unit as a larger quantity of the lower unit. For example: "1m 3s" vs "63s". Default is NO.
 */
@property BOOL collapsesLargestUnit;

/* Choose whether to indicate that the allowed units/insignificant units choices lead to inexact results. In some languages, simply prepending "about " to the string will produce incorrect results; this handles those cases correctly. Default is NO.
 */
@property BOOL includesApproximationPhrase;

/* Choose whether to produce strings like "35 minutes remaining". Default is NO.
 */
@property BOOL includesTimeRemainingPhrase;

/* 
   Not yet supported.
 */
@property NSFormattingContext formattingContext;

/* NSDateComponentsFormatter currently only implements formatting, not parsing. Until it implements parsing, this will always return NO.
 */
- (BOOL)getObjectValue:(out id _Nullable * _Nullable)obj forString:(NSString *)string errorDescription:(out NSString * _Nullable * _Nullable)error;

@end

NS_ASSUME_NONNULL_END
