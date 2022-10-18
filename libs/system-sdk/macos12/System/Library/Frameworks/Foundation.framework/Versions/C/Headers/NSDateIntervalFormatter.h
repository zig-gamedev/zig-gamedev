/*	NSDateIntervalFormatter.h
        Copyright (c) 2013-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSFormatter.h>
#import <Foundation/NSDateInterval.h>
#include <dispatch/dispatch.h>
@class NSLocale, NSCalendar, NSTimeZone, NSDate;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NSDateIntervalFormatterStyle) {
    NSDateIntervalFormatterNoStyle = 0,
    NSDateIntervalFormatterShortStyle = 1,
    NSDateIntervalFormatterMediumStyle = 2,
    NSDateIntervalFormatterLongStyle = 3,
    NSDateIntervalFormatterFullStyle = 4
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

// NSDateIntervalFormatter is used to format the range between two NSDates in a locale-sensitive way.
// NSDateIntervalFormatter returns nil and NO for all methods in NSFormatter.

API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSDateIntervalFormatter : NSFormatter
{
@private
    NSLocale *_locale;
    NSCalendar *_calendar;
    NSTimeZone *_timeZone;
    NSString *_dateTemplate;
    NSString *_dateTemplateFromStyles;
    void *_formatter;
    NSDateIntervalFormatterStyle _dateStyle;
    NSDateIntervalFormatterStyle _timeStyle;
    BOOL _modified;
    BOOL _useTemplate;
    dispatch_semaphore_t _lock;
    void *_reserved[4];
}

@property (null_resettable, copy) NSLocale   *locale;                // default is [NSLocale currentLocale]
@property (null_resettable, copy) NSCalendar *calendar;              // default is the calendar of the locale
@property (null_resettable, copy) NSTimeZone *timeZone;              // default is [NSTimeZone defaultTimeZone]
@property (null_resettable, copy) NSString   *dateTemplate;          // default is an empty string
@property NSDateIntervalFormatterStyle dateStyle;   // default is NSDateIntervalFormatterNoStyle
@property NSDateIntervalFormatterStyle timeStyle;   // default is NSDateIntervalFormatterNoStyle

/*
     If the range smaller than the resolution specified by the dateTemplate, a single date format will be produced. If the range is larger than the format specified by the dateTemplate, a locale-specific fallback will be used to format the items missing from the pattern.
     
     For example, if the range is 2010-03-04 07:56 - 2010-03-04 19:56 (12 hours)
     - The pattern jm will produce
        for en_US, "7:56 AM - 7:56 PM"
        for en_GB, "7:56 - 19:56"
     - The pattern MMMd will produce
        for en_US, "Mar 4"
        for en_GB, "4 Mar"
     If the range is 2010-03-04 07:56 - 2010-03-08 16:11 (4 days, 8 hours, 15 minutes)
     - The pattern jm will produce
        for en_US, "3/4/2010 7:56 AM - 3/8/2010 4:11 PM"
        for en_GB, "4/3/2010 7:56 - 8/3/2010 16:11"
     - The pattern MMMd will produce
        for en_US, "Mar 4-8"
        for en_GB, "4-8 Mar"
*/
- (NSString *)stringFromDate:(NSDate *)fromDate toDate:(NSDate *)toDate;

- (nullable NSString *)stringFromDateInterval:(NSDateInterval *)dateInterval API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@end

NS_ASSUME_NONNULL_END
