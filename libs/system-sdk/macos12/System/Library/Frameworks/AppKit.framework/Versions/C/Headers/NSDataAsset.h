/*
	NSDataAsset.h
	Application Kit
	Copyright (c) 2015-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE

typedef NSString * NSDataAssetName NS_SWIFT_BRIDGED_TYPEDEF API_AVAILABLE(ios(9.0), macos(10.11), tvos(9.0), watchos(2.0));

/* An NSDataAsset instance provides access to a data entry in an asset catalog such as Images.xcassets. Data entries and image entries in asset catalogs do not share the same namespace. For example, if an asset catalog contains an image entry named @"foo" but no data entry named @"foo", [[NSDataAsset alloc] initWithName:@"foo"] will return nil.
*/

API_AVAILABLE(ios(9.0), macos(10.11), tvos(9.0), watchos(2.0)) @interface NSDataAsset : NSObject<NSCopying>

- (instancetype)init NS_UNAVAILABLE;

/// Equivalent to -initWithName:name bundle:[NSBundle mainBundle];
- (nullable instancetype)initWithName:(NSDataAssetName)name;
/// Create a data asset with the given name from the given bundle. Returns nil if the asset was not found.
- (nullable instancetype)initWithName:(NSDataAssetName)name bundle:(NSBundle *)bundle NS_DESIGNATED_INITIALIZER;

/// The name used to reference the data asset
@property (readonly, copy) NSDataAssetName name;
/// The data for this asset, as stored in the asset catalog
@property (readonly, copy) NSData *data;
/// The Uniform Type Identifier for this data object.
@property (readonly, copy) NSString *typeIdentifier;

@end
#endif // !TARGET_OS_IPHONE

NS_ASSUME_NONNULL_END
