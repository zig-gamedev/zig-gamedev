/* CoreImage - CIFilterConstructor.h
 
 Copyright (c) 2014 Apple, Inc.
 All rights reserved. */


@class NSString;
@class CIFilter;

// Used by +[CIFilter registerFilterName:constructor:classAttributes:]

NS_ASSUME_NONNULL_BEGIN

@protocol CIFilterConstructor
- (nullable CIFilter *)filterWithName:(NSString *)name;
@end

NS_ASSUME_NONNULL_END
