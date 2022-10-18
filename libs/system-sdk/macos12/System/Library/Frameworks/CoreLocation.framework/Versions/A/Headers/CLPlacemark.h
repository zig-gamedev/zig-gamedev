/*
 *  CLPlacemark.h
 *  CoreLocation
 *
 *  Copyright (c) 2010 Apple Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

@class CLLocation;
@class CLRegion;
@class CLPlacemarkInternal;
@class CNPostalAddress;


/*
 *  CLPlacemark
 *  
 *  Discussion:
 *    Represents placemark data for a geographic location. Placemark data can be
 *    information such as the country, state, city, and street address.
 */
CL_EXTERN
API_AVAILABLE(macos(10.8), ios(5.0))
@interface CLPlacemark : NSObject <NSCopying, NSSecureCoding>
{
@private
    CLPlacemarkInternal *_internal;
}

/*
 * initWithPlacemark:
 *
 * Discussion:
 *   Initialize a newly allocated placemark from another placemark, copying its data.
 */
- (instancetype)initWithPlacemark:(CLPlacemark *) placemark;

/*
 *  location
 *  
 *  Discussion:
 *    Returns the geographic location associated with the placemark.
 */
@property (nonatomic, readonly, copy, nullable) CLLocation *location;

/*
 *  region
 *  
 *  Discussion:
 *    Returns the geographic region associated with the placemark.
 */
@property (nonatomic, readonly, copy, nullable) CLRegion *region;

/*
 *  timeZone
 *
 *  Discussion:
 *		Returns the time zone associated with the placemark.
 */
@property (nonatomic, readonly, copy, nullable) NSTimeZone *timeZone NS_AVAILABLE(10_11,9_0);

/*
 *  addressDictionary
 *  
 *  Discussion:
 *    This dictionary can be formatted as an address using ABCreateStringWithAddressDictionary,
 *    defined in the AddressBookUI framework.
 */
@property (nonatomic, readonly, copy, nullable) NSDictionary *addressDictionary API_DEPRECATED("Use @properties", macos(10.8, 10.13), ios(5.0, 11.0), watchos(1.0, 4.0));

// address dictionary properties
@property (nonatomic, readonly, copy, nullable) NSString *name; // eg. Apple Inc.
@property (nonatomic, readonly, copy, nullable) NSString *thoroughfare; // street name, eg. Infinite Loop
@property (nonatomic, readonly, copy, nullable) NSString *subThoroughfare; // eg. 1
@property (nonatomic, readonly, copy, nullable) NSString *locality; // city, eg. Cupertino
@property (nonatomic, readonly, copy, nullable) NSString *subLocality; // neighborhood, common name, eg. Mission District
@property (nonatomic, readonly, copy, nullable) NSString *administrativeArea; // state, eg. CA
@property (nonatomic, readonly, copy, nullable) NSString *subAdministrativeArea; // county, eg. Santa Clara
@property (nonatomic, readonly, copy, nullable) NSString *postalCode; // zip code, eg. 95014
@property (nonatomic, readonly, copy, nullable) NSString *ISOcountryCode; // eg. US
@property (nonatomic, readonly, copy, nullable) NSString *country; // eg. United States
@property (nonatomic, readonly, copy, nullable) NSString *inlandWater; // eg. Lake Tahoe
@property (nonatomic, readonly, copy, nullable) NSString *ocean; // eg. Pacific Ocean
@property (nonatomic, readonly, copy, nullable) NSArray<NSString *> *areasOfInterest; // eg. Golden Gate Park
@end

@interface CLPlacemark (ContactsAdditions)
@property (nonatomic, nullable, readonly) CNPostalAddress *postalAddress API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) API_UNAVAILABLE(tvos);
@end

NS_ASSUME_NONNULL_END
