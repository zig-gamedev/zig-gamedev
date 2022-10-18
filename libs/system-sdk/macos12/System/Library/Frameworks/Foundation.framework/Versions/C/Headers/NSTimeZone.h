/*	NSTimeZone.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSNotification.h>

@class NSString, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSDate, NSData, NSLocale;

NS_ASSUME_NONNULL_BEGIN

@interface NSTimeZone : NSObject <NSCopying, NSSecureCoding>

@property (readonly, copy) NSString *name;
@property (readonly, copy) NSData *data;

- (NSInteger)secondsFromGMTForDate:(NSDate *)aDate;
- (nullable NSString *)abbreviationForDate:(NSDate *)aDate;
- (BOOL)isDaylightSavingTimeForDate:(NSDate *)aDate;
- (NSTimeInterval)daylightSavingTimeOffsetForDate:(NSDate *)aDate API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (nullable NSDate *)nextDaylightSavingTimeTransitionAfterDate:(NSDate *)aDate API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end

@interface NSTimeZone (NSExtendedTimeZone)

@property (class, readonly, copy) NSTimeZone *systemTimeZone;
+ (void)resetSystemTimeZone;

@property (class, copy) NSTimeZone *defaultTimeZone;

@property (class, readonly, copy) NSTimeZone *localTimeZone;

@property (class, readonly, copy) NSArray<NSString *> *knownTimeZoneNames;

@property (class, copy) NSDictionary<NSString *, NSString *> *abbreviationDictionary API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (NSDictionary<NSString *, NSString *> *)abbreviationDictionary;

@property (class, readonly, copy) NSString *timeZoneDataVersion API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property (readonly) NSInteger secondsFromGMT;
@property (nullable, readonly, copy) NSString *abbreviation;
@property (readonly, getter=isDaylightSavingTime) BOOL daylightSavingTime;
@property (readonly) NSTimeInterval daylightSavingTimeOffset API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)); // for current instant
@property (nullable, readonly, copy) NSDate *nextDaylightSavingTimeTransition API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)); // after current instant

@property (readonly, copy) NSString *description;

- (BOOL)isEqualToTimeZone:(NSTimeZone *)aTimeZone;

typedef NS_ENUM(NSInteger, NSTimeZoneNameStyle) {
	NSTimeZoneNameStyleStandard,		// Central Standard Time
	NSTimeZoneNameStyleShortStandard,	// CST
	NSTimeZoneNameStyleDaylightSaving,	// Central Daylight Time
	NSTimeZoneNameStyleShortDaylightSaving,	// CDT
	NSTimeZoneNameStyleGeneric,		// Central Time
	NSTimeZoneNameStyleShortGeneric		// CT
};

- (nullable NSString *)localizedName:(NSTimeZoneNameStyle)style locale:(nullable NSLocale *)locale API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end

@interface NSTimeZone (NSTimeZoneCreation)

// Primary creation method is +timeZoneWithName:; the
// data-taking variants should rarely be used directly

+ (nullable instancetype)timeZoneWithName:(NSString *)tzName;
+ (nullable instancetype)timeZoneWithName:(NSString *)tzName data:(nullable NSData *)aData;

- (nullable instancetype)initWithName:(NSString *)tzName;
- (nullable instancetype)initWithName:(NSString *)tzName data:(nullable NSData *)aData;

// Time zones created with this never have daylight savings and the
// offset is constant no matter the date; the name and abbreviation
// do NOT follow the POSIX convention (of minutes-west).
+ (instancetype)timeZoneForSecondsFromGMT:(NSInteger)seconds;

+ (nullable instancetype)timeZoneWithAbbreviation:(NSString *)abbreviation;

@end

FOUNDATION_EXPORT NSNotificationName const NSSystemTimeZoneDidChangeNotification API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
