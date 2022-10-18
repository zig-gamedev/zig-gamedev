/*
        NSMovie.h
        Application Kit
        Copyright (c) 2000-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class QTMovie;

API_DEPRECATED("", macos(10.0,10.5))
@interface NSMovie : NSObject <NSCoding>
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)init NS_DESIGNATED_INITIALIZER API_DEPRECATED("As of macOS 10.15 this method always returns nil.", macos(10.0,10.5));
- (nullable instancetype)initWithMovie:(QTMovie *)movie NS_DESIGNATED_INITIALIZER API_DEPRECATED("As of macOS 10.15 this method always returns nil.", macos(10.0,10.5));
- (nullable QTMovie *)QTMovie API_DEPRECATED("As of macOS 10.15 this method always returns nil.", macos(10.0,10.5));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
