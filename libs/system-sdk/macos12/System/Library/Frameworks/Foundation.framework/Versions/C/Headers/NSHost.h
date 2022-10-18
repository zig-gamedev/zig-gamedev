/*	NSHost.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSArray<ObjectType>, NSMutableArray;

/**
 * DEPRECATION NOTICE
 *
 * If you’re using `NSHost` to resolve DNS names so that you can connect to a
 * service, switch to a connect-by-name API, for example, `nw_connection`.
 *
 * If you have other DNS resolution needs, switch to <dns_sd.h>.
 */

NS_ASSUME_NONNULL_BEGIN

API_DEPRECATED("Use Network framework instead, see deprecation notice in <Foundation/NSHost.h>", macos(10.0, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSHost : NSObject {
@private
    NSArray 	*names;
    NSArray 	*addresses;
    id		reserved;
}

+ (instancetype)currentHost;
+ (instancetype)hostWithName:(nullable NSString *)name;
+ (instancetype)hostWithAddress:(NSString *)address;

- (BOOL)isEqualToHost:(NSHost *)aHost;

@property (nullable, readonly, copy) NSString *name;	// arbitrary choice
@property (readonly, copy) NSArray<NSString *> *names;	// unordered list

@property (nullable, readonly, copy) NSString *address;	// arbitrary choice
@property (readonly, copy) NSArray<NSString *> *addresses;	// unordered list of IPv6 and IPv4 addresses

@property (nullable, readonly, copy) NSString *localizedName API_AVAILABLE(macos(10.6)) API_UNAVAILABLE(ios, watchos, tvos);

// NSHost does not implement any caching any longer
+ (void)setHostCacheEnabled:(BOOL)flag API_DEPRECATED("Caching no longer supported", macos(10.0,10.7)) API_UNAVAILABLE(ios, watchos, tvos);
+ (BOOL)isHostCacheEnabled API_DEPRECATED("Caching no longer supported", macos(10.0,10.7)) API_UNAVAILABLE(ios, watchos, tvos);
+ (void)flushHostCache API_DEPRECATED("Caching no longer supported", macos(10.0,10.7)) API_UNAVAILABLE(ios, watchos, tvos);
@end

NS_ASSUME_NONNULL_END
