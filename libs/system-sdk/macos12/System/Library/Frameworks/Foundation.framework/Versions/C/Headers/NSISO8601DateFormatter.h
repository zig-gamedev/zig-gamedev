/*
    NSISO8601DateFormatter.h
    Copyright (c) 2015-2019, Apple Inc. All rights reserved.
 */

#include <CoreFoundation/CFDateFormatter.h>
#import <Foundation/NSFormatter.h>

NS_ASSUME_NONNULL_BEGIN

@class NSString, NSDate, NSTimeZone;

typedef NS_OPTIONS(NSUInteger, NSISO8601DateFormatOptions) {
    /* The format for year is inferred based on whether or not the week of year option is specified.
     - if week of year is present, "YYYY" is used to display week dates.
     - if week of year is not present, "yyyy" is used by default.
     */
    NSISO8601DateFormatWithYear API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithYear,
    NSISO8601DateFormatWithMonth API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithMonth,

    NSISO8601DateFormatWithWeekOfYear API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithWeekOfYear,  // This includes the "W" prefix (e.g. "W49")

    /* The format for day is inferred based on provided options.
     - if month is not present, day of year ("DDD") is used.
     - if month is present, day of month ("dd") is used.
     - if either weekOfMonth or weekOfYear is present, local day of week ("ee") is used.
     */
    NSISO8601DateFormatWithDay API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithDay,

    NSISO8601DateFormatWithTime API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithTime,  // This uses the format "HHmmss"
    NSISO8601DateFormatWithTimeZone API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithTimeZone,

    NSISO8601DateFormatWithSpaceBetweenDateAndTime API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithSpaceBetweenDateAndTime,  // Use space instead of "T"
    NSISO8601DateFormatWithDashSeparatorInDate API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithDashSeparatorInDate,  // Add separator for date ("-")
    NSISO8601DateFormatWithColonSeparatorInTime API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithColonSeparatorInTime,  // Add separator for time (":")
    NSISO8601DateFormatWithColonSeparatorInTimeZone API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithColonSeparatorInTimeZone,  // Add ":" separator in timezone (e.g. +08:00)
    NSISO8601DateFormatWithFractionalSeconds API_AVAILABLE(macosx(10.13), ios(11.0), watchos(4.0), tvos(11.0)) = kCFISO8601DateFormatWithFractionalSeconds,  // Add 3 significant digits of fractional seconds (".SSS")

    NSISO8601DateFormatWithFullDate API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithFullDate,
    NSISO8601DateFormatWithFullTime API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithFullTime,

    NSISO8601DateFormatWithInternetDateTime API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0)) = kCFISO8601DateFormatWithInternetDateTime,  // RFC 3339
};

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSISO8601DateFormatter : NSFormatter <NSSecureCoding> {
@private
    CFDateFormatterRef _formatter;
    NSTimeZone *_timeZone;
    NSISO8601DateFormatOptions _formatOptions;
}

/* Please note that there can be a significant performance cost when resetting these properties.  Resetting each property can result in regenerating the entire CFDateFormatterRef, which can be very expensive. */
@property (null_resettable, copy) NSTimeZone *timeZone;  // The default time zone is GMT.

@property NSISO8601DateFormatOptions formatOptions;

/* This init method creates a formatter object set to the GMT time zone and preconfigured with the RFC 3339 standard format ("yyyy-MM-dd'T'HH:mm:ssXXXXX") using the following options:
 NSISO8601DateFormatWithInternetDateTime | NSISO8601DateFormatWithDashSeparatorInDate | NSISO8601DateFormatWithColonSeparatorInTime | NSISO8601DateFormatWithColonSeparatorInTimeZone
 */
- (instancetype)init NS_DESIGNATED_INITIALIZER;

- (NSString *)stringFromDate:(NSDate *)date;
- (nullable NSDate *)dateFromString:(NSString *)string;

+ (NSString *)stringFromDate:(NSDate *)date timeZone:(NSTimeZone *)timeZone formatOptions:(NSISO8601DateFormatOptions)formatOptions;

@end

NS_ASSUME_NONNULL_END
