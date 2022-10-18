/*	NSURLHandle.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/
 
#import <Foundation/NSObject.h>

/* NSURLHandle has been deprecated; please use NSURLConnection instead. */

@class NSURLHandle, NSMutableArray, NSMutableData, NSData, NSURL;

	// HTTP Specific Property Keys
FOUNDATION_EXPORT NSString *NSHTTPPropertyStatusCodeKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *NSHTTPPropertyStatusReasonKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *NSHTTPPropertyServerHTTPVersionKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *NSHTTPPropertyRedirectionHeadersKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *NSHTTPPropertyErrorPageDataKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *NSHTTPPropertyHTTPProxy API_DEPRECATED("", macos(10.2, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // NSDictionary containing proxy information to use in place of proxy identified in SystemConfiguration.framework
													// To avoid any proxy use, pass an empty dictionary

// FTP Specific Property Keys
	// All keys are optional.  The default configuration allows an
	// anonymous, passive-mode, one-off transfer of the specified URL.
FOUNDATION_EXPORT NSString *NSFTPPropertyUserLoginKey API_DEPRECATED("", macos(10.2, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // NSString - default "anonymous"
FOUNDATION_EXPORT NSString *NSFTPPropertyUserPasswordKey API_DEPRECATED("", macos(10.2, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // NSString - default "NSURLHandle@apple.com"
FOUNDATION_EXPORT NSString *NSFTPPropertyActiveTransferModeKey API_DEPRECATED("", macos(10.2, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber BOOL - default NO (i.e. passive mode)
FOUNDATION_EXPORT NSString *NSFTPPropertyFileOffsetKey API_DEPRECATED("", macos(10.2, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber - default 0

// NSDictionary containing proxy information to use in place of proxy identified in SystemConfiguration.framework
// To avoid any proxy use, pass an empty dictionary
FOUNDATION_EXPORT NSString *NSFTPPropertyFTPProxy API_DEPRECATED("", macos(10.3, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);



typedef NS_ENUM(NSUInteger, NSURLHandleStatus) {
    NSURLHandleNotLoaded = 0,
    NSURLHandleLoadSucceeded,
    NSURLHandleLoadInProgress,
    NSURLHandleLoadFailed
};

API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos)
@protocol NSURLHandleClient
- (void)URLHandle:(NSURLHandle *)sender resourceDataDidBecomeAvailable:(NSData *)newBytes API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)URLHandleResourceDidBeginLoading:(NSURLHandle *)sender API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)URLHandleResourceDidFinishLoading:(NSURLHandle *)sender API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)URLHandleResourceDidCancelLoading:(NSURLHandle *)sender API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)URLHandle:(NSURLHandle *)sender resourceDidFailLoadingWithReason:(NSString *)reason API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
@end

@interface NSURLHandle:NSObject
{
    NSMutableArray *_clients;
    id _data; 
    NSURLHandleStatus _status;
    NSInteger _reserved;
}

+ (void)registerURLHandleClass:(Class)anURLHandleSubclass API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // Call this to register a new subclass of NSURLHandle
+ (Class)URLHandleClassForURL:(NSURL *)anURL API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

- (NSURLHandleStatus)status API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (NSString *)failureReason API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // if status is NSURLHandleLoadFailed, then failureReason returns the reason for failure; otherwise, it returns nil

- (void)addClient:(id <NSURLHandleClient>)client API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)removeClient:(id <NSURLHandleClient>)client API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

- (void)loadInBackground API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)cancelLoadInBackground API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

- (NSData *)resourceData API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // Blocks until all data is available
- (NSData *)availableResourceData API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);  // Immediately returns whatever data is available

- (long long) expectedResourceDataSize API_DEPRECATED("", macos(10.3, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);	// Length of all of the resource data (can be queried before all data has arrived; negative if unknown)

- (void)flushCachedData API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

// As a background load progresses, subclasses should call these methods
- (void)backgroundLoadDidFailWithReason:(NSString *)reason API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // Sends the failure message to clients
- (void)didLoadBytes:(NSData *)newBytes loadComplete:(BOOL)yorn API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

// The primitives; these must be overridden by subclasses.
+ (BOOL)canInitWithURL:(NSURL *)anURL API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSURLHandle *)cachedHandleForURL:(NSURL *)anURL API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- initWithURL:(NSURL *)anURL cached:(BOOL)willCache API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);

- (id)propertyForKey:(NSString *)propertyKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);  // Must be overridden by subclasses
- (id)propertyForKeyIfAvailable:(NSString *)propertyKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (BOOL)writeProperty:(id)propertyValue forKey:(NSString *)propertyKey API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);
- (BOOL)writeData:(NSData *)data API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos); // Must be overridden by subclasses; returns success or failure

- (NSData *)loadInForeground API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);   // Called from resourceData, above.
- (void)beginLoadInBackground API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);  // Called from -loadInBackground, above.
- (void)endLoadInBackground API_DEPRECATED("", macos(10.0, 10.4)) API_UNAVAILABLE(ios, watchos, tvos);    // Called from -cancelLoadInBackground, above.

@end

