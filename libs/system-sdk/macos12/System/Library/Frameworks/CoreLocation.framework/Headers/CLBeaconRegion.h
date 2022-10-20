/*
 *  CLBeaconRegion.h
 *  CoreLocation
 *
 *  Copyright (c) 2012 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLRegion.h>
#import <CoreLocation/CLAvailability.h>
#import <CoreLocation/CLBeaconIdentityConstraint.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLBeaconRegion
 *
 *  Discussion:
 *    A region containing similar beacons.
 *
 *    Such a region can be defined by UUID, major and minor values.
 *    UUID must be specified. If only UUID is specified, the major and
 *    minor values will be wildcarded and the region will match any
 *    beacons with the same UUID. Similarly if only UUID and major
 *    value are specified, the minor value will be wildcarded and the
 *    region will match against any beacons with the same UUID and
 *    major value.
 *
 */
CL_EXTERN
API_AVAILABLE(ios(7.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos)
@interface CLBeaconRegion : CLRegion

/*
 *  initWithUUID:identifier:
 *
 *  Discussion:
 *    Initialize a beacon region with a UUID. Major and minor values will be wildcarded.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid identifier:(NSString *)identifier API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);
- (instancetype)initWithProximityUUID:(NSUUID *)proximityUUID identifier:(NSString *)identifier API_DEPRECATED_WITH_REPLACEMENT("-initWithUUID:identifier:", ios(7.0, 13.0), macos(10.15, 10.15));

/*
 *  initWithUUID:major:identifier:
 *
 *  Discussion:
 *    Initialize a beacon region with a UUID and major value. Minor value will be wildcarded.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid major:(CLBeaconMajorValue)major identifier:(NSString *)identifier API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);
- (instancetype)initWithProximityUUID:(NSUUID *)proximityUUID major:(CLBeaconMajorValue)major identifier:(NSString *)identifier API_DEPRECATED_WITH_REPLACEMENT("-initWithUUID:major:identifier:", ios(7.0, 13.0), macos(10.15, 10.15));

/*
 *  initWithUUID:major:minor:identifier:
 *
 *  Discussion:
 *    Initialize a beacon region identified by a UUID, major and minor values.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid major:(CLBeaconMajorValue)major minor:(CLBeaconMinorValue)minor identifier:(NSString *)identifier API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);
- (instancetype)initWithProximityUUID:(NSUUID *)proximityUUID major:(CLBeaconMajorValue)major minor:(CLBeaconMinorValue)minor identifier:(NSString *)identifier API_DEPRECATED_WITH_REPLACEMENT("-initWithUUID:major:identifier:", ios(7.0, 13.0), macos(10.15, 10.15));

/*
 *  initWithBeaconIdentityConstraint:identifier:
 *
 *  Discussion:
 *    Initialize a beacon region described by a beacon identity
 *    constraint.
 *
 */
- (instancetype)initWithBeaconIdentityConstraint:(CLBeaconIdentityConstraint *)beaconIdentityConstraint identifier:(NSString *)identifier API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  peripheralDataWithMeasuredPower:
 *
 *  Discussion:
 *    This dictionary can be used to advertise the current device as a beacon when
 *    used in conjunction with CoreBluetooth's CBPeripheralManager startAdvertising: method.
 *    The dictionary will contain data that represents the current region in addition to a measured power value.
 *
 *    measuredPower is the RSSI of the device observed from one meter in its intended environment.
 *    This value is optional, but should be specified to achieve the best ranging performance.
 *    If not specified, it will default to a pre-determined value for the device.
 *
 */
- (NSMutableDictionary<NSString *, id> *)peripheralDataWithMeasuredPower:(nullable NSNumber *)measuredPower;

/*
 *  beaconIdentityConstraint
 *
 *  Discussion:
 *    Returns a CLBeaconIdentityConstraint describing the beacons this region monitors.
 */
@property (readonly, nonatomic, copy) CLBeaconIdentityConstraint *beaconIdentityConstraint API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  UUID
 *
 *  Discussion:
 *    UUID associated with the region.
 *
 */
@property (readonly, nonatomic, copy) NSUUID *UUID API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);
@property (readonly, nonatomic, copy) NSUUID *proximityUUID API_DEPRECATED_WITH_REPLACEMENT("-UUID", ios(7.0, 13.0), macos(10.15, 10.15));

/*
 *  major
 *
 *  Discussion:
 *    Most significant value associated with the region. If a major value wasn't specified, this will be nil.
 *
 */
@property (readonly, nonatomic, copy, nullable) NSNumber *major;

/*
 *  minor
 *
 *  Discussion:
 *    Least significant value associated with the region. If a minor value wasn't specified, this will be nil.
 *
 */
@property (readonly, nonatomic, copy, nullable) NSNumber *minor;

/*
 *  notifyEntryStateOnDisplay
 *
 *  Discussion:
 *    App will be launched and the delegate will be notified via locationManager:didDetermineState:forRegion:
 *    when the device's screen is turned on and the user is in the region. By default, this is NO.
 */
@property (atomic, assign) BOOL notifyEntryStateOnDisplay;

@end

@class CLBeaconInternal;

/*
 *  CLBeacon
 *
 *  Discussion:
 *    A single beacon within a CLBeaconRegion.
 *
 */
API_AVAILABLE(ios(7.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos)
@interface CLBeacon : NSObject <NSCopying, NSSecureCoding>
{
@package
	CLBeaconInternal *_internal;
}

/*
 *  timestamp
 *
 *  Discussion:
 *    The time when this beacon was observed.
 *
 */
@property (readonly, nonatomic, copy) NSDate *timestamp API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  UUID
 *
 *  Discussion:
 *    UUID associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy) NSUUID *UUID API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);
@property (readonly, nonatomic, copy) NSUUID *proximityUUID API_DEPRECATED_WITH_REPLACEMENT("-UUID", ios(7.0, 13.0), macos(10.15, 10.15));

/*
 *  major
 *
 *  Discussion:
 *    Most significant value associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy) NSNumber *major;

/*
 *  minor
 *
 *  Discussion:
 *    Least significant value associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy) NSNumber *minor;

/*
 *  proximity
 *
 *  Discussion:
 *    Proximity of the beacon from the device.
 *
 */
@property (readonly, nonatomic) CLProximity proximity;

/*
 *  accuracy
 *
 *  Discussion:
 *    Represents an one sigma horizontal accuracy in meters where the measuring device's location is
 *    referenced at the beaconing device. This value is heavily subject to variations in an RF environment.
 *    A negative accuracy value indicates the proximity is unknown.
 *
 */
@property (readonly, nonatomic) CLLocationAccuracy accuracy;

/*
 *  rssi
 *
 *  Discussion:
 *    Received signal strength in decibels of the specified beacon.
 *    This value is an average of the RSSI samples collected since this beacon was last reported.
 *
 */
@property (readonly, nonatomic) NSInteger rssi;

@end

NS_ASSUME_NONNULL_END
