/*	NSDateInterval.h
	Copyright (c) 2015-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSCoder.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSDateInterval : NSObject <NSCopying, NSSecureCoding>

/*
 NSDateInterval represents a closed date interval in the form of [startDate, endDate].  It is possible for the start and end dates to be the same with a duration of 0.  NSDateInterval does not support reverse intervals i.e. intervals where the duration is less than 0 and the end date occurs earlier in time than the start date.
 */

@property (readonly, copy) NSDate *startDate;
@property (readonly, copy) NSDate *endDate;
@property (readonly) NSTimeInterval duration;

// This method initializes an NSDateInterval object with start and end dates set to the current date and the duration set to 0.
- (instancetype)init;

- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

// This method will throw an exception if the duration is less than 0.
- (instancetype)initWithStartDate:(NSDate *)startDate duration:(NSTimeInterval)duration NS_DESIGNATED_INITIALIZER;

// This method will throw an exception if the end date comes before the start date.
- (instancetype)initWithStartDate:(NSDate *)startDate endDate:(NSDate *)endDate;

/*
 (NSComparisonResult)compare:(NSDateInterval *) prioritizes ordering by start date. If the start dates are equal, then it will order by duration.
 e.g.
    Given intervals a and b
        a.   |-----|
        b.      |-----|
 [a compare:b] would return NSOrderedAscending because a's startDate is earlier in time than b's start date.

 In the event that the start dates are equal, the compare method will attempt to order by duration.
 e.g.
    Given intervals c and d
        c.  |-----|
        d.  |---|
 [c compare:d] would result in NSOrderedDescending because c is longer than d.

 If both the start dates and the durations are equal, then the intervals are considered equal and NSOrderedSame is returned as the result.
 */
- (NSComparisonResult)compare:(NSDateInterval *)dateInterval;

- (BOOL)isEqualToDateInterval:(NSDateInterval *)dateInterval;
- (BOOL)intersectsDateInterval:(NSDateInterval *)dateInterval;

/*
 This method returns an NSDateInterval object that represents the interval where the given date interval and the current instance intersect. In the event that there is no intersection, the method returns nil.
 */
- (nullable NSDateInterval *)intersectionWithDateInterval:(NSDateInterval *)dateInterval;

- (BOOL)containsDate:(NSDate *)date;

@end

NS_ASSUME_NONNULL_END
