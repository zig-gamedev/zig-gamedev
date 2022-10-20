/*
 *  CLLocationManager.h
 *  CoreLocation
 *
 *  Copyright (c) 2008-2010 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CLAvailability.h>
#import <CoreLocation/CLLocation.h>
#import <CoreLocation/CLRegion.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  CLDeviceOrientation
 *  
 *  Discussion:
 *      Specifies a physical device orientation, equivalent to UIDeviceOrientation.
 *      
 */
typedef NS_ENUM(int, CLDeviceOrientation) {
	CLDeviceOrientationUnknown = 0,
	CLDeviceOrientationPortrait,
	CLDeviceOrientationPortraitUpsideDown,
	CLDeviceOrientationLandscapeLeft,
	CLDeviceOrientationLandscapeRight,
	CLDeviceOrientationFaceUp,
	CLDeviceOrientationFaceDown
};

/*
 *  CLAuthorizationStatus
 *  
 *  Discussion:
 *      Represents the current authorization state of the application.
 *      
 */
typedef NS_ENUM(int, CLAuthorizationStatus) {
	// User has not yet made a choice with regards to this application
	kCLAuthorizationStatusNotDetermined = 0,

	// This application is not authorized to use location services.  Due
	// to active restrictions on location services, the user cannot change
	// this status, and may not have personally denied authorization
	kCLAuthorizationStatusRestricted,

	// User has explicitly denied authorization for this application, or
	// location services are disabled in Settings.
	kCLAuthorizationStatusDenied,

	// User has granted authorization to use their location at any
	// time.  Your app may be launched into the background by
	// monitoring APIs such as visit monitoring, region monitoring,
	// and significant location change monitoring.
	//
	// This value should be used on iOS, tvOS and watchOS.  It is available on
	// MacOS, but kCLAuthorizationStatusAuthorized is synonymous and preferred.
	kCLAuthorizationStatusAuthorizedAlways API_AVAILABLE(macos(10.12), ios(8.0)),

	// User has granted authorization to use their location only while
	// they are using your app.  Note: You can reflect the user's
	// continued engagement with your app using
	// -allowsBackgroundLocationUpdates.
	//
	// This value is not available on MacOS.  It should be used on iOS, tvOS and
	// watchOS.
	kCLAuthorizationStatusAuthorizedWhenInUse API_AVAILABLE(ios(8.0)) API_UNAVAILABLE(macos),

	// User has authorized this application to use location services.
	//
	// This value is deprecated or prohibited on iOS, tvOS and watchOS.
	// It should be used on MacOS.
	kCLAuthorizationStatusAuthorized API_DEPRECATED("Use kCLAuthorizationStatusAuthorizedAlways", ios(2.0, 8.0)) API_AVAILABLE(macos(10.6)) API_UNAVAILABLE(watchos, tvos) = kCLAuthorizationStatusAuthorizedAlways
};

typedef NS_ENUM(NSInteger, CLAccuracyAuthorization) {
	// This application has the user's permission to receive accurate location information.
	CLAccuracyAuthorizationFullAccuracy,

	// The user has chosen to grant this application access to location information with reduced accuracy.
	// Region monitoring and beacon ranging are not available to the application.  Other CoreLocation APIs
	// are available with reduced accuracy.

	// Location estimates will have a horizontalAccuracy on the order of about 5km.  To achieve the
	// reduction in accuracy, CoreLocation will snap location estimates to a nearby point which represents
	// the region the device is in.  Furthermore, CoreLocation will reduce the rate at which location
	// estimates are produced.  Applications should be prepared to receive locations that are up to 20
	// minutes old.
	CLAccuracyAuthorizationReducedAccuracy,
};

/*
 *	CLActivityType
 *
 *  Discussion:
 *		Expected user activity for the lifetime of the corresponding CLLocationManager instance.
 *		Used to optimize the positioning experience delivered to this CLLocationManager instance
 *		for the expected activity.
 */
typedef NS_ENUM(NSInteger, CLActivityType) {
    CLActivityTypeOther = 1,			// positioning in activities that are not covered by one of
										// the other activity types.  Includes activities without a
										// specific user intention; for example, positioning while
										// the user sits on a bench interacting with the device

    CLActivityTypeAutomotiveNavigation,	// positioning in an automobile following a road network

    CLActivityTypeFitness,				// positioning in dedicated fitness sessions, e.g. walking
										// workouts, running workouts, cycling workouts, etc.

    CLActivityTypeOtherNavigation, 		// positioning for transportation that does not or may not
										// adhere to roads such as cycling, scooters, trains, boats
										// and off-road vehicles; also for positioning indoors and
										// outdoors that isn’t tied to a dedicated fitness session,
										// e.g. walking

    CLActivityTypeAirborne API_AVAILABLE(ios(12.0), macos(10.14), tvos(12.0), watchos(5.0)),
										// positioning for activities in the air, e.g. flying in an
										// airplane or helicopter, paragliding, flying on a drone,
										// skydiving, etc.  Also includes runway taxiing
};

@class CLLocation;
@class CLHeading;
@class CLBeaconRegion;
@class CLBeaconIdentityConstraint;
@protocol CLLocationManagerDelegate;

/*
 *  CLLocationManager
 *  
 *  Discussion:
 *    The CLLocationManager object is your entry point to the location service.
 */
CL_EXTERN
API_AVAILABLE(macos(10.6), ios(2.0))
@interface CLLocationManager : NSObject
{
@private
	id _internal;
}

/*
 *  locationServicesEnabled
 *
 *  Discussion:
 *      Determines whether the user has location services enabled.
 *      If NO, and you proceed to call other CoreLocation API, user will be prompted with the warning
 *      dialog. You may want to check this property and use location services only when explicitly requested by the user.
 */
+ (BOOL)locationServicesEnabled API_AVAILABLE(ios(4.0), macos(10.7));

/*
 *  headingAvailable
 *
 *  Discussion:
 *      Returns YES if the device supports the heading service, otherwise NO.
 */
+ (BOOL)headingAvailable API_AVAILABLE(ios(4.0), macos(10.7), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  significantLocationChangeMonitoringAvailable
 *  
 *  Discussion:
 *      Returns YES if the device supports significant location change monitoring, otherwise NO.
 */
+ (BOOL)significantLocationChangeMonitoringAvailable API_AVAILABLE(ios(4.0), macos(10.7)) API_UNAVAILABLE(watchos, tvos);

/*
 *  isMonitoringAvailableForClass:
 *
 *  Discussion:
 *      Determines whether the device supports monitoring for the specified type of region.
 *      If NO, all attempts to monitor the specified type of region will fail.
 */
+ (BOOL)isMonitoringAvailableForClass:(Class)regionClass API_AVAILABLE(ios(7.0), macos(10.10)) API_UNAVAILABLE(watchos, tvos);

/*
 *  regionMonitoringAvailable
 *  
 *  Discussion:
 *      Deprecated. Use +isMonitoringAvailableForClass: instead.
 */
+ (BOOL)regionMonitoringAvailable API_DEPRECATED_WITH_REPLACEMENT("+isMonitoringAvailableForClass:", ios(4.0, 7.0), macos(10.8, 10.10)) API_UNAVAILABLE(watchos, tvos);

/*
 *  regionMonitoringEnabled
 *  
 *  Discussion:
 *      Deprecated. Use +isMonitoringAvailableForClass: and -authorizationStatus instead.
 */
+ (BOOL)regionMonitoringEnabled API_DEPRECATED("Use +isMonitoringAvailableForClass: and -authorizationStatus instead", ios(4.0, 6.0), macos(10.8, 10.10)) API_UNAVAILABLE(watchos, tvos);

/*
 *  isRangingAvailable
 *
 *  Discussion:
 *      Determines whether the device supports ranging.
 *      If NO, all attempts to range beacons will fail.
 */
+ (BOOL)isRangingAvailable API_AVAILABLE(ios(7.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  authorizationStatus
 *
 *  Discussion:
 *      Returns the current authorization status of the calling application.
 */
@property (nonatomic, readonly) CLAuthorizationStatus authorizationStatus API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));
/*
 *  authorizationStatus
 *
 *  Discussion:
 *      Deprecated.  Use the instance property authorizationStatus instead.
 */
+ (CLAuthorizationStatus)authorizationStatus API_DEPRECATED_WITH_REPLACEMENT("-authorizationStatus", ios(4.2, 14.0), macos(10.7, 11.0), watchos(1.0, 7.0), tvos(9.0, 14.0));

/*
 *  accuracyAuthorization
 *
 *  Discussion:
 *      Returns information about the calling application's access to accurate location.  See the
 *      documentation for CLAccuracyAuthorization for information about how to interpret the return value.
 *
 *      Note that this property should generally be interpreted in the context of the authorizationStatus
 *      property.  For example, if authorizationStatus == kCLAuthorizationStatusNotDetermined then accurate
 *      location information cannot be received even when accuracyAuthorization is
 *      CLAccuracyAuthorizationFullAccuracy.
 */
@property (nonatomic, readonly) CLAccuracyAuthorization accuracyAuthorization API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));

/*
 *  authorizedForWidgetUpdates
 *
 *  Discussion:
 *      Returns true if widgets of the calling application may be eligible to receive location updates.
 *
 *      If the calling application has authorizationStatus == kCLAuthorizationStatusAuthorizedWhenInUse,
 *      and  authorizedForWidgetUpdates == YES, then widgets will be able to get location updates if called upon
 *      to refresh within a few minutes of having been seen.
 *
 *      If the calling application has authorizationStatus == kCLAuthorizationStatusAuthorizedAlways,
 *      then authorizedForWidgetUpdates will always be YES.
 */
@property (nonatomic, readonly, getter=isAuthorizedForWidgetUpdates) BOOL authorizedForWidgetUpdates API_AVAILABLE(ios(14.0), macos(11.0)) API_UNAVAILABLE(watchos, tvos);

@property(weak, nonatomic, nullable) id<CLLocationManagerDelegate> delegate;

/*
 *  locationServicesEnabled
 *  
 *  Discussion:
 *      Deprecated. Use +locationServicesEnabled instead.
 */
@property(readonly, nonatomic) BOOL locationServicesEnabled API_DEPRECATED_WITH_REPLACEMENT("+locationServicesEnabled", ios(2.0, 4.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  purpose
 *  
 *  Discussion:
 *      Allows the application to specify what location will be used for in their app. This
 *      will be displayed along with the standard Location permissions dialogs. This property will need to be
 *      set prior to calling startUpdatingLocation.
 *
 *      Deprecated.  Set the purpose string in Info.plist using key NSLocationUsageDescription.
 */
@property(copy, nonatomic, nullable) NSString *purpose API_DEPRECATED("Set the purpose string in Info.plist using key NSLocationUsageDescription", ios(3.2, 6.0), macos(10.7, 11.0)) API_UNAVAILABLE(watchos, tvos);

/*
 *	activityType
 *
 *  Discussion:
 *		Specifies the type of user activity. Currently affects behavior such as
 *		the determination of when location updates may be automatically paused.
 *		By default, CLActivityTypeOther is used.
 */
@property(assign, nonatomic) CLActivityType activityType API_AVAILABLE(ios(6.0), macos(10.15), watchos(4.0)) API_UNAVAILABLE(tvos);

/*
 *  distanceFilter
 *  
 *  Discussion:
 *      Specifies the minimum update distance in meters. Client will not be notified of movements of less 
 *      than the stated value, unless the accuracy has improved. Pass in kCLDistanceFilterNone to be 
 *      notified of all movements. By default, kCLDistanceFilterNone is used.
 */
@property(assign, nonatomic) CLLocationDistance distanceFilter;

/*
 *  desiredAccuracy
 *  
 *  Discussion:
 *      The desired location accuracy. The location service will try its best to achieve
 *      your desired accuracy. However, it is not guaranteed. To optimize
 *      power performance, be sure to specify an appropriate accuracy for your usage scenario (eg,
 *      use a large accuracy value when only a coarse location is needed). Use kCLLocationAccuracyBest to
 *      achieve the best possible accuracy. Use kCLLocationAccuracyBestForNavigation for navigation.
 *      The default value varies by platform.
 */
@property(assign, nonatomic) CLLocationAccuracy desiredAccuracy;

/*
 *	pausesLocationUpdatesAutomatically
 *
 *  Discussion:
 *		Specifies that location updates may automatically be paused when possible.
 *		By default, this is YES for applications linked against iOS 6.0 or later.
 */
@property(assign, nonatomic) BOOL pausesLocationUpdatesAutomatically API_AVAILABLE(ios(6.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  allowsBackgroundLocationUpdates
 *
 *  Discussion:
 *      By default, this is NO for applications linked against iOS 9.0 or later,
 *      regardless of minimum deployment target.
 *
 *      With UIBackgroundModes set to include "location" in Info.plist, you must
 *      also set this property to YES at runtime whenever calling
 *      -startUpdatingLocation with the intent to continue in the background.
 *
 *      Setting this property to YES when UIBackgroundModes does not include
 *      "location" is a fatal error.
 *
 *      Resetting this property to NO is equivalent to omitting "location" from
 *      the UIBackgroundModes value.  Access to location is still permitted
 *      whenever the application is running (ie not suspended), and has
 *      sufficient authorization (ie it has WhenInUse authorization and is in
 *      use, or it has Always authorization).  However, the app will still be
 *      subject to the usual task suspension rules.
 *
 *      See -requestWhenInUseAuthorization and -requestAlwaysAuthorization for
 *      more details on possible authorization values.
 */
@property(assign, nonatomic) BOOL allowsBackgroundLocationUpdates API_AVAILABLE(ios(9.0), macos(10.15), watchos(4.0)) API_UNAVAILABLE(tvos);

/*
 *  showsBackgroundLocationIndicator
 *
 *  Discussion:
 *      Specifies that an indicator be shown when the app makes use of continuous
 *      background location updates.  Starting continuous background location
 *      updates requires the app to set UIBackgroundModes to include "location"
 *      and to set the property allowsBackgroundLocationUpdates to YES before
 *      calling -startUpdatingLocation with the intent to continue in the
 *      background.
 *
 *      Note that this property only applies to apps with Always authorization.
 *      For apps with WhenInUse authorization, the indicator is always shown when
 *      using continuous background location updates in order to maintain user
 *      visibility and that the app is still in use.
 *
 *      The default value of this property is NO.
 */
@property(assign, nonatomic) BOOL showsBackgroundLocationIndicator API_AVAILABLE(ios(11.0)) API_UNAVAILABLE(watchos, tvos, macos);

/*
 *  location
 *  
 *  Discussion:
 *      The last location received. Will be nil until a location has been received.
 */
@property(readonly, nonatomic, copy, nullable) CLLocation *location;

/*
 *  headingAvailable
 *
 *  Discussion:
 *      Deprecated. Use +headingAvailable instead.
 */
@property(readonly, nonatomic) BOOL headingAvailable API_DEPRECATED_WITH_REPLACEMENT("+headingAvailable", ios(3.0, 4.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  headingFilter
 *  
 *  Discussion:
 *      Specifies the minimum amount of change in degrees needed for a heading service update. Client will not
 *      be notified of updates less than the stated filter value. Pass in kCLHeadingFilterNone to be
 *      notified of all updates. By default, 1 degree is used.
 */
@property(assign, nonatomic) CLLocationDegrees headingFilter API_AVAILABLE(ios(3.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  headingOrientation
 *  
 *  Discussion:
 *      Specifies a physical device orientation from which heading calculation should be referenced. By default,
 *      CLDeviceOrientationPortrait is used. CLDeviceOrientationUnknown, CLDeviceOrientationFaceUp, and
 *      CLDeviceOrientationFaceDown are ignored.
 *      
 */
@property(assign, nonatomic) CLDeviceOrientation headingOrientation API_AVAILABLE(ios(4.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  heading
 *  
 *  Discussion:
 *      Returns the latest heading update received, or nil if none is available.
 */
@property(readonly, nonatomic, copy, nullable) CLHeading *heading API_AVAILABLE(ios(4.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  maximumRegionMonitoringDistance
 *  
 *  Discussion:
 *       the maximum region size, in terms of a distance from a central point, that the framework can support.
 *       Attempts to register a region larger than this will generate a kCLErrorRegionMonitoringFailure.
 *       This value may vary based on the hardware features of the device, as well as on dynamically changing resource constraints.
 */
@property (readonly, nonatomic) CLLocationDistance maximumRegionMonitoringDistance API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  monitoredRegions
 *  
 *  Discussion:
 *       Retrieve a set of objects for the regions that are currently being monitored.  If any location manager
 *       has been instructed to monitor a region, during this or previous launches of your application, it will
 *       be present in this set.
 */
@property (readonly, nonatomic, copy) NSSet<__kindof CLRegion *> *monitoredRegions API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  rangedRegions
 *
 *  Discussion:
 *       Retrieve a set of objects representing the regions for which this location manager is actively providing ranging.
 */
@property (readonly, nonatomic, copy) NSSet<__kindof CLRegion *> *rangedRegions API_DEPRECATED("Use -rangedBeaconConstraints", ios(7.0, 13.0), macos(10.15, 10.15), macCatalyst(14.0, 14.0)) API_UNAVAILABLE(watchos, tvos);

/*
 *  rangedBeaconConstraints
 *
 *  Discussion:
 *      Retrieve a set of beacon constraints for which this location manager is actively providing ranging.
 */
@property (readonly, nonatomic, copy) NSSet<CLBeaconIdentityConstraint *> *rangedBeaconConstraints API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  requestWhenInUseAuthorization
 *
 *  Discussion:
 *      When -authorizationStatus == kCLAuthorizationStatusNotDetermined,
 *      calling this method will trigger a prompt to request "when-in-use"
 *      authorization from the user.  Any authorization change as a result of
 *      the prompt will be reflected via the usual delegate callback:
 *      -locationManager:didChangeAuthorizationStatus:.
 *
 *      If possible, perform this call in response to direct user request for a
 *      location-based service so that the reason for the prompt will be clear,
 *      and the utility of a one-time grant is maximized.
 *
 *      If received, "when-in-use" authorization grants access to the user's
 *      location via any CoreLocation API as long as your app is being actively
 *      used by the user.  Typically this means your app must be in the
 *      foreground.  If you start a Continuous Background Location session (see
 *      -allowsBackgroundLocationUpdates), then CoreLocation will maintain
 *      visibility for your app as it enters the background.  This will enable
 *      your app to continue receiving location information even as another app
 *      enters the foreground.  Your app will remain visible in this way until
 *      location updates are stopped or your app is killed by the user.
 *
 *      When -authorizationStatus != kCLAuthorizationStatusNotDetermined, (ie
 *      generally after the first call) this method will do nothing.
 *
 *      If your app is not currently in use, this method will do nothing.
 *
 *      The NSLocationWhenInUseUsageDescription key must be specified in your
 *      Info.plist; otherwise, this method will do nothing, as your app will be
 *      assumed not to support WhenInUse authorization.
 */
- (void)requestWhenInUseAuthorization API_AVAILABLE(ios(8.0), macos(10.15));

/*
 *  requestAlwaysAuthorization
 *
 *  Discussion:
 *      When -authorizationStatus == kCLAuthorizationStatusNotDetermined,
 *      calling this method will start the process of requesting "always"
 *      authorization from the user.  Any authorization change as a result of
 *      the prompt will be reflected via the usual delegate callback:
 *      -locationManager:didChangeAuthorizationStatus:.
 *
 *      If possible, perform this call in response to direct user request for a
 *      location-based service so that the reason for the prompt will be clear,
 *      and the utility of a one-time grant is maximized.
 *
 *      If received, "always" authorization grants access to the user's location
 *      via any CLLocationManager API.  In addition, monitoring APIs may launch
 *      your app into the background when they detect an event.  Even if killed by
 *      the user, launch events triggered by monitoring APIs will cause a
 *      relaunch.
 *
 *      "Always" authorization presents a significant risk to user privacy, and
 *      as such requesting it is discouraged unless background launch behavior
 *      is genuinely required.  Do not call +requestAlwaysAuthorization unless
 *      you think users will thank you for doing so.
 *
 *      An application which currently has "when-in-use" authorization and has
 *      never before requested "always" authorization may use this method to
 *      request "always" authorization one time only.  Otherwise, if
 *      -authorizationStatus != kCLAuthorizationStatusNotDetermined, (ie
 *      generally after the first call) this method will do nothing.
 *
 *      If your app is not currently in use, this method will do nothing.
 *
 *      Both the NSLocationAlwaysAndWhenInUseUsageDescription and
 *      NSLocationWhenInUseUsageDescription keys must be specified in your
 *      Info.plist; otherwise, this method will do nothing, as your app will be
 *      assumed not to support Always authorization.
 */
- (void)requestAlwaysAuthorization API_AVAILABLE(ios(8.0), macos(10.15)) API_UNAVAILABLE(tvos);

/*
 *  requestTemporaryFullAccuracyAuthorizationWithPurposeKey:completion:
 *
 *  Discussion:
 *      If your app doesn't have permission to access accurate location (see
 *      -accuracyAuthorization), you can use this method to request temporary access
 *      to accurate location.  This access will expire automatically, but it won't expire
 *      while the user is still engaged with your app.  So, for example, while your app
 *      is in the foreground your app will retain the temporary access it was granted.
 *      Similarly, if your app starts a Continuous Background Location session with the
 *      background location indicator enabled (see -showsBackgroundLocationIndicator),
 *      your access to accurate location will remain as long as the background location
 *      indicator remains enabled.  This allows your app to provide session-oriented
 *      experiences which require accurate location (e.g. fitness or navigation), even if
 *      the user has decided not to grant your app persistent access to accurate location.
 *
 *      When CoreLocation prepares the prompt for display, it will look at the
 *      NSLocationTemporaryUsageDescriptionDictionary key in your Info.plist.  The value
 *      should be a dictionary containing usage descriptions.  The purposeKey you provide
 *      to this method must correspond to an entry in that dictionary.  To retrieve a
 *      localized string, CoreLocation will load your InfoPlist.strings file and try to
 *      look up a string using the purposeKey you provided.  If that fails, CoreLocation
 *      will use the content provided in your Info.plist.  For example, suppose your
 *      Info.plist contains the following.
 *
 *          <key>NSLocationTemporaryUsageDescriptionDictionary</key>
 *          <dict>
 *            <key>ExampleUsageDescription</key>
 *            <string>This app needs accurate location so it can verify that you're in a supported region.</string>
 *            <key>AnotherUsageDescription</key>
 *            <string>This app needs accurate location so it can show you relevant results.</string>
 *          </dict>
 *
 *      In this case, you would be able to provide either "ExampleUsageDescription" or
 *      "AnotherUsageDescription" for the purposeKey argument.  Passing any other string
 *      would result in the prompt not being displayed.  To localize these usage
 *      descriptions, you would add entries for "ExampleUsageDescription" and
 *      "AnotherUsageDescription" to your InfoPlist.strings file.
 *
 *      CoreLocation may decide against showing the user a prompt.  For example, if your
 *      app already has access to accurate location or it is currently in the background.
 *      When that happens, your completion block will be called with an appropriate
 *      NSError.  If the user responded to the prompt, your completion block will be
 *      called with a nil error.  At that point, you may query the state of your
 *      CLLocationManager to see if you have been granted accurate access.
 *
 *      The completion block will be called in the same threading context where delegate
 *      callbacks occur.  That is, it will be called on the runloop where the
 *      CLLocationManager was originally initialized.
 */
- (void)requestTemporaryFullAccuracyAuthorizationWithPurposeKey:(NSString *)purposeKey completion:(void(^ _Nullable)(NSError * _Nullable))completion API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));

/*
 *  requestTemporaryFullAccuracyAuthorizationWithPurposeKey:
 *
 *  Discussion:
 *      This is a variant of requestTemporaryAccurateLocationAuthorizationWithPurposeKey:completion:
 *      which doesn't take a completion block.  This is equivalent to passing in a nil
 *      completion block.
 */
- (void)requestTemporaryFullAccuracyAuthorizationWithPurposeKey:(NSString *)purposeKey API_AVAILABLE(ios(14.0), macos(11.0), watchos(7.0), tvos(14.0));

/*
 *  startUpdatingLocation
 *  
 *  Discussion:
 *      Start updating locations.
 */
- (void)startUpdatingLocation API_AVAILABLE(watchos(3.0)) API_UNAVAILABLE(tvos);

/*
 *  stopUpdatingLocation
 *  
 *  Discussion:
 *      Stop updating locations.
 */
- (void)stopUpdatingLocation;

/*
 *  requestLocation
 *
 *  Discussion:
 *      Request a single location update.
 *
 *      The service will attempt to determine location with accuracy according
 *      to the desiredAccuracy property.  The location update will be delivered
 *      via the standard delegate callback, i.e. locationManager:didUpdateLocations:
 *
 *      If the best available location has lower accuracy, then it will be
 *      delivered via the standard delegate callback after timeout.
 *
 *      If no location can be determined, the locationManager:didFailWithError:
 *      delegate callback will be delivered with error location unknown.
 *
 *      There can only be one outstanding location request and this method can
 *      not be used concurrently with startUpdatingLocation or
 *      allowDeferredLocationUpdates.  Calling either of those methods will
 *      immediately cancel the location request.  The method
 *      stopUpdatingLocation can be used to explicitly cancel the request.
 */
- (void)requestLocation API_AVAILABLE(ios(9.0), macos(10.14));

/*
 *  startUpdatingHeading
 *
 *  Discussion:
 *      Start updating heading.
 */
- (void)startUpdatingHeading API_AVAILABLE(ios(3.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  stopUpdatingHeading
 *
 *  Discussion:
 *      Stop updating heading.
 */
- (void)stopUpdatingHeading API_AVAILABLE(ios(3.0), watchos(2.0)) API_UNAVAILABLE(tvos, macos);

/*
 *  dismissHeadingCalibrationDisplay
 *  
 *  Discussion:
 *      Dismiss the heading calibration immediately.
 */
- (void)dismissHeadingCalibrationDisplay API_AVAILABLE(ios(3.0), macos(10.15), watchos(2.0)) API_UNAVAILABLE(tvos);

/*
 *  startMonitoringSignificantLocationChanges
 *
 *  Discussion:
 *      Start monitoring significant location changes.  The behavior of this service is not affected by the desiredAccuracy
 *      or distanceFilter properties.  Locations will be delivered through the same delegate callback as the standard
 *      location service.
 *
 */
- (void)startMonitoringSignificantLocationChanges API_AVAILABLE(ios(4.0), macos(10.7)) API_UNAVAILABLE(watchos, tvos);

/*
 *  stopMonitoringSignificantLocationChanges
 *
 *  Discussion:
 *      Stop monitoring significant location changes.
 *
 */
- (void)stopMonitoringSignificantLocationChanges API_AVAILABLE(ios(4.0), macos(10.7)) API_UNAVAILABLE(watchos, tvos);

/*
 *  startMonitoringLocationPushes
 *
 *  Discussion:
 *   	Request an Apple Push Notification service token to be used to send location pushes. Incoming location pushes launch the app's Location Push Service Extension. Requires the com.apple.developer.location.push entitlement.
 *
 */
- (void)startMonitoringLocationPushesWithCompletion:(void(^ _Nullable)(NSData * _Nullable token, NSError * _Nullable))completion API_AVAILABLE(ios(15.0)) API_UNAVAILABLE(macos, macCatalyst, tvos, watchos);

/*
 *  stopMonitoringLocationPushes
 *
 *  Discussion:
 *		Stop monitoring for location pushes.
 *
 */
- (void)stopMonitoringLocationPushes API_AVAILABLE(ios(15.0)) API_UNAVAILABLE(macos, macCatalyst, tvos, watchos);

/*
 *  startMonitoringForRegion:desiredAccuracy:
 *
 *  Discussion:
 *      Start monitoring the specified region.  desiredAccuracy represents the distance past the border of the region at
 *      which the application would like to be notified that the region border has been crossed.  This is useful to prevent
 *      repeated notifications when the user is on the border of the region.  This value will be honored on a best-effort basis,
 *      and may not be respected if desiredAccuracy is large with respect to the size of the region, or if the device is not
 *      capable of providing the precision desired.
 *
 *      If a region of the same type with the same identifier is already being monitored for this application, it will be
 *      removed from monitoring.
 *
 *      This is done asynchronously and may not be immediately reflected in monitoredRegions.
 */
- (void)startMonitoringForRegion:(CLRegion *)region
                 desiredAccuracy:(CLLocationAccuracy)accuracy API_DEPRECATED_WITH_REPLACEMENT("-startMonitoringForRegion:", ios(4.0, 6.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  stopMonitoringForRegion:
 *
 *  Discussion:
 *      Stop monitoring the specified region.  It is valid to call stopMonitoringForRegion: for a region that was registered
 *      for monitoring with a different location manager object, during this or previous launches of your application.
 *
 *      This is done asynchronously and may not be immediately reflected in monitoredRegions.
 */
- (void)stopMonitoringForRegion:(CLRegion *)region API_AVAILABLE(ios(4.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  startMonitoringForRegion:
 *
 *  Discussion:
 *      Start monitoring the specified region.
 *
 *      If a region of the same type with the same identifier is already being monitored for this application,
 *      it will be removed from monitoring. For circular regions, the region monitoring service will prioritize
 *      regions by their size, favoring smaller regions over larger regions.
 *
 *      This is done asynchronously and may not be immediately reflected in monitoredRegions.
 */
- (void)startMonitoringForRegion:(CLRegion *)region API_AVAILABLE(ios(5.0), macos(10.8)) API_UNAVAILABLE(watchos, tvos);

/*
 *  requestStateForRegion:
 *
 *  Discussion:
 *      Asynchronously retrieve the cached state of the specified region. The state is returned to the delegate via
 *      locationManager:didDetermineState:forRegion:.
 */
- (void)requestStateForRegion:(CLRegion *)region API_AVAILABLE(ios(7.0), macos(10.10)) API_UNAVAILABLE(watchos, tvos);

/*
 *  startRangingBeaconsInRegion:
 *
 *  Discussion:
 *      Start calculating ranges for beacons in the specified region.
 */
- (void)startRangingBeaconsInRegion:(CLBeaconRegion *)region API_DEPRECATED("Use -startRangingBeaconsSatisfyingConstraint:", ios(7.0, 13.0), macos(11.0, 11.0), macCatalyst(14.0, 14.0)) API_UNAVAILABLE(watchos, tvos);

/*
 *  stopRangingBeaconsInRegion:
 *
 *  Discussion:
 *      Stop calculating ranges for the specified region.
 */
- (void)stopRangingBeaconsInRegion:(CLBeaconRegion *)region API_DEPRECATED("Use -stopRangingBeaconsSatisfyingConstraint:", ios(7.0, 13.0), macos(11.0, 11.0), macCatalyst(14.0, 14.0)) API_UNAVAILABLE(watchos, tvos);

/*
 *  startRangingBeaconsSatisfyingConstraint:
 *
 *  Discussion:
 *      Start producing ranging measurements for beacons that satisfy
 *      the provided constraint.  Ranging will continue until you pass
 *      an equivalent constraint to stopRangingBeaconsSatisfyingConstraint:.
 */
- (void)startRangingBeaconsSatisfyingConstraint:(CLBeaconIdentityConstraint *)constraint API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  stopRangingBeaconsSatisfyingConstraint:
 *
 *  Discussion:
 *      Stop an earlier beacon ranging request.  See startRangingBeaconsSatisfyingConstraint:.
 */
- (void)stopRangingBeaconsSatisfyingConstraint:(CLBeaconIdentityConstraint *)constraint API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *	allowDeferredLocationUpdatesUntilTraveled:timeout:
 *
 *	Discussion:
 *		Indicate that the application will allow the location manager to defer
 *		location updates until an exit criterion is met. This may allow the
 *		device to enter a low-power state in which updates are held for later
 *		delivery. Once an exit condition is met, the location manager will
 *		continue normal updates until this method is invoked again.
 *
 *		Exit conditions, distance and timeout, can be specified using the constants
 *		CLLocationDistanceMax and CLTimeIntervalMax, respectively, if you are
 *		trying to achieve an unlimited distance or timeout.
 *
 *		The CLLocationManagerDelegate will continue to receive normal updates as
 *		long as the application remains in the foreground. While the process is
 *		in the background, the device may be able to enter a low-power state for
 *		portions of the specified distance and time interval. While in this
 *		state, locations will be coalesced for later delivery.
 *
 *		Location updates will be deferred as much as is reasonable to save
 *		power. If another process is using location, the device may not enter a
 *		low-power state and instead updates will continue normally. Deferred
 *		updates may be interspersed with normal updates if the device exits and
 *		re-enters a low-power state.
 *
 *		All location updates, including deferred updates, will be delivered via
 *		the delegate callback locationManager:didUpdateLocations:
 *
 *		When deferred updates have ended, the manager will invoke the delegate
 *		callback locationManagerDidFinishDeferredUpdates:withError:. An error
 *		will be returned if the manager will not defer updates and the exit
 *		criteria have not been met.
 */
- (void)allowDeferredLocationUpdatesUntilTraveled:(CLLocationDistance)distance
                                          timeout:(NSTimeInterval)timeout API_DEPRECATED("You can remove calls to this method", ios(6.0, 13.0), macos(10.15, 10.15))  API_UNAVAILABLE(watchos, tvos);

/*
 *	disallowDeferredLocationUpdates
 *
 *	Discussion:
 *		Disallow deferred location updates if previously enabled. Any outstanding
 *		updates will be sent and regular location updates will resume.
 */
- (void)disallowDeferredLocationUpdates API_DEPRECATED("You can remove calls to this method", ios(6.0, 13.0), macos(10.15, 10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  deferredLocationUpdatesAvailable
 *
 *  Discussion:
 *      Returns YES if the device supports deferred location updates, otherwise NO.
 */
+ (BOOL)deferredLocationUpdatesAvailable API_DEPRECATED("You can remove calls to this method", ios(6.0, 13.0), macos(10.9, 10.15)) API_UNAVAILABLE(watchos, tvos);

@end

NS_ASSUME_NONNULL_END
