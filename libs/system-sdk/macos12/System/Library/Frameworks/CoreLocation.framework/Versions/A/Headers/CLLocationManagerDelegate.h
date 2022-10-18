/*
 *  CLLocationManagerDelegate.h
 *  CoreLocation
 *
 *  Copyright (c) 2008-2010 Apple Inc. All rights reserved.
 *
 */

#import <Availability.h>
#import <Foundation/Foundation.h>
#import <CoreLocation/CLLocationManager.h>
#import <CoreLocation/CLRegion.h>
#import <CoreLocation/CLVisit.h>

NS_ASSUME_NONNULL_BEGIN

@class CLLocation;
@class CLHeading;
@class CLBeacon;
@class CLVisit;

/*
 *  CLLocationManagerDelegate
 *  
 *  Discussion:
 *    Delegate for CLLocationManager.
 */
@protocol CLLocationManagerDelegate<NSObject>

@optional

/*
 *  locationManager:didUpdateToLocation:fromLocation:
 *  
 *  Discussion:
 *    Invoked when a new location is available. oldLocation may be nil if there is no previous location
 *    available.
 *
 *    This method is deprecated. If locationManager:didUpdateLocations: is
 *    implemented, this method will not be called.
 */
- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation API_AVAILABLE(macos(10.6)) API_DEPRECATED("Implement -locationManager:didUpdateLocations: instead", ios(2.0, 6.0)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didUpdateLocations:
 *
 *  Discussion:
 *    Invoked when new locations are available.  Required for delivery of
 *    deferred locations.  If implemented, updates will
 *    not be delivered to locationManager:didUpdateToLocation:fromLocation:
 *
 *    locations is an array of CLLocation objects in chronological order.
 */
- (void)locationManager:(CLLocationManager *)manager
	 didUpdateLocations:(NSArray<CLLocation *> *)locations API_AVAILABLE(ios(6.0), macos(10.9));

/*
 *  locationManager:didUpdateHeading:
 *  
 *  Discussion:
 *    Invoked when a new heading is available.
 */
- (void)locationManager:(CLLocationManager *)manager
       didUpdateHeading:(CLHeading *)newHeading API_AVAILABLE(ios(3.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  locationManagerShouldDisplayHeadingCalibration:
 *
 *  Discussion:
 *    Invoked when a new heading is available. Return YES to display heading calibration info. The display 
 *    will remain until heading is calibrated, unless dismissed early via dismissHeadingCalibrationDisplay.
 */
- (BOOL)locationManagerShouldDisplayHeadingCalibration:(CLLocationManager *)manager  API_AVAILABLE(ios(3.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  locationManager:didDetermineState:forRegion:
 *
 *  Discussion:
 *    Invoked when there's a state transition for a monitored region or in response to a request for state via a
 *    a call to requestStateForRegion:.
 */
- (void)locationManager:(CLLocationManager *)manager
	didDetermineState:(CLRegionState)state forRegion:(CLRegion *)region API_AVAILABLE(ios(7.0), macos(10.10)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didRangeBeacons:inRegion:
 *
 *  Discussion:
 *    Invoked when a new set of beacons are available in the specified region.
 *    beacons is an array of CLBeacon objects.
 *    If beacons is empty, it may be assumed no beacons that match the specified region are nearby.
 *    Similarly if a specific beacon no longer appears in beacons, it may be assumed the beacon is no longer received
 *    by the device.
 */
- (void)locationManager:(CLLocationManager *)manager
		didRangeBeacons:(NSArray<CLBeacon *> *)beacons
			   inRegion:(CLBeaconRegion *)region API_DEPRECATED_WITH_REPLACEMENT("Use locationManager:didRangeBeacons:satisfyingConstraint:", ios(7.0, 13.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:rangingBeaconsDidFailForRegion:withError:
 *
 *  Discussion:
 *    Invoked when an error has occurred ranging beacons in a region. Error types are defined in "CLError.h".
 */
- (void)locationManager:(CLLocationManager *)manager
rangingBeaconsDidFailForRegion:(CLBeaconRegion *)region
			  withError:(NSError *)error API_DEPRECATED_WITH_REPLACEMENT("Use locationManager:didFailRangingBeaconsForConstraint:error:", ios(7.0, 13.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

- (void)locationManager:(CLLocationManager *)manager
		didRangeBeacons:(NSArray<CLBeacon *> *)beacons
   satisfyingConstraint:(CLBeaconIdentityConstraint *)beaconConstraint API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

- (void)locationManager:(CLLocationManager *)manager
didFailRangingBeaconsForConstraint:(CLBeaconIdentityConstraint *)beaconConstraint
				  error:(NSError *)error API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didEnterRegion:
 *
 *  Discussion:
 *    Invoked when the user enters a monitored region.  This callback will be invoked for every allocated
 *    CLLocationManager instance with a non-nil delegate that implements this method.
 */
- (void)locationManager:(CLLocationManager *)manager
	didEnterRegion:(CLRegion *)region API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didExitRegion:
 *
 *  Discussion:
 *    Invoked when the user exits a monitored region.  This callback will be invoked for every allocated
 *    CLLocationManager instance with a non-nil delegate that implements this method.
 */
- (void)locationManager:(CLLocationManager *)manager
	didExitRegion:(CLRegion *)region API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didFailWithError:
 *  
 *  Discussion:
 *    Invoked when an error has occurred. Error types are defined in "CLError.h".
 */
- (void)locationManager:(CLLocationManager *)manager
	didFailWithError:(NSError *)error;

/*
 *  locationManager:monitoringDidFailForRegion:withError:
 *  
 *  Discussion:
 *    Invoked when a region monitoring error has occurred. Error types are defined in "CLError.h".
 */
- (void)locationManager:(CLLocationManager *)manager
	monitoringDidFailForRegion:(nullable CLRegion *)region
	withError:(NSError *)error API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didChangeAuthorizationStatus:
 *  
 *  Discussion:
 *    Invoked when the authorization status changes for this application.
 */
- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status API_DEPRECATED_WITH_REPLACEMENT("-locationManagerDidChangeAuthorization:", ios(4.2, 14.0), macos(10.7, 11.0), watchos(1.0, 7.0), tvos(9.0, 14.0));

/*
 *  locationManagerDidChangeAuthorization:
 *
 *  Discussion:
 *    Invoked when either the authorizationStatus or
 *    accuracyAuthorization properties change
 */
- (void)locationManagerDidChangeAuthorization:(CLLocationManager *)manager API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));

/*
 *  locationManager:didStartMonitoringForRegion:
 *  
 *  Discussion:
 *    Invoked when a monitoring for a region started successfully.
 */
- (void)locationManager:(CLLocationManager *)manager
	didStartMonitoringForRegion:(CLRegion *)region API_AVAILABLE(ios(5.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  Discussion:
 *    Invoked when location updates are automatically paused.
 */
- (void)locationManagerDidPauseLocationUpdates:(CLLocationManager *)manager API_AVAILABLE(ios(6.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  Discussion:
 *    Invoked when location updates are automatically resumed.
 *
 *    In the event that your application is terminated while suspended, you will
 *	  not receive this notification.
 */
- (void)locationManagerDidResumeLocationUpdates:(CLLocationManager *)manager API_AVAILABLE(ios(6.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didFinishDeferredUpdatesWithError:
 *
 *  Discussion:
 *    Invoked when deferred updates will no longer be delivered. Stopping
 *    location, disallowing deferred updates, and meeting a specified criterion
 *    are all possible reasons for finishing deferred updates.
 *
 *    An error will be returned if deferred updates end before the specified
 *    criteria are met (see CLError), otherwise error will be nil.
 */
- (void)locationManager:(CLLocationManager *)manager
	didFinishDeferredUpdatesWithError:(nullable NSError *)error API_AVAILABLE(ios(6.0), macos(10.9)) API_UNAVAILABLE(watchos, tvos);

/*
 *  locationManager:didVisit:
 *
 *  Discussion:
 *    Invoked when the CLLocationManager determines that the device has visited
 *    a location, if visit monitoring is currently started (possibly from a
 *    prior launch).
 */
- (void)locationManager:(CLLocationManager *)manager didVisit:(CLVisit *)visit API_AVAILABLE(ios(8.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

@end

NS_ASSUME_NONNULL_END
