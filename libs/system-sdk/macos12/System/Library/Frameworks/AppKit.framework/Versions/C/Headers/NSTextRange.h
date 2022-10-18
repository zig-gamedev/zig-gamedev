#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextRange.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextRange.h
//  Text Kit
//
//  Copyright (c) 2018-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark NSTextLocation
// NSTextLocation represents an abstract location inside document contents. A concrete class conforming to the protocol should be associated with a particular document backing store type. The interpretation of a location in enumeration operations is depending on the logical direction. When enumerating forward, it should start with the item containing the location. The enumeration should start with an item preceding the location for reverse operations.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos) NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSTextLocation <NSObject>

// Compares and returns the logical ordering to location
- (NSComparisonResult)compare:(id <NSTextLocation>)location API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);
@end

#pragma mark NSTextRange
// NSTextRange represents a contiguous range between two locations inside document contents. It consists of the starting location and terminating location. They are the two basic properties, location and endLocation, respectively. The terminating location, endLocation, is directly following the last location in the range. So, a location is contained in a range if (range.location <= location) && (location < range.endLocation).
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextRange : NSObject
#pragma mark Initialization
// Returns an empty range when endLocation=nil
- (nullable instancetype)initWithLocation:(id <NSTextLocation>)location endLocation:(nullable id <NSTextLocation>)endLocation NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithLocation:(id <NSTextLocation>)location;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

#pragma mark Basic properties
// An empty range when location == endLocation
@property (readonly, getter=isEmpty) BOOL empty;

// Represented with the inclusive location and the exclusive endLocation
@property (strong, readonly) id <NSTextLocation> location;
@property (strong, readonly) id <NSTextLocation> endLocation;

- (BOOL)isEqualToTextRange:(NSTextRange *)textRange;

#pragma mark Basic operations
- (BOOL)containsLocation:(id <NSTextLocation>)location;
- (BOOL)containsRange:(NSTextRange *)textRange;
- (BOOL)intersectsWithTextRange:(NSTextRange *)textRange;

#pragma mark Basic mutations
// Returns the intersection with textRange. Returns nil when not intersecting.
- (nullable instancetype)textRangeByIntersectingWithTextRange:(NSTextRange *)textRange;
- (instancetype)textRangeByFormingUnionWithTextRange:(NSTextRange *)textRange;
@end


NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextRange.h>
#endif
