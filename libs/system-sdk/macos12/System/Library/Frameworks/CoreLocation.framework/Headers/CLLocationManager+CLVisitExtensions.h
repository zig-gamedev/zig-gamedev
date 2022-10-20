/*
 *  CLLocationManager+CLVisitExtensions.h
 *  CoreLocation
 *
 *  Copyright (c) 2014 Apple Inc. All rights reserved.
 *
 */

#import <CoreLocation/CLLocationManager.h>

#import <CoreLocation/CLAvailability.h>

NS_ASSUME_NONNULL_BEGIN

@interface CLLocationManager (CLVisitExtensions)

/*
 *  startMonitoringVisits
 *
 *  Discussion:
 *    Begin monitoring for visits.  All CLLLocationManagers allocated by your
 *    application, both current and future, will deliver detected visits to
 *    their delegates.  This will continue until -stopMonitoringVisits is sent
 *    to any such CLLocationManager, even across application relaunch events.
 *
 *    Detected visits are sent to the delegate's -locationManager:didVisit:
 *    method.
 */
- (void)startMonitoringVisits API_AVAILABLE(ios(8.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

/*
 *  stopMonitoringVisits
 *
 *  Discussion:
 *    Stop monitoring for visits.  To resume visit monitoring, send
 *    -startMonitoringVisits.
 *
 *    Note that stopping and starting are asynchronous operations and may not
 *    immediately reflect in delegate callback patterns.
 */
- (void)stopMonitoringVisits API_AVAILABLE(ios(8.0), macos(10.15)) API_UNAVAILABLE(watchos, tvos);

@end

NS_ASSUME_NONNULL_END
