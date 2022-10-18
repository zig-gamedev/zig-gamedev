/*
 *  CLError.h
 *  CoreLocation
 *
 *  Copyright (c) 2008-2010 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLError
 *  
 *  Discussion:
 *    Error returned as code to NSError from CoreLocation.
 */
typedef NS_ENUM(NSInteger, CLError) {
    kCLErrorLocationUnknown  = 0,         // location is currently unknown, but CL will keep trying
    kCLErrorDenied,                       // Access to location or ranging has been denied by the user
    kCLErrorNetwork,                      // general, network-related error
    kCLErrorHeadingFailure,               // heading could not be determined
    kCLErrorRegionMonitoringDenied,       // Location region monitoring has been denied by the user
    kCLErrorRegionMonitoringFailure,      // A registered region cannot be monitored
    kCLErrorRegionMonitoringSetupDelayed, // CL could not immediately initialize region monitoring
    kCLErrorRegionMonitoringResponseDelayed, // While events for this fence will be delivered, delivery will not occur immediately
    kCLErrorGeocodeFoundNoResult,         // A geocode request yielded no result
    kCLErrorGeocodeFoundPartialResult,    // A geocode request yielded a partial result
    kCLErrorGeocodeCanceled,              // A geocode request was cancelled
    kCLErrorDeferredFailed,               // Deferred mode failed
    kCLErrorDeferredNotUpdatingLocation,  // Deferred mode failed because location updates disabled or paused
    kCLErrorDeferredAccuracyTooLow,       // Deferred mode not supported for the requested accuracy
    kCLErrorDeferredDistanceFiltered,     // Deferred mode does not support distance filters
    kCLErrorDeferredCanceled,             // Deferred mode request canceled a previous request
	kCLErrorRangingUnavailable,           // Ranging cannot be performed
	kCLErrorRangingFailure,               // General ranging failure
	kCLErrorPromptDeclined,               // Authorization request not presented to user
};

/*
 *  kCLErrorUserInfoAlternateRegionKey
 *  
 *  Discussion:
 *    When an error with code kCLErrorRegionMonitoringResponseDelayed is received, this key may be populated
 *    in the userInfo dictionary.  The value is a CLRegion that the location service can more effectively monitor.
 */
CL_EXTERN NSString *const kCLErrorUserInfoAlternateRegionKey NS_AVAILABLE(10_7, 5_0) API_UNAVAILABLE(watchos, tvos);

NS_ASSUME_NONNULL_END
