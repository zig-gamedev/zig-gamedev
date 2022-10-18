/*    NSRelativeDateTimeFormatter.h
      Copyright (c) 2018-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSDate.h>
#import <Foundation/NSFormatter.h>

@class NSCalendar, NSLocale, NSDateComponents;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSRelativeDateTimeFormatterStyle) {
    NSRelativeDateTimeFormatterStyleNumeric = 0, // "1 day ago", "2 days ago", "1 week ago", "in 1 week"
    NSRelativeDateTimeFormatterStyleNamed, // “yesterday”, "2 days ago", "last week", "next week"; falls back to the numeric style if no name is available
} API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

typedef NS_ENUM(NSInteger, NSRelativeDateTimeFormatterUnitsStyle) {
    NSRelativeDateTimeFormatterUnitsStyleFull = 0, // "2 months ago"
    NSRelativeDateTimeFormatterUnitsStyleSpellOut, // "two months ago"
    NSRelativeDateTimeFormatterUnitsStyleShort, // "2 mo. ago"
    NSRelativeDateTimeFormatterUnitsStyleAbbreviated, // "2 mo. ago"; might give different results in languages other than English
} API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* NSRelativeDateTimeFormatter provides locale-aware formatting of a relative date or time, such as "1 hour ago", "in 2 weeks", "yesterday", and "tomorrow." Note that the string produced by the formatter should only be used in a standalone manner as it may not be grammatically correct to embed the string in longer strings.
 */
API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSRelativeDateTimeFormatter : NSFormatter
#if !__OBJC2__
{
    @private
    void * _formatter;
    NSRelativeDateTimeFormatterStyle _dateTimeStyle;
    NSRelativeDateTimeFormatterUnitsStyle _unitsStyle;
    NSFormattingContext _formattingContext;
    NSCalendar *_calendar;
    NSLocale *_locale;
}
#endif // !__OBJC2__

/* Specifies how to describe a relative date. For example, "yesterday" vs "1 day ago" in English. Default is NSRelativeDateTimeFormatterStyleNumeric.
 */
@property NSRelativeDateTimeFormatterStyle dateTimeStyle;

/* Specifies how to format the quantity or the name of the unit. For example, "1 day ago" vs "one day ago" in English. Default is NSRelativeDateTimeFormatterUnitsStyleFull.
 */
@property NSRelativeDateTimeFormatterUnitsStyle unitsStyle;

/* Specifies the formatting context of the output. Default is NSFormattingContextUnknown.
 */
@property NSFormattingContext formattingContext;

/* Specifies the calendar to use for formatting values that do not have an inherent calendar of their own. Defaults to autoupdatingCurrentCalendar. Also resets to autoupdatingCurrentCalendar on assignment of nil.
 */
@property (null_resettable, copy) NSCalendar *calendar;

/* Specifies the locale of the output string. Defaults to and resets on assignment of nil to the calendar's locale.
 */
@property (null_resettable, copy) NSLocale *locale;

/* Convenience method for formatting a relative time represented by an NSDateComponents object. Negative component values are evaluated as a date in the past. This method formats the value of the least granular unit in the NSDateComponents object, and does not provide a compound format of the date component.

 Note this method only supports the following components: year, month, week of month, day, hour, minute, and second. The rest will be ignored.
 */
- (NSString *)localizedStringFromDateComponents:(NSDateComponents *)dateComponents;

/* Convenience method for formatting a time interval using the formatter's calendar. Negative time interval is evaluated as a date in the past.
 */
- (NSString *)localizedStringFromTimeInterval:(NSTimeInterval)timeInterval;

/* Formats the date interval from the reference date to the given date using the formatter's calendar.
 */
- (NSString *)localizedStringForDate:(NSDate *)date relativeToDate:(NSDate *)referenceDate;

/* Inherited from NSFormatter. 'obj' must be an instance of NSDate. Returns nil otherwise. When formatting a relative date using this method, the class uses -[NSDate date] as the reference date.
 */
- (nullable NSString *)stringForObjectValue:(nullable id)obj;

@end

NS_ASSUME_NONNULL_END
