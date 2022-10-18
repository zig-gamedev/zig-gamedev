/*	NSGarbageCollector.h
	Copyright (c) 2006-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

API_DEPRECATED("Building Garbage Collected apps is no longer supported.", macos(10.5, 10.10)) API_UNAVAILABLE(ios, watchos, tvos)
NS_AUTOMATED_REFCOUNT_UNAVAILABLE

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_UNAVAILABLE("Garbage Collection is not supported")
@interface NSGarbageCollector : NSObject

+ (id)defaultCollector;

- (BOOL)isCollecting API_DEPRECATED("", macos(10.0, 10.6)) API_UNAVAILABLE(ios, watchos, tvos);

- (void)disable;
- (void)enable;
- (BOOL)isEnabled;

- (void)collectIfNeeded;
- (void)collectExhaustively;

- (void)disableCollectorForPointer:(const void *)ptr;
- (void)enableCollectorForPointer:(const void *)ptr;

- (NSZone *)zone;

@end

NS_ASSUME_NONNULL_END
