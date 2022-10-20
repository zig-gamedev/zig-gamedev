/*
 *  CLVisit.h
 *  CoreLocation
 *
 *  Copyright (c) 2014 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLLocation.h>

#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLVisit
 *
 *  Discussion
 *    An instance of this class represents a possibly open-ended event
 *    during which the device was at the specified coordinate.
 */
CL_EXTERN
API_AVAILABLE(ios(8.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos)
@interface CLVisit : NSObject <NSSecureCoding, NSCopying>

/*
 *  arrivalDate
 *
 *  Discussion:
 *    The date when the visit began.  This may be equal to [NSDate
 *    distantPast] if the true arrival date isn't available.
 */
@property (nonatomic, readonly, copy) NSDate *arrivalDate;

/*
 *  departureDate
 *
 *  Discussion:
 *    The date when the visit ended.  This is equal to [NSDate
 *    distantFuture] if the device hasn't yet left.
 */
@property (nonatomic, readonly, copy) NSDate *departureDate;

/*
 *  coordinate
 *
 *  Discussion:
 *    The center of the region which the device is visiting.
 */
@property (nonatomic, readonly) CLLocationCoordinate2D coordinate;

/*
 *
 *  horizontalAccuracy
 *
 *  Discussion:
 *    An estimate of the radius (in meters) of the region which the
 *    device is visiting.
 */
@property (nonatomic, readonly) CLLocationAccuracy horizontalAccuracy;
@end

NS_ASSUME_NONNULL_END
