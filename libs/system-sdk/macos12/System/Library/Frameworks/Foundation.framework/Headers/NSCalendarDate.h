/*	NSCalendarDate.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSDate.h>

@class NSString, NSArray, NSTimeZone;

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

API_DEPRECATED("Use NSCalendar and NSDateComponents and NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0))
NS_SWIFT_UNAVAILABLE("Use NSCalendar and NSDateComponents and NSDateFormatter instead")
@interface NSCalendarDate : NSDate {
@private
    NSUInteger		refCount;
    NSTimeInterval 	_timeIntervalSinceReferenceDate;
    NSTimeZone		*_timeZone;
    NSString 		*_formatString;
    void		*_reserved;
}

/*    DEPRECATED DEPRECATED DEPRECATED
 *    These methods are deprecated.
 *    Use NSCalendar for calendrical calculations.
 *    Use NSDateFormatter for date<->string conversions.
 */
+ (id)calendarDate API_DEPRECATED("Use NSCalendar instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (nullable id)dateWithString:(NSString *)description calendarFormat:(NSString *)format locale:(nullable id)locale API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (nullable id)dateWithString:(NSString *)description calendarFormat:(NSString *)format API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (id)dateWithYear:(NSInteger)year month:(NSUInteger)month day:(NSUInteger)day hour:(NSUInteger)hour minute:(NSUInteger)minute second:(NSUInteger)second timeZone:(nullable NSTimeZone *)aTimeZone API_DEPRECATED("Use NSCalendar and NSDateComponents instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSCalendarDate *)dateByAddingYears:(NSInteger)year months:(NSInteger)month days:(NSInteger)day hours:(NSInteger)hour minutes:(NSInteger)minute seconds:(NSInteger)second API_DEPRECATED("Use NSCalendar instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)dayOfCommonEra API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)dayOfMonth API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)dayOfWeek API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)dayOfYear API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)hourOfDay API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)minuteOfHour API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)monthOfYear API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)secondOfMinute API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSInteger)yearOfCommonEra API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar component:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSString *)calendarFormat API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSString *)descriptionWithCalendarFormat:(NSString *)format locale:(nullable id)locale API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSString *)descriptionWithCalendarFormat:(NSString *)format API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSString *)descriptionWithLocale:(nullable id)locale API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSTimeZone *)timeZone API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (nullable id)initWithString:(NSString *)description calendarFormat:(NSString *)format locale:(nullable id)locale API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (nullable id)initWithString:(NSString *)description calendarFormat:(NSString *)format API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (nullable id)initWithString:(NSString *)description API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (id)initWithYear:(NSInteger)year month:(NSUInteger)month day:(NSUInteger)day hour:(NSUInteger)hour minute:(NSUInteger)minute second:(NSUInteger)second timeZone:(nullable NSTimeZone *)aTimeZone API_DEPRECATED("Use NSCalendar and NSDateComponents instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (void)setCalendarFormat:(nullable NSString *)format API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (void)setTimeZone:(nullable NSTimeZone *)aTimeZone API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (void)years:(nullable NSInteger *)yp months:(nullable NSInteger *)mop days:(nullable NSInteger *)dp hours:(nullable NSInteger *)hp minutes:(nullable NSInteger *)mip seconds:(nullable NSInteger *)sp sinceDate:(NSCalendarDate *)date API_DEPRECATED_WITH_REPLACEMENT("-[NSCalendar components:fromDate:]", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (instancetype)distantFuture API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (instancetype)distantPast API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
@end

@interface NSDate (NSCalendarDateExtras)
/*    DEPRECATED DEPRECATED DEPRECATED
 *    These methods are deprecated.
 *    Use NSCalendar for calendrical calculations.
 *    Use NSDateFormatter for date<->string conversions.
 */
+ (nullable id)dateWithNaturalLanguageString:(NSString *)string locale:(nullable id)locale API_DEPRECATED("Create an NSDateFormatter with `init` and set the dateFormat property instead.", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (nullable id)dateWithNaturalLanguageString:(NSString *)string API_DEPRECATED("Create an NSDateFormatter with `init` and set the dateFormat property instead.", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
+ (id)dateWithString:(NSString *)aString API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (NSCalendarDate *)dateWithCalendarFormat:(nullable NSString *)format timeZone:(nullable NSTimeZone *)aTimeZone API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (nullable NSString *)descriptionWithCalendarFormat:(nullable NSString *)format timeZone:(nullable NSTimeZone *)aTimeZone locale:(nullable id)locale API_DEPRECATED("", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
- (nullable id)initWithString:(NSString *)description API_DEPRECATED("Use NSDateFormatter instead", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
@end

NS_ASSUME_NONNULL_END
