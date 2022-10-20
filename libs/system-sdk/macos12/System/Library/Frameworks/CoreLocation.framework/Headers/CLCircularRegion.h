/*
 *  CLCircularRegion.h
 *  CoreLocation
 *
 *  Copyright (c) 2012 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLRegion.h>
#import <CoreLocation/CLLocation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLCircularRegion
 *
 *  Discussion:
 *    A circular geographic area.
 */
CL_EXTERN
API_AVAILABLE(macos(10.10), ios(7.0))
@interface CLCircularRegion : CLRegion

/*
 *  initWithCenter:radius:identifier:
 *
 *  Discussion:
 *    Initialize a region.  center gives the coordinates of center of the region, while radius gives
 *    the distance in meters between the center and the region's boundary. identifier is a description
 *    for the region that could be displayed to the user, and ideally should be chosen by the user.
 */
- (instancetype)initWithCenter:(CLLocationCoordinate2D)center
                            radius:(CLLocationDistance)radius
                        identifier:(NSString *)identifier;

/*
 *  center
 *
 *  Discussion:
 *    Returns the coordinate of the center of the region.
 */
@property (readonly, atomic) CLLocationCoordinate2D center;

/*
 *  radius
 *
 *  Discussion:
 *    Returns the radius of the region.
 */
@property (readonly, atomic) CLLocationDistance radius;

/*
 *  containsCoordinate:
 *
 *  Discussion:
 *    Returns YES if the coordinate lies inside the region, and NO otherwise.
 */
- (BOOL)containsCoordinate:(CLLocationCoordinate2D)coordinate;

@end

NS_ASSUME_NONNULL_END
