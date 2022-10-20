/*
 *  CLGeocoder.h
 *  CoreLocation
 *
 *  Copyright (c) 2010 Apple Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLLocation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

@class CLRegion;
@class CLPlacemark;
@class CLGeocoderInternal;
@class CNPostalAddress;

// geocoding handler, CLPlacemarks are provided in order of most confident to least confident
typedef void (^CLGeocodeCompletionHandler)(NSArray< CLPlacemark *> * __nullable placemarks, NSError * __nullable error);

CL_EXTERN
API_AVAILABLE(macos(10.8), ios(5.0))
@interface CLGeocoder : NSObject
{
@private
    CLGeocoderInternal *_internal;
}

@property (nonatomic, readonly, getter=isGeocoding) BOOL geocoding;

// reverse geocode requests
- (void)reverseGeocodeLocation:(CLLocation *)location completionHandler:(CLGeocodeCompletionHandler)completionHandler;
- (void)reverseGeocodeLocation:(CLLocation *)location preferredLocale:(nullable NSLocale *)locale completionHandler:(CLGeocodeCompletionHandler)completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// forward geocode requests
// geocodeAddressDictionary:completionHandler: takes an address dictionary as defined by the AddressBook framework.
// You can obtain an address dictionary from an ABPerson by retrieving the kABPersonAddressProperty property.
// Alternately, one can be constructed using the kABPersonAddress* keys defined in <AddressBook/ABPerson.h>.

- (void)geocodeAddressDictionary:(NSDictionary *)addressDictionary completionHandler:(CLGeocodeCompletionHandler)completionHandler API_DEPRECATED("Use -geocodePostalAddress:completionHandler:", macos(10.8, 10.13), ios(5.0, 11.0), watchos(1.0, 4.0));
- (void)geocodeAddressString:(NSString *)addressString completionHandler:(CLGeocodeCompletionHandler)completionHandler;
- (void)geocodeAddressString:(NSString *)addressString inRegion:(nullable CLRegion *)region completionHandler:(CLGeocodeCompletionHandler)completionHandler;
- (void)geocodeAddressString:(NSString *)addressString inRegion:(nullable CLRegion *)region preferredLocale:(nullable NSLocale *)locale completionHandler:(CLGeocodeCompletionHandler)completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

- (void)cancelGeocode;

@end

@interface CLGeocoder (ContactsAdditions)
- (void)geocodePostalAddress:(CNPostalAddress *)postalAddress completionHandler:(CLGeocodeCompletionHandler)completionHandler API_AVAILABLE(ios(11.0), macos(10.13), watchos(4.0)) API_UNAVAILABLE(tvos);
- (void)geocodePostalAddress:(CNPostalAddress *)postalAddress preferredLocale:(nullable NSLocale *)locale completionHandler:(CLGeocodeCompletionHandler)completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) API_UNAVAILABLE(tvos);
@end

NS_ASSUME_NONNULL_END
