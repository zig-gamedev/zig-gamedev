/*
 *  CLBeaconIdentityConstraint.h
 *  CoreLocation
 *
 *  Copyright (c) 2019 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLBeaconMajorValue
 *
 *  Discussion:
 *    Type represents the most significant value in a beacon.
 *
 */
typedef uint16_t CLBeaconMajorValue;

/*
 *  CLBeaconMinorValue
 *
 *  Discussion:
 *    Type represents the least significant value in a beacon.
 *
 */
typedef uint16_t CLBeaconMinorValue;


/*
 *  CLBeaconIdentityConstraint
 *
 *  Discussion:
 *    A constraint that describes the identity caracteristics of a beacon.
 *
 *    A beacon identity is defined by UUID, major and minor values.
 *    UUID must be specified. If only UUID is specified, the major and
 *    minor values will be wildcarded and any beacons with the same
 *    UUID will satisfy the constraint. Similarly if only UUID and
 *    major value are specified, the minor value will be wildcarded
 *    and any beacons with the same UUID and major value will satisfy
 *    the constraint.
 *
 */
CL_EXTERN
API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos)
@interface CLBeaconIdentityConstraint : NSObject <NSCopying, NSSecureCoding>

/*
 *  UUID
 *
 *  Discussion:
 *    UUID associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy) NSUUID *UUID;

/*
 *  major
 *
 *  Discussion:
 *    Most significant value associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy, nullable) NSNumber *major;

/*
 *  minor
 *
 *  Discussion:
 *    Least significant value associated with the beacon.
 *
 */
@property (readonly, nonatomic, copy, nullable) NSNumber *minor;

/*
 *  initWithUUID:
 *
 *  Discussion:
 *    Initialize a beacon identity constraint with a UUID. Major and
 *    minor values will be wildcarded.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid;

/*
 *  initWithUUID:major:
 *
 *  Discussion:
 *    Initialize a beacon identity constraint with a UUID and major
 *    value.  Minor value will be wildcarded.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid major:(CLBeaconMajorValue)major;

/*
 *  initWithUUID:major:minor:
 *
 *  Discussion:
 *    Initialize a beacon identity constraint with a UUID, major, and
 *    minor values.
 *
 */
- (instancetype)initWithUUID:(NSUUID *)uuid major:(CLBeaconMajorValue)major minor:(CLBeaconMinorValue)minor;
@end

NS_ASSUME_NONNULL_END
