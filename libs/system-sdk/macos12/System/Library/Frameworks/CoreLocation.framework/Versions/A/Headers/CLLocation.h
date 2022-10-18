/*
 *  CLLocation.h
 *  CoreLocation
 *
 *  Copyright (c) 2008-2010 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLLocationDegrees
 *  
 *  Discussion:
 *    Type used to represent a latitude or longitude coordinate in degrees under the WGS 84 reference
 *    frame. The degree can be positive (North and East) or negative (South and West).  
 */
typedef double CLLocationDegrees;

/*
 *  CLLocationAccuracy
 *  
 *  Discussion:
 *    Type used to represent a location accuracy level in meters. The lower the value in meters, the
 *    more physically precise the location is. A negative accuracy value indicates an invalid location.
 */
typedef double CLLocationAccuracy;

/*
 *  CLLocationSpeed
 *  
 *  Discussion:
 *    Type used to represent the speed in meters per second.
 */
typedef double CLLocationSpeed;

/*
 *  CLLocationSpeedAccuracy
 *
 *  Discussion:
 *    Type used to represent a speed accuracy level in meters/second.  The lower the value the more
 *    precise the speed is.  A negative accuracy value indicates an invalid speed.
 */
typedef double CLLocationSpeedAccuracy;

/*
 *  CLLocationDirection
 *  
 *  Discussion:
 *    Type used to represent the direction in degrees from 0 to 359.9. A negative value indicates an
 *    invalid direction.
 */
typedef double CLLocationDirection;

/*
 *  CLLocationDirectionAccuracy
 *
 *  Discussion:
 *    Type used to represent a direction accuracy in degrees.  The lower the value the more precise the
 *    direction is.  A negative accuracy value indicates an invalid direction.
 */
typedef double CLLocationDirectionAccuracy;

/*
 *  CLLocationCoordinate2D
 *  
 *  Discussion:
 *    A structure that contains a geographical coordinate.
 *
 *  Fields:
 *    latitude:
 *      The latitude in degrees.
 *    longitude:
 *      The longitude in degrees.
 */
struct CLLocationCoordinate2D {
	CLLocationDegrees latitude;
	CLLocationDegrees longitude;
};
typedef struct CLLocationCoordinate2D CLLocationCoordinate2D;

/*
 *  CLLocationDistance
 *  
 *  Discussion:
 *    Type used to represent a distance in meters.
 */
typedef double CLLocationDistance;

/*
 *  kCLDistanceFilterNone
 *  
 *  Discussion:
 *    Use as the distanceFilter property for CLLocationManager. This indicates 
 *    to the location service that no minimum movement filter is desired - ie, client will be informed
 *    of any movement.
 */
CL_EXTERN const CLLocationDistance kCLDistanceFilterNone;

/*
 *  kCLLocationAccuracy<x>
 *  
 *  Discussion:
 *    Used to specify the accuracy level desired. The location service will try its best to achieve
 *    your desired accuracy. However, it is not guaranteed. To optimize
 *    power performance, be sure to specify an appropriate accuracy for your usage scenario (eg,
 *    use a large accuracy value when only a coarse location is needed).
 */
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyBestForNavigation API_AVAILABLE(ios(4.0), macos(10.7));
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyBest;
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyNearestTenMeters;
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyHundredMeters;
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyKilometer;
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyThreeKilometers;

/*
 *  kCLLocationAccuracyReduced
 *
 *  Discussion:
 *    If you set your CLLocationManager's desiredAccuracy property to
 *    this value, locations delivered to your delegate in response to
 *    startUpdatingLocation or requestLocation will have their
 *    accuracy reduced.  The locations you receive will match the
 *    locations your app would have received if the user had decided
 *    not to grant your app authorization for precise location.
 */
CL_EXTERN const CLLocationAccuracy kCLLocationAccuracyReduced API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));

/*
 *  CLLocationDistanceMax
 *
 *  Discussion:
 *  	Used to specify the maximum CLLocationDistance
 */
CL_EXTERN const CLLocationDistance CLLocationDistanceMax API_AVAILABLE(ios(6.0), macos(10.14));

/*
 *  CLTimeIntervalMax
 *
 *  Discussion:
 *  	Used to specify the maximum NSTimeInterval
 */
CL_EXTERN const NSTimeInterval CLTimeIntervalMax API_AVAILABLE(ios(6.0), macos(10.14));

/*
 *  kCLLocationCoordinate2DInvalid
 *  
 *  Discussion:
 *    Used to specify an invalid CLLocationCoordinate2D.
 */
CL_EXTERN const CLLocationCoordinate2D kCLLocationCoordinate2DInvalid API_AVAILABLE(ios(4.0), macos(10.7));

#ifdef __cplusplus
extern "C" {
#endif    

/*
 *  CLLocationCoordinate2DIsValid
 *  
 *  Discussion:
 *    Returns YES if the specified coordinate is valid, NO otherwise.
 */
CL_EXTERN
BOOL CLLocationCoordinate2DIsValid(CLLocationCoordinate2D coord) API_AVAILABLE(ios(4.0), macos(10.7));

/*
 *  CLLocationCoordinate2DMake:
 *
 *  Discussion:
 *    Returns a new CLLocationCoordinate2D at the given latitude and longitude
 */
CL_EXTERN
CLLocationCoordinate2D CLLocationCoordinate2DMake(CLLocationDegrees latitude, CLLocationDegrees longitude) API_AVAILABLE(ios(4.0), macos(10.7));

#ifdef __cplusplus
}
#endif

/*
 *  CLFloor
 *
 *  Discussion:
 *    Encapsulates the information about a floor.
 */
CL_EXTERN
API_AVAILABLE(ios(8.0), macos(10.15))
@interface CLFloor : NSObject <NSCopying, NSSecureCoding>

/*
 *  level
 *
 *  Discussion:
 *    This is a logical representation that will vary on definition from building-to-building.
 *    Floor 0 will always represent the floor designated as "ground".
 *    This number may be negative to designate floors below the ground floor
 *    and positive to indicate floors above the ground floor.
 *    It is not intended to match any numbering that might actually be used in the building.
 *    It is erroneous to use as an estimate of altitude.
 */
@property(readonly, nonatomic) NSInteger level;

@end

/*
 * CLLocationSourceInformation
 *
 * Discussion:
 * 	For a CLLocation, represents information about the source of the location, including whether it came from a simulator
 */
CL_EXTERN
API_AVAILABLE(ios(15.0), watchos(8.0), tvos(15.0), macos(12.0))
@interface CLLocationSourceInformation : NSObject <NSCopying, NSSecureCoding>

- (instancetype)initWithSoftwareSimulationState:(BOOL)isSoftware
					  andExternalAccessoryState:(BOOL)isAccessory;

/*
 * isSimulatedBySoftware
 *
 * Discussion:
 *	Set to YES if this location was detected as being generated by a software simulator, such as Xcode
 */
@property(readonly, nonatomic) BOOL isSimulatedBySoftware;

/*
 * isProducedByAccessory
 *
 * Discussion:
 * 	Set to YES if this location was generated from an external accessory, such as CarPlay or an MFi accessory
 */
@property(readonly, nonatomic) BOOL isProducedByAccessory;
@end

/*
 *  CLLocation
 *  
 *  Discussion:
 *    Represents a geographical coordinate along with accuracy and timestamp information.
 */
CL_EXTERN
API_AVAILABLE(macos(10.6), ios(2.0))
@interface CLLocation : NSObject <NSCopying, NSSecureCoding>
{
@private
	id _internal;
}

/*
 *  initWithLatitude:longitude:
 *  
 *  Discussion:
 *    Initialize with the specified latitude and longitude.
 */
- (instancetype)initWithLatitude:(CLLocationDegrees)latitude
	longitude:(CLLocationDegrees)longitude;

/*
 *  initWithCoordinate:altitude:horizontalAccuracy:verticalAccuracy:timestamp:
 *  
 *  Discussion:
 *    Initialize with the specified parameters.
 */
- (instancetype)initWithCoordinate:(CLLocationCoordinate2D)coordinate
	altitude:(CLLocationDistance)altitude
	horizontalAccuracy:(CLLocationAccuracy)hAccuracy
	verticalAccuracy:(CLLocationAccuracy)vAccuracy
	timestamp:(NSDate *)timestamp;

/*
 *  initWithCoordinate:altitude:horizontalAccuracy:verticalAccuracy:course:speed:timestamp:
 *  
 *  Discussion:
 *    Initialize with the specified parameters.
 */
- (instancetype)initWithCoordinate:(CLLocationCoordinate2D)coordinate
    altitude:(CLLocationDistance)altitude
    horizontalAccuracy:(CLLocationAccuracy)hAccuracy
    verticalAccuracy:(CLLocationAccuracy)vAccuracy
    course:(CLLocationDirection)course
    speed:(CLLocationSpeed)speed
    timestamp:(NSDate *)timestamp API_AVAILABLE(ios(4.2), macos(10.7));

/*
 *  initWithCoordinate:altitude:horizontalAccuracy:verticalAccuracy:course:courseAccuracy:speed:speedAccuracy:timestamp:
 *
 *  Discussion:
 *    Initialize with the specified parameters.
 */
- (instancetype)initWithCoordinate:(CLLocationCoordinate2D)coordinate
    altitude:(CLLocationDistance)altitude
    horizontalAccuracy:(CLLocationAccuracy)hAccuracy
    verticalAccuracy:(CLLocationAccuracy)vAccuracy
    course:(CLLocationDirection)course
    courseAccuracy:(CLLocationDirectionAccuracy)courseAccuracy
    speed:(CLLocationSpeed)speed
    speedAccuracy:(CLLocationSpeedAccuracy)speedAccuracy
    timestamp:(NSDate *)timestamp API_AVAILABLE(ios(13.4), macos(10.15.4), watchos(6.2), tvos(13.4));

/*
 *  initWithCoordinate:altitude:horizontalAccuracy:verticalAccuracy:course:courseAccuracy:speed:speedAccuracy:timestamp:sourceInfo
 *
 *  Discussion:
 *    Initialize with the specified parameters.
 */
- (instancetype)initWithCoordinate:(CLLocationCoordinate2D)coordinate
	altitude:(CLLocationDistance)altitude
	horizontalAccuracy:(CLLocationAccuracy)hAccuracy
	verticalAccuracy:(CLLocationAccuracy)vAccuracy
	course:(CLLocationDirection)course
	courseAccuracy:(CLLocationDirectionAccuracy)courseAccuracy
	speed:(CLLocationSpeed)speed
	speedAccuracy:(CLLocationSpeedAccuracy)speedAccuracy
	timestamp:(NSDate *)timestamp
	sourceInfo:(CLLocationSourceInformation*)sourceInfo API_AVAILABLE(ios(15.0), watchos(8.0), tvos(15.0), macos(12.0));

/*
 *  coordinate
 *  
 *  Discussion:
 *    Returns the coordinate of the current location.
 */
@property(readonly, nonatomic) CLLocationCoordinate2D coordinate;

/*
 *  altitude
 *  
 *  Discussion:
 *    Returns the altitude of the location. Can be positive (above sea level) or negative (below sea level).
 */
@property(readonly, nonatomic) CLLocationDistance altitude;

/*
 *  ellipsoidalAltitude
 *
 *  Discussion:
 *    Returns the ellipsoidal altitude of the location under the WGS 84 reference frame.
 *    Can be positive or negative.
 */
@property(readonly, nonatomic) CLLocationDistance ellipsoidalAltitude API_AVAILABLE(ios(15), macos(12), watchos(8), tvos(15));

/*
 *  horizontalAccuracy
 *  
 *  Discussion:
 *    Returns the horizontal accuracy of the location. Negative if the lateral location is invalid.
 */
@property(readonly, nonatomic) CLLocationAccuracy horizontalAccuracy;

/*
 *  verticalAccuracy
 *  
 *  Discussion:
 *    Returns the vertical accuracy of the location. Negative if the altitude is invalid.
 */
@property(readonly, nonatomic) CLLocationAccuracy verticalAccuracy;

/*
 *  course
 *  
 *  Discussion:
 *    Returns the course of the location in degrees true North. Negative if course is invalid.
 *
 *  Range:
 *    0.0 - 359.9 degrees, 0 being true North
 */
@property(readonly, nonatomic) CLLocationDirection course API_AVAILABLE(ios(2.2), macos(10.7));

/*
 *  courseAccuracy
 *
 *  Discussion:
 *    Returns the course accuracy of the location in degrees.  Returns negative if course is invalid.
 */
@property(readonly, nonatomic) CLLocationDirectionAccuracy courseAccuracy API_AVAILABLE(ios(13.4), macos(10.15.4), watchos(6.2), tvos(13.4));

/*
 *  speed
 *  
 *  Discussion:
 *    Returns the speed of the location in m/s. Negative if speed is invalid.
 */
@property(readonly, nonatomic) CLLocationSpeed speed API_AVAILABLE(ios(2.2), macos(10.7));

/*
 *  speedAccuracy
 *
 *  Discussion:
 *    Returns the speed accuracy of the location in m/s. Returns -1 if invalid.
 */
@property(readonly, nonatomic) CLLocationSpeedAccuracy speedAccuracy API_AVAILABLE(macos(10.15), ios(10.0), watchos(3.0), tvos(10.0));

/*
 *  timestamp
 *  
 *  Discussion:
 *    Returns the timestamp when this location was determined.
 */
@property(readonly, nonatomic, copy) NSDate *timestamp;

/*
 *  floor
 *
 *  Discussion:
 *    Contains information about the logical floor that you are on
 *    in the current building if you are inside a supported venue.
 *    This will be nil if the floor is unavailable.
 */
@property(readonly, nonatomic, copy, nullable) CLFloor *floor API_AVAILABLE(ios(8.0), macos(10.15));

/*
 *  sourceInformation
 *
 *  Discussion:
 *    Contains information about the source of this location.
 */
@property(readonly, nonatomic, nullable) CLLocationSourceInformation *sourceInformation API_AVAILABLE(ios(15.0), watchos(8.0), tvos(15.0), macos(12.0));

/*
 *  getDistanceFrom:
 *
 *  Discussion:
 *    Deprecated. Use -distanceFromLocation: instead.
 */
- (CLLocationDistance)getDistanceFrom:(const CLLocation *)location API_DEPRECATED_WITH_REPLACEMENT("-distanceFromLocation:", ios(2.0, 3.2), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  distanceFromLocation:
 *
 *  Discussion:
 *    Returns the lateral distance between two locations.
 */
- (CLLocationDistance)distanceFromLocation:(const CLLocation *)location API_AVAILABLE(ios(3.2), macos(10.6));

@end

NS_ASSUME_NONNULL_END
