//
//  CKAsset.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKAsset : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! Initialize an asset to be saved with the content at the given file URL */
- (instancetype)initWithFileURL:(NSURL *)fileURL;

/*! Local file URL where fetched records are cached and saved records originate from. */
@property (nonatomic, readonly, copy, nullable) NSURL *fileURL;

@end

NS_ASSUME_NONNULL_END
