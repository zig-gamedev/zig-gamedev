/*
 *  CLLocationPushServiceExtension.h
 *  CoreLocation
 *
 *  Copyright © 2021 Apple Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(ios(15.0)) API_UNAVAILABLE(watchos, tvos, macos, macCatalyst)
@protocol CLLocationPushServiceExtension <NSObject>

@required

// Call the completion handler once the location push has been handled.
- (void)didReceiveLocationPushPayload:(NSDictionary <NSString *, id> *)payload completion:(void (^)(void))completion;

@optional

// Called just before this extension is terminated by the system.
- (void)serviceExtensionWillTerminate;

@end

NS_ASSUME_NONNULL_END
